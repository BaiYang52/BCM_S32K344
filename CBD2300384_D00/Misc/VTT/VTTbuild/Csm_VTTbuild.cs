
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleCsm : IModule
  {
    public string Name
    {
      get { return "Csm"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/Csm"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string csmDir = Helper.GetModuleBSWDir(model.Directory.BSW, "Csm");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(csmDir);
        
        
        // Select all header-files in BSW directory
        foreach (string file in System.IO.Directory.EnumerateFiles(csmDir, "*.h", System.IO.SearchOption.TopDirectoryOnly))
        {
          model.Project.AddBswFile(file, Name);
        }

        // Select all header-files in BSW directory
        foreach (string file in System.IO.Directory.EnumerateFiles(csmDir, "*.c", System.IO.SearchOption.TopDirectoryOnly))
        {
          model.Project.AddBswFile(file, Name);
        }

        // Select all header-files in GenData directory
        foreach (string file in System.IO.Directory.EnumerateFiles(genDataDir, "Csm*.h", System.IO.SearchOption.TopDirectoryOnly))
        {
            model.Project.AddBswFile(file, Name);
        }

        // Select all source-files in GenData directory
        foreach (string file in System.IO.Directory.EnumerateFiles(genDataDir, "Csm*.c", System.IO.SearchOption.TopDirectoryOnly))
        {
            model.Project.AddBswFile(file, Name);
        }
      }
    }
  }
}

