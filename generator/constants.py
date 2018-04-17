'''
Generate constants.h file.
Run this file (with the spec path as a command line argument) to write just
constants.h or main.py to write all files.
'''
import sys
sys.path.append("ParseCAN")
import ParseCAN
from common import constants_path, coord, templ, ifndef, endif


def write(car, output_path=constants_path):
    '''
    Generate constants.h file, which has CAN IDs and enum values.

    :param output_path: file to be written to
    :param car: CAN spec
    '''
    header_name = '_CAN_LIBRARY_CONSTANTS_H'

    with open(output_path, 'w') as f:
        fw = f.write

        fw(ifndef(header_name))

        props = (
            ('can_id', 'define', int),
            ('period', 'define', lambda x: int(ParseCAN.parse.number_in('ms')(x))),
        )

        for bus in car.buses:
            for attrnm, form, transform in props:
                if form == 'enum':
                    fw('typedef enum {\n')

                for msg in bus.messages:
                    try:
                        attr = transform(getattr(msg, attrnm))
                        fw(templ[form].format(coord(bus.name, msg.name, attrnm), attr))
                    except AttributeError:
                        pass

                if form == 'enum':
                    fw('} ' + '{}_T;'.format(coord(bus.name, attrnm)))

                f.write('\n\n')

        fw(endif(header_name))

if __name__ == '__main__':
    spec_path = sys.argv[1]
    car = ParseCAN.spec.car(spec_path)
    write(car)
