/*Assignment 1 Problem 3
Benjamin Good and Logan Chamberlain
CSCE A321: Operating Systems
February 18, 2021

Implementation of the tail linux function. Arguments may be passed in the form \
-n [num] [filename] or as [filename] -n [num]. Will read from standard input if\
 no filename is supplied.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


int calcExp(int a, int b);
int convertToInt(char *str);
int calcExp(int b, int e);
long getGap(char* cstr, int start);
int* shiftArray(int *arr, int x, int len);
char* copyChunk(char* source, char *dest, int start);

int main(int argc, char **argv) {
	int fd;
	char *fileName;
	int passedFile = 0;
	int n = 10000;
	int lines;
	int i;
	int linesRead;
	
	char buff[n];
	// Handle case:  number -n
	if (argc == 4 && strComp(argv[2], "-n") == 0) {
		lines = convertToInt(argv[3]);
		fileName = argv[1];
		passedFile = 1;
	}
	// Handle case: num -n <file>
	else if (argc == 4 && strComp(argv[1], "-n") == 0) {
		lines = convertToInt(argv[2]);
		fileName = argv[3];
		passedFile = 1;
	}
	 // Handle case: <file>
	else if (argc == 2) {
		lines = 10;
		passedFile = 1;
		fileName = argv[1];
	}
	// Handle case: read from standard input
	else {
		lines = 10;
	}
	if (passedFile) {
	  fd = open(fileName, O_RDONLY);
	  //If error opening file, exit
		if (fd == -1) {
                  fprintf(stderr, "Failed to open file: %s\n", strerror(errno));
                  return 1;
		}

	} else {
		fd = 0;
	}
	
	linesRead = 0;
	int readSuccess = read(fd, buff, n);
	if(readSuccess < 0){
	  //If error reading file, exit
                  fprintf(stderr, "Error reading file: %s\n", strerror(errno));
                  return 1;
	}

	int lastLines[lines+1];
	int lineIndex = 0;
	for (i = 0; buff[i] != '\0'; i++) {
		if (buff[i] == '\n') {
			linesRead++;
			if (lineIndex != lines + 1) {
				lastLines[lineIndex] = i;
				lineIndex++;
			}
			else {
				shiftArray(lastLines, i, lines + 1);
			}
		}
	}
	if (linesRead > lines) {
		long outSize = getGap(buff, lastLines[0]);
		char *output = (char *) malloc(outSize);
		if (output == NULL) {
		  fprintf(stderr, "Error allocating memory \n");
		  return 1;
		}
		copyChunk(buff, output, lastLines[0]);
			write(1, output, outSize);
		free(output);
	}
	else {
	  write(1, buff, sizeof(buff));
	}

	if (passedFile) {
		if(close(fd) < 0){
                  fprintf(stderr, "Failed to close file %s\n", strerror(errno));
                  return 1;
		}
	}

return 0;
}

//strComp implements strcmp, will return 1 if the given strings are equal and 0 if not.
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
//convertToInt implements atoi, returns the integer equivelent of a string number, will return an error message if not all numbers 0-9.
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

//calcExp implements math.pow functionality, reuturns a long value as exponents grow large quickly.
int calcExp(int b, int e) {
        int x = 1;
        for (int i = 0; i < e; i++) {
                x *= b;
        }
        return x;
}

long getGap(char* cstr, int start) {
        int i;
        long sizeGap = 0;
        for (i = start; cstr[i] != '\0'; i++) {
                sizeGap++;
        }
        return sizeGap;
}

int* shiftArray(int *arr, int x, int len) {
        int i;
        for (i = 1; i < len; i++) {
                arr[i-1] = arr[i];
        }
        arr[len-1] = x;
        return arr;
}

char* copyChunk(char *source, char *dest, int start) {
        int i;
        int destIndex = 0;
        for (i = start + 1; source[i] != '\0'; i++) {
                dest[destIndex] = source[i];
                destIndex++;
        }
        return dest;
}
