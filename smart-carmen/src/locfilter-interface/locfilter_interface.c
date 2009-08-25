#include <carmen/carmen.h>

#include "locfilter_interface.h"

void locfilter_subscribe_filteredpos_message(locfilter_filteredpos_message* 
  msg, carmen_handler_t handler, carmen_subscribe_t subscribe_how) {
  carmen_subscribe_message(LOCFILTER_FILTEREDPOS_MESSAGE_NAME, 
    LOCFILTER_FILTEREDPOS_MESSAGE_FMT, msg, 
    sizeof(locfilter_filteredpos_message), handler, subscribe_how);
}

void locfilter_unsubscribe_filteredpos_message(carmen_handler_t handler) {
  carmen_unsubscribe_message(LOCFILTER_FILTEREDPOS_MESSAGE_NAME, handler);
}
