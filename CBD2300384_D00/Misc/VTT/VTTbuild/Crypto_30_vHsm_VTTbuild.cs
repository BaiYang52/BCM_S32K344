
using System.Collections.Generic;
using System.Linq;

using DSI4 = GenTool_CsDataServerDomAsr4.Iface;
namespace Vector.VTT.VsProjectGenerator
{
  class ModuleCrypto_30_vHsm : IModule
  {
    public string Name
    {
      get { return "Crypto_30_vHsm"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/Crypto_30_vHsm/Crypto"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      DSI4.IEcucModuleConfigurationValues[] moduleArray;

      moduleArray = ecuc.FindModuleConfigurationsByDefinitionPath(DefinitionPath);
      if (moduleArray.Count() == 1)
      {
        string moduleDir = Helper.GetModuleBSWDir(model.Directory.BSW, Name);
        string modulePrefix = Name + "_";
        string genDataDir = model.Directory.GenData;
        
        string vttCntrlDir = Helper.GetModuleBSWDir(model.Directory.BSW, "VttCntrl");



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
          
          model.Project.AddBswFile(System.IO.Path.Combine(vttCntrlDir, "VttCntrl_Crypto_vHsm.c"), Name);
          model.Project.AddBswFile(System.IO.Path.Combine(vttCntrlDir, "VttCntrl_Crypto_vHsm.h"), Name);

          model.CANoeEmu.AddStateChangeFunction("VttCntrl_Crypto_vHsm_OnStateChange", "VttCntrl_Crypto_vHsm.h");
        }
      }
    }
  }
}

