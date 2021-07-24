#pragma once

#include <vector>
#include <string>
#include <algorithm>

template <typename T_STR_CONTAINER>
inline void
mstr_split(T_STR_CONTAINER& container,
           const typename T_STR_CONTAINER::value_type& str,
           const typename T_STR_CONTAINER::value_type& chars)
{
    container.clear();
    size_t i = 0, k = str.find_first_of(chars);
    while (k != T_STR_CONTAINER::value_type::npos)
    {
        container.push_back(str.substr(i, k - i));
        i = k + 1;
        k = str.find_first_of(chars, i);
    }
    container.push_back(str.substr(i));
}

template <typename T_STR_CONTAINER>
inline typename T_STR_CONTAINER::value_type
mstr_join(const T_STR_CONTAINER& container,
          const typename T_STR_CONTAINER::value_type& sep)
{
    typename T_STR_CONTAINER::value_type result;
    typename T_STR_CONTAINER::const_iterator it, end;
    it = container.begin();
    end = container.end();
    if (it != end)
    {
        result = *it;
        for (++it; it != end; ++it)
        {
            result += sep;
            result += *it;
        }
    }
    return result;
}

template <typename T_CHAR>
inline void mstr_trim(std::basic_string<T_CHAR>& str, const T_CHAR *spaces)
{
    typedef std::basic_string<T_CHAR> string_type;
    size_t i = str.find_first_not_of(spaces);
    size_t j = str.find_last_not_of(spaces);
    if ((i == string_type::npos) || (j == string_type::npos))
    {
        str.clear();
    }
    else
    {
        str = str.substr(i, j - i + 1);
    }
}

template <typename T_CHAR>
inline void
mstr_to_hex(std::basic_string<T_CHAR>& str, unsigned int value)
{
    static const char hex[] = "0123456789ABCDEF";
    str.clear();
    while (value)
    {
        str += T_CHAR(hex[value & 0xF]);
        value >>= 4;
    }
    std::reverse(str.begin(), str.end());
    if (str.empty())
        str += T_CHAR('0');
}

template <typename T_CHAR>
inline std::basic_string<T_CHAR>
mchr_to_hex(T_CHAR value)
{
    std::basic_string<T_CHAR> ret;
    if (sizeof(T_CHAR) == 1)
        mstr_to_hex(ret, (value & 0xFF));
    else if (sizeof(T_CHAR) == 2)
        mstr_to_hex(ret, (value & 0xFFFF));
    else if (sizeof(T_CHAR) == 4)
        mstr_to_hex(ret, (value & 0xFFFFFFFF));
    return ret;
}

template <typename T_CHAR>
inline std::basic_string<T_CHAR>
mstr_escape(const std::basic_string<T_CHAR>& str)
{
    std::basic_string<T_CHAR> ret;

    for (size_t i = 0; i < str.size(); ++i)
    {
        T_CHAR ch = str[i];
        switch (ch)
        {
        case T_CHAR('\"'): ret += T_CHAR('\"'); ret += T_CHAR('\"'); break;
        case T_CHAR('\\'): ret += T_CHAR('\\'); ret += T_CHAR('\\'); break;
        case T_CHAR('\0'): ret += T_CHAR('\\'); ret += T_CHAR('0'); break;
        case T_CHAR('\a'): ret += T_CHAR('\\'); ret += T_CHAR('a'); break;
        case T_CHAR('\b'): ret += T_CHAR('\\'); ret += T_CHAR('b'); break;
        case T_CHAR('\f'): ret += T_CHAR('\\'); ret += T_CHAR('f'); break;
        case T_CHAR('\n'): ret += T_CHAR('\\'); ret += T_CHAR('n'); break;
        case T_CHAR('\r'): ret += T_CHAR('\\'); ret += T_CHAR('r'); break;
        case T_CHAR('\t'): ret += T_CHAR('\\'); ret += T_CHAR('t'); break;
        case T_CHAR('\v'): ret += T_CHAR('\\'); ret += T_CHAR('v'); break;
        default:
            if (ch < 0x20)
            {
                ret += T_CHAR('\\');
                ret += T_CHAR('x');
                ret += mchr_to_hex(ch);
            }
            else
            {
                ret += ch;
            }
        }
    }

    return ret;
}

template <typename T_CHAR>
inline std::basic_string<T_CHAR>
mstr_escape(const T_CHAR *str)
{
    return mstr_escape(std::basic_string<T_CHAR>(str));
}

template <typename T_STR>
inline bool
mstr_replace_all(T_STR& str, const T_STR& from, const T_STR& to)
{
    bool ret = false;
    size_t i = 0;
    for (;;) {
        i = str.find(from, i);
        if (i == T_STR::npos)
            break;
        ret = true;
        str.replace(i, from.size(), to);
        i += to.size();
    }
    return ret;
}

template <typename T_STR>
inline bool
mstr_replace_all(T_STR& str,
                 const typename T_STR::value_type *from,
                 const typename T_STR::value_type *to)
{
    return mstr_replace_all(str, T_STR(from), T_STR(to));
}
