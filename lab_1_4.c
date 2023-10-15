#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Программа %s: <input_file> <flag>\n", argv[0]);
        return 1;
    }

    const char* input_file_path = argv[1];
    const char* flag = argv[2];

    FILE* input_file = fopen(input_file_path, "rb");

    if (input_file == NULL) {
        perror("Не удалось открыть входной файл\n");
        return 1;
    }

    FILE* output_file = fopen("output.bin", "wb");

    if (output_file == NULL) {
        perror("Не удалось открыть выходной файл\n");
        fclose(input_file);
        return 1;
    }

    int operation = 0;

    if (strcmp("xor8", flag) == 0){
        operation = 1;
    }

    if (strcmp("xor32", flag) == 0){
        operation = 2;
    }

    if (strcmp("mask", flag) == 0){
        operation = 3;
    }

    __uint32_t group = 0;
    __uint8_t byte_read = 0;
    __uint8_t byte;
    __uint32_t hex = 0x123AB4CD;

    __uint32_t value;
    size_t bytes_read;
    int find = 0;

    switch (operation) {
        case 1:
            while ((fread(&byte, sizeof(__uint8_t), 1, input_file)) == 1) {
                byte = byte ^ 0xFF;
                fwrite(&byte, sizeof(__uint8_t), 1, output_file);
            }
            printf("Файл изменен\n");
            break;

        case 2:
            while ((fread(&byte, sizeof(__uint8_t), 1, input_file)) == 1) {
                if (byte == 0) {
                    if (byte_read > 0) {
                        group = group ^ 0x00000000;
                        fwrite(&group, sizeof(__uint32_t), 1, output_file);
                    }
                    break;
                }

                group = (group << 8) | byte;
                byte_read++;

                if (byte_read == 4) {
                    fwrite(&group, sizeof(__uint32_t), 1, output_file);
                    byte_read = 0;
                    group = 0;
                }
            }
            printf("Файл изменен\n");
            break;

        case 3:
            while (fread(&value, sizeof(__uint32_t), 1, input_file) == 1) {
                if (value == hex) {
                    find++;
                    printf("Найдено совпадение: 0x%08X\n", value);
                }
            }
            printf("Найдено %d совпадений\n", find);
            break;

        default:
            printf("Недопустимый код операции\n");
            break;
    }

    fclose(input_file);
    fclose(output_file);

    return 0;
}
