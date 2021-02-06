#include <unistd.h>
#include <errno.h>
#include<string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

#include <stdio.h>

/*Each user will have to find the nanpa.txt file and change the path name*/

const size_t MAX_FILE_SIZE = 2147483648;
const char PATH_NAME[] = "nanpa";
const int LINES_IN_NANPA = 166482;

/*checkValue takes in a pointer to a line in the mappedByLine array, slices out the phone code at indicies 1-7 (each entry begins with a 0 due to the null terminator), then casts to integers and compares the inputted prefix. If the inputted prefix (denoted toCompare) is larger than the area code, 1 is returned. If in the prefix matches the area code, 0 is returned. If prefix is smaller than area code, -1 is returned. */
int checkValue(char mappedByLine[], int toCompare){
  char slice[6];
  for(int i = 1; i < 7; i ++){
    slice[i-1] = mappedByLine[i];
  }
  int num = (int) slice;
  if(num > toCompareNum){
    return -1;
  }
  if(num < toCompareNum){
    return 1;
  }
  if(num == toCompareNum){
    return 0;
  }
}
/*Implemenation of binary search on nanpa array, accepts the nanpa file stored in an array by line and uses helper function checkVaule to compare midpoint area code values until the target is found. Returns the line index of the target, -1 if not in the array. */
int binarySearchOfNanpaArray(char mappedByLine[][], int startIndex, int endIndex, int target){
  while(start_index <= endIndex){
    int midPoint = startIndex + (endIndex - startIndex) / 2;
    int result = checkValue(mappedByLine[m], target);
    if(result == 0){
      return midpoint;
    }
    if(result == -1){
      endIndex = midPoint - startIndex;
    }
    if(result == 1){
      startIndex = midpoint + startIndex;
    }
    return -1;
  }




int main(int argc, char *argv[]){
  int fd;
  char mappedByLine[166482][32];
  /* prefix is an array that will hold the given prefix, declare and clear prefix
  for(i = 0; i < 6; i++){
    prefix[i] = " ";
  }
  
  char words[] = "Hello there this is using write";

  write(1, words, sizeof(words));
  */
  /*Handle case too many arguments passed*/
  if(argc > 2){
    char tooManyArgumentsErrorMessage[] = "Enter 1 prefix\n";
    write(1, tooManyArgumentsErrorMessage, sizeof(tooManyArgumentsErrorMessage));
    return 1;
  }
  /*Handle case no argument passed*/
  if( argc < 2){
    char tooFewArgumentsErrorMessage[] = "No prefix entered\n";
    write(1, tooFewArgumentsErrorMessage, sizeof(tooFewArgumentsErrorMessage));
    return 1;
  }
  /*  int lenOfInput = 0;
  int i = 0;
  while(argv[1][i] != "\0"){
    lenOfInput++;
    i++;
  }
  */
  
    /*Move from arguments into array*/
    if(argc == 2){
      /* prefix is stored in argv[1] */
      const char *prefix = argv[1];
      /*
      if(sizeof(argv[1]) != 6){
	char not6DigitsErrorMessage[] = "Input not 6 digts\n";
	write(1, not6DigitsErrorMessage, sizeof(not6DigitsErrorMessage));
	return 1;
      }
      */
      /*(  int isAllDigits = 1;
      int i;
      for(i = 0; i < lenOfInput; i++){
	if((argv[1][i] == 0) || (argv[1][i] == 1) || (argv[1][i] == 2) || (argv[1][i] == 3) || (argv[1][i] == 4) || (argv[1][i] == 5) || (argv[1][i] == 6) || (argv[1][i] == 7)  || (argv[1][i] == 8) || (argv[1][i] == 9)){
	  isAllDigits = 0;
	}else{
	  isAllDigits = 1;
	  break;
	}
	write(1, &i, sizeof(i));
      }
      if(isAllDigits == 0){
	char notAllDigitsErrorMessage[] = "Input not comprised of digits 0 - 9\n";
        write(1, notAllDigitsErrorMessage, sizeof(notAllDigitsErrorMessage));
        return 1;
      }
    }

    
      */

      /*Open Nanpa.txt */
      write(1, PATH_NAME, sizeof(PATH_NAME));
      fd = open(PATH_NAME, O_RDONLY, 0);
      
      if(fd == -1){
	char fileOpenFailedMessage[] = "File failed to open\n";
	write(1, fileOpenFailedMessage, sizeof(fileOpenFailedMessage));
	return 1;
      }
      if(fd >= 0){
	char fileOpenSuccess[] = "File opened successfully\n";
	write(1, fileOpenSuccess, sizeof(fileOpenSuccess));
      }
    }

    /*Map to memory using mmap*/
    char *mappedFile = mmap(NULL, MAX_FILE_SIZE, PROT_READ, MAP_PRIVATE, fd, 0);
    if(mappedFile == MAP_FAILED){
      char mappingFailureMessage[] = "Mapping failed\n";
      write(1, mappingFailureMessage, sizeof(mappingFailureMessage));
      return 1;
    }
    else{
      char mappingSuccessMessage[] = "File Mapped Successfully\n";
      write(1, mappingSuccessMessage, sizeof(mappingSuccessMessage));

    }


    /*As determined in an earlier section of the HW, nanpa is 166482 lines long and each line is 32 characters long. To break up the array mapped to memory into strings by line, a nested loop can be used to over every character in mappedFile and add them in increments of 32 to mapped by line*/
    int i, j, k;
    k = 0;
   
    write(1, &mappedFile[1], sizeof(mappedFile[0]));
    for(i = 0; i < 166482; i++){
      for(j = 0; j < 32; j++){
	mappedByLine[i][j] = mappedFile[k];
	
	k++;
      }
    }
    for(i = 0; i < sizeof(mappedByLine[166481]); i++){
      
      write(1, &mappedByLine[166481][i], sizeof(mappedByLine[166481][i]));

    }
      return 0;
}
