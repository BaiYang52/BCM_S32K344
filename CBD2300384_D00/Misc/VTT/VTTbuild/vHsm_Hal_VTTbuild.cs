using System.Collections.Generic;
using System.Linq;
using System;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleVTTvHsm_Hal : IModule
  {
    public string Name
    {
      get { return "vHsm_Hal"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/vHsm"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      /* Check if vHsm configuration */
      if (ecuc.FindModuleConfigurationsByDefinitionPath(DefinitionPath).Length > 0)
      {
        string moduleDir = Helper.GetModuleBSWDir(model.Directory.BSW, "VTT" + Name);
        string modulePrefix = Name+"_";
        string genDataDir = model.Directory.GenData;

        if (System.IO.Directory.Exists(moduleDir))
        {
          model.Project.AddIncludeDirectory(moduleDir);

          // Select all header-files in genDataDir directory
          foreach (string file in System.IO.Directory.EnumerateFiles(genDataDir, modulePrefix+"*.h", System.IO.SearchOption.TopDirectoryOnly))
          {
            model.Project.AddBswFile(file, Name);
          }
          // Select all C-files in BSW directory
          foreach (string file in System.IO.Directory.EnumerateFiles(genDataDir, modulePrefix+"*.c", System.IO.SearchOption.TopDirectoryOnly))
          {
            model.Project.AddBswFile(file, Name);
          }

          // Select all header-files in BSW directory
          foreach (string file in System.IO.Directory.EnumerateFiles(moduleDir, "*.h", System.IO.SearchOption.TopDirectoryOnly))
          {
            model.Project.AddBswFile(file, Name);
          }

          // Select all C-files in BSW directory
          foreach (string file in System.IO.Directory.EnumerateFiles(moduleDir, "*.c", System.IO.SearchOption.TopDirectoryOnly))
          {
            model.Project.AddBswFile(file, Name);
          }

          model.CANoeEmu.AddStateChangeFunction("VttvHsm_Hal_OnStateChange", "vHsm_Hal.h");
        }
      }
    }
  }
}

