#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Constants as defined by the challenge */
#define WORDS_FILE_PATH "enable1.txt"
#define MAX_BONUS_WORDS 5
#define WORDS_WITH_MAX_BONUS_WORDS 28

/* A hash code in HashTable */
typedef struct
{
	/* The hash code */
	uint32_t Value;
	
	/* If the hash code has been set. This is necessary because there are no */
	/* hash code values (e.g. 0) that never occur. */
	uint32_t IsSet;
} HashCode;

/* A hash table of words */
typedef struct
{
	/* Total length of Hashes and Words */
	/* Greater than or equal to the number of stored elements */
	uint32_t Length;
	
	/* Array of hash codes. Indices correspond with Words. */
	HashCode* HashCodes;
	
	/* Array of pointers to stored words. Indices correspond with Hashes. */
	const char** Words;
} HashTable;

/* Hash the characters of a string with 32-bit FNV-1a */
uint32_t hashString(const char* str)
{
	uint32_t hashCode = 2166136261u;
	char cur;
	while ((cur = *str))
	{
		hashCode ^= (uint32_t)cur;
		hashCode *= 16777619u;
		str++;
	}
	return hashCode;
}

/* Hash the characters of a string with 32-bit FNV-1a */
/* The string has a one character "gap" that is not hashed*/
uint32_t hashOneCharGapString(const char* start, const char* gap)
{
	/* Hash up to the gap */
	uint32_t hashCode = 2166136261u;
	while (start < gap)
	{
		hashCode ^= (uint32_t)(*start);
		hashCode *= 16777619u;
		start++;
	}
	
	/* Skip the gap */
	start++;
	
	/* Hash until the end */
	char cur;
	while ((cur = *start))
	{
		hashCode ^= (uint32_t)cur;
		hashCode *= 16777619u;
		start++;
	}
	
	return hashCode;
}

/* Insert the words of a string into a hash table. */
/* Words are separated by a NUL character. */
/* The string is terminated by two NUL characters. */
/* Only words at least (MAX_BONUS_WORDS-1) long are inserted. */
/* The hash table's length must be at least the number of words */
void insertWordsIntoHashTable(const char* words, HashTable* hashTable)
{
	/* Loop over all the words in the string */
	while (*words)
	{
		/* Only insert if it's long enough */
		uint32_t curLength = (uint32_t)strlen(words);
		if (curLength >= MAX_BONUS_WORDS - 1)
		{
			uint32_t curHashCode = hashString(words);
			
			/* Insert into the hash table */
			/* Use linear probing to resolve collisions */
			uint32_t index = curHashCode % hashTable->Length;
			for (;;)
			{
				/* This slot is vacant */
				if (!hashTable->HashCodes[index].IsSet)
				{
					hashTable->HashCodes[index].Value = curHashCode;
					hashTable->HashCodes[index].IsSet = 1;
					hashTable->Words[index] = words;
					break;
				}
				
				/* Go to the next index and wrap around */
				index = (index + 1) % hashTable->Length;
			}
		}
		
		/* Skip the word and its NUL terminator */
		words += curLength + 1;
	}
}

/* Check if the characters of a string equal the characters of another string */
/* The first string has a one character "gap" that isn't checked */
/* Character comparison is case-sensitive */
/* If the strings aren't equal, zero is returned. Otherwise, non-zero. */
int32_t isOneCharGapGapStringEqual(
	const char* start,
	const char* gap,
	const char* target)
{
	/* Compare up to the gap */
	while (start < gap)
	{
		if (*start != *target)
		{
			return 0;
		}
		start++;
		target++;
	}
	
	/* Skip the gap */
	start++;
	
	/* Compare up to the end */
	char cur;
	while ((cur = *start))
	{
		if (cur != *target)
		{
			return 0;
		}
		start++;
		target++;
	}
	
	/* The target needs to also be at the end */
	return !*target;
}

/* Print an array of strings */
/* Each is in quotes separated by a comma and space. */
/* For example: "A", "B", "C" */
void printStrings(const char** strings, uint32_t numStrings)
{
	for (uint32_t i = 0; i < numStrings; ++i)
	{
		if (i)
		{
			printf(", ");
		}
		printf("\"%s\"", strings[i]);
	}
}

/* Count the number of words in a string. */
/* Words are separated by a NUL character. */
/* The string is terminated by two NUL characters. */
uint32_t countNumWords(const char* words)
{
	uint32_t numWords = 0;
	const char* begin = words;
	while (*begin)
	{
		begin += strlen(begin) + 1;
		numWords++;
	}
	return numWords;
}

/* Read the input words file */
/* Null is returned upon error. Otherwise a pointer returned by malloc. */
/* Words are separated by a NUL character. */
/* The string is terminated by two NUL characters. */
char* readWordsFile()
{
	/* Open the words file */
	FILE* file = fopen(WORDS_FILE_PATH, "rb");
	if (!file)
	{
		fprintf(stderr, "Couldn't open " WORDS_FILE_PATH "\n");
		return 0;
	}
	
	/* Seek to the end */
	if (fseek(file, 0, SEEK_END))
	{
		fprintf(stderr, "Couldn't determine size of " WORDS_FILE_PATH "\n");
		return 0;
	}
	
	/* Get the offset from the start at the current (end) point */
	int32_t fileLengthSigned = (int32_t)ftell(file);
	if (fileLengthSigned == EOF)
	{
		fprintf(stderr, "Couldn't determine size of " WORDS_FILE_PATH "\n");
		return 0;
	}
	uint32_t fileLength = (uint32_t)fileLengthSigned;
	
	/* Seek back to the beginning */
	if (fseek(file, 0, SEEK_SET))
	{
		fprintf(stderr, "Couldn't determine size of " WORDS_FILE_PATH "\n");
		return 0;
	}
	
	/* Allocate enough room for the whole file plus a NUL terminator */
	char* words = (char*)malloc(fileLength + 1);
	
	/* Read the whole file */
	if (fread(words, 1, fileLength, file) < fileLength)
	{
		fprintf(stderr, "Couldn't read " WORDS_FILE_PATH "\n");
		return 0;
	}
	
	/* Set the NUL terminator */
	words[fileLength] = 0;
	
	/* Convert all newline characters to NUL characters */
	for (uint32_t i = 0; i < fileLength; ++i)
	{
		if (words[i] == '\n')
		{
			words[i] = 0;
		}
	}
	
	/* Close the file */
	fclose(file);
	
	/* Return the file's contents */
	return words;
}

/* The main part of the challenge */
/* Check if removing any one character of 'full' results in 'partial' */
/* Zero if this isn't possible. Non-zero otherwise. */
int32_t funnel(const char* full, const char* partial)
{
	/* Number of character mismatches so far */
	int32_t numMismatches = 0;
	
	/* Current values of 'full' and 'partial' */
	char fullCur = *full;
	char partialCur = *partial;
	
	/* Loop over the strings */
	for (;;)
	{
		/* Found the end of 'full' */
		if (!fullCur)
		{
			/* Also found the end of 'partial' and there was one mismatch */
			return !partialCur && numMismatches;
		}
		
		/* Found the end of 'partial' */
		if (!partialCur)
		{
			/* Also found the end of 'full' or there were no mismatches and */
			/* we're at the last character of 'full' so the last character is */
			/* the mismatch. */
			return !fullCur || (!numMismatches && !*(full + 1));
		}
		
		/* Found a mismatch */
		if (fullCur != partialCur)
		{
			/* Already had a mismatch. Can't have two. */
			if (numMismatches)
			{
				return 0;
			}
			
			/* Count the mismatch */
			numMismatches++;
		}
		/* No mismatch */
		else
		{
			/* Go to the next character of 'partial' */
			partial++;
			partialCur = *partial;
		}
		
		/* Go to the next character of 'full' */
		full++;
		fullCur = *full;
	}
}

/* Call funnel with 'full' and 'partial' and print the results */
void testFunnel(const char* full, const char* partial)
{
	printf("funnel(\"%s\", \"%s\") => ", full, partial);
	printf(funnel(full, partial) ? "true\n" : "false\n");
}

/* The challenge's first bonus */
/* Find all words that can be made by removing one character from 'word' */
/* Possible words are provided via the hash table */
/* Pointers to found words are appended to the 'outFound' array */
/* The number of found words is assigned to 'outNumFound' */
void bonus(
	const char* word,
	HashTable* hashTable,
	const char** outFound,
	uint32_t* outNumFound)
{
	/* Keep track of found words */
	uint32_t numFound = 0;
	const char* found[MAX_BONUS_WORDS];
	
	/* Try using each character as the 'gap' */
	for (const char* gap = word; *gap; ++gap)
	{
		uint32_t hash;
		uint32_t curIndex;
		
		/* Skip if we've already found the word */
		for (uint32_t i = 0; i < numFound; ++i)
		{
			if (isOneCharGapGapStringEqual(word, gap, found[i]))
			{
				goto continueGapLoop;
			}
		}
		
		hash = hashOneCharGapString(word, gap);
		
		/* Use linear probing to find the first slot with a matching hash */
		curIndex = hash % hashTable->Length;
		for (;;)
		{
			HashCode curHash = hashTable->HashCodes[curIndex];
			
			/* We've gone beyond the possible hits */
			if (!curHash.IsSet)
			{
				goto continueGapLoop;
			}
			
			/* Found the matching hash code */
			if (hash == curHash.Value)
			{
				break;
			}
			
			/* Go to the next slot and wrap around */
			curIndex = (curIndex + 1) % hashTable->Length;
		}
		
		/* Use linear probing to find the word */
		for (;;)
		{
			/* We've gone beyond the possible hits */
			if (hashTable->HashCodes[curIndex].Value != hash)
			{
				break;
			}
			
			/* Check if this word matches exactly */
			const char* curWord = hashTable->Words[curIndex];
			if (isOneCharGapGapStringEqual(word, gap, curWord))
			{
				/* Append the found word to the output array */
				*outFound = curWord;
				outFound++;
				found[numFound] = curWord;
				
				numFound++;
				break;
			}
			
			/* Go to the next slot and wrap around */
			curIndex = (curIndex + 1) % hashTable->Length;
		}
		continueGapLoop:;
	}
	
	/* Return the number of words found */
	*outNumFound = numFound;
}

/* Call bonus with 'word' and 'hashTable' and print the results */
void testBonus(const char* word, HashTable* hashTable)
{
	printf("bonus(\"%s\") => [", word);
	
	const char* found[MAX_BONUS_WORDS];
	uint32_t numFound;
	bonus(word, hashTable, (const char**)&found, &numFound);
	printStrings(found, numFound);
	
	printf("]\n");
}

/* The second bonus of the challenge */
/* Find all words with MAX_BONUS_WORDS results */
/* Words are separated by a NUL character. */
/* The string is terminated by two NUL characters. */
void bonus2(const char* words, HashTable* hashTable)
{
	/* Iterate over all words */
	while (*words)
	{
		/* The word has to be long enough to possibly have enough results */
		uint32_t curLength = (uint32_t)strlen(words);
		if (curLength >= MAX_BONUS_WORDS)
		{
			/* Get results for the word */
			const char* found[MAX_BONUS_WORDS];
			uint32_t numFound;
			bonus(words, hashTable, (const char**)&found, &numFound);
			
			/* If it had the maximum results, print it */
			if (numFound == MAX_BONUS_WORDS)
			{
				printf("bonus2(\"%s\") => [", words);
				printStrings(found, numFound);
				printf("]\n");
			}
		}
		
		/* Skip the word and its NUL terminator */
		words += curLength + 1;
	}
}

int main(int argc, char** argv)
{
	/* Command line parameters are not used */
	(void)argc;
	(void)argv;
	
	/* Test the specified funnel words */
	testFunnel("leave", "eave");
	testFunnel("reset", "rest");
	testFunnel("dragoon", "dragon");
	testFunnel("eave", "leave");
	testFunnel("sleet", "lets");
	testFunnel("skiff", "ski");
	printf("\n");
	
	/* Read words from the input file */
	char* words = readWordsFile();
	
	/* Count the number of words in the input file */
	uint32_t numWords = countNumWords(words);
	
	/* Create a hash table */
	/* Size it big enough for all the words plus extra to reduce collisions */
	HashTable hashTable;
	hashTable.Length = numWords * 2;
	hashTable.HashCodes = (HashCode*)calloc(
		hashTable.Length,
		sizeof(HashCode));
	hashTable.Words = (const char**)calloc(
		hashTable.Length,
		sizeof(const char*));
	
	/* Insert all the words into the hash table */
	insertWordsIntoHashTable(words, &hashTable);
	
	/* Test the specified bonus words */
	testBonus("dragoon", &hashTable);
	testBonus("boats", &hashTable);
	testBonus("affidavit", &hashTable);
	printf("\n");
	
	/* Time the second bonus */
	clock_t before = clock();
	bonus2(words, &hashTable);
	clock_t after = clock();
	clock_t duration = after - before;
	double durationMs = ((double)(duration * 1000)) / CLOCKS_PER_SEC;
	printf("Bonus 2 took %f ms\n", durationMs);
	
	/* Free allocated memory */
	free(words);
	free(hashTable.HashCodes);
	free(hashTable.Words);
}
