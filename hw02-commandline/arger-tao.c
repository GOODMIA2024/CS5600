/*
* TaoM.CS5600.LearnC.c / Command Line Arguments
*
* Minjia Tao / CS5600 / Northeastern University
* Spring 2024 / Jan 26, 2024
*
*/

#include <stdio.h>
#include "mystring.h"  // #include <string.h>
#define MAX_SIZE 1024 

int main( int argc, char *argv[] ) 
{
    if (argc < 3)                                                              // at least 3 parameters to be passed through, including the text
    { 
        printf("Error. Invalid arguments.\nProvide Usage: %s [-u/-l/-cap] \"String\"\n", argv[0]); // name of the program
        return -1;
    }
    char res[MAX_SIZE] = {0};                                                 // initialize a very large string to store the result
    // concatenate all the elements after the command
    for (int i = 2; i < argc; i++) { 
        my_strcat(res, argv[i]);                                               // not *argv - the pointer to the first parameter - the name of the program, append char by char
        if ( i < argc - 1) {
            my_strcat(res, " ");                                               // add space between words, not the last word argc: count of variables, argv: access the truestring
        }
    }
    if ( my_strcmp(argv[1], "-u") == 0) {
        my_toupper(res); 
    } else if ( my_strcmp (argv[1], "-l") == 0) {
        my_tolower(res);
    } else if ( my_strcmp (argv[1], "-cap") == 0) {                              // string "", char ''
        my_tocap(res);
    } 
    else {
        printf("Error: Invalid command\n");
        return -1;
    }
    printf("%s\n", res);
    return 0;
}

