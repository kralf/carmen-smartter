#ifndef LOCFILTER_MESSAGES_H
#define LOCFILTER_MESSAGES_H

#ifdef __cplusplus
extern "C" {
#endif

#define LOCFILTER_FILTEREDPOS_MESSAGE_NAME "locfilter_filteredpos_message"
#define LOCFILTER_FILTEREDPOS_MESSAGE_FMT "{{double,double,double},{double,double,double},double,string}"

typedef struct {
  carmen_point_t filteredpos;
  carmen_point_t odometrypos;

  double timestamp;
  char *host;
} locfilter_filteredpos_message;

#ifdef __cplusplus
}
#endif

#define LOCFILTER_INIT_MESSAGE_NAME "locfilter_init_message"
#define LOCFILTER_INIT_MESSAGE_FMT "{{double,double,double},double,string}"

typedef struct {
  carmen_point_t initpos;

  double timestamp;
  char *host;
} locfilter_init_message;

#ifdef __cplusplus
}
#endif

#endif
