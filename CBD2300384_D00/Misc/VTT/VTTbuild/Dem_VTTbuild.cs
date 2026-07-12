
using System.Collections.Generic;
using System.Linq;
using System;
using System.IO;
using System.Text.RegularExpressions;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleDem : IModule
  {
    public string Name
    {
      get { return "Dem"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/Dem"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string demDir = Helper.GetModuleBSWDir(model.Directory.BSW, "Dem");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(demDir);

        /* Dem files */
        foreach(string file in Helper.GetFiles(demDir, @"^Dem.*\.(h|c)"))
        {
          model.Project.AddBswFile(file, Name);
        }

        /* GenData */
        foreach(string file in Helper.GetFiles(genDataDir, @"^Dem.*\.(h|c)"))
        {
          model.Project.AddBswFile(file, Name);
        }
      }
    }
  }
}

