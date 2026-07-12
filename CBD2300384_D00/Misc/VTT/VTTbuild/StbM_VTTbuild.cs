
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleStbM : IModule
  {
    public string Name
    {
      get { return "StbM"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/StbM"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string stbmDir = Helper.GetModuleBSWDir(model.Directory.BSW, "StbM");
        string genDataDir = model.Directory.GenData;
        string sourceDir = model.Directory.Source;

        model.Project.AddIncludeDirectory(stbmDir);

        // Number of Gendata Files depend on the use case, therefor a pattern matching is used:
        foreach (string file in System.IO.Directory.EnumerateFiles(stbmDir, "StbM*.h", System.IO.SearchOption.TopDirectoryOnly))
        {
          model.Project.AddBswFile(file, Name);
        }

        foreach (string file in System.IO.Directory.EnumerateFiles(stbmDir, "StbM*.c", System.IO.SearchOption.TopDirectoryOnly))
        {
          model.Project.AddBswFile(file, Name);
        }
        foreach (string file in System.IO.Directory.EnumerateFiles(genDataDir, "StbM*.h", System.IO.SearchOption.TopDirectoryOnly))
        {
          model.Project.AddBswFile(file, Name);
        }
        foreach (string file in System.IO.Directory.EnumerateFiles(genDataDir, "StbM*.c", System.IO.SearchOption.TopDirectoryOnly))
        {
          model.Project.AddBswFile(file, Name);
        }
      }
    }
  }
}

