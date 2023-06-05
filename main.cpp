#include <iostream>
#include <unordered_map>
#include <vector>
#include <sstream>
#include <iomanip>
#include <climits>

using namespace std;

const int MAX_NODES = 100;

// Adjacency matrix to store distances between nodes
int adjMatrix[MAX_NODES][MAX_NODES];

// Map to store the index of each node in the adjacency matrix
unordered_map<string, int> nodeIndexMap;

// Function to parse the "topology" command and update the adjacency matrix
void updateTopology(string topologyCmd) {
    istringstream iss(topologyCmd);
    string nodePair;
    while (iss >> nodePair) {
        int dashPos1 = nodePair.find('-');
        int dashPos2 = nodePair.find('-', dashPos1 + 1);
        if (dashPos1 == string::npos || dashPos2 == string::npos) {
            cout << "Invalid input: " << nodePair << endl;
            continue;
        }
        string node1 = nodePair.substr(0, dashPos1);
        string node2 = nodePair.substr(dashPos1 + 1, dashPos2 - dashPos1 - 1);
        int distance = stoi(nodePair.substr(dashPos2 + 1));
        int node1Index, node2Index;
        if (nodeIndexMap.find(node1) == nodeIndexMap.end()) {
            // Add node1 to the map if it's not already there
            node1Index = nodeIndexMap.size();
            nodeIndexMap[node1] = node1Index;
        } else {
            node1Index = nodeIndexMap[node1];
        }
        if (nodeIndexMap.find(node2) == nodeIndexMap.end()) {
            // Add node2 to the map if it's not already there
            node2Index = nodeIndexMap.size();
            nodeIndexMap[node2] = node2Index;
        } else {
            node2Index = nodeIndexMap[node2];
        }
        adjMatrix[node1Index][node2Index] = distance;
        adjMatrix[node2Index][node1Index] = distance;
    }
}

string getNodeId(int index) {
    for (auto const& node : nodeIndexMap) {
        if (node.second == index) {
            return node.first;
        }
    }
    return "";
}

// Function to print the adjacency matrix
void printAdjMatrix() {
    cout << setw(2) << "|" << setw(2);
    for (int i = 0; i < nodeIndexMap.size(); i++) {
        cout << " " << getNodeId(i);
    }
    cout << endl;
    cout << "------------------------" << endl;
    for (int i = 0; i < nodeIndexMap.size(); i++) {
        cout << setw(2) << getNodeId(i) << " |";
        for (int j = 0; j < nodeIndexMap.size(); j++) {
            if (adjMatrix[i][j] == INT_MAX) {
                cout << setw(2) << " " << setw(2) << "-";
            } else {
                cout << setw(2) << " " << setw(2) << adjMatrix[i][j];
            }
        }
        cout << endl;
    }
}

void modifyDistance(const string& cmd) {
    int dashPos1 = cmd.find('-');
    int dashPos2 = cmd.find('-', dashPos1 + 1);
    if (dashPos1 == string::npos || dashPos2 == string::npos) {
        cout << "Invalid input: " << cmd << endl;
        return;
    }
    string node1 = cmd.substr(0, dashPos1);
    string node2 = cmd.substr(dashPos1 + 1, dashPos2 - dashPos1 - 1);
    int distance = stoi(cmd.substr(dashPos2 + 1));
    if (nodeIndexMap.find(node1) == nodeIndexMap.end() || nodeIndexMap.find(node2) == nodeIndexMap.end()) {
        cout << "Invalid input: " << cmd << endl;
        return;
    }
    int node1Index = nodeIndexMap[node1];
    int node2Index = nodeIndexMap[node2];
    adjMatrix[node1Index][node2Index] = distance;
    adjMatrix[node2Index][node1Index] = distance;
    cout << "Distance between " << node1 << " and " << node2 << " updated to " << distance << endl;
}

void removeDistance(const string& cmd) {
    int dashPos1 = cmd.find('-');
    int dashPos2 = cmd.find('-', dashPos1 + 1);
    if (dashPos1 == string::npos || dashPos2 != string::npos) {
        cout << "Invalid input: " << cmd << endl;
        return;
    }
    string node1 = cmd.substr(0, dashPos1);
    string node2 = cmd.substr(dashPos1 + 1);
    if (nodeIndexMap.find(node1) == nodeIndexMap.end() || nodeIndexMap.find(node2) == nodeIndexMap.end()) {
        cout << "Invalid input: " << cmd << endl;
        return;
    }
    int node1Index = nodeIndexMap[node1];
    int node2Index = nodeIndexMap[node2];
    adjMatrix[node1Index][node2Index] = -1;
    adjMatrix[node2Index][node1Index] = -1;
    cout << "Distance between " << node1 << " and " << node2 << " removed" << endl;
}

void lsrp(const string& sourceId) {
    int numNodes = nodeIndexMap.size();
    vector<int> dist(numNodes, INT_MAX);
    vector<bool> visited(numNodes, false);
    vector<int> prev(numNodes, -1);
    vector<vector<int>> adjList(numNodes, vector<int>());
    int sourceIndex = nodeIndexMap[sourceId];
    dist[sourceIndex] = 0;

    // Build adjacency list
    for (int i = 0; i < numNodes; i++) {
        for (int j = 0; j < numNodes; j++) {
            if (i != j && adjMatrix[i][j] != -1) {
                adjList[i].push_back(j);
                adjList[j].push_back(i);
            }
        }
    }

    // Run Dijkstra's algorithm for each node
    for (int iter = 0; iter < numNodes; iter++) {
        // Find the node with the minimum distance that has not been visited
        int minDist = INT_MAX;
        int currNode = -1;
        for (int i = 0; i < numNodes; i++) {
            if (!visited[i] && dist[i] < minDist) {
                minDist = dist[i];
                currNode = i;
            }
        }
        if (currNode == -1) {
            break;
        }

        // Update the distances of the neighbors of the current node
        for (int neighbor : adjList[currNode]) {
            int newDist = dist[currNode] + adjMatrix[currNode][neighbor];
            if (newDist < dist[neighbor]) {
                dist[neighbor] = newDist;
                prev[neighbor] = currNode;
            }
        }

        visited[currNode] = true;

        // Print the cost to reach each destination node in this iteration
        cout << "Iteration " << iter << ":" << endl;
        cout << setw(10) << "Node" << setw(10) << "Cost" << endl;
        for (int i = 0; i < numNodes; i++) {
            if (dist[i] == INT_MAX) {
                cout << setw(10) << getNodeId(i) << setw(10) << "INF" << endl;
            } else {
                cout << setw(10) << getNodeId(i) << setw(10) << dist[i] << endl;
            }
        }
        cout << endl;
    }

    // Print the final cost to reach each destination node
    cout << "Final result:" << endl;
    cout << setw(10) << "Node" << setw(10) << "Cost" << endl;
    for (int i = 0; i < numNodes; i++) {
        if (dist[i] == INT_MAX) {
            cout << setw(10) << getNodeId(i) << setw(10) << "INF" << endl;
        } else {
            cout << setw(10) << getNodeId(i) << setw(10) << dist[i] << endl;
        }
    }
}

void dvrp(const string& sourceId) {
    int numNodes = nodeIndexMap.size();
    vector<vector<int>> dist(numNodes, vector<int>(numNodes, INT_MAX));
    vector<vector<int>> nextHop(numNodes, vector<int>(numNodes, -1));
    int sourceIndex = nodeIndexMap[sourceId];

    // Initialize distance matrix and next hop matrix
    for (int i = 0; i < numNodes; i++) {
        for (int j = 0; j < numNodes; j++) {
            if (i == j) {
                dist[i][j] = 0;
            } else if (adjMatrix[i][j] != -1) {
                dist[i][j] = adjMatrix[i][j];
                nextHop[i][j] = j;
            }
        }
    }

    // Run DVRP algorithm
    bool updated = true;
    while (updated) {
        updated = false;
        for (int i = 0; i < numNodes; i++) {
            if (i != sourceIndex) {
                int minDist = INT_MAX;
                int next = -1;
                for (int j = 0; j < numNodes; j++) {
                    if (dist[sourceIndex][j] != INT_MAX && adjMatrix[j][i] != -1 && dist[sourceIndex][j] + adjMatrix[j][i] < minDist) {
                        minDist = dist[sourceIndex][j] + adjMatrix[j][i];
                        next = j;
                    }
                }
                if (minDist < dist[sourceIndex][i]) {
                    dist[sourceIndex][i] = minDist;
                    nextHop[sourceIndex][i] = next;
                    updated = true;
                }
            }
        }
    }

    // Print the results
    cout << "Routing table for node " << sourceId << ":" << endl;
    cout << setw(10) << "Dest" << setw(10) << "Next Hop" << setw(10) << "Dist" << setw(20) << "Shortest-Path" << endl;
    for (int i = 0; i < numNodes; i++) {
        if (i != sourceIndex) {
            int next = nextHop[sourceIndex][i];
            string nextId = getNodeId(next);
            cout << setw(10) << getNodeId(i) << setw(10) << nextId << setw(10) << dist[sourceIndex][i] << setw(20) << sourceId;
            while (next != i) {
                cout << " -> " << nextId;
                next = nextHop[next][i];
                nextId = getNodeId(next);
            }
            cout << " -> " << getNodeId(i) << endl;
        }
    }
}


int main() {
    // Initialize the adjacency matrix with -1 to represent unassigned distances
    for (int i = 0; i < MAX_NODES; i++) {
        for (int j = 0; j < MAX_NODES; j++) {
            if (i == j)
            {
                adjMatrix[i][j] = 0;
            }
            
            else
            {
                adjMatrix[i][j] = -1;
            }
                
        }
    }
    string cmd;
    while (true) {
        cout << "Enter command: ";
        getline(cin, cmd);
        if (cmd.substr(0, 8) == "topology") {
            updateTopology(cmd.substr(9));
        } else if (cmd == "show") {
            printAdjMatrix();
        }
        else if (cmd.substr(0, 6) == "modify") {
            modifyDistance(cmd.substr(7));
        } 
        else if (cmd.substr(0, 6) == "remove") {
            removeDistance(cmd.substr(7));
        }
        else if (cmd.substr(0, 4) == "lsrp") {
            lsrp(cmd.substr(5));
        }
        else if (cmd.substr(0, 4) == "dvrp") {
            dvrp(cmd.substr(5));
        }
        else 
        {
            cout << "Invalid command." << endl;
        }
    }
    return 0;
}