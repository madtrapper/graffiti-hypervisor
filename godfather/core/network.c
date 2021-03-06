


#include "network.h"
#include "vmmstring.h"
#include "debug.h"

#ifdef GUEST_WINDOWS
#include "winxp.h"
#elif defined GUEST_LINUX
#include "linux.h"
#endif

Bit16u vmm_ntohs(Bit16u v) {
  return ((v & 0xff00) >> 8) | ((v & 0xff) << 8);
}

Bit32u vmm_ntohl(Bit32u v) {
  return (
	  (v & 0xff000000) >> 24 |
	  (v & 0x00ff0000) >>  8 |
	  (v & 0x0000ff00) <<  8 |
	  (v & 0x000000ff) << 24
	  );
}

void inet_ntoa(Bit32u a, char *internal_buffer)
{
  a = vmm_ntohl(a);

  vmm_snprintf(internal_buffer, 16,
	       "%d.%d.%d.%d",
	       (a & 0xff000000) >> 24,
	       (a & 0x00ff0000) >> 16,
	       (a & 0x0000ff00) >>  8,
	       (a & 0x000000ff)
	       );
}

void inet_ntoa_v6(Bit16u *a, char *internal_buffer)
{
  unsigned int i;

  for(i = 0; i < 8; i++)
    a[i] = vmm_ntohs(a[i]);

  vmm_snprintf(internal_buffer, 40,
	       "%0x:%0x:%0x:%0x:%0x:%0x:%0x:%0x",
	       a[0], a[1], a[2], a[3],
	       a[4], a[5], a[6], a[7]
	       );
}

hvm_status NetworkBuildSocketList(hvm_address cr3, SOCKET *buf, Bit32u maxsize, Bit32u *psize)
{
  hvm_status r;
  r = HVM_STATUS_UNSUCCESSFUL;
  
#ifdef GUEST_WINDOWS
#ifdef GUEST_WIN_7
  r = Windows7BuildSocketList(cr3, buf, maxsize, psize);
#else
  r = WindowsBuildSocketList(cr3, buf, maxsize, psize);
#endif
#elif defined GUEST_LINUX
  r = LinuxBuildSocketList(cr3, buf, maxsize, psize);
#endif

  return r;
}
