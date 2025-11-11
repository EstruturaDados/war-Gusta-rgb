#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

/* Função auxiliar para remover '\n' do fgets */
void trim_newline(char *s) {
    size_t len = strlen(s);
    if (len > 0 && s[len-1] == '\n') s[len-1] = '\0';
}

/* Cadastro dinâmico de territórios */
Territorio* cadastrarTerritorios(int n) {
    Territorio *mapa = (Territorio*) calloc(n, sizeof(Territorio));
    if (!mapa) {
        printf("Erro de alocação.\n");
        exit(EXIT_FAILURE);
    }

    int c;
    while ((c = getchar()) != '\n' && c != EOF) {} // limpar buffer

    for (int i = 0; i < n; i++) {
        printf("\n--- Cadastro do Território %d ---\n", i+1);
        printf("Nome do território: ");
        fgets(mapa[i].nome, sizeof(mapa[i].nome), stdin);
        trim_newline(mapa[i].nome);

        printf("Cor do exército (dono): ");
        fgets(mapa[i].cor, sizeof(mapa[i].cor), stdin);
        trim_newline(mapa[i].cor);

        while (1) {
            printf("Quantidade de tropas: ");
            if (scanf("%d", &mapa[i].tropas) != 1 || mapa[i].tropas < 0) {
                printf("Valor inválido. Tente novamente.\n");
                while ((c = getchar()) != '\n' && c != EOF) {}
                continue;
            }
            while ((c = getchar()) != '\n' && c != EOF) {}
            break;
        }
    }

    return mapa;
}

/* Exibe o mapa */
void exibirMapa(Territorio *mapa, int n) {
    printf("\n===== MAPA ATUAL =====\n");
    for (int i = 0; i < n; i++) {
        printf("[%d] Nome: %s | Dono: %s | Tropas: %d\n", 
            i, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
    printf("=====================\n");
}

/* Função de ataque entre territórios */
void atacar(Territorio *atacante, Territorio *defensor) {
    if (!atacante || !defensor) return;
    if (strcmp(atacante->cor, defensor->cor) == 0) {
        printf("Ataque inválido: mesmos donos.\n");
        return;
    }
    if (atacante->tropas < 2) {
        printf("Ataque inválido: atacante precisa ter >=2 tropas.\n");
        return;
    }

    int dadoAtq = (rand() % 6) + 1;
    int dadoDef = (rand() % 6) + 1;
    printf("%s (atacante) rolou %d | %s (defensor) rolou %d\n",
        atacante->nome, dadoAtq, defensor->nome, dadoDef);

    if (dadoAtq > dadoDef) {
        int transfer = atacante->tropas / 2;
        if (transfer < 1) transfer = 1;

        printf("Atacante venceu! Transferindo %d tropas e assumindo o território.\n", transfer);
        strncpy(defensor->cor, atacante->cor, sizeof(defensor->cor)-1);
        defensor->cor[sizeof(defensor->cor)-1] = '\0';
        defensor->tropas = transfer;
        atacante->tropas -= transfer;
    } else {
        printf("Defensor resistiu. Atacante perde 1 tropa.\n");
        atacante->tropas -= 1;
        if (atacante->tropas < 0) atacante->tropas = 0;
    }
}

/* Atribuir missão aleatória ao jogador */
void atribuirMissao(char **destino, char *missoes[], int totalMissoes) {
    int idx = rand() % totalMissoes;
    size_t len = strlen(missoes[idx]) + 1;
    *destino = (char*) malloc(len * sizeof(char));
    if (!*destino) {
        printf("Erro de memória.\n");
        exit(EXIT_FAILURE);
    }
    strcpy(*destino, missoes[idx]);
}

/* Verificar missão simples: aqui, ex.: conquistar 3 territórios da mesma cor */
int verificarMissao(char *missao, Territorio *mapa, int tamanho) {
    // Exemplo simples: "Conquistar 3 territórios da mesma cor"
    if (strcmp(missao, "Conquistar 3 territórios da mesma cor") == 0) {
        for (int i = 0; i < tamanho; i++) {
            int corCount = 1;
            for (int j = i+1; j < tamanho; j++) {
                if (strcmp(mapa[i].cor, mapa[j].cor) == 0)
                    corCount++;
            }
            if (corCount >= 3) return 1; // missão cumprida
        }
        return 0;
    }
    // Outras missões podem ser implementadas aqui
    return 0;
}

/* Liberar memória */
void liberarMemoria(Territorio *mapa, int n, char *missao) {
    free(mapa);
    free(missao);
}

int main() {
    srand((unsigned int)time(NULL));

    int n;
    printf("Número de territórios: ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("Valor inválido.\n");
        return 1;
    }

    Territorio *mapa = cadastrarTerritorios(n);
    exibirMapa(mapa, n);

    // Missões
    char *missoes[] = {
        "Conquistar 3 territórios da mesma cor",
        "Eliminar todas as tropas da cor vermelha",
        "Possuir ao menos 5 tropas em um território",
        "Conquistar dois territórios consecutivos no vetor",
        "Ter o maior número de tropas entre todos"
    };
    int totalMissoes = 5;

    char *missaoJogador = NULL;
    atribuirMissao(&missaoJogador, missoes, totalMissoes);
    printf("\nSua missão (revelada apenas uma vez): %s\n", missaoJogador);

    int opc;
    while (1) {
        printf("\nMenu:\n1 - Ataque\n2 - Mostrar mapa\n0 - Sair\nOpção: ");
        if (scanf("%d", &opc) != 1) {
            int c; while ((c = getchar()) != '\n' && c != EOF) {}
            continue;
        }
        if (opc == 0) break;
        else if (opc == 2) exibirMapa(mapa, n);
        else if (opc == 1) {
            exibirMapa(mapa, n);
            int atk, def;
            printf("Índice do atacante: "); scanf("%d", &atk);
            printf("Índice do defensor: "); scanf("%d", &def);
            if (atk>=0 && atk<n && def>=0 && def<n && atk!=def)
                atacar(&mapa[atk], &mapa[def]);
            else printf("Índices inválidos.\n");

            // Verificar missão após cada ataque
            if (verificarMissao(missaoJogador, mapa, n)) {
                printf("\nParabéns! Você cumpriu sua missão: %s\n", missaoJogador);
                break;
            }
        }
    }

    liberarMemoria(mapa, n, missaoJogador);
    printf("Fim do jogo.\n");
    return 0;
}
