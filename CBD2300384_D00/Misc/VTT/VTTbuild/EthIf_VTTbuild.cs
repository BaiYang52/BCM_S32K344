
using System.Collections.Generic;
using System.Linq;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleEthIf : IModule
  {
    public string Name
    {
      get { return "EthIf"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/EthIf"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string ethIfStaticFilePattern = @"^EthIf.*\.(h|c)$";
        string ipBaseStaticFilePattern = @"^IpBase.*\.(h|c)$";
        string ethIfGenDataFilePattern = @"^EthIf_.*\.(h|c)$";

        // EthIf directory
        string ethifDir = Helper.GetModuleBSWDir(model.Directory.BSW, "EthIf");
        // IpBase directory
        string ipbaseDir = Helper.GetModuleBSWDir(model.Directory.BSW, "IpBase");
        // GenData directory
        string genDataDir = model.Directory.GenData;

        // add both IpBase and EthIf directory to include path
        model.Project.AddIncludeDirectory(ethifDir);
        model.Project.AddIncludeDirectory(ipbaseDir);

        // get static files of EthIf and add them to compilation list
        foreach(string file in Helper.GetFiles(ethifDir, ethIfStaticFilePattern, System.IO.SearchOption.AllDirectories))
        {
          model.Project.AddIncludeDirectory(System.IO.Path.GetDirectoryName(file));
          model.Project.AddBswFile(file, "EthIf");
          System.Console.WriteLine(file);
        }

        // get static files of IpBase and add them to compilation list
        foreach(string file in Helper.GetFiles(ipbaseDir, ipBaseStaticFilePattern))
        {
          model.Project.AddBswFile(file, "IpBase");
          System.Console.WriteLine(file);
        }

        // get generated files of EthIf and add them to compilation list
        foreach(string file in Helper.GetFiles(genDataDir, ethIfGenDataFilePattern))
        {
          model.Project.AddBswFile(file, "EthIf");
          System.Console.WriteLine(file);
        }
      }
    }
  }
}

