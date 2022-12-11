#include <iostream>
#include <string>
#include <pthread.h>

//char *p_string;
size_t string_size;
char *end_of_string;
size_t step;
char *start_it;
char *end_it;

pthread_mutex_t mutex;// мьютекс для условных переменных

void *func(void *param) {
    std::cout << "start\n";
    int p = *(int *) param;  //номер потока
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
        std::cout << param << ": " << *left_it << " and " << *right_it << '\n';
        //конец критической секции
        pthread_mutex_unlock(&mutex);  //протокол выхода из КС:
        //открыть двоичный семафор
        for (char* it = left_it; it < right_it; ++it) {
            *it = (*it - 3);
        }
    } while (end_it < end_of_string);
    return nullptr;
}

int main() {
    const std::string encrypted("Wklv#wh{w#vkrxog#eh#hqfu|swhg");
    std::string text = encrypted;

    start_it = &text[0];
    string_size = text.size();

    end_of_string = start_it + string_size;

    step = string_size / 3;
    end_it = start_it;
    if (end_it + step >= end_of_string) {
        end_it = end_of_string;
    } else {
        end_it = end_it + step;
    }

    pthread_t pthread_first, pthread_second;
    pthread_mutex_init(&mutex, NULL); //инициализация двоичного семафора
//    char key = 3;
//    for (auto i = 0; i < text.size(); ++i) {
//        text[i] = encrypted[i] - 3;
//    }
//    step = text.size();
    int num[2];
    num[0]= 1;
    num[1] = 2;
    pthread_create(&pthread_first, NULL, func, (void *)(num)) ;
    pthread_create(&pthread_second, NULL, func, (void *)(num+1)) ;

    pthread_join(pthread_first, nullptr);
    pthread_join(pthread_second, nullptr);

    std::cout << text << std::endl;

    return 0;
}
