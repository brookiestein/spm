#include "gui.h"
#include "power_options.h"

static void
exec_option(GtkWidget* caller, gpointer unused)
{
        (void) unused;
        spm_power(gtk_button_get_label(GTK_BUTTON(caller)));
}

int
gui(int* argc, char** argv)
{
        gtk_init(argc, &argv);

        const size_t width = 390;
        const size_t height = 70;

        GtkWidget* window;
        GtkWidget* question_label;
        GtkWidget* grid_layout;
        GtkWidget* shutdown_button;
        GtkWidget* hibernate_button;
        GtkWidget* reboot_button;
        GtkWidget* suspend_button;
        GtkWidget* exit_button;
        GtkWidget* shutdown_image;
        GtkWidget* hibernate_image;
        GtkWidget* reboot_image;
        GtkWidget* suspend_image;
        GtkWidget* exit_image;

        char* icon_file = "resources/icons/icon.png";
        FILE* fp = fopen(icon_file, "r");
        bool use_icons = false;
        /* FIXME: If chdir() gives error, log it to the user when log option be available. */
        if (fp == NULL) {
                if (chdir("/usr/share/spm") == 0)
                        use_icons = true;
        } else {
                use_icons = true;
        }

        window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        gtk_window_set_title(GTK_WINDOW(window), "System Power Manager");
        gtk_window_set_default_size(GTK_WINDOW(window), width, height);
        gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
        gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

        if (use_icons)
                gtk_window_set_icon_from_file(GTK_WINDOW(window), icon_file, NULL);

        question_label = gtk_label_new("What would you like to do?");
        grid_layout = gtk_grid_new();

        shutdown_button = gtk_button_new_with_label(POWEROFF);
        hibernate_button = gtk_button_new_with_label(HIBERNATE);
        reboot_button = gtk_button_new_with_label(REBOOT);
        suspend_button = gtk_button_new_with_label(SUSPEND);
        exit_button = gtk_button_new_with_label("Exit");

        if (use_icons) {
                shutdown_image = gtk_image_new_from_file("resources/icons/shutdown.png");
                hibernate_image = gtk_image_new_from_file("resources/icons/hibernate.png");
                reboot_image = gtk_image_new_from_file("resources/icons/reboot.png");
                suspend_image = gtk_image_new_from_file("resources/icons/suspend.png");
                exit_image = gtk_image_new_from_file("resources/icons/exit.png");

                gtk_button_set_image(GTK_BUTTON(shutdown_button), shutdown_image);
                gtk_button_set_image(GTK_BUTTON(hibernate_button), hibernate_image);
                gtk_button_set_image(GTK_BUTTON(reboot_button), reboot_image);
                gtk_button_set_image(GTK_BUTTON(suspend_button), suspend_image);
                gtk_button_set_image(GTK_BUTTON(exit_button), exit_image);
        }

        gtk_grid_set_column_spacing(GTK_GRID(grid_layout), 5);
        gtk_grid_set_row_spacing(GTK_GRID(grid_layout), 5);
        gtk_grid_insert_row(GTK_GRID(grid_layout), 0);
        gtk_grid_attach(GTK_GRID(grid_layout), question_label, 0, 0, 5, 3);
        gtk_grid_attach(GTK_GRID(grid_layout), shutdown_button, 0, 3, 1, 1);
        gtk_grid_attach(GTK_GRID(grid_layout), hibernate_button, 1, 3, 1, 1);
        gtk_grid_attach(GTK_GRID(grid_layout), reboot_button, 2, 3, 1, 1);
        gtk_grid_attach(GTK_GRID(grid_layout), suspend_button, 3, 3, 1, 1);
        gtk_grid_attach(GTK_GRID(grid_layout), exit_button, 4, 3, 1, 1);

        gtk_container_add(GTK_CONTAINER(window), grid_layout);

        g_signal_connect(window, "destroy", gtk_main_quit, NULL);
        g_signal_connect(shutdown_button, "clicked", G_CALLBACK(exec_option), NULL);
        g_signal_connect(hibernate_button, "clicked", G_CALLBACK(exec_option), NULL);
        g_signal_connect(reboot_button, "clicked", G_CALLBACK(exec_option), NULL);
        g_signal_connect(suspend_button, "clicked", G_CALLBACK(exec_option), NULL);
        g_signal_connect(exit_button, "clicked", gtk_main_quit, NULL);

        gtk_widget_show_all(window);
        gtk_main();
        return 0;
}
