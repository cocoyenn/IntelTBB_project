#include "tbb/flow_graph.h"
using namespace tbb::flow;
typedef multifunction_node<int, tuple<int,int> > multi_node;

struct MultiBody {
  void operator()(const int &i, multi_node::output_ports_type &op) {
    if(i % 2)
      std::get<1>(op).try_put(i); // put to odd queue
    else
      std::get<0>(op).try_put(i); // put to even queue
  }
};

int main() {
  graph g;

  queue_node<int> even_queue(g);
  queue_node<int> odd_queue(g);

  multi_node node1(g,unlimited,MultiBody());

  make_edge(output_port<0>(node1), odd_queue);
  make_edge(output_port<1>(node1), odd_queue);

  for(int i = 0; i < 1000; ++i) {
    node1.try_put(i);
  }
  g.wait_for_all();
}