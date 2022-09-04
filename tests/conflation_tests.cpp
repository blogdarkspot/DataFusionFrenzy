#include "book.h"
#include "conflation.h"
#include <memory>
#include <vector>
#include "gtest/gtest.h"
#include <iostream>

class Ordem
{
	public:
	
	Ordem(int id, float price) : _id(id), _price(price) {}
	int _id;
	float _price;
};

class ConflationTest : public ::testing::Test {
	protected:
		void SetUp() override {

			auto pos1 = std::make_shared<Ordem>(1, 1.0);    
			auto pos2 = std::make_shared<Ordem>(2, 2.0);   
			auto pos3 = std::make_shared<Ordem>(3, 3.0);   
			auto pos4 = std::make_shared<Ordem>(4, 4.0);    
			auto pos5 = std::make_shared<Ordem>(5, 5.0);  
			auto pos6 = std::make_shared<Ordem>(6, 6.0); 
			auto pos7 = std::make_shared<Ordem>(7, 7.0); 
			auto pos8 = std::make_shared<Ordem>(8, 8.0); 
			auto pos9 = std::make_shared<Ordem>(9, 9.0);

			_b0.emplace_back(pos1);
			_b0.emplace_back(pos2);
			_b0.emplace_back(pos3);
			_b0.emplace_back(pos4);
			_b0.emplace_back(pos5);
			_b0.emplace_back(pos6);
			_b0.emplace_back(pos7);
			_b0.emplace_back(pos8);
			_b0.emplace_back(pos9);

			_b1.emplace_back(std::make_shared<Ordem>(*pos1));
			_b1.emplace_back(std::make_shared<Ordem>(*pos2));
			_b1.emplace_back(std::make_shared<Ordem>(*pos3));
			_b1.emplace_back(std::make_shared<Ordem>(*pos4));
			_b1.emplace_back(std::make_shared<Ordem>(*pos5));
			_b1.emplace_back(std::make_shared<Ordem>(*pos6));
			_b1.emplace_back(std::make_shared<Ordem>(*pos7));
			_b1.emplace_back(std::make_shared<Ordem>(*pos8));
			_b1.emplace_back(std::make_shared<Ordem>(*pos9));
		}

	
	std::vector<std::shared_ptr<Ordem>> _b0;
	std::vector<std::shared_ptr<Ordem>> _b1;
};

TEST_F(ConflationTest, CreateBookSnapshot)
{
	auto bookSnap = std::make_shared<book::BookSnapshot<Ordem>>(_b0.begin(), _b0.end());
	int pos = 0;
	for(const auto& value : *bookSnap)
	{
		EXPECT_EQ(_b0[pos++]->_id, value->id);
	}

}

TEST_F(ConflationTest, GetDiffTwoBookEquals)
{
	auto bs0 = std::make_shared<book::BookSnapshot<Ordem>>(_b0.begin(), _b0.end());
	auto bs1 = std::make_shared<book::BookSnapshot<Ordem>>(_b1.begin(), _b1.end());

	auto commands = conflation::compute_diff<Ordem>(bs0, bs1);
	EXPECT_EQ(commands.size(), 0);
}

TEST_F(ConflationTest, EraseFirstPosition)
{
	_b1.erase(_b1.begin());
	auto bs0 = std::make_shared<book::BookSnapshot<Ordem>>(_b0.begin(), _b0.end());
	auto bs1 = std::make_shared<book::BookSnapshot<Ordem>>(_b1.begin(), _b1.end());

	auto commands = conflation::compute_diff<Ordem>(bs0, bs1);
	EXPECT_EQ(commands.size(), 1);
	EXPECT_EQ(commands[0]._type, book::OPERATION::DELETE_THRU);
	EXPECT_EQ(commands[0]._p0, 1);
	EXPECT_EQ(commands[0]._p1, 2);
	EXPECT_EQ(commands[0]._id, 0);
}

TEST_F(ConflationTest, InsertOnePosition)
{
	_b1.insert(_b1.begin(), std::make_shared<Ordem>(10, 0.5));
	auto bs0 = std::make_shared<book::BookSnapshot<Ordem>>(_b0.begin(), _b0.end());
	auto bs1 = std::make_shared<book::BookSnapshot<Ordem>>(_b1.begin(), _b1.end());

	auto commands = conflation::compute_diff<Ordem>(bs0, bs1);
	EXPECT_EQ(commands.size(), 1);
	EXPECT_EQ(commands[0]._type, book::OPERATION::INSERT);
	EXPECT_EQ(commands[0]._p0, 1);
	EXPECT_EQ(commands[0]._p1, 0);
	EXPECT_EQ(commands[0]._id, 10);
}

TEST_F(ConflationTest, InsertTwoPosition)
{
	_b1.insert(_b1.begin(), std::make_shared<Ordem>(10, 0.5));
	_b1.insert(_b1.begin() + 1, std::make_shared<Ordem>(11, 0.7));
	auto bs0 = std::make_shared<book::BookSnapshot<Ordem>>(_b0.begin(), _b0.end());
	auto bs1 = std::make_shared<book::BookSnapshot<Ordem>>(_b1.begin(), _b1.end());

	auto commands = conflation::compute_diff<Ordem>(bs0, bs1);
	EXPECT_EQ(commands.size(), 2);
	EXPECT_EQ(commands[0]._type, book::OPERATION::INSERT);
	EXPECT_EQ(commands[0]._p0, 1);
	EXPECT_EQ(commands[0]._p1, 0);
	EXPECT_EQ(commands[0]._id, 10);
}

TEST_F(ConflationTest, InsertTwoPositionRemoveOne)
{
	_b1.erase(_b1.begin());
	_b1.insert(_b1.begin(), std::make_shared<Ordem>(10, 0.5));
	_b1.insert(_b1.begin() + 1, std::make_shared<Ordem>(11, 0.7));
	auto bs0 = std::make_shared<book::BookSnapshot<Ordem>>(_b0.begin(), _b0.end());
	auto bs1 = std::make_shared<book::BookSnapshot<Ordem>>(_b1.begin(), _b1.end());

	auto commands = conflation::compute_diff<Ordem>(bs0, bs1);
	EXPECT_EQ(commands.size(), 3);
	EXPECT_EQ(commands[0]._type, book::OPERATION::INSERT);
	EXPECT_EQ(commands[0]._p0, 1);
	EXPECT_EQ(commands[0]._p1, 0);
	EXPECT_EQ(commands[0]._id, 10);
}
