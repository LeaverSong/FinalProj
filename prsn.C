#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <omp.h>
using namespace std;

class Edge{
public:
    int u, v;
    int flow, capacity;
    int getAvailableFlow(int id){
        if (u == id) {
            return capacity - flow;
        } else {
            return flow;
        }
    }
    int getOtherId(int id){
        return u == id ? v : u;
    }
    void updateFlow(int id, int delta){
        if(u == id)
            flow += delta;
        else
            flow -= delta;
    }
};
class Node{
public:
    int id;
    vector<Edge *> edges;
    vector<Node *> discoveredVertices;
    int excess, height;
    int e_updated, d_updated;
    bool isDiscovered = false;
    int addedExcess = 0;
};

class Graph{
private:
    int s, t;
public:
///<-------------------------------INITIALIZE-------------------------------->///
    vector<Node *> nodes;
    int n;
    Graph(int n){
        this->n = n;
        for(int i = 0; i < n; i++){
            Node *node = new Node();
            node->id = i;
            node->excess = 0;
            node->height = 0;
            nodes.push_back(node);
        }
    }
    void addEdge(int u, int v, int capacity){
        Edge *edge = new Edge();
        edge->u = u;
        edge->v = v;
        edge->capacity = capacity;
        edge->flow = 0;
        nodes[u]->edges.push_back(edge);
        nodes[v]->edges.push_back(edge);
    }
    void print(){
        for(int i = 0; i < n; i++){
            cout << "Node " << i << ": ";
            for(uint j = 0; j < nodes[i]->edges.size(); j++){
                cout << "(" << nodes[i]->edges[j]->u << "," << nodes[i]->edges[j]->v << "," << nodes[i]->edges[j]->flow << ") ";
            }
            cout << endl;
        }
    }
    vector<Edge *> getEdges(int u){
        return nodes[u]->edges;
    }
///<-------------------------------PUSH & RELABEL-------------------------------->///
    void PushRelabel(int s, int t) {
        vector<Node *> workingSet;
        workingSet.push_back(nodes[t]);
        while (true) {

            // Global Relabel to be completed.

            int wSize = workingSet.size();
            if (wSize == 0) break;

            //#pragma omp parallel for
            for (int i = 0;i<wSize;i++) {
                Node *v = workingSet[i];

                int d_updated = v->height;
                int e_updated = v->excess;
                //Line 31
                while (e_updated > 0) {

                    int newLabel = n;
                    bool skipped = false;
                    //Line 34
                    int eSize = v->edges.size();
                    //#pragma omp parallel for
                    for (int j = 0;j<eSize;j++) {
                        if (e_updated == 0) continue;
                        Edge* e = v->edges[j];
                        int idv = v->id;
                        int idw = e->getOtherId(idv);
                        Node *w = nodes[idw];

                        bool admissible = (d_updated == w->height + 1);
                        //Line 39
                        if (w->excess > 0) {
                            bool win = v->height == w->height + 1 
                                       || v->height < w->height - 1 
                                       || v < w;
                            if (admissible && !win) {
                                skipped = true;
                                continue;
                            }
                        }
                        //Line 46
                        int available_flow = e->getAvailableFlow(idv);
                        if (admissible && available_flow > 0) {
                            int delta = available_flow > v->excess ? v->excess : available_flow;
                            e->updateFlow(idv, delta);
                            e_updated -= delta;
                            //#pragma omp atomic
                            w->excess += delta;

                            bool testAndSet;
                            //#pragma omp critical
                            {
                                testAndSet = !w->isDiscovered;
                                if (testAndSet) w->isDiscovered = true;
                            }
                            if (idw != t && testAndSet) {
                                v->discoveredVertices.push_back(w);
                            }
                        }
                        //Line 56
                        if (available_flow > 0 && w->height >= d_updated) {
                            //#pragma omp critical
                            {
                                newLabel = newLabel > w->height + 1?w->height + 1 : newLabel;
                            }
                        }
                    }
                    if (e_updated == 0 || skipped) break;
                    d_updated = newLabel;
                    //Line 61 skipped.
                    if (e_updated == n) break;

                }
                v->addedExcess = e_updated - v->excess;
                bool testAndSet;
                //#pragma omp critical
                {
                    testAndSet = !v->isDiscovered;
                    if (testAndSet) v->isDiscovered = true;
                }
                if (e_updated > 0 && testAndSet) {
                    v->discoveredVertices.push_back(v);
                }
                v->d_updated = d_updated;
                v->e_updated = e_updated;
            }
            //Line 68.
            //#pragma omp for
            for (int i = 0;i<wSize;i++) {
                Node *v = workingSet[i];
                v->height = v->d_updated;
                v->excess += v->addedExcess;
                v->addedExcess = 0;
                v->isDiscovered = false;
            }

            vector <Node *> temp;
            for (auto i:workingSet) {
                temp.insert(temp.end(), i->discoveredVertices.begin(), i->discoveredVertices.end());
            }
            workingSet.swap(temp);

            wSize = workingSet.size();
            cout << "Working Set Size: " << wSize << endl;
            //#pragma omp for
            for (int i = 0;i<wSize;i++) {
                Node *v = workingSet[i];
                v->excess += v->addedExcess;
                v->addedExcess = 0;
                v->isDiscovered = false;
            }
        }
    }
    void Push (Node *u, vector<Node *> &excess_nodes) {
        
    }
    void Relabel (Node *u, vector<Node *> &excess_nodes) {
        
    }
///<------------------------------- DEBUG -------------------------------->///
    void show_graph(){
        for(int i = 0; i < n; i++){
            cout << "Node " << i << " Height : " << nodes[i]->height << "Excess : " << nodes[i]->excess << ": ";
            for(uint j = 0; j < nodes[i]->edges.size(); j++){
                if (nodes[i]->edges[j]->u == nodes[i]->id)
                    cout << "(" << nodes[i]->edges[j]->u << "," << nodes[i]->edges[j]->v << "," << nodes[i]->edges[j]->flow << ") ";
            }
            cout << endl;
        }
    }
};
int main(int argc, char *argv[]) {
    int n, m;
    ifstream infile(argv[1]);
    infile >> n >> n >> m;
    Graph *graph = new Graph(n + 1);
    int u, v, C;
    while (infile >> u >> v >> C) {
        graph->addEdge(u, v, C);
    }
    cout << "Load Finished\n";

    cout << "Input s and t\n";
    int s, t;
    cin >> s >> t;
    cout << "Calculating\n";
    graph->PushRelabel(s, t);
    cout << "Calculation Finished\n";

    cout << graph->nodes[t]->excess;
    return 0; 
}
