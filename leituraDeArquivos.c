#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

typedef struct grafo Grafo;

struct grafo {
    int origem;
    int destino;
    float peso;
    Grafo *prox;
};

void inserirFila (Grafo **G, int o, int d, float p) {
    Grafo *novo = (Grafo *)malloc(sizeof(Grafo));
    Grafo *aux;

    novo->origem = o;
    novo->destino = d;
    novo->peso = p;
    novo->prox = NULL;

    if (*G == NULL)
        *G = novo;
    else {
        aux = *G;
        while (aux->prox != NULL)
            aux = aux->prox;
        aux->prox = novo;
    }
}

void imprimir (Grafo *g) {
    Grafo *aux = g;

    while (aux != NULL) {
        printf("%d %d %f\n", aux->origem, aux->destino, aux->peso);
        aux = aux->prox;
    }
}

int main( int argc, char *argv[]) {
    FILE *entrada;
    FILE *saida;
    Grafo *grafo = NULL;
    char buffer[50];
    int o, d, m, n;
    float p;

    if (argc != 3)
        printf("Esta faltando argumento! Execute novamente e e coloque o nome dos arquivos de entrada e saída\n");
    else {
        strcpy(buffer, argv[1]);
        entrada = fopen(buffer, "r");

        strcpy(buffer, argv[2]);
        saida = fopen(buffer, "w+");

        fscanf(entrada, "%d %d", &n, &m);


        while(!feof(entrada)) {
          fscanf(entrada, "%d %d %f", &o, &d, &p);
          inserirFila(&grafo, o, d, p);
          fprintf(saida, "*%d %d %f*\n", o, d, p);
        }

        imprimir(grafo);

        fclose(entrada);
        fclose(saida);
    }

    return 0;
}
