#include<iostream>
#include<cstdlib>
#include<math.h>
#include<time.h>

using namespace std;

//######################################
//随机生成八数码的初始态
int** NewMap() {
	//设置随机排序0-9,0表示可移动的空位
	int array1[9] = { 0,1,2,3,4,5,6,7,8 };
	static int array2[9] = { 0 };
	srand((unsigned)time(NULL));
	for (int i = 8; i > 0; i--) {
		int pos = rand() % i;
		array2[i] = array1[pos];
		for (int j = pos; j < 8; j++) {
			array1[j] = array1[j + 1];
		}
	}
	//array2记录了0-9的随机排序
	array2[0] = array1[0];
	int array3[9] = { 1,2,3,7,8,4,6,5,0 };
	//利用随机数列初始化地图MAP
	int** MAP;
	MAP = (int**)malloc(3 * sizeof(int**));
	for (int i = 0; i < 3; i++) {
		MAP[i] = (int*)malloc(3 * sizeof(int**));
	}
	for (int i = 0, k = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++, k++) {
			MAP[i][j] = array3[k];
		}
	}
	return MAP;
};

//打印地图
void PrintMap(int** MAP) {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			printf("%d ", MAP[i][j]);
		}
		printf("\n");
	}
	printf("———\n");
}

//判定移动，index表示0的移动方向，2↓4←6→8↑，如果无法移动则返回空
int** Move(int index, int** MAP) {
	//找0的位置
	int i0, j0 = 0;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (MAP[i][j] == 0) {
				i0 = i;
				j0 = j;
			}
		}
	}
	//控制移动边界
	if (i0 == 0 && index == 8) {
		return NULL;
	}
	if (i0 == 2 && index == 2) {
		return NULL;
	}
	if (j0 == 0 && index == 4) {
		return NULL;
	}
	if (j0 == 2 && index == 6) {
		return NULL;
	}

	int** map;
	map = (int**)malloc(3 * sizeof(int**));
	for (int i = 0; i < 3; i++) {
		map[i] = (int*)malloc(3 * sizeof(int**));
	}
	for (int i = 0, k = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++, k++) {
			map[i][j] = MAP[i][j];
		}
	}
	if (index == 2) {
		map[i0][j0] = map[i0 + 1][j0];
		map[i0 + 1][j0] = 0;
	}
	if (index == 8) {
		map[i0][j0] = map[i0 - 1][j0];
		map[i0 - 1][j0] = 0;
	}
	if (index == 4) {
		map[i0][j0] = map[i0][j0 - 1];
		map[i0][j0 - 1] = 0;
	}
	if (index == 6) {
		map[i0][j0] = map[i0][j0 + 1];
		map[i0][j0 + 1] = 0;
	}
	return map;
}

//#######################################
//树结构，采用双亲表示法，便于回溯完整路径
template<typename T>
struct Node {
	T _data;
	int parent;
	int deep;
};
template<typename T>
struct Tree {
	Node<T> array[700];
	int count;
};
template<typename T>
void AddNode(T data, int parent, int deep, Tree<T>& tree) {
	tree.array[tree.count]._data = data;
	tree.array[tree.count].parent = parent;
	tree.array[tree.count].deep = deep;
	tree.count++;
}
template<typename T>
bool FindEqual(Tree<T>& tree, int** map) {
	for (int t = 0; t < tree.count; t++) {
		int count = 0;
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				if (map[i][j] == tree.array[t]._data[i][j]) {
					count++;
				}
			}
		}
		if (count == 9) {
			return false;
		}
	}
	return true;
}

//########################################
//OPEN添加元素,index表示结点值
template<typename T>
void OpenAdd(int* open, int index, int** map, Tree<T>& tree, int deep) {
	int** t1, ** t2, ** t3, ** t4;
	t1 = Move(2, map);
	t2 = Move(4, map);
	t3 = Move(6, map);
	t4 = Move(8, map);
	if (NULL != t1 && FindEqual(tree,t1)) {
		AddNode(t1, index, deep, tree);
		open[0]++;
		open[open[0]] = tree.count - 1;
	}
	if (NULL != t2 && FindEqual(tree, t2)) {
		AddNode(t2, index, deep, tree);
		open[0]++;
		open[open[0]] = tree.count - 1;
	}
	if (NULL != t3 && FindEqual(tree, t3)) {
		AddNode(t3, index, deep, tree);
		open[0]++;
		open[open[0]] = tree.count - 1;
	}
	if (NULL != t4 && FindEqual(tree, t4)) {
		AddNode(t4, index, deep, tree);
		open[0]++;
		open[open[0]] = tree.count - 1;
	}
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
void CloseAdd(int* close,int index) {
	close[close[0] + 1] = index;
	close[0]++;
}

//检验是否完成八数码的排序
bool EndAll(int** map) {
	int arrayEnd[9] = { 1,2,3,8,0,4,7,6,5 };
	for (int i = 0, k = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++, k++) {
			if (map[i][j] != arrayEnd[k]) {
				return false;
			}
		}
	}
	return true;
}

//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//普通图搜索，-1表示false，正数表示success后的结尾index坐标
template<typename T>
int GraphSearching(Tree<T>& tree, int* open, int* close, int deep) {
	int sizeOpen = open[0];
	deep++;
	//检验OPEN表是否为空
	if (sizeOpen == 0) {
		return -1;
	}
	//执行open表和close表的增改操作
	int index = OpenRemove(open);
	CloseAdd(close, index);
	//检测是否结束排序(完成或超出最大步长限制)
	if (EndAll(tree.array[index]._data)) {
		printf("Do It!!!\n");
		return index;
	}
	//拓展open表
	if (deep < 10) {
		OpenAdd(open, index, tree.array[index]._data, tree, deep);
	}
	GraphSearching(tree, open, close, deep);
}

int main() {
	int** MAP;
	//初始化地图
	MAP = NewMap();
	PrintMap(MAP);
	//设置Open表和Close表
	int* open1, * close1;
	open1 = (int*)malloc(10000 * sizeof(int*));
	close1 = (int*)malloc(10000 * sizeof(int*));
	open1[0] = 1;
	open1[1] = 0;
	close1[0] = 0;
	//定义树
	Tree<int**> tree1;
	tree1.count = 0;
	int deep = 0;
	AddNode(MAP, -1, deep, tree1);
	int flag = GraphSearching(tree1, open1, close1, deep);

	if (flag >= 0) {
		do {
			PrintMap(tree1.array[flag]._data);
			flag = tree1.array[flag].parent;
		} while (tree1.array[flag].parent != -1);
		PrintMap(MAP);
	}

	return 0;
}