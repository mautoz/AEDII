/* Aluno: Mauricio Tavares Ozaki	    */
/* NUSP: 7577427					    */
/* ACH2024 - AEDII - 2018			    */
/* Prof.: Helton					    */
/* EP1 - Algoritmo de Prim              */
/* Data de Entrega: 30/04/2018          */
/* Obs.: o EP foi feito on UBUNTU 16.04 */
/* e compilado com o GCC 5.4.0.         */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define true 1
#define false 0
typedef int bool;
//Variáveis globais de nmero de vértices e arestas
int m, n;
//Estrutura do Grafo (Lista de Adjacência), que recebe os vértices de origem, destinod e seu peso.
typedef struct grafo Grafo;
struct grafo {
    int origem;
    int destino;
    double peso;
    Grafo *prox;
};
//Recebe um Grafo G e insere na Lista de Adjacências, ou seja, insere na fila.
void inserirFila (Grafo **G, int o, int d, double p) {
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
//Verifica se o "vetor" de custos já tem todos os vértices na árvore. Se todos forem conhecidos (== 1), então
//devolve o true, caso contrário, devolve false.
bool verificar (int *v) {
    int i = 0;

    while (i < n)
        if (v[i++] == 0) return 0;
    return 1;
}
//Calcula a soma do vetor de custos
double soma (double *v) {
    double resultado = 0;
    int i;

    for (i = 0; i < n; i++)
        resultado+=v[i];

    return resultado;
}
//Função que executa o Algoritmo do PRIM
void prim (Grafo **G, FILE **s) {
    Grafo *gaux = NULL;
    double custo[n];
    int conhecido[n], ant[n]; //Sempre que o Vértice for escolhido pelo preço mínimo, atribui-se o valor de 1
    int menor, i = 0;
    //Inicio, atribuir custo infinito e ant[] = -1
    for (i = 0; i < n; i++) {
        custo[i] = INFINITY;
        conhecido[i] = 0;
        ant[i] = -1;
    }
    custo[0] = 0; //custo[v0] = 0

    while (!verificar(conhecido)) { //Enquanto S != V
        i = 0;
        menor = 0;
        //Achar o primeiro vértice que ainda nao esta em S
        while (i < n) {
            if (conhecido[i] == 0) {
                menor = i;
                break;
            }
            i++;
        }
        //Procurar u em V - S tal que custo[u] é mínimo
        i = 0;
        while (i < n) {
            if (conhecido[i] == 0) //conhecido == 0 pois não pertence a S
                if (custo[i] < custo[menor])
                    menor = i;
            i++;
        }
        conhecido[menor] = 1; //Adicionando u em S

        i = 0;
        gaux = *G;
        while (gaux != NULL) {
            if (gaux->origem == menor) {
                if (custo[gaux->destino] > gaux->peso && conhecido[gaux->destino] == 0) {
                    custo[gaux->destino] = gaux->peso;
                    ant[gaux->destino] = gaux->origem;
                }
            }
            else if (gaux->destino == menor) {
                if (custo[gaux->origem] > gaux->peso && conhecido[gaux->origem] == 0) {
                    custo[gaux->origem] = gaux->peso;
                    ant[gaux->origem] = gaux->destino;
                }
            }
            gaux = gaux->prox;
        }
    }
    //Imprimir na saída
    fprintf(*s, "%lf\n", soma(custo));
    for (i = 1; i < n; i++){
         fprintf(*s, "%d %d\n", ant[i], i);
    }
}

int main( int argc, char *argv[]) {
    FILE *entrada;
    FILE *saida;
    Grafo *grafo = NULL;

    char buffer[50];
    int o, d;
    double p;

    if (argc != 3)
        printf("Esta faltando argumento! Execute novamente e e coloque o nome dos arquivos de entrada e saída\n");
    else {
        //Leitura dos nomes dos arquivos de Entrada e Saída
        strcpy(buffer, argv[1]);
        entrada = fopen(buffer, "r");
        strcpy(buffer, argv[2]);
        saida = fopen(buffer, "w+");
        //Leitura da primeira linha da Entrada, em que, n = num de vértoce e m = num de arestas
        fscanf(entrada, "%d %d", &n, &m);
        //Enquanto não chega ao fim do arquivo, lê o vértice de origem, destino e peso de cada linha.
        while (!feof(entrada)) {
            fscanf(entrada, "%d %d %lf", &o, &d, &p);
            inserirFila(&grafo, o, d, p);
        }
        //Inicia o PRIM
        prim(&grafo, &saida);

        fclose(entrada);
        fclose(saida);
    }

    return 0;
}
