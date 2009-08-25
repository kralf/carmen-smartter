#ifndef GYRO_MESSAGES_H
#define GYRO_MESSAGES_H

#ifdef __cplusplus
extern "C" {
#endif

#define GYRO_INTEGRATED_MESSAGE_NAME "gyro_integrated_message"
#define GYRO_INTEGRATED_MESSAGE_FMT "{double,double,string}"

typedef struct {
  double theta;         //!< Integrated angle measured by the gyro in [rad].
  
  double timestamp;
  char* host;
} gyro_integrated_message;

#ifdef __cplusplus
}
#endif

#endif
