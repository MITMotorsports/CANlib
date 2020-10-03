import sys
sys.path.append("ParseCAN")
import ParseCAN
from common import bus_path


def write(env, output_path=bus_path):
    template = env.get_template("bus.h.j2")
    with open(output_path, 'w') as f:
        f.write(template.render())
