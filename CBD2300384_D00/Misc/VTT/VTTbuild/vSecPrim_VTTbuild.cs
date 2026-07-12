
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModulevSecPrim : IModule
  {
    public string Name
    {
      get { return "vSecPrim"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/vSecPrim"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string vSecPrimDir = Helper.GetModuleBSWDir(model.Directory.BSW, "vSecPrim");
        string genDataDir = model.Directory.GenData;


        // Select all header-files in genDataDir directory
        foreach (string file in System.IO.Directory.EnumerateFiles(genDataDir, "vSecPrim_*.h", System.IO.SearchOption.TopDirectoryOnly))
        {
          model.Project.AddBswFile(file, Name);
        }
        // Select all C-files in BSW directory
        foreach (string file in System.IO.Directory.EnumerateFiles(genDataDir, "vSecPrim_*.c", System.IO.SearchOption.TopDirectoryOnly))
        {
          model.Project.AddBswFile(file, Name);
        }

        if (System.IO.Directory.Exists(vSecPrimDir))
        {
          model.Project.AddIncludeDirectory(vSecPrimDir);

          // Select all header-files in BSW directory
          foreach (string file in System.IO.Directory.EnumerateFiles(vSecPrimDir, "*.h", System.IO.SearchOption.TopDirectoryOnly))
          {
            model.Project.AddBswFile(file, Name);
          }

          // Select all C-files in BSW directory
          foreach (string file in System.IO.Directory.EnumerateFiles(vSecPrimDir, "*.c", System.IO.SearchOption.TopDirectoryOnly))
          {
            model.Project.AddBswFile(file, Name);
          }
        }
      }
    }
  }
}

