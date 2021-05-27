#include "spm.h"

int
main(int argc, char** argv)
{
        if (argc == 1) {
#ifdef _ENABLE_GUI
                logger("main", &main, "Using GUI...\n", stdout);
                return gui(&argc, argv);
#else
                logger("main", &main, "The program was not compiled with GUI support.\n", stderr);
#endif
                return 0;
        }

        const uint8_t options_size = 7;
        const char* large_options[] = {
                "poweroff",
                "hibernate",
                "reboot",
                "suspend",
                "monitor",
                "daemon",
                "help",
        };
        const char short_options[] = { 'p', 'h', 'r', 's', 'm', 'd', '?' };

        make_lowercase(&argc, argv);
        make_easier_to_work(&argc, argv);

        char* unrecognized_args[argc];
        size_t unrecognized_args_size = 0;

        Options options;
        options.daemon = false;
        options.monitor = false;

        look_for_an_option(&argc, argv, large_options,
                        short_options, &options_size, unrecognized_args,
                        &unrecognized_args_size, &options);

        /* TODO: Check whether this works as expected. */
        if (options.daemon)
        {
                if (!options.monitor) {
                        logger("main", &main,
                                "Monitor option is not set. Ignoring daemon option.\n", stderr);
                } else {
                        logger("main", &main, "Trying to run as a daemon...\n", stdout);
                        pid_t pid;
                        pid_t sid;
                        pid = fork();
                        if (pid < 0) {
                                logger("main", &main, "Error while forking.\n", stderr);
                                return EXIT_FAILURE;
                        } else if (pid > 0) {
                                return EXIT_SUCCESS;
                        }

                        umask(0);
                        sid = setsid();
                        if (sid < 0) {
                                logger("main", &main, "Error while setting sid.\n", stderr);
                                return EXIT_FAILURE;
                        }

                        if ((chdir("/")) < 0) {
                                logger("main", &main, "Error while changing directory.\n", stderr);
                                return EXIT_FAILURE;
                        }

                        close(STDIN_FILENO);
                        close(STDOUT_FILENO);
                        close(STDERR_FILENO);
                }
        }

        pthread_t monitor_id;
        if (options.monitor)
                pthread_create(&monitor_id, NULL, &battery_monitor, NULL);

        uint8_t status = exec_option(&options);

        if (unrecognized_args_size > 0)
        {
                const char fmt[] = "Argument%s not recognized: ";
                const size_t size = strlen(fmt);
                char* message = format(size, fmt, (unrecognized_args_size > 1 ? "s" : ""));
                logger("main", &main, message, stderr);
                free(message);

                for (size_t i = 0; i < unrecognized_args_size; ++i)
                {
                        printf("%s%c", unrecognized_args[i],
                                (i < unrecognized_args_size - 1 ? ' ' : '\n'));
                        free(unrecognized_args[i]);
                }
        }

        if (options.monitor)
                pthread_join(monitor_id, NULL);

        return status;
}

uint8_t
usage(void)
{
        printf("SPM usage:\n\n");
        printf("Power options:\n");
        printf("[-]p | [--]poweroff\tShutdowns the computer.\n");
        printf("[-]h | [--]hibernate\tHibernates the computer.\n");
        printf("[-]r | [--]reboot\tRestarts the computer.\n");
        printf("[-]s | [--]suspend\tSuspends the computer.\n");
        printf("\nGuardian options:\n");
        printf("[-]d | [--]daemon\tRun as daemon. Useful when combined with -m, and -f.\n");
        printf("[-]m | [--]monitor\tChecks whether the battery's charge is greater than 15%%.\n");
        printf("\t\t\tIf not, then suspends the computer.\n");
        printf("\nFile options:\n");
        printf("[-]f | [--]file file\tFile where log is going to be saved.\n");
        printf("\nHelp options:\n");
        printf("[-]? | [--]help\t\tShows this help and exit.\n");
        printf("\nGUI Options:\n");
        printf("Run without arguments to show the GUI.\n");
        return 0;
}

char**
make_lowercase(const int* argc, char** argv)
{
        for (int i = 1; i < *argc; ++i) {
                const size_t size = strlen(argv[i]);
                for (size_t j = 0; j < size; ++j)
                        argv[i][j] = tolower(argv[i][j]);
        }
        return argv;
}

char*
spm_strncpy(char* dest, const char* src, const size_t* start, const size_t* end)
{
        for (size_t i = *start, j = 0; i < *end; ++i, ++j)
                dest[j] = src[i];
        dest[strlen(dest) - *start] = '\0';
        return dest;
}

char**
make_easier_to_work(const int* argc, char** argv)
{
        for (int i = 0; i < *argc; ++i) {
                bool delete = false;
                size_t start = 0;

                if (argv[i][0] == '-') {
                        delete = true;
                        ++start;
                        if (strlen(argv[i]) > 1 && argv[i][1] == '-')
                                ++start;
                }

                if (delete) {
                        const size_t size = strlen(argv[i]);
                        char* tmp = (char*) malloc(size * sizeof(char));
                        strncpy(tmp, argv[i], size);
                        spm_strncpy(argv[i], tmp, &start, &size);
                        free(tmp);
                }
        }
        return argv;
}

Options*
look_for_an_option(const int* argc,
        char** argv, const char *large_options[],
        const char* short_options, const uint8_t* options_size,
        char** unrecognized, size_t* size, Options* options)
{
        /* char *tmp = NULL; */
        uint16_t len = 0;
        for (int i = 1; i < *argc; ++i) {
                bool found_options = false;
                for (size_t j = 0; j < *options_size; ++j) {
                        if (strncmp(argv[i], large_options[j], strlen(large_options[j])) == 0
                                || (argv[i][0] == short_options[j] && strlen(argv[i]) == 1))
                        {
                                found_options = true;
                                switch (short_options[j])
                                {
                                case 'd': options->daemon = true; break;
                                case 'm': options->monitor = true; break;
                                case 'p': options->poweroff = true; break;
                                case 'h': options->hibernate = true; break;
                                case 'r': options->reboot = true; break;
                                case 's': options->suspend = true; break;
                                case '?': options->help = true; break;
                                }
                        }
                }

                if (!found_options)
                {
                        len = strlen(argv[i]);
                        unrecognized[*size] = (char*) malloc(len * sizeof(char));
                        strncpy(unrecognized[*size], argv[i], len);
                        ++(*size);
                }
        }
        return options;
}

bool
confirm(void)
{
        printf("Continue? [y/n]: ");
        const uint8_t size = UINT8_MAX;
        char* input = (char*) malloc(size * sizeof(char));
        input = fgets(input, size, stdin);
        bool response = !strncmp(input, "y", 1);
        free(input);
        return response;
}

uint8_t
exec_option(const Options *options)
{
        if (options->help)
                return usage();

        bool perform = true;

        if ((options->poweroff || options->hibernate) && (options->reboot || options->suspend))
        {
                logger("exec_option", &exec_option, "Running more than one power option.\n", stdout);
                printf("Please, confirm that want really want to perform them. This is not recommendable!\n");
                perform = confirm();
                if (perform)
                {
                        logger("exec_option", &exec_option, "Performing anyway. Just take in account that \
the options will be performed in the following order:\n", stdout);
                        logger("exec_option", &exec_option,
                        "\n- Shutdown\n- Hibernate\n- Reboot\n- Suspend\n",
                        stdout);
                }
                else
                {
                        logger("exec_option", &exec_option, "Performing nothing.\n", stdout);
                        return 1;
                }
        }

        if (options->poweroff)
                spm_power(POWEROFF);
        if (options->hibernate)
                spm_power(HIBERNATE);
        if (options->reboot)
                spm_power(REBOOT);
        if (options->suspend)
                spm_power(SUSPEND);
        return 0;
}
