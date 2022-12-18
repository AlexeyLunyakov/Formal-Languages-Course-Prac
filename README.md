# tfya_pract_mirea
Practical work on the subject: "Theory of formal languages"


## Практика 1
**Задание:** 
* *на любом языке программирования реализовать преобразование простого алгебраического выражения в обратную польскую запись.*

**Пример работы программы:**
* ввод: ( 10 + 2 ) * 2
* вывод: 10 2 + 2 *

## Практика 2
**Задание:** 
* *на любом языке программирования реализовать простой калькулятор алгебраических 
выражений в обратной польской записи с целыми числами. 
Поддерживаемые операции: сложение, вычитание, умножение, деление.*

**Пример работы программы:**
* ввод: 10 2 + 2 *
* вывод: 24

**Решение:**
[1-2](https://github.com/AlexeyLunyakov/tfya_pract_mirea/tree/main/tfya_1_2)

## Практика 3
**Задание:** 
* *на любом языке программирования (или с помощью FLEX-
лексического анализатора) реализовать простой анализатор JSON файла.
Обработка несуществующей лексемы: либо завершить выполнение
программы с соответствующим сообщением, либо вывести лексему без
соотношения к определенному классу токенов.*

**Пример работы программы:**
* ввод (через консоль или файл):
{
"name": "Ivan",
"age": 37,
"children": [
"Masha",
"Igor"
],
"married": true
}
* вывод:
Tokenlist:
(BEGIN_OBJECT, ‘{’)
(STRING, ‘name’)
(COLON, ‘:’)
(STRING, ‘Ivan’)
(COMMA, ‘,’)
(STRING, ‘age’)
(COLON, ‘:’)
(NUMBER, 37)
(COMMA, ‘,’)
(STRING, ‘children’)
(COLON, ‘:’)
(BEGIN_ARRAY, ‘[’)
(STRING, ‘Masha’)
(COMMA, ‘,’)
(STRING, ‘Igor’)
(END_ARRAY, ‘]’)
(COMMA, ‘,’)
(STRING, ‘married’)
(COLON, ‘:’)
(LITERAL, ‘true’)
(END_OBJECT, ‘}’)

**Решение:**
[3](https://github.com/AlexeyLunyakov/tfya_pract_mirea/tree/main/tfya_3)

## Практика 4
**Задание:** 
* *написать на любом языке программирования (или доработать
листинг 1) лексический анализатор на базе конечного автомата входного
языка, описанного диаграммой состояний*
![изображение](https://user-images.githubusercontent.com/100156578/208295551-204db3b6-f16d-41dd-82b8-a8dca13c5003.png)
![изображение](https://user-images.githubusercontent.com/100156578/208295567-fe64adfa-7d33-4420-97a3-86f5059d94fc.png)

**Решение:**
[4](https://github.com/AlexeyLunyakov/tfya_pract_mirea/tree/main/tfya_4)

## Практика 5
**Задание:** 
* *на любом языке программирования написать программу
преобразования недетерминированного конечного автомата (НКА) в
детерминированный (ДКА)*

**Пример работы программы:**
* ввод (через консоль или файл):

Enter set of states:
1 2 3

Enter the input alphabet:
a b

Enter state-transitions function (current state, input
character, next state):
(1,a,1) (1,a,2) (1,b,3) (2,a,2) (2,b,1) (2,b,3) (3,a,3)
(3,b,3)

Enter a set of initial states:
1

Enter a set of final states:
3
* вывод:

DFA:

Set of states: 1, 12, 13, 3, 123

Input alphabet: a, b

State-transitions function:
D(1, a) = 12
D(1, b) = 3
D(12, a) = 12
D(12, b) = 13
D(13, a) = 123
D(13, b) = 3
D(3, a) = 3
D(3, b) = 3
D(123, a) = 123
D(123, b) = 13

Initial states: 1

Final states: 3, 13, 123

**Решение:**
[5](https://github.com/AlexeyLunyakov/tfya_pract_mirea/tree/main/tfya_5)
