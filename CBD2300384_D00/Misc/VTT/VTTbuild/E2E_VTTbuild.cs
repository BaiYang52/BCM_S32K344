
using System.Collections.Generic;
using System.Linq;
using System;
using System.IO;
using System.Text.RegularExpressions;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleE2E : IModule
  {
    public string Name
    {
      get { return "E2E"; }
    }

    public string DefinitionPath
    {
      get { return ""; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if ((ecuc.FindModuleConfigurationsByDefinitionPath("/MICROSAR/E2EPW").Length > 0) || (ecuc.FindModuleConfigurationsByDefinitionPath("/MICROSAR/E2EXf").Length > 0))
      {
        string filePattern = @"^E2E.*\.(h|c)$";
        string staticDir = Helper.GetModuleBSWDir(model.Directory.BSW, "E2E");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(staticDir);

        // get static files of the module and add them to compilation list
        foreach(string file in Helper.GetFiles(staticDir, filePattern))
        {
          model.Project.AddBswFile(file, Name);
        }

        // no generated files of the module exist
      }
    }
  }
}

