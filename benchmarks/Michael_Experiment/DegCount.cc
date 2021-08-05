#include "DegCount.h"
#include <stdlib.h> 

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

        std::string msgs[40]; 
        timer it; it.start();
        parallel_for_alloc<Storage>(init_f, finish_f, 0, 40, [&](size_t i, Storage* stor){
            msgs[i] = std::to_string(i) + " " + std::to_string(worker_id()) + " " + std::to_string(stor->a);
            msgs[i] += std::string(" ") + std::to_string(it.stop());
            std::cout<<"start "<<msgs[i]<<std::endl;
            int* hello = new int[500000];
            par_for(0,5000,[&](size_t i){
                par_for(0,500000,[&](size_t j){
                    hello[j]=0;
                });
            });
            std::cout<<"end "<<msgs[i]<<std::endl;
        });
        for(int i=0;i<40;i++)
            std::cout<<msgs[i]<<std::endl;
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