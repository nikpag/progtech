#include <iostream>
using namespace std;

double pow(double x, int n) {
  double result = 1.0;
  for (unsigned i = 0; i < n; i++) result *= x;
  return result;
}

class Polynomial {
protected:
  class Term {
  protected:
    int exponent;
    int coefficient;
    Term *next;
    Term(int exp, int coeff, Term *n): exponent(exp), coefficient(coeff), next(n) {}
    friend class Polynomial;
  };
  Term *head;

public:
  Polynomial(): head(nullptr) {}
  Polynomial(const Polynomial &p): head(nullptr) {
    for (const Term *ptr = p.head; ptr != nullptr; ptr = ptr->next)
      addTerm(ptr->exponent, ptr->coefficient);
  }
  ~Polynomial() {
    Purge();
  }

  static int Exponent(Term *t) { return t->exponent; }
  static int Coefficient(Term *t) { return t->coefficient; }
  static void Next(Term * &t) { t = t->next; }

  void Purge() {
    while (head != nullptr) {
      Term * const tmp = head;
      head = head->next;
      delete tmp;
    }
  }

  Polynomial & operator = (const Polynomial &p) {
    if (&p != this) {
      Purge();
      for (const Term *ptr = p.head; ptr != nullptr; ptr = ptr->next)
        addTerm(ptr->exponent, ptr->coefficient);
    }
    return *this;
  }

  void addTerm(int expon, int coeff) {
    if (head == nullptr) {
      Term * tmp = new Term(expon, coeff, nullptr);
      head = tmp;
      }
    else if (expon > head->exponent) {
      Term * tmp = new Term(expon, coeff, head);
      head = tmp;
    }
    else if (expon < head->exponent) {
      Term *p = head;
      while (true) {
        if (p->next == nullptr or expon > (p->next)->exponent) {
          Term *q = new Term(expon, coeff, p->next);
          p->next = q;
          break;
        }
        if ((p->next)->exponent == expon) {
          (p->next)->coefficient += coeff;
          break;
        }
        p = p->next;
      }
    }
    else if (expon == head->exponent) head->coefficient += coeff;
  }

  double evaluate(double x) {
    double result = 0.0;
    for (Term *ptr = head; ptr != nullptr; ptr = ptr->next)
      result += ptr->coefficient * pow(x, ptr->exponent);
    return result;
  }

  friend Polynomial operator + (const Polynomial &p, const Polynomial &q) {
    Polynomial sum;
    Term *pp = p.head, *qq = q.head;
    while (pp != nullptr or qq != nullptr) {
      if (pp != nullptr and qq == nullptr) {
        sum.addTerm(Exponent(pp), Coefficient(pp));
        Next(pp);
      }
      else if (qq != nullptr and pp == nullptr) {
        sum.addTerm(Exponent(qq), Coefficient(qq));
        Next(qq);
      }
      else if (Exponent(pp) > Exponent(qq)) {
        sum.addTerm(Exponent(pp), Coefficient(pp));
        Next(pp);
      }
      else if (Exponent(qq) > Exponent(pp)) {
        sum.addTerm(Exponent(qq), Coefficient(qq));
        Next(qq);
      }
      else {
        sum.addTerm(Exponent(pp), Coefficient(pp) + Coefficient(qq));
        Next(pp); Next(qq);
      }
    }
    return sum;
  }
  friend Polynomial operator * (const Polynomial &p, const Polynomial &q) {
    Polynomial product;
    for (Term *pp = p.head; pp != nullptr; Next(pp))
      for (Term *qq = q.head; qq != nullptr; Next(qq))
        product.addTerm(Exponent(pp) + Exponent(qq), Coefficient(pp) * Coefficient(qq));
    return product;
  }

  friend ostream & operator << (ostream &out, const Polynomial &p) {
    Term *q = p.head;
    while (Coefficient(q) == 0) {
      Next(q);
      if (q == nullptr) {
        out << "0";
        return out;
      }
    }
    if (Coefficient(q) < 0) out << "- "; 
    if (abs(Coefficient(q)) == 1 and Exponent(q) >= 2) out << "x^" << Exponent(q);
    else if(abs(Coefficient(q)) == 1 and Exponent(q) == 1) out << "x";
    else if (Exponent(q) == 1 and Coefficient(q) != 0) out << abs(Coefficient(q)) << "x";
    else if (Exponent(q) == 0 and Coefficient(q) != 0) out << abs(Coefficient(q));
    else if (Coefficient(q) != 0) out << abs(Coefficient(q)) << "x^" << Exponent(q);
    for (Next(q); q != nullptr; Next(q)) {
      while (Coefficient(q) == 0) {
        Next(q);
        if (q == nullptr) return out;
      }
      if (Coefficient(q) < 0) out << " -"; else if (Coefficient(q) != 0) out << " +";
      if (abs(Coefficient(q)) == 1 and Exponent(q) >= 2) out << " x^" << Exponent(q);
      else if (abs(Coefficient(q)) == 1 and Exponent(q) == 1) out << " x";
      else if (Exponent(q) == 1 and Coefficient(q) != 0) out << ' ' << abs(Coefficient(q)) << "x";
      else if (Exponent(q) == 0 and Coefficient(q) != 0) out << ' ' << abs(Coefficient(q));
      else if (Coefficient(q) != 0) out << ' ' << abs(Coefficient(q)) << "x^" << Exponent(q);
    }
    return out;
  }
};
