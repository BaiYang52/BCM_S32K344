
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleVTTMcu : IModule
  {
    public string Name
    {
      get { return "VTTMcu"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/VTT/VTTMcu"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string mcuDir = Helper.GetModuleBSWDir(model.Directory.BSW, "VttMcu");
        string vttCntrlDir = Helper.GetModuleBSWDir(model.Directory.BSW, "VttCntrl");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(mcuDir);

        model.Project.AddBswFile(System.IO.Path.Combine(mcuDir, "Mcu.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(mcuDir, "Mcu.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Mcu_PBcfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Mcu_Cfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(vttCntrlDir, "VttCntrl_Mcu.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(vttCntrlDir, "VttCntrl_Mcu.h"), Name);

        model.CANoeEmu.AddStateChangeFunction("VttMcu_OnStateChange", "VttCntrl_Mcu.h");
      }
    }
  }
}

