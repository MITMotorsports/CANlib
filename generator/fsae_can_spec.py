"""
Generate fsae_can_spec.h file.
Run this file to write just fsae_can_spec.h or main.py to write all files.
"""
import sys
sys.path.append("ParseCAN")
import ParseCAN
from common import fsae_can_spec_path, spec_path
import re


def write(output_path, spec_path):
    """
    Generate fsae_can_spec.h file.

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
            if message.frequency:
                f.write('#define ' + message.name.upper() + '__freq ' + str(int(message.frequency)) + '\n')
            else:
                f.write('#define ' + message.name.upper() + '__freq ' + str(-1) + '\n')
            for segment_name, segment in message.segments.items():
                f.write('#define __' + message.name.upper() + '__' + segment_name.upper() + '__start ' +
                        str(segment.position[0]) + '\n')
                f.write('#define __' + message.name.upper() + '__' + segment_name.upper() + '__end ' +
                        str(segment.position[1]) + '\n')
                for value_name, value in segment.values.items():
                    if value.range[0] == value.range[1]:
                        f.write('#define ____' + message.name.upper() + '__' + segment_name.upper() + '__' +
                                value_name + ' ' + str(value.range[0]) + '\n')
                    else:
                        f.write('#define ____' + message.name.upper() + '__' + segment_name.upper() + '__' +
                                value_name + '__FROM ' + str(value.range[0]) + '\n')
                        f.write('#define ____' + message.name.upper() + '__' + segment_name.upper() + '__' +
                                value_name + '__TO ' + str(value.range[1]) + '\n')

            f.write('\n')
        f.write('#endif // ____' + clean_output_path + '\n')


if __name__ == "__main__":
    write(fsae_can_spec_path, spec_path)
