#include <iostream>

using namespace std;

template <typename T>
class stack {
public:
  stack(int size) {
    arr = new T[size];
    _size = 0;
    top = 0;
  }
  stack(const stack &s) {
    _size = s._size;
    top = s.top;
    arr = new T[_size];
    for (int i = 0; i < _size; i++) {
      arr[i] = s.arr[i];
    }
  }
  ~stack() {
    delete [] arr;
  }
  const stack & operator = (const stack &s) {
    _size = s._size;
    top = s.top;
    delete [] arr;
    arr = new T[_size];
    for (int i = 0; i < _size; i++) {
      arr[i] = s.arr[i];
    }

    return *this;
  }

  bool empty() {
    return top == 0;
  }

  void push (const T &x) {
    arr[top++] = x;
    _size++;
  }

  T pop() {
    _size--;
    return arr[--top];
  }

  int size() {
    return top;
  }

  friend ostream & operator << (ostream &out, const stack &s) {
    cout << "[";
    for (int i = 0; i < s._size; i++) {
      out << s.arr[i] << (i < s._size - 1 ? ", " : "");
    }
    cout << "]";

    return out;
  }

private:
  T *arr;
  int top;
  int _size;
};

int main() {
  stack<int> s(10);
  cout << "s is empty: " << s << endl;
  s.push(42);
  cout << "s has one element: " << s << endl;
  s.push(17);
  s.push(34);
  cout << "s has more elements: " << s << endl;
  cout << "How many? " << s.size() << endl;
  stack<int> t(5);
  t.push(7);
  cout << "t: " << t << endl;
  t = s;
  cout << "popping from s: " << s.pop() << endl;
  s.push(8);
  stack<int> a(s);
  t.push(99);
  a.push(77);
  cout << "s: " << s << endl;
  cout << "t: " << t << endl;
  cout << "a: " << a << endl;
  stack<double> c(4);
  c.push(3.14);
  c.push(1.414);
  cout << "c contains doubles " << c << endl;
  stack<char> k(4);
  k.push('$');
  cout << "k contains a character " << k << endl;
}
