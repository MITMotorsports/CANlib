import sys
sys.path.append('ParseCAN')
import ParseCAN
from common import can_lib_h_path, templ, coord, ifndef, endif, frame_handler


def write_declare(frame, name_prepends, fw):
    tot_name = coord(name_prepends, frame.name, prefix=False)
    fw (
        'CANlib_Transmit_Error_T CANlib_Transmit_{}'.format(tot_name) +
        '(CANlib_{}_T *type);\n'.format(tot_name)
    )
    fw(
        'void CANlib_Pack_{}(CANlib_{}_T *type_in, Frame *can_out);\n'.format(tot_name, tot_name) +
        'void CANlib_Unpack_{}(Frame *can_in, CANlib_{}_T *type_out);\n'.format(tot_name, tot_name)
    )


def write(can, output_path=can_lib_h_path):
    header_name = '_PACK_UNPACK_H'

    with open(output_path, 'w') as f:
        fw = f.write

        # Setup file
        fw(ifndef(header_name))

        # Includes
        fw(
            '#include <stdint.h>' '\n'
            '#include <stdbool.h>' '\n\n'
            '#include "static.h"' '\n'
            '#include "driver.h"\n'
            '#include "constants.h"\n'
            '#include "structs.h"\n\n'
        )

        # Declare
        for bus in can.bus:
            for msg in bus.frame:
                frame_handler(msg, bus.name, write_declare, fw)
                fw('\n\n')

        fw('void to_bitstring(uint8_t in[], uint64_t *out);\n\n')
        fw(endif(header_name))
