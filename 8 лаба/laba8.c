#include <sys/stat.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>

int recieved1 = 0;
int received2 = 0;
int sent1 = 0;
int sent2 = 0;
int CurrProc = 1;

void handleSIGUSR(int SIG)
{
    if (SIG == SIGUSR1 && recieved1 < 101)
    {
        recieved1++;
    }
    if (SIG == SIGUSR2 && received2 < 101)
    {
        received2++;
    }
}

void handleSIGTERM()
{
    sleep(0.5);
    printf("\nCOMPLETE <<<< Number of proc=%d, pid=%d, ppid=%d, sented USR1=%d, USR2=%d,received USR1=%d,received USR2=%d >>>>\n",
           CurrProc, getpid(), getppid(), sent1, sent2, recieved1, received2);

    if (CurrProc != 1)
    {
        kill(getpid(), SIGKILL);
    }
    else
    {
        pid_t childpid = wait(NULL);
        while (childpid != -1)
        {
            childpid = wait(NULL);
        }
        exit(0);
    }
}

void makeGroup(pid_t p1, pid_t p2, pid_t p3)
{
    if (CurrProc == 2)
    {
        setpgid(p1, p1);
        setpgid(p2, p1);
        setpgid(p3, p1);
    }
}

int main()
{
    pid_t pid1, pid2, pid3, pid4, pid5, pid6, pid7, pid8;
    pid1 = getpid();
    signal(SIGUSR1, handleSIGUSR);
    signal(SIGUSR2, handleSIGUSR);
    signal(SIGTERM, handleSIGTERM);

    // создаем процессы
    if (CurrProc == 1)
    {
        pid2 = fork();
        if (pid2 == 0)
        {
            CurrProc = 2;
        }
    }
    if (CurrProc == 2)
    {
        pid3 = fork();
        if (pid3 == 0)
        {
            CurrProc = 3;
        }
    }
    if (CurrProc == 2)
    {
        pid4 = fork();
        if (pid4 == 0)
        {
            CurrProc = 4;
        }
    }
    if (CurrProc == 2)
    {
        pid5 = fork();
        if (pid5 == 0)
        {
            CurrProc = 5;
        }
    }
    if (CurrProc == 3)
    {
        pid7 = fork();
        if (pid7 == 0)
        {
            CurrProc = 7;
        }
    }
    if (CurrProc == 4)
    {
        pid6 = fork();
        if (pid6 == 0)
        {
            CurrProc = 6;
        }
    }
    if (CurrProc == 5)
    {
        pid8 = fork();
        if (pid8 == 0)
        {
            CurrProc = 8;
        }
    }

    // создаем группу и идентификатору группы присваиваем pid4
    makeGroup(pid4, pid5, pid6);
    sleep(2);

    // проверка на номер процесса и псоледуюшая его логика
    if (CurrProc == 1)
    {
        sent1 = 0;
        sent2 = 0;
        recieved1 = 0;
        received2 = 0;
        while (sent1 < 101)
        {
            kill(pid8, SIGUSR1);
            sleep(0.5);

            printf("\nNumber of proc=%d, pid=%d, ppid=%d,sent USR1=%d, USR2=%d\n",
                   CurrProc, getpid(), getppid(), sent1++, sent2);

            if (recieved1 < 101 && received2 < 101)
                printf("\nNumber of proc=%d, pid=%d, ppid=%d, received USR1=%d,received USR2=%d\n",
                       CurrProc, getpid(), getppid(), recieved1, received2);
        }
    }
    else if (CurrProc == 8)
    {

        sent1 = 0;
        sent2 = 0;
        recieved1 = 0;
        received2 = 0;

        while (sent1 < 101)
        {
            kill(pid7, SIGUSR1);
            sleep(0.5);

            printf("\nNumber of proc=%d, pid=%d, ppid=%d,sent USR1=%d, USR2=%d\n",
                   CurrProc, getpid(), getppid(), sent1++, sent2);

            if (recieved1 < 101 && received2 < 101)
                printf("\nNumber of proc=%d, pid=%d, ppid=%d, received USR1=%d,received USR2=%d\n",
                       CurrProc, getpid(), getppid(), recieved1, received2);
        }
    }
    else if (CurrProc == 7)
    {

        sent1 = 0;
        sent2 = 0;
        recieved1 = 0;
        received2 = 0;

        while (sent2 < 101)
        {
            kill(-pid4, SIGUSR2);
            sleep(0.5);

            printf("\nNumber of proc=%d, pid=%d, ppid=%d,sent USR1=%d, USR2=%d\n",
                   CurrProc, getpid(), getppid(), sent1, sent2++);

            if (recieved1 < 101 && received2 < 101)
                printf("\nNumber of proc=%d, pid=%d, ppid=%d, received USR1=%d,received USR2=%d\n",
                       CurrProc, getpid(), getppid(), recieved1, received2);
        }
    }
    else if (CurrProc == 4)
    {

        sent1 = 0;
        sent2 = 0;
        recieved1 = 0;
        received2 = 0;

        while (sent1 < 101)
        {
            kill(pid2, SIGUSR1);
            sleep(0.5);

            printf("\nNumber of proc=%d, pid=%d, ppid=%d,sent USR1=%d, USR2=%d\n",
                   CurrProc, getpid(), getppid(), sent1++, sent2);

            if (recieved1 < 101 && received2 < 101)
                printf("\nNumber of proc=%d, pid=%d, ppid=%d, received USR1=%d,received USR2=%d\n",
                       CurrProc, getpid(), getppid(), recieved1, received2);
        }
    }
    else if (CurrProc == 2)
    {

        sent1 = 0;
        sent2 = 0;
        recieved1 = 0;
        received2 = 0;

        while (sent1 < 101)
        {
            kill(pid3, SIGUSR1);
            sleep(0.5);

            printf("\nNumber of proc=%d, pid=%d, ppid=%d,sent USR1=%d, USR2=%d\n",
                   CurrProc, getpid(), getppid(), sent1++, sent2);

            if (recieved1 < 101 && received2 < 101)
                printf("\nNumber of proc=%d, pid=%d, ppid=%d, received USR1=%d,received USR2=%d\n",
                       CurrProc, getpid(), getppid(), recieved1, received2);
        }
    }
    else if (CurrProc == 3)
    {

        sent1 = 0;
        sent2 = 0;
        recieved1 = 0;
        received2 = 0;
        while (sent1 < 101)
        {
            kill(pid1, SIGUSR2);
            sleep(0.5);

            printf("\nNumber of proc=%d, pid=%d, ppid=%d,sent USR1=%d, USR2=%d\n",
                   CurrProc, getpid(), getppid(), sent1, sent2++);

            if (recieved1 < 101 && received2 < 101)
                printf("\nNumber of proc=%d, pid=%d, ppid=%d, received USR1=%d,received USR2=%d\n",
                       CurrProc, getpid(), getppid(), recieved1, received2);
        }
    }
    else if (CurrProc == 5)
    {
        sent1 = 0;
        sent2 = 0;
        recieved1 = 0;
        received2 = 0;
        while (received2 < 101)
        {
            sleep(0.5);

            printf("\nNumber of proc=%d, pid=%d, ppid=%d, received USR1=%d,received USR2=%d\n",
                   CurrProc, getpid(), getppid(), recieved1, received2);
        }
    }
    else if (CurrProc == 6)
    {
        sent1 = 0;
        sent2 = 0;
        recieved1 = 0;
        received2 = 0;
        while (received2 < 101)
        {
            sleep(0.5);

            printf("\nNumber of proc=%d, pid=%d, ppid=%d, received USR1=%d,received USR2=%d\n",
                   CurrProc, getpid(), getppid(), recieved1, received2);
        }
    }

    // уничтожение процессов, если процесс не отсылает сигнал, то посылаем SIGTERM ему самому, иначе детям, а потом ему
    if (CurrProc == 1)
        kill(0, SIGTERM);

    if (CurrProc == 2)
        kill(-pid3, SIGTERM);
        kill(pid2, SIGTERM);

    if (CurrProc == 3)
    {
        kill(pid7, SIGTERM);
        kill(pid3, SIGTERM);
    }
    if (CurrProc == 4)
    {
        kill(pid6, SIGTERM);
        kill(pid4, SIGTERM);
    }
    if (CurrProc == 5)
    {
        kill(pid8, SIGTERM);
        kill(pid5, SIGTERM);
    }
    if (CurrProc == 6)
        kill(pid6, SIGTERM);
    if (CurrProc == 7)
        kill(pid7, SIGTERM);
    if (CurrProc == 8)
        kill(pid8, SIGTERM);
}