#include <iostream>

unsigned long long fibonacci(int n) {
    if (n <= 1) {
        return n;
    }

    unsigned long long fib0 = 0, fib1 = 1, fibN = 0;
    for (int i = 2; i <= n; i++) {
        fibN = fib0 + fib1;
        fib0 = fib1;
        fib1 = fibN;
    }

    return fibN;
}

int main() {
    int n = 0;
    std::cout << "Nhap vi tri so Fibonacci muon tinh: ";
    std::cin >> n;
    std::cout << "So Fibonacci thu " << n << "la: " << fibonacci(n) << std::endl;
    return 0;
}
