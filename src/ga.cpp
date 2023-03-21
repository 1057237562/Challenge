#include <bits/stdc++.h>

#define M_PI 3.14159265358979323846

using namespace std;

random_device rd;
mt19937 gen(rd());
uniform_real_distribution<float> desti(0.0, 100.0);

uniform_real_distribution<float> dis(-2.0, 6.0);

uniform_real_distribution<float> mut(-1.0, 1.0);

uniform_real_distribution<float> possibility(0.0, 1.0);

struct Pos
{
    float x, y;

    Pos operator+(const Pos &pos) const
    {
        return {x + pos.x, y + pos.y};
    }

    Pos operator-(const Pos &pos) const
    {
        return {x - pos.x, y - pos.y};
    }

    Pos operator*(const int &mul) const
    {
        return {x * mul, y * mul};
    }

    float euclidDistance(const Pos &pos) const
    {
        return sqrtf(powf(x - pos.x, 2) + powf(y - pos.y, 2));
    }

    float manhattanDistance(const Pos &pos) const
    {
        return abs(x - pos.x) + abs(y - pos.y);
    }

    static Pos fromRadian(float radian)
    {
        return {cosf(radian), sin(radian)};
    }
};

struct Mutator
{
    Pos dest;
    Pos pos;
    Pos velocity;
    float direction = 0.0;
    int cost = 0;

    double data[10];

    pair<int, double> travel()
    {
        Pos diff = dest - pos;
        int f = min(6, max(-2, (int)(data[0] * diff.x + data[1] * diff.y + data[2] * velocity.x + data[3] * velocity.y + data[4] * direction)));
        double r = min(M_PI, max(-M_PI, data[5] * diff.x + data[6] * diff.y + data[7] * velocity.x + data[8] * velocity.y + data[9] * direction));
        return make_pair(f, r);
    }

    void mutate()
    {
        int sbit = pick(gen);
        data[sbit] += mut(gen);
    }

    void cross(const Mutator &fa)
    {
        for (int i = 0; i < 10; i++)
        {
            if (binary_pick(gen))
            {
                data[i] = fa.data[i];
            }
        }
    }
};

bool cmpDis(const Mutator &a, const Mutator &b)
{
    return a.dest.euclidDistance(a.pos) < b.dest.euclidDistance(b.pos);
}

bool cmpCost(const Mutator &a, const Mutator &b)
{
    return a.cost < b.cost;
}

const int N = 100;
const int S = 10;

const double crossRate = 0.67;
const double mutateRate = 0.1;

Mutator mutators[N];

void init()
{

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            mutators[i].data[j] = dis(gen);
        }
    }
}

void process()
{

    Pos destination = {desti(gen), desti(gen)};
    for (int i = 0; i < N; i++)
    {
        mutators[i].dest = destination;
    }
    for (int i = 0; i < 50; i++)
    {
        for (int i = 0; i < N; i++)
        {
            pair<int, double> result = mutators[i].travel();
            mutators[i].direction += result.second;
            mutators[i].velocity = Pos::fromRadian(mutators[i].direction) * result.first;
            mutators[i].pos = mutators[i].pos + mutators[i].velocity;
        }
    }
    sort(mutators, mutators + N, cmpDis);
    for (int i = 0; i < N; i++)
    {
        mutators[i].cost += i;
    }
}

void SA()
{
    sort(mutators, mutators + N, cmpCost);
    for (int i = S; i < N; i++)
    {
        int mother = pick(gen);
        mutators[i] = mutators[mother];
        if (possibility(gen) > crossRate)
        {
            int father = pick(gen);
            mutators[i].cross(mutators[father]);
        }
        if (possibility(gen) > mutateRate)
        {
            mutators[i].mutate();
        }
        mutators[i].cost = 0;
    }
    for (int i = 0; i < S; i++)
        mutators[i].cost = 0;
}

int main(void)
{
    srand(time(0));
    int cnt = 0;
    while (++cnt)
    {
        cout << "Processing Batch : #" << cnt << endl;
        for (int i = 0; i < 10; i++)
        {
            for (int j = 0; j < 10; j++)
            {
                process();
                SA();
            }
            cout << "■";
        }
        cout << endl
             << "Best Cost Value : " << mutators[0].cost << endl;
    }
}