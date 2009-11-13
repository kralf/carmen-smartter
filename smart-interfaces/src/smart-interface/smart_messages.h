#ifndef SMART_MESSAGES_H
#define SMART_MESSAGES_H

#ifdef __cplusplus
extern "C" {
#endif

#include <carmen/base_messages.h>

#define SMART_STATUS_MESSAGE_NAME "smart_status_message"
#define SMART_STATUS_MESSAGE_FMT "{double,int,double,double,double,double,double,double,double,string}"

typedef struct {
  double gas_pos;         //!< Gas pedal position of vehicle.
  int gear;               //!< Gear of vehicle.
  double steering_angle;  //!< Steering angle in [rad].

  double tv;              //!< Translational velocity of vehicle in [m/s].

  double rv_front_right;  //!< Rotational velocity of right front wheel in [rps].
  double rv_front_left;   //!< Rotational velocity of left front wheel in [rps].
  double rv_rear_right;   //!< Rotational velocity of right rear wheel in [rps].
  double rv_rear_left;    //!< Rotational velocity of left rear wheel in [rps].

  double timestamp;
  char* host;
} smart_status_message;

#define SMART_INIT_ODOMETRYPOS_MESSAGE_NAME "smart_init_odometrypos_message"
#define SMART_INIT_ODOMETRYPOS_MESSAGE_FMT "{{double,double,double},double,string}"

typedef struct {
  carmen_point_t odometrypos;

  double timestamp;
  char *host;
} smart_init_odometrypos_message;

#define SMART_VELOCITY_MESSAGE_NAME "smart_velocity_message"
#define SMART_VELOCITY_MESSAGE_FMT "{double,double,double,string}"

typedef struct {
  double tv;              //!< Translational velocity of vehicle in [m/s].
  double steering_angle;  //!< Steering angle in [rad].

  double timestamp;
  char* host;
} smart_velocity_message;

#ifdef __cplusplus
}
#endif

#endif
