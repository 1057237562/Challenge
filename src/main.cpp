#include <iostream>
#include <algorithm>
#include <math.h>
using namespace std;

vector<string> map;

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
    float linearVelocity;
    float direction;
    Pos currentPosition;

    virtual pair<int, double> travel()
    {
        return make_pair(0, 0);
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
        map.push_back(line);
    }
    return false;
}

void readLine()
{
    int frame, money;
    cin >> frame >> money;
    int numOfWorkingTable;
    cin >> numOfWorkingTable;
    for (int i = 0; i < numOfWorkingTable; i++)
    {
    }
}

int main()
{
    initMap();
    puts("OK");
    fflush(stdout);
    int frameID;
    while (scanf("%d", &frameID) != EOF)
    {
        readUntilOK();
        printf("%d\n", frameID);
        int lineSpeed = 3;
        double angleSpeed = 1.5;
        for (int robotId = 0; robotId < 4; robotId++)
        {
            printf("forward %d %d\n", robotId, lineSpeed);
            printf("rotate %d %f\n", robotId, angleSpeed);
        }
        printf("OK\n", frameID);
        fflush(stdout);
    }
    return 0;
}
