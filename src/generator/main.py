import sys
import os
import pkg_resources
import jinja2
from jinja2 import nodes
from jinja2.ext import Extension
from jinja2.exceptions import TemplateRuntimeError
from pathlib import Path

import ParseCAN

import generator.constants as constants
import generator.computers_h as computers_h
import generator.computers_c as computers_c
import generator.drivers_inc as drivers_inc


template_dir = Path(pkg_resources.resource_filename('generator', 'templates'))
computer_c_template_path = template_dir.joinpath('computer.cpp.j2')
computer_h_template_path = template_dir.joinpath('computer.h.j2')
constants_template_path = template_dir.joinpath('constants.h.j2')
drivers_inc_template_dir_path = template_dir.joinpath('drivers/inc')


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
    template = env.get_template(input_path.name)
    with open(output_path, 'w') as f:
        f.write(template.render())


def render_template(env, relative_path, output_dir=None):
    if output_dir is None:
        output_dir = Path(os.getcwd())
    render_template_from_to(env, template_dir.joinpath(f"{relative_path}.j2"), output_dir.joinpath(relative_path))


def main():
    specpath = Path(os.getcwd()).joinpath(sys.argv[1]).resolve()
    specfile = open(specpath, 'r')
    system = ParseCAN.spec.System.from_yaml(specfile)
    can = system.protocol['name']['can']

    output_dir = Path(os.path.join(os.getcwd(), sys.argv[2]))
    output_dir = output_dir.resolve()
    output_dir.mkdir(parents=True, exist_ok=True)
    constants_path = output_dir.joinpath('constants.h')
    drivers_inc_dir_path = output_dir.joinpath('drivers/inc')
    computer_h_dir_path = output_dir.joinpath('computers/inc')
    computer_c_dir_path = output_dir.joinpath('computers/src')

    units_path = None if len(sys.argv) < 4 else sys.argv[3]
    if units_path is not None:
        units_path = os.path.join(units_path, 'au', 'code', 'au', 'units')
   
    specfile = open(specpath, 'r')
    system = ParseCAN.spec.System.from_yaml(specfile)
    can = system.protocol['name']['can']

    all_unit_files = None
    if units_path is not None and os.path.exists(units_path):
        print('units!')
        all_unit_files = ['au.hh'] + [header for header in os.listdir(units_path) if header.endswith('.hh')]

    script_dir = os.path.dirname(sys.argv[0])
    if script_dir == '':
        script_dir = '.'
    os.chdir(script_dir)

    # Grabs all the templates from the templates directory.
    template_dir = pkg_resources.resource_filename('generator', 'templates')

    # Initializes the Jinja2 FileSystemLoader with the template directory.
    template_loader = jinja2.FileSystemLoader(searchpath=template_dir)
    template_env = jinja2.Environment(loader=template_loader, keep_trailing_newline=True, extensions=[RaiseExtension])
    template_env.globals["can"] = can
    template_env.globals["system"] = system
    template_env.globals["unit_types"] = system.unit_types
    template_env.globals["all_unit_files"] = all_unit_files

    # Renders the templates to the output directory.
    for filename in ["pack_unpack.cpp", "pack_unpack.h", "enum_atom.h", "send_receive.cpp", "structs.h", "bus.h"]:
        render_template(template_env, filename)

    # Writes the constants, computer, and drivers files.
    constants.write(template_env, constants_template_path, constants_path)
    computers_h.write(template_env, system.computer, computer_h_template_path, computer_h_dir_path)
    computers_c.write(template_env, system.computer, computer_c_template_path, computer_c_dir_path)
    drivers_inc.write(template_env, system, drivers_inc_template_dir_path, drivers_inc_dir_path)

    # Copies over the static source files for the drivers.
    src_dir = Path(__file__).parent.parent.joinpath('src')
    for src_file in src_dir.iterdir():
        if src_file.is_file():
            src_file_path = src_file.resolve()
            dst_file_path = output_dir.joinpath(f"src/{src_file.name}")
            with open(src_file_path, 'r') as src_file:
                # Creates the destination directory if it doesn't exist.
                dst_file_dir = dst_file_path.parent
                dst_file_dir.mkdir(parents=True, exist_ok=True)
                with open(dst_file_path, 'w') as dst_file:
                    dst_file.write(src_file.read())

if __name__ == "__main__":
    main()