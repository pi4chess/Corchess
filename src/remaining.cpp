#include <iostream>

using namespace std;

template<int T>
  int remaining1(int myTime, int myInc, int moveOverhead, int movesToGo, int ply)
  {
    if (myTime <= 0)
        return 0;

    double TRatio, sd = 8.5;
    int mn = (ply + 1) / 2; // current move number for any side

    /// In moves to go case we distribute time according to quadratic function with the maximum around move 20 for 40 moves in y time case.

    if (movesToGo)
    {
        TRatio = (T == 0 ? 1.0 : 6.0) / std::min(50, movesToGo);
        if (mn <= 40)
            TRatio *= (1.1 - 0.001 * (mn-20) * (mn-20));
        else
            TRatio *= 1.5;
    }
    else
    {
        /// In non-moves to go case we increase usage of remaining time as the game goes on. This is controlled by parameter sd.

        sd = 1.0 + 20.0 * mn / (500.0 + mn);
        TRatio = (T == 0 ? 0.017 : 0.07) * sd;
    }

    /// In the case of no increment we simply have ratio = std::min(1.0, TRatio); The usage of increment follows quadratic distribution with the maximum at move 25.

    double incUsage = 0.0;

    if (myInc)
        incUsage = std::max(55.0, 120.0 - 0.12 * (mn-25) * (mn-25));

    double ratio = std::min(1.0, TRatio * (1.0 + incUsage * myInc / (myTime * sd)));
    int timeLeft = std::max(0, myTime - moveOverhead);

    return int(timeLeft * ratio); // Intel C++ asks for an explicit cast
  }

int remaining2(int myTime, int myInc, int moveOverhead,
                int movesToGo, int ply, int type) {

    if (myTime <= 0)
        return 0;

    int moveNumber = (ply + 1) / 2;
    double ratio;    // Which ratio of myTime we are going to use. It is <= 1
    double sd = 8.5; // ???

    // Usage of increment follows quadratic distribution with the maximum at move 25
    double inc = myInc * std::max(55.0, 120.0 - 0.12 * (moveNumber - 25) * (moveNumber - 25));

    // In moves-to-go we distribute time according to a quadratic function with
    // the maximum around move 20 for 40 moves in y time case.
    if (movesToGo)
    {
        ratio = (type == 0 ? 1.0 : 6.0) / std::min(50, movesToGo);

        if (moveNumber <= 40)
            ratio *= 1.1 - 0.001 * (moveNumber - 20) * (moveNumber - 20);
        else
            ratio *= 1.5;
    }
    // Otherwise we increase usage of remaining time as the game goes on
    else
    {
        sd = 1 + 20 * moveNumber / (500.0 + moveNumber);
        ratio = (type == 0 ? 0.017 : 0.07) * sd;
    }

    ratio = std::min(1.0, ratio * (1 + inc / (myTime * sd)));

    return int(ratio * std::max(0, myTime - moveOverhead));
  }


int main()
{
    int a,b,c,d;
    int count1 = 0;
    int count2 = 0;

    for (a=0; a<=100000; a+=100)
        for (b=0; b<=100; b+=100)
        for (c=0; c<=40; c+=10)
        for (d=1; d<=200; d++)
    {
        if (remaining1<0>(a,b,30,c,d) == remaining2(a,b,30,c,d,0)) count1++;
        else
            count2++;
        if (remaining1<1>(a,b,30,c,d) == remaining2(a,b,30,c,d,1)) count1++;
        else
            count2++;
    }

    cout << count1 << " checks were successful!\n";
    cout << count2 << " checks were not successful!\n";

    return 0;
}
