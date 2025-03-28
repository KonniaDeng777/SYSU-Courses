#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <limits>
#include <pthread.h>
#include <sys/time.h>

using namespace std;

const int INF = numeric_limits<int>::max();
const int MAX_THREADS = 16;

struct Edge {
    int target;
    double distance;
    Edge(int t, double d) : target(t), distance(d) {}
};

vector<vector<Edge>> graph;
vector<vector<double>> shortest_distances;
vector<pair<int, int>> tests;
int num_vertices;

void loadGraph(const string& filename) {
    ifstream file(filename);
    string line;

    // Skip header
    getline(file, line);

    while (getline(file, line)) {
        stringstream ss(line);
        int source, target;
        double distance;
        char comma;
        ss >> source >> comma >> target >> comma >> distance;

        num_vertices = max(num_vertices, max(source, target) + 1);
        if (graph.size() <= max(source, target)) {
            graph.resize(max(source, target) + 1);
        }

        graph[source].emplace_back(target, distance);
        graph[target].emplace_back(source, distance); 
    }
}

void loadTests(const string& filename, int n) {
    ifstream file(filename);
    string line;
    int count = 0;

    while (count < n && getline(file, line)) {
        stringstream ss(line);
        int source, target;
        char comma;
        ss >> source >> comma >> target;
        tests.emplace_back(source, target);
        count++;
    }
}

void* dijkstra(void* arg) {
    int tid = *(int*)arg;
    int start = tid * (num_vertices / MAX_THREADS);
    int end = (tid == MAX_THREADS - 1) ? num_vertices : (tid + 1) * (num_vertices / MAX_THREADS);

    for (int i = start; i < end; ++i) {
        vector<double>& dist = shortest_distances[i];
        dist[i] = 0.0;

        vector<bool> visited(num_vertices, false);

        for (int j = 0; j < num_vertices - 1; ++j) {
            int u = -1;
            double min_dist = INF;

            for (int k = start; k < end; ++k) {
                if (!visited[k] && dist[k] < min_dist) {
                    min_dist = dist[k];
                    u = k;
                }
            }

            if (u == -1)
                break;

            visited[u] = true;

            for (const Edge& edge : graph[u]) {
                int v = edge.target;
                double alt = dist[u] + edge.distance;
                if (alt < dist[v]) {
                    dist[v] = alt;
                }
            }
        }
    }

    pthread_exit(NULL);
}

double getCurrentTime() {
    timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec / 1000000.0;
}

int main() {
    loadGraph("updated_mouse.csv");
    int num_tests = 2048; 
    loadTests("updated_flower.csv", num_tests);

    // Initialize 
    shortest_distances.resize(num_vertices, vector<double>(num_vertices, INF));

    pthread_t threads[MAX_THREADS];
    int thread_ids[MAX_THREADS];

    double start_time = getCurrentTime();

    // Create threads
    for (int i = 0; i < MAX_THREADS; ++i) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, dijkstra, (void*)&thread_ids[i]);
    }

    // Join threads
    for (int i = 0; i < MAX_THREADS; ++i) {
        pthread_join(threads[i], NULL);
    }

    double end_time = getCurrentTime();
    double elapsed_time = end_time - start_time;


    // Output
    for (const auto& test : tests) {
        int source = test.first;
        int target = test.second;
        cout << "Shortest distance from " << source << " to " << target << ": " << shortest_distances[source][target] << endl;
    }
    cout << "Running time: " << elapsed_time << " seconds" << endl;

    return 0;
}
