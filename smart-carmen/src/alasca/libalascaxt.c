#ifndef unix
#define WIN32
#include <windows.h>
#include <winsock.h>
#else
#define closesocket close
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <errno.h>
#endif

#include <stdio.h>
#include <string.h>

#include "alascaxt.h"

inline unsigned int axt_to_uint32(unsigned int axt_uint32) {
  unsigned char* buf = (unsigned char*)&axt_uint32;

  return (((unsigned int) buf[0]) << 24) 
    + (((unsigned int) buf[1]) << 16)
    + (((unsigned int) buf[2]) << 8)  
    + (((unsigned int) buf[3]));  
}

inline unsigned short axt_to_uint16(unsigned short axt_uint16) {
  unsigned char* buf = (unsigned char*)&axt_uint16;

  return (((unsigned int) buf[0]) << 8)
    + (((unsigned int) buf[1]));  
}

inline short axt_to_int16(short axt_int16) {
  unsigned char* buf = (unsigned char*)&axt_int16;

  return (((unsigned int) buf[0]) << 8)
    + (((unsigned int) buf[1]));
}

inline float axt_decode_angle(int angle) {
  return angle/10000.0;
}

inline float axt_decode_distance(int n) {
  if (n < -10000)
    return n*0.1+900.0;
  else {
    if (n > 10000)
      return n*0.1-900.0;
    else 
      return n*0.01;
  }
}

double axt_get_time() {
  struct timeval time;
  gettimeofday(&time, 0);

  return time.tv_sec+time.tv_usec*1e-6;
}

unsigned int axt_recv(int sd, unsigned char* data, unsigned int num) {
  unsigned int num_read = 0;
  struct timeval time;
  fd_set set;
  int error;

  while (num_read < num) {
    time.tv_sec = 0;
    time.tv_usec = AXT_RECV_TIMEOUT*1e6;

    FD_ZERO(&set);
    FD_SET(sd, &set);

    error = select(sd+1, &set, NULL, NULL, &time);
    if (error == 0)
      return 0;

    unsigned int n;
    n = read(sd, &data[num_read], num-num_read);
    if ((n < 0) && (errno != EWOULDBLOCK))
      return 0;
    if (n > 0)
      num_read += n;
  }
  
  return num_read;
}

unsigned int axt_align(int sd) {
	unsigned char buf[4];
	int foundmagic = 0;
	int i = 0;
	int n = 0;

	while (!foundmagic) {
		n = axt_recv(sd, buf, sizeof(buf));
		if ((buf[0] == 0xaf) && (buf[1] == 0xfe) && (buf[2] == 0xc0) &&
      (buf[3] == 0xc0)) 
			foundmagic = 1;

		i += n;
	}

	return i-4;
}

int axt_connect(char *host, int *sd) {
	struct  sockaddr_in sad; /* structure to hold an IP address     */
	struct  hostent  *ptrh;  /* pointer to a host table entry       */
	struct  protoent *ptrp;  /* pointer to a protocol table entry   */
	
	int port;

	memset((char *)&sad,0,sizeof(sad)); /* clear sockaddr structure */
	sad.sin_family = AF_INET;           /* set family to Internet     */
	port = AXT_DEFAULT_PORT;
	sad.sin_port =  htons((u_short)port);

	printf("Trying to connect to host %s on port %d\n", host,sad.sin_port); 

	ptrh = gethostbyname(host);
	if ( ((char *)ptrh) == NULL ) {
		fprintf(stderr,"Invalid host: %s\n", host);
		exit(1);
	}
	memcpy(&sad.sin_addr, ptrh->h_addr, ptrh->h_length);
	
	/* Map TCP transport protocol name to protocol number. */
	if ( ((int)(ptrp = getprotobyname("tcp"))) == 0) {
		fprintf(stderr, "Cannot map \"tcp\" to protocol number");
		exit(1);
	}

	/* Create a socket. */
	*sd = socket(AF_INET, SOCK_STREAM, ptrp->p_proto);
	if (*sd < 0) {
		fprintf(stderr, "Socket creation failed\n");
		exit(1);
	}

	/* Connect the socket to the specified server. */
	if (connect(*sd, (struct sockaddr *)&sad, sizeof(sad)) < 0) {
		fprintf(stderr,"Connect failed\n");
		exit(1);
	}
	return 1;
}

void axt_close(int sd) {
	closesocket(sd);
}

void axt_dump_scan(FILE* f, AXT_SCAN_STR *scan) {
	int i;
	fprintf(f, "# %d  %d %d %d %f %f %d %d \n"
					, scan->header.version
					, scan->header.scanner_type
					, scan->header.ecu_id
					, scan->header.timestamp
					, scan->header.start_angle
					, scan->header.end_angle
					, scan->header.scan_counter
					, scan->header.num_points);

			for (i=0;i<scan->header.num_points;i++)
				{
					//if (scan->points[i].scanner_id == 67) 
					fprintf(f, "%d %d %d %d %f %f %f %d\n"
						,scan->points[i].scanner_id 
						,scan->points[i].channel 
						,scan->points[i].sub_channel 
						,scan->points[i].point_status 
						,scan->points[i].x 
						,scan->points[i].y 
						,scan->points[i].z 
						,scan->points[i].echo_pulse_width); 
				}
}

void axt_dump_scan_line(FILE* f, AXT_SCAN_STR *scan) {
	int i;
	fprintf(f, "%d %d %d %d %f %f %d %d "
					, scan->header.version
					, scan->header.scanner_type
					, scan->header.ecu_id
					, scan->header.timestamp
					, scan->header.start_angle
					, scan->header.end_angle
					, scan->header.scan_counter
					, scan->header.num_points);

			for (i=0;i<scan->header.num_points;i++)
				{
					//if (scan->points[i].scanner_id == 67)
					fprintf(f, "%d %d %d %d %f %f %f %d "
									,scan->points[i].scanner_id 
									,scan->points[i].channel 
									,scan->points[i].sub_channel 
									,scan->points[i].point_status 
									,scan->points[i].x 
									,scan->points[i].y 
									,scan->points[i].z 
									,scan->points[i].echo_pulse_width); 
				}
			fprintf(f, "\n");
}

int axt_get_scan_from_file(FILE* f, AXT_SCAN_STR *scan) {
	int i;
	
	fscanf(f, "# %d  %d %d %d %f %f %d %d \n"
				 , &scan->header.version
				 , &scan->header.scanner_type
				 , &scan->header.ecu_id
				 , &scan->header.timestamp
				 , &scan->header.start_angle
				 , &scan->header.end_angle
				 , &scan->header.scan_counter
				 , &scan->header.num_points);

			for (i=0;i<scan->header.num_points;i++)
				{
					fscanf(f, "%d %d %d %d %f %f %f %d\n"
								 ,&scan->points[i].scanner_id 
								 ,&scan->points[i].channel 
								 ,&scan->points[i].sub_channel 
								 ,&scan->points[i].point_status 
								 ,&scan->points[i].x 
								 ,&scan->points[i].y 
								 ,&scan->points[i].z 
								 ,&scan->points[i].echo_pulse_width); 
				}
			return 0;
}

int axt_message_to_scan(unsigned char* msg, AXT_SCAN_STR *scan, 
  double timestamp) {
  unsigned char buf[4];
  int n=0;
  int i=0;
  
  int temp_x;
  int temp_y;
  int temp_z;
  
  AXT_SCAN_HDR_RAW *header = (AXT_SCAN_HDR_RAW*)msg;
  AXT_SCAN_POINT_RAW *points = (AXT_SCAN_POINT_RAW*)
    &msg[sizeof(AXT_SCAN_HDR_RAW)];

  scan->header.version = header->version;
  scan->header.scanner_type = header->scanner_type;
  scan->header.ecu_id = header->ecu_id;

  scan->header.timestamp = axt_to_uint32(header->timestamp);
  scan->header.timestamp_sync = timestamp;  
  scan->header.start_angle = axt_decode_angle(
    axt_to_int16(header->start_angle));
  scan->header.end_angle = axt_decode_angle(
    axt_to_int16(header->end_angle));
  scan->header.scan_counter = axt_to_uint16(header->scan_counter);
  scan->header.num_points = axt_to_uint16(header->num_points);
    
  for (i = 0; i < scan->header.num_points; i++) {
    scan->points[i].scanner_id = points[i].scanner_id;
    scan->points[i].channel = points[i].channel;
    scan->points[i].sub_channel = points[i].sub_channel;
    scan->points[i].point_status = points[i].point_status;

    scan->points[i].x = axt_decode_distance(
      axt_to_int16(points[i].x));
    scan->points[i].y = axt_decode_distance(
      axt_to_int16(points[i].y));
    scan->points[i].z = axt_decode_distance(
      axt_to_int16(points[i].z));
    scan->points[i].echo_pulse_width = axt_to_uint16(
      points[i].echo_pulse_width);
  }

  return 0;
}

int axt_parse(int sd, AXT_SCAN_STR *scan) {
  unsigned int axt_size, size;
  unsigned int type;
  
  axt_align(sd);

  if (axt_recv(sd, (unsigned char*)&axt_size, sizeof(unsigned int))) {
    size = axt_to_uint32(axt_size);
    unsigned char pkg[size];
    
    if (axt_recv(sd, pkg, size) == size) {
      AXT_PKG_HDR_RAW *header = (AXT_PKG_HDR_RAW*)pkg;
      type = axt_to_uint32(header->type);
      
      switch (type) {
        case AXT_MSG_TYPE_SCAN:
          axt_message_to_scan(&pkg[sizeof(AXT_PKG_HDR_RAW)], scan, 
            axt_get_time());
          break;
        default:
          // unknown message type
          break;
      }

      return type;
    }
  }
	  
  return 0;
}
