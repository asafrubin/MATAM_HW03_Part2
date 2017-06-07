#include <stdio.h>
#include <strings.h>
#include <memory.h>
#include "EscapeTechnion.h"
#include "mtm_ex3.h"


/**************************************************************/
/*  The following defines are used to translate the commands  */
/*  and the sub commands to integers for the switch case      */
/**************************************************************/
#define COMPANY 100
#define COMPANY_ADD 101
#define COMPANY_REMOVE 102
#define ROOM 200
#define ROOM_ADD 201
#define ROOM_REMOVE 202
#define ESCAPER 300
#define ESCAPER_ADD 301
#define ESCAPER_REMOVE 302
#define ESCAPER_ORDER 303
#define ESCAPER_RECOMMEND 304
#define REPORT 400
#define REPORT_DAY 401
#define REPORT_BEST 402
#define INVALID_COMMAND -1

static MtmErrorCode handleFullCommand(int command, char *string, FILE *input, FILE *output);
static int commandToInt(char *command, char *subCommand);
static int subCommandCompanyToInt(int command, char *subCommand);

void parser(FILE *inputStream, FILE *outputStream, EscapeTechnion *escapeTechnion)
{
    char string[MAX_LEN] = { 0 };
    char *command = NULL, *subCommand = NULL;
    char *delimeter = " '\t'";
    MtmErrorCode result;

    while(feof(inputStream)){
        fgets(string, MAX_LEN, inputStream);
        command = strtok(string, delimeter);
        if(command[0] == '#' || command[0] == '\n'){
            continue;
        }
        subCommand = strtok(NULL, delimeter);
        result = handleFullCommand( commandToInt(command, subCommand), string, inputStream, outputStream );
        if(result ==  MTM_OUT_OF_MEMORY) {
            return;
        }
    }
}

static int commandToInt(char *command, char *subCommand)
{
    if( strcmp("company", command) == 0 ){
        return subCommandCompanyToInt(COMPANY, subCommand);
    }
    else if( strcmp("room", command) == 0){
        return ROOM;
    }
    else if( strcmp("escaper", command) == 0){
        return ESCAPER;
    }
    else if(strcmp("report", command) == 0){
        return REPORT;
    }
    else
        return INVALID_COMMAND;
}

static int subCommandCompanyToInt(int command, char *subCommand)
{
    if( strcmp("add", subCommand) == 0 ){
        return COMPANY_ADD;
    }
    else if( strcmp("remove", subCommand ) == 0){
        return COMPANY_REMOVE;
    }
        return INVALID_COMMAND;
}

static int subCommandRoomToInt(int command, char *subCommand)
{
    if( strcmp("add", subCommand) == 0 ){
        return ROOM_ADD;
    }
    else if( strcmp("remove", subCommand ) == 0){
        return ROOM_REMOVE;
    }
    return INVALID_COMMAND;
}

static int subCommandEscaperToInt(int command, char *subCommand){
    if( strcmp("add", subCommand) == 0 ){
        return ESCAPER_ADD;
    }
    else if( strcmp("remove", subCommand ) == 0){
        return ESCAPER_REMOVE;
    }
    else if( strcmp("order", subCommand ) == 0){
        return ESCAPER_ORDER;
    }
    else if( strcmp("recommend", subCommand ) == 0){
        return ESCAPER_RECOMMEND;
    }

    return INVALID_COMMAND;
}
static MtmErrorCode handleFullCommand(int command, char *string, FILE *input, FILE *output)
{
    switch(command) {
        case COMPANY_ADD:
            parseCompanyAdd()
            break;
        case COMPANY_REMOVE:
            break;
        case ROOM_ADD:
            break;
        case ROOM_REMOVE:
            break;
        case ESCAPER_ADD:
            break;
        case ESCAPER_REMOVE:
            break;
        case ESCAPER_ORDER:
            break;
        case ESCAPER_RECOMMEND:
            break;
        case REPORT_DAY:
            break;
        case REPORT_BEST:
            break;
        case INVALID_COMMAND:
            mtmPrintErrorMessage(output, MTM_INVALID_PARAMETER);
            return MTM_INVALID_PARAMETER;
            break;
        default:
            break;
    }
}