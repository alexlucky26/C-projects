#include "ip_filter.h"
#include <gtest/gtest.h>
#include <vector>
#include <string>
#include <cstdlib>
#include <algorithm>
#include <iostream>

// Test class for ip_filter
class IpFilterTest : public ::testing::Test {
protected:
    ip_filter pool_filter;
    ip_pool_type test_pool;
    void SetUp() override {
        // pool ip adresses for testing 
        testing::internal::Strings test_adresses = {
            "222.173.235.246", 
            "222.130.177.64", 
            "222.82.198.61", 
            "1.70.44.170", 
            "1.29.168.152", 
            "1.1.234.8", 
            "46.70.225.39", 
            "46.70.147.26", 
            "46.70.113.73",
            "46.70.29.76", 
            "186.204.34.46", 
            "46.161.63.66"
        };
        for(string line : test_adresses)
        {
            vector<string> v = pool_filter.split(line, '.');
            ip_elem ip;
            for (size_t i = 0; i < ip.size(); ++i)
            {
                ip[i] = stoi(v[i]);
            }
            test_pool.push_back(ip);
        }
        pool_filter.SetPool(test_pool);
    }
};

TEST_F(IpFilterTest, FilterByFirstByte) {
    ip_pool_type result = pool_filter.filter({1});
    ip_pool_type expected = {
        {1,70,44,170},
        {1,29,168,152},
        {1,1,234,8}
    };
    ASSERT_EQ(result, expected);
}

TEST_F(IpFilterTest, FilterByFirstAndSecondBytes) {
    ip_pool_type result = pool_filter.filter({46, 70});
    ip_pool_type expected = {
        {46,70,225,39},
        {46,70,147,26},
        {46,70,113,73},
        {46,70,29,76}
    };
    ASSERT_EQ(result, expected);
}

TEST_F(IpFilterTest, FilterByAnyByte) {
    ip_pool_type result = pool_filter.filter_any(46);
    ip_pool_type expected = {
        {46,70,225,39},
        {46,70,147,26},
        {46,70,113,73},
        {46,70,29,76},
        {186,204,34,46},
        {46,161,63,66}
    };
    ASSERT_EQ(result, expected);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}