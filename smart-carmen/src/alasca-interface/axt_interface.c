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

void axt_send_message(AXT_SCAN_STR *scan, axt_message *msg) {
  IPC_RETURN_TYPE err;
  int i;
  
  msg->num_points = scan->header.num_points;
  msg->version = scan->header.version;
  msg->scanner_type = scan->header.scanner_type;
  msg->ecu_id = scan->header.ecu_id;
  msg->timestamp_sensor = scan->header.timestamp;
  msg->start_angle = scan->header.start_angle;
  msg->end_angle = scan->header.end_angle;
  msg->scan_counter = scan->header.num_points;
  
  msg->timestamp= scan->header.timestamp_sync;  

  float x_max, x_min, y_max, y_min;
  x_max = -1000.0; x_min = 1000.0;    
  y_max = -1000.0; y_min = 1000.0;

  for (i = 0; i < msg->num_points; i++) {
    msg->channel[i] = scan->points[i].channel;
    msg->point_status[i] = scan->points[i].point_status;
    msg->x[i] = scan->points[i].x;
    msg->y[i] = scan->points[i].y;
    msg->z[i] = scan->points[i].z;

    if( msg->x[i] < x_min) x_min = msg->x[i];
    else if( msg->x[i] > x_max) x_max = msg->x[i];

    if( msg->y[i] < y_min) y_min = msg->y[i];
    else if( msg->y[i] > y_max) y_max = msg->y[i];
  }

  err = IPC_publishData(AXT_MESSAGE_NAME, msg);
  
  carmen_test_ipc(err, "Could not publish", AXT_MESSAGE_NAME);
  msg->timestamp = scan->header.timestamp_sync;

  return;
}

void axt_handle_carmen_message(axt_message *msg, AXT_SCAN_STR *scan) {
	int i;

	scan->header.version = msg->version;
	scan->header.scanner_type = msg->scanner_type;
	scan->header.ecu_id = msg->ecu_id;
	scan->header.timestamp = msg->timestamp_sensor;
	scan->header.timestamp_sync = msg->timestamp;
	
	scan->header.start_angle = msg->start_angle;
	scan->header.end_angle = msg->end_angle;
	scan->header.scan_counter = msg->scan_counter;
	scan->header.num_points = msg->num_points;

	for (i = 0; i < scan->header.num_points; i++) {
    scan->points[i].channel = msg->channel[i];
    scan->points[i].point_status = msg->point_status[i];
    scan->points[i].x = msg->x[i];
    scan->points[i].y = msg->y[i];
    scan->points[i].z = msg->z[i];
  }
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
