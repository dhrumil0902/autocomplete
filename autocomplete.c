#include <stdlib.h>
#include <stdio.h>
#include "autocomplete.h"
#include <string.h>
int myCompare(const void *a, const void *b)
{

    const struct term *ap = (const struct term *)(a);
    const struct term *bp = (const struct term *)(b);

    return strcmp(ap->term, bp->term);
}

void read_in_terms(struct term **terms, int *pnterms, char *filename)
{
    char line[200];
    FILE *fp = fopen(filename, "r");
    char p_char[200]; // = (char *)malloc(sizeof(char)*200);

    fgets(p_char, sizeof(line), fp);
    int N = atoi(p_char);

    *pnterms = N;
    struct term *arr = (struct term *)malloc(sizeof(struct term) * N);
    for (int i = 0; i < N; i++)
    {
        fgets(line, sizeof(line), fp); //read in at most sizeof(line) characters
                                       //(including '\0') into line.
        strtok(line, "\n");
        strncpy(arr[i].term, line + 15, 200);
        arr[i].weight = (double)(atof(line));
    }
    qsort(arr, N, sizeof(struct term), myCompare);
    *terms = arr;
}
int lowest_match(struct term *terms, int nterms, char *substr)
{
    int len = strlen(substr);
    int low = 0;
    int high = nterms - 1;
    int mid = (int)((low + high) / 2);
    int min = nterms;
    while (low <= high)
    {
        mid = (int)(low + high) / 2;
        char *cmparr = terms[mid].term;
        char temp[len + 1];
        strncpy(temp, cmparr, len);
        temp[len] = '\0';
        if (strcmp(substr, temp) == 0)
        {
            if (mid < min)
            {
                min = mid;
            }
        }
        if (strcmp(substr, temp) <= 0)
        {
            high = mid - 1;
        }
        else
        {
            low = mid + 1;
        }
    }
    if (min == nterms)
    {
        return -1;
    }
    return min;
}
int highest_match(struct term *terms, int nterms, char *substr)
{
    int len = strlen(substr);
    int low = 0;
    int high = nterms - 1;
    int mid = (int)(low + high) / 2;
    int max = -1;
    while (low <= high)
    {
        mid = (int)((low + high) / 2);
        char *cmparr = terms[mid].term;
        char temp[len + 1];
        strncpy(temp, cmparr, len);
        temp[len] = '\0';
        //printf("%s\n", temp);
        if (strcmp(substr, temp) == 0)
        {
            if (mid > max)
            {
                max = mid;
            }
        }
        if (strcmp(substr, temp) < 0)
        {

            high = mid - 1;
        }
        else
        {
            low = mid + 1;
        }
    }
    if (max == nterms)
    {
        return -1;
    }
    return max;
}

void autocomplete(struct term **answer, int *n_answer, struct term *terms, int nterms, char *substr)
{
    int start = lowest_match(terms, nterms, substr);
    int end = highest_match(terms, nterms, substr);
    if (end == -1 || start == -1)
    {
        *n_answer = 0;
    }
    else
    {
        struct term *res = (struct term *)malloc(sizeof(struct term) * (end - start + 1));

        for (int i = start; i < end + 1; i++)
        {
            res[i - start] = terms[i];
        }
        double min;
        int index;
        struct term temp;
        for (int i = end - start; i > 0; i--)
        {
            min = res[i].weight;
            for (int j = 0; j <= i; j++)
            {
                if (res[j].weight <= min)
                {
                    index = j;
                    min = res[j].weight;
                }
            }
            temp = res[i];
            res[i] = res[index];
            res[index] = temp;
        }
        *answer = res;
        *n_answer = end - start + 1;
    }
}
