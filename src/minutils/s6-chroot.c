/* ISC license. */

#include <unistd.h>
#include "strerr2.h"
#include "djbunix.h"

#define USAGE "s6-chroot dir prog..."

int main (int argc, char const *const *argv, char const *const *envp)
{
  PROG = "s6-chroot" ;
  if (argc < 3) strerr_dieusage(100, USAGE) ;
  if (chdir(argv[1]) == -1) strerr_diefu2sys(111, "chdir to ", argv[1]) ;
  if (chroot(".") == -1) strerr_diefu2sys(111, "chroot in ", argv[1]) ;
  pathexec_run(argv[2], argv+2, envp) ;
  strerr_dieexec(111, argv[2]) ;
}
