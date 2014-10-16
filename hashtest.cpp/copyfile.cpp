#ifdef __cplusplus
#include <iostream>
#include <fstream>

#include "copyfile.hpp"

using namespace std;

int copy_file(std::string from, std::string to) {

    errno = 0;
    ifstream source(from, ios::binary);
    ofstream dest(to, ios::binary);

    dest << source.rdbuf();

    source.close();
    dest.close();

    return errno;
}
#endif
