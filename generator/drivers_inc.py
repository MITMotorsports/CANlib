from pathlib import Path


def find_architecture(system, template):
    interest = system.architecture['name']
    if template in interest:
        return interest[template]

    for architecture in system.architecture:
        if architecture.family == template:
            return architecture

    return None


def write(env, system, input_path, output_path, testing = False):
    input_path = Path(input_path)
    output_path = Path(output_path)
    output_path.mkdir(parents=True, exist_ok=True)

    for template_path in input_path.glob('*'):
        architecture_name = template_path.stem.strip('.hpp')
        architecture = find_architecture(system, architecture_name)
        output_file = output_path.joinpath(template_path.stem)

        template = env.get_template(str(template_path))
        if (testing == (architecture_name == 'testfamily')):
            with open(output_file, 'w') as f:
                f.write(template.render(architecture=architecture))
