#include <bits/stdc++.h>

#define M_PI 3.14159265358979323846

using namespace std;

vector<string> maps;

struct Pos
{
    double x, y;

    Pos operator+(const Pos &pos) const
    {
        return {x + pos.x, y + pos.y};
    }

    Pos operator-(const Pos &pos) const
    {
        return {x - pos.x, y - pos.y};
    }

    Pos operator*(const double &mul) const
    {
        return {x * mul, y * mul};
    }

    Pos operator/(const double &mul) const
    {
        return {x / mul, y / mul};
    }

    double euclidDistance(const Pos &pos) const
    {
        return sqrtf(powf(x - pos.x, 2) + powf(y - pos.y, 2));
    }

    double manhattanDistance(const Pos &pos) const
    {
        return abs(x - pos.x) + abs(y - pos.y);
    }

    static Pos fromRadian(double radian)
    {
        return {cosf(radian), sin(radian)};
    }

    double length() const
    {
        return sqrtf(powf(x, 2) + powf(y, 2));
    }

    Pos normalize() const
    {
        double l = length();
        return {x / l, y / l};
    }

    double dot(const Pos &b) const
    {
        return x * b.x + y * b.y;
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
public:
    int workingTableID;
    int carriedItemType;
    float timeValMultiplier;
    float collisionValMultiplier;
    float angluarVelocity;
    Pos velocity;
    float direction;
    Pos position;

    Pos destination;

    double data[16] = {0.861136, -1.93058, -2.10806, 3.34344, -0.863897, 1.27084, 4.77327, 2.83506, 4.38575, -0.367908, 2.4501, 2.92614, 2.25585, -2.45845, 6.02638, 0.897712};

    void travel(int &f, double &r)
    {
        Pos diff = destination - position;
        double n1 = diff.length();
        double directionDiff = acosf(diff.normalize().dot(Pos::fromRadian(direction)));
        float arg[4];
        for (int i = 0; i < 4; i++)
        {
            arg[i] = data[i * 2] * n1 + data[i * 2 + 1] * directionDiff;
        }
        f = r = 0;
        for (int i = 0; i < 4; i++)
        {
            f += arg[i] * data[8 + i * 2];
            r += arg[i] * data[9 + i * 2];
        }
        f = min(6, max(-2, f));
        r = min(M_PI, max(-M_PI, r));
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
            int f;
            double r;
            robot[robotId].destination = {0, 0};
            robot[robotId].travel(f, r);
            printf("forward %d %d\n", robotId, f);
            printf("rotate %d %f\n", robotId, r);
        }
        printf("OK\n", frameID);
        fflush(stdout);
    }
    return 0;
}
