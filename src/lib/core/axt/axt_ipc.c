/*********************************************************
 *
 * This source code is part of the Carnegie Mellon Robot
 * Navigation Toolkit (CARMEN)
 *
 * CARMEN Copyright (c) 2002 Michael Montemerlo, Nicholas
 * Roy, Sebastian Thrun, Dirk Haehnel, Cyrill Stachniss,
 * and Jared Glover
 *
 * CARMEN is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public 
 * License as published by the Free Software Foundation; 
 * either version 2 of the License, or (at your option)
 * any later version.
 *
 * CARMEN is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied 
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more 
 * details.
 *
 * You should have received a copy of the GNU General 
 * Public License along with CARMEN; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place, 
 * Suite 330, Boston, MA  02111-1307 USA
 *
 ********************************************************/

#include <carmen/global.h>

#include "axt_ipc.h"

int axt_ipc_initialize(int argc, char *argv[]) {
  IPC_RETURN_TYPE err;

  carmen_ipc_initialize(argc, argv);
  carmen_param_check_version(argv[0]);

  err = IPC_defineMsg(AXT_MESSAGE_NAME, IPC_VARIABLE_LENGTH, AXT_MESSAGE_FMT);
  carmen_test_ipc_exit(err, "Could not define message", AXT_MESSAGE_NAME);

  return 0;
}

void axt_publish(unsigned int version, unsigned int scanner_type, unsigned int
    ecu_id, double start_angle, double end_angle, unsigned int scan_counter,
    unsigned int timestamp_sensor, unsigned int num_points, float* x, float* y,
    float* z, unsigned char* channel, unsigned char* point_status, double
    timestamp) {
  axt_message msg;
  IPC_RETURN_TYPE err;

  msg.num_points = num_points;
  msg.version = version;
  msg.scanner_type = scanner_type;
  msg.ecu_id = ecu_id;
  msg.timestamp_sensor = timestamp_sensor;
  msg.start_angle = start_angle;
  msg.end_angle = end_angle;
  msg.scan_counter = scan_counter;

  msg.channel = channel;
  msg.point_status = point_status;

  msg.x = x;
  msg.y = y;
  msg.z = z;

  msg.timestamp = timestamp;
  msg.host = carmen_get_host();

  err = IPC_publishData(AXT_MESSAGE_NAME, &msg);
  carmen_test_ipc_exit(err, "Could not publish", AXT_MESSAGE_NAME);
}
