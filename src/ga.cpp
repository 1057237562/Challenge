#include <bits/stdc++.h>

#define M_PI 3.14159265358979323846

using namespace std;

random_device rd;
mt19937 gen(rd());
uniform_real_distribution<double> desti(0.0, 100.0);

uniform_real_distribution<double> dis(-2.0, 6.0);

uniform_real_distribution<double> mut(-0.1, 0.1);

uniform_real_distribution<double> possibility(0.0, 1.0);

const int N = 1000;
const int S = 100;
const int DS = 28;

const double crossRate = 0.67;
const double mutateRate = 0.1;

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

struct Mutator
{
    Pos destination;
    Pos position;
    Pos velocity;
    double direction = 0.0;
    int timer = 0;

    vector<double> data;
    double startDistance;
    const double density = 20;
    const double force = 200;
    double radius = 0.45;

    double getAcceleration()
    {
        return force / (M_PI * radius * radius * density);
    }

    void travel(int &f, double &r)
    {
        Pos diff = destination - position;
        double n1 = diff.length();
        double directionDiff = acosf(diff.normalize().dot(Pos::fromRadian(direction)));
        float arg[4];
        for (int i = 0; i < 4; i++)
        {
            arg[i] = data[i * 5] * position.x + data[i * 5 + 1] * position.y + data[i * 5 + 2] * destination.x + data[i * 5 + 3] * destination.y + data[i * 5 + 4] * direction;
        }
        f = r = 0;
        for (int i = 0; i < 4; i++)
        {
            f += arg[i] * data[20 + i * 2];
            r += arg[i] * data[21 + i * 2];
        }
        f = min(6, max(-2, f));
        r = min(M_PI, max(-M_PI, r));
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

    double distance() const
    {
        return destination.euclidDistance(position);
    }

    bool operator<(const Mutator &b)
    {
        if (timer == b.timer)
            return distance() / startDistance < b.distance() / b.startDistance;
        else
            return timer < b.timer;
    }

    void fun1(const int &t)
    {
        static int res1;
        static double res2;
        travel(res1, res2);
        direction += res2 / 50.0;
        velocity = (velocity + (Pos::fromRadian(direction) * getAcceleration()) / 50.0);
        position = position + velocity;
        timer = t;
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
        mutators[i].destination = destination;
        mutators[i].startDistance = mutators[i].distance();
    }
    for (int t = 0; t < 50 * 6; t++)
    {
        for (int i = 0; i < N; i++)
        {
            if (mutators[i].distance() > 0.1)
            {
                mutators[i].fun1(t);
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
    ifstream fin;
    fin.open("best.ckpt", ios::in);
    if (fin.is_open())
    {
        cout << "Reading Checkpoint" << endl;
        for (int i = 0; i < S; i++)
        {
            for (int j = 0; j < DS; j++)
            {
                double data;
                fin >> data;
                mutators[i].data.push_back(data);
            }
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
        fin.close();
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
            for (int i = 0; i < DS; i++)
                cout
                    << mutators[0].data[i] << " ";
            cout << endl;

            cout << "Writing To files..." << endl;
            ofstream out;
            out.open("best.ckpt", ios::out);
            for (int i = 0; i < S; i++)
            {
                for (int j = 0; j < DS; j++)
                    out << mutators[i].data[j] << " ";
                out << endl;
            }
            out.close();
        }
    }
}