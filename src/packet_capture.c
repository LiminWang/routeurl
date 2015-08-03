#include "packet_capture.h"
#include "debug.h"
#include "packet_parser.h"

int dt_ip_data_len(struct ip_hdr *iph)
{
  return ntohs(iph->ip_len) - iph->ip_hl * 4;
}

const u_char *dt_data_iph(const u_char *data)
{
  return data + ETH_HDR_LEN;
}

const u_char *dt_data_tcph(const u_char *data, struct ip_hdr *iph)
{
  return data + ETH_HDR_LEN + iph->ip_hl * 4;
}

int dt_tcp_data_len(struct ip_hdr *iph, struct tcp_hdr *tcph)
{
  return dt_ip_data_len(iph) - tcph->th_off * 4;
}


u_char *
get_packet_data(const u_char *bytes, int *data_len)
{
  struct eth_hdr *eh;
  struct ip_hdr *iph;
  struct tcp_hdr *tcph;
  u_char *data;

  *data_len = 0;

  eh = (struct eth_hdr *)bytes;
  if (ntohs(eh->eth_type) != ETH_TYPE_IP) {
    return NULL;
  }
  iph = (struct ip_hdr *)dt_data_iph(bytes);
  if (iph->ip_p != IP_PROTO_TCP) {
    return NULL;
  }
  tcph = (struct tcp_hdr *)dt_data_tcph(bytes, iph);
  *data_len = dt_tcp_data_len(iph, tcph);
  data = (u_char *)tcph + (tcph->th_off) * 4;
  return data;
}

void
myCallback(u_char *user,
	   const struct pcap_pkthdr *h,
	   const u_char *bytes)
{
  int data_len = 0;
  const u_char *data = get_packet_data(bytes, &data_len);
  struct http_req req;

  if (data_len <= 0) {
    return;
  }
  if (parse_http_req_line(data, data_len, &req) == 0) {
    while(1) {
      if (parse_http_req_header(data, data_len, &req) == 0) {
	if (memcmp(req.header_name_start, "Host", 4) == 0) {
	  output_http_req(&req);
	  break;
	}
      } else {
	break;
      }
    }
  }
}
