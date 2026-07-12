
using System.Collections.Generic;
using System.Linq;
using System;
using System.IO;
using System.Text.RegularExpressions;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleWdgM : IModule
  {
    public string Name
    {
      get { return "WdgM"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/WdgM"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string filePattern = @"^WdgM.*\.(h|c)$";

        string staticDir = Helper.GetModuleBSWDir(model.Directory.BSW, Name);
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(staticDir);

        // get static files of the module and add them to compilation list
        foreach(string file in Helper.GetFiles(staticDir, filePattern))
        {
          model.Project.AddBswFile(file, Name);
        }

        // get generated files of the module and add them to compilation list
        foreach(string file in Helper.GetFiles(genDataDir, filePattern))
        {
          model.Project.AddBswFile(file, Name);
        }
      }
    }
  }
}

