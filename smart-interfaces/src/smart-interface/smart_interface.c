#include <carmen/carmen.h>

#include "smart_interface.h"

void smart_subscribe_status_message(smart_status_message* msg,
  carmen_handler_t handler, carmen_subscribe_t subscribe_how) {
  carmen_subscribe_message(SMART_STATUS_MESSAGE_NAME, SMART_STATUS_MESSAGE_FMT,
    msg, sizeof(smart_status_message), handler, subscribe_how);
}

void smart_unsubscribe_status_message(carmen_handler_t handler) {
  carmen_unsubscribe_message(SMART_STATUS_MESSAGE_NAME, handler);
}

void smart_subscribe_init_odometrypos_message(
  smart_init_odometrypos_message* msg, carmen_handler_t handler,
  carmen_subscribe_t subscribe_how) {
  carmen_subscribe_message(SMART_INIT_ODOMETRYPOS_MESSAGE_NAME,
    SMART_INIT_ODOMETRYPOS_MESSAGE_FMT, msg,
    sizeof(smart_init_odometrypos_message), handler, subscribe_how);
}

void smart_unsubscribe_init_odometrypos_message(carmen_handler_t handler) {
  carmen_unsubscribe_message(SMART_INIT_ODOMETRYPOS_MESSAGE_NAME, handler);
}

void smart_subscribe_velocity_message(smart_velocity_message* msg,
  carmen_handler_t handler, carmen_subscribe_t subscribe_how) {
  carmen_subscribe_message(SMART_VELOCITY_MESSAGE_NAME,
    SMART_VELOCITY_MESSAGE_FMT, msg, sizeof(smart_velocity_message), handler,
    subscribe_how);
}

void smart_unsubscribe_velocity_message(carmen_handler_t handler) {
  carmen_unsubscribe_message(SMART_VELOCITY_MESSAGE_NAME, handler);
}
