#include "buttons.h"
#include "git.h"
#include "repository.h"
#include "window.h"


static void
show_error_dialog(
    const char *title,
    const char *message
)
{
    GtkWidget *dialog;
    GtkWidget *box;
    GtkWidget *label;
    GtkWidget *button;

    dialog = gtk_window_new();

    gtk_window_set_title(
        GTK_WINDOW(dialog),
        title
    );

    gtk_window_set_default_size(
        GTK_WINDOW(dialog),
        400,
        150
    );

    box = gtk_box_new(
        GTK_ORIENTATION_VERTICAL,
        12
    );

    gtk_widget_set_margin_top(box, 16);
    gtk_widget_set_margin_bottom(box, 16);
    gtk_widget_set_margin_start(box, 16);
    gtk_widget_set_margin_end(box, 16);

    label = gtk_label_new(message);

    gtk_label_set_wrap(
        GTK_LABEL(label),
        TRUE
    );

    button = gtk_button_new_with_label("OK");

    gtk_widget_set_halign(
        button,
        GTK_ALIGN_END
    );

    g_signal_connect_swapped(
        button,
        "clicked",
        G_CALLBACK(gtk_window_destroy),
        dialog
    );

    gtk_box_append(GTK_BOX(box), label);
    gtk_box_append(GTK_BOX(box), button);

    gtk_window_set_child(
        GTK_WINDOW(dialog),
        box
    );

    gtk_window_present(
        GTK_WINDOW(dialog)
    );
}
static void
on_add_clicked(
    GtkButton *button,
    gpointer data)
{
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
    } else {
        show_error_dialog(
                "Add Failed",
                "Git could not stage the repository changes"
                );
    }
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
    } else {
        show_error_dialog(
            "Commit failed",
            "Git could not commit the changes"
            );
    }
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


static void
on_pull_clicked(
    GtkButton *button,
    gpointer data)
{
    RepositoryView *view = data;

    if (view->selected_repository == NULL) {
        printf("No repository selected\n");
        return;
    }

    printf(
        "Pulling: %s\n",
        view->selected_repository->name
    );

    int result = git_pull(
        view->selected_repository->path
    );

    printf("git_pull result: %d\n", result);

    if (result == 0){
        refresh_repository_view(view);
    } else {
        show_error_dialog("Pull Failed",
                "Git could not pull the repository"
                );
    }
}

static void
on_push_clicked(
    GtkButton *button,
    gpointer data)
{
    RepositoryView *view = data;

    if (view->selected_repository == NULL) {
        printf("No repository selected\n");
        return;
    }

    printf(
        "Pushing: %s\n",
        view->selected_repository->name
    );

    int result = git_push(
        view->selected_repository->path
    );

    printf("git_push result: %d\n", result);

    if (result == 0) {
        refresh_repository_view(view);
    } else {
        show_error_dialog(
                "Push Failed",
                "Git could not push the repository"
                );
    }
}

GtkWidget *
create_action_bar(RepositoryView *view)
{
    GtkWidget *action_box;
    GtkWidget *add_button;
    GtkWidget *commit_button;
    GtkWidget *pull_button;
    GtkWidget *push_button;
    GtkWidget *action_frame;
    GtkWidget *separator;
    action_frame = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    separator = gtk_separator_new(
            GTK_ORIENTATION_HORIZONTAL
            );
    action_box = gtk_box_new(
            GTK_ORIENTATION_HORIZONTAL,
            6
            );

    gtk_widget_set_margin_top(action_box, 12);
    gtk_widget_set_margin_bottom(action_box, 12);
    gtk_widget_set_margin_start(action_box, 16);
    gtk_widget_set_margin_end(action_box, 16);

    gtk_widget_set_halign(
        action_box,
        GTK_ALIGN_END
    );

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

    g_signal_connect(
        pull_button,
        "clicked",
        G_CALLBACK(on_pull_clicked),
        view
    );

    g_signal_connect(
        push_button,
        "clicked",
        G_CALLBACK(on_push_clicked),
        view
    );
    gtk_box_append(
        GTK_BOX(action_frame),
        separator
    );

    gtk_box_append(
        GTK_BOX(action_frame),
        action_box
    );
    gtk_box_append(GTK_BOX(action_box), add_button);
    gtk_box_append(GTK_BOX(action_box), commit_button);
    gtk_box_append(GTK_BOX(action_box), pull_button);
    gtk_box_append(GTK_BOX(action_box), push_button);

    return action_frame;
}


