#include <stdio.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 200

int ReadFile(char *);
void show_error(char *);

int main(int argc, char **argv) {
  if (argc <= 1) {
    printf("Uso %s <archivo a abrir>\n", (char *)argv[0]);
    exit(1);
  }

  char *file = (char *) argv[1];
  if(ReadFile(file) != 0)
    show_error("Error abriendo archivo!!");
}

int ReadFile(char *my_file) {
  FILE *textfile;
  char line[MAX_LINE_LENGTH];

  textfile = fopen(my_file, "r");
  if(textfile == NULL)
    return 1;

  while(fgets(line, MAX_LINE_LENGTH, textfile))
        printf(line);

  fclose(textfile);
  return 0;
}

void show_error(char *msg) {
  printf("ERROR: %s", msg);
}
