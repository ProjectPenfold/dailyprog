#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct node 
{
    // change hard coded len of array (longest real word = 46 char)
    char string[46];
    struct node *next;
}
node;

// create Word Funnel by removing single char at each postion of input str. Store in linked list
int writeFunnel(char *input, node *first)
{
    // assign initial node value
    strcpy(first->string, input);
    first->next = NULL;

    // create result on heap
    char *result = malloc(sizeof(char) * (strlen(input)));

    for (int inIndex = 0; inIndex < strlen(input); inIndex++)
    {
        // create copy of input and remove index char using memmove
        strcpy(result, input);
        memmove(&result[inIndex], &result[inIndex + 1], strlen(result) - inIndex);
        
        // assign new str to a new node in the linked list
        node *new_node = malloc(sizeof(node));
        if (!new_node) return 2;
        strcpy(new_node->string, result);
        new_node->next = NULL;

        // assign node->next pointer values
        if (!first->next)
        {
            first->next = new_node;
        }
        else
        {
            new_node->next = first->next;
            first->next = new_node;
        }
    }
}

// search linked list from funnel()
bool readFunnel(char *searchTerm, node *node1)
{
    // set LL cursor start point
    node *cursor = node1;

    // traverse linked list comparing strings
    while (cursor != NULL)
    {
        if (strcmp(cursor->string, searchTerm) == 0) return true;
        else cursor = cursor->next;
    }

    return false;
}

int main(int argc, char *argv[])
{
    // TODO: sort input1(original) and input2(search for this in LL)
    if (argc != 3)
    {
        printf("Usage: funnel Src Test");
        return 1;
    }

    char *original = argv[1];
    char *test = argv[2];

    // new linked list
    node *node1 = malloc(sizeof(node));

    // create linked list of possible new strings
    writeFunnel(original, node1);

    // search LL for input2
    bool found = readFunnel(test, node1);

    printf("Found: %s\n", found ? "true" : "false");
    // printf("Original: %s\nAlt: %s\n", original, result);

    free(node1);
}

