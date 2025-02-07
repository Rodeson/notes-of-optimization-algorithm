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
	vector<int> Item_Index;
	vector<int> Item_Weight;
	GetBPInfo("BP_Ins.txt",Item_Index,Item_Weight);

	//按照重量从大到小排序
	Pair *ch = new Pair[Item_Weight.size()];
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
	
	int AccWeight = 0; //当前箱子所装物品的累积重量
	vector<vector<int>> ItemsinAllBins; //二维数组，记录各个箱子所装物品
	vector<int> IteminBin; //记录当前箱子所装物品
	vector<int> WeightinBin; //箱子所装物品重量
	for (int i = 0; i < Item_Weight.size(); i++)
	{
		int CurItem = Permutation[i];
		if (AccWeight + Item_Weight[CurItem] <= Volume) // The bin is not full
		{
			AccWeight += Item_Weight[CurItem];
			IteminBin.push_back(CurItem);
		}
		else //The bin has been filled
		{			
			ItemsinAllBins.push_back(IteminBin);
			WeightinBin.push_back(AccWeight);
			IteminBin.clear();

			//Next bin start
			AccWeight = Item_Weight[CurItem];
			IteminBin.push_back(CurItem);
		}
	}

	//output
	cout << "Total Bins:" << ItemsinAllBins.size()<<endl;
	cout << "Items in each bin:" << endl;
	for (int i = 0; i < ItemsinAllBins.size(); i++)
	{
		for (int j = 0; j < ItemsinAllBins[i].size(); j++)
			cout << ItemsinAllBins[i][j] << "\t";
		cout << endl;
	}
	cout << "Weight each Bin" << endl;
	for (int k = 0; k < WeightinBin.size(); k++)
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



