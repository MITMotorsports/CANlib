import sys
import os
import jinja2

sys.path.append('ParseCAN')
import ParseCAN

import constants
import pack_unpack_c
import pack_unpack_h
import enum_atom
import structs
import sys
import computers_h
import computers_c
# import test_h
# import test_c
import send_receive
import bus
import drivers_inc

from jinja2 import nodes
from jinja2.ext import Extension
from jinja2.exceptions import TemplateRuntimeError


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


if __name__ == '__main__':

    specpath = sys.argv[1]
    specfile = open(specpath, 'r')
    system = ParseCAN.spec.System.from_yaml(specfile)
    can = system.protocol['name']['can']

    generator_dir = os.path.dirname(sys.argv[0])
    template_dir = os.path.join(generator_dir, "templates")
    template_loader = jinja2.FileSystemLoader(searchpath=[template_dir, "."])
    template_env = jinja2.Environment(loader=template_loader, keep_trailing_newline=True, extensions=[RaiseExtension])
    template_env.globals["can"] = can
    template_env.globals["system"] = system

    constants.write(template_env)
    pack_unpack_c.write(template_env)
    pack_unpack_h.write(template_env)
    enum_atom.write(template_env)
    send_receive.write(template_env)
    structs.write(template_env)
    bus.write(template_env)
    computers_h.write(template_env, system.computer)
    computers_c.write(template_env, system.computer)
    # test_h.write(can)
    # test_c.write(can)

    drivers_inc.write(template_env, system)
