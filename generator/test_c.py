import sys
sys.path.append("ParseCAN")
import os
import ParseCAN
from common import test_c_dir_path, coord, templ, ifndef, endif, frame_handler
from computers_c import msg_handler


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
                '\tFrame frame;\n' +
                '\tCANlib_ReadFrame(&frame, {});\n'.format(bus.name) +
                '\tswitch(frame.id) {\n'
            )

            for msg in bus.frame:
                msg_handler(msg, bus.name, fw)

            fw(
                '\t}\n}\n\n'
            )

        fw('void CANlib_update_can_TEST() {\n')
        for bus in can.bus:
            fw('\tCANlib_update_can_{}();\n'.format(bus.name))
        fw('}\n\n')
