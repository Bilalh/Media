#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>

void errMsg( const char* pMsg ) {
	fflush( stdout );
	fprintf(stderr, "%d %s", getpid(), pMsg );
	fflush( stderr );
}


void readFromPipe( int fd ) {
	FILE *stream;
	int ch;
	errMsg( "reading\n" );
	if ( (stream = fdopen( fd, "r" )) == NULL ) {
		perror( "fdopen() r" );
		exit(255);
	}
	while ( (ch = getc( stream )) != EOF )
		putc( ch, stdout );
	fflush( stdout );
	fclose( stream );
	errMsg( "reading done\n" );
}


int main() {
	pid_t nPid;
	int pipeto[2];		/* pipe to feed the exec'ed program input */
	int pipefrom[2];	/* pipe to get the exec'ed program output */

	if ( pipe( pipeto ) != 0 ) {
		perror( "pipe() to" );
		exit(255);
	}
	if ( pipe( pipefrom ) != 0 ) {
		perror( "pipe() from" );
		exit(255);
	}

	nPid = fork();
	if ( nPid < 0 ) {
		perror( "fork() 1" );
		exit(255);
	} else if ( nPid == 0 ) { // child
		errMsg("Child\n");
		/* dup pipe read/write to stdin/stdout */
		dup2( pipeto[0], STDIN_FILENO );
		dup2( pipefrom[1], STDOUT_FILENO  );
		/* close unnecessary pipe descriptors for a clean environment */
		close( pipeto[0] );
		close( pipeto[1] );
		close( pipefrom[0] );
		close( pipefrom[1] );
		//mplayer a file
		execlp( "/Users/bilalh/bin/mplayer_osx/mplayer",
				"mplayer",
				"/Users/bilalh/Movies/20100224_001.mp4"
				, NULL );
		perror( "execlp()" );
		exit(255);
	} else {	// parent
		errMsg("Parent start\n");
		pid_t nPid2;

		/* Close unused pipe ends. This is especially important for the
		 * pipefrom[1] write descriptor, otherwise readFromPipe will never
		 * get an EOF. */
		close( pipeto[0] );
		close( pipefrom[1] );

		nPid2 = fork();
		if ( nPid2 < 0 ) {
			perror( "fork() 2" );
			exit(255);
		} else if ( nPid2 == 0 ) { // child
			errMsg("Parent-child start\n");
			/* Close pipe write descriptor, or we will never know when the
			 * writer process closes its end of the pipe and stops feeding the
			 * exec'ed program. */
			close( pipeto[1] );
			readFromPipe( pipefrom[0] );
		} else {
			errMsg("Parent-Parent start\n");
			int status;

			/* close unused pipe end */
			close( pipefrom[0] );
			// writeToPipe( pipeto[1], "p" );

			system("stty raw");
			FILE *stream;
			errMsg( "writing\n" );
			if ( (stream = fdopen( pipeto[1], "w" )) == NULL ) {
				perror( "fdopen() w" );
				exit(255);
			}
			fprintf(stream, "%s", "p");
			fflush(stream);


			bool line = false;
			while(1) {

				if (!line) {
					char c = fgetc(stdin);
					if (c == 'c') {
						system("stty cooked");
						line = true;
						continue;
					}
					fprintf(stream, "%c", c);
					fflush(stream);
				} else {
					char *s;
					s = readline(">");
					if (strcmp(s, "c") == 0) {
						system("stty raw");
						line = false;
						continue;
					}
					fprintf(stream, "%s", s);
					fflush(stream);
				}


				usleep(1);
			}
			system("stty cooked");

			fclose( stream );
			errMsg( "writing done\n" );

			errMsg( "waiting for readFromPipe\n" );
			if ( -1 == waitpid( nPid2, &status, 0 ) ) {
				perror( "waitpid()" );
				exit(255);
			}
			if ( WIFEXITED( status ) )
				fprintf( stderr, "%d, child %d exit status: %d\n", getpid(), nPid2,	 WEXITSTATUS( status ) );
			if ( WIFSIGNALED( status ) )
				fprintf( stderr, "%d, child %d signal status: %d\n", getpid(), nPid2, WTERMSIG( status ) );
			fflush( stderr );
		}
	}
	fflush( stdout );
	return( 0 );
}
