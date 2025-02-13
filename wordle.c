#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <errno.h>

#define max            5195

#define ResultGreen    1
#define ResultYellow   2
#define ResultRed      4


#define ValOk          0
#define ValBadInput    1
#define ValNoSuchWord  2

#define ClrGreen       "\033[0;32m"
#define ClrYellow      "\033[0;33m"
#define ClrRed         "\033[0;31m"
#define ClrStop        "\033[0m"

typedef char Result;
typedef char ValResult;

ValResult validator(char*);
char *read_line(void);
void game_loop(char *);
void seed(void);
char *random_word(int);
int main(int, char**);
int read_file(char*);
bool is_in(char, char*);
void prompt(char*);
void print_results(Result*, char*, char*);
Result *cw(char*, char*);
Result cc(char, int, char*);

static char words[max][5];
bool continuation, win;
bool corrects[5];
int rounds;

int main(int argc, char **argv) {
	int n;
	char *p;

	seed();

	corrects[0] = false;
	corrects[1] = false;
	corrects[2] = false;
	corrects[3] = false;
	corrects[4] = false;
	
	rounds = 0;
	win = false;
	continuation = true;

	
	n = read_file("wordlist.txt");
	assert(!(n < 0));
	p = random_word(n);


	while(continuation) {
		game_loop(p);
	}

	printf("Correct word was: %s\n", p);
	if (win)
		printf("Congratulations you won the game.\n");
	else
		printf("You lost game over!\n");

	return 0;
}

ValResult validator(char *word) {
	int n, i;
	bool ret;

	bool strcmp_(char *str1, char *str2) {
		int s, i;

		s = 0;
		for (i=0; i<5; i++) {
			if (str1[i] == str2[i])
				s++;
		}

		return (s == 5);
	}
	
	n = strlen(word);
	if (n != 5)
		return ValBadInput;

	ret = false;
	n = 0;
	for (i=0; i<max; i++) {
		if (strcmp_(words[i], word)) {
			ret = true;
			break;
		}
	}

	if (ret)
		return ValOk;
	else
		return ValNoSuchWord;
}


char *read_line() {
	static char buf[max];
	int size;

	memset(buf, 0, max);
	fgets(buf, max-1, stdin);

	size = strlen(buf);
	assert(size > 0);
	size--;
	buf[size] = 0;

	printf("%s\n", buf);
	return buf;
}

void game_loop(char *correct_word) {
	char *input;
	Result *res;
	ValResult valres;
	int i, c;
	char copy_word[5];
	strncpy(copy_word, correct_word, 5);

	printf("DEBUG: Correct word: '%s'\n", correct_word);
	prompt(correct_word);
	
	input = read_line();
	valres = validator(input);
	
	switch (valres) {
	case ValBadInput:
		printf("%s\n", "Bad input");
		return;
	case ValNoSuchWord:
		printf("%s\n", "No such word - only 5 letter English words are permitted");
		return;
	default:
		break;
	}

	
	res = cw(input, copy_word);
	for (i=c=0; i<5; i++) {
		if (corrects[i]) {
			c++;
		}
	}

	for (i=c=0; i<5; i++) {
		if (corrects[i]) {
			c++;
		}
	}
	rounds++;

	print_results(res, input, correct_word);

	if (c==5) {
		win = true;
		continuation = false;
		return;
	}
		

	if (rounds > 4) {
		win = false;
		continuation = false;
		return;
	}
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



void prompt(char *correct_word) {
		printf("\n\n%d>", 5-rounds);
		fflush(stdout);

	
}

void print_results(Result *res, char *guess, char *correct) {
	int i;

	for (i = 0; i < 5; ++i) {
		switch (res[i]) {
			case ResultGreen:

				printf("%s%c%s", ClrGreen, guess[i], ClrStop);
				break;
			case ResultYellow:

				printf("%s%c%s", ClrYellow, guess[i], ClrStop);
				break;
			case ResultRed:

				printf("%s%c%s", ClrRed, guess[i], ClrStop);
				break;
			default:
				printf("Unknown: %d\n", res[i]);
				break;
		}
	}
	printf("\n");
}

/*
Checks all letters in the user's guess to the
correct word
*/
Result *cw(char *guess, char *word) {
	static Result res[5];
	int i;

	for (i = 0; i <5; ++i)
		res[i] = cc(guess[i], i, word);

	return res;
}

/*
Checks a single individual letter in the user's guess
to the correct word
*/
Result cc(char guess, int i, char *word) {
	char correct = word[i];
	
	if (guess == correct) {
		corrects[i] = true;
		word[i] = '-';
		return ResultGreen;
	}
	else if (is_in(guess, word)) {
		corrects[i] = false;
		return ResultYellow;
	}

	corrects[i] = false;
	return ResultRed;
}

bool is_in(char c, char *str) {
	int i, size;

	size = strlen(str);
	
	for (i = 0; i < size; i++)
		if (c == str[i]) {
			str[i] = '-';
			return true;
		}

	return false;
}
