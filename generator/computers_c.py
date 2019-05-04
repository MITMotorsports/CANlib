import sys
sys.path.append("ParseCAN")
import os
import ParseCAN
from common import computer_c_dir_path, coord, templ, ifndef, endif, \
    frame_handler, is_multplxd
from math import ceil

raw_bus_to_handle = {"CAN_1": "hcan1", "CAN_2": "hcan2", "CAN_3": "hcan3"}
raw_bus_to_instance = {"CAN_1": "CAN1", "CAN_2": "CAN2", "CAN_3": "CAN3"}

def single_handler(frame, name_prepends, num_tabs, fw):
    tot_name = coord(name_prepends, frame.name, prefix=False)
    fw(
        '\t' * num_tabs + 'case CANlib_{}_key:'.format(tot_name) + '\n' +
        '\t' * (num_tabs + 1) + 'CANlib_Handle_{}(ts_frame);\n'.format(tot_name) +
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
            fw('#include <time.h>\n')
            fw('#include <stdbool.h>\n')
            fw('#include <stm32f4xx_hal.h>\n')
            fw('#include "pack_unpack.h"\n')
            fw('#include "canlib_{}.h"\n\n'.format(computer.name))

            fw('extern CAN_HandleTypeDef hcan1;\n')
            fw('extern CAN_HandleTypeDef hcan2;\n')
            fw('extern CAN_HandleTypeDef hcan3;\n\n')

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
                    'static void CANlib_HandleFrame_{}(TimestampedFrame* ts_frame)'.format(busnm) + '{\n' +
                        '\tswitch(ts_frame->frame.id) {\n'
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
                    'static void CANlib_Update_can_{}(void)'.format(busnm) + ' {\n' +
                    '\tTimestampedFrame ts_frame;\n'
                )
                if any(is_multplxd(msg) for msg in bus):
                    fw('\tuint64_t bitstring;\n')

                raw_bus = computer.participation['name']['can'].mapping[busnm]
                instance = raw_bus_to_handle[raw_bus]
                fw(
                    '\tif (HAL_CANlib_ReadFrame(&{}, &(ts_frame.frame))) {{\n'.format(instance) +
                    '\t\tts_frame.stamp = HAL_GetTick();\n' +
                    '\t\tCANlib_HandleFrame_{}(&ts_frame);\n'.format(busnm) +
                    '\t}\n' +
                    '}\n\n'
                )

            fw('void CANlib_update_can() {\n')
            for busnm in computer.participation['name']['can'].subscribe.keys():
                fw('\tCANlib_Update_can_{}();\n'.format(busnm))
            fw('}\n\n')

            fw('void CANlib_HandleFrame(TimestampedFrame *ts_frame, time_t stamp, CAN_TypeDef* instance) {\n')
            if len(computer.participation['name']['can'].subscribe.keys()) > 0: # check if computer receives messages
                fw('\tts_frame->stamp = stamp;\n')
                fw('\tswitch ((intptr_t)instance) {\n')
                for bus in computer.participation['name']['can'].subscribe.keys():
                    fw(
                        '\t\tcase (intptr_t){}:\n'.format(raw_bus_to_instance[computer.participation['name']['can'].mapping[
                            bus]]) +
                        '\t\t\tCANlib_HandleFrame_{}(ts_frame);\n'.format(bus) +
                        '\t\t\tbreak;\n'
                    )
                fw('\t\tdefault:\n')
                fw('\t\t\tbreak;\n')
                fw('\t}\n')
            else: # prevent unused warning
                fw('\tUNUSED(ts_frame);\n')
                fw('\tUNUSED(stamp);\n')
                fw('\tUNUSED(raw_bus);\n')
            fw('}\n')
