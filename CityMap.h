#ifndef CITYMAP_CITYMAP_H
#define CITYMAP_CITYMAP_H

#define MAX 16 //max number
#include<string>
#include<map>
#include<vector>
#include<iostream>

using namespace std;


struct Path
{
    int length;
    int pathSize;
    int path[MAX];
    Path(){
        length=0;
        int pathSize=0;
    }

};
class CityMap
{
private:
    int AdjMat[MAX][MAX];
    map<string, int> nums;
    int numsSize;
    string InputFile;
    int currentLocation;

    string findByValue(int n);
public:
    //CityMap();
    CityMap(string inputFile);
    bool isWay(int start, int end);
    void isWay(string startName,string endName);
    bool isWayToAll(int start);
    void isWayToAll(string name);
    void close(int index);
    void open(int index);
    int partTour(int start);
    bool isEnd(int index);
    void PrintAllDeadEnds();
    int evaluatePath(Path path);
    void FindAllPaths(int start,int end,int* path,int& pathIndex,bool* visited,vector<Path> &paths);
    void PrintThreeShortest(int start, int end,int);
    void PrintSinglePath(int start,int end);
    void printPath(Path path);
    bool isEulerian();

    void interface();

};
#endif //CITYMAP_CITYMAP_H
