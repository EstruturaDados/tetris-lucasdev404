#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_FILA 5
#define TAM_PILHA 3

// Estrutura que representa uma peça
typedef struct {
    char tipo;
    int id;
} Peca;

// Estrutura da fila circular
typedef struct {
    Peca itens[TAM_FILA];
    int frente, tras, tamanho;
} Fila;

// Estrutura da pilha
typedef struct {
    Peca itens[TAM_PILHA];
    int topo;
} Pilha;

// ---------- FUNÇÕES DE FILA ----------
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

// ---------- FUNÇÕES DE PILHA ----------
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

// ---------- FUNÇÃO DE GERAÇÃO DE PEÇAS ----------
Peca gerarPeca(int id) {
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca p;
    p.tipo = tipos[rand() % 4];
    p.id = id;
    return p;
}

// ---------- EXIBIÇÃO DAS ESTRUTURAS ----------
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
    printf("\n========== ESTADO ATUAL ==========\n");
    exibirFila(f);
    exibirPilha(p);
    printf("==================================\n");
}

// ---------- TROCAS ENTRE FILA E PILHA ----------
void trocarPeçaAtual(Fila *f, Pilha *p) {
    if (filaVazia(f) || pilhaVazia(p)) {
        printf("\n⚠️  Não é possível trocar: uma das estruturas está vazia!\n");
        return;
    }

    int indiceFrente = f->frente;
    Peca temp = f->itens[indiceFrente];
    f->itens[indiceFrente] = p->itens[p->topo];
    p->itens[p->topo] = temp;

    printf("\n🔄 Troca realizada entre a frente da fila e o topo da pilha!\n");
}

void trocarTres(Fila *f, Pilha *p) {
    if (f->tamanho < 3 || p->topo < 2) {
        printf("\n⚠️  Não há peças suficientes para a troca múltipla!\n");
        return;
    }

    for (int i = 0; i < 3; i++) {
        int indiceFila = (f->frente + i) % TAM_FILA;
        Peca temp = f->itens[indiceFila];
        f->itens[indiceFila] = p->itens[p->topo - i];
        p->itens[p->topo - i] = temp;
    }

    printf("\n🔁 Troca múltipla (3 peças) realizada entre fila e pilha!\n");
}

// ---------- FUNÇÃO PRINCIPAL ----------
int main() {
    Fila fila;
    Pilha pilha;
    int opcao, idGerado = 0;

    srand(time(NULL));
    inicializarFila(&fila);
    inicializarPilha(&pilha);

    // --- TÍTULO DO JOGO ---
    printf("=====================================\n");
    printf("         🎮 TETRIS STACK 🎮          \n");
    printf("  Gerenciador de Peças - ByteBros\n");
    printf("=====================================\n\n");

    // Preenche a fila inicial
    for (int i = 0; i < TAM_FILA; i++) {
        enfileirar(&fila, gerarPeca(idGerado++));
    }

    do {
        exibirEstado(&fila, &pilha);

        printf("\nOpções de Ação:\n");
        printf("1 - Jogar peça da frente da fila\n");
        printf("2 - Enviar peça da fila para a pilha de reserva\n");
        printf("3 - Usar peça da pilha de reserva\n");
        printf("4 - Trocar peça da frente da fila com o topo da pilha\n");
        printf("5 - Trocar as 3 primeiras da fila com as 3 peças da pilha\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1: { // Jogar peça
                if (filaVazia(&fila)) {
                    printf("\n⚠️  Fila vazia!\n");
                } else {
                    Peca jogada = desenfileirar(&fila);
                    printf("\n🎮 Peça jogada: [%c %d]\n", jogada.tipo, jogada.id);
                    enfileirar(&fila, gerarPeca(idGerado++));
                }
                break;
            }

            case 2: { // Reservar peça
                if (filaVazia(&fila)) {
                    printf("\n⚠️  Fila vazia!\n");
                } else if (pilhaCheia(&pilha)) {
                    printf("\n⚠️  Pilha cheia!\n");
                } else {
                    Peca reservada = desenfileirar(&fila);
                    empilhar(&pilha, reservada);
                    printf("\n📦 Peça [%c %d] movida para a reserva.\n", reservada.tipo, reservada.id);
                    enfileirar(&fila, gerarPeca(idGerado++));
                }
                break;
            }

            case 3: { // Usar peça reservada
                if (pilhaVazia(&pilha)) {
                    printf("\n⚠️  Nenhuma peça na pilha de reserva!\n");
                } else {
                    Peca usada = desempilhar(&pilha);
                    printf("\n🚀 Peça [%c %d] usada da reserva!\n", usada.tipo, usada.id);
                }
                break;
            }

            case 4:
                trocarPeçaAtual(&fila, &pilha);
                break;

            case 5:
                trocarTres(&fila, &pilha);
                break;

            case 0:
                printf("\nSaindo do jogo... 👋\n");
                break;

            default:
                printf("\n❌ Opção inválida! Tente novamente.\n");
        }

    } while (opcao != 0);

    return 0;
}
