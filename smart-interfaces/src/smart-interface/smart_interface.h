#ifndef SMART_INTERFACE_H
#define SMART_INTERFACE_H

#include "smart_messages.h"

#ifdef __cplusplus
extern "C" {
#endif

void smart_subscribe_status_message(smart_status_message* msg,
  carmen_handler_t handler, carmen_subscribe_t subscribe_how);
void smart_unsubscribe_status_message(carmen_handler_t handler);

void smart_subscribe_velocity_message(smart_velocity_message* msg,
  carmen_handler_t handler, carmen_subscribe_t subscribe_how);
void smart_unsubscribe_velocity_message(carmen_handler_t handler);

#ifdef __cplusplus
}
#endif

#endif
// @}
