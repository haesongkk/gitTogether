//포인터 배열 문제

#include <iostream>
#include <string>
using namespace std;

//int main()
//{
//
//	//다음 코드 실행의 예상되는 결과는? **
//	char str[50] = "I like C programming";
//	str[8] = '\0';
//	cout << str << endl;
//	/// I like C
//
//	//다음 코드 실행의 예상되는 결과는? ** 
//	int arr[3] = { 11, 22, 33 };
//	int* ptr = arr;
//	cout << *(ptr + 2);
//	/// 33
//
//	//다음 코드의 실행 결과를 쓰시오. *
//	char arr2[] = "Hello World";
//	char* str1 = arr2;
//	cout << arr2[0] << *(str1 + 6) << endl;
//	/// HW
//
//	//아래의 포인터 배열을 for 문을 이용해 출력하는 코드를 작성하세요. *
//	const char* str2[] = { "seoul","paris","london" };
//
//	for (int i = 0; i < 3; i++)
//	{
//		cout << *(str2 + i) << " ";
//	}
//	cout << endl;
//
//	// 결과는?
//	char str_a[] = "abc";
//	char str_b[] = "abc";
//	printf("%d", str_a == str_b);
//	/// 배열의 주소를 비교하고 있으므로 다르다 -> 0
//
//	return 0;
//}

/*

//1. 다음과 같은 다차원 배열을 바르게 선언한 것은?
	1	2	3	4
	5	6	7	8

   (1) int Array[2][2];
   (2) int Array[2][4];
   (3) int Array[4][2];
   (4) int Array[4][4];
   
   답 : 2

//2. 다음 코드의 실행 후 예상되는 결과는? *

  int main(void)
  {
	int Array[3][3] = {
		{1, 2, 3},
		{4, 5, 6},
		{7, 8, 9}
	};

	cout << Array[1][2] << endl;
  }

  답 : 6

//3. ( )안에 들어갈 적당한 것을 고르시오  **

   int arr[10];      //arr의 포인터형은 ( int * )
   int * parr[20];  //parr의 포인터형은 (        )

   (1) int
   (2) int[]
   (3) int *
   (4) int **

   답 : 4

//4. 2차원 배열의 연산에 관한 코드입니다. 예상되는 출력은 얼마입니까? *

	int main(void)
	{
		int arr[3][2] = { 1,2,3,4,5,6 };
		cout << **(arr + 1);
		return 0;
	}

	답 : 12 34 56 -> 3 이 출력된다


//5. 함수로 배열을 전달하기 위해서는 배열의 주소값을 전달하는 방식을 취한다. **
   다음 중 A, B 자리에 들어갈 값으로 적당한 것은?

	void SimpleFunc(  A  )
	{
		printf(“%d %d”, param[0], param[1]);
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

   답 : 4

//6. Call-by-value 는 값을 전달하는 형태의 함수호출,
  Call-by-reference 는 주소 값을 전달하는 형태의 함수호출을 뜻합니다.
  다음 코드는 어떤 방식인가요?

	int Add(int num1, int num2)
	{
		return num1 + num2;
	}

	int main(void)
	{
		int num1 = 2, num2 = 3;
		int sum = Add(num1, num2);
	}

	답 : Add 함수는 값을 전달하고 있으므로 call-by-value

//7. 다음 중 A, B 자리에 들어갈 값으로 적당한 것은?

	int num;
	scanf(“%d”,   A   );
	char str[10];
	scanf(%s,  B   );

   (1) num, str
   (2) num, &str
   (3) &num, str
   (4) &num, &str

   답 : 3

//8. 다음 코드에 필요한 두 변수의 값을 교환하는 swap 함수를 작성해 보세요. *

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

//9. 다음 코드에 필요한 두 변수의 값을 교환하는 swap 함수를 작성해 보세요. **

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


//10. 절대값을 구하는 함수 작성하기  //참조, 포인터 두가지 사용

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
//11. 배열을 함수의 인자로 전달하여 출력하는 함수를 만들어 보세요.

#include <stdio.h>

int main(void)
{
	int arr[3] = { 1, 2, 3 };
	int len = sizeof(arr) / sizeof(int);
	ShowArray(arr, len);		//출력 함수 작성해 보자
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

//12. 포인터의 배열 //arr1 을 이용해서, 변수의 값을 아래와 같이 출력하세요.
	//10 20 30 40

	int num1 = 10, num2 = 20, num3 = 30, num4 = 40;
	int* arr1[4] = { &num1, &num2, &num3, &num4 };   // 포인터 배열

	
	cout << *arr1 << " " << *(arr1 + 1) << " " << *(arr1 + 2) << " " << *(arr1 + 3) << endl;


//13. 배열의 포인터 //아래와 같이 출력하세요.
	//1 2 3 4
	//5 6 7 8

	int arr2d[2][4] = { 1, 2, 3, 4, 5, 6, 7, 8 };
	int(*parr)[4] = arr2d;							 // 배열 포인터

	cout << *(*(parr))     << " " << *(*(parr) + 1)     << " " << *(*(parr) + 2)     << " " << *(*(parr) + 3)	  << endl;
	cout << *(*(parr + 1)) << " " << *(*(parr + 1) + 1) << " " << *(*(parr + 1) + 2) << " " << *(*(parr + 1) + 3) << endl;

//14. 이중 포인터 //출력하는 함수를 만들어 보세요.

	#include <iostream>
	using namespace std;
	*/
	void PrintStr(const char** strArr);
	void ShowString(const char* str);
	int MyCompare(const void* str1, const void* str2);

	//int main(void)
	//{
	//	const char* strArr[3] = { "Simple", "String", "Array" };

	//	printf("%s \n", strArr[0]); //const char * 형 주소
	//	printf("%s \n", strArr[2]);
	//	printf("%d \n", sizeof(strArr[0]));	// 8 - 포인터는 주소값이 들어가기 때문에 어떤 자료형의 주소를 담건 8 바이트로 동일하다

	//	PrintStr(strArr);		//출력하세요.

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


//15. 함수포인터 //함수를 함수포인터로 선언해서 사용해 보세요.

	void ShowString(const char * str)
	{
		printf("%s \n", str);
	}


	
//16. MyCompare 함수를 작성하세요. (void*, 콜백)

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

//17. 문자열의 길이를 구하는 함수를 만드세요.

	//#include <stdio.h>

	int str_length(char* str);

	int main()
	{
		char str[] = { "What is your name?" };
		printf("이 문자열의 길이 : %d \n", str_length(str));
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
		
		// null 문자도 세어준다
		count++;

		return count;
	}
