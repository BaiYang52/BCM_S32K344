using System.Collections.Generic;
using System.Linq;
using System;

using DSI4 = GenTool_CsDataServerDomAsr4.Iface;
namespace Vector.VTT.VsProjectGenerator
{
  class ModuleVTTEthSwt : IModule
  {
    public string Name
    {
      /* Only used for Folder in VS Solution */
      get { return "VTTEthSwt"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/VTT/VTTEthSwt"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      DSI4.IEcucModuleConfigurationValues[] moduleArray;

      moduleArray = ecuc.FindModuleConfigurationsByDefinitionPath(DefinitionPath);

      if (moduleArray.Count() == 0)
      {
        //Console.WriteLine("INFO: VTTEthSwt is not part of this configuration.");
      }
      else if (moduleArray.Count() == 1)
      {
        string ethSwtDir = Helper.GetModuleBSWDir(model.Directory.BSW, "VttEthSwt_30_Vtt");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(ethSwtDir);

        // Select all header-files in BSW directory
        foreach (string file in System.IO.Directory.EnumerateFiles(ethSwtDir, "*.h", System.IO.SearchOption.TopDirectoryOnly))
        {
          model.Project.AddBswFile(file, Name);
        }
        foreach (string file in System.IO.Directory.EnumerateFiles(ethSwtDir, "*.c", System.IO.SearchOption.TopDirectoryOnly))
        {
          model.Project.AddBswFile(file, Name);
        }

        foreach (string file in System.IO.Directory.EnumerateFiles(genDataDir, "EthSwt_30_*.h", System.IO.SearchOption.TopDirectoryOnly))
        {
          model.Project.AddBswFile(file, Name);
        }

        foreach (string file in System.IO.Directory.EnumerateFiles(genDataDir, "EthSwt_30_*.c", System.IO.SearchOption.TopDirectoryOnly))
        {
          model.Project.AddBswFile(file, Name);
        }

        foreach (string file in System.IO.Directory.EnumerateFiles(genDataDir, "SchM_EthSwt_30_Vtt.h", System.IO.SearchOption.TopDirectoryOnly))
        {
          model.Project.AddBswFile(file, Name);
        }
      }
      else
      {
        uint count = 0;
        Console.WriteLine("ERROR: Invalid Number ({0}) of configurations with definition for VTTEthSwt!", moduleArray.Count());
        foreach (DSI4.IEcucModuleConfigurationValues element in moduleArray)
        {
          count++;
          Console.WriteLine(" - Name of element number {0}: {1}", count, element.ShortName);
        }
      }
    }
  }
}

