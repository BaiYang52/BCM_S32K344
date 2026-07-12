
using System.Collections.Generic;
using System.Linq;
using System;
using System.IO;
using System.Text.RegularExpressions;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleCanIf : IModule
  {
    public string Name
    {
      get { return "CanIf"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/CanIf"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string moduleDir = Helper.GetModuleBSWDir(model.Directory.BSW, Name);
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(moduleDir);

        /* Module files */
        foreach(string file in Helper.GetFiles(moduleDir, @"^CanIf.*\.(h|c)"))
        {
          model.Project.AddBswFile(file, Name);
        }

        /* GenData */
        foreach(string file in Helper.GetFiles(genDataDir, @"^CanIf.*\.(h|c)"))
        {
          model.Project.AddBswFile(file, Name);
        }
      }
    }
  }
}

