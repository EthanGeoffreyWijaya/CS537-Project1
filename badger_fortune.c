#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int maxF = 0;
int numF = 0;

char **readFortunes (FILE *file) {
	char temp[10];
	if (fgets(temp, 10, file) == NULL) return NULL;
	numF = atoi(temp);
	fgets(temp, 10, file);
	maxF = atoi(temp);
	char storage[maxF];
	char quote[maxF];
	//printf("%d, %d\n", numF, maxF);	
	char *fortunes[numF];
	char **fstart = fortunes;
	int start = 1;
	fgets(temp, 3, file);
	//printf("%s", temp);
	for (int i = 0; i < numF; i++) {
		while (fgets(storage, maxF+1, file) != NULL && strcmp(storage, "%\n") != 0) {
			//printf("st: %s", storage);
			if (start) {
				strcpy(quote, storage);
				start = 0;
			} else {
				strcat(quote, storage);
			}
			//printf("quote: %s", quote);
		} 
		//printf("quote: %s\n", quote);
		fortunes[i] = (char*)malloc(strlen(quote) + 1);
		strcpy(fortunes[i], quote);
		//printf("final: %s", *fortunes);
		//printf("%ld\n", strlen(quote));
		memset(quote, 0, maxF);
		memset(storage, 0, maxF);
	/*
	printf("%d\n", i);
	for (int j = 0; j < i + 1; j++) {
		//printf("q: %s\n", *ftest);
		//ftest += sizeof(char*);
		printf("q: %s\n", fortunes[j]);
	}*/
	
	}
	return fstart;
}

void freeArray (char** arr) {
	for (int i = 0; i < numF; i++) {
		free(arr[i]);
	}
}

int main (int argc, char *argv[]) {
	int indexF = 0;
	int indexB = 0;
	int indexN = 0;
	int indexO = 0;
	//ERROR CASE 1
	if (argc < 5) {
		printf("USAGE: \n\tbadger-fortune -f <file> -n <number> (optionally: -o <output file>) \n\t\t OR \n\tbadger-fortune -f <file> -b <batch file> (optionally: -o <output file>)\n");
		return 1;
	}
	//ERROR CASE 2
	for (int z = 1; z < argc; z += 2) {
		/*
		if (strcmp(argv[z], "-f") != 0 && strcmp(argv[z], "-n") != 0 && strcmp(argv[z], "-b") != 0 && strcmp(argv[z], "-o") != 0) {
			printf("ERROR: Invalid Flag Types\n");
			return 1;
		}*/

		if (strcmp(argv[z], "-f") == 0) {//ERROR CASE 13
			indexF = z;
			if (*argv[z+1] == '-') {
				printf("ERROR: No fortune file was provided\n");
			}
		} else if (strcmp(argv[z], "-b") == 0) {
			if (indexN) {//ERROR CASE 5
				printf("ERROR: You can't use batch mode when specifying a fortune number using -n\n");
				return 1;
			}
			indexB = z;
		} else if (strcmp(argv[z], "-n") == 0) {
			if (indexB) {// ERROR CASE 6
				printf("ERROR: You can't specify a specific fortune number in conjunction with batch mode\n");
				return 1;
			}
			indexN = z;
		} else if (strcmp(argv[z], "-o") == 0) {
			indexO = z;
		} else {//ERROR CASE 2
			printf("ERROR: Invalid Flag Types\n");
			return 1;
		}
	}
	//ERROR CASE 13
	if (indexF == 0) {
		printf("ERROR: No fortune file was provided\n");
		return 1;
	}
	
	FILE *file = fopen(argv[indexF+1], "r");
	
	//ERROR CASE 3
	if (file == NULL) {
		printf("ERROR: Can't open fortune file\n");
		return 1;
	}
	
	char **fortunes = readFortunes(file);
	
	//ERROR CASE 8
	if (fortunes == NULL) {
		printf("ERROR: Fortune File Empty\n");
		return 1;
	}

	fclose(file);
	/*
	for (int i = 0; i < numF; i++) {
		//printf("%s", *(fortunes + (i * sizeof(char*))));
		printf("%s", fortunes[i]);
	}*/
	
	char selected[numF * maxF];
	if (indexB) {//////////BATCH MODE//////////
		FILE *batch = fopen(argv[indexB+1], "r");
		
		//ERROR CASE 4
		if (batch == NULL) {
			printf("ERROR: Can't open batch file\n");
			return 1;
		}

		char store[10];
		store[0] = 0;
		int start = 2;
		int index = 0;
		while (fgets(store, 10, batch) != NULL) {
			if (start == 2) start = 1;
			index = atoi(store) - 1;
			memset(store, 0, 10);

			if (index < 0) {//ERROR CASE 11
				if (start == 0) {
					strcat(selected, "ERROR: Invalid Fortune Number\n\n");
				} else {
					strcpy(selected, "ERROR: Invalid Fortune Number\n\n");
				}
				continue;
			} else if (index >= numF) {// ERROR CASE 12
				if (start == 0) {
					strcat(selected, "ERROR: Invalid Fortune Number\n\n");
				} else {
					strcpy(selected, "ERROR: Invalid Fortune Number\n\n");
				}
				continue;
			}

			if (start) {
				strcpy(selected, fortunes[index]);
				start = 0;
			} else {
				strcat(selected, fortunes[index]);
			}
			strcat(selected, "\n\n");
		}

		//ERROR CASE 7
		if (start == 2) {
			printf("ERROR: Batch File Empty\n");
			return 1;
		}

		fclose(batch);
	} else if (indexN) {//////////NUMBER MODE//////////
		int index = atoi(argv[indexN+1]);

		if (index <= 0) {//ERROR CASE 9
			printf("ERROR: Invalid Fortune Number\n");
			return 1;
		} else if (index > numF) {//ERROR CASE 10
			printf("ERROR: Invalid Fortune Number\n");
			return 1;
		}

		strcpy(selected, fortunes[index-1]);
	} 
	
	if (indexO) {//////////OUTPUT FILE//////////
		FILE *output = fopen(argv[indexO+1], "w");
		fputs(selected, output);	
	} else {
		printf("%s", selected);
	}

	freeArray(fortunes);
	
	return 0;
}
