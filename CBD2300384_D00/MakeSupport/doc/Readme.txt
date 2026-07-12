
/** @page makesupport_readme_page MakeSupport Readme

@ingroup mod_Makefile

Short MakeSupport Introduction
==============================

General:
--------
- Currently, MakeSupport works with GNU make on cygwin.
- MakeSupport can be used inside a cygwin shell or a DOS box.
- MakeSupport contains needed cygwin commands in sub-directory 'cygwin_root/cmd'.
- Call set_cygwin_path.bat to enable cygwin in directory 'cygwin_root/cmd'.

Documenation:
-------------
For getting help, run
    - target <b>help</b> and read console output or
    - target <b>doxygen_makesupport</b> and read generated doxygen documentation

Calling of make with useful options:
------------------------------------
- <b>make help:</b>      show implemented targets
- <b>make <target>:</b>  build target
- <b>make -j <n>:</b>    parallel build
- <b>make -n:</b>        show commands only ; do not build
- <b>make V=1:</b>       show commands during build
  
MakeSupport is safe in
----------------------
- build directories automatically
- build parallel
- build dependencies automatically (no make depend needed anymore)

Please read
  https://www.gnu.org/software/make/manual/make.html
for further information

*/
