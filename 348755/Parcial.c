#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <ctype.h>

void get_process_info(char *filename) {
    long int id, priority,vsize;
    char comm[1024];
    char line[1000];

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("No se pudo abrir el directorio /proc/[pid]/stat\n");
        return;
    }

    while (fgets(line, sizeof(line), file)) {
        printf ("\n");
            sscanf(line, "%d %s %*c %*d %*d %*d %*d %*d %*u %*lu %*lu %*lu %*lu %*lu %*lu %*ld %*ld %ld %*ld %*ld %*ld %*lu %lu,",
            &id, comm, &priority, &vsize);

            printf("ID: %d\n", id);
            printf("NOMBRE: %s\n", comm);
            printf("PRIORIDAD: %d\n", priority);
            printf("MEMORIA: %lu kB\n", vsize);
    }
    fclose(file);
}

void get_process_type(char *filname){
    char n;
    FILE *file = fopen(filname,"r");
    
    if(file==NULL){
        printf("No se pudo abrir el directorio /proc/[pid]/cmdline\n");
        return;
    }else{
        if((n=fgetc(file))==EOF){
            printf("PROCESO: Kernel\n");
        }else{
            printf("PROCESO: Usuario\n");
        }
    }
    fclose(file);
}

int main() {
    DIR *dir;
    struct dirent *ent;
    char pid[10];
    char filename[100];

    // Abrir directorio /proc
    dir = opendir("/proc");
    if (dir == NULL) {
        printf("No se pudo abrir el directorio /proc\n");
        return 1;
    }

    // Leer directorio /proc
    while ((ent = readdir(dir))) {
        if (isdigit (ent->d_name[0])) {
            strcpy(pid, ent->d_name);
            sprintf(filename, "/proc/%s/stat", pid);
            get_process_info(filename);
            strcpy(pid, ent->d_name);
            sprintf(filename, "/proc/%s/cmdline", pid);
            get_process_type(filename);
        }
    }
    closedir(dir);

    FILE *meminfo = fopen("/proc/meminfo", "r");
    if (meminfo == NULL) {
        printf("No se pudo abrir el directorio /proc/meminfo\n");
        return 1;
    }
    char line[100];
    long long total_memory = 0;
    while (fgets(line, sizeof(line), meminfo)) {
        if (sscanf(line, "MemTotal: %lld", &total_memory) == 1) {
            break;
        }
    }
    fclose(meminfo);
    printf("\nMEMORIA TOTAL: %lld kB\n", total_memory);
    return 0;
}
