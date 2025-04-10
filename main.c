
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "huffman.h"


void forma_de_uso() {
    printf("\nCodificador de Huffman:\n\n");
    printf("\tProy1.exe [comprimir|descomprimir] archivoent archivosal\n");
}


/* Este es un main() con argumentos.
    argc - numero de argumentos (incluyendo el ejecutable)
    argv - vector de argumentos
*/
int main(int argc, char* argv[]) {
    
    int errores = 0;
    // comentar campobitsDemo() es solo de ayuda para comenzar con campobits BitStream y Arbol
    campobitsDemo();
    /* Revisar que estan bien olos parametros */
    if (argc != 4) {
        forma_de_uso();
        return 1;
    }

    if (0 == strcmp("comprimir", argv[1])) {
        errores = comprimir(argv[2], argv[3]);
    } else {
        errores = descomprimir(argv[2], argv[3]);
    }
    /*
    char** str;
    printf("Presione Enter para continuar ... %c",str);
    scanf("%s",&str);
    */
    return errores;
}
