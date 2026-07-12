
using System.Collections.Generic;
using System.Linq;
using System;
using System.IO;
using System.Text.RegularExpressions;
using DSI4 = GenTool_CsDataServerDomAsr4.Iface;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleVTTFr : IModule
  {
    public string Name
    {
      get { return "VTTFr"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/VTT/VTTFr"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      DSI4.IEcucModuleConfigurationValues[] moduleArray;

      moduleArray = ecuc.FindModuleConfigurationsByDefinitionPath(DefinitionPath);
      if (moduleArray.Count() == 0)
      {
        //Console.WriteLine("INFO: VttFr is not part of this configuration.");
      }
      else if (moduleArray.Count() == 1)
      {


        string moduleDir = Helper.GetModuleBSWDir(model.Directory.BSW, "VttFr");
        string genDataDir = model.Directory.GenData;
        string vttCntrlDir = Helper.GetModuleBSWDir(model.Directory.BSW, "VttCntrl");

        model.Project.AddIncludeDirectory(moduleDir);

        /* Module files */
        model.Project.AddBswFile(System.IO.Path.Combine(moduleDir, "Fr.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(moduleDir, "Fr.h"), Name);
        foreach(string file in Helper.GetFiles(moduleDir, @"^Fr_.*\.(h|c)"))
        {
          model.Project.AddBswFile(file, Name);
        }

        foreach(string file in Helper.GetFiles(vttCntrlDir, @"^VttCntrl_Fr.*\.(h|c)"))
        {
          model.Project.AddBswFile(file, Name);
        }

        /* GenData */
        foreach(string file in Helper.GetFiles(genDataDir, @"^Fr_.*\.(h|c)"))
        {
          model.Project.AddBswFile(file, Name);
        }

        foreach(string file in Helper.GetFiles(genDataDir, @"^VttCntrl_Fr.*\.(h|c)"))
        {
          model.Project.AddBswFile(file, Name);
        }

        model.CANoeEmu.AddStateChangeFunction("VttFr_OnStateChange", "VttCntrl_Fr.h");

        /* Add Fr Transceiver if available */
        /* Hint: The Fr Transceiver is added to the Fr Driver, as there is no static definition path for Fr Transceiver. */
        if (System.IO.File.Exists(System.IO.Path.Combine(genDataDir, "VttCntrl_FrTrcv.c")))
        {
          model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "VttCntrl_FrTrcv.c"), Name);
        }

      }
    }
  }
}

