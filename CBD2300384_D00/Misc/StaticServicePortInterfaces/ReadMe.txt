/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *
 *                 This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                 Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                 All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *              File: ReadMe.txt
 *
 *********************************************************************************************************************/

 /**********************************************************************************************************************
  AUTHOR IDENTITY
-----------------------------------------------------------------------------------------------------------------------
  Name                          Initials      Company
-----------------------------------------------------------------------------------------------------------------------
  Sven Hesselmann               vissvh        Vector Informatik GmbH
-----------------------------------------------------------------------------------------------------------------------
  REVISION HISTORY
-----------------------------------------------------------------------------------------------------------------------
  Version   Date        Author  Change Id     Description
-----------------------------------------------------------------------------------------------------------------------
  01.00.00  2018-04-06  vissvh  -             Initial creation
  02.00.00  2021-03-10  vissvh  -             Updated to MSR4 R25
                                              AUTOSAR_4-3-0.xsd schema updated to AUTOSAR_00046.xsd
                                              MICROSAR_R<xx>_Static_ServicePortInterfaces package changed to MICROSAR_Static_ServicePortInterfaces
                                              Significant changes within CSM service ports due to changes from AR V4.2 to AR V4.3
                                              Significant changes within DLT service ports due to changes to AR V4.3 and AR R19-11
                                              Added IdsM, KeyM
**********************************************************************************************************************/


The file MSR4_R25_Static_ServicePortInterfaces.arxml provides a list of Service Port Interfaces of the MICROSAR modules in R25.
The list only contains Service Port Interfaces that are (mostly) static and not depending on the configuration of the Service Compoment.
The provided Service Port Interfaces can be used to setup application SWCs without having a configured Service Compoment.

