#ifndef __BOOK__
#define __BOOK__

#include <vector>
#include <set>
#include <unordered_set>
#include <memory>
#include <algorithm>
#include <iostream>

namespace book
{
	struct ordem_snapshot
	{
		int position;
		int id;
	};

	enum class OPERATION
	{
		INSERT,
		DELETE,
		DELETE_THRU
	};

	struct command
	{
		OPERATION _type;
		int _p0;
		int _p1;
		int _id;
	};

	static command make_command(OPERATION type, int p0, int p1 = 0, int id = 0)
	{
		auto cmd = command();
		cmd._type = type;
		cmd._p0 = p0;
		cmd._p1 = p1;
		cmd._id = id;
		return cmd;
	}


	struct Hash
	{
		template<typename T>
		std::size_t operator() (std::shared_ptr<T> const &p) const {
			return std::hash<int>()(p->id);
		}
	};

	struct Compare
	{
		template<typename T>
		std::size_t operator()(std::shared_ptr<T> const &a, 
				std::shared_ptr<T> const &b) const {
			return a->id == b->id;
		}
	};

	template<class T>
	class BookSnapshot
	{
		public:

		BookSnapshot(typename std::vector<std::shared_ptr<T>>::iterator begin,
						typename std::vector<std::shared_ptr<T>>::iterator end)
		{
			int pos = 0;
			size_t size = end - begin;
			_positions.reserve(size);
			_ordens.reserve(size);
			for(auto i = begin; i != end; ++i)
			{
				auto ord = std::make_shared<ordem_snapshot>();
				ord->id = (*i)->_id;
				ord->position = ++pos;

				_positions.emplace_back(ord);
				_ordens.insert(ord);
			}
		}	

		
		std::vector<std::shared_ptr<ordem_snapshot>>::iterator begin()
		{
			return _positions.begin();
		}

		std::vector<std::shared_ptr<ordem_snapshot>>::iterator end()
		{
			return _positions.end();
		}

		
		std::shared_ptr<ordem_snapshot> find(const std::shared_ptr<ordem_snapshot>& ordem)
		{
			auto value = _ordens.find(ordem);

			if(value != _ordens.end())
			{
				return *value;
			}
			else
			{
				return nullptr;
			}
		}
		

		private:

		std::vector<std::shared_ptr<ordem_snapshot>> _positions;
		std::unordered_set<std::shared_ptr<ordem_snapshot>, Hash, Compare> _ordens;
	};

};
#endif 
