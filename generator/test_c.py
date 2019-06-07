import sys
sys.path.append("ParseCAN")
import os
import ParseCAN
from common import test_c_dir_path, coord, templ, ifndef, endif, \
    frame_handler, is_multplxd
from math import ceil

def single_handler(frame, name_prepends, num_tabs, fw):
    tot_name = coord(name_prepends, frame.name, prefix=False)
    fw(
        '\t' * num_tabs + 'case CANlib_{}_key:'.format(tot_name) + '\n' +
        '\t' * (num_tabs + 1) + 'CANlib_Handle_{}(&frame);\n'.format(tot_name) +
        '\t' * (num_tabs + 1) + 'break;\n'
    )


def multplxd_handler(frame, name_prepends, num_tabs, fw):
    fw('\t' * num_tabs + 'case {}_key:\n'.format(coord(name_prepends, frame.name)))
    key_size = ceil(frame.slice.length / 8) * 8
    key_name = '_'.join([name_prepends,frame.name, 'key'])
    fw('\t' * (num_tabs + 1) + 'to_bitstring(frame.data, &bitstring);' '\n')
    fw(
        '\t' * (num_tabs + 1) + 'uint{}_t {} = EXTRACT(bitstring, {}, {});\n'.format(key_size, key_name, frame.slice.start, frame.slice.length) + '\t' * (num_tabs + 1) + 'switch(' + key_name + ') {' '\n'
    )

    name_prepends += '_' + frame.name

    for sub_frame in frame.frame:
        if is_multplxd(sub_frame):
            multplxd_handler(sub_frame, name_prepends, num_tabs + 2, fw)
        else:
            single_handler(sub_frame, name_prepends, num_tabs + 2, fw)
    fw('\t' * (num_tabs + 1) + '}\n')


def msg_handler(msg, busnm, fw):
    if is_multplxd(msg):
        multplxd_handler(msg, busnm, 2, fw)
    else:
        single_handler(msg, busnm, 2, fw)


def write(can, output_path=test_c_dir_path):
    os.makedirs(output_path, exist_ok=True)

    f_path = os.path.join(output_path, 'canlib_test.c')

    with open(f_path, 'w') as f:
        fw = f.write
        fw('#include "pack_unpack.h"\n')
        fw('#include "canlib_test.h"\n\n')

        fw(
            '#ifndef TEST\n'
            '#error "test.c should only be compiled testing files!"\n'
            '#endif\n\n'
        )

        for bus in can.bus:
            fw(
                'static void CANlib_update_can_TEST_{}(void)'.format(bus.name) + '{\n' +
                '\tFrame frame;\n'
            )
            if any(is_multplxd(msg) for msg in bus.frame):
                fw('\tuint64_t bitstring;\n')
            fw(
                '\tCANlib_ReadFrame(&frame, {});\n'.format(bus.name) +
                '\tswitch(frame.id) {\n'
            )

            for msg in bus.frame:
                msg_handler(msg, bus.name, fw)

            fw('\t}\n')

            fw(
                '}\n\n'
            )

        fw('void CANlib_update_can_TEST() {\n')
        for bus in can.bus:
            fw('\tCANlib_update_can_TEST_{}();\n'.format(bus.name))
        fw('}\n\n')
