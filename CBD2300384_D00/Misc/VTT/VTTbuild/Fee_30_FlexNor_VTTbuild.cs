
using System.Collections.Generic;
using System.Linq;
using System;
using DSI4 = GenTool_CsDataServerDomAsr4.Iface;

namespace Vector.VTT.VsProjectGenerator
{
    class ModuleFee_30_FlexNor : IModule
  {
    string myName = "Fee_30_FlexNor";
    public string Name
    {
      /* not used anymore! */
      get { return ""; }
    }

    public string DefinitionPath
    {
        get { return "/MICROSAR/" + myName + "/Fee"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      DSI4.IEcucModuleConfigurationValues[] moduleArray;

      moduleArray = ecuc.FindModuleConfigurationsByDefinitionPath(DefinitionPath);

      if (moduleArray.Count() == 0)
      {
        Console.WriteLine("INFO: " + myName + " is not part of this configuration.");
      }
      else if (moduleArray.Count() == 1)
      {
        string FeeDir = Helper.GetModuleBSWDir(model.Directory.BSW, myName);
        string genDataDir = model.Directory.GenData;
        string moduleName = moduleArray[0].ShortName;

        Console.WriteLine("INFO: " + myName + "(" + moduleName + ") is part of this configuration and added to VS Solution.");

        model.Project.AddIncludeDirectory(FeeDir);
        if (System.IO.Directory.Exists(genDataDir))
        {
          foreach (string file in System.IO.Directory.EnumerateFiles(genDataDir, "*.h", System.IO.SearchOption.TopDirectoryOnly))
          {
            if (System.IO.Path.GetFileName(file).StartsWith(myName))
            {
              model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, file), moduleName);
            }
          }
          foreach (string file in System.IO.Directory.EnumerateFiles(genDataDir, "*.c", System.IO.SearchOption.TopDirectoryOnly))
          {
            if (System.IO.Path.GetFileName(file).StartsWith(myName))
            {
              model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, file), moduleName);
            }
          }
        }

        // Select all header-files in BSW directory except templates
        foreach (string file in System.IO.Directory.EnumerateFiles(FeeDir, myName + "*.h", System.IO.SearchOption.TopDirectoryOnly))
        {
          model.Project.AddBswFile(file, moduleName);
        }

        // Select all C-files in BSW directory except templates
        foreach (string file in System.IO.Directory.EnumerateFiles(FeeDir, myName + "*.c", System.IO.SearchOption.TopDirectoryOnly))
        {
          model.Project.AddBswFile(file, moduleName);
        }

      }
      else
      {

        uint count = 0;
        Console.WriteLine("ERROR: Invalid Number ({0}) of configurations with definition for " + myName + "!", moduleArray.Count());
        foreach (DSI4.IEcucModuleConfigurationValues element in moduleArray)
        {
          count++;
          Console.WriteLine(" - Name of element number {0}: {1}", count, element.ShortName);
        }
      }
    }
  }
}

