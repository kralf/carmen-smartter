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
#include <carmen/base_interface.h>

#include <gyro-interface/gyro_interface.h>

#include "locfilter_ipc.h"

#define LOCFILTER_UPDATE_INTERVAL 5.0

#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))

int quit = 0;

locfilter_init_message init;
carmen_base_odometry_message odometry;
gyro_integrated_message gyro;

int initialized = 0;

double filtered_x = 0.0;
double filtered_y = 0.0;
double filtered_theta = 0.0;

double gyro_theta = 0.0;

unsigned int num_updates = 0;

void locfilter_sigint_handler(int q __attribute__((unused))) {
  quit = 1;
}

double locfilter_theta_mod_2pi(double theta) {
  while (theta > M_PI)
    theta -= (2.0*M_PI);
  while (theta < -M_PI)
    theta += (2.0*M_PI);

  return theta;
}

int locfilter_read_parameters(int argc, char **argv) {
  int num_params;
  carmen_param_t params[] = {
  };

  num_params = sizeof(params)/sizeof(carmen_param_t);
  carmen_param_install_params(argc, argv, params, num_params);

  return num_params;
}

void locfilter_initialize() {
  initialized = 1;

  if (odometry.timestamp > 0.0) {
    filtered_x = 0.0;
    filtered_y = 0.0;
  }
  else
    initialized &= 0;

  if (gyro.timestamp > 0.0) {
    filtered_theta = 0.0;
    gyro_theta = gyro.theta;
  }
  else
    initialized &= 0;
}

void locfilter_update(double update_freq) {
  if (initialized) {
    filtered_theta = locfilter_theta_mod_2pi(filtered_theta+gyro.theta-
      gyro_theta);
    filtered_x += odometry.tv*cos(filtered_theta)/update_freq;
    filtered_y += odometry.tv*sin(filtered_theta)/update_freq;
    gyro_theta = gyro.theta;

    carmen_point_t filteredpos = {filtered_x, filtered_y, filtered_theta};
    carmen_point_t odometrypos = {odometry.x, odometry.y, odometry.theta};
    double timestamp = max(odometry.timestamp, gyro.timestamp);

    locfilter_publish_filteredpos(&filteredpos, &odometrypos, timestamp);

    num_updates++;
  }
  else
    locfilter_initialize();
}

void locfilter_init_handler(locfilter_init_message* init) {
  filtered_x = init->initpos.x;
  filtered_y = init->initpos.y;
  filtered_theta = init->initpos.theta;
}

void locfilter_odometry_handler(carmen_base_odometry_message* odometry) {
  // wait for gyro
}

void gyro_integrated_handler(gyro_integrated_message* gyro) {
  static double timestamp = 0.0;

  locfilter_update(1.0/(gyro->timestamp-timestamp));
  timestamp = gyro->timestamp;
}

int main(int argc, char *argv[]) {
  locfilter_ipc_initialize(argc, argv);
  locfilter_read_parameters(argc, argv);

  odometry.timestamp = 0.0;
  gyro.timestamp = 0.0;

  locfilter_subscribe_init_message(&init,
    (carmen_handler_t)locfilter_init_handler, CARMEN_SUBSCRIBE_LATEST);

  carmen_base_subscribe_odometry_message(&odometry,
    (carmen_handler_t)locfilter_odometry_handler, CARMEN_SUBSCRIBE_LATEST);
  gyro_subscribe_integrated_message(&gyro,
    (carmen_handler_t)gyro_integrated_handler, CARMEN_SUBSCRIBE_LATEST);

  signal(SIGINT, locfilter_sigint_handler);

  double interval_start = 0.0;

  while (!quit) {
    double now = carmen_get_time();
    interval_start = (interval_start > 0.0) ? interval_start : now;

    carmen_ipc_sleep(0.01);

    now = carmen_get_time();
    if ((now-interval_start) >= LOCFILTER_UPDATE_INTERVAL) {
      fprintf(stderr, "Update frequency is %4.2f Hz\n",
        num_updates/(now-interval_start));

      num_updates = 0;
      interval_start = now;
    }
  }

  return 0;
}
