#include <iostream>
using namespace std;

int main() {
	int N, M;
	cin >> N >> M;
	int a[N][N];
	for (int i = 0; i < N; ++i)
		for (int j = 0; j < N; ++j)
			a[i][j] = 0;

	for (int i = 0; i < M; ++i) {
		int u, v;
		cin >> u >> v;
		if (u != v) {
			a[u][v]++;
			a[v][u]++;
		}
	}

	int odd1 = -1, odd2 = -1;

	for (int i = 0; i < N; ++i) {
		int sum = 0;
		for (int j = 0; j < N; ++j) sum += a[i][j];

		if (sum % 2) {
			if (odd1 == -1) odd1 = i;
			else if (odd2 == -1) odd2 = i;
			else {
				cout << "IMPOSSIBLE" << endl;
				exit(0);
			}
		}
	}
	if (odd1 == -1) cout << "CYCLE" << endl;
	else if (odd2 == -1) cout << "IMPOSSIBLE" << endl;
	else cout << "PATH " << odd1 << ' ' << odd2 << endl;
}