## [ascii_predicates](https://github.com/matgat/ascii_predicates.git)
[![linux-test](https://github.com/matgat/ascii_predicates/actions/workflows/linux-test.yml/badge.svg)](https://github.com/matgat/ascii_predicates/actions/workflows/linux-test.yml)
[![ms-test](https://github.com/matgat/ascii_predicates/actions/workflows/ms-test.yml/badge.svg)](https://github.com/matgat/ascii_predicates/actions/workflows/ms-test.yml)

A constexpr type safe replacement of `<cctype>` for c++,
providing predicates valid for codepoints less than `0x80`.

[basic example](https://gcc.godbolt.org/z/W5sqd5K69)

```cpp
#include "ascii_predicates.hpp" // ascii::is_*
static_assert( ascii::is_alnum('a') );
static_assert( not ascii::is_digit(U'⛵') );
int main()
{
    if( ascii::is_space('c') )
    {
        return 1;
    }
}
```

All the predicates can be called with either
`char`, `unsigned char`, or `char32_t`.
What about the others? `char8_t` and `char16_t` are codeunits,
it doesn't make sense to apply a predicate to them, must
be combined to form a `char32_t` codepoint.
Regarding `wchar_t`, leave it in the dark ages where belongs.


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


### Non-standard predicates

|                              |                                        |
|------------------------------|----------------------------------------|
| `ascii::is_blank()`          | aka `std::isspace() and !='\n'`        |
| `ascii::is_ident()`          | aka `std::isalnum() or =='_'`          |
| `ascii::is_float()`          | aka `std::isalnum() or =='+,-,.,E,e'`  |
| `ascii::is_space_or_punct()` | aka `std::isspace() or std::ispunct()` |
| `ascii::is_endline()`        | aka `==\n`                             |


### Helper predicates
Not strictly related to *ASCII*, useful

|                                        |                                      |
|----------------------------------------|--------------------------------------|
| `ascii::is_always_false()`             | always `false`                       |
| `ascii::is<C>()`                       | equality to a single codepoint       |
| `ascii::is_any_of<C,...>()`            | contained in a set of codepoints     |
| `ascii::is_none_of<C,...>()`           | not contained in a set of codepoints |



### Composite predicates

|                                        |
|----------------------------------------|
| `ascii::is_space_or_any_of<C,...>()`   |
| `ascii::is_alnum_or_any_of<C,...>()`   |
| `ascii::is_digit_or_any_of<C,...>()`   |
| `ascii::is_punct_and_none_of<C,...>()` |


### Case conversion

|                                 |
|---------------------------------|
| `ascii::to_lower(const char c)` |
| `ascii::to_upper(const char c)` |

This is included for completeness but deceiving, so is provided
just the `char` overload. Non very useful, since along with their
corresponding predicates `ascii::is_lower()` and `ascii::is_upper()`
are valid just for codepoints less than `0x80`.
If you need to check and convert case safely use a unicode library.


## Examples

---
[simple](https://gcc.godbolt.org/z/rYvbafh5f)

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
[overloads](https://gcc.godbolt.org/z/bafqoh6o4)

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
[simple loop](https://gcc.godbolt.org/z/zMbf7raTb)

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
[predicates lexer](https://gcc.godbolt.org/z/ce1s7G7Ez)

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

🍕🍞🧀🍇🍌☕🍄🌿🌸🔥💥🌋🌊💧🔩🔦💡🔌
