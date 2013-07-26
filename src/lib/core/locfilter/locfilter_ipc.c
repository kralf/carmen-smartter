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

#include "locfilter_ipc.h"

int locfilter_ipc_initialize(int argc, char *argv[]) {
  IPC_RETURN_TYPE err;

  carmen_ipc_initialize(argc, argv);
  carmen_param_check_version(argv[0]);

  err = IPC_defineMsg(LOCFILTER_FILTEREDPOS_MESSAGE_NAME, IPC_VARIABLE_LENGTH,
    LOCFILTER_FILTEREDPOS_MESSAGE_FMT);
  carmen_test_ipc_exit(err, "Could not define message",
    LOCFILTER_FILTEREDPOS_MESSAGE_NAME);

  err = IPC_defineMsg(LOCFILTER_INIT_FILTEREDPOS_MESSAGE_NAME,
    IPC_VARIABLE_LENGTH, LOCFILTER_INIT_FILTEREDPOS_MESSAGE_FMT);
  carmen_test_ipc_exit(err, "Could not define message",
    LOCFILTER_INIT_FILTEREDPOS_MESSAGE_NAME);

  return 0;
}

void locfilter_publish_filteredpos(carmen_point_p filteredpos, carmen_point_p
  odometrypos, double tv, double rv, double timestamp) {
  locfilter_filteredpos_message msg;
  IPC_RETURN_TYPE err;

  msg.filteredpos = *filteredpos;
  msg.odometrypos = *odometrypos;

  msg.tv = tv;
  msg.rv = rv;

  msg.timestamp = timestamp;
  msg.host = carmen_get_host();

  err = IPC_publishData(LOCFILTER_FILTEREDPOS_MESSAGE_NAME, &msg);
  carmen_test_ipc_exit(err, "Could not publish",
    LOCFILTER_FILTEREDPOS_MESSAGE_NAME);
}

void locfilter_publish_init_filteredpos(carmen_point_p filteredpos,
  double timestamp) {
  locfilter_init_filteredpos_message msg;
  IPC_RETURN_TYPE err;

  msg.filteredpos = *filteredpos;

  msg.timestamp = timestamp;
  msg.host = carmen_get_host();

  err = IPC_publishData(LOCFILTER_INIT_FILTEREDPOS_MESSAGE_NAME, &msg);
  carmen_test_ipc_exit(err, "Could not publish",
    LOCFILTER_INIT_FILTEREDPOS_MESSAGE_NAME);
}
