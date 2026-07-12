
using System.Collections.Generic;
using System.Linq;
using System;

using DSI4 = GenTool_CsDataServerDomAsr4.Iface;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleCrypto_30_KeyM : IModule
  {
    public string Name
    {
      get { return "Crypto_30_KeyM"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/Crypto_30_KeyM/Crypto"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      DSI4.IEcucModuleConfigurationValues[] moduleArray = ecuc.FindModuleConfigurationsByDefinitionPath(DefinitionPath);

      if (moduleArray.Count() == 1)
      {
        string moduleDir = Helper.GetModuleBSWDir(model.Directory.BSW, Name);
        string modulePrefix = Name+"_";
        string genDataDir = model.Directory.GenData;

        // Select all header-files in genDataDir directory
        foreach (string file in System.IO.Directory.EnumerateFiles(genDataDir, modulePrefix+"Cfg.h", System.IO.SearchOption.TopDirectoryOnly))
        {
          model.Project.AddBswFile(file, Name);
        }
        // Select all C-files in BSW directory
        foreach (string file in System.IO.Directory.EnumerateFiles(genDataDir, modulePrefix+"Cfg.c", System.IO.SearchOption.TopDirectoryOnly))
        {
          model.Project.AddBswFile(file, Name);
        }

        if (System.IO.Directory.Exists(moduleDir))
        {
          model.Project.AddIncludeDirectory(moduleDir);

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
        }
      }
    }
  }
}

