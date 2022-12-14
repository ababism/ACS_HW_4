# ACS_HW_4
# Архитектуры вычислительных систем. ИДЗ № 4. Вариант 33 <br/> Киселев Иван БПИ217.

## Описание репозитория:

### Решение задачи на C сразу на 9 балов 
**[Main.cpp](https://github.com/ababism/ACS_HW_4/blob/main/main.cpp)** 
> прием аргументов командной строки + ввода с файла/консоли + генерация и вывод в файл
> Код послужил основанием для комментариев

**[task9.cpp](https://github.com/ababism/ACS_HW_4/blob/main/task9.cpp)** 
> Код для показа некорректного поведения при отключении синхропримитивов для задания 9


### Тестирование программы и  файлы для тестирования
[Tests and Files](https://github.com/ababism/ACS_HW_4/tree/main/Tests)


# О программе
> Программа получает на ввод зашифрованную строку не разделенную '\n', и выводит результат в консоль (и файл если надо)

1. При обычном запуске будет выведена понятная менюшка

2. При вводе одного аргумента командной строки -- зашифрованную строку не разделенную '\n' и пробелами -- программа будет выведет дешифрованную строку в консоль
```
./program.exe ваша_строка
```

3.1 При файловом вводе-выводе надо ввести два аргумента -- названия существующих двух файлов, которые лежат в папке с .exe: первый файл для ввода. Программа выведет дешифрованную строку и в консоль и в файл

```
./program.exe input.txt output.txt
```

3.2 При рандомной генерации (+ файловом выводе) надо ввести два аргумента -- первый это "флаг" ':', второй -- название файла, который лежит в папке с .exe. Рандомная зашифрованная строка появится на дисплее в консоли, а ответ будет в файле и в консоли

```
./program.exe : output.txt
```

# Отсчет

## На 4 балла:

**✔ Условие задачи:**
33. И снова пляшущие человечки. Узнав о планах преступников озвученных в задаче 32 (сунул ее ниже), Шерлок Холмс предложил лондонской полиции специальную машину для дешифровки сообщений злоумышленников. Реализовать многопоточное приложение, дешифрующее кодированный текст. В качестве ключа используется известная кодовая таблица, устанавливающая однозначное соответствие между каждой буквой и каким-нибудь числом. Процессом узнавания кода в решении задачи пренебречь. Каждый поток дешифрует свои кусочки текста. При решении **использовать парадигму портфеля задач**.

> 32. Пляшущие человечки. На тайном собрании глав преступного мира города Лондона председатель собрания профессор Мориарти постановил: отныне вся переписка между преступниками должна вестись тайнописью. В качестве стандарта были выбраны «пляшущие человечки», шифр, в котором каждой букве латинского алфавита соответствует хитроумный значок. Реализовать многопоточное приложение, шифрующее исходный текст (в качестве ключа используется кодовая таблица, устанавливающая однозначное соответствие между каждой буквой и каким-нибудь числом). Каждый поток шифрует свои кусочки текста. При решении использовать парадигму портфеля задач.

**✔[Решение задачи на C++](https://github.com/ababism/ACS_HW_4/blob/main/main.cpp)**
Прочитав условие 32 задачи, мы понимаем, что "хитроумные значки"/числа которые однозначно соответствуют буквам в таблице это есть тип данных char. Тогда мы наша кодовая таблица это функция взаимооднозначно отображающая коды латинских букв в коды других символов. Поскольку нам не нужно узнавать код, то в качестве кодовой таблицы(этой функции) была взята функция, которая для дешифровки уменьшает код каждого символа на три.
>✔Реализовано консольное приложение, решающее задачу с использованием одного варианта синхропримитивов (mutex)

>✔Ввод данных с консоли.

**✔ Модель параллельных вычислений**
Для решения этой задачи была использована модель построения многопоточных приложений -- Взаимодействующие равные. Здесь исключен управляющий поток, не занимающийся непосредственными вычислениями (есть только два одинаковых потока pthread_first pthread_second, которые обрабатывают свои куски строки). Распределение работ по потокам динамически определяется во время исполнения (строка делится куски разного размера в зависимости от длинны общей строки) с помощью "портфеля задач" -- двух глобальных переменных start_it end_it, которые содержат адреса границ интервала строки для обработки следующим потоком. Доступ к этой переменной-интервалу может одновременно иметь лишь один поток, при обращении ней поток (любой из двух) обновляет границы интервала на следующие.


## Задание на 5 баллов:

**✔[Комментарии поясняющие выполняемые действия и описание используемых переменных в main.cpp](https://github.com/ababism/ACS_HW_4/blob/main/main.cpp)**

**✔ Сценарий взаимодействия наших объектов**
В качестве объектов у нас выступает два "равных" потока, по сути отличаются они только номером. Два потока начинают одинаковую для них функцию, в которой каждый поток пока не дойдет до конца строки будет обрабатывать выделенный ему интервал. При ситуации когда два потока (как освободятся) попытаются получить один и тот же интервал (или интервал во время его обновления), мютекс им не даст это сделать: поток пришедший первым получит свой интервал из критической секции (портфеля), затем в ней же обновит значения интервала (для следующего свободного потока), затем мютекс разблокируется и следующий поток получит корректный интервал после первого. В иных случаях потоки просто получают свои интервалы (из портфеля). Такое безопасное поведение происходит благодаря хорошо подобранной к задаче модели построения многопоточного приложения (см. прошлый пункт)
Итого: Два потока дешифруют выданные им кусочки строки пока не дойдут до конца и завершат работу, программа ждет терминацию потоков, выводит результат и завершается


## Задание на 6 баллов:

**✔ Обобщенный алгоритм**
Работа приложении была подробно описана раннее, чтобы не повторятся опишем алгоритм схематически:
![algorithm](https://user-images.githubusercontent.com/111705295/207375590-dc288a4e-6952-4f93-8a00-17a0c194b104.jpg)
Зеленым цветом помечены процессы работающие параллельно

**✔[Ввод данных с командной строки в main.cpp](https://github.com/ababism/ACS_HW_4/blob/main/main.cpp)**


## Задание на 7 баллов:

**✔[Решение задачи на C++](https://github.com/ababism/ACS_HW_4/blob/main/main.cpp)**
>✔В программу добавлены ввод данных из файла (или рандомной генерации) и вывод результатов в файл.

>✔Результаты работы программы выводятся на экран и записываются в файл.

>✔Результаты работы программы выводятся на экран и записываются в файл.

>✔Ввод данных из командной строки расширен с учетом введенных изменений.

**✔[Tests and Files](https://github.com/ababism/ACS_HW_4/tree/main/Tests)**

• Приведены входные и выходные файлы с различными результатами выполнения программы.


## Задание на 8 баллов:

**✔[Решение задачи на C++](https://github.com/ababism/ACS_HW_4/blob/main/main.cpp)**
>✔В программу добавлена генерация случайных данных в допустимых диапазонах..

>✔[Приведены входные и выходные файлы с различными результатами выполнения программы](https://github.com/ababism/ACS_HW_4/tree/main/Tests).

>✔Результаты работы программы выводятся на экран и записываются в файл.

>✔Ввод данных из командной строки расширен с учетом введенных изменений.

## Задание на 9 баллов:

Добавим в критическую секцию перед дешифровкой текста вывод индексов интервала, который будет дешифровать поток, таким образом мы сможем отслеживать какие куски текста какой поток и когда берет

``` c++
std::cout << thr_number << ": " << left_it - start_of_string << " and " << right_it - start_of_string << '\n';
```

<img width="948" alt="image" src="https://user-images.githubusercontent.com/111705295/207439884-d607090e-eb6f-4243-928c-3126640403d8.png">
<img width="884" alt="image" src="https://user-images.githubusercontent.com/111705295/207441033-ea4ead24-1e8f-4ef1-a3b2-951f53eabb08.png">
<img width="901" alt="image" src="https://user-images.githubusercontent.com/111705295/207441099-16901941-aa46-4bec-ba8b-0bb32761eb57.png">


Мы видим что используя синхропримитив мютекс мы гарантируем то, что потоки будут получать разные интервалы последовательно (то есть интервалы будут разные и все интервалы будут дешифрованы)

Теперь уберем (закомментируем) наш синхропримитив, а также увеличим количество потоков на два и поделим строку на 1000 частей (так нам будет легче поймать ошибку при взаимодействии потоков)
``` c++
//        pthread_mutex_lock(&mutex); //протокол входа в КС: закрытие двоичного семафора (мютекс)
//        pthread_mutex_unlock(&mutex);  //протокол выхода из КС: произошло открытие двоичного семафора

```
Спустя примерно час прогонов (не сразу дошел до количества частей и потоков где эту ошибку можно за адекватное время отловить глазами в таком большом тексте) **была найдена ошибка**:
<img width="1054" alt="image" src="https://user-images.githubusercontent.com/111705295/207446440-1e970da2-8143-4c8d-9493-e3e5140077e7.png">
Как мы видим здесь наша программа неправильно дешифровала 9 символов, почему же это произошло? 
Все очень просто (гораздо проще чем найти ее)
Какой-то поток взял себе следующий интервал A1 (справа глобальные переменные содержащие интервал для следующего потока)
``` c++
 left_it = start_it;
 right_it = end_it;
```
Но до того как глобальный интервал успел обновиться до следующего интервала A2:
> ``` c++
>start_it = end_it;
>if (end_it + step >= end_of_string) {
>   end_it = end_of_string;
>} else {
>   end_it = end_it + step;
>}
>```
Какой-то другой поток успел присвоить себе тот же интервал A1

``` c++
 left_it = start_it;
 right_it = end_it;
```
После чего первый поток дообновлял глобальные переменные интервала до A2, и наш второй поток может либо совпасть с первым в действиях и повторно обновить глобальный интервал до A2 (у нас здесь произошло именно так, так как потоки практически одновременно идут), либо обновить с A2 на A3, либо оставить начало интервала в виде начала A2 и успеть два раза увеличить конец интервала до конца A3 (но это очень очень маловероятно и отловить такую ошибку у меня не получилось). 

По итогу в любом случает **при таком поведении дешифровка применяется на интервал A1** два раза => некорректно, как мы видим на скриншоте ошибки выше как раз интервал размера девять символов и он был дешифрован два раза (d:100 -> a:97 (верный символ) -> ^:94(неверный символ) должно было быть p**ack while**, но дешифровав еще раз стало p^`htefib)

Аналогично здесь
<img width="974" alt="image" src="https://user-images.githubusercontent.com/111705295/207457701-2d8e4559-b95e-4127-a3ee-b572173e2359.png">

Поскольку о нас здесь только один синхропримитив и только в одном месте (спасибо нашей модели), то мы описали все варианты отключения синхропримитивов и что происходит из-за них

**✔[Вот task9.cpp в котором отключен мютекс и я отлавливал ошибку](https://github.com/ababism/ACS_HW_4/blob/main/main.cpp)**


Спасибо за прочтение и курс, было интересно! :,)
