#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void funnel(char *input, char *result)
{
    int indexSkip, inIndex, outIndex;
    
    for (indexSkip = inIndex = outIndex = 0; inIndex <= strlen(input); inIndex++)
    {
        if (inIndex == indexSkip)
        {
            printf("Found index\n");
        }
        else if (inIndex == strlen(input))
        {
            result[outIndex] = '\0';
            printf("alt[%i]: %c\n", outIndex, result[outIndex]);
        }
        else
        {
            //*(result + i) = *(input + i);
            result[outIndex] = input[inIndex];
            printf("alt[%i]: %c\n", outIndex, result[outIndex]);
            outIndex++;
        }
    }
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

