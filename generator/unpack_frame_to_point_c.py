import os


def write(env, computers, input_path, output_path):
    os.makedirs(output_path, exist_ok=True)
    template = env.get_template(str(input_path))
    [computer] = [computer for computer in computers if computer.name == "telemetry"]
    f_path = os.path.join(output_path, computer.name)
    os.makedirs(f_path, exist_ok=True)
    f_path = os.path.join(f_path, 'canlib_unpack_frame_to_point_{}.cpp'.format(computer.name))
    with open(f_path, 'w') as f:
        f.write(template.render(computer=computer))