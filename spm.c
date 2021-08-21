#include "spm.h"

int
main(int argc, char** argv)
{
        debug = false;
        if (argc == 1) {
#ifdef _ENABLE_GUI
                return gui(&argc, argv);
#else
                logger("main", (debug ? &main : NULL)
                        , "The program was not compiled with GUI support.\n"
                        , stderr);
#endif
                return 0;
        }

        const struct option large_options[] = {
                { "daemon",       no_argument, 0, 'd' },
                { "debug",        no_argument, 0, 'D' },
                { "file",required_argument,    0, 'f' },
                { "help",         no_argument, 0, 'h' },
                { "hibernate",    no_argument, 0, 'H' },
                { "monitor",      no_argument, 0, 'm' },
                { "locker",required_argument,  0, 'l' },
                { "poweroff",     no_argument, 0, 'p' },
                { "reboot",       no_argument, 0, 'r' },
                { "suspend",      no_argument, 0, 's' },
                { NULL, 0, NULL, 0 }
        };

        Options options = {
                .locker_cmd = NULL,
                .daemon = false,
                .help = false,
                .hibernate = false,
                .monitor = false,
                .poweroff = false,
                .reboot = false,
                .suspend = false
        };

        log_file = NULL;

        int option = 0;
        while ((option = getopt_long(argc, argv, "dDf:hHl:mprs", large_options, NULL)) >= 0) {
                switch (option)
                {
                case 'f': log_file = optarg; break;
                case 'd': options.daemon = true; break;
                case 'D': debug = true; break;
                case 'm': options.monitor = true; break;
                case 'l': options.locker_cmd = optarg; break;
                case 'h': return usage();
                case 'H': options.hibernate = true; break;
                case 'p': options.poweroff = true; break;
                case 'r': options.reboot = true; break;
                case 's': options.suspend = true; break;
                default: fprintf(stderr, "Unknown option: %c\n", optopt);
                }
        }

        uint8_t value = exec_option(&options);
        if (value > 0)
                return 0; /* A power option was performed! Everything's ok. */

        /* TODO: Check whether this works as expected. */
        if (options.daemon)
        {
                if (!options.monitor) {
                        logger("main", (debug ? &main : NULL),
                                "Monitor option is not set. Ignoring daemon option.\n", stderr);
                } else {
                        logger("main", (debug ? &main : NULL),
                                "Trying to run as a daemon...\n", stdout);
                        pid_t pid;
                        pid_t sid;
                        pid = fork();
                        if (pid < 0) {
                                logger("main", (debug ? &main : NULL),
                                        "Error while forking.\n", stderr);
                                return EXIT_FAILURE;
                        } else if (pid > 0) {
                                return EXIT_SUCCESS;
                        }

                        umask(0);
                        sid = setsid();
                        if (sid < 0) {
                                logger("main", (debug ? &main : NULL),
                                        "Error while setting sid.\n", stderr);
                                return EXIT_FAILURE;
                        }

                        if ((chdir("/")) < 0) {
                                logger("main", (debug ? &main : NULL),
                                        "Error while changing directory.\n", stderr);
                                return EXIT_FAILURE;
                        }

                        close(STDIN_FILENO);
                        close(STDOUT_FILENO);
                        close(STDERR_FILENO);
                }
        }

        if (options.monitor) {
                pthread_t monitor_id;
                pthread_create(&monitor_id, NULL, &battery_monitor, NULL);
                pthread_join(monitor_id, NULL);
        }

        return 0;
}

uint8_t
usage(void)
{
        printf("SPM usage:\n\n");
        printf("Power options:\n");
        printf("-p | --poweroff\t\tShutdowns the computer.\n");
        printf("-H | --hibernate\tHibernates the computer.\n");
        printf("-r | --reboot\t\tRestarts the computer.\n");
        printf("-s | --suspend\t\tSuspends the computer.\n");
        printf("-l | --locker\t\tScreen locker which would be run when suspending.\n");
        printf("\nGuardian options:\n");
        printf("-d | --daemon\t\tRun as daemon. Useful when combined with -m, and -f.\n");
        printf("-m | --monitor\t\tChecks whether the battery's charge is greater than 15%%.\n");
        printf("\t\t\tIf not, then suspends the computer.\n");
        printf("\nFile options:\n");
        printf("-f | --file file\tFile where log is going to be saved.\n");
        printf("\nHelp options:\n");
        printf("-h | --help\t\tShows this help and exit.\n");
        printf("\nGUI Options:\n");
        printf("Run without arguments to show the GUI.\n");
        return 0;
}

/* This function will return one of the following values:
 * 0 = nothing was done.
 * 1 = shutdown was performed.
 * 2 = hibernate was performed.
 * 3 = reboot was performed.
 * 4 = suspend was performed.
 */
uint8_t
exec_option(const Options *options)
{
        if ((options->poweroff || options->hibernate) && (options->reboot || options->suspend))
        {
                logger("exec_option", (debug ? &exec_option : NULL),
                        "User has chose to run more than one power option. Turning off...\n",
                        stdout);
                spm_power(POWEROFF);
                return 1;
        }

        if (options->poweroff) {
                spm_power(POWEROFF);
                return 1;
        }

        if (options->hibernate) {
                pthread_t locker_id;
                if (options->locker_cmd) {
                        pthread_create(&locker_id, NULL, run_locker, options->locker_cmd);
                        spm_power(HIBERNATE);
                        pthread_join(locker_id, NULL);
                } else {
                        spm_power(HIBERNATE);
                }
                return 2;
        }

        if (options->reboot) {
                spm_power(REBOOT);
                return 3;
        }

        if (options->suspend) {
                pthread_t locker_id;
                if (options->locker_cmd) {
                        pthread_create(&locker_id, NULL, run_locker, options->locker_cmd);
                        spm_power(SUSPEND);
                        pthread_join(locker_id, NULL);
                } else {
                        spm_power(SUSPEND);
                }
                return 4;
        }

        return 0;
}

void*
run_locker(void* locker_cmd)
{
        char* locker = (char*) locker_cmd;
        FILE* cmd_out = popen(locker, "r");
        pclose(cmd_out);
        return locker_cmd;
}
