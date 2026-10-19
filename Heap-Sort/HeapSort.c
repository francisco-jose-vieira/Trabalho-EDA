#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void trocar(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

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

void descer(int v[], long long n, long long i) {
    while (i < n / 2) {
        long long esq = 2 * i + 1;
        long long dir = esq + 1;
        long long maior = esq;

        if (dir < n && v[dir] > v[esq])
            maior = dir;

        if (v[maior] > v[i]) {
            trocar(&v[i], &v[maior]);
            i = maior;
        } else {
            break;
        }
    }
}

void construir(int v[], long long n) {
    for (long long i = n / 2; i > 0; i--)
        descer(v, n, i - 1);
}

void heapSort(int v[], long long n) {
    construir(v, n);

    for (long long i = n; i > 1; i--) {
        trocar(&v[0], &v[i - 1]);
        descer(v, i - 1, 0);
    }
}

int main() {
    long long tamanhos[] = {
        1000, 10000, 100000, 1000000,
        10000000, 100000000, 1000000000
    };

    int op, continuar = 1;
    srand((unsigned int)time(NULL));

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
        int *v = malloc((size_t)n * sizeof(int));

        if (v == NULL) {
            printf("Memoria insuficiente!\n");
            continue;
        }

        for (long long i = 0; i < n; i++)
            v[i] = rand();

        printf("Executando com %lld elementos...\n", n);

        clock_t inicio = clock();
        heapSort(v, n);
        double tempo = (double)(clock() - inicio) / CLOCKS_PER_SEC;

        printf("Tempo: %.6f segundos\n", tempo);

        FILE *f = fopen("relatorio.csv", "a+");

        if (f != NULL) {
            fseek(f, 0, SEEK_END);

            if (ftell(f) == 0)
                fprintf(f, "algoritmo,tamanho,tempo_segundos\n");

            fprintf(f, "Heap Sort,%lld,%.6f\n", n, tempo);
            fclose(f);
            printf("Resultado salvo em relatorio.csv\n");
        } else {
            printf("Erro ao salvar o relatorio!\n");
        }

        free(v);

        printf("Outro teste? (1-Sim / 0-Nao): ");

        if (scanf("%d", &continuar) != 1)
            break;
    }

    return 0;
}
