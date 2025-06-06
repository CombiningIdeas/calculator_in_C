# Компилятор:
CC = gcc

# Флаг cUnit:
#LDFLAGS = -lcunit

#Фалги компиляции:
#CFLAGS = -Wall -Wextra -g -std=c11 -DDEBUG

# Объектные файлы(.о-файлы)
OBJ_MAIN = mongoose/mongoose.o \
 backend/src/calculator/calculator.o \
 backend/src/output/output.o \
 backend/tests/test_the_size_data_type.o \
 backend/src/main.o


# Имена для финальных исполняемых файлов
TARGET = backend/build/package/main


# Компиляция  main-исполняемого файла:
$(TARGET): $(OBJ_MAIN)
	$(CC)  $(OBJ_MAIN) -o $(TARGET)
#$(CFLAGS)


# Правила компиляции каждого .c-файла в .o-файл
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@


# Запуск проекта
run:
	@HOST=127.0.0.1 PORT=8080 bash -c '\
	trap "echo Завершено; exit 0" SIGINT SIGTERM; \
	./backend/build/package/main; \
	exit_code=$$?; \
	if [ $$exit_code -eq 143 ]; then exit 0; else exit $$exit_code; fi'


# Очистка
clean:
	rm -f $(OBJ_MAIN)

all-clean:
	rm -f $(OBJ_MAIN) $(TARGET) 

# Перекомпиляция
rebuild: all-clean $(TARGET)