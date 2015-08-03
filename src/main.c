#include "main.h"
#include "packet_capture.h"

static char errbuf[PCAP_ERRBUF_SIZE];

int
routeurl_main(int argc, char *argv[])
{
  pcap_t *pt = NULL;
  char *filter = "port 80";
  bpf_u_int32 maskp = {0}, netp = {0};
  struct bpf_program fp = {0};

  pt = pcap_open_live(argv[1],
		      BIG_BUF_SIZE,
		      1,
		      1000,
		      errbuf);

  if (pt == NULL) {
    printf("Error: pcap_open_live: %s\n", errbuf);
    return 1;
  }

  if (pcap_compile(pt, &fp, filter, 0, netp) == -1) {
    printf("Error: pcap_compile.\n");
    return 1;
  }

  if (pcap_setfilter(pt, &fp) == -1) {
    printf("Error: pcap_setfilter.\n");
    return 1;
  }

  pcap_loop(pt, -1, myCallback, NULL);

  printf("[INFO] ALL DONE. \n");
  
  return 0;
}


int main(int argc, char *argv[])
{
  if (argc < 2) {
    printf("Usage: %s eth0", argv[0]);
    return 1;
  }

  return routeurl_main(argc, argv);
}
