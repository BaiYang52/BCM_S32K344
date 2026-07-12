using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text.RegularExpressions;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleLinTp : IModule
  {
    public string Name
    {
      get { return "LinTp"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/LinTp"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string filePattern = @"^LinTp.*\.(h|c)$";
        string genDataDir = model.Directory.GenData;

        string linifDir = Helper.GetModuleBSWDir(model.Directory.BSW, "LinIf");
        model.Project.AddBswFile(System.IO.Path.Combine(linifDir, "LinTp_Types.h"), Name);

        // get generated files of the module and add them to compilation list
        foreach(string file in Helper.GetFiles(genDataDir, filePattern))
        {
          model.Project.AddBswFile(file, Name);
        }
      }
    }
  }
}
