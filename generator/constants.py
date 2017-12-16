"""
Generate constants.h file.
Run this file to write just constants.h or main.py to write all files.
"""
import sys
sys.path.append("ParseCAN")
import ParseCAN
from common import constants_path, spec_path
import re


def write(output_path, spec_path):
    """
    Generate constants.h file, which has CAN IDs and enum values.

    :param output_path: file to be written to
    :param spec_path: CAN spec path
    """
    spec = ParseCAN.spec.can(spec_path)
    with open(output_path, 'w') as f:
        clean_output_path = re.sub('[^A-Za-z0-9_]+', '_', output_path).upper()
        f.write('#ifndef ____' + clean_output_path + '\n')
        f.write('#define ____' + clean_output_path + '\n\n')
        for id, message in spec.messages.items():
            f.write('#define ' + message.name.upper() + '__id ' + str(id) + '\n')
            for segment_name, segment in message.segments.items():
                if len(segment.values) > 0:   # Segment is type enum
                    for value_name, value in segment.values.items():
                        f.write('#define ____' + message.name.upper() + '__' + segment_name.upper() + '__' + value_name
                                + ' ' + str(value.value) + '\n')

            f.write('\n')
        f.write('#endif // ____' + clean_output_path + '\n')


if __name__ == "__main__":
    write(constants_path, spec_path)
