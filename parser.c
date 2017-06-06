#include <stdio.h>
#include <strings.h>
#include <memory.h>
#include "EscapeTechnion.h"
#include "mtm_ex3.h"

void parser(FILE *inputStream, FILE *outputStream, EscapeTechnion *escapeTechnion)
{
    char inputString[MAX_LEN] = { 0 };

    while(feof(inputStream)){
        fgets(inputString, MAX_LEN, inputStream);
        strtok()
    }
}