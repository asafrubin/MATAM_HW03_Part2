#include <stdio.h>
#include <strings.h>
#include <memory.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include "EscapeTechnion.h"
#include "mtm_ex3.h"


/**************************************************************/
/*  The following enum is used to translate the commands      */
/*  and the sub commands to enum type for the switch case     */
/**************************************************************/

typedef enum{cmdCompany, cmdCompanyAdd, cmdCompanyRemove, cmdRoom, cmdRoomAdd, cmdRoomRemove,
cmdEscaper, cmdEscaperAdd, cmdEscaperRemove, cmdEscaperOrder, cmdEscaperRecommend,
cmdReport, cmdReportDay, cmdReportBest, cmdInvalidCommand}MtmCommand;

/**************************************************************/
/*  Start Functions decleration  block                        */
/**************************************************************/
static TechnionFaculty stringToFaculty(int id);
static MtmErrorCode parseEscaperRecommend(char **escperEmail, int *roomNumOfPpl);
static MtmErrorCode parseEscaperOrder(char **escaperEmail, TechnionFaculty *faculty, int *roomId,
                                      int *requestedTime, int *requestedDay, int *roomNumOfPpl );
static MtmErrorCode parseEscaperRemove(char **email);
static MtmErrorCode parseEscaperAdd(char **email, TechnionFaculty *faculty, int *skill);
static MtmErrorCode parseRoomRemove(char **string, TechnionFaculty *faculty, int *roomId);
static MtmErrorCode parseCompanyRemove(char **string, char **email);
static MtmErrorCode handleFullCommand(MtmCommand command, char *string, FILE *output,
                                      EscapeTechnion escapeTechnion);
static MtmErrorCode parseCompanyAdd(char **string, char **email, TechnionFaculty *faculty);
static MtmErrorCode parseRoomAdd(char **string, char **email, int *roomId, int *roomPrice,
                                 int *roomNumOfPpl, int *roomOpenHour, int *roomCloseHour, int *roomDifficulty);
static MtmCommand commandToEnum(char *command, char *subCommand);
static MtmCommand subCommandCompanyToEnum(MtmCommand command, char *subCommand);
static MtmCommand subCommandRoomToEnum(MtmCommand command, char *subCommand);
static MtmCommand subCommandEscaperToEnum(MtmCommand command, char *subCommand);
static MtmCommand subCommandReportToEnum(MtmCommand command, char *subCommand);
int getopt(int, char * const [], const char *);

/**************************************************************/
/*  End Functions decleration  block                          */
/**************************************************************/


/**
 * parser function - parsing the input stream into commands and sub-commands
 * then calling the correct function from EscapeTechnion ADT
 *
 * @param inputStream - stdin or some input file
 * @param outputStream - stdout or some output file
 * @param escapeTechnion  - a constructed escapeTechnion ADT to work on
 * @return MTM_OUT_OF_MEMORY - in case something have failed during the proccess
 *         MTM_SUCCESS - in case EOF recived
 */
MtmErrorCode static parser(FILE *inputStream, FILE *outputStream, EscapeTechnion escapeTechnion)
{
    char string[MAX_LEN+2] = { 0 };
    char *command = NULL, *subCommand = NULL;
    char *delimiter = " \t";
    MtmErrorCode result;

    while(!feof(inputStream)){
        fgets(string, MAX_LEN+1, inputStream);
        command = strtok(string, delimiter);
        if(command[0] == '#' || command[0] == '\n'){
            continue;
        }
        subCommand = strtok(NULL, " \t\n");
        if(command == NULL || subCommand == NULL){
            continue;
        }
        result = handleFullCommand( commandToEnum(command, subCommand), string, outputStream, escapeTechnion );
        if(result ==  MTM_OUT_OF_MEMORY) {
            return result;
        }
        if(result != MTM_SUCCESS){
            mtmPrintErrorMessage(stderr, result);
        }
    }

    return MTM_SUCCESS;
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

static MtmCommand subCommandEscaperToEnum(MtmCommand command, char *subCommand)
{
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

static MtmCommand subCommandReportToEnum(MtmCommand command, char *subCommand)
{
    if (strcmp("day", subCommand) == 0) {
        return cmdReportDay;
    } else if (strcmp("best", subCommand) == 0) {
        return cmdReportBest;
    }

    return cmdInvalidCommand;
}

static MtmErrorCode handleFullCommand(MtmCommand command, char *string, FILE *output, EscapeTechnion escapeTechnion)
{
    char *email;
    TechnionFaculty faculty;
    MtmErrorCode result;
    int roomId, roomPrice, roomNumOfPpl, roomOpenHour, roomCloseHour,
            roomDifficulty, skill, requestedTime, requestedDay;

    switch(command) {
        case cmdCompanyAdd:
            parseCompanyAdd(&string, &email, &faculty);
            result = mtmCompanyAdd(email, faculty, escapeTechnion);
            free(email);
            return result;
            break;
        case cmdCompanyRemove:
            parseCompanyRemove(&string, &email);
            result = mtmCompanyRemove(email, escapeTechnion);
            free(email);
            return result;
            break;
        case cmdRoomAdd:
            parseRoomAdd(&string, &email, &roomId, &roomPrice, &roomNumOfPpl, &roomOpenHour,
                         &roomCloseHour, &roomDifficulty);
            result = mtmRoomAdd(email, roomId, roomPrice, roomNumOfPpl, roomOpenHour, roomCloseHour,
                       roomDifficulty, escapeTechnion);
            free(email);
            return result;
            break;
        case cmdRoomRemove:
            parseRoomRemove(&string, &faculty, &roomId);
            result = mtmRoomRemove(faculty, roomId, escapeTechnion);
            return result;
            break;
        case cmdEscaperAdd:
            parseEscaperAdd(&email, &faculty, &skill );
            result = mtmEscaperAdd(email, faculty, skill, escapeTechnion);
            free(email);
            return result;
            break;
        case cmdEscaperRemove:
            parseEscaperRemove(&email);
            result = mtmEscaperRemove(email, escapeTechnion);
            free(email);
            return result;
            break;
        case cmdEscaperOrder:
            parseEscaperOrder(&email, &faculty, &roomId, &requestedTime, &requestedDay, &roomNumOfPpl );
            result = mtmEscaperOrder(email, faculty, roomId, requestedTime, requestedDay, roomNumOfPpl, escapeTechnion);
            free(email);
            return result;
            break;
        case cmdEscaperRecommend:
            parseEscaperRecommend(&email, &roomNumOfPpl);
            result = mtmEscaperRecommend(email, roomNumOfPpl, escapeTechnion);
            free(email);
            return result;
            break;
        case cmdReportDay:
            result = mtmReportDay(escapeTechnion);
            return result;
            break;
        case cmdReportBest:
            result = mtmReportBest(escapeTechnion);
            return result;
            break;
        case cmdInvalidCommand:
            mtmPrintErrorMessage(output, MTM_INVALID_PARAMETER);
            return MTM_INVALID_PARAMETER;
            break;
        default:
            break;
    }
    return MTM_SUCCESS;
}

static TechnionFaculty stringToFaculty(int id)
{
    switch(id){
        case 0:
            return CIVIL_ENGINEERING;
        case 1:
            return MECHANICAL_ENGINEERING;
        case 2:
            return ELECTRICAL_ENGINEERING;
        case 3:
            return CHEMICAL_ENGINEERING;
        case 4:
            return BIOTECHNOLOGY_AND_FOOD_ENGINEERING;
        case 5:
            return AEROSPACE_ENGINEERING;
        case 6:
            return INDUSTRIAL_ENGINEERING_AND_MANAGEMENT;
        case 7:
            return MATHEMATICS;
        case 8:
            return PHYSICS;
        case 9:
            return CHEMISTRY;
        case 10:
            return BIOLOGY;
        case 11:
            return ARCHITECTURE;
        case 12:
            return EDUCATION_IN_TECH_AND_SCIENCE;
        case 13:
            return COMPUTER_SCIENCE;
        case 14:
            return MEDICINE;
        case 15:
            return MATERIALS_ENGINEERING;
        case 16:
            return HUMANITIES_AND_ARTS;
        case 17:
            return BIOMEDICAL_ENGINEERING;
        default:
            return UNKNOWN;
    }
}

/**************************************************************/
/* command : company add                                      */
/* parameters to parse : <email> <faculty>                    */
/**************************************************************/
static MtmErrorCode  parseCompanyAdd(char **string, char **email, TechnionFaculty *faculty)
{
    char *parameter;
    int id;
    char delimiter[] = " \n\t";
    parameter = strtok(NULL, delimiter);
    *email = malloc(strlen(parameter) + 1);
    if(*email == NULL){
        return MTM_OUT_OF_MEMORY;
    }
    strcpy(*email, parameter);
    id = atoi (strtok(NULL, delimiter) );
    *faculty = stringToFaculty(id);

    return MTM_SUCCESS;
}

/**************************************************************/
/* command : company remove                                   */
/* parameters to parse : <email>                              */
/**************************************************************/
static MtmErrorCode parseCompanyRemove(char **string, char **email)
{
    char *parameter;
    char delimiter[] = " \n\t";
    parameter = strtok(NULL, delimiter);
    *email = malloc(strlen(parameter) + 1);
    if(*email == NULL){
        return MTM_OUT_OF_MEMORY;
    }
    strcpy(*email, parameter);

    return MTM_SUCCESS;
}

/******************************************************************************************************************/
/* command : room add                                                                                             */
/* parameters to parse : <email>  <room id>  <room price>  <Number of People>  <OpenHour-CloseHour>  <difficulty> */
/******************************************************************************************************************/
static MtmErrorCode parseRoomAdd(char **string, char **email, int *roomId, int *roomPrice,
                                 int *roomNumOfPpl, int *roomOpenHour, int *roomCloseHour, int *roomDifficulty)
{
    char *parameter;
    char delimiter[] = " \n\t-";
    parameter = strtok(NULL, delimiter);
    *email = malloc(strlen(parameter) + 1);
    if(*email == NULL){
        return MTM_OUT_OF_MEMORY;
    }
    strcpy(*email, parameter);

    *roomId = atoi( strtok(NULL, delimiter) );
    *roomPrice = atoi( strtok(NULL, delimiter) );
    *roomNumOfPpl = atoi( strtok(NULL, delimiter) );
    *roomOpenHour = atoi( strtok(NULL, delimiter) );
    *roomCloseHour = atoi( strtok(NULL, delimiter) );
    *roomDifficulty =  atoi( strtok(NULL, delimiter) );

    return MTM_SUCCESS;
}

/**************************************************************/
/* command : room remove                                      */
/* parameters to parse : <faculty>  <room id>                 */
/**************************************************************/
static MtmErrorCode parseRoomRemove(char **string, TechnionFaculty *faculty, int *roomId)
{

    int id;
    char delimiter[] = " \n\t";
    id = atoi ( strtok(NULL, delimiter) );
    *faculty = stringToFaculty(id);
    *roomId = atoi( strtok(NULL, delimiter) );

    return MTM_SUCCESS;
}

/**************************************************************/
/* command : escaper add                                      */
/* parameters to parse : <email>  <faculty>  <skill_level>    */
/**************************************************************/
static MtmErrorCode parseEscaperAdd(char **email, TechnionFaculty *faculty, int *skill)
{
    char *parameter;
    char delimiter[] = " \n\t";
    parameter = strtok(NULL, delimiter);
    *email = malloc(strlen(parameter) + 1);
    if(*email == NULL){
        return MTM_OUT_OF_MEMORY;
    }
    strcpy(*email, parameter);
    *faculty = stringToFaculty( atoi (strtok(NULL, delimiter) ) );
    *skill = atoi( strtok(NULL, delimiter) );

    return MTM_SUCCESS;
}

/**************************************************************/
/* command : escaper remove                                   */
/* parameters to parse : <email>                              */
/**************************************************************/
static MtmErrorCode parseEscaperRemove(char **email)
{
    char *parameter;
    char delimiter[] = " \n\t";
    parameter = strtok(NULL, delimiter);
    *email = malloc(strlen(parameter) + 1);
    if(*email == NULL){
        return MTM_OUT_OF_MEMORY;
    }
    strcpy(*email, parameter);

    return MTM_SUCCESS;
}

/************************************************************************************************/
/* command : escaper order                                                                      */
/* parameters to parse : <email>  <faculty>  <room id>  <Requested time>  <Number of People>    */
/************************************************************************************************/
static MtmErrorCode parseEscaperOrder(char **escaperEmail, TechnionFaculty *faculty, int *roomId,
                                      int *requestedTime, int *requestedDay, int *roomNumOfPpl )
{
    char *parameter;
    char delimiter[] = " \n\t-";
    parameter = strtok(NULL, delimiter);
    *escaperEmail = malloc(strlen(parameter) + 1);
    if(*escaperEmail == NULL){
        return MTM_OUT_OF_MEMORY;
    }
    strcpy(*escaperEmail, parameter);
    *faculty = stringToFaculty( atoi (strtok(NULL, delimiter)) );
    *roomId = atoi( strtok(NULL, delimiter) );
    *requestedDay = atoi( strtok(NULL, delimiter) );
    *requestedTime = atoi( strtok(NULL, delimiter) );
    *roomNumOfPpl = atoi( strtok(NULL, delimiter) );

    return MTM_SUCCESS;
}

/**************************************************************/
/* command : escaper recommend                                */
/* parameters to parse : <escaper email>                      */
/**************************************************************/
static MtmErrorCode parseEscaperRecommend(char **escperEmail, int *roomNumOfPpl)
{
    char *parameter;
    char delimiter[] = " \n\t";
    parameter = strtok(NULL, delimiter);
    *escperEmail = malloc(strlen(parameter) + 1);
    if(*escperEmail == NULL){
        return MTM_OUT_OF_MEMORY;
    }
    strcpy(*escperEmail, parameter);

    *roomNumOfPpl = atoi( strtok(NULL, delimiter) );

    return MTM_SUCCESS;
}

void static fileClose(FILE *inputStream, FILE *outputstream, bool iflag, bool oflag)
{
    if(iflag == true){
        fclose(inputStream);
    }
    if(oflag == true){
        fclose(outputstream);
    }
    return;
}

int main(int argc, char *argv[])
{
    int c;
    bool iflag = false, oflag = false;
    FILE *inputStream, *outputStream;
    extern char *optarg;
    extern int opterr;
    MtmErrorCode mtmErrorCode;
    char *string;

    if( ( (argc-1) % 2) != 0 ){
        mtmPrintErrorMessage(stderr, MTM_INVALID_COMMAND_LINE_PARAMETERS);
        return 0;
    }
    opterr = 0;
    while((c = getopt(argc, argv, ":i:o:")) != -1) {

        switch (c) {
            case 'i':
                if(optarg == NULL){
                    if(oflag){
                        fclose(outputStream);
                    }
                    mtmPrintErrorMessage(stderr, MTM_INVALID_COMMAND_LINE_PARAMETERS);
                    return 0;
                }
                iflag = true;
                string = optarg;
                inputStream = fopen(string, "r");
                if (inputStream == NULL) {
                    if(oflag) {
                        fclose(outputStream);
                    }
                    mtmPrintErrorMessage(stderr, MTM_CANNOT_OPEN_FILE);
                    return 0;
                }
                break;
            case 'o':
                if(optarg == NULL){
                    if(iflag){
                        fclose(inputStream);
                    }
                    mtmPrintErrorMessage(stderr, MTM_INVALID_COMMAND_LINE_PARAMETERS);
                    return 0;
                }
                oflag = true;
                string = optarg;
                outputStream = fopen(string, "w+");
                if (outputStream == NULL) {
                    if(iflag){
                        fclose(outputStream);
                        }
                    mtmPrintErrorMessage(stderr, MTM_CANNOT_OPEN_FILE);
                    return 0;
                }
                break;
            case '?':
                if(oflag){
                    fclose(outputStream);
                }
                if(iflag){
                    fclose(inputStream);
                }
                mtmPrintErrorMessage(stderr, MTM_INVALID_COMMAND_LINE_PARAMETERS);
                return 0;
            case ':':
                if(oflag){
                    fclose(outputStream);
                }
                if(iflag){
                    fclose(inputStream);
                }
                mtmPrintErrorMessage(stderr, MTM_INVALID_COMMAND_LINE_PARAMETERS);
                return 0;
            default:
                if(oflag){
                    fclose(outputStream);
                }
                if(iflag){
                    fclose(inputStream);
                }
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
        mtmPrintErrorMessage(stderr, MTM_OUT_OF_MEMORY);
        return 0;
    }

    mtmErrorCode = parser(inputStream, outputStream, escapeTechnion);
    if(mtmErrorCode != MTM_SUCCESS){
        mtmPrintErrorMessage(stderr, mtmErrorCode);
    }

    fileClose(inputStream, outputStream, iflag, oflag);

    mtmDestroyEscapeTechnion(escapeTechnion);

    return 0;
}