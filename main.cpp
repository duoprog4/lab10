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
string double_add(const string& s1, const string& s2) {
    // Step 1: Detect and strip signs
    bool neg1 = false, neg2 = false;
    string a = s1, b = s2;
    if (!a.empty() && a[0] == '-') { neg1 = true; a = a.substr(1); }
    else if (!a.empty() && a[0] == '+') { a = a.substr(1); }
    if (!b.empty() && b[0] == '-') { neg2 = true; b = b.substr(1); }
    else if (!b.empty() && b[0] == '+') { b = b.substr(1); }

    // Step 2: Split into integer and fractional parts
    size_t pos1 = a.find('.'), pos2 = b.find('.');
    string a_int = (pos1 == string::npos) ? a : a.substr(0, pos1);
    string a_frac = (pos1 == string::npos) ? "" : a.substr(pos1 + 1);
    string b_int = (pos2 == string::npos) ? b : b.substr(0, pos2);
    string b_frac = (pos2 == string::npos) ? "" : b.substr(pos2 + 1);

    // Step 3: Pad fractional and integer parts
    while (a_frac.size() < b_frac.size()) a_frac += '0';
    while (b_frac.size() < a_frac.size()) b_frac += '0';
    while (a_int.size() < b_int.size()) a_int = '0' + a_int;
    while (b_int.size() < a_int.size()) b_int = '0' + b_int;

    // Step 4: Combine for magnitude comparison
    string A = a_int + a_frac;
    string B = b_int + b_frac;

    string rf, ri;
    int carry = 0;

    if (neg1 == neg2) {
        // Same sign → addition
        rf = string(a_frac.length(), '0');
        for (int i = a_frac.length() - 1; i >= 0; --i) {
            int sum = (a_frac[i] - '0') + (b_frac[i] - '0') + carry;
            rf[i] = (sum % 10) + '0';
            carry = sum / 10;
        }

        ri = string(a_int.length(), '0');
        for (int i = a_int.length() - 1; i >= 0; --i) {
            int sum = (a_int[i] - '0') + (b_int[i] - '0') + carry;
            ri[i] = (sum % 10) + '0';
            carry = sum / 10;
        }
        if (carry) ri = '1' + ri;
        string result = rf.empty() ? ri : ri + '.' + rf;
        return (neg1 ? "-" + result : result);
    } else {
        // Opposite signs → subtraction
        bool a_bigger = (A >= B);
        string hi_int = a_bigger ? a_int : b_int;
        string hi_frac = a_bigger ? a_frac : b_frac;
        string lo_int = a_bigger ? b_int : a_int;
        string lo_frac = a_bigger ? b_frac : a_frac;
        bool result_negative = a_bigger ? neg1 : neg2;

        rf = string(hi_frac.length(), '0');
        int borrow = 0;
        for (int i = hi_frac.length() - 1; i >= 0; --i) {
            int diff = (hi_frac[i] - '0') - (lo_frac[i] - '0') - borrow;
            if (diff < 0) { diff += 10; borrow = 1; }
            else borrow = 0;
            rf[i] = diff + '0';
        }

        ri = string(hi_int.length(), '0');
        for (int i = hi_int.length() - 1; i >= 0; --i) {
            int diff = (hi_int[i] - '0') - (lo_int[i] - '0') - borrow;
            if (diff < 0) { diff += 10; borrow = 1; }
            else borrow = 0;
            ri[i] = diff + '0';
        }

        // Trim leading zeros in integer part
        size_t nz = ri.find_first_not_of('0');
        ri = (nz != string::npos) ? ri.substr(nz) : "0";

        // Trim trailing zeros in fractional part
        size_t tz = rf.find_last_not_of('0');
        rf = (tz != string::npos) ? rf.substr(0, tz + 1) : "";

        string result = rf.empty() ? ri : ri + '.' + rf;
        return (result_negative && result != "0") ? "-" + result : result;
    }
}

int main() {
    string filename;
    cout << "Enter filename: ";
    cin >> filename;

    vector<string_pair> doubles = load_doubles(filename);

    for (const auto& [a, b] : doubles) {
        if (!verify_double(a) || !verify_double(b)) {
            cout << "Invalid double(s): " << a << ", " << b << "\n";
            continue;
        }
        string sum = double_add(a, b);
        cout << a << " + " << b << " = " << sum << "\n";
    }
    return 0;
}   