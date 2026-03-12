// SharedPtr
//------------------------------------------------------------------------------
#pragma once

// Includes
//------------------------------------------------------------------------------
#include "Core/Containers/Move.h"
#include "Core/Env/Assert.h"
#include "Core/Mem/Mem.h"

// SharedPtr
//------------------------------------------------------------------------------
template <class T>
class SharedPtr
{
public:
    explicit SharedPtr() = default;
    explicit SharedPtr( T * ptr )
    {
        *this = ptr;
    }
    explicit SharedPtr( SharedPtr<T> && other )
    {
        *this = Move( other );
    }
    explicit SharedPtr( const SharedPtr<T> & other )
    {
        *this = other;
    }
    ~SharedPtr()
    {
        Clear();
    }

    [[nodiscard]] T * Get() { return m_Pointer; }
    [[nodiscard]] const T * Get() const { return m_Pointer; }
    [[nodiscard]] T * operator->()
    {
        ASSERT( m_Pointer );
        return m_Pointer;
    }
    [[nodiscard]] const T * operator->() const
    {
        ASSERT( m_Pointer );
        return m_Pointer;
    }

    void Replace( T * newPtr )
    {
        *this = newPtr;
    }
    void operator=( T * ptr )
    {
        Clear();

        if ( ptr )
        {
            m_ReferenceCount = FNEW( uint32_t( 1 ) );
            m_Pointer = ptr;
        }
    }
    void operator=( const SharedPtr<T> & other )
    {
        Clear();

        m_ReferenceCount = other.m_ReferenceCount;
        m_Pointer = other.m_Pointer;

        if ( m_ReferenceCount )
        {
            *m_ReferenceCount++;
        }
    }
    void operator=( SharedPtr<T> && other )
    {
        Clear();

        m_ReferenceCount = other.m_ReferenceCount;
        m_Pointer = other.m_Pointer;

        other.m_ReferenceCount = nullptr;
        other.m_Pointer = nullptr;
    }

    uint32_t ReferenceCount() const
    {
        return ( m_ReferenceCount ? *m_ReferenceCount : 0 );
    }

    // clear this ptr, decrement the reference count, and potentially delete the object
    void Clear()
    {
        if ( m_Pointer )
        {
            ASSERT( m_ReferenceCount != nullptr );
            ASSERT( *m_ReferenceCount > 0 );
            *m_ReferenceCount--;
            if ( *m_ReferenceCount == 0 )
            {
                FDELETE( m_ReferenceCount );
                FDELETE( m_Pointer );
            }
            m_ReferenceCount = nullptr;
            m_Pointer = nullptr;
        }
    }

    // clear this ptr, decrement the reference count, but do not delete the object
    [[nodiscard]] T * ReleaseOwnership()
    {
        T * ptr = m_Pointer;
        if ( m_Pointer )
        {
            ASSERT( m_ReferenceCount != nullptr );
            ASSERT( *m_ReferenceCount > 0 );
            *m_ReferenceCount--;
            if ( *m_ReferenceCount == 0 )
            {
                FDELETE( m_ReferenceCount );
            }
            m_ReferenceCount = nullptr;
            m_Pointer = nullptr;
        }
        return ptr;
    }

private:
    T * m_Pointer = nullptr;
    uint32_t * m_ReferenceCount = nullptr;
};

