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
