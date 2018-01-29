'''
Generate header files that set up structs for all buses.
Run this file (with the spec path as a command line argument) to write just
these files or main.py to write all files.
'''
import sys
sys.path.append('ParseCAN')
import ParseCAN
from common import structs_path, coord, ifndef, endif

def write(car, output_path=structs_path):
    '''
    Write the header files for the main structs in the library.

    :param output_path: file to be written to
    :param car: CAN spec
    '''
    header_name = '_CAN_LIBRARY_STRUCTS'

    with open(output_path, 'w') as f:
        fw = f.write

        fw(ifndef(header_name))
        fw('#include <stdint.h>\n')
        fw('#include <stdbool.h>\n\n')
        fw('#include "constants.h"\n\n')
        fw('#include "enum_segments.h"\n\n')

        for bus in car.buses:
            for msg in bus.messages:
                fw('typedef struct {\n')

                for seg in msg.segments:
                    if seg.c_type == 'enum':
                        enum_name = coord(bus.name, msg.name, seg.name)
                        fw('\t{} {};\n'.format(enum_name, seg.name))
                    else:
                        fw('\t{} {};\n'.format(seg.c_type, seg.name))

                fw('} ' + '{}_T;\n\n'.format(coord(bus.name, msg.name)))

        fw(endif(header_name))


if __name__ == '__main__':
    spec_path = sys.argv[1]
    car = ParseCAN.spec.car(spec_path)
    write(car)
