#include <cmath>
#include <iostream>
#include <stdexcept>
using namespace std;

class Move {
public:
  // Take sc coins from heap sh and put tc coins to heap th.
  Move(int sh, int sc, int th, int tc):
  csh(sh), csc(sc), cth(th), ctc(tc) {}

  int getSource() const { return csh; }
  int getSourceCoins() const { return csc; }
  int getTarget() const { return cth; }
  int getTargetCoins() const { return ctc; }

  friend ostream & operator << (ostream &out, const Move &move) {
    out << "takes " << move.getSourceCoins() << " coins from heap "
    << move.getSource() << " and puts ";
    if (move.getTargetCoins() == 0) out << "nothing";
    else out << move.getTargetCoins() << " coins to heap " << move.getTarget();
    return out;
  }

private:
  int csh, csc, cth, ctc;
};

class State {
public:
  // State with h heaps, where the i-th heap starts with c[i] coins.
  State(int h, const int c[]) {
    totalheaps = h;
    a = new int[h];
    for (int i = 0; i < h; i++) a[i] = c[i];
  }

  ~State() {
    delete [] a;
  }

  void next(const Move &move) throw(logic_error) {
    if (move.getSource() < 0 or move.getTarget() < 0
      or move.getSource() > totalheaps or move.getTarget() > totalheaps
      or move.getSourceCoins() <= move.getTargetCoins())
      throw logic_error("invalid heap");
    if (move.getSourceCoins() > a[move.getSource()])
      throw logic_error("not enough coins");
    a[move.getSource()] -= move.getSourceCoins();
    a[move.getTarget()] += move.getTargetCoins();
  }
  bool winning() const {
    for (int i = 0; i < totalheaps; i++)
      if (a[i] != 0) return false;
    return true;
  }

  int getHeaps() const { return totalheaps; }
  int getCoins(int h) const throw(logic_error) {
    if (h < 0 or h >= totalheaps) throw logic_error("invalid heap");
    return a[h];
  }

  friend ostream & operator << (ostream &out, const State &state) {
    out << state.a[0];
    for (int i = 1; i < state.totalheaps; i++) out << ", " << state.a[i];
    return out;
  }
private:
  int *a, totalheaps;
};

class Player {
public:
  Player(const string &n): name(n) {}
  virtual ~Player() {
    name.clear();
  }

  virtual const string & getType() const = 0;
  virtual Move play(const State &s) = 0;

  friend ostream & operator << (ostream &out, const Player &player) {
    out << player.getType() << " player " << player.name;
    return out;
  }
protected:
  string name;
};

class GreedyPlayer: public Player {
public:
  GreedyPlayer(const string &n): Player(n), type("Greedy") {}
  const string & getType() const override { return type; }
  Move play(const State &s) override {
    int maxcoins = 0, maxcoinsheap;
    for (int i = 0; i < s.getHeaps(); i++)
      if (s.getCoins(i) > maxcoins) {
        maxcoins = s.getCoins(i);
        maxcoinsheap = i;
      }
    return Move(maxcoinsheap, maxcoins, 0, 0);
  }
private:
  string type;
};

class SpartanPlayer: public Player {
public:
  SpartanPlayer(const string &n): Player(n), type("Spartan") {}
  const string & getType() const override { return type; }
  Move play(const State &s) override {
    int maxcoins = 0, maxcoinsheap;
    for (int i = 0; i < s.getHeaps(); i++)
      if (s.getCoins(i) > maxcoins) {
        maxcoins = s.getCoins(i);
        maxcoinsheap = i;
      }
    return Move(maxcoinsheap, 1, 0, 0);
  }
private:
  string type;
};

class SneakyPlayer: public Player {
public:
  SneakyPlayer(const string &n): Player(n), type("Sneaky") {}
  const string & getType() const override { return type; }
  Move play(const State &s) override {
   int maxcoins = 0;
    for (int i = 0; i < s.getHeaps(); i++)
      if (s.getCoins(i) > maxcoins) maxcoins = s.getCoins(i);
    int mincoins = maxcoins + 1, mincoinsheap;
    for (int i = 0; i < s.getHeaps(); i++)
      if (s.getCoins(i) < mincoins and s.getCoins(i) != 0) {
        mincoins = s.getCoins(i);
        mincoinsheap = i;
      }
    return Move(mincoinsheap, mincoins, 0, 0);
  }
private:
  string type;
};

class RighteousPlayer: public Player {
public:
  RighteousPlayer(const string &n): Player(n), type("Righteous") {}
  const string & getType() const override { return type; }
  Move play(const State &s) override {
    int maxcoins = s.getCoins(0), mincoins = s.getCoins(0), maxcoinsheap = 0, mincoinsheap = 0;
    for (int i = 1; i < s.getHeaps(); i++) {
      if (s.getCoins(i) > maxcoins) {
        maxcoins = s.getCoins(i);
        maxcoinsheap = i;
      }
      else if (s.getCoins(i) < mincoins) {
        mincoins = s.getCoins(i);
        mincoinsheap = i;
      }
    }
    return Move(maxcoinsheap, ceil(maxcoins / 2.0), mincoinsheap, ceil(maxcoins / 2.0) - 1);
  }
private:
  string type;
};

class Game {
public:
  Game(int heaps, int players): totalh(heaps), totalp(players), h(0), p(0) {
    coinz = new int [heaps];
    playah = new Player* [players];
  }
  ~Game() {
    delete [] coinz;
    delete [] playah;
  }

  void addHeap(int coins) throw(logic_error) {
    if (h == totalh) throw logic_error("too many heaps");
    coinz[h++] = coins;
  }

  void addPlayer(Player *player) throw(logic_error) {
    if (p == totalp) throw logic_error("too many players");
    playah[p++] = player;
  }

  void play(ostream &out) throw(logic_error) {
    bool won = false;
    State state(totalh, coinz);
    out << "State: " << state << endl;
    while(true) {
      for (int i = 0; i < totalp; i++) {
        out << *playah[i] << ' ' << playah[i]->play(state) << endl;
        state.next(playah[i]->play(state));
        out << "State: " << state << endl;
        if (state.winning()) {
          out << *playah[i] << " wins" << endl;
          won = true;
          break;
        }
      }
      if (won) break;
    }
  }
private:
  int totalh, totalp, h, p, *coinz;
  Player **playah;
};

