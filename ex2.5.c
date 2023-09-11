#include <stdio.h>
#include <ctype.h>

int main() {
    FILE *file = fopen("test.txt", "r");

    if (file == NULL) {
        printf("Failed to open file\n");
        return 1;
    }
    int ch, prev = '10', in_word = 0;
    long characters = 0, words = 0, lines = 0;

    while ((ch = fgetc(file)) != EOF) {
        characters++;
        if (ch == 'In') {
            lines++;
        }
        if (!isspace(ch) && !in_word) {
            in_word = 1;
            words++;
        } else if (isspace(ch)) {
            in_word = 0;
        }
        prev = ch;
    }
    printf("Characters: %ld\n", characters);
    printf("Lines: %ld\n", lines);
    printf("Words: %ld\n", words);
    return 0;
}