
using System.Collections.Generic;
using System.Linq;
using System;
using System.IO;
using System.Text.RegularExpressions;
using DSI4 = GenTool_CsDataServerDomAsr4.Iface;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleVTTEth : IModule
  {
    public string Name
    {
      get { return "VTTEth"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/VTT/VTTEth"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      DSI4.IEcucModuleConfigurationValues[] moduleArray;

      moduleArray = ecuc.FindModuleConfigurationsByDefinitionPath(DefinitionPath);
      if (moduleArray.Count() == 0)
      {
        //Console.WriteLine("INFO: VttCan is not part of this configuration.");
      }
      else if (moduleArray.Count() == 1)
      {
        string moduleDir = Helper.GetModuleBSWDir(model.Directory.BSW, "VttEth");
        string genDataDir = model.Directory.GenData;
        string vttCntrlDir = Helper.GetModuleBSWDir(model.Directory.BSW, "VttCntrl");

        model.Project.AddIncludeDirectory(moduleDir);

        // get static files and add them
        foreach (string file in Helper.GetFiles(moduleDir, ".*.(h|c)"))
        {
          model.Project.AddBswFile(file, Name);
          System.Console.WriteLine(file);
        }

        // get generated files and add them
        foreach (string file in Helper.GetFiles(genDataDir, "Eth(_30_.*)?.(h|c)"))
        {
          model.Project.AddBswFile(file, Name);
        }

        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "VttCntrl_Eth_Cfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "VttCntrl_Eth_Cfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(vttCntrlDir, "VttCntrl_Eth.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(vttCntrlDir, "VttCntrl_Eth.h"), Name);

        model.CANoeEmu.AddStateChangeFunction("VttEth_OnStateChange", "VttCntrl_Eth.h");
      }
    }
  }
}
