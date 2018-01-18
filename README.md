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
  board2: # Note that this board has no architecture specified (good for boards we don't program ourselves)
    publish:
      can_bus0:
        - Msg2
    subscribe:
      can_bus0:
        - Msg0
buses: # List of CAN buses on the car
  can_bus0: # All messages on can_bus0
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
* **Add the message to the list of messages.** Using the format like the messages above, specify `can_id`, `is_big_endian`, `frequency` (if necessary), and `segments`. For each segment, add `position`, `length`, `c_type`, as well as any optional fields you need (`signed`, `enum`, and `unit`).
* **Update the publish and subscribe fields for the relevant boards.** Add the name of your message to the publish list of your board, and then optionally whichever boards subscribes to it. (You'll get errors for if you don't have a board publish your message, but it's okay to have no subscribers for it.)


## Using the library
Include `Can_Library.h` and define your architecture by defining either `CAN_ARCHITECTURE_ARM` or `CAN_ARCHITECTURE_AVR` (usually in your Makefile). Once you do this, the following functions, variables, and structs are available and are what you'll want to be using:

### `void YourBusName_YourBoardName_Init(uint32_t baudrate)`
Call this before you use CAN at all. It initializes CAN and configures the message filtering for your particular board on the specified bus. (Note that `YourBusName` and `YourBoardName` are the names of your bus and your board converted to title case, so if youre bus is "can0" and your board is called "dash," you would call `Can0_Dash_Init` with the right baudrate.)

*Note:* The vanilla `CAN_Init` is still available and is fine for small tests, but you should generally use the CAN init function for your board so you can filter messages properly.

### `Can_MsgID_T`
This is an enum that can take on values `Can_No_Msg`, `Can_Unknown_Msg`, and `Can_Error_Msg` as well as `Can_YourMessageName_Msg` for each message in the spec. (So for example, if you have a message named `FrontCanNodeOutput`, one of the values will be `Can_FrontCanNodeOutput_Msg`.)

### `Can_MsgID_T Can_MsgType(void)`
This reads the latest message from the CAN bus and returns which CAN message it is. Typically what you'll want to do is call this function and add a switch statement on what this returns, handling each type of message you care about.

### `Can_YourMessageName_T`
This is a struct generated for each message in the spec. It will have fields corresponding to each of the segments in the message. (So if your message `FrontCanNodeOutput` has segments `requested_torque`, `is_throttle_implausible`, and `has_brake_throttle_conflict`, these will all be fields in the struct.)

### `Can_ErrorID_T Can_YourMessageName_Read(Can_YourMessageName_T *type)`
When you pass in a pointer to a message struct (the one described in the previous bullet), this function will populate it with the message from the CAN bus. (`Can_ErrorID_T` describes the various errors that might be returned when you read from the CAN bus. See [can_raw.h](can_raw.h) for more.)

### `Can_ErrorID_T Can_YourMessageName_Write(Can_YourMessageName_T *type)`
Once you have a message struct populated with all the information you want to write, call this function and pass in a pointer to your struct. It will be written to the CAN bus.

### `YOURMESSAGENAME__freq`
This variable is created if if you defined the frequency for your message in the CAN spec. (Note that the name of the variable starts with you message name, but in all caps.)
