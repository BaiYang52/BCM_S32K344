
using System.Collections.Generic;
using System.Linq;
using System;

using DSI4 = GenTool_CsDataServerDomAsr4.Iface;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleCrypto_30_Vtt : IModule
  {
    public string Name
    {
      /* not used anymore! */
      get { return ""; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/VTT/VTTCrypto"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      DSI4.IEcucModuleConfigurationValues[] moduleArray;

      moduleArray = ecuc.FindModuleConfigurationsByDefinitionPath(DefinitionPath);

      if (moduleArray.Count() == 0 || ecuc.FindModuleConfigurationsByDefinitionPath("/MICROSAR/vHsm").Length > 0)
      {
        /* Module not configured or it is a vHsm configuration. */
      }
      else if (moduleArray.Count() == 1)
      {
        string cryptoDir = Helper.GetModuleBSWDir(model.Directory.BSW, "VttCrypto_30_Vtt");

        if (System.IO.Directory.Exists(cryptoDir))
        {
          string vttCntrlDir = Helper.GetModuleBSWDir(model.Directory.BSW, "VttCntrl");
          string genDataDir = model.Directory.GenData;
          string moduleName = moduleArray[0].ShortName;

          Console.WriteLine("INFO: Crypto_30_Vtt is part of this configuration and added to VS Solution.");
        
          model.Project.AddIncludeDirectory(cryptoDir);
          if (System.IO.Directory.Exists(genDataDir))
          {
            foreach (string file in System.IO.Directory.EnumerateFiles(genDataDir, "*.h", System.IO.SearchOption.TopDirectoryOnly))
            {
              if (System.IO.Path.GetFileName(file).StartsWith("Crypto_30_Vtt"))
              {
                model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, file), moduleName);
              }
            }
            foreach (string file in System.IO.Directory.EnumerateFiles(genDataDir, "*.c", System.IO.SearchOption.TopDirectoryOnly))
            {
              if (System.IO.Path.GetFileName(file).StartsWith("Crypto_30_Vtt"))
              {
                model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, file), moduleName);
              }
            }
          }
        
          // Select all header-files in BSW directory except templates
          foreach (string file in System.IO.Directory.EnumerateFiles(cryptoDir, "Crypto_30_Vtt*.h", System.IO.SearchOption.TopDirectoryOnly))
          {
            model.Project.AddBswFile(file, moduleName);
          }

          // Select all C-files in BSW directory except templates
          foreach (string file in System.IO.Directory.EnumerateFiles(cryptoDir, "Crypto_30_Vtt*.c", System.IO.SearchOption.TopDirectoryOnly))
          {
            model.Project.AddBswFile(file, moduleName);
          }
        
          model.Project.AddBswFile(System.IO.Path.Combine(vttCntrlDir, "VttCntrl_Crypto.c"), moduleName);
          model.Project.AddBswFile(System.IO.Path.Combine(vttCntrlDir, "VttCntrl_Crypto.h"), moduleName);
        }
      }
      else
      {

        uint count = 0;
        Console.WriteLine("ERROR: Invalid Number ({0}) of configurations with definition for Crypto_30_Vtt!", moduleArray.Count());
        foreach (DSI4.IEcucModuleConfigurationValues element in moduleArray)
        {
          count++;
          Console.WriteLine(" - Name of element number {0}: {1}", count, element.ShortName);  
        }
        
      }
    }
  }
}

