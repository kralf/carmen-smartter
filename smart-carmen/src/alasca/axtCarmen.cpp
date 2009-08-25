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
