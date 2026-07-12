
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleCddOsph : IModule
  {
    public string Name
    {
      get { return "CddOsph"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/CddOsph/Cdd"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string implDir = Helper.GetModuleBSWDir(model.Directory.BSW, "CddOsph");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(implDir);

        model.Project.AddBswFile(System.IO.Path.Combine(implDir, "CddOsph.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(implDir, "CddOsph.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(implDir, "CddOsph_Cbk.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(implDir, "CddOsph_Priv.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(implDir, "CddOsph_Types.h"), Name);

        /* Data Structures depend on configuration. However, unused data structures may be added without negative impact */
        model.Project.AddBswFile(System.IO.Path.Combine(implDir, "CddOsph_DataStructure_DirectDid.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(implDir, "CddOsph_DataStructure_DirectDid.h"), Name);

        model.Project.AddBswFile(System.IO.Path.Combine(implDir, "CddOsph_DataStructure_DtcList.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(implDir, "CddOsph_DataStructure_DtcList.h"), Name);
        
        model.Project.AddBswFile(System.IO.Path.Combine(implDir, "CddOsph_DataStructure_Snapshot.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(implDir, "CddOsph_DataStructure_Snapshot.h"), Name);
        
        model.Project.AddBswFile(System.IO.Path.Combine(implDir, "CddOsph_DataStructure_Histogram.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(implDir, "CddOsph_DataStructure_Histogram.h"), Name);

        model.Project.AddBswFile(System.IO.Path.Combine(implDir, "CddOsph_DataStructure_Statistics.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(implDir, "CddOsph_DataStructure_Statistics.h"), Name);

        

        
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "CddOsph_Cfg.h"), Name);

        if (System.IO.File.Exists(System.IO.Path.Combine(genDataDir, "CddOsph_Cfg.c")))
        {
          model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "CddOsph_Cfg.c"), Name);
        }
      }
    }
  }
}

