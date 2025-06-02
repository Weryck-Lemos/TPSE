#include <stdio.h>
void bubble_sort(int *arr, int size);

int main() {
    int arr[] = {5, 2, 9, 1, 6};
    int size = sizeof(arr)/sizeof(arr[0]);

    bubble_sort(arr,size);
    for (int i=0; i<size; i++) {
        printf("%d ", arr[i]);
    }
    
}