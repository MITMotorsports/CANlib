'''
Generate header files that set up structs for all bus.
Run this file (with the spec path as a command line argument) to write just
these files or main.py to write all files.
'''
import sys
sys.path.append('ParseCAN')
import ParseCAN
from common import structs_path, coord, ifndef, endif

def process_single(fw, frame, prefix):
    """
    Helper function to avoid duplicate code between multiplexed
    and single frames.

    :param fw: file writer
    :param frame: frame to write
    :prefix: bus name for single frames, bus name + top level name for
             Multiplexed
    """
    for atom in frame.atom:
        if atom.type.isenum():
            enum_name = coord(prefix, frame.name, atom.name) + '_T'
            fw('\t{} {};\n'.format(enum_name, atom.name))
        else:
            fw('\t{} {};\n'.format(atom.type.ctype(), atom.name))

    fw('} ' + '{}_T;\n\n'.format(coord(prefix, frame.name)))

def write(can, output_path=structs_path):
    '''
    Write the header files for the main structs in the library.

    :param output_path: file to be written to
    :param can: CAN spec
    '''
    header_name = '_CAN_LIBRARY_STRUCTS'

    with open(output_path, 'w') as f:
        fw = f.write

        fw(ifndef(header_name))
        fw('#include <stdint.h>\n')
        fw('#include <stdbool.h>\n\n')
        fw('#include "constants.h"\n\n')
        fw('#include "enum_atom.h"\n\n')

        for bus in can.bus:
            for msg in bus.frame:
                fw('typedef struct {\n')

                if isinstance(msg, ParseCAN.spec.bus.MultiplexedFrame):
                    for frame in msg.frame:
                        process_single(fw, frame, bus.name + "_" + msg.name)
                else:
                    process_single(fw, msg, bus.name)
        fw(endif(header_name))
