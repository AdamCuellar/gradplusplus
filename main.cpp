#include <iostream>
#include <value.h>

int main() {
    Value<float> a = -4.0f;
    Value<float> b = 2.0f;
    Value<float> x = 2.0f;
    auto c = a * x + b;
    c.backward();
    std::cout << "A value: " << a << " A grad: " << a.grad << std::endl;
    std::cout << "B value: " << b << " B grad: " << b.grad << std::endl;
    std::cout << "C value: " << c << " C grad: " << c.grad << std::endl;
    std::cout << "X value: " << x << " X grad: " << x.grad << std::endl;
    return 0;
}
