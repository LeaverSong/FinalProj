#include <vector>
#include <iostream>
#include <fstream>
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

    int excess, height;
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
        this->s = s; this->t = t;
        vector <Node *> excess_nodes;

        Node *source = nodes[s];
        source->height = (this->n) + 1;
        for (auto edge : source->edges) {
            edge->flow = edge->capacity;
            nodes[edge->v]->excess = edge->capacity;
            if (edge->v != t)
                excess_nodes.push_back(nodes[edge->v]);
        }

        while (excess_nodes.size() != 0) {
            //FIFO
            Node *u = excess_nodes[0];
            excess_nodes.erase(excess_nodes.begin());


            Push(u, excess_nodes);
            Relabel(u, excess_nodes);
            
            // this->show_graph();
            // cout << endl;
        }
    }
    void Push (Node *u, vector<Node *> &excess_nodes) {
        int idu = u->id;
        for (auto edge : u->edges) {

            int idv = edge->getOtherId(idu);
            // cout << idv << ":" ;
            int available_flow = edge->getAvailableFlow(idu);
            if (available_flow > 0 && \
                u->height > nodes[idv]->height && \
                u->excess > 0) {
                int flow = min(edge->getAvailableFlow(idu), u->excess);
                edge->updateFlow(idu, flow);
                nodes[idu]->excess -= flow;
                nodes[idv]->excess += flow;
                if (nodes[idv]->excess > 0 && idv != s && idv != t) {
                    excess_nodes.push_back(nodes[idv]);
                }
            }
        }
    }
    void Relabel (Node *u, vector<Node *> &excess_nodes) {
        if (u->excess > 0) {
            int min_height = INT32_MAX;
            for (auto edge : u->edges) {
                int idv = edge->getOtherId(u->id);
                if (nodes[idv]->height < min_height && nodes[idv]->height >= u->height) {
                    min_height = nodes[idv]->height + 1;
                }
            }
            u->height = min_height;
            if (u->height < n) excess_nodes.push_back(u);
        }
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
