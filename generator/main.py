"""
Generate all necessary files.
Pass the spec_path as a command line argument.
"""
import constants
import MY18_Can_Library_c
import MY18_Can_Library_h
import enum_segments
import boards
import sys
from common import *  # All method arguments come from here

if __name__ == "__main__":
    spec_path = sys.argv[1]
    constants.write(constants_path, spec_path)
    MY18_Can_Library_c.write(can_lib_c_path, spec_path, can_lib_c_base_path)
    MY18_Can_Library_h.write(can_lib_h_path, spec_path)
    enum_segments.write(enum_segments_path, spec_path)
    boards.write(spec_path)
