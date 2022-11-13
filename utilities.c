#include "utilities.h"

void runCommand(char *command)
{
    FILE *pipe = popen(command, "r");

    char buffer[1024];
    while (!feof(pipe) && !ferror(pipe))
    {
        if (fgets(buffer, sizeof(buffer), pipe) == NULL)
            break;
    }

    int exitCode = WEXITSTATUS(pclose(pipe));

    if (exitCode != 0)
    {
        perror("Unable to execute command:");
        printf(" command: %s\n", command);
        printf(" exit code: %d\n", exitCode);
    }
}

bool readUser()
{
   bool button = 0;
   FILE *pFile = fopen(USER, "r");
   if (pFile == NULL)
   {
      printf("ERROR: Unable to open file (%s) for read\n", USER);
      exit(-1);
   }
   else
   {
      char buff[2];
      fgets(buff, 2, pFile);
      if (buff[0] == '0') // inverse the bits (by default 0 means pressed)
      {
        button = 1;
      }
      fclose(pFile);
   }
   return button;
}

long long getTimeInMs(void)
{
	struct timespec spec;
	clock_gettime(CLOCK_REALTIME, &spec);
	long long seconds = spec.tv_sec;
	long long nanoSeconds = spec.tv_nsec;
	long long microSeconds = seconds * 1000000
	+ nanoSeconds / 1000;
	return microSeconds;
}

void sleepForMs(long long delayInMs)
{
	const long long NS_PER_MS = 1000 * 1000;
	const long long NS_PER_SECOND = 1000000000;
	long long delayNs = delayInMs * NS_PER_MS;
	int seconds = delayNs / NS_PER_SECOND;
	int nanoseconds = delayNs % NS_PER_SECOND;
	struct timespec reqDelay = {seconds, nanoseconds};
	nanosleep(&reqDelay, (struct timespec *) NULL);
}

