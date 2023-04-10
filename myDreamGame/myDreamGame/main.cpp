#include "main.h"

using namespace std;



///
/// 04. 10
/// 커서 받기 gotoxy, initConsole 등
/// 창 사이즈 설정
/// ***더블 버퍼링 찾아보기***
/// 






// 화면 버퍼값 받아올 구조체 (사각형마냥 값을 저장한다고 함)
SMALL_RECT consoleScreenSize;
// UI 최대 사이즈 설정값
SMALL_RECT _UImaxSize;




/// <summary>
/// 초기화
/// </summary>
void initConsole()
{
	// 화면 초기화
	system("cls");

	// 커서 숨겨버리기 (안깜빡임! 안보임!)
	HANDLE consonleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO consoleCursor;
	consoleCursor.bVisible = 0;
	consoleCursor.dwSize = 1;
	SetConsoleCursorInfo(consonleHandle, &consoleCursor);

	// 화면 사이즈 받아오기 (버퍼값 받아서 ScreenSize 에 대입)
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	consoleScreenSize.Left = csbi.srWindow.Left;
	consoleScreenSize.Right = csbi.srWindow.Right;
	consoleScreenSize.Bottom = csbi.srWindow.Bottom;
	consoleScreenSize.Top = csbi.srWindow.Top;

	_UImaxSize.Left = consoleScreenSize.Left + 2;
	_UImaxSize.Right = consoleScreenSize.Right - 2;
	_UImaxSize.Bottom = consoleScreenSize.Bottom - 2;
	_UImaxSize.Top = consoleScreenSize.Top + 2;

}




/// <summary>
/// 커서 위치 이동하는 함수
/// y 좌표는 아래로 갈수록 증가, 원점은 좌측 상단
/// </summary>
/// <param name="x">이동할 x좌표</param>
/// <param name="y">이동할 y좌표</param>
void gotoXY(int x, int y)
{
	COORD cursor = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursor);
}











///
/// 04. 10
/// 인풋 값을 받으면
/// 색칠이 되는 UI
/// 거의 전체화면
/// 




/// <summary>
/// 색깔 팔레트
/// </summary>
const int color_black = 0;
const int color_white = 15;
const int color_green = 10;
const int color_red = 12;
const int color_yellow = 6;
const int color_dark_white = 7;
const int color_blue = 9;


// 배경 색, 글꼴 색 지정
void setColor(int backColor, int textColor = color_white)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (backColor << 4) + textColor);
}




/// <summary>
/// 화살표 출력 원본 (UP KEY)
/// </summary>
const int arr[5][5] =
{
	{0,0,1,0,0},
	{0,1,1,1,0},
	{1,0,1,0,1},
	{0,0,1,0,0},
	{0,0,1,0,0}
};


/// <summary>
/// 너무 지저분하고 다양한 사용(나중에 위에 올라올 노트 출력에도 써야하니)
/// 따로 빼줘볼까
/// 상하좌우 화살표 출력 함수이다
/// </summary>
/// <param name="pos">좌표 정보를 담고있는 구조체</param>
/// <param name="color">무슨 색으로 출력할깡?</param>
void DrawUpArrow(COORD pos, int color)
{
	
	// arr[i][j] up
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			// j는 가로 출력 (x), i는 세로 출력을 담당 (y)
			// 가로는 세로보다 두배 출력해야함.. -> j*2
			gotoXY(pos.X + 2 * j, pos.Y + i);
			//if (arr[i][j] == 0)
			//{
			//	setColor(color_black);
			//	//printf("  ");
			//}	 없어도 무관
			if (arr[i][j] == 1)
			{
				setColor(color);
				printf("  ");
			}
		}
	}
}

void DrawLeftArrow(COORD pos, int color)
{
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			gotoXY(pos.X + 2 * i, pos.Y + j);

			if (arr[i][j] == 1)
			{
				setColor(color);
				printf("  ");
			}
		}
	}
}

void DrawRightArrow(COORD pos, int color)
{
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			gotoXY(pos.X + 8 - 2 * i, pos.Y + j);

			if (arr[i][j] == 1)
			{
				setColor(color);
				printf("  ");
			}
		}
	}
}

void DrawDownArrow(COORD pos, int color)
{
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			gotoXY(pos.X + 2 * j, pos.Y + 4 - i);

			if (arr[i][j] == 1)
			{
				setColor(color);
				printf("  ");
			}
		}
	}
}






/// 
/// 배열로 출력하자
/// 0 이면 넘김, 1이면 공백을 출력하고 배경 색상 변경
/// 인터페이스 상 키를 그린다! 키가 입력되면 색상이 활성화 됨
void DrawKeyInterface()
{
	// 인터페이스 출력 좌표 지정 
	// 5x5 크기의 화살표. 출력의 시작은 좌측 상단 끝
	const int x = 8;
	const int y = 1;

	const int padding = 16;

	const COORD posLeft = { x + padding * 0, y };
	const COORD posDown = { x + padding * 1, y };
	const COORD posUp = { x + padding * 2, y };
	const COORD posRight = { x + padding * 3, y };

	/// 안 누른 기본 상태는 흰색
	// Up
	int colorUp = color_dark_white;
	// Down
	int colorDown = color_dark_white;
	// Left
	int colorLeft = color_dark_white;
	// Right
	int colorRight = color_dark_white;


	// 키가 눌리면?
	if (GetKeyTable(UP))
	{
		// 한번만 입력받기 위해 false 처리
		SetKeyTable(UP, false);
		// 색상을 빨강으로 바꾼다
		colorUp = color_red;
	}

	DrawUpArrow(posUp, colorUp);


	if (GetKeyTable(DOWN))
	{
		SetKeyTable(DOWN, false);
		// 색상을 green으로 바꾼다
		colorDown = color_green;
	}
	DrawDownArrow(posDown, colorDown);

	if (GetKeyTable(LEFT))
	{
		SetKeyTable(LEFT, false);
		// 색상을 skyblue으로 바꾼다
		colorLeft = color_blue;
	}
	DrawLeftArrow(posLeft, colorLeft);

	if (GetKeyTable(RIGHT))
	{
		SetKeyTable(RIGHT, false);
		// 색상을 yellow으로 바꾼다
		colorRight = color_yellow;
	}
	DrawRightArrow(posRight, colorRight);


	/// 다른 함수로 정리함
	{
		// 이차원 배열을 설정하고 화살표 모양을 만듦
	//int arr[5][5] =
	//{
	//	{0,0,1,0,0},
	//	{0,1,1,1,0},
	//	{1,0,1,0,1},
	//	{0,0,1,0,0},
	//	{0,0,1,0,0}
	//};

	// arr[i][j] up
	//for (int i = 0; i < 5; i++)
	//{
	//	for (int j = 0; j < 5; j++)
	//	{
	//		// j는 가로 출력 (x), i는 세로 출력을 담당 (y)
	//		// 가로는 세로보다 두배 출력해야함.. -> j*2
	//		gotoXY(x + 2 * j, y + i);
	//		//if (arr[i][j] == 0)
	//		//{
	//		//	setColor(color_black);
	//		//	//printf("  ");
	//		//}	 없어도 무관
	//		if (arr[i][j] == 1)
	//		{
	//			setColor(color);
	//			printf("  ");
	//		}
	//	}
	//}

	// arr[i][j] left
	// y=x 대칭 (윈도우의 y축은 아래쪽 증가)
	//for (int i = 0; i < 5; i++)
	//{
	//	for (int j = 0; j < 5; j++)
	//	{
	//		gotoXY(x +  2*i, y + j);
	//		
	//		if (arr[i][j] == 1)
	//		{
	//			setColor(color);
	//			printf("  ");
	//		}
	//	}
	//}


	// arr[i][j] right
	// left의 x= 5 대칭 (가로줄은 두칸단위임!!!!!) -> 그래서 (5-1)가 아니라 (10-2)
	//for (int i = 0; i < 5; i++)
	//{
	//	for (int j = 0; j < 5; j++)
	//	{
	//		gotoXY(x + 8 - 2 * i, y + j);
	//		
	//		if (arr[i][j] == 1)
	//		{
	//			setColor(color);
	//			printf("  ");
	//		}
	//	}
	//}
	//

	// arr[i][j] 
	// 배열은 항상 0번째부터 출력됨을 명심하자!! (5 - 1) = 4
	//for (int i = 0; i < 5; i++)
	//{
	//	for (int j = 0; j < 5; j++)
	//	{
	//		gotoXY(x + 2 * j, y + 4 - i);
	//		
	//		if (arr[i][j] == 1)
	//		{
	//			setColor(color);
	//			printf("  ");
	//		}
	//	}
	//}


	/*setColor(color_black);
	printf("    ");
	setColor(color, 15);
	printf("  \n");

	setColor(color_black);
	printf("  ");
	setColor(color, 15);
	printf("  ");
	setColor(color_black);
	printf("  ");
	setColor(color, 15);
	printf("  \n");

	setColor(color, 15);
	printf("  ");
	setColor(color_black);
	printf("      ");
	setColor(color, 15);
	printf("  ");

	setColor(color_black);*/
	}
}





void Interface()
{

	
}



























int main()
{
	initConsole();
	while (1)
	{
		UpdateInput();
		DrawKeyInterface();

		//system("cls");
	}
}