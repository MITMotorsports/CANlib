import sys
sys.path.append('ParseCAN')
import ParseCAN
from common import can_lib_h_path, templ, coord, ifndef, endif, frame_handler

def write(env, output_path=can_lib_h_path):
    template = env.get_template("pack_unpack.h.j2")
    with open(output_path, 'w') as f:
        f.write(template.render())
