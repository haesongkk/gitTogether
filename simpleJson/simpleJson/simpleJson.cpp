#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
using namespace std;
using namespace nlohmann;

struct Object
{
    string name;
    int level;
    int x, y, z;
    int a, b;
};

vector<Object> objects;

void Save(string str)
{
    json data;
    
    while (true)
    {
        json obj_data;

        string name;
        int level;
        int x, y, z;
        int a, b;

        cout << "이름 : ";
        cin >> name;
        if (name == "end") break;

        cout << "레벨 : ";
        cin >> level;

        cout << "위치 : ";
        cin >> x;
        cin >> y;
        cin >> z;

        cout << "기타 : ";
        cin >> a;
        cin >> b;

        obj_data["name"] = name;
        obj_data["level"].push_back(level);
        obj_data["pos"] = { x,y,z };
        obj_data["etc"].push_back(a);
        obj_data["etc"].push_back(b);

        data.push_back(obj_data);
    }
   
    ofstream file(str);
    assert(file.is_open());

    file << data.dump(2);
    file.close();
}

void Load(string str)
{
    ifstream file(str);
    json data;
    file >> data;

    for (const auto& obj_data : data)
    {
        if (obj_data.contains("name"))
        {
            string str = obj_data["name"];
            cout << "이름 : " << obj_data["name"] << endl;
            cout << "이름 : " << str << endl;
            cout << "레벨 : " << obj_data["level"] << endl;
            cout << "위치 : " << obj_data["pos"][0] << ' ';
            cout << obj_data["pos"][1] << ' ';
            cout << obj_data["pos"][2] << endl;
        }
    }
    file.close();
}
int main()
{
    Save("default.json");
    //Load("default.json");
}

