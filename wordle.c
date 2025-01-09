#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define ResultGreen    1
#define ResultYellow   2
#define ResultRed      4

typedef char Result;

struct s_words {
	char **arr;
	int n;
};

typedef struct s_words Words;

int main(int, char**);
Words read_file(char*, int);
bool is_in(char, char*);
void Example_print_results(Result*);
Result *cw(char*, char*);
Result cc(char, int, char*);

int main(int argc, char **argv) {
	char *correct, *guess;
	Result *res;
	if (argc < 3) {
		fprintf(stderr, "Usage: %s CORRECTWORD GUESSWORD\n", argv[0]);
		return -1;	
	}

	correct = argv[1];
	guess = argv[2];

	res = cw(guess, correct);
	Example_print_results(res);
}

/* 
This function reads the text file of the word list
and returns it as an array of strings
*/
Words read_file(char *filename, int max) {
	char buf[8];
	int i, size;
	FILE *fd;
	char **ret;
	Words words;

	fd = fopen(filename, "r"); 
	if (!fd) {
		perror("fopen");
		words = (Words) {
			.arr = (char **)0,
			.n = 0
		};
		return words;
	}

	size = max * 5;
	ret = (char **)malloc(size);
	if (!ret) {
		fclose(fd);
		perror("malloc");
		words = (Words) {
			.arr = (char **)0,
			.n = 0
		};
		return words;
	}

	i = 0;
	memset(buf, 0, 8);

	while (fgets(buf, 7, fd)) {
		size = strlen(buf);

		if (size < 1) {
			memset(buf, 0, 8);
			continue;
		}

		size--;
		buf[size] = 0;

		if (size != 5) {
			memset(buf, 0, 8);
			continue;
		}

		ret[i][0] = buf[0];
		ret[i][1] = buf[1];
		ret[i][2] = buf[2];
		ret[i][3] = buf[3];
		ret[i][4] = buf[4];

		memset(buf, 0, 8);
		i++;

		if (max <= i)
			break;
	}

	fclose(fd);
	words = (Words) {
		.arr = ret,
		.n = i
	};

	return words;
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