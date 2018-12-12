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
import enum_segments
import structs
import sys

if __name__ == '__main__':
    specpath = sys.argv[1]
    specfile = open(specpath, 'r')
    car = ParseCAN.spec.Car.from_yaml(specfile)

    constants.write(car)
    CANlib_c.write(car)
    CANlib_h.write(car)
    enum_segments.write(car)
    structs.write(car)
