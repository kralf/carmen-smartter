// axtCarmen.cpp
/*! Carmen Modul for IBEO Alasca XT Laser Scanner */

#define AXT_UPDATE_INTERVAL 5.0
#define MAX_POINTS 5000

#include <stdio.h>

#include <libaxt/axt.h>

#include "axt_interface.h"
#include "axt_messages.h"

unsigned int version;
unsigned int scanner_type;
unsigned int ecu_id;
double start_angle;
double end_angle;
unsigned int scan_counter;
unsigned int timestamp_sensor;

unsigned int num_points;
float x[MAX_POINTS], y[MAX_POINTS], z[MAX_POINTS];
unsigned char channel[MAX_POINTS], point_status[MAX_POINTS];

double timestamp;

void process_scan(AXT_SCAN_STR *scan) {
  size_t i;
  num_points = scan->header.num_points;
  version = scan->header.version;
  scanner_type = scan->header.scanner_type;
  ecu_id = scan->header.ecu_id;
  timestamp_sensor = scan->header.timestamp;
  start_angle = scan->header.start_angle;
  end_angle = scan->header.end_angle;
  scan_counter = scan->header.num_points;
  
  float x_max, x_min, y_max, y_min;
  x_max = -1000.0; x_min = 1000.0;    
  y_max = -1000.0; y_min = 1000.0;

  for (i = 0; i < num_points; i++) {
    channel[i] = scan->points[i].channel;
    point_status[i] = scan->points[i].point_status;
    x[i] = scan->points[i].x;
    y[i] = scan->points[i].y;
    z[i] = scan->points[i].z;

    if( x[i] < x_min) x_min = x[i];
    else if( x[i] > x_max) x_max = x[i];

    if( y[i] < y_min) y_min = y[i];
    else if( y[i] > y_max) y_max = y[i];
  }
  
  timestamp = scan->header.timestamp_sync;
}
                                
int main(int argc, char** argv) {
  AXT_SCAN_STR scan;
  int sd = 0;

  // Init CARMEN stuff
  char tmp[256];
  strcpy(tmp, "alascaxt");
  char** argv_tmp = (char**)malloc(sizeof(char*));
  argv_tmp[0] = tmp;

  fprintf(stderr, "Welcome to %s\n", argv[0]);
  carmen_ipc_initialize(1, argv_tmp);
  fprintf(stderr, "Initialized CARMEN connection...\n");

  axt_ipc_initialize(argc, argv);
  fprintf(stderr, "Message defined...\n");

  fprintf(stderr, "Trying to connect to laser...\n");
  axt_connect(AXT_DEFAULT_IP, &sd);

  double interval_start = axt_get_time();
  unsigned int num_scans = 0;

  while (1) {
    if (axt_parse(sd, &scan) == AXT_MSG_TYPE_SCAN) {
      process_scan(&scan);
      axt_publish(version, scanner_type, ecu_id, start_angle, end_angle,
        scan_counter, timestamp_sensor, num_points, x, y, z, channel,
        point_status, timestamp);
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
