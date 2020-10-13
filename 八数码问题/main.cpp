#include<iostream>
#include<cstdlib>
#include<math.h>
#include<time.h>
#include<Windows.h>

using namespace std;
//��������������
int DEPTH = 15;

//######################################
//������ɰ�����ĳ�ʼ̬
int** NewMap() {
	//�����������0-9,0��ʾ���ƶ��Ŀ�λ
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
	//array2��¼��0-9���������
	array2[0] = array1[0];
	int array3[9] = { 0,1,2,7,8,3,6,5,4 };
	//����������г�ʼ����ͼMAP
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

//��ӡ��ͼ
void PrintMap(int** MAP) {
	printf("������\n");
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			printf("%d ", MAP[i][j]);
		}
		printf("\n");
	}
	printf("������\n");
}

//�ж��ƶ���index��ʾ0���ƶ�����2��4��6��8��������޷��ƶ��򷵻ؿ�
int** Move(int index, int** MAP) {
	//��0��λ��
	int i0, j0 = 0;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (MAP[i][j] == 0) {
				i0 = i;
				j0 = j;
			}
		}
	}
	//�����ƶ��߽�
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

//���������Ĵ��ۺ�����h������
int InspiredMap(int** map) {
	int count = 0;
	int arrayEnd[9] = { 1,2,3,8,0,4,7,6,5 };
	for (int i = 0, k = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++, k++) {
			if (map[i][j] != arrayEnd[k]) {
				count++;
			}
		}
	}
	return count;
}

//#######################################
//���ṹ������˫�ױ�ʾ�������ڻ�������·��
template<typename T>
struct Node {
	T _data;
	int parent;
	int deep;
	int flag;
};
template<typename T>
struct Tree {
	Node<T> array[10000];
	int count;
};
template<typename T>
void AddNode(T data, int parent, int deep, Tree<T>& tree, int flag = 0) {
	tree.array[tree.count]._data = data;
	tree.array[tree.count].parent = parent;
	tree.array[tree.count].deep = deep;
	//�ڹ�����ʱ���f�����ļ���
	tree.array[tree.count].flag = flag + deep;
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
//OPEN���Ԫ��,index��ʾ���ֵ
template<typename T>
void OpenAdd(int* open, int index, int** map, Tree<T>& tree, int deep) {
	int** t1, ** t2, ** t3, ** t4;
	t1 = Move(2, map);
	t2 = Move(4, map);
	t3 = Move(6, map);
	t4 = Move(8, map);
	if (NULL != t1 && FindEqual(tree,t1)) {
		AddNode(t1, index, deep, tree, InspiredMap(t1));
		open[0]++;
		open[open[0]] = tree.count - 1;
	}
	if (NULL != t2 && FindEqual(tree, t2)) {
		AddNode(t2, index, deep, tree, InspiredMap(t2));
		open[0]++;
		open[open[0]] = tree.count - 1;
	}
	if (NULL != t3 && FindEqual(tree, t3)) {
		AddNode(t3, index, deep, tree, InspiredMap(t3));
		open[0]++;
		open[open[0]] = tree.count - 1;
	}
	if (NULL != t4 && FindEqual(tree, t4)) {
		AddNode(t4, index, deep, tree, InspiredMap(t4));
		open[0]++;
		open[open[0]] = tree.count - 1;
	}
}

//OPEN��ɾ����һ��Ԫ��,����ɾ��Ԫ�ط���
int OpenRemove(int* open) {
	int remove = open[1];
	for (int i = 1; i < open[0]; i++) {
		open[i] = open[i + 1];
	}
	open[open[0]] = 0;
	open[0]--;
	return remove;
}

//Close���Ԫ��
void CloseAdd(int* close,int index) {
	close[close[0] + 1] = index;
	close[0]++;
}

//�����Ƿ���ɰ����������
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

//��ӡ��ɺ���ƶ�·��������
template<typename T>
void PrintALL(Tree<T>& tree1, int flag, int** MAP) {
	if (flag >= 0) {
		do {
			PrintMap(tree1.array[flag]._data);
			flag = tree1.array[flag].parent;
		} while (tree1.array[flag].parent != -1);
		PrintMap(MAP);
	}
}

//��������㷨OPEN����
template<typename T>
void FirstDeepOpen(Tree<T>& tree, int* open) {
	int max = 0;
	int flagIndex = 0;
	for (int i = 1; i <= open[0]; i++) {
		if (tree.array[open[i]].deep > max) {
			max = tree.array[open[i]].deep;
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

//��������㷨OPEN����
template<typename T>
void FirstBreadthOpen(Tree<T>& tree, int* open) {
	int min = 100;
	int flagIndex = 0;
	for (int i = 1; i <= open[0]; i++) {
		if (tree.array[open[i]].deep < min) {
			min = tree.array[open[i]].deep;
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

//����ʽ������OPEN����
template<typename T>
void FirstInspiredOpen(Tree<T>& tree, int* open) {
	int min = 100;
	int flagIndex = 0;
	for (int i = 1; i <= open[0]; i++) {
		if (tree.array[open[i]].flag < min) {
			min = tree.array[open[i]].flag;
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

//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//��ͨͼ������-1��ʾfalse��������ʾsuccess��Ľ�βindex����
template<typename T>
int GraphSearching(Tree<T>& tree, int* open, int* close) {
	int sizeOpen = open[0];
	//����OPEN���Ƿ�Ϊ��
	if (sizeOpen == 0) {
		return -1;
	}
	//ִ��open���close������Ĳ���
	int index = OpenRemove(open);
	CloseAdd(close, index);
	//����Ƿ��������(��ɻ򳬳���󲽳�����)
	if (EndAll(tree.array[index]._data)) {
		printf("##########\nGraph Searching Finished��\n");
		return index;
	}
	//��չopen��
	int deep = tree.array[index].deep + 1;
	if (deep < DEPTH) {
		OpenAdd(open, index, tree.array[index]._data, tree, deep);
	}
	GraphSearching(tree, open, close);
}

//�������ͼ������-1��ʾfalse��������ʾsuccess��Ľ�βindex����
template<typename T>
int DepthFirstSearching(Tree<T>& tree, int* open, int* close) {
	int sizeOpen = open[0];
	//����OPEN���Ƿ�Ϊ��
	if (sizeOpen == 0) {
		return -1;
	}
	//ִ��open���close������Ĳ���
	int index = OpenRemove(open);
	CloseAdd(close, index);
	//����Ƿ��������(��ɻ򳬳���󲽳�����)
	if (EndAll(tree.array[index]._data)) {
		printf("##########\nDepth First Searching Finished��\n");
		return index;
	}
	//��չopen��
	int deep = tree.array[index].deep + 1;
	if (deep < DEPTH) {
		OpenAdd(open, index, tree.array[index]._data, tree, deep);
	}
	//����ȶ�open������
	FirstDeepOpen(tree, open);
	DepthFirstSearching(tree, open, close);
}

//�������ͼ������-1��ʾfalse��������ʾsuccess��Ľ�βindex����
template<typename T>
int BreadthFirstSearching(Tree<T>& tree, int* open, int* close) {
	int sizeOpen = open[0];
	//����OPEN���Ƿ�Ϊ��
	if (sizeOpen == 0) {
		return -1;
	}
	//ִ��open���close������Ĳ���
	int index = OpenRemove(open);
	CloseAdd(close, index);
	//����Ƿ��������(��ɻ򳬳���󲽳�����)
	if (EndAll(tree.array[index]._data)) {
		printf("##########\nBreadth First Searching Finished��\n");
		return index;
	}
	//��չopen��
	int deep = tree.array[index].deep + 1;
	if (deep < DEPTH) {
		OpenAdd(open, index, tree.array[index]._data, tree, deep);
	}
	//����ȶ�open������
	FirstBreadthOpen(tree, open);
	BreadthFirstSearching(tree, open, close);
}

//����ʽͼ������-1��ʾfalse��������ʾsuccess��Ľ�βindex����
template<typename T>
int InspiredSearching(Tree<T>& tree, int* open, int* close) {
	int sizeOpen = open[0];
	//����OPEN���Ƿ�Ϊ��
	if (sizeOpen == 0) {
		return -1;
	}
	//ִ��open���close������Ĳ���
	int index = OpenRemove(open);
	CloseAdd(close, index);
	//����Ƿ��������(��ɻ򳬳���󲽳�����)
	if (EndAll(tree.array[index]._data)) {
		printf("##########\nInspired Searching Finished��\n");
		return index;
	}
	//��չopen��
	int deep = tree.array[index].deep + 1;
	if (deep < DEPTH) {
		OpenAdd(open, index, tree.array[index]._data, tree, deep);
	}
	//����ȶ�open������
	FirstInspiredOpen(tree, open);
	InspiredSearching(tree, open, close);
}

int main() {
	int** MAP;
	//��ʼ����ͼ
	MAP = NewMap();
	PrintMap(MAP);
	printf("����ʼ��ͼ��\n");

	//==========================================
	//��ͨͼ����
	//����Open���Close��
	int* open1, * close1;
	open1 = (int*)malloc(10000 * sizeof(int*));
	close1 = (int*)malloc(10000 * sizeof(int*));
	open1[0] = 1;
	open1[1] = 0;
	close1[0] = 0;
	//������
	Tree<int**> tree1;
	tree1.count = 0;
	int deep1 = 0;
	AddNode(MAP, -1, deep1, tree1);
	clock_t start1 = clock();
	int flag1 = GraphSearching(tree1, open1, close1);
	clock_t  end1 = clock();
	PrintALL(tree1, flag1, MAP);

	//==========================================
	//�������ͼ����
	//����Open���Close��
	int* open2, * close2;
	open2 = (int*)malloc(10000 * sizeof(int*));
	close2 = (int*)malloc(10000 * sizeof(int*));
	open2[0] = 1;
	open2[1] = 0;
	close2[0] = 0;
	//������
	Tree<int**> tree2;
	tree2.count = 0;
	int deep2 = 0;
	AddNode(MAP, -1, deep2, tree2);
	clock_t start2 = clock();
	int flag2 = DepthFirstSearching(tree2, open2, close2);
	clock_t  end2 = clock();
	PrintALL(tree2, flag2, MAP);

	//==========================================
	//�������ͼ����
	//����Open���Close��
	int* open3, * close3;
	open3 = (int*)malloc(10000 * sizeof(int*));
	close3 = (int*)malloc(10000 * sizeof(int*));
	open3[0] = 1;
	open3[1] = 0;
	close3[0] = 0;
	//������
	Tree<int**> tree3;
	tree3.count = 0;
	int deep3 = 0;
	AddNode(MAP, -1, deep3, tree3);
	clock_t start3 = clock();
	int flag3 = BreadthFirstSearching(tree3, open3, close3);
	clock_t  end3 = clock();
	PrintALL(tree3, flag3, MAP);

	//==========================================
	//����ʽͼ����
	//����Open���Close��
	int* open4, * close4;
	open4 = (int*)malloc(10000 * sizeof(int*));
	close4 = (int*)malloc(10000 * sizeof(int*));
	open4[0] = 1;
	open4[1] = 0;
	close4[0] = 0;
	//������
	Tree<int**> tree4;
	tree4.count = 0;
	int deep4 = 0;
	AddNode(MAP, -1, deep4, tree4, InspiredMap(MAP));
	clock_t start4 = clock();
	int flag4 = InspiredSearching(tree4, open4, close4);
	clock_t  end4 = clock();
	PrintALL(tree4, flag4, MAP);

	//���������Ƚ��㷨����
	cout << "��ͨ-> time:" << end1 - start1 << "\t������" << tree1.count << endl;
	cout << "���-> time:" << end2 - start2 << "\t������" << tree2.count << endl;
	cout << "���-> time:" << end3 - start3 << "\t������" << tree3.count << endl;
	cout << "����-> time:" << end4 - start4 << "\t������" << tree4.count << endl;

	return 0;
}