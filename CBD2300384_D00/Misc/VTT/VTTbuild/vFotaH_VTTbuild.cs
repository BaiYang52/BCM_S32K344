
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModulevFotaH : IModule
  {
    public string Name
    {
      get { return "vFotaH"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/vFotaH"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string vFotaHDir = Helper.GetModuleBSWDir(model.Directory.BSW, "vFotaH");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(vFotaHDir);

        foreach (string file in System.IO.Directory.EnumerateFiles(vFotaHDir, "vFotaH*.h", System.IO.SearchOption.TopDirectoryOnly))
        {
          model.Project.AddBswFile(file, Name);
        }

        // Select all C-files in BSW directory except templates
        foreach (string file in System.IO.Directory.EnumerateFiles(vFotaHDir, "vFotaH*.c", System.IO.SearchOption.TopDirectoryOnly))
        {
          model.Project.AddBswFile(file, Name);
        }

        foreach (string file in System.IO.Directory.EnumerateFiles(genDataDir, "vFotaH*.h", System.IO.SearchOption.TopDirectoryOnly))
        {
          model.Project.AddBswFile(file, Name);
        }

        // Select all C-files in BSW directory except templates
        foreach (string file in System.IO.Directory.EnumerateFiles(genDataDir, "vFotaH*.c", System.IO.SearchOption.TopDirectoryOnly))
        {
          model.Project.AddBswFile(file, Name);
        }

      }
    }
  }
}

