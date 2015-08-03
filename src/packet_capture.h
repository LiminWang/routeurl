#ifndef _PACKET_CAPTURE_H
#define _PACKET_CAPTURE_H

#include "main.h"

void
myCallback(u_char *user,
	   const struct pcap_pkthdr *h,
	   const u_char *bytes);


#endif
