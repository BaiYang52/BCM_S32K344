
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleFblFlash : IModule
  {
    public string Name
    {
      get { return "Flash"; }
    }

    public string DefinitionPath
    {
      get { return ""; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      string flashDir = Helper.GetModuleBSWDir(model.Directory.BSW, "Flash");

      model.Project.AddIncludeDirectory(flashDir);

      if (System.IO.Directory.Exists(flashDir))
      {   
        model.Project.AddIncludeDirectory(flashDir);

        model.Project.AddBswFile(System.IO.Path.Combine(flashDir, "FlashRom.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(flashDir, "FlashRom.h"), Name);
      }
    }
  }
}

