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

MtmErrorCode mtmCompanyAdd(char *email, TechnionFaculty faculty, EscapeTechnion escapeTechnion);

MtmErrorCode mtmCompanyRemove(char *email, EscapeTechnion escapeTechnion);

MtmErrorCode mtmRoomAdd(char *email, int id, int price,int numOfPpl,int OpenHour,
                        int closeHour, int difficulty, EscapeTechnion escapeTechnion);

MtmErrorCode mtmRoomRemove(TechnionFaculty faculty, int id, EscapeTechnion escapeTechnion);

MtmErrorCode mtmEscaperAdd(char *email, TechnionFaculty faculty, int skill, EscapeTechnion escapeTechnion);

MtmErrorCode mtmEscaperRemove(char *email, EscapeTechnion escapeTechnion);

MtmErrorCode mtmEscaperOrder(char *escaperEmail, TechnionFaculty faculty, int roomId, int requestedTime,
                             int requestedDay, int numOfPpl, EscapeTechnion escapeTechnion);

MtmErrorCode mtmEscaperRecommend(char *escaperEmail, int numOfPpl, EscapeTechnion escapeTechnion);

MtmErrorCode mtmReportDay(EscapeTechnion escapeTechnion);

MtmErrorCode mtmReportBest(EscapeTechnion escapeTechnion);

MtmErrorCode mtmInitEscapeTechnion(EscapeTechnion *escapeTechnion, FILE *outputStream);

#endif //HW03_PART2_ESCAPETECHNION_H
