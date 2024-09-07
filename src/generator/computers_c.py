import os


def write(env, computers, input_path, output_path):
    output_path.mkdir(parents=True, exist_ok=True)
    template = env.get_template(input_path.name)
    for computer in computers:
        f_path = os.path.join(output_path, 'canlib_{}.c'.format(computer.name))
        with open(f_path, 'w') as f:
            f.write(template.render(computer=computer))
