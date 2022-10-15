#include "conflation.h"
#include "container.h"
#include <cstdint>
#include <vector>
#include <memory>
#include <cmath>

struct MktEntry
{
    MktEntry(std::uint64_t id, std::uint64_t pos, std::uint64_t size, std::double_t price) :
    _id(id), _pos(pos), _size(size), _price(price)
    {

    }
    std::uint64_t _id;
    std::uint64_t _pos;
    std::uint64_t _size;
    std::double_t _price;
};

std::vector<std::shared_ptr<MktEntry>> getFakeBid(size_t size)
{
    std::vector<std::shared_ptr<MktEntry>> ret;

    for(int i = 1; i <= size; ++i)
    {
         ret.push_back(std::make_shared<MktEntry>(i, i, 10 * i, i * 1.0));
    }
    return ret;
}

std::vector<std::shared_ptr<MktEntry>> getFakeAsk(size_t size)
{
    std::vector<std::shared_ptr<MktEntry>> ret;

    for(int i = 1; i <= size; ++i)
    {
         ret.push_back(std::make_shared<MktEntry>(i, i, 10 * i, (size - i) * 1.0));
    }
    return ret;
}



int main()
{
    return 0;
};