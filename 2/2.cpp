#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <numeric>
#include <string>
#include <vector>
#include <list>
#include <stack>
#include <queue>
#include <set>
#include <map>
using namespace std;

// 백트래킹 할 때, 다음 상태로 넘어가면 안되는 값들 제대로 복원시키기(mp, runner, catcher, score)★★
// 전역변수, 지역변수 범위 잘 생각★★
// 함수 호출시 복사할건지, 참조 할건지 조심★★

struct info {
	int r, c, value, mov;

	info(int aa, int bb, int cc, int dd) {
		r = aa;
		c = bb;
		value = cc;
		mov = dd;
	}

	bool operator<(const info& b)const {
		return value < b.value;
	}
};

int score, res;
int dr[] = { -1, -1, 0, 1, 1, 1, 0, -1 };
int dc[] = { 0, -1, -1, -1, 0, 1, 1, 1 };
info catcher = info(0, 0, 0, 0);
vector<info> runner;
vector<vector<int> > mp(4, vector<int>(4, 0));

void runner_move(info run, int mov, int idx) {
	int nr = run.r + dr[mov];
	int nc = run.c + dc[mov];

	if (nr < 0 || nr>3 || nc < 0 || nc > 3 || mp[nr][nc] == 2147) {
		runner_move(run, (mov + 1) % 8, idx);
	}

	else {
		if (mp[nr][nc] == 0) {
			mp[nr][nc] = mp[run.r][run.c];
			mp[run.r][run.c] = 0;

			runner[idx].r = nr;
			runner[idx].c = nc;
			runner[idx].mov = mov;
		}

		else {
			int temp = mp[nr][nc];

			mp[nr][nc] = mp[run.r][run.c];
			runner[idx].r = nr;
			runner[idx].c = nc;
			runner[idx].mov = mov;

			mp[run.r][run.c] = temp;
			for (int i = 0; i < runner.size(); i++) {
				if (runner[i].value == temp) {
					runner[i].r = run.r;
					runner[i].c = run.c;
				}
			}
		}
	}
}

void move_start() {
	vector<info> catch_list;

	for (int i = 0; i < runner.size(); i++) {
		runner_move(runner[i], runner[i].mov, i);
	}

	int nr = catcher.r + dr[catcher.mov];
	int nc = catcher.c + dc[catcher.mov];

	while (nr >= 0 && nr <= 3 && nc >= 0 && nc <= 3) {
		if (mp[nr][nc] != 0) {
			for (int i = 0; i < runner.size(); i++) {
				if (runner[i].value == mp[nr][nc]) {
					catch_list.push_back(runner[i]);
				}
			}
		}

		nr += dr[catcher.mov];
		nc += dc[catcher.mov];
	}

	if (catch_list.size() == 0) {
		res = max(res, score);
	}

	else {
		info ori_catcher = catcher;
		vector<info> tempp = runner;

		for (int i = 0; i < catch_list.size(); i++) {
			info ori_runner = catch_list[i];
			vector<vector<int> > temp(4, vector<int>(4, 0));

			for (int a = 0; a < 4; a++) {
				for (int b = 0; b < 4; b++) {
					temp[a][b] = mp[a][b];
				}
			}

			score += catch_list[i].value;

			mp[catch_list[i].r][catch_list[i].c] = catcher.value;
			mp[catcher.r][catcher.c] = 0;

			catcher.r = catch_list[i].r;
			catcher.c = catch_list[i].c;
			catcher.mov = catch_list[i].mov;
			
			for (int k = 0; k < runner.size(); k++) {
				if (runner[k].value == catch_list[i].value) {
					runner.erase(runner.begin() + k);
					break;
				}
			}

			move_start();

			score -= ori_runner.value;
			catcher = ori_catcher;
			runner = tempp;

			for (int a = 0; a < 4; a++) {
				for (int b = 0; b < 4; b++) {
					mp[a][b] = temp[a][b];
				}
			}
		}
	}
}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	freopen("input.txt", "rt", stdin);

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			int temp1, temp2;
			cin >> temp1 >> temp2;
			mp[i][j] = temp1;
			
			if (i == 0 && j == 0) {
				score += mp[i][j];
				catcher = info(i, j, 2147, temp2 - 1);
				mp[i][j] = catcher.value;
			}

			else {
				runner.push_back(info(i, j, temp1, temp2 - 1));
			}
		}
	}

	sort(runner.begin(), runner.end());
	move_start();

	cout << res << "\n";
	return 0;
}