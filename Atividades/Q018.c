#include <stdio.h>

 typedef struct{
    int id;
    float temperatura;
    float humidity;
} SensorData;

void print_sensor(SensorData s){
    printf("%d\n %d\n %d\n", s.id, s.temperatura, s.humidity);
}