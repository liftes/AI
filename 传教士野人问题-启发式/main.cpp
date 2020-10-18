#include <iostream>
#include <cstdlib>
#include <math.h>
#include <time.h>
#include <Windows.h>
#include <stack>

#define DEPTH 100

using namespace std;
//N为传教士和野人数目，K为船最大承载人数
int N = 5;
int K = 3;
int NUM = 1;

class PathNode {
public:
	int* date;
	int F;
	int h;
	int deep;
	int father;//该节点的后继节点的数组id
};

//OPEN表删除第一个元素,并将删除元素返回
int OpenRemove(int* open) {
	int remove = open[1];
	for (int i = 1; i < open[0]; i++) {
		open[i] = open[i + 1];
	}
	open[open[0]] = 0;
	open[0]--;
	return remove;
}

//Close添加元素
void CloseAdd(int* close, int index) {
	close[close[0] + 1] = index;
	close[0]++;
}

bool FindEqual(int* open, int* close, int* date, PathNode* map) {
	for (int i = 1; i <= open[0]; i++) {
		int count = 0;
		for (int j = 0; j < 3; j++) {
			if (map[open[i]].date[j] == date[j]) {
				count++;
			}
		}
		if (count == 3) {
			return false;
		}
	}
	for (int i = 1; i <= close[0]; i++) {
		int count = 0;
		for (int j = 0; j < 3; j++) {
			if (map[close[i]].date[j] == date[j]) {
				count++;
			}
		}
		if (count == 3) {
			return false;
		}
	}
	return true;
}

void OpenAdd(int* open, int* close, int deep, int index, PathNode* map) {
	int* date = map[index].date;
	int BL;
	if (date[2] == 0) {
		BL = 1;
		for (int i = 1; i <= K; i++) {
			for (int BM = 0; BM <= i; BM++) {
				int BC = i - BM;
				int* date0;
				date0 = (int*)malloc(3 * sizeof(int*));
				date0[0] = date[0] + BM;
				date0[1] = date[1] + BC;
				date0[2] = BL;
				if ((BM >= BC || BM == 0) && (date0[0] >= date0[1] || date0[0] == 0) && (N - date0[0] >= N - date0[1] || N - date0[0] == 0) && date0[0] >= 0 && date0[1] >= 0) {
					if (FindEqual(open, close, date0, map)) {
						map[NUM].date = date0;
						open[0]++;
						open[open[0]] = NUM;
						map[NUM].deep = deep;
						map[NUM].h = date0[0] + date0[1];
						map[NUM].F = map[NUM].h + deep;
						map[NUM].father = index;
						NUM++;
					}
				}
			}
		}
	}
	else {
		BL = 0;
		for (int i = 1; i <= K; i++) {
			for (int BM = 0; BM <= i; BM++) {
				int BC = i - BM;
				int* date0;
				date0 = (int*)malloc(3 * sizeof(int*));
				date0[0] = date[0] - BM;
				date0[1] = date[1] - BC;
				date0[2] = BL;
				if ((BM >= BC || BM == 0) && (date0[0] >= date0[1] || date0[0] == 0) && (N - date0[0] >= N - date0[1] || N - date0[0] == 0) && date0[0] >= 0 && date0[1] >= 0) {
					if (FindEqual(open, close, date0, map)) {
						map[NUM].date = date0;
						open[0]++;
						open[open[0]] = NUM;
						map[NUM].deep = deep;
						map[NUM].h = date0[0] + date0[1];
						map[NUM].F = map[NUM].h + deep;
						map[NUM].father = index;
						NUM++;
					}
				}
			}
		}
	}

}

void FirstOpen(PathNode* map, int* open) {
	int min = 1000;
	int flagIndex = 0;
	for (int i = 1; i <= open[0]; i++) {
		if (map[open[i]].F < min) {
			min = map[open[i]].F;
			flagIndex = i;
		}
	}
	int minID = open[flagIndex];
	if (flagIndex != 1 && flagIndex != 0) {
		for (int i = flagIndex; i > 1; i--) {
			open[i] = open[i - 1];
		}
		open[1] = minID;
	}
}

int Searching(PathNode* map, int* open, int* close) {
	int sizeOpen = open[0];
	//检验OPEN表是否为空
	if (sizeOpen == 0) {
		return -1;
	}
	//执行open表和close表的增改操作
	int index = OpenRemove(open);
	CloseAdd(close, index);
	//检测是否结束排序(完成或超出最大步长限制)
	if (map[index].date[0] + map[index].date[1] + map[index].date[2] == 0) {
		printf("##########\nSearching Finished！\n");
		return index;
	}
	//拓展open表
	int deep = map[index].deep + 1;
	if (deep < DEPTH) {
		OpenAdd(open, close, deep, index, map);
	}
	//按启发函数对open表排序
	FirstOpen(map, open);
	Searching(map, open, close);
}

void PrintALL(PathNode* map, int flag) {
	int count = 0;
	if (flag >= 0) {
		do {
			cout << map[flag].date[0] << map[flag].date[1] << map[flag].date[2] << endl;
			flag = map[flag].father;
			count++;
		} while (map[flag].father != -1);
	}
	cout << map[0].date[0] << map[0].date[1] << map[0].date[2] << endl;
	cout << count << endl;
}

int main() {
	//初始化地图，其中NUM表示map数组中的有效结点个数
	PathNode map[1000];
	int first[3] = { N,N,1 };
	map[0].date = first;
	map[0].deep = 0;
	map[0].h = N + N;
	map[0].F = N + N;
	map[0].father = -1;
	//设置Open表和Close表
	int* open1, * close1;
	open1 = (int*)malloc(1000 * sizeof(int*));
	close1 = (int*)malloc(1000 * sizeof(int*));
	open1[0] = 1;
	open1[1] = 0;
	close1[0] = 0;

	clock_t start1 = clock();
	int flag1 = Searching(map, open1, close1);
	clock_t  end1 = clock();

	PrintALL(map, flag1);

	return 0;
}