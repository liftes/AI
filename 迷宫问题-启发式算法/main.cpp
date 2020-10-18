#include <iostream>
#include <cstdlib>
#include <math.h>
#include <time.h>
#include <Windows.h>
#include <stack>

#define ROWS 4
#define COLS 4
#define DEPTH 20
using namespace std;
//迷宫规模的大小
int rows = 4;
int cols = 4;

class PathNode {
public:
	int row;
	int col;
	int F;
	int h;
	int deep;
	stack<int> son;//该节点的后继节点
};

PathNode* MapGet(PathNode* map) {
	for (int i = 0; i < ROWS; i++) {
		for (int j = 0; j < COLS; j++) {
			map[i * ROWS + j].row = i;
			map[i * ROWS + j].col = j;
			map[i * ROWS + j].h = ROWS - i + COLS - j;
		}
	}
	//初始化地图
	map[0].son.push(4);
	map[1].son.push(2);
	map[1].son.push(5);
	map[2].son.push(1);
	map[2].son.push(3);
	map[2].son.push(6);
	map[3].son.push(2);
	map[3].son.push(7);
	map[4].son.push(8);
	map[5].son.push(9);
	map[5].son.push(6);
	map[6].son.push(5);
	map[6].son.push(10);
	map[7].son.push(11);
	map[8].son.push(9);
	map[9].son.push(8);
	map[9].son.push(13);
	map[10].son.push(14);
	map[10].son.push(11);
	map[11].son.push(10);
	map[11].son.push(15);
	map[12].son.push(13);
	map[13].son.push(9);
	map[13].son.push(14);
	map[14].son.push(13);
	map[14].son.push(10);

	map[0].deep = 0;
	map[0].F = map[0].deep + map[0].h;

	return map;
}

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

bool FindEqual(int* open, int* close, int index) {
	for (int i = 1; i <= open[0]; i++) {
		if (open[i] == index) {
			return false;
		}
	}
	for (int i = 1; i <= close[0]; i++) {
		if (close[i] == index) {
			return false;
		}
	}
	return true;
}

void OpenAdd(int* open, int* close, int deep, int index, PathNode* map) {
	while (!map[index].son.empty()) {
		int id = map[index].son.top();
		map[index].son.pop();
		if (FindEqual(open, close, id)) {
			open[0]++;
			open[open[0]] = id;
			map[id].deep = deep;
			map[id].F = map[id].h + deep;
		}
	}
}

void FirstOpen(PathNode* map, int* open) {
	int max = 0;
	int flagIndex = 0;
	for (int i = 1; i <= open[0]; i++) {
		if (map[open[i]].F > max) {
			max = map[open[i]].F;
			flagIndex = i;
		}
	}
	int maxID = open[flagIndex];
	if (flagIndex != 1 && flagIndex != 0) {
		for (int i = flagIndex; i > 1; i--) {
			open[i] = open[i - 1];
		}
		open[1] = maxID;
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
	if (index == 15) {
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
	cout << index << endl;

	Searching(map, open, close);
}

int main() {
	//初始化地图
	PathNode map[ROWS * COLS];
	MapGet(map);
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

	return 0;
}