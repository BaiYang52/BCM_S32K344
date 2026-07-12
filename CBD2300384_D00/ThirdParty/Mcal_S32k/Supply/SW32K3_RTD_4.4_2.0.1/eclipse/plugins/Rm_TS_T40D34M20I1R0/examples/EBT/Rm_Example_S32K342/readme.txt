1. Example Description
    This application demonstrates the usage of a the Resource Manager HLD
    The application includes an EB Tresos project that configures the Rm driver.
    1.1 The application software functionality
            - Rm_Init
                The application will configure the Rm module peripherals(Virt Wrapper, Xrdc and Mpu_M7).
                Virt wrapper is configured to MSCR 1 access by MIRROR 1 - PDAC1_M7_0_0 and MSCR 2 access by MIRROR 2 - PDAC1_M7_0_1.
                Xrdc is configured to allocate domain 1 to the Cortex-M7_0, configures full access to Virtual Wrapper PDAC1_M7_0_0 and block access to PDAC1_M7_0_1.
                Mpu_M7 will be configured to remove any right for SRAM_REGION.
            - The application will write on SIU2 MSCR 1 by write on the MIRROR 1 - PDAC1_M7_0_0 to verify Virt Wrapper.
            - The application will write on SIU2 MSCR 2 by writing on MIRROR 2 - PDAC1_M7_0_1 which is blocked by XRDC.
              This action will be prevented by XRDC and cause an exception. The app checks that XRDC error report.
            - The application will try to access the denied memory regions and will compare the errors reported by the Mpu modules.
2. Installation steps
    2.1 Hardware installation
        2.1.1 Supported boards
            - XS32K3X2CVB-Q172 PCB 48307 RevX1 SCH RevA. Silicon: P32K342EHVPBE 0P97C CTZM2139A
        2.1.2 Connections
            - Powered board through power Adapter 12V
        2.1.3 Debugger
            - The Lauterbach debugger must be connected to J205 - 20 pin ARM Standard JTAG Connector.
    2.2 Software installation
        2.2.1 Tresos Project Installation
            The following procedure requires that the user has EB Tresos Studio installed and it is only needed for viewing, updating and generating the driver's configuration.
            Procedure:
                1. Make sure that all RTD plugins are already installed.
                2. Open Tresos Studio
                3. Import example application project
                    a. Click on "File" and select "Import"
                    b. Select "Existing Projects into Workspace" and click on "Next" button
                    c. Select "Select root directory" and click on "Browse"
                    d. Select the location of the [project] folder in the installed example application package folder. Do not copy the project into workspace unless you copy the whole example, otherwise compiling the files will not work as intended.
                    e. Click on "Finish" button
        2.2.2 Example configuration
            The following procedure requires that the user has EB Tresos Studio installed and the toolchains versions specified in the Release Notes.
            The toolchain that will be used needs to be installed for correct operation and the path to the installation location shall be added into the system environment variable(s):
            - TRESOS_DIR the installation path of Electrobit Tresos (used for generating the project from command line)
                Ex: TRESOS_DIR = C:/EB/tresos27
            - GCC_DIR The path to the GCC installation dir
                Ex: GCC_DIR = C:/NXP/S32DS.3.3/S32DS/build_tools/gcc_v9.2/gcc-9.2-arm32-eabi
            - PLUGINS_DIR The path to the Tresos plugins directory
                Ex: PLUGINS_DIR = C:/EB/tresos27/plugins
            - T32_DIR The path to the T32 directory
                Ex: T32_DIR = C:/T32

            Procedure:
                1. Open project_parameters.mk file in a text editor and update the variables mentioned above with the correct paths
3. Building the example application
    For using the command line a make tool needs to be installed on your machine. For full functionality from command line sed is also required.
    3.1 Generating the Tresos project
        Procedure:
            1. Open the Windows command prompt window
            2. Change the current directory to the example application folder
            3. To generate the Tresos project of the example, execute the following command to build: make generate
            4. The output files of the generation shall be generated in the /generate subdirectory
    3.2 Compiling the application
        Procedure:
            1. Open the Windows command prompt window
            2. Change the current directory to the example application folder
            3. To build the example, execute the following command to build: make build
            4. The object files and linker output file (main.elf) shall be generated in the /out subdirectory
    3.3 Running the application on the board
        For executing the script in a T32 window already you can run the run.cmm script as is.
        To execute the example application from your command line execute make run. Note: to be able to run this command a sed package needs to be installed on your machine as it is used for dynamically updating some paths in the files that are used by this step.
    3.4 Modifying the configuration in Tresos and generating the code
        Users may change the application configuration according to their needs.
        Procedure:
            1. Open the EB Tresos Studio GUI
            2. Open previously imported Example Application project
            3. Use the Tresos Studio GUI to modify configuration parameter values and save the changes.
            4. Select the Example Application project and click on "Generate" button to generate the configuration files.
