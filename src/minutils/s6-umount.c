/* ISC license. */

#include <sys/mount.h>
#include "allreadwrite.h"
#include "bytestr.h"
#include "buffer.h"
#include "strerr2.h"
#include "stralloc.h"
#include "djbunix.h"
#include "skamisc.h"

#define USAGE "s6-umount mountpoint <or> s6-umount -a"

#define BUFSIZE 4096
#define MAXLINES 512

static int umountall ( )
{
  stralloc mountpoints[MAXLINES] ;
  char buf[BUFSIZE] ;
  buffer b ;
  stralloc sa = STRALLOC_ZERO ;
  unsigned int line = 0 ;
  int e = 0 ;
  int r ;
  int fd = open_readb("/proc/mounts") ;
  if (fd == -1) strerr_diefu1sys(111, "open /proc/mounts") ;
  byte_zero(mountpoints, sizeof(mountpoints)) ;
  buffer_init(&b, &fd_read, fd, buf, BUFSIZE) ;
  for (;;)
  {
    unsigned int n, p ;
    if (line >= MAXLINES) strerr_dief1x(111, "/proc/mounts too big") ;
    sa.len = 0 ;
    r = skagetln(&b, &sa, '\n') ;
    if (r <= 0) break ;
    p = byte_chr(sa.s, sa.len, ' ') ;
    if (p >= sa.len) strerr_dief1x(111, "bad /proc/mounts format") ;
    p++ ;
    n = byte_chr(sa.s + p, sa.len - p, ' ') ;
    if (n == sa.len - p) strerr_dief1x(111, "bad /proc/mounts format") ;
    if (!stralloc_catb(&mountpoints[line], sa.s + p, n) || !stralloc_0(&mountpoints[line]))
      strerr_diefu1sys(111, "store mount point") ;
    line++ ;
  }
  fd_close(fd) ;
  stralloc_free(&sa) ;
  if (r == -1) strerr_diefu1sys(111, "read /proc/mounts") ;
  while (line--)
    if (umount(mountpoints[line].s) == -1)
    {
      e++ ;
      strerr_warnwu2sys("umount ", mountpoints[line].s) ;
    }
  return e ;
}

int main (int argc, char const *const *argv)
{
  PROG = "s6-umount" ;
  if (argc < 2) strerr_dieusage(100, USAGE) ;
  if ((argv[1][0] == '-') && (argv[1][1] == 'a') && !argv[1][2])
    return umountall() ;
  if (umount(argv[1]) == -1) strerr_diefu2sys(111, "umount ", argv[1]) ;
  return 0 ;
}
