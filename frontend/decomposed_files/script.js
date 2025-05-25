// Динамическая анимация разлетающегося текста:

// Получаем элемент заголовка по ID
const textElement = document.getElementById('exploding-text');

// Извлекаем текст внутри элемента и убираем пробелы
const textContent = textElement.textContent.trim();

// Очищаем HTML-содержимое элемента перед вставкой
textElement.innerHTML = '';

// Разбиваем текст на отдельные символы
textContent.split('').forEach((char, index) => {
    // Создаем span-обертку для каждой буквы
    const span = document.createElement('span');

    // Помещаем текущий символ в span
    span.textContent = char;

    // Добавляем атрибут data-char с символом (для свечения)
    span.setAttribute('data-char', char);

    // Назначаем CSS-класс для анимации
    span.classList.add('letter');

    // Вычисляем случайный угол для разлета (в радианах)
    const angle = Math.random() * 2.5 * Math.PI;

    // Задаем радиус — до края экрана
    const radius = Math.max(window.innerWidth, window.innerHeight);

    // Вычисляем смещение по X — дальность и направление
    const x = Math.cos(angle) * radius + 'px';

    // Вычисляем смещение по Y — дальность и направление
    const y = Math.sin(angle) * radius + 'px';

    // Генерируем случайный угол поворота от -360° до 360°
    const rotate = Math.floor(Math.random() * 720 - 360) + 'deg';

    // Устанавливаем переменную CSS для движения по X
    span.style.setProperty('--x', x);

    // Устанавливаем переменную CSS для движения по Y
    span.style.setProperty('--y', y);

    // Устанавливаем переменную CSS для поворота буквы
    span.style.setProperty('--r', rotate);

    // Добавляем задержку для волнообразного эффекта
    span.style.animationDelay = `${index * 0.08}s`;

    // Добавляем созданный span в заголовок
    textElement.appendChild(span);
});