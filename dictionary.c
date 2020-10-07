// Implements a dictionary's functionality
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents number of buckets in a hash table
#define N 26

int word_count = 0;
bool load_check = false;

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Represents a hash table
node *hashtable[N];

// Hashes word to a number between 0 and 25, inclusive, based on its first letter
unsigned int hash(const char *word)
{
    return tolower(word[0]) - 'a';
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Initialize hash table
    for (int i = 0; i < N; i++)
    {
        hashtable[i] = NULL;
    }

    // Open dictionary
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        unload();
        return false;
    }

    // Buffer for a word
    char word[LENGTH + 1];

    // Insert words into hash table
    while (fscanf(file, "%s", word) != EOF)
    {
        int hash_index = hash(word);
        word_count++;
        node *temp = malloc(sizeof(node));
        strcpy(temp->word, word);
        temp->next = NULL;
        if (hashtable[hash_index] != NULL)
        {
            temp->next = hashtable[hash_index];
            hashtable[hash_index] = temp;
        }
        else
        {
            hashtable[hash_index] = temp;
        }
    }

    // Close dictionary
    fclose(file);

    // Indicate success
    load_check = true;
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    if (load_check)
    {
        return word_count;
    }
    return 0;
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    for (node *temp_ptr = hashtable[hash(word)]; temp_ptr != NULL; temp_ptr = temp_ptr->next)
    {
        if (strcasecmp(temp_ptr->word, word) == 0)
        {
            return true;
        }
    }
    return false;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    int index = 0;
    node *backup_ptr = malloc(sizeof(node));
    while (index < 26)
    {
        for (node *temp_ptr = hashtable[index]; temp_ptr != NULL; temp_ptr = backup_ptr->next)
        {
            *backup_ptr = *temp_ptr;
            free(temp_ptr);
        }
        index++;
    }
    free (backup_ptr);
    return true;
}
