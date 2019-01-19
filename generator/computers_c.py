import sys
sys.path.append("ParseCAN")
import os
import ParseCAN
from common import computer_c_dir_path, coord, templ, ifndef, endif, frame_handler


def msg_handler(frame, name_prepends, num_tabs, fw):
    tot_name = name_prepends + '_' + frame.name
    fw(
        '\t' * num_tabs + 'case CANlib_{}_key:\n'.format(tot_name) +
        '\t' * (num_tabs + 1) + 'handle_{}_msg(&frame);\n'.format(tot_name) +
        '\t' * (num_tabs + 1) + 'break;\n'
    )


def write(can, computers, output_path=computer_c_dir_path):
    os.makedirs(output_path, exist_ok=True)

    raw_buses = set()

    for computer in computers:
        if not ('can' in computer.participation['name'].keys()):
            # This computer neither sends nor recieves can messagess
            continue

        raw_buses |= set(computer.participation['name']['can'].mapping.values())

    for computer in computers:
        f_path = os.path.join(output_path, 'canlib_{}.c'.format(computer.name))

        if not ('can' in computer.participation['name'].keys()):
            # This computer neither sends nor recieves can messagess
            continue

        with open(f_path, 'w') as f:
            fw = f.write
            fw('#include "pack_unpack.h"\n')
            fw('#include "canlib_{}.h"\n\n'.format(computer.name))

            fw(
              'CAN_Raw_Bus_T CANlib_GetRawBus(CANlib_Bus_T bus) {\n'
              '\tswitch (bus) {\n'
            )

            for busnm, rawnm in computer.participation['name']['can'].mapping.items():
                fw('\t\tcase {}:\n'.format(busnm) + '\t\t\treturn {};\n'.format(rawnm))

            fw('\t}\n}\n\n')

            for busnm, bus in computer.participation['name']['can'].subscribe.items():
                fw(
                    'static void update_can_{}(void)'.format(busnm) + '{\n' +
                    '\tFrame frame;\n' +
                    '\tCANlib_ReadFrame(&frame, {});\n'.format(busnm) +
                    '\tswitch(frame.id) {\n'
                )

                for msg in bus:
                    frame_handler(msg, busnm, msg_handler, 2, fw)

                fw(
                    '\t}\n}\n\n'
                )

            fw('void update_can() {\n')
            for busnm in computer.participation['name']['can'].subscribe.keys():
                fw('\tupdate_can_{}();\n'.format(busnm))
            fw('}\n\n')
