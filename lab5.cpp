#include <iostream>
#include <cmath>
#include <limits>

using namespace std;

// ----------------------------------------------------------
// Допоміжні функції для безпечного введення (коментарі укр)
// ----------------------------------------------------------

int inputInt(const string& prompt) {
    int x;
    while (true) {
        cout << prompt;
        if (cin >> x) return x;

        cout << "Input error! Try again.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

double inputDouble(const string& prompt) {
    double x;
    while (true) {
        cout << prompt;
        if (cin >> x) return x;

        cout << "Input error! Try again.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

// Перевірки коректності
bool isValidN(int n)    { return n > 0; }
bool isValidR(double r) { return r > 0; }
bool isValidE(double e) { return e > 0; }
bool isValidG(double g) { return g > 0; }

// ----------------------------------------------------------
// TASK 1. Variant 36 – brown regions
// ----------------------------------------------------------
// Інтерпретація по малюнку:
// маємо коло x^2 + y^2 <= r^2 з центром у (0,0).
// Коричнева область = об’єднання двох частин:
//
// 1) Верхня частина в I квадранті – перетин кола з ромбом
//    |x| + |y| <= r, тобто x >= 0, y >= 0, x + y <= r.
// 2) Нижня частина в IV квадранті – сегмент кола справа від осі Oy:
//    x >= 0, y <= 0, x^2 + y^2 <= r^2.
//
bool inArea36(double x, double y, double r) {
    bool inCircle = (x * x + y * y <= r * r);

    // верхня коричнева частина (I квадрант + ромб)
    bool upper = (x >= 0 && y >= 0 && x + y <= r && inCircle);

    // нижня коричнева частина (IV квадрант, просто сегмент кола)
    bool lower = (x >= 0 && y <= 0 && inCircle);

    return upper || lower;
}

void task1() {
    cout << "\n=== TASK 1 (figure, variant 36) ===\n";

    int n = inputInt("Enter number of points n: ");
    while (!isValidN(n)) {
        cout << "n must be > 0\n";
        n = inputInt("Enter number of points n: ");
    }

    double r = inputDouble("Enter radius r > 0: ");
    while (!isValidR(r)) {
        cout << "r must be > 0\n";
        r = inputDouble("Enter radius r: ");
    }

    int count = 0;

    for (int i = 1; i <= n; ++i) {
        cout << "\nPoint " << i << ":\n";
        double x = inputDouble("  x = ");
        double y = inputDouble("  y = ");

        if (inArea36(x, y, r)) {
            ++count;
        }
    }

    cout << "\nNumber of points inside the brown area (variant 36): "
         << count << "\n";
}

// ----------------------------------------------------------
// TASK 2. Variant 13 (Table 2)
// S = sum_{k=1}^n (x^k + 1) / (k-1)!
// For odd variants: output every 4-th term.
// ----------------------------------------------------------

void task2() {
    cout << "\n=== TASK 2 (series, variant 13) ===\n";

    double x = inputDouble("Enter x (real number): ");
    int n = inputInt("Enter n (natural number): ");
    while (!isValidN(n)) {
        cout << "n must be > 0\n";
        n = inputInt("Enter n: ");
    }

    double sum = 0.0;
    double xPow = 1.0; // x^0
    double fact = 1.0; // (k-1)! , for k=1 -> 0! = 1

    cout << "\nEvery 4-th term of the series:\n";

    for (int k = 1; k <= n; ++k) {
        // обчислення x^k
        xPow *= x;

        // обчислення (k-1)!
        if (k > 1) fact *= (k - 1);

        double term = (xPow + 1.0) / fact;
        sum += term;

        if (k % 4 == 0) {
            cout << "k = " << k << "\tterm = " << term << "\n";
        }
    }

    cout << "\nSeries sum S = " << sum << "\n";
}

// ----------------------------------------------------------
// TASK 3. Variant 38 (Table 3)
// Series: sum_{n=1}^∞ (x^{n-1} + 3n) / (2n-1)!
//
// З методички: цикл припиняємо, якщо
// |u_n| < e  (ряд вважаємо збіжним)
// або |u_n| > g (ряд вважаємо розбіжним).
// ----------------------------------------------------------

void task3() {
    cout << "\n=== TASK 3 (convergence, variant 38) ===\n";

    double x = inputDouble("Enter x: ");
    double e = inputDouble("Enter e (small value, e.g. 1e-4): ");
    while (!isValidE(e)) {
        cout << "e must be > 0\n";
        e = inputDouble("Enter e: ");
    }

    double g = inputDouble("Enter g (big value, e.g. 1e2): ");
    while (!isValidG(g)) {
        cout << "g must be > 0\n";
        g = inputDouble("Enter g: ");
    }

    double sum = 0.0;
    int n = 1;

    double xPow = 1.0; // x^{n-1}, for n=1 -> x^0 = 1
    double fact = 1.0; // (2n-1)!, for n=1 -> 1! = 1

    const int MAX_ITER = 1000;
    bool divergent = false;

    cout << "\nSeries terms:\n";

    while (true) {
        double term = (xPow + 3.0 * n) / fact;
        double absTerm = fabs(term);

        cout << "n = " << n << "\tterm = " << term << "\n";
        sum += term;

        if (absTerm < e) {
            cout << "\n|u_n| < e -> stop (series behaves as convergent).\n";
            break;
        }

        if (absTerm > g) {
            cout << "\n|u_n| > g -> stop (series is considered divergent).\n";
            divergent = true;
            break;
        }

        if (n >= MAX_ITER) {
            cout << "\nReached MAX_ITER, stopping for safety.\n";
            break;
        }

        // підготовка наступного члена
        ++n;
        xPow *= x;  // x^{n-1} for new n

        // оновлення факторіала:
        // (2n-1)! = (2n-1)*(2n-2)*(2n-3)!
        int a = 2 * n - 2;
        int b = 2 * n - 1;
        fact *= a;
        fact *= b;
    }

    cout << "\nApproximate series sum S = " << sum << "\n";
    if (divergent)
        cout << "Conclusion: according to g, the series is divergent.\n";
    else
        cout << "Conclusion: according to e, the series shows convergence.\n";
}

// ----------------------------------------------------------
// MENU (Task 4)
// ----------------------------------------------------------

void printMenu() {
    cout << "\n============================\n";
    cout << "MENU:\n";
    cout << "1 - Task 1 (figure, variant 36)\n";
    cout << "2 - Task 2 (series sum, variant 13)\n";
    cout << "3 - Task 3 (series convergence, variant 38)\n";
    cout << "-1 - Exit\n";
    cout << "Your choice: ";
}

int main() {
    int menu;

    do {
        printMenu();

        if (!(cin >> menu)) {
            cout << "Input error! Program will terminate.\n";
            break;
        }

        switch (menu) {
            case 1:
                task1();
                break;
            case 2:
                task2();
                break;
            case 3:
                task3();
                break;
            case -1:
                cout << "Exiting...\n";
                break;
            default:
                cout << "Wrong task number!\n";
        }
    } while (menu != -1);

    return 0;
}
