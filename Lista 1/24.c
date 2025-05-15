#include <stdio.h>
#include <stdint.h>

int main() {
    uint32_t valor = 0xCAFEBABE;

    printf("Valor: 0x%08X\n", valor);

    for (int i=7; i>=0;i--){
        uint8_t bloco = (valor >> (i*4)) &0xF;
        printf("Bloco %d: %X (%d)\n", 7 - i, bloco, bloco);
    }
}
