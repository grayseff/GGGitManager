#include "buttons.h"
#include "git.h"
#include "repository.h"
#include "window.h"



static void
on_add_clicked(
    GtkButton *button,
    gpointer data)
{
    printf("ADD CLICKED\n");

    RepositoryView *view = data;

    if (view->selected_repository == NULL) {
        printf("No repository selected\n");
        return;
    }

    printf(
        "Adding: %s\n",
        view->selected_repository->name
    );

    int result = git_add(
        view->selected_repository->path
    );
    if (result ==0){
        refresh_repository_view(view);
    }
    printf("git_add result: %d\n", result);
}
static void
on_commit_confirmed(
    GtkButton *button,
    gpointer data)
{
    RepositoryView *view = data;

    GtkWidget *message_entry = g_object_get_data(
        G_OBJECT(button),
        "message-entry"
    );

    const char *message = gtk_editable_get_text(
        GTK_EDITABLE(message_entry)
    );
    int result = git_commit(
            view->selected_repository->path,
            message
        );
    if (result == 0){
        refresh_repository_view(view);
        GtkWidget *dialog = g_object_get_data(
                    G_OBJECT(button),
                    "dialog"
                );
        gtk_window_destroy(GTK_WINDOW(dialog));
    }
    printf("git_commit result: %d\n", result);
}


static void
show_commit_dialog(RepositoryView *view)
{
    GtkWidget *dialog;
    GtkWidget *content_box;
    GtkWidget *message_entry;
    GtkWidget *button_box;
    GtkWidget *cancel_button;
    GtkWidget *commit_button;

    dialog = gtk_window_new();

    gtk_window_set_title(
        GTK_WINDOW(dialog),
        "Commit Changes"
    );

    gtk_window_set_default_size(
        GTK_WINDOW(dialog),
        400,
        150
    );

    content_box = gtk_box_new(
        GTK_ORIENTATION_VERTICAL,
        12
    );

    gtk_widget_set_margin_top(content_box, 16);
    gtk_widget_set_margin_bottom(content_box, 16);
    gtk_widget_set_margin_start(content_box, 16);
    gtk_widget_set_margin_end(content_box, 16);

    message_entry = gtk_entry_new();

    gtk_entry_set_placeholder_text(
        GTK_ENTRY(message_entry),
        "Commit message"
    );

    gtk_box_append(
        GTK_BOX(content_box),
        message_entry
    );

    button_box = gtk_box_new(
        GTK_ORIENTATION_HORIZONTAL,
        6
    );

    gtk_widget_set_halign(
        button_box,
        GTK_ALIGN_END
    );

    cancel_button = gtk_button_new_with_label("Cancel");
    commit_button = gtk_button_new_with_label("Commit");
    g_object_set_data(
        G_OBJECT(commit_button),
        "message-entry",
        message_entry
    );

    g_object_set_data(
        G_OBJECT(commit_button),
        "dialog",
        dialog
    );
    g_signal_connect(
        commit_button,
        "clicked",
        G_CALLBACK(on_commit_confirmed),
        view
    );

    g_signal_connect_swapped(
        cancel_button,
        "clicked",
        G_CALLBACK(gtk_window_destroy),
        dialog
    );
    gtk_box_append(
        GTK_BOX(button_box),
        cancel_button
    );

    gtk_box_append(
        GTK_BOX(button_box),
        commit_button
    );

    gtk_box_append(
        GTK_BOX(content_box),
        button_box
    );

    gtk_window_set_child(
        GTK_WINDOW(dialog),
        content_box
    );

    gtk_window_present(
        GTK_WINDOW(dialog)
    );
}
static void
on_commit_clicked(
    GtkButton *button,
    gpointer data)
{
    RepositoryView *view = data;

    if (view->selected_repository == NULL) {
        printf("No repository selected\n");
        return;
    }
    show_commit_dialog(view);
    }



GtkWidget *
create_action_bar(RepositoryView *view)
{
    GtkWidget *action_box;
    GtkWidget *add_button;
    GtkWidget *commit_button;
    GtkWidget *pull_button;
    GtkWidget *push_button;

    action_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);

    add_button = gtk_button_new_with_label("Add");
    g_signal_connect(
        add_button,
        "clicked",
        G_CALLBACK(on_add_clicked),
        view
    );
    commit_button = gtk_button_new_with_label("Commit");
g_signal_connect(
    commit_button,
    "clicked",
    G_CALLBACK(on_commit_clicked),
    view
);
    pull_button = gtk_button_new_with_label("Pull");
    push_button = gtk_button_new_with_label("Push");

    gtk_box_append(GTK_BOX(action_box), add_button);
    gtk_box_append(GTK_BOX(action_box), commit_button);
    gtk_box_append(GTK_BOX(action_box), pull_button);
    gtk_box_append(GTK_BOX(action_box), push_button);

    return action_box;
}
