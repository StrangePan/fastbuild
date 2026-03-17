// ExecNode
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

// ExecNode
//------------------------------------------------------------------------------
class ExecNode : public FileNode
{
    REFLECT_NODE_DECLARE( ExecNode )
public:
    explicit ExecNode();
    virtual bool Initialize( NodeGraph & nodeGraph, const BFFToken * iter, const Function * function ) override;
    virtual ~ExecNode() override;

    static Node::Type GetTypeS() { return Node::EXEC_NODE; }

private:
    virtual bool DoDynamicDependencies( NodeGraph & nodeGraph ) override;
    virtual bool DetermineNeedToBuildStatic() const override;
    virtual BuildResult DoBuild( Job * job ) override;
    virtual uint8_t GetConcurrencyGroupIndex() const override;

    const FileNode * GetExecutable() const { return m_StaticDependencies[ 0 ].GetNode()->CastTo<FileNode>(); }
    void GetFullArgs( AString & fullArgs ) const;
    void GetInputFiles( AString & fullArgs, const AString & pre, const AString & post ) const;

    void EmitCompilationMessage( const AString & args ) const;

    // Exposed Properties
    SharedPtr<AString> m_ExecExecutable;
    SharedPtr<Array<SharedPtr<AString>>> m_ExecInput;
    SharedPtr<Array<SharedPtr<AString>>> m_ExecInputPath;
    SharedPtr<Array<SharedPtr<AString>>> m_ExecInputPattern;
    SharedPtr<Array<SharedPtr<AString>>> m_ExecInputExcludePath;
    SharedPtr<Array<SharedPtr<AString>>> m_ExecInputExcludedFiles;
    SharedPtr<Array<SharedPtr<AString>>> m_ExecInputExcludePattern;
    SharedPtr<AString> m_ExecArguments;
    SharedPtr<AString> m_ExecWorkingDir;
    int32_t m_ExecReturnCode;
    bool m_ExecAlwaysShowOutput;
    bool m_ExecUseStdOutAsOutput;
    bool m_ExecAlways;
    bool m_ExecInputPathRecurse;
    Array<Node *> m_PreBuildDependencyNames;
    SharedPtr<Array<SharedPtr<AString>>> m_Environment;
    SharedPtr<AString> m_ConcurrencyGroupName;

    // Internal State
    uint32_t m_NumExecInputFiles;
    uint8_t m_ConcurrencyGroupIndex = 0;
    mutable const char * m_EnvironmentString = nullptr;
};

//------------------------------------------------------------------------------
