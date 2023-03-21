#include <bits/stdc++.h>
using namespace std;

vector<string> maps;

struct Pos
{
    float x, y;

    Pos operator+(const Pos &pos)
    {
        return {x + pos.x, y + pos.y};
    }

    Pos operator-(const Pos &pos)
    {
        return {x - pos.x, y - pos.y};
    }

    float euclidDistance(const Pos &pos)
    {
        return sqrtf(powf(x - pos.x, 2) + powf(y - pos.y, 2));
    }

    float manhattanDistance(const Pos &pos)
    {
        return abs(x - pos.x) + abs(y - pos.y);
    }
};

struct WorkingTable
{
    int type;
    Pos position;
    int remainTime;
    int resourceState;
    bool productState;
} table[51];

class RobotBrain
{
private:
    Pos destination;

public:
    int workingTableID;
    int carriedItemType;
    float timeValMultiplier;
    float collisionValMultiplier;
    float angluarVelocity;
    Pos velocity;
    float direction;
    Pos position;

    pair<int, double> travel()
    {
        Pos diff = destination - position;

        return make_pair(3, 1.5);
    }
};

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

void readFrame()
{
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
    string str;
    getline(cin, str);
    if (str[0] = 'O' && str[1] == 'K')
        return;
}

int main()
{
    initMap();
    puts("OK");
    fflush(stdout);
    int frameID;
    while (scanf("%d", &frameID) != EOF)
    {
        int money;
        scanf("%d", &money);
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
