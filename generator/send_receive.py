'''
Generate constants.h file.
Run this file (with the spec path as a command line argument) to write just
constants.h or main.py to write all files.
'''
import sys
sys.path.append("ParseCAN")
import ParseCAN
from common import send_recieve_path, coord, templ, ifndef, endif, is_multplxd, is_multplxd


def write(can, output_path=send_recieve_path):
    '''
    Generate constants.h file, which has CAN IDs and enum values.

    :param output_path: file to be written to
    :param can: CAN spec
    '''

    with open(output_path, 'w') as f:
        fw = f.write
        fw('#include "pack_unpack.h"\n\n')

        def define_pub_frame(frame, name_prepends):
            if is_multplxd(frame):
                for sub_frame in frame.frame:
                    define_pub_frame(sub_frame, name_prepends + '_' + frame.name)
            else:
                tot_name = coord(name_prepends, frame.name,
                    prefix=False)
                fw('void send_{}_msg(CANlib_{}_T *inp)'.format(
                    tot_name, tot_name) + ' {\n'
                )

                if frame.period is not None:
                    fw('\tLIMIT(CANlib_{});\n'.format(tot_name))

                fw('\tCANlib_Transmit_{}(inp);\n'.format(tot_name, tot_name) + '}\n\n')

        def define_sub_frame(frame, name_prepends):
            if is_multplxd(frame):
                for sub_frame in frame.frame:
                    define_sub_frame(sub_frame, name_prepends + '_' + frame.name)
            else:
                tot_name = coord(name_prepends, frame.name,
                    prefix=False)
                fw('void handle_{}_msg(Frame *frame)'.format(
                   tot_name, tot_name) + ' {\n' + '\tCANlib_Unpack_{}(frame, &{}_inp);\n'.format(tot_name, tot_name) + '}\n\n')
        
        def declare_struct(frame, name_prepends):
                if is_multplxd(frame):
                    for sub_frame in frame.frame:
                        declare_struct(sub_frame, name_prepends + '_' + frame.name)
                else:
                    tot_name = coord(name_prepends, frame.name, prefix=False)
                    fw('CANlib_{}_T {}_inp;\n'.format(
                        tot_name, tot_name))

        for bus in can.bus:
            for msg in bus.frame:
                declare_struct(msg, bus.name)
        fw('\n')

        for bus in can.bus:
            for msg in bus.frame:
                define_pub_frame(msg, bus.name)
                define_sub_frame(msg, bus.name)
        fw('\n')


