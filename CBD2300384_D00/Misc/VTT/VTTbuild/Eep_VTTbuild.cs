
using System.Collections.Generic;
using System.Linq;
using System;

using DSI4 = GenTool_CsDataServerDomAsr4.Iface;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleEep : IModule
  {
    public string Name
    {
      /* not used anymore! */
      get { return ""; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/VTT/VTTEep"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      DSI4.IEcucModuleConfigurationValues[] moduleArray;

      moduleArray = ecuc.FindModuleConfigurationsByDefinitionPath(DefinitionPath);

      if (moduleArray.Count() == 0)
      {
        //Console.WriteLine("INFO: Eep is not part of this configuration.");
      }
      else if (moduleArray.Count() == 1)
      {
        string eepDir = Helper.GetModuleBSWDir(model.Directory.BSW, "VttEep");
        string vttCntrlDir = Helper.GetModuleBSWDir(model.Directory.BSW, "VttCntrl");
        string genDataDir = model.Directory.GenData;
        string moduleName = moduleArray[0].ShortName;

        Console.WriteLine("INFO: Eep is part of this configuration and added to VS Solution.");

        model.Project.AddIncludeDirectory(eepDir);

        model.Project.AddBswFile(System.IO.Path.Combine(eepDir, "Eep.c"), moduleName);
        model.Project.AddBswFile(System.IO.Path.Combine(eepDir, "Eep.h"), moduleName);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Eep_Cfg.h"), moduleName);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Eep_PBcfg.c"), moduleName);
        model.Project.AddBswFile(System.IO.Path.Combine(vttCntrlDir, "VttCntrl_Eep.c"), moduleName);
        model.Project.AddBswFile(System.IO.Path.Combine(vttCntrlDir, "VttCntrl_Eep.h"), moduleName);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "VttCntrl_Eep_Cfg.c"), moduleName);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "VttCntrl_Eep_Cfg.h"), moduleName);

        model.CANoeEmu.AddStateChangeFunction("VttEep_OnStateChange", "VttCntrl_Eep.h");
      }
      else
      {

        uint count = 0;
        Console.WriteLine("ERROR: Invalid Number ({0}) of configurations with definition for Eep!", moduleArray.Count());
        foreach (DSI4.IEcucModuleConfigurationValues element in moduleArray)
        {
          count++;
          Console.WriteLine(" - Name of element number {0}: {1}", count, element.ShortName);
        }

      }
    }
  }
}

