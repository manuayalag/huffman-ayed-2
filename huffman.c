#define _CRT_SECURE_NO_WARNINGS
#include "huffman.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arbol.h"
#include "pq.h"
#include "bitstream.h"
#include "confirm.h"

/*====================================================
     Constantes
  ====================================================*/

#define NUM_CHARS 256

/*
estructura para almacenar valores de un nodo de un arbol, 
c es el caracter
frec es la frecuencia
*/
typedef struct _keyvaluepair {
    char c;
    int frec;

} keyvaluepair;

/*====================================================
     Campo de bits.. agrega funciones si quieres
     para facilitar el procesamiento de bits.
  ====================================================*/

typedef struct _campobits {
    unsigned int bits;
    int tamano;
} campobits;

/* Esto utiliza aritmetica de bits para agregar un
   bit a un campo.
   
   Supongamos que bits->bits inicialmene es (en binario):
   
      000110001000
      
   y le quiero agregar un 1 en vez del segundo 0 (desde izq).
   Entonces, creo una "mascara" de la siguiente forma:
   
      1 << 11   me da 0100000000000

   Y entonces si juntamos los dos (utilizando OR binario):      
      000110001000
    | 0100000000000
    ----------------
      010110001000

    Esta funcion utiliza bits->tamano para decidir donde colocar
    el siguiente bit.
    
    Nota: asume que bits->bits esta inicialmente inicializado a 0,
    entonces agregar un 0, no requiere mas que incrementar bits->tamano.
*/
      
static void bits_agregar(campobits* bits, int bit) {
    CONFIRM_RETURN(bits);
    CONFIRM_RETURN((unsigned int)bits->tamano < 8*sizeof(bits->bits));
    bits->tamano++;
    if (bit) {
        bits->bits = bits->bits | ( 0x1 << (bits->tamano-1));
    } 
}
/*
    funcion de utilidad para leer un bit dentro de campobits dado el indice pos
    pos = 0, primer bit 
    pos = 1, segundo bit
    pos = 2, tercer bit
    pos = k, k bit

*/
static int bits_leer(campobits* bits, int pos) {
    CONFIRM_TRUE(bits,0);
    CONFIRM_TRUE(!(pos < 0 || pos > bits->tamano),0);
    // para saber si campobits tiene un 1 o 0 en la posicion dada 
    // recorro bits usando shift << y >>
    int bit = (bits->bits & (0x1 << (pos))) >> (pos);
    return bit;
}



static void testCampobitsBitstream() {
    BitStream bs = NULL;
    BitStream bsIn = NULL;
    char* testbitstreamtxt = "testbitsteam.txt";
    int i = 0;
    // crear un campobits
    campobits* b = (campobits*)malloc(sizeof(struct _campobits));
    CONFIRM_RETURN(b);
    b->bits = 0;
    b->tamano = 0;
    // ej quiero codificar 00101 
    bits_agregar(b, 0);
    bits_agregar(b, 0);
    bits_agregar(b, 1);
    bits_agregar(b, 0);
    bits_agregar(b, 1);
    // crear un archivo y escribir bit a bit
    bs = OpenBitStream(testbitstreamtxt, "w");
    // para escribir en un archivo PutBit agrega bits 
    // ej recorro el campobits y agrego bit a bit
    for (i = 0; i < b->tamano; i++) {
        int bit = bits_leer(b, i);
        PutBit(bs, bit);
    }
    // PutByte para escribir 1 byte completo, 
    // ej agrego un caracter
    PutByte(bs, 'z');
    // no olvidar cerrar el archvio
    CloseBitStream(bs);
    // y liberar memoria utilizada
    free(b);
    // Mi archivo entonces contiene: 00101z
    // si quiero leer el mismo
    bsIn = OpenBitStream(testbitstreamtxt, "r");
    // leer bit a bit
    printf("%d", GetBit(bsIn));
    printf("%d", GetBit(bsIn));
    printf("%d", GetBit(bsIn));
    printf("%d", GetBit(bsIn));
    printf("%d", GetBit(bsIn));
    // leer un byte
    printf("%c\n", GetByte(bsIn));
    CloseBitStream(bsIn);



}
static void imprimirNodoEjemplo(Arbol nodo) {
    CONFIRM_RETURN(nodo);
    keyvaluepair* val = (keyvaluepair*)arbol_valor(nodo);
    printf("%d,%c\n", val->frec, val->c);
}
static void testArbol() {

    /* si quiero crear un arbol que contiene:
    char freq
    a    4
    s    2
    entonces tengo un nodo padre
    con freq 6 y dos hijos, con freq 2  y 4 con sus caracteres correspondientes
    */
    keyvaluepair* v1 = malloc(sizeof(struct _keyvaluepair));
    keyvaluepair* v2 = malloc(sizeof(struct _keyvaluepair));
    keyvaluepair* v3 = malloc(sizeof(struct _keyvaluepair));
    Arbol n1;
    Arbol n2;
    Arbol n3;
    CONFIRM_RETURN(v1);
    CONFIRM_RETURN(v2);
    CONFIRM_RETURN(v3);
    v1->c = 'a';
    v1->frec = 4;
    v2->c = 's';
    v2->frec = 2;
    v3->c = ' ';
    v3->frec = 6;
    n1 = arbol_crear(v1);
    n2 = arbol_crear(v2);
    n3 = arbol_crear(v3);
    arbol_agregarIzq(n3, n2);
    arbol_agregarDer(n3, n1);
    //ejemplo de recorrer el arbol e imprimir nodos con valor keyvaluepair
    arbol_imprimir(n3, imprimirNodoEjemplo);
    arbol_destruir(n3);
    free(v1);
    free(v2);
    free(v3);
}

void campobitsDemo() {
    printf("***************CAMPOBITS DEMO*******************\n");
    testCampobitsBitstream();
    printf("***************ARBOL DEMO******************\n");
    testArbol();
    printf("***************FIN DEMO*****************\n");
}
/*====================================================
     Declaraciones de funciones 
  ====================================================*/

/* Puedes cambiar esto si quieres.. pero entiende bien lo que haces */
static int calcular_frecuencias(int* frecuencias, char* entrada);
static Arbol crear_huffman(int* frecuencias);
static int codificar(Arbol T, char* entrada, char* salida);
static void crear_tabla(campobits* tabla, Arbol T, campobits *bits);

static Arbol leer_arbol(BitStream bs);
static void decodificar(BitStream in, BitStream out, Arbol arbol);

static void imprimirNodo(Arbol nodo);

/*====================================================
     Implementacion de funciones publicas
  ====================================================*/

/*
  Comprime archivo entrada y lo escriba a archivo salida.
  
  Retorna 0 si no hay errores.
*/
int comprimir(char* entrada, char* salida) {
    
    /* 256 es el numero de caracteres ASCII.
       Asi podemos utilizar un unsigned char como indice.
     */
    int frecuencias[NUM_CHARS]; 
    Arbol arbol = NULL;
    /* Primer recorrido - calcular frecuencias */
    CONFIRM_TRUE(0 == calcular_frecuencias(frecuencias, entrada), 0);
    printf("Frecuencias calculadas\n");
    for(int i=0;i<256;i++){
		if(frecuencias[i]>0){
			printf("Caracter: %c, Frecuencia: %d\n",i,frecuencias[i]);
		}
	}
    arbol = crear_huffman(frecuencias);
    arbol_imprimir(arbol, imprimirNodo); 

    /* Segundo recorrido - Codificar archivo */
    CONFIRM_TRUE(0 == codificar(arbol, entrada, salida), 0);
    
    arbol_destruir(arbol);
    
    return 0;
}


/*
  Descomprime archivo entrada y lo escriba a archivo salida.
  
  Retorna 0 si no hay errores.
*/
int descomprimir(char* entrada, char* salida) {

    BitStream in = 0;
    BitStream out = 0;
    Arbol arbol = NULL;
        
    /* Abrir archivo de entrada */
    in = OpenBitStream(entrada, "r");
    
    /* Leer Arbol de Huffman */
    arbol = leer_arbol(in);
    arbol_imprimir(arbol, imprimirNodo);

    /* Abrir archivo de salida */
    out = OpenBitStream(salida, "w");
    
    /* Decodificar archivo */
    decodificar(in, out, arbol);
    
    CloseBitStream(in);
    CloseBitStream(out);
    return 0;
}

/*====================================================
     Funciones privadas
  ====================================================*/

/* Devuelve 0 si no hay errores */
static int calcular_frecuencias(int* frecuencias, char* entrada) {


    /* Este metodo recorre el archivo contando la frecuencia
       que ocurre cada caracter y guardando el resultado en
       el arreglo frecuencias
    */
    FILE* file = fopen(entrada, "r");
    if (!file) {
        perror("Error al abrir el archivo");
        return -1;
    }

    // Inicializar todas las frecuencias a cero
    memset(frecuencias, 0, NUM_CHARS * sizeof(int));

    int c;
    while ((c = fgetc(file)) != EOF) {
        frecuencias[(unsigned char)c]++;
    }

    fclose(file);
    return 0;
    /* TU IMPLEMENTACION AQUI */
    
    /* Nota: comienza inicializando todas las frecuencias a cero!! */

}



/* Crea el arbol huffman en base a las frecuencias dadas */
static Arbol crear_huffman(int* frecuencias) {

    /* TU IMPLEMENTACION AQUI */
    
    PQ pq = pq_create();

    // Insertar caracteres con frecuencia > 0 en la cola de prioridad
    for (int i = 0; i < NUM_CHARS; i++) {
        if (frecuencias[i] > 0) {
            keyvaluepair* kvp = malloc(sizeof(keyvaluepair));
            kvp->c = (char)i;
            kvp->frec = frecuencias[i];
            printf("Insertando nodo: '%c' con frecuencia: %d\n", kvp->c, kvp->frec);
            Arbol nodo = arbol_crear(kvp);
            pq_add(pq, nodo, kvp->frec);
        }
    }
    printf("Tamaño inicial de la cola: %d\n", pq_size(pq));
    // Construir el árbol de Huffman
    while (pq_size(pq) > 1) {
        Arbol izq, der;
        pq_remove(pq, (void**)&izq);
        pq_remove(pq, (void**)&der);
        keyvaluepair* kvpIzq = (keyvaluepair*)arbol_valor(izq);
        keyvaluepair* kvpDer = (keyvaluepair*)arbol_valor(der);

        keyvaluepair* kvpPadre = malloc(sizeof(keyvaluepair));
        kvpPadre->c = '\0'; // Nodo interno
        kvpPadre->frec = kvpIzq->frec + kvpDer->frec;
        printf("Extrayendo nodos con frecuencia: %c %d y %c %d\n", kvpIzq->c, (kvpIzq)->frec, (kvpDer)->c, (kvpDer)->frec);
        Arbol nuevoNodo = arbol_crear(kvpPadre);
        arbol_agregarIzq(nuevoNodo, izq);
        arbol_agregarDer(nuevoNodo, der);

        pq_add(pq, nuevoNodo, kvpPadre->frec);
    }

    Arbol arbolHuffman;
    pq_remove(pq, (void**)&arbolHuffman);
    
    /* agregar cada nodo a cola... */
    
    /* mientras haya mas de un arbol en la cola, 
       sacar dos arboles y juntarlos (segun el algoritmo de huffman)
       reinsertar el nuevo arbol combinado
    */

    /* limpieza */
    
    pq_destroy(pq);
	return arbolHuffman;
}

static void escribir_arbol(BitStream bs, Arbol T) {
    if (arbol_izq(T) == NULL && arbol_der(T) == NULL) {
        keyvaluepair* kv = (keyvaluepair*)arbol_valor(T);
        PutBit(bs, 1); // hoja
        PutByte(bs, kv->c); // carácter
    }
    else {
        PutBit(bs, 0); // nodo interno
        escribir_arbol(bs, arbol_izq(T));
        escribir_arbol(bs, arbol_der(T));
    }
}


static int codificar(Arbol T, char* entrada, char* salida) {

    FILE* in = fopen(entrada, "r");
    if (in == NULL) { printf("error al abrir el archivo\n"); return -1; }

    BitStream out = OpenBitStream(salida, "w");
    if (out == NULL) { printf("error en bitstream\n"); fclose(in); return -1; }

    /* Dado el arbol crear una tabla que contiene la
       secuencia de bits para cada caracter.
       
       Los bits se guardan en compobits que es un struct
       que contiene un int (sin signo) y un tamano.
       La idea es que voy agregando bits al campo de bits
       mientras en un campo (bits), y el numero de bits
       que necesito en otro (tamano)
    */
    campobits tabla[NUM_CHARS];

    /* Inicializar tabla de campo de bits a cero */
    memset(tabla, 0, NUM_CHARS*sizeof(struct _campobits));
    
    /* Abrir archivos */
    /* TU IMPLEMENTACION VA AQUI .. */

    
    /* Escribir arbol al archivo de salida */

    /* TU IMPLEMENTACION VA AQUI .. 


        Nota: puedes utilizar arbol_preorden() para lograr esto
        facilmente.
        
        El truco es que al escribir el arbol, 
           - Si no es hoja: 
               escribe un bit 0 
           - Si es hoja:
                bit 1 seguido por el byte ASCII que representa el caracter 
        
        Para escribir bits utiliza PutBits() de bitstream.h
        Para escribir bytes utiliza PutByte() de bitstream.h
    */
    campobits bitsActuales = { 0, 0 };
    crear_tabla(tabla, T, &bitsActuales);

    // Escribir árbol de Huffman
    escribir_arbol(out, T);
    /* Escribir el texto codificado al archivo*/

    /* 
        TU IMPLEMENTACION VA AQUI .. 
        
        
        Lee todos los datos de nuevo del archivo de entrada
        y agregalos al archivo de salida utilizando la
        tabla de conversion.
        
        Recuerda que tienes que escribir bit por bit utilzando
        PutBit() de bitstream.h. Por ejemplo, dado una secuencia
        de bits podrias escribirlo al archivo asi:
        
            /- Esto escribe todos los bits en un campobits a un
               BitStream  -/
            for (i = 0; i < tabla[c].tamano; i++) {
               int bit =  0 != (tabla[c].bits & (0x1<<i));
               PutBit(out, bit);
            }
        
        Puedes colocarlo en una funcion si quieres

    */
    
    // Codificar archivo
    int c;
    while ((c = fgetc(in)) != EOF) {
        campobits b = tabla[(unsigned char)c];
        for (int i = 0; i < b.tamano; i++) {
            int bit = bits_leer(&b, i);
            PutBit(out, bit);
        }
    }

    /* No te olvides de limpiar */
    if (in)
        fclose(in);
    if (out)
        CloseBitStream(out);

    return 0;
}

static void crear_tabla(campobits* tabla, Arbol T, campobits* bitsActuales) {
    if (arbol_izq(T) == NULL && arbol_der(T) == NULL) {
        keyvaluepair* kv = (keyvaluepair*)arbol_valor(T);
        tabla[(unsigned char)kv->c] = *bitsActuales;
        return;
    }

    // Ir a la izquierda: agregar 0
    if (arbol_izq(T)) {
        campobits copia = *bitsActuales;
        bits_agregar(&copia, 0);
        printf("%d", bits_leer(&copia, copia.tamano - 1)); // Debug: imprimir el bit que se va a agregar
        crear_tabla(tabla, arbol_izq(T), &copia);
    }

    // Ir a la derecha: agregar 1
    if (arbol_der(T)) {
        campobits copia = *bitsActuales;
        bits_agregar(&copia, 1);
        printf("%d", bits_leer(&copia, copia.tamano - 1)); // Debug: imprimir el bit que se va a agregar
        crear_tabla(tabla, arbol_der(T), &copia);
    }
}


/* Esto se utiliza como parte de la descompresion (ver descomprimir())..
   
   Para leer algo que esta guardado en preorden, hay que
   pensarlo un poquito.
   
   Pero basicamente la idea es que vamos a leer el archivo
   en secuencia. Inicialmente, el archivo probablemente va 
   a empezar con el bit 0 representando la raiz del arbol. 
   Luego, tenemos que leer recursivamente (utiliza otra funcion
   para ayudarte si lo necesitas) un nodo izquierdo y uno derecho.
   Si uno (o ambos) son hojas entonces tenemos que leer tambien su
   codigo ASCII. Hacemos esto hasta que todos los nodos tienen sus 
   hijos. (Si esta bien escrito el arbol el algoritmo terminara
   porque no hay mas nodos sin hijos)
*/
static Arbol leer_arbol(BitStream bs) {
  
    /* TU IMPLEMENTACION AQUI */
    int bit = GetBit(bs); // lee el bit actual

    if (bit == 1) {
        // Es hoja, entonces leer el carácter asociado
        unsigned char caracter = GetByte(bs);
        keyvaluepair* kv = malloc(sizeof(keyvaluepair));
        if (!kv) {
            perror("Error al asignar memoria para keyvaluepair");
            exit(1);
        }
        kv->c = caracter;
        kv->frec = 0;  // La frecuencia no es necesaria para la descompresión
        return arbol_crear(kv); // crear nodo hoja
    }
    else {
        // Es nodo interno, crear nodo y leer recursivamente
        Arbol izq = leer_arbol(bs);
        Arbol der = leer_arbol(bs);

        keyvaluepair* kv = malloc(sizeof(keyvaluepair));
        if (!kv) {
            perror("Error al asignar memoria para keyvaluepair");
            exit(1);
        }
        kv->c = '\0'; // Nodo interno no tiene carácter
        kv->frec = 0;

        Arbol nodo = arbol_crear(kv); // los nodos internos no tienen valor
        arbol_agregarIzq(nodo, izq);
        arbol_agregarDer(nodo, der);
        return nodo;
    }
}

/* Esto se utiliza como parte de la descompresion (ver descomprimir())..
   
   Ahora lee todos los bits que quedan en in, y escribelos como bytes
   en out. Utiliza los bits para navegar por el arbol de huffman, y
   cuando llegues a una hoja escribe el codigo ASCII al out con PutByte()
   y vuelve a comenzar a procesar bits desde la raiz.
   
   Sigue con este proceso hasta que no hay mas bits en in.
*/   
static void decodificar(BitStream in, BitStream out, Arbol arbol) {
  
    /* TU IMPLEMENTACION AQUI */
    Arbol actual = arbol;
    int bit;

    while (!IsEmptyBitStream(in))
    {
        bit = GetBit(in);
        if(bit == 0) {
			// Ir a la izquierda
			actual = arbol_izq(actual);
		}
		else {                      
			// Ir a la derecha
			actual = arbol_der(actual);
		}

        //Cuando llegamos a la hoja, escribir byte
        if(arbol_izq(actual) == NULL && arbol_der(actual) == NULL) {
			keyvaluepair* kvp = (keyvaluepair*)arbol_valor(actual);
			PutByte(out, kvp->c);
			actual = arbol;                     // volver a la raiz
		}
    }
}


/* Esto es para imprimir nodos..
   Tal vez tengas mas de uno de estas funciones debendiendo
   de como decidiste representar los valores del arbol durante
   la compresion y descompresion.
*/
static void imprimirNodo(Arbol nodo) {
    /* TU IMPLEMENTACION AQUI */
    if (!nodo) return;
    keyvaluepair* kvp = (keyvaluepair*)arbol_valor(nodo);
    if (kvp->c == '\0') {
        printf("(*, %d)", kvp->frec);
    }
    else if (kvp->c == '\n') {
        printf("('\\n', %d)", kvp->frec);  // Imprimir salto de línea como texto
    }
    else {
        printf("('%c', %d)", kvp->c, kvp->frec);
    }
}
