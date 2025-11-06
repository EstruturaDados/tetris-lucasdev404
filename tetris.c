#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// ============================
// Definição da struct Peça
// ============================
typedef struct {
    char nome; // Tipo da peça: I, O, T, L
    int id;    // Identificador único
} Peca;

// ============================
// Constantes e variáveis globais
// ============================
#define TAM_FILA 5 // Tamanho fixo da fila

typedef struct {
    Peca pecas[TAM_FILA];
    int frente;  // Índice da frente da fila
    int tras;    // Índice do final da fila
    int qtd;     // Quantidade atual de elementos
} Fila;

// ============================
// Protótipos das funções
// ============================
void inicializarFila(Fila *f);
int filaCheia(Fila *f);
int filaVazia(Fila *f);
void enfileirar(Fila *f, Peca nova);
void desenfileirar(Fila *f);
void exibirFila(Fila *f);
Peca gerarPeca();

// ============================
// Função principal
// ============================
int main() {
    Fila fila;
    inicializarFila(&fila);
    srand(time(NULL)); // Semente para gerar peças aleatórias

    // Preenche a fila inicial com 5 peças
    for (int i = 0; i < TAM_FILA; i++) {
        enfileirar(&fila, gerarPeca());
    }

    int opcao;
    do {
        printf("\n==============================\n");
        printf("     TETRIS STACK - ByteBros\n");
        printf("==============================\n");
        printf("Fila de Pecas:\n");
        exibirFila(&fila);

        printf("\nOpcoes de Acao:\n");
        printf("1 - Jogar peca (dequeue)\n");
        printf("2 - Inserir nova peca (enqueue)\n");
        printf("0 - Sair\n");
        printf("------------------------------\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                desenfileirar(&fila);
                break;
            case 2:
                enfileirar(&fila, gerarPeca());
                break;
            case 0:
                printf("\nEncerrando simulacao do Tetris Stack...\n");
                break;
            default:
                printf("\nOpcao invalida! Tente novamente.\n");
        }
    } while (opcao != 0);

    return 0;
}

// ============================
// Funções da Fila
// ============================

// Inicializa a fila vazia
void inicializarFila(Fila *f) {
    f->frente = 0;
    f->tras = -1;
    f->qtd = 0;
}

// Verifica se a fila está cheia
int filaCheia(Fila *f) {
    return f->qtd == TAM_FILA;
}

// Verifica se a fila está vazia
int filaVazia(Fila *f) {
    return f->qtd == 0;
}

// Insere nova peça no final da fila
void enfileirar(Fila *f, Peca nova) {
    if (filaCheia(f)) {
        printf("\nFila cheia! Nao e possivel adicionar nova peca.\n");
        return;
    }

    f->tras = (f->tras + 1) % TAM_FILA;
    f->pecas[f->tras] = nova;
    f->qtd++;

    printf("\nPeca [%c %d] adicionada ao final da fila.\n", nova.nome, nova.id);
}

// Remove a peça da frente da fila
void desenfileirar(Fila *f) {
    if (filaVazia(f)) {
        printf("\nFila vazia! Nenhuma peca para jogar.\n");
        return;
    }

    Peca removida = f->pecas[f->frente];
    f->frente = (f->frente + 1) % TAM_FILA;
    f->qtd--;

    printf("\nPeca [%c %d] jogada (removida da fila).\n", removida.nome, removida.id);
}

// Exibe o conteúdo atual da fila
void exibirFila(Fila *f) {
    if (filaVazia(f)) {
        printf("[Fila vazia]\n");
        return;
    }

    printf("[ ");
    for (int i = 0; i < f->qtd; i++) {
        int index = (f->frente + i) % TAM_FILA;
        printf("%c %d", f->pecas[index].nome, f->pecas[index].id);
        if (i < f->qtd - 1) printf(" | ");
    }
    printf(" ]\n");
}

// Gera uma nova peça com tipo e ID únicos
Peca gerarPeca() {
    static int contadorID = 0; // Garante IDs únicos
    char tipos[] = {'T', 'O', 'L', 'I'};
    Peca nova;
    nova.nome = tipos[rand() % 4]; // Escolhe aleatoriamente entre I, O, T, L
    nova.id = contadorID++;
    return nova;
}
