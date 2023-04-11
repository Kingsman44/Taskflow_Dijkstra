#include<iostream>
#include<climits>
#include <taskflow/taskflow.hpp>
using namespace std;

tf::Executor executor;
tf::Taskflow taskflow;

void DijkstraAlgo(int graph[6][6],int src) // adjacency matrix 
{
    int distance[6]; // // array to calculate the minimum distance for each node                             
    bool Tset[6];// boolean array to mark visited and unvisited for each node
    
    int counter,minimum,m;
    auto A = taskflow.emplace([&](){
        // std::cout << "initializes the counter to zero\n";
        counter = -1;
    }).name("A");

    auto B = taskflow.emplace([&](){
        // std::cout << "loops to increment the counter\n";
        counter++;
    }).name("B");

    auto C = taskflow.emplace([&](){
        // std::cout << "counter is " << counter << " -> ";
        if(counter < 6) {
        // std::cout << "loops again (goes to B)\n";
            distance[counter] = INT_MAX;
            Tset[counter] = false;   
        return 0;
        }
        // std::cout << "breaks the loop (goes to D)\n";
        return 1;
    }).name("C");

    auto D = taskflow.emplace([&](){
        distance[src] = 0;   // Source vertex distance is set 0 
    }).name("D");
    
    auto F = taskflow.emplace([&](){
        // std::cout << "initializes the counter to zero\n";
        counter = 0;
    }).name("F");

    auto G = taskflow.emplace([&](){
        // std::cout << "loops to increment the counter\n";
        counter++;
    }).name("G");

    auto H = taskflow.emplace([&](){
        //std::cout << "counter is " << counter << " -> ";
        if(counter<6) {
            //std::cout << "loops again (goes to B)\n";
            minimum=INT_MAX;
            for(int l=0;l<6;l++) 
            {
                if(Tset[l]==false && distance[l]<=minimum)      
                {
                    minimum=distance[l];
                    m=l;
                }
            }
            Tset[m]=true;
            for(int x = 0; x<6; x++)                  
            {
                // updating the distance of neighbouring vertex
                if(!Tset[x] && graph[m][x] && distance[m]!=INT_MAX && distance[m]+graph[m][x]<distance[x])
                    distance[x]=distance[m]+graph[m][x];
            }
        return 0;
        }
        // std::cout << "breaks the loop (goes to D)\n";
        return 1;
    }).name("H");

    auto I = taskflow.emplace([&](){
        cout<<"Vertex\t\tDistance from source vertex"<<endl;
        counter=-1;
    }).name("I");

    auto J = taskflow.emplace([&](){
        counter++;
    }).name("J");

    auto K = taskflow.emplace([&](){
        if(counter < 6) {
            char str=65+counter; 
            cout<<str<<"\t\t\t"<<distance[counter]<<endl;
        return 0;
        }
        return 1;
    }).name("K");

    A.precede(B);
    B.precede(C);
    C.precede(B);
    C.precede(D);
    D.precede(F);
    F.precede(G);
    G.precede(H);
    H.precede(G);
    H.precede(I);
    I.precede(J);
    J.precede(K);
    K.precede(J);

    // // executes the taskflow
    executor.run(taskflow).wait();

    cout << "\n\n";
    taskflow.dump(std::cout);
}

int main()
{
    int graph[6][6]={
        {0, 1, 2, 0, 0, 0},
        {1, 0, 0, 5, 1, 0},
        {2, 0, 0, 2, 3, 0},
        {0, 5, 2, 0, 2, 2},
        {0, 1, 3, 2, 0, 1},
        {0, 0, 0, 2, 1, 0}};
    DijkstraAlgo(graph,0);
    return 0;                           
}
