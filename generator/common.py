import sys
sys.path.append('ParseCAN')
import ParseCAN

src_dir = '../src/'
constants_path = f'{src_dir}constants.h'
pack_unpack_c_path = f'{src_dir}pack_unpack.c'
can_lib_h_path = f'{src_dir}pack_unpack.h'
enum_atom_path = f'{src_dir}enum_atom.h'
bus_path = f'{src_dir}bus.h'
drivers_inc_dir_path = f'{src_dir}/drivers/inc'
drivers_inc_template_dir_path = f'templates/drivers/inc'
structs_path = f'{src_dir}structs.h'
computer_h_dir_path = f'{src_dir}computers/inc'
computer_c_dir_path = f'{src_dir}computers/src'
test_h_dir_path = f'{src_dir}test/inc'
test_c_dir_path = f'{src_dir}test/src'
send_recieve_path = f'{src_dir}send_receive.c'
pack_unpack_c_base_path = 'templates/pack_unpack_c.template'


def coord(*args, prefix=True):
    '''
    Returns the proper format for global names.
    '''

    if prefix:
        args = ('CANlib', ) + args

    return '_'.join(args)


def ifndef(name):
    '''
    Return a string for beginning the
    #ifndef ...
    #define ...
    <code>
    #endif // ... procedure.
    '''
    return '#ifndef {0}\n#define {0}\n\n'.format(name)


def endif(name):
    '''
    Return a string for ending the
    #ifndef ...
    #define ...
    <code>
    #endif // ... procedure.
    '''
    return '\n#endif // {0}\n'.format(name)


def switch_case(switch, cases, default=None, indentation=0):
    '''
    Creates a switch-case block, where cases is a list of tuples.
    '''
    tabs = '\t'*indentation
    result = '{0}switch({1}) {{\n'.format(tabs, switch)
    for case, action in cases:
        result += '\t{0}case {1}:\n'.format(tabs, case)
        result += '\t\t{0}{1}\n'.format(tabs, action)
        result += '\t\t{}break;\n'.format(tabs)
    result += '\t{}default:\n'.format(tabs)
    if default is not None:
        result += '\t\t{0}{1}\n'.format(tabs, default)
    result += '\t\t{}break;\n'.format(tabs)
    result += tabs + '}'
    return result


def is_multplxd(frame):
    return isinstance(frame, ParseCAN.spec.bus.MultiplexedFrame)


def frame_handler(frame, name_prepends, func, *args):
    '''
    Call func on frame if frame is single, otherwise call frame handler on its
    sub frames (while properly handling names)
    '''
    if is_multplxd(frame):
        for sub_frame in frame.frame:
            frame_handler(sub_frame, name_prepends + '_' + frame.name, func, *args)
    else:
        func(frame, name_prepends, *args)


'''A template dict to define assignment within a `key`.'''
templ = {
    'enum': '\t{} = {},\n',
    'define': '#define {} {}\n',
}
