#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>

int countFilesAndSize(char *path, FILE *outputFile)
{
    DIR *dir;
    int finSize = 0;
    struct dirent *entry;
    struct stat fileStat;
    char filePath[256];
    int totalFiles = 0;
    int totalSize = 0;
    int maxSize = 0;
    char maxFileName[256];

    dir = opendir(path);
    if (dir == NULL)
    {
        fprintf(stderr, "Error opening directory %s\n", path);
        return finSize;
    }

    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_type == DT_DIR)
        {
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
            {
                sprintf(filePath, "%s/%s", path, entry->d_name);
                finSize += countFilesAndSize(filePath, outputFile);
            }
        }
        else
        {
            sprintf(filePath, "%s/%s", path, entry->d_name);
            if (stat(filePath, &fileStat) == 0)
            {
                totalFiles++;
                totalSize += fileStat.st_size;
                finSize += totalSize;
                if (fileStat.st_size > maxSize)
                {
                    maxSize = fileStat.st_size;
                    strcpy(maxFileName, entry->d_name);
                }
            }
            else
            {
                fprintf(stderr, "Error getting file stats for %s\n", filePath);
            }
        }
    }

    fprintf(outputFile, "Directory: %s\n", path);
    fprintf(outputFile, "Total files: %d\n", totalFiles);
    fprintf(outputFile, "Total size: %lld bytes\n", totalSize);
    fprintf(outputFile, "File with max size: %s\n", maxFileName);
    fprintf(outputFile, "Finsize: %d\n", finSize);
    fprintf(outputFile, "\n");

    closedir(dir);
    return finSize;
}

int main(int argc, char *argv[])
{

    if (argc < 3)
    {
        fprintf(stderr, "Usage: %s <directory> <output_file>\n", argv[0]);
        return 1;
    }

    FILE *outputFile = fopen(argv[2], "w");
    if (outputFile == NULL)
    {
        fprintf(stderr, "Error opening output file\n");
        return 1;
    }

    countFilesAndSize(argv[1], outputFile);

    fclose(outputFile);

    return 0;
}