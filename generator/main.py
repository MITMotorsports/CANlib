"""
Generate all necessary files.
Pass the spec_path as a command line argument.
"""
import sys
sys.path.append('ParseCAN')
import ParseCAN

import constants
import CANlib_c
import CANlib_h
import enum_atom
import structs
import sys
import computers
import send_receive

if __name__ == '__main__':
    specpath = sys.argv[1]
    specfile = open(specpath, 'r')
    system = ParseCAN.spec.System.from_yaml(specfile)
    can = system.protocol['name']['can']

    constants.write(can)
    CANlib_c.write(can)
    CANlib_h.write(can)
    enum_atom.write(can)
    send_receive.write(can)
    structs.write(can)
    computers.write(can, system.computer)
