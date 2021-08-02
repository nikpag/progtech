#include <iostream>

class rational {
public:
  rational(int n, int d = 1) {
    nom = n;
    den = d;
    nom /= gcd(n, d);
    den /= gcd(n, d);
  }

  friend rational operator + (const rational &x, const rational &y) {
    return rational(x.nom * y.den + x.den * y.nom, x.den * y.den);
  }

  friend rational operator - (const rational &x, const rational &y) {
    return rational(x.nom * y.den - x.den * y.nom, x.den * y.den);
  }

  friend rational operator * (const rational &x, const rational &y) {
    return rational(x.nom * y.nom, x.den * y.den);
  }

  friend rational operator / (const rational &x, const rational &y) {
    return rational(x.nom * y.den, x.den * y.nom);
  }

  friend std::ostream & operator << (std::ostream &out, const rational &x) {
    out << x.nom << "/" << x.den;
    return out;
  }

private:
  int nom, den;

  static int gcd(int a, int b) {
    while (a > 0 && b > 0) {
      if (a > b) a %= b; else b %= a;
    }
    return a + b;
  }
};

int main() {
  rational a(1, 2);
  rational b(3, 4);
  rational c(5, 6);
  std::cout << a + b - c << std::endl;
  std::cout << a << " should still be 1/2" << std::endl;
}
