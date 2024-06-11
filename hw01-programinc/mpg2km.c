#include <stdio.h>
#include "make.h"
/* 
* mpg2lphm() 
* Argument: miles per gallon
* Return: the equivalent number of liters per 100 kilometers. 
*/
// 不确定输入的数据类型怎么办？
float mpg2lphm(float mpg) {
    if (mpg > 0) {
        return 235.21 / mpg;
    }
    return -1;
}

/*
Argument: the liters per 100 kilometers
Return: miles per gallon
*/
float lph2mpg(float lph) {
    if ( lph > 0 ){
        return 235.21/lph;
    }
    return -1;
}

float mpg2kml(float mpg) {
    if ( mpg > 0){
        return mpg*1.60934/3.78541;
    }
    return -1;
}