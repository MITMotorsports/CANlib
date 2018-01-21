"""
Generate header files that set up structs for each board).
Run this file (with the spec path as a command line argument) to write just
these files or main.py to write all files.
"""
import sys
sys.path.append("ParseCAN")
import ParseCAN
import os

def write(spec_path):
    """
    Write the header files for the main structs in the library.

    :param output_paths: a dictionary with a path for the BMS, CAN node, current sensor, dash, and VCU header files
    :param spec_path: path to the CAN spec
    """
    car = ParseCAN.spec.car(spec_path)
    try:
        os.mkdir("../boards")
    except FileExistsError:
        pass
    for board in car.boards:
        with open("../boards/" + board.name + ".h", 'w') as f:
            f.write("#ifndef _CAN_LIBRARY_" + board.name.upper() + "_H\n")
            f.write("#define _CAN_LIBRARY_" + board.name.upper() + "_H\n\n")
            f.write('#include "constants.h"\n\n')
            f.write("#include <stdint.h>\n")
            f.write("#include <stdbool.h>\n\n")
            f.write('#include "enum_segments.h"\n\n')

            for bus in board.publish:
                for message in bus.messages:
                    f.write("typedef struct {\n")
                    for segment in message.segments:
                        if segment.c_type != "enum":
                            f.write("  " + segment.c_type + " " + segment.name + ";\n")
                        else:
                            enum_name = "Can_" + message.name
                            enum_name += "ID_T"
                            f.write("  " + enum_name + " " + segment.name + ";\n")
                    f.write("} Can_" + message.name + "_T;\n\n")
            f.write("#endif // _CAN_LIBRARY_" + board.name.upper() + "_H")


if __name__ == "__main__":
    spec_path = sys.argv[1]
    write(spec_path)
