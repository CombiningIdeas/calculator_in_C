#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "../../mongoose/mongoose.h" 

#include "../configs/config.h"
#include "output/output.h"
#include "../tests/test_the_size_data_type.h"



// Обработчик HTTP-запросов
static void ev_handler(struct mg_connection *c, int ev, void *ev_data) {
    // Проверка что пришел запрос
    if (ev == MG_EV_HTTP_MSG) {

      // Приведение указателя ev_data к структуре mg_http_message, 
      // cодержащей данные запроса
      struct mg_http_message *hm = (struct mg_http_message*) ev_data;

      // Приравняли к 0 указатели, которые могут отдавать статические файлы
      struct mg_http_serve_opts opts = {0}; 
      opts.root_dir = "frontend"; // Путь к HTML/CSS/JS-файлам
  
      if (mg_match(hm->uri, mg_str("/"), NULL)) {
        mg_http_serve_file(c, hm, "frontend/MainPage.html", &opts);
      } else if (mg_match(hm->uri, mg_str("/api/get"), NULL)) { 
        handle_get_request(c); // Отправляет JSON-ответ     
      } else if (mg_match(hm->uri, mg_str("/api/post"), NULL)) {
        handle_post_request(c, hm); // Отправляет результат вычислений
      } else {
        // Если путь не распознан, отправляем ошибку 500 в JSON-формате
        mg_http_reply(c, 500, "", "{%m:%m}\n", MG_ESC("error"), MG_ESC("Unsupported URI")); 
      }
    }
}

int main() {
    int flag = 0;
    
    // Запуск тестов
    if (test_the_size_data_type() != 0) {
        perror("Ошибка при запуске тестов");
        flag = 1;
    }
    

    // Запуск сервера:
    struct mg_mgr mgr; // Объявляем менеджер событий библиотеки Mongoose
    mg_mgr_init(&mgr);

    // Получаем параметры из переменных окружения:
    const char *host = getenv("HOST");
    const char *port = getenv("PORT");

    char listen_addr[MAX_URL_LENGTH];
    snprintf(listen_addr, sizeof(listen_addr), "http://%s:%s", host, port);
  
    // Настраиваем сервер, ev_handler - функция, срабатывающая при получении HTTP-запросов
    struct mg_connection *c = mg_http_listen(&mgr, listen_addr, ev_handler, NULL);
    if (c == NULL) {
        fprintf(stderr, "Не удалось запустить сервер на %s\n", listen_addr);
        return 1;
    }

    printf("Сервер запущен: %s\n", listen_addr);

    pid_t pid = getpid();
    printf("Номер процесса, PID: %d\n", pid);
    printf("Убить процесс: kill %d\n", pid);

    for (;;) {
      mg_mgr_poll(&mgr, 500);// Проверяет входящие соединения c интервалом 0.5 сек
    }

    return flag;
}