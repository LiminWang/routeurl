#ifndef _PACKET_PARSER_H
#define _PACKET_PARSER_H

#include "main.h"
#include "http.h"

#define LF     (u_char) '\n'
#define CR     (u_char) '\r'
#define CRLF   "\r\n"

int
parse_http_req_line(const u_char *data, int data_len, struct http_req *req);

int
parse_http_req_header(const u_char *data, int data_len, struct http_req *req);

#endif
