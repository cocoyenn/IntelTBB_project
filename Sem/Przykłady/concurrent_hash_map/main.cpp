#include "tbb/concurrent_hash_map.h"
#include "tbb/blocked_range.h"
#include "tbb/parallel_for.h"
#include "tbb/tick_count.h"
#include "tbb/task_scheduler_init.h"
#include <iostream>
#include <string>

using namespace tbb;
using namespace std;

#define N 1000000

static string Data[N];


struct MyHashCompare
{
    static size_t hash(const string &x)
    {
        size_t h = 0;
        for (const char *s = x.c_str(); *s; s++)
        {
            h = (h * 17) ^ *s;
        }
        return h;
    }
    static bool equal(const string &x, const string &y)
    {
        return x == y;
    }
};
typedef concurrent_hash_map<string, int, MyHashCompare> StringTable;

class Tally
{
  public:
    Tally(StringTable &table_) : m_table(table_) {}

    void operator()(const blocked_range<string *> range) const
    {
        for (string *p = range.begin(); p != range.end(); ++p)
        {
            StringTable::accessor a;
            m_table.insert(a, *p);
            a->second += 1;
        }
    }
    static void print(StringTable &table)
    {
        for (auto i : table)
            std::cout << i.first.c_str() << "  " << i.second << std::endl;
    }

  private:
    StringTable &m_table;
};

static void CreateData()
{
    srand(2);

    const vector<string> adj = {"sour", "sweet", "bitter", "salty", "big", "small"};

    const vector<string> noun =
        {"apple", "banana", "cherry", "date", "eggplant",
         "fig", "grape", "honeydew", "icao", "jujube"};

    for (int i = 0; i < N; ++i)
    {
        Data[i] = adj[rand() % adj.size()] + " " + noun[rand() % noun.size()];
    }
}

int main(int argc, char *argv[])
{
    int i = 1;

    int NThread = 1;
    if (i < argc)
        NThread = strtol(argv[i++], 0, 0);

    task_scheduler_init init(NThread);
    CreateData();
    StringTable table;

    tick_count t0 = tick_count::now();
    parallel_for(blocked_range<string *>(Data, Data + N, 1000), Tally(table));
    tick_count t1 = tick_count::now();

    Tally::print(table);
    std::cout << "threads = " << NThread << " total = " << N << " time = " << (t1 - t0).seconds() << std::endl;
}