#include <stdio.h>
#include <stdlib.h>
#include <ctype.h> 
#include "calculator.h" 


int division_by_zero_detected = 0; 

// Парсит число из строки
long double parse_number(const char **str) {
    long double result = 0;
    int pointer_counter = 0;

    // Считываем long double из строки:
    sscanf(*str, "%Lf%n", &result, &pointer_counter);
    *str += pointer_counter;

    return result; 
}

// Парсит множители: число или скобки
long double parse_factor(const char **str) {
    while (isspace(**str)) {
        (*str)++;  // пропускаем пробелы
    }

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

    while (1) { 
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
            if (divisor != 0) {    // Проверка деления на 0
                result /= divisor; // Если не 0, то делим
            }else {
                division_by_zero_detected = 1;
                return 0;          // деление на 0
            }          
        } else {
            break;                 // Если никакой известной операции больше нет, то выходим из цикла
        }
    }

    return result;
}

// Парсит + и - так как уних наименьший приоритет
long double parse_expression(const char **str) {
    // Перед парсингом ("+" и "-") обрабатываем ("*" и "/") 
    // затем возвращаем результат
    long double result = parse_partial_expression(str); 

    while (1) {
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

// Главная функция, которая переводит строковое выражение, вычисляяя результат 
long double calculate(const char *input) {
    division_by_zero_detected = 0; // сбрасываем флаг
    const char *ptr = input;
    return parse_expression(&ptr); 
}
