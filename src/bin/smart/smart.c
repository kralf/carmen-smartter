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

#include <carmen/carmen.h>

#include <libsmartter/smart.h>
#include <libsmartter/lss.h>
#include <libsmartter/cst.h>
#include <libsmartter/handlers.h>

#include "smart_ipc.h"

#define SMART_UPDATE_INTERVAL 5.0

int quit = 0;
unsigned char msg[8];

MCTRL_CONFIG control_config = {
  1,                // accept_brake_ajustment

  0,                // enable_gas_ctrl
  0,                // enable_brake_ctrl
  0,                // enable_steering_ctrl

  MCTRL_NO_SOURCE,  // get_cmd_from

  -1.0,             // car_min_acc
  1.0,              // car_max_acc
};

double brake_accurate_max_position = 0.0;
MCTRL_ACC_INPUT acc_control_input;

char* vcan_dev;

double axle_base = 1.812;
double wheel_base = 1.35;
double wheel_radius = 0.27;

double control_freq = 100.0;

int control_steering_enable = 0;
double control_steering_p = 15.0;
double control_steering_i = 0.2;
double control_steering_d = 0.3;
double control_max_steering = 0.436319;

int control_velocity_enable = 0;
double control_max_tv = 5.0;
double control_max_rv = 0.5;
double control_min_acceleration = -1.0;
double control_max_acceleration = 1.0;

double control_timeout_motion_command = 0.0;

double tv = 0.0;
double steering_angle = 0.0;

double last_motion_command = 0.0;

double odometry_x = 0.0;
double odometry_y = 0.0;
double odometry_theta = 0.0;
double odometry_tv = 0.0;
double odometry_rv = 0.0;
double odometry_acceleration = 0.0;

smart_init_odometrypos_message init;
smart_velocity_message smart_velocity;
carmen_base_velocity_message base_velocity;

void smart_sigint_handler(int q __attribute__((unused))) {
  quit = 1;
}

double smart_theta_mod_2pi(double theta) {
  while (theta > M_PI)
    theta -= (2.0*M_PI);
  while (theta < -M_PI)
    theta += (2.0*M_PI);

  return theta;
}

int smart_read_parameters(int argc, char **argv) {
  int num_params;
  carmen_param_t params[] = {
    {"smart", "vcan_dev", CARMEN_PARAM_STRING, &vcan_dev, 0, NULL},

    {"smart", "axle_base", CARMEN_PARAM_DOUBLE, &axle_base, 0, NULL},
    {"smart", "wheel_base", CARMEN_PARAM_DOUBLE, &wheel_base, 0, NULL},
    {"smart", "wheel_radius", CARMEN_PARAM_DOUBLE, &wheel_radius, 0, NULL},

    {"smart", "control_freq", CARMEN_PARAM_DOUBLE, &control_freq, 0, NULL},

    {"smart", "control_steering_enable", CARMEN_PARAM_ONOFF,
      &control_steering_enable, 0, NULL},
    {"smart", "control_steering_p", CARMEN_PARAM_DOUBLE,
      &control_steering_p, 1, NULL},
    {"smart", "control_steering_i", CARMEN_PARAM_DOUBLE,
      &control_steering_i, 1, NULL},
    {"smart", "control_steering_d", CARMEN_PARAM_DOUBLE,
      &control_steering_d, 1, NULL},
    {"smart", "control_max_steering", CARMEN_PARAM_DOUBLE,
      &control_max_steering, 0, NULL},

    {"smart", "control_velocity_enable", CARMEN_PARAM_ONOFF,
      &control_velocity_enable, 0, NULL},
    {"smart", "control_max_tv", CARMEN_PARAM_DOUBLE,
      &control_max_tv, 0, NULL},
    {"smart", "control_max_rv", CARMEN_PARAM_DOUBLE,
      &control_max_rv, 0, NULL},
    {"smart", "control_min_acceleration", CARMEN_PARAM_DOUBLE,
      &control_min_acceleration, 0, NULL},
    {"smart", "control_max_acceleration", CARMEN_PARAM_DOUBLE,
      &control_max_acceleration, 0, NULL},

    {"smart", "control_timeout_motion_command", CARMEN_PARAM_DOUBLE,
      &control_timeout_motion_command, 0, NULL},
  };

  num_params = sizeof(params)/sizeof(carmen_param_t);
  carmen_param_install_params(argc, argv, params, num_params);

  return num_params;
}

int smart_open() {
  int result = -1;
  fprintf(stderr, "Opening Smart CAN connection... ");

  if (!canHWInit(SMART_VCAN_BUSID, SMART_VCAN_BAUD_RATE, vcan_dev) && \
    smart_sensor_update()) {
    cstDoubleSetVoltage(SMART_ICAN_BUSID, 2.5, 1);
    cstDoubleSetVoltage(SMART_ICAN_BUSID, 0.0, 0);

    msg[0]=0;
    msg[1]=0;
    msg[2]=0;
    msg[3]=0;
    msg[4]=0;
    msg[5]=1;
    msg[6]=1;
    msg[7]=0;

    result = my_send_can_message(SMART_ICAN_BUSID, 0x88, msg);
  }

  carmen_ipc_sleep(0.5);

  fprintf(stderr, result ? "failure\n" : "success\n");
  return result;
}

int smart_close() {
  fprintf(stderr, "Closing Smart CAN connection... ");

  canHWCleanup(SMART_VCAN_BUSID);

  fprintf(stderr, "done\n");
  return 0;
}

int smart_brake_calibrate() {
  int result = -1;
  fprintf(stderr, "Calibrating brake... \n");

  if (smart_sensor_update() && (smart.status.v_curr < 0.8)) {
    result = 0;
    carmen_ipc_sleep(1.0);

    if (lss_set_max_drive_current(SMART_ICAN_BUSID, 0.5, 1.0))
      result = -1;

    if (!result && !lss_test_stroke_min(SMART_ICAN_BUSID)) {
      fprintf(stderr, "  minimum brake motor stroke\n");
      carmen_ipc_sleep(7.5);
    }
    else
      result = -1;

    if (!result && !lss_test_stroke_max(SMART_ICAN_BUSID)) {
      fprintf(stderr, "  maximum brake motor stroke\n");
      carmen_ipc_sleep(6.0);
    }
    else
      result = -1;

    if (!result && !lss_set_position(SMART_ICAN_BUSID, LSS_MAX_POSITION)) {
      fprintf(stderr, "  maximum brake position... ");
      carmen_ipc_sleep(3.0);

      while (lss.actual_position == 0.0) {
        lss_get_actual_position(SMART_ICAN_BUSID);
        smart_sensor_update();

        carmen_ipc_sleep(0.1);
      }

      brake_accurate_max_position = lss.actual_position;
      fprintf(stderr, "%.2f mm\n", brake_accurate_max_position);
    }
    else
      result = -1;

    if (!result) {
      fprintf(stderr, "  minimum brake position... ");

      smart_sensor_update();
      carmen_ipc_sleep(1.0);

      while (smart.status.brake_light_on) {
        smart_sensor_update();
        if (lss_get_actual_position(SMART_ICAN_BUSID) ||
          lss_set_position(SMART_ICAN_BUSID, (lss.actual_position-1.0))) {
          result = -1;
          break;
        }
      }

      fprintf(stderr, "%.2f mm\n", lss.actual_position);
    }

    if (!result) {
      lss.homing_done = 1;

      acc_control_input.brake_accurate_offset = lss.actual_position-2.0;
      acc_control_input.brake_accurate_range = brake_accurate_max_position-
        lss.actual_position;
      acc_control_input.brake_ready_to_serve = 1;

      control_config.accept_brake_ajustment = 0;

      if (!lss_set_max_drive_current(SMART_ICAN_BUSID, 1.0, 1.0)) {
        fprintf(stderr, "  brake offset is %.2f mm\n",
          acc_control_input.brake_accurate_offset);
        fprintf(stderr, "  brake range is %.2f mm\n",
          acc_control_input.brake_accurate_range);
      }
      else
        result = -1;
    }
  }
  else
    fprintf(stderr, "  vehicle velocity exceeds safety limits\n");

  fprintf(stderr, result ? "failure\n" : "success\n");
  return result;
}

int smart_activate_esx() {
  int result = -1;
  fprintf(stderr, "Activating emergency stop system... ");

  int max_brake_stroke = LSS_MM_TO_INC(brake_accurate_max_position);

  msg[0] = 1;
  msg[1] = 0;
  msg[2] = 0;
  msg[3] = (unsigned char)(max_brake_stroke >> 8);
  msg[4] = (unsigned char)(max_brake_stroke);
  msg[5] = 0;
  msg[6] = 0;
  msg[7] = 0;

  result = my_send_can_message(SMART_ICAN_BUSID, 0x88, msg);

  fprintf(stderr, result ? "failure\n" : "success\n");
  return result;
}

void smart_control_init() {
  int i;

  fuzzy_acc_init();

  for (i = 0; i < (WINDOW); i++) {
    acc_control_input.velocity_raw[i] = 0.0;
    acc_control_input.velocity_filtered[i] = 0.0;
    acc_control_input.timestamp_v[i] = 0.0;
    acc_control_input.velocity_err[i] = 0.0;
  }
}

int smart_sensor_update() {
  return read_can_message(SMART_VCAN_BUSID);
}

void smart_control_steering(double steering_angle, double update_freq) {
  static double steering_voltage = 0.0;

  double current_steering_angle = smart.status.phi_curr;
  double current_steering_voltage = steering_voltage;
  double now = carmen_get_time();

  control_config.enable_steering_ctrl = 1;

  cstSteeringPID(control_steering_p, control_steering_i, control_steering_d,
    1.0/update_freq, steering_angle, current_steering_angle,
    current_steering_voltage, &steering_voltage);

  steering_voltage = (steering_voltage < STEERING_CONTROL_MIN_OUTPUT_VOLTAGE) ?
    STEERING_CONTROL_MIN_OUTPUT_VOLTAGE : steering_voltage;
  steering_voltage = (steering_voltage > STEERING_CONTROL_MAX_OUTPUT_VOLTAGE) ?
    STEERING_CONTROL_MAX_OUTPUT_VOLTAGE : steering_voltage;

  cstSetSteeringVoltage(SMART_ICAN_BUSID, steering_voltage);
}

void smart_control_velocity(double velocity, double update_time) {
  control_config.enable_gas_ctrl = 1;
  control_config.enable_brake_ctrl = 1;

  control_config.car_min_acc = control_min_acceleration;
  control_config.car_max_acc = control_max_acceleration;

  if (velocity == 0.0) {
    acc_control_input.gas_pedal_cmd = 0.0;
    acc_control_input.brake_pedal_cmd =
      acc_control_input.brake_accurate_offset+
      acc_control_input.brake_accurate_range;
  }
  else
    mctrl_accelerationControl(&acc_control_input, &control_config,
    &smart.status, velocity, &smart.engine, update_time);

  if (smart.status.brake_light_on)
    acc_control_input.gas_pedal_cmd = 0.0;

  cstSetPedalValue(SMART_ICAN_BUSID, acc_control_input.gas_pedal_cmd);
  lss_set_position(SMART_ICAN_BUSID, acc_control_input.brake_pedal_cmd);
}

void smart_integrate_odometry(double update_freq) {
  double odometry_last_tv = odometry_tv;
  double wheel_circum = 2.0*M_PI*wheel_radius;
  double sign = (smart.engine.actual_gear == 7) ? -1.0 : 1.0;

  if (!smart.wheelspeed.rear_right_not_plausible &&
    !smart.wheelspeed.rear_left_not_plausible) {
    odometry_tv = sign*wheel_circum*0.5*(smart.wheelspeed.rear_right+
      smart.wheelspeed.rear_left);
    odometry_rv = sign*wheel_circum/wheel_base*(smart.wheelspeed.rear_right-
      smart.wheelspeed.rear_left);
  }
  else {
    odometry_tv = 0.0;
    odometry_rv = 0.0;
  }

  if (update_freq > 0.0) {
    odometry_theta = smart_theta_mod_2pi(odometry_theta+odometry_rv/
      update_freq);
    odometry_x += odometry_tv*cos(odometry_theta)/update_freq;
    odometry_y += odometry_tv*sin(odometry_theta)/update_freq;;
  }

  odometry_acceleration = (odometry_tv-odometry_last_tv)*update_freq;
}

void smart_control_cycle(double tv, double steering_angle, double update_time,
  double update_freq) {
  if (control_velocity_enable)
    smart_control_velocity(tv, update_time);
  if (control_steering_enable)
    smart_control_steering(steering_angle, update_freq);
}

void smart_init_odometrypos_handler(smart_init_odometrypos_message*
  init) {
  odometry_x = init->odometrypos.x;
  odometry_y = init->odometrypos.y;
  odometry_theta = init->odometrypos.theta;
}

void smart_velocity_handler(smart_velocity_message* velocity) {
  tv = (velocity->tv > control_max_tv) ? control_max_tv : velocity->tv;

  steering_angle = velocity->steering_angle;
  steering_angle = (steering_angle > control_max_steering) ?
    control_max_steering : steering_angle;
  steering_angle = (steering_angle < -control_max_steering) ?
    -control_max_steering : steering_angle;

  last_motion_command = carmen_get_time();
}

void base_velocity_handler(carmen_base_velocity_message* velocity) {
  tv = (velocity->tv > control_max_tv) ? control_max_tv : velocity->tv;
  tv = (velocity->tv < -control_max_tv) ? -control_max_tv : velocity->tv;

  double r = -tv/velocity->rv;
  if (tv != 0.0)
    steering_angle = atan2(r, axle_base);

  steering_angle = (steering_angle > control_max_steering) ?
    control_max_steering : steering_angle;
  steering_angle = (steering_angle < -control_max_steering) ?
    -control_max_steering : steering_angle;

  last_motion_command = carmen_get_time();
}

int main(int argc, char *argv[]) {
  smart_ipc_initialize(argc, argv);
  smart_read_parameters(argc, argv);

  if ((argc > 1) && !strcmp(argv[1], "--no-control")) {
    control_steering_enable = 0;
    control_velocity_enable = 0;
  }

  if (smart_open())
    carmen_die("ERROR: Smart initialization failed\n");
  if (control_velocity_enable && smart_brake_calibrate())
    carmen_die("ERROR: Brake calibration failed\n");

  if (control_velocity_enable && smart_activate_esx())
    carmen_die("ERROR: Emergency stop activation failed\n");

  smart_control_init();

  smart_subscribe_init_odometrypos_message(&init,
    (carmen_handler_t)smart_init_odometrypos_handler,
    CARMEN_SUBSCRIBE_LATEST);

  smart_subscribe_velocity_message(&smart_velocity,
    (carmen_handler_t)smart_velocity_handler, CARMEN_SUBSCRIBE_LATEST);
  carmen_base_subscribe_velocity_message(&base_velocity,
    (carmen_handler_t)base_velocity_handler, CARMEN_SUBSCRIBE_LATEST);

  signal(SIGINT, smart_sigint_handler);

  double interval_start = 0.0;
  double cycle_start = 0.0;
  unsigned int num_cycles = 0;

  while (!quit) {
    double now = carmen_get_time();
    double update_freq = (cycle_start > 0.0) ? 1.0/(now-cycle_start) : 0.0;
    interval_start = (interval_start > 0.0) ? interval_start : now;
    cycle_start = now;

    if (now - last_motion_command > control_timeout_motion_command) {
      tv             = 0.0;
      steering_angle = 0.0;
    }

    double update_start = carmen_get_time();
    if (smart_sensor_update()) {
      double update_time = 0.5*(carmen_get_time()+update_start);

      smart_integrate_odometry(update_freq);
      smart_control_cycle(tv, steering_angle, update_time, update_freq);

      smart_publish_status(&smart, update_time);
      smart_publish_odometry(odometry_x, odometry_y, odometry_theta,
        odometry_tv, odometry_rv, odometry_acceleration, update_time);
    }

    now = carmen_get_time();
    if ((now-cycle_start) < (1.0/control_freq))
      carmen_ipc_sleep(1.0/control_freq-(now-cycle_start));

    now = carmen_get_time();
    if ((now-interval_start) >= SMART_UPDATE_INTERVAL) {
      fprintf(stderr, "Update frequency is %4.2f Hz\n",
        num_cycles/(now-interval_start));

      num_cycles = 0;
      interval_start = now;
    }
    else
      num_cycles++;
  }

  smart_close();
  return 0;
}
