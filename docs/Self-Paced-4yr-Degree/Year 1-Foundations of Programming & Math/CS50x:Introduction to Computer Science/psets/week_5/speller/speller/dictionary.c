// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Word counter
unsigned int word_count = 0;

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// Choose number of buckets in hash table
#define N 65536

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // Hash the word
    unsigned int index = hash(word);

    // Access the correct bucket
    node *cursor = table[index];

    // Traverse the linked list 
    while (cursor != NULL)
    {
        // Compare words, ignoring case
        if (strcasecmp(cursor->word, word) == 0)
        {
            return true;
        }
        cursor = cursor->next;
    }
    
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // Start with 0
    unsigned long hash = 5381;

    // For each character: 
    for (int i = 0; word[i] != '\0'; i++)
    {
        // 32 * hash + hash = 33 * hash
       hash = ((hash << 5) + hash) + tolower(word[i]); 
    }

    return hash % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Open the dictionary file
    FILE *dict_file = fopen(dictionary, "r");
    if (dict_file == NULL)
    {
        return false;
    }
    
    // Word to read
    char word[LENGTH + 1];

    // Read each word in the file
    while (fscanf(dict_file, "%s", word) != EOF)
    {
        // Allocate memory for a node
        node *new_node = malloc(sizeof(node));
        if (new_node == NULL)
        {
            fclose(dict_file);
            return false;
        }

        // Copy word into node
        strcpy(new_node->word, word);

        // Hash word
        unsigned int index = hash(word);

        // Insert node at head of linked list
        new_node->next = table[index];
        table[index] = new_node;

        // Increment word count
        word_count++;
    }

    

    // Close the dictionary file
    fclose(dict_file);

    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // Return number of words in dictionary
    return word_count;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // Loop over every bucket.
    for (int i = 0; i < N; i++)
    {
        // Start at the end of the list
        node *cursor = table[i];

        // Traverse the linked list
        while (cursor != NULL)
        {
            // Save current node
            node *tmp = cursor;
            // Move forward
            cursor = cursor->next;
            // Free old node
            free(tmp);
        }
    }

    // Reset word count
    word_count = 0;

    // Return success
    return true;
}
