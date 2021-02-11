#include <unistd.h>
#include <errno.h>
#include<string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

#include <stdio.h>
/*Would like to use this but can't figure out how..*/
struct phoneNumberEntry{
  char prefix[6];
  char area[26];
};
int returnDigit(char ch);
int checkValue(char mappedByLine[], int toCompare);
int binarySearchOfNanpaArray(char mappedByLine[166482][32], int startIndex, int endIndex, int target);
int getNumFromString(char str[]);


const size_t MAX_FILE_SIZE = 2147483648;
const char PATH_NAME[] = "nanpa";
const int LINES_IN_NANPA = 166482;
const int CHARACTERS_PER_LINE = 32;

int main(int argc, char *argv[]){
  int fd;
  int prefixValue;
  char mappedByLine[LINES_IN_NANPA][CHARACTERS_PER_LINE];
  
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
  
    /*Move from arguments into array*/
   if(argc == 2){
      /* prefix is stored in argv[1] */
      const char *prefix = argv[1];
      prefixValue = getNumFromString(prefix);
     
      /*Handle case: not 6 digit input*/
      if(prefixValue == 0){
	char not6DigitsErrorMessage[] = "Input not 6 digts\n";
	write(1, not6DigitsErrorMessage, sizeof(not6DigitsErrorMessage));
	return 1;
      }
      
      /*Handle case: not all digits*/
      if(prefixValue < 0){
	char notAllDigitsErrorMessage[] = "Input not comprised of digits 0 - 9\n";
        write(1, notAllDigitsErrorMessage, sizeof(notAllDigitsErrorMessage));
        return 1;
      }    

      /*Open Nanpa.txt */
     
      fd = open(PATH_NAME, O_RDONLY, 0);
      int offset = lseek(fd, 0, SEEK_CUR);
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
for(int i = 0; i < LINES_IN_NANPA; i++){
      for(int j = 0; j < CHARACTERS_PER_LINE; j++){
	mappedByLine[i][j] = mappedFile[k];
	
	k++;
     }
    }
   
 int lineNum;
 lineNum = binarySearchOfNanpaArray(mappedByLine, 1, LINES_IN_NANPA - 1, prefixValue);
 if(lineNum == -1){
   char notFoundInNanpa[] = "Prefix not found in Nanpa\n";
   write(1, notFoundInNanpa, sizeof(notFoundInNanpa));
   return 1;
 }
 else{
   char area[32];
   for(int i = 0; i < 32; i++){
     area[i] = NULL;
   }
   int len = 6;
   for(int i = 0; i < 20; i++){
     area[i] = mappedByLine[lineNum][i+6];
   }
   write(1, area, sizeof(area));
 }
 if(munmap(mappedFile, MAX_FILE_SIZE) < 0){
   char unmapFailedMessage[] = "Unmap failed \n";
   write(1, unmapFailedMessage, sizeof(unmapFailedMessage));
 }
 return 0;
}

/*checkValue takes in a pointer to a line in the mappedByLine array,
slices out the phone code at indicies 1-7 (ea ch entry begins with a 0
due to the null terminator), then casts to integers and compares the
inputted prefix. If\ the inputted prefix (denoted toCompare) is larger
than the area code, 1 is returned. If in the prefix matches the area
code, 0 is returned. If prefix is smaller than area code, -1 is
returned. */
int checkValue(char mappedByLine[], int toCompare){
  char slice[6];
  
  for(int i = 0; i < 6; i++){
    slice[i] = "1";
  }
  slice[6] = NULL;
   for(int i = 0; i < 6; i++){
    slice[i] = mappedByLine[i];
  }
  int num;
  /*printf("slice = %s\n", slice);*/
  num = getNumFromString(slice);
   if(num > toCompare){
    return -1;
  }
  if(num < toCompare){
    return 1;
  }
  if(num == toCompare){
    return 0;
  }

}
/*Implemenation of binary search on nanpa array, accepts the nanpa file stored in an array by line and uses helpe\
 function checkVaule to compare midpoint area code values until the target is found. Returns the line index of the
 target, -1 if not in the array. */
int binarySearchOfNanpaArray(char mappedByLine[LINES_IN_NANPA][CHARACTERS_PER_LINE], int startIndex, int endIndex, int target){
  int linesSearched = 0;
  int midpoint, result;
  while(startIndex <= endIndex){
    midpoint = startIndex + (endIndex - startIndex) / 2;
    /* printf("midpoint index = %d\n", midpoint);
      printf("Midpoint = %s\n", mappedByLine[midpoint]);*/
    result = checkValue(mappedByLine[midpoint], target);
    if(result == 0){
     
      return midpoint;
    }
    if(result == -1){
      endIndex = midpoint - 1;
    }
    if(result == 1){
      startIndex = midpoint + 1;
    }
    linesSearched++;
      }
    return -1;
}

/*getNumFromString accepts a string and then returns the integer value. Will return 0 if input is not of size 6 and will return a negative number if the input was not entirely digits 0-9.*/
int getNumFromString(char str[]){
  int integerValue = 0;

  /*Get the length of the string as sizeof(char*) will return the size of the p\
ointer, not the array*/
  int strLength = 0;
  while(str[strLength] != NULL){
    strLength++;
      }
    /*Handle case where input is not of the correct size*/
  if(strLength != 6){
     return 0;
  }
  else{
    integerValue += returnDigit(str[0]) * 100000;
    integerValue += returnDigit(str[1]) * 10000;
    integerValue += returnDigit(str[2]) * 1000;
    integerValue += returnDigit(str[3]) * 100;
    integerValue += returnDigit(str[4]) * 10;
    integerValue += returnDigit(str[5]);

  }
     return integerValue;
}

/*returnDigit is a helper function to getNumFromStr that accepts a character and returns the digit value. In the case where the character is not a digit 0-9, a negative number greater than the prefix is returned so that if the integerValue is negative, the input was not exclusively digits*/
int returnDigit(char ch){
  int digitVal;
  int chVal = (int) ch;
  switch(chVal){
  case 48:
    digitVal = 0;
    break;
  case 49:
    digitVal = 1;
    break;
  case 50:
    digitVal = 2;
    break;
  case 51:
    digitVal = 3;
    break;
  case 52:
    digitVal = 4;
    break;
  case 53:
    digitVal = 5;
    break;
  case 54:
    digitVal = 6;
    break;
  case 55:
    digitVal = 7;
    break;
  case 56:
    digitVal = 8;
    break;
  case 57:
    digitVal = 9;
    break;
  default:
    /*If any characters are not digits, returning a negative value larger than the most significant digit will ensure than the return value of get numFromStr is negative if the input is not exclusively*/
    digitVal = -1000000;

  }
  return  digitVal;

}

