"""Generate all necessary files."""
import constants
import MY18_Can_Library_c
import MY18_Can_Library_h
import enum_segments
import boards
from common import *  # All method arguments come from here

if __name__ == "__main__":
    constants.write(constants_path, spec_path)
    MY18_Can_Library_c.write(can_lib_c_path, spec_path, can_lib_c_base_path)
    MY18_Can_Library_h.write(can_lib_h_path, spec_path, boards_paths.values())
    enum_segments.write(enum_segments_path, spec_path)
    boards.write(boards_paths, spec_path)
