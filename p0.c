#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

void date() {
  // Obtener la fecha actual
  time_t t;
  struct tm *tm_info;

  time(&t);
  tm_info = localtime(&t);

  // Formatear la fecha en el formato DD/MM/YYYY
  char fecha[11]; // Espacio suficiente para almacenar "DD/MM/YYYY\0"
  strftime(fecha, sizeof(fecha), "%d/%m/%Y", tm_info);

  // Imprimir la fecha formateada
  printf("Fecha actual: %s\n", fecha);
}

void times() {
  // Obtener la hora actual
  time_t t;
  struct tm *tm_info;

  time(&t);
  tm_info = localtime(&t);

  // Formatear la hora en el formato hh:mm:ss
  char hora[9]; // Espacio suficiente para almacenar "hh:mm:ss\0"
  strftime(hora, sizeof(hora), "%H:%M:%S", tm_info);

  // Imprimir la hora formateada
  printf("Hora actual: %s\n", hora);
}

void procesar_comando(const char *comando, int *terminado) {
    if (strcmp(comando, "exit") == 0 ||strcmp(comando, "quit") == 0 || strcmp(comando, "bye") == 0)
      *terminado = 0;
    else if (strcmp(comando, "date") == 0)
      date();
    else if(strcmp(comando, "time") == 0)
      times();
}

int main() {
    char comando[256]; // Usamos un array de caracteres para almacenar el comando
    int terminado = 1;

    while (terminado) {
        printf("Escriba el comando: \n");

        gets(comando); // Leemos la entrada del usuario
        comando[strcspn(comando, "\n")] = '\0'; // Eliminamos el carácter de salto de línea

        procesar_comando(comando, &terminado);
    }

    return 0;
}
