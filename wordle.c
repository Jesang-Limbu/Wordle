#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define ResultGreen    1
#define ResultYellow   2
#define ResultRed      4
#define max            5195

typedef char Result;

void seed(void);
char *random_word(int);
int main(int, char**);
int read_file(char*);
bool is_in(char, char*);
void Example_print_results(Result*);
Result *cw(char*, char*);
Result cc(char, int, char*);

static char words[max][5];

int main(int argc, char **argv) {
	int n, x;
	srand(time(NULL));


	n = read_file("wordlist.txt");
	if (n < 0)
		printf("Failed\n");
	else {
		printf("n: %d\n", n);
		char correct_word[5];
		int x;
		printf("Random word: %s\n", random_word(n));
		
	}
	return 0;
}

void seed() {
	int x;

	x = getpid();
	srand(x);

	return;
}

char *random_word(int m) {
	static char p[8];
	int x;

	x = rand() % m;

	p[0] = words[x][0];
	p[1] = words[x][1];
	p[2] = words[x][2];
	p[3] = words[x][3];
	p[4] = words[x][4];
	p[5] = 0;

	return p;
}

/* 
This function reads the text file of the word list
and returns it as an array of strings
*/


int read_file(char *filename) {
	char buf[8];
	int i, size;

	FILE *fd;

	fd = fopen(filename, "r"); 
	if (!fd) {
		perror("fopen");
		return -1;
	}

	i = 0;
	memset(buf, 0, 8);

	while (fgets(buf, 7, fd)) {
		size = strlen(buf);

		if (size < 1) {
			memset(buf, 0, 8);
			continue;
		}

		buf[size] = 0;
		size--;

		if (size != 5) {
			memset(buf, 0, 8);
			continue;
		}

		words[i][0] = buf[0];
		words[i][1] = buf[1];
		words[i][2] = buf[2];
		words[i][3] = buf[3];
		words[i][4] = buf[4];

		memset(buf, 0, 8);
		i++;

		if (max <= i)
			break;
	}

	fclose(fd);
	return i;
}

/*
This function checks if a character c, is in the string str
*/
bool is_in(char c, char *str) {
	int i, size;

	size = strlen(str);
	
	for (i = 0; i < size; ++i)
		if (c == str[i]) {
			return true;
		}

	return false;
}

/*
This function is for testing only, and will not be in
the main code.
This function gives a colour to each character in the string
res
*/
void Example_print_results(Result *res) {
	int i;

	for (i = 0; i < 5; ++i) {
		switch (res[i]) {
			case ResultGreen:
				printf("%s\n", "Green");
				break;
			case ResultYellow:
				printf("%s\n", "Yellow");
				break;
			case ResultRed:
				printf("%s\n", "Red");
				break;
			default:
				printf("Unknown: %d\n", res[i]);
				break;
		}
	}
}

/*
This function checks each character of the string guess to
see if the character is either in the right place, in the word 
but not in the right place or not in the word at all, and
give it a corresponding Result type (Red, Yellow, or Green).
*/
Result *cw(char *guess, char *word) {
	static Result res[5];
	int i;

	for (i = 0; i <5; ++i)
		res[i] = cc(guess[i], i, word);

	return res;
}

/*
This function checks if the character is in the right place,
not in the right place but in the word, or not in the word
at all.
*/
Result cc(char guess, int i, char *word) {
	char correct = word[i];
	
	if (guess == correct)
		return ResultGreen;
	else if (is_in(guess, word))
		return ResultYellow;

	return ResultRed;
}
