'''
Generate constants.h file.
Run this file (with the spec path as a command line argument) to write just
constants.h or main.py to write all files.
'''
import sys
sys.path.append("ParseCAN")
import ParseCAN
from common import constants_path, coord, templ, ifndef, endif


def write(can, output_path=constants_path):
    '''
    Generate constants.h file, which has CAN IDs and enum values.

    :param output_path: file to be written to
    :param can: CAN spec
    '''
    header_name = '_CAN_LIBRARY_CONSTANTS_H'

    with open(output_path, 'w') as f:
        fw = f.write

        fw(ifndef(header_name))

        props = (
            (('key', 'id'), 'define', int),
            # TODO(nistath): Add this back.
            # ('period', 'define', lambda x: int(ParseCAN.parse.number_in('ms')(x))),
        )

        for bus in can.bus:
            for attrnm, form, transform in props:
                finalnm = attrnm
                if isinstance(attrnm, tuple):
                    attrnm, finalnm = attrnm

                if form == 'enum':
                    fw('typedef enum {\n')

                for frame in bus.frame:
                    try:
                        attr = transform(getattr(frame, attrnm))
                        fw(templ[form].format(coord(bus.name, frame.name, finalnm), attr))
                    except AttributeError:
                        pass

                if form == 'enum':
                    fw('} ' + '{}_T;'.format(coord(bus.name, attrnm)))

                f.write('\n\n')

        fw(endif(header_name))

