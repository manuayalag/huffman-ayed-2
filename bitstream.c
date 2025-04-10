/* 
  Origen de este archivo:
  
  University of Washington
  CSE 326 --- Steve Burns, Spring 1994  
  
  Modificaciones:
  - Amin Mansuri, 2003
  - Andi Fukuchi, 2019
*/

#include <stdlib.h>
#include <stdio.h>
#include "bitstream.h"

#define NDEPURAR

#define MALLOC(type) (type *) malloc( sizeof( type))
#pragma warning(disable : 4996)

struct _BitStream {
   int c1, c2, c3;
   int type;
   int position;
   FILE *fp;
};


BitStream OpenBitStream( char *filename, char *type_str)
{
	struct _BitStream *bs = MALLOC( struct _BitStream);
	#pragma warning(suppress:6011)
	if ( !(bs->fp=fopen( filename, type_str))) {
		perror( "OpenBitStream");
		free( (void *) bs);
		return 0;
	}
	if ( *type_str == 'w') {
		bs->type = BITSTREAM_WRITE;
		bs->c1 = 0;
	} else {
		bs->type = BITSTREAM_READ;
		bs->c1 = getc( bs->fp);
		bs->c2 = getc( bs->fp);
		bs->c3 = getc( bs->fp);
	}
	bs->position = 0;
	return bs;
}

int CloseBitStream(BitStream bitStream)
{
	int rt=0;
	struct _BitStream *bs = (struct _BitStream*) bitStream;

#ifdef DEPURAR
    rt = fclose(bs->fp);
#else 	
	if ( bs->type & BITSTREAM_WRITE) {
		if ( (bs->position & 0x7) > 0) {
			putc( bs->c1, bs->fp);
			putc( (bs->position & 0x7), bs->fp);
		} else if ( bs->position == 0)
		putc( 0, bs->fp);
		else
			putc( 8, bs->fp);
	}
	
	if ( rt=fclose( bs->fp))
		perror( "CloseBitStream");
#endif	
	return rt;
	
}

int IsEmptyBitStream(BitStream bitStream)
{
	struct _BitStream *bs = (struct _BitStream*) bitStream;
	return bs->c2 == EOF ||
	bs->c3 == EOF && (bs->position & 0x7) >= bs->c2;
}

int GetBit(BitStream bitStream)
{
	struct _BitStream *bs = (struct _BitStream*) bitStream;
	int value = (bs->c1 & (0x80 >> (bs->position & 0x7))) ? 1 : 0;
	if ( ((++bs->position) & 0x7) == 0) {
		bs->c1 = bs->c2;
		bs->c2 = bs->c3;
		bs->c3 = getc( bs->fp);
	}
	return value;
}

unsigned char GetByte(BitStream bitStream)
{
	unsigned int i;
	unsigned char c = 0;
	struct _BitStream *bs = (struct _BitStream*) bitStream;
	for (i=0; i<8; i++)
		if ( GetBit( bs))
			c |= 0x80 >> i;
	return c;
}

void PutBit(BitStream bitStream, int bit)
{
	struct _BitStream *bs = (struct _BitStream*) bitStream;
#ifdef DEPURAR
    putc(bit?'1':'0', bs->fp);
#else
	if ( bit)
		bs->c1 |= (0x80 >> (bs->position & 0x7));
	else
		bs->c1 &= ~(0x80 >> (bs->position & 0x7));
	
	if ( ((++bs->position) & 0x7) == 0) {
		putc( bs->c1, bs->fp);
		bs->c1 = 0;
	}
#endif
}

void PutByte(BitStream bitStream, unsigned char c)
{
	int i;
	struct _BitStream *bs = (struct _BitStream*) bitStream;

#ifdef DEPURAR
    putc(c, bs->fp);
#else 
	for( i=0; i<8; i++)
		PutBit( bs, c & ( 0x80 >> i));
#endif
}
