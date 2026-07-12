
using System.Collections.Generic;
using System.Linq;
using System;
using System.IO;
using System.Text.RegularExpressions;
using DSI4 = GenTool_CsDataServerDomAsr4.Iface;

namespace Vector.VTT.VsProjectGenerator
{
  class ModuleXcp : IModule
  {
    public string Name
    {
      get { return "Xcp"; }
    }

    public string DefinitionPath
    {
      get { return "/MICROSAR/Xcp"; }
    }

    public void Configure(IEcucProvider ecuc, IGeneratorModel model)
    {
      if (ecuc.IsConfigured)
      {
        string xcpGenFilePattern = @"^Xcp_.*\.(h|c)$";
        string canXcpGenFilePattern = @"^CanXcp_.*\.(h|c)$";
        string frXcpGenFilePattern = @"^FrXcp_.*\.(h|c)$";
        string tcpIpXcpGenFilePattern = @"^TcpIpXcp_.*\.(h|c)$";

        string xcpDir = Helper.GetModuleBSWDir(model.Directory.BSW, "Xcp");
        string genDataDir = model.Directory.GenData;

        model.Project.AddIncludeDirectory(xcpDir);

        string canxcpDir = Helper.GetModuleBSWDir(model.Directory.BSW, "CanXcp");
        string frxcpDir = Helper.GetModuleBSWDir(model.Directory.BSW, "FrXcp");
        string tcpIpXcpDir = Helper.GetModuleBSWDir(model.Directory.BSW, "TcpIpXcp");

        bool xcpOnCanEnabled = false;
        bool XcpOnFrEnabled = false;
        bool XcpOnEthernetEnabled = false;
        var containerList = ecuc.EcucModuleConfigurationValues.GetContainersByDefinition("XcpGeneral").OfType<DSI4.IARObject>();
        if (containerList.Count() == 1)
        {
          DSI4.IEcucContainerValue containerValue = (DSI4.IEcucContainerValue)containerList.First();
          DSI4.IEcucParameterValue parameterValue = containerValue.GetParameterValueByDefinition("/MICROSAR/Xcp/XcpGeneral/XcpOnCanEnabled", true);
          if (parameterValue != null)          {            DSI4.IEcucNumericalParamValue numValue = (DSI4.IEcucNumericalParamValue)parameterValue;            string value = numValue.Value.Value;            if ( "true" == value )            {              xcpOnCanEnabled = true;            }          }        }
        if (containerList.Count() == 1)        {          DSI4.IEcucContainerValue containerValue = (DSI4.IEcucContainerValue)containerList.First();          DSI4.IEcucParameterValue parameterValue = containerValue.GetParameterValueByDefinition("/MICROSAR/Xcp/XcpGeneral/XcpOnFlexRayEnabled", true);
          if (parameterValue != null)          {            DSI4.IEcucNumericalParamValue numValue = (DSI4.IEcucNumericalParamValue)parameterValue;            string value = numValue.Value.Value;            if ( "true" == value )            {              XcpOnFrEnabled = true;            }          }        }
        if (containerList.Count() == 1)        {          DSI4.IEcucContainerValue containerValue = (DSI4.IEcucContainerValue)containerList.First();          DSI4.IEcucParameterValue parameterValue = containerValue.GetParameterValueByDefinition("/MICROSAR/Xcp/XcpGeneral/XcpOnEthernetEnabled", true);
          if (parameterValue != null)          {            DSI4.IEcucNumericalParamValue numValue = (DSI4.IEcucNumericalParamValue)parameterValue;            string value = numValue.Value.Value;            if ( "true" == value )            {              XcpOnEthernetEnabled = true;            }          }        }
        if( true == xcpOnCanEnabled )
        {
            model.Project.AddIncludeDirectory(canxcpDir);
            model.Project.AddBswFile(System.IO.Path.Combine(canxcpDir, "CanXcp.c"), "CanXcp");
            model.Project.AddBswFile(System.IO.Path.Combine(canxcpDir, "CanXcp.h"), "CanXcp");
            model.Project.AddBswFile(System.IO.Path.Combine(canxcpDir, "CanXcp_Types.h"), "CanXcp");
            // Add all generated CanXcp files to VS-solution
            foreach(string file in Helper.GetFiles(genDataDir, canXcpGenFilePattern))
            {
              model.Project.AddBswFile(file, "CanXcp");
            }
        }
        if( true == XcpOnFrEnabled )
        {
          model.Project.AddIncludeDirectory(frxcpDir);
          model.Project.AddBswFile(System.IO.Path.Combine(frxcpDir, "FrXcp.c"), "FrXcp");
          model.Project.AddBswFile(System.IO.Path.Combine(frxcpDir, "FrXcp.h"), "FrXcp");
          model.Project.AddBswFile(System.IO.Path.Combine(frxcpDir, "FrXcp_Cbk.h"), "FrXcp");
          // Add all generated FrXcp files to VS-solution
          foreach(string file in Helper.GetFiles(genDataDir, frXcpGenFilePattern))
          {
            model.Project.AddBswFile(file, Name);
          }
        }
        if( true == XcpOnEthernetEnabled )
        {          model.Project.AddIncludeDirectory(tcpIpXcpDir);          foreach (string file in System.IO.Directory.EnumerateFiles(tcpIpXcpDir, "*.h", System.IO.SearchOption.TopDirectoryOnly))          {
            model.Project.AddBswFile(file, Name);
          }
          foreach (string file in System.IO.Directory.EnumerateFiles(tcpIpXcpDir, "*.c", System.IO.SearchOption.TopDirectoryOnly))          {            model.Project.AddBswFile(file, Name);
          }
          foreach(string file in Helper.GetFiles(genDataDir, tcpIpXcpGenFilePattern))
          {
            model.Project.AddBswFile(file, Name);
          }
        }
        model.Project.AddBswFile(System.IO.Path.Combine(xcpDir, "Xcp.c"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(xcpDir, "Xcp.h"), Name);
        model.Project.AddBswFile(System.IO.Path.Combine(xcpDir, "Xcp_Types.h"), Name);

        // Add all generated Xcp files to VS-solution
        foreach(string file in Helper.GetFiles(genDataDir, xcpGenFilePattern))
        {
          model.Project.AddBswFile(file, Name);
        }
      }
    }
  }
}

