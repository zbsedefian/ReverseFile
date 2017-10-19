/* 
	Name: Zachary Sedefian
    Unix login ID: zs122278@itsunix.albany.edu
    Instructor: Vaidyanathan Subramanian
    Lab section: Wednesday 4:15-5:10
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define NUMARG 3
#define INFILEARG 1
#define OUTFILEARG 2

int countWords(const char *sentence);

/* Reverses each line in a txt file and outputs to new file */
int main(int argc, char *argv[]){
	
	/* File pointers */
	FILE *finp;
	FILE *foutp;

	/* Three levels of strings: word < line < text. 
	   Both token variables to use with strtok splitting the text. */
	char **holdWords = NULL, *holdLine = NULL, *holdText = NULL,
		 *lineToken = NULL, *wordToken = NULL;
	int stringSize = 0, totalStrings, i;
	size_t size = 0;

	/* Add .txt extension */
	int inFileNameSize = strlen(argv[INFILEARG]);
	int outFileNameSize = strlen(argv[OUTFILEARG]);

	char inFileName[inFileNameSize+4]; /* add 4 to have room for ".txt" */
	char outFileName[outFileNameSize+4];

	strcat(inFileName, argv[INFILEARG]);
	strcat(inFileName, ".txt");
	strcat(outFileName, argv[OUTFILEARG]);
	strcat(outFileName, ".txt");

	/* Check for errors in argument number and opening files. */
	if(argc != NUMARG){
		printf("You have to put the input and output files after the program name.\n"); fflush(stdout);
		return(1);
	}

	if( (finp = fopen(inFileName, "r")) == NULL ){
		printf("Couldn't open %s for reading.\n", inFileName); fflush(stdout);
		return(1);
	}

	if( (foutp = fopen(outFileName, "w")) == NULL){
		printf("Couldn't open %s for writing.\n", outFileName); fflush(stdout);
		return(1);
	}


	/* Count char total in input file and allocate size in holdText. */
	fseek(finp, 0, SEEK_END); /* Bring pointer to end of input file */
	size = ftell(finp); /* Return length of file */
	rewind(finp); /* Bring pointer back to beginning */

	/* Allocate space for holdText */
	holdText = malloc((size + 1) * sizeof(*holdText));
	fread(holdText, size, 1, finp); /* Insert file data in holdText */
	holdText[size] = '\0';
	
	/* Break holdText up into lines */
	for(lineToken = strtok(holdText, "\n"); lineToken != NULL; 
		lineToken = strtok(lineToken + strlen(lineToken) + 1, "\n")){

		/* Dynamically allocate space for each line */
		holdLine = realloc(holdLine, strlen(lineToken) + 1);
		strcpy(holdLine, lineToken);

		/* Get number of strings in the line */
		totalStrings = countWords(holdLine);

		/* Allocate space for words */
		holdWords = realloc(holdWords, totalStrings * sizeof(char **));

		/* Break holdLine up into words, store in holdWords */
		i = 0;
		for(wordToken = strtok(holdLine, " "); wordToken != NULL; 
		    wordToken = strtok(wordToken + strlen(wordToken) + 1, " ")){
			holdWords[i] = malloc(strlen(wordToken) + 1);
			strcpy(holdWords[i], wordToken);
			i++;
		}
		
		/* Put the words into the output file in opposite order */
		for(i = totalStrings-1; i >= 0; i--){
			fputs(holdWords[i], foutp);
			fputc(' ', foutp);
		}
		fputc('\n', foutp); 
	}


	// Close files, then free memory.
	if (fclose(finp) == EOF)
		printf("Error closing input file.\n"); fflush(stdout);

	if (fclose(foutp) == EOF)
		printf("Error closing output file.\n"); fflush(stdout);

	free(holdText);
	free(holdWords);

	puts("Finished.");
	return 0;
}


int countWords(const char *sentence){
    int total = 0, contains = 0;
    const char *s = sentence;

    do switch(*s) {
        case '\0': case ' ': case '\t':
            if (contains) { 
            	contains = 0; 
            	total++; 
            }
            break;
        default: contains = 1;
    } while (*s++);

    return total;
}