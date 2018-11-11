#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void funnel(char *input, char *result)
{
    
}

int main(void)
{
    char *original = "first";
    //char *alt = calloc(strlen(original), sizeof(char));
    //char alternative[strlen(original)];
    char *result = malloc(sizeof(char) * (10 + strlen(original)));

    //call new function
    funnel(original, &result);

    printf("Original: %s\nAlt: %s\n", original, result);
}

