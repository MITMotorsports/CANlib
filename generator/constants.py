'''
Generate constants.h file.
Run this file (with the spec path as a command line argument) to write just
constants.h or main.py to write all files.
'''
import sys
sys.path.append("ParseCAN")
import ParseCAN
from common import constants_path, coord, templ, ifndef, endif, is_multplxd
from pint import UnitRegistry as UR


def get_ms(period_str):
    if type(period_str) is int:
        # If it's set as an integer, assume ms
        return period_str

    ur = UR()
    t = int(''.join([s for s in period_str if s.isdigit()]))
    units = ''.join([s for s in period_str if s.isalpha()])
    units = ur[units]
    t = t * units
    return t.to('ms').magnitude


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
            ('key', 'define', int),
            ('period', 'define', get_ms),
        )

        optional_props = {'period'}

        for bus in can.bus:
            for attrnm, form, transform in props:
                finalnm = attrnm
                for msg in bus.frame:
                    if is_multplxd(msg):
                        for frame in msg.frame:
                            try:
                                attr = getattr(msg, attrnm)
                            except AttributeError as e:
                                if not attrnm in optional_props:
                                    raise e
                            if attr is None:
                                if attrnm in optional_props:
                                    continue
                                else:
                                    raise AttributeError('{} missing required attribute {}'.format(msg.name, attrnm))
                            attr = transform(attr)
                            fw(templ[form].format(coord(bus.name, msg.name, frame.name, finalnm), attr))
                    try:
                        attr = getattr(msg, attrnm)
                    except AttributeError as e:
                        if not attrnm in optional_props:
                            raise e
                    if attr is None:
                        if attrnm in optional_props:
                            continue
                        else:
                            raise AttributeError('{} missing required attribute {}'.format(msg.name, attrnm))
                    attr = transform(attr)
                    fw(templ[form].format(coord(bus.name, msg.name, finalnm), attr))

        fw(endif(header_name))

