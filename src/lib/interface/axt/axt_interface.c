//AXT INTERFACE.C

#include "axt_interface.h"
#include "axt_message.h"

void axt_subscribe_message(axt_message *m, carmen_handler_t handler,
  carmen_subscribe_t subscribe_how) {
  carmen_subscribe_message(AXT_MESSAGE_NAME, AXT_MESSAGE_FMT, m, 
    sizeof(axt_message), handler, subscribe_how);
}

void axt_define_message() {
  IPC_RETURN_TYPE err;

  err = IPC_defineMsg(AXT_MESSAGE_NAME, IPC_VARIABLE_LENGTH, 
    AXT_MESSAGE_FMT);
  carmen_test_ipc_exit(err, "Could not define message", AXT_MESSAGE_NAME);
}

void axt_send_message(axt_message *msg) {
  IPC_RETURN_TYPE err = IPC_publishData(AXT_MESSAGE_NAME, msg);

  carmen_test_ipc(err, "Could not publish", AXT_MESSAGE_NAME);
  return;
}

void axt_message_alloc(axt_message *msg) {
  msg->channel = (unsigned char*) malloc(5000 * sizeof(int));
  msg->point_status = (unsigned char*) malloc(5000 * sizeof(int));
  msg->x = (float*) malloc(5000 * sizeof(float));
  msg->y = (float*) malloc(5000 * sizeof(float));
  msg->z = (float*) malloc(5000 * sizeof(float));
}

void axt_message_dealloc(axt_message *msg) {
  free(msg->channel);
  free(msg->point_status);
  free(msg->x);
  free(msg->y);
  free(msg->z);
}
