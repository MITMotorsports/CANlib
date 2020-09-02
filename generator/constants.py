import sys
sys.path.append("ParseCAN")
import ParseCAN
from common import constants_path
from pint import UnitRegistry
import jinja2


def get_ms(period_str):
    if type(period_str) is int:
        # If it's set as an integer, assume ms
        return period_str

    ur = UnitRegistry()
    return ur[period_str].to('ms').magnitude


def write(env, can, output_path=constants_path):
    template = env.get_template("constants.h.j2")
    with open(output_path, 'w') as f:
        f.write(template.render(can=can, get_ms=get_ms))
