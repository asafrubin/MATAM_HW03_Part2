#ifndef HW03_PART2_ESCAPETECHNION_H
#define HW03_PART2_ESCAPETECHNION_H

#include "list.h"
#include "set.h"
#include "mtm_ex3.h"
#include "Escapers.h"

typedef struct SEscapeTechnion EscapeTechnion;
typedef struct SFaculties Faculties;


/**
 * a function to add a client to the list.
 *
 * @param email the email of the client, must have only one @ in the string
 * @param faculty must be a vaild faculty defined in mtm_ex3.h
 * @param skill must be an int between 1-10
 * @return
 */
MtmErrorCode escaperAdd(char *email, TechnionFaculty faculty, int skill, Set escapers);

#endif //HW03_PART2_ESCAPETECHNION_H
