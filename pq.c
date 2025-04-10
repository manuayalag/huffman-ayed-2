#include "pq.h"
#include <stdlib.h>


/* Crea la cola de prioridad PQ e inicializa sus atributos
retorna un puntero a la cola de prioridad 
retorna NULL si hubo error*/
PQ pq_create() {
   
	/* PQ pq = (PQ) malloc(sizeof(struct Heap)); */
	/* AGREGUE SU CODIGO AQUI */
    PQ pq = (PQ)malloc(sizeof(struct Heap));
    if (!pq) return NULL;
    pq->cap = 256;
    pq->size = 1; // Se sacrifica el primer espacio de memoria
    pq->arr = (PrioValue*)malloc(pq->cap * sizeof(PrioValue));
    if (!pq->arr) {
        free(pq);
        return NULL;
    }
    return pq;
}

/*
Agrega un valor a la cola con la prioridad dada

retorna TRUE si tuvo exito, FALSE si no
*/
BOOLEAN pq_add(PQ pq, void* valor, int prioridad) {
   
	/* AGREGUE SU CODIGO AQUI */
    if (!pq) return FALSE;
    if (pq->size == pq->cap) pq_resize(pq);
    PrioValue nuevo = (PrioValue)malloc(sizeof(struct _PrioValue));
    if (!nuevo) return FALSE;
    nuevo->prio = prioridad;
    nuevo->value = valor;
    pq->arr[pq->size] = nuevo;
    pq_percolateUp(pq, pq->size);
    pq->size++;
    return TRUE;
}

/* 
  Saca el valor de menor prioridad (cima del monticulo) y lo guarda en la posicion retVal (paso por referencia)
  retorna FALSE si tiene un error
  retorna TRUE si tuvo EXITO
*/
BOOLEAN pq_remove(PQ pq, void** retVal) {

   /* AGREGUE SU CODIGO AQUI */
    if (!pq || pq->size <= 1) return FALSE;
    *retVal = pq->arr[1]->value;
    free(pq->arr[1]);
    pq->arr[1] = pq->arr[pq->size - 1];
    pq->size--;
    pq_percolateDown(pq, 1);
    return TRUE;
}

/* retorna el tamaño de la cola de prioridad, 
   retorna 0 si hubo error 
 */
int pq_size(PQ pq) {

   	/* AGREGUE SU CODIGO AQUI */
    return pq ? pq->size - 1 : 0;
}

/* Destruye la cola de prioridad, 
retorna TRUE si tuvo exito
retorna FALSE si tuvo error*/
BOOLEAN pq_destroy(PQ pq) {
   
   /* AGREGUE SU CODIGO AQUI */
    if (!pq) return FALSE;
    for (int i = 1; i < pq->size; i++) free(pq->arr[i]);
    free(pq->arr);
    free(pq);
    return TRUE;
}

BOOLEAN pq_resize(PQ pq) {
    if (pq == NULL) {
        return FALSE;
    }
    int nuevaCap = pq->size * 2;
    PrioValue* nuevoArr = (PrioValue*)malloc((nuevaCap + 1) * sizeof(PrioValue));
    if (nuevoArr == NULL) return FALSE;
    for (int i = 1; i <= pq->size; i++) {
        nuevoArr[i] = pq->arr[i];
    }
    pq->cap = nuevoArr;
    free(pq->arr);
    pq->arr = nuevoArr;

    return TRUE;
}

/* Funcion para reacomodar el monticulo hacia arriba */
BOOLEAN pq_percolateUp(PQ pq, int pos) {
    PrioValue temp;
    if (pq == NULL) return FALSE;

    // Si pos es 1, es el padre. si pos menor que pos/2, swap

    while (pos != 1 && (pq->arr[pos]->prio < pq->arr[pos / 2]->prio)) {
        temp = pq->arr[pos / 2];
        pq->arr[pos / 2] = pq->arr[pos];
        pq->arr[pos] = temp;
        pos = pos / 2;
    }
}

/* Funcion para reacomodar el monticulo hacia abajo */
BOOLEAN pq_percolateDown(PQ pq, int pos) {
    if (pq == NULL) return FALSE;

    while (2 * pos < pq->size) {
        int menorHijo = pos * 2;
        if (menorHijo + 1 < pq->size && pq->arr[menorHijo + 1]->prio < pq->arr[menorHijo]->prio) {
            menorHijo++;
        }
        if (pq->arr[pos]->prio <= pq->arr[menorHijo]->prio) break;

        PrioValue temp = pq->arr[pos];
        pq->arr[pos] = pq->arr[menorHijo];
        pq->arr[menorHijo] = temp;
        pos = menorHijo;
    }
    return TRUE;
}