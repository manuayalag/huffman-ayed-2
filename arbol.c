/** Nota: mi cabecera debe ir antes que nada */
#include "arbol.h"

#include <stdlib.h>
#include <stdio.h>
#include "confirm.h"

#pragma warning(disable : 6011)

/* Implementacion de cortesía ;P */

/* Definicion interna para esta impelementacion */
typedef struct _NodoArbol {
  void* valor;
  struct _NodoArbol* izq;
  struct _NodoArbol* der;
} * _Arbol;

Arbol arbol_crear(void* valor) {
    _Arbol nuevo = (_Arbol) malloc(sizeof(struct _NodoArbol));
    nuevo->valor = valor;
    nuevo->izq = NULL;
    nuevo->der = NULL;
    return (Arbol) nuevo;
}

/* Static en C (en este contexto) significa que solamente existe dentro de este archivo,
   es decir, es privado
 */
static void _destruir(Arbol raiz, void* ignorado) {
    free(raiz);
}

void arbol_destruir(Arbol raiz) {
    arbol_postorden(raiz, _destruir, 0);
}

void arbol_agregar(Arbol raiz, void* valor, int (*comparador)(void*,void*)) {
    _Arbol T = (_Arbol) raiz;

    CONFIRM_RETURN(T);

    if (comparador(valor, T->valor) < 0) {
        if (T->izq) {
                arbol_agregar(T->izq, valor, comparador);
        } else {
                T->izq = arbol_crear(valor);
                CONFIRM_RETURN(T->izq);
        }
    } else {
        if (T->der) {
                arbol_agregar(T->der, valor, comparador);
        } else {
                T->der = arbol_crear(valor);
                CONFIRM_RETURN(T->der);
        }
    }
}
   
   
/* Nota: para llamar la funcion visitar solamente tienes que poner:

    visitar(nodo);
    
   Donde nodo es el nodo que quieres visitar.. Facil??
   
*/
void arbol_postorden(Arbol raiz, void (*visitar) (Arbol, void*), void* dato) {

    _Arbol T = (_Arbol) raiz;
    if (T) {
        arbol_postorden(T->izq, visitar, dato);
        arbol_postorden(T->der, visitar, dato);
        visitar(T, dato);
    }

}

void arbol_preorden(Arbol raiz, void (*visitar) (Arbol, void*), void* dato) {

    _Arbol T = (_Arbol) raiz;
    if (T) {
        visitar(T, dato);
        arbol_preorden(T->izq, visitar, dato);
        arbol_preorden(T->der, visitar, dato);
    }

}

void arbol_enorden(Arbol raiz, void (*visitar) (Arbol, void*), void* dato) {

    _Arbol T = (_Arbol) raiz;
    if (T) {
        arbol_enorden(T->izq, visitar, dato);
        visitar(T, dato);
        arbol_enorden(T->der, visitar, dato);
    }

}


static void _imprimir(Arbol raiz, int profundidad, void (*imprimir)(Arbol)) {

    _Arbol T = (_Arbol) raiz;
    if (T) {
        
        /* Imprimir el nodo */
        int i = 0;
        for (i = 0; i < profundidad; i++) {
           printf("\t");
        }
        imprimir(T);
        printf("\n");
        
        _imprimir(T->izq, profundidad+1, imprimir);
        _imprimir(T->der, profundidad+1, imprimir);
    }

}

void arbol_imprimir(Arbol T, void (*imprimir)(Arbol)) {

    /* Delega su trabajo sucio a _imprimir() */
    _imprimir(T, 0, imprimir);

}



void* arbol_valor(Arbol T) {
    CONFIRM_NOTNULL(T, NULL);
    return ((_Arbol)T)->valor;
}

Arbol arbol_izq(Arbol T) {
    CONFIRM_NOTNULL(T, NULL);
    return ((_Arbol)T)->izq;
}

Arbol arbol_der(Arbol T) {
    CONFIRM_NOTNULL(T, NULL);
    return ((_Arbol)T)->der;
}

void arbol_agregarIzq(Arbol nodo, Arbol izq) {
    _Arbol T = (_Arbol) nodo;
    CONFIRM_RETURN(T);
    T->izq = izq;
}


void arbol_agregarDer(Arbol nodo, Arbol der) {
    _Arbol T = (_Arbol) nodo;
    CONFIRM_RETURN(T);
    T->der= der;
}

