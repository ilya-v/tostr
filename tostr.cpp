#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <chrono>
#include <cstdint>
#include <type_traits>
using namespace std;

char line[1024];
char *p = line;


void to_str(const char *s)  {    while (*s) *p++ = *s++;  }
void to_str(const string &s) { for (auto q = s.c_str(); *q; *p++ = *q++); }

template <typename T, class = typename enable_if<is_integral<T>::value>::type >
void to_str(T t)
{
    if (t < 0)  *p++ = '-';
    *p++ = '0';
    for (auto s = t/10; s; s/=10, p++);
    for (char *q = p - 1; t; *q-- = (char)(t%10 + '0'), t/=10);
}


template<typename T, typename... Args>
void to_str(const char *s, T value, Args... args)
{
    for (; *s && *s != '%'; s++)
      *(p++) = *s;
      
    for (; *s != 0   && *s != ' ' && *s != 'i' && *s != 'd' && *s != 'u' 
        && *s != 'o' && *s != 'x' && *s != 'X' && *s != 'f' && *s != 'F' 
        && *s != 'g' && *s != 'G' && *s != 'a' && *s != 'A' && *s != 'c' 
        && *s != 'p' && *s != 's';    s++);
    
    if (*s != 0) s++;
    
    to_str( value );
    to_str( s, args...);
}

template <typename ...Args>
void Fstr(const char *s, Args... args)
{
    p=line;
    to_str(s, args...);
    *p = 0;
}

template <typename T>
void tim(T&t)
{
    cout<< 
    chrono::duration_cast<chrono::milliseconds>( chrono::steady_clock::now() - t ).count()
    << endl;
    t = chrono::steady_clock::now();
}

int main() {
    
    const int N = 1000000/2;
    char buf[1024] = {};
    
    auto t = chrono::steady_clock::now();
    
    for (int i =0 ; i< N; i++)
        Fstr("Log string %d %d %s logged", 100500, 20, "30000");
        
    tim(t);
    
    for (int i =0 ; i< N; i++)
        sprintf(buf, "Log string %lld %d %s logged", (int64_t)100500, 20, "30000");
    tim(t);
            
    for (int i =0 ; i< N; i++)
        Fstr("Log string %llu %llu %lf %s logged", (uint64_t)(-1), (uint64_t)(-1), 1.7, "=========");
    tim(t);
    
    for (int i =0 ; i< N; i++)
        sprintf(buf, "Log string %llu %llu %lf %s logged", (uint64_t)(-1), (uint64_t)(-1), 1.7, "=========");
    tim(t);

    cout <<line << "."<<endl;
    cout <<buf << "."<<endl;



}