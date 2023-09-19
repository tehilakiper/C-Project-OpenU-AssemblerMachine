#include "data.h"
char symbols[64] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
    'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'
};
Base64* convertBinaryToBase64(BinaryWord* bin) {
    Base64* new64 = malloc(sizeof(Base64));
    int count = 0;
    int num = 0;
    int i = 0;
    int position = 1;

    char* temp = malloc(3 * sizeof(char));
    if (!temp)
        return NULL;

    for (i = BINARY_WORD_SIZE - 1; i >= 0; i--) {
        num += (int)(pow(2, count) * bin->digit[i].on);
        count++;

        if (count == 6) {
            temp[position] = symbols[num % 64];
            position--;
            count = 0;
            num = 0;
        }
    }

    if (count > 0) {
        temp[position] = symbols[num % 64];
        position--;
    }

    temp[2] = '\0';

    new64->_A = temp[0];
    new64->_B = temp[1];

    free(temp);

    return new64;
}
char *cloneString(char *s)
{
    char *copy;
    if (!(*s))
        return "";
    copy = (char *)calloc(strlen(s), sizeof(char *));
    strcpy(copy, s);
    return copy;
}
char *trimFromLeft(char *s)
{
    while (isspace(*s) && *s != '\0')
        s++;
    return s;
}


char* numToBin(int num) {
    int i;
    char* binary = (char*)malloc((BINARY_WORD_SIZE + 1) * sizeof(char)); /* Allocate memory for the binary string*/

    if (binary == NULL) {
        printf("Memory allocation failed!");
        return NULL;
    }

    binary[BINARY_WORD_SIZE] = '\0'; /* Null-terminate the string*/

    for ( i = BINARY_WORD_SIZE - 1; i >= 0; i--) {
        binary[i] = (num & 1) ? '1' : '0'; /* Get the least significant bit of the number and store it as '0' or '1'*/
        num >>= 1; /* Right shift the number by 1 to process the next bit*/
    }

    return binary;
}


