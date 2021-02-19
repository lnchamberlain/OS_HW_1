#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>


long power(int base, int power);
int strComp(char str1[], char str2[]);
int strToInt(char str[]);

int main(int argc, char **argv) {
	int fd;
	char *buff;
	//Not sure if we can use booleans, we can use 0 to represent 0
	int passedFile = 0;
	//Not sure if math is up for grabs, implemented pow just to be safe
	//2 ^ 64 is too large for an int, we can use a long
	long n =  power(2, 64);
	int lines;
	int i;
	int linesRead;
	
	buff = (char *) malloc(n);
	
	if (argc == 4 && strComp(argv[2], "-n") == 0) {
		lines = strToInt(argv[3]);
		passedFile = 1;
	} else if (argc == 2) {
		lines = 10;
		passedFile = 1;
	} else {
		lines = 10;
	}
	
	if (passedFile) {
		fd = open(argv[1], O_RDONLY);
		if (fd == -1) {
		  fprintf(stderr, "Failed to open file.%s\n", strerr(errno));
		  return 1;
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
		free(buff);
		printf("%s\n", output);
		free(output);
		
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
		free(buff);
	}
	
	return 0;
	
}

long power(int base, int pow){
  int numIterations;
  long result = base;
  numIterations = 0;
  while(numIterations < pow- 1){
    result *= base;
    numIterations++;
  }
  return result;
}

int strComp(char str1[], char str2[]){
  int result, len1, len2, i;
  result = 0;
  len1 = 0;
  len2 = 0;
  i = 0;
  while(str1[i] != NULL){
    len1++;
    i++;
  }
  i = 0;
  while(str2[i] != NULL){
    len2++;
    i++;
  }
  if(len1 != len2){
    return 0;
  }
  for(i = 0; i < len1; i++){
    if(str1[i] != str2[i]){
      return 0;
    }
  }
  return 1;
}

int strToInt(char str[]){
  int result = 0;
  int i;
  for(i = 0; str[i] != NULL; i++){
    if(str[i] <= '0' || str[i] >= '9'){
      return -1;
    }
    result = result * 10 + (str[i] - '0');
  }
  return result;
}
