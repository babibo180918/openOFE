//
//  dijkstra.h
//  Implement Dijkstra algorithm.
//
//  Created by Nhan Nguyen on 4/27/16.
//  Copyright © 2016 Nhan Nguyen. All rights reserved.
//

#ifndef dijkstra_h
#define dijkstra_h

#include <iostream>

template<class Cost>
class Dijkstra {
public:
    static const int INF = -1;
    Dijkstra();
    ~Dijkstra();
    
    /**
     *  @param:
     *      grapth: input grapth as 2D square Matrix
     *      N:      Size of Matrix
     *      s:      Start node
     *      t:      Target node
     *  @Return<br>
     *      array of integer of Node label of shortest path.
     */
    int *findPath(Cost *grapth[], int N, int s, int t, int &step);
private:
    Cost *closeSet;
    Cost *openSet;
    int *trace;
};

template<class Cost>
Dijkstra<Cost>::Dijkstra(){
    openSet = NULL;
    closeSet = NULL;
    trace = NULL;
}

template<class Cost>
Dijkstra<Cost>::~Dijkstra(){
    if(closeSet != NULL)
        delete closeSet;
    if(openSet != NULL)
        delete openSet;
    if(trace != NULL)
        delete trace;
}

template<class Cost>
int *Dijkstra<Cost>::findPath(Cost *grapth[], int N, int s, int t, int &step){
    for(int i=0; i<N; i++){
        if (grapth[i][i] > 0){
            std::cout << "Grapth error!" << std::endl;
            return NULL;
        }
    }
    if(s>=N || t>=N){
        std::cout << "Nodes error!" << std::endl;
        return NULL;
    }
    int crrtNode;
    if(closeSet != NULL)
        delete closeSet;
    if(openSet != NULL)
        delete openSet;
    if(trace != NULL)
        delete trace;
    openSet  = new Cost[N];
    closeSet = new Cost[N];
    trace    = new int[N];
    for(int i=0; i<N; i++){ // set cost value equals INF for each open set's and close set's element.
        openSet [i] = static_cast<Cost>(INF);
        closeSet[i] = static_cast<Cost>(INF);
    }
    //
    openSet[s] = static_cast<Cost>(0); // add s to open set.
    crrtNode = s; // assign s to current node
    while(crrtNode != t){
        closeSet[crrtNode] = openSet[crrtNode];
        openSet[crrtNode] = static_cast<Cost>(INF);
        for(int j=0; j<N; j++){ // add successor nodes of current node to open set.
            if(grapth[crrtNode][j] > 0 &&
               openSet[j]==static_cast<Cost>(INF) &&
               closeSet[j] == static_cast<Cost>(INF)){
                openSet[j] = (closeSet[crrtNode] + grapth[crrtNode][j]);
                trace[j] = crrtNode;
            } else if(grapth[crrtNode][j] > 0 &&
               openSet[j]!=static_cast<Cost>(INF) &&
               openSet[j] > (closeSet[crrtNode] + grapth[crrtNode][j]) &&
               closeSet[j] == static_cast<Cost>(INF)){
                openSet[j] = (closeSet[crrtNode] + grapth[crrtNode][j]);
                trace[j] = crrtNode;
            }
        }
        // find lowest cost value in open set.
        Cost min;
        bool isInit = false;
        for (int i=0; i<N; i++){
            if(openSet[i]!=static_cast<Cost>(INF) && !isInit){
                min = openSet[i];
                crrtNode = i;
                isInit = true;
            } else if(openSet[i]!=static_cast<Cost>(INF) && isInit && openSet[i] < min){
                min = openSet[i];
                crrtNode = i;
            }
        }
        if(!isInit){ // open set is empty
            std::cout<< "Search Failed!" << std::endl;
            return NULL;
        }
    }
    std::cout<< "Found path!" << std::endl;
    
    // tracing
    int node = t;
    step = 0;
    while(node != s){
        node = trace[node];
        step++;
    }
    int *output = new int[step+1];
    node = t;
    for(int i=step; i>=0; i--){
        output[i] = node;
        node = trace[node];
    }
    return output;
    
}


#endif /* dijkstra_h */
