use std::f64; 
use std::io; 

// функция для вычисления значения y в зависимости от x
fn calculate_y(x: f64) -> f64 {
    if x >= -5.0 && x <= -2.0 {
        // y = (1/3)*(x + 5) - 1
        (1.0 / 3.0) * (x + 5.0) - 1.0
    } else if x > -2.0 && x < 0.0 {
        // y = 0.5*(x + 2) - 1
        0.5 * (x + 2.0) - 1.0
    } else if x >= 0.0 && x < 2.0 {
        // y = tan(x/2)
        (x / 2.0).tan()
    } else if x >= 2.0 && x < 3.0 {
        // y = -tan(1)*(x - 2) + tan(1)
        -f64::tan(1.0) * (x - 2.0) + f64::tan(1.0)
    } else if x >= 3.0 && x <= 5.0 {
        // y = 0.5*(x - 3)
        0.5 * (x - 3.0)
    } else {
        // вне диапазона
        f64::NAN
    }
}

fn main() {
    let mut input = String::new();

    println!("введите начальное значение x (от -5 до 5):");
    io::stdin().read_line(&mut input).expect("не удалось прочитать строку");
    let xstart: f64 = input.trim().parse().unwrap_or(0.0);
    input.clear();

    println!("введите конечное значение x (от -5 до 5):");
    io::stdin().read_line(&mut input).expect("не удалось прочитать строку");
    let xend: f64 = input.trim().parse().unwrap_or(0.0);
    input.clear();

    println!("введите шаг dx (положительное число):");
    io::stdin().read_line(&mut input).expect("не удалось прочитать строку");
    let dx: f64 = input.trim().parse().unwrap_or(0.0);

    if xstart < -5.0 || xend > 5.0 || xstart > xend || dx <= 0.0 {
        println!("ошибка: некорректные входные данные.");
        println!("убедитесь, что:");
        println!("- начальное x >= -5");
        println!("- конечное x <= 5");
        println!("- начальное x <= конечное x");
        println!("- шаг dx > 0");
        return;
    }

    println!("-----------------------------");
    println!("|     x     |      y      |");
    println!("-----------------------------");

    let mut x = xstart;
    while x <= xend + 1e-9 {
        let y = calculate_y(x);
        if y.is_nan() {
            println!("| {:>9.3} |   не опр.  |", x);
        } else if y.abs() > 10.0 {
            println!("| {:>9.3} |  очень бол. |", x);
        } else {
            println!("| {:>9.3} | {:>10.3} |", x, y);
        }
        x += dx;
    }

    println!("-----------------------------");
}
