
using System.Collections.Generic;
using System.Linq;
using System;
using System.IO;
using System.Text.RegularExpressions;

namespace Vector.VTT.VsProjectGenerator
{
  class ModulevMem_30_Blocking : IModule
  {
    public string Name
    {
      get { return "vMem_30_Blocking"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/vMem_30_Blocking/vMem"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string moduleDir = Helper.GetModuleBSWDir(model.Directory.BSW, Name);
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(moduleDir);

        /* Module files */
        foreach(string file in Helper.GetFiles(moduleDir, @"^vMem_30_Blocking.*\.(h|c)"))
        {
          model.Project.AddBswFile(file, Name);
        }

        /* GenData */
        foreach(string file in Helper.GetFiles(genDataDir, @"^vMem_30_Blocking.*\.(h|c)"))
        {
          model.Project.AddBswFile(file, Name);
        }

      }
    }
  }
}

