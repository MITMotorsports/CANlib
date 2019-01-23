import sys
sys.path.append("ParseCAN")
import ParseCAN
from common import bus_path, coord, templ, ifndef, endif

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

        raw_buses = set()
        for computer in computers:
            if not ('can' in computer.participation['name'].keys()):
                # This computer neither sends nor recieves can messages
                continue

            raw_buses |= set(computer.participation['name']['can'].mapping.values())

        fw('typedef enum {\n')
        for bus in raw_buses:
          fw('\t' + bus + ',\n')
        fw('} CAN_Raw_Bus_T;\n\n')

        fw(endif(header_name))


