'''
Generate Can_Libary.h file.
Run this file (with the spec path as a command line argument) to write just
Can_Libary.h or main.py to write all files.
'''
import sys
sys.path.append('ParseCAN')
import ParseCAN
from common import can_lib_h_path, coord, ifndef, endif


def write(car, output_path=can_lib_h_path):
    '''
    Generate Can_Libary.h file.

    :param output_path: file to be written to
    :param car: CAN spec
    '''
    header_name = '_CAN_LIBRARY_H'

    with open(output_path, 'w') as f:
        fw = f.write

        # Setup file
        fw(ifndef(header_name))

        # Includes
        fw('#include <stdint.h>' '\n'
           '#include <stdbool.h>' '\n\n')

        # Create enum
        for bus in car.buses:
            fw('typedef enum {\n')

            for msgt in ['CAN_UNKNOWN_MSG', 'CAN_ERROR_MSG']:
                fw('\t' '{},\n'.format(msgt))

            for msg in bus.messages:
                fw('\t' '{},\n'.format(coord(bus.name, msg.name)))

            fw('} ' + '{}_T;\n\n'.format(coord(bus.name)))

        # fw('Can_MsgID_T Can_MsgType(void);' '\n\n')

        # Include static stuff
        fw('#include "static_can.h"' '\n\n')

        # Write DECLARE statements
        for bus in car.buses:
            for msg in bus.messages:
                fw('DECLARE({})\n'.format(coord(bus.name, msg.name)))

        fw('\n')

        # Finish up
        fw(endif(header_name))


if __name__ == '__main__':
    spec_path = sys.argv[1]
    car = ParseCAN.spec.car(spec_path)
    write(spec_path)
