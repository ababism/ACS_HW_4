#include <iostream>
#include <string>
#include <pthread.h>

char *p_string;
size_t string_size;
size_t step;
char *start_it;
char *end_it;

std::pair<char*, char*> updateCurrentIterators() {
    if (start_it >= p_string + string_size) {
        return {nullptr, nullptr}
    }
    if (end_it >= p_string + string_size) {
        return {}
    }
    start_it += step;
    return { }
}

void* func(void *param) {
    int p=*(int *)param ;  //номер строки, заполняемой потоком
    //вычисление элементов результирующей матрицы, стоящих в строке с номером p
    for (int i=0 ; i<3 ; i++) {
        C[p][i]=0 ;
        for (int j=0 ; j<3 ; j++) {
            C[p][i]+=A[p][j]*B[j][i] ;
        }
        pthread_mutex_lock(&mutex) ; //протокол входа в КС: закрыть двоичный семафор
        //начало критической секции – запись результата в очередь
        newrez = new result ;
        sprintf(newrez->str,
                "Поток %d: вычислен элемент [%d][%d] = %d",
                p, p, i, C[p][i]
        ) ;
        newrez->next = NULL ;
        rez->next = newrez ;
        rez = newrez ;

        //конец критической секции
        pthread_mutex_unlock(&mutex) ;  //протокол выхода из КС:
        //открыть двоичный семафор
        //sleep(1);
    }
    return nullptr;
}
int main() {
    pthread_mutex_t mutex;// мьютекс для условных переменных
    const std::string encrypted("Wklv#wh{w#vkrxog#eh#hqfu|swhg");
    std::string text = encrypted;
    char key = 3;
    for (auto i = 0; i < text.size(); ++i) {
        text[i] = encrypted[i] - 3;
    }
    step = text.size();

    pthread_t pthread_first,  pthread_second ;
    pthread_mutex_init(&mutex, NULL) ; //инициализация двоичного семафора
//    pthread_mutex_lock(&mutex);

    std::cout << text << std::endl;

    return 0;
}




