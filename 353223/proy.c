#include <stdio.h>
#include <stdlib.h>
#include <dirent.h> //biblioteca necesario para trabajar con el directorio /proc

int main() {
    int total_memory = 0; //memoria total de todos los procesos
    int user_processes = 0, kernel_processes = 0; //contador de procesos de ususario y de kernel
    DIR *procDir; //apuntador a la estructura DIR para representar procDir como un directorio
    struct dirent *fileInDirectory; //lista de archivos contenidos en el directorio  
    char path[100]; //matriz de caracteres para indicar la ruta del archivo (para cmdline y statm en este caso)
    char pName[256]; //matriz de caracteres para almacenar los nombres de los procesos

    printf("PID | Nombre | Memoria | Niceness\n");
    printf("----------------------------------------------\n");

    procDir = opendir("/proc"); //se abre el directorio /proc 
    while ((fileInDirectory = readdir(procDir))) {  //se lee cada archivo en el directorio /proc
        int pid = atoi(fileInDirectory->d_name); //se convierte el nombre del archivo de cadena a entero (PID) atoi() --> ascii to integer
        if (pid != 0) {
            sprintf(path, "/proc/%d/cmdline", pid); //se crea la ruta del archivo cmdline
            FILE *cmdline = fopen(path, "r"); //se abre ese archivo en solo lectura "r"
            if (fgets(pName, sizeof(pName), cmdline)) { //lee la primera linea del archivo cmdline y la almacena en pName
                char name[100]; 
                int nice;
                sprintf(path, "/proc/%d/stat", pid);//se crea la ruta del arhivo stat
                FILE *stat = fopen(path, "r"); 
                //se crea la cadena con el contenido del archivo stat de ese archivo y obtiene el nombre y la prioridad del proceso
                fscanf(stat, "%*d (%99[^)]) %*c %*d %*d %*d %*d %*d %*u %*lu %*lu %*lu %*lu %*lu %*lu %*ld %*ld %*d %d", name, &nice);               
                fclose(stat); 
                int memory = 0;
                sprintf(path, "/proc/%d/statm", pid); //se crea la ruta a statm donde se obtiene el valoor de la memoria que consume
                FILE *statm = fopen(path, "r"); 
                fscanf(statm, "%d", &memory);
                fclose(statm);
                total_memory += memory; 
                if (nice >= 0) {  //se verifica si es proceso de usuario o de kernel 
                    user_processes++;
                    printf("-----Proceso de usuario-----\n");
                } else {
                    kernel_processes++;
                    printf("-----Proceso de kernel-----\n");
                }

                printf("%d | %s | %d KB | %d\n", pid, name, memory, nice);
            }
            fclose(cmdline);
        }
    }
    closedir(procDir);    //se cierra el directorio /proc 

    printf("\nTotal de memoria utilizada en el sistema: %d KB\n", total_memory);
    printf("# de procesos de usuario: %d\n", user_processes);
    printf("# de procesos de kernel: %d\n", kernel_processes);

    return 0;
}   