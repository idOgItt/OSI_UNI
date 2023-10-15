#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Программа %s: <input_file> <output_file>\n", argv[0]);
        return 1;
    }

    const char* input_file_path = argv[1];
    const char* output_file_path = argv[2];

    FILE* input_file = fopen(input_file_path, "rb");

    if (input_file == NULL) {
        perror("Ошибка при открытии входного файла");
        return 1;
    }

    FILE* output_file = fopen(output_file_path, "wb");

    if (output_file == NULL) {
        perror("Ошибка при открытии выходного файла");
        fclose(input_file);
        return 1;
    }

    __uint8_t byte;

    while (fread(&byte, 1, 1, input_file) == 1) {
        fwrite(&byte, 1, 1, output_file);
    }

    fclose(input_file);
    fclose(output_file);

    return 0;
}
