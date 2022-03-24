#include <vector>
#include <map>
#include <list>
#include <fstream>
#include <iostream>

#ifdef DEBUG
#include <chrono>
#endif

#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)

using namespace std;

struct Edge {
    int u,v;
    int C;
    int f;
};
class Network {
private:
    int s,t;
    vector <Edge *> edges;
    map <int, vector<Edge *> > nodemap;

    map <int, int> levels;

public:
    Network (int source, int target) : s(source), t(target) {};
    bool addEdge(int u, int v, int C) {
        Edge *edge = new Edge;
        edge->u = u; edge->v = v; edge->C = C; edge->f = 0;

        edges.push_back(edge);
        nodemap[u].push_back(edge);
        nodemap[v].push_back(edge);
#ifdef DEBUG
        cout << "Edge added from " << edge->u << " to " << edge->v << "\n";
#endif
        return true;
    }

    bool BFS () {
        map<int, int> empty;
        levels.swap(empty);

        list <int> l = {s};
        levels[s] = 1; // I begin with 1 because i don't want to initialize.

        while (!l.empty()) {

            int u = l.front();
            l.pop_front();
            if(unlikely(u == t)) break;
            for (uint i = 0;i<nodemap[u].size();i++) {
                Edge *e = nodemap[u][i];
                if (e->u == u && levels[e->v] == 0 && e->C - e->f > 0) {
                    levels[e->v] = levels[u] + 1;
                    l.push_back(e->v);
                    continue;
                } else if (e->v == u && levels[e->u] == 0 && e->f > 0) {
                    levels[e->u] = levels[u] + 1;
                    l.push_back(e->u);
                }
            }
        }

#ifdef DEBUG
            for (auto i:levels) cout << "(" << i.first << "," << i.second << ")" << ", ";
            cout << endl;
#endif

        return levels[t] == 0 ? false : true;
    }

    // int wave (int u) {
    //     if (u)

    // }

};

int main() {
#ifdef DEBUG
        auto begin = chrono::high_resolution_clock::now();
#endif  
    Network network(0, 5);
    
    ifstream infile("/home/song/Maximumflow-Dinic-CUDA/testcases/testcases.txt");
    int u, v, C;
    // Since the txt need.
    C = 1;
    while (infile >> u >> v) {
        network.addEdge(u, v, C);
    }
    network.BFS();


#ifdef DEBUG
        auto end = chrono::high_resolution_clock::now();
        auto elapsed = chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
        begin = end;
        printf("Time measured: %.3f seconds.\n", elapsed.count() * 1e-9);
#endif  

    return 0;
}