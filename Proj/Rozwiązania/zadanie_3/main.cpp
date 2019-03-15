/*
Zadanie 3.
Proszę w pliku main.cpp zaimplementować graf, wykonujący podstawowe obliczenia dla danej wejściowej: promien r.
Graf oblicza: obwód koła, powierzchnie koła, pole powierzchni kuli oraz objętość kuli. 
Schemat implementowanego grafu znajduje się w pliku graph.jpg załączonym w katalogu zadania.
Ostatni węzeł w grafie jest źródłem outputu zamieszczonego na końcu pliku.
Graf należy napisać w pliku main.cpp.
*/
#include "tbb/flow_graph.h"
#include <math.h>
#include <iostream>
#include <tuple>

using namespace tbb::flow;

using mlt_node =  multifunction_node<int, tuple<int,int> >;

int main(){
    graph g;
    broadcast_node< int > start(g);
    
    mlt_node main_logic(g , unlimited, [](const int &i, mlt_node::output_ports_type &output) {
        std::get<0>(output).try_put(i);
        std::get<1>(output).try_put(i);  
    });

    mlt_node kolo_logic(g , unlimited, [](const int &i, mlt_node::output_ports_type &output) {
        std::get<0>(output).try_put(i);
        std::get<1>(output).try_put(i);  
    });
    mlt_node kula_logic(g , unlimited, [](const int &i, mlt_node::output_ports_type &output) {
        std::get<0>(output).try_put(i);
        std::get<1>(output).try_put(i);  
    });

    function_node< int, float > pole( g, unlimited, [](const int &v) { 
        return 2 * v * M_PI; 
    } ); 
    
    function_node< int, float > powierzchnia( g, unlimited, [](const int &v) { 
        return pow(v,2) * M_PI; 
    } );

    function_node< int, float > sfera( g, unlimited, [](const int &v) { 
        return 4.0 * pow(v,2) * M_PI; 
    } );

    function_node< int, float > objetosc( g, unlimited, [](const int &v) { 
        return 4.0/3.0 * pow(v,3) * M_PI; 
    } );

    join_node< tuple<int, float,float, float, float> > j(g);

    function_node< tuple< int, float,float, float, float> > print(g ,unlimited, []( const tuple<int, float,float, float, float> &t ) {
               printf( "Dla zadanego r = %d:\npole koła:  %f\npowierzchnia koła:  %f\nsfera: %f, objętość kuli: %f\n"
               ,std::get<0>(t), std::get<1>(t), std::get<2>(t), std::get<3>(t), std::get<4>(t));
           } );

    make_edge(start, main_logic);

    output_port<0>(main_logic).register_successor(kolo_logic);
    make_edge(output_port<1>(main_logic), kula_logic);

    output_port<0>(kolo_logic).register_successor(pole);
    make_edge(output_port<1>(kolo_logic), powierzchnia);

    output_port<0>(kula_logic).register_successor(sfera);
    make_edge(output_port<1>(kula_logic), objetosc);
        

    make_edge( start, input_port<0>( j ) );
    make_edge( pole, input_port<1>( j ) );
    make_edge( powierzchnia, input_port<2>( j ) );
    make_edge( sfera, input_port<3>( j ) );
    make_edge( objetosc, input_port<4>( j ) );

    make_edge( j, print);

    start.try_put(20);

    g.wait_for_all();
}
/* --OUTPUT--
Dla zadanego r = 20:
obwód koła:  125.663704
powierzchnia koła:  1256.637085
sfera: 5026.548340, 
objętość kuli: 33510.320312
*/