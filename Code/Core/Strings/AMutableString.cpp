// AMutableString.cpp
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
#include "AMutableString.h"
#include "AStackString.h"
#include "Core/Math/Conversions.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// CONSTRUCTOR
//------------------------------------------------------------------------------
AMutableString::AMutableString()
    : AString()
{
    m_Contents = m_Storage;
    SetReserved( RESERVED, false );
    m_Storage[ 0 ] = '\0';
}

// CONSTRUCTOR (uint32_t)
//------------------------------------------------------------------------------
AMutableString::AMutableString( uint32_t reserve )
{
    char * mem = const_cast<char *>( s_EmptyString ); // cast to allow pointing to protected string
    if ( reserve > 0 )
    {
        reserve = Math::RoundUp( reserve, (uint32_t)2 );
        mem = (char *)ALLOC( reserve + 1 );
        mem[ 0 ] = '\000';
    }
    m_Contents = mem;
    m_Length = 0;
    SetReserved( reserve, true );
}

// CONSTRUCTOR (const AString &)
//------------------------------------------------------------------------------
AMutableString::AMutableString( const AString & string )
    : AString()
{
    m_Contents = m_Storage;
    SetReserved( RESERVED, false );
    Assign( string );
}

// CONSTRUCTOR (AString &&)
//------------------------------------------------------------------------------
AMutableString::AMutableString( AString && string )
    : AString()
{
    m_Contents = m_Storage;
    SetReserved( RESERVED, false );
    Assign( Move( string ) );
}

// CONSTRUCTOR (const AMutableString &)
//------------------------------------------------------------------------------
AMutableString::AMutableString( const AMutableString & string )
    : AString()
{
    m_Contents = m_Storage;
    SetReserved( RESERVED, false );
    Assign( string );
}

// CONSTRUCTOR (AMutableString &&)
//------------------------------------------------------------------------------
AMutableString::AMutableString( AMutableString && string )
    : AString()
{
    m_Contents = m_Storage;
    SetReserved( RESERVED, false );
    Assign( Move( string ) );
}

// CONSTRUCTOR (const char *)
//------------------------------------------------------------------------------
AMutableString::AMutableString( const char * string )
  : AString()
{
    m_Contents = m_Storage;
    SetReserved( RESERVED, false );
    Assign( string );
}

// CONSTRUCTOR( const char *, const char *)
//------------------------------------------------------------------------------
AMutableString::AMutableString( const char * start, const char * end )
  : AString()
{
    m_Contents = m_Storage;
    SetReserved( RESERVED, false );
    Assign( start, end );
}

// Format
//------------------------------------------------------------------------------
AString & AString::Format( MSVC_SAL_PRINTF const char * fmtString, ... )
{
    va_list args;
    va_start( args, fmtString );
    VFormat( fmtString, args );
    va_end( args );

    return *this;
}

// VFormat
//------------------------------------------------------------------------------
AString & AString::VFormat( const char * fmtString, va_list args )
{
    // try to work entirely on the stack
    const uint32_t STACK_BUFFER_SIZE( 8 * KILOBYTE );
    char stackBuffer[ STACK_BUFFER_SIZE ];
    char * buffer = stackBuffer;
    size_t bufferSize = STACK_BUFFER_SIZE;

#if defined( __WINDOWS__ )
loop:
    // attempt the formatting
    const int len = vsnprintf_s( buffer, bufferSize, _TRUNCATE, fmtString, args );

    // did it fail to fit?
    if ( len < 0 )
    {
        // free any old buffer allocations
        if ( buffer != stackBuffer )
        {
            FREE( buffer );
        }

        // double the buffer and try again
        bufferSize *= 2;
        buffer = (char *)ALLOC( bufferSize );
        goto loop;
    }
#else
    va_list argsCopy;
    va_copy( argsCopy, args );
    PRAGMA_DISABLE_PUSH_CLANG( "-Wformat-nonliteral" )
    int len = vsnprintf( nullptr, 0, fmtString, argsCopy );
    PRAGMA_DISABLE_POP_CLANG
    va_end( argsCopy );
    if ( len > ( (int)bufferSize - 1 ) )
    {
        bufferSize = static_cast<size_t>( len ) + 1;
        buffer = (char *)ALLOC( bufferSize );
    }
    PRAGMA_DISABLE_PUSH_CLANG( "-Wformat-nonliteral" )
    VERIFY( vsnprintf( buffer, bufferSize, fmtString, args ) >= 0 );
    PRAGMA_DISABLE_POP_CLANG
#endif

    // keep the final result
    Assign( buffer, buffer + len );

    // free memory if we allocated any
    if ( buffer != stackBuffer )
    {
        FREE( buffer );
    }

    return *this;
}

// Scan
//------------------------------------------------------------------------------
int32_t AString::Scan( MSVC_SAL_SCANF const char * fmtString, ... ) const
{
    va_list args;
    va_start( args, fmtString );
    const int32_t result = vsscanf( m_Contents, fmtString, args );
    va_end( args );
    return result;
}

// ScanS
//------------------------------------------------------------------------------
/*static*/ int32_t AString::ScanS( const char * buffer, MSVC_SAL_SCANF const char * fmtString, ... )
{
    va_list args;
    va_start( args, fmtString );
    const int32_t result = vsscanf( buffer, fmtString, args );
    va_end( args );
    return result;
}

// RemoveQuotes
//------------------------------------------------------------------------------
/*static*/ void AString::RemoveQuotes( Array<AString> & inoutTokens )
{
    for ( AString & token : inoutTokens )
    {
        token.RemoveQuotes();
    }
}

// RemoveQuotes
//------------------------------------------------------------------------------
void AString::RemoveQuotes()
{
    // Remove quotes in-place
    char * src = Get();
    char * dst = src;
    const char * const end = GetEnd();
    char quoteChar = 0;
    while ( src < end )
    {
        const char c = *src;
        if ( c == '"' )
        {
            if ( quoteChar == 0 )
            {
                // opening quote - remove from output
                quoteChar = c;
                ++src;
                continue;
            }
            else
            {
                // closing quote
                ASSERT( quoteChar == c );
                quoteChar = 0;
                ++src;
                continue; // Remove quote from token
            }
        }
        else if ( c == '\\' ) // Escape char
        {
            // collapse escaped quotes
            if ( ( src + 1 ) < end )
            {
                const char nextChar = src[ 1 ];
                if ( nextChar == '"' )
                {
                    // Replace escaped char with quote
                    src += 2;
                    *dst = nextChar;
                    ++dst;
                    continue;
                }
            }
        }

        *dst = *src;
        ++dst;
        ++src;
    }
    SetLength( static_cast<uint32_t>( dst - Get() ) );
}

//------------------------------------------------------------------------------
