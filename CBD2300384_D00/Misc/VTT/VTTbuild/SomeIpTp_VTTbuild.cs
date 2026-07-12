
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleSomeIpTp : IModule
  {
    public string Name
    {
      get { return "SomeIpTp"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/SomeIpTp"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string soadDir = Helper.GetModuleBSWDir(model.Directory.BSW, "SomeIpTp");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(soadDir);

        model.Project.AddBswFile(System.IO.Path.Combine(soadDir, "SomeIpTp.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(soadDir, "SomeIpTp.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(soadDir, "SomeIpTp_Cbk.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(soadDir, "SomeIpTp_Priv.h"), Name);
        
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "SomeIpTp_Cfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "SomeIpTp_Lcfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "SomeIpTp_Lcfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "SomeIpTp_PBcfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "SomeIpTp_PBcfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "SomeIpTp_Types.h"), Name);

      }
    }
  }
}

