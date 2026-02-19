#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "user_input.h"

char *read_username(void)
{
    char buffer[32];
    char *username = NULL;

    fgets(buffer, 32, stdin);

    username = malloc(sizeof(char) * strlen(buffer));

    if (username == NULL)
        return NULL;

    strcpy(username, buffer);
    
    return username;
}
