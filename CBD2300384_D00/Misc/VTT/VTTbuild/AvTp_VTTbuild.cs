
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleAvTp : IModule
  {
    public string Name
    {
      get { return "AvTp"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/AvTp"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string avtpDir = Helper.GetModuleBSWDir(model.Directory.BSW, "AvTp");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(avtpDir);

        foreach (string file in System.IO.Directory.EnumerateFiles(avtpDir, "AvTp*.h", System.IO.SearchOption.TopDirectoryOnly))
        {
          model.Project.AddBswFile(file, Name);
        }

        // Select all C-files in BSW directory
        foreach (string file in System.IO.Directory.EnumerateFiles(avtpDir, "AvTp*.c", System.IO.SearchOption.TopDirectoryOnly))
        {
          model.Project.AddBswFile(file, Name);
        }

        // Select all header-files in GenData directory
        foreach (string file in System.IO.Directory.EnumerateFiles(genDataDir, "AvTp_*.h", System.IO.SearchOption.TopDirectoryOnly))
        {
          model.Project.AddBswFile(file, Name);
        }

        // Select all source-files in GenData directory
        foreach (string file in System.IO.Directory.EnumerateFiles(genDataDir, "AvTp_*.c", System.IO.SearchOption.TopDirectoryOnly))
        {
          model.Project.AddBswFile(file, Name);
        }
      }
    }
  }
}

