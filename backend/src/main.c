#include <stdio.h>
#include <errno.h>

// Подключение основной библиотеки Mongoose, содержащей сетевой стек и HTTP-функции:
#include "../../mongoose/mongoose.h" 

#include "output/output.h"
#include "../tests/test_the_size_data_type.h"



// Обработчик HTTP-событий, вызывается каждый раз, когда приходит HTTP-запрос
static void ev_handler(struct mg_connection *c, int ev, void *ev_data) {
    // Проверка, что произошло событие обработки HTTP-сообщения
    if (ev == MG_EV_HTTP_MSG) {

      // Приведение указателя ev_data к структуре mg_http_message, содержащей данные запроса
      struct mg_http_message *hm = (struct mg_http_message*) ev_data;

      struct mg_http_serve_opts opts = {0};
      opts.root_dir = "frontend";   // путь к HTML/CSS/JS-файлам
  
      if (mg_match(hm->uri, mg_str("/"), NULL)) {
        mg_http_serve_file(c, hm, "frontend/MainPage.html", &opts);
      } else if (mg_match(hm->uri, mg_str("/api/get"), NULL)) { // Проверка, совпадает ли URI запроса 
        // с "http://127.0.0.1:7979/api/time/get" 
        
        handle_get_request(c);      // Обрабатывает GET-запрос: отправляет данные через JSON-ответ

      } else if (mg_match(hm->uri, mg_str("/api/post"), NULL)) {

        handle_post_request(c, hm); // Обрабатывает POST-запрос: получает выражение, подставляет в шаблон и отправляет результат

      } else {
        // Если путь не распознан, отправляем ошибку 500 с сообщением в JSON
        // Это позволяет отлаживать сервер и получать корректный ответ при ошибках
        mg_http_reply(c, 500, "", "{%m:%m}\n", MG_ESC("error"), MG_ESC("Unsupported URI")); 
      }
    }
}

int main() {
    int flag = 0;
    
    // Запуск тестов для получения информации о количестве 
    // доступных байт(или же бит) для чисел с плавающей запятой

    if (test_the_size_data_type() != 0) {
        perror("Ошибка при запуске тестов");
        flag = 1;
    }
    

    // Процесс запуск сервера:

    struct mg_mgr mgr;  // Объявляем переменную типа mg_mgr — это менеджер событий Mongoose

    mg_mgr_init(&mgr);  // Инициализируем менеджер событий. Без этого сервер не сможет работать с событиями и соединениями.
  
    // Настраиваем HTTP-сервер, указывая адрес и порт для прослушивания — 0.0.0.0:7979
    // Это значит, что сервер будет доступен по локальной сети и даже из других сетей, если открыть порт
    // ev_handler — это функция, которая будет вызываться при приходе новых HTTP-запросов
    mg_http_listen(&mgr, "http://0.0.0.0:7979", ev_handler, NULL);
  
    printf("Сервер запущен: http://localhost:7979 или же http://127.0.0.1:7979\n");

    // Запускаем бесконечный цикл обработки событий — сервер будет работать, пока его не остановят вручную
    for (;;) {
      // mg_mgr_poll проверяет входящие соединения и вызывает соответствующие обработчики
      // 500 — тайм-аут в миллисекундах (0.5 секунды), как часто опрашивать события
      mg_mgr_poll(&mgr, 500);
    }

    return flag;
}