#!/bin/sh

# Display usage
cpack_usage()
{
  cat <<EOF
Usage: $0 [options]
Options: [defaults in brackets after descriptions]
  --help            print this message
  --version         print cmake installer version
  --prefix=dir      directory in which to install
  --include-subdir  include the amo_tools_suite--Darwin-x86_64 subdirectory
  --exclude-subdir  exclude the amo_tools_suite--Darwin-x86_64 subdirectory
  --skip-license    accept license
EOF
  exit 1
}

cpack_echo_exit()
{
  echo $1
  exit 1
}

# Display version
cpack_version()
{
  echo "amo_tools_suite Installer Version: 0.3.1, Copyright (c) Oak Ridge National Laboratory"
}

# Helper function to fix windows paths.
cpack_fix_slashes ()
{
  echo "$1" | sed 's/\\/\//g'
}

interactive=TRUE
cpack_skip_license=FALSE
cpack_include_subdir=""
for a in "$@"; do
  if echo $a | grep "^--prefix=" > /dev/null 2> /dev/null; then
    cpack_prefix_dir=`echo $a | sed "s/^--prefix=//"`
    cpack_prefix_dir=`cpack_fix_slashes "${cpack_prefix_dir}"`
  fi
  if echo $a | grep "^--help" > /dev/null 2> /dev/null; then
    cpack_usage 
  fi
  if echo $a | grep "^--version" > /dev/null 2> /dev/null; then
    cpack_version 
    exit 2
  fi
  if echo $a | grep "^--include-subdir" > /dev/null 2> /dev/null; then
    cpack_include_subdir=TRUE
  fi
  if echo $a | grep "^--exclude-subdir" > /dev/null 2> /dev/null; then
    cpack_include_subdir=FALSE
  fi
  if echo $a | grep "^--skip-license" > /dev/null 2> /dev/null; then
    cpack_skip_license=TRUE
  fi
done

if [ "x${cpack_include_subdir}x" != "xx" -o "x${cpack_skip_license}x" = "xTRUEx" ]
then
  interactive=FALSE
fi

cpack_version
echo "This is a self-extracting archive."
toplevel="`pwd`"
if [ "x${cpack_prefix_dir}x" != "xx" ]
then
  toplevel="${cpack_prefix_dir}"
fi

echo "The archive will be extracted to: ${toplevel}"

if [ "x${interactive}x" = "xTRUEx" ]
then
  echo ""
  echo "If you want to stop extracting, please press <ctrl-C>."

  if [ "x${cpack_skip_license}x" != "xTRUEx" ]
  then
    more << '____cpack__here_doc____'
Copyright 2018, UT-Battelle, LLC
All rights reserved
 AMO Tools Suite
OPEN SOURCE LICENSE (Permissive)

Subject to the conditions of this License, UT-Battelle, LLC (the “Licensor”) hereby grants, free of charge, to any person 
(the “Licensee”) obtaining a copy of this software and associated documentation files (the "Software"), a perpetual, 
worldwide, non-exclusive, no-charge, royalty-free, irrevocable copyright license to use, copy, modify, merge, publish, 
distribute, and/or sublicense copies of the Software.

1. Redistributions of Software must retain the above open source license grant, copyright and license notices, this list of 
conditions, and the disclaimer listed below.  Changes or modifications to, or derivative works of the Software must be noted 
with comments and the contributor and organization’s name.

2. Neither the names of Licensor, the Department of Energy, or their employees may be used to endorse or promote products 
derived from this Software without their specific prior written permission.

3. If the Software is protected by a proprietary trademark owned by Licensor or the Department of Energy, then derivative 
works of the Software may not be distributed using the trademark without the prior written approval of the trademark owner. 
	


****************************************************************************************************************
DISCLAIMER

UT-BATTELLE, LLC AND THE GOVERNMENT MAKE NO REPRESENTATIONS AND DISCLAIM ALL WARRANTIES, BOTH EXPRESSED AND IMPLIED.  THERE 
ARE NO EXPRESS OR IMPLIED WARRANTIES OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE, OR THAT THE USE OF THE SOFTWARE 
WILL NOT INFRINGE ANY PATENT, COPYRIGHT, TRADEMARK, OR OTHER PROPRIETARY RIGHTS, OR THAT THE SOFTWARE WILL ACCOMPLISH THE 
INTENDED RESULTS OR THAT THE SOFTWARE OR ITS USE WILL NOT RESULT IN INJURY OR DAMAGE.  THE USER ASSUMES RESPONSIBILITY FOR ALL 
LIABILITIES, PENALTIES, FINES, CLAIMS, CAUSES OF ACTION, AND COSTS AND EXPENSES, CAUSED BY, RESULTING FROM OR ARISING OUT OF, 
IN WHOLE OR IN PART THE USE, STORAGE OR DISPOSAL OF THE SOFTWARE.

****************************************************************************************************************

____cpack__here_doc____
    echo
    echo "Do you accept the license? [yN]: "
    read line leftover
    case ${line} in
      y* | Y*)
        cpack_license_accepted=TRUE;;
      *)
        echo "License not accepted. Exiting ..."
        exit 1;;
    esac
  fi

  if [ "x${cpack_include_subdir}x" = "xx" ]
  then
    echo "By default the amo_tools_suite will be installed in:"
    echo "  \"${toplevel}/amo_tools_suite--Darwin-x86_64\""
    echo "Do you want to include the subdirectory amo_tools_suite--Darwin-x86_64?"
    echo "Saying no will install in: \"${toplevel}\" [Yn]: "
    read line leftover
    cpack_include_subdir=TRUE
    case ${line} in
      n* | N*)
        cpack_include_subdir=FALSE
    esac
  fi
fi

if [ "x${cpack_include_subdir}x" = "xTRUEx" ]
then
  toplevel="${toplevel}/amo_tools_suite--Darwin-x86_64"
  mkdir -p "${toplevel}"
fi
echo
echo "Using target directory: ${toplevel}"
echo "Extracting, please wait..."
echo ""

# take the archive portion of this file and pipe it to tar
# the NUMERIC parameter in this command should be one more
# than the number of lines in this header file
# there are tails which don't understand the "-n" argument, e.g. on SunOS
# OTOH there are tails which complain when not using the "-n" argument (e.g. GNU)
# so at first try to tail some file to see if tail fails if used with "-n"
# if so, don't use "-n"
use_new_tail_syntax="-n"
tail $use_new_tail_syntax +1 "$0" > /dev/null 2> /dev/null || use_new_tail_syntax=""

extractor="pax -r"
command -v pax > /dev/null 2> /dev/null || extractor="tar xf -"

tail $use_new_tail_syntax +180 "$0" | gunzip | (cd "${toplevel}" && ${extractor}) || cpack_echo_exit "Problem unpacking the amo_tools_suite--Darwin-x86_64"

echo "Unpacking finished successfully"

exit 0
#-----------------------------------------------------------
#      Start of TAR.GZ file
#-----------------------------------------------------------;

� \r�] ���j1�=�S��L&��Co=x+o�e�݅�.x�q|��-V\�K�R��2d2�?�Y��4�Mhƾ��m]o�:vE��AD+��r�x��"-k@�$:��3\���7)Jc��so����.}Q�2�&��?�� ��D[#9	t��NG��ʶ(C����87�ʰV,6����_�m��o����0)���[��'�]�?Yb��<"Lߓ��`0x^G5�� 
  