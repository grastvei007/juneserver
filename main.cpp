#include "app.h"

#ifdef __linux__
    #include <stdio.h>
    #include <stdlib.h>
    #include <unistd.h>
    #include <signal.h>
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <syslog.h>

    #include <QStringList>
#endif

#ifndef __linux__
int main(int argc, char *argv[])
{
    App a(argc, argv);
    return a.exec();
}
#else


static void deamonize()
{
    pid_t pid;

    /* Fork off the parent process */
    pid = fork();

    /* An error occurred */
    if (pid < 0)
        exit(EXIT_FAILURE);

    /* Success: Let the parent terminate */
    if (pid > 0)
        exit(EXIT_SUCCESS);

    /* On success: The child process becomes session leader */
    if (setsid() < 0)
        exit(EXIT_FAILURE);

    /* Catch, ignore and handle signals */
    //TODO: Implement a working signal handler */
    signal(SIGCHLD, SIG_IGN);
    signal(SIGHUP, SIG_IGN);

    /* Fork off for the second time*/
    pid = fork();

    /* An error occurred */
    if (pid < 0)
        exit(EXIT_FAILURE);

    /* Success: Let the parent terminate */
    if (pid > 0)
        exit(EXIT_SUCCESS);

    /* Set new file permissions */
    umask(0);

    /* Change the working directory to the root directory */
    /* or another appropriated directory */
    chdir("/");

    /* Close all open file descriptors */
    int x;
    for (x = sysconf(_SC_OPEN_MAX); x>=0; x--)
    {
        close (x);
    }

    /* Open the log file */
    openlog ("juneserver", LOG_PID, LOG_DAEMON);
}


int main(int argc, char *argv[])
{
        QStringList list;
        for(int i=0; i<argc; ++i)
        {
            list << argv[i];
        }
        bool deamon = false;
        if(list.contains("deamon"))
        {
            deamon = true;
        }

        if(deamon)
        {
            deamonize();
            syslog (LOG_NOTICE, "JuneServer daemon started.");
        }

        /* Never ending loop of server */
        App a(argc, argv);
        a.exec();

        if(deamon)
        {
            syslog (LOG_NOTICE, "JuneServer daemon terminated.");
            closelog();

            return EXIT_SUCCESS;
        }
        return 0;




}
#endif
