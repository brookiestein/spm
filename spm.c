#include "spm.h"

int usage(void);
char** make_lowercase(const int* argc, char** argv);
char* spm_strncpy(char* dest, const char* src, const size_t* start, const size_t* end);
/* This function will delete the -, and -- from command line arguments if needed. */
char** make_easier_to_work(const int* argc, char** argv);
const char* look_for_an_option(const int* argc, char** argv, const char* large_options[],
                const char* short_options, const size_t* options_size, char** unrecognized,
                size_t* size);
int exec_option(const char* option);

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

        const size_t options_size = 6;
        const char* large_options[] = {
                "poweroff",
                "hibernate",
                "reboot",
                "suspend",
                "monitor",
                "help"
        };
        const char short_options[] = { 'p', 'h', 'r', 's', 'm', '?' };

        make_lowercase(&argc, argv);
        make_easier_to_work(&argc, argv);

        char* unrecognized_args[argc];
        size_t unrecognized_args_size = 0;

        const char* opt = look_for_an_option(&argc, argv, large_options,
                        short_options, &options_size, unrecognized_args,
                        &unrecognized_args_size);

        int status = exec_option(opt);

        if (unrecognized_args_size > 0) {
                char* message = format(26, "Argument%s not recognized: ",
                        (unrecognized_args_size > 1 ? "s" : ""));
                logger("main", &main, message, stderr);
                free(message);

                for (size_t i = 0; i < unrecognized_args_size; ++i) {
                        printf("%s%c", unrecognized_args[i],
                                (i < unrecognized_args_size - 1 ? ' ' : '\n'));
                        free(unrecognized_args[i]);
                }
        }

        return status;
}

int
usage(void)
{
        printf("SPM usage:\n\n");
        printf("Power options:\n");
        printf("[-]p | [--]poweroff\tShutdown the computer.\n");
        printf("[-]h | [--]hibernate\tHibernate the computer.\n");
        printf("[-]r | [--]reboot\tRestart the computer.\n");
        printf("[-]s | [--]suspend\tSuspend the computer.\n");
        printf("\nGuardian options:\n");
        printf("[-]m | [--]monitor\tCheck whether the battery's charge is greater than 15%%.\n");
        printf("\t\t\tIf not, then suspend the computer.\n");
        printf("\nHelp options:\n");
        printf("[-]? | [--]help\tShow this help and exit.\n");
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

const char*
look_for_an_option(const int* argc,
        char** argv, const char *large_options[],
        const char* short_options, const size_t* options_size,
        char** unrecognized, size_t* size)
{
        char *tmp = NULL;
        size_t len = 0;
        for (int i = 1; i < *argc; ++i) {
                for (size_t j = 0; j < *options_size; ++j) {
                        if (strncmp(argv[i], large_options[j], strlen(large_options[j])) == 0
                                || (argv[i][0] == short_options[j] && strlen(argv[i]) == 1)) {
                                return &short_options[j];
                        } else {
                                len = strlen(argv[i]);
                                tmp = (char*) malloc(len * sizeof(char));
                                strncpy(tmp, argv[i], len);
                        }
                }

                if (tmp) {
                        unrecognized[*size] = (char*) malloc(len * sizeof(char));
                        strncpy(unrecognized[*size], tmp, len);
                        free(tmp);
                        ++(*size);
                }
        }
        return NULL;
}

int
exec_option(const char* option)
{
        if (option == NULL)
                return EXIT_FAILURE;

        int status = EXIT_SUCCESS;

        switch (*option)
        {
        case 'p':
                spm_power(POWEROFF);
                break;
        case 'h':
                spm_power(HIBERNATE);
                break;
        case 'r':
                spm_power(REBOOT);
                break;
        case 's':
                spm_power(SUSPEND);
                break;
        case 'm':
                battery_monitor();
                break;
        case '?':
                status = usage();
                break;
        default:
                logger("exec_option", &exec_option, "Option unrecognized.\n", stderr);
        }
        return status;
}
