#include <stdio.h>
#include <malloc.h>
#include "cebek.h"

const int n = 3;
const int m = 4;

int zero = 0;
int m_one = -1;

int nVeryLargeNumber = 99999999999;

int PivotN = -1;
int PivotM = -1;


int FindPath(int** aPath, int** aRoute, int i, int j) // ���������� �����
{
	insertFirst(i, j); // ��������� �� ����� �������

	if (LookHorizontaly(aPath, aRoute, i, j, i, j) == 0) // �������� ������������ �� �������� ���������� ������� ��� �����
														 // ����� �������� � ����������, ��� ��� ����
	{
		printf("Path error, press key", i, j);
	}
	return 0;
}

void GetDual(float **aRoute, int** aCost, float** aDual, int** aPath)
{
	int x; // ���������� ����������
	int z; // ���������� -1/1

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			aDual[i][j] = -0.5;
			if (aRoute[i][j] == 0)
			{
				FindPath(aPath, aRoute, i, j); // ����������� ������ ����� ���� ��������� (i, j)
				struct node *ptr = head;       // ptr - �������� �� ��������� �������
				z = 1;
				x = 0;
				while (ptr != NULL)
				{
					x += z * aCost[ptr->key][ptr->data]; // ��� ��������� �� ����������, ��������, �� U1 = 0, � ���� ���������� ���������� 
														 // � ������������ x = Cost[i,j]*z + x
					z *= -1;
					clear(ptr->key);
					ptr = ptr->next;
				}
				aDual[i][j] = x;
			}
		}
	}
}

void PrintOut(float *aDemand, float * aSupply, float**aRoute, int** aCost, float** aDual, int** aPath)// +
{
	GetDual(aRoute, aCost, aDual, aPath);								// ���������� ����������
	printf("\n");
	float nCost = 0.0;
	printf("DEMAND                                                                                     SUPPLY \n");

	for (int i = 0; i < m; i++)
	{
		printf("%f      ", aDemand[i]);									// ��������� �� ����� ������ ������ ������������ 
	}
	printf("\n");

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			nCost += (aCost[i][j] * aRoute[i][j]);						// ���������� �������� �������
			if (aRoute[i][j] == 0.0)									// ���� ������� �� �������
			{
				printf("[<%2i>%4i]", aCost[i][j], (int)aDual[i][j]);	//��������� ����������
			}
			else
			{
				printf("[<%2i>(%2i)]", aCost[i][j], (int)aRoute[i][j]); // ��������� �� ����� ������� �����������
			}
		}
		printf(" : %f\n", aSupply[i]);									// ��������� �� ����� ������ ����� ������������� 
	}
	printf("------------\n");
	printf("Cost: %.1f", nCost);
}

void NorthWest(float *aDemand, float * aSupply, float** aRoute, int** aCost, float** aDual)
{
	float z = 0.0;											// �������� ���� �ij ����� �������
	float aS[4];											// ��� �� ����������� �������
	float aD[3];   

	for (int i = 0; i<m; i++)
	{
		aS[i] = 0.0;
	}

	for (int i = 0; i<n; i++)
	{
		aD[i] = 0.0;
	}

	for (int i = 0; i < n; i++)								 // �������� ����� ����������� ������ ������� ������-��������� ����
															 // ���������� ����� ������ ���� 
	{
		for (int j = 0; j < m; j++)
		{
			if ((aDemand[j] - aS[j]) < (aSupply[i] - aD[i])) // ������� � �� ������� ������������ �����,
															 // ���� �������� ����� ������������� � ����� ���������, ����� ����� ����� 
															 // ���������� ���� ������� ���� ������� ����� ��� ��������� ��������
			{
				z = aDemand[j] - aS[j];						 // ������� � ������� ������ ������ �� ������ �� ������ �����
				aRoute[i][j] = z;
				aS[j] += z;									 
				aD[i] += z;
			}
			else
			{
				z = aSupply[i] - aD[i];
				aRoute[i][j] = z;
				aS[j] += z;
				aD[i] += z;
			}
		}
	}
}

int NotOptimal(float *aDemand, float * aSupply, float ** aRoute, int ** aCost, float **aDual, int ** aPath)//
{
	int x;										// ��������� �����
	int nMax;
	nMax = -nVeryLargeNumber;					// ���������� nMax ���� ����� ������
	GetDual(aRoute, aCost, aDual, aPath);		// ��������� ������� ���������� ���������� �� ���������� �����
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			x = aDual[i][j];

			if (x > nMax)
			{
				nMax = x;
				PivotN = i;						// ���������� ��������� ������� �����
				PivotM = j;
			}
		}
	}
	return (nMax > 0);							// ���� � ���������� ������� (Dij=Cij-(Vj+Ui)) � ����'��� ��������
												//  � ���� - ������ ������������.
}

int LookHorizontaly(int** aPath, float** aRoute, int u, int v, int u1, int v1);//

int LookVerticaly(int** aPath, float** aRoute, int u, int v, int u1, int v1);//

void BetterOptimal(float *aDemand, float *aSupply, float** aRoute, int** aCost, float** aDual, int** aPath)
{
	int nMin;
	FindPath(aPath, aRoute, PivotN, PivotM);	// ������ ������� �������� �����
	printList();
	nMin = nVeryLargeNumber;					// ����������� ���� �������� �����

	struct node *ptr1 = head;					// ��������� �� ������� ������ ������ �����
	struct node *ptr4 = head;
	struct node *ptr0 = head;
	struct node *ptr5 = head;

	int t;
	ptr1 = ptr1->next;
	while (ptr4 != NULL)
	{
		t = aRoute[ptr4->key][ptr4->data];		// ���������� �������� �������� �� ���������� ������ (-) ������ �����  �� ��������� �� �� t 
		if (t < nMin)
		{
			nMin = t;
		}


		ptr4 = ptr4->next;
		if (ptr4 == NULL) {
			break;
		}
		ptr4 = ptr4->next;
	}
	while (ptr1 != NULL)
	{
		aRoute[ptr5->key][ptr5->data] -= nMin;	// ������� � ������ ����� �������� t
		aRoute[ptr1->key][ptr1->data] += nMin;	// ������ �� ������ ����� � ������ (+) �������� t 
		ptr5 = ptr5->next;
		if (ptr5 == NULL) {
			break;
		}
		ptr5 = ptr5->next;

		ptr1 = ptr1->next;
		if (ptr1 == NULL) {
			break;
		}
		ptr1 = ptr1->next;
	}

	while (ptr0 != NULL) 
	{
		clear(ptr0->key);
		ptr0 = ptr0->next;
	}
	printList();
}

int main()
{
	//
	int Demand[4] = { 16, 18, 30, 25 };
	//printf("Demands:\");
	//for (int i = 0; i < m; i++)
	//{
	//	printf("Enter demand value #(%i) ==>  ", i);
	//	scanf_s("%f", &Demand[i]);
	//}

	int count_demand = 0;
	float * aDemand;										// ��䳺�� ���'��� �� ����� ������ ������������
	aDemand = (float*)malloc(m * sizeof(float));
	for (int i = 0; i < m; i++)
	{
		aDemand[i] = Demand[count_demand];
		count_demand++;
	}


	//
	int Supply[3] = { 19, 37, 34 };
	//printf("Supplyes:\n");
	//for (int i = 0; i < n; i++)
	//{
	//	printf("Enter supply value #(%i) ==>  ", i);
	//	scanf_s("%f", &Supply[i]);
	//}
	int counter_supply = 0;
	float * aSupply;										// ��䳺�� ���'��� �� ����� ������ �������������
	aSupply = (float*)malloc(n * sizeof(float));
	for (int i = 0; i < n; i++)
	{
		aSupply[i] = Supply[counter_supply];
		counter_supply++;
	}
	//



	float** aRoute; // ����� ������� �������� �������
	aRoute = (float**)malloc(sizeof(float*) * n);
	for (int i = 0; i < n; i++)
	{
		aRoute[i] = (float*)malloc(sizeof(float) * m);
	}

	for (int count_row = 0; count_row < n; count_row++)
	{
		for (int count_column = 0; count_column < m; count_column++)
		{
			aRoute[count_row][count_column] = 0;
		}
	}

	//printf("Costs:\n");

	//int Cost[24];
	//int k = 0;
	//int j = 0;
	//for (int i = 0; i < m*n; i++)
	//{
	//	if (j < 5)
	//	{
	//		printf("Enter cost #(%i) (%i) ==>  ", k, j);
	//		scanf_s("%i", &Cost[i]);
	//		j++;
	//	}
	//	else
	//	{

	//		printf("Enter cost #(%i) (%i) ==>  ", k, j);
	//		scanf_s("%i", &Cost[i]);
	//		j = 0;
	//		k++;
	//	}
	//}

	int Cost[12] =
	{ 5, 3, 6, 2
	, 4, 7, 9, 1 
	, 3, 4, 7, 5 };

	int** aCost; // ����� ��� ����������� �ij
	aCost = (int**)malloc(sizeof(int*) * n);
	for (int i = 0; i < n; i++)
	{
		aCost[i] = (int*)malloc(sizeof(int) * m);
	}

	int count_cost = 0;
	for (int count_row = 0; count_row < n; count_row++)
	{
		for (int count_column = 0; count_column < m; count_column++)
		{
			aCost[count_row][count_column] = Cost[count_cost];
			count_cost++;
		}
	}


	//
	float** aDual; //����� ����������
	aDual = (float**)malloc(sizeof(float*) * n);
	for (int i = 0; i < n; i++)
	{
		aDual[i] = (float*)malloc(sizeof(float) * m);
	}

	for (int count_row = 0; count_row < n; count_row++)
	{
		for (int count_column = 0; count_column < m; count_column++)
		{
			aDual[count_row][count_column] = -1;
		}
	}


	//
	int** aPath; // ���������� ����� ��������� �������
	aPath = (int**)malloc(sizeof(int*) * n * m);
	for (int i = 0; i < n*m; i++)
	{
		aPath[i] = (int*)malloc(sizeof(int*) * 2);
	}

	for (int count_row = 0; count_row < n*m; count_row++)
	{
		for (int count_column = 0; count_column < 2; count_column++)
		{
			aPath[count_row][count_column] = zero;
		}
	}
	//
	NorthWest(aDemand, aSupply, aRoute, aCost, aDual);       //������������� �������� ������� ���������������
													         //������� ������-��������� ����
	PrintOut(aDemand, aSupply, aRoute, aCost, aDual, aPath); //��������� �� ����� ����������� �������
																		//�� ��������� �������� ������� ���������������
	while (NotOptimal(aDemand, aSupply, aRoute, aCost, aDual, aPath) == 1) //�������� ������ �� ������������
	{
		printf("\nPIVOTING ON %i %i", PivotN, PivotM);
		BetterOptimal(aDemand, aSupply, aRoute, aCost, aDual, aPath); //�������� '�����' ������ ����������� ������
		PrintOut(aDemand, aSupply, aRoute, aCost, aDual, aPath); 
	}
	printf("\n");
	printf("FINISHED\n");

	system("pause");
	return 0;
}

int LookVerticaly(int** aPath, float** aRoute, int u, int v, int u1, int v1)
{
	for (int i = 0; i < n; i++)
	{
		if ((i != u) && (aRoute[i][v] != 0))				  // ���� ������� ������� �� �� ����� �� ������ (pivotN) ��� �� ������������
															  // (����� ��� ����������� �� ��� ���� �������� �� ����� �� �������
		{
			if (LookHorizontaly(aPath, aRoute, i, v, u1, v1)) // ���� �������� ������������ ������ �������
			{
				insertFirst(i, v);
				return 1;
			}
		}
	}
	return 0;
}

int LookHorizontaly(int** aPath, float** aRoute, int u, int v, int u1, int v1)
{
	for (int i = 0; i < m; i++)
	{
		if ((i != v) && (aRoute[u][i] != 0))				  // ���� ������� ������� �� �� ������� �� ������ (pivotM) ��� �� ������������
		{
			if (i == v1)									  
			{
				insertFirst(u, i);							  // �������� �� ��������� ������ � ����������� ������� ��������� ������� �����
															  // ������ ���������� ��������� ������� �����
				return 1;
			}
			if (LookVerticaly(aPath, aRoute, u, i, u1, v1))	  // ���������� �������� ��� ��� ��� �������� �������
															  // �� ������ ���������� � ���� � �� �� ����� ��� ������ � ���� � �������. 
			{
				insertFirst(u, i);
				return 1;
			}
		}
	}
	return 0;
}