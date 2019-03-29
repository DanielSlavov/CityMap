#include<iostream>
#include"CityMap.h"
#include <chrono>
#include<fstream>

using namespace std;


int main()
{
    CityMap cm = CityMap("../input.txt");//location of file must be correct, may vary on IDE
    cm.interface();
    //location, change @, neighbours, move @, close @, open @, closed, routes @, deadends, waytoall, tour, iseulerian

    return 0;
}