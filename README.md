# Calculator in C

![LICENSE: GPLv2](https://img.shields.io/badge/LICENSE:-GPLv2-000080)
![Version: 1.0](https://img.shields.io/badge/Version:-1.0-B50080)
![Last Commit](https://img.shields.io/github/last-commit/CombiningIdeas/calculator_in_C)
![Language](https://img.shields.io/badge/language-C-yellow)
![Contributors](https://img.shields.io/github/contributors/CombiningIdeas/calculator_in_C)
![Repo Size](https://img.shields.io/github/repo-size/CombiningIdeas/calculator_in_C)
![Stars](https://img.shields.io/github/stars/CombiningIdeas/calculator_in_C?style=social)

### Проект представляет собой веб-калькулятор, реализованный на языке C с использованием библиотеки [Mongoose](https://github.com/cesanta/mongoose) для запуска веб-сервера. Интерфейс написан на HTML. Серверная часть выполняет вычисления, обрабатывая POST  и GET запросы от клиента. <br/>
### Ссылка на полную документацию в проекта через средства автоматизации генерации документации на основе исходных текстов программ - Doxygen: [Документация](docs/docs.md)

---

- [📁 Структура проекта](#📁-структура-проекта)
- [⚙️ Сборка проекта](#⚙️-сборка-проекта)
- [🚀 Запуск сервера](#🚀-запуск-сервера)
- [💡 Дополнительно](#💡-дополнительно)
- [🔧 Зависимости](#🔧-зависимости)
- [📌 Автор](#📌-автор)
- [📄 Лицензия](#📄-лицензия)

---

## 📁 Структура проекта

```
/Calculator_in_C
│── backend
│   │── build/ 
│   │   │── package/
│   │   │   │── main
│   │── configs/ 
│   │   │── config.env
│   │   │── config.h
│   │── src/
│   │   │── calculator/
│   │   │   │── calculator.c
│   │   │   │── calculator.h
│   │   │── output/
│   │   │   │── output.c
│   │   │   │── output.h
│   │   │── main.c
│   │   │── main.h
│   │── tests/
│   │   │── test_the_size_data_type.c
│   │   │── test_the_size_data_type.h
│── docs/
│   │── html/
│   │── latex/
│   │── docs.md
│── frontend/
│   │── decomposed_files/
│   │   │── advertising_integration.html
│   │   │── background.svg
│   │   │── index.html
│   │   │── style.css
│   │   │── script.js
│   │── MainPage.html
│   │── OutputPage.html
├── mongoose/
│   │── mogoose.c
│   │── mogoose.h
│   │── LICENSE
│   │── README.md
│── .gitignore
│── Doxygen
│── LICENSE.md
│── Makefile
│── README.md
```

---

## ⚙️ Сборка проекта

Для сборки сервера будет использоваться Makefile. Но тут так же будут указана часть команд, которые можно вручную написать в консоль для изменения каких-либо параметров.<br/>
К примеру установить хост и порт можно через консоль:
```bash
export HOST=... PORT=...
```

Проверить их так же можно через консоль:
```bash
echo $HOST $PORT
```

### Сборка, пересборка, запуск и очистка проекта с помощью make-команд:

### 🔨 1. Сборка проекта

Сборка проекта:
```bash
make
```
Эта команда скомпилирует .c-файлы в .o-файлы;


### 🧹 2. Очистка проекта

Удаления всех объектных файлов, кроме скомпилированного .o-файла:
```bash
make clean
```

Для полной очистки (включая финальный .o-файл):
```bash
make all-clean
```

### 🔁 3. Перекомпиляция проекта

Для удаления старого и создания нового итогового .o-файл:
```bash
make rebuild
```

## 🚀 Запуск проекта

Запуск проекта(сервера):

```bash
make run
```

## 💡 Дополнительно

+ Серверная часть обрабатывает математические выражения, переданные из формы на index.html, и возвращает результат пользователю.

+ Используется HTML-форма для отправки запроса, JavaScript и CSS отсутствуют в основной логике вычислений.

+ Расширенная документация может быть сгенерирована с помощью Doxygen из backend/Doxygen/.

+ Использование автоматичской документации проекта на основе исходного кода через Doxygen:
```bash
doxygen -g Doxyfile
```
 Подробное описание настроек Doxygen-файла для проекта есть в другом моем github проекте: <br/>
 [![link](https://img.shields.io/badge/github-0F0F0F?style=for-the-badge&logo=github&logoColor=0AAE10)](https://github.com/CombiningIdeas/Doxygen-Documentation)
 <br/> Для отключения документирования папки mongoose в файле Doxygen мы написали в одном флаге следующее значение:
 ```bash
 EXCLUDE = mongoose/
 ```


## 🔧 Зависимости

+ Компилятор gcc

+ Библиотека mongoose.c (в папке mongoose/)


## 📌 Автор

+ Этот проект был разработан в учебных целях Иваном Широких.

## 📄 Лицензия

Этот проект распространяется под лицензией **GNU General Public License v2.0**.

Вы можете свободно использовать, изменять и распространять данный код, при условии соблюдения условий GPL. Проект предоставляется "как есть", без каких-либо гарантий.

Текст лицензии доступен в файле [`LICENSE.md`](./LICENSE.md) или по ссылке:  
[http://www.gnu.org/licenses/old-licenses/gpl-2.0.html](http://www.gnu.org/licenses/old-licenses/gpl-2.0.html)


