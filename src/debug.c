#include "debug.h"


void
output_http_req(struct http_req *req)
{
  char uri[1024] = {0};
  char header_name[128] = {0};
  char header_value[1024] = {0};
  int len = 0;

  if (req->header_name_start != NULL &&
      req->header_name_end != NULL) {
    len = req->header_name_end - req->header_name_start;
    if (len > 127) {
      len = 127;
    }
    memcpy(header_name, req->header_name_start, len);
    //    printf("header_name: %s\n", header_name);
  }
  if (req->header_start != NULL &&
      req->header_end != NULL) {
    len = req->header_end - req->header_start;
    if (len > 127) {
      len = 127;
    }
    memcpy(header_value, req->header_start, len);
    //    printf("header_value: %s\n", header_value);
  }
  if (req->uri_start != NULL &&
      req->uri_end != NULL) {
    len = req->uri_end - req->uri_start;
    if (len > 1023) {
      len = 1023;
    }
    memcpy(uri, req->uri_start, len);
    //    printf("uri: %s\n", uri);
  }
  printf("http://%s%s\n", header_value, uri);
}
