#include <stdio.h>
#include <regex.h>

#define MAX_LINE 4096

#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */

static int selected_color = 0;
static char *colors[] = {
  "-green", GREEN,
  "-black", BLACK,
  "-red", RED,
  "-yellow", YELLOW,
  "-blue", BLUE,
  "-magenta", MAGENTA,
  "-cyan", CYAN,
  "-white", WHITE,
  "-boldgreen", BOLDGREEN,
  "-boldblack", BOLDBLACK,
  "-boldred", BOLDRED,
  "-boldyellow", BOLDYELLOW,
  "-boldblue", BOLDBLUE,
  "-boldmagenta", BOLDMAGENTA,
  "-boldcyan", BOLDCYAN,
  "-boldwhite", BOLDWHITE,
  NULL
};

/*----------------------------------------------------------------------*/

int main(int argc, char *argv[]) {
  char buf[MAX_LINE];
  int has_re = 0;
  regex_t re;

  if (argc > 1) {
    if (argc > 2) {
      int idx = 0;
      while (colors[idx*2]) {
        if (!strcmp(colors[idx*2], argv[1])) {
          selected_color = idx;
          break;
        }
        idx++;
      }
      if (regcomp(&re, argv[2], REG_EXTENDED | REG_NEWLINE)) {
        printf("regcomp() failed!\n");
        return -1;
      }
    } else if (regcomp(&re, argv[1], REG_EXTENDED | REG_NEWLINE)) {
      printf("regcomp() failed!\n");
      return -1;
    }
    has_re = 1;
  } else {
    printf("Usage: %s [ -red | -blue | -cyan | -white | -black | "
           "-yellow | -magenta ] <regexp>\n", argv[0]);
    return -1;  
  }

  while (fgets(buf, MAX_LINE, stdin) == buf) {
    char *bbuf = buf;
    while (1) {
      if (has_re) {
        regmatch_t match[10];
        if (regexec(&re, bbuf, re.re_nsub + 1, match, 0)) {
          printf("%s", bbuf);
          break;
        } else {
          int i, idx;
          for (i=idx=0; i<1; i++) {
            if (match[0].rm_so < 0) {
              break;
            } else {
              printf("%.*s", 
                     (int)(match[i].rm_so-idx), 
                     bbuf+idx);
              printf( "%s%.*s" RESET, 
                      colors[selected_color*2+1],
                      (int)(match[i].rm_eo-match[i].rm_so), 
                      bbuf+(int)match[i].rm_so);
              idx = match[i].rm_eo;
              bbuf += idx;
            }
          }
        }
      }
      fflush(stdout);
    }
  }

  if (has_re) {
    regfree(&re);
  }

  return 0;
}
