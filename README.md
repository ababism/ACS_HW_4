# ACS_HW_4
# Архитектуры вычислительных систем. ИДЗ № 4. Вариант 33 <br/> Киселев Иван БПИ217.

## Описание репозитория:

### Решение задачи на C сразу на 9 балов 
**[C_Files](https://github.com/ababism/ACS_HW_3/tree/main/C_Files)** 
> прием аргументов командной строки + ввода с файла/консоли
> Код послужил основанием для комментариев, доп. комментарии в README.md

### Тестирование программы
[Tests](https://github.com/ababism/ACS_HW_3/tree/main/Tests)


# О программе
> Программа получает на ввод зашифрованную строку не разделенную '\n', и выводит результат в конлось (и файл если надо)

1. При обычном запуске будет выведена понятная менюшка

2. При вводе одного аргумента командной строки -- зашифрованную строку не разделенную '\n' и пробелами -- программа будет выведет дешифрованную строку в консоль
```
./program.exe вашe_строка
```

3.1 При файловом вводе-выводе надо ввести два аргумента -- названия существующих двух файлов, которые лежат в папке с .exe: первый файл для ввода. Программа выведет дешифрованную строку и в консоль и в файл

```
./program.exe input.txt output.txt
```

# Отсчет

## На 4 балла:

**✔ Условие задачи:**
33. И снова пляшущие человечки. Узнав о планах преступников озвученных в задаче 32 (сунул ее ниже), Шерлок Холмс предложил лондонской полиции специальную машину для дешифровки сообщений злоумышленников. Реализовать многопоточное приложение, дешифрующее кодированный текст. В качестве ключа используется известная кодовая таблица, устанавливающая однозначное соответствие между каждой буквой и каким-нибудь числом. Процессом узнавания кода в решении задачи пренебречь. Каждый поток дешифрует свои кусочки текста. При решении **использовать парадигму портфеля задач**.

> 32. Пляшущие человечки. На тайном собрании глав преступного мира города Лондона председатель собрания профессор Мориарти постановил: отныне вся переписка между преступниками должна вестись тайнописью. В качестве стандарта были выбраны «пляшущие человечки», шифр, в котором каждой букве латинского алфавита соответствует хитроумный значок. Реализовать многопоточное приложение, шифрующее исходный текст (в качестве ключа используется кодовая таблица, устанавливающая однозначное соответствие между каждой буквой и каким-нибудь числом). Каждый поток шифрует свои кусочки текста. При решении использовать парадигму портфеля задач.

**✔[Решение задачи на C](https://github.com/ababism/ACS_HW_3/tree/main/C_Files)**
Прочитав условие 32 задачи, мы понимаем, что "хитроумные значки"/числа которые одназначно соответсуют буквам в таблице это есть тип данных char. Тогда мы наша кодовая таблица это функция взаимоодонозначно отображающая коды латинских букв в коды других символов. Поскольку нам не нужно узнавать код, то в качестве кодовой таблицы(этой фукции) была взята функиця, которая для дешифровки уменьшает код каждого символа на три.
>✔Реализовано консольное приложение, решающее задачу с использованием одного варианта синхропримитивов (mutex)

>✔Ввод данных с консоли.

**✔ Модель параллельных вычислений**
Для решения этой задачи была использована модель построения многопоточных приложений -- Взаимодействующие равные. Здесь исключен управляющий поток, не занимающийся непосредственными вычислениями (есть только два одинаковых потока pthread_first pthread_second, котороые обрабатывают свои куски строки). Распределение работ по потокам динамичски определяется во время исполнения (строка делится куски разного размера в зависимости от длинны общей строки) с помощью "портфеля задач" -- двух глобальных переменных start_it end_it, которые содержат адресса границ интервала строки для обработки следующим потоком. Доступ к этой пременной-интервалу может одовременно иметь лишь один поток, при обращении ней поток (любой из двух) обновляет границы интервала на слудющие.

## Задание на 5 баллов:

