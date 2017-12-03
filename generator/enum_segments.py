"""
Generate ids.h file.
Run this file to write just ids.h or main.py to write all files.
"""
import sys
sys.path.append("ParseCAN")
import ParseCAN
from common import enum_segments_path, spec_path


def write(output_path, spec_path):
    """
    Generate enum_segments.h file, which includes enums for segments of type enum.

    :param output_path: file to be written to
    :param spec_path: CAN spec path
    """
    spec = ParseCAN.spec.can(spec_path)
    with open(output_path, 'w') as f:
        f.write('#ifndef _MY18_CAN_LIBRARY_IDS_H\n')
        f.write('#define _MY18_CAN_LIBRARY_IDS_H\n\n')
        f.write('#include "can_validator/fsae_can_spec.h"\n\n')

        for message in spec.messages.values():
            for segment_name, segment in message.segments.items():
                if segment.c_type == 'enum':
                    f.write('typedef enum {\n')
                    for value_name, value in segment.values.items():
                        message_name = "CAN_" + message.name.upper()
                        f.write(
                            "  " + message_name + "_" + value_name.upper() + " = " +
                            '____' + message.name.upper() + '__' + segment_name.upper() + '__' + value_name +
                            ",\n")
                    f.write("} Can_" + message.name.replace('Heartbeat', 'State') + "ID_T;\n\n")
        f.write('#endif // _MY18_CAN_LIBRARY_IDS_H')


if __name__ == "__main__":
    write(enum_segments_path, spec_path)
