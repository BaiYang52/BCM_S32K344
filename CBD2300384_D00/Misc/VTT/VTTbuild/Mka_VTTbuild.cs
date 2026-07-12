
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleMka : IModule
  {
    public string Name
    {
      get { return "Mka"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/Mka"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string mkaDir = Helper.GetModuleBSWDir(model.Directory.BSW, "Mka");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(mkaDir);


        // Select all header-files in BSW directory
        foreach (string file in System.IO.Directory.EnumerateFiles(mkaDir, "Mka*.h", System.IO.SearchOption.TopDirectoryOnly))
        {
          model.Project.AddBswFile(file, Name);
        }

        // Select all header-files in BSW directory
        foreach (string file in System.IO.Directory.EnumerateFiles(mkaDir, "Mka*.c", System.IO.SearchOption.TopDirectoryOnly))
        {
          model.Project.AddBswFile(file, Name);
        }

        // Select all header-files in GenData directory
        foreach (string file in System.IO.Directory.EnumerateFiles(genDataDir, "Mka_*.h", System.IO.SearchOption.TopDirectoryOnly))
        {
            model.Project.AddBswFile(file, Name);
        }

        // Select all source-files in GenData directory
        foreach (string file in System.IO.Directory.EnumerateFiles(genDataDir, "Mka_*.c", System.IO.SearchOption.TopDirectoryOnly))
        {
            model.Project.AddBswFile(file, Name);
        }
      }
    }
  }
}

