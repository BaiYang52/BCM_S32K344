
using System.Collections.Generic;
using System.Linq;
using System;
using System.IO;
using System.Text.RegularExpressions;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleVTTvMem : IModule
  {
    public string Name
    {
      get { return "VTTvMem"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/VTT/VTTvMem"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string filePatternvMem = @"^vMem_30_Vtt.*\.(h|c)$";
        string filePatternvMem_Gen = @"^vMem(_30_.*).*\.(h|c)$";
        string filePatternvCntrlvMem = @"^VttCntrl_vMem.*\.(h|c)$";

        string moduleDir = Helper.GetModuleBSWDir(model.Directory.BSW, "VttvMem_30_Vtt");
        string genDataDir = model.Directory.GenData;
        string vttCntrlDir = Helper.GetModuleBSWDir(model.Directory.BSW, "VttCntrl");
        model.Project.AddIncludeDirectory(moduleDir);
        model.Project.AddIncludeDirectory(vttCntrlDir);

        /* vMem_30_Vtt Module files */
        foreach(string file in Helper.GetFiles(moduleDir, filePatternvMem))
        {
          model.Project.AddBswFile(file, Name);
        }

        /* vttCntrl_vMem impl files */
        foreach(string file in Helper.GetFiles(vttCntrlDir, filePatternvCntrlvMem))
        {
          model.Project.AddBswFile(file, Name);
        }

        /* get generated files of the module and add them to compilation list */
        foreach(string file in Helper.GetFiles(genDataDir, filePatternvMem_Gen))
        {
          model.Project.AddBswFile(file, Name);
        }

        foreach(string file in Helper.GetFiles(genDataDir, filePatternvCntrlvMem))
        {
          model.Project.AddBswFile(file, Name);
        }

        model.CANoeEmu.AddStateChangeFunction("VttvMem_OnStateChange", "VttCntrl_vMem.h");
      }
    }

  }
}

