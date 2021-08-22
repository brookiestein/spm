#include "spm.h"

int
main(int argc, char** argv)
{
        debug = false;
        prog_name = argv[0];

        if (argc < 2) {
#ifdef _ENABLE_GUI
                return gui(&argc, argv);
#else
                logger("main", "The program was not compiled with GUI support.\n", stderr);
#endif
                return 0;
        }

        const struct option large_options[] = {
                { "daemon",     no_argument, 0, 'd' },
                { "debug",      no_argument, 0, 'D' },
                { "file",required_argument,  0, 'f' },
                { "help",       no_argument, 0, 'h' },
                { "hibernate",  no_argument, 0, 'H' },
                { "monitor",    no_argument, 0, 'm' },
                { "locker",required_argument,0, 'l' },
                { "poweroff",   no_argument, 0, 'p' },
                { "reboot",     no_argument, 0, 'r' },
                { "suspend",    no_argument, 0, 's' },
                { "verbose",    no_argument, 0, 'v' },
                { "version",    no_argument, 0, 'V' },
                { "wait",required_argument,  0, 'w' },
                { NULL, 0, NULL, 0 }
        };

        Options options = {
                .help = false,
                .hibernate = false,
                .monitor = false,
                .poweroff = false,
                .reboot = false,
                .suspend = false,
                .time_to_wait = 0
        };

        as_daemon = false;
        verbose = false;
        log_file = "/tmp/spm.log";
        locker_cmd = NULL;

        int option = 0;
        while ((option = getopt_long(argc, argv, "dDf:hHl:mprsw:vV", large_options, NULL)) >= 0)
        {
                switch (option)
                {
                case 'f': log_file = optarg; break;
                case 'd': as_daemon = true; break;
                case 'D': debug = true; break;
                case 'm': options.monitor = true; break;
                case 'l': locker_cmd = optarg; break;
                case 'h': return usage();
                case 'H': options.hibernate = true; break;
                case 'p': options.poweroff = true; break;
                case 'r': options.reboot = true; break;
                case 's': options.suspend = true; break;
                case 'v': verbose = true; break;
                case 'V': printf("%s\n", version); return 0;
                case 'w': options.time_to_wait = atoi(optarg); break;
                default: fprintf(stderr, "Unknown option: %c\n", optopt);
                }
        }

        uint8_t value = exec_option(&options);
        if (value > 0)
                return 0; /* A power option was performed! Everything's ok. */

        /* TODO: Check whether this works as expected. */
        if (as_daemon) {
                if (!options.monitor) {
                        logger("main",
                                "Monitor option is not set. Ignoring daemon option.\n",
                                stderr);
                        return EXIT_FAILURE;
                }

                logger("main", "Running as daemon...\n", stdout);
                pid_t pid;
                pid_t sid;
                pid = fork();
                if (pid < 0) {
                        logger("main", "Error while forking.\n", stderr);
                        return EXIT_FAILURE;
                } else if (pid > 0) {
                        return EXIT_SUCCESS;
                }

                umask(0);
                sid = setsid();
                if (sid < 0) {
                        logger("main", "Error while setting sid.\n", stderr);
                        return EXIT_FAILURE;
                }

                /* I've seen it's more recommendable to change the directory to /
                 * but for some reason, the program crashed when set to it, but
                 * I don't see any problems in moving to /tmp. */
                if ((chdir("/tmp")) < 0) {
                        logger("main", "Error while changing directory.\n", stderr);
                        return EXIT_FAILURE;
                }

                close(STDIN_FILENO);
                close(STDOUT_FILENO);
                close(STDERR_FILENO);
        }

        if (options.monitor)
                monitorize(options.monitor);

        return 0;
}

void
monitorize(bool monitor)
{
        if (monitor) {
                pthread_t id;
                pthread_create(&id, NULL, battery_monitor, NULL);
                pthread_join(id, NULL);
        }
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
        printf("\nGuardian options:\n");
        printf("-d | --daemon\t\tRun as daemon. Useful when combined with -m, and -f.\n");
        printf("-m | --monitor\t\tChecks whether the battery's charge is greater than 15%%.\n");
        printf("\t\t\tIf not, then suspends the computer.\n");
        printf("\nFile options:\n");
        printf("-f | --file=file\tFile where log is going to be saved.\n");
        printf("\nHelp options:\n");
        printf("-h | --help\t\tShows this help and exit.\n");
        printf("\nVerbose options:\n");
        printf("-D | --debug\t\tShows debug information.\n");
        printf("-v | --verbose\t\tBe verbose.\n");
        printf("\nMisc options:\n");
        printf("-l | --locker=locker\tScreen locker which would be run when suspending.\n");
        printf("-w | --wait=sec\t\tTime to wait before performing a power option.\n");
        printf("-V | --version\t\tShows program version and exists.\n");
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
                logger("exec_option",
                        "More than one option chosen. Turning off...\n",
                        stdout);
                spm_power(POWEROFF);
                return 1;
        }

        if (options->poweroff) {
                if (options->time_to_wait > 0)
                        run_timer(options->time_to_wait);
                spm_power(POWEROFF);
                return 1;
        }

        if (options->hibernate) {
                if (options->time_to_wait > 0)
                        run_timer(options->time_to_wait);
                spm_power(HIBERNATE);
                return 2;
        }

        if (options->reboot) {
                if (options->time_to_wait > 0)
                        run_timer(options->time_to_wait);
                spm_power(REBOOT);
                return 3;
        }

        if (options->suspend) {
                spm_power(SUSPEND);
                if (options->time_to_wait > 0)
                        run_timer(options->time_to_wait);
                return 4;
        }

        return 0;
}

void
run_timer(const size_t seconds)
{
        for (size_t i = seconds; i > 0; --i) {
                const size_t fmt_size = 23;
                char* message = format(fmt_size, "%ld seconds remaining...\n", i);
                logger("run_timer", message, stdout);
                free(message);
                sleep(1);
        }
}
