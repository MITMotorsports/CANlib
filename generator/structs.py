import sys
sys.path.append('ParseCAN')
import ParseCAN
from common import structs_path, coord, ifndef, endif, frame_handler


def msg_handler(frame, name_prepends, fw):
    for atom in frame.atom:
        if atom.type.isenum():
            enum_name = coord(name_prepends, frame.name, atom.name) + '_T'
            fw('\t{} {};\n'.format(enum_name, atom.name))
        else:
            fw('\t{} {};\n'.format(atom.type.ctype(), atom.name))

    fw('} ' + '{}_T;\n\n'.format(coord(name_prepends, frame.name)))


def write(can, output_path=structs_path):
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

                frame_handler(msg, bus.name, msg_handler, fw)
        fw(endif(header_name))
