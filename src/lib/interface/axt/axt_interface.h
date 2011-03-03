#ifndef LOCFILTER_INTERFACE_H
#define LOCFILTER_INTERFACE_H

#include <carmen/global.h>

#include "axt_messages.h"

#ifdef __cplusplus
extern "C" {
#endif

void axt_subscribe_message(axt_message* msg, carmen_handler_t handler,
  carmen_subscribe_t subscribe_how);
void axt_unsubscribe_message(carmen_handler_t handler);

#ifdef __cplusplus
}
#endif

#endif
// @}
