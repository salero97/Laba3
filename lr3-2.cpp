#include <iostream>
#include <vector>
#include <tuple>
#include <set>
#include <cmath>
#include <random>
#include <algorithm>
#include <iomanip>
#include <string>

using namespace std;

// функция для быстрого возведения в степень по модулю
// принимает основание, показатель степени и модуль
// возвращает результат возведения в степень по модулю
int modPow(int base, int exponent, int modulus) {
    // инициализируем результат
    int result = 1;
    // берем основание по модулю, чтобы избежать переполнения
    base %= modulus;
    
    // основной цикл возведения в степень
    while (exponent > 0) {
        // если показатель степени нечетный
        if (exponent % 2 == 1) {
            // умножаем результат на основание по модулю
            result = (result * base) % modulus;
        }
        // делим показатель степени на 2
        exponent >>= 1;
        // возводим основание в квадрат по модулю
        base = (base * base) % modulus;
    }
    // возвращаем конечный результат
    return result;
}

// реализация решета эратосфена для нахождения всех простых чисел до n
// возвращает вектор простых чисел
vector<int> sieveOfEratosthenes(int n) {
    // если n меньше 2, возвращаем пустой вектор
    if (n < 2) return {};

    // создаем вектор булевых значений, изначально все true
    vector<bool> is_prime(n + 1, true);
    is_prime[0] = is_prime[1] = false;

    // основной цикл просеивания
    for (int p = 2; p * p <= n; ++p) {
        if (is_prime[p]) {
            // отмечаем все кратные p числа как составные
            for (int multiple = p * p; multiple <= n; multiple += p) {
                is_prime[multiple] = false;
            }
        }
    }

    // собираем все простые числа в вектор
    vector<int> primes;
    for (int i = 2; i <= n; ++i) {
        if (is_prime[i]) {
            primes.push_back(i);
        }
    }

    return primes;
}

// генерация случайного целого числа в диапазоне [min, max]
int getRandomInt(int min, int max) {
    // создаем устройство для получения случайных чисел
    static random_device rd;
    // инициализируем генератор случайных чисел
    static mt19937 gen(rd());
    // создаем равномерное распределение в заданном диапазоне
    uniform_int_distribution<int> dist(min, max);
    // возвращаем случайное число
    return dist(gen);
}

// генерация случайного вещественного числа в диапазоне [0.0, 1.0)
double getRandomDouble() {
    // создаем устройство для получения случайных чисел
    static random_device rd;
    static mt19937 gen(rd());
    uniform_real_distribution<double> dist(0.0, 1.0);
    // возвращаем случайное число
    return dist(gen);
}

// класс для реализации алгоритма поклингтона
class PocklingtonPrimality {
public:
    // метод для проверки числа на простоту по тесту поклингтона
    // принимает число для проверки, количество тестов и вектор множителей
    // возвращает true, если число вероятно простое
    static bool isPrime(int n, int tests, const vector<int>& factors) {
        // множество для хранения уникальных баз
        set<int> usedBases;

        // генерируем необходимое количество уникальных баз
        while (usedBases.size() != static_cast<size_t>(tests)) {
            // получаем случайное основание
            int base = getRandomInt(2, n - 1);
            // добавляем в множество
            usedBases.insert(base);
        }

        // проверка первого условия теста поклингтона
        for (int base : usedBases) {
            // если a^(n-1) не сравнимо с 1 по модулю n
            if (modPow(base, n - 1, n) != 1) {
                // число составное
                return false;
            }
        }

        // проверка второго условия теста поклингтона
        for (int base : usedBases) {
            // флаг, что число составное
            bool isComposite = true;
            // проверяем для каждого множителя
            for (int factor : factors) {
                // если a^((n-1)/q) сравнимо с 1 по модулю n
                if (modPow(base, (n - 1) / factor, n) == 1) {
                    // число может быть простым
                    isComposite = false;
                    break;
                }
            }
            // если для всех множителей условие выполнено
            if (isComposite) {
                // число вероятно простое
                return true;
            }
        }

        // если не нашли подходящего основания, число составное
        return false;
    }

    // генерация кандидата на простое число и его множителей
    // принимает вектор простых чисел и битовую длину
    // возвращает кортеж из кандидата и вектора множителей
    static tuple<int, vector<int>> generateCandidate(const vector<int>& primes, int bits) {
        // максимальный индекс в векторе простых чисел
        int maxIndex = primes.size() - 1;
        // минимальное значение для R
        int min = bits / 2 + 1;
        // максимальное значение для R
        int max = bits / 2 + 2;
        // 2 в степени max
        int powMax = pow(2, max);

        // вычисляем максимальную степень для множителей
        int maxPow = 1;
        while (pow(2, maxPow) < pow(2, (bits / 2) + 1)) {
            maxPow++;
        }

        // инициализация множителя f
        int f = 1;
        // вектор для хранения множителей
        vector<int> factorList;

        // генерация множителей f
        while (true) {
            // выбираем случайный индекс простого числа
            int randIdx = getRandomInt(0, maxIndex);
            // выбираем случайную степень
            int randPow = getRandomInt(1, maxPow);

            // проверяем, что произведение не превышает powMax
            if (f * pow(primes[randIdx], randPow) < powMax) {
                // умножаем f на простое число в степени
                f *= pow(primes[randIdx], randPow);
                // добавляем простое число в список множителей
                factorList.push_back(primes[randIdx]);
            }

            // если f вышло за границы
            if (f > pow(2, (bits / 2))) {
                // если f слишком большое, начинаем заново
                if (f >= pow(2, (bits / 2) + 1)) {
                    f = 1;
                    factorList.clear();
                } else {
                    // иначе завершаем генерацию
                    break;
                }
            }
        }

        // генерируем случайное R
        int R = getRandomInt(min - 1, max - 1);
        // делаем R четным
        if (R % 2 != 0) R++;
        // вычисляем кандидата n = R*f + 1
        int candidate = R * f + 1;
        // возвращаем кандидата и множители
        return make_tuple(candidate, factorList);
    }

    // метод для генерации простых чисел алгоритмом поклингтона
    // принимает битовую длину, количество тестов и количество чисел для генерации
    static void generatePrimes(int bits, int tests, int count) {
        // получаем список простых чисел до 500
        vector<int> primes = sieveOfEratosthenes(500);
        // вектор для хранения найденных простых чисел
        vector<int> candidates;
        // вектор для хранения результатов тестов
        vector<string> testResults;
        // вектор для хранения количества попыток
        vector<int> attempts;
        // счетчик текущих попыток
        int currentAttempts = 0;

        // генерируем указанное количество простых чисел
        while (candidates.size() < static_cast<size_t>(count)) {
            // генерируем кандидата и его множители
            int n;
            vector<int> factors;
            tie(n, factors) = generateCandidate(primes, bits);

            // проверяем кандидата на простоту
            if (isPrime(n, tests, factors)) {
                // если число прошло тест, добавляем в результат
                candidates.push_back(n);
                // дополнительная проверка тестом миллера-рабина
                testResults.push_back(millerRabinTest(n, 3) ? "+" : "-");
                // сохраняем количество попыток
                attempts.push_back(currentAttempts);
                // сбрасываем счетчик попыток
                currentAttempts = 0;
            } else {
                // если число не прошло тест
                if (millerRabinTest(n, 1)) {
                    // увеличиваем счетчик попыток
                    currentAttempts++;
                }
            }
        }

        // выводим результаты
        printResults(candidates, testResults, attempts);
    }

private:
    // вспомогательный метод для теста миллера-рабина
    // принимает число для проверки и количество тестов
    // возвращает true, если число вероятно простое
    static bool millerRabinTest(int num, int tests) {
        // обработка тривиальных случаев
        if (num == 2 || num == 3) return true;
        if (num < 2 || num % 2 == 0) return false;

        // раскладываем num-1 на d * 2^s
        int d = num - 1;
        int s = 0;
        while (d % 2 == 0) {
            d /= 2;
            s++;
        }

        // проводим указанное количество тестов
        for (int i = 0; i < tests; i++) {
            // выбираем случайное основание
            int a = getRandomInt(2, num - 2);
            // вычисляем x = a^d mod num
            int x = modPow(a, d, num);
            int y;
            
            // проводим s итераций
            for (int j = 0; j < s; j++) {
                // вычисляем y = x^2 mod num
                y = modPow(x, 2, num);
                // проверяем условия
                if (y == 1 && x != 1 && x != (num - 1)) {
                    // число составное
                    return false;
                }
                x = y;
            }
            // проверяем окончательное условие
            if (y != 1) {
                // число составное
                return false;
            }
        }
        // если все тесты пройдены, число вероятно простое
        return true;
    }

    // метод для вывода результатов
    // принимает векторы найденных чисел, результатов тестов и количества попыток
    static void printResults(const vector<int>& primes, 
                           const vector<string>& tests, 
                           const vector<int>& attempts) {
        // выводим заголовок
        cout << "\nрезультаты генерации алгоритмом поклингтона:\n";
        cout << "--------------------------------------------------\n";
        cout << "номер\tпростое число\tтест\tпопыток\n";
        cout << "--------------------------------------------------\n";

        // выводим все найденные простые числа
        for (size_t i = 0; i < primes.size(); i++) {
            cout << i+1 << "\t" << primes[i] << "\t" << tests[i] << "\t" << attempts[i] << "\n";
        }

        // выводим разделитель
        cout << "--------------------------------------------------\n";
    }
};

// класс для реализации алгоритма миллера
class MillerPrimality {
public:
    // метод для проверки числа на простоту по тесту миллера
    // принимает кандидата, количество тестов и вектор множителей
    // возвращает true, если число вероятно простое
    static bool isPrime(int candidate, int testCount, const vector<int>& factors) {
        // множество для хранения уникальных баз
        set<int> bases;

        // генерируем необходимое количество уникальных баз
        while (bases.size() < static_cast<size_t>(testCount)) {
            // получаем случайное основание
            int base = getRandomInt(2, candidate - 2);
            // добавляем в множество
            bases.insert(base);
        }

        // проверка первого условия теста миллера
        for (int base : bases) {
            // если a^(n-1) не сравнимо с 1 по модулю n
            if (modPow(base, candidate - 1, candidate) != 1) {
                // число составное
                return false;
            }
        }

        // проверка второго условия теста миллера
        for (int factor : factors) {
            // флаг, что все основания дали 1
            bool allOnes = true;
            // проверяем для каждого основания
            for (int base : bases) {
                // вычисляем показатель степени
                int exponent = (candidate - 1) / factor;
                // если a^((n-1)/q) не сравнимо с 1 по модулю n
                if (modPow(base, exponent, candidate) != 1) {
                    // не все основания дали 1
                    allOnes = false;
                    break;
                }
            }
            // если для какого-то множителя все основания дали 1
            if (allOnes) {
                // число составное
                return false;
            }
        }

        // если все условия выполнены, число вероятно простое
        return true;
    }

    // генерация кандидата на простое число и его множителей
    // принимает вектор простых чисел и битовую длину
    // возвращает пару из кандидата и вектора множителей
    static pair<int, vector<int>> generateCandidate(const vector<int>& primes, int bitLength) {
        // нижняя граница для m
        const int lowerBound = pow(2, bitLength - 2);
        // верхняя граница для m
        const int upperBound = pow(2, bitLength - 1) - 1;
        
        // инициализация m
        int m = 1;
        // вектор для хранения множителей
        vector<int> factors;

        // генерация множителей m
        while (true) {
            // выбираем случайный индекс простого числа
            int randomIndex = getRandomInt(0, primes.size() - 1);
            // получаем простое число по индексу
            int randomPrime = primes[randomIndex];
            
            // проверяем, не превысили ли верхнюю границу
            if (m * randomPrime > upperBound) {
                // если достигли нижней границы, завершаем
                if (m >= lowerBound) break;
                // иначе начинаем заново
                m = 1;
                factors.clear();
                continue;
            }
            
            // умножаем m на простое число
            m *= randomPrime;
            // добавляем простое число в множители
            factors.push_back(randomPrime);
        }

        // возвращаем кандидата n = 2*m + 1 и множители
        return {2 * m + 1, factors};
    }

    // метод для генерации простых чисел алгоритмом миллера
    // принимает битовую длину, количество тестов и количество чисел для генерации
    static void generatePrimes(int bitLength, int testCount, int targetCount) {
        // получаем список простых чисел до 500
        auto smallPrimes = sieveOfEratosthenes(500);
        // вектор для хранения найденных простых чисел
        vector<int> primesFound;
        // вектор для хранения результатов тестов
        vector<string> testResults;
        // вектор для хранения количества попыток
        vector<int> attemptsCount;
        // счетчик текущих попыток
        int attempts = 0;

        // генерируем указанное количество простых чисел
        while (primesFound.size() < static_cast<size_t>(targetCount)) {
            // генерируем кандидата и его множители
            auto [candidate, factors] = generateCandidate(smallPrimes, bitLength);
            
            // проверяем кандидата на простоту
            if (isPrime(candidate, testCount, factors)) {
                // если число прошло тест, добавляем в результат
                primesFound.push_back(candidate);
                // дополнительная проверка тестом миллера-рабина
                testResults.push_back(millerRabinTest(candidate, 3) ? "+" : "-");
                // сохраняем количество попыток
                attemptsCount.push_back(attempts);
                // сбрасываем счетчик попыток
                attempts = 0;
            } else {
                // если число не прошло тест
                if (millerRabinTest(candidate, 1)) {
                    // увеличиваем счетчик попыток
                    ++attempts;
                }
            }
        }

        // выводим результаты
        printResults(primesFound, testResults, attemptsCount);
    }

private:
    // вспомогательный метод для теста миллера-рабина
    // принимает число для проверки и количество тестов
    // возвращает true, если число вероятно простое
    static bool millerRabinTest(int num, int count) {
        // обработка тривиальных случаев
        if (num == 2 || num == 3) return true;
        if (num % 2 == 0 || num < 2) return false;

        // раскладываем num-1 на d * 2^s
        int d = num - 1;
        int s = 0;
        while (d % 2 == 0) {
            d /= 2;
            ++s;
        }

        // проводим указанное количество тестов
        for (int i = 0; i < count; ++i) {
            // выбираем случайное основание
            int a = getRandomInt(2, num - 2);
            // вычисляем x = a^d mod num
            int x = modPow(a, d, num);
            
            // если x равно 1 или num-1, переходим к следующему тесту
            if (x == 1 || x == num - 1) continue;
            
            // флаг, что число составное
            bool composite = true;
            // проводим s-1 итераций
            for (int j = 0; j < s - 1; ++j) {
                // вычисляем x = x^2 mod num
                x = modPow(x, 2, num);
                // если x равно num-1, число вероятно простое
                if (x == num - 1) {
                    composite = false;
                    break;
                }
            }
            
            // если составное, возвращаем false
            if (composite) return false;
        }
        
        // если все тесты пройдены, число вероятно простое
        return true;
    }

    // метод для вывода результатов
    // принимает векторы найденных чисел, результатов тестов и количества попыток
    static void printResults(const vector<int>& primes, 
                           const vector<string>& tests, 
                           const vector<int>& attempts) {
        // выводим заголовок
        cout << "\nрезультаты генерации алгоритмом миллера:\n";
        cout << "--------------------------------------------------\n";
        cout << "номер\tпростое число\tтест\tпопыток\n";
        cout << "--------------------------------------------------\n";

        // выводим все найденные простые числа
        for (size_t i = 0; i < primes.size(); i++) {
            cout << i+1 << "\t" << primes[i] << "\t" << tests[i] << "\t" << attempts[i] << "\n";
        }

        // выводим разделитель
        cout << "--------------------------------------------------\n";
    }
};

// класс для реализации алгоритма гост р 34.10-94
class GOSTPrimality {
public:
    // метод для генерации простого числа по гост
    // принимает вектор простых чисел и битовую длину
    // возвращает простое число
    static int generatePrime(const vector<int>& primes, int bitLen) {
        // вычисляем длину q в битах
        int qBitLen = (bitLen + 1) / 2;
        // минимальное значение для q
        int qMin = 0;
        // максимальное значение для q
        int qMax = (1 << qBitLen) - 1;

        // переменная для хранения q
        int q;
        // выбираем простое число q подходящего размера
        do {
            q = primes[getRandomInt(0, primes.size() - 1)];
        } while (q < qMin || q > qMax);

        // основной цикл генерации простого числа p
        while (true) {
            // генерируем случайное число ξ в диапазоне [0, 1)
            double ξ = getRandomDouble();
            // вычисляем n по формуле
            double n = (double(1 << (bitLen - 1)) + (ξ * double(1 << (bitLen - 1)))) / q;
            // преобразуем n в целое число
            int nInt = int(n);
            // если nInt нечетное, делаем его четным
            if (nInt % 2 != 0) nInt++;

            // поиск подходящего p = (nInt + k)*q + 1
            for (int k = 0; ; k += 2) {
                // вычисляем кандидата p
                int p = (nInt + k) * q + 1;
                // если p превысило максимальное значение, выходим из цикла
                if (p > (1 << bitLen)) break;

                // проверка условий простоты
                if (modPow(2, p - 1, p) == 1 && modPow(2, nInt + k, p) != 1) {
                    // если условия выполнены, возвращаем p
                    return p;
                }
            }
        }
    }

    // метод для генерации простых чисел по гост
    // принимает битовую длину, количество тестов и количество чисел для генерации
    static void generatePrimes(int bitLength, int testRounds, int targetCount) {
        // множество для хранения уникальных простых чисел
        set<int> primesSet;
        // получаем список простых чисел до 500
        vector<int> primes = sieveOfEratosthenes(500);
        // вектор для хранения результатов тестов
        vector<string> testResults;
        // счетчик отвергнутых чисел
        int rejected = 0;

        // выводим сообщение о начале генерации
        cout << "начало генерации простых чисел по гост...\n";
        cout << "это может занять некоторое время...\n";

        // генерируем указанное количество уникальных простых чисел
        while (primesSet.size() < static_cast<size_t>(targetCount)) {
            // генерируем кандидата p
            int p = generatePrime(primes, bitLength);
            // если число уже есть в множестве, пропускаем его
            if (primesSet.count(p)) continue;

            // проверяем кандидата тестом миллера-рабина
            if (millerRabinTest(p, testRounds)) {
                // если тест пройден, добавляем число в множество
                primesSet.insert(p);
                testResults.push_back("пройден");
                // выводим прогресс
                cout << "найдено простых чисел: " << primesSet.size() << " из " << targetCount << "\n";
            } else {
                // увеличиваем счетчик отвергнутых чисел
                rejected++;
            }
        }

        // выводим результаты
        printResults(primesSet, testResults, rejected);
    }

private:
    // вспомогательный метод для теста миллера-рабина
    // принимает число для проверки и количество тестов
    // возвращает true, если число вероятно простое
    static bool millerRabinTest(int num, int tests) {
        // обработка тривиальных случаев
        if (num == 2 || num == 3) return true;
        if (num < 2 || num % 2 == 0) return false;

        // раскладываем num-1 на d * 2^s
        int d = num - 1;
        int s = 0;
        while (d % 2 == 0) {
            d /= 2;
            s++;
        }

        // проводим указанное количество тестов
        for (int i = 0; i < tests; i++) {
            // выбираем случайное основание
            int a = getRandomInt(2, num - 2);
            // вычисляем x = a^d mod num
            int x = modPow(a, d, num);
            int y;
            
            // проводим s итераций
            for (int j = 0; j < s; j++) {
                // вычисляем y = x^2 mod num
                y = modPow(x, 2, num);
                // проверяем условия
                if (y == 1 && x != 1 && x != (num - 1)) {
                    // число составное
                    return false;
                }
                x = y;
            }
            // проверяем окончательное условие
            if (y != 1) {
                // число составное
                return false;
            }
        }
        // если все тесты пройдены, число вероятно простое
        return true;
    }

    // метод для вывода результатов
    // принимает множество простых чисел, результаты тестов и количество отвергнутых чисел
    static void printResults(const set<int>& primes, 
                           const vector<string>& tests, 
                           int rejected) {
        // выводим заголовок
        cout << "\nрезультаты генерации алгоритмом гост р 34.10-94:\n";
        cout << "--------------------------------------------------\n";
        cout << "номер\tпростое число\tтест\n";
        cout << "--------------------------------------------------\n";

        // выводим все найденные простые числа
        int i = 1;
        for (int p : primes) {
            cout << i++ << "\t" << p << "\t" << tests[i-2] << "\n";
        }

        // выводим статистику
        cout << "--------------------------------------------------\n";
        cout << "всего отвергнуто чисел: " << rejected << "\n";
        cout << "--------------------------------------------------\n";
    }
};

// главная функция программы
int main() {
    // выводим приветственное сообщение
    cout << "генерация простых чисел различными алгоритмами\n";
    cout << "--------------------------------------------------\n\n";

    // переменная для хранения выбора пользователя
    int choice;
    // основной цикл меню
    do {
        // выводим варианты выбора
        cout << "выберите алгоритм:\n";
        cout << "1. алгоритм поклингтона\n";
        cout << "2. алгоритм миллера\n";
        cout << "3. алгоритм гост р 34.10-94\n";
        cout << "0. выход\n";
        cout << "ваш выбор: ";
        // считываем выбор пользователя
        cin >> choice;

        // обработка выбора пользователя
        switch (choice) {
            case 1: {
                // генерация чисел алгоритмом поклингтона
                cout << "\nгенерация простых чисел алгоритмом поклингтона\n";
                PocklingtonPrimality::generatePrimes(13, 10, 10);
                break;
            }
            case 2: {
                // генерация чисел алгоритмом миллера
                cout << "\nгенерация простых чисел алгоритмом миллера\n";
                MillerPrimality::generatePrimes(10, 5, 10);
                break;
            }
            case 3: {
                // генерация чисел алгоритмом гост
                cout << "\nгенерация простых чисел алгоритмом гост р 34.10-94\n";
                // уменьшаем параметры для ускорения работы
                GOSTPrimality::generatePrimes(10, 3, 5);
                break;
            }
            case 0: {
                // выход из программы
                cout << "выход из программы.\n";
                break;
            }
            default: {
                // неверный выбор
                cout << "неверный выбор. попробуйте снова.\n";
            }
        }
        cout << endl;
    } while (choice != 0);

    return 0;
}