#include "data.h"
static Item *symbols[HASHSIZE] = {0};
static Item *macros[HASHSIZE] = {0};
static unsigned entriesCount = 0;
static unsigned externalCount = 0;

static ExtListItem *extListHead = NULL;
extern unsigned getICF();
extern Bool verifyLabelNaming(char *s);

void findAllExternals();
void addExtListItem(char *name);
void resetExtList();
ExtListItem *findExtOpListItem(char *name);
void updateExtPositionData(char *name, unsigned val);/***/
void freeTableItem(Item *item);
void freeTablesMemory();
ExtListItem *findExtOpListItem(char *name)
{
    extern ExtListItem *extListHead;
    ExtListItem *p = extListHead;
    while (p != NULL)
    {
        if (p->name)
        {
            if (strcmp(name, p->name) == 0)
                return p;
        }
        p = p->next;
    }
    return NULL;
}

void resetExtList()
{
    ExtListItem *np = extListHead, *next;
    ExtPositionData *pos, *nextPos;
    externalCount = 0;

    while (np != NULL)
    {
        next = np->next;
        nextPos = np->value.next;
        while (nextPos != NULL)
        {
            pos = nextPos;
            nextPos = pos->next;
            free(pos);
        }
        free(np);
        np = next;
    }

    extListHead = NULL;
}

void updateExtPositionData(char *name, unsigned val)
{

    ExtListItem *np = findExtOpListItem(name);
  
    if (np->value.val)
    {
        ExtPositionData *new = (ExtPositionData *)malloc(sizeof(ExtPositionData));
        new->val = val;
        new->next = np->value.next;
        np->value.next = new;
    }
    else
    {
        np->value.val = val;        
    }

    externalCount++;
}

void addExtListItem(char *name)
{

    ExtListItem *next;
    next = (ExtListItem *)calloc(1, sizeof(ExtListItem));
    strncpy(next->name, name, strlen(name));

    if (extListHead != NULL)
    {
        next->next = extListHead->next;
        extListHead->next = next;
    }
    else
    {
        next->next = NULL;
        extListHead = next;
    }
}

unsigned hash(char *s)
{
    unsigned hashval = 1;
    for (hashval = 0; *s != '\0'; s++)
        hashval = *s + 31 * hashval;
    return hashval % HASHSIZE;
}

Item *lookup(char *s, ItemType type)
{
    Item *np;
    int i = hash(s);
    for (np = (type == Symbol ? symbols[i] : macros[i]); np != NULL; np = np->next)
        if (!strcmp(s, np->name))
            return np;

    return NULL;
}

Item *install(char *name, ItemType type)
{
    unsigned hashval;
    Item *np;
    np = (Item *)malloc(sizeof(Item));

    if (np == NULL)
    {
        yieldError(memoryAllocationFailure);
        return NULL;
    }
    else
    {
        np->name = cloneString(name);
        if (type == Symbol)
        {
            np->val.s.attrs.code = 0;
            np->val.s.attrs.entry = 0;
            np->val.s.attrs.external = 0;
            np->val.s.attrs.data = 0;           
            np->val.s.value = 0;
            
        }
        else if (type == Macro)
        {
            np->val.m.start = -1;
            np->val.m.end = -1;
        }

        hashval = hash(name);
        np->next = (type == Symbol ? symbols[hashval] : macros[hashval]);
        if (type == Symbol)
            symbols[hashval] = np;
        else
            macros[hashval] = np;
    }

    return np;
}

Bool addSymbol(char *name, unsigned value, unsigned isCode, unsigned isData, unsigned isEntry, unsigned isExternal)
{
    
    Item *p;

    if (name[strlen(name) - 1] == ':')
        name[strlen(name) - 1] = '\0';

    if (!verifyLabelNamingAndPrintErrors(name))
        return False;
    p = lookup(name, Symbol);
    if (p != NULL)
        return updateSymbol(p, value, isCode, isData, isEntry, isExternal);
    else
    {
        p = install(name, Symbol);
      
        p->val.s.value = value;
        
        p->val.s.attrs.code = isCode ? 1 : 0;
        p->val.s.attrs.entry = isEntry ? 1 : 0;
        p->val.s.attrs.external = isExternal ? 1 : 0;
        p->val.s.attrs.data = isData ? 1 : 0;
    }

    return True;
}

Bool updateSymbol(Item *p, unsigned value, unsigned isCode, unsigned isData, unsigned isEntry, unsigned isExternal)
{
    
    if (((p->val.s.attrs.external) && (value || isData || isEntry || isCode)))
        return yieldError(illegalOverrideOfExternalSymbol);

    else if ((p->val.s.attrs.code || p->val.s.attrs.data || p->val.s.attrs.entry) && isExternal)
        return yieldError(illegalOverrideOfLocalSymbolWithExternalSymbol);

    else
    {
        if ((isData && isCode) || (isCode && p->val.s.attrs.data) || (isData && p->val.s.attrs.code))
            return yieldError(illegalSymbolNameAlreadyInUse);

        if (value)
        {
            
            p->val.s.value = value;
            
        }

        if (isEntry)
            p->val.s.attrs.entry = 1;
        if (isCode)
            p->val.s.attrs.code = 1;
        if (isData)
            p->val.s.attrs.data = 1;
    }

    return True;
}

Item *getSymbol(char *name)
{
    return lookup(name, Symbol);
}

int getSymbolAddress(char *name)
{
    Item *p = lookup(name, Symbol);
    if (p == NULL)
        return -1;

    return p->val.s.value;
}

Bool isSymbolExist(char *name)
{
    return lookup(name, Symbol) != NULL ? True : False;
}
Bool isExternal(char *name)
{
    Item *p = lookup(name, Symbol);
    if (p == NULL)
        return False;
    return p->val.s.attrs.external;
}
Bool isEntry(char *name)
{
    Item *p = lookup(name, Symbol);
    if (p == NULL)
        return False;

    return p->val.s.attrs.entry == 1 ? True : False;
}

Bool isNonEmptyExternal(char *name)
{
    Item *p = lookup(name, Symbol);
    if (p == NULL)
        return False;

    return (p->val.s.attrs.code || p->val.s.attrs.data) ? True : False;
}

Bool isNonEmptyEntry(char *name)
{
    Item *p = lookup(name, Symbol);
    if (p == NULL)
        return False;
    return (p->val.s.attrs.code || p->val.s.attrs.data) ? True : False;
}

Bool isLabelNameAlreadyTaken(char *name, ItemType type)
{
    Item *p = lookup(name, type);

    if (name[strlen(name) - 1] == ':')
        name[strlen(name) - 1] = '\0';

    if (p != NULL)
    {
        if (type == Symbol)
        {
            if (p->val.s.attrs.data || p->val.s.attrs.code)
                return True;
            if (p->val.s.attrs.entry)
                return (!p->val.s.attrs.data && !p->val.s.attrs.code && !p->val.s.attrs.external) ? False : True;
            if (p->val.s.attrs.external)
                return (!p->val.s.attrs.data && !p->val.s.attrs.code && !p->val.s.attrs.entry) ? False : True;
        }

        else if (type == Macro)
            return p->val.m.start != -1 ? False : True;
    }

    return False;
}

Item *updateSymbolAddressValue(char *name, int newValue)
{
    Item *p = getSymbol(name);
    
    

    if (p != NULL)
    {
        
        p->val.s.value = newValue;
    }
    else
        yieldError(symbolDoesNotExist);

    return p;
}

Item *getMacro(char *s)
{
    return lookup(s, Macro);
}

Item *addMacro(char *name, int start, int end)
{
    Item *macro = lookup(name, Macro);

    if (macro != NULL)
    {
        yieldError(illegalMacroNameAlreadyInUse);
        return NULL;
    }
    else
    {
        macro = install(name, Macro);

        if (start != -1)
            macro->val.m.start = start;
        if (end != -1)
            macro->val.m.end = end;
    }

    return macro;
}
Item *updateMacro(char *name, int start, int end)
{
    Item *macro = getMacro(name);
    if (!macro)
        return NULL;
    if (start != -1)
        macro->val.m.start = start;
    if (end != -1)
        macro->val.m.end = end;

    return macro;
}

void updateFinalSymbolTableValues()
{
    int i = 0;
    while (i < HASHSIZE)
    {
        if (symbols[i] != NULL)
            updateFinalValueOfSingleItem(symbols[i]);
        i++;
    }
}

void updateFinalValueOfSingleItem(Item *item)
{
    if (item->val.s.attrs.entry)
        entriesCount++;
    if (item->val.s.attrs.external)
        addExtListItem(item->name);

    if (item->val.s.attrs.data)
    {
        unsigned newValue = item->val.s.value + getICF();
       
        item->val.s.value = newValue;
    }

    if (item->next != NULL)
        updateFinalValueOfSingleItem(item->next);
}

Bool areEntriesExist()
{
    return entriesCount > 0 ? True : False;
}
Bool areExternalsExist()
{
    return externalCount > 0 ? True : False;
}

void writeExternalsToFile(FILE *fp)
{
    ExtListItem *p = extListHead;
    while (p != NULL)
    {
        if (p->value.val)
            writeSingleExternal(fp, p->name, p->value.val, p->value.next);
        p = p->next;
    }
}

void writeSingleExternal(FILE *fp, char *name, unsigned val, ExtPositionData *next)
{

    fprintf(fp, "%s \t %u\n", name, val);
   
    if (next != NULL)
        writeSingleExternal(fp, name, next->val, next->next);
}

void writeEntriesToFile(FILE *fp)
{
    int i = 0;
    int totalCount = 0;
    while (i < HASHSIZE)
    {
        if (symbols[i] != NULL && totalCount <= entriesCount)
            totalCount += writeSingleEntry(symbols[i], fp, 0);
        i++;
    }
}

int writeSingleEntry(Item *item, FILE *fp, int count)
{
    if (item->val.s.attrs.entry)
    {
        fprintf(fp, "%s\t%d\n", item->name, item->val.s.value);
        count++;
    }
    if (item->next != NULL)
        writeSingleEntry(item->next, fp, count);

    return count;
}

void initTables()
{
    extern unsigned externalCount, entriesCount;
    int i = 0;
    if (extListHead != NULL)
        resetExtList();

    externalCount = entriesCount = 0;
    while (i < HASHSIZE)
    {
        symbols[i] = NULL;
        macros[i] = NULL;
        i++;
    }
}

void freeHashTable(ItemType type)
{

    int i = 0;
    while (i < HASHSIZE)
    {

        if (type == Symbol)
        {
            if (symbols[i] != NULL)
                freeTableItem(symbols[i]);
        }
        else
        {
            if (macros[i] != NULL)
                freeTableItem(macros[i]);
        }

        i++;
    }
}

void freeTableItem(Item *item)
{
    if (item->next != NULL)
        freeTableItem(item->next);
   
    free(item);
    return;
}

void printMacroTable()
{
    int i = 0;
    printf("\n\t ~ MACRO TABLE ~ \n");
    printf("\tname\tstart\tend");
    while (i < HASHSIZE)
    {
        if (macros[i] != NULL)
            printMacroItem(macros[i]);
        i++;
    }
    printf("\n\n");
}

int printMacroItem(Item *item)
{

    printf("\n\t%s\t %5d\t%6d", item->name, item->val.m.start, item->val.m.end);
    if (item->next != NULL)
        printMacroItem(item->next);
    return 0;
}

void printSymbolTable()
{
    int i = 0;

    printf("\n\t ~ SYMBOL TABLE ~ \n");
    printf("\tname\t\tvalue");

    while (i < HASHSIZE)
    {
        if (symbols[i] != NULL)
            printSymbolItem(symbols[i]);
        i++;
    }
    printf("\n\n");
}

int printSymbolItem(Item *item)
{
    /*  printf("line 94, inside printSymbolItem \n");
     */

    printf("\n\t%s\t\t%u\t", item->name, item->val.s.value);
    
    if (item->next != NULL)
        printSymbolItem(item->next);
    return 0;
}
