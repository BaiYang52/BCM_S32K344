
using System.Collections.Generic;
using System.Linq;
using System;
using System.IO;
using System.Text.RegularExpressions;
using DSI4 = GenTool_CsDataServerDomAsr4.Iface;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleVTTCan : IModule
  {
    public string Name
    {
      get { return "VTTCan"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/VTT/VTTCan"; }
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
        string moduleDir = Helper.GetModuleBSWDir(model.Directory.BSW, "VttCan");
        string genDataDir = model.Directory.GenData;
        string vttCntrlDir = Helper.GetModuleBSWDir(model.Directory.BSW, "VttCntrl");

        model.Project.AddIncludeDirectory(moduleDir);

        /* Module files */
        model.Project.AddBswFile(System.IO.Path.Combine(moduleDir, "Can.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(moduleDir, "Can.h"), Name);
        foreach(string file in Helper.GetFiles(moduleDir, @"^Can_.*\.(h|c)"))
        {
          model.Project.AddBswFile(file, Name);
        }

        foreach(string file in Helper.GetFiles(vttCntrlDir, @"^VttCntrl_Can.*\.(h|c)"))
        {
          model.Project.AddBswFile(file, Name);
        }

        /* GenData */
        foreach(string file in Helper.GetFiles(genDataDir, @"^Can_.*\.(h|c)"))
        {
          model.Project.AddBswFile(file, Name);
        }

        foreach(string file in Helper.GetFiles(genDataDir, @"^VttCntrl_Can.*\.(h|c)"))
        {
          model.Project.AddBswFile(file, Name);
        }

        model.CANoeEmu.AddStateChangeFunction("VttCan_OnStateChange", "VttCntrl_Can.h");
      }
    }
  }
}

