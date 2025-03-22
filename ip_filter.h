/// This class uses for filtering ip adresses that are located in a separated file (where each ip is on new row).
/// This command is used on Windows to run the program with data that are read from the file:
/// type ip_filter.tsv | ip_filter_cli.exe
///
/// - Alexander Lukichev
#include <string>
#include <vector>
#include <array>

using namespace std;

#define IP_ADRESS_SIZE 4
typedef array<int, IP_ADRESS_SIZE> ip_elem;
typedef vector<ip_elem> ip_pool_type;

class ip_filter 
{
private:
    ip_pool_type ip_pool;
public:
    ip_filter();

    ip_pool_type filter(const initializer_list<int>& args);
    ip_pool_type filter_any(const int anyElem);
    vector<string> split(const string &str, char d);
    void SetPool(ip_pool_type& new_ip_pool) { ip_pool = new_ip_pool; };
};