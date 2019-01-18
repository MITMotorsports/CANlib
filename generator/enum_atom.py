'''
Generate enum.h file.
Run this file (with the spec path as a command line argument) to write just
ids.h or main.py to write all files.
'''
import sys
sys.path.append("ParseCAN")
import ParseCAN
from common import enum_atom_path, templ, coord, ifndef, endif


def write(can, output_path=enum_atom_path):
    '''
    Generate enum_atom.h file, which includes enums for atom of type enum.

    :param output_path: file to be written to
    :param spec_path: CAN spec path
    '''

    header_name = '_CAN_LIBRARY_ENUM_ATOM_H'

    with open(output_path, 'w') as f:
        fw = f.write

        fw(ifndef(header_name))
        fw('#include "constants.h"\n\n')

        for bus in can.bus:
            for msg in bus.frame:
                if isinstance(msg, ParseCAN.spec.bus.MultiplexedFrame):
                    for frame in msg.frame:
                        for atom in frame.atom:
                            if atom.type.isenum():
                                # Only C++11 feature
                                # fw('typedef enum ' + (atom.type.type + ' ' if atom.type.type else '') + '{\n')

                                fw('typedef enum {\n')

                                for enum in atom.type.enum:
                                    fw(templ['enum'].format(
                                        coord(bus.name, msg.name, atom.name, enum.name),
                                        enum.value
                                    ))

                                fw('} ' + '{}_T;\n\n'.format(coord(bus.name, msg.name, atom.name)))

                else:
                    for atom in msg.atom:
                        if atom.type.isenum():
                            # Only C++11 feature
                            # fw('typedef enum ' + (atom.type.type + ' ' if atom.type.type else '') + '{\n')

                            fw('typedef enum {\n')

                            for enum in atom.type.enum:
                                fw(templ['enum'].format(
                                    coord(bus.name, msg.name, atom.name, enum.name),
                                    enum.value
                                ))

                            fw('} ' + '{}_T;\n\n'.format(coord(bus.name, msg.name, atom.name)))

        fw(endif(header_name))
