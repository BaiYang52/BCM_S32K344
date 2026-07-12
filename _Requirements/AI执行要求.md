1._Requirements，CBD2300384_D00，Davinci_Project，EBtresos_Project
这三个文件夹重要的配置文件，绝对不允许改动，确实无法避免时允许增加文件
2.不允许直接改动而不知会我，所有的改动必须经过我审核
3._Requirements文件下为需求输入文件，可以作为参考
4.当前工程目的为基于S32K344实现Autosar BCM ECU的APP A开发
Flash Size 4M 计划flash分区如下
Bootloader    256KB    FBL
Boot Config    64KB    Boot Flag、安全配置
APP A    1408KB    主程序
APP B    1408KB    OTA 升级备用
Calibration    256KB    XCP 标定
Fee/NvM    512KB    AUTOSAR NvM
Reserved    剩余    后续扩展
推荐做法：
目前阶段只考虑APP A，要能独立编译烧录和运行，并且便于后续增加新FBl工程
FBL 做好后，要将 APP 烧录到 FBL 后面时（比如 Flash 偏移到 0x00440000）
只需要在 IAR 中右键 boot_header.c -> 选择 **Exclude from Build（不参与编译）**，
然后修改一下 .icf 文件的起始地址，APP 工程就直接变成了一个标准的、受 FBL 引导的子工程
5.vscode工程中添加了IAR Build插件可供使用
6.IAR安装本地路径为D:\SoftWare\Programming\IARSystems\IAR_9.32.1
7.启动文件在BCM_S32K344\_SourceCode\StartUp