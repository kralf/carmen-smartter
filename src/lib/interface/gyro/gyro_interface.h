#ifndef GYRO_INTERFACE_H
#define GYRO_INTERFACE_H

#include "gyro_messages.h"

#ifdef __cplusplus
extern "C" {
#endif

void gyro_subscribe_integrated_message(gyro_integrated_message* msg, 
  carmen_handler_t handler, carmen_subscribe_t subscribe_how);
void gyro_unsubscribe_integrated_message(carmen_handler_t handler);

#ifdef __cplusplus
}
#endif

#endif
// @}
