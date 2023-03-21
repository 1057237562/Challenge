#include <bits/stdc++.h>
using namespace std;

#include "utils.h"
#include "workingtable.h"
#include "robotbrain.h"

vector<string> maps;

WorkingTable table[51];

RobotBrain robot[4];


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
    int money;
    cin >> money;
    int numOfWorkingTable;
    cin >> numOfWorkingTable;
    for (int i = 0; i < numOfWorkingTable; i++)
    {
        cin >> table[i].type >> table[i].position.x >> table[i].position.y >> table[i].remainTime >> table[i].resourceState >> table[i].productState;
    }
    for (int i = 0; i < 4; i++)
    {
        cin >> robot[i].workingTableID >> robot[i].carriedItemType >> robot[i].timeValMultiplier >> robot[i].collisionValMultiplier >> robot[i].angluarVelocity >> robot[i].velocity.x >> robot[i].velocity.y >> robot[i].direction >> robot[i].position.x >> robot[i].position.y;
    }
    readUntilOK();
}


int main()
{
    initMap();
    puts("OK");
    fflush(stdout);
    int frameID;
    while (scanf("%d", &frameID) != EOF)
    {
        readFrame();
        printf("%d\n", frameID);
        for (int robotId = 0; robotId < 4; robotId++)
        {
            pair<int, double> result = robot[robotId].travel();
            printf("forward %d %d\n", robotId, result.first);
            printf("rotate %d %f\n", robotId, result.second);
        }
        printf("OK\n", frameID);
        fflush(stdout);
    }
    return 0;
}
