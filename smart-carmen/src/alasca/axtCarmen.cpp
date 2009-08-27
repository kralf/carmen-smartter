// axtCarmen.cpp
/*! Carmen Modul for IBEO Alasca XT Laser Scanner */

#define AXT_UPDATE_INTERVAL 5.0

#include <iostream>
#include <string>

#include "alascaxt.h"

#include <alasca-interface/axt_interface.h>
#include <alasca-interface/axt_message.h>

using namespace std;

const int live(1);
const int playback(2);

void scan_to_message(AXT_SCAN_STR *scan, axt_message *msg) {
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
  msg->timestamp = scan->header.timestamp_sync;
}

void message_to_scan(axt_message *msg, AXT_SCAN_STR *scan) {
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

void axt_handler(axt_message *msg) {
  AXT_SCAN_STR scan;
  axt_handle_carmen_message(msg, &scan);
}

void handle_args(int argc, char** argv, string &filename, int &mode)
{
  if (argc<2)
    {
      cout << "usage: axtCarmen live|playback {filename} " << endl;
      exit(-1);
    }
  if (!strcmp("live",argv[1]))
    {
      cout << "Live mode" << endl;
      mode = live;
    }
  else
    if (!strcmp("playback",argv[1]))
      {
        cout << "Playback mode" << endl;
        if (argc<3)
          {
            cout << "error: no filename specified" << endl;
            exit(-1);
          }
        else
          {
            filename = argv[2];
            mode = playback;
          }
      }
    else cout << "usage: axtCarmen live|playback {filename} " << endl;

}

int main(int argc, char** argv) 
{
  string filename;
  int mode(0);
  
  int sd(0); 

  AXT_SCAN_STR scan;
  axt_message msg;
  axt_message_alloc(&msg);

  handle_args(argc, argv, filename, mode);

  // Init CARMEN stuff
  char tmp[256];
  strcpy(tmp, "alascaxt");
  char** argv_tmp = (char**)malloc(sizeof(char*));
  argv_tmp[0] = tmp;

  cout << "Welcome to axtCarmen" << endl;
  carmen_ipc_initialize(1, argv_tmp);
  cout << "initialized CARMEN connection..." << endl;
  axt_define_message();
  cout << "message defined..." << endl;

  if (mode == playback) {
    FILE *f;
    f = fopen("../logs/firstscan_corridor_blocked_at_first_door.log", "r");
    axt_get_scan_from_file(f, &scan);

    while (1) {
      axt_send_message(&scan, &msg);
      IPC_listen(5);
    }
  }
  else if (mode == live) {
    cout << "Trying to connect to laser... " << endl;
    axt_connect(AXT_DEFAULT_IP, &sd);

    double interval_start = axt_get_time();
    unsigned int num_scans = 0;

    while (1) {
      if (axt_parse(sd, &scan) == AXT_MSG_TYPE_SCAN) {
        axt_send_message(&scan, &msg);
        num_scans++;
      }

      double now = axt_get_time();
      if (now-interval_start >= AXT_UPDATE_INTERVAL) {
        fprintf(stderr, "Scan frequency is %4.2fHz\n", 
          num_scans/(now-interval_start));

        num_scans = 0;
        interval_start = now;
      } 
    }
  }

  axt_message_dealloc(&msg);
}
