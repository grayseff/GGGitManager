#include <gtk/gtk.h>
#include <stdio.h>
#include "window.h"
#include "repository.h"
#include "git.h"
static void
activate(GtkApplication *app, gpointer data)
{

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

repository_list_init(&repositories);
repository_list_scan(&repositories, "/home/grayseff/lab");

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
