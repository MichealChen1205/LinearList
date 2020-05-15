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


void swap(int arr[], int i, int j)
{
	int temp = arr[i];
	arr[i] = arr[j];
	arr[j] = temp;
}

//------------------------冒泡排序-------------------------------------
//1>>基本思想--两个数比较大小，较大的数下沉，较小的数冒起来。
//2>>过程--1.比较相邻的两个数据，如果后面的数小，就交换位置；
//---------2.从后向前两两比较，一直到比较最前两个数据。
//---------3.继续重复上述过程，依次将第2.3...n-1个最小数排好位置
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

//----------------------希尔排序------------------------------
//1>>基本思想与过程--1.在要排序的一组数据中，根据某一增量N将该组
//--------------------数据若干个子序列，并对子序列进行插入排序
//------------------2.然后逐渐减小增量N，并重复上述过程，直至增量
//--------------------N=1，此时数据基本有序，最后再进行插入排序
void shellSort()
{
	int incFlag = dataLen;
	while (true)
	{
		incFlag = incFlag / 2;			//增量每次除以2
		if (incFlag < 1)				//如果增量小于1则退出
		{
			break;
		}

		for (int k = 0; k < incFlag; ++k)							//分为incFlag个子序列，要对其循环incFalg次，每次对每个子序列进行排序
		{
			for (int i = k + incFlag; i < dataLen; i += incFlag)	//对每个子序列进行插入排序
			{
				for (int j = i; j > k; j -= incFlag)
				{
					if (nums[j] < nums[j - incFlag])				//对第j个和j-incFlag数据进行对比
					{
						auto temp = nums[j - incFlag];
						nums[j - incFlag] = nums[j];
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
	}
}
//----------------------OVER----------------------------------

//----------------------快速排序------------------------------
//1>>基本思想--1.先从数列中取出一个数作为key
//------------2.将比这个数小的数全部放在它的左边，大于或等于它的数全部放在它的右边
//------------3.对左右两个小数列重复第二步，直至各区间只有一个数
//--------------------N=1，此时数据基本有序，最后再进行插入排序
//2>>平均时间复杂度：O(N*logN)

void quickSort(int lFlag,  int rFlag)
{
	if (lFlag >= rFlag)
	{
		return;
	}

	int r = rFlag;
	int l = lFlag;
	int key = nums[lFlag];
	while (r > l)
	{
		if (key < nums[l + 1])
		{
			int temp = nums[r];
			nums[r] = nums[l + 1];
			nums[l + 1] = temp;
			r--;
		}
		else
		{
			nums[l] = nums[l + 1];
			nums[l + 1] = key;
			l++;
		}

		if (r == l)
		{
			quickSort(lFlag, l - 1);
			quickSort(l + 1, rFlag);
		}
	}
}
//----------------------OVER----------------------------------

//----------------------归并排序------------------------------
//1>>基本思想--1.归并排序是建立在归并操作上的一种有效的排序算法。该算法是采用分治法的一个非常典型的应用。
//------------2.基本思路就是将数组分成2组A，B，如果这2组组内的数据都是有序的，那么就可以很方便的将这2组数据进行排序。
//------------3.虑下如何将2个有序数列合并。这个非常简单，只要从比较2个数列的第一个数，谁小就先取谁，
//--------------取了后就在对应数列中删除这个数。然后再进行比较，如果有数列为空，那直接将另一个数列的数据依次取出即可。
//------------4.可以将A，B组各自再分成2组。依次类推，当分出来的小组只有1个数据时，可以认为这个小组组内已经达到了有序，
//--------------然后再合并相邻的2个小组就可以了。这样通过先递归的分解数列，再合并数列就完成了归并排序
//2>>平均时间复杂度：O(N*logN)
void memeryArray(int arr1[], int len1, int arr2[], int len2, int memeryArr[])
{
	if (len1 <= 0 || len2 <= 0)
	{
		return;
	}
	int i = 0, j = 0, k = 0;
	int len = len1 + len2;
	while (i < len1 || j < len2)
	{
		if (i >= len1)
		{
			memeryArr[k] = arr2[j];
			++j;
			++k;
			continue;
		}

		if (j >= len2)
		{
			memeryArr[k] = arr1[i];
			++i;
			++k;
			continue;
		}

		if (arr1[i] < arr2[j])
		{
			memeryArr[k] = arr1[i];
			++i;
		}
		else
		{
			memeryArr[k] = arr2[j];
			++j;
		}
		++k;
	}
}

void mergeSort(int start, int end, int temp[])
{
	if (start == end)
	{
		temp[0] = nums[start];
		return;
	}

	int mid = (start + end) / 2;
	int *temp1 = new int[mid -start + 1];
	mergeSort(start, mid, temp1);
	int *temp2 = new int[end - mid];
	mergeSort(mid + 1, end, temp2);
	memeryArray(temp1, mid - start + 1, temp2, end - mid, temp);
}
//--------------------------OVER----------------------------------  

//----------------------堆排序------------------------------
//1>>基本思想--1.利用堆这种数据结构所设计的一种排序算法。堆可以看成是一个完全二叉树，堆顶元素是整棵树中最大值。
//------------2.将N个数据数列变成堆结构后，将堆顶元素取出，与第N个数据进行交换。
//------------3.然后对N-1个数据以前的数据变成堆结构，重复第2步，直至数列数据个数N=1。
//--------------取了后就在对应数列中删除这个数。然后再进行比较，如果有数列为空，那直接将另一个数列的数据依次取出即可。
//2>>堆的定义--N个序列元素(A1, A2, A3......An-1, An);满足以下一个关系则为堆结构
//------------1.最小化堆--> Ai <= A2i 并且 Ai <= A2i+1
//------------1.最大化堆--> Ai >= A2i 并且 Ai >= A2i+1
//3>>平均时间复杂度：O(N*logN)
void changeMaxHeap(int arr[], int index, int len)
{
	int lChildIndex = index * 2 + 1;
	int rChildIndex = index * 2 + 2;
	
	if (lChildIndex < len && arr[index] < arr[lChildIndex])
	{
		swap(arr, index, lChildIndex);
	}

	if (rChildIndex < len && arr[index] < arr[rChildIndex])
	{
		swap(arr, index, rChildIndex);
	}
}

void buildMaxHeap(int arr[], int len)
{
	for (int i = len / 2; i >= 0; --i)
	{
		changeMaxHeap(arr, i, len);
	}
}

void heapSort(int arr[], int len)
{
	if (len > 1)
	{
		buildMaxHeap(arr, len);
		swap(arr, 0, len - 1);
		heapSort(arr, len - 1);
	}
}

//--------------------------OVER----------------------------------  

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

void changeArrData(int arr[])
{
	arr[0] = 100;
}

void testSort()
{
	insertAppend(68);
	insertAppend(33);
	insertAppend(1);
	//insertAppend(88);
	//initTestData();
	printMyList();
	//bubbleSort();
	//selectionSort();
	//insertSort();
	//shellSort();
	//quickSort(0, 1);
	//int temp[9] = { 0 };
	//mergeSort(0, dataLen - 1, temp);
	//printListElement(temp, 9, sizeof(int), printIntElement);
	heapSort(nums, dataLen);
	printMyList();
}

int _tmain(int argc, _TCHAR* argv[])
{
	//testPrependData();
	//testAppendData();
	int a = 10;
	int b = 8;
	if (a > b)
	{
		a = 100;
	}
	else
	{
		b = 100;
	}

	testSort();
	system("pause");
	return 0;
}

