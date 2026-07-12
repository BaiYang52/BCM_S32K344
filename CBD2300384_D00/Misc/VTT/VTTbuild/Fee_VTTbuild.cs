
using System.Collections.Generic;
using System.Linq;
using System;
using System.IO;
using System.Text.RegularExpressions;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleFee : IModule
  {
    public string Name
    {
      get { return "Fee"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/Fee"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string feeDir = Helper.GetModuleBSWDir(model.Directory.BSW, "Fee");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(feeDir);

        /* Fee files */
        foreach(string file in Helper.GetFiles(feeDir, @"^Fee.*\.(h|c)"))
        {
          model.Project.AddBswFile(file, Name);
        }

        /* GenData */
        foreach(string file in Helper.GetFiles(genDataDir, @"^Fee.*\.(h|c)"))
        {
          model.Project.AddBswFile(file, Name);
        }

      }
    }
  }
}

