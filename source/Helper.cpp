#include "Helper.h"

#include <iomanip>
#include <sstream>

namespace fs = std::filesystem;


std::string createIsoTimeString(fs::file_time_type fileTime)
{
    std::time_t tt = to_time_t(fileTime);
    std::tm* gmt = std::gmtime(&tt);
    std::stringstream buffer;
    buffer << std::put_time(gmt, "%Y-%M-%dT%H:%M:%S");
    return buffer.str();
}


bool ci_char_traits::eq(char c1, char c2) { return toupper(c1) == toupper(c2); }
bool ci_char_traits::ne(char c1, char c2) { return toupper(c1) != toupper(c2); }
bool ci_char_traits::lt(char c1, char c2) { return toupper(c1) < toupper(c2); }

int ci_char_traits::compare(const char* s1, const char* s2, size_t n) 
{
    while (n-- != 0) 
    {
        if (toupper(*s1) < toupper(*s2)) return -1;
        if (toupper(*s1) > toupper(*s2)) return 1;
        ++s1; ++s2;
    }
    return 0;
}
const char* ci_char_traits::find(const char* s, int n, char a)
{
    while (n-- > 0 && toupper(*s) != toupper(a)) 
    {
        ++s;
    }
    return s;
}

std::ostream& operator<<(std::ostream& os, const ci_string& str) {
    return os.write(str.data(), str.size());
}