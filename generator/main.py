import sys
import os
import jinja2
from jinja2 import nodes
from jinja2.ext import Extension
from jinja2.exceptions import TemplateRuntimeError
from pathlib import Path

import ParseCAN.ParseCAN as ParseCAN

import constants
import computers_hpp
import computers_cpp
import drivers_inc


src_dir = Path('../src/src/')
inc_dir = Path('../src/inc/')
drivers_inc_dir_path = inc_dir.joinpath('drivers/')
computer_hpp_dir_path = inc_dir.joinpath('computers/')
computer_cpp_dir_path = src_dir.joinpath('computers/')

template_dir = Path('./templates/')
computer_cpp_template_path = template_dir.joinpath('computer.cpp.j2')
computer_hpp_template_path = template_dir.joinpath('computer.hpp.j2')
drivers_inc_template_dir_path = template_dir.joinpath('drivers/inc')

from pint import UnitRegistry
def get_ms(period_str):
    if type(period_str) is int:
        # If it's set as an integer, assume ms
        return period_str

    ur = UnitRegistry()
    return (int)(ur[period_str].to('ms').magnitude)

def get_msg_len(msg):
    if hasattr(msg, "frame"):
       return 1 + sum([get_msg_len(sub_frame) for sub_frame in msg.frame])
    return 1

# FROM: https://github.com/duelafn/python-jinja2-apci/blob/master/jinja2_apci/error.py
class RaiseExtension(Extension):
    # This is our keyword(s):
    tags = set(['raise'])

    # See also: jinja2.parser.parse_include()
    def parse(self, parser):
        # the first token is the token that started the tag. In our case we
        # only listen to "raise" so this will be a name token with
        # "raise" as value. We get the line number so that we can give
        # that line number to the nodes we insert.
        lineno = next(parser.stream).lineno

        # Extract the message from the template
        message_node = parser.parse_expression()

        return nodes.CallBlock(
            self.call_method('_raise', [message_node], lineno=lineno),
            [], [], [], lineno=lineno
        )

    def _raise(self, msg, caller):
        raise TemplateRuntimeError(msg)

def render_template_from_to(env, input_path, output_path):
    template = env.get_template(str(input_path))
    with open(output_path, 'w') as f:
        if output_path in [inc_dir.joinpath("structs.hpp"), src_dir.joinpath("structs.cpp")]:
            f.write(template.render(get_ms = get_ms, get_msg_len = get_msg_len))
        else:
            f.write(template.render())

def render_template(env, relative_path):
    if relative_path.endswith('hpp') or relative_path.endswith('h'):
        render_template_from_to(env, template_dir.joinpath(f"{relative_path}.j2"), inc_dir.joinpath(relative_path))
    else:
        render_template_from_to(env, template_dir.joinpath(f"{relative_path}.j2"), src_dir.joinpath(relative_path))


if __name__ == '__main__':
    if '--clean' in sys.argv:
        os.system("rm src/inc/computers/*.hpp")
        os.system("rm src/inc/drivers/*.hpp")
        os.system("rm src/inc/bus.hpp src/inc/pack_unpack.hpp src/inc/structs.hpp")
        os.system("rm src/src/computers/*.cpp")
        os.system("rm src/src/bus.cpp src/src/pack_unpack.cpp src/src/structs.cpp")
    else:
        specpath = sys.argv[1]
        specfile = open(specpath, 'r')
        system = ParseCAN.spec.System.from_yaml(specfile)
        can = system.protocol['name']['can']

        script_dir = os.path.dirname(sys.argv[0])
        if script_dir == '':
            script_dir = '.'
        os.chdir(script_dir)

        template_loader = jinja2.FileSystemLoader(searchpath=".")
        template_env = jinja2.Environment(loader=template_loader, keep_trailing_newline=True, extensions=[RaiseExtension])
        template_env.globals["can"] = can
        template_env.globals["system"] = system

        for filename in ["pack_unpack.cpp", "pack_unpack.hpp", "structs.hpp", "bus.hpp", "bus.cpp", "structs.cpp"]:
            render_template(template_env, filename)

        testing = '--testing' in sys.argv 
        computers_hpp.write(template_env, system.computer, computer_hpp_template_path, computer_hpp_dir_path, testing)
        computers_cpp.write(template_env, system.computer, computer_cpp_template_path, computer_cpp_dir_path, testing)
        drivers_inc.write(template_env, system, drivers_inc_template_dir_path, drivers_inc_dir_path, testing)
        pth = os.path.join(os.path.dirname(os.path.abspath(__file__)), '..')
        try:
            spth = str(pth)
            os.system('clang-format -i ' + spth + '/src/inc/drivers/*.hpp ' + spth + '/src/src/drivers/*.cpp')
            os.system('clang-format -i ' + spth + '/src/inc/computers/*.hpp ' + spth + '/src/src/computers/*.cpp')
            os.system('clang-format -i ' + spth + '/src/inc/*.hpp')
            os.system('clang-format -i ' + spth + '/src/src/*.cpp')
        except:
            print('Error during clang-format, is it installed?')
