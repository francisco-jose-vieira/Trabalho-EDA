#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void insertionSort(int v[], long long n) {
    for (long long i = 1; i < n; i++) {
        int chave = v[i];
        long long j = i - 1;

        while (j >= 0 && v[j] > chave) {
            v[j + 1] = v[j];    
            j--;
        }

        v[j + 1] = chave;
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
        printf("\n=== INSERTION SORT ===\n");

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
        insertionSort(v, n);
        double tempo = (double)(clock() - inicio) / CLOCKS_PER_SEC;

        printf("Tempo: %.6f segundos\n", tempo);

        FILE *f = fopen("relatorio.csv", "a+");

        if (f != NULL) {
            fseek(f, 0, SEEK_END);

            if (ftell(f) == 0)
                fprintf(f, "algoritmo,tamanho,tempo_segundos\n");

            fprintf(f, "Insertion Sort,%lld,%.6f\n", n, tempo);
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
