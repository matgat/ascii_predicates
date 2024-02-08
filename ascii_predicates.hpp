#pragma once
//  ---------------------------------------------
//  ASCII encoding predicates and facilities
//  ---------------------------------------------
//  #include "ascii_predicates.hpp" // ascii::is_*
//  ---------------------------------------------
#include <concepts> // std::same_as<>
#include <cstdint> // std::uint16_t
#include <limits> // std::numeric_limits


//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
namespace ascii //:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
{

// Supporting the following character types:
template<typename T> concept CharLike = std::same_as<T, char> or
                                        std::same_as<T, unsigned char> or
                                        std::same_as<T, char32_t>;

//---------------------------------------------------------------------------
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
    // | 32      | space (' ')               |     |  X  |     |  X  |  X  |     |     |     |     |     |     |      |
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
        VALMASK = 0b0000'0000'0000'1111 // Associated value
        // Standard predicates
       ,ISLOWER = 0b0000'0000'0001'0000 // std::islower
       ,ISUPPER = 0b0000'0000'0010'0000 // std::isupper
       ,ISSPACE = 0b0000'0000'0100'0000 // std::isspace
       ,ISBLANK = 0b0000'0000'1000'0000 // std::isspace and not '\n' (not equiv to std::isblank)
       ,ISALPHA = 0b0000'0001'0000'0000 // std::isalpha
       ,ISDIGIT = 0b0000'0010'0000'0000 // std::isdigit
       ,ISXDIGI = 0b0000'0100'0000'0000 // std::isxdigit
       ,ISPUNCT = 0b0000'1000'0000'0000 // std::ispunct
       ,ISCNTRL = 0b0001'0000'0000'0000 // std::iscntrl
       ,ISPRINT = 0b0010'0000'0000'0000 // std::isprint
       // Extended predicates
       ,ISIDENT = 0b0100'0000'0000'0000 // std::isalnum or '_'
       ,ISFLOAT = 0b1000'0000'0000'0000 // std::isdigit or any of "+-.Ee"
       };

    static_assert( sizeof(unsigned char)==1 );
    static_assert( std::numeric_limits<unsigned char>::digits==8u );
    inline static constexpr mask_t masks_table[1u+std::numeric_limits<unsigned char>::max()] =
       {0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x10C0, 0x1040, 0x10C0, 0x10C0, 0x10C0, 0x1000, 0x1000,
        0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000,
        0x20C0, 0x2800, 0x2800, 0x2800, 0x2800, 0x2800, 0x2800, 0x2800, 0x2800, 0x2800, 0x2800, 0xA800, 0x2800, 0xA800, 0xA800, 0x2800,
        0xE600, 0xE601, 0xE602, 0xE603, 0xE604, 0xE605, 0xE606, 0xE607, 0xE608, 0xE609, 0x2800, 0x2800, 0x2800, 0x2800, 0x2800, 0x2800,
        0x2800, 0x652A, 0x652B, 0x652C, 0x652D, 0xE52E, 0x652F, 0x6120, 0x6120, 0x6120, 0x6120, 0x6120, 0x6120, 0x6120, 0x6120, 0x6120,
        0x6120, 0x6120, 0x6120, 0x6120, 0x6120, 0x6120, 0x6120, 0x6120, 0x6120, 0x6120, 0x6120, 0x2800, 0x2800, 0x2800, 0x2800, 0x6800,
        0x2800, 0x651A, 0x651B, 0x651C, 0x651D, 0xE51E, 0x651F, 0x6110, 0x6110, 0x6110, 0x6110, 0x6110, 0x6110, 0x6110, 0x6110, 0x6110,
        0x6110, 0x6110, 0x6110, 0x6110, 0x6110, 0x6110, 0x6110, 0x6110, 0x6110, 0x6110, 0x6110, 0x2800, 0x2800, 0x2800, 0x2800, 0x1000,
           0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,
           0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,
           0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,
           0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,
           0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,
           0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,
           0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,
           0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0 };

    //-----------------------------------------------------------------------
    template<CharLike Char>
    [[nodiscard]] constexpr mask_t apply_mask(const Char c, const mask_t mask) noexcept
       {
        if constexpr( std::same_as<Char, unsigned char> )
           {
            return details::masks_table[c] & mask;
           }
        else if constexpr( sizeof(Char)==sizeof(unsigned char) )
           {
            return details::masks_table[static_cast<unsigned char>(c)] & mask;
           }
        else
           {
            if( ascii::is_ascii(c) ) [[likely]]
               {
                return details::masks_table[static_cast<unsigned char>(c)] & mask;
               }
            return 0;
           }
       }

    //-----------------------------------------------------------------------
    template<CharLike Char>
    [[nodiscard]] inline constexpr bool check_any(const Char c, const mask_t mask) noexcept
       {
        return apply_mask(c, mask)!=0u;
       }

    //-----------------------------------------------------------------------
    template<CharLike Char>
    [[nodiscard]] inline constexpr bool check_all(const Char c, const mask_t mask) noexcept
       {
        return apply_mask(c, mask) == mask;
       }


    //-----------------------------------------------------------------------
    // Facility for case conversion
    template<bool SET>
    [[nodiscard]] inline constexpr char set_case_bit(char ch) noexcept
       {
        // static constexpr char case_bit = '\x20'; // c++23 should allow static constexpr here
        if constexpr( SET ) ch |=  '\x20';
        else                ch &= ~'\x20'; // '\xDF'
        return ch;
       }

} //::::::::::::::::::::::::::::::: details ::::::::::::::::::::::::::::::::::



// Standard predicates
template<CharLike Char> [[nodiscard]] constexpr bool is_lower(const Char c) noexcept { return details::check_any(c, details::ISLOWER); }
template<CharLike Char> [[nodiscard]] constexpr bool is_upper(const Char c) noexcept { return details::check_any(c, details::ISUPPER); }
template<CharLike Char> [[nodiscard]] constexpr bool is_space(const Char c) noexcept { return details::check_any(c, details::ISSPACE); }
template<CharLike Char> [[nodiscard]] constexpr bool is_alpha(const Char c) noexcept { return details::check_any(c, details::ISALPHA); }
template<CharLike Char> [[nodiscard]] constexpr bool is_alnum(const Char c) noexcept { return details::check_any(c, details::ISALPHA | details::ISDIGIT); }
template<CharLike Char> [[nodiscard]] constexpr bool is_digit(const Char c) noexcept { return details::check_any(c, details::ISDIGIT); }
template<CharLike Char> [[nodiscard]] constexpr bool is_xdigi(const Char c) noexcept { return details::check_any(c, details::ISXDIGI); }
template<CharLike Char> [[nodiscard]] constexpr bool is_punct(const Char c) noexcept { return details::check_any(c, details::ISPUNCT); }
template<CharLike Char> [[nodiscard]] constexpr bool is_cntrl(const Char c) noexcept { return details::check_any(c, details::ISCNTRL); }
template<CharLike Char> [[nodiscard]] constexpr bool is_graph(const Char c) noexcept { return details::apply_mask(c, details::ISPRINT | details::ISBLANK) == details::ISPRINT; }
template<CharLike Char> [[nodiscard]] constexpr bool is_print(const Char c) noexcept { return details::check_any(c, details::ISPRINT); }
// Non standard ones
template<CharLike Char> [[nodiscard]] constexpr bool is_blank(const Char c) noexcept { return details::check_any(c, details::ISBLANK); } // not equiv to std::isblank
template<CharLike Char> [[nodiscard]] constexpr bool is_ident(const Char c) noexcept { return details::check_any(c, details::ISIDENT); } // std::isalnum or _
template<CharLike Char> [[nodiscard]] constexpr bool is_float(const Char c) noexcept { return details::check_any(c, details::ISFLOAT); } // std::isdigit or any of "+-.Ee"
template<CharLike Char> [[nodiscard]] constexpr bool is_space_or_punct(const Char c) noexcept { return details::check_any(c, details::ISSPACE | details::ISPUNCT); }
template<CharLike Char> [[nodiscard]] constexpr bool is_endline(const Char c) noexcept { return c==static_cast<Char>('\n'); }


//---------------------------------------------------------------------------
// Helper predicates (not strictly ASCII related)
template<CharLike Char> [[nodiscard]] constexpr bool is_always_false(const Char) noexcept
   {
    return false;
   }

template<CharLike auto C>
[[nodiscard]] constexpr bool is(const decltype(C) c) noexcept
   {
    return c==C;
   }

template<CharLike auto C1, decltype(C1)... CS>
[[nodiscard]] constexpr bool is_any_of(const decltype(C1) c) noexcept
   {
    return c==C1 or ((c==CS) or ...);
   }

template<CharLike auto C1, decltype(C1)... CS>
[[nodiscard]] constexpr bool is_none_of(const decltype(C1) c) noexcept
   {
    return c!=C1 and ((c!=CS) and ...);
   }

//template<CharLike auto C>
//[[nodiscard]] constexpr bool is_greater_than(const decltype(C) c) noexcept
//   {
//    return c>C;
//   }

//template<CharLike auto C>
//[[nodiscard]] constexpr bool is_less_than(const decltype(C) c) noexcept
//   {
//    return c<C;
//   }

//template<CharLike auto C1, CharLike auto C2>
//[[nodiscard]] constexpr bool is_between(const decltype(C) c) noexcept
//   {
//    return c>=C1 and c<=C2;
//   }



//---------------------------------------------------------------------------
// Some examples of non-type parameterized template composite predicates

template<CharLike auto C1, decltype(C1)... CS>
[[nodiscard]] constexpr bool is_space_or_any_of(const decltype(C1) c) noexcept
   {
    return is_space(c) or is_any_of<C1, CS ...>(c);
   }

template<CharLike auto C1, decltype(C1)... CS>
[[nodiscard]] constexpr bool is_alnum_or_any_of(const decltype(C1) c) noexcept
   {
    return is_alnum(c) or is_any_of<C1, CS ...>(c);
   }

template<CharLike auto C1, decltype(C1)... CS>
[[nodiscard]] constexpr bool is_digit_or_any_of(const decltype(C1) c) noexcept
   {
    return is_digit(c) or is_any_of<C1, CS ...>(c);
   }

template<CharLike auto C1, decltype(C1)... CS>
[[nodiscard]] constexpr bool is_punct_and_none_of(const decltype(C1) c) noexcept
   {
    return is_punct(c) and is_none_of<C1, CS ...>(c);
   }



//---------------------------------------------------------------------------
// Case conversion (only for ASCII char, so not so useful!)
[[nodiscard]] constexpr char to_lower(char ch) noexcept { if(is_upper(ch)) ch = details::set_case_bit<true>(ch); return ch; }
[[nodiscard]] constexpr char to_upper(char ch) noexcept { if(is_lower(ch)) ch = details::set_case_bit<false>(ch); return ch; }



//---------------------------------------------------------------------------
template<CharLike Char>
[[nodiscard]] constexpr std::uint8_t value_of_digit(const Char c) noexcept
{
    // Using `std::uint8_t` because is the easiest to promote to other integrals
    return static_cast<std::uint8_t>( apply_mask(c, details::VALMASK) );
}


}//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::



/////////////////////////////////////////////////////////////////////////////
#ifdef TEST_UNITS ///////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
#include <cctype>
/////////////////////////////////////////////////////////////////////////////
static ut::suite<"MG::ascii_predicates"> ascii_predicates_tests = []
{////////////////////////////////////////////////////////////////////////////

static_assert( ascii::is_alnum('a') );
static_assert( not ascii::is_digit(U'⛵') );
static_assert( ascii::value_of_digit('3') == 3 );
static_assert( ascii::to_lower('A') == 'a' );

// Compatibility with standard predicates, but just in ASCII range
for( unsigned char ch=0; ch<0x80u; ++ch )
   {
    ut::test( std::to_string(static_cast<int>(ch)) ) = [ch]
       {
        //ut::log << fmt::format("standard predicates on '\\x{:X}'\n",ch);

        //ut::expect( ut::that % ascii::is_blank(ch) == (std::isblank(ch)!=0) ); // mine's not standard
        ut::expect( ut::that % ascii::is_alnum(ch) == (std::isalnum(ch)!=0) ) << "(char '" << ch << "')\n";
        ut::expect( ut::that % ascii::is_alpha(ch) == (std::isalpha(ch)!=0) ) << "(char '" << ch << "')\n";
        ut::expect( ut::that % ascii::is_cntrl(ch) == (std::iscntrl(ch)!=0) ) << "(char '" << ch << "')\n";
        ut::expect( ut::that % ascii::is_digit(ch) == (std::isdigit(ch)!=0) ) << "(char '" << ch << "')\n";
        ut::expect( ut::that % ascii::is_graph(ch) == (std::isgraph(ch)!=0) ) << "(char '" << ch << "')\n";
        ut::expect( ut::that % ascii::is_print(ch) == (std::isprint(ch)!=0) ) << "(char '" << ch << "')\n";
        ut::expect( ut::that % ascii::is_punct(ch) == (std::ispunct(ch)!=0) ) << "(char '" << ch << "')\n";
        ut::expect( ut::that % ascii::is_space(ch) == (std::isspace(ch)!=0) ) << "(char '" << ch << "')\n";
        ut::expect( ut::that % ascii::is_xdigi(ch) == (std::isxdigit(ch)!=0) ) << "(char '" << ch << "')\n";
        ut::expect( ut::that % ascii::is_lower(ch) == (std::islower(ch)!=0) ) << "(char '" << ch << "')\n";
        ut::expect( ut::that % ascii::is_upper(ch) == (std::isupper(ch)!=0) ) << "(char '" << ch << "')\n";

        ut::expect( ut::that % static_cast<int>(ascii::to_lower(static_cast<char>(ch))) == std::tolower(ch) ) << "(char '" << ch << "')\n";
        ut::expect( ut::that % static_cast<int>(ascii::to_upper(static_cast<char>(ch))) == std::toupper(ch) ) << "(char '" << ch << "')\n";
       };
   }

ut::test("basic predicates") = []
   {
    ut::expect( ut::that % not ascii::is_lower('\1') );
    ut::expect( ut::that % not ascii::is_upper('\1') );
    ut::expect( ut::that % not ascii::is_space('\1') );
    ut::expect( ut::that % not ascii::is_blank('\1') );
    ut::expect( ut::that % not ascii::is_endline('\1') );
    ut::expect( ut::that % not ascii::is_alpha('\1') );
    ut::expect( ut::that % not ascii::is_alnum('\1') );
    ut::expect( ut::that % not ascii::is_digit('\1') );
    ut::expect( ut::that % not ascii::is_xdigi('\1') );
    ut::expect( ut::that % not ascii::is_punct('\1') );
    ut::expect( ut::that % ascii::is_cntrl('\1') );
    ut::expect( ut::that % not ascii::is_graph('\1') );
    ut::expect( ut::that % not ascii::is_print('\1') );
    ut::expect( ut::that % not ascii::is_ident('\1') );
    ut::expect( ut::that % not ascii::is_float('\1') );
    ut::expect( ut::that % not ascii::is_space_or_punct('\1') );

    ut::expect( ut::that % ascii::is_lower('a') );
    ut::expect( ut::that % not ascii::is_upper('a') );
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

    ut::expect( ut::that % not ascii::is_lower('2') );
    ut::expect( ut::that % not ascii::is_upper('2') );
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

    ut::expect( ut::that % not ascii::is_lower('\t') );
    ut::expect( ut::that % not ascii::is_upper('\t') );
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

    ut::expect( ut::that % not ascii::is_lower('\n') );
    ut::expect( ut::that % not ascii::is_upper('\n') );
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

    ut::expect( ut::that % not ascii::is_lower(';') );
    ut::expect( ut::that % not ascii::is_upper(';') );
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

    ut::expect( ut::that % not ascii::is_lower('\xE0') );
    ut::expect( ut::that % not ascii::is_upper('\xE0') );
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

    ut::expect( ut::that % not ascii::is_lower(U'🍌') );
    ut::expect( ut::that % not ascii::is_upper(U'🍌') );
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

ut::test("value_of_digit()") = []
   {
    ut::expect( ut::that % ascii::value_of_digit('0') == 0u);
    ut::expect( ut::that % ascii::value_of_digit('1') == 1u);
    ut::expect( ut::that % ascii::value_of_digit('2') == 2u);
    ut::expect( ut::that % ascii::value_of_digit('3') == 3u);
    ut::expect( ut::that % ascii::value_of_digit('4') == 4u);
    ut::expect( ut::that % ascii::value_of_digit('5') == 5u);
    ut::expect( ut::that % ascii::value_of_digit('6') == 6u);
    ut::expect( ut::that % ascii::value_of_digit('7') == 7u);
    ut::expect( ut::that % ascii::value_of_digit('8') == 8u);
    ut::expect( ut::that % ascii::value_of_digit('9') == 9u);
    ut::expect( ut::that % ascii::value_of_digit('A') == 0xAu);
    ut::expect( ut::that % ascii::value_of_digit('B') == 0xBu);
    ut::expect( ut::that % ascii::value_of_digit('C') == 0xCu);
    ut::expect( ut::that % ascii::value_of_digit('D') == 0xDu);
    ut::expect( ut::that % ascii::value_of_digit('E') == 0xEu);
    ut::expect( ut::that % ascii::value_of_digit('F') == 0xFu);
    ut::expect( ut::that % ascii::value_of_digit('z') == 0u);

    ut::expect( ut::that % ascii::value_of_digit(U'0') == 0u);
    ut::expect( ut::that % ascii::value_of_digit(U'1') == 1u);
    ut::expect( ut::that % ascii::value_of_digit(U'2') == 2u);
    ut::expect( ut::that % ascii::value_of_digit(U'3') == 3u);
    ut::expect( ut::that % ascii::value_of_digit(U'4') == 4u);
    ut::expect( ut::that % ascii::value_of_digit(U'5') == 5u);
    ut::expect( ut::that % ascii::value_of_digit(U'6') == 6u);
    ut::expect( ut::that % ascii::value_of_digit(U'7') == 7u);
    ut::expect( ut::that % ascii::value_of_digit(U'8') == 8u);
    ut::expect( ut::that % ascii::value_of_digit(U'9') == 9u);
    ut::expect( ut::that % ascii::value_of_digit(U'A') == 0xAu);
    ut::expect( ut::that % ascii::value_of_digit(U'B') == 0xBu);
    ut::expect( ut::that % ascii::value_of_digit(U'C') == 0xCu);
    ut::expect( ut::that % ascii::value_of_digit(U'D') == 0xDu);
    ut::expect( ut::that % ascii::value_of_digit(U'E') == 0xEu);
    ut::expect( ut::that % ascii::value_of_digit(U'F') == 0xFu);
    ut::expect( ut::that % ascii::value_of_digit(U'z') == 0u);
    ut::expect( ut::that % ascii::value_of_digit(U'🍄') == 0u);
   };

};///////////////////////////////////////////////////////////////////////////
#endif // TEST_UNITS ////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////


// Lookup tables generation:
// https://gcc.godbolt.org/z/Gna45Esjc
//#include <iostream>
//#include <iomanip>
//#include <array>
//#include <format>
//#include <cctype>
//#include <cstdint>
//
//static_assert( sizeof(unsigned char)==1 );
//
//template<typename T>
//struct AsciiTableGenerator
//{
//    using elem_t = T;
//    std::array<elem_t,256> table;
//
//    const AsciiTableGenerator<T>& generate()
//       {
//        for(std::size_t i=0; i<table.size(); ++i)
//           {
//            table[i] = generate_element_for(static_cast<unsigned char>(i));
//           }
//        return *this;
//       }
//
//    virtual elem_t generate_element_for(unsigned char c) =0;
//
//    void list_elements() const
//       {
//        for(std::size_t i=0; i<table.size(); ++i)
//           {
//            std::cout << std::hex << "\\x" << i << " (" << char(i) << ") 0x" << table[i] << '\n';
//           }
//       }
//
//    void print_array() const
//       {
//        std::cout << "static constexpr elem_t table[256] = \n";
//        std::cout << std::left << '{';
//        for(std::size_t i=0; i<table.size(); ++i)
//           {
//            std::cout << std::setfill(' ') << std::setw(6) << std::right << std::format("0x{:X}",table[i]) << ", ";
//            if((i+1)%16 == 0)
//               {
//                if(i!=255) std::cout << '\n';
//                else std::cout << "};\n";
//               }
//           }
//       }
//};
//
//struct PredMasksAndValuesGenerator : AsciiTableGenerator<std::uint16_t>
//{
//    AsciiTableGenerator::elem_t generate_element_for(unsigned char c) override
//       {
//        enum : AsciiTableGenerator::elem_t
//           {
//            VALMASK = 0b0000'0000'0000'1111 // Associated value
//            // Standard predicates
//           ,ISLOWER = 0b0000'0000'0001'0000 // std::islower
//           ,ISUPPER = 0b0000'0000'0010'0000 // std::isupper
//           ,ISSPACE = 0b0000'0000'0100'0000 // std::isspace
//           ,ISBLANK = 0b0000'0000'1000'0000 // std::isspace and not '\n' (not equiv to std::isblank)
//           ,ISALPHA = 0b0000'0001'0000'0000 // std::isalpha
//           ,ISDIGIT = 0b0000'0010'0000'0000 // std::isdigit
//           ,ISXDIGI = 0b0000'0100'0000'0000 // std::isxdigit
//           ,ISPUNCT = 0b0000'1000'0000'0000 // std::ispunct
//           ,ISCNTRL = 0b0001'0000'0000'0000 // std::iscntrl
//           ,ISPRINT = 0b0010'0000'0000'0000 // std::isprint
//           // Extended predicates
//           ,ISIDENT = 0b0100'0000'0000'0000 // std::isalnum or '_'
//           ,ISFLOAT = 0b1000'0000'0000'0000 // std::isdigit or any of "+-.Ee"
//           };
//
//        AsciiTableGenerator::elem_t mask = 0;
//        // Associated value
//        mask = dig_value_of(c) & VALMASK;
//        // Predicates
//        if( std::islower(c) )  mask |= ISLOWER;
//        if( std::isupper(c) )  mask |= ISUPPER;
//        if( std::isspace(c) )  mask |= ISSPACE;
//        if( std::isspace(c) and c!='\n' ) mask |= ISBLANK; // std::isblank(c)
//        if( std::isalpha(c) )  mask |= ISALPHA;
//        if( std::isdigit(c) )  mask |= ISDIGIT;
//        if( std::isxdigit(c) ) mask |= ISXDIGI;
//        if( std::ispunct(c) )  mask |= ISPUNCT;
//        if( std::iscntrl(c) )  mask |= ISCNTRL;
//        if( std::isprint(c) )  mask |= ISPRINT;
//        // Extended predicates
//        if( std::isalnum(c) or c=='_' ) mask |= ISIDENT;
//        if( std::isdigit(c) or c=='+' or c=='-' or c=='.' or c=='E' or c=='e' ) mask |= ISFLOAT;
//        return mask;
//       }
//
//    AsciiTableGenerator::elem_t dig_value_of(unsigned char c)
//       {
//        if( std::isdigit(c) )  return c-'0';
//        else if( std::isxdigit(c) ) return 0xA + c - (std::islower(c) ? 'a' : 'A');
//        return 0;
//       }
//};
//
//int main()
//{
//    PredMasksAndValuesGenerator().generate().print_array();
//}
