#include "ip_filter.h"
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <initializer_list>

static void print_ips(const ip_pool_type& ips)
{
    for(ip_pool_type::const_iterator ip = ips.cbegin(); ip != ips.cend(); ++ip)
    {
        for(vector<string>::const_iterator ip_part = ip->cbegin(); ip_part != ip->cend(); ++ip_part)
        {
            if (ip_part != ip->cbegin())
            {
                cout << ".";
            }
            cout << *ip_part;
        }
        cout << endl;
    }
    cout << "///////////////////\n";
}

bool lexic_sort(const vector<string>& ip1, const vector<string>& ip2)
{
	for (size_t i = 0; i < min(ip1.size(), ip2.size()); ++i)
	{
		if (ip1[i] != ip2[i])
		{
			int numIp1 = stoi(ip1[i]);
			int numIp2 = stoi(ip2[i]);
			return numIp1 > numIp2;
		}
	}
	return ip1.size() > ip2.size();
};

ip_filter::ip_filter()
{
    // use the command on Windows to read the file: type ip_filter.tsv | helloworld_cli.exe
    for(string line; getline(cin, line);)
    {
        vector<string> v = split(line, '\t');
        ip_pool.push_back(split(v.at(0), '.'));
    }

	sort(ip_pool.begin(), ip_pool.end(), lexic_sort);
    print_ips(ip_pool);
}

vector<string> ip_filter::split(const string &str, char d)
{
    vector<string> r;
    std::istringstream stream(str);
    std::string token;
    while (std::getline(stream, token, d)) {
        r.push_back(token);
    }

    return r;
};

/* // older variant with a variadic template

vector<vector<string>> filter()
{
    return vector<vector<string>>();
}

template <typename T, typename... Args>
vector<vector<string>> filter(T t, Args... args) {
    vector<vector<string>> ip_buf;
    for (const auto& ip_elem : ip_pool)
    {
        int num_ip = stoi(ip_elem.at(sort_idx));
        if (t == num_ip)
        {
            ip_buf.push_back(ip_elem);
            break;
        }
    }
    //sort(ip_buf.begin(), ip_buf.end(), greater<>()); // Обратная сортировка
    auto v_filter = filter(args...);
    ip_buf.insert(ip_buf.end(), v_filter.begin(), v_filter.end());
    return ip_buf;
}*/

// The filtration function for ip adresses that gets as input some parts of ip adresses (a mask) up to 4 ip pieces and returns filtrated adresses based on this mask.
ip_pool_type ip_filter::filter(const initializer_list<int>& args) 
{
    const int MAX_ARGS_AMOUNT = 4;
    ip_pool_type ip_buf;
    if (args.size() > MAX_ARGS_AMOUNT) 
    {
        cerr << "filter(initializer_list<int> args) function cannot take more than 4 arguments.\n";
        return ip_buf;
    }

    for (const auto& ip_elem : ip_pool)
    {
        if (ip_elem.size() < args.size()) 
        {
            continue;
        }

        bool match = true;
        size_t i = 0;
        for (int arg : args) 
        {
            if (stoi(ip_elem.at(i)) != arg) 
            {
                match = false;
                break;
            }
            ++i;
        }

        if (match) 
        {
            ip_buf.push_back(ip_elem);
        }
    }
    print_ips(ip_buf);
    return ip_buf;
}

// The function which tries to find the number in ip parts and returns these filtered ips.
ip_pool_type ip_filter::filter_any(const int anyElem)
{
    ip_pool_type ip_buf;
    for (const auto& ip_elem : ip_pool)
    {
        for (size_t arg_counter = 0; arg_counter < ip_elem.size(); ++arg_counter)
        {
            int num_ip = stoi(ip_elem.at(arg_counter));
            if (anyElem == num_ip)
            {
                ip_buf.push_back(ip_elem);
                break;
            }
        }
    }
    print_ips(ip_buf);
    return ip_buf;
}