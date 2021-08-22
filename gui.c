#include "gui.h"
#include "power_options.h"

#ifdef _ASK_FOR_LOCKER
#include "ask_for_locker.h"
#endif

static void
spm_exit(void)
{
        gtk_main_quit();
}

static void
exec_option(GtkWidget* caller, gpointer parent)
{
        pthread_t id;
        char* locker = NULL;

#ifdef _ASK_FOR_LOCKER
        locker = ask_for_locker(GTK_WINDOW(parent));
#endif

        pthread_create(&id, NULL, run_locker, (void*) locker);
        spm_power(gtk_button_get_label(GTK_BUTTON(caller)));

        if (locker) {
                g_free(locker);
                pthread_join(id, NULL);
        }

        spm_exit();
}

uint8_t
gui(int* argc, char** argv)
{
        gtk_init(argc, &argv);

        const uint16_t width = 400;
        const uint16_t height = 70;

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
        GtkAccelGroup* accel;

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

        accel  = gtk_accel_group_new();
        window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        gtk_window_set_title(GTK_WINDOW(window), "System Power Manager");
        gtk_window_set_default_size(GTK_WINDOW(window), width, height);
        gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
        gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
        gtk_window_add_accel_group(GTK_WINDOW(window), accel);

        if (use_icons)
                gtk_window_set_icon_from_file(GTK_WINDOW(window), icon_file, NULL);

        question_label = gtk_label_new("What would you like to do?");
        grid_layout = gtk_grid_new();

        shutdown_button = gtk_button_new_with_label(POWEROFF);
        hibernate_button = gtk_button_new_with_label(HIBERNATE);
        reboot_button = gtk_button_new_with_label(REBOOT);
        suspend_button = gtk_button_new_with_label(SUSPEND);
        exit_button = gtk_button_new_with_label("Exit");

        gtk_widget_add_accelerator(shutdown_button, "clicked", accel, GDK_KEY_p, 0, GTK_ACCEL_VISIBLE);
        gtk_widget_add_accelerator(hibernate_button, "clicked", accel, GDK_KEY_h, 0, GTK_ACCEL_VISIBLE);
        gtk_widget_add_accelerator(reboot_button, "clicked", accel, GDK_KEY_r, 0, GTK_ACCEL_VISIBLE);
        gtk_widget_add_accelerator(suspend_button, "clicked", accel, GDK_KEY_s, 0, GTK_ACCEL_VISIBLE);
        gtk_widget_add_accelerator(exit_button, "clicked", accel, GDK_KEY_Escape, 0, GTK_ACCEL_VISIBLE);

        gtk_widget_set_tooltip_text(shutdown_button, "Shutdown the computer. (P)");
        gtk_widget_set_tooltip_text(hibernate_button, "Shutdown the computer saving your current session. (H)");
        gtk_widget_set_tooltip_text(reboot_button, "Reboot the computer. (R)");
        gtk_widget_set_tooltip_text(suspend_button, "Suspend the computer. (S)");
        gtk_widget_set_tooltip_text(exit_button, "Close the program. (ESC)");

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
        g_signal_connect(shutdown_button, "clicked", G_CALLBACK(exec_option), window);
        g_signal_connect(hibernate_button, "clicked", G_CALLBACK(exec_option), window);
        g_signal_connect(reboot_button, "clicked", G_CALLBACK(exec_option), window);
        g_signal_connect(suspend_button, "clicked", G_CALLBACK(exec_option), window);
        g_signal_connect(exit_button, "clicked", G_CALLBACK(spm_exit), NULL);

        gtk_widget_show_all(window);
        gtk_main();
        return 0;
}
