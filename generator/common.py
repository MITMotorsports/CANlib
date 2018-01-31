constants_path = '../constants.h'
can_lib_c_path = '../Can_Library.c'
can_lib_h_path = '../Can_Library.h'
enum_segments_path = '../enum_segments.h'
structs_path = '../structs.h'
can_lib_c_base_path = 'templates/Can_Library_BASE.c'


def coord(*args):
    '''
    Returns the proper format for global names.

    `coordinate('bus0', 'temp') === 'bus0_temp'`
    '''
    if len(args) == 0:
        return ''

    return ('_'.join(args))


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


'''A template dict to define assignment within a `key`.'''
templ = {
    'enum': '\t{} = {},\n',
    'define': '#define {} {}\n',
}
