#include <stdio.h>

void printHeader() {
    printf("=====================================\n");
    printf("||                                 ||\n");
    printf("||           CALCULADORA C         ||\n");
    printf("||                                 ||\n");
    printf("=====================================\n");
}

void printMenu() {
    printf("\nEscolha uma operação:\n");
    printf(" [1] Soma (+)\n");
    printf(" [2] Subtração (-)\n");
    printf(" [3] Multiplicação (*)\n");
    printf(" [4] Divisão (/)\n");
    printf(" [0] Sair\n");
    printf(">> ");
}

int main() {
    int opcao;
    float num1, num2, resultado;

    do {
        printHeader();
        printMenu();
        scanf("%d", &opcao);

        if (opcao == 0) {
            printf("\nEncerrando a calculadora...\n");
            break;
        }

        if (opcao < 1 || opcao > 4) {
            printf("\nOpção inválida! Tente novamente.\n");
            continue;
        }

        printf("\nDigite o primeiro número: ");
        scanf("%f", &num1);
        printf("Digite o segundo número: ");
        scanf("%f", &num2);

        switch(opcao) {
            case 1:
                resultado = num1 + num2;
                printf("\nResultado: %.2f + %.2f = %.2f\n", num1, num2, resultado);
                break;
            case 2:
                resultado = num1 - num2;
                printf("\nResultado: %.2f - %.2f = %.2f\n", num1, num2, resultado);
                break;
            case 3:
                resultado = num1 * num2;
                printf("\nResultado: %.2f * %.2f = %.2f\n", num1, num2, resultado);
                break;
            case 4:
                if (num2 == 0) {
                    printf("\nErro: Divisão por zero!\n");
                } else {
                    resultado = num1 / num2;
                    printf("\nResultado: %.2f / %.2f = %.2f\n", num1, num2, resultado);
                }
                break;
        }

        printf("\nPressione ENTER para continuar...\n");
        getchar(); // Captura o ENTER após scanf
        getchar(); // Espera ENTER
        system("clear || cls"); // Limpa a tela (Linux ou Windows)

    } while (1);

    return 0;
}
