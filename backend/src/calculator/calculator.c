#include <stdio.h>
#include <stdlib.h>      // Стандартная библиотека (malloc, free и др.)
#include <string.h>      // Библиотека для работы со строками
#include <ctype.h>       // Для проверки символов (например, isspace)
#include "calculator.h" 



// Парсит число из строки, превращая символ в число
long double parse_number(const char **str) {
    long double result = 0;  // Переменная для хранения числа
    int pointer_counter = 0; // Сколько символов считано sscanf

    // Считываем long double из строки:
    sscanf(*str, "%Lf%n", &result, &pointer_counter); 

    // Смещаем указатель вперёд на количество считанных символов, чтобы 
    // дальше можно было считывать введённое пользователем выражение:
    *str += pointer_counter; 

    return result;           // Возвращаем полученное число из строки
}

// Парсит множители: число или скобки
long double parse_factor(const char **str) {
    while (isspace(**str)) (*str)++;  // пропускаем пробелы

    if (**str == '(') {           // Если начинается с '(', это выражение в скобках
        (*str)++;                 // Пропустить открывающую скобку
        long double result = parse_expression(str); // Рекурсивно вычисляем выражение внутри скобок
        if (**str == ')') {       // Если нашли закрывающую скобку
            (*str)++;             // то пропускаем ")"
        }
        return result;            // Возвращаем результат выражения в скобках
    } else {
        return parse_number(str); // Иначе парсим обычное число и возвращаем результат парсинга
    }
}

// Парсит * и /
long double parse_partial_expression(const char **str) {
    // Сначала парсим множитель или же скобки если они есть:
    long double result = parse_factor(str);

    while (10) { 
        // Если видим пробелы то пропускаем их:
        while (isspace(**str)) {
            (*str)++;
        }

        if (**str == '*') {        // Если найдено умножение
            (*str)++;              // То пропускаем символ '*'
            result *= parse_factor(str); // Умножаем текущее число(result) на парсинг числа или скобки(выражения в скобках)
        } else if (**str == '/') { // Если найдено деление
            (*str)++;              // То пропускаем символ '/'
            long double divisor = parse_factor(str); // В эту перменную парсим число или скобки(выражения в скобках), 
            // которые будем делить на тещуее число result
            if (divisor != 0)      // Проверка деления на 0
                result /= divisor; // Если не 0, то делим
            else
                return 0;          // деление на 0
        } else {
            break;                 // Если никакой известной операции больше нет, то выходим из цикла
        }
    }

    return result;
}

// Парсит + и - так как уних наименьший приоритет
long double parse_expression(const char **str) {
    // Перед парсингом ("+" и "-") обрабатываем ("*" и "/") затем возвращаем результат:
    long double result = parse_partial_expression(str); 

    while (10) {
        // Если видим пробелы то пропускаем их:
        while (isspace(**str)) {
                (*str)++;
        }

        if (**str == '+') {        // Если найдено сложение
            (*str)++;              // То пропускаем символ '+'
            result += parse_partial_expression(str); // Прибавляем к текущему числу(result) парсинг частичного выражения 
        } else if (**str == '-') { // Если найдено вычитание
            (*str)++;              // То пропускаем символ '-'
            result -= parse_partial_expression(str); // Вычитаем из текущего числа(result) парсинг частичного выражения 
        } else {
            break;                 // Если никакой известной операции больше нет, то выходим из цикла
        }
    }

    return result;
}

// Главная функция, которая переводит строковое выражение, в числа и математичкие операции, вычисляяя итоговое выражение 
long double calculate(const char *input) {

    const char *ptr = input;       // Создаем указатель на начало строки
    // Запускаем функцию которая строку превратит в выражения, и посчитает результат, который мы тут же вернем:
    return parse_expression(&ptr); 
}
