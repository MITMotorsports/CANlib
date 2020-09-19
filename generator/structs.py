import sys
sys.path.append('ParseCAN')
import ParseCAN
from common import structs_path


def write(env, output_path=structs_path):
    template = env.get_template("structs.h.j2")
    with open(output_path, 'w') as f:
        f.write(template.render())
