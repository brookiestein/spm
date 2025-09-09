#include "util.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STRINGIFY_HELPER(x) #x
#define STRINGIFY(x) STRINGIFY_HELPER(x)

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

void
free_version(char **ptr)
{
	 if (ptr && *ptr) {
		  free(*ptr);
		  ptr = NULL;
	 }
}

char *
get_version(const char *name)
{
#ifndef SPM_VERSION
	 #error "Version macro was not defined."
#else
	 size_t size = strlen(name) + 32;
	 char *version = malloc(size * sizeof(char));

	 if (!version)
		  die("Not enough memory to show version information.");

	 snprintf(version, size, "%s v%s", name, STRINGIFY(SPM_VERSION));
	 return version;
#endif
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

	 return NULL;
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

	 return NULL;
}
