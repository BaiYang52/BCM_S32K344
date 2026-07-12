
using System.Collections.Generic;
using System.Linq;
using System;
using System.Text.RegularExpressions;

using DSI4 = GenTool_CsDataServerDomAsr4.Iface;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleFbl : IModule
  {
    public string Name
    {
      /* Only used for Folder in VS Solution */
      get { return "Fbl"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/Fbl"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string moduleDir = Helper.GetModuleBSWDir(model.Directory.BSW, "Fbl");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(moduleDir);

        /* Module files */
        /* Generally used module files */
        foreach (string file in Helper.GetFiles(moduleDir, @"^fbl_[^b].*\.(h|c)"))
        {
          Regex fblExcludeRegex = new Regex(@"fbl_(main|mem|ramio|diag.*)\.(h|c)");
          Match m;

          if (!(m = fblExcludeRegex.Match(file)).Success)
          {
            model.Project.AddBswFile(file, Name);
          }
        }

        /* FBL specific module files */
        if (System.IO.File.Exists(System.IO.Path.Combine(genDataDir, "FblCw_Cfg.h")))
        {
          /* Existence of FblCw_Cfg.h indicates FBL use case. Add also FBL files except fbl_bspatch.c (see below) */
          foreach (string file in Helper.GetFiles(moduleDir, @"^fbl_(main|mem|ramio|diag.*)\.(h|c)"))
          {
            model.Project.AddBswFile(file, Name);
          }
        }

        if (System.IO.File.Exists(System.IO.Path.Combine(genDataDir, "fbl_bspatch_cfg.h")))
        {
          model.Project.AddBswFile(System.IO.Path.Combine(moduleDir, "fbl_bspatch.c"), Name);
          model.Project.AddBswFile(System.IO.Path.Combine(moduleDir, "fbl_bspatch.h"), Name);
          model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "fbl_bspatch_cfg.h"), Name);
        }
        model.Project.AddBswFile(System.IO.Path.Combine(moduleDir, "applvect.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(moduleDir, "iotypes.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(moduleDir, "v_ver.h"), Name);

        /* GenData */
        foreach (string file in Helper.GetFiles(genDataDir, @"^Fbl_.*\.(h|c)"))
        {
          model.Project.AddBswFile(file, Name);
        }
      }
    }
  }
}

