#include "CityMap.h"
#include<fstream>
#include<iostream>
#include<vector>
#include<limits>
using namespace std;



string* split(string str)
{
    return nullptr;
}



bool isWayInGraph(int start, int end,const int Matrix[MAX][MAX] , int size,bool* visited)
{
    visited[start] = true;
    if(Matrix[end][end]<0)
        return false;
    if (Matrix[start][end] > 0)
    {
        visited[end] = true;
        return true;
    }

    for (int i = 0; i < size; i++)
    {
        if(Matrix[i][i]<0)//check if closed
        {
            continue;
        }
        if (visited[i] == false && Matrix[start][i] > 0 && isWayInGraph(i, end, Matrix, size, visited))
        {
            return true;
        }
    }
    return false;
}
CityMap::CityMap(const string inputFile)
{
    currentLocation=0;
    for(int i=0;i<MAX;i++)
    {
        for(int k=0;k<MAX;k++)
        {
            this->AdjMat[i][k]=0;
        }
    }

    this->numsSize = 0;

    ifstream input;
    input.open(inputFile);

    string currentLine;
    while (getline(input, currentLine))
    {
        string currentWord = "";
        int index = 0;
        int currentCrossIndex;

        while(currentLine[index]!=' ')
        {
            currentWord += currentLine[index];
            index++;
        }
        if (nums.find(currentWord) == nums.end())
        {
            nums.insert(pair<string,int>(currentWord, numsSize));
            currentCrossIndex = numsSize;
            numsSize++;
        }
        else
        {
            currentCrossIndex = nums[currentWord];
        }
        index++;
        while (index  < currentLine.length())
        {
            currentWord = "";
            int crossIndex;
            while (currentLine[index] != ' ')
            {
                currentWord += currentLine[index];
                index++;
            }
            if (nums.find(currentWord) == nums.end())
            {
                nums.insert(pair<string, int>(currentWord, numsSize));
                crossIndex = numsSize;
                numsSize++;
            }
            else
            {
                crossIndex = nums[currentWord];
            }
            index++;
            currentWord = "";

            while (currentLine[index] != ' ' && index < currentLine.length())
            {
                currentWord += currentLine[index];
                index++;
            }
            AdjMat[currentCrossIndex][crossIndex] = stoi(currentWord);
            index++;
        }

    }

    input.close();
}

bool CityMap::isWay(int start, int end)
{
    bool visited[MAX] = { false };
    return isWayInGraph(start, end, this->AdjMat, numsSize, visited );
}

bool CityMap::isWayToAll(int start)
{
    bool visited[MAX] = { false };
    bool visitedCurrent[MAX]= { false };
    bool flag;
    for (int i = 0; i < numsSize; i++)
    {
        if (i == start) continue;
        flag = isWayInGraph(start, i, AdjMat, numsSize, visitedCurrent);
        if (flag)
        {
            for (int k = 0; k < numsSize; k++)
            {
                visited[k] = visited[k] || visitedCurrent[k];
                visitedCurrent[k] = 0;
                flag = flag && visited[k];
            }
            if (flag) return true;
        }
        else return false;
    }
    return false;
}

void CityMap::close(int index)
{
    this->AdjMat[index][index] = -1;
}
int CityMap::partTour(int start) {
    for(int i=numsSize/2;i<numsSize;i++)
    {
        if(i==start) continue;
        if(isWay(start,i)&&isWay(i,start))
        {
            return i;
        }
    }
    for(int i=numsSize/2;i>=0;i--)
    {
        if(i==start) continue;
        if(isWay(start,i)&&isWay(i,start))
        {
            return i;
        }
    }

    return -1;
}

bool CityMap::isEnd(int index) {
    if(this->AdjMat[index][index]<0) return true;
    for(int i=0;i<numsSize;i++)
    {
        if(i==index) continue;
        if(this->AdjMat[index][i] >0)
            return false;
    }

    return true;
}

void CityMap::PrintAllDeadEnds() {
    for(int i=0;i<numsSize;i++)
    {
        if(isEnd(i))
        {
            for(int k=0;k<numsSize;k++)
            {
                if(i==k) continue;
                if(this->AdjMat[k][i]>0)
                {
                    cout<<findByValue(k)<<" - "<<findByValue(i)<<endl;
                }
            }
        }
    }
}

void CityMap::PrintThreeShortest(int start, int end,int count=3)
{
    if(!isWay(start,end))
    {
        //cout<<"No way"<<endl;
        return;
    }
    bool visited[MAX]={false};
    int* path=new int[numsSize];
    int pathIndex=0;
    vector<Path> paths;
    FindAllPaths(start,end,path,pathIndex,visited,paths);
    delete[] path;
    for(int i=0;i<count && !paths.empty();i++)
    {
        int min=INT32_MAX;
        int minIndex=0;
        for(int k = 0;k < paths.size();k++)
        {   int eval=evaluatePath(paths[k]);
            if(eval < min)
            {
                min = eval;
                minIndex = k;
            }
        }
        printPath(paths[minIndex]);
        cout<<"- "<<min<<endl;
        paths.erase(paths.begin() + minIndex);
    }
}

void CityMap::FindAllPaths(int start, int end, int *path, int &pathIndex,bool* visited,vector<Path> &paths) {

    visited[start]=true;
    path[pathIndex]=start;
    pathIndex++;

    if(start==end)
    {
        //path found
        Path CurrentPath;
        CurrentPath.pathSize = pathIndex;
        for(int i=0;i<pathIndex;i++)
            CurrentPath.path[i] = path[i];
        paths.push_back(CurrentPath);
    }
    else
    {
        for(int i=0;i<numsSize;i++)
        {
            if(!visited[i] && AdjMat[start][i]>0 && AdjMat[i][i]>=0)
            {
                FindAllPaths(i, end, path, pathIndex, visited,paths);
            }
        }
    }
    pathIndex--;
    visited[start]=false;
}

int CityMap::evaluatePath(Path path)
{
    int sum=0;
    for(int i=0;i<path.pathSize - 1; i++)
    {
        sum+=AdjMat[path.path[i]][path.path[i+1]];
    }
    return sum;
}

void CityMap::printPath(Path path) {
    for(int i=0;i<path.pathSize;i++)
    {
        cout<<findByValue(path.path[i])<<" ";
    }
    //cout<<endl;
}

string CityMap::findByValue(int n) {
    map<string,int>::const_iterator it = this->nums.begin();
    while(it!=this->nums.end())
    {
        if(it->second == n)
            return it->first;
        it++;
    }
    return "";
}

void CityMap::isWay(string startName, string endName) {
    if(isWay(nums[startName],nums[endName]))
    {
        cout<<"there is way between "<<startName<<" and "<<endName<<endl;
        return;
    }
    cout<<"there is NO way between "<<startName<<" and "<<endName<<endl;
}

void CityMap::isWayToAll(string name) {
    if(isWayToAll(nums[name]))
    {
        cout<<"there is way from "<<name<<" to all"<<endl;
        return;
    }
    cout<<"there is NO way from "<<name<<" to all"<<endl;
}

void CityMap::interface() {
    cout<<"Welcome to CityMap"<<endl<<endl;
    string command,argument;
    while(true)
    {
        cin>>command;
        if(command=="location")
        {
            cout<<"Current location: "<<findByValue(currentLocation)<<endl;
        }
        else if(command == "change") {
            cin>>argument;
            if(nums.find(argument) == nums.end()) {
                cout<<"Location does not exist"<<endl;
            }
            else {
                int index=nums[argument];
                    cout<<"Location changed"<<endl;
                    currentLocation=index;
            }
        }
        else if(command == "neighbours")
        {
            for(int i=0;i<numsSize;i++)
            {

                if(AdjMat[currentLocation][i]>0)
                {
                    cout<<findByValue(i)<<" ";
                }
            }
            cout<<endl;
        }
        else if(command=="move")
        {
            cin>>argument;
            if(nums.find(argument) == nums.end()) {
                cout<<"Location does not exist"<<endl;
            }
            else {
                int index=nums[argument];
                if(isWay(currentLocation,index)) {
                    PrintSinglePath(currentLocation,index);
                    cout<<endl;
                    currentLocation=index;
                }
                else{
                    cout<<"No way to location"<<endl;
                }
            }

        }
        else if(command=="close")
        {
            cin>>argument;
            if(nums.find(argument) == nums.end()) {
                cout<<"Location does not exist"<<endl;
            }
            else
            {
                close(nums[argument]);
                cout<<argument<<" closed"<<endl;
            }
        }
        else if(command=="open")
        {
            cin>>argument;
            if(nums.find(argument) == nums.end()) {
                cout<<"Location does not exist"<<endl;
            }
            else
            {
                open(nums[argument]);
                cout<<argument<<" opened"<<endl;
            }
        }
        else if(command=="closed")
        {
            for(int i=0;i<numsSize;i++)
            {
                if(AdjMat[i][i]<0)
                {
                    cout<<findByValue(i)<<" ";
                }
            }
            cout<<endl;
        }
        else if(command=="exit")
        {
            return;
        }
        else if(command=="routes")
        {
            cin>>argument;
            if(nums.find(argument) == nums.end()) {
                cout<<"Location does not exist"<<endl;
            }
            else
            {
                PrintThreeShortest(currentLocation,nums[argument]);
            }
        }
        else if(command=="deadends")
        {
            PrintAllDeadEnds();
        }
        else if(command=="waytoall")
        {
            if(isWayToAll(currentLocation))
            {
                cout<<"yes"<<endl;
            }
            else
                cout<<"no"<<endl;
        }
        else if(command=="tour")
        {
            int destination=partTour(currentLocation);
            if(destination<0)
            {
                cout<<"tour unavailable"<<endl;
            }
            else
            {
                PrintThreeShortest(currentLocation,destination,1);
                cout<<endl;
                PrintThreeShortest(destination,currentLocation,1);
                cout<<endl;
            }

        }
        else if(command=="iseulerian")
        {
            if(isEulerian())
            {
                cout<<"yes the graph is Eulerian"<<endl;
            }
            else
            {
                cout<<"no the graph is not Eulerian"<<endl;
            }
        }
    }
}

void CityMap::PrintSinglePath(int start, int end) {
    bool visited[MAX]={false};
    int* path=new int[numsSize];
    int pathIndex=0;
    vector<Path> paths;
    FindAllPaths(start,end,path,pathIndex,visited,paths);
    printPath(paths[0]);
    delete path;
}

void CityMap::open(int index) {
    this->AdjMat[index][index] = 0;
}

bool CityMap::isEulerian() {
    bool flag=false;
    int first=-1;
    int second=-1;
    for(int i=0;i<numsSize;i++)
    {
        int indegree=0;
        int outdegree=0;
        for(int k=0;k<numsSize;k++)
        {
            if(AdjMat[k][i]>0) indegree++;
            if(AdjMat[i][k]>0) outdegree++;
        }
        if(indegree+1==outdegree)
        {
            first=i;
            break;
        }
    }
    if(first== -1)
        return false;
    for(int i=0;i<numsSize;i++)
    {
        int indegree=0;
        int outdegree=0;
        for(int k=0;k<numsSize;k++)
        {
            if(AdjMat[k][i]>0) indegree++;
            if(AdjMat[i][k]>0) outdegree++;
        }
        if(indegree==outdegree + 1)
        {
            second=i;
            break;
        }
    }
    if(second == -1)
        return false;
    for(int i=0;i<numsSize;i++)
    {
        if(i==first||i==second)
            continue;
        int indegree=0;
        int outdegree=0;
        for(int k=0;k<numsSize;k++)
        {
            if(AdjMat[k][i]>0) indegree++;
            if(AdjMat[i][k]>0) outdegree++;
        }
        if(indegree!=outdegree)
        {
            return false;
        }
    }
    return true;
}
