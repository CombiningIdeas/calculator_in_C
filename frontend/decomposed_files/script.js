// // Динамическая анимация разлетающегося текста:

// // Получаем элемент заголовка по ID
// const textElement = document.getElementById('exploding-text');

// // Извлекаем текст внутри элемента и убираем пробелы
// const textContent = textElement.textContent.trim();

// // Очищаем HTML-содержимое элемента перед вставкой
// textElement.innerHTML = '';

// // Разбиваем текст на отдельные символы
// textContent.split('').forEach((char, index) => {
//     // Создаем span-обертку для каждой буквы
//     const span = document.createElement('span');

//     // Помещаем текущий символ в span
//     span.textContent = char;

//     // Добавляем атрибут data-char с символом (для свечения)
//     span.setAttribute('data-char', char);

//     // Назначаем CSS-класс для анимации
//     span.classList.add('letter');

//     // Вычисляем случайный угол для разлета (в радианах)
//     const angle = Math.random() * 2.5 * Math.PI;

//     // Задаем радиус — до края экрана
//     const radius = Math.max(window.innerWidth, window.innerHeight);

//     // Вычисляем смещение по X — дальность и направление
//     const x = Math.cos(angle) * radius + 'px';

//     // Вычисляем смещение по Y — дальность и направление
//     const y = Math.sin(angle) * radius + 'px';

//     // Генерируем случайный угол поворота от -360° до 360°
//     const rotate = Math.floor(Math.random() * 720 - 360) + 'deg';

//     // // Устанавливаем переменную CSS для движения по X
//     // span.style.setProperty('--x', x);
//     // // Устанавливаем переменную CSS для движения по Y
//     // span.style.setProperty('--y', y);

//     //Aктивировать GPU, более стабильной интерполяцией, по сравнению с CPU:
//     span.style.setProperty('--x', `translate3d(${x}, ${y}, 0)`);

//     // Устанавливаем переменную CSS для поворота буквы
//     span.style.setProperty('--r', rotate);

//     // Добавляем задержку для волнообразного эффекта
//     span.style.animationDelay = `${index * 0.08}s`;

//     // Добавляем созданный span в заголовок
//     textElement.appendChild(span);
// });


const textElement = document.getElementById('exploding-text');
const textContent = textElement.textContent.trim();
textElement.innerHTML = '';

 const allChars = textContent.split('');

// Определим, сколько букв анимировать
let animateCount = allChars.length; // по умолчанию все

if (navigator.hardwareConcurrency <= 4) {
    animateCount = 2; // только первые 2
} else if (navigator.hardwareConcurrency <= 6) {
    animateCount = 6; // только первые 6
} else if (navigator.hardwareConcurrency <= 8) {
    animateCount = 8; // только первые 8
}  

allChars.forEach((char, index) => {
    const span = document.createElement('span');
    span.textContent = char;
    span.setAttribute('data-char', char);

    if (index < animateCount) {
        // Только для анимированных букв
        span.classList.add('letter');

        const angle = Math.random() * 2.5 * Math.PI;
        const radius = Math.max(window.innerWidth, window.innerHeight);
        const x = Math.cos(angle) * radius + 'px';
        const y = Math.sin(angle) * radius + 'px';
        const rotate = Math.floor(Math.random() * 720 - 360) + 'deg';

        span.style.setProperty('--x', `translate3d(${x}, ${y}, 0)`);
        span.style.setProperty('--r', rotate);
        span.style.animationDelay = `${index * 0.08}s`;
    } else {
        // Без анимации
        span.style.opacity = '1';
    }

    textElement.appendChild(span);
});


// Проверка на валидность: разрешены только цифры, пробелы, точки и круглые скобки
document.getElementById('calculator-form').addEventListener('submit', function(event) {
    const input = document.querySelector('input[name="expression"]');
    const expression = input.value.trim();

    // Проверка недопустимых символов
    if (/[^0-9+\-*/().\s]/.test(expression)) {
        alert('Ошибка: допустимы только цифры, пробелы, точки, круглые скобки и операторы + - * /');
        event.preventDefault();
        return;
    }

    // Удаляем пробелы для упрощения
    const expr = expression.replace(/\s+/g, '');

    // Проверка баланса скобок
    let balance = 0;
    for (const ch of expr) {
        if (ch === '(') balance++;
        else if (ch === ')') balance--;
        if (balance < 0) {
            alert('Ошибка: закрывающая скобка без открывающей');
            event.preventDefault();
            return;
        }
    }
    if (balance !== 0) {
        alert('Ошибка: несбалансированные скобки');
        event.preventDefault();
        return;
    }

    // Токенизация — разбиваем выражение на токены: числа, операторы, скобки
    const tokens = [];
    const regex = /\d+(\.\d+)?|[+\-*/()]|./g;
    let match;
    while ((match = regex.exec(expr)) !== null) {
        tokens.push(match[0]);
    }

    // Функции для проверки типа токена
    const isNumber = (t) => /^\d+(\.\d+)?$/.test(t);
    const isOperator = (t) => /^[+\-*/]$/.test(t);
    const isOpenPar = (t) => t === '(';
    const isClosePar = (t) => t === ')';

    for (let i = 0; i < tokens.length; i++) {
        const token = tokens[i];
        const prev = tokens[i - 1];
        const next = tokens[i + 1];

        if (isNumber(token)) {
            // после числа может идти оператор, закрывающая скобка или конец выражения
            if (next && !(isOperator(next) || isClosePar(next))) {
                alert(`Ошибка: после числа '${token}' ожидается оператор или ')'`);
                event.preventDefault();
                return;
            }
        } else if (isOperator(token)) {
            if (token === '-') {
                // минус может быть унарным, если стоит в начале или после оператора или после '('
                if (!(i === 0 || (prev && (isOperator(prev) || isOpenPar(prev))))) {
                    // бинарный минус, после него должен идти число или '('
                    if (!(next && (isNumber(next) || isOpenPar(next)))) {
                        alert(`Ошибка: после оператора '${token}' должно быть число или '('`);
                        event.preventDefault();
                        return;
                    }
                } else {
                    // унарный минус, должен идти число или '(' после него
                    if (!(next && (isNumber(next) || isOpenPar(next)))) {
                        alert(`Ошибка: после унарного минуса должно быть число или '('`);
                        event.preventDefault();
                        return;
                    }
                }
            } else {
                // остальные операторы — после них должен идти число или '('
                if (!(next && (isNumber(next) || isOpenPar(next)))) {
                    alert(`Ошибка: после оператора '${token}' должно быть число или '('`);
                    event.preventDefault();
                    return;
                }
                // перед оператором должен быть число или ')'
                if (!(prev && (isNumber(prev) || isClosePar(prev)))) {
                    alert(`Ошибка: перед оператором '${token}' должно быть число или ')'`);
                    event.preventDefault();
                    return;
                }
            }
        } else if (isOpenPar(token)) {
            // после '(' может идти унарный минус, число или '('
            if (!(next && (isNumber(next) || isOpenPar(next) || (next === '-' && (tokens[i+2] && (isNumber(tokens[i+2]) || isOpenPar(tokens[i+2]))))))) {
                alert(`Ошибка: после '(' должны идти унарный минус, число или '('`);
                event.preventDefault();
                return;
            }
        } else if (isClosePar(token)) {
            // после ')' может идти оператор, ')' или конец выражения
            if (next && !(isOperator(next) || isClosePar(next))) {
                alert(`Ошибка: после ')' ожидается оператор или ')'`);
                event.preventDefault();
                return;
            }
            // перед ')' должен быть число или ')'
            if (!(prev && (isNumber(prev) || isClosePar(prev)))) {
                alert(`Ошибка: перед ')' должно быть число или ')'`);
                event.preventDefault();
                return;
            }
        } else {
            alert(`Ошибка: недопустимый символ '${token}'`);
            event.preventDefault();
            return;
        }
    }
});