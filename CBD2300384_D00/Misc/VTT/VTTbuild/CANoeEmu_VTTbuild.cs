
using System.Collections.Generic;
using System.Linq;


namespace Vector.VTT.VsProjectGenerator
{
  class ModuleVTTCANoeEmu : IModule
  {
    public string Name
    {
      get { return "CANoeEmu"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/VTT/VTTOs"; }
    }

    private string ModuleDefinitionFilePath(IGeneratorModel model, string filename)
    {
      string CANoeEmuLibDir = Helper.GetModuleBSWDir(model.Directory.BSW, "VttCANoeEmu");
      string filepath = System.IO.Path.Combine(CANoeEmuLibDir, filename);
      return filepath;
    }

    private void Configure_VC120_Windows_x86(IGeneratorModel model)
    {
#if (VTT_MultiArch)
      string defFilePath = ModuleDefinitionFilePath(model, "CANoeEmu_VC_x86.def");
      CompilerVersion compiler = CompilerVersion.eVC120;
      model.Project.SetModuleDefinitionFile2(defFilePath,                 ProjectConfiguration.eAll,     compiler, TargetArchitecture.eWindowsX86);
      model.Project.AddLibrary2("CanoeEmu_VC120_Windows_x86_Release.lib", ProjectConfiguration.eRelease, compiler, TargetArchitecture.eWindowsX86);
      model.Project.AddLibrary2("CanoeEmu_VC120_Windows_x86_Debug.lib",   ProjectConfiguration.eDebug,   compiler, TargetArchitecture.eWindowsX86);
#else
      string defFilePath = ModuleDefinitionFilePath(model, "CANoeEmu_VC_x86.def");
      model.Project.SetModuleDefinitionFile(defFilePath);
      model.Project.AddLibrary("CanoeEmu_VC120_Windows_x86_Release.lib",  ProjectConfiguration.eRelease);
      model.Project.AddLibrary("CanoeEmu_VC120_Windows_x86_Debug.lib",    ProjectConfiguration.eDebug);
#endif
    }
    
    private void Configure_VC120_Windows_x64(IGeneratorModel model)
    {
#if (VTT_MultiArch)
      CompilerVersion compiler = CompilerVersion.eVC120;
      model.Project.AddLibrary2("CanoeEmu_VC120_Windows_x64_Release.lib", ProjectConfiguration.eRelease, compiler, TargetArchitecture.eWindowsX64);
      model.Project.AddLibrary2("CanoeEmu_VC120_Windows_x64_Debug.lib",   ProjectConfiguration.eDebug,   compiler, TargetArchitecture.eWindowsX64);
#endif
    }
    
    private void Configure_VC141_Windows_x86(IGeneratorModel model)
    {
#if (VTT_MultiArch)
      string defFilePath = ModuleDefinitionFilePath(model, "CANoeEmu_VC_x86.def");
      CompilerVersion compiler = CompilerVersion.eVC141;
      model.Project.SetModuleDefinitionFile2(defFilePath,                 ProjectConfiguration.eAll,     compiler, TargetArchitecture.eWindowsX86);
      model.Project.AddLibrary2("CanoeEmu_VC141_Windows_x86_Release.lib", ProjectConfiguration.eRelease, compiler, TargetArchitecture.eWindowsX86);
      model.Project.AddLibrary2("CanoeEmu_VC141_Windows_x86_Debug.lib",   ProjectConfiguration.eDebug,   compiler, TargetArchitecture.eWindowsX86);
#elif (VTT_VC141)
      string defFilePath = ModuleDefinitionFilePath(model, "CANoeEmu_VC_x86.def");
      model.Project.SetModuleDefinitionFile(defFilePath);
      model.Project.AddLibrary("CanoeEmu_VC141_Windows_x86_Release.lib",  ProjectConfiguration.eRelease);
      model.Project.AddLibrary("CanoeEmu_VC141_Windows_x86_Debug.lib",    ProjectConfiguration.eDebug);
#endif
    }
    
   private void Configure_VC141_Windows_x64(IGeneratorModel model)
    {
#if (VTT_MultiArch)
      CompilerVersion compiler = CompilerVersion.eVC141;
      model.Project.AddLibrary2("CanoeEmu_VC141_Windows_x64_Release.lib", ProjectConfiguration.eRelease, compiler, TargetArchitecture.eWindowsX64);
      model.Project.AddLibrary2("CanoeEmu_VC141_Windows_x64_Debug.lib",   ProjectConfiguration.eDebug,   compiler, TargetArchitecture.eWindowsX64);
#endif
    }

    private void Configure_VC142_Windows_x86(IGeneratorModel model)
    {
#if (VTT_MultiArch)
      string defFilePath = ModuleDefinitionFilePath(model, "CANoeEmu_VC_x86.def");
      CompilerVersion compiler = CompilerVersion.eVC142;
      model.Project.SetModuleDefinitionFile2(defFilePath,                 ProjectConfiguration.eAll,     compiler, TargetArchitecture.eWindowsX86);
      model.Project.AddLibrary2("CanoeEmu_VC142_Windows_x86_Release.lib", ProjectConfiguration.eRelease, compiler, TargetArchitecture.eWindowsX86);
      model.Project.AddLibrary2("CanoeEmu_VC142_Windows_x86_Debug.lib",   ProjectConfiguration.eDebug,   compiler, TargetArchitecture.eWindowsX86);
#endif
    }

    private void Configure_VC142_Windows_x64(IGeneratorModel model)
    {
#if (VTT_MultiArch)
      CompilerVersion compiler = CompilerVersion.eVC142;
      model.Project.AddLibrary2("CanoeEmu_VC142_Windows_x64_Release.lib", ProjectConfiguration.eRelease, compiler, TargetArchitecture.eWindowsX64);
      model.Project.AddLibrary2("CanoeEmu_VC142_Windows_x64_Debug.lib",   ProjectConfiguration.eDebug,   compiler, TargetArchitecture.eWindowsX64);
#endif
    }

    private void Configure_VC143_Windows_x86(IGeneratorModel model)
    {
#if (VTT_VC143)
      string defFilePath = ModuleDefinitionFilePath(model, "CANoeEmu_VC_x86.def");
      CompilerVersion compiler = CompilerVersion.eVC143;
      model.Project.SetModuleDefinitionFile2(defFilePath,                 ProjectConfiguration.eAll,     compiler, TargetArchitecture.eWindowsX86);
      model.Project.AddLibrary2("CanoeEmu_VC143_Windows_x86_Release.lib", ProjectConfiguration.eRelease, compiler, TargetArchitecture.eWindowsX86);
      model.Project.AddLibrary2("CanoeEmu_VC143_Windows_x86_Debug.lib",   ProjectConfiguration.eDebug,   compiler, TargetArchitecture.eWindowsX86);
#endif
    }

    private void Configure_VC143_Windows_x64(IGeneratorModel model)
    {
#if (VTT_VC143)
      CompilerVersion compiler = CompilerVersion.eVC143;
      model.Project.AddLibrary2("CanoeEmu_VC143_Windows_x64_Release.lib", ProjectConfiguration.eRelease, compiler, TargetArchitecture.eWindowsX64);
      model.Project.AddLibrary2("CanoeEmu_VC143_Windows_x64_Debug.lib",   ProjectConfiguration.eDebug,   compiler, TargetArchitecture.eWindowsX64);
#endif
    }

    private void Configure_Mingw81_Windows_x86(IGeneratorModel model, string CANoeEmuLibDir)
    {
#if (VTT_MultiArch)
      string defFilePath = ModuleDefinitionFilePath(model, "CANoeEmu_Mingw_x86.def");
      CompilerVersion compiler = CompilerVersion.eMingw81;
      string libPathRelease = System.IO.Path.Combine(CANoeEmuLibDir, "CanoeEmu_Mingw81_Windows_x86_Release.lib");
      string libPathDebug   = System.IO.Path.Combine(CANoeEmuLibDir, "CanoeEmu_Mingw81_Windows_x86_Debug.lib");
      if (System.IO.File.Exists(libPathRelease) && System.IO.File.Exists(libPathDebug))
      {
        model.Project.SetModuleDefinitionFile2(defFilePath,                   ProjectConfiguration.eAll,     compiler, TargetArchitecture.eWindowsX86);
        model.Project.AddLibrary2("CanoeEmu_Mingw81_Windows_x86_Release.lib", ProjectConfiguration.eRelease, compiler, TargetArchitecture.eWindowsX86);
        model.Project.AddLibrary2("CanoeEmu_Mingw81_Windows_x86_Debug.lib",   ProjectConfiguration.eDebug,   compiler, TargetArchitecture.eWindowsX86);
      }
      else
      {
        throw new System.Exception("Compiler Version: Mingw-w64 compiler for Windows-x86 is not supported by CANoeEmu library");
      }
#endif
    }

    private void Configure_Mingw122_Windows_x86(IGeneratorModel model, string CANoeEmuLibDir)
    {
#if (VTT_MinGW122)
      string defFilePath = ModuleDefinitionFilePath(model, "CANoeEmu_Mingw_x86.def");
      CompilerVersion compiler = CompilerVersion.eMingw122;
      string libPathRelease = System.IO.Path.Combine(CANoeEmuLibDir, "CANoeEmu_Mingw12_2_Windows_x86_Release.lib");
      string libPathDebug   = System.IO.Path.Combine(CANoeEmuLibDir, "CANoeEmu_Mingw12_2_Windows_x86_Debug.lib");
      if (System.IO.File.Exists(libPathRelease) && System.IO.File.Exists(libPathDebug))
      {
        model.Project.SetModuleDefinitionFile2(defFilePath,                     ProjectConfiguration.eAll,     compiler, TargetArchitecture.eWindowsX86);
        model.Project.AddLibrary2("CANoeEmu_Mingw12_2_Windows_x86_Release.lib", ProjectConfiguration.eRelease, compiler, TargetArchitecture.eWindowsX86);
        model.Project.AddLibrary2("CANoeEmu_Mingw12_2_Windows_x86_Debug.lib",   ProjectConfiguration.eDebug,   compiler, TargetArchitecture.eWindowsX86);
      }
      else
      {
        throw new System.Exception("Compiler Version: Mingw-w64 compiler for Windows-x86 is not supported by CANoeEmu library");
      }
#endif
    }

    private void Configure_Clang60_Linux_x64(IGeneratorModel model, string CANoeEmuLibDir)
    {
#if (VTT_MultiArch)
      CompilerVersion compiler = CompilerVersion.eClang60;
      string libPathRelease = System.IO.Path.Combine(CANoeEmuLibDir, "libCANoeEmu_Clang60_Linux_x64_Release.a");
      string libPathDebug   = System.IO.Path.Combine(CANoeEmuLibDir, "libCANoeEmu_Clang60_Linux_x64_Debug.a");
      if (System.IO.File.Exists(libPathRelease) && System.IO.File.Exists(libPathDebug))
      {
        model.Project.AddLibrary2("libCANoeEmu_Clang60_Linux_x64_Release.a", ProjectConfiguration.eRelease, compiler, TargetArchitecture.eLinuxX64);
        model.Project.AddLibrary2("libCANoeEmu_Clang60_Linux_x64_Debug.a",   ProjectConfiguration.eDebug,   compiler, TargetArchitecture.eLinuxX64);
      }
      else
      {
        throw new System.Exception("Compiler Version: Clang compiler for Linux-x64 is not supported by CANoeEmu library");
      }
#endif
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      string CANoeEmuLibDir = Helper.GetModuleBSWDir(model.Directory.BSW, "VttCANoeEmu");
      string genDataDir = model.Directory.GenData;

      model.Project.AddIncludeDirectory(CANoeEmuLibDir);
      model.Project.AddLibraryDirectory(CANoeEmuLibDir);

      bool useVC120 = false;
      bool useVC141 = false;
      bool useVC142 = false;
      bool useVC143 = false;
      bool useMingw81 = false;
      bool useMingw122 = false;
      bool useClang60 = false;

      switch (model.Project.Compiler)
      {
        case CompilerVersion.eVC100:
          throw new System.Exception("Compiler Version: Visual Studio 2010 is not longer supported by CANoeEmu library");

        case CompilerVersion.eVC120:
          useVC120 = true;
          break;
#if (VTT_VC141)
        case CompilerVersion.eVC141:
          useVC141 = true;
          break;
#endif
#if (VTT_VC142)
        case CompilerVersion.eVC142:
          useVC142 = true;
          break;
#endif
#if (VTT_VC143)
        case CompilerVersion.eVC143:
          useVC143 = true;
          break;
#endif
#if (VTT_MinGW122)
        case CompilerVersion.eMingw122:
          useMingw122 = true;
          break;
#endif
#if (VTT_MultiArch)
        case CompilerVersion.eMingw81:
          useMingw81 = true;
          break;
        case CompilerVersion.eClang60:
          useClang60 = true;
          break;
        case CompilerVersion.eCompilerIndependent:
          useVC120 = true;
          useVC141 = true;
          useVC142 = true;
          useVC143 = true;
          useMingw81 = true;
          useMingw122 = true;
          useClang60 = true;
          break;
#endif
        default:
          throw new System.Exception("Compiler Version: The selected compiler version is not supported by CANoeEmu library");
      }
      
      bool useWindowsX86 = false;
      bool useWindowsX64 = false;
      bool useLinuxX64 = false;

#if (VTT_MultiArch)
      switch(model.Project.Architecture)
      {
        case TargetArchitecture.eWindowsX86:
          useWindowsX86 = true;
          break;
        case TargetArchitecture.eWindowsX64:
          useWindowsX64 = true;
          break;
        case TargetArchitecture.eLinuxX64:
          useLinuxX64 = true;
          break;
        case TargetArchitecture.eArchitectureIndependent:
          useWindowsX86 = true;
          useWindowsX64 = true;
          useLinuxX64 = true;
          break;
        default:
          throw new System.Exception("Target Architecture: The selected target architecture is not supported by CANoeEmu library");
      }
#else
      useWindowsX86 = true; // Legacy mode, only Windows-x86 is supported
#endif

      if (useVC120 && useWindowsX86)
      {
        Configure_VC120_Windows_x86(model);
      }
      if (useVC120 && useWindowsX64)
      {
        Configure_VC120_Windows_x64(model);
      }
      if (useVC141 && useWindowsX86)
      {
        Configure_VC141_Windows_x86(model);
      }
      if (useVC141 && useWindowsX64)
      {
        Configure_VC141_Windows_x64(model);
      }
      if (useVC142 && useWindowsX86)
      {
        Configure_VC142_Windows_x86(model);
      }
      if (useVC142 && useWindowsX64)
      {
        Configure_VC142_Windows_x64(model);
      }
      if (useVC143 && useWindowsX86)
      {
        Configure_VC143_Windows_x86(model);
      }
      if (useVC143 && useWindowsX64)
      {
        Configure_VC143_Windows_x64(model);
      }      
      if (useMingw81 && useWindowsX86)
      {
        Configure_Mingw81_Windows_x86(model, CANoeEmuLibDir);
      }
      if (useMingw122 && useWindowsX86)
      {
        Configure_Mingw122_Windows_x86(model, CANoeEmuLibDir);
      }
      if (useClang60 && useLinuxX64)
      {
        Configure_Clang60_Linux_x64(model, CANoeEmuLibDir);
      }

      model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "CANoeEmu_cfg.c"), Name);
      model.Project.AddBswFile(System.IO.Path.Combine(CANoeEmuLibDir, "CANoeEmu_DllMain.cpp"), Name);
      model.Project.AddBswFile(System.IO.Path.Combine(CANoeEmuLibDir, "CANoeApi.h"), Name);
      model.Project.AddBswFile(System.IO.Path.Combine(CANoeEmuLibDir, "CANoeEmuProcessor.h"), Name);
      model.Project.AddBswFile(System.IO.Path.Combine(CANoeEmuLibDir, "CANoeEmuLibExport.h"), Name);
      model.Project.AddBswFile(System.IO.Path.Combine(CANoeEmuLibDir, "CANoeEmuLin.h"), Name);

      string openSutFolder = Name + "/OpenSUT";
      model.Project.AddBswFile(System.IO.Path.Combine(CANoeEmuLibDir, "opensut", "bus.h"), openSutFolder);
      model.Project.AddBswFile(System.IO.Path.Combine(CANoeEmuLibDir, "opensut", "can.h"), openSutFolder);
      model.Project.AddBswFile(System.IO.Path.Combine(CANoeEmuLibDir, "opensut", "com_item.h"), openSutFolder);
      model.Project.AddBswFile(System.IO.Path.Combine(CANoeEmuLibDir, "opensut", "eth.h"), openSutFolder);
      model.Project.AddBswFile(System.IO.Path.Combine(CANoeEmuLibDir, "opensut", "flexray.h"), openSutFolder);
      model.Project.AddBswFile(System.IO.Path.Combine(CANoeEmuLibDir, "opensut", "lin.h"), openSutFolder);
      model.Project.AddBswFile(System.IO.Path.Combine(CANoeEmuLibDir, "opensut", "log.h"), openSutFolder);
      model.Project.AddBswFile(System.IO.Path.Combine(CANoeEmuLibDir, "opensut", "sut.h"), openSutFolder);
      model.Project.AddBswFile(System.IO.Path.Combine(CANoeEmuLibDir, "opensut", "type_system.h"), openSutFolder);
      model.Project.AddBswFile(System.IO.Path.Combine(CANoeEmuLibDir, "opensut", "typedefs.h"), openSutFolder);
    }
  }
}

