
using System.Collections.Generic;
using System.Linq;
using System;
using System.IO;
using System.Text.RegularExpressions;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleCom : IModule
  {
    public string Name
    {
      get { return "Com"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/Com"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string filePattern = @"^Com_.*\.(h|c)$";

        string staticDir = Helper.GetModuleBSWDir(model.Directory.BSW, Name);
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(staticDir);

        // get static files of the module and add them to compilation list
        model.Project.AddBswFile(System.IO.Path.Combine(staticDir, "Com.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(staticDir, "Com.h"), Name);
        foreach(string file in Helper.GetFiles(staticDir, filePattern))
        {
          model.Project.AddBswFile(file, Name);
        }

        // get generated files of the module and add them to compilation list
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Appl_Cbk.h"), Name);
        foreach(string file in Helper.GetFiles(genDataDir, filePattern))
        {
          model.Project.AddBswFile(file, Name);
        }
      }
    }
  }
}

