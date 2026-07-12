
using System.Collections.Generic;
using System.Linq;
using System;
using System.IO;
using System.Text.RegularExpressions;

namespace Vector.VTT.VsProjectGenerator
{
  public static class Helper
  {
    public static string GetModuleBSWDir(string BSWPath, string ModuleName)
    {
      string moduletDirLegacyBased = System.IO.Path.Combine(BSWPath, ModuleName);
      string dirName = Path.GetFileName(BSWPath);

      if (dirName != "Components" && Directory.Exists(moduletDirLegacyBased))
      {
        return moduletDirLegacyBased;
      }
      else
      {
        string moduletDirComponentBased = System.IO.Path.Combine(BSWPath, "./../Components/" + ModuleName + "/Implementation");
        if (Directory.Exists(moduletDirComponentBased))
        {
          return moduletDirComponentBased;
        }
        else
        {
          string moduletDirDemoComponentBased = System.IO.Path.Combine(BSWPath, "./../DemoComponents/" + ModuleName + "/Implementation");
          return moduletDirDemoComponentBased;
        }
      }
    }
    
    public static IEnumerable<string> GetFiles(string path, string reFileNamePattern, System.IO.SearchOption sopt = System.IO.SearchOption.TopDirectoryOnly)
    {
        if(System.IO.Directory.Exists(path))
        {
          System.Text.RegularExpressions.Regex rePattern = new System.Text.RegularExpressions.Regex(reFileNamePattern);
          return System.IO.Directory.EnumerateFiles(path, "*", sopt)
                                    .Where(file => rePattern.IsMatch(Path.GetFileName(file)));
        }
        return Enumerable.Empty<string>();
    }
  }
}

