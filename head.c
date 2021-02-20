/*Assignment 1 Problem 3
Benjamin Good and Logan Chamberlain
CSCE A321: Operating Systems
February 18, 2021
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

long calcExp(int b, int e);
int convertToInt(char *str);
int strComp(char str1[], char str2[]);


int main(int argc, char **argv) {
	int fd;
	char *buff;
	char *fileName;
	int passedFile = 0;
	long n = (long) calcExp(2, 64) - 1;
	int lines;
	int i;
	int linesRead;
	printf("n is %ld \n", n);
	buff = (char *) malloc(n);
	if (buff == NULL) {
	  fprintf(stderr, "Error allocating memory \n");
	  return 1;
	}
	//Handle case:  number -n
	if (argc == 4 && strcmp(argv[2], "-n") == 0) {
		lines = convertToInt(argv[3]);
		fileName = argv[1];
		passedFile = 1;
	}
	//Handle case: -n number 
	else if (argc == 4 && strcmp(argv[1], "-n") == 0) {
		lines = convertToInt(argv[2]);
		fileName = argv[3];
		passedFile = 1;
	}
	//Handle case: no number specified but read from file
	else if (argc == 2) {
		lines = 10;
		passedFile = 1;
		fileName = argv[1];
	}
	// Handle case: read from standard input for 10 lines
	else {
		lines = 10;
	}
	
	size_t endPoint;
	if (passedFile) {
		fd = open(fileName, O_RDONLY);
		if (fd == -1) {
		  fprintf(stderr, "Failed to open file: %s\n", strerror(errno));
		  return 1;
		}
		endPoint = read(fd, buff, n);
		//If error reading file, exit
		if(endPoint == -1){
		  fprintf(stderr, "Error reading file: %s\n", strerror(errno));
		  return 1;
		}
		buff[endPoint] = '\0';
		if(close(fd) < 0){
		  fprintf(stderr, "Failed to close file %s\n", strerror(errno));
		  return 1;
		}
		
	} else {
	  //Read from standard input
		endPoint = read(0, buff, n);
		//If error reading file, exit
                if(endPoint == -1){
                  fprintf(stderr, "Error reading file: %s\n", strerror(errno));
                  return 1;
                }

		buff[endPoint] = '\0';
	}
	
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
	write(1, output, sizeof(output));
	free(output);
	
	return 0;
	
}
long calcExp(int b, int e) {
     int x = 1;
        for (int i = 0; i < e; i++) {
                x *= b;
        }
        return x;
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
  
int convertToInt(char *str) {
        int i;
        char c;
        int digit;
        int res = 0;
        int incCounter = 0;
        for (i = 0; str[i] != '\0'; i++) {
                incCounter++;
        }
        i = 0;
        while (incCounter > 0) {
                c = str[i];
                switch (c) {
                        case '0':
                                digit = 0;
                                break;
                        case '1':
                                digit = 1;
                                break;
                        case '2':
                                digit = 2;
                                break;
                        case '3':
                                digit = 3;
                                break;
                        case '4':
                                digit = 4;
                                break;
                        case '5':
                                digit = 5;
                                break;
                        case '6':
                                digit = 6;
                                break;
                        case '7':
                                digit = 7;
                                break;
                        case '8':
                                digit = 8;
                                break;
                        case '9':
                                digit = 9;
                                break;
                        default:
                                printf("An issue occurred while converting digits.");
                }
                if (incCounter == 1) {
                    res += digit;
                } else {
                    res += calcExp(10, incCounter-1) * digit;
                }
                incCounter--;
                i++;
        }
        return res;
}
