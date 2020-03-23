#include <string>
#include <filesystem>

template <typename TP>
std::time_t to_time_t(TP tp)
{
    using namespace std::chrono;
    // The following line is an ugly workaround. Because std::chrono will be fully
    // available only in C++20 I didn't found a better solution. Adapted from: 
    // https://stackoverflow.com/questions/56788745/how-to-convert-stdfilesystemfile-time-type-to-a-string-using-gcc-9
    auto sctp = time_point_cast<system_clock::duration>(tp - TP::clock::now()
        + system_clock::now());
    return system_clock::to_time_t(sctp);
}
std::string createIsoTimeString(std::filesystem::file_time_type fileTime);

// char_traits is copy paste from 
// https://en.cppreference.com/w/cpp/string/char_traits
struct ci_char_traits : public std::char_traits<char> {
    static bool eq(char c1, char c2);
    static bool ne(char c1, char c2);
    static bool lt(char c1, char c2);
    static int compare(const char* s1, const char* s2, size_t n);
    static const char* find(const char* s, int n, char a);
};

typedef std::basic_string<char, ci_char_traits> ci_string;

std::ostream& operator<<(std::ostream& os, const ci_string& str);