## [ascii_predicates](https://github.com/matgat/ascii_predicates.git)
[![linux-build](https://github.com/matgat/ascii_predicates/actions/workflows/linux-build.yml/badge.svg)](https://github.com/matgat/ascii_predicates/actions/workflows/linux-build.yml)
[![ms-build](https://github.com/matgat/ascii_predicates/actions/workflows/ms-build.yml/badge.svg)](https://github.com/matgat/ascii_predicates/actions/workflows/ms-build.yml)

A type safe replacement of `<cctype>` for c++.
Tired of casting to unsigned char `std::isalnum(static_cast<unsigned char>(ch))`.

```cpp
#include <string_view>
using namespace std::literals; // "..."sv

#include "ascii_predicates.hpp" // ascii::is_*

int main()
{
    for( const char ch : "Hello, 123 \n"sv )
    {
        const bool lower = asci::is_lower(ch);
        const bool alnum = asci::is_alnum(ch);
        const bool space = asci::is_space(ch);
        //...
        const bool l = asci::is<'l'>(ch);
        const bool l_or_o = asci::is_any_of<'l','o'>(ch);
        const bool some_punct = asci::is_punct_and_none_of<'#','&'>(ch);
    }
}
```

Does the following make sense?
Yes if you're parsing markup (ASCII punctuation):

```cpp
for( const char32_t cp : U"<てぃあじぃ>❤️</てぃあじぃ>"sv )
{
    if( ascii::is<U'>'>(cp) ) break; // found >
}
```

An example passing predicates:

```cpp
#include <concepts> // std::predicate

class SimpleParser
{
 public:
    const std::u32string_view input;
    std::size_t i = 0;

 public:
    constexpr explicit simple_parser(const std::u32string_view buf) noexcept
      : input(buf)
       {}

    template<std::predicate<const char32_t> CharPredicate =decltype(ascii::is_always_false<char32_t>)>
    constexpr void skip_while(CharPredicate is =ascii::is<U'😊'>) noexcept
       {
        while( i<input.size() and is(input[i]) ) ++i;
       }
};

SimpleParser p(U"🤪  😎  abc123");
p.skip_while( ascii::is_space_or_any_of<U'😊',U'🤪',U'😎'> );
```

