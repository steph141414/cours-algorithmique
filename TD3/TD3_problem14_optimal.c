#include <stdlib.h>
#include <string.h>



char* longestCommonPrefix(char** strs, int strsSize){

    if (strsSize == 0){

        return "";
    }
    
    int min = 0;
    for (int i = 1; i < strsSize; i = i+1){
        if (strlen(strs[i]) < strlen(strs[min])){
            min = i;
        }
    }

    int continuer =1;
    char *premier = strs[0];
    int len_min = strlen(strs[min]);

    int p = 0;
    while (p < len_min && continuer) {
        for (int i =1 ; i < strsSize; i = i+1){
            if (strs[i][p] != strs[0][p]){
                continuer = 0;
                break;
            }
        }
        if (continuer){
            p =p +1;
        }
    }

    char *pre = malloc(p + 1);
    memcpy(pre, premier, p);
    pre[p] = '\0';
    return pre;
}

