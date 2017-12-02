TO_CAN(Can_MC_ErrorAndWarning) {
  uint64_t bitstring = 0;
  bitstring = INSERT(CAN_MC_REG_ERRORS_AND_WARNINGS, bitstring, 0, 8);

  // Start as big endian for now, swap later.
  // Also need to work as 0-as-least-significant-bit due to motor controller
  // reasons.
  uint32_t data_string = 0;
  BIT_SET(data_string, type_in->error_parameter_damaged, 0);
  BIT_SET(data_string, type_in->error_output_stage_fault, 1);
  BIT_SET(data_string, type_in->error_rfe_fault, 2);
  BIT_SET(data_string, type_in->error_bus_fault, 3);
  BIT_SET(data_string, type_in->error_faulty_encoder, 4);
  BIT_SET(data_string, type_in->error_power_voltage_missing, 5);
  BIT_SET(data_string, type_in->error_motor_temp_high, 6);
  BIT_SET(data_string, type_in->error_device_temp_high, 7);
  BIT_SET(data_string, type_in->error_over_voltage, 8);
  BIT_SET(data_string, type_in->error_over_current, 9);
  BIT_SET(data_string, type_in->error_raceaway, 10);
  BIT_SET(data_string, type_in->error_user_selected_fault, 11);
  BIT_SET(data_string, type_in->error_i2r_overload, 12);
  BIT_SET(data_string, type_in->error_incompatible_firmware, 14);
  BIT_SET(data_string, type_in->error_ballast_overload, 15);

  BIT_SET(data_string, type_in->warning_inconsistent_device, 16);
  BIT_SET(data_string, type_in->warning_illegal_status_emi, 17);
  BIT_SET(data_string, type_in->warning_rfe_signal_inactive, 18);
  BIT_SET(data_string, type_in->warning_power_voltage_low, 21);
  BIT_SET(data_string, type_in->warning_motor_temp_high, 22);
  BIT_SET(data_string, type_in->warning_device_temp_high, 23);
  BIT_SET(data_string, type_in->warning_over_voltage, 24);
  BIT_SET(data_string, type_in->warning_over_current, 25);
  BIT_SET(data_string, type_in->warning_i2r_overload, 28);
  BIT_SET(data_string, type_in->warning_ballast_overload, 31);

  uint32_t swapped_data_string = swap_uint32(data_string);
  bitstring = INSERT(swapped_data_string, bitstring, 8, 32);
  from_bitstring(&bitstring, can_out->data);
  can_out->id = MC_RESPONSE__id;
  can_out->len = 5;
}

FROM_CAN(Can_MC_ErrorAndWarning) {
  uint64_t bitstring = 0;
  to_bitstring(can_in->data, &bitstring);

  uint32_t swapped_data_string = EXTRACT(bitstring, 8, 32);
  uint32_t data_string = swap_uint32(swapped_data_string);

  type_out->error_parameter_damaged = BIT_GET(data_string, 0);
  type_out->error_output_stage_fault = BIT_GET(data_string, 1);
  type_out->error_rfe_fault = BIT_GET(data_string, 2);
  type_out->error_bus_fault = BIT_GET(data_string, 3);
  type_out->error_faulty_encoder = BIT_GET(data_string, 4);
  type_out->error_power_voltage_missing = BIT_GET(data_string, 5);
  type_out->error_motor_temp_high = BIT_GET(data_string, 6);
  type_out->error_device_temp_high = BIT_GET(data_string, 7);
  type_out->error_over_voltage = BIT_GET(data_string, 8);
  type_out->error_over_current = BIT_GET(data_string, 9);
  type_out->error_raceaway = BIT_GET(data_string, 10);
  type_out->error_user_selected_fault = BIT_GET(data_string, 11);
  type_out->error_i2r_overload = BIT_GET(data_string, 12);
  type_out->error_incompatible_firmware = BIT_GET(data_string, 14);
  type_out->error_ballast_overload = BIT_GET(data_string, 15);

  type_out->warning_inconsistent_device = BIT_GET(data_string, 16);
  type_out->warning_illegal_status_emi = BIT_GET(data_string, 17);
  type_out->warning_rfe_signal_inactive = BIT_GET(data_string, 18);
  type_out->warning_power_voltage_low = BIT_GET(data_string, 21);
  type_out->warning_motor_temp_high = BIT_GET(data_string, 22);
  type_out->warning_device_temp_high = BIT_GET(data_string, 23);
  type_out->warning_over_voltage = BIT_GET(data_string, 24);
  type_out->warning_over_current = BIT_GET(data_string, 25);
  type_out->warning_i2r_overload = BIT_GET(data_string, 28);
  type_out->warning_ballast_overload = BIT_GET(data_string, 31);
}

TO_CAN(Can_MC_State) {
  uint64_t bitstring = 0;
  bitstring = INSERT(CAN_MC_REG_ERRORS_AND_WARNINGS, bitstring, 0, 8);

  // Start as big endian for now, swap later.
  // Also need to work as 0-as-least-significant-bit due to motor controller
  // reasons.
  uint32_t data_string = 0;
  BIT_SET(data_string, type_in->hardware_enable, 0);
  BIT_SET(data_string, type_in->drive_stopped, 1);
  BIT_SET(data_string, type_in->lim_plus_assigned, 2);
  BIT_SET(data_string, type_in->lim_minus_assigned, 3);
  BIT_SET(data_string, type_in->drive_ok, 4);
  BIT_SET(data_string, type_in->current_limit_to_continuous, 5);
  BIT_SET(data_string, type_in->speed_limited_torque_mode, 6);
  BIT_SET(data_string, type_in->position_control_mode, 7);
  BIT_SET(data_string, type_in->speed_control_mode, 8);
  BIT_SET(data_string, type_in->low_speed, 9);
  BIT_SET(data_string, type_in->btb_rdy, 14);
  BIT_SET(data_string, type_in->regen_active, 15);
  BIT_SET(data_string, type_in->inverted_command, 16);
  BIT_SET(data_string, type_in->speed_limited_via_switch, 17);
  BIT_SET(data_string, type_in->current_limited_via_switch, 20);
  BIT_SET(data_string, type_in->active_current_reduction, 21);
  BIT_SET(data_string, type_in->current_limited_via_speed, 22);
  BIT_SET(data_string, type_in->current_limited_via_igbt_temp, 23);
  BIT_SET(data_string, type_in->current_limited_to_continuous_via_igbt_temp, 24);
  BIT_SET(data_string, type_in->current_reduction_low_frequency, 25);
  BIT_SET(data_string, type_in->current_reduction_via_motor_temp, 26);
  BIT_SET(data_string, type_in->current_reduction_via_analog_input, 27);
  BIT_SET(data_string, type_in->current_accumulator_limit_charged, 28);
  BIT_SET(data_string, type_in->handwheel_input_selected, 31);

  uint32_t swapped_data_string = swap_uint32(data_string);
  bitstring = INSERT(swapped_data_string, bitstring, 8, 32);
  from_bitstring(&bitstring, can_out->data);
  can_out->id = MC_RESPONSE__id;
  can_out->len = 5;
}

FROM_CAN(Can_MC_State) {
  uint64_t bitstring = 0;
  to_bitstring(can_in->data, &bitstring);

  uint32_t swapped_data_string = EXTRACT(bitstring, 8, 32);
  uint32_t data_string = swap_uint32(swapped_data_string);

  type_out->hardware_enable = BIT_GET(data_string, 0);
  type_out->drive_stopped = BIT_GET(data_string, 1);
  type_out->lim_plus_assigned = BIT_GET(data_string, 2);
  type_out->lim_minus_assigned = BIT_GET(data_string, 3);
  type_out->drive_ok = BIT_GET(data_string, 4);
  type_out->current_limit_to_continuous = BIT_GET(data_string, 5);
  type_out->speed_limited_torque_mode = BIT_GET(data_string, 6);
  type_out->position_control_mode = BIT_GET(data_string, 7);
  type_out->speed_control_mode = BIT_GET(data_string, 8);
  type_out->low_speed = BIT_GET(data_string, 9);
  type_out->btb_rdy = BIT_GET(data_string, 14);
  type_out->regen_active = BIT_GET(data_string, 15);
  type_out->inverted_command = BIT_GET(data_string, 16);
  type_out->speed_limited_via_switch = BIT_GET(data_string, 17);
  type_out->current_limited_via_switch = BIT_GET(data_string, 20);
  type_out->active_current_reduction = BIT_GET(data_string, 21);
  type_out->current_limited_via_speed = BIT_GET(data_string, 22);
  type_out->current_limited_via_igbt_temp = BIT_GET(data_string, 23);
  type_out->current_limited_to_continuous_via_igbt_temp = BIT_GET(data_string, 24);
  type_out->current_reduction_low_frequency = BIT_GET(data_string, 25);
  type_out->current_reduction_via_motor_temp = BIT_GET(data_string, 26);
  type_out->current_reduction_via_analog_input = BIT_GET(data_string, 27);
  type_out->current_accumulator_limit_charged = BIT_GET(data_string, 28);
  type_out->handwheel_input_selected = BIT_GET(data_string, 31);
}

TO_CAN(Can_MC_DataReading) {
  uint64_t bitstring = 0;
  bitstring = INSERT(type_in->type, bitstring, 0, 8);

  // Little Endian
  int16_t swap_value = swap_int16(type_in->value);

  bitstring = INSERT(swap_value, bitstring, 8, 16);
  from_bitstring(&bitstring, can_out->data);
  can_out->id = MC_RESPONSE__id;
  can_out->len = 3;
}

FROM_CAN(Can_MC_DataReading) {
  uint64_t bitstring = 0;
  to_bitstring(can_in->data, &bitstring);
  type_out->type = (Can_MC_RegID_T)(EXTRACT(bitstring, 0, 8));

  // Little Endian
  int16_t swap_value = (int16_t)(swap_uint16(EXTRACT(bitstring, 8, 16)));

  type_out->value = swap_value;
}

TO_CAN(Can_Vcu_MCTorque) {
  uint64_t bitstring = 0;
  bitstring = INSERT(CAN_MC_REG_TORQUE_CMD, bitstring, 0, 8);
  // TODO all other devices are big endian except motor controllers,
  // so we want to reverse the endianness
  uint64_t temp_torque_cmd = type_in->torque_cmd;
  uint8_t high_byte = EXTRACT(temp_torque_cmd, 48, 8);
  uint8_t low_byte = EXTRACT(temp_torque_cmd, 56, 8);
  bitstring = INSERT(low_byte, bitstring, 8, 8);
  bitstring = INSERT(high_byte, bitstring, 16, 8);
  from_bitstring(&bitstring, can_out->data);
  can_out->id = VCU_MC_MESSAGE__id;
  can_out->len = 3;
}

FROM_CAN(Can_Vcu_MCTorque) {
  uint64_t bitstring = 0;
  to_bitstring(can_in->data, &bitstring);
  // TODO all other devices are big endian except motor controllers,
  // so we want to reverse the endianness
  uint8_t low_byte = EXTRACT(bitstring, 8, 8);
  uint8_t high_byte = EXTRACT(bitstring, 16, 8);
  int16_t low_word = (int16_t) low_byte;
  int16_t high_word = (int16_t) (high_byte << 8);
  type_out->torque_cmd = low_word | high_word;
}

TO_CAN(Can_Vcu_MCRequest) {
  uint64_t bitstring = 0;
  bitstring = INSERT(CAN_MC_REG_MSG_REQUEST, bitstring, 0, 8);
  bitstring = INSERT(type_in->requestType, bitstring, 8, 8);
  bitstring = INSERT(type_in->period, bitstring, 16, 8);
  from_bitstring(&bitstring, can_out->data);
  can_out->id = VCU_MC_MESSAGE__id;
  can_out->len = 3;
}

FROM_CAN(Can_Vcu_MCRequest) {
  uint64_t bitstring = 0;
  to_bitstring(can_in->data, &bitstring);
  type_out->requestType = (Can_MC_RegID_T) (EXTRACT(bitstring, 8, 8));
  type_out->period = EXTRACT(bitstring, 16, 8);
}

DEFINE(Can_Vcu_MCRequest)
DEFINE(Can_Vcu_MCTorque)
DEFINE(Can_MC_DataReading)
DEFINE(Can_MC_ErrorAndWarning)
DEFINE(Can_MC_State)
