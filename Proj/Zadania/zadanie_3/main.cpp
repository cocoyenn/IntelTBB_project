/*
Zadanie 3.
Proszę w pliku main.cpp zaimplementować graf, wykonujący podstawowe obliczenia na pojedynczej danej wejściowej: promien r.
Graf oblicza: pole koła, powierzchnie koła, pole powierzchni kuli oraz objętość kuli. 
Schemat implementowanego grafu znajduje się w pliku graph.jpg załączonym w katalogu zadania.
Ostatni węzeł w grafie jest źródłem outputu zamieszczonego na końcu pliku.
Graf należy zaimplementować w pliku main.cpp.
*/
#include "tbb/flow_graph.h"
#include <math.h>
#include <iostream>
#include <tuple>

using namespace tbb::flow;

using mlt_node =  multifunction_node<int, tuple<int,int> >;

int main(){
    graph g;
    
    /*
    --Miejsce na implementację grafu--
    */

    g.wait_for_all();
}
/* --OUTPUT--
Dla zadanego r = 20:
pole koła:  125.663704
powierzchnia koła:  1256.637085
sfera: 5026.548340, 
objętość kuli: 33510.320312
*/