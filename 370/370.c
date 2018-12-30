// [2018-12-17] Challenge #370 [Easy] UPC check digits https://www.reddit.com/r/dailyprogrammer/comments/a72sdj/20181217_challenge_370_easy_upc_check_digits/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t upc(size_t input) {

    size_t evenSum = 0;
    size_t oddSum = 0;
    size_t checkDigit;
    char *paddedInput = malloc(11);

    // Add left hand side index padding, ensures total no. digits = 11
    sprintf(paddedInput, "%011zu", input);

    // Loop over each char in string input
    for (size_t i = 0; i < strlen(paddedInput); i++) {
        // Convert ASCII value of char to int. 
        size_t upcDigit = paddedInput[i] - 48;
        if (i % 2 == 0) {
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

    free(paddedInput);
    return checkDigit;
}

int main(void) {

    printf("upc(4210000526) => %zu\n", upc(4210000526));
    printf("upc(3600029145) => %zu\n", upc(3600029145));
    printf("upc(12345678910) => %zu\n", upc(12345678910));
    printf("upc(1234567) => %zu\n", upc(1234567));

    return 0;
}
