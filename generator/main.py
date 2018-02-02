"""
Generate all necessary files.
Pass the spec_path as a command line argument.
"""
import sys
sys.path.append('ParseCAN')
import ParseCAN

import constants
import Can_Library_c
import Can_Library_h
import enum_segments
import structs
import sys

if __name__ == '__main__':
    spec_path = sys.argv[1]
    car = ParseCAN.spec.car(spec_path)

    constants.write(car)
    Can_Library_c.write(car)
    Can_Library_h.write(car)
    enum_segments.write(car)
    structs.write(car)
