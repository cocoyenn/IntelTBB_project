#include "tbb/flow_graph.h"
#include <iostream>

using namespace tbb::flow;
using namespace std;

int main(){
    int sum = 0;
    graph g;

    function_node< int, int > kwadrat( g, unlimited, [](const int &v) { 
        cout << "Kwadrat" << v << "\n";
        return v*v; 
    } );

    function_node< int, int > szescian( g, unlimited, [](const int &v) { 
        cout << "Sześcian" <<v << "\n";
        return v*v*v; 
    } );

    function_node< int, int > suma( g, 1, [&](const int &v ) -> int { 
        return sum += v; 
    } );
    // //graf przepływu danych
    broadcast_node< int > start(g);
    make_edge(start, kwadrat);
    make_edge(start, szescian);
    make_edge( kwadrat, suma );
    make_edge( szescian, suma );
   
    for ( int i = 1; i <= 10; ++i ) {
        start.try_put(i);
        // kwadrat.try_put(i);
        // szescian.try_put(i);
    }
    g.wait_for_all();

    cout << "Suma wynosi " << sum << "\n";
}
    
