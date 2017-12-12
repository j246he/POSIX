#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>

const char * requests = "/tmp/.jobrequests";

void process_requests()
{
    /* shared access to this directory by multiple processes:
       Option 1: create a lock file (e.g., setup a desginated 
       filename such as .locked that all processes agree on).

       Have to be careful:  we don't want two processes that 
       concurrently attempt to create the file to be victims 
       of a race condition and have both believe that they 
       locked the directory.

       POSIX guarantees that the call to open (to open a file) 
       with flags O_CREATE | O_EXCL is atomic:  it *atomically* 
       either creates the file, or the call fails if the file 
       already exists --- no race condition possible:  if two 
       or more processes attempt this concurrently, only one 
       will create the file and will know it was the one who 
       created it, and the rest will know that they failed to 
       create the file.
    */

    static int filename_counter = 1;
    pid_t pid = getpid();

    chdir (requests);
    DIR * dir = opendir (requests);
    if (dir != NULL)
    {
        struct dirent * entry;
        while ((entry = readdir(dir)) != NULL)
        {
            struct stat statbuf;
            stat (entry->d_name, &statbuf);
            if (S_ISREG (statbuf.st_mode) 
                && strncmp (entry->d_name, "locked", 6) != 0
                && strncmp (entry->d_name, "tmp", 3) != 0)
            {
                char date[16], time[16], user[32], ip[16];
                char unique_filename[32];
                sprintf (unique_filename, "locked-%x-%x.txt", pid, filename_counter++);
                if (rename (entry->d_name, unique_filename) == 0)
                {
                    FILE * file = fopen (unique_filename, "r");
                    if (file)
                    {
                        fscanf (file, "%s %s %s %s", date, time, user, ip);
                        printf ("Process %d found file: %s: Request from %s (from IP %s) at %s %s\n", 
                                pid, entry->d_name, user, ip, date, time);
                        remove (unique_filename);
                        usleep (5000000);
                        break;
                    }
                }
                else
                {
                    printf ("Process %d failed to lock file\n", pid);
                }
            }
        }
        closedir (dir);
    }
    else
    {
        printf ("Could not read directory -- %d\n", errno);
    }
}

int main()
{
    int i;
    for (i = 0; i < 20; ++i)
    {
        if (fork() == 0)
           // fork() returns 0 for the child process and nonzero for the parent 
           // (containing the PID of the child process).  We want the child processes 
           // to go on and process requests (several of them running concurrently) 
           // while the parent continues in the loop spawning additional child processes 
           // and then does nothing.
        {
            srand48(time(NULL));

            while (1)
            {
                process_requests();
                usleep (1000000*drand48());
                    /* Wait a random amount of time between 0 and 1 second */
            }
            return 0;
        }
    }

    while (1) { usleep(10000); }
        // This is just so that the parent does not exit and we can see the "tree" 
        // representation when running  ps --forest  (in a normal application, we 
        // could perhaps have the parent exit after having spawned the child processes; 
        // or it could go on and do entirely different things)

    return 0;
}

