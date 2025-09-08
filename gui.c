#include "gui.h"
#include "util.h"

#include <assert.h>
#include <gtk/gtk.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static char *formatLabel(const char *action, int seconds);
static void exec_option(GtkWidget *caller, void *data);
static void *updateTimeLabel(void *data);
static enum POWER_OPTION defaultOption;
static int seconds;

int
show_gui(int argc, char **argv, enum POWER_OPTION option, int secs)
{
	 const int width = 400;
	 const int height = 90;
	 
	 defaultOption = option;
	 seconds = secs;

	 gtk_init(&argc, &argv);

	 GtkAccelGroup *accelerator = gtk_accel_group_new();
	 
	 GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	 gtk_window_set_title(GTK_WINDOW(window), "System Power Manager");
	 gtk_window_set_default_size(GTK_WINDOW(window), width, height);
	 gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	 gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
	 gtk_window_add_accel_group(GTK_WINDOW(window), accelerator);

	 GtkWidget *questionLabel = gtk_label_new("What would you like to do?");
	 GtkWidget *timerLabel = gtk_label_new("");

	 GtkWidget *hibernateButton = gtk_button_new_with_label("Hibernate");
	 GtkWidget *powerOffButton = gtk_button_new_with_label("PowerOff");
	 GtkWidget *rebootButton = gtk_button_new_with_label("Reboot");
	 GtkWidget *suspendButton = gtk_button_new_with_label("Suspend");
	 GtkWidget *exitButton = gtk_button_new_with_label("Exit");

     GtkWidget *hibernateIcon = gtk_image_new_from_icon_name("system-shutdown", GTK_ICON_SIZE_BUTTON);
     GtkWidget *powerOffIcon = gtk_image_new_from_icon_name("system-shutdown", GTK_ICON_SIZE_BUTTON);
     GtkWidget *rebootIcon = gtk_image_new_from_icon_name("system-reboot", GTK_ICON_SIZE_BUTTON);
     GtkWidget *exitIcon = gtk_image_new_from_icon_name("application-exit", GTK_ICON_SIZE_BUTTON);

     gtk_button_set_image(GTK_BUTTON(hibernateButton), hibernateIcon);
     gtk_button_set_image(GTK_BUTTON(powerOffButton), powerOffIcon);
     gtk_button_set_image(GTK_BUTTON(rebootButton), rebootIcon);
     gtk_button_set_image(GTK_BUTTON(exitButton), exitIcon);

	 gtk_widget_add_accelerator(hibernateButton, "clicked", accelerator, GDK_KEY_h, 0, GTK_ACCEL_VISIBLE);
	 gtk_widget_add_accelerator(powerOffButton, "clicked", accelerator, GDK_KEY_p, 0, GTK_ACCEL_VISIBLE);
	 gtk_widget_add_accelerator(rebootButton, "clicked", accelerator, GDK_KEY_r, 0, GTK_ACCEL_VISIBLE);
	 gtk_widget_add_accelerator(suspendButton, "clicked", accelerator, GDK_KEY_s, 0, GTK_ACCEL_VISIBLE);
	 gtk_widget_add_accelerator(exitButton, "clicked", accelerator, GDK_KEY_Escape, 0, GTK_ACCEL_VISIBLE);

	 gtk_widget_set_tooltip_text(hibernateButton, "Save current work and power off the computer. (H)");
	 gtk_widget_set_tooltip_text(powerOffButton, "Power off the computer. (P)");
	 gtk_widget_set_tooltip_text(rebootButton, "Reboot the computer. (R)");
	 gtk_widget_set_tooltip_text(suspendButton, "Put the computer to sleep. (S)");
     gtk_widget_set_tooltip_text(exitButton, "Close the application. (Esc)");

	 GtkWidget *gridLayout = gtk_grid_new();
	 
	 gtk_grid_set_column_spacing(GTK_GRID(gridLayout), 5);
	 gtk_grid_set_row_spacing(GTK_GRID(gridLayout), 5);
	 gtk_grid_insert_row(GTK_GRID(gridLayout), 0);
	 
	 gtk_grid_attach(GTK_GRID(gridLayout), questionLabel,   0, 0, 5, 3);

     if (seconds >= 0)
	    gtk_grid_attach(GTK_GRID(gridLayout), timerLabel,      0, 3, 5, 3);

	 gtk_grid_attach(GTK_GRID(gridLayout), hibernateButton, 0, 6, 1, 1);
	 gtk_grid_attach(GTK_GRID(gridLayout), powerOffButton,  1, 6, 1, 1);
	 gtk_grid_attach(GTK_GRID(gridLayout), rebootButton,    2, 6, 1, 1);
	 gtk_grid_attach(GTK_GRID(gridLayout), suspendButton,   3, 6, 1, 1);
	 gtk_grid_attach(GTK_GRID(gridLayout), exitButton,      4, 6, 1, 1);

	 gtk_container_add(GTK_CONTAINER(window), gridLayout);

	 enum POWER_OPTION hibernate = HIBERNATE;
	 enum POWER_OPTION poweroff = POWEROFF;
	 enum POWER_OPTION reboot = REBOOT;
	 enum POWER_OPTION suspend = SUSPEND;
	 
	 g_signal_connect(window, "destroy", gtk_main_quit, NULL);
	 g_signal_connect(exitButton, "clicked", gtk_main_quit, NULL);
	 g_signal_connect(hibernateButton, "clicked", G_CALLBACK(exec_option), (void *) &hibernate);
	 g_signal_connect(powerOffButton, "clicked", G_CALLBACK(exec_option), (void *) &poweroff);
	 g_signal_connect(rebootButton, "clicked", G_CALLBACK(exec_option), (void *) &reboot);
	 g_signal_connect(suspendButton, "clicked", G_CALLBACK(exec_option), (void *) &suspend);

	pthread_t id;
     if (seconds >= 0)
	    pthread_create(&id, NULL, updateTimeLabel, (void *) timerLabel);
	 
	 gtk_widget_show_all(window);
	 gtk_main();

     if (seconds >= 0)
	    pthread_cancel(id);
	 
	 return 0;
}

static char *
formatLabel(const char *action, int seconds)
{
	 const int BUFFER_SIZE = 256;
	 char *message = malloc(BUFFER_SIZE * sizeof(char));
	 if (!message) {
		  die("Not enough memory to update time label.");
		  return NULL;
	 }

	 snprintf(
		  message,
		  BUFFER_SIZE,
		  "%s in %d second%s.",
		  action,
		  seconds,
		  (seconds > 1 ? "s" : "")
	 );

	 return message;
}

static void
exec_option(GtkWidget *caller, void *data)
{
	 (void) caller;
	 assert(data);
	 enum POWER_OPTION *option = (enum POWER_OPTION *) data;
	 spm_exec(*option);
	 gtk_main_quit();
}

static void *
updateTimeLabel(void *data)
{
	 GtkWidget *timerLabel = (GtkWidget *) data;

	 for (int i = seconds; i >= 0; --i) {
		  char *label = formatLabel(get_gerund(defaultOption), seconds--);
		  gtk_label_set_text(GTK_LABEL(timerLabel), label);
		  free(label);
		  sleep(1);
	 }

	 spm_exec(defaultOption);
	 gtk_main_quit();

	 return NULL;
}
