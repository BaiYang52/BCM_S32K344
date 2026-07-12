
########################################################################
#
#  HERE script to generate Visual studio project file.
#
#  Input Param as environment variables:
#  -------------------------------------
#  - VC_INC_LIST : ';' separated list of include paths
#  - VC_DEF_LIST : ';' separated list of defines
#  - VC_SRC_LIST : space separated list of source files
#  - LST_PATH    : $(LST_PATH) from Makefile
#  - APPL_DIR    : Application directory. Like 'Appl'
#  - MAKE_FILES  : MakeSupport files.
#
#  Call example:
#  -------------
#  VC_INC_LIST="c:/;d:/" \
#  VC_DEF_LIST="BRS_01;BRS_02" \
#  VC_SRC_LIST="file1.c file2.c" \
#  vcxproj_10_HERE.sh
#
########################################################################

cat <<+++
<?xml version="1.0" encoding="utf-8"?>
<Project DefaultTargets="Build" ToolsVersion="4.0" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">
  <ItemGroup Label="ProjectConfigurations">
    <ProjectConfiguration Include="Debug|Win32">
      <Configuration>Debug</Configuration>
      <Platform>Win32</Platform>
    </ProjectConfiguration>
    <ProjectConfiguration Include="Release|Win32">
      <Configuration>Release</Configuration>
      <Platform>Win32</Platform>
    </ProjectConfiguration>
  </ItemGroup>
  <PropertyGroup Label="Globals">
    <ProjectGuid>{1E8CA329-3881-4C34-9C23-A1BED2081640}</ProjectGuid>
    <Keyword>Win32Proj</Keyword>
    <RootNamespace>${APPL_DIR}</RootNamespace>
  </PropertyGroup>
  <Import Project="\$(VCTargetsPath)\Microsoft.Cpp.Default.props" />
  <PropertyGroup Condition="'\$(Configuration)|\$(Platform)'=='Debug|Win32'" Label="Configuration">
    <ConfigurationType>Application</ConfigurationType>
    <UseDebugLibraries>true</UseDebugLibraries>
    <CharacterSet>NotSet</CharacterSet>
  </PropertyGroup>
  <PropertyGroup Condition="'\$(Configuration)|\$(Platform)'=='Release|Win32'" Label="Configuration">
    <ConfigurationType>Application</ConfigurationType>
    <UseDebugLibraries>false</UseDebugLibraries>
    <WholeProgramOptimization>true</WholeProgramOptimization>
    <CharacterSet>Unicode</CharacterSet>
  </PropertyGroup>
  <Import Project="\$(VCTargetsPath)\Microsoft.Cpp.props" />
  <ImportGroup Label="ExtensionSettings">
  </ImportGroup>
  <ImportGroup Label="PropertySheets" Condition="'\$(Configuration)|\$(Platform)'=='Debug|Win32'">
    <Import Project="\$(UserRootDir)\Microsoft.Cpp.\$(Platform).user.props" Condition="exists('\$(UserRootDir)\Microsoft.Cpp.\$(Platform).user.props')" Label="LocalAppDataPlatform" />
  </ImportGroup>
  <ImportGroup Label="PropertySheets" Condition="'\$(Configuration)|\$(Platform)'=='Release|Win32'">
    <Import Project="\$(UserRootDir)\Microsoft.Cpp.\$(Platform).user.props" Condition="exists('\$(UserRootDir)\Microsoft.Cpp.\$(Platform).user.props')" Label="LocalAppDataPlatform" />
  </ImportGroup>
  <PropertyGroup Label="UserMacros" />
  <PropertyGroup Condition="'\$(Configuration)|\$(Platform)'=='Debug|Win32'">
    <LinkIncremental>true</LinkIncremental>
    <IncludePath>${VC_INC_LIST};\$(IncludePath)</IncludePath>
  </PropertyGroup>
  <PropertyGroup Condition="'\$(Configuration)|\$(Platform)'=='Release|Win32'">
    <LinkIncremental>false</LinkIncremental>
  </PropertyGroup>
  <ItemDefinitionGroup Condition="'\$(Configuration)|\$(Platform)'=='Debug|Win32'">
    <ClCompile>
      <PrecompiledHeader>NotUsing</PrecompiledHeader>
      <WarningLevel>Level3</WarningLevel>
      <Optimization>Disabled</Optimization>
      <PreprocessorDefinitions>${VC_DEF_LIST};WIN32;NDEBUG;_CONSOLE;%(PreprocessorDefinitions)</PreprocessorDefinitions>
      <ExceptionHandling>false</ExceptionHandling>
      <DisableLanguageExtensions>false</DisableLanguageExtensions>
      <PrecompiledHeaderFile>
      </PrecompiledHeaderFile>
      <PrecompiledHeaderOutputFile>
      </PrecompiledHeaderOutputFile>
      <AssemblerOutput>AssemblyAndSourceCode</AssemblerOutput>
      <AssemblerListingLocation>${LST_PATH}</AssemblerListingLocation>
      <ObjectFileName>obj</ObjectFileName>
    </ClCompile>
    <Link>
      <SubSystem>Console</SubSystem>
      <GenerateDebugInformation>true</GenerateDebugInformation>
      <EntryPointSymbol>
      </EntryPointSymbol>
    </Link>
  </ItemDefinitionGroup>
  <ItemDefinitionGroup Condition="'\$(Configuration)|\$(Platform)'=='Release|Win32'">
    <ClCompile>
      <WarningLevel>Level3</WarningLevel>
      <PrecompiledHeader>
      </PrecompiledHeader>
      <Optimization>MaxSpeed</Optimization>
      <FunctionLevelLinking>true</FunctionLevelLinking>
      <IntrinsicFunctions>true</IntrinsicFunctions>
      <PreprocessorDefinitions>${VC_DEF_LIST};WIN32;NDEBUG;_CONSOLE;%(PreprocessorDefinitions)</PreprocessorDefinitions>
    </ClCompile>
    <Link>
      <SubSystem>Console</SubSystem>
      <GenerateDebugInformation>true</GenerateDebugInformation>
      <EnableCOMDATFolding>true</EnableCOMDATFolding>
      <OptimizeReferences>true</OptimizeReferences>
    </Link>
  </ItemDefinitionGroup>
+++
for file in ${VC_SRC_LIST}; do
  cat <<+++
  <ItemGroup>
    <ClCompile Include="${file}" />
  </ItemGroup>
+++
done

for file in ${MAKE_FILES}; do
  cat <<+++
  <ItemGroup>
    <None Include="${file}" />    
  </ItemGroup>
+++
done

cat <<+++
  <Import Project="\$(VCTargetsPath)\Microsoft.Cpp.targets" />
  <ImportGroup Label="ExtensionTargets">
  </ImportGroup>
</Project>
+++
