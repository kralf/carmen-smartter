#ifndef LOCFILTER_INTERFACE_H
#define LOCFILTER_INTERFACE_H

#include "locfilter_messages.h"

#ifdef __cplusplus
extern "C" {
#endif

void locfilter_subscribe_filteredpos_message(locfilter_filteredpos_message*
  msg, carmen_handler_t handler, carmen_subscribe_t subscribe_how);
void locfilter_unsubscribe_filteredpos_message(carmen_handler_t handler);

void locfilter_subscribe_init_message(locfilter_init_message* msg,
  carmen_handler_t handler, carmen_subscribe_t subscribe_how);
void locfilter_unsubscribe_init_message(carmen_handler_t handler);

#ifdef __cplusplus
}
#endif

#endif
// @}
