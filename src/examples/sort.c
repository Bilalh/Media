#include <stdlib.h>

typedef struct {
	char *full;
	long   num;
} Ep;

static inline int longcmp(long a, long b) {
	return  a > b ? 1 : (a < b ? -1 : 0) ;
}

// Returns  1,0 or -1 if a is >=,= or <= then b.
int longpcmp( long *const*a, long *const*b) {
	return  **a > **b ? 1 : (**a < **b ? -1 : 0) ;
}

int mainc (int argc, char const *argv[]) {

	long **arr = malloc(sizeof(size_t)*3);
	arr[0] =  malloc(sizeof(size_t));
	*arr[0] = 12;
	arr[1] =  malloc(sizeof(size_t));
	*arr[1] = 6;
	arr[2] =  malloc(sizeof(size_t));
	*arr[2] = 7;
	
	qsort_b(arr, 3, sizeof(long),
		^(const void *a, const void *b){
			printf("%ld %ld\n", **((long**)a), **((long**)b) );
			return longpcmp( a, b );
		}
	);	
	
	int i;
	for(i = 0; i < 3; ++i){
		printf("%d\n", *arr[i]);
	}
	
	return 0;
}

int main2 (int argc, char const *argv[]) {

	int arr[] = {4,5,3};
	
	qsort_b(arr, 3, sizeof(long),
		^(const void *a, const void *b){
			printf("%ld %ld res: %d\n", *((long*)a), *((long*)b), longpcmp( a, b ) );
			return longpcmp( a, b );
		}
	);	
	return 0;
	
}

int main (int argc, char const *argv[]) {
	 
	void **arr = malloc(sizeof(size_t)*3);
	
	Ep e1 = {NULL,1};
	Ep e2 = {NULL,4};
	Ep e3 = {NULL,7};
	
	Ep *e = malloc(sizeof(Ep));
	*e = e1;
	arr[0] = e;
	
	e = malloc(sizeof(Ep));
	*e = e2;
	arr[1] = e;
	
	e = malloc(sizeof(Ep));
	*e = e3;
	arr[2] = e;
	
	int index = 3;
	
	qsort_b(arr, index, sizeof(size_t),
		^(const void *a, const void *b){
			Ep *ea = *((Ep**)a), *eb = *((Ep**)b);
			printf("%ld %ld res: %d\n", ea->num, eb->num, longcmp( ea->num, eb->num ) );
			return longcmp( ea->num, eb->num );
		}
	);
	
	return 0;
}