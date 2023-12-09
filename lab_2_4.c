#include <iso646.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdarg.h>
#include <string.h>
#include <wait.h>

#define printf(format, ...) printf(format, ##__VA_ARGS__); fflush(stdout);

typedef enum {
    SUCCESS = 0,
    INCORRECT_INPUT,
    OVERFLOW_ERROR,
    MALLOC_ERROR,
    FILE_OPENING_ERROR,
    FILE_READING_ERROR,
    UNKNOWN_ERROR,
    FORK_ERROR
} ErrorCode;

static const char* errorMessages[] = {
    "Everything went fine",
    "Invalid input",
    "Stack overflow",
    "Sygmentation fault",
    "File invalid",
    "File reading error",
    "Missing '('",
    "Missing ')'",
    "Invalid char",
    "Devision zero",
    "Something went wrong"
};

int searchSubstringInFiles(int filesCnt, const char* substring, ...);
ErrorCode getInput(char* buffer, int BUFFER_SIZE);

int main(int argc, char *argv[]) 
{
	if (argc != 2)
	{
        printf("Программа.exe /folder/file.in\n");
        return INCORRECT_INPUT;
	}

    FILE* file = fopen(argv[1], "r");

    if (file == NULL)
    {
        printf("%s\n", errorMessages[INCORRECT_INPUT]);
        return INCORRECT_INPUT;
    }

    printf("Введите подстроку: ");
    char substrBuf[50];
    switch (getInput(substrBuf, sizeof(substrBuf))) {
        case INCORRECT_INPUT:
            printf("%s\n", errorMessages[INCORRECT_INPUT]);
            return INCORRECT_INPUT;
        default:
            break;
    }
    char* substr = strtok(substrBuf, "\n");
    if (substr == NULL) {
        printf("%s\n", errorMessages[INCORRECT_INPUT]);
        return INCORRECT_INPUT;
    }

    char buffer[50];
    for (int i = 0; i < sizeof(buffer); ++i) buffer[i] = '\0';

    //char *files[] = {".conf", "lines.txt", "ok"};
    int found = 0;

    int fileCnt = 0;

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        //printf("Read from file: %s", buffer);
        ++fileCnt;
    }
    rewind(file);

    for (int i = 0; i < fileCnt; ++i) {
    // while (1) {
    //     for (int i = 0; i < sizeof(buffer); ++i) buffer[i] = '\0';
        if (fgets(buffer, sizeof(buffer), file) == NULL) break;
    // while (fgets(buffer, sizeof(buffer), file) != NULL) {
        char* input = strtok(buffer, "\n");
        if (input == NULL) {
            printf("%s\n", errorMessages[INCORRECT_INPUT]);
            return INCORRECT_INPUT;
        }
        //printf("Read from file: %s", buffer);
        //const char* input = buffer;
        //char* input = (char*)malloc(sizeof(buffer) * sizeof(char));
        //char* output = (char*)malloc(maxLen * sizeof(char));
        //*input = *buffer;

        //++fileCnt;
        int pid = fork();
        //int pid = 1;

        if (pid == 0)
        {
            if (searchSubstringInFiles(1, substr, buffer)) 
            {
                printf(" %s\n", buffer);
                return 8976; // found code = 4096
            }
            return 8977; // not found code = 4352
        }
        else if (pid == -1)
        {
            printf("%s\n", errorMessages[FORK_ERROR]);
            return FORK_ERROR;
        }
    }

    for (int i = 0; i < fileCnt; ++i) 
    {
        // ждём завершения дочерних процессов
        int status;
        wait(&status);
        if (status != 4096 && status != 4352) 
        {
            printf("⚠️ Сессия закончилась аварийно, грустно\n");
            printf("Child process stat = %d\n", status);
        }
        else if (status == 4096) 
        {
            found = 1;
        }
    }

    if (!(found))
    {
        printf("'Сообщение об отсутствии строки в заданном файле.'\n");
    }

    fclose(file);
    return SUCCESS;
}


int searchSubstringInFiles(int filesCnt, const char* substring, ...) {
    va_list args;
    va_start(args, substring);

    const char* currentFileName;

    for (int i = 0; i < filesCnt; ++i) {
        currentFileName = va_arg(args, const char*);
        //printf("\n %s:\n", currentFileName);

        FILE* file = fopen(currentFileName, "r");
        if (file == NULL) {
            printf("%s\n", errorMessages[FILE_OPENING_ERROR]);
            va_end(args);
            return 0; // not found
            //continue;
        }

        char line[6];
        int lineLen = sizeof(line);
        for (int i = 0; i < lineLen; ++i) line[i] = '\0';

        int lineNumber = 0;
        //int subsLen = sizeof(substring); 
        int subsLen = strlen(substring);

        while (fgets(line, lineLen, file) != NULL) {
            ++lineNumber;
            //DEBUG_PRINT("line %d:\n", lineNumber);

            for (int j = 0; j < lineLen - subsLen ; ++j) 
            {
                if (line[j] == substring[0]) 
                {
                    //DEBUG_PRINT(" %d\n",j + 1);
                    int found = 1;
                    for (int k = 1; k < subsLen; ++k) 
                    {
                        if (line[j + k] != substring[k]) 
                        {
                            found = 0;
                            break;
                        }
                    }
                    if (found) 
                    {
                        fclose(file);
                        va_end(args);
                        return 1; // found
                        //printf("%d строка, %d символ\n", lineNumber, j + 1);
                    }
                }
            }
            for (int i = 0; i < lineLen; ++i) line[i] = '\0';
        }
        fclose(file);
        va_end(args);
        return 0; // not found
    }

    va_end(args);
    return 0; // not found
}

ErrorCode getInput(char* buffer, int BUFFER_SIZE) 
{
    //const int BUFFER_SIZE = 50;
    //char buffer[BUFFER_SIZE];
    for (int i = 0; i < BUFFER_SIZE; ++i) buffer[i] = '\0';
    if (!fgets(buffer, sizeof(char) * BUFFER_SIZE, stdin)) 
    {
        //printf("%s\n", errorMessages[INCORRECT_INPUT]);
        return INCORRECT_INPUT;
    }
    if (!(buffer[BUFFER_SIZE - 1] == '\0' || buffer[BUFFER_SIZE - 2] == '\n'))
    {
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
    }
    // char* input = strtok(buffer, "\n");
    // if (input == NULL) 
    // {
    //     return INCORRECT_INPUT;
    // }
    return SUCCESS;
}