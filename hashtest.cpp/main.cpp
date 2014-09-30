#include <iostream>
#include <unordered_map>

using namespace std;

int main()
{

std::string data[] = { "alpha", "bravo", "charlie", "delta",
     "echo", "foxtrot", "golf", "hôtel", "india", "juliet",
     "kilo", "lima", "mike", "november", "oscar", "papa",
     "québec", "romeo", "sierra", "tango", "uniform",
     "victor", "whisky", "x-ray", "yankee", "zulu"
};

unordered_map<std::string, int> H;
int i=0;

for (std::string s: data) {

    H[s] = ++i;
    cout << s << "  " << H[s] << std::endl ;
}

cout << std::endl << "size of data =" << sizeof(data)/sizeof(std::string) << std::endl;

for (const unordered_map<std::string, int>::value_type& x: H) {
    std::cout << x.first << ": " << x.second << std::endl;
  }
    return 0;
}
