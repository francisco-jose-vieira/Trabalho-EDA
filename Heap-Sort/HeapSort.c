#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Troca os valores de duas posições do vetor
void trocar(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Sobe um elemento na árvore enquanto ele for maior que seu pai
void subir(int v[], long long i) {
    while (i > 0) {
        long long pai = (i - 1) / 2;

        if (v[i] > v[pai]) {
            trocar(&v[i], &v[pai]);
            i = pai;
        } else {
            break;
        }
    }
}

// Desce um elemento para manter a propriedade do heap máximo
void descer(int v[], long long n, long long i) {
    while (i < n / 2) {
        long long esq = 2 * i + 1;
        long long dir = esq + 1;
        long long maior = esq;

        // Identifica o maior entre os filhos esquerdo e direito
        if (dir < n && v[dir] > v[esq])
            maior = dir;

        // Troca com o filho maior se ele for maior que o elemento atual
        if (v[maior] > v[i]) {
            trocar(&v[i], &v[maior]);
            i = maior;
        } else {
            break;
        }
    }
}

// Transforma o vetor em um heap máximo
void construir(int v[], long long n) {
    for (long long i = n / 2; i > 0; i--)
        descer(v, n, i - 1);
}

// Ordena o vetor utilizando o algoritmo Heap Sort
void heapSort(int v[], long long n) {
    construir(v, n);

    // Coloca o maior elemento no final e reorganiza o heap
    for (long long i = n; i > 1; i--) {
        trocar(&v[0], &v[i - 1]);
        descer(v, i - 1, 0);
    }
}

int main(void) {
    // Tamanhos disponíveis para os testes de desempenho
    long long tamanhos[] = {
        1000, 10000, 100000, 1000000,
        10000000, 100000000, 1000000000
    };

    int op, continuar = 1;
    srand((unsigned int)time(NULL));

    // Menu para selecionar o tamanho do vetor
    while (continuar) {
        printf("\n=== HEAP SORT ===\n");

        for (int i = 0; i < 7; i++)
            printf("%d - %lld elementos\n", i + 1, tamanhos[i]);

        printf("0 - Sair\nEscolha: ");

        if (scanf("%d", &op) != 1 || op == 0)
            break;

        if (op < 1 || op > 7) {
            printf("Opcao invalida!\n");
            continue;
        }

        long long n = tamanhos[op - 1];

        // Reserva memória para armazenar os elementos
        int *v = malloc((size_t)n * sizeof(int));

        if (v == NULL) {
            printf("Memoria insuficiente!\n");
            continue;
        }

        // Preenche o vetor com números aleatórios
        for (long long i = 0; i < n; i++)
            v[i] = rand();

        printf("Executando com %lld elementos...\n", n);

        // Mede o tempo gasto exclusivamente na ordenação
        clock_t inicio = clock();
        heapSort(v, n);
        double tempo = (double)(clock() - inicio) / CLOCKS_PER_SEC;

        printf("Tempo: %.6f segundos\n", tempo);

        // Abre o CSV para acrescentar o resultado do teste
        FILE *f = fopen("relatorio.csv", "a+");

        if (f != NULL) {
            fseek(f, 0, SEEK_END);

            // Cria o cabeçalho somente se o arquivo estiver vazio
            if (ftell(f) == 0)
                fprintf(f, "algoritmo,tamanho,tempo_segundos\n");

            fprintf(f, "Heap Sort,%lld,%.6f\n", n, tempo);
            fclose(f);

            printf("Resultado salvo em relatorio.csv\n");
        } else {
            printf("Erro ao salvar o relatorio!\n");
        }

        // Libera a memória utilizada pelo vetor
        free(v);

        printf("Outro teste? (1-Sim / 0-Nao): ");

        if (scanf("%d", &continuar) != 1)
            break;
    }

    return 0;
}
