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

#include <dsp3000/libdsp3000.h>

#include "gyro_ipc.h"

#define GYRO_CONNECTION_TIMEOUT 0.5
#define GYRO_UPDATE_INTERVAL 5.0

int quit = 0;

char* serial_dev;

int inverse = 0;
int hw_integrate = 0;

unsigned long measurement_id = 0;
double integrated_theta = 0.0;

int integrated_init = 0;
double integrated_offset = 0.0;

DSP3000_Data dsp3000;

void gyro_sigint_handler(int q __attribute__((unused))) {
  quit = 1;
}

double gyro_theta_mod_2pi(double theta) {
  while (theta > M_PI)
    theta -= (2.0*M_PI);
  while (theta < -M_PI)
    theta += (2.0*M_PI);

  return theta;
}

int gyro_read_parameters(int argc, char **argv) {
  int num_params;
  carmen_param_t params[] = {
    {"gyro", "serial_dev", CARMEN_PARAM_STRING, &serial_dev, 0, NULL},

    {"gyro", "hw_integrate", CARMEN_PARAM_ONOFF, &hw_integrate, 0, NULL},
    {"gyro", "inverse", CARMEN_PARAM_ONOFF, &inverse, 0, NULL},
  };

  num_params = sizeof(params)/sizeof(carmen_param_t);
  carmen_param_install_params(argc, argv, params, num_params);

  return num_params;
}

int gyro_open() {
  int result = -1;
  fprintf(stderr, "Opening gyro RS232 connection... ");

  integrated_init = 0;
  
  if (DSP3000_OpenPort(serial_dev, GYRO_CONNECTION_TIMEOUT*1e3) > 0)
    result = !DSP3000_SwitchMode(hw_integrate ? MODULE_NAME_DSP3000_INT :
      MODULE_NAME_DSP3000_INC);

  fprintf(stderr, result ? "failure\n" : "success\n");
  return result;
}

int gyro_close() {
  int result;
  fprintf(stderr, "Closing gyro RS232 connection... ");

  result = DSP3000_ClosePort();

  fprintf(stderr, result ? "failure\n" : "success\n");
  return result;
}

int gyro_capture() {
  unsigned char buffer[DSP_SIZE_BUFF];
  TIMEVAL time;

  if (DSP3000_ParseStream(buffer, DSP_SIZE_BUFF, &time))
    return DSP3000_FillContainer(buffer, &time, hw_integrate ?
      MODULE_NAME_DSP3000_INT : MODULE_NAME_DSP3000_INC,
      measurement_id++, &dsp3000);
}

void gyro_integrate_theta() {
  if (hw_integrate) {
    if (integrated_init) {
      if (inverse)
        integrated_theta = gyro_theta_mod_2pi(integrated_offset-dsp3000.data);
      else
        integrated_theta = gyro_theta_mod_2pi(dsp3000.data-integrated_offset);
    }
    else {
      integrated_theta = 0.0;
      integrated_offset = gyro_theta_mod_2pi(dsp3000.data);
      
      integrated_init = 1;
    }
  }
  else {
    if (inverse)
      integrated_theta = gyro_theta_mod_2pi(integrated_theta-dsp3000.data);
    else
      integrated_theta = gyro_theta_mod_2pi(integrated_theta+dsp3000.data);
  }
}

int main(int argc, char *argv[]) {
  gyro_ipc_initialize(argc, argv);
  gyro_read_parameters(argc, argv);

  signal(SIGINT, gyro_sigint_handler);

  if (gyro_open())
    carmen_die("ERROR: Gyro initialization failed\n");

  double interval_start = 0.0;
  unsigned int num_cycles = 0;

  while (!quit) {
    double now = carmen_get_time();
    interval_start = (interval_start > 0.0) ? interval_start : now;

    double update_start = carmen_get_time();
    if (gyro_capture()) {
      double update_time = 0.5*(carmen_get_time()+update_start);

      gyro_integrate_theta();

      gyro_publish_integrated(integrated_theta, update_time);
    }

    now = carmen_get_time();
    if ((now-interval_start) >= GYRO_UPDATE_INTERVAL) {
      fprintf(stderr, "Measurement frequency is %4.2f Hz\n",
        num_cycles/(now-interval_start));

      num_cycles = 0;
      interval_start = now;
    }
    else
      num_cycles++;
  }

  gyro_close();
  return 0;
}
