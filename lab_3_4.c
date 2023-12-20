#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

void print_usage() {
    fprintf(stderr, "Использование: file_attributes <file_path> [-r] [-w] [-x] [-h]\n");
    exit(EXIT_FAILURE);
}

void change_file_attributes(const char *file_path, int read_only, int write, int execute, int hidden) {
    struct stat file_stat;

    if (stat(file_path, &file_stat) == -1) {
        perror("Ошибка при получении атрибутов файла");
        exit(EXIT_FAILURE);
    }

    if (read_only) {
        file_stat.st_mode &= ~S_IWUSR;
    }

    if (write) {
        file_stat.st_mode |= S_IWUSR;
    }

    if (execute) {
        file_stat.st_mode |= S_IXUSR; 
    }

    if (hidden) {
        char new_path[strlen(file_path) + 2];
        sprintf(new_path, ".%s", file_path);
        if (rename(file_path, new_path) != 0) {
            perror("Ошибка при скрытии файла");
            exit(EXIT_FAILURE);
        }
    }

    if (chmod(file_path, file_stat.st_mode) == -1) {
        perror("Ошибка при изменении атрибутов файла");
        exit(EXIT_FAILURE);
    }

    printf("Атрибуты файла %s успешно изменены.\n", file_path);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        print_usage();
    }

    const char *file_path = argv[1];
    int read_only = 0;
    int write = 0;
    int execute = 0;
    int hidden = 0;

    for (int i = 2; i < argc; ++i) {
        if (strcmp(argv[i], "-r") == 0) {
            read_only = 1;
        } else if (strcmp(argv[i], "-w") == 0) {
            write = 1;
        } else if (strcmp(argv[i], "-x") == 0) {
            execute = 1;
        } else if (strcmp(argv[i], "-h") == 0) {
            hidden = 1;
        } else {
            fprintf(stderr, "Неизвестный аргумент: %s\n", argv[i]);
            print_usage();
        }
    }

    change_file_attributes(file_path, read_only, write, execute, hidden);

    return EXIT_SUCCESS;
}
