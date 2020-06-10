// BPP_SA.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "stdafx.h"

//装箱问题相关参数
#define ItemSum 500  //物品总量
#define BININFO1 300 //箱子限制条件1
#define BININFO2 200 //箱子限制条件2
#define INTXT "BP_SA_2.txt" //读入数据文件名称

//SA 相关参数
#define k  0.01  
#define r  0.97 //降速快慢
#define T  50 //初始温度
#define T_min  0.1  //温度下限 停止搜索
#define Iteration 10000 //迭代次数


using namespace std;


//function declaration


//struct
typedef struct{
    int ItemNo;
    int Item_info1;
    int Item_info2;
}Item;

typedef struct {
	int Bin_info1;
	int Bin_info2;
	vector<Item> IteminBin;
}Bin;


void GetBPInfo(string FileName, vector<int>& Item_Index, vector<int>& Item_Info1, vector<int>& Item_Info2);
void Init_Solution(Item* bpitem, vector<int> index, vector<int> info1, vector<int> info2);
void swap(int* a, int pa, int pb);
void swapItem(Item* bpitem, int pa, int pb);
void New_Solution(int swapTimes, Item* bpitem);
void InitBin(Bin bpbin);
int FF(Item* bpitem);
double random(double dblow, double dbhigh);
void SimulatedFire(void);


int main()
{
	cout << "SA to solve BPP" << endl;

	srand((unsigned)(time(NULL)));

	clock_t begin, end;
	double totol_time;
	begin = clock();

	SimulatedFire();

	end = clock();

	totol_time = (double)(end - begin) / CLOCKS_PER_SEC;

	//输出相关参数
	cout << "Iteration\t" << Iteration << endl;
	cout << "T\t\t" << T << endl;
	cout << "T_min\t\t" << T_min << endl;
	cout << "r\t\t" << r << endl;
	cout << "k\t\t" << k << endl;
	cout << endl;
	cout << "Running time " << totol_time << " s"<<endl;
	return 0;
}

//function

//读取文件信息
void GetBPInfo(string FileName, vector<int>& Item_Index, vector<int>& Item_Info1, vector<int>& Item_Info2)
{
	Item_Index.clear();
	Item_Info1.clear();
	Item_Info2.clear();
	ifstream ifile;
	ifile.open(FileName);
	while (true)
	{
		int Item, Info1, Info2;
		ifile >> Item >> Info1 >> Info2;
		if (ifile.peek() == EOF) break;
		Item_Index.push_back(Item);
		Item_Info1.push_back(Info1);
		Item_Info2.push_back(Info2);
	}
	ifile.close();
}

//Init Solution
void Init_Solution(Item* bpitem, vector<int> index, vector<int> info1, vector<int> info2)
{
	for (int i = 0; i < ItemSum; i++)
	{
		bpitem[i].ItemNo = index[i];
		bpitem[i].Item_info1 = info1[i];
		bpitem[i].Item_info2 = info2[i];
	}
}

void swap(int* a, int pa, int pb)
{
	int temp = a[pa];
	a[pa] = a[pb];
	a[pb] = temp;
}

void swapItem(Item* bpitem, int pa, int pb)
{
	swap(&bpitem->ItemNo, pa, pb);
	swap(&bpitem->Item_info1, pa, pb);
	swap(&bpitem->Item_info2, pa, pb);
}

void New_Solution(int swapTimes, Item* bpitem)
{
	for (int i = 0; i < swapTimes; i++)
	{
		int posa = rand() % ItemSum;
		int posb = rand() % ItemSum;

		if (posa == posb) break;
		else
		{
			swapItem(bpitem, posa, posb);
		}
	}
}

void InitBin(Bin bpbin)
{
	bpbin.Bin_info1 = 0;
	bpbin.Bin_info2 = 0;
	bpbin.IteminBin.clear();
}


int FF(Item* bpitem)
{
	Bin bpbin[200];


	int binNum = 0;//使用箱子总数量 < BinSum

	for (int i = 0; i < ItemSum; i++)
	{
		int flag = 0;//装入物品是否需要开新的箱子(0为需要开箱)

		for (int j = 0; j < binNum; j++)
		{
			if (bpitem[i].Item_info1 + bpbin[j].Bin_info1 <= BININFO1)
			{
				if (bpitem[i].Item_info2 + bpbin[j].Bin_info2 <= BININFO2)
				{
					bpbin[j].IteminBin.push_back(bpitem[i]);
					bpbin[j].Bin_info1 += bpitem[i].Item_info1;
					bpbin[j].Bin_info2 += bpitem[i].Item_info2;
					flag = 1;
					break;
				}
			}
		}
		if (flag == 0)//已有箱子无法装下该物品，需要开箱
		{
			bpbin[binNum].IteminBin.push_back(bpitem[i]);
			bpbin[binNum].Bin_info1 = bpitem[i].Item_info1;
			bpbin[binNum].Bin_info2 = bpitem[i].Item_info2;
			binNum++;
		}
	}
	return binNum ;
}


double random(double dblow,double dbhigh)//产生dblow-dbhigh之间随机数
{
	double dbtemp = rand() / ((double)RAND_MAX + 1.0);
	return dblow + dbtemp * (dbhigh - dblow);
}

void SimulatedFire(void)
{
	Item bppitem[ItemSum];

	vector<int> item_index;
	vector<int> item_info1;
	vector<int> item_info2;
	item_index.clear();
	item_info1.clear();
	item_info2.clear();
	GetBPInfo(INTXT, item_index, item_info1, item_info2);

	Init_Solution(bppitem, item_index, item_info1, item_info2);
	int best = 0;
	best = FF(bppitem);//将初始解所需箱子数作为初始最优值
	int current;
	int dE;

	double current_T = T;

	while (current_T>T_min)
	{
		for (int i = 0; i < Iteration; i++)
		{
			New_Solution(20,bppitem);
			current = FF(bppitem);
			dE = current - best;

			if (dE < 0)
				best = current; //移动后解更优
			else
			{
				if (exp(-dE / (current_T * k)) > random(0.0, 1.0))
				{
					best = current;
				}
			}
		}
		current_T = r * current_T;
	}

	cout << "Bin_min " << best << endl;
}





// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
