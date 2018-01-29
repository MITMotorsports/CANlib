'''
Generate Can_Libary.c file.
Run this file (with the spec path as a command line argument) to write just
Can_Libary.c or main.py to write all files.
'''
import sys
sys.path.append('ParseCAN')
import ParseCAN
from math import ceil, floor, log2
from common import can_lib_c_path, can_lib_c_base_path, coord


def write(car, output_path=can_lib_c_path, base_path=can_lib_c_base_path):
    '''
    Generate Can_Libary.c file.

    :param output_path: file to be written to
    :param car: CAN spec
    :param base_path: File with template code that's not autogenerated
    '''
    with open(output_path, 'w') as f:
        fw = f.write

        fw('#include "Can_Library.h"\n')

        # Copy over base
        with open(base_path) as base:
            lines = base.readlines()
            f.writelines(lines)
        fw('\n')

        # Write init functions
        for board in car.boards:
            if board.arch:  # Means it's a board we program
                for bus in board.subscribe:
                    fw('void {}_INIT(uint32_t baudrate) '.format(coord(bus.name, board.name)))
                    fw(
                        '{' '\n'
                        '\t' 'Can_Init(baudrate);' '\n'
                    )

                    max_id = max(msg.can_id for msg in bus.messages)

                    # Find mask
                    # The way hardware filtering works is that incoming IDs are
                    # ANDed with the mask and then compared with a preset ID
                    # The goal of this mask is to throw away most higher ID (i.e.,
                    # lower priority) messages
                    mask = 2**(floor(log2(max_id)) + 1) - 1
                    # On a standard bus, IDs are 11 bit
                    max_possible_id = 2**11-1
                    if bus.is_extended:
                        # On an extended bus, IDs are 29 bit
                        max_possible_id = 2**29-1
                    mask = mask ^ max_possible_id

                    # Set mask (pass in binary to make file more readable)
                    fw('\t' 'Can_SetFilter(' + bin(mask) + ', 0);' '\n')

                    # Finish up
                    fw('}' '\n\n')

                # We still need to create init functions for boards that publish
                # on a bus but don't subscribe
                # Filtering doesn't matter here
                for bus in board.publish:
                    if bus.name not in board.subscribe.name:
                        fw(
                            'void ' + coord(bus.name, board.name) +
                            '_INIT(uint32_t baudrate) {' '\n'
                        )
                        fw('\t' 'Can_Init(baudrate);' '\n')
                        fw('}' '\n\n')

        # # Write switch statement
        # fw(
        #     'Can_MsgID_T Can_MsgType(void) {' '\n'
        #     '\t' 'switch(id) {' '\n'
        # )
        # for bus in car.buses:
        #     for msg in bus.messages:
        #         fw(
        #             '\t' 'case {}_ID:\n'.format('asd') +  # FIX
        #             '\t' '\t' 'return Can_' + msg.name + '_Msg;\n'
        #         )
        #
        # fw(
        #     '\t' 'default:' '\n'
        #     '\t' '\t' 'return Can_Unknown_Msg;' '\n'
        #     '\t' '}' '\n'
        #     '}' '\n\n'
        # )

        for bus in car.buses:
            for msg in bus.messages:

                # Write CAN_PACK
                fw(
                    'CAN_PACK(' + coord(bus.name, msg.name) + ') {' '\n'
                    '\t' 'uint64_t bitstring = 0;' '\n'
                )

                for seg in msg.segments:
                    if not msg.is_big_endian and (seg.c_type.startswith('int') or
                                                  seg.c_type.startswith('uint')):
                        fw(
                            '\t' + seg.c_type + ' ' + seg.name + '_swap_value = swap_' + seg.c_type[:-2] + '(type_in->' + seg.name + ');' '\n'
                            '\t' 'bitstring = INSERT(' + seg.name + '_swap_value, bitstring, ' + str(seg.position) + ', ' + str(seg.length) + ');' '\n\n'
                        )
                    else:
                        fw(
                            '\t' 'bitstring = INSERT(type_in->' + seg.name + ', bitstring, ' + str(seg.position) +
                            ', ' + str(seg.length) + ');' '\n'
                        )

                length = max(seg.position + seg.length for seg in msg.segments)

                fw(
                    '\t' 'from_bitstring(&bitstring, can_out->data);' '\n'
                    '\t' 'can_out->id = ' + msg.name.upper() + '__id;' '\n'
                    '\t' 'can_out->len = ' + str(ceil(length / 8)) + ';' '\n'
                    '}' '\n\n'
                )

                # Write CAN_UNPACK
                fw(
                    'CAN_UNPACK(Can_' + msg.name + ') {' '\n'
                    '\t' 'uint64_t bitstring = 0;' '\n'
                    '\t' 'to_bitstring(can_in->data, &bitstring);\n'
                )

                for seg in msg.segments:
                    if seg.c_type == 'enum':
                        enum_name = 'Can_' + msg.name + 'ID_T'

                        fw(
                            '\t' 'type_out->' + seg.name + ' = (' + enum_name + ')EXTRACT(bitstring, ' +
                            str(seg.position) + ', ' + str(seg.length) + ');' '\n'
                        )
                    elif seg.c_type == 'bool':
                        fw(
                            '\t' 'type_out->' + seg.name + ' = EXTRACT(bitstring, ' + str(seg.position) + ', ' +
                            str(seg.length) + ');' '\n'
                        )
                    else:
                        if not msg.is_big_endian:
                            if seg.signed:
                                fw(
                                    '\t' + seg.c_type + ' ' + seg.name + '_swap_value=(swap_' +
                                    seg.c_type[:-2] + '(EXTRACT(bitstring, ' + str(seg.position) + ', ' +
                                    str(seg.length) + ')));' '\n'
                                )
                            else:
                                fw(
                                    '\t' + seg.c_type + ' ' + seg.name + '_swap_value=swap_u' +
                                    seg.c_type[:-2] + '(EXTRACT(bitstring, ' + str(seg.position) + ', ' +
                                    str(seg.length) + ')));' '\n'
                                )

                            fw('\t' 'type_out->' + seg.name + ' = ' + seg.name + '_swap_value;' '\n')
                        else:
                            if seg.signed:
                                fw(
                                    '\t' 'type_out->' + seg.name + ' = SIGN(EXTRACT(bitstring, ' +
                                    str(seg.position) + ', ' + str(seg.length) + '), ' +
                                    str(seg.length) + ');' '\n'
                                )
                            else:
                                fw(
                                    '\t' 'type_out->' + seg.name + ' = EXTRACT(bitstring, ' +
                                    str(seg.position) + ', ' + str(seg.length) + ');' '\n'
                                )

                fw('}' '\n\n')

        # Write DEFINE statements
        for bus in car.buses:
            for msg in bus.messages:
                fw('DEFINE(' + coord(bus.name, msg.name) + ')\n')


if __name__ == '__main__':
    spec_path = sys.argv[1]
    car = ParseCAN.spec.car(spec_path)
    write(car)
