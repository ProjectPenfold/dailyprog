#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(void)
{
    char *original = "first";
    //char *alt = calloc(strlen(original), sizeof(char));
    char alternative[strlen(original)];
    //char *alternate = malloc(sizeof(char) * strlen(original));

    //call new function
    void funnel(&original, &alternative);

    printf("Original: %s\nAlt: %s\n", original, alternative);
}

void funnel(char *input, char *output)
{
    int indexSkip = 0;

    for (int i = 0; i <= strlen(*input); i++)
    {
        if (i == indexSkip)
        {
            i++;
        }
        else if (i == strlen(*input))
        {
            *output[i] = '\0';
        }
        else
        {
            *(output + i) = *(input + i);
            // alt[i] = original[i];
            printf("alt[%i]: %c\n", i, *output[i]);
        }
    }
}