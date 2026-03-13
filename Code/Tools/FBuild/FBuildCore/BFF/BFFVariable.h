// BFFVariable - a single variable in a BFFStack
//------------------------------------------------------------------------------
#pragma once

// Includes
//------------------------------------------------------------------------------
#include "Core/Containers/Array.h"
#include "Core/Containers/SharedPtr.h"
#include "Core/Strings/AString.h"

// Forward Declarations
//------------------------------------------------------------------------------
class BFFToken;

// BFFVariable
//------------------------------------------------------------------------------
class BFFVariable
{
public:
    enum VarType : uint8_t
    {
        VAR_ANY = 0, // used for searching
        VAR_STRING = 1,
        VAR_BOOL = 2,
        VAR_ARRAY_OF_STRINGS = 3,
        VAR_INT = 4,
        VAR_STRUCT = 5,
        VAR_ARRAY_OF_STRUCTS = 6,
        MAX_VAR_TYPES    // NOTE: Be sure to update s_TypeNames when adding to here
    };

    explicit BFFVariable( const BFFVariable & other );

    explicit BFFVariable( const SharedPtr<AString> & name, const BFFToken & token, VarType type );
    explicit BFFVariable( const SharedPtr<AString> & name, const BFFToken & token, const SharedPtr<AString> & value );
    explicit BFFVariable( const SharedPtr<AString> & name, const BFFToken & token, bool value );
    explicit BFFVariable( const SharedPtr<AString> & name, const BFFToken & token, const SharedPtr<Array<SharedPtr<AString>>> & values );
    explicit BFFVariable( const SharedPtr<AString> & name, const BFFToken & token, int32_t i );
    explicit BFFVariable( const SharedPtr<AString> & name, const BFFToken & token, const SharedPtr<Array<BFFVariable>> & values );
    explicit BFFVariable( const SharedPtr<AString> & name, const BFFToken & token, SharedPtr<Array<BFFVariable>> && values );
    explicit BFFVariable( const SharedPtr<AString> & name, const BFFToken & token, const SharedPtr<Array<BFFVariable>> & structs, VarType type ); // type for disambiguation
    ~BFFVariable();

    BFFVariable & operator=( const BFFVariable & other );

    const SharedPtr<AString> & GetName() const { return m_Name; }

    const SharedPtr<AString> & GetString() const
    {
        ASSERT( IsString() );
        return m_StringValue;
    }
    const SharedPtr<Array<SharedPtr<AString>>> & GetArrayOfStrings() const
    {
        ASSERT( IsArrayOfStrings() );
        return m_ArrayValues;
    }
    int32_t GetInt() const
    {
        ASSERT( IsInt() );
        return m_IntValue;
    }
    bool GetBool() const
    {
        ASSERT( IsBool() );
        return m_BoolValue;
    }
    const SharedPtr<Array<BFFVariable>> & GetStructMembers() const
    {
        ASSERT( IsStruct() );
        return m_SubVariables;
    }
    const SharedPtr<Array<BFFVariable>> & GetArrayOfStructs() const
    {
        ASSERT( IsArrayOfStructs() );
        return m_SubVariables;
    }

    VarType GetType() const { return m_Type; }
    static const char * GetTypeName( VarType t ) { return s_TypeNames[ (uint32_t)t ]; }

    bool IsString() const { return m_Type == BFFVariable::VAR_STRING; }
    bool IsBool() const { return m_Type == BFFVariable::VAR_BOOL; }
    bool IsArrayOfStrings() const { return m_Type == BFFVariable::VAR_ARRAY_OF_STRINGS; }
    bool IsInt() const { return m_Type == BFFVariable::VAR_INT; }
    bool IsStruct() const { return m_Type == BFFVariable::VAR_STRUCT; }
    bool IsArrayOfStructs() const { return m_Type == BFFVariable::VAR_ARRAY_OF_STRUCTS; }

    bool Frozen() const { return m_FreezeCount > 0; }
    void Freeze() const { ++m_FreezeCount; }
    void Unfreeze() const
    {
        ASSERT( m_FreezeCount != 0 );
        --m_FreezeCount;
    }

    BFFVariable * ConcatVarsRecurse( const AString & dstName, const BFFVariable & other, const BFFToken * operatorIter ) const;

    static const BFFVariable * GetMemberByName( const AString & name, const Array<BFFVariable> & members );

    const BFFToken & GetToken() const { return m_Token; }

private:
    friend class BFFStackFrame;

    void SetValueString( const SharedPtr<AString> & value );
    void SetValueBool( bool value );
    void SetValueArrayOfStrings( const SharedPtr<Array<SharedPtr<AString>>> & values );
    void SetValueInt( int i );
    void SetValueStruct( const SharedPtr<Array<BFFVariable>> & members );
    void SetValueStruct( SharedPtr<Array<BFFVariable>> && members );
    void SetValueArrayOfStructs( const SharedPtr<Array<BFFVariable>> & values );

    SharedPtr<AString> m_Name;
    VarType m_Type;

    mutable uint8_t m_FreezeCount = 0;

    //
    bool m_BoolValue = false;
    int32_t m_IntValue = 0;
    SharedPtr<AString> m_StringValue;
    SharedPtr<Array<SharedPtr<AString>>> m_ArrayValues;
    SharedPtr<Array<BFFVariable>> m_SubVariables; // Used for struct members of arrays of structs
    const BFFToken & m_Token;

    static const char * s_TypeNames[ MAX_VAR_TYPES ];
};

//------------------------------------------------------------------------------
