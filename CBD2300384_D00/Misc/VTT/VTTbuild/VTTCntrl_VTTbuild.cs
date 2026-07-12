
using System.Collections.Generic;
using System.Linq;

using DSI4 = GenTool_CsDataServerDomAsr4.Iface;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleVTTCntrl : IModule
  {
    public string Name
    {
      get { return "VTTCntrl"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/VTT/VTTCntrl"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
          string vttDir = Helper.GetModuleBSWDir(model.Directory.BSW, "VttCntrl");
          string genDataDir = model.Directory.GenData;

          model.Project.AddIncludeDirectory(vttDir);

          model.Project.AddBswFile(System.IO.Path.Combine(vttDir, "VttCntrl_Det.c"), Name);
          model.Project.AddBswFile(System.IO.Path.Combine(vttDir, "VttCntrl_Det.h"), Name);
          model.Project.AddBswFile(System.IO.Path.Combine(vttDir, "VttCntrl_Base.c"), Name);
          model.Project.AddBswFile(System.IO.Path.Combine(vttDir, "VttCntrl_Base.h"), Name);
          model.Project.AddBswFile(System.IO.Path.Combine(vttDir, "VttCntrl_ModuleIncludes.h"), Name);
          model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "VttCntrl.h"), Name);
          model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "VttCntrl_Cfg.h"), Name);
          model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "VttCntrl_Cfg.c"), Name);

          model.CANoeEmu.AddStateChangeFunction("VttCntrl_Base_OnStateChange", "VttCntrl_Base.h");
          model.CANoeEmu.AddStateChangeFunction("VttCntrl_Det_OnStateChange", "VttCntrl_Det.h");

          /* User-defined CANoe system variables. */
          {
              var containerList = ecuc.EcucModuleConfigurationValues.GetContainersByDefinition("VttSystemVariables/VttSysVar").OfType<DSI4.IARObject>();

              if (containerList.Count() > 0)
              {
                  /* at least one sysvar has been configured */
                  model.Project.AddBswFile(System.IO.Path.Combine(vttDir, "VttCntrl_SysVar.c"), Name);
                  model.Project.AddBswFile(System.IO.Path.Combine(vttDir, "VttCntrl_SysVar.h"), Name);
                  model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "VttCntrl_SysVar_Cfg.c"), Name);
                  model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "VttCntrl_SysVar_Cfg.h"), Name);
                  model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, "VttCntrl_SysVar_Cbk.h"), Name);
                  model.CANoeEmu.AddStateChangeFunction("VttCntrl_SysVar_OnStateChange", "VttCntrl_SysVar.h");
              }
          }

          /* Add EntryPoint and the corresponding Header File */
          /* Default value is the by AUTOSAR specified Entry Point */
          string EntryPoint = "EcuM_Init";
          string HeaderFile = "EcuM.h";
          {
              var containerList = ecuc.EcucModuleConfigurationValues.GetContainersByDefinition("VttGeneral").OfType<DSI4.IARObject>();
              if (containerList.Count() == 1)
              {
                  DSI4.IEcucContainerValue containerValue = (DSI4.IEcucContainerValue)containerList.First();
                  DSI4.IEcucParameterValue vttEntryPointParamValue = containerValue.GetParameterValueByDefinition("/MICROSAR/VTT/VTTCntrl/VttGeneral/VttEntryPointName", false);
                  DSI4.IEcucTextualParamValue vttEntryPointTextualParamValue = (DSI4.IEcucTextualParamValue)vttEntryPointParamValue;
                  DSI4.IEcucParameterValue vttHeaderFileParamValue = containerValue.GetParameterValueByDefinition("/MICROSAR/VTT/VTTCntrl/VttGeneral/VttHeaderFile", false);
                  DSI4.IEcucTextualParamValue vttHeaderFileTextualParamValue = (DSI4.IEcucTextualParamValue)vttHeaderFileParamValue;

                  if (vttEntryPointTextualParamValue != null)
                  {
                      EntryPoint = vttEntryPointTextualParamValue.Value;
                  }

                  if (vttHeaderFileTextualParamValue != null)
                  {
                      HeaderFile = vttHeaderFileTextualParamValue.Value;
                  }

                  string vttCommonDir = Helper.GetModuleBSWDir(model.Directory.BSW, "Vtt_Common");
                  var vttCommonFiles = containerValue.GetParameterValuesByDefinition("/MICROSAR/VTT/VTTCntrl/VttGeneral/VttCommonFile");
                  foreach (DSI4.IEcucTextualParamValue commonFileParam in vttCommonFiles)
                  {
                      string srcPath = System.IO.Path.Combine(vttCommonDir, commonFileParam.Value);
                      string fileName = System.IO.Path.GetFileName(commonFileParam.Value);
                      string dstPath = System.IO.Path.Combine(genDataDir, fileName);
                      System.IO.File.Copy(srcPath, dstPath, true);

                      model.Project.AddBswFile(System.IO.Path.Combine(genDataDir, fileName), "Common");
                  }
              }
          }
          model.CANoeEmu.SetMainFunction(EntryPoint, HeaderFile);
      }
    }
  }
}
