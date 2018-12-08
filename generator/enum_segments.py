'''
Generate enum_segments.h file.
Run this file (with the spec path as a command line argument) to write just
ids.h or main.py to write all files.
'''
import sys
sys.path.append("ParseCAN")
import ParseCAN
from common import enum_segments_path, templ, coord, ifndef, endif


def write(car, output_path=enum_segments_path):
    '''
    Generate enum_segments.h file, which includes enums for segments of type enum.

    :param output_path: file to be written to
    :param spec_path: CAN spec path
    '''

    header_name = '_CAN_LIBRARY_ENUM_SEGMENTS_H'

    with open(output_path, 'w') as f:
        fw = f.write

        fw(ifndef(header_name))
        fw('#include "constants.h"\n\n')

        for bus in car.buses:
            for msg in bus.frames:
                for seg in msg.segments:
                    if seg.type.isenum():
                        fw('typedef enum {\n')

                        for enum in seg.enumerations:
                            fw(templ['enum'].format(
                                coord(bus.name, msg.name, seg.name, enum.name),
                                enum.value
                            ))

                        fw('} ' + '{}_T;\n\n'.format(coord(bus.name, msg.name, seg.name)))

        fw(endif(header_name))
