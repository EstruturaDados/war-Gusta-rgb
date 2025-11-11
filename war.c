#include <stdio.h>
#include <string.h>

/*
  Estrutura: Territorio
  Armazena dados relacionados a um território, como:
  - nome: nome do território
  - cor: cor do exército que o controla
  - tropas: quantidade de tropas disponíveis
 */
struct Territorio {
    char nome[30];
    char cor[10];
    int tropas;
};

int main() {
    // Declaração de um vetor com capacidade para 5 territórios
    struct Territorio territorios[5];

    // Loop para cadastrar os 5 territórios
    for (int i = 0; i < 5; i++) {
        printf("\n--- Cadastro do Território %d ---\n", i + 1);

        // Leitura do nome do território
        printf("Digite o nome do território: ");
        scanf(" %[^\n]", territorios[i].nome); // lê até o ENTER (permite espaços)

        // Leitura da cor do exército
        printf("Digite a cor do exército: ");
        scanf(" %[^\n]", territorios[i].cor);

        // Leitura da quantidade de tropas
        printf("Digite a quantidade de tropas: ");
        scanf("%d", &territorios[i].tropas);
    }

    // Exibição dos dados cadastrados
    printf("\n LISTA DE TERRITÓRIOS CADASTRADOS\n");
    for (int i = 0; i < 5; i++) {
        printf("\nTerritório %d\n", i + 1);
        printf("Nome: %s\n", territorios[i].nome);
        printf("Cor do exército: %s\n", territorios[i].cor);
        printf("Tropas: %d\n", territorios[i].tropas);
    }

    printf("\nCadastro concluído!\n");

    return 0;
}
