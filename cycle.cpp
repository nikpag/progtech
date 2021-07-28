#include <iostream>
#include <vector>
#include <stack>
using namespace std;

class Graph {
public:
	Graph(int v);
	~Graph();
	bool cycle(vector<int> &path) const;
	void addEdge(int u, int v);
private:
	bool cycle_help(int i, int p[], int status[], stack<int> &s) const;
	int **adj;
	int size;
};

Graph::Graph(int v) {
	size = v;
	adj = new int* [v];
	for (int i = 0; i < v; ++i) adj[i] = new int [v];
	for (int i = 0; i < v; ++i)
		for (int j = 0; j < v; ++j)
			adj[i][j] = 0;
}

Graph::~Graph() {
	for (int i = 0; i < size; ++i) delete [] adj[i];
	delete [] adj;
}

void Graph::addEdge(int u, int v) { adj[u][v]++; }

bool Graph::cycle(vector<int> &path) const {
	int status[size], p[size];
	stack<int> s;
	for (int i = 0; i < size; ++i) {
		status[i] = -1;
		p[i] = -1;
	}
	for (int i = 0; i < size; ++i) {
		if (status[i] == -1) {
			if(cycle_help(i, p, status, s)) {
				path.insert(path.begin(), s.top());
				int flag = s.top();
				s.pop();
				while (s.top() != flag) {
					path.insert(path.begin(), s.top());
					s.pop();
				}
				return true;
 		  }
		}
	}
	return false;
}

bool Graph::cycle_help(int i, int p[], int status[], stack<int> &s) const {
	status[i] = 0;
	s.push(i);
	for (int j = 0; j < size; ++j) {
		if (adj[i][j] == 1) {
			if (status[j] == -1) {
				p[j] = i;
				if (cycle_help(j, p, status, s)) return true;
			}
			else if (status[j] == 0) {
				s.push(j);
				p[j] = i;
				return true;
			}
		}
	}
	status[i] = 1;
	return false;
}
#if 0
int main() {
	int V, E;
	cin >> V >> E;
	Graph g(V);
	for (int i = 0; i < E; ++i) {
		int u, v;
		cin >> u >> v;
		g.addEdge(u, v);
	}
	vector<int> path;
	bool c = g.cycle(path);
	if (c) {
		cout << "CYCLE: ";
		for (int i = 0; i < path.size(); ++i)
			cout << path[i] << (i == path.size() - 1 ? "\n" : " ");
	}
	else {
		cout << "NO CYCLE" << endl;
	}
}

#else
int main() {
	int n;
	cin >> n;
	Graph g(n);
	for (int i = 0; i < n - 1; ++i) g.addEdge(i, i + 1);
	g.addEdge(n - 1, 0);
	vector<int> path;
	bool c = g.cycle(path);
	if (c) {
		cout << "CYCLE: ";
		for (int i = 0; i < path.size(); ++i)
			cout << path[i] << (i == path.size() - 1 ? "\n" : " ");
	}
	else {
		cout << "NO CYCLE" << endl;
	}
}
#endif