// SharedPtr
//------------------------------------------------------------------------------
#pragma once

// Includes
//------------------------------------------------------------------------------
#include "Core/Containers/Forward.h"
#include "Core/Containers/Move.h"
#include "Core/Env/Assert.h"
#include "Core/Math/Conversions.h"
#include "Core/Mem/Mem.h"

// SharedPtr
//------------------------------------------------------------------------------
template <class T>
class SharedPtr
{
public:
    explicit SharedPtr() = default;
    template <class... ARGS>
    explicit SharedPtr( ARGS &&... args )
    {
        Emplace( Forward( ARGS, args )... );
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

    template <class... ARGS>
    void Emplace( ARGS &&... args )
    {
        Clear();

        const size_t offset = Math::RoundUp( sizeof( T ), __alignof( uint32_t ) );
        const size_t size = offset + sizeof( T );
        void * ptr = ALLOC( size, Math::Max( __alignof( T ), __alignof( uint32_t ) ) );

        m_Pointer = static_cast<T *>( ptr );
        m_ReferenceCount = static_cast<uint32_t *>( static_cast<void *>( static_cast<uint8_t *>( ptr ) + offset ) );

        INPLACE_NEW( m_Pointer ) T( Forward( ARGS, args )... );
        INPLACE_NEW( m_ReferenceCount ) uint32_t( 1 );
    }
    void operator=( const SharedPtr<T> & other )
    {
        Clear();

        m_ReferenceCount = other.m_ReferenceCount;
        m_Pointer = other.m_Pointer;

        if ( m_ReferenceCount )
        {
            (*m_ReferenceCount)++;
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
            (*m_ReferenceCount)--;
            if ( *m_ReferenceCount == 0 )
            {
                m_Pointer->~T();
                FREE( static_cast<void *>( m_Pointer ) );
            }
            m_ReferenceCount = nullptr;
            m_Pointer = nullptr;
        }
    }

private:
    T * m_Pointer = nullptr;
    uint32_t * m_ReferenceCount = nullptr;
};

