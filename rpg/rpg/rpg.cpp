#include <iostream>
#include <cstring>
#include <Windows.h>
using namespace std;

struct point
{
    int xPos;
    int yPos;
};

struct nameCard
{
    char name[20];
    int age;
};
struct Player
{
    char name[20] = { 0 };
    char weapon[20] = { 0 };
    char ability[20] = { 0 };
};
Player setPlayer()
{
    Player p;
    cout << "이름 : ";
    cin >> p.name;
    cout << "무기 : ";
    cin >> p.weapon;
    cout << "기술 : ";
    cin >> p.ability;
    return p;
}
void getPlayer(Player p)
{
    cout << "이름 : " << p.name << endl;
    cout << "무기 : " << p.weapon << endl;
    cout << "기술 : " << p.ability << endl;
}
int main()
{
    cout << "플레이어 수를 입력하시오." << endl;
    int playerNum;
    cin >> playerNum;
    Player* player = new Player[playerNum];

    for (int i = 0; i < playerNum; i++)
    {
        cout << "플레이어 " << i + 1 << " 정보" << endl;
        player[i] = setPlayer();
    }

    char input[20] = { 0 };
    while (true)
    {
        cout << "확인할 플레이어 : ";
        cin >> input;
        for (int i = 0; i < playerNum; i++)
        {
            if (strcmp(input, player[i].name) == 0)   getPlayer(player[i]);
            else cout << "finding" << endl;
            Sleep(500);
        }
    }

}