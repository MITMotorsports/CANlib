"""Generate all necessary files."""
import fsae_can_spec
import MY17_Can_Library_c
import MY17_Can_Library_h
import ids
import structs
from common import *  # All method arguments come from here

if __name__ == "__main__":
    fsae_can_spec.write(fsae_can_spec_path, spec_path)
    MY17_Can_Library_c.write(can_lib_c_path, spec_path, can_lib_c_base_path, can_lib_c_special_cases_path,
                             unused_messages)
    MY17_Can_Library_h.write(can_lib_h_path, spec_path, struct_paths.values(), unused_messages)
    ids.write(ids_path, spec_path)
    structs.write(struct_paths, spec_path)
