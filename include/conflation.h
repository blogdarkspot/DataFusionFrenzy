#pragma once

#include <concepts>

template<typename Key, typename Container>
concept ContainerOrderStatistcs = requires (Container& container, 
    const Key& key, std::size_t rank )
{
    {container.select(rank)} -> std::same_as<Key>;
    {container.rank(key)} -> std::same_as<int>;
};

template<typename Key, ContainerOrderStatistcs<Key> Container>
void algo(Container _oldBook, Container _newBook)
{
    int oldBookDelta = 0x00;
    for(const auto& entry : _newBook)
    {
        auto oldPosition = _oldBook.rank(entry.first);
        auto newPosition = _newBook.rank(entry.first);

        if(oldPosition > 0)
        {
            auto delta = newPosition - (oldPosition + oldBookDelta);            
            if (delta < 0)
            {
                oldBookDelta += delta;
            }
        }
        else
        {
            ++oldBookDelta;
        }
    }
}