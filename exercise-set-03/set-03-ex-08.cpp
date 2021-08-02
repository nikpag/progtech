#include <iostream>
#include <stdexcept>
#include <iomanip>
using namespace std;

class ChessBoardArray {
protected:
  int *data;
  unsigned length, base;
  unsigned loc(int i, int j) const throw(out_of_range) {
    int di = i - base, dj = j - base;
    if (di < 0 or di >= length or dj < 0 or dj >= length or (di + dj) % 2 != 0) throw out_of_range("invalid index");
    return (di * length + dj) / 2;
  }

  class Row {
  public:
    Row(ChessBoardArray &a, int i): chessboardarray(a), row(i) {}
    int & operator [] (int j) const {
      return chessboardarray.select(row, j);
    }
  protected:
    ChessBoardArray &chessboardarray;
    int row;
  };

  class ConstRow {
  public:
    ConstRow(const ChessBoardArray &a, int i): chessboardarray(a), row(i) {}
    int operator [] (int j) const {
      return chessboardarray.select(row, j);
    }
  private:
    const ChessBoardArray &chessboardarray;
    int row;
  };
public:
  ChessBoardArray(unsigned l = 0, unsigned b = 0): data(new int[(l * l + 1) / 2]), length(l), base(b) {   for (int i = 0; i < (l * l + 1) / 2; i++) data[i] = 0;
  }
  ChessBoardArray(const ChessBoardArray &a):
  data(new int[(a.length * a.length + 1) / 2]), length(a.length), base(a.base) {
    for (unsigned i = 0; i < (length * length + 1) / 2; i++) data[i] = a.data[i];
  }
  ~ChessBoardArray() {
    delete [] data;
  }

  ChessBoardArray & operator = (const ChessBoardArray &a) {
    delete [] data;
    data = new int[(a.length * a.length + 1) / 2];
    length = a.length;
    base = a.base;
    for (unsigned i = 0; i < (length * length + 1) / 2; i++) data[i] = a.data[i];
    return *this;
  }

  int & select(int i, int j) {
    return data[loc(i, j)];
  }

  int select(int i, int j) const {
    return data[loc(i, j)];
  }

  const Row operator [] (int i) {
    return Row(*this, i);
  }

  const ConstRow operator [] (int i) const {
    return ConstRow(*this, i);
  }

  friend ostream & operator << (ostream &out, const ChessBoardArray &a) {
    for (int i = 0; i < a.length; i++) {
      for (int j = 0; j < a.length; j++)
        if ((i + j) % 2) out << setw(4) << 0;
        else out << setw(4) << a.data[(i * a.length + j) / 2];
      out << endl;
    }
    return out;
  }
};
