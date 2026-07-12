using System.Collections.Generic;
using System.Linq;
using System;
using System.IO;
using System.Text.RegularExpressions;
using DSI4 = GenTool_CsDataServerDomAsr4.Iface;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleVTTLin : IModule
  {
    public string Name
    {
      get { return "VTTLin"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/VTT/VTTLin"; }
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
        string filePattern = @"^Lin_.*\.(h|c)$";

        string moduleDir = Helper.GetModuleBSWDir(model.Directory.BSW, "VttLin");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(moduleDir);

        // get static files of the module and add them to compilation list
        model.Project.AddBswFile(System.IO.Path.Combine(moduleDir, "Lin.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(moduleDir, "Lin.h"), Name);
        foreach(string file in Helper.GetFiles(moduleDir, filePattern))
        {
          model.Project.AddBswFile(file, Name);
        }

        // get generated files of the module and add them to compilation list
        foreach(string file in Helper.GetFiles(genDataDir, filePattern))
        {
          model.Project.AddBswFile(file, Name);
        }

        // get generated files of LinVtt* (Lin slave) and add them to compilation list
        string filePattern_LinVtt = @"^LinVtt.*\.(h|c)$";
        foreach(string file in Helper.GetFiles(genDataDir, filePattern_LinVtt))
        {
          model.Project.AddBswFile(file, Name);
        }

        string filePattern_VttCntrl_Lin = @"^VttCntrl_Lin.*\.(h|c)$";
        // get generated files of VttCntrl_Lin* and add them to compilation list
        foreach(string file in Helper.GetFiles(genDataDir, filePattern_VttCntrl_Lin))
        {
          model.Project.AddBswFile(file, Name);
        }
        model.CANoeEmu.AddStateChangeFunction("VttLin_OnStateChange", "Lin.h");
      }
    }
  }
}
