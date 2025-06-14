use rand::Rng;
use std::collections::HashSet;
use std::time::Instant;
use rand::thread_rng;

// функция для быстрого возведения в степень по модулю
// принимает основание, показатель степени и модуль
// возвращает результат возведения в степень по модулю
fn mod_pow(base: u64, exponent: u64, modulus: u64) -> u64 {
    // проверяем тривиальный случай
    if modulus == 1 {
        return 0;
    }
    // инициализируем результат
    let mut result = 1;
    // берем основание по модулю, чтобы избежать переполнения
    let mut base = base % modulus;
    // создаем копию показателя степени для изменений
    let mut exponent = exponent;

    // основной цикл возведения в степень
    while exponent > 0 {
        // если показатель степени нечетный
        if exponent % 2 == 1 {
            // умножаем результат на основание по модулю
            result = (result * base) % modulus;
        }
        // делим показатель степени на 2
        exponent >>= 1;
        // возводим основание в квадрат по модулю
        base = (base * base) % modulus;
    }
    // возвращаем конечный результат
    result
}

// реализация решета эратосфена для нахождения всех простых чисел до n
// возвращает вектор простых чисел
fn sieve_of_eratosthenes(n: usize) -> Vec<u64> {
    // если n меньше 2, возвращаем пустой вектор
    if n < 2 {
        return Vec::new();
    }

    // создаем вектор булевых значений, изначально все true
    let mut is_prime = vec![true; n + 1];
    is_prime[0] = false;
    is_prime[1] = false;

    // основной цикл просеивания
    for p in 2..=((n as f64).sqrt() as usize) {
        if is_prime[p] {
            // отмечаем все кратные p числа как составные
            let mut multiple = p * p;
            while multiple <= n {
                is_prime[multiple] = false;
                multiple += p;
            }
        }
    }

    // собираем все простые числа в вектор
    is_prime.iter()
        .enumerate()
        .filter(|&(_, &is_p)| is_p)
        .map(|(i, _)| i as u64)
        .collect()
}

// структура для реализации алгоритма поклингтона
struct PocklingtonPrimality;

impl PocklingtonPrimality {
    // метод для проверки числа на простоту по тесту поклингтона
    // принимает число для проверки, количество тестов и вектор множителей
    // возвращает true, если число вероятно простое
    fn is_prime(n: u64, tests: usize, factors: &[u64]) -> bool {
        // создаем генератор случайных чисел
        let mut rng = thread_rng();
        // множество для хранения уникальных баз
        let mut used_bases = HashSet::new();

        // генерируем необходимое количество уникальных баз
        while used_bases.len() != tests {
            // получаем случайное основание
            let base = rng.gen_range(2..n);
            // добавляем в множество
            used_bases.insert(base);
        }

        // проверка первого условия теста поклингтона
        for &base in &used_bases {
            // если a^(n-1) не сравнимо с 1 по модулю n
            if mod_pow(base, n - 1, n) != 1 {
                // число составное
                return false;
            }
        }

        // проверка второго условия теста поклингтона
        for &base in &used_bases {
            // флаг, что число составное
            let mut is_composite = true;
            // проверяем для каждого множителя
            for &factor in factors {
                // если a^((n-1)/q) сравнимо с 1 по модулю n
                if mod_pow(base, (n - 1) / factor, n) == 1 {
                    // число может быть простым
                    is_composite = false;
                    break;
                }
            }
            // если для всех множителей условие выполнено
            if is_composite {
                // число вероятно простое
                return true;
            }
        }

        // если не нашли подходящего основания, число составное
        false
    }

    // генерация кандидата на простое число и его множителей
    // принимает вектор простых чисел и битовую длину
    // возвращает кортеж из кандидата и вектора множителей
    fn generate_candidate(primes: &[u64], bits: usize) -> (u64, Vec<u64>) {
        // создаем генератор случайных чисел
        let mut rng = thread_rng();
        // максимальный индекс в векторе простых чисел
        let max_index = primes.len() - 1;
        // минимальное значение для R
        let min = (bits / 2 + 1) as u64;
        // максимальное значение для R
        let max = (bits / 2 + 2) as u64;
        // 2 в степени max
        let pow_max = 2u64.pow(max as u32);

        // вычисляем максимальную степень для множителей
        let mut max_pow = 1;
        while 2u64.pow(max_pow) < 2u64.pow((bits / 2 + 1) as u32) {
            max_pow += 1;
        }

        // инициализация множителя f
        let mut f = 1;
        // вектор для хранения множителей
        let mut factor_list = Vec::new();

        // генерация множителей f
        loop {
            // выбираем случайный индекс простого числа
            let rand_idx = rng.gen_range(0..=max_index);
            // выбираем случайную степень
            let rand_pow = rng.gen_range(1..=max_pow);
            // вычисляем простое число в степени
            let prime_pow = primes[rand_idx].pow(rand_pow);

            // проверяем, что произведение не превышает pow_max
            if f * prime_pow < pow_max {
                // умножаем f на простое число в степени
                f *= prime_pow;
                // добавляем простое число в список множителей
                factor_list.push(primes[rand_idx]);
            }

            // если f вышло за границы
            if f > 2u64.pow((bits / 2) as u32) {
                // если f слишком большое, начинаем заново
                if f >= 2u64.pow((bits / 2 + 1) as u32) {
                    f = 1;
                    factor_list.clear();
                } else {
                    // иначе завершаем генерацию
                    break;
                }
            }
        }

        // генерируем случайное R
        let mut R = rng.gen_range(min - 1..max - 1);
        // делаем R четным
        if R % 2 != 0 {
            R += 1;
        }
        // вычисляем кандидата n = R*f + 1
        let candidate = R * f + 1;
        // возвращаем кандидата и множители
        (candidate, factor_list)
    }

    // метод для генерации простых чисел алгоритмом поклингтона
    // принимает битовую длину, количество тестов и количество чисел для генерации
    pub fn generate_primes(bits: usize, tests: usize, count: usize) {
        // получаем список простых чисел до 500
        let primes = sieve_of_eratosthenes(500);
        // вектор для хранения найденных простых чисел
        let mut candidates = Vec::new();
        // вектор для хранения результатов тестов
        let mut test_results = Vec::new();
        // вектор для хранения количества попыток
        let mut attempts = Vec::new();
        // счетчик текущих попыток
        let mut current_attempts = 0;

        // выводим сообщение о начале генерации
        println!("начало генерации простых чисел алгоритмом поклингтона...");

        // генерируем указанное количество простых чисел
        while candidates.len() < count {
            // генерируем кандидата и его множители
            let (n, factors) = Self::generate_candidate(&primes, bits);

            // проверяем кандидата на простоту
            if Self::is_prime(n, tests, &factors) {
                // если число прошло тест, добавляем в результат
                candidates.push(n);
                // дополнительная проверка тестом миллера-рабина
                let test_result = if Self::miller_rabin_test(n, 3) { "+" } else { "-" };
                test_results.push(test_result.to_string());
                // сохраняем количество попыток
                attempts.push(current_attempts);
                // сбрасываем счетчик попыток
                current_attempts = 0;
                // выводим прогресс
                println!("найдено простых чисел: {} из {}", candidates.len(), count);
            } else {
                // если число не прошло тест
                if Self::miller_rabin_test(n, 1) {
                    // увеличиваем счетчик попыток
                    current_attempts += 1;
                }
            }
        }

        // выводим результаты
        Self::print_results(&candidates, &test_results, &attempts);
    }

    // вспомогательный метод для теста миллера-рабина
    // принимает число для проверки и количество тестов
    // возвращает true, если число вероятно простое
    fn miller_rabin_test(num: u64, tests: usize) -> bool {
    // Обработка тривиальных случаев
    if num == 2 || num == 3 {
        return true;
    }
    if num < 2 || num % 2 == 0 {
        return false;
    }

    // Раскладываем num-1 на d * 2^s
    let mut d = num - 1;
    let mut s = 0;
    while d % 2 == 0 {
        d /= 2;
        s += 1;
    }

    let mut rng = thread_rng();
    
    // Проводим указанное количество тестов
    for _ in 0..tests {
        let a = rng.gen_range(2..num - 1);
        let mut x = mod_pow(a, d, num);
        
        // Если x равно 1 или num-1, сразу переходим к следующему тесту
        if x == 1 || x == num - 1 {
            continue;
        }
        
        // Проводим s-1 итераций
        let mut i = 0;
        while i < s - 1 {
            x = mod_pow(x, 2, num);
            if x == num - 1 {
                break;
            }
            i += 1;
        }
        
        // Если не нашли свидетельства простоты
        if i == s - 1 && x != num - 1 {
            return false;
        }
    }
    
    true
}

    // метод для вывода результатов
    // принимает векторы найденных чисел, результатов тестов и количества попыток
    fn print_results(primes: &[u64], tests: &[String], attempts: &[usize]) {
        // выводим заголовок
        println!("\nрезультаты генерации алгоритмом поклингтона:");
        println!("--------------------------------------------------");
        println!("номер\tпростое число\tтест\tпопыток");
        println!("--------------------------------------------------");

        // выводим все найденные простые числа
        for (i, (prime, test)) in primes.iter().zip(tests.iter()).enumerate() {
            println!("{}\t{}\t{}\t{}", i + 1, prime, test, attempts[i]);
        }

        // выводим разделитель
        println!("--------------------------------------------------");
    }
}

// структура для реализации алгоритма миллера
struct MillerPrimality;

impl MillerPrimality {
    // метод для проверки числа на простоту по тесту миллера
    // принимает кандидата, количество тестов и вектор множителей
    // возвращает true, если число вероятно простое
    fn is_prime(candidate: u64, test_count: usize, factors: &[u64]) -> bool {
        // создаем генератор случайных чисел
        let mut rng = thread_rng();
        // множество для хранения уникальных баз
        let mut bases = HashSet::new();

        // генерируем необходимое количество уникальных баз
        while bases.len() < test_count {
            // получаем случайное основание
            let base = rng.gen_range(2..candidate - 1);
            // добавляем в множество
            bases.insert(base);
        }

        // проверка первого условия теста миллера
        for &base in &bases {
            // если a^(n-1) не сравнимо с 1 по модулю n
            if mod_pow(base, candidate - 1, candidate) != 1 {
                // число составное
                return false;
            }
        }

        // проверка второго условия теста миллера
        for &factor in factors {
            // флаг, что все основания дали 1
            let mut all_ones = true;
            // проверяем для каждого основания
            for &base in &bases {
                // вычисляем показатель степени
                let exponent = (candidate - 1) / factor;
                // если a^((n-1)/q) не сравнимо с 1 по модулю n
                if mod_pow(base, exponent, candidate) != 1 {
                    // не все основания дали 1
                    all_ones = false;
                    break;
                }
            }
            // если для какого-то множителя все основания дали 1
            if all_ones {
                // число составное
                return false;
            }
        }

        // если все условия выполнены, число вероятно простое
        true
    }

    // генерация кандидата на простое число и его множителей
    // принимает вектор простых чисел и битовую длину
    // возвращает пару из кандидата и вектора множителей
    fn generate_candidate(primes: &[u64], bit_length: usize) -> (u64, Vec<u64>) {
        // создаем генератор случайных чисел
        let mut rng = thread_rng();
        // нижняя граница для m
        let lower_bound = 2u64.pow((bit_length - 2) as u32);
        // верхняя граница для m
        let upper_bound = 2u64.pow((bit_length - 1) as u32)- 1;
        
        // инициализация m
        let mut m = 1;
        // вектор для хранения множителей
        let mut factors = Vec::new();

        // генерация множителей m
        loop {
            // выбираем случайный индекс простого числа
            let random_index = rng.gen_range(0..primes.len());
            // получаем простое число по индексу
            let random_prime = primes[random_index];
            
            // проверяем, не превысили ли верхнюю границу
            if m * random_prime > upper_bound {
                // если достигли нижней границы, завершаем
                if m >= lower_bound {
                    break;
                }
                // иначе начинаем заново
                m = 1;
                factors.clear();
                continue;
            }
            
            // умножаем m на простое число
            m *= random_prime;
            // добавляем простое число в множители
            factors.push(random_prime);
        }

        // возвращаем кандидата n = 2*m + 1 и множители
        (2 * m + 1, factors)
    }

    // метод для генерации простых чисел алгоритмом миллера
    // принимает битовую длину, количество тестов и количество чисел для генерации
    pub fn generate_primes(bit_length: usize, test_count: usize, target_count: usize) {
        // получаем список простых чисел до 500
        let small_primes = sieve_of_eratosthenes(500);
        // вектор для хранения найденных простых чисел
        let mut primes_found = Vec::new();
        // вектор для хранения результатов тестов
        let mut test_results = Vec::new();
        // вектор для хранения количества попыток
        let mut attempts_count = Vec::new();
        // счетчик текущих попыток
        let mut attempts = 0;

        // выводим сообщение о начале генерации
        println!("начало генерации простых чисел алгоритмом миллера...");

        // генерируем указанное количество простых чисел
        while primes_found.len() < target_count {
            // генерируем кандидата и его множители
            let (candidate, factors) = Self::generate_candidate(&small_primes, bit_length);
            
            // проверяем кандидата на простоту
            if Self::is_prime(candidate, test_count, &factors) {
                // если число прошло тест, добавляем в результат
                primes_found.push(candidate);
                // дополнительная проверка тестом миллера-рабина
                let test_result = if Self::miller_rabin_test(candidate, 3) { "+" } else { "-" };
                test_results.push(test_result.to_string());
                // сохраняем количество попыток
                attempts_count.push(attempts);
                // сбрасываем счетчик попыток
                attempts = 0;
                // выводим прогресс
                println!("найдено простых чисел: {} из {}", primes_found.len(), target_count);
            } else {
                // если число не прошло тест
                if Self::miller_rabin_test(candidate, 1) {
                    // увеличиваем счетчик попыток
                    attempts += 1;
                }
            }
        }

        // выводим результаты
        Self::print_results(&primes_found, &test_results, &attempts_count);
    }

    // вспомогательный метод для теста миллера-рабина
    // принимает число для проверки и количество тестов
    // возвращает true, если число вероятно простое
  fn miller_rabin_test(num: u64, tests: usize) -> bool {
    // Обработка тривиальных случаев
    if num == 2 || num == 3 {
        return true;
    }
    if num < 2 || num % 2 == 0 {
        return false;
    }

    // Раскладываем num-1 на d * 2^s
    let mut d = num - 1;
    let mut s = 0;
    while d % 2 == 0 {
        d /= 2;
        s += 1;
    }

    let mut rng = thread_rng();
    
    // Проводим указанное количество тестов
    for _ in 0..tests {
        let a = rng.gen_range(2..num - 1);
        let mut x = mod_pow(a, d, num);
        
        // Если x равно 1 или num-1, сразу переходим к следующему тесту
        if x == 1 || x == num - 1 {
            continue;
        }
        
        // Проводим s-1 итераций
        let mut i = 0;
        while i < s - 1 {
            x = mod_pow(x, 2, num);
            if x == num - 1 {
                break;
            }
            i += 1;
        }
        
        // Если не нашли свидетельства простоты
        if i == s - 1 && x != num - 1 {
            return false;
        }
    }
    
    true
}


    // метод для вывода результатов
    // принимает векторы найденных чисел, результатов тестов и количества попыток
    fn print_results(primes: &[u64], tests: &[String], attempts: &[usize]) {
        // выводим заголовок
        println!("\nрезультаты генерации алгоритмом миллера:");
        println!("--------------------------------------------------");
        println!("номер\tпростое число\tтест\tпопыток");
        println!("--------------------------------------------------");

        // выводим все найденные простые числа
        for (i, (prime, test)) in primes.iter().zip(tests.iter()).enumerate() {
            println!("{}\t{}\t{}\t{}", i + 1, prime, test, attempts[i]);
        }

        // выводим разделитель
        println!("--------------------------------------------------");
    }
}

// структура для реализации алгоритма гост р 34.10-94
struct GOSTPrimality;

impl GOSTPrimality {
    // метод для генерации простого числа по гост
    // принимает вектор простых чисел и битовую длину
    // возвращает простое число
    fn generate_prime(primes: &[u64], bit_len: usize) -> u64 {
    // создаем генератор случайных чисел
    let mut rng = thread_rng();
    // вычисляем длину q в битах
    let q_bit_len = (bit_len + 1) / 2;
    // минимальное значение для q
    let q_min = 0;
    // максимальное значение для q
    let q_max = (1 << q_bit_len) - 1;

    // переменная для хранения q
    let q = loop {
        // выбираем случайное простое число из списка
        let candidate = primes[rng.gen_range(0..primes.len())];
        // проверяем, что оно подходит по размеру
        if candidate >= q_min as u64 && candidate <= q_max as u64 {
            break candidate;
        }
    };

    // основной цикл генерации простого числа p
    loop {
        // генерируем случайное число xi в диапазоне [0, 1)
        let xi = rng.gen_range(0.0..1.0);
        // вычисляем n по формуле (используем as f64 вместо from)
        let n = ((1u64 << (bit_len - 1)) as f64 + (xi * (1u64 << (bit_len - 1)) as f64)) / q as f64;
        // преобразуем n в целое число
        let mut n_int = n as u64;
        // если n_int нечетное, делаем его четным
        if n_int % 2 != 0 {
            n_int += 1;
        }

        // поиск подходящего p = (n_int + k)*q + 1
        for k in (0..).step_by(2) {
            // вычисляем кандидата p
            let p = (n_int + k) * q + 1;
            // если p превысило максимальное значение, выходим из цикла
            if p > (1 << bit_len) as u64 {
                break;
            }

            // проверка условий простоты
            if mod_pow(2, p - 1, p) == 1 && mod_pow(2, n_int + k, p) != 1 {
                // если условия выполнены, возвращаем p
                return p;
            }
        }
    }
}

    // метод для генерации простых чисел по гост
    // принимает битовую длину, количество тестов и количество чисел для генерации
    pub fn generate_primes(bit_length: usize, test_rounds: usize, target_count: usize) {
        // множество для хранения уникальных простых чисел
        let mut primes_set = HashSet::new();
        // получаем список простых чисел до 500
        let primes = sieve_of_eratosthenes(500);
        // вектор для хранения результатов тестов
        let mut test_results = Vec::new();
        // счетчик отвергнутых чисел
        let mut rejected = 0;

        // выводим сообщение о начале генерации
        println!("начало генерации простых чисел по гост...");
        println!("это может занять некоторое время...");

        // замеряем время выполнения
        let start_time = Instant::now();

        // генерируем указанное количество уникальных простых чисел
        while primes_set.len() < target_count {
            // генерируем кандидата p
            let p = Self::generate_prime(&primes, bit_length);
            // если число уже есть в множестве, пропускаем его
            if primes_set.contains(&p) {
                continue;
            }

            // проверяем кандидата тестом миллера-рабина
            if Self::miller_rabin_test(p, test_rounds) {
                // если тест пройден, добавляем число в множество
                primes_set.insert(p);
                test_results.push("пройден".to_string());
                // выводим прогресс
                println!("найдено простых чисел: {} из {}", primes_set.len(), target_count);
            } else {
                // увеличиваем счетчик отвергнутых чисел
                rejected += 1;
            }
        }

        // выводим время выполнения
        let duration = start_time.elapsed();
        println!("время выполнения: {:?}", duration);

        // выводим результаты
        Self::print_results(&primes_set, &test_results, rejected);
    }

    // вспомогательный метод для теста миллера-рабина
    // принимает число для проверки и количество тестов
    // возвращает true, если число вероятно простое
    fn miller_rabin_test(num: u64, tests: usize) -> bool {
    // Обработка тривиальных случаев
    if num == 2 || num == 3 {
        return true;
    }
    if num < 2 || num % 2 == 0 {
        return false;
    }

    // Раскладываем num-1 на d * 2^s
    let mut d = num - 1;
    let mut s = 0;
    while d % 2 == 0 {
        d /= 2;
        s += 1;
    }

    let mut rng = thread_rng();
    
    // Проводим указанное количество тестов
    for _ in 0..tests {
        let a = rng.gen_range(2..num - 1);
        let mut x = mod_pow(a, d, num);
        
        // Если x равно 1 или num-1, сразу переходим к следующему тесту
        if x == 1 || x == num - 1 {
            continue;
        }
        
        // Проводим s-1 итераций
        let mut i = 0;
        while i < s - 1 {
            x = mod_pow(x, 2, num);
            if x == num - 1 {
                break;
            }
            i += 1;
        }
        
        // Если не нашли свидетельства простоты
        if i == s - 1 && x != num - 1 {
            return false;
        }
    }
    
    true
}

    // метод для вывода результатов
    // принимает множество простых чисел, результаты тестов и количество отвергнутых чисел
    fn print_results(primes: &HashSet<u64>, tests: &[String], rejected: usize) {
        // выводим заголовок
        println!("\nрезультаты генерации алгоритмом гост р 34.10-94:");
        println!("--------------------------------------------------");
        println!("номер\tпростое число\tтест");
        println!("--------------------------------------------------");

        // выводим все найденные простые числа
        for (i, prime) in primes.iter().enumerate() {
            println!("{}\t{}\t{}", i + 1, prime, tests[i]);
        }

        // выводим статистику
        println!("--------------------------------------------------");
        println!("всего отвергнуто чисел: {}", rejected);
        println!("--------------------------------------------------");
    }
}

// главная функция программы
fn main() {
    // выводим приветственное сообщение
    println!("генерация простых чисел различными алгоритмами");
    println!("--------------------------------------------------\n");

    // переменная для хранения выбора пользователя
    let mut choice = String::new();
    // основной цикл меню
    loop {
        // выводим варианты выбора
        println!("выберите алгоритм:");
        println!("1. алгоритм поклингтона");
        println!("2. алгоритм миллера");
        println!("3. алгоритм гост р 34.10-94");
        println!("0. выход");
        print!("ваш выбор: ");
        // сбрасываем буфер вывода
        std::io::Write::flush(&mut std::io::stdout()).unwrap();
        // считываем выбор пользователя
        std::io::stdin().read_line(&mut choice).unwrap();

        // обработка выбора пользователя
        match choice.trim() {
            "1" => {
                // генерация чисел алгоритмом поклингтона
                println!("\nгенерация простых чисел алгоритмом поклингтона");
                PocklingtonPrimality::generate_primes(13, 10, 10);
            }
            "2" => {
                // генерация чисел алгоритмом миллера
                println!("\nгенерация простых чисел алгоритмом миллера");
                MillerPrimality::generate_primes(10, 5, 10);
            }
            "3" => {
                // генерация чисел алгоритмом гост
                println!("\nгенерация простых чисел алгоритмом гост р 34.10-94");
                // уменьшаем параметры для ускорения работы
                GOSTPrimality::generate_primes(10, 3, 5);
            }
            "0" => {
                // выход из программы
                println!("выход из программы.");
                break;
            }
            _ => {
                // неверный выбор
                println!("неверный выбор. попробуйте снова.");
            }
        }
        println!();
        choice.clear();
    }
}
