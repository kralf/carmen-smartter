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

#include "smart_ipc.h"

int smart_ipc_initialize(int argc, char *argv[]) {
  IPC_RETURN_TYPE err;

  carmen_ipc_initialize(argc, argv);
  carmen_param_check_version(argv[0]);

  err = IPC_defineMsg(CARMEN_BASE_ODOMETRY_NAME, IPC_VARIABLE_LENGTH,
    CARMEN_BASE_ODOMETRY_FMT);
  carmen_test_ipc_exit(err, "Could not define message",
    CARMEN_BASE_ODOMETRY_NAME);

  err = IPC_defineMsg(CARMEN_BASE_VELOCITY_NAME, IPC_VARIABLE_LENGTH,
    CARMEN_BASE_VELOCITY_FMT);
  carmen_test_ipc_exit(err, "Could not define message",
    CARMEN_BASE_VELOCITY_NAME);

  err = IPC_defineMsg(SMART_STATUS_MESSAGE_NAME, IPC_VARIABLE_LENGTH,
    SMART_STATUS_MESSAGE_FMT);
  carmen_test_ipc_exit(err, "Could not define message",
    SMART_STATUS_MESSAGE_NAME);

  err = IPC_defineMsg(SMART_INIT_ODOMETRYPOS_MESSAGE_NAME,
    IPC_VARIABLE_LENGTH, SMART_INIT_ODOMETRYPOS_MESSAGE_FMT);
  carmen_test_ipc_exit(err, "Could not define message",
    SMART_INIT_ODOMETRYPOS_MESSAGE_NAME);

  return 0;
}

void smart_publish_status(SMART_VEHICLE_STATUS* smart, double timestamp) {
  smart_status_message msg;
  IPC_RETURN_TYPE err;

  msg.gas_pos = smart->engine.pedal;
  msg.gear = smart->engine.actual_gear;
  msg.steering_angle = smart->status.phi_curr;

  msg.tv = smart->status.v_curr;

  msg.rv_front_right = smart->wheelspeed.front_right_not_plausible ? 0.0 :
    smart->wheelspeed.front_right;
  msg.rv_front_left = smart->wheelspeed.front_left_not_plausible ? 0.0 :
    smart->wheelspeed.front_left;
  msg.rv_rear_right = smart->wheelspeed.rear_right_not_plausible ? 0.0 :
    smart->wheelspeed.rear_right;
  msg.rv_rear_left = smart->wheelspeed.rear_left_not_plausible ? 0.0 :
    smart->wheelspeed.rear_left;

  if (smart->engine.actual_gear == 7) {
    msg.rv_front_right = -msg.rv_front_right;
    msg.rv_front_left = -msg.rv_front_left;
    msg.rv_rear_right = -msg.rv_rear_right;
    msg.rv_rear_left = -msg.rv_rear_left;
  }

  msg.timestamp = timestamp;
  msg.host = carmen_get_host();

  err = IPC_publishData(SMART_STATUS_MESSAGE_NAME, &msg);
  carmen_test_ipc_exit(err, "Could not publish",
    SMART_STATUS_MESSAGE_NAME);
}

void smart_publish_odometry(double x, double y, double theta, double tv,
  double rv, double acceleration, double timestamp) {
  carmen_base_odometry_message msg;
  IPC_RETURN_TYPE err;

  msg.x = x;
  msg.y = y;
  msg.theta = theta;

  msg.tv = tv;
  msg.rv = rv;

  msg.acceleration = acceleration;

  msg.timestamp = timestamp;
  msg.host = carmen_get_host();

  err = IPC_publishData(CARMEN_BASE_ODOMETRY_NAME, &msg);
  carmen_test_ipc_exit(err, "Could not publish",
    CARMEN_BASE_ODOMETRY_NAME);
}

void smart_publish_init_odometrypos(carmen_point_p odometrypos,
  double timestamp) {
  smart_init_odometrypos_message msg;
  IPC_RETURN_TYPE err;

  msg.odometrypos = *odometrypos;

  msg.timestamp = timestamp;
  msg.host = carmen_get_host();

  err = IPC_publishData(SMART_INIT_ODOMETRYPOS_MESSAGE_NAME, &msg);
  carmen_test_ipc_exit(err, "Could not publish",
    SMART_INIT_ODOMETRYPOS_MESSAGE_NAME);
}
