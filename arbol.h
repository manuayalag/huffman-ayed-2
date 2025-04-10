/* Estas lineas hacen que este archivo se incluya solamente una vez por modulo
   al compilar (esto evita que archivo A incluya a archivo B que incluye a 
   archivo A que incluye archivo B.... ) */
#ifndef DEFINE_ARBOL_H
#define DEFINE_ARBOL_H

/* Tipo opaco Arbol - no se revela su tipo verdadero en la interfaz */
typedef void* Arbol;

/* Este metodo es para crear un Arbol.
   Recibe un valor para el primer nodo del arbol.
   
   Nota: este metodo tambien puede ser utilizado para crear nuevos nodos,
   ya que un nodo no es mas que un Arbol de un solo elemento.
   
   Retorna NULL si falla.
*/
Arbol arbol_crear(void* valor);

/* Este metodo es para destruir un Arbol (y todos sus sub-arboles).

   Despues de llamar esto no utilice el arbol.
*/
void arbol_destruir(Arbol T);

/* Agrega un elemento al arbol.

  Tiene que especificar un comparador. El comparador permite al 
  algoritmo de agregado comparar el nuevo valor con los valores
  de los nodos para determinar su posicion en el arbol.
  
  Si fueramos a agregar ints al arbol su comparador podria ser:
  
     int comparadorInt(void* valor1, void* valor2) {
          int a = (int) valor1;
          int b = (int) valor2;
          if (a < b) return -1;
          else if (a > b) return 1;
          else return 0;
     }   

   O su version simplificada (pero equivalente):

     int comparadorInt(void* valor1, void* valor2) {
          return (int) (valor1 - valor2);
     }

   Para llamarlo hariamos lo siguiente:
   
      Arbol T = arbol_crear((void*) 4);
      arbol_agregar(T, (void*) 2, comparadorInt);
      arbol_agregar(T, (void*) 6, comparadorInt);
      
*/
void arbol_agregar(Arbol T, void* valor, int (*comparador)(void*,void*));


/* Agrega un subarbol izquierdo al nodo dado.
   (Nota: esto no destruye el subarbol que estaba alli)
*/
void arbol_agregarIzq(Arbol nodo, Arbol izq);


/* Agrega un subarbol derecho al nodo dado.
   (Nota: esto no destruye el subarbol que estaba alli)
*/
void arbol_agregarDer(Arbol nodo, Arbol der);
   
   
/* Este metodo es para recorrer un arbol en post-orden.

   Especifique el metodo de visita.
   
   Un metodo de visita ejemplar (para valores que son cadenas) es:
   
      void imprimir(Arbol T, void* ignorado) {
            char* valor = arbol_valor(T);
            if (NULL == valor)
                  printf("NULL");
            else 
                  printf("%s", valor);
      }
      
      
   Despues si tengo un arbol T puedo imprimirlo todo asi:
   
      arbol_postorden(T, imprimir, 0);
      
   El ultimo campo es pasado como segundo parametro a la funcion de 
   visita por si este lo necesita. Un ejemplo podria ser si convierto
   el arbol en una lista podria hacerlo asi:
   
       arbol_postorden(T, convertir, lista);
       
   Donde convertir es la funcion que hace la conversion y lista la lista
   que recibe los datos. 
   
      
*/
void arbol_postorden(Arbol T, void (*visitar) (Arbol, void*), void* dato);

/* Preorden */
void arbol_preorden(Arbol T, void (*visitar) (Arbol, void*), void* dato);

/* Enorden */
void arbol_enorden(Arbol T, void (*visitar) (Arbol, void*), void* dato);

/* Obtiene el valor en la raiz del arbol especificado */
void* arbol_valor(Arbol T);

/* Obtiene el sub-arbol izquierdo */
Arbol arbol_izq(Arbol T);

/* Obtiene el sub-arbol derecho */
Arbol arbol_der(Arbol T);

/* Esta funcion imprime el arbol de manera linda para 
   depuracion. 
   
   Recibe un metodo imprimir() para indicar como 
   debe imprimir los nodos. Un ejemplo puede ser:
   
      static int mi_imprimir(Arbol nodo) {
            Datos d = (Datos) arbol_valor(nodo);
            printf("%d", d->valor);
      }
   Como lo implementas depende de los tipos de 
   datos que contenga el arbol.
*/
void arbol_imprimir(Arbol T, void (*imprimir)(Arbol));


#endif /* DEFINE_ARBOL_H */
