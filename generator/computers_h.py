'''
Generate all of the headers in the computers directory.
Run this file (with the spec path as a command line argument) to write just
these files or main.py to write all files.
'''
import sys
sys.path.append("ParseCAN")
import os
import ParseCAN
from common import computer_h_dir_path, coord, templ, ifndef, endif, is_multplxd, frame_handler

def handle_frame(frame, bus_name, fw):
    fw(coor())

def write(can, computers, output_path=computer_h_dir_path):
    '''
    Generate computer header files.

    :param output_path: file to be written to
    :param can: CAN spec
    '''

    os.makedirs(output_path, exist_ok=True)

    for computer in computers:
        header_name = '_CAN_LIBRARY_{}_H'.format(computer.name.upper())
        f_path = os.path.join(output_path, 'canlib_{}.h'.format(computer.name))

        if not ('can' in computer.participation['name'].keys()):
            # This computer neither sends nor recieves can messagess
            continue

        with open(f_path, 'w') as f:
            fw = f.write

            fw(ifndef(header_name))
            fw('#include <stdint.h>\n')
            fw('#include <stdbool.h>\n\n')
            fw('#include "constants.h"\n')
            fw('#include "enum_atom.h"\n')
            fw('#include "structs.h"\n')
            fw('#include "static.h"\n')
            fw('#include "pack_unpack.h"\n\n')

            for assigned_name, periph_name in computer.participation['name']['can'].mapping.items():
                if assigned_name in computer.participation['name']['can'].publish.keys():
                    fw('#define USING_{}\n'.format(periph_name))
                elif assigned_name in computer.participation['name']['can'].subscribe.keys():
                    fw('#define USING_{}\n'.format(periph_name))

            fw('\n')

            # Helper functions to be used recursively
            def declare_pub_frame(frame, name_prepends, *args):
                tot_name = coord(name_prepends, frame.name, prefix=False)
                fw('void send_{}_msg(CANlib_{}_T *inp);\n'.format(tot_name, tot_name))
            
            def declare_sub_frame(frame, name_prepends, *args):
                tot_name = coord(name_prepends, frame.name, prefix=False)
                fw('extern CANlib_{}_T {}_inp;\n'.format(tot_name, tot_name))
                fw('void handle_{}_msg(Frame *frame);\n'.format(tot_name, tot_name))
            
            def write_struct(frame, name_prepends):
                if is_multplxd(frame):
                    for sub_frame in frame.frame:
                        write_struct(sub_frame, name_prepends + '_' + frame.name)
                else:
                    fw('typedef struct {\n')
                    for atom in frame.atom:
                        if atom.type.isenum():
                            enum_name = coord(name_prepends, frame.name, atom.name) + '_T'
                            fw('\t{} {};\n'.format(enum_name, atom.name))
                        else:
                            fw('\t{} {};\n'.format(atom.type.ctype(), atom.name))

                    fw('} ' + '{}_T;\n\n'.format(coord(name_prepends, frame.name)))

            # Pub
            try:
                for bus_name, bus in computer.participation['name']['can'].publish.items():
                    for frame in bus:
                        frame_handler(frame, bus_name, declare_sub_frame)
            except KeyError:
                pass # No CAN messages sent by this board

            fw('\n')

            # Sub
            try:
                for bus_name, bus in computer.participation['name']['can'].subscribe.items():
                    for frame in bus:
                        frame_handler(frame, bus_name, declare_sub_frame)
                        fw('\n')
                fw('void update_can(void);\n')
            except KeyError:
                pass # No CAN messages received by this board

            fw(endif(header_name))




    header_name = '_CAN_LIBRARY_CONSTANTS_H'

