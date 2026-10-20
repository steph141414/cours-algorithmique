#include <stdlib.h>
#include <string.h>


int comparer_chaines(const void *a, const void *b)
{
    return strcmp(*(const char **)a, *(const char **)b);
}

char* longestCommonPrefix(char** strs,int strsSize)
{
    qsort(strs, strsSize, sizeof(char *),comparer_chaines);

    char *premier = strs[0];
    char *dernier =strs[strsSize - 1];

    int p = 0;
    while (premier[p] != '\0' && premier[p] == dernier[p]) {
        p = p +1;
    }

    char *pre = malloc(p +1);
    memcpy(pre, premier, p);
    pre[p] ='\0';
    return pre;
}

