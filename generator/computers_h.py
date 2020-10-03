import sys
sys.path.append("ParseCAN")
import os
import ParseCAN
from common import computer_h_dir_path


def write(env, computers, output_path=computer_h_dir_path):
    os.makedirs(output_path, exist_ok=True)
    template = env.get_template("computer.h.j2")

    for computer in computers:
        f_path = os.path.join(output_path, 'canlib_{}.h'.format(computer.name))
        with open(f_path, 'w') as f:
            f.write(template.render(computer=computer))
