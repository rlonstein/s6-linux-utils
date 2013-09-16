#include <unistd.h>
#include <sys/reboot.h>
#include "strerr2.h"

int main ()
{
  PROG = "s6-halt" ;
  sync() ;
  reboot(RB_HALT_SYSTEM) ;
  strerr_diefu1sys(111, "reboot()") ;
}
