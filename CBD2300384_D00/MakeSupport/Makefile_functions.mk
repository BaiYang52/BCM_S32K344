########################################################################################################################
# File Name  : Makefile_functions.mk
# Description: This Sub-Makefile contains common function independent from other settings.
#
#              This file MUST NOT BE CHANGED on project integration
#
# Project    : Vector PES Build Environment
# Module     : GlobalMake 4.1
#
#-----------------------------------------------------------------------------------------------------------------------
# COPYRIGHT                                                                                                           
#-----------------------------------------------------------------------------------------------------------------------
# Copyright (c) 2019 by Vector Informatik GmbH.                                                  All rights reserved. 
#                                                                                                                     
#-----------------------------------------------------------------------------------------------------------------------
# AUTHOR IDENTITY                                                                                                     
#-----------------------------------------------------------------------------------------------------------------------
# Name                          Initials      Company                                                                 
# ----------------------------  ------------  --------------------------------------------------------------------------
# Clemens von Mann              vircvo        Vector Informatik GmbH
# Claudia Buhl                  vircbl        Vector Informatik GmbH
#-----------------------------------------------------------------------------------------------------------------------
# REVISION HISTORY                                                                                                    
#-----------------------------------------------------------------------------------------------------------------------
# Version   Date        Author  Description                                                                           
# --------  ----------  ------  ----------------------------------------------------------------------------------------
# 04.00.00  2018-09-26  vircvo  Initial Release
# 04.00.01  2018-10-08  vircvo  Added function setsuffix, modified value for extsubst
# 04.00.02  2018-10-15  vircbl  Modified header, added BETA disclaimer
# 04.00.04  2018-10-23  vircvo  Added function wildcard_save
# 04.00.05  2018-11-07  vircvo  Changed semantic of function extsubst, renamed function setsuffix to setext
#           2018-11-22  vircvo  Added function canonical
# 04.00.08  2019-01-10  vircvo  Fixed possible wrong convertion, added obj2err, obj2lst, erase obj2log
#           2019-01-14  vircvo  Fixed obj2err, obj2lst, erase obj2log
# 04.01.14  2019-01-16  vircbl  Bugfix in function realpath_rel
# 04.01.20  2019-06-03  vircbl  Updated file header and doxygen documentation
########################################################################################################################

########################################################################################################################
#    EXAMPLE CODE ONLY
#    -------------------------------------------------------------------------------------------------------------------
#    This Example Code is only intended for illustrating an example of a possible BSW integration and BSW configuration.
#    The Example Code has not passed any quality control measures and may be incomplete. The Example Code is neither
#    intended nor qualified for use in series production. The Example Code as well as any of its modifications and/or
#    implementations must be tested with diligent care and must comply with all quality requirements which are necessary
#    according to the state of the art before their use.
########################################################################################################################


## @file
#
#  Makefile_functions.mk
#  =====================
#
#  This Sub-Makefile contains function definitions. It is included by Global.Makefile.target.\$(VERSION).mk.
#
#  @ingroup mod_Makefile
#


func_uname:=$(shell uname)

#################################################################################
#
#  function os_path(filepath...)
#
#  return list of 'file' with os specific path delimiter
#
ifneq ($(filter CYGWIN_NT%,$(func_uname)),)
  os_path=$(subst \,/,$(1))
else ifneq ($(filter MINGW%,$(func_uname)),)
  os_path=$(subst /,\,$(1))
else
  $(warning platform = '$(func_uname)')
  os_path=$(subst \,/,$(1))
endif

################################################################################
#
#  function uniq(list)
#
#  do uniq with sort.
#
#  Makfile has uniq only in sort which is not possible for include path order.
#
#  tr " " "\n" : fields -> lines
#  cat -n      : add lines numbers
#  sort -uk2   : sort uniq with field 2
#  sort -nk1   : sort in previous order
#  cut -f2-    : remove line numbers
#  tr "\n" " " : lines -> fields
#
uniq=$(shell echo $(1) | tr " " "\n" | cat -n | sort -uk2 | sort -nk1| cut -f2- | tr "\n" " ")

################################################################################
#
#  function to_upper files...
#
#  return uppercase files
#
to_upper=$(shell echo $(1) | tr [:lower:] [:upper:])

################################################################################
#
#  function to_lower files...
#
#  return uppercase files
#
to_lower=$(shell echo $(1) | tr [:upper:] [:lower:])

################################################################################
#
#  function realpath_rel(from,target)
#
#  return a path without <dir/..> elements. They must be reduced
#
realpath_rel=$(if $(and $(1),$(2)),$(strip $(shell realpath -s -m --relative-to=$(1) $(2) | tr "\n" " ")))

################################################################################
#
#  function get_abspath(path-list)
#
get_abspath=$(filter /%,$(1))

################################################################################
#
#  function get_relpath(path-list)
#
get_relpath=$(filter-out $(call get_abspath,$(1)),$(1))

################################################################################
#
#  function canonical(path-list)
#
#  return  path without <dir/..> elements. They must be reduced
#
#  Example:
#    result = canonical( file1.c ./file2 dir/file3.c ./dir/file4.c sub/../file5.c /dir/file6.c)
#    -> file1.c file2 dir/file3.c dir/file4.c file5.c /dir/file6.c
#
canonical=$(shell $(MAKESUPPORT_DIR_U)/util/path_mk_canonical.sh $(1))

################################################################################
#
#  function wildcard_dir(dir, file_pattern...)
#
#  wildcard_dir: do $(wildcard 'file_pattern') function in subdirectory 'dir'
#                 like $(MAKE) -C <dir>
#
#  'dir' is save again missing trailing '/'
#
wildcard_dir=$(patsubst $(call os_path,$(1)/)%,%,$(wildcard $(addprefix $(call os_path,$(1)/),$(2))))

################################################################################
#
#  function wildcard_dir_u(dir, file_pattern...)
#
#  wildcard_dir_u: like 'wildcard_dir' but return os path delimiter
#
wildcard_dir_u=$(call wildcard_dir,$(1),$(call os_path,$(2)))

################################################################################
#
#  function extsubst(ext-list,new-ext,list)
#
#  subst file extension of multiple different extension.
#  Only those files whose extension is in 'ext-list' will be returned.
#
#  Example:
#    $(call extsubst,asm s,o,file1.asm file2.asm file3.s)
#    -> file1.o file2.o file3.o
#
extsubst=$(foreach EXTSUBST_EXT,$(1),$(subst .$(EXTSUBST_EXT),.$(2),$(filter %.$(EXTSUBST_EXT),$(3))))

################################################################################
#
#  function setext(ext,list)
#
#  Exchange all suffixes to .'suf'
#
#  Example:
#    OBJS=$(call setext,o,file1.c file2.asm)
#    -> OBJS=file1.o file2.o
#
setext=$(call extsubst,$(sort $(subst .,,$(suffix $(2)))),$(1),$(2))

################################################################################
#
#  function obj2outfile(newpath,new-ext)
#
#  Take object file name and exchange $(OBJ_DIR) to 'newpath'
#  and exchange extension to 'new-ext'
#
#  Example:
#    $(call obj2outfile,$(ERR_PATH),err)
#    obj/dir/file.o -> err/dir/file.err
#
obj2outfile=$(subst $(OBJ_PATH),$1,$(@:.$(OBJ_SUFFIX)=.$2))

################################################################################
#
#  function wildcard_save(path_list)
#
#  Like wildcard, but doesn't erase not existing files.
#  Erase duplicate entries.
#
wildcard_save=$(uniq $(foreach F,$(1),$(if $(wildcard $(F),$(wildcard $(F)),$(F)))))

################################################################################
#
#  function obj2err(path_list)
#
#  Convert an obj path to an err path.
#
obj2err=$(if $(ERR_PATH),$(patsubst $(OBJ_PATH)/%,$(ERR_PATH)/%,$(basename $(1)).$(ERR_SUFFIX)))

################################################################################
#
#  function obj2lst(path_list)
#
#  Convert an obj path to an list path.
#
obj2lst=$(if $(LST_PATH),$(patsubst $(OBJ_PATH)/%,$(LST_PATH)/%,$(basename $(1)).$(LST_SUFFIX)))

################################################################################
#
#  function list2sep(sep, list)
#
#  Convert a make 'list' to a string separated with 'sep'
#
#  Example:
#   $(call list2sep,a b c,;)  -> "a;b;c"
#
list2sep_space:=
list2sep_space+=
list2sep=$(subst $(list2sep_space),$(1),$(2))
