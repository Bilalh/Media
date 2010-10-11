
#include <fts.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
int main(int argc, char **argv) {
	int f_i=0;
	int f_len = 10;
	char **files =malloc(sizeof(char*) *f_len);
	
    char *dot[] = {".", 0};
    char **paths = argc > 1 ? argv + 1 : dot;

    FTS *tree = fts_open(paths, FTS_NOCHDIR, 0);
    if (!tree) {
        perror("fts_open");
        return 1;
    }

    FTSENT *node;
    while ((node = fts_read(tree))) {
        if (node->fts_level > 0 && node->fts_name[0] == '.')
            fts_set(tree, node, FTS_SKIP);
        else if (node->fts_info & FTS_F) {
            // printf("got file named %s at depth %d, "
            //     "accessible via %s from the current dir "
            //     "or via %s from the original start dir\n\n",
            //     node->fts_name, node->fts_level,
            //     node->fts_accpath, node->fts_path);
            /* if fts_open is not given FTS_NOCHDIR,
             * fts may change the program's current working directory */
				files[f_i++] = strdup(node->fts_path);
				if (f_i == f_len){
					f_len *= 2;
					files = realloc(files, sizeof(char*)* f_len);
				}
        }
    }
    if (errno) {
        perror("fts_read");
        return 1;
    }

    if (fts_close(tree)) {
        perror("fts_close");
        return 1;
    }
	
	int i=0;
	printf("%d %d\n",f_len, f_i );
	for(i = 0; i < f_i; ++i){
		printf("%d %s\n",i, files[i]);
	}
	
    return 0;
}
