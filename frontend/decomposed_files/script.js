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