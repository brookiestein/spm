#include "util.h"

#include <stdio.h>
#include <stdlib.h>

void
die(const char *fmt, ...)
{
	 va_list ap;
	 va_start(ap, fmt);
	 vfprintf(stderr, fmt, ap);
	 va_end(ap);
	 putchar('\n');
	 exit(1);
}

/* Each verb corresponds to a DBus method. */
const char *
get_method(enum POWER_OPTION option)
{
	 switch (option)
	 {
	 case HIBERNATE:
		  return "Hibernate";
	 case POWEROFF:
		  return "PowerOff";
	 case REBOOT:
		  return "Reboot";
	 case SUSPEND:
		  return "Suspend";
	 }
}

/* Just to show a right message to the user. */
const char *
get_gerund(enum POWER_OPTION option)
{
	 switch (option)
	 {
	 case HIBERNATE:
		  return "Hibernating";
	 case POWEROFF:
		  return "Shutting down";
	 case REBOOT:
		  return "Rebooting";
	 case SUSPEND:
		  return "Suspending";
	 }
}
