
using System.Collections.Generic;
using System.Linq;
using System;
using System.IO;
using System.Text.RegularExpressions;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleIdsM : IModule
  {
    public string Name
    {
      get { return "IdsM"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/IdsM"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string moduleDir = Helper.GetModuleBSWDir(model.Directory.BSW, Name);
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(moduleDir);

        /* Module files */
        foreach(string file in Helper.GetFiles(moduleDir, @"^IdsM.*\.(h|c)"))
        {
          model.Project.AddBswFile(file, Name);
        }

        /* GenData */
        foreach(string file in Helper.GetFiles(genDataDir, @"^IdsM.*\.(h|c)"))
        {
          model.Project.AddBswFile(file, Name);
        }

      }
    }
  }
}

