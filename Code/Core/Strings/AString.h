// AString.h
//------------------------------------------------------------------------------
#pragma once

// Includes
//------------------------------------------------------------------------------
#include "Core/Containers/Array.h"
#include "Core/Containers/Move.h"
#include "Core/Env/Assert.h"
#include "Core/Env/MSVCStaticAnalysis.h"
#include "Core/Env/Types.h"

// Typedefs
//------------------------------------------------------------------------------
#ifndef va_list
    #if defined( __WINDOWS__ )
typedef char * va_list; // avoid stdlib include
    #elif defined( __APPLE__ ) || defined( __LINUX__ )
        #include <stdarg.h>
    #endif
#endif

// AString
//------------------------------------------------------------------------------
class AString
{
public:
    explicit AString();
    explicit AString( const AString & string );
    explicit AString( AString && string );
    explicit AString( const char * string );
    explicit AString( const char * start, const char * end );
    ~AString();

    [[nodiscard]] uint32_t GetLength() const { return m_Length; }
    [[nodiscard]] bool IsEmpty() const { return ( m_Length == 0 ); }

    // C-style compatibility
    [[nodiscard]] const char * Get() const { return m_Contents; }
    [[nodiscard]] const char * GetEnd() const { return ( m_Contents + m_Length ); }
    [[nodiscard]] const char & operator[]( size_t index ) const
    {
        ASSERT( index < m_Length );
        return m_Contents[ index ];
    }

    // a pre-constructed global empty string for convenience
    static const AString & GetEmpty() { return s_EmptyAString; }

    // assignment
    AString & operator=( const char * string )
    {
        Assign( string );
        return *this;
    }
    AString & operator=( const AString & string )
    {
        Assign( string );
        return *this;
    }
    AString & operator=( AString && string )
    {
        Assign( Move( string ) );
        return *this;
    }
    void Assign( const char * string );
    void Assign( const char * start, const char * end );
    void Assign( const AString & string );
    void Assign( AString && string );

    // comparison
    [[nodiscard]] bool operator==( const char * other ) const;
    [[nodiscard]] bool operator==( const AString & other ) const;
    [[nodiscard]] bool operator!=( const char * other ) const { return !( *this == other ); }
    [[nodiscard]] bool operator!=( const AString & other ) const { return !( *this == other ); }
    [[nodiscard]] int32_t Compare( const AString & other ) const;
    [[nodiscard]] int32_t Compare( const char * other ) const;
    [[nodiscard]] int32_t CompareI( const AString & other ) const;
    [[nodiscard]] int32_t CompareI( const char * other ) const;
    [[nodiscard]] bool Equals( const char * other ) const { return ( *this == other ); }
    [[nodiscard]] bool Equals( const AString & other ) const { return ( *this == other ); }
    [[nodiscard]] bool EqualsI( const char * other ) const { return ( CompareI( other ) == 0 ); }
    [[nodiscard]] bool EqualsI( const AString & other ) const { return ( CompareI( other ) == 0 ); }
    [[nodiscard]] bool operator<( const AString & other ) const { return ( Compare( other ) < 0 ); }
    [[nodiscard]] bool operator>( const AString & other ) const { return ( Compare( other ) > 0 ); }

    [[nodiscard]] bool MemoryMustBeFreed() const { return ( ( m_ReservedAndFlags & kMemMustBeFreedFlag ) == kMemMustBeFreedFlag ); }

    class TokenRange
    {
    public:
        uint32_t m_StartIndex = 0;
        uint32_t m_EndIndex = 0;
    };
    void Tokenize( Array<TokenRange> & outTokenRanges,
                   char splitChar = ' ' ) const;
    void Tokenize( Array<AString> & tokens, char splitChar = ' ' ) const;

    // searching
    [[nodiscard]] const char * Find( char c, const char * startPos = nullptr, const char * endPos = nullptr ) const;
    [[nodiscard]] const char * Find( const char * subString, const char * startPos = nullptr, const char * endPos = nullptr ) const;
    [[nodiscard]] const char * Find( const AString & subString, const char * startPos = nullptr, const char * endPos = nullptr ) const;

    [[nodiscard]] const char * FindI( char c, const char * startPos = nullptr, const char * endPos = nullptr ) const;
    [[nodiscard]] const char * FindI( const char * subString, const char * startPos = nullptr, const char * endPos = nullptr ) const;
    [[nodiscard]] const char * FindI( const AString & subString, const char * startPos = nullptr, const char * endPos = nullptr ) const;

    [[nodiscard]] const char * FindLast( char c, const char * startPos = nullptr, const char * endPos = nullptr ) const;
    [[nodiscard]] const char * FindLast( const char * subString, const char * startPos = nullptr, const char * endPos = nullptr ) const;
    [[nodiscard]] const char * FindLast( const AString & subString, const char * startPos = nullptr, const char * endPos = nullptr ) const;

    [[nodiscard]] const char * FindLastI( char c, const char * startPos = nullptr, const char * endPos = nullptr ) const;
    [[nodiscard]] const char * FindLastI( const char * subString, const char * startPos = nullptr, const char * endPos = nullptr ) const;
    [[nodiscard]] const char * FindLastI( const AString & subString, const char * startPos = nullptr, const char * endPos = nullptr ) const;

    [[nodiscard]] bool EndsWith( char c ) const;
    [[nodiscard]] bool EndsWith( const char * string ) const;
    [[nodiscard]] bool EndsWith( const AString & string ) const;

    [[nodiscard]] bool EndsWithI( const char * other ) const;
    [[nodiscard]] bool EndsWithI( const AString & other ) const;

    [[nodiscard]] bool BeginsWith( char c ) const;
    [[nodiscard]] bool BeginsWith( const char * string ) const;
    [[nodiscard]] bool BeginsWith( const AString & string ) const;

    [[nodiscard]] bool BeginsWithI( const char * string ) const;
    [[nodiscard]] bool BeginsWithI( const AString & string ) const;

    // pattern matching
    [[nodiscard]] static bool Match( const char * pattern, const char * string );
    [[nodiscard]] bool Matches( const char * pattern ) const { return Match( pattern, m_Contents ); }
    [[nodiscard]] static bool MatchI( const char * pattern, const char * string );
    [[nodiscard]] bool MatchesI( const char * pattern ) const { return MatchI( pattern, m_Contents ); }

    // string manipulation helpers
    static void Copy( const char * src, char * dst );
    static void Copy( const char * src, char * dst, size_t len );
    [[nodiscard]] static size_t StrLen( const char * string );
    [[nodiscard]] static int32_t StrNCmp( const char * a, const char * b, size_t num );
    [[nodiscard]] static int32_t StrNCmpI( const char * a, const char * b, size_t num );

    // Character helpers
    [[nodiscard]] static bool IsWhitespace( char c ) { return ( ( c == ' ' ) || ( c == '\r' ) || ( c == '\n' ) || ( c == '\t' ) ); }
    [[nodiscard]] static bool IsUppercaseLetter( char c ) { return ( ( c >= 'A' ) && ( c <= 'Z' ) ); }
    [[nodiscard]] static bool IsLowercaseLetter( char c ) { return ( ( c >= 'a' ) && ( c <= 'z' ) ); }
    [[nodiscard]] static bool IsLetter( char c ) { return IsUppercaseLetter( c ) || IsLowercaseLetter( c ); }
    [[nodiscard]] static bool IsNumber( char c ) { return ( ( c >= '0' ) && ( c <= '9' ) ); }

    // range iteration
    [[nodiscard]] const char * begin() const { return m_Contents; }
    [[nodiscard]] const char * end() const { return m_Contents + m_Length; }

protected:
    inline static const uint32_t kMemMustBeFreedFlag = 0x00000001;
    inline static const uint32_t kReservedMask = 0xFFFFFFFE;

    void SetReserved( uint32_t reserved, bool mustFreeMemory )
    {
        ASSERT( ( reserved & kMemMustBeFreedFlag ) == 0 ); // ensure reserved does not use lower bit
        m_ReservedAndFlags = ( reserved ^ ( mustFreeMemory ? (uint32_t)kMemMustBeFreedFlag : 0 ) );
    }
    NO_INLINE void Grow( uint32_t newLen );     // Grow capacity, transferring existing string data (for concatenation)
    NO_INLINE void GrowNoCopy( uint32_t newLen ); // Grow capacity, discarding existing string data (for assignment/construction)

    uint32_t* m_ReferenceCount; // automatic reference count for m_Contents (if not null)
    char * m_Contents; // always points to valid null terminated string (even when empty)
    uint32_t m_Length; // length in characters
    uint32_t m_ReservedAndFlags; // reserved space in characters (even) and least significant bit used for static flag

    static const char * const s_EmptyString;
    static const AString s_EmptyAString;
};

//------------------------------------------------------------------------------
