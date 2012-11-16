/* 
 * File:   main.cpp
 * Author: umitcavus
 *
 * Created on November 10, 2012, 10:53 PM
 */


#include <iostream>
#include <stdio.h>
#include <vector>
#include <queue>


#define MAX 3000
#define STARTLEFTID 1000
#define STARTRIGHTID 2000
#define NIL 0
#define MINUS -1
#define INF (1<<28)


using namespace std;
vector<int> *G = new vector<int>[MAX];  //whole graph
vector<int> T;                          //minimum vertex cover
vector<int> leftIDs;                    // IDs in the left side(Stockholm)
vector<int> result;                     //result
int match[MAX] = {-1};                  // for max matching, each index represents a node, -1:no node
int Tvisited[MAX] = {-1};               // visited node while traversing T, -1:no node
int layer[MAX] = {-1};                  // layer in tree

int nleft, numPairs;   //n: #ofdistinct node in left side, numPairs: #ofpairs in input

bool bfs() {            
    int i, j, u, v, len, leftID;
    queue <int> Q;
    for(j=0; j<nleft; j++) {
        leftID = leftIDs[j];
        if(match[leftID]==NIL) {
            layer[leftID] = 0;
            Q.push(leftID);
        }
        else layer[leftID] = INF;
    }
    
    layer[NIL] = INF;
    while(!Q.empty()) {
        u = Q.front(); Q.pop();
        if(u!=NIL) {
            len = G[u].size();
            for(i=0; i<len; i++) {
                v = G[u][i];
                if(layer[match[v]]==INF) {
                    layer[match[v]] = layer[u] + 1;
                    Q.push(match[v]);
                }
            }
        }
    }
    return (layer[NIL]!=INF);
}

bool dfs(int u) {
    int i, v, len;
    if(u!=NIL) {
        len = G[u].size();
        for(i=0; i<len; i++) {
            v = G[u][i];
            if(layer[match[v]]==layer[u]+1) {
                if(dfs(match[v])) {
                    match[v] = u;
                    match[u] = v;
                    return true;
                }
            }
        }
        layer[u] = INF;
        return false;
    }
    return true;
}
 
void recvParse(int i, int pos) {  //0: left nodes, 1: right nodes, T= leftTOright_unmatched U rightTOleft_matched
    T.push_back(i);
    Tvisited[i] = 1;
    int len = G[i].size();
    for(int j=0;j<len;j++) {
        int goTo = G[i][j];
        if(Tvisited[goTo] == NIL && (match[i] != goTo && pos == 0  || match[i] == goTo && pos == 1 )) {
                recvParse(goTo, (pos+1)%2);
        }
    }
    return ;
}

bool isInT (int n) {
    for(int i=0;i<T.size();i++) {
        if(T[i]==n) 
            return true;
    }
    return false;
}
void konig() {          //for finding min vertex cover
     int leftID, rightID;
    for (int j=0;j<nleft;j++) {
        leftID = leftIDs[j];
        if (match[leftID]==NIL) {
            recvParse(leftID,0);
        }   
    }
    
    // prepare result vector
    for(int i=0;i<nleft;i++) {
        leftID = leftIDs[i];
        if( !isInT(leftID) ) { 
            result.push_back(leftID);
        }
    }
     for(int i=0;i<T.size();i++) {
         rightID = T[i];
         if(rightID >= STARTRIGHTID)
             result.push_back(T[i]);
        }         
}


void hopcroft_karp() {  // finding max matching
    int matching = 0, leftID;
    while(bfs()) {  
        for(int j=0; j<nleft; j++) {
            leftID = leftIDs[j];
            if(match[leftID]==NIL && dfs(leftID))
                matching++;
        }
    }
    konig();
}

void checkLeftIDs(int i) {  //records IDs on left side

    int len = leftIDs.size();
    for(int j=0;j<len;j++) {
        if(leftIDs[j] == i) {
            return;
        }
    }
    leftIDs.push_back(i);
}

void print_result() {
    
    int size = result.size(); 
    cout<< size <<endl;
    for(int i=0;i<size;i++)
        cout<<result[i]<<endl;
}

int main () {
    
    scanf("%d",&numPairs);
    int num1, num2, temp= numPairs;
    
    while(temp-->0) {
        scanf("%d %d", &num1, &num2);
        G[num1].push_back(num2);
        match[num1] = NIL;
        Tvisited[num1] = NIL;
        G[num2].push_back(num1);
        match[num2] = NIL;
        Tvisited[num2]=NIL;
        
        checkLeftIDs(num1); // 0:left 1:right
        
    }
    nleft = leftIDs.size();

    hopcroft_karp();
    print_result();
}


