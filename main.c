#include <stdio.h>
#include <strings.h>
#include <memory.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include "EscapeTechnion.h"
#include "mtm_ex3.h"

#include "parser.c"



int main(int argc, char *argv[])
{
    int c;
    bool iflag = false, oflag = false;
    FILE *inputStream, *outputStream;

    while( (c = getopt(argc, argv, "io") != -1 )) {
        switch (c) {
            case 'i':
                iflag = true;
                inputStream = fopen(optarg, "r");
                if (inputStream == NULL) {
                    mtmPrintErrorMessage(stdout, MTM_INVALID_COMMAND_LINE_PARAMETERS);
                    return 0;
                }
                break;
            case 'o':
                oflag = true;
                outputStream = fopen(optarg, "w");
                if (outputStream == NULL) {
                    mtmPrintErrorMessage(stdout, MTM_INVALID_COMMAND_LINE_PARAMETERS);
                    return 0;
                }
                break;
            default:
                abort();
                break;
        }
    }
    if(iflag == false){
        inputStream = stdin;
    }
    if(oflag == false){
        outputStream = stdout;
    }
    EscapeTechnion escapeTechnion = NULL;
    escapeTechnion = mtmCreateEscapeTechnion(outputStream);
    if(escapeTechnion == NULL){
        fileClose(inputStream, outputStream, iflag, oflag);
        mtmPrintErrorMessage(outputStream, MTM_OUT_OF_MEMORY);
        return 0;
    }

    parser(inputStream, outputStream, escapeTechnion);


    fileClose(inputStream, outputStream, iflag, oflag);

    return 0;
}