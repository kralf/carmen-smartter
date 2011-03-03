#include "axt_interface.h"

void axt_subscribe_message(axt_message* msg, carmen_handler_t handler,
    carmen_subscribe_t subscribe_how) {
  carmen_subscribe_message(AXT_MESSAGE_NAME, AXT_MESSAGE_FMT, msg,
    sizeof(axt_message), handler, subscribe_how);
}

void axt_unsubscribe_message(carmen_handler_t handler) {
  carmen_unsubscribe_message(AXT_MESSAGE_NAME, handler);
}
