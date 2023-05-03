#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include <sys/sysinfo.h>

void Bucle(char *comando){

    long int pid, nice, priority;
    char cmdline[1024];
    char NA[255];

	FILE *fd; //FD es File descriptor
	fd = fopen(comando, "r");
	char linea[255];

	//Te regresa un error si no existe archivo de proceso
	if (fd == NULL){
		printf ("%c%c%c No hay archivo de proceso %c%c%c",16,16,16,17,17,17);
	}
	else{
                //Aquí imprime todos los procesos que encuentra y muestra el PID
		while (fgets(linea,255,fd) != NULL){
			printf ("--------------------------------------------------------------------------------\n");
			sscanf(linea, "%d %s %c %d %d %d %d %d %u %lu %lu %lu %lu %lu %lu %ld %ld %ld %ld",
            &pid, cmdline, &NA, &NA, &NA, &NA, &NA, &NA, &NA, &NA,
            &NA, &NA, &NA, &NA, &NA, &NA, &NA, &priority, &nice);

			printf("PID: %d\n", pid);
			printf("cmdline: %s\n", cmdline);
			printf("Prioridad: %d\n", priority);
		}
		fclose(fd);
	}
}

int main(){
    DIR *folder;
    struct dirent *entry; //Entry->d_name viene incluida en la estructura dirent
    char comando[295];
    folder = opendir("/proc/");
    struct sysinfo info; //Estructura donde viene información del sistema
    sysinfo(&info);
	
    while((entry = readdir(folder)) != NULL){

        if (isdigit (entry->d_name[0]) != 0){
        	strcpy (comando,"/proc/"); //Copia la cadena
        	strcat (comando,entry->d_name); //Concatena la cadena
        	strcat (comando,"/stat");
        	Bucle(comando);
        	
			sprintf(comando, "/proc/%s/status", entry->d_name);
			FILE *fdu = fopen(comando, "r");

			if(fdu != NULL ){

				char linea[256];
				int i = 0;

				while (fgets(linea, sizeof linea, fdu) != NULL){
					
					if(i == 8){
						printf("Es un proceso de: ");

						if (linea[5] == '0'){
							printf("[Kernel]");
						}
						else{
							printf("[Usuario]");
						}
						
						printf("\n");
						break;
					}
					else{
						i++;
					}
				}
				
				fclose(fdu);
			}
        }
    }
	/*
	MEMORIA
	*/
    unsigned long Size = info.totalram/1000;

	printf ("--------------------------------------------------------------------------------\n");
	printf("Se está usando un total de: %ld KB\n\n", Size);
    
    closedir(folder);

    return 0;
}
