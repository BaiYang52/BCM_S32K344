1. Example Description
    This application demonstrates the usage of the Adc Driver by Interrupt and DMA Transfer.
        The application includes an EB Tresos project that configures the Mcu, Mcl Platform and Adc drivers.
    If all returned status and converted results are correct, the bStatus variable remains "TRUE" until end of the example.
    1.1 The application software functionality
        The example is divided in 3 parts:
        - Part 1: Example with SW Triggered One-Shot Conversion Mode, data conversion is updated by Interrupt.
        - Part 2: Example with BCTU controlled mode conversion by Interrupt.
        - Part 3: Example with SW Triggered One-Shot Conversion Mode, data conversion is updated by DMA.
        - Mcu_Init, Mcu_InitClock, Mcu_SetMode
                Initialize the clock sources, the clock tree and to configure the clock gating of the peripherals.
                The clock configuration that is used will enable and use the PLL as source clock.
        - Mcl_Init
                Initialize the Mcl Driver: Initialize the DMA Logic Instance and DMA Logic Channel 0.
                Logic Channel 0 is configured in EB Tresos to run in Scatter/Gather Mode, enables DMAMUX with source is DMA_IP_REQ_MUX1_SARADC1.
        - Platform_Init
                Initialize the interrupt controller and to apply the interrupt configuration according to the Platform configuration.
        - Platform_SetIrq
                Enables interrupt request of Adc Hardware Instance 0, BCTU Hardware unit 0 and DMA transfer complete Channel 1.
        - Platform_InstallIrqHandler
                Install the ISR for the Adc Hardware Instance 0, BCTU Hardware unit 0 and DMA transfer complete Channel 1.
        - Adc_Init
                Initialize the Adc Driver: Initialize the Adc Hardware Instances and BCTU Hardware Instance.
        - Adc_Calibrate
                Enable calibration functionality of Adc Hardware Instances.
        - Adc_SetupResultBuffer
                Initializes ADC driver with the group specific result buffer start address (ResultBuffer and ResultBufferDma) where the
                conversion results will be stored.
        - Adc_EnableGroupNotification
                Enables the notification mechanism for the requested ADC Channel group.
        - Adc_StartGroupConversion
                Starts the conversion of all channels of the requested ADC Channel group.
        - Adc_ReadGroup
                Reads the group conversion result of the last completed conversion round of the
                requested group and stores the channel values starting at the AdcReadGroupResult address.
        - Adc_EnableCtuControlMode
                Enables the control mode functionality of BCTU Hardware unit 0.
        - Adc_CtuStartConversion
                Start conversion of a BCTU trigger source
        - Adc_CtuReadConvData
                Read data from BCTU CDR register.
        - Adc_DisableCtuControlMode
                Disables the control mode functionality of BCTU Hardware unit 0.
        - Adc_DeInit
                Returns all ADC HW Units to a state comparable to their power on reset state.
        - Verification criteria for a successful example run is bStatus being TRUE at the end of the example.

2. Installation steps
    2.1 Hardware installation
        2.1.1 Supported boards
            - S32K312EVB-Q172 PCB 50892 RevA SCH RevB (EVB)
        2.1.2 Connections
            - Connect J18.1 to J18.2 as default position for 5V VREF on the board.
            - By default S32K312 board will have 5V voltage reference selected, please uncomment '#define ADC_3V3_VREF_SELECTED' in main.c if 3.3v is manually selected.
        2.1.3 Debugger
            - The debugger must be connected to J12 10-pin JTAG Cortex Debug connector on the board.
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
                        Ex: TRESOS_DIR = C:/EB/tresos27_1_0
                    - GCC_DIR The path to the GCC installation dir
                        Ex: GCC_DIR = C:/NXP/S32DS.3.3/S32DS/build_tools/gcc_v10.2/gcc-10.2-arm32-eabi
                    - PLUGINS_DIR The path to the Tresos plugins directory
                        Ex: PLUGINS_DIR = C:/EB/tresos27_1_0/plugins
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
