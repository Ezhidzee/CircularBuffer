#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include <string>

#include "CCircularBuffer.hpp"
#include "CCircularBufferExt.hpp"

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

CCircularBuffer<int> c;
CCircularBuffer<int> c1(10);
CCircularBuffer<int> c2(c1);
CCircularBuffer<int> c3 = {1, 2, 3, 4, 5, 8, 9};
CCircularBuffer<int> c4(c3.begin(), c3.end());
CCircularBufferExt<int> c5 = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
CCircularBufferExt<int> c6 = {14, 15, 16};
CCircularBufferExt<int> c7(10);
CCircularBufferExt<std::string> c8(10);

TEST(CCircularBuffer, ConstructorsTest) {
    EXPECT_EQ(c.size(), 0);
    EXPECT_EQ(c1.capacity(), 11);
    EXPECT_EQ(c2.capacity(), 11);
    EXPECT_EQ(c2.capacity(), 11);
    ASSERT_THAT(c3, testing::ElementsAre(1, 2, 3, 4, 5, 8, 9));
    ASSERT_THAT(c4, testing::ElementsAre(1, 2, 3, 4, 5, 8, 9));
}

TEST(CCircularBuffer, EquatingOperatorTest) {
    c = c3;
    ASSERT_THAT(c, testing::ElementsAre(1, 2, 3, 4, 5, 8, 9));
}

TEST(CCircularBuffer, BeginTest) {
    for (int i = 0; i < c3.size(); ++i) {
        EXPECT_EQ(*(c3.begin() + i), c3[i]);
    }
}

TEST(CCircularBuffer, EndTest) {
    for (int i = c3.size(); i >= 1; --i) {
        EXPECT_EQ(*(c3.end() - i), c3[c3.size() - i]);
    }
}

TEST(CCircularBuffer, EqualityOperatorTest) {
    EXPECT_EQ(c4 == c3, true);
}

TEST(CCircularBuffer, InequalityOperatorTest) {
    EXPECT_EQ(c4 != c3, false);
}

TEST(CCircularBuffer, SwapabilityTest) {
    c4 = {1, 2, 3};
    c4.swap(c3);
    ASSERT_THAT(c3, testing::ElementsAre(1, 2, 3));
    ASSERT_THAT(c4, testing::ElementsAre(1, 2, 3, 4, 5, 8, 9));
}

TEST(CCircularBuffer, EmptyTest) {
    EXPECT_EQ(c1.empty(), true);
}

TEST(CCircularBuffer, InsertTest) {
    c4.insert(c4.end(), 727);
    ASSERT_THAT(c4, testing::ElementsAre(2, 3, 4, 5, 8, 9, 727));
    c4.insert(c4.end(), 3, 727);
    ASSERT_THAT(c4, testing::ElementsAre(5, 8, 9, 727, 727, 727, 727));
    c4.insert(c4.end(), c3.begin(), c3.begin() + 3);
    ASSERT_THAT(c4, testing::ElementsAre(727, 727, 727, 727, 1, 2, 3));
    c4.insert(c4.end(), {4, 5, 6, 7});
    ASSERT_THAT(c4, testing::ElementsAre(1, 2, 3, 4, 5, 6, 7));
}

TEST(CCircularBuffer, EraseTest) {
    c4.erase(c4.begin());
    ASSERT_THAT(c4, testing::ElementsAre(2, 3, 4, 5, 6, 7));
    c4.erase(c4.begin() + 3, c4.end());
    ASSERT_THAT(c4, testing::ElementsAre(2, 3, 4));
}

TEST(CCircularBuffer, ClearTest) {
    c3.clear();
    EXPECT_EQ(c3.empty(), true);
}

TEST(CCircularBuffer, AssignTest) {
    c4 = {1, 2, 3};
    c3.assign(c4.begin(), c4.end());
    ASSERT_THAT(c3, testing::ElementsAre(1, 2, 3));
    c3.assign({727});
    ASSERT_THAT(c3, testing::ElementsAre(727));
    c3.assign(5, 727);
    ASSERT_THAT(c3, testing::ElementsAre(727, 727, 727, 727, 727));
}

TEST(CCircularBuffer, FrontTest) {
    c3 = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    EXPECT_EQ(c3.front(), 0);
}

TEST(CCircularBuffer, BackTest) {
    EXPECT_EQ(c3.back(), 9);
}

TEST(CCircularBuffer, Push_frontTest) {
    for (int i = 0; i < 5; ++i) c1.push_front(i);
    ASSERT_THAT(c1, testing::ElementsAre(4, 3, 2, 1, 0));
    for (int i = 0; i < 15; ++i) c1.push_front(i);
    ASSERT_THAT(c1, testing::ElementsAre(14, 13, 12, 11, 10, 9, 8, 7, 6, 5));
}

TEST(CCircularBuffer, Push_backTest) {
    c1.clear();
    for (int i = 0; i < 5; ++i) c1.push_back(i);
    ASSERT_THAT(c1, testing::ElementsAre(0, 1, 2, 3, 4));
    for (int i = 0; i < 15; ++i) c1.push_back(i);
    ASSERT_THAT(c1, testing::ElementsAre(5, 6, 7, 8, 9, 10, 11, 12, 13, 14));
}

TEST(CCircularBuffer, Pop_frontTest) {
    c1.pop_front();
    EXPECT_EQ(c1.front(), 6);
}

TEST(CCircularBuffer, Pop_backTest) {
    c1.pop_back();
    EXPECT_EQ(c1.back(), 13);
}

TEST(CCircularBuffer, CapacityTest) {
    c3 = {1, 2, 3, 4, 5};
    EXPECT_EQ(c3.capacity(), 6);
}

TEST(CCircularBuffer, ReserveTest) {
    c3.reserve(3);
    EXPECT_EQ(c3.capacity(), 6);
    EXPECT_EQ(c3.size(), 5);
    c3.reserve(10);
    EXPECT_EQ(c3.capacity(), 11);
    EXPECT_EQ(c3.size(), 5);
}

TEST(CCircularBuffer, ResizeTest) {
    c3.resize(7);
    EXPECT_EQ(c3.capacity(), 11);
    EXPECT_EQ(c3.size(), 7);
}

TEST(CCircularBufferExt, InsertTest) {
    c5.insert(c5.end(), 727);
    ASSERT_THAT(c5, testing::ElementsAre(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 727));
    EXPECT_EQ(c5.capacity(), 21);
    EXPECT_EQ(c5.size(), 11);
    c5.insert(c5.begin() + 5, 727);
    ASSERT_THAT(c5, testing::ElementsAre(0, 1, 2, 3, 4, 727, 5, 6, 7, 8, 9, 727));
    EXPECT_EQ(c5.capacity(), 21);
    EXPECT_EQ(c5.size(), 12);
    c5.insert(c5.end(), 3, 8);
    ASSERT_THAT(c5, testing::ElementsAre(0, 1, 2, 3, 4, 727, 5, 6, 7, 8, 9, 727, 8, 8, 8));
    c5.insert(c5.end(), c6.begin(), c6.end());
    ASSERT_THAT(c5, testing::ElementsAre(0, 1, 2, 3, 4, 727, 5, 6, 7, 8, 9, 727, 8, 8, 8, 14, 15, 16));
    c5.insert(c5.end(), {1, 2, 3});
    ASSERT_THAT(c5, testing::ElementsAre(0, 1, 2, 3, 4, 727, 5, 6, 7, 8, 9, 727, 8, 8, 8, 14, 15, 16, 1, 2, 3));
}

TEST(CCircularBufferExt, Push_frontTest) {
    for (int i = 0; i < 15; ++i) c7.push_front(i);
    ASSERT_THAT(c7, testing::ElementsAre(14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0));
    EXPECT_EQ(c7.capacity(), 21);
    EXPECT_EQ(c7.size(), 15);
}

TEST(CCircularBufferExt, Push_backTest) {
    c7.clear();
    for (int i = 0; i < 15; ++i) c7.push_back(i);
    ASSERT_THAT(c7, testing::ElementsAre(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14));
    EXPECT_EQ(c7.capacity(), 21);
    EXPECT_EQ(c7.size(), 15);
}

TEST(CCircularBufferExt, StringTest) {
    for (int i = 0; i < 10; ++i) c8.push_back(std::to_string(i));
    ASSERT_THAT(c8, testing::ElementsAre("0", "1", "2", "3", "4", "5", "6", "7", "8", "9"));
}