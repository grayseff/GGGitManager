#include <gtk/gtk.h>
#include <stdio.h>
#include "window.h"
#include "repository.h"
#include <glib.h>
#include "git.h"
static void
activate(GtkApplication *app, gpointer data)
{

    GtkCssProvider *provider;

    provider = gtk_css_provider_new();

    char *css_path;

    css_path = g_build_filename(
        GGGM_DATADIR,
        "style.css",
        NULL
);

gtk_css_provider_load_from_path(
    provider,
    css_path
);

g_free(css_path);
    gtk_style_context_add_provider_for_display(
        gdk_display_get_default(),
        GTK_STYLE_PROVIDER(provider),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION
    );

    g_object_unref(provider);

    RepositoryList *repositories = data;

    create_window(app, repositories);
}

int
main(int argc, char **argv)
{
    
    
    GtkApplication *app;
    
    int status;

    app = gtk_application_new(
            "com.gray.gggitmanager",
            G_APPLICATION_DEFAULT_FLAGS
            );
    RepositoryList repositories;
    const char *home = g_get_home_dir();
    repository_list_init(&repositories);
    repository_list_scan(&repositories,home);
    
    g_signal_connect(
        app,
        "activate",
        G_CALLBACK(activate),
        &repositories
    );

    status = g_application_run(G_APPLICATION(app),argc,argv);
    repository_list_free(&repositories) ;
    g_object_unref(app);


    return status;
}
