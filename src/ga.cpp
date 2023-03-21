#include <bits/stdc++.h>
using namespace std;

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

class Mutator
{
public:
    Pos dest;
    Pos pos;
    Pos velocity;
    float direction = 0.0;

    pair<int, double> travel()
    {
    }
};

int main(void)
{
}