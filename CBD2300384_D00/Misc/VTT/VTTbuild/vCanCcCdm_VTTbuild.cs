
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModulevCanCcCdm : IModule
  {
    public string Name
    {
      get { return "vCanCcCdm"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/vCanCcCdm/Cdd"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string vCanCcCdmDir = Helper.GetModuleBSWDir(model.Directory.BSW, "vCanCcCdm");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(vCanCcCdmDir);

        model.Project.AddBswFile(System.IO.Path.Combine(vCanCcCdmDir, "vCanCcCdm.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(vCanCcCdmDir, "vCanCcCdm.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(vCanCcCdmDir, "vCanCcCdm_Types.h"), Name);
        

        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "vCanCcCdm_Cfg.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "vCanCcCdm_Cfg.h"), Name);

      }
    }
  }
}

