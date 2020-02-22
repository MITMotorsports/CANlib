import sys
sys.path.append("ParseCAN")
import ParseCAN
from common import drivers_inc_template_dir_path, drivers_inc_dir_path
from pathlib import Path

def find_architecture(system, template):
    interest = system.architecture['name']
    if template in interest:
        return interest[template]

    for architecture in system.architecture:
        if architecture.family == template:
            return architecture

    return None


def write(system, input_path=drivers_inc_template_dir_path, output_path=drivers_inc_dir_path):
    input_path = Path(input_path)
    output_path = Path(output_path)
    output_path.mkdir(parents=True, exist_ok=True)

    TARGET_STR = '// BUILD CAN_Raw_Bus_T'

    for template_path in input_path.glob('*'):
        architecture_name = template_path.stem
        architecture = find_architecture(system, architecture_name)
        template = template_path.open('r').read()
        idx = template.find(TARGET_STR)

        if architecture is None:
            if idx != -1:
                raise ValueError('spec given has no information about architecture {}'.format(architecture_name))
        else:
            raw_buses = architecture.participation['name']['can'].buses
            assert 'INVALID_BUS' not in raw_buses, "INVALID_BUS is a reserved bus name"

            code = ''
            code += 'typedef enum {\n'
            for bus in raw_buses:
                code += '\t' + bus + ',\n'
            code += '\tINVALID_BUS\n} CAN_Raw_Bus_T;\n\n'

            template = template.replace(TARGET_STR, code)

        output_path.joinpath(template_path.name).open('w').write(template)


