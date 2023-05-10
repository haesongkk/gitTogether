//������ �迭 ����

#include <iostream>
#include <string>
using namespace std;

//int main()
//{
//
//	//���� �ڵ� ������ ����Ǵ� �����? **
//	char str[50] = "I like C programming";
//	str[8] = '\0';
//	cout << str << endl;
//	/// I like C
//
//	//���� �ڵ� ������ ����Ǵ� �����? ** 
//	int arr[3] = { 11, 22, 33 };
//	int* ptr = arr;
//	cout << *(ptr + 2);
//	/// 33
//
//	//���� �ڵ��� ���� ����� ���ÿ�. *
//	char arr2[] = "Hello World";
//	char* str1 = arr2;
//	cout << arr2[0] << *(str1 + 6) << endl;
//	/// HW
//
//	//�Ʒ��� ������ �迭�� for ���� �̿��� ����ϴ� �ڵ带 �ۼ��ϼ���. *
//	const char* str2[] = { "seoul","paris","london" };
//
//	for (int i = 0; i < 3; i++)
//	{
//		cout << *(str2 + i) << " ";
//	}
//	cout << endl;
//
//	// �����?
//	char str_a[] = "abc";
//	char str_b[] = "abc";
//	printf("%d", str_a == str_b);
//	/// �迭�� �ּҸ� ���ϰ� �����Ƿ� �ٸ��� -> 0
//
//	return 0;
//}

/*

//1. ������ ���� ������ �迭�� �ٸ��� ������ ����?
	1	2	3	4
	5	6	7	8

   (1) int Array[2][2];
   (2) int Array[2][4];
   (3) int Array[4][2];
   (4) int Array[4][4];
   
   �� : 2

//2. ���� �ڵ��� ���� �� ����Ǵ� �����? *

  int main(void)
  {
	int Array[3][3] = {
		{1, 2, 3},
		{4, 5, 6},
		{7, 8, 9}
	};

	cout << Array[1][2] << endl;
  }

  �� : 6

//3. ( )�ȿ� �� ������ ���� ���ÿ�  **

   int arr[10];      //arr�� ���������� ( int * )
   int * parr[20];  //parr�� ���������� (        )

   (1) int
   (2) int[]
   (3) int *
   (4) int **

   �� : 4

//4. 2���� �迭�� ���꿡 ���� �ڵ��Դϴ�. ����Ǵ� ����� ���Դϱ�? *

	int main(void)
	{
		int arr[3][2] = { 1,2,3,4,5,6 };
		cout << **(arr + 1);
		return 0;
	}

	�� : 12 34 56 -> 3 �� ��µȴ�


//5. �Լ��� �迭�� �����ϱ� ���ؼ��� �迭�� �ּҰ��� �����ϴ� ����� ���Ѵ�. **
   ���� �� A, B �ڸ��� �� ������ ������ ����?

	void SimpleFunc(  A  )
	{
		printf(��%d %d��, param[0], param[1]);
	}

	int main(void)
	{
		int arr[3] = {1, 2, 3};
		SimpleFunc(  B  );
	}

   (1) int param, &arr
   (2) int param, arr
   (3) int *param, &arr
   (4) int *param, arr

   �� : 4

//6. Call-by-value �� ���� �����ϴ� ������ �Լ�ȣ��,
  Call-by-reference �� �ּ� ���� �����ϴ� ������ �Լ�ȣ���� ���մϴ�.
  ���� �ڵ�� � ����ΰ���?

	int Add(int num1, int num2)
	{
		return num1 + num2;
	}

	int main(void)
	{
		int num1 = 2, num2 = 3;
		int sum = Add(num1, num2);
	}

	�� : Add �Լ��� ���� �����ϰ� �����Ƿ� call-by-value

//7. ���� �� A, B �ڸ��� �� ������ ������ ����?

	int num;
	scanf(��%d��,   A   );
	char str[10];
	scanf(%s,  B   );

   (1) num, str
   (2) num, &str
   (3) &num, str
   (4) &num, &str

   �� : 3

//8. ���� �ڵ忡 �ʿ��� �� ������ ���� ��ȯ�ϴ� swap �Լ��� �ۼ��� ������. *

	void main()
	{
		int num1 = 1;
		int num2 = 2;
		swap( &num1, &num2);
	}

	void swap(int* a. int* b)
	{
		int temp=0;
		temp = *a;
		*a = *b;
		*b = temp; 
	}

//9. ���� �ڵ忡 �ʿ��� �� ������ ���� ��ȯ�ϴ� swap �Լ��� �ۼ��� ������. **

	void main()
	{
		int num1 = 1;
		int num2 = 2;
		swap( num1, num2);
	}

	void swap(int& a, int& b)
	{
		int tmp = a;
		a = b;
		b = tmp;
	}


//10. ���밪�� ���ϴ� �Լ� �ۼ��ϱ�  //����, ������ �ΰ��� ���

void absolute1(int& n)
{
	if(n >= 0) return;
	else n = -n;
}

void absolute2(int* n)
{
	if(*n < 0) *n = -*n;
	else return;
}


//-----------------------------------------------------------------
//11. �迭�� �Լ��� ���ڷ� �����Ͽ� ����ϴ� �Լ��� ����� ������.

#include <stdio.h>

int main(void)
{
	int arr[3] = { 1, 2, 3 };
	int len = sizeof(arr) / sizeof(int);
	ShowArray(arr, len);		//��� �Լ� �ۼ��� ����
	return 0;
}

void ShowArray(int* arr, int len)
{
	for(int i = 0; i < len; i++)
	{
		cout << arr[i] << " ";
	}
	return;
}

//12. �������� �迭 //arr1 �� �̿��ؼ�, ������ ���� �Ʒ��� ���� ����ϼ���.
	//10 20 30 40

	int num1 = 10, num2 = 20, num3 = 30, num4 = 40;
	int* arr1[4] = { &num1, &num2, &num3, &num4 };   // ������ �迭

	
	cout << *arr1 << " " << *(arr1 + 1) << " " << *(arr1 + 2) << " " << *(arr1 + 3) << endl;


//13. �迭�� ������ //�Ʒ��� ���� ����ϼ���.
	//1 2 3 4
	//5 6 7 8

	int arr2d[2][4] = { 1, 2, 3, 4, 5, 6, 7, 8 };
	int(*parr)[4] = arr2d;							 // �迭 ������

	cout << *(*(parr))     << " " << *(*(parr) + 1)     << " " << *(*(parr) + 2)     << " " << *(*(parr) + 3)	  << endl;
	cout << *(*(parr + 1)) << " " << *(*(parr + 1) + 1) << " " << *(*(parr + 1) + 2) << " " << *(*(parr + 1) + 3) << endl;

//14. ���� ������ //����ϴ� �Լ��� ����� ������.

	#include <iostream>
	using namespace std;
	*/
	void PrintStr(const char** strArr);
	void ShowString(const char* str);
	int MyCompare(const void* str1, const void* str2);

	//int main(void)
	//{
	//	const char* strArr[3] = { "Simple", "String", "Array" };

	//	printf("%s \n", strArr[0]); //const char * �� �ּ�
	//	printf("%s \n", strArr[2]);
	//	printf("%d \n", sizeof(strArr[0]));	// 8 - �����ʹ� �ּҰ��� ���� ������ � �ڷ����� �ּҸ� ��� 8 ����Ʈ�� �����ϴ�

	//	PrintStr(strArr);		//����ϼ���.

	//	void (*ptr)(const char*);

	//	ptr = ShowString;
	//	ptr("STUPID");


	//	int aList[5] = { 30, 10, 40, 50, 20 };
	//	int i = 0;

	//	qsort(aList, 5, sizeof(int), MyCompare);

	//	for (i = 0; i < 5; ++i)
	//		printf("%d\t", aList[i]);
	//	return 0;

	//}
	//int MyCompare(const void* str1, const void* str2)
	//{
	//	if (*((int*)str1) > *((int*)str2))
	//		return 1;
	//	else if ((*((int*)str1) < *((int*)str2)))
	//		return -1;
	//	else
	//		return 0;
	//}


	void PrintStr(const char** strArr)
	{
		for(int i = 0; i < 3; i++)
		{
			cout << *(strArr + i) << " ";
		}
		cout << endl;

		cout << sizeof(*(strArr)) << endl;

		return;
	}


//15. �Լ������� //�Լ��� �Լ������ͷ� �����ؼ� ����� ������.

	void ShowString(const char * str)
	{
		printf("%s \n", str);
	}


	
//16. MyCompare �Լ��� �ۼ��ϼ���. (void*, �ݹ�)

	/*int main(void)
	{
		int aList[5] = { 30, 10, 40, 50, 20 };
		int i = 0;

		qsort(aList, 5, sizeof(int), MyCompare);

		for (i = 0; i < 5; ++i)
			printf("%d\t", aList[i]);
		return 0;
	}*/

	int MyCompare(const void* str1, const void* str2)
	{
		if (*((int*)str1) > *((int*)str2))
			return 1;
		else if ((*((int*)str1) < *((int*)str2)))
			return -1;
		else
			return 0;
	}

//17. ���ڿ��� ���̸� ���ϴ� �Լ��� ���弼��.

	//#include <stdio.h>

	int str_length(char* str);

	int main()
	{
		char str[] = { "What is your name?" };
		printf("�� ���ڿ��� ���� : %d \n", str_length(str));
		return 0;
	}

	int str_length(char* str)
	{
		int count = 0;
		while (*str != '\0')
		{
			str = str + 1;
			count++;
		}
		
		// null ���ڵ� �����ش�
		count++;

		return count;
	}
