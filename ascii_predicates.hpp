#pragma once
//  ---------------------------------------------
//  ASCII encoding predicates and facilities
//  ---------------------------------------------
//  #include "ascii_predicates.hpp" // ascii::is_*
//  ---------------------------------------------
#include <concepts> // std::same_as<>
#include <cstdint> // std::uint16_t
//#include <limits> // std::numeric_limits


//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
namespace ascii //:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
{

//----------------------------------------------------------------------------
// Should be called is_probably_ascii() but it's too long
[[nodiscard]] constexpr bool is_ascii(const char32_t cp) noexcept
   {
    return cp<U'\x80';
   }


namespace details //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
{   // The standard <cctype> behavior:
    // |         |                           |is   |is   |is   |is   |is   |is   |is   |is   |is   |is   |is   |is    |
    // | ASCII   | characters                |cntrl|print|graph|space|blank|punct|alnum|alpha|upper|lower|digit|xdigit|
    // |---------|---------------------------|-----|-----|-----|-----|-----|-----|-----|-----|-----|-----|-----|------|
    // | 0–8     | control codes (NUL, etc.) |  X  |     |     |     |     |     |     |     |     |     |     |      |
    // | 9       | tab (\t)                  |  X  |     |     |  X  |  X  |     |     |     |     |     |     |      |
    // | 10–13   | whitespaces (\n,\v,\f,\r) |  X  |     |     |  X  |     |     |     |     |     |     |     |      |
    // | 14–31   | control codes             |  X  |     |     |     |     |     |     |     |     |     |     |      |
    // | 32      | space                     |     |  X  |     |  X  |  X  |     |     |     |     |     |     |      |
    // | 33–47   | !"#$%&amp;'()*+,-./       |     |  X  |  X  |     |     |  X  |     |     |     |     |     |      |
    // | 48–57   | 0123456789                |     |  X  |  X  |     |     |     |  X  |     |     |     |  X  |  X   |
    // | 58–64   | :;&lt;=&gt;?@             |     |  X  |  X  |     |     |  X  |     |     |     |     |     |      |
    // | 65–70   | ABCDEF                    |     |  X  |  X  |     |     |     |  X  |  X  |  X  |     |     |  X   |
    // | 71–90   | GHIJKLMNOPQRSTUVWXYZ      |     |  X  |  X  |     |     |     |  X  |  X  |  X  |     |     |      |
    // | 91–96   | [\]^_`                    |     |  X  |  X  |     |     |  X  |     |     |     |     |     |      |
    // | 97–102  | abcdef                    |     |  X  |  X  |     |     |     |  X  |  X  |     |  X  |     |  X   |
    // | 103–122 | ghijklmnopqrstuvwxyz      |     |  X  |  X  |     |     |     |  X  |  X  |     |  X  |     |      |
    // | 123–126 | {|}~                      |     |  X  |  X  |     |     |  X  |     |     |     |     |     |      |
    // | 127     | backspace character (DEL) |  X  |     |     |     |     |     |     |     |     |     |     |      |

    using mask_t = std::uint16_t;
    enum : mask_t
       {
        ISLOWER = 0b0000'0000'0000'0001 // std::islower
       ,ISUPPER = 0b0000'0000'0000'0010 // std::isupper
       ,ISSPACE = 0b0000'0000'0000'0100 // std::isspace
       ,ISBLANK = 0b0000'0000'0000'1000 // std::isspace and not '\n' (not equiv to std::isblank)
       ,ISALPHA = 0b0000'0000'0001'0000 // std::isalpha
       ,ISALNUM = 0b0000'0000'0010'0000 // std::isalnum
       ,ISDIGIT = 0b0000'0000'0100'0000 // std::isdigit
       ,ISXDIGI = 0b0000'0000'1000'0000 // std::isxdigit
       ,ISPUNCT = 0b0000'0001'0000'0000 // std::ispunct
       ,ISCNTRL = 0b0000'0010'0000'0000 // std::iscntrl
       ,ISGRAPH = 0b0000'0100'0000'0000 // std::isgraph
       ,ISPRINT = 0b0000'1000'0000'0000 // std::isprint
       // Extended
       ,ISIDENT = 0b0001'0000'0000'0000 // std::isalnum or '_'
       ,ISFLOAT = 0b0010'0000'0000'0000 // std::isdigit or any of "+-.Ee"
       //,ISXXXXX = 0b0100'0000'0000'0000
       //,ISXXXXX = 0b1000'0000'0000'0000
       };

    static_assert( sizeof(unsigned char)==1 ); // Should really check CHAR_BIT?
    static constexpr mask_t ascii_lookup_table[256] =
      { 0x200 , 0x200 , 0x200 , 0x200 , 0x200 , 0x200 , 0x200 , 0x200 , 0x200 , 0x20C , 0x204 , 0x20C , 0x20C , 0x20C , 0x200 , 0x200 ,
        0x200 , 0x200 , 0x200 , 0x200 , 0x200 , 0x200 , 0x200 , 0x200 , 0x200 , 0x200 , 0x200 , 0x200 , 0x200 , 0x200 , 0x200 , 0x200 ,
        0x80C , 0xD00 , 0xD00 , 0xD00 , 0xD00 , 0xD00 , 0xD00 , 0xD00 , 0xD00 , 0xD00 , 0xD00 , 0x2D00, 0xD00 , 0x2D00, 0x2D00, 0xD00 ,
        0x3CE0, 0x3CE0, 0x3CE0, 0x3CE0, 0x3CE0, 0x3CE0, 0x3CE0, 0x3CE0, 0x3CE0, 0x3CE0, 0xD00 , 0xD00 , 0xD00 , 0xD00 , 0xD00 , 0xD00 ,
        0xD00 , 0x1CB2, 0x1CB2, 0x1CB2, 0x1CB2, 0x3CB2, 0x1CB2, 0x1C32, 0x1C32, 0x1C32, 0x1C32, 0x1C32, 0x1C32, 0x1C32, 0x1C32, 0x1C32,
        0x1C32, 0x1C32, 0x1C32, 0x1C32, 0x1C32, 0x1C32, 0x1C32, 0x1C32, 0x1C32, 0x1C32, 0x1C32, 0xD00 , 0xD00 , 0xD00 , 0xD00 , 0x1D00,
        0xD00 , 0x1CB1, 0x1CB1, 0x1CB1, 0x1CB1, 0x3CB1, 0x1CB1, 0x1C31, 0x1C31, 0x1C31, 0x1C31, 0x1C31, 0x1C31, 0x1C31, 0x1C31, 0x1C31,
        0x1C31, 0x1C31, 0x1C31, 0x1C31, 0x1C31, 0x1C31, 0x1C31, 0x1C31, 0x1C31, 0x1C31, 0x1C31, 0xD00 , 0xD00 , 0xD00 , 0xD00 , 0x200 ,
        0x0   , 0x0   , 0x0   , 0x0   , 0x0   , 0x0   , 0x0   , 0x0   , 0x0   , 0x0   , 0x0   , 0x0   , 0x0   , 0x0   , 0x0   , 0x0,
        0x0   , 0x0   , 0x0   , 0x0   , 0x0   , 0x0   , 0x0   , 0x0   , 0x0   , 0x0   , 0x0   , 0x0   , 0x0   , 0x0   , 0x0   , 0x0,
        0x0   , 0x0   , 0x0   , 0x0   , 0x0   , 0x0   , 0x0   , 0x0   , 0x0   , 0x0   , 0x0   , 0x0   , 0x0   , 0x0   , 0x0   , 0x0,
        0x0   , 0x0   , 0x0   , 0x0   , 0x0   , 0x0   , 0x0   , 0x0   , 0x0   , 0x0   , 0x0   , 0x0   , 0x0   , 0x0   , 0x0   , 0x0,
        0x0   , 0x0   , 0x0   , 0x0   , 0x0   , 0x0   , 0x0   , 0x0   , 0x0   , 0x0   , 0x0   , 0x0   , 0x0   , 0x0   , 0x0   , 0x0,
        0x0   , 0x0   , 0x0   , 0x0   , 0x0   , 0x0   , 0x0   , 0x0   , 0x0   , 0x0   , 0x0   , 0x0   , 0x0   , 0x0   , 0x0   , 0x0,
        0x0   , 0x0   , 0x0   , 0x0   , 0x0   , 0x0   , 0x0   , 0x0   , 0x0   , 0x0   , 0x0   , 0x0   , 0x0   , 0x0   , 0x0   , 0x0,
        0x0   , 0x0   , 0x0   , 0x0   , 0x0   , 0x0   , 0x0   , 0x0   , 0x0   , 0x0   , 0x0   , 0x0   , 0x0   , 0x0   , 0x0   , 0x0 };

    //-----------------------------------------------------------------------
    [[nodiscard]] inline constexpr bool check(const unsigned char ch, const mask_t mask) noexcept
       {
        // When a static constexpr variable can be inside a constexpr function, move the table here
        //static_assert( std::numeric_limits<unsigned char>::max()+1 < ascii_lookup_table.size() ); // <numeric_limits> and <array>
        return ascii_lookup_table[ch] & mask;
       }
    [[nodiscard]] inline constexpr bool check(const char ch, const mask_t mask) noexcept
       {
        return check(static_cast<unsigned char>(ch), mask);
       }
    [[nodiscard]] inline constexpr bool check(const char32_t cp, const mask_t mask) noexcept
       {
        // Good enough for parsing code where non ASCII codepoints are just in text or comments
        if( ascii::is_ascii(cp) ) [[likely]]
           {
            return check(static_cast<unsigned char>(cp), mask);
           }
        return false;
       }

    // Facility for case conversion
    template<bool SET>
    [[nodiscard]] inline constexpr char set_case_bit(char c) noexcept
       {
        static constexpr char case_bit = '\x20';
        if constexpr (SET) c |= case_bit;
        else               c &= ~case_bit;
        return c;
       }
} //::::::::::::::::::::::::::::::: details ::::::::::::::::::::::::::::::::::

// Supporting the following character types:
template<typename T> concept CharLike = std::same_as<T, char> or
                                        std::same_as<T, unsigned char> or
                                        std::same_as<T, char32_t>;

// Standard predicates
template<CharLike Chr> [[nodiscard]] constexpr bool is_lower(const Chr c) noexcept { return details::check(c, details::ISLOWER); }
template<CharLike Chr> [[nodiscard]] constexpr bool is_upper(const Chr c) noexcept { return details::check(c, details::ISUPPER); }
template<CharLike Chr> [[nodiscard]] constexpr bool is_space(const Chr c) noexcept { return details::check(c, details::ISSPACE); }
template<CharLike Chr> [[nodiscard]] constexpr bool is_alpha(const Chr c) noexcept { return details::check(c, details::ISALPHA); }
template<CharLike Chr> [[nodiscard]] constexpr bool is_alnum(const Chr c) noexcept { return details::check(c, details::ISALNUM); }
template<CharLike Chr> [[nodiscard]] constexpr bool is_digit(const Chr c) noexcept { return details::check(c, details::ISDIGIT); }
template<CharLike Chr> [[nodiscard]] constexpr bool is_xdigi(const Chr c) noexcept { return details::check(c, details::ISXDIGI); }
template<CharLike Chr> [[nodiscard]] constexpr bool is_punct(const Chr c) noexcept { return details::check(c, details::ISPUNCT); }
template<CharLike Chr> [[nodiscard]] constexpr bool is_cntrl(const Chr c) noexcept { return details::check(c, details::ISCNTRL); }
template<CharLike Chr> [[nodiscard]] constexpr bool is_graph(const Chr c) noexcept { return details::check(c, details::ISGRAPH); }
template<CharLike Chr> [[nodiscard]] constexpr bool is_print(const Chr c) noexcept { return details::check(c, details::ISPRINT); }
// Non standard ones
template<CharLike Chr> [[nodiscard]] constexpr bool is_blank(const Chr c) noexcept { return details::check(c, details::ISBLANK); } // not equiv to std::isblank
template<CharLike Chr> [[nodiscard]] constexpr bool is_ident(const Chr c) noexcept { return details::check(c, details::ISIDENT); } // std::isalnum or _
template<CharLike Chr> [[nodiscard]] constexpr bool is_float(const Chr c) noexcept { return details::check(c, details::ISFLOAT); } // std::isdigit or +,-,.,E,e
template<CharLike Chr> [[nodiscard]] constexpr bool is_space_or_punct(const Chr c) noexcept { return details::check(c, details::ISSPACE | details::ISPUNCT); }
template<CharLike Chr> [[nodiscard]] constexpr bool is_endline(const Chr c) noexcept { return c==static_cast<Chr>('\n'); }


//---------------------------------------------------------------------------
// Helper predicates (not strictly ASCII related)
template<CharLike Chr> [[nodiscard]] constexpr bool is_always_false(const Chr) noexcept
   {
    return false;
   }

template<CharLike auto CH>
[[nodiscard]] constexpr bool is(const decltype(CH) ch) noexcept
   {
    return ch==CH;
   }

template<CharLike auto CH1, decltype(CH1)... CHS>
[[nodiscard]] constexpr bool is_any_of(const decltype(CH1) ch) noexcept
   {
    return ch==CH1 or ((ch==CHS) or ...);
   }

template<CharLike auto CH1, decltype(CH1)... CHS>
[[nodiscard]] constexpr bool is_none_of(const decltype(CH1) ch) noexcept
   {
    return ch!=CH1 and ((ch!=CHS) and ...);
   }

//template<CharLike auto CH>
//[[nodiscard]] constexpr bool is_greater_than(const decltype(CH) ch) noexcept
//   {
//    return ch>CH;
//   }

//template<CharLike auto CH>
//[[nodiscard]] constexpr bool is_less_than(const decltype(CH) ch) noexcept
//   {
//    return ch<CH;
//   }

//template<CharLike auto CH1, CharLike auto CH2>
//[[nodiscard]] constexpr bool is_between(const decltype(CH) ch) noexcept
//   {
//    return ch>=CH1 and ch<=CH2;
//   }



//---------------------------------------------------------------------------
// Some examples of non-type parameterized template composite predicates

template<CharLike auto CH1, decltype(CH1)... CHS>
[[nodiscard]] constexpr bool is_space_or_any_of(const decltype(CH1) ch) noexcept
   {
    return is_space(ch) or is_any_of<CH1, CHS ...>(ch);
   }

template<CharLike auto CH1, decltype(CH1)... CHS>
[[nodiscard]] constexpr bool is_alnum_or_any_of(const decltype(CH1) ch) noexcept
   {
    return is_alnum(ch) or is_any_of<CH1, CHS ...>(ch);
   }

template<CharLike auto CH1, decltype(CH1)... CHS>
[[nodiscard]] constexpr bool is_digit_or_any_of(const decltype(CH1) ch) noexcept
   {
    return is_digit(ch) or is_any_of<CH1, CHS ...>(ch);
   }

template<CharLike auto CH1, decltype(CH1)... CHS>
[[nodiscard]] constexpr bool is_punct_and_none_of(const decltype(CH1) ch) noexcept
   {
    return is_punct(ch) and is_none_of<CH1, CHS ...>(ch);
   }



//---------------------------------------------------------------------------
// Case conversion (only for ASCII char, so not so useful!)
[[nodiscard]] constexpr char to_lower(char c) noexcept { if(is_upper(c)) c = details::set_case_bit<true>(c); return c; }
[[nodiscard]] constexpr char to_upper(char c) noexcept { if(is_lower(c)) c = details::set_case_bit<false>(c); return c; }

}//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::



/////////////////////////////////////////////////////////////////////////////
#ifdef TEST_UNITS ///////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
static ut::suite<"MG::ascii_predicates"> ascii_predicates_tests = []
{////////////////////////////////////////////////////////////////////////////

ut::test("basic predicates") = []
   {
    ut::expect( ut::that % not ascii::is_space('a') );
    ut::expect( ut::that % not ascii::is_blank('a') );
    ut::expect( ut::that % not ascii::is_endline('a') );
    ut::expect( ut::that % ascii::is_alpha('a') );
    ut::expect( ut::that % ascii::is_alnum('a') );
    ut::expect( ut::that % not ascii::is_digit('a') );
    ut::expect( ut::that % ascii::is_xdigi('a') );
    ut::expect( ut::that % not ascii::is_punct('a') );
    ut::expect( ut::that % not ascii::is_cntrl('a') );
    ut::expect( ut::that % ascii::is_graph('a') );
    ut::expect( ut::that % ascii::is_print('a') );
    ut::expect( ut::that % ascii::is_ident('a') );
    ut::expect( ut::that % not ascii::is_float('a') );
    ut::expect( ut::that % not ascii::is_space_or_punct('a') );

    ut::expect( ut::that % not ascii::is_space('2') );
    ut::expect( ut::that % not ascii::is_blank('2') );
    ut::expect( ut::that % not ascii::is_endline('2') );
    ut::expect( ut::that % not ascii::is_alpha('2') );
    ut::expect( ut::that % ascii::is_alnum('2') );
    ut::expect( ut::that % ascii::is_digit('2') );
    ut::expect( ut::that % ascii::is_xdigi('2') );
    ut::expect( ut::that % not ascii::is_punct('2') );
    ut::expect( ut::that % not ascii::is_cntrl('2') );
    ut::expect( ut::that % ascii::is_graph('2') );
    ut::expect( ut::that % ascii::is_print('2') );
    ut::expect( ut::that % ascii::is_ident('2') );
    ut::expect( ut::that % ascii::is_float('2') );
    ut::expect( ut::that % not ascii::is_space_or_punct('2') );

    ut::expect( ut::that % ascii::is_space('\t') );
    ut::expect( ut::that % ascii::is_blank('\t') );
    ut::expect( ut::that % not ascii::is_endline('\t') );
    ut::expect( ut::that % not ascii::is_alpha('\t') );
    ut::expect( ut::that % not ascii::is_alnum('\t') );
    ut::expect( ut::that % not ascii::is_digit('\t') );
    ut::expect( ut::that % not ascii::is_xdigi('\t') );
    ut::expect( ut::that % not ascii::is_punct('\t') );
    ut::expect( ut::that % ascii::is_cntrl('\t') );
    ut::expect( ut::that % not ascii::is_graph('\t') );
    ut::expect( ut::that % not ascii::is_print('\t') );
    ut::expect( ut::that % not ascii::is_ident('\t') );
    ut::expect( ut::that % not ascii::is_float('\t') );
    ut::expect( ut::that % ascii::is_space_or_punct('\t') );

    ut::expect( ut::that % ascii::is_space('\n') );
    ut::expect( ut::that % not ascii::is_blank('\n') );
    ut::expect( ut::that % ascii::is_endline('\n') );
    ut::expect( ut::that % not ascii::is_alpha('\n') );
    ut::expect( ut::that % not ascii::is_alnum('\n') );
    ut::expect( ut::that % not ascii::is_digit('\n') );
    ut::expect( ut::that % not ascii::is_xdigi('\n') );
    ut::expect( ut::that % not ascii::is_punct('\n') );
    ut::expect( ut::that % ascii::is_cntrl('\n') );
    ut::expect( ut::that % not ascii::is_graph('\n') );
    ut::expect( ut::that % not ascii::is_print('\n') );
    ut::expect( ut::that % not ascii::is_ident('\n') );
    ut::expect( ut::that % not ascii::is_float('\n') );
    ut::expect( ut::that % ascii::is_space_or_punct('\n') );

    ut::expect( ut::that % not ascii::is_space(';') );
    ut::expect( ut::that % not ascii::is_blank(';') );
    ut::expect( ut::that % not ascii::is_endline(';') );
    ut::expect( ut::that % not ascii::is_alpha(';') );
    ut::expect( ut::that % not ascii::is_alnum(';') );
    ut::expect( ut::that % not ascii::is_digit(';') );
    ut::expect( ut::that % not ascii::is_xdigi(';') );
    ut::expect( ut::that % ascii::is_punct(';') );
    ut::expect( ut::that % not ascii::is_cntrl(';') );
    ut::expect( ut::that % ascii::is_graph(';') );
    ut::expect( ut::that % ascii::is_print(';') );
    ut::expect( ut::that % not ascii::is_ident(';') );
    ut::expect( ut::that % not ascii::is_float(';') );
    ut::expect( ut::that % ascii::is_space_or_punct(';') );

    ut::expect( ut::that % not ascii::is_space('\xE0') );
    ut::expect( ut::that % not ascii::is_blank('\xE0') );
    ut::expect( ut::that % not ascii::is_endline('\xE0') );
    ut::expect( ut::that % not ascii::is_alpha('\xE0') );
    ut::expect( ut::that % not ascii::is_alnum('\xE0') );
    ut::expect( ut::that % not ascii::is_digit('\xE0') );
    ut::expect( ut::that % not ascii::is_xdigi('\xE0') );
    ut::expect( ut::that % not ascii::is_punct('\xE0') );
    ut::expect( ut::that % not ascii::is_cntrl('\xE0') );
    ut::expect( ut::that % not ascii::is_graph('\xE0') );
    ut::expect( ut::that % not ascii::is_print('\xE0') );
    ut::expect( ut::that % not ascii::is_ident('\xE0') );
    ut::expect( ut::that % not ascii::is_float('\xE0') );
    ut::expect( ut::that % not ascii::is_space_or_punct('\xE0') );

    ut::expect( ut::that % not ascii::is_space(U'🍌') );
    ut::expect( ut::that % not ascii::is_blank(U'🍌') );
    ut::expect( ut::that % not ascii::is_endline(U'🍌') );
    ut::expect( ut::that % not ascii::is_alpha(U'🍌') );
    ut::expect( ut::that % not ascii::is_alnum(U'🍌') );
    ut::expect( ut::that % not ascii::is_digit(U'🍌') );
    ut::expect( ut::that % not ascii::is_xdigi(U'🍌') );
    ut::expect( ut::that % not ascii::is_punct(U'🍌') );
    ut::expect( ut::that % not ascii::is_cntrl(U'🍌') );
    ut::expect( ut::that % not ascii::is_graph(U'🍌') );
    ut::expect( ut::that % not ascii::is_print(U'🍌') );
    ut::expect( ut::that % not ascii::is_ident(U'🍌') );
    ut::expect( ut::that % not ascii::is_float(U'🍌') );
    ut::expect( ut::that % not ascii::is_space_or_punct(U'🍌') );
   };

ut::test("spaces predicates") = []
   {
    ut::expect( ut::that % ascii::is_space(' ')  and ascii::is_blank(' ') and not ascii::is_endline(' ') );
    ut::expect( ut::that % ascii::is_space('\t') and ascii::is_blank('\t') and not ascii::is_endline('\t') );
    ut::expect( ut::that % ascii::is_space('\n') and not ascii::is_blank('\n') and ascii::is_endline('\n') );
    ut::expect( ut::that % ascii::is_space('\r') and ascii::is_blank('\r') and not ascii::is_endline('\r') );
    ut::expect( ut::that % ascii::is_space('\v') and ascii::is_blank('\v') and not ascii::is_endline('\v') );
    ut::expect( ut::that % ascii::is_space('\f') and ascii::is_blank('\f') and not ascii::is_endline('\f') );
    ut::expect( ut::that % not ascii::is_space('\b') and not ascii::is_blank('\b') and not ascii::is_endline('\b') );
   };

ut::test("helper predicates") = []
   {
    ut::expect( ut::that % not ascii::is_always_false('a') );
    ut::expect( ut::that % ascii::is<'a'>('a') );
    ut::expect( ut::that % ascii::is_any_of<'a','\xE0',';'>('a') );
    ut::expect( ut::that % not ascii::is_none_of<'a','\xE0',';'>('a') );

    ut::expect( ut::that % not ascii::is_always_false('1') );
    ut::expect( ut::that % not ascii::is<'a'>('1') );
    ut::expect( ut::that % not ascii::is_any_of<'a','\xE0',';'>('1') );
    ut::expect( ut::that % ascii::is_none_of<'a','\xE0',';'>('1') );

    ut::expect( ut::that % not ascii::is_always_false(',') );
    ut::expect( ut::that % not ascii::is<'a'>(',') );
    ut::expect( ut::that % not ascii::is_any_of<'a','\xE0',';'>(',') );
    ut::expect( ut::that % ascii::is_none_of<'a','\xE0',';'>(',') );

    ut::expect( ut::that % not ascii::is_always_false(';') );
    ut::expect( ut::that % not ascii::is<'a'>(';') );
    ut::expect( ut::that % ascii::is_any_of<'a','\xE0',';'>(';') );
    ut::expect( ut::that % not ascii::is_none_of<'a','\xE0',';'>(';') );

    ut::expect( ut::that % not ascii::is_always_false(' ') );
    ut::expect( ut::that % not ascii::is<'a'>(' ') );
    ut::expect( ut::that % not ascii::is_any_of<'a','\xE0',';'>(' ') );
    ut::expect( ut::that % ascii::is_none_of<'a','\xE0',';'>(' ') );

    ut::expect( ut::that % not ascii::is_always_false('\xE0') );
    ut::expect( ut::that % not ascii::is<'a'>('\xE0') );
    ut::expect( ut::that % ascii::is_any_of<'a','\xE0',';'>('\xE0') );
    ut::expect( ut::that % not ascii::is_none_of<'a','\xE0',';'>('\xE0') );

    ut::expect( ut::that % not ascii::is_always_false('\xE1') );
    ut::expect( ut::that % not ascii::is<'a'>('\xE1') );
    ut::expect( ut::that % not ascii::is_any_of<'a','\xE0',';'>('\xE1') );
    ut::expect( ut::that % ascii::is_none_of<'a','\xE0',';'>('\xE1') );
   };

ut::test("composite predicates") = []
   {
    ut::expect( ut::that % ascii::is_space_or_any_of<'a','\xE0',';'>('a') );
    ut::expect( ut::that % ascii::is_alnum_or_any_of<'a','\xE0',';'>('a') );
    ut::expect( ut::that % not ascii::is_digit_or_any_of<'E',','>('a') );
    ut::expect( ut::that % not ascii::is_punct_and_none_of<','>('a') );

    ut::expect( ut::that % not ascii::is_space_or_any_of<'a','\xE0',';'>('1') );
    ut::expect( ut::that % ascii::is_alnum_or_any_of<'a','\xE0',';'>('1') );
    ut::expect( ut::that % ascii::is_digit_or_any_of<'E',','>('1') );
    ut::expect( ut::that % not ascii::is_punct_and_none_of<','>('1') );

    ut::expect( ut::that % not ascii::is_space_or_any_of<'a','\xE0',';'>(',') );
    ut::expect( ut::that % not ascii::is_alnum_or_any_of<'a','\xE0',';'>(',') );
    ut::expect( ut::that % ascii::is_digit_or_any_of<'E',','>(',') );
    ut::expect( ut::that % not ascii::is_punct_and_none_of<','>(',') );

    ut::expect( ut::that % ascii::is_space_or_any_of<'a','\xE0',';'>(';') );
    ut::expect( ut::that % ascii::is_alnum_or_any_of<'a','\xE0',';'>(';') );
    ut::expect( ut::that % not ascii::is_digit_or_any_of<'E',','>(';') );
    ut::expect( ut::that % ascii::is_punct_and_none_of<','>(';') );

    ut::expect( ut::that % ascii::is_space_or_any_of<'a','\xE0',';'>(' ') );
    ut::expect( ut::that % not ascii::is_alnum_or_any_of<'a','\xE0',';'>(' ') );
    ut::expect( ut::that % not ascii::is_digit_or_any_of<'E',','>(' ') );
    ut::expect( ut::that % not ascii::is_punct_and_none_of<','>(' ') );

    ut::expect( ut::that % ascii::is_space_or_any_of<'a','\xE0',';'>('\xE0') );
    ut::expect( ut::that % ascii::is_alnum_or_any_of<'a','\xE0',';'>('\xE0') );
    ut::expect( ut::that % not ascii::is_digit_or_any_of<'E',','>('\xE0') );
    ut::expect( ut::that % not ascii::is_punct_and_none_of<','>('\xE0') );

    ut::expect( ut::that % not ascii::is_space_or_any_of<'a','\xE0',';'>('\xE1') );
    ut::expect( ut::that % not ascii::is_alnum_or_any_of<'a','\xE0',';'>('\xE1') );
    ut::expect( ut::that % not ascii::is_digit_or_any_of<'E',','>('\xE1') );
    ut::expect( ut::that % not ascii::is_punct_and_none_of<','>('\xE1') );
   };

ut::test("implicit conversions") = []
   {
    ut::expect( ut::that % ascii::is_any_of<U'a',L'b',u8'c','d'>('b') );
   };

ut::test("non ascii char32_t") = []
   {
    // For non-ascii char32_t codepoints all predicates are false
    ut::expect( not ascii::is_space(U'▙') );
    ut::expect( not ascii::is_blank(U'▙') );
    ut::expect( not ascii::is_endline(U'▙') );
    ut::expect( not ascii::is_alpha(U'▙') );
    ut::expect( not ascii::is_alnum(U'▙') );
    ut::expect( not ascii::is_digit(U'▙') );
    ut::expect( not ascii::is_xdigi(U'▙') );
    ut::expect( not ascii::is_punct(U'▙') );
    ut::expect( not ascii::is_cntrl(U'▙') );
    ut::expect( not ascii::is_graph(U'▙') );
    ut::expect( not ascii::is_print(U'▙') );
    ut::expect( not ascii::is_ident(U'▙') );
    ut::expect( not ascii::is_float(U'▙') );
    ut::expect( not ascii::is_space_or_punct(U'▙') );

    ut::expect( not ascii::is_space(U'❸') );
    ut::expect( not ascii::is_blank(U'❸') );
    ut::expect( not ascii::is_endline(U'❸') );
    ut::expect( not ascii::is_alpha(U'❸') );
    ut::expect( not ascii::is_alnum(U'❸') );
    ut::expect( not ascii::is_digit(U'❸') );
    ut::expect( not ascii::is_xdigi(U'❸') );
    ut::expect( not ascii::is_punct(U'❸') );
    ut::expect( not ascii::is_cntrl(U'❸') );
    ut::expect( not ascii::is_graph(U'❸') );
    ut::expect( not ascii::is_print(U'❸') );
    ut::expect( not ascii::is_ident(U'❸') );
    ut::expect( not ascii::is_float(U'❸') );
    ut::expect( not ascii::is_space_or_punct(U'❸') );

    ut::expect( ascii::is<U'♦'>(U'♦') and not ascii::is<U'♥'>(U'♦') );
    ut::expect( ascii::is_any_of<U'♥',U'♦',U'♠',U'♣'>(U'♣') and not ascii::is_any_of<U'♥',U'♦',U'♠',U'♣'>(U'☺') );
    ut::expect( ascii::is_none_of<U'♥',U'♦',U'♠',U'♣'>(U'☺') and not ascii::is_none_of<U'♥',U'♦',U'♠',U'♣'>(U'♣') );
   };


ut::test("case conversion") = []
   {
    ut::expect( ut::that % ascii::to_lower('!') == '!');
    ut::expect( ut::that % ascii::to_lower('\"') == '\"');
    ut::expect( ut::that % ascii::to_lower('#') == '#');
    ut::expect( ut::that % ascii::to_lower('$') == '$');
    ut::expect( ut::that % ascii::to_lower('%') == '%');
    ut::expect( ut::that % ascii::to_lower('&') == '&');
    ut::expect( ut::that % ascii::to_lower('\'') == '\'');
    ut::expect( ut::that % ascii::to_lower('(') == '(');
    ut::expect( ut::that % ascii::to_lower(')') == ')');
    ut::expect( ut::that % ascii::to_lower('*') == '*');
    ut::expect( ut::that % ascii::to_lower('+') == '+');
    ut::expect( ut::that % ascii::to_lower(',') == ',');
    ut::expect( ut::that % ascii::to_lower('-') == '-');
    ut::expect( ut::that % ascii::to_lower('.') == '.');
    ut::expect( ut::that % ascii::to_lower('/') == '/');
    ut::expect( ut::that % ascii::to_lower('0') == '0');
    ut::expect( ut::that % ascii::to_lower('1') == '1');
    ut::expect( ut::that % ascii::to_lower('2') == '2');
    ut::expect( ut::that % ascii::to_lower('3') == '3');
    ut::expect( ut::that % ascii::to_lower('4') == '4');
    ut::expect( ut::that % ascii::to_lower('5') == '5');
    ut::expect( ut::that % ascii::to_lower('6') == '6');
    ut::expect( ut::that % ascii::to_lower('7') == '7');
    ut::expect( ut::that % ascii::to_lower('8') == '8');
    ut::expect( ut::that % ascii::to_lower('9') == '9');
    ut::expect( ut::that % ascii::to_lower(':') == ':');
    ut::expect( ut::that % ascii::to_lower(';') == ';');
    ut::expect( ut::that % ascii::to_lower('<') == '<');
    ut::expect( ut::that % ascii::to_lower('=') == '=');
    ut::expect( ut::that % ascii::to_lower('>') == '>');
    ut::expect( ut::that % ascii::to_lower('?') == '?');
    ut::expect( ut::that % ascii::to_lower('@') == '@');
    ut::expect( ut::that % ascii::to_lower('A') == 'a');
    ut::expect( ut::that % ascii::to_lower('B') == 'b');
    ut::expect( ut::that % ascii::to_lower('C') == 'c');
    ut::expect( ut::that % ascii::to_lower('D') == 'd');
    ut::expect( ut::that % ascii::to_lower('E') == 'e');
    ut::expect( ut::that % ascii::to_lower('F') == 'f');
    ut::expect( ut::that % ascii::to_lower('G') == 'g');
    ut::expect( ut::that % ascii::to_lower('H') == 'h');
    ut::expect( ut::that % ascii::to_lower('I') == 'i');
    ut::expect( ut::that % ascii::to_lower('J') == 'j');
    ut::expect( ut::that % ascii::to_lower('K') == 'k');
    ut::expect( ut::that % ascii::to_lower('L') == 'l');
    ut::expect( ut::that % ascii::to_lower('M') == 'm');
    ut::expect( ut::that % ascii::to_lower('N') == 'n');
    ut::expect( ut::that % ascii::to_lower('O') == 'o');
    ut::expect( ut::that % ascii::to_lower('P') == 'p');
    ut::expect( ut::that % ascii::to_lower('Q') == 'q');
    ut::expect( ut::that % ascii::to_lower('R') == 'r');
    ut::expect( ut::that % ascii::to_lower('S') == 's');
    ut::expect( ut::that % ascii::to_lower('T') == 't');
    ut::expect( ut::that % ascii::to_lower('U') == 'u');
    ut::expect( ut::that % ascii::to_lower('V') == 'v');
    ut::expect( ut::that % ascii::to_lower('W') == 'w');
    ut::expect( ut::that % ascii::to_lower('X') == 'x');
    ut::expect( ut::that % ascii::to_lower('Y') == 'y');
    ut::expect( ut::that % ascii::to_lower('Z') == 'z');
    ut::expect( ut::that % ascii::to_lower('[') == '[');
    ut::expect( ut::that % ascii::to_lower('\\') == '\\');
    ut::expect( ut::that % ascii::to_lower(']') == ']');
    ut::expect( ut::that % ascii::to_lower('^') == '^');
    ut::expect( ut::that % ascii::to_lower('_') == '_');
    ut::expect( ut::that % ascii::to_lower('`') == '`');
    ut::expect( ut::that % ascii::to_lower('a') == 'a');
    ut::expect( ut::that % ascii::to_lower('b') == 'b');
    ut::expect( ut::that % ascii::to_lower('c') == 'c');
    ut::expect( ut::that % ascii::to_lower('d') == 'd');
    ut::expect( ut::that % ascii::to_lower('e') == 'e');
    ut::expect( ut::that % ascii::to_lower('f') == 'f');
    ut::expect( ut::that % ascii::to_lower('g') == 'g');
    ut::expect( ut::that % ascii::to_lower('h') == 'h');
    ut::expect( ut::that % ascii::to_lower('i') == 'i');
    ut::expect( ut::that % ascii::to_lower('j') == 'j');
    ut::expect( ut::that % ascii::to_lower('k') == 'k');
    ut::expect( ut::that % ascii::to_lower('l') == 'l');
    ut::expect( ut::that % ascii::to_lower('m') == 'm');
    ut::expect( ut::that % ascii::to_lower('n') == 'n');
    ut::expect( ut::that % ascii::to_lower('o') == 'o');
    ut::expect( ut::that % ascii::to_lower('p') == 'p');
    ut::expect( ut::that % ascii::to_lower('q') == 'q');
    ut::expect( ut::that % ascii::to_lower('r') == 'r');
    ut::expect( ut::that % ascii::to_lower('s') == 's');
    ut::expect( ut::that % ascii::to_lower('t') == 't');
    ut::expect( ut::that % ascii::to_lower('u') == 'u');
    ut::expect( ut::that % ascii::to_lower('v') == 'v');
    ut::expect( ut::that % ascii::to_lower('w') == 'w');
    ut::expect( ut::that % ascii::to_lower('x') == 'x');
    ut::expect( ut::that % ascii::to_lower('y') == 'y');
    ut::expect( ut::that % ascii::to_lower('z') == 'z');
    ut::expect( ut::that % ascii::to_lower('{') == '{');
    ut::expect( ut::that % ascii::to_lower('|') == '|');
    ut::expect( ut::that % ascii::to_lower('}') == '}');
    ut::expect( ut::that % ascii::to_lower('~') == '~');

    ut::expect( ut::that % ascii::to_upper('!') == '!');
    ut::expect( ut::that % ascii::to_upper('\"') == '\"');
    ut::expect( ut::that % ascii::to_upper('#') == '#');
    ut::expect( ut::that % ascii::to_upper('$') == '$');
    ut::expect( ut::that % ascii::to_upper('%') == '%');
    ut::expect( ut::that % ascii::to_upper('&') == '&');
    ut::expect( ut::that % ascii::to_upper('\'') == '\'');
    ut::expect( ut::that % ascii::to_upper('(') == '(');
    ut::expect( ut::that % ascii::to_upper(')') == ')');
    ut::expect( ut::that % ascii::to_upper('*') == '*');
    ut::expect( ut::that % ascii::to_upper('+') == '+');
    ut::expect( ut::that % ascii::to_upper(',') == ',');
    ut::expect( ut::that % ascii::to_upper('-') == '-');
    ut::expect( ut::that % ascii::to_upper('.') == '.');
    ut::expect( ut::that % ascii::to_upper('/') == '/');
    ut::expect( ut::that % ascii::to_upper('0') == '0');
    ut::expect( ut::that % ascii::to_upper('1') == '1');
    ut::expect( ut::that % ascii::to_upper('2') == '2');
    ut::expect( ut::that % ascii::to_upper('3') == '3');
    ut::expect( ut::that % ascii::to_upper('4') == '4');
    ut::expect( ut::that % ascii::to_upper('5') == '5');
    ut::expect( ut::that % ascii::to_upper('6') == '6');
    ut::expect( ut::that % ascii::to_upper('7') == '7');
    ut::expect( ut::that % ascii::to_upper('8') == '8');
    ut::expect( ut::that % ascii::to_upper('9') == '9');
    ut::expect( ut::that % ascii::to_upper(':') == ':');
    ut::expect( ut::that % ascii::to_upper(';') == ';');
    ut::expect( ut::that % ascii::to_upper('<') == '<');
    ut::expect( ut::that % ascii::to_upper('=') == '=');
    ut::expect( ut::that % ascii::to_upper('>') == '>');
    ut::expect( ut::that % ascii::to_upper('?') == '?');
    ut::expect( ut::that % ascii::to_upper('@') == '@');
    ut::expect( ut::that % ascii::to_upper('A') == 'A');
    ut::expect( ut::that % ascii::to_upper('B') == 'B');
    ut::expect( ut::that % ascii::to_upper('C') == 'C');
    ut::expect( ut::that % ascii::to_upper('D') == 'D');
    ut::expect( ut::that % ascii::to_upper('E') == 'E');
    ut::expect( ut::that % ascii::to_upper('F') == 'F');
    ut::expect( ut::that % ascii::to_upper('G') == 'G');
    ut::expect( ut::that % ascii::to_upper('H') == 'H');
    ut::expect( ut::that % ascii::to_upper('I') == 'I');
    ut::expect( ut::that % ascii::to_upper('J') == 'J');
    ut::expect( ut::that % ascii::to_upper('K') == 'K');
    ut::expect( ut::that % ascii::to_upper('L') == 'L');
    ut::expect( ut::that % ascii::to_upper('M') == 'M');
    ut::expect( ut::that % ascii::to_upper('N') == 'N');
    ut::expect( ut::that % ascii::to_upper('O') == 'O');
    ut::expect( ut::that % ascii::to_upper('P') == 'P');
    ut::expect( ut::that % ascii::to_upper('Q') == 'Q');
    ut::expect( ut::that % ascii::to_upper('R') == 'R');
    ut::expect( ut::that % ascii::to_upper('S') == 'S');
    ut::expect( ut::that % ascii::to_upper('T') == 'T');
    ut::expect( ut::that % ascii::to_upper('U') == 'U');
    ut::expect( ut::that % ascii::to_upper('V') == 'V');
    ut::expect( ut::that % ascii::to_upper('W') == 'W');
    ut::expect( ut::that % ascii::to_upper('X') == 'X');
    ut::expect( ut::that % ascii::to_upper('Y') == 'Y');
    ut::expect( ut::that % ascii::to_upper('Z') == 'Z');
    ut::expect( ut::that % ascii::to_upper('[') == '[');
    ut::expect( ut::that % ascii::to_upper('\\') == '\\');
    ut::expect( ut::that % ascii::to_upper(']') == ']');
    ut::expect( ut::that % ascii::to_upper('^') == '^');
    ut::expect( ut::that % ascii::to_upper('_') == '_');
    ut::expect( ut::that % ascii::to_upper('`') == '`');
    ut::expect( ut::that % ascii::to_upper('a') == 'A');
    ut::expect( ut::that % ascii::to_upper('b') == 'B');
    ut::expect( ut::that % ascii::to_upper('c') == 'C');
    ut::expect( ut::that % ascii::to_upper('d') == 'D');
    ut::expect( ut::that % ascii::to_upper('e') == 'E');
    ut::expect( ut::that % ascii::to_upper('f') == 'F');
    ut::expect( ut::that % ascii::to_upper('g') == 'G');
    ut::expect( ut::that % ascii::to_upper('h') == 'H');
    ut::expect( ut::that % ascii::to_upper('i') == 'I');
    ut::expect( ut::that % ascii::to_upper('j') == 'J');
    ut::expect( ut::that % ascii::to_upper('k') == 'K');
    ut::expect( ut::that % ascii::to_upper('l') == 'L');
    ut::expect( ut::that % ascii::to_upper('m') == 'M');
    ut::expect( ut::that % ascii::to_upper('n') == 'N');
    ut::expect( ut::that % ascii::to_upper('o') == 'O');
    ut::expect( ut::that % ascii::to_upper('p') == 'P');
    ut::expect( ut::that % ascii::to_upper('q') == 'Q');
    ut::expect( ut::that % ascii::to_upper('r') == 'R');
    ut::expect( ut::that % ascii::to_upper('s') == 'S');
    ut::expect( ut::that % ascii::to_upper('t') == 'T');
    ut::expect( ut::that % ascii::to_upper('u') == 'U');
    ut::expect( ut::that % ascii::to_upper('v') == 'V');
    ut::expect( ut::that % ascii::to_upper('w') == 'W');
    ut::expect( ut::that % ascii::to_upper('x') == 'X');
    ut::expect( ut::that % ascii::to_upper('y') == 'Y');
    ut::expect( ut::that % ascii::to_upper('z') == 'Z');
    ut::expect( ut::that % ascii::to_upper('{') == '{');
    ut::expect( ut::that % ascii::to_upper('|') == '|');
    ut::expect( ut::that % ascii::to_upper('}') == '}');
    ut::expect( ut::that % ascii::to_upper('~') == '~');
   };


};///////////////////////////////////////////////////////////////////////////
#endif // TEST_UNITS ////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////


// Lookup table generation:
// https://gcc.godbolt.org/z/6ds8faf3Y
//#include <iostream>
//#include <iomanip>
//#include <array>
//#include <format>
//#include <cctype>
//#include <cstdint>
//
//static_assert( sizeof(unsigned char)==1 );
//using mask_t = std::uint16_t;
//
//struct LookupTableGenerator
//{
//    std::array<mask_t,256> lookup_table;
//
//    LookupTableGenerator()
//       {
//
//        for(std::size_t i=0; i<lookup_table.size(); ++i)
//           {
//            lookup_table[i] = generate_mask_for(static_cast<unsigned char>(i));
//           }
//       }
//
//    static mask_t generate_mask_for(unsigned char c)
//       {
//        enum : mask_t
//           {
//            ISLOWER = 0b0000'0000'0000'0001 // std::islower
//           ,ISUPPER = 0b0000'0000'0000'0010 // std::isupper
//           ,ISSPACE = 0b0000'0000'0000'0100 // std::isspace
//           ,ISBLANK = 0b0000'0000'0000'1000 // std::isspace and not \n (not equiv to std::isblank)
//           ,ISALPHA = 0b0000'0000'0001'0000 // std::isalpha
//           ,ISALNUM = 0b0000'0000'0010'0000 // std::isalnum
//           ,ISDIGIT = 0b0000'0000'0100'0000 // std::isdigit
//           ,ISXDIGI = 0b0000'0000'1000'0000 // std::isxdigit
//           ,ISPUNCT = 0b0000'0001'0000'0000 // std::ispunct
//           ,ISCNTRL = 0b0000'0010'0000'0000 // std::iscntrl
//           ,ISGRAPH = 0b0000'0100'0000'0000 // std::isgraph
//           ,ISPRINT = 0b0000'1000'0000'0000 // std::isprint
//           // Extended
//           ,ISIDENT = 0b0001'0000'0000'0000 // std::isalnum or _
//           ,ISFLOAT = 0b0010'0000'0000'0000 // std::isdigit or +,-,.,E,e
//           //,ISXXXXX = 0b0100'0000'0000'0000
//           //,ISXXXXX = 0b1000'0000'0000'0000
//           };
//
//        mask_t mask = 0;
//        if( std::islower(c) )  mask |= ISLOWER;
//        if( std::isupper(c) )  mask |= ISUPPER;
//        if( std::isspace(c) )  mask |= ISSPACE;
//        if( std::isspace(c) and c!='\n' ) mask |= ISBLANK; // std::isblank(c)
//        if( std::isalpha(c) )  mask |= ISALPHA;
//        if( std::isalnum(c) )  mask |= ISALNUM;
//        if( std::isdigit(c) )  mask |= ISDIGIT;
//        if( std::isxdigit(c) ) mask |= ISXDIGI;
//        if( std::ispunct(c) )  mask |= ISPUNCT;
//        if( std::iscntrl(c) )  mask |= ISCNTRL;
//        if( std::isgraph(c) )  mask |= ISGRAPH;
//        if( std::isprint(c) )  mask |= ISPRINT;
//        if( std::isalnum(c) or c=='_' ) mask |= ISIDENT;
//        if( std::isdigit(c) or c=='+' or c=='-' or c=='.' or c=='E' or c=='e' ) mask |= ISFLOAT;
//        return mask;
//       }
//
//    void print_table()
//       {
//        for(std::size_t i=0; i<lookup_table.size(); ++i)
//           {
//            std::cout << std::hex << "\\x" << i << " (" << char(i) << ") 0x" << lookup_table[i] << '\n';
//           }
//       }
//
//    void print_array()
//       {
//        std::cout << "static constexpr mask_t ascii_lookup_table[256] = \n";
//        std::cout << std::left << '{';
//        for(std::size_t i=0; i<lookup_table.size(); ++i)
//           {
//            std::cout << std::setw(6) << std::format("0x{:X}",lookup_table[i]) << ", ";
//            if((i+1)%16 == 0)
//               {
//                if(i!=255) std::cout << '\n';
//                else std::cout << "};\n";
//               }
//           }
//       }
//};
//
//int main()
//{
//    LookupTableGenerator tbl;
//    //tbl.print_table();
//    tbl.print_array();
//}
