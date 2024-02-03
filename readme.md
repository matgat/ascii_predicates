## [ascii_predicates](https://github.com/matgat/ascii_predicates.git)
[![linux-test](https://github.com/matgat/ascii_predicates/actions/workflows/linux-test.yml/badge.svg)](https://github.com/matgat/ascii_predicates/actions/workflows/linux-test.yml)
[![ms-test](https://github.com/matgat/ascii_predicates/actions/workflows/ms-test.yml/badge.svg)](https://github.com/matgat/ascii_predicates/actions/workflows/ms-test.yml)

A constexpr type safe replacement of `<cctype>` for c++,
providing predicates valid for codepoints less than `0x80`.

```cpp
#include "ascii_predicates.hpp" // ascii::is_*
static_assert( ascii::is_alnum('a') and not ascii::is_digit(U'⛵') );
int main()
{
    if( ascii::is_space('c') )
    {
        ...
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


### Non-standard or composite predicates

|                              |                                        |
|------------------------------|----------------------------------------|
| `ascii::is_blank()`          | aka `std::isspace() and !='\n'`        |
| `ascii::is_ident()`          | aka `std::isalnum() or =='_'`          |
| `ascii::is_space_or_punct()` | aka `std::isspace() or std::ispunct()` |
| `ascii::is_endline()`        | aka `==\n`                             |


### Conveniency predicates
Not all strictly related to *ASCII*, useful when passing
predicates around.

|                                        |                                      |
|----------------------------------------|--------------------------------------|
| `ascii::is_always_false()`             | always `false`                       |
| `ascii::is<C>()`                       | equality to a single codepoint       |
| `ascii::is_any_of<C,...>()`            | contained in a set of codepoints     |
| `ascii::is_none_of<C,...>()`           | not contained in a set of codepoints |
| `ascii::is_space_or_any_of<C,...>()`   |                                      |
| `ascii::is_alnum_or_any_of<C,...>()`   |                                      |
| `ascii::is_digit_or_any_of<C,...>()`   |                                      |
| `ascii::is_punct_and_none_of<C,...>()` |                                      |


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


```cpp
#include <string_view>
using namespace std::literals; // "..."sv

#include "ascii_predicates.hpp" // ascii::is_*

int main()
{
    for( const char ch : "Hello, 123 \n"sv )
    {
        const bool alnum = ascii::is_alnum(ch);
        const bool space = ascii::is_space(ch);
        //...
        const bool l = ascii::is<'l'>(ch);
        const bool l_or_o = ascii::is_any_of<'l','o'>(ch);
        const bool some_punct = ascii::is_punct_and_none_of<'#','&'>(ch);
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
