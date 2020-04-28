// List.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include "stdlib.h"

using namespace std;

//最大长度
#define MAX_SIZE  100  //0x3FF 1KB,根据情况而定
//链表长度
int curSize = 1;
//插入标记，当插入标记等于链表长度则需要扩容
int dataLen = 0;
//c++分配连续内存控件在GVAR区  全局变量初始化区
int *nums = new int[curSize]{0};

//打印元素为int类型的数据，作为函数指针传值
void printIntElement(void *elem)
{
	auto data = (int *)elem;
	cout << *data << endl;
}

//打印顺序链表内容
//@para  startAddr 链表起始地址  len链表长度  perSize链表中每个元素的长度   (*printElement)(void *)函数指针用于打印对应元素数据
void printListElement(void *starAddr, const int len, const int perSize, void(*printElement)(void *))
{
	if (starAddr == nullptr)
	{
		cout << "error data" << endl;
		return;
	}

	//void类型指针要转换成char类型才可以进行运算操作
	char *addr = (char *)starAddr;

	for (int i = 0; i < len; i++)
	{
		printElement(addr + perSize*i);
	}

	cout << "print over" << endl;
}

void printMyList()
{
	printListElement(nums, dataLen, sizeof(int), printIntElement);
}

void doubleCapacity()
{
	//如果没有设置MAXSIZE，扩容一倍,如果超出则设置为最大值4G
	/*------------------------------
	unsigned int size = curSize<<2;
	if (size < curSize)
	{
	curSize = 0xFFFFFFFF;
	}
	-------------------------------*/
	int size = curSize << 1;
	if (size > MAX_SIZE)
	{
		size = MAX_SIZE;
	}

	int *ptr = new int[size]{0};

	//数据搬移
	for (int i = 0; i < curSize; ++i)
	{
		ptr[i] = nums[i];
	}
	delete nums;
	nums = ptr;
	curSize = size;
	ptr = nullptr;
}

int insertData(const int data, const int pos)
{
	//插入数据长度如果大于当前链表长度则数据有问题
	if (dataLen > curSize)
	{
		return -1;
	}

	//超出插入长度
	if (dataLen == MAX_SIZE)
	{
		return -2;
	}

	//如果没有设置MAXSIZE，如果超出则设置为最大值4G
	/*-------------------------------
	if (dataLen + 1 < dataLen)
	{
	return -2;
	}
	--------------------------------*/

	//需要扩容
	if (dataLen == curSize)
	{
		doubleCapacity();
	}

	//移动pos以后的数据
	for (int i = dataLen; i > pos; --i)
	{
		nums[i] = nums[i - 1];
	}
	//插入数据到pos
	nums[pos] = data;
	//数据长度+1
	++dataLen;
	return 0;
}

//从最前面插入数据
int insertPrepend(const int data)
{
	return insertData(data, 0);
}

//从最后面插入数据
int insertAppend(const int data)
{
	return insertData(data, dataLen);
}


//------------------------冒泡排序-------------------------------------
//1>>基本思想--两个数比较大小，较大的数下沉，较小的数冒起来。
//2>>过程--1.比较相邻的两个数据，如果后面的数小，就交换位置；
//------2.从后向前两两比较，一直到比较最前两个数据。
//------3.继续重复上述过程，依次将第2.3...n-1个最小数排好位置
//3>>时间复杂度O(N²)
//4>>优化方案--设置flag，如果在当前一轮比较后，数据没有发生交换则跳出循环
void bubbleSort()
{
	for (int i = 0; i < dataLen; ++i)
	{
		//设置当前一轮是否有数据交换标志
		bool flag = false;
		for (int j = dataLen - 1; j > i; --j)
		{
			if (nums[j] < nums[j - 1])
			{
				int temp = nums[j - 1];
				nums[j - 1] = nums[j];
				nums[j] = temp;
				//有数据交换设置为true
				flag = true;
			}
		}
		//flag == false 无数据交换，已经排序成功
		if (!flag)
		{
			return;
		}
	}
}
//----------------------OVER-------------------------------


//-----------------------选择排序---------------------------
//1>>基本思想--找到最小的数与当前标记的第一个数进行交换
//2>>过程--1.在长度为N的无序数组中，第一次遍历n-1个数，找到最小的数值与第一个元素交换
//---------2.第二次遍历n-2个数，找到最小的数值与第二个元素交换
//---------3.第n-1次遍历，找到最小的数值与第n-1个元素交换，排序完成
//3>>时间复杂度O(N²)
void selectionSort()
{
	for (int i = 0; i < dataLen - 1; ++i)
	{
		int flag = i;
		for (int j = i + 1; j < dataLen; ++j)
		{
			if (nums[j] < nums[flag])
			{
				flag = j;
			}
		}
		if (flag != i)
		{
			int temp = nums[i];
			nums[i] = nums[flag];
			nums[flag] = temp;

		}
	}
}
//----------------------OVER-------------------------------

//----------------------插入排序---------------------------
//1>>基本思想--将当前第n个未排序的数插入到前n-1个有序数列中
//2>>过程-----1.当n = 2的时候 如果当前数大于第一个数则进行交换 则前两个数形成有序链表
//------------2.当n = 3时时候 将当前数插入到前面2个数形成的有序数列中形成新的3个数有序数列
//------------3.当n = k的时候 将当前数插入到前面k-1个数形成的有序数列中形成新的kge数的有序数列
//------------4.重复以上步骤当n = n-1时排序完成
void insertSort()
{
	for (int i = 0; i < dataLen - 1; ++i)
	{
		for (int j = i + 1; j > 0; --j)
		{
			if (nums[j] < nums[j - 1])
			{
				auto temp = nums[j - 1];
				nums[j - 1] = nums[j];
				nums[j] = temp;
			}
			//找到合适位置（小于后一个数，大于等于前面一个数），退出当前循环
			else
			{
				break;
			}
		}
	}
}

//----------------------OVER-------------------------------


void testAppendData()
{
	int res = 0;
	do{
		static int index = 0;
		res = insertAppend(index);
		++index;
	} while (res == 0);

	printMyList();
}

void testPrependData()
{
	int res = 0;
	do{
		static int index = 0;
		res = insertPrepend(index);
		++index;
	} while (res == 0);

	printMyList();
}

void initTestData()
{
	insertAppend(68);
	insertAppend(33);
	insertAppend(108);
	insertAppend(42);
	insertAppend(5);
	insertAppend(12);
	insertAppend(54);
	insertAppend(77);
	insertAppend(33);
}

void testSort()
{
	//insertAppend(68);
	//insertAppend(33);
	initTestData();
	printMyList();
	//bubbleSort();
	//selectionSort();
	insertSort();
	printMyList();
}

int _tmain(int argc, _TCHAR* argv[])
{
	//testPrependData();
	//testAppendData();
	testSort();
	system("pause");
	return 0;
}

