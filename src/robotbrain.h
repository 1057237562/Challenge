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

    pair<int, double> travel()
    {
        Pos diff = destination - position;

        return make_pair(3, 1.5);
    }
};