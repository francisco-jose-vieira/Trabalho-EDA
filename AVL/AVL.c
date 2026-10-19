#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

#define TESTES 1000
#define INSERCOES 10000
#define REMOCOES 1000
#define MAX_CHAVE 100000
#define V 1
#define F 0

// Representa um nó da árvore AVL
typedef struct No {
    int chave;
    int bal; /* Altura da direita - altura da esquerda */
    struct No *esq, *dir;
} No;

// Cria um novo nó com fator de balanceamento igual a zero
No *novoNo(int x) {
    No *p = malloc(sizeof(*p));

    if (!p) {
        fprintf(stderr, "Erro de memoria.\n");
        exit(EXIT_FAILURE);
    }

    p->chave = x;
    p->bal = 0;
    p->esq = p->dir = NULL;
    return p;
}

// Trata desequilíbrios para a esquerda, usando rotação simples ou dupla
No *Caso1(No *pt, int insercao) {
    No *ptu = pt->esq;

    if (ptu->bal <= 0) {
        int balFilho = ptu->bal;

        // Rotação simples à direita
        pt->esq = ptu->dir;
        ptu->dir = pt;

        if (balFilho == 0 && !insercao) {
            pt->bal = -1;
            ptu->bal = 1;
        } else {
            pt->bal = 0;
            ptu->bal = 0;
        }

        return ptu;
    }

    // Rotação dupla: esquerda no filho e direita no nó atual
    No *ptv = ptu->dir;
    int balNeto = ptv->bal;

    ptu->dir = ptv->esq;
    pt->esq = ptv->dir;
    ptv->esq = ptu;
    ptv->dir = pt;

    // Atualiza os fatores de balanceamento após a rotação
    if (balNeto == -1) {
        pt->bal = 1;
        ptu->bal = 0;
    } else if (balNeto == 0) {
        pt->bal = 0;
        ptu->bal = 0;
    } else {
        pt->bal = 0;
        ptu->bal = -1;
    }

    ptv->bal = 0;
    return ptv;
}

// Trata desequilíbrios para a direita, usando rotação simples ou dupla
No *Caso2(No *pt, int insercao) {
    No *ptu = pt->dir;

    if (ptu->bal >= 0) {
        int balFilho = ptu->bal;

        // Rotação simples à esquerda
        pt->dir = ptu->esq;
        ptu->esq = pt;

        if (balFilho == 0 && !insercao) {
            pt->bal = 1;
            ptu->bal = -1;
        } else {
            pt->bal = 0;
            ptu->bal = 0;
        }

        return ptu;
    }

    // Rotação dupla: direita no filho e esquerda no nó atual
    No *ptv = ptu->esq;
    int balNeto = ptv->bal;

    ptu->esq = ptv->dir;
    pt->dir = ptv->esq;
    ptv->dir = ptu;
    ptv->esq = pt;

    // Ajusta os fatores de balanceamento após a rotação
    if (balNeto == 1) {
        pt->bal = -1;
        ptu->bal = 0;
    } else if (balNeto == 0) {
        pt->bal = 0;
        ptu->bal = 0;
    } else {
        pt->bal = 0;
        ptu->bal = 1;
    }

    ptv->bal = 0;
    return ptv;
}

// Insere uma chave e atualiza o balanceamento dos ancestrais
No *InserirAVL(int x, No *pt, int *h) {
    if (pt == NULL) {
        *h = V;
        return novoNo(x);
    }

    // Não permite chaves duplicadas
    if (x == pt->chave) {
        *h = F;
        return pt;
    }

    if (x < pt->chave) {
        pt->esq = InserirAVL(x, pt->esq, h);

        if (*h == V) {
            if (pt->bal == 1) {
                pt->bal = 0;
                *h = F;
            } else if (pt->bal == 0) {
                pt->bal = -1;
            } else {
                pt = Caso1(pt, V);
                *h = F;
            }
        }
    } else {
        pt->dir = InserirAVL(x, pt->dir, h);

        if (*h == V) {
            if (pt->bal == -1) {
                pt->bal = 0;
                *h = F;
            } else if (pt->bal == 0) {
                pt->bal = 1;
            } else {
                pt = Caso2(pt, V);
                *h = F;
            }
        }
    }

    return pt;
}

// Reequilibra a árvore depois que uma subárvore diminui de altura
No *balancear(No *pt, int R, int *h) {
    if (pt == NULL || *h == F)
        return pt;

    if (R == V) {
        // A subárvore esquerda diminuiu
        if (pt->bal == -1) {
            pt->bal = 0;
        } else if (pt->bal == 0) {
            pt->bal = 1;
            *h = F;
        } else {
            pt = Caso2(pt, F);
            if (pt->bal == -1)
                *h = F;
        }
    } else {
        // A subárvore direita diminuiu
        if (pt->bal == 1) {
            pt->bal = 0;
        } else if (pt->bal == 0) {
            pt->bal = -1;
            *h = F;
        } else {
            pt = Caso1(pt, F);
            if (pt->bal == 1)
                *h = F;
        }
    }

    return pt;
}

// Encontra o nó com a menor chave de uma subárvore
No *menorNo(No *pt) {
    while (pt && pt->esq)
        pt = pt->esq;

    return pt;
}

// Remove uma chave e corrige o balanceamento quando necessário
No *RemoverAVL(No *pt, int x, int *h) {
    if (pt == NULL) {
        *h = F;
        return NULL;
    }

    if (x < pt->chave) {
        pt->esq = RemoverAVL(pt->esq, x, h);
        return balancear(pt, V, h);
    }

    if (x > pt->chave) {
        pt->dir = RemoverAVL(pt->dir, x, h);
        return balancear(pt, F, h);
    }

    // Nó encontrado: possui zero ou um filho
    if (pt->esq == NULL || pt->dir == NULL) {
        No *filho = pt->esq ? pt->esq : pt->dir;
        free(pt);
        *h = V;
        return filho;
    }

    // Com dois filhos, substitui a chave pelo sucessor em ordem
    No *s = menorNo(pt->dir);
    pt->chave = s->chave;
    pt->dir = RemoverAVL(pt->dir, s->chave, h);

    return balancear(pt, F, h);
}

// Conta quantos nós existem na árvore
int contarNos(No *pt) {
    return pt ? 1 + contarNos(pt->esq) + contarNos(pt->dir) : 0;
}

// Verifica a ordenação, os fatores de balanceamento e a altura da árvore
int verificarAVL(No *pt, long long min, long long max, int *altura) {
    if (!pt) {
        *altura = 0;
        return 1;
    }

    // Confere se a chave respeita os limites da árvore de busca
    if (pt->chave <= min || pt->chave >= max)
        return 0;

    int he, hd;

    if (!verificarAVL(pt->esq, min, pt->chave, &he) ||
        !verificarAVL(pt->dir, pt->chave, max, &hd))
        return 0;

    // Confere se o fator armazenado corresponde ao fator real
    int balReal = hd - he;

    if (balReal < -1 || balReal > 1 || pt->bal != balReal)
        return 0;

    *altura = 1 + (he > hd ? he : hd);
    return 1;
}

// Libera todos os nós da árvore recursivamente
void liberar(No *pt) {
    if (!pt)
        return;

    liberar(pt->esq);
    liberar(pt->dir);
    free(pt);
}

// Embaralha o vetor para selecionar chaves de remoção em ordem aleatória
void embaralhar(int *v, int n) {
    for (int i = n - 1; i > 0; --i) {
        int j = rand() % (i + 1);
        int tmp = v[i];
        v[i] = v[j];
        v[j] = tmp;
    }
}

// Executa os testes de inserção, remoção e validação da árvore AVL
void testarAVL(void) {
    int chaves[INSERCOES];
    unsigned char usadas[MAX_CHAVE + 1];
    int erros = 0;

    for (int teste = 1; teste <= TESTES; ++teste) {
        No *raiz = NULL;
        int h = F, altura;

        // Reinicia o controle de chaves para este teste
        for (int i = 0; i <= MAX_CHAVE; ++i)
            usadas[i] = 0;

        // Gera e insere chaves aleatórias sem repetição
        for (int i = 0; i < INSERCOES; ++i) {
            int x;

            do {
                x = rand() % (MAX_CHAVE + 1);
            } while (usadas[x]);

            usadas[x] = 1;
            chaves[i] = x;
            raiz = InserirAVL(x, raiz, &h);
        }

        // Confere a quantidade de nós e a validade da árvore
        if (contarNos(raiz) != INSERCOES ||
            !verificarAVL(raiz, LLONG_MIN, LLONG_MAX, &altura)) {
            ++erros;
            printf("Falha apos insercao no teste %d.\n", teste);
            liberar(raiz);
            continue;
        }

        // Remove uma amostra aleatória de chaves inseridas
        embaralhar(chaves, INSERCOES);

        for (int i = 0; i < REMOCOES; ++i)
            raiz = RemoverAVL(raiz, chaves[i], &h);

        // Confere a quantidade restante e o balanceamento após as remoções
        if (contarNos(raiz) != INSERCOES - REMOCOES ||
            !verificarAVL(raiz, LLONG_MIN, LLONG_MAX, &altura)) {
            ++erros;
            printf("Falha apos remocao no teste %d.\n", teste);
        }

        liberar(raiz);

        // Exibe o progresso a cada 100 testes
        if (teste % 100 == 0)
            printf("Testes: %d/%d | falhas: %d\n",
                   teste, TESTES, erros);
    }

    printf("\nResultado: %d falha(s) em %d testes.\n", erros, TESTES);
}

int main(void) {
    srand((unsigned)time(NULL));

    testarAVL();

    return 0;
}