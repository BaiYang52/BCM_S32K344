
using System.Collections.Generic;
using System.Linq;
using System;
using System.IO;
using System.Text.RegularExpressions;
using DSI4 = GenTool_CsDataServerDomAsr4.Iface;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleDcmAddOn : IModule
  {
    public string Name
    {
      get { return "DcmAddOn"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/DcmAddOn/Cdd"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      DSI4.IEcucModuleConfigurationValues[] moduleArray;

      moduleArray = ecuc.FindModuleConfigurationsByDefinitionPath(DefinitionPath);
      if (moduleArray.Count() == 0)
      {
      }
      else if (moduleArray.Count() == 1)
      {
        string moduleDir = Helper.GetModuleBSWDir(model.Directory.BSW, "DcmAddOn");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(moduleDir);
        
        
        // Select all header-files in BSW directory
        foreach (string file in System.IO.Directory.EnumerateFiles(moduleDir, "*.h", System.IO.SearchOption.TopDirectoryOnly))
        {
          model.Project.AddBswFile(file, Name);
        }

        // Select all header-files in BSW directory
        foreach (string file in System.IO.Directory.EnumerateFiles(moduleDir, "*.c", System.IO.SearchOption.TopDirectoryOnly))
        {
          model.Project.AddBswFile(file, Name);
        }

        // Select all header-files in GenData directory
        foreach (string file in System.IO.Directory.EnumerateFiles(genDataDir, "DcmAddOn*.h", System.IO.SearchOption.TopDirectoryOnly))
        {
            model.Project.AddBswFile(file, Name);
        }

        // Select all source-files in GenData directory
        foreach (string file in System.IO.Directory.EnumerateFiles(genDataDir, "DcmAddOn*.c", System.IO.SearchOption.TopDirectoryOnly))
        {
            model.Project.AddBswFile(file, Name);
        }
      }
    }
  }
}

