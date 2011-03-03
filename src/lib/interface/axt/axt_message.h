#ifndef AXT_MESSAGE_H
#define AXT_MESSAGE_H

#ifdef __cplusplus
extern "C" {
#endif

#define AXT_MESSAGE_NAME "axt_message"
#define AXT_MESSAGE_FMT "{uint,uint,uint,uint,uint,double,double,uint,<ubyte:1>,<ubyte:1>,<float:1>,<float:1>,<float:1>,double}"

typedef struct {
  unsigned int num_points;
  unsigned int version;
  unsigned int scanner_type;
  unsigned int ecu_id;
  unsigned int timestamp_sensor;
  double start_angle;
  double end_angle;
  unsigned int scan_counter;

  unsigned char *channel;
  unsigned char *point_status;
  
  float *x;
  float *y;
  float *z;
  
  double timestamp;
} axt_message;

#ifdef __cplusplus
}
#endif

#endif
