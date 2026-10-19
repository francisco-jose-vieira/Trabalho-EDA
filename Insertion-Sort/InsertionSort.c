#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Ordena o vetor utilizando o algoritmo Insertion Sort
void insertionSort(int v[], long long n) {
    for (long long i = 1; i < n; i++) {
        int chave = v[i];
        long long j = i - 1;

        // Desloca para a direita os elementos maiores que a chave
        while (j >= 0 && v[j] > chave) {
            v[j + 1] = v[j];
            j--;
        }

        // Insere a chave na posição correta
        v[j + 1] = chave;
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

        // Reserva memória para armazenar os elementos do vetor
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
        insertionSort(v, n);
        double tempo = (double)(clock() - inicio) / CLOCKS_PER_SEC;

        printf("Tempo: %.6f segundos\n", tempo);

        // Abre o CSV para acrescentar o resultado do teste
        FILE *f = fopen("relatorio.csv", "a+");

        if (f != NULL) {
            fseek(f, 0, SEEK_END);

            // Cria o cabeçalho somente se o arquivo estiver vazio
            if (ftell(f) == 0)
                fprintf(f, "algoritmo,tamanho,tempo_segundos\n");

            fprintf(f, "Insertion Sort,%lld,%.6f\n", n, tempo);
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
