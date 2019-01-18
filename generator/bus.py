import sys
sys.path.append("ParseCAN")
import ParseCAN
from common import bus_path, coord, templ, ifndef, endif
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


def write(can, computers, output_path=bus_path):
    header_name = '_CAN_LIBRARY_BUS_H'

    with open(output_path, 'w') as f:
        fw = f.write

        fw(ifndef(header_name))

        # Create enum among buses
        fw('typedef enum {\n')
        for bus in can.bus:
          fw('\t' + bus.name + ',\n')
        fw('} CANlib_Bus_T;\n\n')

        fw(endif(header_name))


