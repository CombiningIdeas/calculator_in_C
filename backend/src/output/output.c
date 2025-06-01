#include <time.h> // Для получения текущего времени (функция time())
#include "output.h"
#include "../calculator/calculator.h"
#include "../../configs/config.c"


// Функция отправки JSON-сообщения с информацией о сервере
void handle_get_request(struct mg_connection *c) {
        // В качестве параметра передается указатель на объект типа time_t, в который 
        // помещается текущие дата/время. То есть мы можем получить дату/время следующим способам:
        time_t mytime = time(NULL);
        // Чтобы получить дату/время и ее компоненты (часы, минуты и т.д.), 
        // нам надо получть из объекта time_t структуру tm с помощью функции localtime():
        struct tm *now = localtime(&mytime);

        //В случае успешного запуска сервера можно спарсить этот файл с него
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
        
        // c - это указатель на соединение, через которое пришёл запрос. 
        // Мы отправляем ответ по этому же соединению.
        // HTTP-статус ответа: 200 означает, что запрос обработан успешно.
}


void handle_post_request(struct mg_connection *c, struct mg_http_message *hm) {
  // expression — массив символов (для хранения строки с выражением, 
  // введённым пользователем, например "3+5").
  char expression[LENGTH_OF_FORMS_RESPONSE_AND_REQUEST], 
  result[LENGTH_OF_FORMS_RESPONSE_AND_REQUEST];

  // Извлекает значение поля expression из тела HTTP-запроса POST:
  mg_http_get_var(&hm->body, "expression", expression, sizeof(expression));
  // &hm->body — указатель на тело POST-запроса;
  // "expression" — имя параметра, который ищем исходной html страничке
  // expression — им массив куда сохранить результат (массив, куда будет записано "3+5-7*8");
  // sizeof(expression) — размер буфер, чтобы избежать переполнения памяти, выдаст размер исходного массива

  // Проверка того что мы получаем от пользователя:
  // printf("Получено выражение: '%s'\n", expression);
  long double result_number = calculate(expression);
  snprintf(result, sizeof(result), "%.15Lf", result_number);


  // Можно было бы использовать функцию sscanf(), но мы не знаем сколько перменных введет пользователь
  // Поэтому и не можем знать сколькоперменных нам заранее потребуется
        

  // Загружаем шаблон и проверяем на ошибку, если шаблон не удалось открыть то возвращаем ошибку 500
  FILE *file = fopen("frontend/OutputPage.html", "r");
  if (file == NULL) {
    mg_http_reply(c, 500, "", "Ошибка загрузки шаблона");
    return;
  }

  fseek(file, 0, SEEK_END);  // Перемещаем указатель чтения файла в конец (SEEK_END)
  // Это делается, чтобы определить размер файла
  long length = ftell(file); // ftell возвращает текущую позицию указателя (в байтах от начала файла)
  // Так как перед этим был SEEK_END, мы получаем размер файла в байтах
  fseek(file, 0, SEEK_SET);  // Возвращаем указатель файла обратно в начало, чтобы начать чтение содержимого


  // Выделяем память под шаблон (на 1 байт больше, чтобы добавить \0 в конце строки):
  char *template = malloc(length + 1); // Указатель template будет хранить содержимое HTML-шаблона

  // Считываем весь файл в буфер template:
  fread(template, 1, length, file); // fread читает length байт по 1 байту за раз из файла file
  template[length] = '\0'; // Добавляем завершающий нуль-терминатор (\0), чтобы строка была валидной C-строкой
  fclose(file); // Закрываем файл — освобождаем системные ресурсы.

  // Подставляем данные в шаблон:
  char final_html[LENGTH_OF_FINAL_HTML_PAGE_IN_BITES]; // Создаём массив(буфер) final_html, где будет итоговая HTML-страница
  // snprintf заполняет final_html, подставляя переменные expression и result в строку-шаблон template:
  snprintf(final_html, sizeof(final_html), template, expression, result); 
  // Если в template есть %s %s, то туда вставятся expression и result.


  // Отправляем результат пользователю
  mg_http_reply(c, 200, "Content-Type: text/html\r\n", "%s", final_html);
  free(template); // Освобождаем выделенную ранее память под template.
}
