
using System.Collections.Generic;
using System.Linq;
using System;

using DSI4 = GenTool_CsDataServerDomAsr4.Iface;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleFblSb : IModule
  {
    public string Name
    {
      /* Only used for Folder in VS Solution */
      get { return "FblSb"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/FblSb"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string moduleDir = Helper.GetModuleBSWDir(model.Directory.BSW, "FblSecBoot");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(moduleDir);

        /* Module files */
        foreach (string file in Helper.GetFiles(moduleDir, @"^fbl_secboot.*\.(h|c)"))
        {
          model.Project.AddBswFile(file, Name);
        }

        /* GenData */
        foreach (string file in Helper.GetFiles(genDataDir, @"^FblSb_.*\.(h|c)"))
        {
          model.Project.AddBswFile(file, Name);
        }
      }
    }
  }
}

