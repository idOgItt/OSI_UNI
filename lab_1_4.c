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
    FILE* output_file = fopen("output.bin", "wb");

    if (input_file == NULL || output_file == NULL) {
        perror("Не удалось открыть файл\n");
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
    int byte_read = 0;
    int byte;
    __uint32_t hex = 0x123AB4CD;

    __uint32_t value;
    size_t bytes_read;

    switch (operation) {
        case 1:
            while ((byte = fgetc(input_file)) != EOF) {
                byte = byte ^ 0xFF;
                fputc(byte, output_file);
            }
            printf("Файл изменен\n");
            break;

        case 2:
            while (1) {
                int byte = fgetc(input_file);

                if (byte == EOF) {
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
            while (fread(&value, sizeof(value), 1, input_file) == 1) {
                if (value == hex) {
                    printf("Найдено совпадение\n");
                }
                else {
                    printf ("Не найдено\n");
                }
            }
            break;

        default:
            printf("Invalid operation code\n");
            break;
    }

    fclose(input_file);
    fclose(output_file);

    return 0;
}
