#include <stdio.h>
#include <strings.h>
#include <memory.h>
#include "EscapeTechnion.h"
#include "mtm_ex3.h"

/**************************************************************/
/*  The following enum is used to translate the commands      */
/*  and the sub commands to enum type for the switch case     */
/**************************************************************/

typedef enum{cmdCompany, cmdCompanyAdd, cmdCompanyRemove, cmdRoom, cmdRoomAdd, cmdRoomRemove,
cmdEscaper, cmdEscaperAdd, cmdEscaperRemove, cmdEscaperOrder, cmdEscaperRecommend,
cmdReport, cmdReportDay, cmdReportBest, cmdInvalidCommand}MtmCommand;

static MtmErrorCode handleFullCommand(MtmCommand command, char *string, FILE *input, FILE *output, EscapeTechnion escapeTechnion);
static MtmCommand commandToEnum(char *command, char *subCommand);
static MtmCommand subCommandCompanyToEnum(MtmCommand command, char *subCommand);
static MtmCommand subCommandRoomToEnum(MtmCommand command, char *subCommand);
static MtmCommand subCommandEscaperToEnum(MtmCommand command, char *subCommand);
static MtmCommand subCommandReportToEnum(MtmCommand command, char *subCommand);

void parser(FILE *inputStream, FILE *outputStream, EscapeTechnion escapeTechnion)
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
        result = handleFullCommand( commandToEnum(command, subCommand), string, inputStream, outputStream, escapeTechnion );
        if(result ==  MTM_OUT_OF_MEMORY) {
            return;
        }
    }
}

static MtmCommand commandToEnum(char *command, char *subCommand)
{
    if( strcmp("company", command) == 0 ){
        return subCommandCompanyToEnum(cmdCompany, subCommand);
    }
    else if( strcmp("room", command) == 0){
        return subCommandRoomToEnum(cmdRoom, subCommand);
    }
    else if( strcmp("escaper", command) == 0){
        return subCommandEscaperToEnum(cmdEscaper, subCommand);
    }
    else if( strcmp("report", command) == 0){
        return subCommandReportToEnum(cmdReport, subCommand);
    }
    else
        return cmdInvalidCommand;
}

static MtmCommand subCommandCompanyToEnum(MtmCommand command, char *subCommand)
{
    if( strcmp("add", subCommand) == 0 ){
        return cmdCompanyAdd;
    }
    else if( strcmp("remove", subCommand ) == 0){
        return cmdCompanyRemove;
    }
        return cmdInvalidCommand;
}

static MtmCommand subCommandRoomToEnum(MtmCommand command, char *subCommand)
{
    if( strcmp("add", subCommand) == 0 ){
        return cmdRoomAdd;
    }
    else if( strcmp("remove", subCommand ) == 0){
        return cmdRoomRemove;
    }
    return cmdInvalidCommand;
}

static MtmCommand subCommandEscaperToEnum(MtmCommand command, char *subCommand){
    if( strcmp("add", subCommand) == 0 ){
        return cmdEscaperAdd;
    }
    else if( strcmp("remove", subCommand ) == 0){
        return cmdEscaperRemove;
    }
    else if( strcmp("order", subCommand ) == 0){
        return cmdEscaperOrder;
    }
    else if( strcmp("recommend", subCommand ) == 0){
        return cmdEscaperRecommend;
    }

    return cmdInvalidCommand;
}

static MtmCommand subCommandReportToEnum(MtmCommand command, char *subCommand) {
    if (strcmp("day", subCommand) == 0) {
        return cmdReportDay;
    } else if (strcmp("best", subCommand) == 0) {
        return cmdReportBest;
    }

    return cmdInvalidCommand;
}
static MtmErrorCode handleFullCommand(MtmCommand command, char *string, FILE *input, FILE *output, EscapeTechnion escapeTechnion)
{
    char *email;
    TechnionFaculty faculty;
    int roomId, roomPrice, roomNumOfPpl, roomOpenHour, roomCloseHour, roomDifficulty, skill, requestedTime;

    switch(command) {
        case cmdCompanyAdd:
            parseCompanyAdd(string, &email, &faculty);
            mtmCompanyAdd(email, faculty, &escapeTechnion);
            break;
        case cmdCompanyRemove:
            parseCompanyRemove(string, &email);
            mtmCompanyRemove(email, &escapeTechnion);
            break;
        case cmdRoomAdd:
            parseRoomAdd(string, &email, &roomId, &roomPrice, &roomNumOfPpl, &roomOpenHour,
                         &roomCloseHour, &roomDifficulty);
            mtmRoomAdd(email, roomId, roomPrice, roomNumOfPpl, roomOpenHour,roomCloseHour,
                       roomDifficulty, &escapeTechnion);
            break;
        case cmdRoomRemove:
            parseRoomRemove(string, faculty, roomId);
            mtmRoomRemove(faculty, roomId, &escapeTechnion);
            break;
        case cmdEscaperAdd:
            parseEscaperAdd(&email, &faculty, &skill );
            mtmEscaperAdd(email, faculty, skill, &escapeTechnion);
            break;
        case cmdEscaperRemove:
            parseEscaperRemove(&email);
            mtmEscaperRemove(email, &escapeTechnion);
            break;
        case cmdEscaperOrder:
            parseEscaperOrder(&email, &faculty, &roomId, &requestedTime, &roomNumOfPpl );
            mtmEscaperOrder(email, faculty, roomId, requestedTime, roomNumOfPpl, &escapeTechnion);
            break;
        case cmdEscaperRecommend:
            parseEscaperRecommend(&email, roomNumOfPpl);
            mtmEscaperRecommend(email, roomNumOfPpl, &escapeTechnion);
            break;
        case cmdReportDay:
            mtmReportDay(&escapeTechnion);
            break;
        case cmdReportBest:
            mtmReportBest(&escapeTechnion);
            break;
        case cmdInvalidCommand:
            mtmPrintErrorMessage(output, MTM_INVALID_PARAMETER);
            return MTM_INVALID_PARAMETER;
            break;
        default:
            break;
    }
}