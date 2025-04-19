#include <stdio.h>
#include "funcao01.h"
#include "funcao02.h"
#include "funcao03.h"
#include "imprime.h"

int main(int argc, char *argv[]) {

    if (argc < 2) {
        return 1;
    }
    int n1, n2;
    int resultado;

    IMPRIME(argv[1]);
    printf("Digite o primeiro valor: ");
    scanf("%d", &n1);
    printf("Digite o segundo valor: ");
    scanf("%d", &n2);

   
    resultado =FUNCAO01(n1, n2);
    printf("Resultado da FUNCAO1 soma e: %d\n", resultado);
    resultado = FUNCAO02(n1, n2);
    printf("Resultado da FUNCAO2 subtracao e: %d\n", resultado);
    resultado = FUNCAO03(n1, n2);
    printf("Resultado da FUNCAO3 multiplicacao e: %d\n",resultado);
}
