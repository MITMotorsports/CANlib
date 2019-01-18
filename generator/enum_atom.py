import sys
sys.path.append("ParseCAN")
import ParseCAN
from common import enum_atom_path, templ, coord, ifndef, endif, frame_handler


def handle_frame(frame, name_prepends, fw):
    for atom in frame.atom:
        tot_name = coord(name_prepends, frame.name, atom.name)
        if atom.type.isenum():
            # Only C++11 feature
            # fw('typedef enum ' + (atom.type.type + ' ' if atom.type.type else '') + '{\n')

            fw('typedef enum {\n')

            for enum in atom.type.enum:
                fw(templ['enum'].format(
                    tot_name + '_' + enum.name,
                    enum.value
                ))

            fw('} ' + '{}_T;\n\n'.format(tot_name))


def write(can, output_path=enum_atom_path):
    header_name = '_CAN_LIBRARY_ENUM_ATOM_H'

    with open(output_path, 'w') as f:
        fw = f.write

        fw(ifndef(header_name))

        for bus in can.bus:
            for msg in bus.frame:
                frame_handler(msg, bus.name, handle_frame, fw)

        fw(endif(header_name))
