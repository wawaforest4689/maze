// 2024/11/25	利用并查集实现迷宫绘制
// 12x12迷宫，行列整数编码
//需要一个144的数组，存储set信息；以及一个大小为264的数组，用于记录迭代过程中格子的情况

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <Windows.h>
#include <cassert>
#include <easyx.h>
#include <conio.h>


#define length 20
#define size length*length
#define walls_amount 2*length*(length-1)
#define window_w 640
#define grid_w 30

using namespace std;

int find_root(int* array, int i);
// suboptimal method（没有进行树的高度的比较）
void set_union1(int* array, int* indexs,int i, int j);
void break_wall(int* array2, int* indexs, int s, int* coo); 
void check(int* array,int t,bool& f);
// use class and array to achieve,2024//11/25
// void set_union2(int i, int j);

int main()
{
	srand(static_cast<int>(time(0)));
	int* grids = new int[size]();
	int* walls = new int[walls_amount]();
	int* coordinates = new int[3]();
	//int grids2[400]={0};	
	for (int i = 0; i < size; i++)
	{
		grids[i] = i;
	}
	// 1 for existent,0 for empty
	for (int i = 0; i < walls_amount; i++)
	{
		walls[i] = 1;
	}
	//int l = walls_amount;
	bool flag = false;
	int* indexs2 = new int[walls_amount]();
	while (!flag)
	{
		//indexs2 = new int[l]();
		int count = 0;
		for (int i = 0; i < walls_amount; i++)
		{
			if (walls[i] == 1)
			{
				indexs2[count] = i;
				count++;
			}
		}
		cout << count << endl;
		//cout << "l==count: " << (l == count) <<" "<< l <<" "<< count << endl;
		break_wall(walls, indexs2, count, coordinates);
		int row = coordinates[0];
		int col = coordinates[1];
		if (coordinates[2] == 0)
		{
			set_union1(grids, walls,row * length + col, row * length + col + 1);
		}
		else {
			set_union1(grids, walls,row * length + col, (row+1) * length + col);
		}
		//Sleep(200);
		//delete[] indexs2;
		//Sleep(200);
		//l--;
		check(grids, size, flag);
	}
	delete[] indexs2;

	for (int j = 0; j < size; j++)
		cout << find_root(grids,j) << " ";
	
	cout << endl << endl;

	for (int i = 0; i < walls_amount/2; i++)
	{
		cout << walls[i] << " ";
	}
	cout << endl << endl;
	for (int i = walls_amount/2; i < walls_amount; i++)
	{
		cout << walls[i] << " ";
	}

	// 绘制迷宫
	initgraph(window_w, window_w);
	// 棕色边线
	setlinecolor(0x996600);
	setlinestyle(PS_SOLID, 4);
	line((window_w-length*grid_w)/2, (window_w - length * grid_w) / 2, 
		(window_w + length * grid_w) / 2,(window_w - length * grid_w) / 2);
	line((window_w - length * grid_w) / 2, (window_w - length * grid_w) / 2, 
		(window_w - length * grid_w) / 2, (window_w + length * grid_w) / 2);
	line((window_w + length * grid_w) / 2, (window_w - length * grid_w) / 2,
		(window_w + length * grid_w) / 2, (window_w + length * grid_w) / 2);
	line((window_w - length * grid_w) / 2, (window_w + length * grid_w) / 2,
		(window_w + length * grid_w) / 2, (window_w + length * grid_w) / 2);

	// 黄色内线
	setlinecolor(0xffff00);
	int pen1 = (window_w - length * grid_w) / 2, 
		pen2= (window_w - length * grid_w) / 2;
	// vertical inner lines
	for (int i = 0; i < walls_amount / 2; i++)
	{
		pen1 = max((pen1 + grid_w) % ((window_w + length * grid_w) / 2), 
			(window_w - length * grid_w) / 2+grid_w);
		if (i % (length - 1) == 0)
		{
			pen2 += grid_w;
		}
		if (walls[i] == 1)
		{
			line(pen1, pen2-grid_w, pen1, pen2);
		}
	}

	pen1 = (window_w - length * grid_w) / 2-grid_w;
	pen2 = (window_w - length * grid_w) / 2;
	// horizontal inner lines
	for (int i = walls_amount/2; i < walls_amount; i++)
	{
		pen1 = max((pen1 + grid_w) % ((window_w + length * grid_w) / 2), 
			(window_w - length * grid_w) / 2);
		if (i % length == 0)
		{
			pen2 += grid_w;
		}
		if (walls[i] == 1)
		{
			line(pen1, pen2, pen1+grid_w, pen2);
		}
	}

	while (1)
	{
		//char c = _getch();
		if (GetAsyncKeyState(VK_RETURN))
			break;
	}

	closegraph();

	return 0;
}








int find_root(int* ptr, int i)
{
	assert(i < size);
	while (ptr[i]!=i)
	{
		i = ptr[i];
	}
	//cout << "depth: " << depth << endl;
	//must be in the range 6*[0,...,11]+[0,1,2,3] in fact
	return i;
}


// combined with hashing
void set_union1(int* ptr,int* ptr2,int i, int j)
{
	int i_r = find_root(ptr, i);
	int j_r = find_root(ptr, j);
	cout << "i_r: " << i_r << " " << j_r << endl;
	if (i_r == j_r)
	{
		return;
	}
	if (j - i == 1)
	{
		ptr2[(length-1)*(i/length) + i%length] = 0;
	}
	else {
		ptr2[walls_amount / 2 + length * (i/length)+i%length] = 0;
	}
	ptr[j_r] = i_r;
}

void break_wall(int* a,int* i, int t,int* coord)
{
	int index = rand() % t;
	index =i[index];
	cout << "a[index]: " << a[index] << endl;
	cout << "index: " << index << endl;
	int row = 0, col = 0;
	if (index < walls_amount / 2)
	{
		col = index % (length - 1);
		row = index / (length-1);
	}
	else
	{
		col = (index- walls_amount / 2) % length;
		row = (index- walls_amount / 2) / length;
	}
	cout << "row: " << row << "  col: " << col << endl;
	//int row = (index >= grids_amount / 2 ? (index - grids_amount / 2) % length : index % (length - 1));
	//int col = (index >= grids_amount / 2 ? (index - grids_amount / 2) / (length-1) : index / length);
	coord[0] = row;
	coord[1] = col;
	coord[2] = (index >= walls_amount / 2);
}


void check(int* ptr,int t,bool& flag)
{
	for (int k = 0; k < t-1; k++)
	{
		int root1 = find_root(ptr, k);
		int root2 = find_root(ptr, k+1);
		if (root1 != root2)
		{
			flag = false;
			return;
		}
	}
	flag = true;

}


