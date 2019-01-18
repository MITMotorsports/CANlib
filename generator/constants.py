import sys
sys.path.append("ParseCAN")
import ParseCAN
from common import constants_path, coord, templ, ifndef, endif, is_multplxd, frame_handler
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

                # Define it in scope because it relies on too many locals
                def msg_handler(frame, name_prepends):
                    attr = None
                    try:
                        attr = getattr(frame, attrnm)
                    except AttributeError as e:
                        if not attrnm in optional_props:
                            raise e
                    if attr is None:
                        if attrnm in optional_props:
                            return
                        else:
                            raise AttributeError('{} missing required attribute {}'.format(frame.name, attrnm))
                    attr = transform(attr)
                    fw(templ[form].format(coord(name_prepends, frame.name, finalnm), attr))

                for msg in bus.frame:
                    if is_multplxd(msg):
                        # Keep multiplexed check because we want to call for
                        # this function for both the top lvl multiplexed msg
                        # and the sub-frames
                        frame_handler(msg, bus.name, msg_handler)
                    msg_handler(msg, bus.name)

        fw(endif(header_name))

