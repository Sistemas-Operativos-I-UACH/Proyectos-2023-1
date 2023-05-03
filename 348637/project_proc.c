#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h>
#include <ctype.h>

//#define MAX_LINE_LENGTH 200

void showStat(char* name, char* buff);
int isPID(char* name);
void getCommand(char* name, char* cmdline);
void getRAMUsage(char* name, char* size);
int startsWith(const char *a, const char *b);
void cleanRAMUsage(char* ramUsage, char* out);
void getTotalRAM(char* out);

int main(void) {
    char buff[1024];                                // Declara un arreglo de caracteres, utilizado para almacenar el contenido de proc
    DIR *d;                                         // Declara un puntero a DIR
    struct dirent *dir;         
    d = opendir("/proc");                           // Abre el directorio proc
    if (d) {                                        // Si se logo abrir correctamente
        while ((dir = readdir(d)) != NULL) {        // Itera cada archivo del directorio utlizando readdir y se iguala ala variable `dir`
            if(isPID(dir->d_name))
                showStat(dir->d_name,buff);
        }
        char totalRAM[100];
        getTotalRAM(totalRAM);
        printf("-----------\nLa memoria total del sistema es: %s",totalRAM);
        closedir(d);
    }
    return 0;
}

void showStat(char* name, char* buff) {             // 
    char dir[256] = "/proc/";
    strcat(dir,name);                               // Se concadeba /proc/ + nombre del directorio del proceso
    strcat(dir, "/stat");                           // + /stat
    int fd = open(dir,O_RDONLY);                    // Se abre el archivo stat del proceso
    read(fd,buff,1024);                             // Se lee el archivo, y se almacena el buff
    char sname[15];                                 
    char* pid = strtok(buff," ");                   // Se obtiene el PID
    char* pname = strtok(NULL," ");                 // Se obtiene el nombre del proceso
    for(int i = 0; i < 16; i++) strtok(NULL," ");   
    char* priority = strtok(NULL, " ");             // Se obtiene el valor de la propridad del proceso
    char cmdline[1024] = "";                        
    getCommand(name,cmdline);                       
    int isKernel = strlen(cmdline) == 2;            // 
    char kresp[3] = "No";
    if(isKernel != 0) {
        strcpy(kresp,"Si");
    }
    char ramUsage[1024];
    getRAMUsage(name,ramUsage);                     // Obtiene la ram que esta utilizando el proceso
    if(startsWith(ramUsage,"VmRSS:")) {
        char cram[100];
        cleanRAMUsage(ramUsage,cram);
        printf("PID: %s | Nombre: %s | Prioridad: %s | Es proceso del kernel: %s | RAM: %s\n",pid,pname,priority,kresp,cram,ramUsage);
    }else{
        printf("PID: %s | Nombre: %s | Prioridad: %s | Es proceso del kernel: %s\n",pid,pname,priority,kresp);
    }
}


int isPID(char* name) {
    for (int i = 0; i < strlen(name); i++) {        // Se recorre cada caracter de name
      if(!isdigit(name[i]))                         // Si no es un dijito devuele 0, pero si si continua con la cadena
        return 0;
   }
   return 1;                                        // SI devuelve 1 es un PID valido
}

void cleanRAMUsage(char* ramUsage, char* out) {
    int currInd = 6;
    while(ramUsage[currInd] == 9 || ramUsage[currInd] == 32) {
        currInd++;
    }
    strcpy(out,ramUsage+currInd);
}

void getRAMUsage(char* name, char* size) {
    char dir[256] = "/proc/";
    strcat(dir,name);
    strcat(dir, "/status");
    char buff[1024];
    int fd = open(dir,O_RDONLY);
    read(fd,buff,1024);
    char* token = strtok(buff,"\n");
    for(int i = 0; i < 20; i++) strtok(NULL,"\n");
    token = strtok(NULL,"\n");
    strcpy(size, token);
}

void getTotalRAM(char* out) {
    char dir[256] = "/proc/meminfo";
    char buff[1024];
    int fd = open(dir,O_RDONLY);
    read(fd,buff,1024);
    char* token = strtok(buff,"\n");
    int currInd = 9;
    while(token[currInd] == 9 || token[currInd] == 32) {
        currInd++;
    }
    strcpy(out,token+currInd);
}

void getCommand(char* name, char* cmdline) {
    char dir[256] = "/proc/";
    strcat(dir,name);
    strcat(dir, "/cmdline");
    char buff[1024];
    int fd = open(dir,O_RDONLY);
    read(fd,buff,1024);
    char* token = strtok(buff,"\0");
    while(token != NULL) {
        strcat(cmdline," ");
        strcat(cmdline,token);
        token = strtok(NULL, "\0");
    }
}

int startsWith(const char *a, const char *b) {
   if(strncmp(a, b, strlen(b)) == 0) return 1;
   return 0;
}