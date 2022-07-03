#include <string>
#include <iostream>
using namespace std;

struct node {
	string word;
	int freq;
	node *left, *right;
	node (const string &w, int f): word(w), freq(f), left(nullptr), right(nullptr) {}
};

class lexicon {
public:
	lexicon();
	~lexicon();

	void insert(const string &s);
	int lookup(const string &s) const;
	int depth(const string &s);
	void replace(const string &s1, const string &s2);

	friend ostream & operator << (ostream &out, const lexicon &l);

private:
	node *root;

	void insert(node* &t, const string &s, int f);
	int lookup(node *t, const string &s) const;
	int depth(node *t, const string &s);

	void purge(node *t);
	node* maximum(node *t);
	node* deleteword(node *t, const string &s);
	void print(node *t) const;
};

lexicon::lexicon(): root(nullptr) {}
lexicon::~lexicon() { purge(root); }

void lexicon::insert(const string &s) { insert(root, s, 1); }
int lexicon::lookup(const string &s) const { return lookup(root, s); }
int lexicon::depth(const string &s) { return depth(root, s); }
void lexicon::replace(const string &s1, const string &s2) {
	int f = lookup(s1);
	if (f == 0) return;
	root = deleteword(root, s1);
	insert(root, s2, f);
}

ostream & operator << (ostream &out, const lexicon &l) { l.print(l.root); return out; }

void lexicon::insert(node* &t, const string &s, int f) {
	if (t == nullptr) t = new node(s, f); 
	else if (s < t->word) insert(t->left, s, f);
	else if (s > t->word) insert(t->right, s, f);
	else t->freq += f;
}
int lexicon::lookup(node *t, const string &s) const {
	if (t == nullptr) return 0;
	else if (s < t->word) return lookup(t->left, s);
	else if (s > t->word) return lookup(t->right, s);
	else return t->freq;
}
int lexicon::depth(node *t, const string &s) {
	if (t == nullptr) return 0;
	if (s == t->word) return 1;
	if (s < t->word) {
		int d = depth(t->left, s);
		return d ? 1 + d : 0;
	}
	else {
		int d = depth(t->right, s);
		return d ? 1 + d : 0;
	}
}

void lexicon::purge(node *t) {
	if (t == nullptr) return;
	purge(t->left);
	purge(t->right);
	delete t;
}
node* lexicon::maximum(node *t) {
	while (t->right != nullptr) t = t->right;
	return t;
}
node* lexicon::deleteword(node *t, const string &s) {
	if(t == nullptr) return nullptr;
	if (s < t->word) t->left = deleteword(t->left, s);
	else if (s > t->word) t->right = deleteword(t->right, s);
	else {
		if (t->left == nullptr) {
			node *temp = t->right;
			delete t;
			return temp;
		}
		else if (t->right == nullptr) {
			node *temp = t->left;
			delete t;
			return temp;
		}
		else {
			node *temp = maximum(t->left);
			t->word = temp->word;
			t->freq = temp->freq;

			t->left = deleteword(t->left, temp->word);
		}
		return t;
	}
}
void lexicon::print(node *t) const {
	if (t != nullptr) {
		print(t->left);
		cout << t->word << ' ' << t->freq << endl;
		print(t->right);
	}
}

#ifndef CONTEST
int main() {
	lexicon l;
	l.insert("the");
	l.insert("boy");
	l.insert("and");
	l.insert("the");
	l.insert("wolf");
	cout << l;
	l.replace("and", "dummy");
	cout << "dummy " << l.lookup("dummy") << " times in depth " << l.depth("dummy") << endl;
	l.replace("boy", "dummy");
	cout << "dummy " << l.lookup("dummy") << " times in depth " << l.depth("dummy") << endl;
	l.replace("the", "dummy");
	cout << "dummy " << l.lookup("dummy") << " times in depth " << l.depth("dummy") << endl;
	l.replace("wolf", "dummy");
	cout << "dummy " << l.lookup("dummy") << " times in depth " << l.depth("dummy") << endl;
	cout << l;
}

#endif
