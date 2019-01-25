import sys
sys.path.append("ParseCAN")
import os
import ParseCAN
from common import test_h_dir_path, coord, templ, ifndef, endif, frame_handler


def declare_pub_frame(frame, name_prepends, fw):
    tot_name = coord(name_prepends, frame.name, prefix=False)
    fw('void CANlib_Send_{}(CANlib_{}_T *inp);\n'.format(tot_name, tot_name))


def declare_sub_frame(frame, name_prepends, fw):
    tot_name = coord(name_prepends, frame.name, prefix=False)
    fw(
        'extern CANlib_{}_T CANlib_{}_Input;\n'.format(tot_name, tot_name) +
        'void CANlib_Handle_{}(Frame *frame);\n'.format(tot_name, tot_name)
    )


def write(can, output_path=test_h_dir_path):
    os.makedirs(output_path, exist_ok=True)

    header_name = '_CAN_LIBRARY_TEST_H'
    f_path = os.path.join(output_path, 'canlib_test.h')

    with open(f_path, 'w') as f:
        fw = f.write

        fw(ifndef(header_name))
        fw('#include <stdint.h>\n')
        fw('#include <stdbool.h>\n\n')

        fw('#include "constants.h"\n')
        fw('#include "enum_atom.h"\n')
        fw('#include "structs.h"\n')
        fw('#include "static.h"\n')
        fw('#include "evil_macros.h"\n')
        fw('#include "pack_unpack.h"\n\n')

        fw(
            '#ifndef TEST\n'
            '#error "test.h should only be included in testing files!"\n'
            '#endif\n\n'
        )

        fw('\n')

        # Pub
        for bus in can.bus:
            for frame in bus.frame:
                frame_handler(frame, bus.name, declare_pub_frame, fw)

        fw('\n')

        for bus in can.bus:
            for frame in bus.frame:
                frame_handler(frame, bus.name, declare_sub_frame, fw)
                fw('\n')
        fw('void CANlib_update_can_TEST(void);\n')

        fw(endif(header_name))

