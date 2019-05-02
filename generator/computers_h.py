import sys
sys.path.append("ParseCAN")
import os
import ParseCAN
from common import computer_h_dir_path, coord, templ, ifndef, endif, frame_handler


def declare_pub_frame(frame, name_prepends, fw):
    tot_name = coord(name_prepends, frame.name, prefix=False)
    fw('void CANlib_Send_{}(CANlib_{}_T *inp);\n'.format(tot_name, tot_name))


def declare_sub_frame(frame, name_prepends, fw):
    tot_name = coord(name_prepends, frame.name, prefix=False)
    fw(
        'extern CANlib_{}_Timestamped_T CANlib_{}_Input;\n'.format(tot_name, tot_name) +
        'void CANlib_Handle_{}(TimestampedFrame *frame);\n'.format(tot_name, tot_name)
    )


def write(can, computers, output_path=computer_h_dir_path):
    os.makedirs(output_path, exist_ok=True)

    for computer in computers:
        header_name = '_CAN_LIBRARY_{}_H'.format(computer.name.upper())
        f_path = os.path.join(output_path, 'canlib_{}.h'.format(computer.name))

        if not ('can' in computer.participation['name'].keys()):
            # This computer neither sends nor receives can messages
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
            fw('#include "evil_macros.h"\n')
            fw('#include "pack_unpack.h"\n')
            fw('#include "bus.h"\n\n')

            fw('\n')

            fw('#ifdef __cplusplus\nextern "C" {\n#endif // __cplusplus\n\n')

            # Pub
            try:
                for bus_name, bus in computer.participation['name']['can'].publish.items():
                    for frame in bus:
                        frame_handler(frame, bus_name, declare_pub_frame, fw)
            except KeyError:
                pass  # No CAN messages sent by this board

            fw('\n')

            # Sub
            try:
                for bus_name, bus in computer.participation['name']['can'].subscribe.items():
                    for frame in bus:
                        frame_handler(frame, bus_name, declare_sub_frame, fw)
                        fw('\n')
                fw('void CANlib_update_can(void);\n')
                fw('void CANlib_HandleFrame(CAN_Raw_Bus_T raw_bus);\n')
            except KeyError:
                pass  # No CAN messages received by this board

            fw('\n#ifdef __cplusplus\n} // extern "C"\n#endif // __cplusplus\n\n')

            fw(endif(header_name))

