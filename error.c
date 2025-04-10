/* Este archivo se encarga de definir el comportamiento para errores */
#include "confirm.h"

#include <stdio.h>
#include <stdlib.h>

extern void GlobalError(char* archivo, int linea) {
    fprintf(stderr, "\n\bError en linea %u del archivo %s\n", linea, archivo);
    system("pause");
}
