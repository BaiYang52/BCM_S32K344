
using System.Collections.Generic;
using System.Linq;
using System;

using DSI4 = GenTool_CsDataServerDomAsr4.Iface;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleFblHal : IModule
  {
    public string Name
    {
      /* Only used for Folder in VS Solution */
      get { return "FblHal"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/FblHal"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string genDataDir = model.Directory.GenData;

        /* GenData */
        foreach (string file in Helper.GetFiles(genDataDir, @"^FblHal_.*\.(h|c)"))
        {
          model.Project.AddBswFile(file, Name);
        }
      }
    }
  }
}

