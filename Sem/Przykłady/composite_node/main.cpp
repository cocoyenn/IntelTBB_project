#include "tbb/flow_graph.h"
#include <iostream>
#include <tuple>
using namespace tbb::flow;
//template< typename InputTuple, typename OutputTuple > class composite_node;

class smart_odd_square : public composite_node<tuple<int, int>, tuple<int>>
{
    join_node<tuple<int, int>, queueing> j;
    function_node<tuple<int, int>, int> f;

    typedef composite_node<tuple<int, int>, tuple<int>> base_type;

    struct f_body
    {
        int operator()(const tuple<int, int> &t)
        {
            int n = (get<1>(t) + 1) / 2;
            int sum = get<0>(t) + get<1>(t);
            std::cout << "Suma " << n << " pozytywnych liczb nieparzystych to 2 ^ " << n << "  i wynosi: " << sum << std::endl;
            return sum;
        }
    };

  public:
    smart_odd_square(graph &g) : base_type(g), j(g), f(g, unlimited, f_body())
    {
        make_edge(j, f);
        //tuple<int, int> = composite_node< tuple<int,int>, tuple<int> >::input_ports_type
        base_type::input_ports_type input_tuple(input_port<0>(j), input_port<1>(j));
        //tuple<int> = composite_node< tuple<int,int>, tuple<int> >::output_ports_type
        base_type::output_ports_type output_tuple(f);
        base_type::set_external_ports(input_tuple, output_tuple);
    }
};

int main()
{
    graph g;
    split_node<tuple<int, int, int, int, int>> s(g);
    smart_odd_square a0(g);
    smart_odd_square a1(g);
    smart_odd_square a2(g);
    smart_odd_square a3(g);

    make_edge(output_port<0>(s), input_port<0>(a0));
    make_edge(output_port<1>(s), input_port<1>(a0));

    make_edge(output_port<0>(a0), input_port<0>(a1));
    make_edge(output_port<2>(s), input_port<1>(a1));

    make_edge(output_port<0>(a1), input_port<0>(a2));
    make_edge(output_port<3>(s), input_port<1>(a2));

    make_edge(output_port<0>(a2), input_port<0>(a3));
    make_edge(output_port<4>(s), input_port<1>(a3));

    s.try_put(std::make_tuple(1, 3, 5, 7, 9));

    g.wait_for_all();

    return 0;
}
