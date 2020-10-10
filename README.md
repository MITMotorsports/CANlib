# CANlib

## Installation
If you're cloning this for the first time, use `git clone --recursive` to pull all submodules. However, if you already have this repository cloned non-recursively, make sure to run `git submodule init` and `git submodule update`.

To generate this library, run `main.py` in the `generator` directory with the path to your CAN spec as an argument (e.g. `python main.py ../../../can_spec_my18.yml` if you're running this from the `lib` directory in MY18). Make sure to use Python 3. This generates all of the necessary source files that are listed in `.gitignore`. These files are dependent on the CAN spec. If you want to change one of these files, don't edit them directly, but instead figure out what needs to change in the CAN spec. (If that still doesn't work, edit the generator scripts in `generator` rather than the generated files.) Source files that aren't ignored aren't generated from the CAN spec, so they can be edited directly.

## Writing a CAN spec
The CAN spec should be a YAML file with the following structure:
```yaml
name: MYwhatever # Name of your CAN spec
units: # Any custom units you need to define
  - newtonmeter = N * m = J = Nm
architecture:
  arch1:
    family: archx
    participation:
      can:
        buses:
          - hardwareBus1
          - hardwareBus2
          - hardwareBus3
  arch2:
    family: archx
    participation:
      can:
        buses:
          - hardwareBus1
          - hardwareBus2
computer: # All boards on the car, along with CAN messages they publish and subscribe to
  board1:
    architecture: arch1
    participation:
      can:
        mapping:
          symbolicBus0: hardwareBus2
          symbolicBus1: hardwareBus3
        publish:
          symbolicBus0:
            - Msg0
          symbolicBus1:
            - Msg0
        subscribe:
          symbolicBus0:
            - Msg2
protocol:
  can:
    bus: # List of CAN buses on the car
      symbolicBus0: # All messages on can0
        baudrate: 500000
        extended: false
        frame:
          key: 0x001 # Lower CAN IDs have higher priority
          period: 50ms # optional, only add it if you will use it
          atom:
            atom0:
              slice: 0 + 32  # [start index] + [length]
              type: int32 big  # type and endianness
              unit: V  # optional
            atom1:
              slice: 32 + 32
              type: int32 big
              unit: V
          Msg2:
            key: 0x002
            atom:
              atom0:
                slice: 0 + 2
                type:
                  type: int8
                  endianness: big
                  enum:
                    ENUM_A: 0x0
                    ENUM_B: 0x1
                    ENUM_C: 0x2
                    ENUM_D: 0x3
      symbolicBus1:
        baudrate: 1000000
        extended: true
        frame:
          Msg0:
            key: 0x001 # Note that lower CAN IDs have higher priority
            atom:
              atom0:
                slice: 0 + 32  # [start index] + [length]
                type: int32 big  # type and endianness
                unit: V
              atom1:
                slice: 32 + 32
                type: int32 big
                unit: V
```
Once this spec is set up, to add a new message you need to do each of the following:
- **Add the message to the list of messages.** Using the format like the messages above, specify `key`, `period`, (if necessary), and `atom`. For each atom, add `slice`, (of the form `starting_position + length`), `type` (of the form `c_type endianness`, or the more expanded version in the enum)
- **Update the publish and subscribe fields for the relevant boards.** Add the name of your message to the publish list of your board and whichever boards subscribes to it.


## Using the library

To use the can library, include the file `canlib_<computer.name>.h` for whatever `computer` it will be running on. (Make sure you have initialized CAN at the baudrate specified in your spec.) The follow names will then be exposed:

### `CANlib_<bus.name>_<frame.name>_T`

This is a struct which has fields corresponding to the atoms in the corresponding frame.
For example, for `Msg0` on `symbolic_can_1` above, it would be defined like this:
```c
typedef struct {
	int32_t atom0;
	int32_t atom1;
} CANlib_symbolicBus1_Msg0_T;
```

 It will be available for every frame which the board publishes or subscribes to.

### `void CANlib_send_<bus.name>_<frame.name>_msg(CANlib_<bus.name>_<frame.name>_T *inp)`

This function takes a pointer to a struct of the type described in the previous bullet and sends in on the correct CAN bus. It is only available for messages which the board publishes.

### `CANlib_update_can()`

This function polls for any can messages and updates the relevant structs (see the next bullet). Call this periodically to update your inputs. It will only update structs corresponding to messages which a board subscribes to.

### `extern CANlib_<bus.name>_<frame.name>_T <bus.name>_<frame.name>_inp;`

This statement extern declares the struct which will be updated when a `frame.name` message is received over can. Declare it in your own file and it will be updated whenever you call `CANlib_update_can`.

### `CANlib_<bus.name>_<message.name>_<atom.name>_T`
This is an enum that includes values of the form `<bus.name>_<message.name>_<atom.name>_<value.name>` forall `value` in `atom.value`. Use this to refer to the type of enums created for enum segments (e.g., `Msg2` `atom1` above).

## Other library names
Do not use these structs in code outside of CANlib. This section is internal documentation.

### `CANlib_<bus.name>_T`
This is an enum that includes values of the form `<bus.name>_<message.name>` forall `message` in `bus.messages`.

### `CANlib_<bus.name>_T CANlib_Identify_<bus.name>(Frame* frame)`
Given a **pointer** to a Frame, returns the appropriate value from `<bus.name>_T`. It currently identifies
messages based on their `id`. If a message is unknown w.r.t. the spec, it returns `CANlib_UNKNOWN_MSG`.

### `void CANlib_Pack_<bus.name>_<message.name>(CANlib_<bus.name>_<message.name>_T* type_in, Frame* can_out)`
Given **pointers** to a message struct and a Frame respectively, unpacks the message contents into the Frame.

### `void CANlib_Unpack_<bus.name>_<message.name>(Frame *can_in, CANlib_<bus.name>_<message.name>_T *type_out)`
Given **pointers** to a Frame and message struct respectively, unpacks the Frame contents into the message object.

### `CANlib_<bus.name>_<message.name>_<property>` forall `property` in `["id", "period"]`
The CANlib exposes certain properties for each message defined in the spec. Use them in your code with the above expression. There is no guarantee as to whether you learn about it as part of an enum or though a #define.
