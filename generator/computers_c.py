import sys
sys.path.append("ParseCAN")
import os
import ParseCAN
from common import computer_c_dir_path, coord, templ, ifndef, endif, \
    frame_handler, is_multplxd
from math import ceil


def single_handler(frame, name_prepends, num_tabs, fw):
    tot_name = coord(name_prepends, frame.name, prefix=False)
    fw(
        '\t' * num_tabs + 'case CANlib_{}_key:'.format(tot_name) + '\n' +
        '\t' * (num_tabs + 1) + 'CANlib_Handle_{}(frame);\n'.format(tot_name) +
        '\t' * (num_tabs + 1) + 'break;\n'
    )


def multplxd_handler(frame, name_prepends, num_tabs, fw):
    fw('\t' * num_tabs + 'case {}_key:\n'.format(coord(name_prepends, frame.name)))
    key_size = ceil(frame.slice.length / 8) * 8
    key_name = '_'.join([name_prepends,frame.name, 'key'])
    fw('\t' * (num_tabs + 1) + 'to_bitstring(frame->data, &bitstring);' '\n')
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

            fw('\t\tdefault:\n\t\t\treturn INVALID_BUS;\n')

            fw('\t}\n}\n\n')

            fw(
              'CAN_Raw_Bus_T CANlib_GetConceptualBus(CAN_Raw_Bus_T bus) {\n'
              '\tswitch (bus) {\n'
            )

            for busnm, rawnm in computer.participation['name']['can'].mapping.items():
                fw('\t\tcase {}:\n'.format(rawnm) + '\t\t\treturn {};\n'.format(busnm))

            fw('\t\tdefault:\n\t\t\treturn INVALID_BUS;\n')

            fw('\t}\n}\n\n')

            for busnm, bus in computer.participation['name']['can'].subscribe.items():
                fw(
                    'static void CANlib_HandleFrame_{}(Frame* frame)'.format(busnm) + '{\n' +
                        '\tswitch(frame->id) {\n'
                )

                for msg in bus:
                    msg_handler(msg, busnm, fw)

                fw('\t\tdefault:\n\t\t\treturn;\n')
                fw('\t}\n')

                fw(
                    '}\n\n'
                )

            for busnm, bus in computer.participation['name']['can'].subscribe.items():
                fw(
                    'static void CANlib_update_can_{}(void)'.format(busnm) + '{\n' +
                    '\tFrame frame;\n'
                )
                if any(is_multplxd(msg) for msg in bus):
                    fw('\tuint64_t bitstring;\n')
                fw(
                    '\tCANlib_ReadFrame(&frame, {});\n'.format(busnm) +
                    '\tCANlib_HandleFrame_{}(&frame);\n'.format(busnm) +
                    '}\n\n'
                )

            fw('void CANlib_update_can() {\n')
            for busnm in computer.participation['name']['can'].subscribe.keys():
                fw('\tCANlib_update_can_{}();\n'.format(busnm))
            fw('}\n\n')

            fw('void CANlib_HandleFrame(CAN_Raw_Bus_T raw_bus, Frame* frame) {\n')
            fw('\tswitch(raw_bus) {\n')
            for bus in computer.participation['name']['can'].subscribe.keys():
                fw(
                    '\t\tcase {}:\n'.format(computer.participation['name']['can'].mapping[bus]) +
                    '\t\t\tCANlib_HandleFrame_{}(frame);\n'.format(bus) +
                    '\t\t\tbreak;\n'
                )
            fw('\t\tdefault:\n\t\t\tbreak;\n')
            fw('\t}\n}\n')
