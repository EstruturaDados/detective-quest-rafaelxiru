#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // Para a função tolower()

// Máximo de caracteres para o nome da sala
#define MAX_NOME 50

/**
 * @brief Estrutura que representa um cômodo (nó) da mansão.
 * * Cada sala contém um nome e ponteiros para os caminhos à esquerda e à direita.
 */
typedef struct Sala {
    char nome[MAX_NOME];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

/**
 * @brief Cria, de forma dinâmica, uma sala com nome.
 * * Aloca memória para uma nova Sala, define seu nome e inicializa
 * os ponteiros dos filhos como NULL.
 * * @param nome O nome da sala a ser criada.
 * @return Um ponteiro para a nova Sala criada ou NULL em caso de falha.
 */
Sala* criarSala(const char *nome) {
    Sala *novaSala = (Sala *)malloc(sizeof(Sala));
    if (novaSala == NULL) {
        printf("Erro de alocacao de memoria!\n");
        exit(EXIT_FAILURE);
    }
    
    // Copia o nome para a estrutura
    strncpy(novaSala->nome, nome, MAX_NOME - 1);
    novaSala->nome[MAX_NOME - 1] = '\0'; // Garantir terminação da string
    
    // Inicializa os caminhos como inexistentes
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    
    return novaSala;
}

/**
 * @brief Permite a navegação interativa do jogador pela árvore.
 * * Inicia a exploração a partir da sala inicial (Hall de entrada).
 * Continua até o jogador sair ou atingir uma sala sem caminhos.
 * * @param atual O ponteiro para o nó (sala) atual na exploração.
 */
void explorarSalas(Sala *atual) {
    char escolha;
    
    // Se a árvore estiver vazia, encerra
    if (atual == NULL) {
        printf("O mapa da mansao esta vazio.\n");
        return;
    }

    printf("\n--- Exploracao da Mansao - Detective Quest ---\n");
    
    // Loop principal de exploração
    while (atual != NULL) {
        printf("\nVoce esta no(a) **%s**.\n", atual->nome);

        // Verifica se é um nó-folha (sala sem caminhos)
        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("\nEste comodo nao tem mais caminhos a explorar. Fim da jornada!\n");
            break; // Sai do loop
        }
        
        printf("Escolha seu proximo caminho: ");
        
        // Monta a string de opcoes disponiveis
        printf("[");
        if (atual->esquerda != NULL) {
            printf("e (Esquerda) ");
        }
        if (atual->direita != NULL) {
            printf("d (Direita) ");
        }
        printf("s (Sair)]: ");
        
        // Leitura da escolha do usuário
        if (scanf(" %c", &escolha) != 1) {
            // Se falhar a leitura (raro), sai
            printf("Erro na leitura. Saindo...\n");
            break;
        }
        
        // Converte a escolha para minúscula
        escolha = tolower(escolha);
        
        // Processa a escolha
        switch (escolha) {
            case 'e':
                if (atual->esquerda != NULL) {
                    atual = atual->esquerda; // Move para a esquerda
                    printf("-> Seguindo para a Esquerda.\n");
                } else {
                    printf("Caminho a esquerda nao existe. Escolha novamente.\n");
                }
                break;
            case 'd':
                if (atual->direita != NULL) {
                    atual = atual->direita; // Move para a direita
                    printf("-> Seguindo para a Direita.\n");
                } else {
                    printf("Caminho a direita nao existe. Escolha novamente.\n");
                }
                break;
            case 's':
                printf("Saindo da mansao... Exploracao encerrada.\n");
                return; // Encerra a função
            default:
                printf("Opcao invalida. Digite 'e', 'd' ou 's'.\n");
                // Limpa o buffer de entrada para evitar loops infinitos em caso de entrada incorreta
                while (getchar() != '\n');
                break;
        }
    }
}

/**
 * @brief Libera a memória alocada para a árvore (pós-ordem).
 * * Evita vazamentos de memória (memory leaks) ao encerrar o programa.
 * * @param sala O ponteiro para a raiz da sub-árvore a ser liberada.
 */
void liberarMapa(Sala *sala) {
    if (sala == NULL) {
        return;
    }
    liberarMapa(sala->esquerda);
    liberarMapa(sala->direita);
    free(sala);
}


/**
 * @brief Monta o mapa inicial e dá início à exploração.
 * * Cria a estrutura fixa da árvore binária e chama a função de exploração.
 */
int main() {
    // ----------------------------------------------------
    // Montagem do Mapa da Mansão (Árvore Binária Fixa)
    // ----------------------------------------------------
    
    Sala *hallEntrada = criarSala("Hall de entrada"); // Raiz

    // Nível 1
    hallEntrada->esquerda = criarSala("Sala de Estar");
    hallEntrada->direita = criarSala("Biblioteca");

    // Nível 2 - Esquerda
    hallEntrada->esquerda->esquerda = criarSala("Cozinha");
    hallEntrada->esquerda->direita = criarSala("Jardim de Inverno");
    
    // Nível 2 - Direita
    hallEntrada->direita->direita = criarSala("Quarto Principal");
    
    // Nível 3 - Sub-árvore da Cozinha
    hallEntrada->esquerda->esquerda->esquerda = criarSala("Despensa"); // Nó-folha
    hallEntrada->esquerda->esquerda->direita = criarSala("Copa"); 
    
    // Nível 4 - Sub-árvore da Copa
    hallEntrada->esquerda->esquerda->direita->direita = criarSala("Varanda Secreta"); // Nó-folha
    
    // *Outros nós, como "Jardim de Inverno" e "Quarto Principal", são nós-folha por não terem filhos.
    
    
    
    // Inicia a exploração interativa a partir da raiz (Hall de entrada)
    explorarSalas(hallEntrada);
    
    // ----------------------------------------------------
    // Limpeza de Memória
    // ----------------------------------------------------
    liberarMapa(hallEntrada);
    
    return 0;
}