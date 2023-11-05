#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 100

// Definição da estrutura do livro
typedef struct {
    char titulo[50];
    char autor[50];
} Livro;

// Definição da estrutura da fila
typedef struct {
    Livro livros[MAX_SIZE];
    int frente, tras;
} Fila;

// Definição da estrutura da pilha
typedef struct {
    Livro livros[MAX_SIZE];
    int topo;
} Pilha;

// Função para ler livros de um arquivo e preencher a fila
void lerLivrosParaFila(Fila *fila, const char *arquivo) {
    FILE *file = fopen(arquivo, "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo %s\n", arquivo);
        exit(1);
    }

    char linha[100];
    while (fgets(linha, sizeof(linha), file) != NULL) {
        Livro livro;
        sscanf(linha, "%[^;];%s", livro.titulo, livro.autor);
        fila->livros[fila->tras] = livro;
        fila->tras = (fila->tras + 1) % MAX_SIZE;
    }

    fclose(file);
}

// Função para adicionar um livro na fila
void adicionarLivroNaFila(Fila *fila, Livro livro, const char *arquivo) {
    FILE *file = fopen(arquivo, "a");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo %s\n", arquivo);
        exit(1);
    }

    fprintf(file, "%s;%s\n", livro.titulo, livro.autor);
    fclose(file);

    fila->livros[fila->tras] = livro;
    fila->tras = (fila->tras + 1) % MAX_SIZE;
}

// Função para remover um livro da fila
Livro removerLivroDaFila(Fila *fila, const char *arquivo) {
    Livro livro = fila->livros[fila->frente];
    fila->frente = (fila->frente + 1) % MAX_SIZE;

    FILE *file = fopen(arquivo, "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo %s\n", arquivo);
        exit(1);
    }

    FILE *tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL) {
        printf("Erro ao criar arquivo temporário\n");
        exit(1);
    }

    char linha[100];
    while (fgets(linha, sizeof(linha), file) != NULL) {
        Livro tempLivro;
        sscanf(linha, "%[^;];%s", tempLivro.titulo, tempLivro.autor);
        if (strcmp(tempLivro.titulo, livro.titulo) != 0 || strcmp(tempLivro.autor, livro.autor) != 0) {
            fprintf(tempFile, "%s;%s\n", tempLivro.titulo, tempLivro.autor);
        }
    }

    fclose(file);
    fclose(tempFile);

    remove(arquivo);
    rename("temp.txt", arquivo);

    return livro;
}

// Função para inicializar uma pilha vazia
void inicializarPilha(Pilha *pilha) {
    pilha->topo = -1;
}

// Função para verificar se a pilha está vazia
int pilhaVazia(Pilha *pilha) {
    return pilha->topo == -1;
}

// Função para verificar se a pilha está cheia
int pilhaCheia(Pilha *pilha) {
    return pilha->topo == MAX_SIZE - 1;
}

// Função para adicionar um livro na pilha
void adicionarLivroNaPilha(Pilha *pilha, Livro livro, const char *arquivo) {
    FILE *file = fopen(arquivo, "a");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo %s\n", arquivo);
        exit(1);
    }

    fprintf(file, "%s;%s\n", livro.titulo, livro.autor);
    fclose(file);

    if (!pilhaCheia(pilha)) {
        pilha->topo++;
        pilha->livros[pilha->topo] = livro;
    }
}

// Função para remover um livro da pilha
Livro removerLivroDaPilha(Pilha *pilha) {
    Livro livro;
    if (!pilhaVazia(pilha)) {
        livro = pilha->livros[pilha->topo];
        pilha->topo--;
    }
    return livro;
}

// Função para exibir a fila de livros
void exibirFila(Fila *fila) {
    printf("Fila de Livros:\n");
    int i = fila->frente;
    while (i != fila->tras) {
        printf("%s (%s)\n", fila->livros[i].titulo, fila->livros[i].autor);
        i = (i + 1) % MAX_SIZE;
    }
}

// Função para exibir a pilha de livros
void exibirPilha(Pilha *pilha) {
    printf("Pilha de Livros:\n");
    for (int i = 0; i <= pilha->topo; i++) {
        printf("%s (%s)\n", pilha->livros[i].titulo, pilha->livros[i].autor);
    }
}

int main() {
    Fila fila;
    Pilha pilha;
    fila.frente = fila.tras = pilha.topo = -1;

    lerLivrosParaFila(&fila, "fila_de_livros.txt");
    lerLivrosParaFila(&pilha, "pilha_de_livros.txt");

    int escolha;
    Livro livro;

    do {
        printf("\nMenu:\n");
        printf("1. Exibir Fila de Livros\n");
        printf("2. Inserir Livro na Fila\n");
        printf("3. Inserir Livro na Pilha\n");
        printf("4. Remover Livro da Fila\n");
        printf("5. Remover Livro da Pilha\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &escolha);

        switch (escolha) {
            case 1:
                exibirFila(&fila);
                break;
            case 2:
                printf("Digite o título do livro: ");
                scanf(" %49[^\n]", livro.titulo);
                printf("Digite o autor do livro: ");
                scanf(" %49[^\n]", livro.autor);
                adicionarLivroNaFila(&fila, livro, "fila_de_livros.txt");
                break;
            case 3:
                printf("Digite o título do livro: ");
                scanf(" %49[^\n]", livro.titulo);
                printf("Digite o autor do livro: ");
                scanf(" %49[^\n]", livro.autor);
                adicionarLivroNaPilha(&pilha, livro, "pilha_de_livros.txt");
                break;
            case 4:
                if (!pilhaVazia(&fila)) {
                    livro = removerLivroDaFila(&fila, "fila_de_livros.txt");
                    printf("Livro removido da fila: %s (%s)\n", livro.titulo, livro.autor);
                } else {
                    printf("Fila de livros vazia.\n");
                }
                break;
            case 5:
                if (!pilhaVazia(&pilha)) {
                    livro = removerLivroDaPilha(&pilha);
                    printf("Livro removido da pilha: %s (%s)\n", livro.titulo, livro.autor);
                } else {
                    printf("Pilha de livros vazia.\n");
                }
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
        }
    } while (escolha != 0);

    return 0;
}
