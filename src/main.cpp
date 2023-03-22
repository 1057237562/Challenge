#include <bits/stdc++.h>
using namespace std;

#include "utils.h"
#include "workingtable.h"
#include "robotbrain.h"

vector<string> maps;

WorkingTable table[51];

RobotBrain robot[4];

int currentFrameID;
int numOfWorkingTable;
int totalMoney;

bool initMap()
{
    string line;
    while (getline(cin, line))
    {
        if (line[0] == 'O' && line[1] == 'K')
        {
            return true;
        }
        maps.push_back(line);
    }
    return false;
}

bool readUntilOK()
{
    char line[1024];
    while (fgets(line, sizeof line, stdin))
    {
        if (line[0] == 'O' && line[1] == 'K')
        {
            return true;
        }
    }
    return false;
}

void readFrame()
{
    cin >> totalMoney;
    cin >> numOfWorkingTable;
    for (int i = 0; i < numOfWorkingTable; i++)
    {
        cin >> table[i].type >> table[i].position.x >> table[i].position.y 
            >> table[i].remainTime >> table[i].resourceState >> table[i].productState;
    }
    for (int i = 0; i < 4; i++)
    {
        cin >> robot[i].workingTableID >> robot[i].carriedItemType >> robot[i].timeValMultiplier 
            >> robot[i].collisionValMultiplier >> robot[i].angluarVelocity >> robot[i].velocity.x 
            >> robot[i].velocity.y >> robot[i].direction >> robot[i].position.x >> robot[i].position.y;
    }
    readUntilOK();
}

inline void beginFrame(const int &frameID)
{
    printf("%d\n", frameID);
}

/*
直接把动作打印出去
示例用法printRobotInst(Foward,0,2,3);
*/
inline void printRobotInst(const RobotInst &inst, const int robotID, const double &arg1 = NAN, const double &arg2 = 0)
{
    if (arg1 == NAN)
    {
        printf("%s %d\n", RobotInstString[inst], robotID);
    }
    else
    {
        printf("%s %d %lf %lf\n", RobotInstString[inst], robotID, arg1, arg2);
    }
}

inline void endFrame()
{
    printf("OK\n");
}

int main()
{
    initMap();
    puts("OK");
    fflush(stdout);
    while (scanf("%d", &currentFrameID) != EOF)
    {
        readFrame();
        printf("%d\n", currentFrameID);
        for (int robotId = 0; robotId < 4; robotId++)
        {
            int f;
            double r;
            robot[robotId].setDestination({0, 0});
            robot[robotId].travel(f, r);
            printf("forward %d %d\n", robotId, f);
            printf("rotate %d %f\n", robotId, r);
        }
        printf("OK\n");
        fflush(stdout);
    }
    return 0;
}

//syq test