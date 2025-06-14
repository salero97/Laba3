#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>
#include <fstream>

using namespace std;

// функция для моделирования остывания кофе
vector<pair<double, double>> cofe(double tcof, double tmed, double k, double time, double step) {
    vector<pair<double, double>> result;
    double currentTime = 0.0;
    double currentTemp = tcof;
    result.push_back({currentTime, currentTemp});
    
    while (currentTime < time) {
        currentTime += step;
        currentTemp = tmed + (currentTemp - tmed) * exp(-k * step);
        result.push_back({currentTime, currentTemp});
    }
    
    return result;
}

// функция для вычисления коэффициента корреляции и детерминации
void korrel(const vector<pair<double, double>>& data, double& r, double& r2) {
    double sumX = 0, sumY = 0, sumXY = 0, sumX2 = 0, sumY2 = 0;
    int n = data.size();
    
    for (const auto& point : data) {
        sumX += point.first;
        sumY += point.second;
        sumXY += point.first * point.second;
        sumX2 += point.first * point.first;
        sumY2 += point.second * point.second;
    }
    
    r = (n * sumXY - sumX * sumY) / sqrt((n * sumX2 - sumX * sumX) * (n * sumY2 - sumY * sumY));
    r2 = r * r;
}

// функция для вычисления аппроксимирующей прямой (y = a*x + b)
void aprox(const vector<pair<double, double>>& data, double& a, double& b) {
    double sumX = 0, sumY = 0, sumXY = 0, sumX2 = 0;
    int n = data.size();
    
    for (const auto& point : data) {
        sumX += point.first;
        sumY += point.second;
        sumXY += point.first * point.second;
        sumX2 += point.first * point.first;
    }
    
    a = (n * sumXY - sumX * sumY) / (n * sumX2 - sumX * sumX);
    b = (sumY - a * sumX) / n;
}

// функция для вывода таблицы
void printTable(const vector<pair<double, double>>& data) {
    cout << "---------------------------------" << endl;
    cout << "| время (мин) | температура (C) |" << endl;
    cout << "---------------------------------" << endl;
    
    for (const auto& row : data) {
        cout << "| " << setw(11) << fixed << setprecision(2) << row.first 
             << " | " << setw(15) << fixed << setprecision(1) << row.second << " |" << endl;
    }
    
    cout << "---------------------------------" << endl;
}

// функция для записи данных в файл
void writeToFile(const vector<pair<double, double>>& data, const string& filename) {
    ofstream outFile(filename);
    if (outFile.is_open()) {
        outFile << "время (мин),температура (C)" << endl;
        for (const auto& row : data) {
            outFile << fixed << setprecision(2) << row.first << "," 
                    << fixed << setprecision(1) << row.second << endl;
        }
        outFile.close();
        cout << "данные сохранены в файл: " << filename << endl;
    } else {
        cout << "ошибка при открытии файла" << endl;
    }
}

int main() {
    double tcof, tmed, k, time, step;
    
    cout << "моделирование остывания кофе" << endl;
    cout << "введите начальную температуру кофе (C): ";
    cin >> tcof;
    cout << "введите температуру окружающей среды (C): ";
    cin >> tmed;
    cout << "введите коэффициент остывания (>0): ";
    cin >> k;
    while (k <= 0) {
        cout << "коэффициент должен быть >0, введите снова: ";
        cin >> k;
    }
    cout << "введите время моделирования (мин): ";
    cin >> time;
    while (time <= 0) {
        cout << "время должно быть >0, введите снова: ";
        cin >> time;
    }
    cout << "введите шаг по времени (мин): ";
    cin >> step;
    while (step <= 0 || step > time) {
        cout << "шаг должен быть >0 и <общего времени, введите снова: ";
        cin >> step;
    }
    
    vector<pair<double, double>> result = cofe(tcof, tmed, k, time, step);
    printTable(result);
    writeToFile(result, "coffee_data.csv");
    
    double r, r2;
    korrel(result, r, r2);
    cout << "коэффициент корреляции: " << r << endl;
    cout << "коэффициент детерминации: " << r2 << endl;
    
    double a, b;
    aprox(result, a, b);
    cout << "аппроксимирующая прямая: y = " << a << " * x + " << b << endl;
    
    return 0;
}
