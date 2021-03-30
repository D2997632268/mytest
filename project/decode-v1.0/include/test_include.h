#ifndef TEST_INCLUDE_H
#define TEST_INCLUDE_H
#include <sys/time.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "stdbool.h"

/**********junlin**********/
struct Decode_Struct_Date
{

    int decode_result[48];
    int X[5] ;
    int Y[5] ;
};



/**
* @details decode_function
* unsigned char* pic   picture addr
* int picture_hight    picture hight
* int picture_width    picture width
* @version 1.0
*/
int decode_function(unsigned char* pic,int picture_hight,int picture_width);
/**********junlin**********/



#endif // TEST_INCLUDE_H
