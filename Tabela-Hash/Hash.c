#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

#define MAX_CHAVE 2000000000ULL

// Tamanhos de tabela disponíveis para os testes
static const size_t tamanhos[] = {
    65536, 98299, 131072, 196613, 262144,
    393209, 524288, 786431, 1048576, 1572869,
    2097152, 3145721, 4194304, 6291449, 8388608
};

#define QTD (sizeof(tamanhos) / sizeof(tamanhos[0]))

// Armazena o tamanho testado e as colisões de cada método
typedef struct {
    long long n;
    unsigned long long divisao, multiplicacao;
} Resultado;

// Gera uma chave aleatória entre 0 e MAX_CHAVE
static int gerarChave(void) {
    return (int)(((uint64_t)rand() << 31 ^ (uint64_t)rand())
                 % (MAX_CHAVE + 1));
}

// Método da divisão: o resto da divisão define o índice da tabela
static size_t hashDivisao(int chave, size_t n) {
    return (size_t)((unsigned int)chave % n);
}

/* Método do meio dos quadrados:
   eleva a chave ao quadrado e extrai dígitos centrais */
static size_t hashMultiplicacao(int chave, size_t n) {
    uint64_t quadrado = (uint64_t)(unsigned int)chave *
                        (unsigned int)chave;
    uint64_t x = quadrado;
    size_t base = 1;
    size_t digitosN = 0, digitosX = 0;

    // Conta os dígitos do tamanho da tabela e do quadrado
    for (size_t t = n; t; t /= 10) digitosN++;
    for (uint64_t t = x; t; t /= 10) digitosX++;

    // Define a quantidade de dígitos que serão extraídos
    for (size_t i = 0; i < digitosN; i++) base *= 10;

    // Calcula quantos dígitos serão removidos da direita
    size_t inicio = digitosX > digitosN
                  ? (digitosX - digitosN) / 2 : 0;

    while (inicio--) x /= 10;

    // Obtém os dígitos selecionados e limita o índice à tabela
    return (size_t)((x % base) % n);
}

/* Insere a chave usando sondagem linear.
   Se a posição estiver ocupada, procura a próxima posição livre. */
static unsigned long long inserir(
    int tabela[], size_t n, int chave,
    size_t (*hash)(int, size_t)
) {
    size_t pos = hash(chave, n);
    unsigned long long colisoes = 0;

    while (tabela[pos] != -1) {
        colisoes++;
        pos = (pos + 1) % n;
    }

    tabela[pos] = chave;
    return colisoes;
}

// Gera as chaves, executa os dois métodos e contabiliza as colisões
static int executarTeste(size_t n, Resultado *r) {
    int *chaves = malloc(n * sizeof(int));
    int *div = malloc(n * sizeof(int));
    int *mul = malloc(n * sizeof(int));

    if (!chaves || !div || !mul) {
        printf("Memoria insuficiente para n = %zu.\n", n);
        free(chaves);
        free(div);
        free(mul);
        return 0;
    }

    printf("\nGerando %zu chaves aleatorias...\n", n);

    // Inicializa as tabelas com -1, indicando posições vazias
    for (size_t i = 0; i < n; i++) {
        chaves[i] = gerarChave();
        div[i] = mul[i] = -1;
    }

    r->n = (long long)n;
    r->divisao = r->multiplicacao = 0;

    // Executa o método da divisão
    printf("Executando divisao...\n");
    for (size_t i = 0; i < n; i++)
        r->divisao += inserir(div, n, chaves[i], hashDivisao);

    // Executa o método do meio dos quadrados
    printf("Executando meio dos quadrados...\n");
    for (size_t i = 0; i < n; i++)
        r->multiplicacao += inserir(mul, n, chaves[i],
                                   hashMultiplicacao);

    printf("Divisao: %llu colisoes\n", r->divisao);
    printf("Multiplicacao: %llu colisoes\n", r->multiplicacao);

    // Libera a memória utilizada no teste
    free(chaves);
    free(div);
    free(mul);
    return 1;
}

// Salva os resultados de todos os testes em um arquivo CSV
static void salvarCSV(const Resultado r[], size_t qtd) {
    FILE *f = fopen("relatorio_hash.csv", "w");

    if (!f) {
        printf("Erro ao criar relatorio_hash.csv!\n");
        return;
    }

    fprintf(f, "n,divisao,multiplicacao\n");

    for (size_t i = 0; i < qtd; i++)
        fprintf(f, "%lld,%llu,%llu\n",
                r[i].n, r[i].divisao, r[i].multiplicacao);

    fclose(f);
    printf("\nRelatorio salvo em relatorio_hash.csv\n");
}

int main(void) {
    Resultado resultados[QTD];
    size_t qtd = 0;
    int op, continuar = 1;

    srand((unsigned int)time(NULL));

    // Menu para selecionar o tamanho da tabela e repetir os testes
    while (continuar) {
        printf("\n=== TABELA HASH: COLISOES ===\n");

        for (size_t i = 0; i < QTD; i++)
            printf("%zu - %zu chaves\n", i + 1, tamanhos[i]);

        printf("0 - Sair\nEscolha: ");

        if (scanf("%d", &op) != 1 || op == 0)
            break;

        if (op < 1 || op > (int)QTD) {
            printf("Opcao invalida!\n");
            continue;
        }

        // Guarda o resultado se o teste for concluído
        if (executarTeste(tamanhos[op - 1], &resultados[qtd]))
            qtd++;

        printf("\nOutro teste? (1-Sim / 0-Nao): ");

        if (scanf("%d", &continuar) != 1 || continuar != 1)
            break;
    }

    // Exporta os resultados acumulados ao encerrar o programa
    if (qtd)
        salvarCSV(resultados, qtd);
    else
        printf("Nenhum teste foi realizado.\n");

    return 0;
}