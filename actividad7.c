#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definición de la estructura del nodo del árbol de Huffman
typedef struct Nodo {
    char caracter;
    int frecuencia;
    struct Nodo *izquierda;
    struct Nodo *derecha;
} Nodo;

// Definición de la estructura para almacenar las codificaciones
typedef struct {
    char caracter;
    char *codigo;
} Codificacion;

// Función para contar las frecuencias de los caracteres en el texto
void contar_frecuencias(char *texto, int *frecuencias) {
    for (int i = 0; texto[i] != '\0'; i++) {
        frecuencias[(int)texto[i]]++;
    }
}

// Función para crear un nuevo nodo del árbol de Huffman
Nodo* crear_nodo(char caracter, int frecuencia) {
    Nodo* nuevo = (Nodo*)malloc(sizeof(Nodo));
    nuevo->caracter = caracter;
    nuevo->frecuencia = frecuencia;
    nuevo->izquierda = NULL;
    nuevo->derecha = NULL;
    return nuevo;
}

// Función para construir el árbol de Huffman a partir de las frecuencias de los caracteres
Nodo* construir_arbol_huffman(int *frecuencias) {
    Nodo* hojas[256];
    int num_hojas = 0;

    // Crear nodos hoja para cada caracter con frecuencia mayor que 0
    for (int i = 0; i < 256; i++) {
        if (frecuencias[i] > 0) {
            hojas[num_hojas++] = crear_nodo((char)i, frecuencias[i]);
        }
    }

    // Construir el árbol de Huffman combinando los nodos hoja
    while (num_hojas > 1) {
        // Encontrar los dos nodos con frecuencia más baja
        int indice_menor1 = 0;
        int indice_menor2 = 1;
        for (int i = 2; i < num_hojas; i++) {
            if (hojas[i]->frecuencia < hojas[indice_menor1]->frecuencia) {
                indice_menor2 = indice_menor1;
                indice_menor1 = i;
            } else if (hojas[i]->frecuencia < hojas[indice_menor2]->frecuencia) {
                indice_menor2 = i;
            }
        }

        // Crear un nuevo nodo interno combinando los dos nodos con menor frecuencia
        Nodo* interno = crear_nodo('\0', hojas[indice_menor1]->frecuencia + hojas[indice_menor2]->frecuencia);
        interno->izquierda = hojas[indice_menor1];
        interno->derecha = hojas[indice_menor2];

        // Eliminar los nodos hoja utilizados y agregar el nuevo nodo interno
        hojas[indice_menor1] = interno;
        hojas[indice_menor2] = hojas[num_hojas - 1];
        num_hojas--;
    }

    // El último nodo restante es la raíz del árbol de Huffman
    return hojas[0];
}

// Función recursiva para generar las codificaciones de los caracteres en el árbol de Huffman
void generar_codificaciones(Nodo* nodo, char *codigo, Codificacion *codificaciones, int *index) {
    if (nodo->izquierda == NULL && nodo->derecha == NULL) {
        codificaciones[*index].caracter = nodo->caracter;
        codificaciones[*index].codigo = strdup(codigo);
        (*index)++;
    } else {
        int len = strlen(codigo);
        char *izquierda = (char*)malloc(len + 2);
        char *derecha = (char*)malloc(len + 2);
        strcpy(izquierda, codigo);
        strcpy(derecha, codigo);
        izquierda[len] = '0';
        izquierda[len + 1] = '\0';
        derecha[len] = '1';
        derecha[len + 1] = '\0';
        generar_codificaciones(nodo->izquierda, izquierda, codificaciones, index);
        generar_codificaciones(nodo->derecha, derecha, codificaciones, index);
        free(izquierda);
        free(derecha);
    }
}

// Función principal para la codificación de Huffman
void huffman(char *texto) {
    // Contar las frecuencias de los caracteres en el texto
    int frecuencias[256] = {0};
    contar_frecuencias(texto, frecuencias);

    // Construir el árbol de Huffman
    Nodo* raiz = construir_arbol_huffman(frecuencias);

    // Generar las codificaciones de los caracteres
    Codificacion codificaciones[256];
    int index = 0;
    generar_codificaciones(raiz, "", codificaciones, &index);

    // Imprimir las codificaciones
    printf("Codificaciones:\n");
    for (int i = 0; i < index; i++) {
        printf("Caracter: %c, Codigo: %s\n", codificaciones[i].caracter, codificaciones[i].codigo);
    }

    // Liberar memoria
    for (int i = 0; i < index; i++) {
        free(codificaciones[i].codigo);
    }
}

int main() {
    char texto[100];
    printf("Introduce el texto a codificar: ");
    fgets(texto, sizeof(texto), stdin);
    texto[strcspn(texto, "\n")] = '\0'; // Eliminar el carácter de nueva línea

    huffman(texto);

    return 0;
}

