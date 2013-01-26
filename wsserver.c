#include "wsserver.h"

char *last_can_frames[MAX_CAN_FRAME_ID];


int callback_http(struct libwebsocket_context * this,
		  struct libwebsocket *wsi,
		  enum libwebsocket_callback_reasons reason, void *user,
		  void *in, size_t len)
{
  char client_name[128];
  char client_ip[128];

  switch (reason) {
  case LWS_CALLBACK_HTTP:
    fprintf(stderr, "serving HTTP URI %s\n", (char *)in);

    if (in && strcmp(in, "/favicon.ico") == 0) {
      if (libwebsockets_serve_http_file(wsi,
					LOCAL_RESOURCE_PATH"/favicon.ico", "image/x-icon"))
	fprintf(stderr, "Failed to send favicon\n");
      break;
    }

    /* send the script... when it runs it'll start websockets */

		if (libwebsockets_serve_http_file(wsi,
				  LOCAL_RESOURCE_PATH"/test.html", "text/html"))
			fprintf(stderr, "Failed to send HTTP file\n");
		break;

	/*
	 * callback for confirming to continue with client IP appear in
	 * protocol 0 callback since no websocket protocol has been agreed
	 * yet.  You can just ignore this if you won't filter on client IP
	 * since the default uhandled callback return is 0 meaning let the
	 * connection continue.
	 */

	case LWS_CALLBACK_FILTER_NETWORK_CONNECTION:

		libwebsockets_get_peer_addresses((int)(long)user, client_name,
			     sizeof(client_name), client_ip, sizeof(client_ip));

		fprintf(stderr, "Received network connect from %s (%s)\n",
							client_name, client_ip);

		/* if we returned non-zero from here, we kill the connection */
		break;

	default:
		break;
	}

	return 0;
}


int callback_can_raw_relay(struct libwebsocket_context * this,
                           struct libwebsocket *wsi,
                           enum libwebsocket_callback_reasons reason,
                           void *user, void *in, size_t len)
{
	int n;

	switch (reason) {

	case LWS_CALLBACK_ESTABLISHED:
		break;

	case LWS_CALLBACK_BROADCAST:
		//n = sprintf((char *)p, "(%d)", pss->number++);
		//n = sprintf((char *)p, "(n=%d) %s", pss->number++, (char *)in);
		//printf("callback data = %s\n", p);
		//n = libwebsocket_write(wsi, p, n, LWS_WRITE_TEXT);
		n = libwebsocket_write(wsi, in, strlen(in), LWS_WRITE_TEXT);
		if (n < 0) {
			fprintf(stderr, "can-raw-relay ERROR writing to socket");
			return 1;
		}
		break;

	case LWS_CALLBACK_RECEIVE:
		fprintf(stderr, "can-raw-relay rx %d\n", (int)len);
		if (len < 6)
			break;
		if (strcmp(in, "reset\n") == 0)
			break;
		break;

	default:
		break;
	}

	return 0;
}


int callback_can_raw_delta(struct libwebsocket_context * this,
                          struct libwebsocket *wsi,
                          enum libwebsocket_callback_reasons reason,
                          void *user, void *in, size_t len)
{
    int n, i;
    char* frame_data;
    char* frame_id_str;
    int frame_id;
    
	switch (reason) {

    // on connect, remove all stored CAN frames to force sending to websocket
    // on next CAN bus receive
	case LWS_CALLBACK_ESTABLISHED:
        printf("connected %i\n", MAX_CAN_FRAME_ID);
 
        for (i = 0; i < MAX_CAN_FRAME_ID; i++) {
            last_can_frames[i] = 0;
        }
        
		break;

	case LWS_CALLBACK_BROADCAST:
        frame_data = strndup((char*)in+strlen(in)-CAN_FRAME_STR_LEN, CAN_FRAME_STR_LEN);
        frame_id_str = strndup((char*)in+strlen(in)-CAN_FRAME_STR_LEN-4, 3);
        frame_id = atoi(frame_id_str);

        // send message on raw delta protocol only if it has changed from cached message
        if (last_can_frames[(int)frame_id] == 0 || strcmp(last_can_frames[(int)frame_id], frame_data) != 0) {
            n = libwebsocket_write(wsi, in, strlen(in), LWS_WRITE_TEXT);

            if (n < 0) {
                fprintf(stderr, "can-raw-delta ERROR writing to socket");
                return 1;
            }
            
            if (last_can_frames[(int)frame_id] == 0) {
                last_can_frames[(int)frame_id] = malloc(CAN_FRAME_STR_LEN);
            }
            strcpy(last_can_frames[(int)frame_id], frame_data);
        }

		break;

	case LWS_CALLBACK_RECEIVE:
		fprintf(stderr, "can-raw-delta rx %d\n", (int)len);
		if (len < 6)
			break;
		if (strcmp(in, "reset\n") == 0)
			break;
		break;

	default:
		break;
	}

	return 0;
}


int callback_data_logging(struct libwebsocket_context * this,
                          struct libwebsocket *wsi,
                          enum libwebsocket_callback_reasons reason,
                          void *user, void *in, size_t len)
{
	int n;

	switch (reason) {

	case LWS_CALLBACK_ESTABLISHED:
		break;

	case LWS_CALLBACK_RECEIVE:
		fprintf(stderr, "can-data-logging rx %d\n", (int)len);
		if (len < 6)
			break;
		if (strcmp(in, "reset\n") == 0)
			break;
		break;

	default:
		break;
	}

	return 0;
}
