import sys
sys.path.append('ParseCAN')
import ParseCAN

src_dir = '../src/'
constants_path = f'{src_dir}constants.h'
pack_unpack_c_path = f'{src_dir}pack_unpack.c'
can_lib_h_path = f'{src_dir}pack_unpack.h'
enum_atom_path = f'{src_dir}enum_atom.h'
bus_path = f'{src_dir}bus.h'
structs_path = f'{src_dir}structs.h'
computer_h_dir_path = f'{src_dir}computers/inc'
computer_c_dir_path = f'{src_dir}computers/src'
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


def is_multplxd(frame):
    return isinstance(frame, ParseCAN.spec.bus.MultiplexedFrame)


def frame_handler(frame, name_prepends, func, *args):
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
