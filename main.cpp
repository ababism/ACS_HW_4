#include <iostream>
#include <string>
#include <pthread.h>
#include <fstream>

size_t string_size;
char *end_of_string;
size_t step;
char *start_it;
char *end_it;

pthread_mutex_t mutex;// мьютекс для условных переменных

void *func(void *param) {
    int thr_number = *(int *) param;  //номер потока
//    std::cout << "start" << thr_number << "\n";
    char *left_it;
    char *right_it;
    do {
        pthread_mutex_lock(&mutex); //протокол входа в КС: закрыть двоичный семафор
        //начало критической секции – обращению в портфелю задач
        left_it = start_it;
        right_it = end_it;
        // обновляем значения глобальных переменных портфеля задач
        start_it = end_it;
        if (end_it + step >= end_of_string) {
            end_it = end_of_string;
        } else {
            end_it = end_it + step;
        }
        //конец критической секции
//        std::cout << thr_number << ": " << *left_it << " and " << *right_it << '\n';
        pthread_mutex_unlock(&mutex);  //протокол выхода из КС:
        //открыть двоичный семафор
        for (char *it = left_it; it < right_it; ++it) {
            *it = (*it - 3);
        }
    } while (end_it < end_of_string);
    return nullptr;
}

int main(int argc, char *argv[]) {
    std::string input_string;
    if (argc == 1) {
        std::cout << "Введите зашифрованную строку:";
        std::cin >> input_string;
    } else if (argc == 2) {
        input_string = argv[1];
    } else if (argc == 3) {
//        FILE *input_stream = fopen(argv[1], "r");
//        if (input_stream == NULL) {
//            printf("Could not open file. Press any key to exit");
//            getchar();
//            return 0;
//        }
//        fscanf(input_stream, "%s", input_string.data());
//        fclose(input_stream);
        std::ifstream ifstream;
        ifstream.open(argv[1]);
        if (!ifstream.is_open()) {
            printf("Could not open file. Press any key to exit");
            getchar();
            return 0;
        }
        std::getline(ifstream, input_string);
        ifstream.close();
    }
    const std::string encrypted = input_string;
    std::string result_text = encrypted;

    start_it = &result_text[0];
    string_size = result_text.size();

    end_of_string = start_it + string_size;

    step = string_size / 5;
    end_it = start_it;
    if (end_it + step >= end_of_string) {
        end_it = end_of_string;
    } else {
        end_it = end_it + step;
    }

    pthread_t pthread_first, pthread_second;
    pthread_mutex_init(&mutex, NULL); //инициализация двоичного семафора
    int num[2];
    num[0] = 1;
    num[1] = 2;
    pthread_create(&pthread_first, NULL, func, (void *) (num));
    pthread_create(&pthread_second, NULL, func, (void *) (num + 1));

    pthread_join(pthread_first, nullptr);
    pthread_join(pthread_second, nullptr);

    std::cout << result_text << std::endl;
    if (argc == 3) {
        FILE *output_stream = fopen(argv[2], "w");
        fprintf(output_stream, "%s", result_text.data());
        fclose(output_stream);
    }
    return 0;
}
