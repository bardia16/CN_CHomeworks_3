# CN_CHomeworks_3
We have a small command handler section in our main which mainly recieves our commands. 

First we start with defining our topology. We have a global adj matrix that we change the cost of for each connection we may or may not have. Initially they're all -1 which means there are no connections. We also have a node index map which shows the index of each node in the adj matrix. 

The show command is done with two for loops that print out the adj matrix according to the number of nodes we have (not the max number of nodes). 

The modify command takes 2 nodes and the cost. It'll print out an error if the 2 nodes are the same. Otherwise it'll change the cost inbetween the two nodes. If none existed before it'll make a new one

The remove command takes 2 nodes and removes the connection that existed before. If none existed, then it'll print out an error.

Then we have our two algorithms for path finding. 

First is the LSRP. If we have an argument for it, then we'll run the Dijkstra's algorithm for each node we'll then update the distances of the neighbors of the current node and finally print out the costs for each iteration until the last one. For the last one we print out the minimum cost in between the source node and other nodes and the shortest path to get to it. If we don't have an argument we'll do this for every node.

The second one is the DVRP. We first initialize the distance matrix and next hop matrix and then we run the DVRP algorithm. We then print out the results which includes the next hop, the shortest path, and the cost. If we don't have an argument we'll do this for every node. 
