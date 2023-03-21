#include <iostream>
using namespace std;

string map[100];



bool readUntilOK()
{
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

int main()
{
    readUntilOK();
    puts("OK");
    fflush(stdout);
    int frameID;
    while (scanf("%d", &frameID) != EOF)
    {
        readUntilOK();
        printf("%d\n", frameID);
        int lineSpeed = 3;
        double angleSpeed = 1.5;
        for (int robotId = 0; robotId < 4; robotId++)
        {
            printf("forward %d %d\n", robotId, lineSpeed);
            printf("rotate %d %f\n", robotId, angleSpeed);
        }
        printf("OK\n", frameID);
        fflush(stdout);
    }
    return 0;
}
