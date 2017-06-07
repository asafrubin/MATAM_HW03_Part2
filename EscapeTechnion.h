#ifndef HW03_PART2_ESCAPETECHNION_H
#define HW03_PART2_ESCAPETECHNION_H

#include "list.h"
#include "set.h"
#include "mtm_ex3.h"
#include "Escapers.h"
#include "Orders.h"
#include "Rooms.h"
#include "Companies.h"

typedef struct SEscapeTechnion *EscapeTechnion;


/**
 * a function to add a client to the list.
 *
 * @param email the email of the client, must have only one @ in the string
 * @param faculty must be a vaild faculty defined in mtm_ex3.h
 * @param skill must be an int between 1-10
 * @return
 */

MtmErrorCode mtmCompanyAdd(char *email, TechnionFaculty faculty, EscapeTechnion *escapeTechnion);

MtmErrorCode mtmCompanyRemove(char *email, EscapeTechnion *escapeTechnion);

MtmErrorCode mtmRoomAdd(char *email, int id, int price,int numOfPpl,int OpenHour,
                        int closeHour, int difficulty, EscapeTechnion *escapeTechnion);

MtmErrorCode mtmRoomRemove(TechnionFaculty faculty, int id, EscapeTechnion *escapeTechnion);

MtmErrorCode mtmEscaperAdd(char *email, TechnionFaculty faculty, int skill, EscapeTechnion *escapeTechnion);

MtmErrorCode mtmEscaperRemove(char *email, EscapeTechnion *escapeTechnion);

MtmErrorCode mtmEscaperOrder(char *escaperEmail, TechnionFaculty faculty, int roomId, int requestedTime,
                             int numOfPpl, EscapeTechnion *escapeTechnion);

MtmErrorCode mtmEscaperRecommend(char *email, int numOfPpl, EscapeTechnion *escapeTechnion);

MtmErrorCode mtmReportDay(EscapeTechnion *escapeTechnion);

MtmErrorCode mtmReportBest(EscapeTechnion *escapeTechnion);

#endif //HW03_PART2_ESCAPETECHNION_H
