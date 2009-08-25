#include <carmen/carmen.h>

#include "gyro_interface.h"

void gyro_subscribe_integrated_message(gyro_integrated_message* msg, 
  carmen_handler_t handler, carmen_subscribe_t subscribe_how) {
  carmen_subscribe_message(GYRO_INTEGRATED_MESSAGE_NAME, 
    GYRO_INTEGRATED_MESSAGE_FMT, msg, sizeof(gyro_integrated_message), 
    handler, subscribe_how);
}

void gyro_unsubscribe_integrated_message(carmen_handler_t handler) {
  carmen_unsubscribe_message(GYRO_INTEGRATED_MESSAGE_NAME, handler);
}
