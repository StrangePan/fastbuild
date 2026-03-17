// ObjectListNode.h - manages a list of ObjectNodes
//------------------------------------------------------------------------------
#pragma once

// Includes
//------------------------------------------------------------------------------
#include "Node.h"

// FBuild
#include <Tools/FBuild/FBuildCore/Graph/ObjectNode.h>

// Core
#include "Core/Containers/Array.h"

// Forward Declarations
//------------------------------------------------------------------------------
class Args;
class CompilerInfoNode;
class CompilerNode;
class Function;
class NodeGraph;

// ObjectListNode
//------------------------------------------------------------------------------
class ObjectListNode : public Node
{
    REFLECT_NODE_DECLARE( ObjectListNode )
public:
    ObjectListNode();
    virtual bool Initialize( NodeGraph & nodeGraph, const BFFToken * iter, const Function * function ) override;
    virtual ~ObjectListNode() override;

    static Node::Type GetTypeS() { return Node::OBJECT_LIST_NODE; }

    virtual bool IsAFile() const override;
    virtual uint8_t GetConcurrencyGroupIndex() const override;

    const char * GetObjExtension() const;

    void GetInputFiles( bool objectsInsteadOfLibs, Array<SharedPtr<AString>> & outInputs ) const;
    void GetInputFiles( Array<SharedPtr<AString>> & files ) const;

    const AString & GetCompilerOutputPath() const { return *m_CompilerOutputPath; }
    const AString & GetCompilerOptions() const { return *m_CompilerOptions; }
    const AString & GetCompilerOptionsDeoptimized() const { return *m_CompilerOptionsDeoptimized; }
    const AString & GetCompilerOptionsPCH() const { return *m_PCHOptions; }
    const AString & GetPreprocessorOptions() const { return *m_PreprocessorOptions; }
    const Array<SharedPtr<AString>> & GetCompilerForceUsing() const { return *m_CompilerForceUsing; }
    CompilerNode * GetCompiler() const { return m_CompilerNode; }
    CompilerNode * GetPreprocessor() const { return m_PreprocessorNode; }
    bool GetDeoptimizeWritableFiles() const { return m_DeoptimizeWritableFiles; }
    bool GetDeoptimizeWritableFilesWithToken() const { return m_DeoptimizeWritableFilesWithToken; }
    const AString & GetPrecompiledHeaderName() const { return *m_PrecompiledHeaderName; }
    const AString & GetPCHObjectFileName() const { return *m_PCHObjectFileName; }

    [[nodiscard]] bool IsCachingAllowed() const { return m_AllowCaching; }
    [[nodiscard]] bool IsDistributionAllowed() const { return m_AllowDistribution; }

    void GetObjectFileName( const AString & fileName, const AString & baseDir, AString & objFile );

    [[nodiscard]] const CompilerInfoNode * GetCompilerInfo() const { return m_CompilerInfoNode; }

    void EnumerateInputFiles( void ( *callback )( const AString & inputFile, const AString & baseDir, void * userData ), void * userData ) const;

protected:
    friend class FunctionObjectList;

    virtual bool GatherDynamicDependencies( NodeGraph & nodeGraph );
    virtual bool DoDynamicDependencies( NodeGraph & nodeGraph ) override;
    virtual BuildResult DoBuild( Job * job ) override;

    // internal helpers
    void CalculateOwnerObjectListHash();
    bool CreateDynamicObjectNode( NodeGraph & nodeGraph,
                                  const AString & inputFileName,
                                  const AString & baseDir,
                                  bool isUnityNode = false,
                                  bool isIsolatedFromUnityNode = false );
    ObjectNode * CreateObjectNode( NodeGraph & nodeGraph,
                                   const BFFToken * iter,
                                   const Function * function,
                                   const ObjectNode::CompilerFlags flags,
                                   const ObjectNode::CompilerFlags preprocessorFlags,
                                   const AString & objectName,
                                   const AString & objectInput );
    [[nodiscard]] bool CheckLightCacheArgs( NodeGraph & nodeGraph,
                                            const BFFToken * iter,
                                            const Function * function,
                                            bool compilingFiles,
                                            CompilerInfoNode *& outCompilerInfoDependency ) const;

    // Exposed Properties
    SharedPtr<AString> m_Compiler;
    SharedPtr<AString> m_CompilerOptions;
    SharedPtr<AString> m_CompilerOptionsDeoptimized;
    SharedPtr<AString> m_CompilerOutputPath;
    SharedPtr<AString> m_CompilerOutputPrefix;
    SharedPtr<AString> m_CompilerOutputExtension;
    SharedPtr<Array<SharedPtr<AString>>> m_CompilerInputPath;
    SharedPtr<Array<SharedPtr<AString>>> m_CompilerInputPattern;
    SharedPtr<Array<SharedPtr<AString>>> m_CompilerInputExcludePath;
    SharedPtr<Array<SharedPtr<AString>>> m_CompilerInputExcludedFiles;
    SharedPtr<Array<SharedPtr<AString>>> m_CompilerInputExcludePattern;
    SharedPtr<Array<SharedPtr<AString>>> m_CompilerInputFiles;
    SharedPtr<Array<SharedPtr<AString>>> m_CompilerInputUnity;
    SharedPtr<AString> m_CompilerInputFilesRoot;
    SharedPtr<Array<SharedPtr<AString>>> m_CompilerInputObjectLists;
    SharedPtr<Array<SharedPtr<AString>>> m_CompilerForceUsing;
    bool m_CompilerInputAllowNoFiles = false;
    bool m_CompilerInputPathRecurse = true;
    bool m_CompilerOutputKeepBaseExtension = false;
    bool m_DeoptimizeWritableFiles = false;
    bool m_DeoptimizeWritableFilesWithToken = false;
    bool m_AllowDistribution = true;
    bool m_AllowCaching = true;
    uint8_t m_ConcurrencyGroupIndex = 0; // Internal; placed here to use padding
    SharedPtr<AString> m_PCHInputFile;
    SharedPtr<AString> m_PCHOutputFile;
    SharedPtr<AString> m_PCHOptions;
    SharedPtr<AString> m_Preprocessor;
    SharedPtr<AString> m_PreprocessorOptions;
    Array<Node *> m_PreBuildDependencyNames;
    SharedPtr<AString> m_ConcurrencyGroupName;

    // Internal State
    CompilerNode * m_CompilerNode = nullptr;
    CompilerNode * m_PreprocessorNode = nullptr;
    CompilerInfoNode * m_CompilerInfoNode = nullptr;
    SharedPtr<AString> m_PrecompiledHeaderName;
#if defined( __WINDOWS__ )
    SharedPtr<AString> m_PrecompiledHeaderCPPFile;
#endif
    SharedPtr<AString> m_PCHObjectFileName;
    SharedPtr<AString> m_ExtraPDBPath;
    SharedPtr<AString> m_ExtraASMPath;
    SharedPtr<AString> m_ExtraSourceDependenciesPath;
    uint32_t m_ObjectListInputStartIndex = 0;
    uint32_t m_ObjectListInputEndIndex = 0;
    uint32_t m_OwnerObjectListHash = 0;
    ObjectNode::CompilerFlags m_CompilerFlags;
    ObjectNode::CompilerFlags m_PreprocessorFlags;
};

//------------------------------------------------------------------------------
