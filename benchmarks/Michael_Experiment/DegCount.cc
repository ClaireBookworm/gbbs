#include "DegCount.h"

namespace gbbs
{
    struct Storage{
        int a;
        Storage(){}
        void alloc(int id){
            a = id;
        }
    };
    template <class Graph>
    double DegCount_runner(Graph &G, commandLine P)
    {
        std::cout << "### Application: DegCount" << std::endl;
        std::cout << "### Graph: " << P.getArgument(0) << std::endl;
        std::cout << "### Threads: " << num_workers() << std::endl;
        std::cout << "### n: " << G.n << std::endl;
        std::cout << "### m: " << G.m << std::endl;
        std::cout << "### ------------------------------------" << std::endl;
        std::cout << "### ------------------------------------" << std::endl;
        auto init_f = [&](Storage* stor){stor->alloc(worker_id());};
        auto finish_f = [&](Storage* stor){return;};
        parallel_for_alloc<Storage>(init_f, finish_f, 0, 40, [&](size_t i, Storage* stor){
            std::cout<< i << " " << worker_id()<< " "<< stor->a << " " << stor <<std::endl;
        });
        // isSymmetry(G);
        // timer t;
        // t.start();
        // DegCount1(G);
        double tt = 0;
        // std::cout << "### Running Time 1: " << tt << std::endl;
        // t.start();
        // DegCount2(G);
        // tt = t.stop();
        // std::cout << "### Running Time 2: " << tt << std::endl;
        return tt;
    }

} // namespace gbbs

generate_main(gbbs::DegCount_runner, false);