
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModulevHsm_Custom : IModule
  {
    public string Name
    {
      get { return "vHsm_Custom"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/vHsm_Custom/Crypto"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string moduleDir = Helper.GetModuleBSWDir(model.Directory.BSW, Name);
        string modulePrefix = Name+"_";
        string genDataDir = model.Directory.GenData;

        // Select all header-files in genDataDir directory
        foreach (string file in System.IO.Directory.EnumerateFiles(genDataDir, modulePrefix+"Cfg.h", System.IO.SearchOption.TopDirectoryOnly))
        {
          model.Project.AddBswFile(file, Name);
        }
        // Select all C-files in BSW directory
        foreach (string file in System.IO.Directory.EnumerateFiles(genDataDir, modulePrefix+"Cfg.c", System.IO.SearchOption.TopDirectoryOnly))
        {
          model.Project.AddBswFile(file, Name);
        }

        if (System.IO.Directory.Exists(moduleDir))
        {
          model.Project.AddIncludeDirectory(moduleDir);

          // Select all header-files in BSW directory
          foreach (string file in System.IO.Directory.EnumerateFiles(moduleDir, "vHsm*.h", System.IO.SearchOption.TopDirectoryOnly))
          {
            model.Project.AddBswFile(file, Name);
          }

          foreach (string file in System.IO.Directory.EnumerateFiles(moduleDir, "Crypto*.h", System.IO.SearchOption.TopDirectoryOnly))
          {
            model.Project.AddBswFile(file, Name);
          }

          // Select all C-files in BSW directory
          foreach (string file in System.IO.Directory.EnumerateFiles(moduleDir, "vHsm*.c", System.IO.SearchOption.TopDirectoryOnly))
          {
            model.Project.AddBswFile(file, Name);
          }

          foreach (string file in System.IO.Directory.EnumerateFiles(moduleDir, "Crypto*.c", System.IO.SearchOption.TopDirectoryOnly))
          {
            model.Project.AddBswFile(file, Name);
          }
        }
      }
    }
  }
}

