#include "container.h"
#include "conflation.h"
#include "gtest/gtest.h"
#include <iostream>
#include <memory>
#include <vector>
#include <atomic>

class Ordem
{
  public:
    Ordem(int id, float price) : _id(id), _price(price)
    {
    }

    Ordem(float price) : _price(price)
    {
        _id = generate_id();
    }
    int _id;
    float _price;

   private:
        static std::atomic<int> _count;

        int generate_id()
        {
            return _count.fetch_add(1);
        }
};

std::atomic<int> Ordem::_count = 0;

class ConflationTest : public ::testing::Test
{
  protected:
    void SetUp() override
    {

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

static void check_book(const std::vector<conflation::container::command> &commands, std::vector<std::shared_ptr<Ordem>> &b0, 
		 std::vector<std::shared_ptr<Ordem>> &b1)
{
    for (const auto &command : commands)
    {
        switch (command._type)
        {
        case conflation::container::OPERATION::INSERT: {
		for(const auto& value : b1)
		{
			if(value->_id == command._id)
			{
				b0.insert(b0.begin() + (command._p0 - 1), std::make_shared<Ordem>(*value));
				break;
			}
		}	
            break;
        }
        case conflation::container::OPERATION::DELETE_THRU: {
		b0.erase(b0.begin() + (command._p0 - 1), b0.begin() + (command._p1 -1));
            break;
        }
        }
    }

    EXPECT_EQ(b0.size(), b1.size());

    for(int i = 0; i < b0.size() ; ++i)
    {
	    EXPECT_EQ(b0[i]->_id, b1[i]->_id);
    }
}

TEST_F(ConflationTest, CreateBookSnapshot)
{
    auto bookSnap = std::make_shared<conflation::container::Snapshot<Ordem>>(_b0.begin(), _b0.end());
    int pos = 0;
    for (const auto &value : *bookSnap)
    {
        EXPECT_EQ(_b0[pos++]->_id, value->id);
    }
}

TEST_F(ConflationTest, GetDiffTwoBookEquals)
{
    auto bs0 = std::make_shared<conflation::container::Snapshot<Ordem>>(_b0.begin(), _b0.end());
    auto bs1 = std::make_shared<conflation::container::Snapshot<Ordem>>(_b1.begin(), _b1.end());

    auto commands = conflation::compute_diff<Ordem>(bs0, bs1);
    EXPECT_EQ(commands.size(), 0);
    check_book(commands, _b0, _b1);
}

TEST_F(ConflationTest, RemoveFirstPosition)
{
    _b1.erase(_b1.begin());

    auto bs0 = std::make_shared<conflation::container::Snapshot<Ordem>>(_b0.begin(), _b0.end());
    auto bs1 = std::make_shared<conflation::container::Snapshot<Ordem>>(_b1.begin(), _b1.end());

    auto commands = conflation::compute_diff<Ordem>(bs0, bs1);
    EXPECT_EQ(commands.size(), 1);
    check_book(commands, _b0, _b1);
}


TEST_F(ConflationTest, RemoveTwoPositions)
{
    _b1.erase(_b1.begin() + 5);
    _b1.erase(_b1.begin() + 5);

    auto bs0 = std::make_shared<conflation::container::Snapshot<Ordem>>(_b0.begin(), _b0.end());
    auto bs1 = std::make_shared<conflation::container::Snapshot<Ordem>>(_b1.begin(), _b1.end());

    auto commands = conflation::compute_diff<Ordem>(bs0, bs1);
    EXPECT_EQ(commands.size(), 1);
    check_book(commands, _b0, _b1);
}

TEST_F(ConflationTest, InsertOnePosition)
{
    _b1.insert(_b1.begin(), std::make_shared<Ordem>(10, 0.5));

    auto bs0 = std::make_shared<conflation::container::Snapshot<Ordem>>(_b0.begin(), _b0.end());
    auto bs1 = std::make_shared<conflation::container::Snapshot<Ordem>>(_b1.begin(), _b1.end());
    auto commands = conflation::compute_diff<Ordem>(bs0, bs1);

    EXPECT_EQ(commands.size(), 1);
    check_book(commands, _b0, _b1);
}

TEST_F(ConflationTest, InsertTwoPosition)
{
    _b1.insert(_b1.begin(), std::make_shared<Ordem>(10, 0.5));
    _b1.insert(_b1.begin() + 1, std::make_shared<Ordem>(11, 0.7));

    auto bs0 = std::make_shared<conflation::container::Snapshot<Ordem>>(_b0.begin(), _b0.end());
    auto bs1 = std::make_shared<conflation::container::Snapshot<Ordem>>(_b1.begin(), _b1.end());
    auto commands = conflation::compute_diff<Ordem>(bs0, bs1);

    EXPECT_EQ(commands.size(), 2);
}

TEST_F(ConflationTest, InsertTwoPositionRemoveOne)
{
    _b1.erase(_b1.begin());
    _b1.insert(_b1.begin(), std::make_shared<Ordem>(10, 0.5));
    _b1.insert(_b1.begin() + 1, std::make_shared<Ordem>(11, 0.7));

    auto bs0 = std::make_shared<conflation::container::Snapshot<Ordem>>(_b0.begin(), _b0.end());
    auto bs1 = std::make_shared<conflation::container::Snapshot<Ordem>>(_b1.begin(), _b1.end());
    auto commands = conflation::compute_diff<Ordem>(bs0, bs1);

    EXPECT_EQ(commands.size(), 3);
    check_book(commands, _b0, _b1);
}

TEST_F(ConflationTest, InsertTwoPositionMiddle)
{
    _b1.insert(_b1.begin() + 1, std::make_shared<Ordem>(10, 1.5));
    _b1.insert(_b1.begin() + 2, std::make_shared<Ordem>(11, 1.7));

    auto bs0 = std::make_shared<conflation::container::Snapshot<Ordem>>(_b0.begin(), _b0.end());
    auto bs1 = std::make_shared<conflation::container::Snapshot<Ordem>>(_b1.begin(), _b1.end());
    auto commands = conflation::compute_diff<Ordem>(bs0, bs1);

    EXPECT_EQ(commands.size(), 2);
    check_book(commands, _b0, _b1);
}

TEST_F(ConflationTest, InsertTwoPositionEnd)
{
    _b1.insert(_b1.end() - 1, std::make_shared<Ordem>(10, 9.5));
    _b1.insert(_b1.end() - 1, std::make_shared<Ordem>(11, 9.7));

    auto bs0 = std::make_shared<conflation::container::Snapshot<Ordem>>(_b0.begin(), _b0.end());
    auto bs1 = std::make_shared<conflation::container::Snapshot<Ordem>>(_b1.begin(), _b1.end());
    auto commands = conflation::compute_diff<Ordem>(bs0, bs1);

    EXPECT_EQ(commands.size(), 2);
    check_book(commands, _b0, _b1);
}

TEST_F(ConflationTest, RemoveTwoPositionEnd)
{
    _b1.erase(_b1.end() - 1);
    _b1.erase(_b1.end() - 1);

    auto bs0 = std::make_shared<conflation::container::Snapshot<Ordem>>(_b0.begin(), _b0.end());
    auto bs1 = std::make_shared<conflation::container::Snapshot<Ordem>>(_b1.begin(), _b1.end());
    auto commands = conflation::compute_diff<Ordem>(bs0, bs1);

    EXPECT_EQ(commands.size(), 1);
    check_book(commands, _b0, _b1);
}

TEST_F(ConflationTest, InsertOneRemoveTwoPositionEnd)
{
    _b1.insert(_b1.begin() + 1, std::make_shared<Ordem>(10, 1.5));
    _b1.erase(_b1.end() - 1);
    _b1.erase(_b1.end() - 1);

    auto bs0 = std::make_shared<conflation::container::Snapshot<Ordem>>(_b0.begin(), _b0.end());
    auto bs1 = std::make_shared<conflation::container::Snapshot<Ordem>>(_b1.begin(), _b1.end());
    auto commands = conflation::compute_diff<Ordem>(bs0, bs1);

    EXPECT_EQ(commands.size(), 2);
    check_book(commands, _b0, _b1);
}

TEST_F(ConflationTest, InsertOneRemoveOnePositionEnd)
{
    _b1.insert(_b1.begin() + 1, std::make_shared<Ordem>(10, 1.5));
    _b1.erase(_b1.end() - 1);

    auto bs0 = std::make_shared<conflation::container::Snapshot<Ordem>>(_b0.begin(), _b0.end());
    auto bs1 = std::make_shared<conflation::container::Snapshot<Ordem>>(_b1.begin(), _b1.end());
    auto commands = conflation::compute_diff<Ordem>(bs0, bs1);

    EXPECT_EQ(commands.size(), 2);
    check_book(commands, _b0, _b1);
}


TEST_F(ConflationTest, BounduarieTest)
{

    _b1.erase(_b1.begin());
    _b1.insert(_b1.begin() + 1, std::make_shared<Ordem>(10, 1.5));
    _b1.erase(_b1.end() - 1);
    _b1.insert(_b1.end() - 1, std::make_shared<Ordem>(11, 10.5));

    auto bs0 = std::make_shared<conflation::container::Snapshot<Ordem>>(_b0.begin(), _b0.end());
    auto bs1 = std::make_shared<conflation::container::Snapshot<Ordem>>(_b1.begin(), _b1.end());
    auto commands = conflation::compute_diff<Ordem>(bs0, bs1);

   // EXPECT_EQ(commands.size(), 2);
    check_book(commands, _b0, _b1);
}

TEST_F(ConflationTest, RemoveMiddle)
{

    _b1.erase(_b1.begin() + 1);
    _b1.erase(_b1.begin() + 1);
    _b1.erase(_b1.begin() + 1);
    _b1.erase(_b1.begin() + 1);
    _b1.erase(_b1.begin() + 1);
    _b1.erase(_b1.begin() + 1);
    _b1.erase(_b1.begin() + 1);

    auto bs0 = std::make_shared<conflation::container::Snapshot<Ordem>>(_b0.begin(), _b0.end());
    auto bs1 = std::make_shared<conflation::container::Snapshot<Ordem>>(_b1.begin(), _b1.end());
    auto commands = conflation::compute_diff<Ordem>(bs0, bs1);

   // EXPECT_EQ(commands.size(), 2);
    check_book(commands, _b0, _b1);
}

TEST_F(ConflationTest, RemoveOddEntry)
{
    _b1.clear();
    _b0.clear();

    for(int i = 1; i < 256; ++i)
    {
        auto pos = std::make_shared<Ordem>(i, i * 1.0);
        _b0.emplace_back(pos);
        if(i % 2)
         _b1.emplace_back(std::make_shared<Ordem>(*pos));
    }

    auto bs0 = std::make_shared<conflation::container::Snapshot<Ordem>>(_b0.begin(), _b0.end());
    auto bs1 = std::make_shared<conflation::container::Snapshot<Ordem>>(_b1.begin(), _b1.end());

    auto commands = conflation::compute_diff<Ordem>(bs0, bs1);

    check_book(commands, _b0, _b1);
}

TEST_F(ConflationTest, RemoveOddEntryAndInsert)
{
    _b1.clear();
    _b0.clear();

    for(int i = 1; i < 256; ++i)
    {

        auto pos = std::make_shared<Ordem>(i, i * 1.0);
        _b0.emplace_back(pos);
        if(i % 2)
         _b1.emplace_back(std::make_shared<Ordem>(*pos));
        else
        {
            auto pos1 = std::make_shared<Ordem>(i * 1000, i * 1.0);
            _b1.emplace_back(pos1);
        }
    }

    auto bs0 = std::make_shared<conflation::container::Snapshot<Ordem>>(_b0.begin(), _b0.end());
    auto bs1 = std::make_shared<conflation::container::Snapshot<Ordem>>(_b1.begin(), _b1.end());

    auto commands = conflation::compute_diff<Ordem>(bs0, bs1);

    check_book(commands, _b0, _b1);
}