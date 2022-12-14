#include <iostream>
#include <string>
#include <pthread.h>
#include <fstream>

#define CRYPTO_KEY -3
#define RAND_STRING_LENGTH 100


// Глобальные переменные
size_t string_size; // Размер строки
char *end_of_string;    // Адрес конца строки
size_t step;    // Размер куска строки для каждого потока

// Переменные портфеля задач -- границы интервала, который должен будет обработать следующий свободный поток
char *start_it;
char *end_it;

pthread_mutex_t mutex;  // мьютекс для условных переменных

// Реализация кодовой таблички
char codeTableFunction(char encrypted) {
    if (encrypted <= 3) {
        return char(0);
    }
    return encrypted + CRYPTO_KEY;
}

// функция для потоков
void *thread_func(void *param) {
//    int thr_number = *(int *) param;  // номер потока (нужен для тестов на 9)
//    std::cout << "start" << thr_number << "\n";
    char *left_it;  // адрес левой границы интервала, с которым будет работать только этот поток
    char *right_it; // адрес правой границы интервала, с которым будет работать только поток
    do {
        pthread_mutex_lock(&mutex); //протокол входа в КС: закрытие двоичного семафора (мютекс)
        //начало критической секции – обращения к портфелю задач

        // условие завершения работы потока (интервал не принадлежит строке)
        if (start_it >= end_of_string) {
            pthread_mutex_unlock(&mutex);  //выход из КС: открытие двоичного семафора
            break;
        }
        // присваиваем интервалу потока интервал выделенный ему портфелем задач
        left_it = start_it;
        right_it = end_it;

        // обновляем значения глобальных переменных портфеля задач
        start_it = end_it;
        if (end_it + step >= end_of_string) {
            end_it = end_of_string;
        } else {
            end_it = end_it + step;
        }
        // (теперь портфель задач хранит интервал для следующего свободного потока)

        //конец критической секции
//        std::cout << thr_number << ": " << *left_it << " and " << *right_it << '\n';
        pthread_mutex_unlock(&mutex);  //протокол выхода из КС: произошло открытие двоичного семафора

        // Для куска строки отделенного данному потоку производим дешифрование через таблицу
        for (char *it = left_it; it < right_it; ++it) {
            *it = codeTableFunction(*it);
        }
    } while (true);
    return nullptr;
}

// генерирует случайную строчку
std::string createRandomString(int length) {
    srand(clock());
    char* ch_arr = new char[length-1];
    for (int j = 0; j < length; ++j) {
        ch_arr[j] = (char) (32 + rand() % 92 + 3);
    }
    printf("Random string: %s\n", ch_arr);
    std::string res = ch_arr;
    return "Wklv#lv#udqgrp#vwulqj=" + res;  // добавим метку, чтобы показать что это строка случайная "This string is random:"
}

int main(int argc, char *argv[]) {
    std::string input_string;   // строка сохраняющая вводные данные

    // вводим эту строку
    if (argc == 3 && argv[1][0] == ':') {   // рандомная генерация
        input_string = createRandomString(RAND_STRING_LENGTH);
    } else if (argc == 1) {        // ввод с консоли
        std::cout << "Введите зашифрованную строку:";
        std::cin >> input_string;
    } else if (argc == 2) {     // ввод с командной строки
        input_string = argv[1];
    } else if (argc == 3) {     // ввод с файла
        std::ifstream input_fstream;    // ifstream
        input_fstream.open(argv[1]);
        if (!input_fstream.is_open()) {
            printf("Could not open file. Press any key to exit");
            getchar();
            return 0;
        }
        std::getline(input_fstream, input_string);  //  ввод строки с файла
        input_fstream.close();
    }
    const std::string encrypted = input_string;     // зашифрованный текст и есть наша строка
    std::string result_text = encrypted;    // расшифрованный текст, пока он совпадает зашифрованным. Это едитсвенная строка с котороый мы работаем

    // инициализируем глобальные переменные
    start_it = &result_text[0]; // адрес левой стороны первого интервала это и есть первый символ стоки
    string_size = result_text.size();   // размер строки

    end_of_string = start_it + string_size; // адрес конца строки (первый симов за строкой '\0')

    step = string_size / 5; // размер кусков строки
    // задаем  адрес правой стороны первого интервала (куска)
    end_it = start_it;
    if (end_it + step >= end_of_string) {
        end_it = end_of_string;
    } else {
        end_it = end_it + step;
    }

    pthread_t pthread_first, pthread_second;        // наши "взаимодействующие равные" потоки
    pthread_mutex_init(&mutex, NULL); //инициализация двоичного семафора
    int num[2]{1, 2}; // номера потоков (нужно для дебага, например в задании на 9)

    // создаем потоки
    pthread_create(&pthread_first, NULL, thread_func, (void *) (num));
    pthread_create(&pthread_second, NULL, thread_func, (void *) (num + 1));

    // ожидаем завершение потоков
    pthread_join(pthread_first, nullptr);
    pthread_join(pthread_second, nullptr);

    // вывод измененной потоками строки содержащей ответ
    std::cout << "Result:\n" << result_text << std::endl;
    if (argc == 3) {    // вывод в файл если надо (также при рандомной генерации)
        FILE *output_stream = fopen(argv[2], "w");
        fprintf(output_stream, "%s", result_text.data());
        fclose(output_stream);
    }
    return 0;
}
