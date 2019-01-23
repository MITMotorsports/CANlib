import sys
sys.path.append("ParseCAN")
import ParseCAN
from common import send_recieve_path, coord, templ, ifndef, endif, frame_handler


def define_pub_frame(frame, name_prepends, busnm, fw):
    tot_name = coord(name_prepends, frame.name,
        prefix=False)
    fw('void CANlib_send_{}_msg(CANlib_{}_T *inp)'.format(
        tot_name, tot_name) + ' {\n'
    )

    if frame.period is not None:
        fw('\tLIMIT(CANlib_{});\n'.format(tot_name))

    fw(
        '\tFrame frame;\n' +
        '\tCANlib_Pack_' + tot_name + '(inp, &frame);\n' +
        '\tCANlib_TransmitFrame(&frame,' + busnm + ');\n'
        '}\n\n'
    )


def define_sub_frame(frame, name_prepends, fw):
    tot_name = coord(name_prepends, frame.name,
        prefix=False)
    fw('void handle_{}_msg(Frame *frame)'.format(
        tot_name, tot_name) + ' {\n' + '\tCANlib_Unpack_{}(frame, &CANlib_{}_inp);\n'.format(tot_name, tot_name) + '}\n\n')


def define_struct(frame, name_prepends, fw):
    tot_name = coord(name_prepends, frame.name)
    fw('{}_T {}_inp;\n'.format(
        tot_name, tot_name))


def write(can, output_path=send_recieve_path):
    with open(output_path, 'w') as f:
        fw = f.write
        fw('#include "pack_unpack.h"\n\n')

        for bus in can.bus:
            for msg in bus.frame:
                frame_handler(msg, bus.name, define_struct, fw)
        fw('\n')

        for bus in can.bus:
            for msg in bus.frame:
                frame_handler(msg, bus.name, define_pub_frame, bus.name, fw)
                frame_handler(msg, bus.name, define_sub_frame, fw)
        fw('\n')


