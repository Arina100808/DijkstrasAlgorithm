#include <map>
#include <set>
#include <list>
#include <queue>
#include <stack>
#include <string>
#include <vector>
#include <fstream>
#include <numeric>
#include <iostream>
#include <algorithm>

using namespace std;

vector<string> split(string input, char delimiter)
{
    vector<string> output;

    int begin = 0, i = 0;
    int l = input.length();
    while(++i < l)
    {
        if (input[i] == delimiter)
        {
            output.push_back(input.substr(begin, i - begin));
            begin = i + 1;
            ++i;
        }
    }

    if (input.length() - begin > 0)
    {
        output.push_back(input.substr(begin));
    }
    return output;
}

struct Vertex
{
    char name;
    vector<pair<unsigned int, char>> Distances;

    void idVertex(char letter)
    {
        name = letter;
    }
};

Vertex MakeNewVertex(string element, int i){
    Vertex vert;
    int j;
    if(i == 1)
    {
        j = 3;
    }
    else
    {
        j = 1;
    }
    char a = element[i];
    vert.idVertex(a);

    vert.Distances.push_back(make_pair(element[5] - '0', element[j]));

    return vert;
}

class List
{
    std::vector<Vertex> vertices;

public:
    void Add(Vertex vertex)
    {
        vertices.push_back(vertex);
    }

    vector<Vertex> FillList(vector<string> InitialData)
    {
        for(string element : InitialData)
        {
            if(vertices.empty())
            {
                Add(MakeNewVertex(element, 1));
                Add(MakeNewVertex(element, 3));
            }
            else
            {
                int i = 0;
                int j = 0;
                for(Vertex & vert : vertices)
                {
                    if(element[1] == vert.name)
                    {
                        vert.Distances.push_back(make_pair(element[5] - '0', element[3]));
                        i++;
                    }
                    if(element[3] == vert.name)
                    {
                        vert.Distances.push_back(make_pair(element[5] - '0', element[1]));
                        j++;
                    }
                }
                if(i == 0)
                {
                    Add(MakeNewVertex(element, 1));
                }
                if(j == 0)
                {
                    Add(MakeNewVertex(element, 3));
                }
            }
        }
        return vertices;
    }
};

Vertex FindVertex(vector<Vertex> vertices, char x)
{
    Vertex NoVertex;
    for(auto & v : vertices)
    {
        if(v.name == x)
        {
            return v;
        }
    }
    return NoVertex;
}

int FindPair(vector<pair<unsigned int, char>> D, char end)
{
    int i = 0;
    for(auto & pair : D)
    {
        if(pair.second == end)
        {
            return i;
        }
        i++;
    }
    return i;
}

bool VertexIsVisited(char endtemp, vector<char> vect)
{
    int s = vect.size();
    for(int i = 0; i < s; i++)
    {
        if(endtemp == vect[i])
        {
            return true;
        }
    }
    return false;
}

vector<char> GetShortestPath(string input, string find)
{
    List list;
    vector<string> InitialData = split(input, ' ');

    vector<Vertex> vertices = list.FillList(InitialData);

    priority_queue<pair<int, char>, vector<pair<int, char>>, greater<pair<int, char>>> queue;

    // find
    char start = find[0];
    char end = find[3];
    vector<string> ww = split(find, ',');
    int max = stoi(ww[1]);

    Vertex Start = FindVertex(vertices, start);

    // update distances in vector<pair<unsigned int, char>> Distances

    queue.push({0, Start.name});

    vector<pair<unsigned int, char>> D;
    vector<pair<vector<char>, char>> Routes;

    D.push_back(make_pair(0, Start.name));

    vector<char> route;
    route.push_back(Start.name);
    Routes.push_back(make_pair(route, Start.name));
    route.clear();

    int sizev = vertices.size();
    for(int i = 1; i < sizev; i++)
    {
        D.push_back(make_pair(999, vertices[i].name));
        vector<char> route;
        route.push_back(Start.name);
        Routes.push_back(make_pair(route, vertices[i].name));
        route.clear();
    }

    vector<char> visited;

    // Dijkstra's algorithm

    while(!queue.empty())
    {
        char nearest = queue.top().second; // get end (char)
        Vertex NearestVertex = FindVertex(vertices, nearest);
        vector<pair<unsigned int, char>> nearestdistances = NearestVertex.Distances;
        queue.pop();

        for(auto & pair : nearestdistances)
        {
            unsigned int weighttemp = pair.first;
            char endtemp = pair.second;
            if(VertexIsVisited(endtemp, visited))
            {
                continue;
            }
            int i = FindPair(D, nearest);
            int k = FindPair(D, endtemp);

            if(D[i].first + weighttemp < D[k].first)
            {
                D[k].first = D[i].first + weighttemp;
                queue.push({D[k].first, endtemp});

                Routes[k].first.clear();
                for(int x = 0; x < Routes[i].first.size(); x++){
                    Routes[k].first.push_back(Routes[i].first[x]);
                }
                Routes[k].first.push_back(endtemp);
            }
        }
        visited.push_back(nearest);
    }

    int shortest = FindPair(D, end);
    int ShortDistance = D[shortest].first;

    if(ShortDistance <= max)
    {
        return Routes[shortest].first;
    }
    else
    {
        vector<char> error;
        error.push_back('E');
        return error;
    }
}

int main() {
    vector<char> result;

    string input;
    getline(cin, input);

    string find;
    getline(cin, find);

    result = GetShortestPath(input, find);

    int size = result.size();
    for(int i = 0; i < size - 1; i++)
    {
        cout << result[i] << "->";
    }
    cout << result[result.size() - 1];

    return 0;
}


//[A,B,1] [A,C,1] [A,D,6] [A,E,8] [B,D,3] [C,D,2] [D,E,3]
//A->E,10
//A, C, D, E