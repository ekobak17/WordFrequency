/*
 * Ellie Kobak
 * usage: ./wordfreq [OPTION] ... [FILE]...
 */

#include "ADTs/hashcskmap.h"
#include "ADTs/arraylist.h"
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "ADTs/stringADT.h"
#include "sort.h"
#include <stdlib.h>

typedef struct my_file {
	FILE *fp;
	char name[256];
} MyFile;

int valcmp(void *x1, void *x2) {     
					MEntry *m1 = (MEntry *)x1;     
					MEntry *m2 = (MEntry *)x2;    
					return (long)m2->value - (long)m1->value; 
} 
int keycmp(void *x1, void *x2) { //key = word  
					MEntry *m1 = (MEntry *)x1;     
					MEntry *m2 = (MEntry *)x2;     
					return strcmp((char *)m1->key, (char *)m2->key); 
}
int valcmpR(void *x1, void *x2) {     
					MEntry *m1 = (MEntry *)x1;     
					MEntry *m2 = (MEntry *)x2;     
					return (long)m1->value - (long)m2->value; 
} 
int keycmpR(void *x1, void *x2) { //key = word  
					MEntry *m1 = (MEntry *)x1;     
					MEntry *m2 = (MEntry *)x2;     
					return strcmp((char *)m2->key, (char *)m1->key); 
}


void wordfreq(int acase, int fcase, int icase, int lcase, int pcase, MyFile files[], int numFiles) {
	long i = 0;
	long j;
	char *st = "";
	int space = ' ';
	char line[BUFSIZ];
	char *word;
	long freq;
	long l;
	
	const CSKMap *m = HashCSKMap(0L, 0.0, NULL);
	MEntry **mes;

	for(; i < numFiles; i++) {
		const ArrayList *aryptr;
		const String *strptr = String_create(st);
		while(fgets(line, BUFSIZ, files[i].fp) != NULL) {
			strptr->append(strptr, line);
			if(lcase == 1) {
				strptr->lower(strptr);
			}
			if(pcase == 1) {
				strptr->translate(strptr, "[:punct:]", space);	
				}
			aryptr = strptr->split(strptr, "");
			if(aryptr == NULL)
				continue;
			for(j = 0; j < aryptr->size(aryptr); j++) {
				(void) aryptr->get(aryptr, j, (void **)&word);
				if(m->containsKey(m, word)) {
					m->get(m, word, (void **)&freq);
					m->put(m, word, (void *)freq+1);
					//printf("word: %s, freq: %ld\n", word, freq);
				}
				else
					m->put(m, word, (void *)1L);
				
			}
			strptr->clear(strptr);
			aryptr->destroy(aryptr);
		}
		strptr->destroy(strptr);
		if (files[i].fp != stdin)
			fclose(files[i].fp);
	}	
	mes = m->entryArray(m, &l);

	if(icase == 1) {
		if (acase)
			sort((void **)mes, l, keycmpR);
		if (fcase)
			sort((void **)mes,l, valcmpR);
	}
	else {
		if(acase == 1) {
			sort((void **)mes, l, keycmp);
		}
		if(fcase == 1) { //sort high=low
			sort((void **)mes, l, valcmp);
		}
	}
	for(long int k=0; k < l; k++) {
		printf("%8ld %s\n", (long)mes[k]->value, mes[k]->key); 
	}
	free(mes);
	m->destroy(m);
}

int main(int argc, char *argv[]) {
	int i;
	int acase = 0, fcase = 0, icase = 0, lcase = 0, pcase = 0;
	int numFiles = 0;
	MyFile files[100];

	for(i = 1; i < argc; i++) {
		if (argv[i][0] != '-')
			break;
		else {
			for (int j = 1; argv[i][j] != '\0'; j++) {
				if (argv[i][j] == 'a')
					acase = 1;
				else if (argv[i][j] == 'f')
					fcase = 1;
				else if(argv[i][j] == 'i')
					icase = 1;
				else if (argv[i][j] == 'l')
					lcase = 1;
				else if (argv[i][j] == 'p')
					pcase = 1;
				else {
					fprintf(stderr, "Illegal flag\n");
						return 1;
				}
			}
		}
	}
	if (icase && (!fcase && !acase)) {
		fprintf(stderr, "Ilegal to run -i without -a or -f\n");
			return 1;
	}
	if (acase && fcase) {
		fprintf(stderr, "Ilegal to combine -a and -f\n");
		return 1;
	}
	for(; i < argc; i++) {
		FILE *fp = fopen(argv[i], "r");
		if (fp == NULL) {
			fprintf(stderr, "Unable to open file: %s\n", argv[i]);
			return 1;
		}
		files[numFiles].fp = fp;
		strcpy(files[numFiles].name, argv[i]);
		numFiles++;
	}
	if (!numFiles) {
		files[numFiles].fp =stdin;
		strcpy(files[numFiles++].name, "");
	}
	wordfreq(acase, fcase, icase, lcase, pcase, files, numFiles);

	return 0;
}
