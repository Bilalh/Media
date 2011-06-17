#include <stdio.h>
#include <stdlib.h>

// arrays passing

void arr_example(int **arr){
	*arr = malloc(sizeof(int)*10);
	*arr[0]=32323;
	printf("%d\n", *arr[0]);
}

int main (int argc, char const *argv[]) {
	int *arr = NULL;
	arr_example(&arr);
	printf("%d\n", arr[0]);
	return 0;
}