1. Example Description
        This application demonstrates the usage of the I2s Driver over Sai hardware Ip.
        The application includes an EB Tresos project that configures the Mcu, Port and Platform drivers.
    1.1 The application software functionality
        - Mcu_Init, Mcu_InitClock, Mcu_SetMode
            Initialize the clock sources, the clock tree and to configure the clock gating of the peripherals.
        - OsIf_Init
            Initialize the OsIf for I2s internal delay.
        - Platform_Init
            Initialize the interrupt controller and to apply the interrupt configuration according to the Platform configuration.
        - Platform_SetIrq
            Enables interrupt request of Sai Instance 0 and 1.
        - Platform_InstallIrqHandler
            Install the ISR for the Sai Hardware Instance 0 and 1.
        - Port_Init
            Initialize Port pins for 2 Sai instances
        - I2s_Init
            Initialization of I2s driver
        - I2s_AsyncTransmit
            Starts an asynchronous transmission on the SAI bus.
        - I2s_GetStatus
            Gets the status of an I2s instance.
        - The application will verify in the received data corresponds with the sent data and update the status flag bStatus accordingly.
        - I2s_DeInit
            De-initialization of I2s modules to reset values

2. Installation steps
    2.1 Hardware installation
        2.1.1 Supported boards
            - Mini-module: XS32K3X2CVB-Q172 PCB 48307 RevX1 SCH RevA
            - Mother-board: X-S32K3XXX-MB PCB 47386 RevA SCH RevA
            - Silicon: P32K342EHVPBE 0P97C CTZM2139A
        2.2.2 Connections
            All connection pins are located in the mother board
            - SAI0 D0 (PTB2 - P4.9) is wired to SAI1 D0 (PTD13 - P4.23)
            - SAI0 SYNC (PTC13 - P4.13) is wired to SAI1 SYNC (PTD15 - P4.29)
            - SAI0 BCLK (PTC12 - P4.11) is wired to SAI1 BCLK (PTE8 - P4.25)
        2.2.3 Debugger
            The debugger must be connected to J365 20-pin JTAG Cortex Debug connector.
    2.2 Software installation
        2.2.1 Tresos Project Installation
            The following procedure requires that the user has EB Tresos Studio installed and it is only needed for viewing, updating and generating the driver's configuration.
            Procedure:
                1. Make sure that all RTD plugins are already installed in the Tresos Studio plugins directory
                2. Open Tresos Studio
                3. Import example application project
                    a. Click on "File" and select "Import"
                    b. Select "Existing Projects into Workspace" and click on "Next" button
                    c. Select "Select root directory" and click on "Browse"
                    d. Select the location of the [project] folder in the installed example application package folder
                    e. Optional* - Select "Copy projects into workspace"
                    f. Click on "Finish" button
        2.2.2 Example configuration
            The following procedure requires that the user has EB Tresos Studio installed and the toolchains versions specified in the Release Notes.
            The toolchain that will be used needs to be installed for correct operation and the path to the installation location shall be added into the system environment variable(s).
            Procedure:
                1. Open project_parameters.mk file in a text editor and update the variables mentioned below with the correct paths:
                    - TRESOS_DIR the installation path of Electrobit Tresos (used for generating the project from command line)
                        Ex: TRESOS_DIR = C:/EB/tresos282
                    - GCC_DIR The path to the GCC installation dir
                        Ex: GCC_DIR = C:/NXP/S32DS.3.4/S32DS/build_tools/gcc_v10.2/gcc-10.2-arm32-eabi
                    - PLUGINS_DIR The path to the Tresos plugins directory
                        Ex: PLUGINS_DIR = C:/EB/tresos282/plugins
                2. Open the config.t32 file and update the path of SYS variable with the correct path:
                    - SYS The path to the T32 directory
                        Ex: SYS=C:/T32
3. Building the example application
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
        To execute the example application from your command line execute make run. For executing the script in a T32 window already open the paths from run.cmm need to be changed relative to the debug folder instead of the example root folder.
    3.4 Modifying the configuration in Tresos and generating the code
        Users may change the application configuration according to their needs.
        Procedure:
            1. Open the EB Tresos Studio GUI
            2. Open previously imported Example Application project
            3. Use the Tresos Studio GUI to modify configuration parameter values and save the changes.
            4. Select the Example Application project and click on "Generate" button to generate the configuration files.
