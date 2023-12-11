#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

#define MAX_PATH 1024

void process_directory(char *path, int minSize, int maxSize, time_t minTime, time_t maxTime,
					   FILE *outputFile, int maxProcesses)
{
	DIR *dir;
	struct dirent *entry;
	struct stat fileStat;
	char filePath[256];
	int numProcesses = 0;
	int filesNum = 0;

	dir = opendir(path);
	if (dir == NULL)
	{
		printf("Error: could not open directory %s\n", path);
		return;
	}

	while ((entry = readdir(dir)) != NULL)
	{
		if (entry->d_type == DT_DIR)
		{
			if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
			{
				snprintf(filePath, MAX_PATH, "%s/%s", path, entry->d_name);
				pid_t pid = fork();

				if (pid == 0)
				{
					process_directory(filePath, minSize, maxSize, minTime, maxTime, outputFile, maxProcesses);
					exit(0);
				}
				else if (pid > 0)
				{

					numProcesses++;
					// printf("\n num processes before: %d in filePath: %s", numProcesses, filePath);
					if (numProcesses >= maxProcesses)
					{
						wait(NULL);
						numProcesses--;
					}
				}
				else
				{
					printf("Error: could not create child process\n");
				}
			}
		}
		else
		{
			snprintf(filePath, MAX_PATH, "%s/%s", path, entry->d_name);
			if (stat(filePath, &fileStat) == 0)
			{
				filesNum++;
				if (fileStat.st_size >= minSize && fileStat.st_size <= maxSize && fileStat.st_ctime >= minTime && fileStat.st_ctime <= maxTime)
				{
					fprintf(outputFile, "\nname: %s, size: %d, creation time: %s, pid: %d, path: %s, visited files: %d\n", entry->d_name, fileStat.st_size, ctime(&fileStat.st_ctime), getpid(), path, filesNum);
					printf("\nname: %s, size: %d, creation time: %s, pid: %d, path: %s, visited files: %d\n", entry->d_name, fileStat.st_size, ctime(&fileStat.st_ctime), getpid(), path, filesNum);
				}
			}
			else
			{
				printf("Error: could not get file information for %s\n", filePath);
			}
		}
	}

	closedir(dir);
}

int main(int argc, char **argv)
{
	if (argc != 4)
	{
		printf("Usage: %s <directory> <outputFile> <maxProcesses>\n", argv[0]);
		return 1;
	}

	char *directory = argv[1];
	char *outputFileName = argv[2];
	int maxProcesses = atoi(argv[3]);

	FILE *outputFile = fopen(outputFileName, "w");
	if (outputFile == NULL)
	{
		printf("Error: could not open output file\n");
		return 1;
	}

	process_directory(directory, 0, 1000000000, 0, time(NULL), outputFile, maxProcesses);
	fclose(outputFile);

	return 0;
}
