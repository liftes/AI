#include<stdio.h>

//用于统计解的数目的全局变量
int count = 0;

//用于判断棋子位置是否合法
int isCorrect(int i, int j, int(*Q)[4])
{
	int s, t;
	for (s = i, t = 0; t < 4; t++)
		if (Q[s][t] == 1 && t != j)
			return 0;//判断行
	for (t = j, s = 0; s < 4; s++)
		if (Q[s][t] == 1 && s != i)
			return 0;//判断列
	for (s = i - 1, t = j - 1; s >= 0 && t >= 0; s--, t--)
		if (Q[s][t] == 1)
			return 0;//判断左上方
	for (s = i + 1, t = j + 1; s < 4 && t < 4; s++, t++)
		if (Q[s][t] == 1)
			return 0;//判断右下方
	for (s = i - 1, t = j + 1; s >= 0 && t < 4; s--, t++)
		if (Q[s][t] == 1)
			return 0;//判断右上方
	for (s = i + 1, t = j - 1; s < 4 && t >= 0; s++, t--)
		if (Q[s][t] == 1)
			return 0;//判断左下方

	return 1;//否则返回
}

//递归函数
void Queue(int j, int(*Q)[4])
{
	int i, k;
	if (j == 4) {//递归结束条件，即已经得到可行解，此时打印输出，否则跳过输出
		for (i = 0; i < 4; i++) {
			//得到一个解，在屏幕上显示
			for (k = 0; k < 4; k++)
				printf("%d ", Q[i][k]);
			printf("\n");
		}
		printf("\n");
		count++;
		return;
	}
	for (i = 0; i < 4; i++) {
		if (isCorrect(i, j, Q)) {//如果Q[i][j]可以放置皇后
			Q[i][j] = 1;//放置皇后
			Queue(j + 1, Q);//搜索下一层是否有可行解，如没有则返回上一层，如有则继续寻找，直到有输出被打印。
			Q[i][j] = 0;//当前位置不放置皇后，即回溯到上一层
		}
	}
}

int main()
{
	int Q[4][4];
	int i, j;
	//地图初始化
	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			Q[i][j] = 0;
	//开始递归
	Queue(0, Q);
	printf("可行解的数量为：%d\n", count);
	return 0;
}