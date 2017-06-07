#ifndef HW03_PART2_ESCAPERS_H
#define HW03_PART2_ESCAPERS_H

typedef struct SEscaper *Escaper;

typedef enum {
    ESCAPER_OUT_OF_MEMORY, // You should exit program after this error
    ESCAPER_NULL_PARAMETER,
    ESCAPER_INVALID_PARAMETER,
    ESCAPER_EMAIL_ALREADY_EXISTS,
    ESCAPER_COMPANY_EMAIL_DOES_NOT_EXIST,
    ESCAPER_CLIENT_EMAIL_DOES_NOT_EXIST,
    ESCAPER_SUCCESS,
} EscaperResult;

//free the inside of the escaper
void freeEscaper(Escaper *escaper);

//copies an escaper element and returns a pointer to a new element
Escaper *copyElement(Escaper *escaper);

EscaperResult createEscaper(char *name, TechnionFaculty faculty, int skill, Escaper *newElement);

void removeEscaper(Escaper *escaper);

#endif //HW03_PART2_ESCAPERS_H
