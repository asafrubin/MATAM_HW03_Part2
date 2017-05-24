#ifndef SOLUTION_MTM_EX3_H
#define SOLUTION_MTM_EX3_H

#include <stdio.h>

/**
 * Maximal input line length
 */
#define MAX_LEN 250

/**
 * This type defines all errors for the system.
 * These codes should be used for calling mtmPrintErrorMessage to report
 * error messages.
 *
 * Notice that after calling mtmPrintErrorMessage with one of the first
 * three messages you should abort the program according to the exercise's
 * rules
 */
typedef enum {
    MTM_OUT_OF_MEMORY, // You should exit program after this error
    MTM_INVALID_COMMAND_LINE_PARAMETERS, // You should exit program after this error
    MTM_CANNOT_OPEN_FILE, // You should exit program after this error
    MTM_NULL_PARAMETER,
    MTM_INVALID_PARAMETER,
    MTM_EMAIL_ALREADY_EXISTS,
    MTM_COMPANY_EMAIL_DOES_NOT_EXIST,
    MTM_CLIENT_EMAIL_DOES_NOT_EXIST,
    MTM_ID_ALREADY_EXIST,
    MTM_ID_DOES_NOT_EXIST,
    MTM_CLIENT_IN_ROOM,
    MTM_ROOM_NOT_AVAILABLE,
    MTM_RESERVATION_EXISTS,
    MTM_NO_ROOMS_AVAILABLE,
    MTM_SUCCESS,
} MtmErrorCode;


/**
 * This type defines all Technion Faculties accepted by the system.
 * Any other Faculty should be handled by the MTM_INVALID_PARAMETER error code.
 */
typedef enum {
    CIVIL_ENGINEERING,
    MECHANICAL_ENGINEERING,
    ELECTRICAL_ENGINEERING,
    CHEMICAL_ENGINEERING,
    BIOTECHNOLOGY_AND_FOOD_ENGINEERING,
    AEROSPACE_ENGINEERING,
    INDUSTRIAL_ENGINEERING_AND_MANAGEMENT,
    MATHEMATICS,
    PHYSICS,
    CHEMISTRY,
    BIOLOGY,
    ARCHITECTURE,
    EDUCATION_IN_TECH_AND_SCIENCE,
    COMPUTER_SCIENCE,
    MEDICINE,
    MATERIALS_ENGINEERING,
    HUMANITIES_AND_ARTS,
    BIOMEDICAL_ENGINEERING,
    UNKNOWN,
} TechnionFaculty;


/**
 * mtmPrintErrorMessage - prints an error message into the given output channel
 * Use the standard error channel to print error messages.
 *
 * @param errorChannel - File descriptor for the error channel
 * @param code - Error code to print message for.
 */
void mtmPrintErrorMessage(FILE* errorChannel, MtmErrorCode code);


/**
 * mtmPrintDayHeader - prints the proper header designed to show the day's info
 * To be used before printing a day's orders.
 *
 * @param outputChannel - File descriptor for the output channel
 * @param day - The day's number in the system. The first day means day=0.
 * @param numberOfEvents - The number of orders that were completed on that day
 */
void mtmPrintDayHeader(FILE* outputChannel, int day, int numberOfEvents);


/**
 * mtmPrintOrder - prints an order description.
 * To be used in printing a day's orders.
 *
 * @param outputChannel - File descriptor for the output channel
 * @param email - The email of the client who ordered the room
 * @param skill - The skill level of the client
 * @param client_faculty - The faculty of the client
 * @param company_email - The email of the company who owns the ordered room
 * @param room_faculty - The faculty to which the room belongs
 * @param id - The room id in the faculty
 * @param hour - The reserved hour in which the room was used
 * @param difficulty - The difficulty level of the room
 * @param num_ppl - The number of people who went to the room
 * @param totalPrice - The total price of the order
 */
void mtmPrintOrder(FILE* outputChannel, char* email, int skill, TechnionFaculty client_faculty, char* company_email,
                   TechnionFaculty room_faculty, int id, int hour, int difficulty, int num_ppl, int totalPrice);

/**
 * mtmPrintDayFooter - prints the proper footer designed to end the printing of a day's info
 * To be used after printing a day's orders.
 *
 * @param outputChannel - File descriptor for the output channel
 * @param day - The day's number in the system. The first day means day=0.
 */
void mtmPrintDayFooter(FILE* outputChannel, int day);

/**
 * mtmPrintFacultiesHeader - prints the proper header designed to show the faculties info
 * To be used before printing the best faculties
 *
 * @param outputChannel - File descriptor for the output channel
 * @param numberOfFaculties - The number of faculties in the system
 * @param numberOfDays - The number of days that passed since the start
 * @param totalRevenue - The total amount of money that *all* faculties in the system made
 */
void mtmPrintFacultiesHeader(FILE* outputChannel, int numberOfFaculties, int numberOfDays, int totalRevenue);


/**
 * mtmPrintFaculty - prints a faculty's description
 * To be used in printing the best faculties
 *
 * @param outputChannel - File descriptor for the output channel
 * @param faculty - The faculty id for the faculty to print
 * @param revenue - The total amount of money earned from rooms that belong to the faculty
 */
void mtmPrintFaculty(FILE* outputChannel, TechnionFaculty faculty, int revenue);

/**
 * mtmPrintFacultiesFooter - prints the proper footer designed to end the printing of the faculties info
 * To be used after printing the best faculties
 *
 * @param outputChannel - File descriptor for the output channel
 */
void mtmPrintFacultiesFooter(FILE* outputChannel);

#endif //SOLUTION_MTM_EX3_H
