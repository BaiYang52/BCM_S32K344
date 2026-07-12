
using System.Collections.Generic;
using System.Linq;
using System;
using System.IO;
using System.Text.RegularExpressions;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleFvM : IModule
  {
    public string Name
    {
      get { return "FvM"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/FvM"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string filePattern = @"^FvM.*\.(h|c)$";

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
