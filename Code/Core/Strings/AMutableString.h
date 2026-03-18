// AMutableString.h
//------------------------------------------------------------------------------
#pragma once

// Includes
//------------------------------------------------------------------------------
#include "Core/Strings/AString.h"

// AMutableString
//------------------------------------------------------------------------------
class AMutableString : public AString
{
public:
    explicit AMutableString();
    explicit AMutableString( size_t reserve );
    explicit AMutableString( const AString & string );
    explicit AMutableString( AString && string );
    explicit AMutableString( const AMutableString & string );
    explicit AMutableString( AMutableString && string );
    explicit AMutableString( const char * string );
    explicit AMutableString( const char * start, const char * end );
    ~AMutableString() = default;

    AMutableString & operator=( const char * string )
    {
        Assign( string );
        return *this;
    }
    AMutableString & operator=( const AString & string )
    {
        Assign( string );
        return *this;
    }
    AMutableString & operator=( AString && string )
    {
        Assign( Move( string ) );
        return *this;
    }
    AMutableString & operator=( const AMutableString & string )
    {
        Assign( string );
        return *this;
    }
    AMutableString & operator=( AMutableString && string )
    {
        Assign( Move( string ) );
        return *this;
    }

    [[nodiscard]] uint32_t GetReserved() const { return ( m_ReservedAndFlags & kReservedMask ); }

    // C-style compatibility
    [[nodiscard]] char * Get() { return m_Contents; }
    [[nodiscard]] char * GetEnd() { return ( m_Contents + m_Length ); }
    [[nodiscard]] char & operator[]( size_t index )
    {
        ASSERT( index < m_Length );
        return m_Contents[ index ];
    }

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
    void Clear();
    void ClearAndFreeMemory();
    void SetReserved( size_t capacity );

    // manually set length - NOTE: caller is responsible for making string contents valid
    void SetLength( uint32_t len );

    // concatenation
    AString & operator+=( char c );
    AString & operator+=( const char * string );
    AString & operator+=( const AString & string );
    AString & Append( char c ) { return this->operator+=( c ); }
    AString & Append( const char * string ) { return this->operator+=( string ); }
    AString & Append( const AString & string ) { return this->operator+=( string ); }
    AString & Append( const char * string, size_t len );
    AString & Append( const char * start, const char * end ) { return Append( start, static_cast<size_t>( end - start ) ); }
    AString & AppendFormat( MSVC_SAL_PRINTF const char * fmtString, ... ) FORMAT_STRING( 2, 3 );
    AString & AppendList( const Array<AString> & list, char separator = ',' );

    // Format
    AString & Format( MSVC_SAL_PRINTF const char * fmtString, ... ) FORMAT_STRING( 2, 3 );
    AString & VFormat( const char * fmtString, va_list arg );

    // ScanF
    [[nodiscard]] int32_t Scan( MSVC_SAL_SCANF const char * fmtString, ... ) const SCAN_STRING( 2, 3 );
    [[nodiscard]] static int32_t ScanS( const char * buffer, MSVC_SAL_SCANF const char * fmtString, ... ) SCAN_STRING( 2, 3 );

    void RemoveQuotes();
    static void RemoveQuotes( Array<AString> & inoutTokens );

    // transformations
    uint32_t Replace( char from, char to, uint32_t maxReplaces = 0 );
    uint32_t Replace( const char * from, const char * to, uint32_t maxReplaces = 0 );
    void ToLower();
    void ToUpper();

    // Trimming
    void Trim( uint32_t startCharsToTrim, uint32_t endCharsToTrim );
    void TrimStart( char charToTrimFromStart );
    void TrimEnd( char charToTrimFromStart );

    // searching
    [[nodiscard]] char * Find( char c, const char * startPos = nullptr, const char * endPos = nullptr ) { return const_cast<char *>( ( (const AString *)this )->Find( c, startPos, endPos ) ); }
    [[nodiscard]] char * Find( const char * subString, const char * startPos = nullptr, const char * endPos = nullptr ) { return const_cast<char *>( ( (const AString *)this )->Find( subString, startPos, endPos ) ); }
    [[nodiscard]] char * Find( const AString & subString, const char * startPos = nullptr, const char * endPos = nullptr ) { return const_cast<char *>( ( (const AString *)this )->Find( subString, startPos, endPos ) ); }

    [[nodiscard]] char * FindI( char c, const char * startPos = nullptr, const char * endPos = nullptr ) { return const_cast<char *>( ( (const AString *)this )->FindI( c, startPos, endPos ) ); }
    [[nodiscard]] char * FindI( const char * subString, const char * startPos = nullptr, const char * endPos = nullptr ) { return const_cast<char *>( ( (const AString *)this )->FindI( subString, startPos, endPos ) ); }
    [[nodiscard]] char * FindI( const AString & subString, const char * startPos = nullptr, const char * endPos = nullptr ) { return const_cast<char *>( ( (const AString *)this )->FindI( subString, startPos, endPos ) ); }

    [[nodiscard]] char * FindLast( char c, const char * startPos = nullptr, const char * endPos = nullptr ) { return const_cast<char *>( ( (const AString *)this )->FindLast( c, startPos, endPos ) ); }
    [[nodiscard]] char * FindLast( const char * subString, const char * startPos = nullptr, const char * endPos = nullptr ) { return const_cast<char *>( ( (const AString *)this )->FindLast( subString, startPos, endPos ) ); }
    [[nodiscard]] char * FindLast( const AString & subString, const char * startPos = nullptr, const char * endPos = nullptr ) { return const_cast<char *>( ( (const AString *)this )->FindLast( subString, startPos, endPos ) ); }

    [[nodiscard]] char * FindLastI( char c, const char * startPos = nullptr, const char * endPos = nullptr ) { return const_cast<char *>( ( (const AString *)this )->FindLastI( c, startPos, endPos ) ); }
    [[nodiscard]] char * FindLastI( const char * subString, const char * startPos = nullptr, const char * endPos = nullptr ) { return const_cast<char *>( ( (const AString *)this )->FindLastI( subString, startPos, endPos ) ); }
    [[nodiscard]] char * FindLastI( const AString & subString, const char * startPos = nullptr, const char * endPos = nullptr ) { return const_cast<char *>( ( (const AString *)this )->FindLastI( subString, startPos, endPos ) ); }

    // range iteration
    [[nodiscard]] char * begin() { return m_Contents; }
    [[nodiscard]] char * end() { return m_Contents + m_Length; }

    // Format
    AString & Format( MSVC_SAL_PRINTF const char * fmtString, ... ) FORMAT_STRING( 2, 3 );
    AString & VFormat( const char * fmtString, va_list arg );

    // ScanF
    [[nodiscard]] int32_t Scan( MSVC_SAL_SCANF const char * fmtString, ... ) const SCAN_STRING( 2, 3 );
    [[nodiscard]] static int32_t ScanS( const char * buffer, MSVC_SAL_SCANF const char * fmtString, ... ) SCAN_STRING( 2, 3 );
};
