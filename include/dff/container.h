/***
MIT License

Copyright (c) 2022 Fábio Santana fss.dsv.src@gmail.com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
**/

#ifndef _DFF_CONTAINER_
#define _DFF_CONTAINER_

#include <algorithm>
#include <cstddef>
#include <iostream>
#include <memory>
#include <set>
#include <unordered_set>
#include <vector>
#include <list>

namespace dff
{
namespace container
{

template <class DATA>
struct entry
{
    int position;
    int id;
	std::shared_ptr<DATA> data;
};

enum class OPERATION
{
    INSERT,
    DELETE,
    DELETE_THRU
};

template <class DATA>
struct command
{
    OPERATION _type;
    int _p0;
    int _p1;
    int _id;
	std::shared_ptr<DATA> _data;
};

template <class DATA>
static command<DATA> make_command(OPERATION type, int p0, int p1 = 0, int id = 0, std::shared_ptr<DATA> data = nullptr)
{
    auto cmd = command<DATA>();
    cmd._type = type;
    cmd._p0 = p0;
    cmd._p1 = p1;
    cmd._id = id;
	cmd._data = data;
    return cmd;
}

struct Hash
{
    template <typename T> std::size_t operator()(std::shared_ptr<T> const &p) const
    {
        return std::hash<int>()(p->id);
    }
};

struct Compare
{
    template <typename T> std::size_t operator()(std::shared_ptr<T> const &a, std::shared_ptr<T> const &b) const
    {
        return a->id == b->id;
    }
};

template <class T> class Snapshot
{
  public:

    Snapshot() {}

    Snapshot(typename std::vector<T>::iterator begin,
             typename std::vector<T>::iterator end)
    {
        int pos = 0;
        size_t size = end - begin;
        _positions.reserve(size);
        _ordens.reserve(size);
        for (auto i = begin; i != end; ++i)
        {
            auto ord = std::make_shared<entry<T>>();
            ord->id = (*i)._id;
            ord->position = ++pos;
			ord->data = std::make_shared<T>();
			*(ord->data) = *i;

            _positions.emplace_back(ord);
            _ordens.insert(ord);
        }
    }

	Snapshot(typename std::list<T>::iterator begin,
             typename std::list<T>::iterator end)
    {
        int pos = 0;
        //size_t size = end - begin;
        //_positions.reserve(size);
        //_ordens.reserve(size);
        for (auto i = begin; i != end; ++i)
        {
            auto ord = std::make_shared<entry<T>>();
            ord->id = (*i)._id;
            ord->position = ++pos;
			ord->data = std::make_shared<T>();
			*(ord->data) = *i;

            _positions.emplace_back(ord);
            _ordens.insert(ord);
        }
    }

    Snapshot(typename std::vector<std::shared_ptr<T>>::iterator begin,
             typename std::vector<std::shared_ptr<T>>::iterator end)
    {
        int pos = 0;
        size_t size = end - begin;
        _positions.reserve(size);
        _ordens.reserve(size);
        for (auto i = begin; i != end; ++i)
        {
            auto ord = std::make_shared<entry<T>>();
            ord->id = (*i)->_id;
            ord->position = ++pos;
			ord->data = std::make_shared<T>();
			*(ord->data) = **i;

            _positions.emplace_back(ord);
            _ordens.insert(ord);
        }
    }

    typename std::vector<std::shared_ptr<entry<T>>>::iterator begin()
    {
        return _positions.begin();
    }

    typename std::vector<std::shared_ptr<entry<T>>>::iterator end()
    {
        return _positions.end();
    }

    std::shared_ptr<entry<T>> find(const std::shared_ptr<entry<T>> &ordem)
    {
        auto value = _ordens.find(ordem);

        if (value != _ordens.end())
        {
            return *value;
        }
        else
        {
            return nullptr;
        }
    }

    std::size_t size()
    {
        return _positions.size();
    }

  private:
    std::vector<std::shared_ptr<entry<T>>> _positions;
    std::unordered_set<std::shared_ptr<entry<T>>, Hash, Compare> _ordens;
};

}; // namespace container
} // namespace dff

#endif
