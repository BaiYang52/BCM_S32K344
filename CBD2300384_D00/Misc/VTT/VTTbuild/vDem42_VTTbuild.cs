
using System.Collections.Generic;
using System.Linq;
using System;
using System.IO;
using System.Text.RegularExpressions;

namespace Vector.VTT.VsProjectGenerator
{
  class ModulevDem42 : IModule
  {
    public string Name
    {
      get { return "vDem42"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/vDem42"; }
    }
    
    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string vDem42Dir = Helper.GetModuleBSWDir(model.Directory.BSW, "vDem42");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(vDem42Dir);

        /* vDem42 files */
        foreach(string file in Helper.GetFiles(vDem42Dir, @"^vDem42.*\.(h|c)"))
        {
          model.Project.AddBswFile(file, Name);
        }

        /* GenData */
        foreach(string file in Helper.GetFiles(genDataDir, @"^vDem42.*\.(h|c)"))
        {
          model.Project.AddBswFile(file, Name);
        }
      }
    }
  }
}

