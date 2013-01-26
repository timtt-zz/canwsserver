#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <sys/time.h>

#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/error.h>

#include "lib/websockets/libwebsockets.h"
#include "canwsserver.h"

#define DATA_BUFLEN 128

enum wss_protocols {
	/* always first */
	PROTOCOL_HTTP = 0,

	PROTOCOL_CAN_RAW_RELAY,
	PROTOCOL_CAN_RAW_DELTA,
    PROTOCOL_DATA_LOGGING,

	/* always last */
	DEMO_PROTOCOL_COUNT
};

#define DATADIR "."
#define LOCAL_RESOURCE_PATH DATADIR"/libwebsockets-test-server"

/* this protocol server (always the first one) just knows how to do HTTP */

int callback_http(struct libwebsocket_context * this,
                  struct libwebsocket *wsi,
                  enum libwebsocket_callback_reasons reason, void *user,
                  void *in, size_t len);

int callback_can_raw_relay(struct libwebsocket_context * this,
                           struct libwebsocket *wsi,
                           enum libwebsocket_callback_reasons reason,
					       void *user, void *in, size_t len);

int callback_can_raw_delta(struct libwebsocket_context * this,
                           struct libwebsocket *wsi,
                           enum libwebsocket_callback_reasons reason,
					       void *user, void *in, size_t len);

int callback_data_logging(struct libwebsocket_context * this,
                          struct libwebsocket *wsi,
                          enum libwebsocket_callback_reasons reason,
                          void *user, void *in, size_t len);

static struct libwebsocket_protocols protocols[] = {
	/* first protocol must always be HTTP handler */
	[PROTOCOL_HTTP] = {
		.name = "http-only",
		.callback = callback_http,
	},
	[PROTOCOL_CAN_RAW_RELAY] = {
		.name = "can-raw-relay",
		.callback = callback_can_raw_relay,
		.per_session_data_size =
				sizeof(struct can_frame),
	},
	[PROTOCOL_CAN_RAW_DELTA] = {
		.name = "can-raw-delta",
		.callback = callback_can_raw_delta,
		.per_session_data_size =
				sizeof(struct can_frame),
	},
	[PROTOCOL_DATA_LOGGING] = {
		.name = "can-data-logging",
		.callback = callback_data_logging,
	},
	[DEMO_PROTOCOL_COUNT] = {  /* end of list */
		.callback = NULL
	}
};
