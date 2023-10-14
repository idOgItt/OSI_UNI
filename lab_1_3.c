#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Программа %s: <input_file> <output_file>\n", argv[0]);
        return 1;
    }

    const char* input_file_path = argv[1];
    const char* output_file_path = argv[2];

    FILE* input_file = fopen(input_file_path, "r");
    FILE* output_file = fopen(output_file_path, "w");

    if (input_file == NULL) {
        perror("Ошибка при открытии входного файла");
        return 1;
    }

    if (output_file == NULL) {
        perror("Ошибка при открытии выходного файла");
        fclose(input_file);
        return 1;
    }

    int ch;

    while ((ch = fgetc(input_file)) != EOF) {
        fputc(ch, output_file);
    }

    fclose(input_file);
    fclose(output_file);

    return 0;
}
