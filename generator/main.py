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
import test_h
import test_c
import send_receive
import bus
import drivers_inc

if __name__ == '__main__':

    specpath = sys.argv[1]
    specfile = open(specpath, 'r')
    system = ParseCAN.spec.System.from_yaml(specfile)
    can = system.protocol['name']['can']

    generator_dir = os.path.dirname(sys.argv[0])
    template_dir = os.path.join(generator_dir, "templates")
    template_loader = jinja2.FileSystemLoader(searchpath=template_dir)
    template_env = jinja2.Environment(loader=template_loader)
    template_env.globals["can"] = can
    template_env.globals["system"] = system

    constants.write(template_env)
    pack_unpack_c.write(template_env, can)
    pack_unpack_h.write(can)
    enum_atom.write(can)
    send_receive.write(can)
    structs.write(can)
    bus.write(can, system.computer)
    computers_h.write(system, can, system.computer)
    computers_c.write(can, system.computer)
    test_h.write(can)
    test_c.write(can)

    drivers_inc.write(system)
