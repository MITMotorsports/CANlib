import sys
sys.path.append("ParseCAN")
import ParseCAN
from common import bus_path, coord, templ, ifndef, endif, is_multplxd

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

        assert 'INVALID_BUS' not in raw_buses, "INVALID_BUS is a reserved bus name"

        fw('typedef enum {\n')
        for bus in raw_buses:
          fw('\t' + bus + ',\n')
        fw('\tINVALID_BUS\n} CAN_Raw_Bus_T;\n\n')

        # Create forms enum for each bus
        for bus in can.bus:
            fw('typedef enum {\n')

            # First frame needs to start at 2, the rest will follow
            first_frame = True
            for msg in bus.frame:
                if is_multplxd(msg):
                    for frame in msg.frame:
                        fw('\t' + coord(bus.name, msg.name, frame.name))
                        if first_frame:
                            fw(' = 2')
                        first_frame = False
                        fw(',\n')
                else:
                    fw('\t' + coord(bus.name, msg.name))
                    if first_frame:
                        fw(' = 2')
                    first_frame = False
                    fw(',\n')

            fw('} ' + '{}_T;\n\n'.format(coord(bus.name)))

            fw('{}_T CANlib_Identify_{}(Frame* frame);'.format(coord(bus.name), coord(bus.name, prefix=False)) + '\n\n')

        fw(endif(header_name))


