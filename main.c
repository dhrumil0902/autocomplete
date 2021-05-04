#include "autocomplete.h"
# include <stdio.h>
#include <stdlib.h>
int main(void)
{
     struct term *terms;
    int nterms;
    read_in_terms(&terms, &nterms, "cities.txt");
    //printf("%f",(terms)[0].weight);

    return 0; 

}

