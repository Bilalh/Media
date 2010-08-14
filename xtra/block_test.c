#include <Block.h>
#include <dispatch/dispatch.h>
#include <stdio.h>
#include <unistd.h>
int main (int argc, char const *argv[]) {
    fprintf(stderr, "DO IT!\n");
    dispatch_apply(10, dispatch_get_global_queue(0, 0), ^(size_t i){
        fprintf(stderr, "I: %zu\n", i);
    });
    sleep(1);


	return 0;
}