/*Assignment 1 Problem 4
Logan Chamberlain and Benjamin Good
CSCE A321: Operating Systems
February 16, 2021

findlocationfast.c is an implementation of a binary search on list of phone number prefixes contained in the file 'nanpa.txt'. Only level two system calls are utilized save for the use of fprintf to print strerr(errno). This algorithm improves upon the search script written for problem 2 of this assignment. File format specified in assignment description. */

#include <unistd.h>
#include <errno.h>
#include<string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdio.h>

int returnDigit(char ch);
int checkValue(char mappedFile[], int toCompare, int offsetIndex);
int binarySearchOfNanpaArray(char mappedFile[], int startIndex, int endIndex, int numCharsPerLine, int target);
int getNumFromString(char str[]);

const char PATH_NAME[] = "nanpa";

int main(int argc, char *argv[]){
  int fd, prefixValue, i, offset;
  const char* prefix;
  size_t fileLength;
  char *mappedFile;
  int numCharsPerLine, numEntries, entryNumber;
  int startIndexOfCity, endIndexOfCity;
  char currChar;
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
      prefix = argv[1];
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
      offset = lseek(fd, 0, SEEK_CUR);
      if(fd == -1){
	fprintf(stderr, "File failed to open: %s\n", strerror(errno));
	return 1;
      }
      if(fd >= 0){
	char fileOpenSuccess[] = "File opened successfully\n";
	write(1, fileOpenSuccess, sizeof(fileOpenSuccess));
      }
    }
  fileLength = lseek(fd, (size_t)0, SEEK_END);
  //If file is not searchable, return 1 and close fd
  if(fileLength == -1){
    fprintf(stderr, "File not searchable: %s\n", strerror(errno));
    if( close(fd) < 0){
      fprintf(stderr, "Error closing file %s\n", strerror(errno));
      return 1;
    }
    else{
      char closeSuccess[] = "File closed Successfully \n";
      write(1, closeSuccess, sizeof(closeSuccess));
    }
    return 1;
  }

  //Map file to memory using mmap
  mappedFile = mmap(NULL, fileLength, PROT_READ, MAP_PRIVATE, fd, 0);

  //If mapping failed, unmap and close fd 
    if(mappedFile == MAP_FAILED){
      fprintf(stderr, "Mapping failed: %s\n", strerror(errno));

      //close fd
      if( close(fd) < 0){
	fprintf(stderr, "Error closing file %s\n", strerror(errno));
	return 1;
      }
      else{
	char closeSuccess[] = "File closed Successfully \n";
	write(1, closeSuccess, sizeof(closeSuccess));
      }
      //unmap memory
      if(munmap(mappedFile, fileLength) < 0){
	fprintf(stderr, "Error unmapping file: %s\n", strerror(errno));
      }
      else{
	char unmapSuccess[] = "File unmapped succesfully \n";
	write(1, unmapSuccess, sizeof(unmapSuccess));
      }     
      return 1;
    }
      
    else{
      char mappingSuccessMessage[] = "File mapped successfully\n";
      write(1, mappingSuccessMessage, sizeof(mappingSuccessMessage));

    }

    //the number of characters per line is specified in the file format, could aquired through 'while mappedFil[i] != '\n' i++' but iterating over entire array is not O(lgn)
    numCharsPerLine = 32;
    numEntries = (int)fileLength / numCharsPerLine;   
    entryNumber = binarySearchOfNanpaArray(mappedFile,0, numEntries-1, numCharsPerLine, prefixValue);
 /*entryNumber is equal to the line number of the prefix, the first letter can be read using mappedFile[entryNumber * numCharsPerLine], entryNumber is set to -1 if the number is not found */
 //If not found in nanpa, unmap memory and close fd
 if(entryNumber == -1){
   char notFoundInNanpa[] = "Prefix not found in Nanpa\n";
   write(1, notFoundInNanpa, sizeof(notFoundInNanpa));
   /*unmap file from memory */
   if(munmap(mappedFile, fileLength) < 0){
     fprintf(stderr, "Error unmapping file: %s\n", strerror(errno));
   }
   else{
     char unmapSuccess[] = "File unmapped succesfully \n";
     write(1, unmapSuccess, sizeof(unmapSuccess));
   }
   /*Close the file descriptor*/
   if( close(fd) < 0){
     fprintf(stderr, "Error closing file %s\n", strerror(errno));
     return 1;
   }
   else{
     char closeSuccess[] = "File closed Successfully \n";
     write(1, closeSuccess, sizeof(closeSuccess));
   }
   return 1;
 }
 else{
   startIndexOfCity = (entryNumber * 32) + 6;
   currChar;
   endIndexOfCity;
   /*To trim whitespace off end of area name, endIndexOfCity is set to the first occurance of two consecutive spaces */
   for( i = 0; i < numCharsPerLine; i++){
     if(mappedFile[startIndexOfCity + i] == ' '){
       if(mappedFile[startIndexOfCity+ i +1] == ' '){
	 endIndexOfCity = i;
	 break;
       }
     }
    }
   //print area
   char city[endIndexOfCity + 1];
   for(i = 0; i < endIndexOfCity; i++){
     city[i] = mappedFile[i + startIndexOfCity];
   }
   //Add newline character for the sake of readability
   city[endIndexOfCity] = '\n';
   write(1, city, sizeof(city));
 }
 /*unmap file from memory */
 if(munmap(mappedFile, fileLength) < 0){
   fprintf(stderr, "Error unmapping file: %s\n", strerror(errno));
    }
 else{
   char unmapSuccess[] = "File unmapped succesfully \n";
   write(1, unmapSuccess, sizeof(unmapSuccess));
}

 /*Close the file descriptor*/
 if( close(fd) < 0){
   fprintf(stderr, "Error closing file %s\n", strerror(errno));
   return 1;
 }
 else{
   char closeSuccess[] = "File closed Successfully \n";
   write(1, closeSuccess, sizeof(closeSuccess));
 }
 return 0;
}

/*checkValue takes in a reference to the mappedFile array, slices out the phone code at indicies 0-6 as specifed in the format of the file, then casts to integers and compares the prefix 'toCompare'. If the inputted prefix is larger than the area code, 1 is returned. If in the prefix matches the area code, 0 is returned. If prefix is smaller than area code, -1 is returned. */
int checkValue(char mappedFile[], int toCompare, int offsetIndex){
  char slice[6];
  int num, i;
  for(i = 0; i < 6; i++){
    slice[i] = "0";
   }
  //getNumFromString function terminates at the NULL character
  slice[6] = NULL;
 
   for(i = 0; i < 6; i++){
    slice[i] = mappedFile[offsetIndex + i];
  }

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
/*Implemenation of binary search on nanpa array, accepts the nanpa file stored in a character array  uses helper function checkVaule to compare midpoint area code values until the target is found. Returns entryNumber of the target, -1 if not in the array. */
int binarySearchOfNanpaArray(char mappedFile[], int startIndex, int endIndex, int numCharsPerLine, int target){
  int midpoint, result;
  while(startIndex <= endIndex){
    midpoint = (startIndex + (endIndex - startIndex) / 2);
     result = checkValue(mappedFile, target, midpoint * numCharsPerLine);
    if(result == 0){
      return midpoint;
    }
    if(result == -1){
      endIndex = midpoint - 1;
    }
    if(result == 1){
      startIndex = midpoint + 1;
    }
      }
    return -1;
}

/*getNumFromString accepts a phone prefix as a string and returns the integer equivelent. Will return 0 if input is not of size 6 and will return a negative number if the input was not entirely digits 0-9.*/
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
  int digitVal, chVal;
  chVal = (int) ch;
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
    /*If any characters are not digits, returning a negative value larger than the most significant digit will ensure than the return value of get numFromString is negative if the input is not exclusively digits 0-9*/
    digitVal = -1000000;

  }
  return  digitVal;

}

