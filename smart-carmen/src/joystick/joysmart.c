/***************************************************************************
 *   Copyright (C) 2009 by Ralf Kaestner                                   *
 *   ralf.kaestner@gmail.com                                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <carmen/carmen.h>

#include <smart-interface/smart_messages.h>

#include "joyctrl.h"

char* joystick_dev;
int joystick_axis_long;
int joystick_axis_lat;
int joystick_btn_deadman;
int joystick_btn_activate;

double control_max_tv;
double control_max_steering;

carmen_joystick_type joystick;
int joystick_activated = 0;

void send_smart_velocity_command(double tv, double steering) {
  IPC_RETURN_TYPE err;
  static smart_velocity_message v;

  v.tv = tv;
  v.steering_angle = steering;
  v.timestamp = carmen_get_time();
  v.host = carmen_get_host();

  if (v.tv > control_max_tv)
    v.tv = control_max_tv;
  else if (v.tv < -control_max_tv)
    v.tv = -control_max_tv;

  if (v.steering_angle > control_max_steering)
    v.steering_angle = control_max_steering;
  else if (v.steering_angle < -control_max_steering)
    v.steering_angle = -control_max_steering;

  err = IPC_publishData(SMART_VELOCITY_MESSAGE_NAME, &v);
  carmen_test_ipc(err, "Could not publish", SMART_VELOCITY_MESSAGE_NAME);
}

void sig_handler(int x) {
  if (x == SIGINT) {
    send_smart_velocity_command(0.0, 0.0);

    carmen_close_joystick(&joystick);
    carmen_ipc_disconnect();

    printf("Disconnected from robot.\n");
    exit(0);
  }
}

void read_parameters(int argc, char **argv) {
  int num_params;

  carmen_param_t param_list[] = {
    {"joystick", "dev", CARMEN_PARAM_STRING, &joystick_dev, 0, NULL},
    {"joystick", "axis_longitudinal", CARMEN_PARAM_INT, &joystick_axis_long,
      0, NULL},
    {"joystick", "axis_lateral", CARMEN_PARAM_INT, &joystick_axis_lat,
      0, NULL},
    {"joystick", "button_deadman", CARMEN_PARAM_INT, &joystick_btn_deadman,
      0, NULL},
    {"joystick", "button_activate", CARMEN_PARAM_INT, &joystick_btn_activate,
      0, NULL},

    {"smart", "control_max_tv", CARMEN_PARAM_DOUBLE, &control_max_tv, 0, NULL},
    {"smart", "control_max_steering", CARMEN_PARAM_DOUBLE,
      &control_max_steering, 0, NULL}
  };

  num_params = sizeof(param_list)/sizeof(param_list[0]);
  carmen_param_install_params(argc, argv, param_list, num_params);
}

int main(int argc, char **argv) {
  double cmd_tv = 0, cmd_steering = 0;
  double timestamp;

  carmen_ipc_initialize(argc, argv);
  carmen_param_check_version(argv[0]);
  read_parameters(argc, argv);
  signal(SIGINT, sig_handler);

  fprintf(stderr,"Looking for joystick at device: %s\n", joystick_dev);

  if (carmen_initialize_joystick(&joystick, joystick_dev) < 0)
    carmen_die("Error: could not find joystick at device: %s\n", joystick_dev);
  else
    fprintf(stderr,"Joystick has %d axes and %d buttons\n\n",
    joystick.nb_axes, joystick.nb_buttons);

  fprintf(stderr,"1. Center the joystick.\n");
  if (joystick_btn_activate <= 0) {
    fprintf(stderr,"2. The joystick is activated.\n");
    joystick_activated = 1;
  }
  else {
    fprintf(stderr,"2. Press button \"%d\" to activate/deactivate the "
      "joystick.\n", joystick_btn_activate);
  }
  if (joystick_btn_deadman > 0)
    fprintf(stderr,"3. Hold button \"%d\" to keep the robot moving.\n\n",
      joystick_btn_deadman);

  timestamp = carmen_get_time();
  while (1) {
    carmen_ipc_sleep(0.01);

    if (carmen_get_joystick_state(&joystick) >= 0) {
      if ((joystick_btn_activate > 0) &&
        joystick.buttons[joystick_btn_activate-1]) {
        joystick_activated = !joystick_activated;

        if (!joystick_activated) {
          send_smart_velocity_command(0.0, 0.0);

          fprintf(stderr,"Joystick deactivated.\n");
        }
        else
          fprintf(stderr,"Joystick activated.\n");
      }

      if (joystick_activated) {
        if ((joystick_btn_deadman <= 0) ||
          joystick.buttons[joystick_btn_deadman-1]) {
          cmd_tv = (joystick.axes[joystick_axis_long]) ?
            joystick.axes[joystick_axis_long]/32767.0*control_max_tv : 0.0;
          cmd_steering = (joystick.axes[joystick_axis_lat]) ?
            joystick.axes[joystick_axis_lat]/32767.0*
            control_max_steering : 0.0;
        }
        else {
          cmd_tv = 0.0;
          cmd_steering = 0.0;
        }

        send_smart_velocity_command(cmd_tv, cmd_steering);
      }
    }
    else if (joystick_activated && carmen_get_time()-timestamp > 0.5) {
      send_smart_velocity_command(cmd_tv, cmd_steering);
      timestamp = carmen_get_time();
    }
  }

  sig_handler(SIGINT);
  return 0;
}
