
using System.Collections.Generic;
using System.Linq;
using System;

using DSI4 = GenTool_CsDataServerDomAsr4.Iface;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleWrapNv : IModule
  {
    public string Name
    {
      /* Only used for Folder in VS Solution */
      get { return "WrapNv"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/WrapNv"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string moduleDir = Helper.GetModuleBSWDir(model.Directory.BSW, "WrapNv");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(moduleDir);

        /* Module files */
        foreach (string file in Helper.GetFiles(moduleDir, @"^WrapNv.*\.(h|c)"))
        {
          model.Project.AddBswFile(file, Name);
        }

        /* GenData */
        foreach (string file in Helper.GetFiles(genDataDir, @"^WrapNv_.*\.(h|c)"))
        {
          model.Project.AddBswFile(file, Name);
        }
      }
    }
  }
}

