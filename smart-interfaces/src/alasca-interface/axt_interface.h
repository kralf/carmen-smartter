#ifndef AXT_INTERFACE_H
#define AXT_INTERFACE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <carmen/carmen.h>
#include "axt_message.h"



  /*!
   * Subscribes to the IBEO Alasca XT message
   */
  void axt_subscribe_message(axt_message *m, 
														 carmen_handler_t handler,
														 carmen_subscribe_t subscribe_how);  
  
  /*!
   * Defines the IBEO Alasca XT  message. Call this before sending messages.
   */
  void axt_define_message();
  
  /*!
   * Sends an AXT_SCAN_STR as carmen message
   */
  void axt_send_message(AXT_SCAN_STR *scan, axt_message *msg);

	/*!
	 * Copy message to AXT_SCAN_STR
	 */
  void axt_handle_carmen_message(axt_message *msg, AXT_SCAN_STR *scan);

  void axt_message_alloc(axt_message *msg);
  void axt_message_dealloc(axt_message *msg);
  
 
#ifdef __cplusplus
}
#endif


#endif
