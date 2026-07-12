
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModulevDtcH : IModule
  {
    public string Name
    {
      get { return "vDtcH"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/vDtcH"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string vDtcHDir = Helper.GetModuleBSWDir(model.Directory.BSW, "vDtcH");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(vDtcHDir);

        model.Project.AddBswFile(System.IO.Path.Combine(vDtcHDir, "vDtcH.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(vDtcHDir, "vDtcH.h"), Name);

        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "vDtcH_Cfg.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "vDtcH_Lcfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "vDtcH_Lcfg.h"), Name);
      }
    }
  }
}

