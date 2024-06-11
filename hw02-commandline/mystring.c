#include <stdio.h>
#include "mystring.h"

// void info() {
//     printf("%s", "please type in \'-u\', \'-l\' or \'-cap\' :");
// }
// scanf("%s", str);

// 这里的constant是什么意思？*又是什么意思？
// 比较字符串str1和str2是否相同

/* This function copies the string from the source string to the target string. */
void my_strcat(char *dest, const char *src) {
    while (*dest) {
        dest++;
    }

    while (*src) { 
        *dest++ = *src++;
        /*  Another way
        *dest = *src;
        dest++;
        src++;
        */
    }
    *dest = '\0';
}

/* This function compares whether the two strings are the same or not */
int my_strcmp(const char *str1, const char *str2) {
    /*不可以用while(*str1++==*str2++)进行比较*/
    while (*str1 == *str2) 
    {
        if (*str1 == '\0')
            return 0;
        str1++;
        str2++;
    }
    
    return *str1 - *str2;
}


/* This function convert a string in all upper case*/
void my_toupper(char *str) {
    if (str == NULL) return;
    while (*str != '\0') {
        if (*str >= 'a' && *str <= 'z') {
            *str = *str + ('A' - 'a');
        }
        str++; // increase the pointer by one and point to the next character in the string
    }
}
    /* 
    // char res[MAX_SIZE] = {0};
    // 
    for (int i = 0; str[i] != '\0'; i++) {
        // 如果是小写字母
        // special character in the input?
        if (my_islower(str[i])) {
            str[i] = str[i] - ('a' - 'A');
        }
    }
    */

/* This function converts a string in all lower case*/
void my_tolower(char *str) {
    if (str == NULL) return;
    for (int i = 0; str[i]!= '\0'; i++ ) {   // c语言中字符串结尾是\0
        if (str[i] >= 'A' && str[i] <= 'Z') { // islower()函数没必要
            str[i] = str[i] + ('a' - 'A');
        } 
    }
}

/* This function makes the first letter of each word capitalized 
    and all other characters of the word in lower case.*/
void my_tocap(char *str) {
    if (str == NULL) return;
    char prev = ' ';               // intialize a character not using a pointer, dummy head
    while (*str != '\0') {
        if (prev == ' ') {
            if ( *str >= 'a' && * str <= 'z') {
                *str = *str + ('A' - 'a');
            }
        }
        else {
            if ( *str >= 'A' && *str <= 'Z') {
                *str = *str + ('a' - 'A');
            }
        }
        prev = *str; // Char reassign to prev
        str++; // move the pointer
    }
}

// if (my_islower(str[0])) {
//     src[0] = src[0] - ('a' - 'A');
// }
// for (int i = 1; src[i]!= 0; i++ ){
//     if (my_isupper(src[i])){
//         src[i] = src[i] + ('a' - 'A');
//     } 
// }

/* This function returns the length of a string. */
int my_strlen(const char *str) {
    int length = 0;
    while ( *str != '\0') {
        length++;
    }
    return length;
}



/* This function is used to check whether all characters are in lowercase */
/* 
int my_islower(char c) {
    if (c >= 'a' && c <= 'z') 
        return 1;
    else    
        return 0;
}
*/

/* This function is used to check whether all characters are in uppercase */
/* 
int my_isupper(char c) {
    if (c >= 'A' && c <= 'Z') 
        return 1;
    else
        return 0;
}
*/
