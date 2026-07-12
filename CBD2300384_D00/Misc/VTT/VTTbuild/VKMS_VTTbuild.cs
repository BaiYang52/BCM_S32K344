
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleVKMS : IModule
  {
    public string Name
    {
      get { return "VKMS"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/VKMS"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string vkmsDir = Helper.GetModuleBSWDir(model.Directory.BSW, "VKMS");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(vkmsDir);

        // Select all header-files in BSW directory
        foreach (string file in System.IO.Directory.EnumerateFiles(vkmsDir, "VKMS*.h", System.IO.SearchOption.TopDirectoryOnly))
        {
          model.Project.AddBswFile(file, Name);
        }

        // add only main implementation file; SWC source file only needed if SWC is used
        model.Project.AddBswFile(System.IO.Path.Combine(vkmsDir, "VKMS.c"), Name);

        // Select all header-files in GenData directory
        foreach (string file in System.IO.Directory.EnumerateFiles(genDataDir, "VKMS_*.h", System.IO.SearchOption.TopDirectoryOnly))
        {
          model.Project.AddBswFile(file, Name);
        }

        // Select all source-files in GenData directory
        foreach (string file in System.IO.Directory.EnumerateFiles(genDataDir, "VKMS_*.c", System.IO.SearchOption.TopDirectoryOnly))
        {
          model.Project.AddBswFile(file, Name);
        }
      }
    }
  }
}

