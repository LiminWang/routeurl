#ifndef _HTTP_H
#define _HTTP_H

struct http_req
{
  const u_char *uri_start;
  const u_char *uri_end;
  const u_char *request_start;
  const u_char *header_name_start;
  const u_char *header_name_end;
  const u_char *header_start;
  const u_char *header_end;
};

#endif

