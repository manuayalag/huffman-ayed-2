#ifndef DEFINE_HUFFMAN_H
#define DEFINE_HUFFMAN_H

/*
  Comprime archivo entrada y lo escriba a archivo salida.
  
  Retorna 0 si no hay errores.
*/
int comprimir(char* entrada, char* salida);

/*
  Descomprime archivo entrada y lo escriba a archivo salida.
  
  Retorna 0 si no hay errores.
*/
int descomprimir(char* entrada, char* salida);

/*
	ejecuta el codigo de ejemplo para manipulacion de campobits y bitstream 
*/
void campobitsDemo();


#endif
