
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleCry : IModule
  {
    public string Name
    {
      get { return "Cry"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/Cry"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string cryDir = Helper.GetModuleBSWDir(model.Directory.BSW, "Cry");
        string cryptoDir = Helper.GetModuleBSWDir(model.Directory.BSW, "Crypto");
        string cryptoCvDir = Helper.GetModuleBSWDir(model.Directory.BSW, "SecMod");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(cryDir);

        // Select all header-files in BSW directory except templates
        foreach (string file in System.IO.Directory.EnumerateFiles(cryDir, "Cry*.h", System.IO.SearchOption.TopDirectoryOnly))
        {
          model.Project.AddBswFile(file, Name);
        }

        // Select all C-files in BSW directory except templates
        foreach (string file in System.IO.Directory.EnumerateFiles(cryDir, "Cry*.c", System.IO.SearchOption.TopDirectoryOnly))
        {
          model.Project.AddBswFile(file, Name);
        }

        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Cry_Cfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Cry_Cfg.h"), Name);

        if (System.IO.File.Exists(System.IO.Path.Combine(genDataDir, "Cry_PBcfg.c")))
         model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "Cry_PBcfg.c"), Name);
        
        if (System.IO.Directory.Exists(cryptoDir))
        {
          model.Project.AddIncludeDirectory(cryptoDir);

          // Select all header-files in BSW directory
          foreach (string file in System.IO.Directory.EnumerateFiles(cryptoDir, "*.h", System.IO.SearchOption.TopDirectoryOnly))
          {
            model.Project.AddBswFile(file, Name);
          }

          // Select all C-files in BSW directory
          foreach (string file in System.IO.Directory.EnumerateFiles(cryptoDir, "*.c", System.IO.SearchOption.TopDirectoryOnly))
          {
            model.Project.AddBswFile(file, Name);
          }
        } 
        else if (System.IO.Directory.Exists(cryptoCvDir))
        {
          model.Project.AddIncludeDirectory(cryptoCvDir);

          // Select all header-files in BSW directory
          foreach (string file in System.IO.Directory.EnumerateFiles(cryptoCvDir, @"^Sec*.h", System.IO.SearchOption.TopDirectoryOnly))
          {
            model.Project.AddBswFile(file, Name);
          }

          // Select all C-files in BSW directory
          foreach (string file in System.IO.Directory.EnumerateFiles(cryptoCvDir, @"^Sec*.c", System.IO.SearchOption.TopDirectoryOnly))
          {
            model.Project.AddBswFile(file, Name);
          }
        }
      }
    }
  }
}

