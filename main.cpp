#include <cassert>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <initializer_list>


// ("",  '.') -> [""]
// ("11", '.') -> ["11"]
// ("..", '.') -> ["", "", ""]
// ("11.", '.') -> ["11", ""]
// (".11", '.') -> ["", "11"]
// ("11.22", '.') -> ["11", "22"]

using namespace std;

typedef vector<vector<string>> ip_pool_type;

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
}

bool lexic_sort(const vector<string>& ip1, const vector<string>& ip2)
{
	for (int i = 0; i < min(ip1.size(), ip2.size()); ++i)
	{
		if (ip1[i] != ip2[i])
		{
			int numIp1 = stoi(ip1[i]);
			int numIp2 = stoi(ip2[i]);
			return numIp1 > numIp2;
		}
	}
	return ip1.size() > ip1.size();
};

class ip_filter 
{
private:
    ip_pool_type ip_pool;
public:
    ip_filter();
    ~ip_filter(){};

    ip_pool_type filter(const initializer_list<int>& args);
    ip_pool_type filter_any(const int anyElem);
    vector<string> split(const string &str, char d);
    //bool lexic_sort(const vector<string>& ip1, const vector<string>& ip2);
};

ip_filter::ip_filter()
{
    ifstream infile("D:\\C-projects\\02_homework-12995-fb7660 (1)\\ip_filter.tsv");
    if (infile.is_open())
    {
        for(string line; getline(infile, line);)
        {
            vector<string> v = split(line, '\t');
            ip_pool.push_back(split(v.at(0), '.'));
        }
    }

    /*for(string line; getline(cin, line);)
    {
        vector<string> v = split(line, '\t');
        ip_pool.push_back(split(v.at(0), '.'));
    }*/

	sort(ip_pool.begin(), ip_pool.end(), lexic_sort);
    print_ips(ip_pool);
}

vector<string> ip_filter::split(const string &str, char d)
{
    vector<string> r;

    string::size_type start = 0;
    string::size_type stop = str.find_first_of(d);
    while(stop != string::npos)
    {
        r.push_back(str.substr(start, stop - start));

        start = stop + 1;
        stop = str.find_first_of(d, start);
    }

    r.push_back(str.substr(start));

    return r;
};

/*vector<vector<string>> filter()
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

// Функция для фильтрации IP-адресов
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
    return ip_buf;
}


ip_pool_type ip_filter::filter_any(const int anyElem)
{
    ip_pool_type ip_buf;
    for (const auto& ip_elem : ip_pool)
    {
        for (int arg_counter = 0; arg_counter < ip_elem.size(); ++arg_counter)
        {
            int num_ip = stoi(ip_elem.at(arg_counter));
            if (anyElem == num_ip)
            {
                ip_buf.push_back(ip_elem);
                break;
            }
        }
    }
    return ip_buf;
}

int main(int argc, char const *argv[])
{
    try
    {
        // initialization and reverse lexicographically sort
        ip_filter pool_filter;

        // 222.173.235.246
        // 222.130.177.64
        // 222.82.198.61
        // ...
        // 1.70.44.170
        // 1.29.168.152
        // 1.1.234.8

        // TODO filter by first byte and output
        auto ip = pool_filter.filter({1});
        print_ips(ip);

        // 1.231.69.33
        // 1.87.203.225
        // 1.70.44.170
        // 1.29.168.152
        // 1.1.234.8

        // TODO filter by first and second bytes and output
        ip = pool_filter.filter({46, 70});
        print_ips(ip);

        // 46.70.225.39
        // 46.70.147.26
        // 46.70.113.73
        // 46.70.29.76

        // TODO filter by any byte and output
        ip = pool_filter.filter_any(46);
        print_ips(ip);

        // 186.204.34.46
        // 186.46.222.194
        // 185.46.87.231
        // 185.46.86.132
        // 185.46.86.131
        // 185.46.86.131
        // 185.46.86.22
        // 185.46.85.204
        // 185.46.85.78
        // 68.46.218.208
        // 46.251.197.23
        // 46.223.254.56
        // 46.223.254.56
        // 46.182.19.219
        // 46.161.63.66
        // 46.161.61.51
        // 46.161.60.92
        // 46.161.60.35
        // 46.161.58.202
        // 46.161.56.241
        // 46.161.56.203
        // 46.161.56.174
        // 46.161.56.106
        // 46.161.56.106
        // 46.101.163.119
        // 46.101.127.145
        // 46.70.225.39
        // 46.70.147.26
        // 46.70.113.73
        // 46.70.29.76
        // 46.55.46.98
        // 46.49.43.85
        // 39.46.86.85
        // 5.189.203.46
    }
    catch(const exception &e)
    {
        cerr << e.what() << endl;
    }

    return 0;
}
