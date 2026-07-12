1. Example Description
        The example application showcases the usage of 3 features offered by the Crypto driver:
         - AES128 ECB Encryption and Decryption
         - CMAC Generate/Verify
         - HASH

    1.1 The application software functionality
        The example application code sections are split in 3 subsections, one for each of the 3 shown features.
        Example code for feature #1:
           Performs 2 synchronous encryption/decryption operations with 2 different values for the key and 2 different plain/cipher texts
           Assumes that both the values of the key and the data to encrypt/decrypt can change at run-time, so they are stored in RAM
        Example code for feature #2:
           Performs 2 synchronous CMAC generate/verify operations with 2 different values for the key and 2 different blocks of data
           Assumes that both the values of the key and the data to generate/verify CMAC over can change at run-time, so they are stored in RAM
        Example code for feature #3:
           Performs 2 synchronous Hash operations (SHA-1, SHA2-256) over a 64 bytes block of data
           Assumes that the value of the data to hash can change at run-time, so it is stored in RAM

        Configuration:
          - HSE Key catalogs should be configured with at least one group of AES128 keys
          - Because the value(Key Material) of the AES128 key that is used is changed at run-time, the key element that stores the key must be stored
        in the HSE RAM Catalog so it can be updated for as many times as needed.
          - The HseKeyFlags of the Key Element storing the HSE AES128 key must have the HSE_KF_USAGE_ENCRYPT, HSE_KF_USAGE_DECRYPT, HSE_KF_USAGE_SIGN and HSE_KF_USAGE_VERIFY flags
          - The Crypto Driver Object that is used to process the jobs (CDO_Symmetric) should have the following primitives set:
             - AES128_Encrypt and AES128_Decrypt
             - CMAC_Generate and CMAC_Verify
             - HASH_SHA1 and HASH_SHA2_256

2. Installation steps
    2.1 Hardware installation
        2.1.1 Supported boards
            - EVB: S32K312EVB-Q172 PCB 50892 RevA SCH RevB
        2.1.2 Connections
            No special wiring connections are needed for this example.
        2.1.3 Debugger
            The debugger must be connected to the EVB board's J12 10-pin JTAG Cortex Debug connector.
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
                Ex: TRESOS_DIR = C:/EB/tresos27.1
            - GCC_DIR The path to the GCC installation dir
                Ex: GCC_DIR = C:/NXP/S32DS.3.4/S32DS/build_tools/gcc_v10.2/gcc-10.2-arm32-eabi
            - T32_DIR The path to the T32 directory
                Ex: T32_DIR = C:/T32
            - ADDITIONAL_INCLUDE_DIRS The list of directories in the 'interface' folder of the HSE Firmware release that was used to test the current version of the Crypto driver.
                The version of the HSE Firmware release that was used to test the Crypto driver is listed at the end of chapter '3.2 Files required for compilation' in the Crypto driver Integration Manual.
                Ex: ADDITIONAL_INCLUDE_DIRS = C:/NXP/HSE_FW_S32K3X2_0_0_11_0/hse_full_mem/interface \
                                              C:/NXP/HSE_FW_S32K3X2_0_0_11_0/hse_full_mem/interface/config \
                                              C:/NXP/HSE_FW_S32K3X2_0_0_11_0/hse_full_mem/interface/inc_common \
                                              C:/NXP/HSE_FW_S32K3X2_0_0_11_0/hse_full_mem/interface/inc_custom \
                                              C:/NXP/HSE_FW_S32K3X2_0_0_11_0/hse_full_mem/interface/inc_services

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
