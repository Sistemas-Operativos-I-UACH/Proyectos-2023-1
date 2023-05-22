#include <unistd.h>
#include <stdio.h>
#include <ctype.h>
#include <dirent.h> // Entradas del Directorio - Facilita el recorrido de directorios
#include <string.h>
#include <sys/sysinfo.h> // Devuelve Estadisticas Globales del Sistema (uso de memoria y sistema de carga)

void printmemsize(char *str, unsigned long ramsize) { 
        printf("%s: %ld KB\n",str, ramsize/1024);          //Mostramos memoria
}  

int i = 0;

void ciclo(char *arg){

	char state, name[32];
    int pid, ppid, pgrp, session, tty, tpgid, nlwp;
    unsigned long flags, min_flt, cmin_flt, maj_flt, cmaj_flt, vsize; //vsize.Tamaño virtual global
    unsigned long long utime, stime, cutime, cstime, start_time;
    long priority, nice, alarm, rss;
	FILE *fd; //Se usa fd para hacer las busquedas
	fd = fopen(arg, "r");
	char linea[255];

	if (fd==NULL){
		printf ("ERROR, ESTE PROCESO NO EXISTE"); //EN caso de que el procesa que se busca no exista se muestra este mensaje de error
		}else{
		while (fgets(linea,255,fd)!=NULL){
			printf ("=====================\n");
			printf ("       %d            \n", i);
            printf ("=====================\n");
			sscanf(linea, "%d %s " "%c " "%d %d %d %d %d " "%lu %lu %lu %lu %lu " "%Lu %Lu %Lu %Lu "
			"%ld %ld " "%d " "%ld " "%Lu " "%lu " "%ld", &pid, name, &state, &ppid, &pgrp, &session, &tty, &tpgid,
			&flags, &min_flt, &cmin_flt, &maj_flt, &cmaj_flt, &utime, &stime, &cutime, &cstime, &priority, &nice,
			&nlwp, &alarm, &start_time, &vsize, &rss);
			printf ("ID: %d\n" "nombre: %s\n"  "Prioridad: %ld\n" "Tamaño: %lu\n" "RAM : %ld KiB\n", //Aqui se imprimen los procesos
			pid, name, nice, vsize, rss);
			i++;
		}
		fclose(fd);
		}
		}
		// pid. ID del proceso. Numero unico asignado
		// ppid. Parent Process. Crea el proceso actual
		//pgrp. Grupo de procesos. Controla que procesos reciben señales del control de trabajos
		//tty imprime el nombre del archivo con el que se interactua
		//session. Identificador de la sesion.
		//tpgid. Identificador del grupo de procesos del proceso terminal al que pertenece el proceso actual
		//flags. Flags del proceso actual. Forma de pasar opciones al comando que se ejecuta
		//min_flt Fallos de pagina menores. no han necesitado cargar un pagina de memoria desde disco
		//cmin_flt Fallos de pagina menores que han hecho los procesos hijos del proceso actual
		//maj_flt Fallos de pagina mayores que han necesitado cargar una pagina de memoria desde disco
		//utime Tiempo que este proceso ha estado en ejecucion en modo usuario. Se mide en ticks de reloj
		//stime Tiempo que este proceso ha estado en ejecucion modo kernel
		//cutime Tiempo que los hijos de este proceso han estado en ejecucuion modo usuario
		//cstime Tiempo que los hijos de este proceso ha estado en ejecucion  modo kernel
		//priority Proridad del procesos. Depende del planificador de procesos activo
		//nice Simpatia del proceso. Este valor va de 19 (prioridad baja por que el sistema intenta no consumir mucho)
		// a -20 (prioridad alta por que el sistema intenta acaparar el cpu)

int main(){
    DIR *folder;	//Genera informacion sobre los archivos y directorios
    struct dirent *entry;
    char arg[295];
    folder = opendir("/proc/");   //Cada vez que se ejecuta un proceso, el sistema operativo nos deja ver información 
	//sobre él a través de un directorio (virtual, no está escrito en disco ni nada) llamado /proc/[PID] donde PID es el identificador del proceso o Process ID     
    struct sysinfo info;
    sysinfo(&info);
	
    while((entry=readdir(folder))!=NULL){
        if (isdigit (entry->d_name[0]) != 0){									//Esto sirve para identificar el tipo											
        	strcpy (arg,"/proc/");												//de proceso que usan los procesos, si es del Kernel o del Usuario
        	strcat (arg,entry->d_name);
        	strcat (arg,"/stat");
        	ciclo(arg);
        	
		sprintf(arg, "/proc/%s/status", entry->d_name);
		FILE *fdu = fopen(arg, "r");

		if ( fdu != NULL )
		{
			char linea[256];
			int contador = 0;
			while (fgets(linea, sizeof linea, fdu) != NULL)
			{
				if (contador == 8 && linea[5] == '0')
				{
					printf("Tipo de proceso: Kernel\n", linea);
					break;
				}else if(contador== 8){
					printf("Tipo de proceso: Usuario\n", linea);
					break;
				}
				else
				{
					contador++;
				}
			}
			fclose(fdu);
		}
        }
    }
	printf("\n\n");
    printf ("=============================\n");
    printmemsize("MEMORIA TOTAL DEL SISTEMA:", info.totalram); //Imprimir la memoria total del sistema
    printf ("=============================\n");
    closedir(folder);
    return 0;
}
