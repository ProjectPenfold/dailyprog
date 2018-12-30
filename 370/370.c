// [2018-12-17] Challenge #370 [Easy] UPC check digits https://www.reddit.com/r/dailyprogrammer/comments/a72sdj/20181217_challenge_370_easy_upc_check_digits/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    size_t evenSum = 0;
    size_t oddSum = 0;
    size_t checkDigit;
    char *paddedInput = malloc(11);

    // Validate input
    if (argc != 2) {
        printf("More than one parameter in input. Usage: upc(<Up to 11 digits>)\n");
        return 1;
    }
    // necessary because of hardcoded malloc limit in var paddedInput
    if (strlen(argv[1]) > 11) {
        printf("More than 11 digits in input. Usage: upc(<Up to 11 digits>)\n");
        return 2;
    }
    
    size_t input = atoi(argv[1]);

    // Add left hand side index padding, ensures total no. digits = 11
    sprintf(paddedInput, "%011zu", input);

    // Loop over each char in string input
    for (size_t i = 0; i < strlen(paddedInput); i++) {
        // Check if even number
        size_t upcDigit = paddedInput[i] - 48;
        if (i % 2 == 0) {
            // Append to existing even digit sum
            evenSum += upcDigit;
        } else {
            oddSum += upcDigit;
        }
    }

    // Calculate M, where M = (3evenSum + oddSum) % 10
    size_t M = (3*evenSum + oddSum) % 10;
    if (!M) {
        checkDigit = 0;
    } else {
        checkDigit = 10 - M;
    }

    printf("%s => %zu\n", paddedInput, checkDigit);
    return 0;
}
