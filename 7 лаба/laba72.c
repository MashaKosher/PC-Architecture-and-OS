#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>
#include <time.h>
#include <linux/stat.h>
#include <linux/fcntl.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <dirent.h>
#include <stdbool.h>
#include <pthread.h>

int min, max;
int maxThread, curThread = 1;

time_t mint, maxt;
FILE* outputFileName;

void* getinfo(void* param){
    char filePath[256];
    DIR* dir;
    struct dirent* entry;
    int filesNum = 0;
    char curfile[256];
    struct stat fileStat;
    char newdir[256];
    pthread_t tid;

    strcpy(filePath, (char*) param);
    dir = opendir(filePath);
    if (dir == NULL) {
        printf("Error: could not open directory %s\n", filePath);
        return;
    }

    strcat(filePath, "/");   


    while ((entry = readdir(dir))!=NULL) {
        memset(curfile, 0, sizeof(curfile));
        strcpy(curfile, filePath);
        strcat(curfile, entry->d_name);
        
        if (entry->d_type == DT_REG){
            stat(curfile, &fileStat);
            filesNum++;
            if(fileStat.st_size >= min && fileStat.st_size <= max 
            && fileStat.st_ctime >= mint && fileStat.st_ctime <= maxt){
                printf("%lu:%s;\t",pthread_self(), curfile);
                fprintf(outputFileName,"%lu:%s;\t",pthread_self(), curfile);
                printf("%lu: size = %d bytes\t time: %s\n", pthread_self(), (int)fileStat.st_size, ctime(&fileStat.st_ctime));
                fprintf(outputFileName,"%lu: size = %d bytes\t time: %s\n", pthread_self(), (int)fileStat.st_size,  ctime(&fileStat.st_ctime));
            }
        }else if (entry->d_type == DT_DIR && entry->d_name[0] != '.' && strcmp(entry->d_name, "..") != 0){
            if(curThread < maxThread){
                strcpy(newdir, curfile);
                curThread++;
                pthread_create(&tid, NULL, getinfo, (void* )newdir);
                printf("%lu: New thread, tid: %lu\n",pthread_self(), tid);
                fprintf(outputFileName,"%lu: Created new thread, tid: %lu\n",pthread_self(), tid);
                pthread_join(tid, NULL);
            }else{
                getinfo(curfile);
            }
        }
    }
    printf("%lu: Number of files in %s : %d\n",pthread_self(),filePath, filesNum);
    fprintf(outputFileName,"%lu: Number of files in %s : %d\n",pthread_self(),filePath, filesNum);
    closedir(dir);
    curThread--;
}

int main(int argc, char *argv[]) {
    
    char *directory = argv[1];
    outputFileName = fopen(argv[2], "w");
    min = atoi(argv[3]);
    max = atoi(argv[4]);
    mint = 0;
    maxt = time(NULL);

    maxThread = atoi(argv[5]);

    getinfo(directory);
    
    fclose(outputFileName);
    
    return 0;
}
