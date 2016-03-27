// ================================================
//解数独
//作者：张楷 、陈晨钜
//开始时间：2016.3.8
//完成度：快速预测模式未完成、过程优化未完成、模式控制未完成
//=================================================
#include "stdafx.h"
#include<iostream>
#include<ctime>
#include<iomanip>
using namespace std;
//class
class sudokuitem {
public:
	int num = 0;
	int predict_num = 0;
	int possible_list[9] = {0};
	int possible_length = 0;
	int possible_seek = 0;//当前循环位置，暂未用到
	int position[2] = {0};//y,x
	int get_info()
	{
		int i = 0;
		cout << "元素信息";
		cout << "num:" << num << endl;
		cout << "predict_num:" << predict_num << endl;
		cout << "possible_list:";
		for ( i = 0; i < 9; i++)
		{
			cout << possible_list[i] << ",";
		}
		cout << endl;
		cout << "possible_length:" << possible_length << endl;
		cout << "possible_seek:" << possible_seek << endl;
		cout << "position:" << position[0]<<":"<<position[1] << endl;
		return 0;
	}


};
/*struct for_predict_conf{
public:
	int times = 0;
	int length = 0;
};
for_predict_conf predict_conf[81];*/
//初始化全局变量
//===============================================
//predict_length:predict数组的长度
//				 predict有2个元素predict_length=2
//for_predict_times:预测循环次数
//cut_times:修剪次数
//find_all:是否显示所有结果
//if_set_num：初始化过程中有无数字被确定
//if_nine:是否使用九宫格判断
//fast_judge：是否使用快速判断
//===============================================
sudokuitem sudoku[9][9], sudoku_line[9];
sudokuitem * predict[81];
int predict_length = 0 ;
long double for_predict_times = 0, cut_times=0;
bool find_all = false, if_nine = true, if_set_num=false,debug=false,fast_judge=false;
//debug 参数
int g_x=2, g_y=2;
//函数定义区
int get_sudoku();//获取输入的数组
bool sudoku_search_possible();//搜索每个数字可能的取值,并确定尽可能多的数字
bool if_same_num(int *list);//检查是否有重复数字,陈晨钜提供
bool if_same_num_v2(int *list);//检查是否有重复数字,自己写的
bool if_nine_number_rule();//九宫格规则
int get_nine_number(int x, int y, int * list);//取得某元素所在的九宫格数组
int get_line_num(int y, int *list);//获取这一列的数字(预测数)
int get_column_num(int x, int *list);//获取这一行的数字(预测数)
bool check_true();//检查是否完成
void get_possible_num(int *list, int *s);//根据行列不重复规则判断取值,陈晨钜提供
int get_possible_num_v2(int *list, int *s);//寻找缺失的数字,自己写的
void show_sudoku();//输出数组
void show_predcit_sudoku();//输出预测数组
void redefin_predict_num(int layers);//把该层之后的预测清空
void find_same_num(int *list, int*s, int *t);//从2个数组中找出相同的数字,陈晨钜提供
void find_same_num3(int *list, int*s, int *L, int *t);//从3个数组中找出相同的数字,陈晨钜提供
int get_list_length(int * list, int length, int end_item = 0);//查看数组长度
//int get_list_length(sudokuitem * list, int length, int end_item = 0);//函数重载
bool if_need_predict();//判断是否需要进入预测模式
int for_predict(int layers);//循环预测
long double diffcult(long double sum);//计算运算量
void test();//测试函数
//defunction

int main()
{
	int  nine_list[9] = { 0 }, nine[9] = { 0 };
	double percent,time;
	bool if_search = true;
	clock_t start, finish;
	get_sudoku();
	cout << "是否取得所有解："<< (find_all?"是":"否") << endl;
	cout << "检查模式：" << (fast_judge ? "快速检查" : "完全检查") << endl;
	cout << "初始化时是否使用九宫格规则：" << (if_nine ? "是" : "否") << endl;
	cout << "DeBug模式：" << (debug ? "开" : "关") << endl;
	start = clock();//开始时间
	if_search = true;
	//test();//测试函数
	if (debug)
	{
		sudoku[g_x][g_y].get_info();
	}
	if_set_num = true;
	while (if_set_num)
	{
		//sudokuitem * predict[81];//predict_length长度之后的元素会残留下来！！！！
		predict_length = 0;
		if_set_num = false;
		if_search = sudoku_search_possible();
	}
	cout << "初始化结束" << endl;
	show_predcit_sudoku();
	if (check_true() == false)
	{
		cout << "数独有错" << endl;
		finish = clock();
		time = finish - start;
		cout << "用时：" << (time / CLOCKS_PER_SEC) << " s " << endl;
		return 0;
	}
	if (if_need_predict())//判断函数是否需要进入预测模式。
	{
		cout << setiosflags(ios::fixed);//设置显示模式
		cout << "待预测数字:" << setprecision(0) << predict_length << endl;
		cout << "完全历遍所需预测次数:" << (diffcult(1)/10000000000000000.0000) << "亿亿次" << endl;
		for_predict(0);//此处初始化第0层函数
		finish = clock();
		cout << "预测修剪次数:" << cut_times << "次" << endl;
		percent = (for_predict_times / diffcult(1));
		cout << "总共预测:" << for_predict_times << "次" << endl;
		cout << "历遍比例" << setprecision(60) << (percent*100) << "%" << endl;
	}
	time = finish - start;
	cout << "用时："<< setprecision(6) << (time / CLOCKS_PER_SEC) << " s " << endl;
	system("pause");
	return 0;
}
int get_sudoku()
{
	int y = 0, x = 0, num = 0;
	sudokuitem sudoku_item;
	while (y<9)
	{
		x = 0;
		while (x<9)
		{
			cin >> num;
			//cout << " " << num;
			sudoku_item.num = num;//定义数
			sudoku_item.predict_num = num;//定义预测数
			sudoku_item.position[0] = y;//定义列
			sudoku_item.position[1] = x;//定义行
			sudoku[y][x] = sudoku_item;
			x++;
		}
		//cout << endl;
		y++;
	}
	return 0;
}
void get_possible_num(int *list, int *s)//寻找缺失的数字
{
	int i = 0, j, a = 1, count, len, m = 0;
	len = 9;
	for (a = 1; a <= len; a++)
	{
		j = 0, count = 0;
		while (j < len)
		{
			if (a != list[j])
				count++;
			if (count == 9)
			{
				s[i] = a;
				//cout << a << endl;
				i++;
			}
			j++;
		}
	}
}
int get_possible_num_v2(int * list, int * result)
{
	int i = 0 , n = 1, count=0;
	bool lost_num[10] = { true,true,true,true,true,true,true,true,true,true };
	for ( i = 0; i < 9; i++)//i : 0-8
	{
		lost_num[list[i]] = false;
	}
	for (n = 1; n < 10; n++)//n : 1-9
	{
		if (lost_num[n])
		{
			result[count] = n;
			count++;
		}
	}
	return 0;
}
bool if_same_num(int *list)
{
	int i, count, j = 1;
	while (j <= 9)
	{
		count = 0, i = 0;
		for (i = 0; i < 9; i++)
		{
			if (list[i] - j == 0)
			{
				count++;
			}
			if (count == 2)
			{
				return true;
			}
		}
		j++;
	}
	return false;/*0代表无重复，1代表重复*/
}
bool if_same_num_v2(int *list)
{
	int i = 0, n = 1, count = 0;
	int lost_num[10] = { 0,0,0,0,0,0,0,0,0,0 };
	for (i = 0; i < 9; i++)//i : 0-8
	{
		lost_num[list[i]]++;
	}
	for (n = 1; n < 10; n++)//n : 1-9
	{
		if (lost_num[n] > 1)
		{
			//cout << n << endl;
			//cout << lost_num[n] << endl;
			return true;
		}
	}
	return false;
}
bool check_true()
{
	int y = 0, x = 0, x_list[9] = { 0 }, y_list[9] = { 0 };
	for ( y = 0; y < 9; y++)
	{
		get_line_num(y, x_list);
		if (if_same_num(x_list))
		{
			//cout << "有行错解~！" << endl;
			return false;
		}
	}
	for ( x = 0; x < 9; x++)
	{
		get_column_num(x, y_list);
		if (if_same_num(y_list))
		{
			//cout << "有列错解~！" << endl;
			return false;
		}
	}
	if (if_nine_number_rule() == false)
	{
		return false;
	}
	return true;
}
bool if_nine_number_rule()
{
	int list[9] = { 0 },y=0,i = 0,n = 0,a=0;
	for ( n = 0; n < 9;  n = n + 3)
	{
		for (i = 0; i < 9; i = i + 3)
		{
			list[0] = sudoku[(n + 0)][i].predict_num;
			list[1] = sudoku[(n + 0)][(i + 1)].predict_num;
			list[2] = sudoku[(n + 0)][(i + 2)].predict_num;
			list[3] = sudoku[(n + 1)][(i + 0)].predict_num;
			list[4] = sudoku[(n + 1)][(i + 1)].predict_num;
			list[5] = sudoku[(n + 1)][(i + 2)].predict_num;
			list[6] = sudoku[(n + 2)][(i + 0)].predict_num;
			list[7] = sudoku[(n + 2)][(i + 1)].predict_num;
			list[8] = sudoku[(n + 2)][(i + 2)].predict_num;
			if (if_same_num(list))
			{
				/*
				for (a = 0; a < 9; a++)
				{
					cout << list[a] << endl;
				}
				cout << n << ":" << i << endl;*/
				return false;
			}
		}
	}
	return true;
}
int get_nine_number(int y, int x,int * list)
{
	if (x < 3) 
	{
		x = 0;
	}
	else if (2< x&&x < 6)
	{
		x = 3;
	}
	else if(5< x&&x < 9)
	{
		x = 6;
	}
	else
	{
		return 0;
	}
	if (y < 3)
	{
		y = 0;
	}
	else if (2< y&&y < 6)
	{
		y = 3;
	}
	else if (5< y&&y < 9)
	{
		y = 6;
	}
	else
	{
		return 0;
	}
	list[0] = sudoku[(y + 0)][x].num;
	list[1] = sudoku[(y + 0)][(x + 1)].num;
	list[2] = sudoku[(y + 0)][(x + 2)].num;
	list[3] = sudoku[(y + 1)][(x + 0)].num;
	list[4] = sudoku[(y + 1)][(x + 1)].num;
	list[5] = sudoku[(y + 1)][(x + 2)].num;
	list[6] = sudoku[(y + 2)][(x + 0)].num;
	list[7] = sudoku[(y + 2)][(x + 1)].num;
	list[8] = sudoku[(y + 2)][(x + 2)].num;
	return 0;
}
int get_line_num(int y, int *list)
{
	int i=0;
	for (i = 0; i < 9; i++)
	{
		list[i] = sudoku[y][i].predict_num;
	}
	return 0;
}
int get_column_num(int x, int *list)
{
	int i = 0;
	for (i = 0; i < 9; i++)
	{
		list[i] = sudoku[i][x].predict_num;
	}
	return 0;
}
void show_sudoku()
{
	int x = 0, y = 0;
	for (y = 0; y < 9; y++)
	{
		for (x = 0; x < 9; x++)
		{
			cout << " " << sudoku[y][x].num;
		}
		cout << endl;
	}

}
void show_predcit_sudoku()
{
	int x = 0, y = 0;
	for (y = 0; y < 9; y++)
	{
		for (x = 0; x < 9; x++)
		{
			if (sudoku[y][x].num != 0)
			{
				cout << " " << sudoku[y][x].num;
			}
			else
			{
				cout << " " << sudoku[y][x].predict_num;
			}
		}
		cout << endl;
	}
}
void redefin_predict_num(int layers)
{
	int i = 0;
	for (i = layers; i < predict_length; i++)
	{
		(*predict[i]).predict_num = (*predict[i]).num;
	}
	
}
void find_same_num(int *list, int*s, int *t)
{
	int a = 1, len = 9, i = 0, count, j = 0;
	for (a = 1; a <= len; a++)
	{
		count = 0, i = 0;
		while (i < len)
		{
			if (a == list[i] && a == s[i])
			{
				count = count + 2;
			}
			else if (a == list[i] || a == s[i])
			{
				count++;
			}
			if (count == 2)
			{
				count = 0;
				t[j] = a;
				j++;
			}
			i++;
		}
	}
}
int get_list_length(int * list, int length,int end_item)
{
	int i=0;
	for (i = 0; i < length; i++)
	{
		if (list[i] == end_item)
		{
			return i;
		}
	}
	return length;
}

bool if_need_predict()
{
	int x = 0, y = 0;
	for (y = 0; y < 9; y++)
	{
		for (x = 0; x < 9; x++)
		{
			if (sudoku[y][x].num == 0)//判断是否需要寻找 && sudoku[y][x].possible_list[0] == 0
			{
				return true;
			}
		}
	}
	return false;
}

int for_predict(int layers)
{
	int length = 0, i = 0,  x = -1, y = -1, x_list[9] = { 0 }, y_list[9] = { 0 }, line[9] = { 0 }, column[9] = { 0 }, nine[9] = { 0 };//
	long int times = 0;
	//int *line = new int[9];
	//int *column = new int[9];
	if (i == 0)//第一次循环时对length初始化
	{
		length = (*predict[layers]).possible_length;
		y = (*predict[layers]).position[0];
		x = (*predict[layers]).position[1];
		//cout << "第" << layers << "次初始化预测|循环次数:" << length << endl;
	}
	for (i = 0; i < length; i++)
	{
		//memset(line, 0, 9 * sizeof(int));
		//memset(column, 0, 9 * sizeof(int));
		(*predict[layers]).predict_num = (*predict[layers]).possible_list[i];
		redefin_predict_num((layers + 1));
		//===============================================
		//快速判断
		if (fast_judge)
		{
			get_column_num(y, column);
			if (if_same_num_v2(column))
			{
				cut_times++;
				continue;
			}
			get_line_num(x, line);
			if (if_same_num_v2(line))
			{
				cut_times++;
				continue;
			}
			get_nine_number(x,y, nine);
			if (if_same_num_v2(nine))
			{
				cut_times++;
				continue;
			}
		}
		else
		{
			if (check_true() == false)
			{
				cut_times++;
				continue;
			}
		}
		//===============================================
		if (layers < (predict_length - 1))//判断是否需要继续嵌套，若一层，此时predict_length=1，layers=0
		{
			//for_predict((layers+1));//继续嵌套
			if (for_predict((layers + 1)) && find_all == false)//是否要继续寻找,找到解后for_predict返回1（true）
			{
				//delete[] line, column;
				return 1;
			}
		}
		else if(check_true())
		{
			//cout << "第" << for_predict_times++ << "次循环" << endl;
			show_predcit_sudoku();
			cout << "========================================" << endl;
			//delete[] line, column;
			return 1;//找到函数返回1
		}
		for_predict_times++;
	}
	//delete[] line, column;
	//cout << "第" << layers << "次循环|循环次数:" << i << endl;
	return 0;
}
long double diffcult(long double sum)
{
	long double total=1;
	int i = 0 ,a = 0;
	a = (predict_length - 1);
	for (i = 0; i < a; i++)
	{
		total = ((*predict[i]).possible_length * total) / sum;
	}
	return total;
}
void test()
{
	int x_list[9] = { 0 }, x = 3, i;
	show_predcit_sudoku();
	cout << check_true() << endl;
	show_sudoku();
	get_column_num(7, x_list);
	for (i = 0; i < 9; i++)
	{
		cout << x_list[i] << endl;
	}
	cout << if_same_num(x_list);
	if (true)
	{
		cout << (x + 1) << "列" << x << "行" << "有行重复数字，跳过第" << for_predict_times << "次循环" << endl;
		system("pause");
	}
}
void find_same_num3(int *list, int*s, int *L, int *t)
{
	int a = 1, len = 9, i = 0, count, j = 0;
	int temp;
	for (a = 1; a <= len; a++)
	{
		count = 0, i = 0;
		while (i < len)
		{
			if (a == list[i] && a == s[i] && a == L[i])
			{
				count = count + 3;
			}
			else if (a == list[i] && a == s[i] && a != L[i])
			{
				count = count + 2;
			}
			else if (a == list[i] && a == L[i] && a != s[i])
			{
				count = count + 2;
			}
			else if (a == L[i] && a == s[i] && a != list[i])
			{
				count = count + 2;
			}
			else if (a == list[i] && a != s[i] && a != L[i])
			{
				count++;
			}
			else if (a == L[i] && a != s[i] && a != list[i])
			{
				count++;
			}
			else if (a == s[i] && a != list[i] && a != L[i])
			{
				count++;
			}

			if (count == 3)
			{
				count = 0;
				t[j] = a;
				j++;
			}
			i++;
		}
	}
	for (i = 0; i < len; i++)
		for (j = 8; j>0 + i; j--)
		{
			if (t[j] > t[j - 1])
			{
				temp = t[j - 1];
				t[j - 1] = t[j];
				t[j] = temp;
			}
		}

}
bool sudoku_search_possible()
{
	//=======================================
	//line:每一行的数
	//column:每一列的数
	//line_list:每一行缺的数
	//column_list:每一列缺的数
	//combine_list:十字线上所缺的数字
	//=======================================
	int y = 0, x = 0, item_y = 0, item_x = 0, for_count = 0, test = 0;
	//int nine[9] = { 0 }, nine_list[9] = { 0 };
	//int line[9] = { 0 }, column[9] = { 0 }, line_list[9] = { 0 }, column_list[9] = { 0 }, combine_list[9] = { 0 };
	bool if_search = false;
	//历遍元素
	//创建变量
	int *line = new int[9];// *column, *nine, *line_list, *column_list, *nine_list, *combine_list;
	int *column = new int[9];
	int *nine = new int[9];
	int *line_list = new int[9];
	int *column_list = new int[9];
	int *nine_list = new int[9];
	int *combine_list = new int[9];
	if_search = false;
	for (y = 0; y < 9; y++)
	{

		for (x = 0; x < 9; x++)
		{
			//初始化变量
			memset(line, 0, 9 * sizeof(int));
			memset(column, 0, 9 * sizeof(int));
			memset(nine, 0, 9 * sizeof(int));
			memset(line_list, 0, 9 * sizeof(int));
			memset(column_list, 0, 9 * sizeof(int));
			memset(nine_list, 0, 9 * sizeof(int));
			memset(combine_list, 0, 9 * sizeof(int));
			//初始化变量
			item_y = 0;
			item_x = 0;
			if (sudoku[y][x].num == 0)//判断是否需要寻找 && sudoku[y][x].possible_list[0] == 0 && sudoku[y][x].possible_length == 0
			{
				if_search = true;
				//===============================
				//判断这一行所缺的数字
				//===============================
				for (item_x = 0; item_x < 9; item_x++)
				{
					line[item_x] = sudoku[y][item_x].num;
					//cout << line[item_x] << endl;
				}
				get_possible_num_v2(line, line_list);//获取可能数字
				if (line_list[1] == 0 && line_list[0] != 0)//取得行可能性列表，判断是否只有一个值
				{
					sudoku[y][x].num = line_list[0];
					sudoku[y][x].predict_num = line_list[0];
					if_set_num = true;
					continue;
				}
				//===============================
				//判断这一列所缺的数字
				//===============================
				for (item_y = 0; item_y < 9; item_y++)
				{
					column[item_y] = sudoku[item_y][x].num;
				}
				get_possible_num_v2(column, column_list);//获取可能数字
				if (column_list[1] == 0 && column_list[0] != 0)//取得列可能性列表，判断是否只有一个值
				{
					sudoku[y][x].num = column_list[0];
					sudoku[y][x].predict_num = column_list[0];
					if_set_num = true;
					continue;
				}

				//===============================
				//判断九宫格所缺的数字,加入后速度大幅下降，且难以找到解，原因不明，放弃该部分
				//===============================
				if (if_nine)
				{
					get_nine_number(y, x, nine);
					get_possible_num_v2(nine, nine_list);
					if (nine_list[1] == 0 && nine_list[0] != 0)//取得九宫格可能性列表，判断是否只有一个值
					{
						sudoku[y][x].num = nine_list[0];
						sudoku[y][x].predict_num = nine_list[0];
						if_set_num = true;
						continue;
					}
					//
					//=========================================
					//判断每种数字剩余的个数
					//如果与剩余可能性数目相同，则确定该数字
					//=========================================temp
					find_same_num3(column_list, line_list, nine_list, combine_list);
				}
				else
				{
					find_same_num(column_list, line_list, combine_list);
				}
				if (debug)
				{
					if (x == g_x && y == g_y)
					{
						for (test = 0; test < 9; test++)
						{
							cout << column[test] << endl;
						}
						cout << "================" << endl;
						for (test = 0; test < 9; test++)
						{
							cout << column_list[test] << endl;
						}
						cout << "================" << endl;
					}
				}
					//cout << combine_list[0];
					if (combine_list[1] == 0 && combine_list[0] != 0)//取得总的可能性列表，判断是否只有一个值
					{
						sudoku[y][x].num = combine_list[0];
						sudoku[y][x].predict_num = combine_list[0];
						if_set_num = true;
						continue;
					}
					else// if (combine_list[1] != 0)//不是则对possible_list循环赋值
					{
						for_count = 0;
						sudoku[y][x].possible_length = get_list_length(combine_list, 9, 0);//设置列表，并设置长度
						predict[predict_length] = &sudoku[y][x];
						predict_length++;
						for (for_count = 0; for_count < 9; for_count++)
						{
							sudoku[y][x].possible_list[for_count] = combine_list[for_count];
							//cout << combine_list[for_count] << endl;
						//===========================
						//或可用数组指针直接指向返回的数组↑
						//===========================
						}
					}
				}
		}
	}
	//删除变量
	delete[] line, column, nine, line_list, column_list, nine_list, combine_list;
		return if_search;
}