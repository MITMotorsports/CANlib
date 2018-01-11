"""
Generate enum_segments.h file.
Run this file (with the spec path as a command line argument) to write just
ids.h or main.py to write all files.
"""
import sys
sys.path.append("ParseCAN")
import ParseCAN
from common import enum_segments_path


def write(output_path, spec_path):
    """
    Generate enum_segments.h file, which includes enums for segments of type enum.

    :param output_path: file to be written to
    :param spec_path: CAN spec path
    """
    car = ParseCAN.spec.car(spec_path)
    with open(output_path, 'w') as f:
        f.write('#ifndef _CAN_LIBRARY_ENUM_SEGMENTS_H\n')
        f.write('#define _CAN_LIBRARY_ENUM_SEGMENTS_H\n\n')
        f.write('#include "constants.h"\n\n')

        for bus in car.buses.values():
            for message in bus.messages:
                for segment in message.segments:
                    if segment.c_type == 'enum':
                        f.write('typedef enum {\n')
                        for value in segment.values:
                            message_name = "CAN_" + message.name.upper()
                            f.write(
                                "  " + message_name + "_" + value.name.upper() + " = " +
                                '____' + message.name.upper() + '__' + segment.name.upper() + '__' + value.name +
                                ",\n")
                        f.write("} Can_" + message.name + "ID_T;\n\n")
        f.write('#endif // _CAN_LIBRARY_ENUM_SEGMENTS_H')


if __name__ == "__main__":
    spec_path = sys.argv[1]
    write(enum_segments_path, spec_path)
