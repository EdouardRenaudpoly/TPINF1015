#include <iostream>
#include <cmath>
using namespace std;
    class Complex {

    public:
        Complex(float x, float y);

        float realPart() const;
        float imagPart() const;
        float modulus() const;

    private:
        float real;
        float imag;
    };
    // Constructeur
    Complex::Complex(float x, float y) : real(x), imag(y) {}

    // Méthodes d'accès
    float Complex::realPart() const { return real; }
    float Complex::imagPart() const { return imag; }

    // Méthode pour calculer le module
    float Complex::modulus() const {
        return std::sqrt(realPart() * realPart() + imagPart() * imagPart());
    }

    // Implémentation de l'opérateur +
    Complex operator+(const Complex& c1, const Complex& c2) {
        return Complex(c1.realPart() + c2.realPart(), c1.imagPart() + c2.imagPart());
    }

    // Implémentation de l'opérateur *
    Complex operator*(const Complex& c1, const Complex& c2) {
        return Complex(c1.realPart() * c2.realPart() - c1.imagPart() * c2.imagPart(),
            c1.realPart() * c2.imagPart() + c1.imagPart() * c2.realPart());
    }
    int mandelbrotSet(const Complex& z, const Complex& c, int count) {
        if (z.modulus() >= 2.0f) return count;          // cas de base 1
        if (count + 1 > 100) return count;          // cas de base 2
        return mandelbrotSet(z * z + c, c, ++count); // cas recursif
    }

int main()
{
    Complex c = Complex(-0.654915, -0.494148);
    int resultat=mandelbrotSet(c, c, 0);
    cout << resultat;
}