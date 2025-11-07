#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;
using string_pair=pair<string, string>;

vector<string_pair> load_doubles(const string& filename) {  
    ifstream file(filename);
    vector<string_pair> result;
    string double1, double2;
    while (file >> double1 >> double2) {
        result.emplace_back(double1, double2);

    }
    file.close();
    return result;
}
bool verify_double(const string& s) {
    size_t i = 0;
    bool digit = false, period = false, digafterdot = false;

    if (s[0] == '+' || s[0] == '-') {
        i=1;
        if (i>= s.size()) return false;
    };
    for (; i < s.size(); ++i) {
        char c = s[i];
        if (c >= '0' && c <= '9') {
            if (period) digafterdot = true;
            digit = true;
        } else if (s[i] == '.') {
            if (period) return false;
            period = true;
        } else {
            return false;
        }
    }
    if (period && !digafterdot) return false;
    return digit;   
}
char double_add(const string& s1, const string& s2) {
    
}