#ifndef ALASCA_XT_HT
#define  ALASCA_XT_HT

#ifdef __cplusplus
extern "C" {
#endif
#ifndef GENOM
#include <stdlib.h>
#include <stdio.h>
#endif

/*! Maximum number of points possible for Alasca XT */
#define AXT_MAX_SCAN_POINTS 8648

/*! Default TCP/IP port */
#define AXT_DEFAULT_PORT 12000

#define AXT_DEFAULT_IP "10.152.10.162"

/*! Receive timeout in [s] */
#define AXT_RECV_TIMEOUT 0.1

/*! Supported message types */
#define AXT_MSG_TYPE_SCAN 15

/*! To be verified */
#define AXT_MAX_DISTANCE 300.00

#define AXT_PT_STATUS_OK 0
#define AXT_PT_STATUS_INVALID 1
#define AXT_PT_STATUS_RAIN 2
#define AXT_PT_STATUS_GROUND 3
#define AXT_PT_STATUS_DIRT 4

/*! Raw package header format */
typedef struct _AXT_PKG_HDR_RAW{
  unsigned int type;
  unsigned int timestamp;
} AXT_PKG_HDR_RAW;

/*! Raw scan header format */
typedef struct _AXT_SCAN_HDR_RAW{
  unsigned char version;
  unsigned char scanner_type;
  unsigned char ecu_id;
  unsigned char unknown;

  unsigned int timestamp;
  short start_angle;
  short end_angle;
  unsigned short scan_counter;
  unsigned short num_points;
} AXT_SCAN_HDR_RAW;

/*! Raw scan point format */
typedef struct _AXT_SCAN_POINT_RAW{
  unsigned char scanner_id;
  unsigned char channel;
  unsigned char sub_channel;
  unsigned char point_status;

  unsigned short x;
  unsigned short y;
  unsigned short z;
  short echo_pulse_width;
} AXT_SCAN_POINT_RAW;

/*! Header of a scan message */
typedef struct _AXT_SCAN_HDR_STR{
	unsigned int version; ///< Version Number of the protocol specification
	unsigned int scanner_type; ///< Alasca XT is type 3
	unsigned int ecu_id;

	unsigned int timestamp; /// Timestamp when the scanner has measured to 0
	double timestamp_sync; /// Timestamp of the hosting computer
	float start_angle; ///< in scanner coordinates
	float end_angle; ///< in scanner coordinates
	unsigned int scan_counter;
	unsigned int num_points;
} AXT_SCAN_HDR_STR;

typedef struct _AXT_SCAN_POINT_STR{
	unsigned int scanner_id;
	unsigned int channel; ///< Scan Layer
	unsigned int sub_channel; ///< echo 1 or 2
	unsigned int point_status;

	float x; ///< in scanner coordinates
	float y; ///< in scanner coordinates
	float z; ///< in scanner coordinates
	unsigned int echo_pulse_width;
} AXT_SCAN_POINT_STR;

typedef struct _AXT_SCAN_STR{
	AXT_SCAN_HDR_STR header;
	AXT_SCAN_POINT_STR points[AXT_MAX_SCAN_POINTS];
} AXT_SCAN_STR;

#ifndef GENOM

/*! \brief Return current time
  @returns The current time in [s]
*/
double axt_get_time();

/*! \brief Connects to the scanner
  @param *host Hostname or IP of scanner
  @param *sd Handle to socket
  @returns 1 on success
*/
int axt_connect(const char *host, int *sd);

/*! \brief Closes connection to the scanner
  @param sd Handle to socket
  @param *f FILE_ptr to dump to
  @param *scan Scan to be dumped
*/
void axt_close(int sd);

/*! \brief Parses the next message
  @param sd Socket handle
  @param *scan New scan
  @returns Message type
*/
int axt_parse(int sd, AXT_SCAN_STR *scan);

/*! \brief Dumps the scan
  @param *f File to dump to
  @param *scan Scan to be dumped
*/
void axt_dump_scan(FILE* f, AXT_SCAN_STR *scan);

/*! \brief Dumps the scan in line
  @param *f File to dump to
  @param *scan Scan to be dumped
*/
void axt_dump_scan_line(FILE* f, AXT_SCAN_STR *scan);

int axt_get_scan_from_file(FILE* f, AXT_SCAN_STR *scan);

#endif //GENOM

#ifdef __cplusplus
}
#endif


#endif
