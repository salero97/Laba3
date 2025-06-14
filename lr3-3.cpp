#include <iostream>
#include <vector>
#include <numeric>

using namespace std; 

// функция для нахождения наибольшего общего делителя двух чисел
int findGCD(int firstNum, int secondNum) {
    while (secondNum != 0) { // пока второе число не ноль
        int temp = secondNum; // сохраняем второе число
        secondNum = firstNum % secondNum; // находим остаток от деления
        firstNum = temp; // меняем первое число на бывшее второе
    }
    return firstNum; // возвращаем НОД
}

// функция для вычисления суммы ряда
pair<int, int> computeSeriesSum(int power, int base) {
    // если степень равна 0, это базовый случай
    if (power == 0) {
        return {1, base - 1}; // возвращаем 1/(b-1)
    }

    // вычисляем сумму предыдущих членов
    int totalNumerator = 0; // общий числитель
    int totalDenominator = 1; // общий знаменатель

    // суммируем все члены от 0 до power-1
    for (int currentPower = 0; currentPower < power; currentPower++) {
        auto [currentNum, currentDen] = computeSeriesSum(currentPower, base); // получаем текущий член
        // (a/b + c/d = (ad + cb)/(b*d))
        // приводим к общему знаменателю
        int newNum = totalNumerator * currentDen + currentNum * totalDenominator;
        int newDen = totalDenominator * currentDen;

        // сокращаем дробь
        int gcdValue = findGCD(newNum, newDen);
        totalNumerator = newNum / gcdValue;
        totalDenominator = newDen / gcdValue;
    }

    // применяем итоговую формулу
    int finalNum = base * totalNumerator;
    int finalDen = (base - 1) * totalDenominator;

    // сокращаем итоговую дробь
    int finalGCD = findGCD(finalNum, finalDen);
    return {finalNum / finalGCD, finalDen / finalGCD};
}

int main() {
    cout << "введите два числа (от 1 до 10): "; // просим ввести числа
    int inputA, inputB;
    cin >> inputA >> inputB; // считываем ввод

    // проверяем диапазон чисел
    if (inputA < 1 || inputA > 10 || inputB < 1 || inputB > 10) {
        cout << "ошибка: числа должны быть от 1 до 10\n"; // сообщение об ошибке
        return 1; // выходим с ошибкой
    }

    // проверяем случай расходящегося ряда
    if (inputB == 1) {
        cout << "infinity\n"; // ряд расходится
        return 0; // нормальное завершение
    }

    // вычисляем сумму ряда
    auto [resultNum, resultDen] = computeSeriesSum(inputA, inputB); // получаем результат

    // выводим результат в виде дроби
    cout << resultNum << "/" << resultDen << "\n";

    return 0; 
}