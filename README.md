EDK2_TestMenu
=============

A Test Menu in EFI shell mode, a EFI Application base on EDK2(UDK2010)

Description
------------------
This program just try to present a simple UEFI application of menu UI and hotkey process, you can also parse command
line in this program.

    Test Menu. Copyright@2012
    Default print text message without parameters.
    Usage:
    TestMenu [-g][-G] [-h][-H]
    -g/-G     Menu mode
    -h/-H/-?  Print help

How to build
------------------
1. you can add TestMenu into folder "MdeModulePkg\Application" , then modify MdeModulePkg.dsc to add your inf file.
2. use "build -a IA32 -p MdeModulePkg\MdeModulePkg.dsc" to build package including TestMenu.efi
   or use "build -a IA32 -p MdeModulePkg\MdeModulePkg.dsc -m MdeModulePkg\Application\TestMenu\TestMenu.inf" to only
   build TestMenu.efi.
   if you need X64 application, please change "IA32" to "X64"
3. i recommanded make your own folder for your own applications, name "MyPkg", can reference MdeModulePkg, and put
   all your application under your own folder. 

