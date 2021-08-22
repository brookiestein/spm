#include "ask_for_locker.h"

static char*
get_screenlocker_path(GtkWindow* parent)
{
        GtkWidget* chooser = gtk_file_chooser_dialog_new("Choose a screen locker",
                        parent, GTK_FILE_CHOOSER_ACTION_OPEN,
                        "Cancel", GTK_RESPONSE_CANCEL,
                        "Open", GTK_RESPONSE_ACCEPT, NULL);

        char* file = NULL;
        int answer = gtk_dialog_run(GTK_DIALOG(chooser));

        if (answer == GTK_RESPONSE_ACCEPT)
                file = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(chooser));

        gtk_widget_destroy(chooser);

        return (file ? file : NULL);
}

char*
ask_for_locker(GtkWindow* parent)
{
        GtkDialogFlags flags = GTK_DIALOG_DESTROY_WITH_PARENT;
        GtkMessageType mtype = GTK_MESSAGE_QUESTION;
        GtkButtonsType btype = GTK_BUTTONS_YES_NO;
        const char* message  = "Do you want to set a screen locker?";
        char* locker = NULL;

        GtkWidget* dialog = gtk_message_dialog_new(parent,
                        flags, mtype, btype, "%s", message);

        gtk_window_set_title(GTK_WINDOW(dialog), "Set screen locker");
        int answer = gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);

        if (answer == GTK_RESPONSE_YES)
                locker = get_screenlocker_path(parent);

        return locker;
}
