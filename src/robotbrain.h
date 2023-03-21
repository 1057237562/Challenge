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
    Pos velocity;
    float direction;
    Pos position;

    void travel(int &f, double &r)
    {
        Pos diff = dest - pos;
        double directionDiff = acosf(diff.normalize().dot(Pos::fromRadian(direction)));
        f = min(6, max(-2, (int)(data[0] * diff.length() + data[1] * directionDiff)));
        r = min(M_PI, max(-M_PI, data[2] * diff.length() + data[3] * directionDiff));
    }

    void setDestination(Pos pos)
    {
        destination = pos;
    }
};