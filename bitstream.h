/* 
  Origen de este archivo:
  
  University of Washington
  CSE 326 --- Steve Burns, Spring 1994  
  
  Modificaciones:
  - Amin Mansuri, 2003
*/

#ifndef DEFINE_BITSTREAM_H
#define DEFINE_BITSTREAM_H

#include <stdio.h>

#define BITSTREAM_READ 0x1
#define BITSTREAM_WRITE 0x2

typedef void* BitStream;

/*
  Abre un archivo para leer (BITSTREAM_READ) o escribir (BITSTREAM_WRITE)
*/
BitStream OpenBitStream( char *filename, char *type_str);

/*
 Cierra el archivo 
*/
int CloseBitStream(BitStream bs);

/*
  Si esta vacio el BitStream
*/
int IsEmptyBitStream(BitStream bs);

/*
  Obtiene el bit
*/
int GetBit(BitStream bs);

/* 
  Escribe un bit
*/
void PutBit(BitStream bs, int bit);

/* 
  Obtiene un byte
*/
unsigned char GetByte(BitStream bs);

/*
  Escribe un byte
*/
void PutByte(BitStream bs, unsigned char c);

#endif
