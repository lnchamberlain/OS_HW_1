#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>

int main(int argc, char **argv) {
	int fd;
	char *buff;
	bool passedFile = false;
	int n = (int) pow(2, 64) - 1;
	int lines;
	int i;
	int linesRead;
	
	buff = (char *) malloc(n);
	
	if (argc == 4 && strcmp(argv[2], "-n") == 0) {
		lines = atoi(argv[3]);
		passedFile = true;
	} else if (argc == 2) {
		lines = 10;
		passedFile = true;
	} else {
		lines = 10;
	}
	
	if (passedFile) {
		fd = open(argv[1], O_RDONLY);
		if (fd == -1) {
			printf("Failed to open file.\n");
			exit(1);
		}
		size_t endPoint = read(fd, buff, n);
		buff[endPoint] = '\0';
		close(fd);
		
		linesRead = 0;
		char* output = (char *) malloc(endPoint);
		for (i = 0; i < endPoint; i++) {
			if (buff[i] == '\0') {
				break;
			}
			if (linesRead >= lines) {
				break;
			}
			if (buff[i] == '\n') {
				linesRead++;
				
			}
				output[i] = buff[i];
		}
		
		printf("%s\n", output);
	} else {
		linesRead = 0;
		int incCounter = 0;
		for (i = 0; i < argc - 1; i++) {
			if (linesRead >= lines) {
				break;
			}
			int j;
			for (j = 0; argv[i+1][j] != '\0'; j++) {
				if (argv[i+1][j] == '\n') {
					linesRead++;
				}
				*buff = argv[i+1][j];
				buff++;
				incCounter++;
			}
			*buff = ' ';
			buff++;
			incCounter++;
		}
		*buff = '\0';
		while (incCounter > 0) {
			buff--;
			incCounter--;
		}
		
		printf("%s\n", buff);
	}
	
	return 0;
	
}