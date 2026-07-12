
using System.Collections.Generic;
using System.Linq;
using System;
using System.IO;
using System.Text.RegularExpressions;

using DSI4 = GenTool_CsDataServerDomAsr4.Iface;
namespace Vector.VTT.VsProjectGenerator
{
  class ModulevMem_30_vRpcProxy : IModule
  {
    public string Name
    {
      get { return "vMem_30_vRpcProxy"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/vMem_30_vRpcProxy/vMem"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      DSI4.IEcucModuleConfigurationValues[] moduleArray;
      
      moduleArray = ecuc.FindModuleConfigurationsByDefinitionPath(DefinitionPath);
      if (moduleArray.Count() == 1)
      {
        string moduleDir = Helper.GetModuleBSWDir(model.Directory.BSW, Name);
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(moduleDir);

        /* Module files */
        foreach(string file in Helper.GetFiles(moduleDir, @"^vMem_30_vRpcProxy.*\.(h|c)"))
        {
          model.Project.AddBswFile(file, Name);
        }

        /* GenData */
        foreach(string file in Helper.GetFiles(genDataDir, @"^vMem_30_vRpcProxy.*\.(h|c)"))
        {
          model.Project.AddBswFile(file, Name);
        }

      }
    }
  }
}

