#include <bits/stdc++.h>

#define M_PI 3.14159265358979323846

using namespace std;

random_device rd;
mt19937 gen(rd());
uniform_real_distribution<float> desti(0.0, 100.0);

uniform_real_distribution<float> dis(-2.0, 6.0);

uniform_real_distribution<float> mut(-0.1, 0.1);

uniform_real_distribution<float> possibility(0.0, 1.0);

const int N = 10000;
const int S = 100;
const int DS = 8;

const double crossRate = 0.67;
const double mutateRate = 0.1;

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

    Pos operator*(const double &mul) const
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

    float length() const
    {
        return sqrtf(powf(x, 2) + powf(y, 2));
    }

    Pos normalize() const
    {
        float l = length();
        return {x / l, y / l};
    }

    float dot(const Pos &b) const
    {
        return x * b.x + y * b.y;
    }
};

struct Mutator
{
    Pos dest;
    Pos pos;
    Pos velocity;
    float direction = 0.0;
    int timer = 0;

    vector<double> data;
    float startDistance;

    pair<int, double> travel()
    {
        Pos diff = dest - pos;
        float directionDiff = acosf(diff.normalize().dot(Pos::fromRadian(direction)));
        int f = min(6, max(-2, (int)(data[0] * diff.length() + data[1] * directionDiff)));
        double r = min(M_PI, max(-M_PI, data[2] * diff.length() + data[3] * directionDiff));
        return make_pair(f, r);
    }

    void mutate()
    {
        int sbit = rand() % DS;
        data[sbit] += mut(gen);
    }

    void cross(const Mutator &fa)
    {
        for (int i = 0; i < DS; i++)
        {
            if (rand() % 2)
            {
                data[i] = fa.data[i];
            }
        }
    }

    float distance() const
    {
        return dest.euclidDistance(pos);
    }

    bool operator<(const Mutator &b)
    {
        if (timer == b.timer)
            return distance() / startDistance < b.distance() / b.startDistance;
        else
            return timer < b.timer;
    }
};

Mutator mutators[N];

void init()
{

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < DS; j++)
        {
            mutators[i].data.push_back(dis(gen));
        }
    }
}

void process()
{

    Pos destination = {desti(gen), desti(gen)};
    for (int i = 0; i < N; i++)
    {
        mutators[i].dest = destination;
        mutators[i].startDistance = mutators[i].distance();
    }
    for (int t = 0; t < 50 * 6; t++)
    {
        for (int i = 0; i < N; i++)
        {
            if (mutators[i].distance() > 0.1)
            {
                pair<int, double> result = mutators[i].travel();
                mutators[i].direction += result.second * 1.0 / 50.0;
                mutators[i].velocity = (Pos::fromRadian(mutators[i].direction) * result.first) * (1.0 / 50.0);
                mutators[i].pos = mutators[i].pos + mutators[i].velocity;
                mutators[i].timer = t;
            }
        }
    }
}

void SA()
{
    sort(mutators, mutators + N);
    if (mutators[0].distance() > mutators[0].startDistance)
    {
        init();
        return;
    }

    for (int i = S; i < N; i++)
    {
        int mother = rand() % S;
        mutators[i] = mutators[mother];
        if (possibility(gen) > crossRate)
        {
            int father = rand() % S;
            mutators[i].cross(mutators[father]);
        }
        if (possibility(gen) > mutateRate)
        {
            mutators[i].mutate();
        }
    }
}

int main(void)
{
    srand(time(0));
    if (freopen("best.ckpt", "r", stdin))
    {
        cout << "Reading Checkpoint" << endl;
        for (int i = 0; i < S; i++)
        {
            for (int j = 0; j < DS; j++)
                cin >> mutators[i].data[j];
        }
        for (int i = S; i < N; i++)
        {
            int mother = rand() % S;
            mutators[i] = mutators[mother];
            if (possibility(gen) > crossRate)
            {
                int father = rand() % S;
                mutators[i].cross(mutators[father]);
            }
            if (possibility(gen) > mutateRate)
            {
                mutators[i].mutate();
            }
        }
        fclose(stdin)
    }
    else
    {
        init();
    }
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
             << "   Min Distance : " << mutators[0].distance() << "/" << mutators[0].startDistance << " Time Consumption:" << mutators[0].timer << endl;
        if (!(cnt % 10))
        {
            cout << "Best Model : " << endl;
            for (int i = 0; i < 10; i++)
                cout
                    << mutators[0].data[i] << " ";
            cout << endl;

            cout << "Writing To files..." << endl;
            freopen("best.ckpt", "w", stdout);
            for (int i = 0; i < S; i++)
            {
                for (int j = 0; j < DS; j++)
                    cout << mutators[i].data[j] << " ";
                cout << endl;
            }
            fclose(stdout);
        }
    }
}