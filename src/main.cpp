#include <bits/stdc++.h>

#define M_PI 3.14159265358979323846

using namespace std;

enum ACTION
{
    BUY,
    SELL
};

char maps[100][100];
int numOfWorkingTable;

ofstream LOGGER("output.log", ios::out);
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

    bool book = false;
    int resultState = 0;
} table[51];

class Scheduler
{
public:
    virtual void decide()
    {
    }
};

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

    list<Pos> destination;
    queue<int> action;

    double data[16] = {0.37761, -2.5042, -2.13474, 4.13467, -1.0751, 1.28702, 5.05836, 3.1689, 4.75077, 0.166093, 3.04023, 3.14458, 1.92327, -1.96513, 6.50947, 0.894788};

    virtual void travel(int &f, double &r)
    {
    }

    double distance() const
    {
        return destination.front().euclidDistance(position);
    }
};

class GARobotBrain : public RobotBrain
{
public:
    void travel(int &f, double &r)
    {
        Pos diff = destination.front() - position;
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

RobotBrain *robot[4] = {new GARobotBrain(), new GARobotBrain(), new GARobotBrain(), new GARobotBrain()};

const int purchase[10] = {0, 0, 0, 0, 6, 10, 12, 112, 128, 254}; // Bitmasked
const int craftTime[10] = {0, 50, 50, 50, 500, 500, 500, 1000, 1, 1};

const int cost[8] = {0, 3000, 4400, 5800, 15400, 17200, 19200, 76000};
const int value[8] = {0, 6000, 7600, 9200, 22500, 25000, 27500, 105000};

double f(double x, double maxX, double minRate)
{
    if (x < maxX)
    {
        double div = 1 - x / maxX;
        return (1 - sqrt(1 - div * div)) * (1 - minRate) + minRate;
    }
    else
    {
        return minRate;
    }
}

struct Quadruple
{
    int a, b;
    double c, d;
};

class GreedySchedulerV1 : public Scheduler
{
public:
    void decide()
    {
        for (int robotId = 0; robotId < 4; robotId++)
        {
            if (!robot[robotId]->destination.empty())
                continue;
            vector<Quadruple> possibleRoute;
            for (int i = 0; i < numOfWorkingTable; i++)
            {
                if (table[i].book)
                    continue;
                if (table[i].productState)
                {
                    double minTimeCost = DBL_MAX;
                    int buyerId = -1;
                    for (int j = 0; j < numOfWorkingTable; j++)
                    {
                        if (table[j].book)
                            continue;
                        if ((purchase[table[j].type] & (1 << table[i].type)) != 0 && (table[j].resourceState & (1 << table[i].type)) == 0 && (table[j].resultState & (1 << table[i].type)) == 0)
                        {
                            double estimateTimeCost = table[j].position.euclidDistance(table[i].position) / 6.0 * 50.0;
                            if (estimateTimeCost < minTimeCost)
                            {
                                minTimeCost = estimateTimeCost;
                                buyerId = j;
                            }
                        }
                    }
                    double expectedBenefit = (value[table[i].type] - cost[table[i].type]) * f(minTimeCost, 9000, 0.8);
                    if (purchase[table[buyerId].type] == table[buyerId].resourceState | (1 << table[i].type))
                    {
                        expectedBenefit += value[table[buyerId].type] - cost[table[buyerId].type];
                    }
                    minTimeCost += table[i].remainTime;
                    // minTimeCost -= craftTime[table[buyerId].type];
                    if (buyerId != -1)
                        possibleRoute.push_back({i, buyerId, minTimeCost, expectedBenefit});
                }
            }

            double maxBenefit = 0.0f;
            Quadruple ans;
            if (possibleRoute.empty())
                continue;
            for (Quadruple quad : possibleRoute)
            {
                bool closest = true;
                for (int i = 0; i < 4; i++)
                {
                    if (!robot[i]->destination.empty())
                    {
                        if (table[quad.a].position.euclidDistance(robot[robotId]->position) > table[quad.a].position.euclidDistance(robot[i]->destination.front()))
                        {
                            closest = false;
                            break;
                        }
                    }
                    else
                    {
                        if (table[quad.a].position.euclidDistance(robot[robotId]->position) > table[quad.a].position.euclidDistance(robot[i]->position))
                        {
                            closest = false;
                            break;
                        }
                    }
                }
                if (!closest)
                    continue;
                double estimateReachTime = robot[robotId]->position.euclidDistance(table[quad.a].position) * 6.0 / 50.0;
                double estimateBenefit = quad.d / (quad.c + estimateReachTime);
                if (maxBenefit < estimateBenefit)
                {
                    maxBenefit = estimateBenefit;
                    ans = quad;
                }
            }

            table[ans.a].book = true;
            table[ans.b].resultState |= (1 << table[ans.a].type);

            LOGGER << "Assign Job : " << robotId << " BUY " << table[ans.a].type << " SELL " << table[ans.b].type << ":" << (purchase[table[ans.a].type] & (1 << table[ans.b].type)) << endl;
            LOGGER << "After bit mask : " << table[ans.b].resultState << endl;

            robot[robotId]->destination.push_back(table[ans.a].position);
            robot[robotId]->destination.push_back(table[ans.b].position);
            robot[robotId]->action.push(BUY);
            robot[robotId]->action.push(SELL);
        }
    }
};

Scheduler *scheduler = new GreedySchedulerV1();

bool init()
{
    for (int i = 0; i < 100; i++)
    {
        for (int j = 0; j < 100; j++)
        {
            maps[i][99 - j] = getchar();
        }
    }
    /*for (int i = 0; i < 4; i++)
    {
        scheduler->idleRobots.push_back(i);
    }*/
    string line;
    while (getline(cin, line))
    {
        if (line[0] == 'O' && line[1] == 'K')
        {
            return true;
        }
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
    cin >> numOfWorkingTable;
    for (int i = 0; i < numOfWorkingTable; i++)
    {
        // LOGGER << table[i].type << " " << table[i].productState << " " << table[i].book << endl;
        cin >> table[i].type >> table[i].position.x >> table[i].position.y >> table[i].remainTime >> table[i].resourceState >> table[i].productState;
    }
    for (int i = 0; i < 4; i++)
    {
        cin >> robot[i]->workingTableID >> robot[i]->carriedItemType >> robot[i]->timeValMultiplier >> robot[i]->collisionValMultiplier >> robot[i]->angluarVelocity >> robot[i]->velocity.x >> robot[i]->velocity.y >> robot[i]->direction >> robot[i]->position.x >> robot[i]->position.y;
    }
    readUntilOK();
}

int main()
{
    init();
    puts("OK");
    fflush(stdout);
    int frameID;
    while (scanf("%d", &frameID) != EOF)
    {
        readFrame();
        LOGGER << frameID << endl;
        printf("%d\n", frameID);
        for (int robotId = 0; robotId < 4; robotId++)
        {
            if (robot[robotId]->distance() <= 0.4)
            {
                switch (robot[robotId]->action.front())
                {
                case BUY:
                    printf("buy %d\n", robotId);
                    table[robot[robotId]->workingTableID].book = false;
                    break;

                case SELL:
                    printf("sell %d\n", robotId);
                    table[robot[robotId]->workingTableID].resultState ^= (1 << robot[robotId]->carriedItemType);
                    table[robot[robotId]->workingTableID].resourceState |= (1 << robot[robotId]->carriedItemType);
                    break;
                }
                robot[robotId]->destination.pop_front();
                robot[robotId]->action.pop();
                /*if (robot[robotId]->destination.empty())
                {
                    scheduler->idleRobots.push_back(robotId);
                }*/
            }
        }
        scheduler->decide();
        for (int robotId = 0; robotId < 4; robotId++)
        {
            int f;
            double r;
            robot[robotId]->travel(f, r);
            printf("forward %d %d\n", robotId, f);
            printf("rotate %d %f\n", robotId, r);
        }
        printf("OK\n", frameID);
        fflush(stdout);
    }
    return 0;
}
