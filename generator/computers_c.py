import os


def write(env, computers, input_path, output_path):
    os.makedirs(output_path, exist_ok=True)
    template = env.get_template(str(input_path))
    for computer in computers:
        f_path = os.path.join(output_path, computer.name)
        os.makedirs(f_path, exist_ok=True)
        f_path = os.path.join(f_path, 'canlib_{}.cpp'.format(computer.name))
        with open(f_path, 'w') as f:
            f.write(template.render(computer=computer))
