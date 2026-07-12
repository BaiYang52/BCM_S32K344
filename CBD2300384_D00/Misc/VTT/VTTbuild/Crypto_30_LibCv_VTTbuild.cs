
using System.Collections.Generic;
using System.Linq;
using System;

using DSI4 = GenTool_CsDataServerDomAsr4.Iface;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleCrypto_30_LibCv : IModule
  {
    public string Name
    {
      /* Only used for Folder in VS Solution */
      get { return "Crypto_30_LibCv"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/Crypto_30_LibCv/Crypto"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      DSI4.IEcucModuleConfigurationValues[] moduleArray;

      moduleArray = ecuc.FindModuleConfigurationsByDefinitionPath(DefinitionPath);

      if (moduleArray.Count() == 0)
      {
        //Console.WriteLine("INFO: Crypto_30_LibCv is not part of this configuration.");
      }
      else if (moduleArray.Count() == 1)
      {
        string cryptoDir = Helper.GetModuleBSWDir(model.Directory.BSW, "Crypto_30_LibCv");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(cryptoDir);

        /* Module files */
        foreach (string file in Helper.GetFiles(cryptoDir, @"^Crypto_30_LibCv.*\.(h|c)"))
        {
          model.Project.AddBswFile(file, Name);
        }

        /* GenData */
        foreach (string file in Helper.GetFiles(genDataDir, @"^Crypto_30_LibCv.*\.(h|c)"))
        {
          model.Project.AddBswFile(file, Name);
        }
      }
      else
      {
        uint count = 0;
        Console.WriteLine("ERROR: Invalid Number ({0}) of configurations with definition for Crypto_30_LibCv!", moduleArray.Count());
        foreach (DSI4.IEcucModuleConfigurationValues element in moduleArray)
        {
          count++;
          Console.WriteLine(" - Name of element number {0}: {1}", count, element.ShortName);
        }
      }
    }
  }
}

