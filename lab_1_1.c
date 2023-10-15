#include <stdio.h>
#include <stdlib.h>

int main (int argc, char * argv[]){

    if (argc != 2){
        printf ("Программа %s: <path_file>\n", argv[0]);
        return 1;
    }

    const char * file_path = argv[1];
    int ch;

    __uint8_t bytes [] = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};

    FILE* input_file = fopen(file_path, "wb");
    if (input_file == NULL) {
        perror("Не удалось открыть файл для записи\n");
        return 1;
    }

    fwrite(bytes, sizeof(__uint8_t), sizeof(bytes) / sizeof(__uint8_t), input_file);

    fclose(input_file);

    input_file = fopen(file_path, "rb");

    if (input_file == NULL) {
        perror("Не удалось открыть файл\n");
        return 1;
    }

    __uint8_t byte;
    size_t bites_read;


    while ((bites_read = fread(&byte, 1, 1, input_file)) == 1) {
        printf("_flags: %d\n", input_file->_flags);
        printf("_IO_read_ptr: %p \n", input_file ->_IO_read_ptr);
        printf("_IO_read_end: %p \n", input_file->_IO_read_end);
        printf("_IO_read_base: %p \n", input_file->_IO_read_base);
        printf("_IO_write_base: %p\n", input_file->_IO_write_base);
        printf("_IO_write_ptr: %p\n", input_file->_IO_write_ptr);
        printf("_IO_write_end: %p\n", input_file->_IO_write_end);
        printf("_IO_buf_base: %p\n", input_file->_IO_buf_base);
        printf("_IO_buf_end: %p\n", input_file->_IO_buf_end);
        printf("_IO_save_base: %p\n", input_file->_IO_save_base);
        printf("_IO_backup_base: %p\n", input_file->_IO_backup_base);
        printf("_IO_save_end: %p\n", input_file->_IO_save_end);


        printf("Считан байт: 0x%02X\n", byte);
    }

    fclose(input_file);

    input_file = fopen(file_path, "rb");

    if (input_file == NULL) {
        perror("Не удалось открыть файл\n");
        return 1;
    }

    long offset = 3;
    int origin = SEEK_SET;
    int result = fseek(input_file, offset, origin);

    if (result == 0) {
        printf("Позиция сдвинута на %ld байт\n", offset);
    } else {
        perror("Ошибка при сдвиге позиции в файле\n");
    }

    unsigned char buffer[4];

    size_t bytes_read = fread(buffer, 1, 4, input_file);

    if (bytes_read == 4) {
        printf("Считано:\n");
        for (size_t i = 0; i < bytes_read; i++) {
            printf("%lu: 0x%02X\n", i, buffer[i]);
        }
    } else if (bytes_read == 0) {
        printf("Достигнут конец файла.\n");
    } else {
        perror("Ошибка при чтении\n");
    }

    fclose(input_file);

    return 0;
}