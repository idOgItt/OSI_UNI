#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_USERS 100
#define MAX_ATTEMPTS 3

// Объявление структуры для хранения данных пользователей
struct User {
    char login[7];
    int pin;
    int sanctions;
};

struct User users[MAX_USERS];
int userCount = 0;

// Перечисление для статусов
enum login_status_code { login_ok, login_fail, login_not_found };
enum regist_user_status_code { regist_user_ok, regist_user_fail, regist_user_space, regist_found};
enum sanctions_status_code { sanctions_ok, sanctions_not_found, sanctions_fail};
enum how_much_status_code { how_much_ok, how_much_fail };
enum get_time_status_code { get_time_ok };
enum get_date_status_code { get_date_ok };

// Функция для авторизации
enum login_status_code login(char* enter_login) {
    char login[7];
    int pin;

    // Проверка длины логина
    if (strlen(enter_login) > 6) {
        return login_fail;
    }

    strcpy(login, enter_login);

    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].login, login) == 0) {
            return login_ok;
        }
    }

    return login_not_found;
}

// Функция для регистрации
enum regist_user_status_code registerUser(char* enter_login) {
    if (userCount >= MAX_USERS) {
        return regist_user_space;
    }

    if (strlen(enter_login) > 6) {
        return regist_user_fail;
    }

    char login[7];
    int pin = rand() % 100001;

    strcpy(login, enter_login);

    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].login, login) == 0) {
            return regist_found;
        }
    }

    strcpy(users[userCount].login, login);
    users[userCount].pin = pin;
    users[userCount].sanctions = 10;

    userCount++;

    return regist_user_ok;
}

// Функция для установки санкций
enum sanctions_status_code sanctions(char* login, int limit) {

    if (strlen(login) > 6) {
        return sanctions_fail;
    }

    char username[7];
    strcpy(username, login);

    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].login, username) == 0) {
            users[i].sanctions = limit;
            return sanctions_ok;
        }
    }

    return sanctions_not_found;
}

// Функция для вычисления разницы во времени
enum how_much_status_code how_much(char flag, time_t past_date, double* result) {
    if (flag != 's' && flag != 'm' && flag != 'h' && flag != 'y') {
        return how_much_fail;
    }

    time_t current_time;
    time(&current_time);

    double diff = difftime(current_time, past_date);
    *result = diff;

    return how_much_ok;
}

// Функция для получения текущего времени
enum get_time_status_code getTime(struct tm* getTime_info) {
    time_t t;
    struct tm* tm_info;
    time(&t);
    tm_info = localtime(&t);
    *getTime_info = *tm_info;
    return get_time_ok;
}

// Функция для получения текущей даты
enum get_date_status_code getDate(struct tm* getDate_info) {
    time_t t;
    struct tm* tm_info;
    time(&t);
    tm_info = localtime(&t);
    *getDate_info = *tm_info;
    return get_date_ok;
}

// Функция для выхода в меню авторизации
int logout() {
    printf("Выход в меню авторизации.\n");
    return -1;
}

int main() {
    int currentUser = -1;
    int attempt = 0;

    srand((unsigned int)time(NULL));

    while (1) {
        if (currentUser == -1) {
            printf("Выберите действие:\n");
            printf("1. Авторизация\n");
            printf("2. Регистрация\n");

            int choice;
            scanf("%d", &choice);
            char enter_login[7];
            char enter_regist[7];
            enum regist_user_status_code reg;

            switch (choice) {
                case 1:
                    printf("Введите логин (не более 6 символов): ");
                    scanf("%s", enter_login);
                    currentUser = login(enter_login);
                    if (currentUser != -1) {
                        attempt++;
                    }
                    if (currentUser == login_ok) {
                        printf("Login success\n");
                    }
                    if (currentUser == login_fail) {
                        printf("Something went wrong\n");
                    }
                    if (currentUser == login_not_found) {
                        printf("There is no such a user\n");
                    }
                    break;
                case 2:
                    printf("Введите логин (не более 6 символов): ");
                    scanf("%s", enter_regist);
                    reg  = registerUser(enter_regist);
                    if (reg == regist_user_ok){
                        printf("Everything went fine\n");
                    }
                    if (reg == regist_user_fail){
                        printf("Login must be less than 7 length\n");
                    }
                    if (reg == regist_user_space){
                        printf("Limits of users\n");
                    }
                    if (reg == regist_found){
                        printf("There is such a user\n");
                    }
                    break;
                default:
                    printf("Неверный выбор. Попробуйте снова.\n");
                    break;
            }
        } else {
            if (attempt >= users[currentUser].sanctions) {
                printf("Превышено максимальное количество запросов. Ваш аккаунт заблокирован.\n");
                currentUser = -1;
                continue;
            }

            if (currentUser == login_ok) {
                printf("Выберите команду:\n");
                printf("3. Time\n");
                printf("4. Date\n");
                printf("5. How_much\n");
                printf("6. Logout\n");
                printf("7. Sanctions\n");

                int choice;
                struct tm getTime_info;
                struct tm getDate_info;
                double result_how_much;
                char flag[2];
                scanf("%d", &choice);

                switch (choice) {
                    case 3:
                        if (getTime(&getTime_info) == get_time_ok) {
                            printf("Текущее время: %02d:%02d:%02d\n", getTime_info.tm_hour, getTime_info.tm_min, getTime_info.tm_sec);
                        }
                        break;
                    case 4:
                        if (getDate(&getDate_info) == get_date_ok) {
                            printf("Текущая дата: %02d:%02d:%d\n", getDate_info.tm_mday, getDate_info.tm_mon + 1, getDate_info.tm_year + 1900);
                        }
                        break;
                    case 5:
                        time_t past_date;
                        printf("Введите флаг: -s (секунды), -m (минуты), -h (часы), -y (годы): ");
                        char flag[3];  // Увеличил размер массива для хранения "-s", "-m", "-h" или "-y".
                        scanf(" %2s", flag);  // Считываем не более двух символов.

                         if (flag[0] == '-')  {
                            if (flag[1] != 's' && flag[1] != 'm' && flag[1] != 'h' && flag[1] != 'y') {
                                printf("There is no such a flag\n");
                                break;
                            }

                            // Очищаем буфер от символа новой строки.
                            int c;
                            while ((c = getchar()) != '\n' && c != EOF);

                            printf("Введите дату от которой считаем (в формате YYYY-MM-DD HH:MM:SS): ");
                            char date_string[20];
                            scanf(" %19[^\n]", date_string);

                            struct tm time_input;
                            if (strptime(date_string, "%Y-%m-%d %H:%M:%S", &time_input) == NULL) {
                                printf("Ошибка в формате даты. Используйте формат YYYY-MM-DD HH:MM:SS\n");
                                break;
                            }

                            past_date = mktime(&time_input);
                            enum how_much_status_code how_status = how_much(flag[1], past_date, &result_how_much);
                            // 2022-12-12 23:23:23

                            if (how_status == how_much_ok) {
                                if (flag[1] == 's') {
                                    printf("Прошло %lf секунд.\n", result_how_much);
                                } else if (flag[1] == 'm') {
                                    printf("Прошло %lf минут.\n", result_how_much / 60);
                                } else if (flag[1] == 'h') {
                                    printf("Прошло %lf часов.\n", result_how_much / 3600);
                                } else if (flag[1] == 'y') {
                                    printf("Прошло %lf лет.\n", result_how_much / (365.25 * 24 * 3600));
                                }
                            }
                        } else {
                            printf("Введен неверный флаг.\n");
                        }
                        break;
                    case 6:
                        currentUser = logout();
                        break;
                    case 7:
                        char username[7];
                        int limit;
                        printf("Введите имя пользователя, на которого хотите наложить санкции (не более 6 символов): ");
                        scanf(" %s", username);
                        printf("Введите лимит для санкций (целое число): ");
                        scanf("%d", &limit);
                        if (sanctions(username, limit) == sanctions_ok) {
                            printf("Санкции для пользователя %s установлены на %d.\n", username, limit);
                        }
                        if (sanctions(username, limit) == sanctions_not_found) {
                            printf("Such user: %s was not found.\n", username);
                        }
                        if (sanctions(username, limit) == sanctions_fail) {
                            printf("Username must me less than 7\n", username);
                        }
                        break;
                    default:
                        printf("Неверный выбор. Попробуйте снова.\n");
                        break;
                }
            }
        }
    }

    return 0;
}
