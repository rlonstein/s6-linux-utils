/* ISC license. */

#include <unistd.h>
#include "bytestr.h"
#include "allreadwrite.h"
#include "strerr2.h"
#include "stralloc.h"
#include "djbunix.h"

#define USAGE "s6-hostname [ hostname ]"

static int getit (void)
{
  stralloc sa = STRALLOC_ZERO ;
  if (sagethostname(&sa) < 0) strerr_diefu1sys(111, "get hostname") ;
  sa.s[sa.len++] = '\n' ;
  if (allwrite(1, sa.s, sa.len) < sa.len)
    strerr_diefu1sys(111, "write to stdout") ;
  return 0 ;
}

static int setit (char const *h)
{
  if (sethostname(h, str_len(h)) < 0)
    strerr_diefu1sys(111, "set hostname") ;
  return 0 ;
}

int main (int argc, char const *const *argv)
{
  PROG = "s6-hostname" ;
  return (argc < 2) ? getit() : setit(argv[1]) ;
}
