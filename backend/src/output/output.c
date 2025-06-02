#include <time.h> // Для получения текущего времени (функция time())
#include "output.h"
#include "../calculator/calculator.h"
#include "../../configs/config.h"


// Отправка JSON-сообщения с информацией о сервере
void handle_get_request(struct mg_connection *c) {

  // Получем текущее время
  time_t mytime = time(NULL);
  struct tm *now = localtime(&mytime);

  // Отправляем JSON
  mg_http_reply(c, 200, "Content-Type: application/json\r\n",
    "{"
    "\"status\": \"200\","
    "\"message\": \"method GET processed successfully\","
    "\"version\": \"2.0\","
    "\"author\": \"Ivan Shirokikh\","
    "\"connected\": true,"
    "\"data\": \"%d.%d.%d\","
    "\"config\": {\"mode\": \"server is running\", \"data\": \"sending data\"},"
    "\"timestamp\": %ld"
    "}",
    now->tm_mday, 
    now->tm_mon + 1, 
    now->tm_year + 1900,
    time(NULL)
  );
}

// Отправки результатов вычисления ввведённого пользователем выражения
void handle_post_request(struct mg_connection *c, struct mg_http_message *hm) {
  // expression — массив для хранения строки с исходным выражением
  char expression[LENGTH_OF_FORMS_RESPONSE_AND_REQUEST], 
  result[LENGTH_OF_FORMS_RESPONSE_AND_REQUEST];

  // Извлекает значение поля expression
  mg_http_get_var(&hm->body, "expression", expression, sizeof(expression));

  long double result_number = calculate(expression); // Вычисляем результат
  snprintf(result, sizeof(result), "%.15Lf", result_number);

  // Загружаем шаблон и проверяем на ошибку, если шаблон не удалось открыть то возвращаем ошибку 500
  FILE *file = fopen("frontend/OutputPage.html", "r");
  if (file == NULL) {
    mg_http_reply(c, 500, "", "Ошибка загрузки шаблона");
    return;
  }

  fseek(file, 0, SEEK_END);  // Перемещаем указатель файла в конец, чтобы определить размер файла
  long length = ftell(file); // Получаем размер файла в байтах
  fseek(file, 0, SEEK_SET);  // Возвращаем указатель файла обратно в начало

  // Выделяем память под шаблон, на 1 байт больше, чтобы добавить \0 в конец
  char *template = malloc(length + 1); // Хранит содержимое HTML-шаблона

  // Считываем весь файл в template:
  fread(template, 1, length, file); 
  template[length] = '\0'; 
  fclose(file);

  
  char final_html[LENGTH_OF_FINAL_HTML_PAGE_IN_BITES]; // Итоговая HTML-страница

  // Подставляем данные в шаблон:
  if (division_by_zero_detected != 1) {
    snprintf(final_html, sizeof(final_html), template, expression, result); 
  } else {
    snprintf(final_html, sizeof(final_html), template, expression, 
      "Ошибка: в выражении присутствует деление на 0!");
  }

  mg_http_reply(c, 200, "Content-Type: text/html\r\n", "%s", final_html);

  free(template);
}
