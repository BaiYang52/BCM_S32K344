
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleCddOvtp : IModule
  {
    public string Name
    {
      get { return "CddOvtp"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/CddOvtp/Cdd"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string implDir = Helper.GetModuleBSWDir(model.Directory.BSW, "CddOvtp");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(implDir);

        model.Project.AddBswFile(System.IO.Path.Combine(implDir, "CddOvtp.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(implDir, "CddOvtp.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(implDir, "CddOvtp_Cbk.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(implDir, "CddOvtp_Priv.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(implDir, "CddOvtp_Types.h"), Name);
        
        
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "CddOvtp_Cfg.h"), Name);

        if (System.IO.File.Exists(System.IO.Path.Combine(genDataDir, "CddOvtp_Cfg.c")))
        {
          model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "CddOvtp_Cfg.c"), Name);
        }
      }
    }
  }
}

