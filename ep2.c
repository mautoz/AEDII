//Implementação de Árvore B de Ordem t
//Considerar que t é o número mínimo de chaves em um nó não raíz, isto é,
//ele possui t descendentes.
// http://staff.ustc.edu.cn/~csli/graduate/algorithms/book6/chap19.htm
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define true 1
#define false 0
#define t 3
//Como não foi dita a ordem da árvore, optei pelo define, assim fica mais
//fácil realizar testes com outras ordens.
typedef int bool;

typedef struct ArvoreB Noh;
//Estrutura da árvore B, considerando a ordem t, o número mínimo de chaves em um nó
struct ArvoreB {
    bool folha;
    int nchaves;
    int chaves[2*t - 1];
    Noh *filhos[2*t];
};
//Início da parte de Inserção e criação da árvore B
//Bloco com criação, inserção, split e imprimir

//Caso a árvore ainda não exista, criamos o primeiro nó.
Noh *criarArvoreB () {
    Noh *novo = (Noh *)malloc(sizeof(Noh));
    int i = 0;

    novo->folha = true;
    novo->nchaves = 0;

    while (i < 2*t) {
        novo->filhos[i] = NULL;
        i++;
    }

    return novo;
}
//Este algoritmo foi baseado no que o Marcelo colocou na lousa, em uma das aulas de árvore B
void imprimir (Noh *arvore, FILE **s) {
    int i;
    if (arvore == NULL) return;

    for (i = 0; i < arvore->nchaves; i++) {
        imprimir(arvore->filhos[i], &(*s));
        fprintf(*s, "%d ", arvore->chaves[i]);
    }
    imprimir(arvore->filhos[arvore->nchaves], &(*s));
}
//Verifica se o elemento existe na árvore, se não existir, é possível inserir
bool buscar (Noh *x, int ch) {
    int i = 0;

    if (x == NULL) return false;

    while (i < x->nchaves && ch > x->chaves[i])
        i++;

    if (i < x->nchaves && ch == x->chaves[i])
        return true;

    if (x->folha == 1)
        return false;
    else
        return buscar(x->filhos[i], ch);
}
//Função split, usada sempre que o nó estiver cheio
void dividirFilho (Noh **x, int i, Noh **y) {
    Noh *z = (Noh *)malloc(sizeof(Noh));
    Noh *aux_x = *x;
    Noh *aux_y = *y;
    int j = 0;

    z->folha = aux_y->folha;
    z->nchaves = t - 1;

    for (j = 0; j < t - 1; j++)
        z->chaves[j] = aux_y->chaves[j + t];
    if (aux_y->folha == 0) {
        for (j = 0; j < t; j++) {
            z->filhos[j] = aux_y->filhos[j + t];
        }
    }
    aux_y->nchaves = t - 1;

    for (j = aux_x->nchaves; j >= i + 1; j--)
        aux_x->filhos[j + 1] = aux_x->filhos[j];
    aux_x->filhos[i+1] = z;


    for (j = aux_x->nchaves - 1; j >= i ; j--)
        aux_x->chaves[j+1] = aux_x->chaves[j];
    aux_x->chaves[i] = aux_y->chaves[t - 1];

    aux_x->nchaves++;
}
//Se o nó não estiver cheio, é só inserir a chave
void inserirNaoCheio (Noh **arvore, int ch) {
    Noh *aux_x = *arvore;
    Noh *y;
    int i = aux_x->nchaves;

    if (aux_x->folha) {
        while (i > 0 && ch < aux_x->chaves[i - 1]) {
            aux_x->chaves[i] = aux_x->chaves[i - 1];
            i--;
        }
        aux_x->chaves[i] = ch;
        aux_x->nchaves++;
    }
    else {
        while (i > 0 && ch < aux_x->chaves[i - 1])
            i--;
        y = aux_x->filhos[i];

        if (y->nchaves == 2*t - 1) {
            dividirFilho(&aux_x, i, &y);
            if (ch > aux_x->chaves[i])
                y = aux_x->filhos[i+1];
        }
        inserirNaoCheio(&y, ch);
    }
}
//Função principal de inserir que decide se é ou não necessário fazer o split
void inserir (Noh **raiz, int ch) {
        Noh *aux = *raiz;
        Noh *novo = (Noh *)malloc(sizeof(Noh));

        if (aux->nchaves == 2*t - 1) {
            *raiz = novo;
            novo->folha = 0;
            novo->nchaves = 0;
            novo->filhos[0] = aux;
            dividirFilho(&novo, 0, &aux);
            inserirNaoCheio(&novo, ch);
        }
        else {
            inserirNaoCheio(&aux, ch);
        }
}

//Término da inserção e criação de árvore B
//Início da remoção de árvore B
void deletar (Noh **r, int ch) {
    Noh *aux = *r;
    int i = 0, j = 0;

    //Case 1
    if (aux->folha == true) {
        while (i < aux->nchaves && ch != aux->chaves[i])
            i++;

        for (j = i; j < aux->nchaves - 1 ; j++)
            aux->chaves[j] = aux->chaves[j + 1];

        aux->nchaves--;
    }
    else {
        //Para o caso 2 e 3, precisamos verificar se a chave está ou não no nó interno, talvez eu tenha que criar essa parte aqui!
        while (i < aux->nchaves && ch != aux->chaves[i])
            i++;
        //Caso 2
        if (i < aux->nchaves) { //Este noh eh interno, então ele deve ter no minimo t-1 chaves.

            if (aux->filhos[i]->nchaves >= t) {
                aux->chaves[i] = aux->filhos[i]->chaves[aux->filhos[i]->nchaves - 1];
                deletar(&(aux->filhos[i]), aux->filhos[i]->chaves[aux->filhos[i]->nchaves - 1]);
            }
            else if (aux->filhos[i + 1]->nchaves >= t) {
                aux->chaves[i] = aux->filhos[i+1]->chaves[0];
                deletar(&(aux->filhos[i + 1]), aux->filhos[i + 1]->chaves[0]);
            }
            else { //Ambo possuem t-1 chaves.
                aux->filhos[i]->chaves[aux->filhos[i]->nchaves++] = aux->chaves[i];
                for (j = 0; j < aux->filhos[i + 1]->nchaves; j++) {
                    aux->filhos[i]->chaves[j + t] = aux->filhos[i + 1]->chaves[j];
                    aux->filhos[i]->nchaves++;
                }
                for (j = i; j < aux->nchaves - 1; j++) {
                    aux->chaves[j] = aux->chaves[j + 1];
                    aux->filhos[j + 1] = aux->filhos[j + 2];
                }
                aux->nchaves--;
                deletar(&(aux->filhos[i]), aux->filhos[i]->chaves[t - 1]);
            }
        }
        else { //Caso 3, não está no nó interno, i == aux->nchaves
            i = 0;
           while (i < aux->nchaves && ch > aux->chaves[i])
                i++;

            if (aux->filhos[i]->nchaves == t - 1) { // Caso 3a
                if (i > 0 && aux->filhos[i - 1]->nchaves >= t) {
                    for (j = aux->filhos[i - 1]->nchaves; j > 0; j--) {
                        aux->filhos[i]->chaves[j] = aux->filhos[i]->chaves[j - 1];
                    }
                    aux->filhos[i]->chaves[0] = aux->chaves[i - 1];
                    aux->filhos[i]->nchaves++;
                    aux->chaves[i - 1] = aux->filhos[i - 1]->chaves[(aux->filhos[i - 1]->nchaves) - 1];
                    aux->filhos[i]->filhos[0] = aux->filhos[i - 1]->filhos[aux->filhos[i - 1]->nchaves];
                    aux->filhos[i - 1]->nchaves--;
                    deletar(&(aux->filhos[i]), ch);
                }
                else if ((i >= 0 && i < aux->nchaves) && aux->filhos[i + 1]->nchaves >= t) {
                    aux->filhos[i]->chaves[t - 1] = aux->chaves[i];
                    aux->filhos[i]->nchaves++;
                    aux->chaves[i] = aux->filhos[i + 1]->chaves[0];
                    aux->filhos[i]->filhos[t + 1] = aux->filhos[i + 1]->filhos[0];
                    for (j = 0; j < aux->filhos[i + 1]->nchaves - 1; j++) {
                        aux->filhos[i + 1]->chaves[j] = aux->filhos[i + 1]->chaves[j + 1];
                    }
                    aux->filhos[i + 1]->nchaves--;
                    deletar(&(aux->filhos[i]), ch);
                }
                else if (i == 0 && aux->filhos[i + 1]->nchaves == t - 1) { //Caso 3b
                    aux->filhos[i]->chaves[t - 1] = aux->chaves[i];
                    aux->filhos[i]->filhos[t] = aux->filhos[i + 1]->filhos[i];
                    for (j = 0; j < t - 1; j++) {
                        aux->filhos[i]->chaves[j + t] = aux->filhos[i + 1]->chaves[j];
                        aux->filhos[i]->filhos[j + t + 1] = aux->filhos[i + 1]->filhos[j + 1];
                    }
                    aux->filhos[i]->nchaves = 2*t - 1;
                    for (j = i; j <  aux->nchaves - 1; j++) {
                        aux->chaves[j] = aux->chaves[j + 1];
                        aux->filhos[j + 1] = aux->filhos[j + 2];
                    }
                    aux->nchaves--;
                    aux->filhos[aux->nchaves + 1] = NULL;
                    deletar(&(aux->filhos[i]), ch);
                }
                else if (i == aux->nchaves && aux->filhos[aux->nchaves - 1]->nchaves == t - 1) {
                    aux->filhos[aux->nchaves - 1]->chaves[t - 1] = aux->chaves[aux->nchaves - 1];
                    aux->filhos[aux->nchaves - 1]->filhos[t] = aux->filhos[i]->filhos[0];
                    for (j = 0; j < t - 1; j++) {
                        aux->filhos[aux->nchaves - 1]->chaves[j + t] = aux->filhos[i]->chaves[j];
                        aux->filhos[aux->nchaves - 1]->filhos[j + t + 1] = aux->filhos[i]->filhos[j + 1];
                    }
                    aux->filhos[aux->nchaves - 1]->nchaves = 2*t - 1;
                    aux->filhos[aux->nchaves] = NULL;
                    aux->nchaves--;
                    deletar(&(aux->filhos[i - 1]), ch);
                }
                else if ((i > 0 && i < aux->filhos[i]->nchaves) && (aux->filhos[i - 1]->nchaves == t - 1 && aux->filhos[i + 1]->nchaves == t - 1)) {
                    aux->filhos[i]->chaves[t - 1] = aux->chaves[i];
                    aux->filhos[i]->filhos[t] = aux->filhos[i];
                    for (j = 0; j < t - 1; j++) {
                        aux->filhos[i]->chaves[j + t] = aux->filhos[i + 1]->chaves[j];
                        aux->filhos[i]->filhos[j + t + 1] = aux->filhos[i + 1]->filhos[j + 1];
                    }
                    aux->filhos[i]->nchaves = 2*t - 1;
                    for (j = i; j <  aux->nchaves - 1; j++) {
                        aux->chaves[j] = aux->chaves[j + 1];
                        aux->filhos[j + 1] = aux->filhos[j + 2];
                    }
                    aux->nchaves--;
                    deletar(&(aux->filhos[i]), ch);
                }
            }
            else { //Quando nenhum dos 3 casos são escolhidos
                deletar(&(aux->filhos[i]), ch);
            }
        }
    }
}

void deletarDaRaiz (Noh **T, int ch) {
    Noh *r = *T;

    if (r->nchaves == 0) return;
    else
        deletar(&r, ch);

    if (r->nchaves == 0 && r->folha == false) {
        *T = r->filhos[0];
    }

}
//Término da remoção de rvore B

int main (int argc, char *argv[]) {
    FILE *entrada;
    FILE *saida;
    Noh *raiz = NULL;
    char buffer[50];
    int ch;
    //Le o arquivo passado
    strcpy(buffer, argv[1]);
    entrada = fopen(buffer, "r");
    saida = fopen("saida.txt", "wb+");

    raiz = criarArvoreB();
    //inicializa a leitura do r arquivo ateh o usuario digitar 'fim'
    while (strcmp(buffer, "fim") != 0) {
        fscanf(entrada, "%s", buffer);
        if (strcmp(buffer, "insere") == 0) {
            fscanf(entrada, "%d", &ch);
            if (!buscar(raiz, ch))
                inserir(&raiz, ch);
        }
        else if (strcmp(buffer, "remove") == 0) {
            fscanf(entrada, "%d", &ch);
            if (buscar(raiz, ch))
                deletarDaRaiz(&raiz, ch);
        }
        else if (strcmp(buffer, "imprime") == 0)  {
            imprimir(raiz, &saida);
            fprintf(saida, "\n");
        }
    }

    fclose(entrada);
    fclose(saida);

    return 0;
}
