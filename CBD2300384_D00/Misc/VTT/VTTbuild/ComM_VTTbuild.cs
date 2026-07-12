
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleComM : IModule
  {
    public string Name
    {
      get { return "ComM"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/ComM"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string commDir = Helper.GetModuleBSWDir(model.Directory.BSW, "ComM");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(commDir);
        
        
        // Select all header-files in BSW directory
        foreach (string file in System.IO.Directory.EnumerateFiles(commDir, "*.h", System.IO.SearchOption.TopDirectoryOnly))
        {
          model.Project.AddBswFile(file, Name);
        }

        // Select all header-files in BSW directory
        foreach (string file in System.IO.Directory.EnumerateFiles(commDir, "*.c", System.IO.SearchOption.TopDirectoryOnly))
        {
          model.Project.AddBswFile(file, Name);
        }

        // Select all header-files in GenData directory
        foreach (string file in System.IO.Directory.EnumerateFiles(genDataDir, "ComM*.h", System.IO.SearchOption.TopDirectoryOnly))
        {
            model.Project.AddBswFile(file, Name);
        }

        // Select all source-files in GenData directory
        foreach (string file in System.IO.Directory.EnumerateFiles(genDataDir, "ComM*.c", System.IO.SearchOption.TopDirectoryOnly))
        {
            model.Project.AddBswFile(file, Name);
        }
      }
    }
  }
}

