#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_FILA 5
#define TAM_PILHA 3

// Estrutura que representa uma peça do jogo
typedef struct {
    char tipo;
    int id;
} Peca;

// Estrutura para a fila circular
typedef struct {
    Peca itens[TAM_FILA];
    int frente, tras, tamanho;
} Fila;

// Estrutura para a pilha de reserva
typedef struct {
    Peca itens[TAM_PILHA];
    int topo;
} Pilha;

// Funções da fila
void inicializarFila(Fila *f) {
    f->frente = 0;
    f->tras = -1;
    f->tamanho = 0;
}

int filaVazia(Fila *f) {
    return f->tamanho == 0;
}

int filaCheia(Fila *f) {
    return f->tamanho == TAM_FILA;
}

void enfileirar(Fila *f, Peca p) {
    if (filaCheia(f)) return;
    f->tras = (f->tras + 1) % TAM_FILA;
    f->itens[f->tras] = p;
    f->tamanho++;
}

Peca desenfileirar(Fila *f) {
    Peca p = {'-', -1};
    if (!filaVazia(f)) {
        p = f->itens[f->frente];
        f->frente = (f->frente + 1) % TAM_FILA;
        f->tamanho--;
    }
    return p;
}

// Funções da pilha
void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

int pilhaVazia(Pilha *p) {
    return p->topo == -1;
}

int pilhaCheia(Pilha *p) {
    return p->topo == TAM_PILHA - 1;
}

void empilhar(Pilha *p, Peca peça) {
    if (!pilhaCheia(p)) {
        p->topo++;
        p->itens[p->topo] = peça;
    }
}

Peca desempilhar(Pilha *p) {
    Peca peça = {'-', -1};
    if (!pilhaVazia(p)) {
        peça = p->itens[p->topo];
        p->topo--;
    }
    return peça;
}

// Geração de peças aleatórias
Peca gerarPeca(int id) {
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca p;
    p.tipo = tipos[rand() % 4];
    p.id = id;
    return p;
}

// Exibição das estruturas
void exibirFila(Fila *f) {
    printf("Fila de peças:\t");
    int i, index = f->frente;
    for (i = 0; i < f->tamanho; i++) {
        printf("[%c %d] ", f->itens[index].tipo, f->itens[index].id);
        index = (index + 1) % TAM_FILA;
    }
    printf("\n");
}

void exibirPilha(Pilha *p) {
    printf("Pilha de reserva (Topo -> Base):\t");
    if (pilhaVazia(p)) {
        printf("[Vazia]");
    } else {
        for (int i = p->topo; i >= 0; i--) {
            printf("[%c %d] ", p->itens[i].tipo, p->itens[i].id);
        }
    }
    printf("\n");
}

void exibirEstado(Fila *f, Pilha *p) {
    printf("\n=== ESTADO ATUAL ===\n");
    exibirFila(f);
    exibirPilha(p);
    printf("=====================\n");
}

// Função principal
int main() {
    Fila fila;
    Pilha pilha;
    int opcao, idGerado = 0;

    srand(time(NULL));

    inicializarFila(&fila);
    inicializarPilha(&pilha);

    // Preenche a fila inicial
    for (int i = 0; i < TAM_FILA; i++) {
        enfileirar(&fila, gerarPeca(idGerado++));
    }

    do {
        exibirEstado(&fila, &pilha);

        printf("\nOpções de Ação:\n");
        printf("1 - Jogar peça\n");
        printf("2 - Reservar peça\n");
        printf("3 - Usar peça reservada\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1: {
                if (filaVazia(&fila)) {
                    printf("\n⚠️  A fila está vazia!\n");
                } else {
                    Peca jogada = desenfileirar(&fila);
                    printf("\n🎮 Peça jogada: [%c %d]\n", jogada.tipo, jogada.id);
                    enfileirar(&fila, gerarPeca(idGerado++)); // Mantém fila cheia
                }
                break;
            }
            case 2: {
                if (filaVazia(&fila)) {
                    printf("\n⚠️  Não há peças na fila para reservar!\n");
                } else if (pilhaCheia(&pilha)) {
                    printf("\n⚠️  A pilha de reserva está cheia!\n");
                } else {
                    Peca reservada = desenfileirar(&fila);
                    empilhar(&pilha, reservada);
                    printf("\n📦 Peça [%c %d] movida para reserva.\n", reservada.tipo, reservada.id);
                    enfileirar(&fila, gerarPeca(idGerado++));
                }
                break;
            }
            case 3: {
                if (pilhaVazia(&pilha)) {
                    printf("\n⚠️  Nenhuma peça na reserva!\n");
                } else {
                    Peca usada = desempilhar(&pilha);
                    printf("\n🚀 Peça [%c %d] usada da reserva!\n", usada.tipo, usada.id);
                }
                break;
            }
            case 0:
                printf("\nSaindo do jogo... 👋\n");
                break;
            default:
                printf("\n❌ Opção inválida! Tente novamente.\n");
        }

    } while (opcao != 0);

    return 0;
}
