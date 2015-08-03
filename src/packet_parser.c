#include "packet_parser.h"

int
parse_http_req_line(const u_char *data,
		    int data_len,
		    struct http_req *req)
{
  const u_char *p = data, *m;
  u_char ch;
  int i;
  enum {
    sw_start,
    sw_method,
    sw_space_before_uri,
    sw_uri
  }state;

  state = sw_start;

  for(i=0; i< data_len; i++) {
    ch = *p;
    switch(state) {
    case sw_start:
      if ((ch < 'A' || ch > 'Z') && ch != '_') {
	return 1;
      }
      state = sw_method;
      req->request_start = p;
      break;
    case sw_method:
      if (ch == ' ') {
	m = req->request_start;
	switch(p-m) {
	case 3:
	  if (*m == 'G' &&
	      *(m+1) == 'E' &&
	      *(m+2) == 'T') {
	    break;
	  }
	case 4:
	  if (*m == 'P' &&
	      *(m+1) == 'O' &&
	      *(m+2) == 'S' &&
	      *(m+3) == 'T') {
	    break;
	  }
	}
	state = sw_space_before_uri;
	break;
      }
      if ((ch < 'A' || ch > 'Z') && ch != '_') {
	return 1;
      }
      break;
    case sw_space_before_uri:
      if (ch == '/') {
	req->uri_start = p;
	state = sw_uri;
      }
      break;
    case sw_uri:
      if (ch == ' ') {
	req->uri_end = p;
	goto done;
      }
      if (ch == CR || ch == LF) {
	req->uri_end = p;
      }
      break;
    }
    p++;
  }
done:

  return 0;
}

int
parse_http_req_header(const u_char *data,
		      int data_len,
		      struct http_req *req)
{
  const u_char *p = data;
  u_char ch, c;

  static u_char  lowcase[] =
    "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
    "\0\0\0\0\0\0\0\0\0\0\0\0\0-\0\0" "0123456789\0\0\0\0\0\0"
    "\0abcdefghijklmnopqrstuvwxyz\0\0\0\0\0"
    "\0abcdefghijklmnopqrstuvwxyz\0\0\0\0\0"
    "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
    "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
    "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"
    "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";
  
  int i=0;
  enum {
    sw_start,
    sw_name,
    sw_space_before_value,
    sw_value,
    sw_space_after_value,
    sw_almost_done,
    sw_header_almost_done
  }state;

  // skip req line.
  for(i=0;i<data_len-1;i++) {
    if (*p == CR &&
	*(p+1) == LF) {
      p += 2;
      break;
    }
    p++;
  }

  data_len -= (p - data);
  state = sw_start;
  for(i=0; i< data_len; i++) {
    ch = *p;
    switch(state) {
    case sw_start:
      req->header_name_start = p;
      state = sw_name;
      c = lowcase[ch];
      if (c)
	break;
      if (ch == '\0') {
	return 1;
      }
      break;
    case sw_name:
      c = lowcase[ch];
      if (c)
	break;
      if (ch == ':') {
	req->header_name_end = p;
	state = sw_space_before_value;
      }
      if (ch == CR) {
	req->header_name_end = p;
	req->header_start = p;
	req->header_end = p;
	state = sw_almost_done;
	break;
      }
      if (ch == LF) {
	req->header_name_end = p;
	req->header_start = p;
	req->header_end = p;
	state = sw_almost_done;
	goto done;
      }
      if (ch == '\0') {
	return 1;
      }
      break;
    case sw_space_before_value:
      switch(ch) {
      case ' ':
	break;
      default:
	req->header_start = p;
	state = sw_value;
	break;
      }
      break;
    case sw_value:
      switch(ch) {
      case ' ':
	req->header_end = p;
	state = sw_space_after_value;
	break;
      case CR:
	req->header_end = p;
	state = sw_almost_done;
	break;
      case LF:
	req->header_end = p;
	goto done;
      case '\0':
	return 1;
      }
      break;
    case sw_space_after_value:
      switch(ch) {
      case ' ':
	break;
      case CR:
	state = sw_almost_done;
	break;
      case LF:
	goto done;
	break;
      case '\0':
	return 6;
      default:
	state = sw_value;
	break;
      }
      break;
    case sw_almost_done:
      switch(ch) {
      case LF:
	goto done;
      case CR:
	break;
      default:
	return 7;
      }
      break;
    case sw_header_almost_done:
      switch(ch) {
      case LF:
	goto header_done;
      default:
	return 8;
      }
    }
    p++;
  }
  return 1;
done:
  
  return 0;
header_done:
  
  
  return 0;
}
