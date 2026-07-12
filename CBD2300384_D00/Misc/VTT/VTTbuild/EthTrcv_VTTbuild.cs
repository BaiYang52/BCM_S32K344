using System.Collections.Generic;
using System.Linq;
using System;

using DSI4 = GenTool_CsDataServerDomAsr4.Iface;
namespace Vector.VTT.VsProjectGenerator
{
  class ModuleVTTEthTrcv : IModule
  {
    public string Name
    {
      /* Only used for Folder in VS Solution */
      get { return "VTTEthTrcv"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/VTT/VTTEthTrcv"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      DSI4.IEcucModuleConfigurationValues[] moduleArray;

      moduleArray = ecuc.FindModuleConfigurationsByDefinitionPath(DefinitionPath);

      if (moduleArray.Count() == 0)
      {
        //Console.WriteLine("INFO: VTTEthTrcv is not part of this configuration.");
      }
      else if (moduleArray.Count() == 1)
      {
        string ethtrcvDir = Helper.GetModuleBSWDir(model.Directory.BSW, "VttEthTrcv_30_Vtt");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(ethtrcvDir);

        // Select all header-files in BSW directory
        foreach (string file in System.IO.Directory.EnumerateFiles(ethtrcvDir, "*.h", System.IO.SearchOption.TopDirectoryOnly))
        {
          model.Project.AddBswFile(file, Name);
        }
        foreach (string file in System.IO.Directory.EnumerateFiles(ethtrcvDir, "*.c", System.IO.SearchOption.TopDirectoryOnly))
        {
          model.Project.AddBswFile(file, Name);
        }

        foreach (string file in System.IO.Directory.EnumerateFiles(genDataDir, "EthTrcv_30_*.h", System.IO.SearchOption.TopDirectoryOnly))
        {
          model.Project.AddBswFile(file, Name);
        }

        foreach (string file in System.IO.Directory.EnumerateFiles(genDataDir, "EthTrcv_30_*.c", System.IO.SearchOption.TopDirectoryOnly))
        {
          model.Project.AddBswFile(file, Name);
        }
      }
      else
      {
        uint count = 0;
        Console.WriteLine("ERROR: Invalid Number ({0}) of configurations with definition for VTTEthTrcv!", moduleArray.Count());
        foreach (DSI4.IEcucModuleConfigurationValues element in moduleArray)
        {
          count++;
          Console.WriteLine(" - Name of element number {0}: {1}", count, element.ShortName);
        }
      }
    }
  }
}

