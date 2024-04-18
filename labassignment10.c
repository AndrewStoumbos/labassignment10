#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ALPHABET_SIZE 26
#define CHAR_TO_INDEX(c) ((int)c - (int)'a')

// Trie node definition
struct TrieNode {
    struct TrieNode *children[ALPHABET_SIZE];
    int count;  // Number of times this word has been inserted
};

struct Trie {
    struct TrieNode *root;
};

// Function to create a new trie node
struct TrieNode* getNode(void) {
    struct TrieNode *pNode = NULL;
    pNode = (struct TrieNode *)malloc(sizeof(struct TrieNode));

    if (pNode) {
        pNode->count = 0;
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            pNode->children[i] = NULL;
        }
    }
    return pNode;
}

// Initializes a trie structure
struct Trie *createTrie() {
    struct Trie *pTrie = (struct Trie *)malloc(sizeof(struct Trie));
    pTrie->root = getNode();
    return pTrie;
}

// Inserts the word into the trie structure
void insert(struct Trie *pTrie, char *word) {
    int level;
    int length = strlen(word);
    int index;

    struct TrieNode *pCrawl = pTrie->root;

    for (level = 0; level < length; level++) {
        index = CHAR_TO_INDEX(tolower(word[level]));
        if (!pCrawl->children[index]) {
            pCrawl->children[index] = getNode();
        }
        pCrawl = pCrawl->children[index];
    }

    pCrawl->count++;
}

// Computes the number of occurrences of the word
int numberOfOccurances(struct Trie *pTrie, char *word) {
    int level;
    int length = strlen(word);
    int index;
    struct TrieNode *pCrawl = pTrie->root;

    for (level = 0; level < length; level++) {
        index = CHAR_TO_INDEX(tolower(word[level]));
        if (!pCrawl->children[index])
            return 0;
        pCrawl = pCrawl->children[index];
    }

    return (pCrawl != NULL && pCrawl->count > 0) ? pCrawl->count : 0;
}

void deallocate(struct TrieNode *node) {
    if (node != NULL) {
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            if (node->children[i] != NULL) {
                deallocate(node->children[i]);
            }
        }
        free(node);
    }
}

struct Trie * deallocateTrie(struct Trie *pTrie) {
    if (pTrie != NULL) {
        deallocate(pTrie->root);
        free(pTrie);
    }
    return pTrie;
}

// This function should ideally load words from a file, not yet implemented
int readDictionary(const char *filePath, char *words[256]) {
    FILE *file = fopen(filePath, "r");
    if (file == NULL) {
        fprintf(stderr, "Unable to open file\n");
        return 0;
    }

    int wordCount = 0;
    char buffer[100]; // Temporary buffer for reading words
    while (fgets(buffer, sizeof(buffer), file) && wordCount < 256) {
        size_t len = strlen(buffer);
        if (buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
        // Allocate memory and copy the word into the array
        words[wordCount] = malloc(strlen(buffer) + 1);
        strcpy(words[wordCount], buffer);
        wordCount++;
    }

    fclose(file);
    return wordCount;
}
int main(void)
{
	char *inWords[256];
	
	//read the number of the words in the dictionary
	int numWords = readDictionary("dictionary.txt", inWords);
	for (int i=0;i<numWords;++i)
	{
		printf("%s\n",inWords[i]);
	}
	
	struct Trie *pTrie = createTrie();
	for (int i=0;i<numWords;i++)
	{   
		insert(pTrie, inWords[i]);
	}
	// parse lineby line, and insert each word to the trie data structure
	char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
	for (int i=0;i<5;i++)
	{
		printf("\t%s : %d\n", pWords[i], numberOfOccurances(pTrie, pWords[i]));
	}
	pTrie = deallocateTrie(pTrie);
	if (pTrie != NULL)
		printf("There is an error in this program\n");
	return 0;
}