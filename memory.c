#include "data.h"

static BinaryWord *binaryImg = NULL;
static Base64 *base64Img = NULL;
unsigned static IC = MEMORY_START;
unsigned static DC = 0;
unsigned static ICF = 0;
unsigned static DCF = 0;

extern Base64 *convertBinaryToBase64(BinaryWord *word);
extern char *numToBin(int num);

unsigned getDC() { return DC; }
unsigned getIC() { return IC; }
unsigned getICF() { return ICF; }
unsigned getDCF() { return DCF; }
void increaseDataCounter(int amount)
{
    DC += amount;
}
void increaseInstructionCounter(int amount)
{
    IC += amount;
}
void allocMemoryImg()
{
    const int totalSize = DCF - MEMORY_START;
    int i, j;

    if (binaryImg != NULL)
        free(binaryImg);
    if (base64Img != NULL)
        free(base64Img);

    binaryImg = (BinaryWord *)malloc(totalSize * sizeof(BinaryWord));
    base64Img = (Base64 *)malloc(totalSize * sizeof(Base64));
    for (i = 0; i < totalSize; i++)
    {
        base64Img[i]._A = 0;
        base64Img[i]._B = 0;

        for (j = 0; j < BINARY_WORD_SIZE; j++)
        {
            binaryImg[i].digit[j].on = 0;
        }
    }
}

void resetMemoryCounters()
{
    IC = MEMORY_START;
    DC = 0;
    ICF = 0;
    DCF = 0;
}
void printBinaryImg()
{
    int i;
    int totalSize = DCF - MEMORY_START;
    for (i = 0; i < totalSize; i++)
    {
        printf("%04d ", MEMORY_START + i);
        printWordBinary(i);
    }
}
void addWord(int value, DataType type)
{
    if (type == Code)
        addWordToCodeImage(numToBin(value));
    else if (type == Data)
        addWordToDataImage(numToBin(value));
}
void addWordToDataImage(char *s)
{
    wordStringToWordObj(s, Data);
    DC++;
}
void addWordToCodeImage(char *s)
{
    wordStringToWordObj(s, Code);
    IC++;
}
void wordStringToWordObj(char *s, DataType type)
{
    int j;
    int index = type == Code ? IC - MEMORY_START : DC - MEMORY_START;
    for (j = 0; j < BINARY_WORD_SIZE; j++)
        binaryImg[index].digit[j].on = s[j] == '1' ? 1 : 0;

}

char* printWordBinary(unsigned index)
{
    int j;
	char* binaryString = (char*)malloc((BINARY_WORD_SIZE + 1) * sizeof(char)); 
    for (j = 0; j < BINARY_WORD_SIZE; j++)
    {binaryString += binaryImg[index].digit[j].on ? 1 : 0;
        
        printf("%d", binaryImg[index].digit[j].on ? 1 : 0);
    }

    printf("\n");
return binaryString;
}
void calcFinalAddrsCountersValues()
{

    ICF = IC;
    DCF = ICF + DC;
    DC = IC;
    IC = MEMORY_START;
}

void printMemoryImgInRequiredObjFileFormat()
{
    extern BinaryWord *binaryImg;
    extern Base64 *base64Img;
    int i;
    int totalSize = DCF - MEMORY_START;
    printf("%d %d\n", ICF - MEMORY_START, DCF - ICF);
    for (i = 0; i < totalSize; i++)
    {
        base64Img[i] = *convertBinaryToBase64(&binaryImg[i]);
        printf("%04d %c%c\n", MEMORY_START + i, base64Img[i]._A, base64Img[i]._B);
    }
}

void writeMemoryImageToObFile(FILE *fp)
{
    extern BinaryWord *binaryImg;
    extern Base64 *base64Img;
    int i;
    int totalSize = DCF - MEMORY_START;
    fprintf(fp, "%d %d\n", ICF - MEMORY_START, DCF - ICF);
    for (i = 0; i < totalSize; i++)
    {
        base64Img[i] = *convertBinaryToBase64(&binaryImg[i]);
        fprintf(fp, "%04d %c%c\n", MEMORY_START + i, base64Img[i]._A, base64Img[i]._B);
    }
}
