import sys
import os
import jinja2

import ParseCAN.ParseCAN as ParseCAN

import constants
import computers_h
import computers_c
# import test_h
# import test_c
import drivers_inc

from jinja2 import nodes
from jinja2.ext import Extension
from jinja2.exceptions import TemplateRuntimeError


src_dir = '../src/'
constants_path = f'{src_dir}constants.h'
drivers_inc_dir_path = f'{src_dir}/drivers/inc'
computer_h_dir_path = f'{src_dir}computers/inc'
computer_c_dir_path = f'{src_dir}computers/src'

template_dir = './templates/'
computer_c_template_path = f'{template_dir}computer.c.j2'
computer_h_template_path = f'{template_dir}computer.h.j2'
constants_template_path = f'{template_dir}constants.h.j2'
drivers_inc_template_dir_path = f'{template_dir}drivers/inc'


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
    template = env.get_template(input_path)
    with open(output_path, 'w') as f:
        f.write(template.render())


def render_template(env, relative_path):
    render_template_from_to(env, template_dir + relative_path + ".j2", src_dir + relative_path)


if __name__ == '__main__':
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

    constants.write(template_env, constants_template_path, constants_path)
    render_template(template_env, "pack_unpack.c")
    render_template(template_env, "pack_unpack.h")
    render_template(template_env, "enum_atom.h")
    render_template(template_env, "send_receive.c")
    render_template(template_env, "structs.h")
    render_template(template_env, "bus.h")
    computers_h.write(template_env, system.computer, computer_h_template_path, computer_h_dir_path)
    computers_c.write(template_env, system.computer, computer_c_template_path, computer_c_dir_path)
    # test_h.write(can)
    # test_c.write(can)

    drivers_inc.write(template_env, system, drivers_inc_template_dir_path, drivers_inc_dir_path)
