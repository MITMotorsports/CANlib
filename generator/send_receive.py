import sys
sys.path.append("ParseCAN")
import ParseCAN
from common import send_recieve_path, coord, templ, ifndef, endif, frame_handler


def define_pub_frame(frame, name_prepends, busnm, fw):
    tot_name = coord(name_prepends, frame.name,
        prefix=False)
    fw('void CANlib_Send_{}(CANlib_{}_T *inp)'.format(
        tot_name, tot_name) + ' {\n'
    )

    if frame.period is not None:
        fw('\tLIMIT(CANlib_{});\n'.format(tot_name))

    fw(
        '\tFrame frame;\n' +
        '\tCANlib_Pack_' + tot_name + '(inp, &frame);\n' +
        '\tCANlib_TransmitFrame(&frame, ' + busnm + ');\n'
        '}\n\n'
    )


def define_sub_frame(frame, name_prepends, fw):
    tot_name = coord(name_prepends, frame.name,
        prefix=False)
    fw('void CANlib_Handle_{}(TimestampedFrame *ts_frame) {{\n'.format(tot_name, tot_name))
    fw('\tCANlib_{}_Input.stamp = HAL_GetTick();\n'.format(tot_name))
    fw('\tCANlib_Unpack_{}(&(ts_frame->frame), &(CANlib_{}_Input.msg));\n'.format(tot_name,tot_name))
    fw('}\n\n')


def define_struct(frame, name_prepends, fw):
    tot_name = coord(name_prepends, frame.name)
    fw('{}_Timestamped_T {}_Input;\n'.format(
        tot_name, tot_name))


def write(can, output_path=send_recieve_path):
    with open(output_path, 'w') as f:
        fw = f.write
        fw('#include <stm32f4xx_hal.h>\n')
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


