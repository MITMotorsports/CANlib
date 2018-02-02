# CANlib

## Installation
If you're cloning this for the first time, use `git clone --recursive` to pull all submodules. However, if you already have this repository cloned non-recursively, make sure to run `git submodule init` and `git submodule update`.

Before using this library, run `main.py` in the `generator` directory with the path to your CAN spec as an argument (e.g. `python main.py ../../../can_spec_my18.yml` if you're running this from the `lib` directory in MY18). Make sure to use Python 3. This generates all of the necessary source files that are listed in `.gitignore`. These files are dependent on the CAN spec. If you want to change one of these files, don't edit them directly, but instead figure out what needs to change in the CAN spec. (If that still doesn't work, edit the generator scripts in `generator` rather than the generated files.) Source files that aren't ignored aren't generated from the CAN spec, so they can be edited directly.

## Writing a CAN spec
The CAN spec should be a YAML file with the following structure:
```yaml
name: MYwhatever # Name of your CAN spec
archs: # Allowed architectures for boards
  - arch0
  - arch1
boards: # All boards on the car, along with CAN messages they publish and subscribe to
  board1:
    arch: arch1
    publish:
      can_bus0: # All messages on the bus "can_bus0" board1 publishes
        - Msg0
      can_bus1: # All messages on the bus "can_bus1" board1 publishes
        - Msg1
    subscribe: # All messages on : bus "can_bus0" board1 subscribes to
      can_bus0:
        - Msg2
  # Note that the next board has no architecture specified
  # (Good for boards we don't program ourselves)
  board2:
    publish:
      can_bus0:
        - Msg2
    subscribe:
      can_bus0:
        - Msg0
buses: # List of CAN buses on the car
  can_bus0: # All messages on can_bus0
    baudrate: 500000
    is_extended: false
    messages:
      Msg0:
        can_id: 0x001 # Note that lower CAN IDs have higher priority
        frequency: 50Hz # Frequency is optional, only add it if you will use it
        is_big_endian: true
        segments:
          segment0:
            position: 0 # Which bit in the CAN message the segment starts at
            length: 32 # How many bits long the segment is
            c_type: int32_t
            signed: true # The signed field is technically optional, but good to add for integers
            unit: V # The unit field is also optional
          segment1:
            position: 0
            length: 32
            c_type: int32_t
            signed: true
            unit: V
      Msg2:
        can_id: 0x002
        is_big_endian: true
        segments:
          segment0:
            position: 0
            length: 2
            c_type: enum
            enum: # Make sure to add this for segments of type enum
              ENUM_A: 0x0
              ENUM_B: 0x1
              ENUM_C: 0x2
              ENUM_D: 0x3
  can_bus1:
    messages:
      Msg1:
        can_id: 0x010
        frequency: 10Hz
        is_big_endian: false
        segments:
          segment0:
            position: 0
            length: 1
            c_type: bool
```
The above spec has fewer boards and messages than a real spec likely would, but it demonstrates all of the functionality that CANlib supports. The basic idea is that once your spec is set up, to add a new message you need to do each of the following:
- **Add the message to the list of messages.** Using the format like the messages above, specify `can_id`, `is_big_endian`, `frequency` (if necessary), and `segments`. For each segment, add `position`, `length`, `c_type`, as well as any optional fields you need (`signed`, `enum`, and `unit`).
- **Update the publish and subscribe fields for the relevant boards.** Add the name of your message to the publish list of your board, and then optionally whichever boards subscribes to it.

## Learning the library
Frames represent the low level CAN messages sent and received from a CAN bus.
```c
typedef struct {
  bool extended; // Whether or not the bus used is extended
  uint32_t id; // (11 bit if extended else 29 bit) CAN id
  uint8_t len; // The data length code = number of bytes of data
  uint8_t data[8]; // Your bytes of data truncated to len_th object
} Frame;
```

## Using the library
Include `CANlib.h` and define your architecture by defining either `CAN_ARCHITECTURE_LPC11CX4`, `CAN_ARCHITECTURE_STM32F2XX` or `CAN_ARCHITECTURE_AVR` (usually in your Makefile).

Once you do this, the following functions, variables, and structs are available and are what you'll want to be using:

The following names are maintained exactly as found in the spec.

### `void init_<bus.name>_<board.name>(void)`
Call this before you use CAN at all. It initializes CAN and configures the message filtering for your particular board on the specified bus.

*Note:* The vanilla `Can_Init` is NOT fine for production code.

### `<bus.name>_T`
This is an enum that includes values of the form `<bus.name>_<message.name>` forall `message` in `bus.messages`.

### `<bus.name>_T identify_<bus.name>(Frame* frame)`
Given a **pointer** to a Frame, returns the appropriate value from `<bus.name>_T`. It currently identifies
messages based on their `can_id`. If a message is unknown w.r.t. the spec, it returns `CAN_UNKNOWN_MSG`.

### `<bus.name>_<message.name>_T`
This is a struct that contains members of the form `<bus.name>_<message.name>_<segment.name>` forall `segment` in `message.segments` each with their appropriate type (`segment.c_type` unless that is equal to `enum` in which case it gets replaces with the formatted ).

### `<bus.name>_<message.name>_<segment.name>_T`
This is an enum that includes values of the form `<bus.name>_<message.name>_<segment.name>_<value.name>` forall `value` in `segment.value`.

### `void pack_<bus.name>_<message.name>(<bus.name>_<message.name>_T* type_in, Frame* can_out)`
Given **pointers** to a message struct and a Frame respectively, unpacks the message contents into the Frame.

### `void unpack_<bus.name>_<message.name>(Frame *can_in, <bus.name>_<message.name>_T *type_out)`
Given **pointers** to a Frame and message struct respectively, unpacks the Frame contents into the message object.

### `Can_ErrorID_T <bus.name>_<message.name>_Write(<bus.name>_<message.name>_T *type)`
Given a **pointer** to a message struct, unpack it into a CAN frame and transmit it over `bus` using `Can_RawRead`.

### `<bus.name>_<message.name>_<property>` forall `property` in `["can_id", "frequency"]`
The CANlib exposes certain properties for each message defined in the spec. Use them in your code with the above expression. There is no guarantee as to whether you learn about it as part of an enum or though a #define.
