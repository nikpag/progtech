#include <iostream>

using namespace std;

class rational {
public:
  rational(int n, int d) {
    nom = n;
    den = d;
    nom /= gcd(n, d);
    den /= gcd(n, d);
  }

  rational add(rational r) {
    return rational(nom * r.den + den * r.nom, den * r.den);
  }

  rational sub(rational r) {
    return rational(nom * r.den - den * r.nom, den * r.den);
  }

  rational mul(rational r) {
    return rational(nom * r.nom, den * r.den);
  }

  rational div(rational r) {
    return rational(nom * r.den, den * r.nom);
  }

  void print() {
    cout << nom << "/" << den;
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
  a.add(b).sub(c).print();
  cout << endl;
  a.print();
  cout << " should still be 1/2" << endl;
}
