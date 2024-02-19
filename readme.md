## [ascii_predicates](https://github.com/matgat/ascii_predicates.git)
[![linux-test](https://github.com/matgat/ascii_predicates/actions/workflows/linux-test.yml/badge.svg)](https://github.com/matgat/ascii_predicates/actions/workflows/linux-test.yml)
[![ms-test](https://github.com/matgat/ascii_predicates/actions/workflows/ms-test.yml/badge.svg)](https://github.com/matgat/ascii_predicates/actions/workflows/ms-test.yml)
[![License: Unlicense](https://img.shields.io/badge/license-Unlicense-blue.svg)](http://unlicense.org/)

A single header c++ library that provides a `constexpr` type safe
replacement of `<cctype>`, a set of predicates valid for codepoints
less than `0x80`.

basic example ([godbolt](https://gcc.godbolt.org/z/xzoTTf3zT))

```cpp
#include "ascii_predicates.hpp" // ascii::is_*
static_assert( ascii::is_alnum('a') );
static_assert( not ascii::is_digit(U'⛵') );
static_assert( ascii::value_of_digit('3') == 3 );
static_assert( ascii::to_lower('A') == 'a' );
```


### Overloads
All the predicates can be called with either
`char`, `unsigned char`, or `char32_t`.
What about the others? `char8_t` and `char16_t` are codeunits,
it doesn't make sense to apply a predicate to them, must
be combined to form a `char32_t` codepoint.
Regarding `wchar_t`, leave it in the dark ages where belongs.

> [!IMPORTANT]
> Since this library is decontaminated from *locales* or *codepages*,
> the compatibility with `<cctype>` is ensured just for pure *ASCII* characters.

> [!NOTE]
> Generally expect a `false` result for codepoints `>= 0x80`.



---
### Standard predicates

|                     |                       |
|---------------------|-----------------------|
| `ascii::is_lower()` | aka `std::islower()`  |
| `ascii::is_upper()` | aka `std::isupper()`  |
| `ascii::is_space()` | aka `std::isspace()`  |
| `ascii::is_alpha()` | aka `std::isalpha()`  |
| `ascii::is_alnum()` | aka `std::isalnum()`  |
| `ascii::is_digit()` | aka `std::isdigit()`  |
| `ascii::is_xdigi()` | aka `std::isxdigit()` |
| `ascii::is_punct()` | aka `std::ispunct()`  |
| `ascii::is_cntrl()` | aka `std::iscntrl()`  |
| `ascii::is_graph()` | aka `std::isgraph()`  |
| `ascii::is_print()` | aka `std::isprint()`  |


---
### Non-standard predicates

|                              |                                        |
|------------------------------|----------------------------------------|
| `ascii::is_blank()`          | aka `std::isspace() and !='\n'`        |
| `ascii::is_ident()`          | aka `std::isalnum() or =='_'`          |
| `ascii::is_float()`          | aka `std::digit() or any of "+-.Ee"`   |
| `ascii::is_space_or_punct()` | aka `std::isspace() or std::ispunct()` |
| `ascii::is_endline()`        | aka `==\n`                             |
> [!NOTE]
> `is_blank()` is redefined to include all spaces except `\n` to ease
> the detection of any spaces in the current line.
> I find that treating `\r` as a generic formatting space is a good
> tradeoff to deal with the various end-of-line conventions.


---
### Helper predicates
Not strictly related to *ASCII*, provided for convenience
(see `std::predicate` in examples below).

|                                        |                                      |
|----------------------------------------|--------------------------------------|
| `ascii::is_always_false()`             | always `false`                       |
| `ascii::is<C>()`                       | equality to a single codepoint       |
| `ascii::is_any_of<C,...>()`            | contained in a set of codepoints     |
| `ascii::is_none_of<C,...>()`           | not contained in a set of codepoints |


---
### Composite predicates
A non exhaustive collection of examples of how basic and
helper predicates can be combined together.

|                                        |
|----------------------------------------|
| `ascii::is_space_or_any_of<C,...>()`   |
| `ascii::is_alnum_or_any_of<C,...>()`   |
| `ascii::is_digit_or_any_of<C,...>()`   |
| `ascii::is_punct_and_none_of<C,...>()` |


---
### Case conversion

|                                 |
|---------------------------------|
| `ascii::to_lower(const char c)` |
| `ascii::to_upper(const char c)` |

> [!CAUTION]
> Case conversion is included for completeness but since gives meaningful
> results just for codepoints less than `0x80`, it has little practical value.
> To avoid abuse is provided just the `char` overload.
> If you need to check and convert codepoints case, use a unicode library.


---
### Number conversion
As convenience for number literal parsing there's a function
`value_of_digit(ch)` that returns the numeric value associated
to the `is_xdigi()`/`is_digit()` character.
> [!NOTE]
> The returned type is `std::uint8_t`, the easiest to promote
to other integral types.

```cpp
static_assert( ascii::value_of_digit('4') == 4 );
static_assert( ascii::value_of_digit('b') == 0xB );
static_assert( ascii::value_of_digit('z') == 0 );
```


## Building
This library uses concepts, so you need a *c++20* compliant compiler
indicating at least `-std=c++20` (`/std:c++20` in case of *msvc*).

### Testing
Run unit tests directly in [godbolt](https://gcc.godbolt.org/z/T8zcdexdz)
or:

```sh
$ git clone https://github.com/matgat/ascii_predicates.git
$ cd ascii_predicates
$ curl -O https://raw.githubusercontent.com/boost-ext/ut/master/include/boost/ut.hpp
$ g++ -std=c++20 -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -o test test.cpp && ./test
```

> [!NOTE]
> On windows:
> ```bat
> $ cl /std:c++latest /permissive- /utf-8 /W4 /WX /EHsc test.cpp
> ```


---
## Examples

---
### simple ([godbolt](https://gcc.godbolt.org/z/559vcffsr))

```cpp
#include "ascii_predicates.hpp" // ascii::is_*

void query_char(const char ch)
{
    const bool alnum = ascii::is_alnum(ch);
    const bool space = ascii::is_space(ch);
    //...
    const bool l = ascii::is<'l'>(ch);
    const bool l_or_o = ascii::is_any_of<'l','o'>(ch);
    const bool some_punct = ascii::is_punct_and_none_of<'#','&'>(ch);
}
```

---
### overloads ([godbolt](https://gcc.godbolt.org/z/9TcdPEsE5))

```cpp
#include <iostream>
#include <string_view>
using namespace std::literals; // "..."sv
#include "ascii_predicates.hpp" // ascii::is_*

int main()
{
    for( const char ch : "hello, @@"sv )
    {
        std::cout << ascii::is_alnum_or_any_of<' '>(ch) << ch << ' ';
    }
    std::cout << '\n';

    for( const char32_t cp : U"hello, ✏️🗒️"sv )
    {
        std::cout << ascii::is_alnum_or_any_of<U' '>(cp) << to_utf8(cp) << ' ';
    }
}
```

---
### simple loop ([godbolt](https://gcc.godbolt.org/z/P1ccjEc3o))

```cpp
#include <iostream>
#include <string_view>
using namespace std::literals; // "..."sv
#include "ascii_predicates.hpp" // ascii::is_*

int main()
{
    const auto csv = U"😊, 😇😇; 😂... 🤣"sv;
    std::size_t i = 0;
    while( i<csv.size() )
    {
        if( ascii::is_punct(csv[i]) ) ++i;
        else if( ascii::is_space(csv[i]) ) ++i;
        else
           {
            std::cout << ' ' << i << ':' << to_utf8(csv[i]);
            ++i;
           }
    }
}
```


---
### std algorithm ([godbolt](https://gcc.godbolt.org/z/54hdGYWeP))

```cpp
#include <iostream>
#include <ranges>
#include <algorithm>
#include <string_view>
#include "ascii_predicates.hpp" // ascii::is_*

int main()
{
    std::string_view str = "123 12a 456 d34";

    auto tokens = str | std::views::split(' ')
                      | std::views::transform([](auto range){ return std::string_view(&*range.begin(), std::ranges::distance(range));})
                      | std::views::filter( [](auto s){ return !s.empty(); });
    for(auto token : tokens)
       {
        if( std::ranges::all_of(token, [](const char c){ return ascii::is_digit(c); }) )
           {
            std::cout << "digits: " << token << '\n';
           }
       }
}
```

---
### predicates lexer ([godbolt](https://gcc.godbolt.org/z/zjPMnhKvd))

```cpp
#include <iostream>
#include <concepts> // std::predicate
#include <string_view>
using namespace std::literals; // "..."sv
#include "ascii_predicates.hpp" // ascii::is_*

class SimpleLexer
{
 public:
    const std::string_view input;
 private:
    std::size_t i = 0;

 public:
    constexpr explicit SimpleLexer(const std::string_view buf) noexcept
      : input(buf)
       {}

    [[nodiscard]] constexpr std::size_t pos() const noexcept { return i; }
    [[nodiscard]] constexpr bool got_data() const noexcept { return i<input.size(); }
    [[maybe_unused]] constexpr bool get_next() noexcept
       {
        if( i<input.size() ) [[likely]]
           {
            return ++i<input.size();
           }
        return false; // 'i' points to one past next
       }

    template<std::predicate<const char> CharPredicate =decltype(ascii::is_always_false<char>)>
    [[nodiscard]] constexpr bool got(CharPredicate is) const noexcept { return i<input.size() and is(input[i]); }

    template<std::predicate<const char> CharPredicate =decltype(ascii::is_always_false<char>)>
    constexpr void skip_while(CharPredicate is) noexcept
       { while( got(is) and get_next() ); }

    template<std::predicate<const char> CharPredicate =decltype(ascii::is_always_false<char>)>
    [[nodiscard]] constexpr std::string_view get_while(CharPredicate is) noexcept
       {
        const std::size_t i_start = pos();
        while( got(is) and get_next() );
        return {input.data()+i_start, pos()-i_start};
       }
};

int main()
{
    SimpleLexer lexer(" len = 23 "sv);
    lexer.skip_while( ascii::is_space );
    const std::string_view var_nam = lexer.get_while(ascii::is_alnum);
    lexer.skip_while( ascii::is_space );
    if( lexer.got( ascii::is<'='>) ) lexer.get_next();
    lexer.skip_while( ascii::is_space );
    const std::string_view var_val = lexer.get_while(ascii::is_digit);
    std::cout << var_nam << ' ' << var_val << '\n';
}

```

---
### number literals parser ([godbolt](https://gcc.godbolt.org/z/dPYr7cjxW))

```cpp
#include <iostream>
#include <format>
#include <string_view>
#include <stdexcept>
#include <concepts>
#include <limits>
#include "ascii_predicates.hpp" // ascii::is_*

class SimpleParser
{
 public:
    const std::string_view input;
 private:
    std::size_t i = 0;

 public:
    constexpr explicit SimpleParser(const std::string_view buf) noexcept
      : input(buf)
       {}

    [[nodiscard]] constexpr bool got_digit() const noexcept { return i<input.size() and ascii::is_digit(input[i]); }
    [[nodiscard]] constexpr char curr_codepoint() const noexcept { return i<input.size() ? input[i] : '\0'; }
    [[maybe_unused]] constexpr bool get_next() noexcept
       {
        if( i<input.size() ) [[likely]]
           {
            return ++i<input.size();
           }
        return false;
       }

    template<std::unsigned_integral Uint>
    [[nodiscard]] constexpr Uint extract_index()
       {
        if( not got_digit() )
           {
            throw std::runtime_error( std::format("Invalid char '{}' in number literal", curr_codepoint()) );
           }
    
        Uint result = ascii::value_of_digit(curr_codepoint());
        constexpr Uint base = 10u;
        constexpr Uint overflow_limit = ((std::numeric_limits<Uint>::max() - (base - 1u)) / (base)) - 1u;
        while( get_next() and got_digit() )
           {
            if( result>overflow_limit )
               {
                throw std::runtime_error( std::format("Integer literal overflow ({}x{} would exceed {})", result, base, std::numeric_limits<Uint>::max()) );
               }
            result = (base*result) + ascii::value_of_digit(curr_codepoint());
           }
        return result;
       }
};

int main()
{
    SimpleParser parser("32759");
    std::cout << parser.extract_index<unsigned int>() << '\n';
}

```


🍕🍞🧀🍇🍌☕🍄🌿🌸🔥💥🌋🌊💧🔩🔦💡🔌



---
## Appendix

### The standard [`<cctype>`](https://en.cppreference.com/w/cpp/header/cctype) behavior

| ASCII     | Characters                | is cntrl   | is print | is graph | is space | is blank | is punct | is alnum | is alpha | is upper | is lower | is digit | is xdigit |
|-----------|---------------------------|:----------:|:--------:|:--------:|:--------:|:--------:|:--------:|:--------:|:--------:|:--------:|:--------:|:--------:|:---------:|
| 0 ÷ 8     | control codes (`NUL`, …)  |    ▣      |          |          |          |          |          |          |          |          |          |          |           |
| 9         | tab (`\t`)                |    ▣      |          |          |    ▣     |    ▣     |          |          |          |          |          |          |           |
| 10 ÷ 13   | whitespaces (`\n`,`\v`,`\f`,`\r`) | ▣ |          |          |    ▣     |          |          |          |          |          |          |          |           |
| 14 ÷ 31   | control codes (`SO`, …) |    ▣      |          |          |          |          |          |          |          |          |          |          |           |
| 32        | space (`' '`)             |           |    ▣     |          |    ▣     |    ▣     |          |          |          |          |          |          |           |
| 33 ÷ 47   | `!\"#$%&'` `()*+,-./`    |           |    ▣     |    ▣     |          |          |    ▣     |          |          |          |          |          |           |
| 48 ÷ 57   | `0123456789`              |           |    ▣     |    ▣     |          |          |          |    ▣     |          |          |          |    ▣     |    ▣      |
| 58 ÷ 64   | `:;<=>?@`           |           |    ▣     |    ▣     |          |          |    ▣     |          |          |          |          |          |           |
| 65 ÷ 70   | `ABCDEF`                  |           |    ▣     |    ▣     |          |          |          |    ▣     |    ▣     |    ▣     |          |          |    ▣      |
| 71 ÷ 90   | `GHIJKLMNOPQ` `RSTUVWXYZ`    |           |    ▣     |    ▣     |          |          |          |    ▣     |    ▣     |    ▣     |          |          |           |
| 91 ÷ 96   | `` [\\]^_` ``                  |           |    ▣     |    ▣     |          |          |    ▣     |          |          |          |          |          |           |
| 97 ÷ 102  | `abcdef`                  |           |    ▣     |    ▣     |          |          |          |    ▣     |    ▣     |          |    ▣     |          |    ▣      |
| 103 ÷ 122 | `ghijklmnopq` `rstuvwxyz`    |           |    ▣     |    ▣     |          |          |          |    ▣     |    ▣     |          |    ▣     |          |           |
| 123 ÷ 126 | `{\|}~ `                   |           |    ▣     |    ▣     |          |          |    ▣     |          |          |          |          |          |           |
| 127       | backspace (`DEL`)         |    ▣      |          |          |          |          |          |          |          |          |          |          |           |
