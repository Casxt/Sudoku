// ================================================
//������
//���ߣ��ſ� ���³���
//��ʼʱ�䣺2016.3.8
//��ɶȣ�����Ԥ��ģʽδ��ɡ������Ż�δ��ɡ�ģʽ����δ���
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
	int possible_seek = 0;//��ǰѭ��λ�ã���δ�õ�
	int position[2] = {0};//y,x
	int get_info()
	{
		int i = 0;
		cout << "Ԫ����Ϣ";
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
//��ʼ��ȫ�ֱ���
//===============================================
//predict_length:predict����ĳ���
//				 predict��2��Ԫ��predict_length=2
//for_predict_times:Ԥ��ѭ������
//cut_times:�޼�����
//find_all:�Ƿ���ʾ���н��
//if_set_num����ʼ���������������ֱ�ȷ��
//if_nine:�Ƿ�ʹ�þŹ����ж�
//fast_judge���Ƿ�ʹ�ÿ����ж�
//===============================================
sudokuitem sudoku[9][9], sudoku_line[9];
sudokuitem * predict[81];
int predict_length = 0 ;
long double for_predict_times = 0, cut_times=0;
bool find_all = false, if_nine = true, if_set_num=false,debug=false,fast_judge=false;
//debug ����
int g_x=2, g_y=2;
//����������
int get_sudoku();//��ȡ���������
bool sudoku_search_possible();//����ÿ�����ֿ��ܵ�ȡֵ,��ȷ�������ܶ������
bool if_same_num(int *list);//����Ƿ����ظ�����,�³����ṩ
bool if_same_num_v2(int *list);//����Ƿ����ظ�����,�Լ�д��
bool if_nine_number_rule();//�Ź������
int get_nine_number(int x, int y, int * list);//ȡ��ĳԪ�����ڵľŹ�������
int get_line_num(int y, int *list);//��ȡ��һ�е�����(Ԥ����)
int get_column_num(int x, int *list);//��ȡ��һ�е�����(Ԥ����)
bool check_true();//����Ƿ����
void get_possible_num(int *list, int *s);//�������в��ظ������ж�ȡֵ,�³����ṩ
int get_possible_num_v2(int *list, int *s);//Ѱ��ȱʧ������,�Լ�д��
void show_sudoku();//�������
void show_predcit_sudoku();//���Ԥ������
void redefin_predict_num(int layers);//�Ѹò�֮���Ԥ�����
void find_same_num(int *list, int*s, int *t);//��2���������ҳ���ͬ������,�³����ṩ
void find_same_num3(int *list, int*s, int *L, int *t);//��3���������ҳ���ͬ������,�³����ṩ
int get_list_length(int * list, int length, int end_item = 0);//�鿴���鳤��
//int get_list_length(sudokuitem * list, int length, int end_item = 0);//��������
bool if_need_predict();//�ж��Ƿ���Ҫ����Ԥ��ģʽ
int for_predict(int layers);//ѭ��Ԥ��
long double diffcult(long double sum);//����������
void test();//���Ժ���
//defunction

int main()
{
	int  nine_list[9] = { 0 }, nine[9] = { 0 };
	double percent,time;
	bool if_search = true;
	clock_t start, finish;
	get_sudoku();
	cout << "�Ƿ�ȡ�����н⣺"<< (find_all?"��":"��") << endl;
	cout << "���ģʽ��" << (fast_judge ? "���ټ��" : "��ȫ���") << endl;
	cout << "��ʼ��ʱ�Ƿ�ʹ�þŹ������" << (if_nine ? "��" : "��") << endl;
	cout << "DeBugģʽ��" << (debug ? "��" : "��") << endl;
	start = clock();//��ʼʱ��
	if_search = true;
	//test();//���Ժ���
	if (debug)
	{
		sudoku[g_x][g_y].get_info();
	}
	if_set_num = true;
	while (if_set_num)
	{
		//sudokuitem * predict[81];//predict_length����֮���Ԫ�ػ����������������
		predict_length = 0;
		if_set_num = false;
		if_search = sudoku_search_possible();
	}
	cout << "��ʼ������" << endl;
	show_predcit_sudoku();
	if (check_true() == false)
	{
		cout << "�����д�" << endl;
		finish = clock();
		time = finish - start;
		cout << "��ʱ��" << (time / CLOCKS_PER_SEC) << " s " << endl;
		return 0;
	}
	if (if_need_predict())//�жϺ����Ƿ���Ҫ����Ԥ��ģʽ��
	{
		cout << setiosflags(ios::fixed);//������ʾģʽ
		cout << "��Ԥ������:" << setprecision(0) << predict_length << endl;
		cout << "��ȫ��������Ԥ�����:" << (diffcult(1)/10000000000000000.0000) << "���ڴ�" << endl;
		for_predict(0);//�˴���ʼ����0�㺯��
		finish = clock();
		cout << "Ԥ���޼�����:" << cut_times << "��" << endl;
		percent = (for_predict_times / diffcult(1));
		cout << "�ܹ�Ԥ��:" << for_predict_times << "��" << endl;
		cout << "�������" << setprecision(60) << (percent*100) << "%" << endl;
	}
	time = finish - start;
	cout << "��ʱ��"<< setprecision(6) << (time / CLOCKS_PER_SEC) << " s " << endl;
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
			sudoku_item.num = num;//������
			sudoku_item.predict_num = num;//����Ԥ����
			sudoku_item.position[0] = y;//������
			sudoku_item.position[1] = x;//������
			sudoku[y][x] = sudoku_item;
			x++;
		}
		//cout << endl;
		y++;
	}
	return 0;
}
void get_possible_num(int *list, int *s)//Ѱ��ȱʧ������
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
	return false;/*0�������ظ���1�����ظ�*/
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
			//cout << "���д��~��" << endl;
			return false;
		}
	}
	for ( x = 0; x < 9; x++)
	{
		get_column_num(x, y_list);
		if (if_same_num(y_list))
		{
			//cout << "���д��~��" << endl;
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
			if (sudoku[y][x].num == 0)//�ж��Ƿ���ҪѰ�� && sudoku[y][x].possible_list[0] == 0
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
	if (i == 0)//��һ��ѭ��ʱ��length��ʼ��
	{
		length = (*predict[layers]).possible_length;
		y = (*predict[layers]).position[0];
		x = (*predict[layers]).position[1];
		//cout << "��" << layers << "�γ�ʼ��Ԥ��|ѭ������:" << length << endl;
	}
	for (i = 0; i < length; i++)
	{
		//memset(line, 0, 9 * sizeof(int));
		//memset(column, 0, 9 * sizeof(int));
		(*predict[layers]).predict_num = (*predict[layers]).possible_list[i];
		redefin_predict_num((layers + 1));
		//===============================================
		//�����ж�
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
		if (layers < (predict_length - 1))//�ж��Ƿ���Ҫ����Ƕ�ף���һ�㣬��ʱpredict_length=1��layers=0
		{
			//for_predict((layers+1));//����Ƕ��
			if (for_predict((layers + 1)) && find_all == false)//�Ƿ�Ҫ����Ѱ��,�ҵ����for_predict����1��true��
			{
				//delete[] line, column;
				return 1;
			}
		}
		else if(check_true())
		{
			//cout << "��" << for_predict_times++ << "��ѭ��" << endl;
			show_predcit_sudoku();
			cout << "========================================" << endl;
			//delete[] line, column;
			return 1;//�ҵ���������1
		}
		for_predict_times++;
	}
	//delete[] line, column;
	//cout << "��" << layers << "��ѭ��|ѭ������:" << i << endl;
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
		cout << (x + 1) << "��" << x << "��" << "�����ظ����֣�������" << for_predict_times << "��ѭ��" << endl;
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
	//line:ÿһ�е���
	//column:ÿһ�е���
	//line_list:ÿһ��ȱ����
	//column_list:ÿһ��ȱ����
	//combine_list:ʮ��������ȱ������
	//=======================================
	int y = 0, x = 0, item_y = 0, item_x = 0, for_count = 0, test = 0;
	//int nine[9] = { 0 }, nine_list[9] = { 0 };
	//int line[9] = { 0 }, column[9] = { 0 }, line_list[9] = { 0 }, column_list[9] = { 0 }, combine_list[9] = { 0 };
	bool if_search = false;
	//����Ԫ��
	//��������
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
			//��ʼ������
			memset(line, 0, 9 * sizeof(int));
			memset(column, 0, 9 * sizeof(int));
			memset(nine, 0, 9 * sizeof(int));
			memset(line_list, 0, 9 * sizeof(int));
			memset(column_list, 0, 9 * sizeof(int));
			memset(nine_list, 0, 9 * sizeof(int));
			memset(combine_list, 0, 9 * sizeof(int));
			//��ʼ������
			item_y = 0;
			item_x = 0;
			if (sudoku[y][x].num == 0)//�ж��Ƿ���ҪѰ�� && sudoku[y][x].possible_list[0] == 0 && sudoku[y][x].possible_length == 0
			{
				if_search = true;
				//===============================
				//�ж���һ����ȱ������
				//===============================
				for (item_x = 0; item_x < 9; item_x++)
				{
					line[item_x] = sudoku[y][item_x].num;
					//cout << line[item_x] << endl;
				}
				get_possible_num_v2(line, line_list);//��ȡ��������
				if (line_list[1] == 0 && line_list[0] != 0)//ȡ���п������б��ж��Ƿ�ֻ��һ��ֵ
				{
					sudoku[y][x].num = line_list[0];
					sudoku[y][x].predict_num = line_list[0];
					if_set_num = true;
					continue;
				}
				//===============================
				//�ж���һ����ȱ������
				//===============================
				for (item_y = 0; item_y < 9; item_y++)
				{
					column[item_y] = sudoku[item_y][x].num;
				}
				get_possible_num_v2(column, column_list);//��ȡ��������
				if (column_list[1] == 0 && column_list[0] != 0)//ȡ���п������б��ж��Ƿ�ֻ��һ��ֵ
				{
					sudoku[y][x].num = column_list[0];
					sudoku[y][x].predict_num = column_list[0];
					if_set_num = true;
					continue;
				}

				//===============================
				//�жϾŹ�����ȱ������,������ٶȴ���½����������ҵ��⣬ԭ�����������ò���
				//===============================
				if (if_nine)
				{
					get_nine_number(y, x, nine);
					get_possible_num_v2(nine, nine_list);
					if (nine_list[1] == 0 && nine_list[0] != 0)//ȡ�þŹ���������б��ж��Ƿ�ֻ��һ��ֵ
					{
						sudoku[y][x].num = nine_list[0];
						sudoku[y][x].predict_num = nine_list[0];
						if_set_num = true;
						continue;
					}
					//
					//=========================================
					//�ж�ÿ������ʣ��ĸ���
					//�����ʣ���������Ŀ��ͬ����ȷ��������
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
					if (combine_list[1] == 0 && combine_list[0] != 0)//ȡ���ܵĿ������б��ж��Ƿ�ֻ��һ��ֵ
					{
						sudoku[y][x].num = combine_list[0];
						sudoku[y][x].predict_num = combine_list[0];
						if_set_num = true;
						continue;
					}
					else// if (combine_list[1] != 0)//�������possible_listѭ����ֵ
					{
						for_count = 0;
						sudoku[y][x].possible_length = get_list_length(combine_list, 9, 0);//�����б������ó���
						predict[predict_length] = &sudoku[y][x];
						predict_length++;
						for (for_count = 0; for_count < 9; for_count++)
						{
							sudoku[y][x].possible_list[for_count] = combine_list[for_count];
							//cout << combine_list[for_count] << endl;
						//===========================
						//���������ָ��ֱ��ָ�򷵻ص������
						//===========================
						}
					}
				}
		}
	}
	//ɾ������
	delete[] line, column, nine, line_list, column_list, nine_list, combine_list;
		return if_search;
}