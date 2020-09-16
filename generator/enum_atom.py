import sys
sys.path.append("ParseCAN")
import ParseCAN
from common import enum_atom_path


def write(env, output_path=enum_atom_path):
    template = env.get_template("enum_atom.h.j2")
    with open(output_path, 'w') as f:
        f.write(template.render())
