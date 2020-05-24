// ListSort.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "stdlib.h"

using namespace std;

//单向链表
typedef struct Node{
	int data;
	Node* next;
}NODE, *LinkNode;

LinkNode headNode = nullptr;
LinkNode tailNode = nullptr;


void swap(LinkNode node1, LinkNode node2)
{
	int temp = node1->data;
	node1->data = node2->data;
	node2->data = temp;
}


//1.冒牌排序
void bubbleSort(LinkNode headNode)
{
	if (!headNode)
	{
		return;
	}

	//外围循环进行一次冒泡排序
	LinkNode head1 = headNode;
	//head到tail的位置即为待排序的数据序列，tail到最后链表最后即为已排序数据序列
	LinkNode tail = NULL;
	while (head1 != tail)
	{
		//内循环，进行数据交换操作
		LinkNode head2 = head1;
		//是否有数据交换
		bool flag = false;
		while (head2->next != tail)
		{
			if (head2->data > head2->next->data)
			{
				flag = true;
				swap(head2, head2->next);
			}
			head2 = head2->next;
		}
		//flag == false 无数据交换，已经排序成功
		if (!flag)
		{
			break;
		}
		tail = head2;
		head2 = NULL;
	}
	head1 = NULL;
}

//2.选择排序
void selectionSort(LinkNode headNode)
{
	if (!headNode)
	{
		return;
	}

	//外围while遍历头结点，找出剩余未排序数据中最小节点
	LinkNode head1 = headNode;
	while (head1)
	{
		//内围while遍历头结点，找出最小data的节点，并赋值给minNode
		LinkNode head2 = head1;
		//设置最小数据节点
		LinkNode minNode = head2;
		while (head2->next)
		{
			if (minNode->data > head2->next->data)
			{
				minNode = head2->next;
			}
			head2 = head2->next;
		}
		swap(head1, minNode);
		head2 = NULL; 
		minNode = NULL;
		head1 = head1->next;
	}
	head1 = NULL;
}

//3.1通过插入节点的方法实现插入排序
void insertSort_ChangeNode(LinkNode &head)
{
	if (!head)
	{
		return;
	}

	LinkNode curNode = head;  //当前节点（包括当前节点）以前的数据序列都是有序的
	while (curNode->next)
	{
		LinkNode node = head;
		LinkNode preNode = node;
		LinkNode tailNode = curNode->next;  //记录当前待插入的节点		                                                                                                     
		bool flag = false;
		while (tailNode != node)			//从头查找tailNode要插入的位置
		{
			if (tailNode->data < node->data)
			{
				curNode->next = tailNode->next;
				if (node == head)		//当前节点为已插入的有序数列中
				{
					head = tailNode;
					head->next = node;
				}
				else
				{
					preNode->next = tailNode;
					tailNode->next = node;
				}
				flag = true;
				break;
			}
			else
			{
				preNode = node;
				node = node->next;
			}
		}
		if (!flag)
		{
			curNode = curNode->next;
		}
	}
	cout << "sort over" << endl;
}

//3.2通过交换数据的方式实现插入排序
void insertSort_SwapData(LinkNode head)
{
	if (!head)
	{
		return;
	}

	LinkNode curNode = head;  //当前节点（包括当前节点）以前的数据序列都是有序的
	while (curNode->next)
	{
		LinkNode node = head;
		LinkNode preNode = node;
		LinkNode tailNode = curNode->next;  //记录当前待插入的节点		                                                                                                     
		bool flag = false;
		while (tailNode != node)			//从头查找tailNode要插入的位置
		{
			LinkNode node2 = node;
			while (node2->next != tailNode)
			{
				node2 = node2->next;
			}

			if (tailNode->data < node2->data)
			{
				swap(tailNode, node2);
				tailNode = node2;
			}
			else
			{
				break;
			}
		}
		curNode = curNode->next;
	}
	cout << "sort over" << endl;
}


//4.希尔排序

//获取下一个增量的节点
LinkNode getNextIncNode(LinkNode node, int inc)
{
	LinkNode head = node;
	while (inc && head)
	{
		head = head->next;
		--inc;
	}

	if (inc == 0)
	{
		return head;
	}
	else
	{
		return NULL;
	}
}

void shellSort(LinkNode head)
{
	int listlen = 0;
	LinkNode head1 = head;
	while (head1)
	{
		++listlen;
		head1 = head1->next;
	}

	int incFlag = listlen/2;
	while (true)
	{
		if (incFlag < 1)
		{
			break;
		}
		head1 = head;
		int flag = incFlag;
		while (flag > 0)
		{
			LinkNode curNode = head1;
			LinkNode curNext = getNextIncNode(curNode, incFlag);
			while (curNext)
			{
				LinkNode node = head1;
				LinkNode tailNode = getNextIncNode(curNode, incFlag);

				while (curNode != tailNode)
				{
					LinkNode preNode = curNode;
					LinkNode nextNode = getNextIncNode(preNode, incFlag);

					while (nextNode != tailNode)
					{
						preNode = nextNode;
						nextNode = getNextIncNode(curNode, incFlag);
					}

					if (preNode->data > tailNode->data)
					{
						swap(tailNode, preNode);
						tailNode = preNode;
					}
					else
					{
						break;
					}
				}
				curNode = curNext;
				curNext = getNextIncNode(curNode, incFlag);
			}

			head1 = head1->next;
			--flag;
		}
		incFlag /= 2;
	}
}

//5.快速排序
void quickSort(LinkNode head, LinkNode tail)
{
	if (head == NULL || tail == NULL)
	{
		return;
	}

	if (head == tail)
	{
		return;
	}

	LinkNode KeyNode = head;
	int key = head->data;



	
}

LinkNode createNode(int data)
{
	LinkNode node = (Node*)malloc(sizeof(NODE));
	node->data = data;
	node->next = NULL;
	return node;
}

void printData(LinkNode head)
{
	LinkNode node = head;
	while (node)
	{
		cout << node->data << endl;
		node = node->next;
	}
	free(node);
	node = NULL;
}

LinkNode initTestData()
{
	int arr[] = {68, 16,99, 32, 6, 77, 43, 88, 22};
	LinkNode node = NULL;
	LinkNode head = NULL;
	for (int i = 0; i < 2; i++)
	{
		if (i == 0)
		{
			head = createNode(arr[i]);
			node = head;
		}
		else
		{
			LinkNode nd = createNode(arr[i]);
			node->next = nd;
			node = nd;
		}
	}
	return head;
}

void testSort()
{
	LinkNode head = initTestData();
	printData(head);
	cout << "sort data" << endl;
	//bubbleSort(head);
	//selectionSort(head);
	//insertSort(head);
	//insertSort_SwapData(head);
	shellSort(head);
	printData(head);
	free(head);
	head = NULL;
}

int _tmain(int argc, _TCHAR* argv[])
{
	/*
	LinkNode node = (Node*)malloc(sizeof(NODE));
	node->data = 0;
	node->next = NULL;
	LinkNode node1 = (Node*)malloc(sizeof(NODE));
	node1->data = 1;
	node1->next = NULL;
	node->next = node1;
	cout << node->next->data << endl;
	*/
	testSort();

	system("pause");
	return 0;
}

