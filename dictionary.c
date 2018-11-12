// Implements a dictionary's functionality
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include "dictionary.h"
#define MAX 300000

unsigned long hashstring(const char *str)
{
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
    {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
    return hash % MAX;
}


// define a node in linked list
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

node *hashtable[MAX];

bool insert(char * str)
{
    ;
    // Compute index
    int index = hashstring(str);

    // Insert the element at the specified index
    node * new_node = malloc(sizeof(node));

    // if hashpointer isn't pointing to anything make head

    if(new_node == NULL)
    {
        unload();
        return false;
    }

    strcpy(new_node->word, str);

    new_node->next = hashtable[index];

    hashtable[index] = new_node;

    return true;
}

bool search(const char * str)
{
        char * c_temp = malloc(LENGTH + 1);

        for(int i = 0, k = strlen(str); i < k; i++)
        {
            if (str[i] == '0')
            {
                c_temp[i] = 'o';
            }
            else
            {
                c_temp[i] = tolower(str[i]);
            }
        }

        int index = hashstring(c_temp);

        node * current = hashtable[index];

        if(current != NULL)
        {
            if(strcmp(c_temp, current->word) == 0)
            {
                free(c_temp);

                return true;
            }
            else
            {
                free(c_temp);

                return false;
            }
        }

        free(c_temp);

        return false;
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // TODO
    bool result = search(word);

    return result;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // open file, create file pointer
    FILE *inptr = fopen(dictionary, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", dictionary);
        return false;
    }

    //char * word = malloc(sizeof(LENGTH + 1));

    char * word = malloc(sizeof(char) * LENGTH + 1);

    // scan dictionary word by word

    while(fscanf(inptr, "%s", word) != EOF)
    {
        bool result = insert(word);

        if (result == false)
        {
            unload();
            return false;
        }
    }

    // check and free memory

    free(word);

    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    // TODO

    unsigned int number = 0;

    for (int i = 0; i < MAX; i++)
    {
        if (hashtable[i] != NULL)
        {
            number++;
        }
    }

    if (number == 0)
    {
        return 0;
    }
    else
    {
        return number;
    }
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    for(int i = 0; i < MAX; i++)
    {
        node *cursor = hashtable[i];

        while(cursor != NULL)
        {
            node *temp = cursor;
            cursor = cursor->next;
            free(temp);
        }

    }

    return true;
}
