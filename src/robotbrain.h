#ifndef _RB_H_
#define _RB_H_

#include "scheduler.h"

class RobotBrain
{
    friend class Scheduler;

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

    double data[4] = {6, 0, 0, 6};

    void travel(int &f, double &r)
    {
        Pos diff = destination - position;
        double directionDiff = acosf(diff.normalize().dot(Pos::fromRadian(direction)));
        f = min(6, max(-2, (int)(data[0] * diff.length() + data[1] * directionDiff)));
        r = min(M_PI, max(-M_PI, data[2] * diff.length() + data[3] * directionDiff));
    }

    void setDestination(Pos pos)
    {
        destination = pos;
    }
};

#endif