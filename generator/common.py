spec_path = "ParseCAN/fsae_can_spec.yml"

fsae_can_spec_path = "fsae_can_spec.h"
can_lib_c_path = "../MY17_Can_Library.c"
can_lib_h_path = "../MY17_Can_Library.h"
ids_path = "../ids.h"

can_lib_c_base_path = "templates/MY17_Can_Library_BASE.txt"
can_lib_c_special_cases_path = "templates/special_cases.txt"

unused_messages = [
    "Lv_Battery_Voltage",
    "Accelerometer_Horizontal",
    "Accelerometer_Vertical",
    "Gyro_Vertical",
    "Gyro_Horizontal",
    "Magnetometer_Horizontal",
    "Magnetometer_Vertical",
    "Mc_Response",
    "Vcu_Mc_Message"
]

struct_paths = {
    "bms": "../bms.h",
    "cannode": "../can_node.h",
    "currentsensor": "../current_sensor.h",
    "dash": "../dash.h",
    "vcu": "../vcu.h"
}
