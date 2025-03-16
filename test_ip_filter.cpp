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
            test_pool.push_back(pool_filter.split(line, '.'));
        }
        pool_filter.SetPool(test_pool);
    }
    void CompareIpPools(ip_pool_type& pool, std::vector<std::string>& expected);
};

void IpFilterTest::CompareIpPools(ip_pool_type& pool, std::vector<std::string>& expected)
{
    ASSERT_EQ(pool.size(), expected.size());
    for (int pool_idx = 0; pool_idx < pool.size(); ++pool_idx)
    {
        string ip_buf = string();
        const testing::internal::Strings& full_ip = pool[pool_idx]; 
        for (int i = 0; i < full_ip.size(); ++i)
        {
            ip_buf += i != full_ip.size() - 1 ? full_ip[i] + '.' : full_ip[i];
        }
        ASSERT_EQ(ip_buf, expected[pool_idx]);
    }
}

TEST_F(IpFilterTest, FilterByFirstByte) {
    auto result = pool_filter.filter({1});
    std::vector<std::string> expected = {
        "1.70.44.170",
        "1.29.168.152",
        "1.1.234.8"
    };
    CompareIpPools(result, expected);
}

TEST_F(IpFilterTest, FilterByFirstAndSecondBytes) {
    auto result = pool_filter.filter({46, 70});
    std::vector<std::string> expected = {
        "46.70.225.39",
        "46.70.147.26",
        "46.70.113.73",
        "46.70.29.76"
    };
    CompareIpPools(result, expected);
}

TEST_F(IpFilterTest, FilterByAnyByte) {
    auto result = pool_filter.filter_any(46);
    std::vector<std::string> expected = {
        "46.70.225.39",
        "46.70.147.26",
        "46.70.113.73",
        "46.70.29.76",
        "186.204.34.46",
        "46.161.63.66"
    };
    CompareIpPools(result, expected);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}