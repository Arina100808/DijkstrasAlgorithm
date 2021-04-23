#include <iostream>
#include <queue>
#define INF 999

std::vector<std::pair<int,int>> *G;
std::vector<int> D;

int main() {

    int n, m, a, b, w;

    std::cin >> n;
    std::cin>> m;

    G = new std::vector<std::pair<int,int>> [n + 1];

    for(int i = 1; i <= m; ++i)
    {
        std::cin >> a;
        std::cin >> w;
        std::cin >> b;
        G[a - 1].push_back({w, b});
        G[b - 1].push_back({w, a});
    }

    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> q;

    D.push_back(0);
    for(int i = 1; i < n; i++){
        D.push_back(INF);
    }

    q.push({0, 1});

    while(!q.empty())
    {
        int y = q.top().second;
        q.pop();

        for(auto &c : G[y - 1]) {
            int ww = c.first;
            int end = c.second;

            if (D[y - 1] + ww < D[end - 1]) {
                D[end - 1] = D[y - 1] + ww;
                q.push({D[end - 1], end});
            }
        }
    }

    for(int i = 1; i < n; i++)
    {
        std::cout << D[i] << " ";
    }

    return 0;
}