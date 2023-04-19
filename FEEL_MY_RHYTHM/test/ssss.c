#include <stdio.h>
#include <windows.h>
#include <stdbool.h>
#include <string.h>

// 아스키 아트가 존재하는 txt 파일을 찾아 버퍼에 저장한다
void FindAsciiArt(const char* asciiArtFilePath)
{
	FILE* fp;
	char buffer[256];
	int line = 0;
	int index = 0;
	char** asciiArt = (char**)malloc(sizeof(char*) * 4);
	int artCount = 0;

	// 네 개의 아스키 아트 파일 읽기
	for (int i = 0; i < 4; i++) {
		asciiArt[i] = (char*)malloc(sizeof(char) * 5000);
		snprintf(buffer, 256, "%s%d.txt", asciiArtFilePath, i);
		errno_t err = fopen_s(&fp, buffer, "r");
		if (err != 0) {
			//printf("아스키 아트 파일을 찾을 수 없습니다.");
			exit(1);
		}
		int readSize = fread(asciiArt[i], sizeof(char), 5000, fp);
		asciiArt[i][readSize] = '\0'; // 마지막에 널 문자 추가
		fclose(fp);
		artCount++;
	}
}


// 버퍼에 저장된 아스키 아트를 출력한다
//void PrintAsciiArt(char* asciiArt[], int x, int y, int artCount)
//{
//
//	HANDLE hConsole = GetScreenHandle();
//	COORD cursorPos;
//
//	for (int i = 0; i < artCount; i++)
//	{
//		int line = 0;
//		int index = 0;
//		int posX = x;
//		int posY = y;   // posX와 posY에 현재 출력 위치 저장
//		//printf("\033[%d;%dH", y + line++, x); // 커서 위치 지정
//		while (asciiArt[i][index] != '\0') {
//			//putchar(asciiArt[i][index++]);
//			ScreenPrint(y + line++, x, asciiArt[i][index++], 1);
//			if (asciiArt[i][index] == '\n') {
//				index++;
//				posY++;   // 다음 줄로 이동할 때마다 증가
//				printf("\033[%d;%dH", posY, posX); // 다음 줄로 이동
//			}
//		}
//		//Sleep(delay); // 딜레이 시간만큼 대기
//	}
//
//
//	// 동적할당 해제
//	for (int i = 0; i < 4; i++) {
//		free(asciiArt[i]);
//	}
//	free(asciiArt);
//
//}

int main()
{
	
		system("cls");

		while (1)
		{
			char* asciiArtFilePath = "C:\\Users\\User\\Documents\\GitHub\\gitTogether\\FEEL_MY_RHYTHM\\x64\\Debug\\ascii_art_"; // 아스키 파일 경로
			int x = 120;
			int y = 10;
			int delay = 100;

			FindAsciiArt(asciiArtFilePath);
		}
	
}