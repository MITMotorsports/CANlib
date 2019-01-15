'''
Generate all of the headers in the computers directory.
Run this file (with the spec path as a command line argument) to write just
these files or main.py to write all files.
'''
import sys
sys.path.append("ParseCAN")
import os
import ParseCAN
from common import computer_dir_path, coord, templ, ifndef, endif, is_multplxd

def handle_frame(frame, bus_name, fw):
    fw(coor())

def write(can, computers, output_path=computer_dir_path):
    '''
    Generate constants.h file, which has CAN IDs and enum values.

    :param output_path: file to be written to
    :param can: CAN spec
    '''

    os.makedirs(output_path, exist_ok=True)

    for computer in computers:
        header_name = '_CAN_LIBRARY_{}_H'.format(computer.name.upper())
        f_path = os.path.join(output_path, 'canlib_{}.h'.format(computer.name))

        with open(f_path, 'w') as f:
            fw = f.write

            fw(ifndef(header_name))
            fw('#include <stdint.h>\n')
            fw('#include <stdbool.h>\n\n')
            fw('#include "constants.h"\n')
            fw('#include "enum_atom.h"\n')
            fw('#include "structs.h"\n')
            fw('#include "static_can.h"\n')
            fw('#include "CANlib.h"\n\n')

            # Helper functions to be used recursively
            def declare_pub_frame(frame, name_prepends):
                if is_multplxd(frame):
                    for sub_frame in frame.frame:
                        declare_pub_frame(sub_frame, name_prepends + '_' + frame.name)
                else:
                    tot_name = coord(name_prepends, frame.name,
                        prefix=False)
                    fw('void send_{}_msg(CANlib_{}_T inp);\n'.format(
                        tot_name, tot_name))
            
            def declare_sub_frame(frame, name_prepends):
                if is_multplxd(frame):
                    for sub_frame in frame.frame:
                        declare_sub_frame(sub_frame, name_prepends + '_' + frame.name)
                else:
                    tot_name = coord(name_prepends, frame.name)
                    fw('extern {}_T inp;\n'.format(
                        tot_name, tot_name))
            
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


            
            # Actual writes
            # Structs
            # pubs_subs = {}
            # try:
            #     for bus_name, bus in computer.participation['name']['can'].publish.items():
            #         for frame in bus:
            #             pubs_subs[frame.name] = (frame, bus_name)
            #     for bus_name, bus in computer.participation['name']['can'].subscribe.items():
            #         for frame in bus:
            #             pubs_subs[frame.name] = (frame, bus_name)
            # except KeyError:
            #     pass # No CAN messages received by this board
            
            # for frame, bus_name in pubs_subs.values():
            #     write_struct(frame, bus_name)


            # Pub
            try:
                for bus_name, bus in computer.participation['name']['can'].publish.items():
                    for frame in bus:
                        declare_pub_frame(frame, bus_name)
            except KeyError:
                pass # No CAN messages sent by this board

            fw('\n')

            # Sub
            try:
                for bus_name, bus in computer.participation['name']['can'].subscribe.items():
                    for frame in bus:
                        declare_sub_frame(frame, bus_name)
                        fw('\n')
            except KeyError:
                pass # No CAN messages received by this board

            fw(endif(header_name))




    header_name = '_CAN_LIBRARY_CONSTANTS_H'

