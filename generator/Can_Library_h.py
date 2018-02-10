'''
Generate Can_Libary.h file.
Run this file (with the spec path as a command line argument) to write just
Can_Libary.h or main.py to write all files.
'''
import sys
sys.path.append('ParseCAN')
import ParseCAN
from common import can_lib_h_path, templ, coord, ifndef, endif


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
        fw(
            '#include <stdint.h>' '\n'
            '#include <stdbool.h>' '\n\n'
            '#include "structs.h"' '\n'
            '#include "static_can.h"' '\n\n'
        )

        # Universal message forms (independent of bus)
        uni = ['CAN_ERROR_MSG', 'CAN_UNKNOWN_MSG']

        # Begin enumerating from the universal forms onward
        # Every time we assign to an enum, we'll increment beg
        # so as to avoid return value equalities between different buses
        idx = 0
        for msgt in uni:
            fw(templ['define'].format(msgt, idx))
            idx += 1

        fw('\n')

        # Create forms enum for each bus
        for bus in car.buses:
            fw('typedef enum {\n')

            for msg in bus.messages:
                fw(templ['enum'].format(coord(bus.name, msg.name), idx))
                idx += 1

            fw('} ' + '{}_T;\n\n'.format(coord(bus.name)))

            fw('{0}_T identify_{0}(Frame* frame);'.format(coord(bus.name)) + '\n')

        # Write DECLARE statements
        for bus in car.buses:
            for msg in bus.messages:
                fw('DECLARE({})\n'.format(coord(bus.name, msg.name)))

        fw('\n')

        # Declare init functions
        # Write init functions
        for board in car.boards:
            if board.arch:  # Means it's a board we program
                for bus in board.subscribe:
                    fw('void init_{}(void);\n'.format(coord(bus.name, board.name)))

                # We still need to create init functions for boards that publish
                # on a bus but don't subscribe
                for bus in board.publish:
                    if bus.name not in board.subscribe.name:
                        fw(
                            'void init_' + coord(bus.name, board.name) +
                            '(void);\n'
                        )

        fw(endif(header_name))


if __name__ == '__main__':
    spec_path = sys.argv[1]
    car = ParseCAN.spec.car(spec_path)
    write(spec_path)
