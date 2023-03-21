#include <bits/stdc++.h>

#define M_PI 3.14159265358979323846

using namespace std;

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

    float euclidDistance(const Pos &pos) const
    {
        return sqrtf(powf(x - pos.x, 2) + powf(y - pos.y, 2));
    }

    float manhattanDistance(const Pos &pos) const
    {
        return abs(x - pos.x) + abs(y - pos.y);
    }
};

struct Mutator
{
    Pos dest;
    Pos pos;
    Pos velocity;
    float direction = 0.0;

    double data[10];

    pair<int, double> travel()
    {
        Pos diff = dest - pos;
        int f = max(6, min(-2, (int)(data[0] * diff.x + data[1] * diff.y + data[2] * velocity.x + data[3] * velocity.y + data[4] * direction)));
        double r = max(M_PI, min(-M_PI, data[5] * diff.x + data[6] * diff.y + data[7] * velocity.x + data[8] * velocity.y + data[9] * direction));
        return make_pair(f, r);
    }

    bool operator<(const Mutator &mut)
    {
        return dest.euclidDistance(pos) < mut.dest.euclidDistance(mut.pos);
    }
};

const int N = 100;

Mutator mutators[N];

random_device rd;
mt19937 gen(rd());
uniform_real_distribution<double> desti(0.0, 100.0);

uniform_real_distribution<double> dis(-2.0, 6.0);

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
    for (int i = 0; i < 1000; i++)
    {
        for (int i = 0; i < N; i++)
        {
            
        }
    }
}

int main(void)
{
    int cnt = 0;
    while (++cnt)
    {
        cout << "Processing Batch : #" << cnt << endl;
        for (int i = 0; i < 10; i++)
        {
            process();
        }
    }
}