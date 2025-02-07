// BP-Heu.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
using namespace std;

struct Pair
{
	int dim;
	int value;
};

struct PairGreater {
	bool operator () (Pair a, Pair b)
	{
		return a.value > b.value;
	}
};

//产生装箱问题的算例
void GenerateItemWeight(string FileName, int nItem)
{
	vector<int> Weight(nItem);
	for (int i = 0; i < nItem; i++) //物品重量在10~110之间
		Weight[i] = 10 + rand() % 100;

	ofstream ofile; //保存到文件中：物品编号 重量
	ofile.open(FileName);
	for (int i = 0; i < nItem; i++)
	{
		ofile << i << "\t" << Weight[i] << "\t";
		if ((i + 1) % 10 == 0)
			ofile << endl;
	}
	ofile.close();
}

//读取算例信息
void GetBPInfo(string FileName, vector<int> &Item_Index, vector<int> &Item_Weight)
{
	Item_Index.clear();
	Item_Weight.clear();
	ifstream ifile;
	ifile.open(FileName);
	while (true)
	{
		int Item, Weight;
		ifile >> Item >> Weight;
		if (ifile.peek() == EOF) break;
		Item_Index.push_back(Item);
		Item_Weight.push_back(Weight);
	}
	ifile.close();
}

int Heu(int Volume = 500)
{
	//vector<int> Item_Index;
	//vector<int> Item_Weight;
	//GetBPInfo("BP_Ins.txt", Item_Index, Item_Weight);

	////从大到小排序
	//int Permutation[200] = { 0 };

	//for (int i = 0; i < 200; i++)
	//{
	//	Permutation[i] = Item_Weight[i];
	//}
	//sort(Permutation, Permutation + 200);
	//

	//for (int i = 0; i < 200; i++)
	//{
	//	for (int j = 0; j < 10; j++)
	//	{
	//		cout << Permutation[i] << "\t";
	//	}
	//	cout << endl;
	//}
	//cout << endl;
	vector<int> Item_Index;
	vector<int> Item_Weight;
	GetBPInfo("BP_Ins.txt", Item_Index, Item_Weight);

	//按照重量从大到小排序
	Pair* ch = new Pair[Item_Weight.size()];
	for (int i = 0; i < Item_Weight.size(); i++)
	{
		ch[i].dim = Item_Index[i];
		ch[i].value = Item_Weight[i];
	}
	//排序函数
	sort(ch, ch + Item_Weight.size(), PairGreater());
	vector<int> Permutation(Item_Weight.size());
	for (int i = 0; i < Permutation.size(); i++)
		Permutation[i] = ch[i].dim;
	delete[]ch;
	int AccItemWeight = 0; //当前物体重量
	int bins_num = 1;//当前总共使用箱子总数
	int ItemsinAllBins[30][30] = {0};//记录每个箱子所装物品（默认给30个箱子）
	vector<int> WeightinBin(30,0); //箱子所装物体重量
	vector<int> NuminBin(30, 0);  //记录每个箱子所装物品数目

	for (int i = 0; i < Item_Weight.size(); i++)
	{
		int CurItem = Permutation[i];
		int flag = 0;//设置标志位
		AccItemWeight = Item_Weight[CurItem];

		for (int j = 0; j < bins_num; j++) //在所有已开箱子前进行检测
		{
			if (WeightinBin[j] + AccItemWeight <= Volume)//如果第j个箱子未装满
			{
				WeightinBin[j] += AccItemWeight;//更新第j个箱子所装中重量
				NuminBin[j]++;//更新第j个箱子所装个数
				ItemsinAllBins[j][NuminBin[j] - 1] = AccItemWeight;//记录数据
				flag = 1;//更新标志位，进行装箱操作
				break;//结束此循环过程不用遍历后续已使用箱子
			}
			else
			{
				continue;//继续此循环直至遍历完所有已使用的箱子
			}
		}
		if(flag==0)//物品无法放入已使用的箱子中，进行开箱操作并将物品装入新箱子中
		{
			bins_num++;//开启新箱子
			WeightinBin[bins_num - 1] = AccItemWeight; //更新新箱子重量
			NuminBin[bins_num - 1] = 1;//更新新使用箱子所存物品数
			ItemsinAllBins[bins_num - 1][0] = AccItemWeight;//记录当前物品数据
		}
	}

	//output
	cout << "Total Bins:" << bins_num <<endl;
	cout << "items number of each bin" << endl;
	for (int i = 0; i < bins_num; i++)
	{
		cout << NuminBin[i] << "\t";
	}
	cout << endl;


	cout << "Items in each bin:" << endl;
	for (int i = 0; i < bins_num; i++)
	{
		cout << "Bin" << i + 1 << ":"<<"\t";
		for (int j = 0; j < NuminBin[bins_num - 1]; j++)
		{
			if (ItemsinAllBins[i][j] == 0)
			{
				break;
			}
			else
			{
				cout << ItemsinAllBins[i][j] << "\t";
			}
		}
		cout << endl;
	}

	cout << "Weight each Bin" << endl;
	for (int k = 0; k < bins_num; k++)
		cout << WeightinBin[k]<< endl;
	return 0;
}

int main()
{
	int nItem = 200;
	GenerateItemWeight("BP_Ins.txt",nItem);
	Heu();
	return 0;
}



