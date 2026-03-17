// CSNode.h - a node that builds a C# assembly
//------------------------------------------------------------------------------
#pragma once

// Includes
//------------------------------------------------------------------------------
// FBuildCore
#include "Tools/FBuild/FBuildCore/Graph/FileNode.h"

// Core
#include "Core/Containers/Array.h"

// Forward Declarations
//------------------------------------------------------------------------------
class Args;
class Function;

// CSNode
//------------------------------------------------------------------------------
class CSNode : public FileNode
{
    REFLECT_NODE_DECLARE( CSNode )
public:
    explicit CSNode();
    virtual bool Initialize( NodeGraph & nodeGraph, const BFFToken * iter, const Function * function ) override;
    virtual ~CSNode() override;

    static Node::Type GetTypeS() { return Node::CS_NODE; }

private:
    virtual bool DoDynamicDependencies( NodeGraph & nodeGraph ) override;
    virtual BuildResult DoBuild( Job * job ) override;

    CompilerNode * GetCompiler() const;

    void EmitCompilationMessage( const Args & fullArgs ) const;

    bool BuildArgs( Args & fullArgs ) const;
    void GetInputFiles( Args & fullArgs, const AString & pre, const AString & post ) const;
    void GetExtraRefs( Args & fullArgs, const AString & pre, const AString & post ) const;

    // Exposed Properties
    SharedPtr<AString> m_Compiler;
    SharedPtr<AString> m_CompilerOptions;
    SharedPtr<AString> m_CompilerOutput;
    SharedPtr<Array<SharedPtr<AString>>> m_CompilerInputPath;
    bool m_CompilerInputPathRecurse;
    SharedPtr<Array<SharedPtr<AString>>> m_CompilerInputPattern;
    SharedPtr<Array<SharedPtr<AString>>> m_CompilerInputExcludePath;
    SharedPtr<Array<SharedPtr<AString>>> m_CompilerInputExcludedFiles;
    SharedPtr<Array<SharedPtr<AString>>> m_CompilerInputExcludePattern;
    SharedPtr<Array<SharedPtr<AString>>> m_CompilerInputFiles;
    SharedPtr<Array<SharedPtr<AString>>> m_CompilerReferences;
    Array<Node *> m_PreBuildDependencyNames;

    // Internal State
    uint32_t m_NumCompilerInputFiles;
    uint32_t m_NumCompilerReferences;
};

//------------------------------------------------------------------------------
