use std::io; 

// функция для нахождения наибольшего общего делителя двух чисел
fn find_gcd(mut a: i32, mut b: i32) -> i32 {
    // пока второе число не равно нулю
    while b != 0 {
        let temp = b; // сохраняем второе число
        b = a % b; // находим остаток от деления
        a = temp; // меняем первое число на бывшее второе
    }
    a // возвращаем gcd
}

// функция для вычисления суммы ряда
// возвращает кортеж (числитель, знаменатель)
fn compute_series_sum(power: i32, base: i32) -> (i32, i32) {
    // если степень равна 0, это базовый случай
    if power == 0 {
        // возвращаем 1/(b-1)
        return (1, base - 1);
    }

    // создаем переменные для суммы: числитель и знаменатель
    let mut total_num = 0; // общий числитель
    let mut total_den = 1; // общий знаменатель

    // суммируем все члены от 0 до power-1
    for current_power in 0..power {
        // получаем текущий член рекурсивно
        let (curr_num, curr_den) = compute_series_sum(current_power, base);
        // складываем дроби: (a/b + c/d) = (ad + cb)/(bd)
        let new_num = total_num * curr_den + curr_num * total_den;
        let new_den = total_den * curr_den;

        // сокращаем дробь
        let gcd_value = find_gcd(new_num.abs(), new_den.abs());
        total_num = new_num / gcd_value;
        total_den = new_den / gcd_value;
    }

    // применяем итоговую формулу: (base * sum) / ((base - 1) * 1)
    let final_num = base * total_num;
    let final_den = (base - 1) * total_den;

    // сокращаем итоговую дробь
    let gcd_final = find_gcd(final_num.abs(), final_den.abs());
    (final_num / gcd_final, final_den / gcd_final)
}

fn main() {
    // создаем буфер для чтения строки
    let mut input = String::new();

    // просим пользователя ввести два числа
    println!("введите два числа (от 1 до 10):");
    // читаем строку
    io::stdin().read_line(&mut input).expect("не удалось прочитать строку");
    // разбиваем по пробелам и парсим
    let parts: Vec<&str> = input.trim().split_whitespace().collect();

    // парсим первое число
    let input_a: i32 = parts.get(0).unwrap_or(&"0").parse().unwrap_or(0);
    // парсим второе число
    let input_b: i32 = parts.get(1).unwrap_or(&"0").parse().unwrap_or(0);

    // проверка диапазона чисел
    if input_a < 1 || input_a > 10 || input_b < 1 || input_b > 10 {
        println!("ошибка: числа должны быть от 1 до 10");
        return;
    }

    // проверка на расходящийся ряд (если base == 1)
    if input_b == 1 {
        println!("infinity");
        return;
    }

    // вычисляем сумму ряда
    let (result_num, result_den) = compute_series_sum(input_a, input_b);

    // выводим результат в виде дроби
    println!("{}/{}", result_num, result_den);
}
