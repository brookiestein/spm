#include <ctype.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "power.h"
#include "util.h"

#ifdef ENABLE_GUI
    #include "gui.h"
    enum POWER_OPTION guessOption(const char *value);
#endif

void usage(bool shouldExitAbnormally);

int
main(int argc, char *argv[])
{
	 const struct option longOptions[] = {
#ifdef ENABLE_GUI
		  { "default-option", required_argument, NULL,    'd' },
		  { "time",           required_argument, NULL,    't' },
#endif
		  { "help",           no_argument,       NULL,    'h' },
		  { "hibernate",      no_argument,       NULL,    'H' },
		  { "poweroff",       no_argument,       NULL,    'p' },
		  { "reboot",         no_argument,       NULL,    'r' },
		  { "suspend",        no_argument,       NULL,    's' },
		  { "version",        no_argument,       NULL,    'v' },
		  { NULL,             0,                 NULL,     0  }
	 };

#ifdef ENABLE_GUI
	 enum POWER_OPTION defaultOption = POWEROFF;
	 int seconds = 60;
	 const char *shortOptions = "d:hHprst:v";
#else
	 const char *shortOptions = "hHprsv";
#endif

	 __attribute__((cleanup(free_version))) char *version = NULL;

	 int option = -1;
	 while ((option = getopt_long(argc, argv, shortOptions, longOptions, NULL)) >= 0) {
		  switch (option)
		  {
#ifdef ENABLE_GUI
		  case 'd':
			   defaultOption = guessOption(optarg);
			   break;
		  case 't':
			   seconds = atoi(optarg);
			   if (seconds == 0 && strlen(optarg) > 2 && strncmp(optarg, "0", 1))
					die("Invalid time: '%s'.", optarg);
			   break;
#endif
		  case 'h':
			   usage(false);
		  case 'H':
			   spm_exec(HIBERNATE);
			   return 0;
		  case 'p':
			   spm_exec(POWEROFF);
			   return 0;
		  case 'r':
			   spm_exec(REBOOT);
			   return 0;
		  case 's':
			   spm_exec(SUSPEND);
			   return 0;
		  case 'v':
			   version = get_version(argv[0]);
			   printf("%s\n", version);
			   return 0;
		  default:
			   fprintf(stderr, "Unknown option: %c\n", optopt);
			   return -1;
		  }
	 }

#ifdef ENABLE_GUI
	 return show_gui(argc, argv, defaultOption, seconds);
#else
	 usage(true);
#endif
}

void
usage(bool shouldExitAbnormally)
{
	 putchar('\n');
#ifdef ENABLE_GUI
	 printf("-d | --default-option    Option to execute in the GUI when *time* ends. "
                                      "Available: Hibernate, PowerOff (Default), Reboot, Suspend.\n");
#endif
	 printf("-h | --help              Show this help.\n");
	 printf("-H | --hibernate         Hibernate the system.\n");
	 printf("-p | --poweroff          Shutdown the system.\n");
	 printf("-r | --reboot            Reboot the system.\n");
	 printf("-s | --suspend           Suspend the system.\n");
#ifdef ENABLE_GUI
	 printf("-t | --time              Time, in seconds, to wait before executing the default option "
                                      "or any value < 0 to disable (Default to 60 seconds).\n");
#endif
     printf("-v | --version           Show version information.\n");
	 putchar('\n');

	 exit(shouldExitAbnormally ? 1 : 0);
}

#ifdef ENABLE_GUI
enum POWER_OPTION
guessOption(const char *value)
{
	 size_t size = strlen(value);
     if (size >= 64) { // Don't allow input excessively long.
         die("Option too long!\n\n%s\n\nSee --help to know which power options are available.", value);
     }

	 char str[size];

	 for (size_t i = 0; i < size; ++i)
		  str[i] = tolower(value[i]);

	 if (!strncmp(str, "hibernate", 9))
		  return HIBERNATE;
	 if (!strncmp(str, "poweroff", 8))
		  return POWEROFF;
	 if (!strncmp(str, "reboot", 6))
		  return REBOOT;
	 if (!strncmp(str, "suspend", 7))
		  return SUSPEND;

	 die("Unknown power option: '%s'.", value);
	 return 0;
}
#endif
