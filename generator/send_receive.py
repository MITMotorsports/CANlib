import sys
sys.path.append("ParseCAN")
import ParseCAN
from common import send_recieve_path

def write(env, output_path=send_recieve_path):
    template = env.get_template("send_receive.c.j2")
    with open(output_path, 'w') as f:
        f.write(template.render())
