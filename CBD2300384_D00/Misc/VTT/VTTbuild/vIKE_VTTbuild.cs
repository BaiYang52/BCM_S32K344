
using System.Collections.Generic;
using System.Linq;
using System;
using System.IO;
using System.Text.RegularExpressions;

namespace Vector.VTT.VsProjectGenerator
{
  class ModulevIKE : IModule
  {
    public string Name
    {
      get { return "vIKE"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/vIKE"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string vIKEDir = Helper.GetModuleBSWDir(model.Directory.BSW, "vIKE");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(vIKEDir);

        /* vIKE files */
        foreach(string file in Helper.GetFiles(vIKEDir, @"^vIKE.*\.(h|c)"))
        {
          model.Project.AddBswFile(file, Name);
        }

        /* GenData */
        foreach(string file in Helper.GetFiles(genDataDir, @"^vIKE.*\.(h|c)"))
        {
          model.Project.AddBswFile(file, Name);
        }
      }
    }
  }
}