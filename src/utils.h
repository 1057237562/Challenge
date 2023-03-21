/**
 * 一些基本的类型、函数
 */
#include <cstdio>
#include <cmath>

enum RobotInst
{ // 机器人指令
    Forward,
    Rotate,
    Buy,
    Sell,
    Destroy,
};

static const char RobotInstString[5][10] = {"forward", "rotate", "buy", "sell", "destroy"};

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

inline void beginFrame(const int &frameID)
{
    printf("%d\n", frameID);
}

/*
直接把动作打印出去
示例用法printRobotInst(Foward,0,2,3);
*/
inline void printRobotInst(const RobotInst &inst, const int robotID, const double &arg1 = NAN, const double &arg2 = 0)
{
    if (arg1 == NAN)
    {
        printf("%s %d\n", RobotInstString[inst], robotID);
    }
    else
    {
        printf("%s %d %lf %lf\n", RobotInstString[inst], robotID, arg1, arg2);
    }
}

inline void endFrame()
{
    printf("OK\n");
}
