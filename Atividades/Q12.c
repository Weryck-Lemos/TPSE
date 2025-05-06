#include <stdio.h>

typedef struct{
    unsigned char enabled;
    unsigned char direction;
    unsigned char level;
} GPIO;

typedef GPIO GPIO_t;

void print_gpio(GPIO_t g){
    printf("%u", g.enabled);
    printf("%u", g.direction);
    printf("%u", g.level);
}