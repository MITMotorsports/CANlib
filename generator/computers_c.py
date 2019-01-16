'''
Generate all of the headers in the computers directory.
Run this file (with the spec path as a command line argument) to write just
these files or main.py to write all files.
'''
import sys
sys.path.append("ParseCAN")
import os
import ParseCAN
from common import computer_c_dir_path, coord, templ, ifndef, endif, is_multplxd

def handle_frame(frame, bus_name, fw):
    fw(coor())

def write(can, computers, output_path=computer_c_dir_path):
    '''
    Generate computer c files

    :param output_path: file to be written to
    :param can: CAN spec
    '''

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
            fw('#include "CANlib.h"\n\n')
            fw(
              'CAN_Raw_Bus_T CANlib_get_raw_bus(CANlib_Bus_T bus) {\n'
              '\tswitch (bus) {\n'
            )

            for busnm, rawnm in computer.participation['name']['can'].mapping.items():
              fw('\t\tcase {}:\n'.format(busnm) + '\t\t\treturn {};\n'.format(rawnm))

            fw('\t}\n}\n\n')

            for busnm in computer.participation['name']['can'].subscribe.keys():
                fw(
                    'static void update_can_{}(void)'.format(busnm) + '{\n' +
                    '\tFrame frame;\n' +
                    #'\tCAN_Raw_Bus_T raw_bus = CANlib_get_raw_bus({})\n'.format(busnm) +
                    '\tCANlib_ReadFrame(&frame, {});\n'.format(busnm) +
                    #'\tswitch(raw_bus) {\n'
                    '\tCANlib_{}_T msgForm = CANlib_Identify_{}(&frame);\n\n'.format(busnm, busnm) +
                    '\tswitch(msgForm) {\n'
                )
                # for raw_bus in raw_buses:
                #     fw(
                #         '\t\tcase {}:\n'.format(raw_bus) +
                #         '\t\t\tCANlib_ReadFrame(&frame, {});\n'.format(raw)
                #     )
                fw(
                    '}\n\n'
                )

            fw('void update_can() {\n')
            for busnm in computer.participation['name']['can'].subscribe.keys():
                fw('\tupdate_can_{}();\n'.format(busnm))
            fw('}\n\n')
