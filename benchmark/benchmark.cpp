#include <atomic>
#include <benchmark/benchmark.h>
#include "container.h"
#include "conflation.h"
#include <vector>
#include <memory>
#include <atomic>

#include <iostream>



class entry {

public:
    entry(int id)
    {
        _id = id;
    } 

    int _id;
};


static void BM_Container(benchmark::State& state) {

    std::vector<std::shared_ptr<entry>> c1;
    std::vector<std::shared_ptr<entry>> c2;

    for(int i = 1; i < state.range(0); ++i)
    {
        c1.emplace_back(std::make_shared<entry>(i));
        if(i % 2)
         c2.emplace_back(std::make_shared<entry>(i));
        else
         c2.emplace_back(std::make_shared<entry>(i * 10));
    }

    auto bs0 = std::make_shared<conflation::container::Snapshot<entry>>(c1.begin(), c1.end());
    auto bs1 = std::make_shared<conflation::container::Snapshot<entry>>(c2.begin(), c2.end());

  for (auto _ : state)
  {
        auto commands = conflation::compute_diff<entry>(bs0, bs1);
  }
}

static void BM_CreateContainer(benchmark::State& state) {

    std::vector<std::shared_ptr<entry>> c1;
    std::vector<std::shared_ptr<entry>> c2;

    for(int i = 1; i < state.range(0); ++i)
    {
        c1.emplace_back(std::make_shared<entry>(i));
        if(i % 2)
         c2.emplace_back(std::make_shared<entry>(i));
        else
         c2.emplace_back(std::make_shared<entry>(i * 10));
    }

    auto bs0 = std::make_shared<conflation::container::Snapshot<entry>>(c1.begin(), c1.end());
    auto bs1 = std::make_shared<conflation::container::Snapshot<entry>>(c2.begin(), c2.end());

  for (auto _ : state)
  {
    auto bs0 = std::make_shared<conflation::container::Snapshot<entry>>(c1.begin(), c1.end());
  }
}
// Register the function as a benchmark
BENCHMARK(BM_Container)
->Arg(1 << 4) 
->Arg(1 << 5) 
->Arg(1 << 6) 
->Arg(1 << 7) 
->Arg(1 << 8) 
->Arg(1 << 9) 
->Arg(1 << 10) 
->Arg(1 << 11) 
->Arg(1 << 12) 
->Arg(1 << 13) 
->Arg(1 << 14);

BENCHMARK(BM_CreateContainer)
->Arg(1 << 4) 
->Arg(1 << 5) 
->Arg(1 << 6) 
->Arg(1 << 7) 
->Arg(1 << 8) 
->Arg(1 << 9) 
->Arg(1 << 10) 
->Arg(1 << 11) 
->Arg(1 << 12) 
->Arg(1 << 13) 
->Arg(1 << 14);

BENCHMARK_MAIN();