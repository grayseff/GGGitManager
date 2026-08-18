#include <gtk/gtk.h>
#include "window.h"
#include "repository.h"
#include "buttons.h"

#include "git.h"

static GtkWidget *create_repository_view(RepositoryList
        *repositories);

static GtkWidget *create_repository_row(const char *name, GitStatus *status);


void
create_window(GtkApplication *app, RepositoryList *repositories)
{
    GtkWidget *window;
    GtkWidget *header_bar;
    GtkWidget *menu_button;
    GtkWidget *content_box;


    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "GGGitManager");
    gtk_window_set_default_size(GTK_WINDOW(window), 700, 450);



    header_bar = gtk_header_bar_new();
    menu_button = gtk_menu_button_new();
    gtk_menu_button_set_icon_name(GTK_MENU_BUTTON(menu_button),
            "open-menu-symbolic"
            );
    gtk_header_bar_pack_end(GTK_HEADER_BAR(header_bar),
            menu_button
            );
    gtk_window_set_titlebar(GTK_WINDOW(window),
            header_bar
            );
    
   content_box = create_repository_view(repositories);
   gtk_window_set_child(
           GTK_WINDOW(window),
           content_box
           );

    gtk_window_present(GTK_WINDOW(window));
}

static void
    on_repository_selected(
        GtkListBox *box,
        GtkListBoxRow *row,
        gpointer data)
{
    RepositoryView *view = data;
    if (row == NULL)
            return;

    GtkWidget *repository_widget;

    repository_widget = gtk_list_box_row_get_child(row);

    Repository *repository = g_object_get_data(
        G_OBJECT(repository_widget),
        "repository"
    );

    if (repository == NULL) {
        printf("Repository is NULL!\n");
        return;
    }
    view->selected_repository = repository;
    printf(
        "Selected repository: %s\n",
        repository->name
    );

}
void
refresh_repository_view(RepositoryView *view)
{
    for (guint i = 0; ; i++) {
        GtkListBoxRow *list_row;

        list_row = gtk_list_box_get_row_at_index(
            GTK_LIST_BOX(view->repository_list),
            i
        );

        if (list_row == NULL)
            break;

        GtkWidget *row = gtk_list_box_row_get_child(list_row);

        Repository *repository = g_object_get_data(
            G_OBJECT(row),
            "repository"
        );

        GtkWidget *status_label = g_object_get_data(
            G_OBJECT(row),
            "status-label"
        );

        if (repository == NULL || status_label == NULL)
            continue;

        GitStatus status = git_get_status(repository->path);

        const char *status_text;

        if (status.untracked)
            status_text = "Untracked";
        else if (status.staged)
            status_text = "Staged";
        else if (status.modified)
            status_text = "Modified";
        else
            status_text = "Clean";

        gtk_label_set_text(
            GTK_LABEL(status_label),
            status_text
        );
    }

}
static GtkWidget *
create_repository_view(RepositoryList *repositories)
{
    GtkWidget *content_box;
    GtkWidget *list_header;
    GtkWidget *repository_label;
    GtkWidget *status_label;
    GtkWidget *separator;
    GtkWidget *action_bar;
    RepositoryView *view;

    view = g_new0(RepositoryView, 1);

    view->repositories = repositories;
    view->selected_repository = NULL;

    content_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    list_header = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

    gtk_widget_set_margin_top(list_header, 12);
    gtk_widget_set_margin_bottom(list_header, 8);
    gtk_widget_set_margin_start(list_header, 16);
    gtk_widget_set_margin_end(list_header, 16);

    repository_label = gtk_label_new("Repository");
    status_label = gtk_label_new("Status");
    separator = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);

    gtk_widget_add_css_class(repository_label, "heading");
    gtk_widget_add_css_class(status_label, "heading");

    gtk_widget_set_hexpand(repository_label, TRUE);

    gtk_label_set_xalign(GTK_LABEL(repository_label), 0.0);
    gtk_label_set_xalign(GTK_LABEL(status_label), 0.0);

    gtk_box_append(GTK_BOX(list_header), repository_label);
    gtk_box_append(GTK_BOX(list_header), status_label);
    gtk_box_append(GTK_BOX(content_box), list_header);
    gtk_box_append(GTK_BOX(content_box), separator);
    
    view->repository_list = gtk_list_box_new();
    gtk_widget_set_vexpand(view->repository_list,TRUE);

    gtk_box_append(
            GTK_BOX(content_box),
            view->repository_list);

    g_signal_connect(
        view->repository_list,
        "row-selected",
        G_CALLBACK(on_repository_selected),
        view
    );


    for (size_t i = 0; i < repositories->count; i++) {
        Repository *repository = &repositories->items[i];
        GitStatus status = git_get_status(repository->path);
       
        GtkWidget *repository_row;
        repository_row = create_repository_row(
            repository->name,
            &status
        );
                g_object_set_data(
                    G_OBJECT(repository_row),
                    "repository",
                    repository
                );
        gtk_list_box_append(GTK_LIST_BOX(view->repository_list), repository_row);
        }
    action_bar = create_action_bar(view);

        gtk_box_append(
            GTK_BOX(content_box),
            action_bar
        );




    return content_box;
}
static GtkWidget *
create_repository_row(const char *name, GitStatus *status)
{
    GtkWidget *row;
    GtkWidget *repo_icon;
    GtkWidget *name_label;
    GtkWidget *status_label;
    GtkWidget *status_box;
    GtkWidget *status_icon;

    row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

    gtk_widget_set_margin_top(row, 8);
    gtk_widget_set_margin_bottom(row, 8);
    gtk_widget_set_margin_start(row, 16);
    gtk_widget_set_margin_end(row, 16);

    repo_icon = gtk_image_new_from_icon_name("folder-symbolic");

    name_label = gtk_label_new(name);
    gtk_widget_set_hexpand(name_label, TRUE);
    gtk_label_set_xalign(GTK_LABEL(name_label), 0.0);

    const char *status_text;

    if (status->clean)
        status_text = "Clean";
    else if (status->staged)
        status_text = "Staged";
    else if (status->modified)
        status_text = "Modified";
    else if (status->untracked)
        status_text = "Untracked";
    else
        status_text = "Unknown";

    if (status->clean)
        status_icon = gtk_image_new_from_icon_name("emblem-ok-symbolic");
    else if (status->staged)
        status_icon = gtk_image_new_from_icon_name("document-save-symbolic");
    else if (status->modified)
        status_icon = gtk_image_new_from_icon_name("document-edit-symbolic");
    else if (status->untracked)
        status_icon = gtk_image_new_from_icon_name("document-new-symbolic");
    else
        status_icon = gtk_image_new_from_icon_name("dialog-question-symbolic");
    
    status_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);


    status_label = gtk_label_new(status_text);

    g_object_set_data(
        G_OBJECT(row),
        "status-label",
        status_label
    );


    gtk_label_set_xalign(GTK_LABEL(status_label), 0.0);
        gtk_box_append(GTK_BOX(status_box), status_icon);
    gtk_box_append(GTK_BOX(status_box), status_label);

    gtk_box_append(GTK_BOX(row), repo_icon);
    gtk_box_append(GTK_BOX(row), name_label);
    gtk_box_append(GTK_BOX(row), status_box);

        return row;
}

