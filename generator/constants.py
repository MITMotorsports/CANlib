"""
Generate constants.h file.
Run this file (with the spec path as a command line argument) to write just
constants.h or main.py to write all files.
"""
import sys
sys.path.append("ParseCAN")
import ParseCAN
from common import constants_path
import re


def write(output_path, spec_path):
    """
    Generate constants.h file, which has CAN IDs and enum values.

    :param output_path: file to be written to
    :param spec_path: CAN spec path
    """
    car = ParseCAN.spec.car(spec_path)
    with open(output_path, 'w') as f:
        clean_output_path = re.sub('[^A-Za-z0-9_]+', '_', output_path).upper()
        f.write('#ifndef ____' + clean_output_path + '\n')
        f.write('#define ____' + clean_output_path + '\n\n')
        for bus in car.buses.values():
            for message in bus.messages:
                # Write IDs
                f.write('#define ' + message.name.upper() + '__id ' + str(message.can_id) + '\n')

                # Write enum fields
                for segment in message.segments:
                    if len(segment.values) > 0:   # Segment is type enum
                        for value in segment.values:
                            f.write('#define ____' + message.name.upper() + '__' +
                                    segment.name.upper() + '__' + value.name
                                    + ' ' + str(value.value) + '\n')

                # Write frequencies
                if message.frequency:
                    f.write('#define ' + message.name.upper() + '__freq ' +
                            str(int(message.frequency)) + '\n')

            f.write('\n')
        f.write('#endif // ____' + clean_output_path + '\n')


if __name__ == "__main__":
    spec_path = sys.argv[1]
    write(constants_path, spec_path)
