#!/bin/bash
# [single_build.sh] : re-build script for ebx
#
#This script runs by SYSROM_SRC folder.
# setenv set the path for SYSROM_SRC as $EB2
# caution LF <-> CR+LF

. setenv

BUILD_THIRDPARTY="1"
BUILD_ALCI="1"
BUILD_SL="1"
BUILD_DLDL="0"

#WEISS_COMPILER_VERSION="GCC4.3.2"
WEISS_COMPILER_VERSION="GCC4.4.1"

export ROOT_DIR=$EB2
export LANG=C
export MAKE=make
export RELEASE_BINDIR=$ROOT_DIR/build/$BUILD_TYPE/bin/
export RELEASE_LIBDIR=$ROOT_DIR/build/$BUILD_TYPE/lib/
export OBJWORK_DIR=./obj/

cd $ROOT_DIR
chmod 777 -R $ROOT_DIR

# check setenv script is already executed or not.
if [ -z "$EB2" ]; then
	echo "### Please run setenv script firstly... ###"
	exit
fi

if [ "$BUILD_THIRDPARTY" = "1" ]; then
	echo "### Build 3rd party is on ###"
fi
if [ "$BUILD_ALCI" = "1" ]; then
	echo "### Build ALCI is on ###"
fi
if [ "$BUILD_SL" = "1" ]; then
	echo "### Build SL and Mishima code is on ###"
fi
if [ "$BUILD_DLDL" = "1" ]; then
	echo "### Build DL_DL  code is on ###"
fi

if [ "$PRODUCT" = "MASH" -o "$PRODUCT" = "BP" ]; then

	# for EX-BP/EX-Mash L3.1
	export PATH=/opt/WindRiver30/workbench-3.1/foundation//x86-linux2/wrbin:/opt/WindRiver30/wrlinux-3.0/layers/wrll-toolchain-4.3-85/powerpc/toolchain/x86-linux2/bin:/opt/WindRiver30:/opt/WindRiver30/utilities-1.0/x86-linux2/bin:/opt/WindRiver30/workbench-3.1/analysis/host/bin/x86-linux2:/opt/WindRiver30/workbench-3.1/x86-linux2/bin:$PATH
	export LD_LIBRARY_PATH=/opt/WindRiver30/workbench-3.1/analysis/host/bin/x86-linux2:/opt/WindRiver30/workbench-3.1/wrwb/platform/eclipse/x86-linux2/bin:/opt/WindRiver30/workbench-3.1/x86-linux2/lib:$LD_LIBRARY_PATH
	export WIND_BASE="/opt/WindRiver30/wrlinux-3.0"
	export WIND_DFW_PATH="/opt/WindRiver30/workbench-3.1/dfw/"
	export WIND_DOCS="/opt/WindRiver30/docs"
	export WIND_EXTENSIONS="/opt/WindRiver30/wrlinux-3.0/wrlinux/rtcore/extensions:/opt/WindRiver30/workbench-3.1/analysis/extensions:/opt/WindRiver30/workbench-3.1/wrsysviewer:/opt/WindRiver30/workbench-3.1/wrwb/tools:/opt/WindRiver30/workbench-3.1/wrwb/wrlinux:/opt/WindRiver30/workbench-3.1/wrwb/wrhv:/opt/WindRiver30/workbench-3.1/wrwb/vthreads:/opt/WindRiver30/workbench-3.1/wrwb/vxworksmilshae:/opt/WindRiver30/workbench-3.1/wrwb/vxworksmils:/opt/WindRiver30/workbench-3.1/wrwb/vxworkscert:/opt/WindRiver30/workbench-3.1/wrwb/vxworks653:/opt/WindRiver30/workbench-3.1/wrwb/vxworks55:/opt/WindRiver30/workbench-3.1/wrwb/vxworks:/opt/WindRiver30/workbench-3.1/wrwb/wrworkbench:/opt/WindRiver30/docs/extensions"
	export WIND_FOUNDATION_PATH="/opt/WindRiver30/workbench-3.1/foundation/"
	export WIND_HOME="/opt/WindRiver30"
	export WIND_HOST_TYPE="x86-linux2"
	export WIND_JRE_HOME="/opt/WindRiver30/jre/1.5.0_11/x86-linux2"
	export WIND_LINUX_BUILD="/opt/WindRiver30/wrlinux-3.0/wrlinux"
	export WIND_LINUX_CONFIGURE="/opt/WindRiver30/wrlinux-3.0/wrlinux/configure"
	export WIND_LX_HOME="/opt/WindRiver30"
	export WIND_PLATFORM="wrlinux-3.0"
	export WIND_PREFERRED_PACKAGES="wrlinux-3.0"
	export WIND_RTC_HOME="/opt/WindRiver30/wrlinux-3.0/layers/wrll-rtcore-5.2"
	export WIND_SAMPLES="/opt/WindRiver30/wrlinux-3.0/layers/wrll-rtcore-5.2/samples:/opt/WindRiver30/wrlinux-3.0/samples:/opt/WindRiver30/workbench-3.1/analysis/target/src/linux:/opt/WindRiver30/workbench-3.1/analysis/target/src/vxworks:/opt/WindRiver30/workbench-3.1/samples"
	export WIND_SCOPETOOLS_BASE="/opt/WindRiver30/workbench-3.1/analysis"
	export WIND_SCOPETOOLS_RPMS_BASE="/opt/WindRiver30/workbench-3.1/analysis/target/RPMS"
	export WIND_TOOLS="/opt/WindRiver30/workbench-3.1"
	export WIND_USERMODE_AGENT="/opt/WindRiver30/linux-2.x/usermode-agent/bin/usermode-agent.sh"
	export WIND_USERMODE_AGENT_PATH="/opt/WindRiver30/linux-2.x/usermode-agent"
	export WIND_UTILITIES="/opt/WindRiver30/utilities-1.0"
	export WIND_WRLINUX_BUILDDEPS_SCRIPT="/opt/WindRiver30/wrlinux-3.0/scripts/builddepsscript.sh"
	export WIND_WRLINUX_LAUNCH_SCRIPT="/opt/WindRiver30/wrlinux-3.0/scripts/launch_script.sh"
	export WIND_WRLINUX_LAYERS="/opt/WindRiver30/wrlinux-3.0/layers/wrll-analysis-1.0:/opt/WindRiver30/workbench-3.1/analysis/wrlinux"
	export WIND_WRLINUX_QUERY_SCRIPT="/opt/WindRiver30/wrlinux-3.0/scripts/queryProperties.sh"
	export WIND_WRLINUX_TARGET_BUTTONS="/opt/WindRiver30/wrlinux-3.0/scripts/wbblddefgen.txt"
	export WIND_WRSV_PATH="/opt/WindRiver30/workbench-3.1/wrsysviewer/"
	export WIND_WRWB_PATH="/opt/WindRiver30/workbench-3.1/wrwb/platform/eclipse"

	# TOOLCHAIN_PATH need to be set.
	export PATH=$PATH:/opt/WindRiver30/wrlinux-3.0/sysroots/mfp_sys430exd-glibc_cgl/x86-linux2/
	export TOOLCHAIN_PATH=/opt/WindRiver30/wrlinux-3.0/sysroots/mfp_sys430exd-glibc_cgl/sysroot
	export TOOL_PREFIX=powerpc-wrs-linux-gnu-ppc_e500v2-glibc_cgl-


elif [ "(" "$PRODUCT" = "LOIRE" -o "$PRODUCT" = "ALABAMA" ")" -a "(" ! "$SUB_PRODUCT" = "EX_SERIES_2" -a ! "$SUB_PRODUCT" = "ECO_LOIRE2" ")" ]; then

	# for EX-Loire/EX-Alabama L3.2
	export PATH=/opt/WindRiver30/workbench-3.1/foundation//x86-linux2/wrbin:/opt/WindRiver30/wrlinux-3.0/layers/wrll-toolchain-4.3-85/powerpc/toolchain/x86-linux2/bin:/opt/WindRiver30:/opt/WindRiver30/utilities-1.0/x86-linux2/bin:/opt/WindRiver30/workbench-3.1/analysis/host/bin/x86-linux2:/opt/WindRiver30/workbench-3.1/x86-linux2/bin:$PATH
	export LD_LIBRARY_PATH=/opt/WindRiver30/workbench-3.1/analysis/host/bin/x86-linux2:/opt/WindRiver30/workbench-3.1/wrwb/platform/eclipse/x86-linux2/bin:/opt/WindRiver30/workbench-3.1/x86-linux2/lib:$LD_LIBRARY_PATH
	export WIND_BASE="/opt/WindRiver30/wrlinux-3.0"
	export WIND_DFW_PATH="/opt/WindRiver30/workbench-3.1/dfw/"
	export WIND_DOCS="/opt/WindRiver30/docs"
	export WIND_EXTENSIONS="/opt/WindRiver30/wrlinux-3.0/wrlinux/rtcore/extensions:/opt/WindRiver30/workbench-3.1/analysis/extensions:/opt/WindRiver30/workbench-3.1/wrsysviewer:/opt/WindRiver30/workbench-3.1/wrwb/tools:/opt/WindRiver30/workbench-3.1/wrwb/wrlinux:/opt/WindRiver30/workbench-3.1/wrwb/wrhv:/opt/WindRiver30/workbench-3.1/wrwb/vthreads:/opt/WindRiver30/workbench-3.1/wrwb/vxworksmilshae:/opt/WindRiver30/workbench-3.1/wrwb/vxworksmils:/opt/WindRiver30/workbench-3.1/wrwb/vxworkscert:/opt/WindRiver30/workbench-3.1/wrwb/vxworks653:/opt/WindRiver30/workbench-3.1/wrwb/vxworks55:/opt/WindRiver30/workbench-3.1/wrwb/vxworks:/opt/WindRiver30/workbench-3.1/wrwb/wrworkbench:/opt/WindRiver30/docs/extensions"
	export WIND_FOUNDATION_PATH="/opt/WindRiver30/workbench-3.1/foundation/"
	export WIND_HOME="/opt/WindRiver30"
	export WIND_HOST_TYPE="x86-linux2"
	export WIND_JRE_HOME="/opt/WindRiver30/jre/1.5.0_11/x86-linux2"
	export WIND_LINUX_BUILD="/opt/WindRiver30/wrlinux-3.0/wrlinux"
	export WIND_LINUX_CONFIGURE="/opt/WindRiver30/wrlinux-3.0/wrlinux/configure"
	export WIND_LX_HOME="/opt/WindRiver30"
	export WIND_PLATFORM="wrlinux-3.0"
	export WIND_PREFERRED_PACKAGES="wrlinux-3.0"
	export WIND_RTC_HOME="/opt/WindRiver30/wrlinux-3.0/layers/wrll-rtcore-5.2"
	export WIND_SAMPLES="/opt/WindRiver30/wrlinux-3.0/layers/wrll-rtcore-5.2/samples:/opt/WindRiver30/wrlinux-3.0/samples:/opt/WindRiver30/workbench-3.1/analysis/target/src/linux:/opt/WindRiver30/workbench-3.1/analysis/target/src/vxworks:/opt/WindRiver30/workbench-3.1/samples"
	export WIND_SCOPETOOLS_BASE="/opt/WindRiver30/workbench-3.1/analysis"
	export WIND_SCOPETOOLS_RPMS_BASE="/opt/WindRiver30/workbench-3.1/analysis/target/RPMS"
	export WIND_TOOLS="/opt/WindRiver30/workbench-3.1"
	export WIND_USERMODE_AGENT="/opt/WindRiver30/linux-2.x/usermode-agent/bin/usermode-agent.sh"
	export WIND_USERMODE_AGENT_PATH="/opt/WindRiver30/linux-2.x/usermode-agent"
	export WIND_UTILITIES="/opt/WindRiver30/utilities-1.0"
	export WIND_WRLINUX_BUILDDEPS_SCRIPT="/opt/WindRiver30/wrlinux-3.0/scripts/builddepsscript.sh"
	export WIND_WRLINUX_LAUNCH_SCRIPT="/opt/WindRiver30/wrlinux-3.0/scripts/launch_script.sh"
	export WIND_WRLINUX_LAYERS="/opt/WindRiver30/wrlinux-3.0/layers/wrll-analysis-1.0:/opt/WindRiver30/workbench-3.1/analysis/wrlinux"
	export WIND_WRLINUX_QUERY_SCRIPT="/opt/WindRiver30/wrlinux-3.0/scripts/queryProperties.sh"
	export WIND_WRLINUX_TARGET_BUTTONS="/opt/WindRiver30/wrlinux-3.0/scripts/wbblddefgen.txt"
	export WIND_WRSV_PATH="/opt/WindRiver30/workbench-3.1/wrsysviewer/"
	export WIND_WRWB_PATH="/opt/WindRiver30/workbench-3.1/wrwb/platform/eclipse"

	# TOOLCHAIN_PATH need to be set.
	export PATH=$PATH:/opt/WindRiver30/wrlinux-3.0/sysroots/mfp_sys470d-glibc_std/x86-linux2/
	export TOOLCHAIN_PATH=/opt/WindRiver30/wrlinux-3.0/sysroots/mfp_sys470d-glibc_std/sysroot
	export TOOL_PREFIX=powerpc-wrs-linux-gnu-ppc_440fp-glibc_std-

elif [ "(" "$PRODUCT" = "WEISS" -a "$WEISS_COMPILER_VERSION" = "GCC4.4.1" ")" -o "(" "(" "$PRODUCT" = "LOIRE" -o "$PRODUCT" = "ALABAMA" ")" -a "(" "$SUB_PRODUCT" = "EX_SERIES_2" -o "$SUB_PRODUCT" = "ECO_LOIRE2" ")" ")" -o "$PRODUCT" = "ST_HELENS" -o "$PRODUCT" = "S2_PRODUCT" ]; then

	# for Weisshorn L4.0 with GCC4.4.1
	export PATH="/opt/WindRiver40/wrlinux-4/layers/wrll-toolchain-4.4a-323/powerpc/toolchain/x86-linux2/bin:/opt/WindRiver40/wrlinux-4/foundation//x86-linux2/wrbin:$PATH"
	export WIND_BASE="/opt/WindRiver40/wrlinux-4"
	export WIND_EXTENSIONS="/opt/WindRiver40/wrlinux-4/docs/extensions"
	export WIND_FOUNDATION_PATH="/opt/WindRiver40/wrlinux-4/foundation/"
	export WIND_HOME="/opt/WindRiver40"
	export WIND_HOST_TYPE="x86-linux2"
	export WIND_LINUX_BUILD="/opt/WindRiver40/wrlinux-4/wrlinux"
	export WIND_LINUX_CONFIGURE="/opt/WindRiver40/wrlinux-4/wrlinux/configure"
	export WIND_LX_HOME="/opt/WindRiver40"
	export WIND_PLATFORM="wrlinux-4"
	export WIND_PLATFORM_DOCS="com.windriver.ide.doc.wr_tipc_1.7.7:com.windriver.ide.doc.wr_mipc_2.1:com.windriver.ide.doc.wr_gnu_compiler_linux_4.4.1:com.windriver.ide.doc.wr_linux_4"
	export WIND_PREFERRED_PACKAGES="wrlinux-4"
	export WIND_RSS_CHANNELS="http://www.windriver.com/feeds/hardware_support_linux_2010.xml:http://www.windriver.com/feeds/wrlinux_400.xml"
	export WIND_SAMPLES="/opt/WindRiver40/wrlinux-4/samples"
	export WIND_TOOLS="/opt/WindRiver40/wrlinux-4"
	export WIND_USERMODE_AGENT_PATH="/opt/WindRiver40/linux-2.x/usermode-agent"
	export WIND_WBGIT="/opt/WindRiver40/wrlinux-4/scripts/wbgit.sh"
	export WIND_WRLINUX_BUILDDEPS_SCRIPT="/opt/WindRiver40/wrlinux-4/scripts/builddepsscript.sh"
	export WIND_WRLINUX_BUILDDIR_SCRIPT="/opt/WindRiver40/wrlinux-4/scripts/builddirscript.sh"
	export WIND_WRLINUX_GIT_SUBMENU_LAYOUT="/opt/WindRiver40/wrlinux-4/scripts/wbgitsubmenu.txt"
	export WIND_WRLINUX_LAUNCH_SCRIPT="/opt/WindRiver40/wrlinux-4/scripts/launch_script.sh"
	export WIND_WRLINUX_POST_PKG_SCRIPT="/opt/WindRiver40/wrlinux-4/scripts/quiltifyPackage.sh"
	export WIND_WRLINUX_QUERY_SCRIPT="/opt/WindRiver40/wrlinux-4/scripts/queryProperties.sh"
	export WIND_WRLINUX_RPMEXTRACT_SCRIPT="wtxtcl /opt/WindRiver40/wrlinux-4/scripts/extractRPMFile.tcl"
	export WIND_WRLINUX_SYSROOT_SCRIPT="sh /opt/WindRiver40/wrlinux-4/scripts/sysroot-test.sh"
	export WIND_WRLINUX_TARGET_BUTTONS="/opt/WindRiver40/wrlinux-4/scripts/wbblddefgen.txt"

	# TOOLCHAIN_PATH need to be set.
#	for ISYS
	export PATH=$PATH:/opt/WindRiver40/wrlinux-4/sysroots/mfp_sys210d-glibc_std/x86-linux2/
	export TOOLCHAIN_PATH=/opt/WindRiver40/wrlinux-4/sysroots/mfp_sys210d-glibc_std/sysroot/
	export TOOL_PREFIX=powerpc-wrs-linux-gnu-ppc_464-glibc_std-

elif [ "$PRODUCT" = "WEISS" -a "$WEISS_COMPILER_VERSION" = "GCC4.3.2" ]; then

	# for Weisshorn L4.0 with GCC4.3.2
	export PATH=/opt/WindRiver30/workbench-3.1/foundation//x86-linux2/wrbin:/opt/WindRiver30/wrlinux-3.0/layers/wrll-toolchain-4.3-85/powerpc/toolchain/x86-linux2/bin:/opt/WindRiver30:/opt/WindRiver30/utilities-1.0/x86-linux2/bin:/opt/WindRiver30/workbench-3.1/analysis/host/bin/x86-linux2:/opt/WindRiver30/workbench-3.1/x86-linux2/bin:$PATH
	export LD_LIBRARY_PATH=/opt/WindRiver30/workbench-3.1/analysis/host/bin/x86-linux2:/opt/WindRiver30/workbench-3.1/wrwb/platform/eclipse/x86-linux2/bin:/opt/WindRiver30/workbench-3.1/x86-linux2/lib:$LD_LIBRARY_PATH
	export WIND_BASE="/opt/WindRiver30/wrlinux-3.0"
	export WIND_DFW_PATH="/opt/WindRiver30/workbench-3.1/dfw/"
	export WIND_DOCS="/opt/WindRiver30/docs"
	export WIND_EXTENSIONS="/opt/WindRiver30/wrlinux-3.0/wrlinux/rtcore/extensions:/opt/WindRiver30/workbench-3.1/analysis/extensions:/opt/WindRiver30/workbench-3.1/wrsysviewer:/opt/WindRiver30/workbench-3.1/wrwb/tools:/opt/WindRiver30/workbench-3.1/wrwb/wrlinux:/opt/WindRiver30/workbench-3.1/wrwb/wrhv:/opt/WindRiver30/workbench-3.1/wrwb/vthreads:/opt/WindRiver30/workbench-3.1/wrwb/vxworksmilshae:/opt/WindRiver30/workbench-3.1/wrwb/vxworksmils:/opt/WindRiver30/workbench-3.1/wrwb/vxworkscert:/opt/WindRiver30/workbench-3.1/wrwb/vxworks653:/opt/WindRiver30/workbench-3.1/wrwb/vxworks55:/opt/WindRiver30/workbench-3.1/wrwb/vxworks:/opt/WindRiver30/workbench-3.1/wrwb/wrworkbench:/opt/WindRiver30/docs/extensions"
	export WIND_FOUNDATION_PATH="/opt/WindRiver30/workbench-3.1/foundation/"
	export WIND_HOME="/opt/WindRiver30"
	export WIND_HOST_TYPE="x86-linux2"
	export WIND_JRE_HOME="/opt/WindRiver30/jre/1.5.0_11/x86-linux2"
	export WIND_LINUX_BUILD="/opt/WindRiver30/wrlinux-3.0/wrlinux"
	export WIND_LINUX_CONFIGURE="/opt/WindRiver30/wrlinux-3.0/wrlinux/configure"
	export WIND_LX_HOME="/opt/WindRiver30"
	export WIND_PLATFORM="wrlinux-3.0"
	export WIND_PREFERRED_PACKAGES="wrlinux-3.0"
	export WIND_RTC_HOME="/opt/WindRiver30/wrlinux-3.0/layers/wrll-rtcore-5.2"
	export WIND_SAMPLES="/opt/WindRiver30/wrlinux-3.0/layers/wrll-rtcore-5.2/samples:/opt/WindRiver30/wrlinux-3.0/samples:/opt/WindRiver30/workbench-3.1/analysis/target/src/linux:/opt/WindRiver30/workbench-3.1/analysis/target/src/vxworks:/opt/WindRiver30/workbench-3.1/samples"
	export WIND_SCOPETOOLS_BASE="/opt/WindRiver30/workbench-3.1/analysis"
	export WIND_SCOPETOOLS_RPMS_BASE="/opt/WindRiver30/workbench-3.1/analysis/target/RPMS"
	export WIND_TOOLS="/opt/WindRiver30/workbench-3.1"
	export WIND_USERMODE_AGENT="/opt/WindRiver30/linux-2.x/usermode-agent/bin/usermode-agent.sh"
	export WIND_USERMODE_AGENT_PATH="/opt/WindRiver30/linux-2.x/usermode-agent"
	export WIND_UTILITIES="/opt/WindRiver30/utilities-1.0"
	export WIND_WRLINUX_BUILDDEPS_SCRIPT="/opt/WindRiver30/wrlinux-3.0/scripts/builddepsscript.sh"
	export WIND_WRLINUX_LAUNCH_SCRIPT="/opt/WindRiver30/wrlinux-3.0/scripts/launch_script.sh"
	export WIND_WRLINUX_LAYERS="/opt/WindRiver30/wrlinux-3.0/layers/wrll-analysis-1.0:/opt/WindRiver30/workbench-3.1/analysis/wrlinux"
	export WIND_WRLINUX_QUERY_SCRIPT="/opt/WindRiver30/wrlinux-3.0/scripts/queryProperties.sh"
	export WIND_WRLINUX_TARGET_BUTTONS="/opt/WindRiver30/wrlinux-3.0/scripts/wbblddefgen.txt"
	export WIND_WRSV_PATH="/opt/WindRiver30/workbench-3.1/wrsysviewer/"
	export WIND_WRWB_PATH="/opt/WindRiver30/workbench-3.1/wrwb/platform/eclipse"

	# TOOLCHAIN_PATH need to be set.
	export PATH=$PATH:/opt/WindRiver30/wrlinux-3.0/sysroots/mfp_sys470d-glibc_std/x86-linux2/
	export TOOLCHAIN_PATH=/opt/WindRiver30/wrlinux-3.0/sysroots/mfp_sys470d-glibc_std/sysroot
	export TOOL_PREFIX=powerpc-wrs-linux-gnu-ppc_440fp-glibc_std-

else
	echo "### Incorrect variables set to \"PRODUCT\" or \"MACHINE\" or \"WEISS_COMPILER_VERSION\" . ###"
	exit
fi

	export BUILD_SPEC=WINDRIVER_PPC74XX_GNU
	export TOOL_PATH=${TOOLCHAIN_PATH}/../x86-linux2
	export VPATH=${TOOLCHAIN_PATH}/usr/lib
	export CI_PROC_TYPE=__POWER_PC_750

echo "### The information of gcc which you selected ###"
`"$TOOL_PREFIX"gcc -v`

#Copying DefaultPrivateArea_EcoSS.xml to DefaultPrivateArea.xml only in case of BAIKAL S&S.
echo "EQUIPMENT_BRAND is $EQUIPMENT_BRAND"
if [ "$PRODUCT" = "LOIRE" -a "$SUB_PRODUCT" = "ECO_LOIRE" -a "$EQUIPMENT_BRAND" = "TOSHIBASS" ]; then
echo "Copying ECO LOIRE S&S Specific DefaultPrivateArea.xml"
cp -fp $EB2/build/common/bin/DefaultPrivateArea_EcoSS.xml $EB2/build/common/bin/DefaultPrivateArea.xml
fi


# ThirdParty layer
if [ "$BUILD_THIRDPARTY" = "1" ]; then
echo "### Building ThirdParty components ###"
folders=(
#$ROOT_DIR/ThirdParty
#$ROOT_DIR/ThirdParty/freetype
#$ROOT_DIR/ThirdParty/bdb
#$ROOT_DIR/ThirdParty/bison
#$ROOT_DIR/ThirdParty/cups
#$ROOT_DIR/ThirdParty/ftplib
#$ROOT_DIR/ThirdParty/gsoap
#$ROOT_DIR/ThirdParty/heapmm
#$ROOT_DIR/ThirdParty/mdnsresponder
#$ROOT_DIR/ThirdParty/mdnsresponder/mDNSResponder-107.6/mDNSPosix
#$ROOT_DIR/ThirdParty/heimdal
#$ROOT_DIR/ThirdParty/snmp
#$ROOT_DIR/ThirdParty/sqlite
#$ROOT_DIR/ThirdParty/ncpfs
#$ROOT_DIR/ThirdParty/FUSE
#$ROOT_DIR/ThirdParty/openslp
#$ROOT_DIR/ThirdParty/AutoIP-DHCP
#$ROOT_DIR/ThirdParty/resolver
#$ROOT_DIR/ThirdParty/libxml2
#$ROOT_DIR/ThirdParty/openssl
#$ROOT_DIR/ThirdParty/cyrussasl
#$ROOT_DIR/ThirdParty/ImageMagick
#$ROOT_DIR/ThirdParty/netatalk
#$ROOT_DIR/ThirdParty/openldap
#$ROOT_DIR/ThirdParty/curl
#$ROOT_DIR/ThirdParty/httpd
#$ROOT_DIR/ThirdParty/ftpserver
#$ROOT_DIR/ThirdParty/xslt
#$ROOT_DIR/ThirdParty/sendmail
#$ROOT_DIR/ThirdParty/samba
#$ROOT_DIR/ThirdParty/SDL
#$ROOT_DIR/ThirdParty/VNC
#$ROOT_DIR/ThirdParty/peg
#$ROOT_DIR/ThirdParty/inputmethod
#$ROOT_DIR/ThirdParty/inputmethod/anthy
#$ROOT_DIR/ThirdParty/inputmethod/scim
#$ROOT_DIR/ThirdParty/inputmethod/scim-anthy
#$ROOT_DIR/ThirdParty/wireless_tools
#$ROOT_DIR/ThirdParty/xsupplicant
#$ROOT_DIR/ThirdParty/IPsec
#$ROOT_DIR/ThirdParty/dibbler
#$ROOT_DIR/ThirdParty/bind9
#$ROOT_DIR/ThirdParty/EWB
#$ROOT_DIR/ThirdParty/GPL
#$ROOT_DIR/ThirdParty/GPL/netNCP
)
for folder in ${folders[@]}
do
	FOLDER_TOP=`echo "$folder" | awk '{print substr($0,1,1);}'`
	if [ ! "$FOLDER_TOP" = "#" -a -d $folder ]; then
		cd $folder;
		TARGET_MODULE_NAME=`echo $folder | sed 's/.*\///'`
		echo -e "\n______________________________________________________" ; \
		echo -e ">>>>> Making all in ./$TARGET_MODULE_NAME  <<<<<\n" ;            \
		make clean all
	fi
done

cd $ROOT_DIR
export BUILD_TYPE=thirdparty
make makelinks 2>/dev/null
export BUILD_TYPE=release
make makelinks 2>/dev/null

else
	echo "#######################################################"
	echo "          Skipping Thirdparty build"
	echo "#######################################################"
fi

# AL/CI layer
if [ "$BUILD_ALCI" = "1" ]; then
echo "### Building CI components ###"
folders=(
#$ROOT_DIR/dev/CI
#$ROOT_DIR/dev/CI/SoftwareDiagnostics/src
#$ROOT_DIR/dev/CI/SI
#$ROOT_DIR/dev/CI/OperatingEnvironment
#$ROOT_DIR/dev/CI/OperatingEnvironment/src
#$ROOT_DIR/dev/CI/OperatingEnvironment64
#$ROOT_DIR/dev/CI/MessagingSystem
#$ROOT_DIR/dev/CI/HierarchicalDB
#$ROOT_DIR/dev/CI/HierarchicalDB/MMAP
#$ROOT_DIR/dev/CI/IndexedDB
#$ROOT_DIR/dev/CI/IndexedDB/Mash/src
#$ROOT_DIR/dev/CI/IndexedDB/NvramBackupRestore
#$ROOT_DIR/dev/CI/SystemInformation
#$ROOT_DIR/dev/CI/Codecs
#$ROOT_DIR/dev/CI/DiagnosticsModeUtility
#$ROOT_DIR/dev/CI/ServiceStartupManager
#$ROOT_DIR/dev/CI/ServiceStartupManager/src
#$ROOT_DIR/dev/CI/HierarchicalDB/src
#$ROOT_DIR/dev/CI/RelationalDB
#$ROOT_DIR/dev/CI/Scheduler
#$ROOT_DIR/dev/CI/PriorityManager
#$ROOT_DIR/dev/CI/PriorityManager/src
#$ROOT_DIR/dev/CI/PriorityManager/test
#$ROOT_DIR/dev/CI/OperatingEnvironment/SMM
#$ROOT_DIR/dev/CI/OperatingEnvironment/test
#$ROOT_DIR/dev/CI/PresentationResources
#$ROOT_DIR/dev/CI/DataTransformation
#$ROOT_DIR/dev/CI/DataStream
#$ROOT_DIR/dev/CI/BoxDocument
#$ROOT_DIR/dev/CI/BoxDocument/src
#$ROOT_DIR/dev/CI/BoxDocument/test
#$ROOT_DIR/dev/CI/LicenseManager
#$ROOT_DIR/dev/CI/SoftwareUpgradeManager
#$ROOT_DIR/dev/CI/ConditionHandler
#$ROOT_DIR/dev/CI/BootModeSensor
#$ROOT_DIR/dev/CI/NetworkClients
#$ROOT_DIR/dev/CI/NetworkClients/Utilities
#$ROOT_DIR/dev/CI/NetworkClients/ShmemUtil
#$ROOT_DIR/dev/CI/NetworkClients/POP3Client
#$ROOT_DIR/dev/CI/NetworkClients/SMTPClient
#$ROOT_DIR/dev/CI/NetworkClients/FTPClient
#$ROOT_DIR/dev/CI/NetworkClients/LDAPClient/src
#$ROOT_DIR/dev/CI/NetworkClients/NCPClient
#$ROOT_DIR/dev/CI/NetworkClients/SambaSendService
#$ROOT_DIR/dev/CI/MfpUtility
#$ROOT_DIR/dev/CI/MfpUtility/src
#$ROOT_DIR/dev/CI/BoxInvalidCombination
#$ROOT_DIR/dev/CI/BoxInvalidCombination/src
#$ROOT_DIR/dev/CI/SecureMessaging
#$ROOT_DIR/dev/CI/SecureMessaging/src
#$ROOT_DIR/dev/CI/LogManager
#$ROOT_DIR/dev/CI/SystemResourceManager/src
#$ROOT_DIR/dev/CI/DeviceAuthentication
#$ROOT_DIR/dev/CI/DeviceAuthentication/src
#$ROOT_DIR/dev/CI/PlatformSI/SignatureCheck
#$ROOT_DIR/dev/CI/PlatformSI/SecuritySetting
)

for folder in ${folders[@]}
do
	FOLDER_TOP=`echo "$folder" | awk '{print substr($0,1,1);}'`
	if [ ! "$FOLDER_TOP" = "#" -a -d $folder ]; then
		cd $folder;
		TARGET_MODULE_NAME=`echo $folder | sed 's/.*\///'`
		echo -e "\n______________________________________________________" ; \
		echo -e ">>>>> Making all in ./$TARGET_MODULE_NAME  <<<<<\n" ;            \
		make clean all
	fi
done

echo "### Building AL components ###"
folders=(
#$ROOT_DIR/dev/AL
#$ROOT_DIR/dev/AL/SoftwareFunction/USBApplication/USBInterface
#$ROOT_DIR/dev/AL/Utility/ConditionParser
#$ROOT_DIR/dev/AL/UIController
#$ROOT_DIR/dev/AL/UIController/src
#$ROOT_DIR/dev/AL/SoftwareFunction/ViewPlugin/ViewPluginLib
#$ROOT_DIR/dev/AL/Utility
#$ROOT_DIR/dev/AL/Utility/DeltaModifier
#$ROOT_DIR/dev/AL/Utility/DocRead
#$ROOT_DIR/dev/AL/Utility/DocWrite
#$ROOT_DIR/dev/AL/Utility/DeviceUUIDGen
#$ROOT_DIR/dev/AL/Utility/iniparser
#$ROOT_DIR/dev/AL/Utility/ImportDB
#$ROOT_DIR/dev/AL/Utility/DiagnosticLibrary
#$ROOT_DIR/dev/AL/Utility/SetInitialLocale
#$ROOT_DIR/dev/AL/Maintenance/FileStorageManager
#$ROOT_DIR/dev/AL/SoftwareFunction/AddressBookLibrary
#$ROOT_DIR/dev/AL/OKI_PJLParser
#$ROOT_DIR/dev/AL/Spooler/src
#$ROOT_DIR/dev/AL/Spooler/taprn
#$ROOT_DIR/dev/AL/JobController
#$ROOT_DIR/dev/AL/JobController/WorkFlowDataList
#$ROOT_DIR/dev/AL/JobController/SessionDataList
#$ROOT_DIR/dev/AL/JobController/ParameterHandler
#$ROOT_DIR/dev/AL/JobController/ActionManager
#$ROOT_DIR/dev/AL/PrintJobManager
#$ROOT_DIR/dev/AL/OKI_PJLManager
#$ROOT_DIR/dev/AL/FrontPanel
#$ROOT_DIR/dev/AL/FrontPanel/GUI_Precompiler
#$ROOT_DIR/dev/AL/FrontPanel/FrontPanelLib
#$ROOT_DIR/dev/AL/FrontPanel/Renderer/src
#$ROOT_DIR/dev/AL/FrontPanel/GenericPanel
#$ROOT_DIR/dev/AL/FrontPanel/FontManager
#$ROOT_DIR/dev/AL/Network
#$ROOT_DIR/dev/AL/Network/utility
#$ROOT_DIR/dev/AL/Network/Utilities
#$ROOT_DIR/dev/AL/Network/Utilities/ShmemUtil
#$ROOT_DIR/dev/AL/Network/Utilities/MsgUtil
#$ROOT_DIR/dev/AL/Network/NSM
#$ROOT_DIR/dev/AL/Network/lpd
#$ROOT_DIR/dev/AL/Network/ipp
#$ROOT_DIR/dev/AL/Network/ipp/ipplib
#$ROOT_DIR/dev/AL/Network/hp9100
#$ROOT_DIR/dev/AL/Network/DPWS
#$ROOT_DIR/dev/AL/Network/DPWS/wsd
#$ROOT_DIR/dev/AL/Network/DPWS/wsmex
#$ROOT_DIR/dev/AL/Network/DPWS/wprt
#$ROOT_DIR/dev/AL/Network/DPWS/wscn
#$ROOT_DIR/dev/AL/Network/LLTD
#$ROOT_DIR/dev/AL/Network/eMailCtrl
#$ROOT_DIR/dev/AL/Network/nprint
#$ROOT_DIR/dev/AL/Network/nprint/sock
#$ROOT_DIR/dev/AL/Network/nprint/common
#$ROOT_DIR/dev/AL/Network/nprint/logger
#$ROOT_DIR/dev/AL/Network/nprint/spooler
#$ROOT_DIR/dev/AL/Network/nprint/sapd
#$ROOT_DIR/dev/AL/Network/nprint/spxserver
#$ROOT_DIR/dev/AL/Network/nprint/pserver
#$ROOT_DIR/dev/AL/Network/nprint/nprinter
#$ROOT_DIR/dev/AL/Network/Bonjour
#$ROOT_DIR/dev/AL/Network/Bonjour/BonjourConfMgr
#$ROOT_DIR/dev/AL/Network/Bonjour/BonjourManager
#$ROOT_DIR/dev/AL/Network/Bonjour/BonjourServer
#$ROOT_DIR/dev/AL/Network/PowerManagement
#$ROOT_DIR/dev/AL/Network/NMO
#$ROOT_DIR/dev/AL/Network/NMO/Generic
#$ROOT_DIR/dev/AL/Network/NMO/ipp
#$ROOT_DIR/dev/AL/Network/NMO/snmp
#$ROOT_DIR/dev/AL/Network/NMO/webserver
#$ROOT_DIR/dev/AL/Network/NMO/smb
#$ROOT_DIR/dev/AL/Network/NMO/8021X
#$ROOT_DIR/dev/AL/Network/NMO/ipsec
#$ROOT_DIR/dev/AL/Network/NMO/FTPServer
#$ROOT_DIR/dev/AL/Network/NMO/tcpip
#$ROOT_DIR/dev/AL/Network/NMO/kerberos
#$ROOT_DIR/dev/AL/Network/NMO/DirectoryService
#$ROOT_DIR/dev/AL/Network/NMO/networkadapter
#$ROOT_DIR/dev/AL/Network/NMO/Bonjour
#$ROOT_DIR/dev/AL/Network/NMO/SLP
#$ROOT_DIR/dev/AL/Network/NMO/DDNS
#$ROOT_DIR/dev/AL/Network/NMO/IPv6
#$ROOT_DIR/dev/AL/Network/NMO/PingTraceroute
#$ROOT_DIR/dev/AL/Network/NMO/eBR
#$ROOT_DIR/dev/AL/Network/NMO/DNS
#$ROOT_DIR/dev/AL/Network/NMO/RemoteApplications
#$ROOT_DIR/dev/AL/Network/NMO/PowerManagement
#$ROOT_DIR/dev/AL/Network/NMO/Wireless
#$ROOT_DIR/dev/AL/Network/smb/SambaPrintingPlugin
#$ROOT_DIR/dev/AL/Network/bluetooth
#$ROOT_DIR/dev/AL/Network/bluetooth/btApplication
#$ROOT_DIR/dev/AL/Network/llmnr
#$ROOT_DIR/dev/AL/Network/iFaxRx
#$ROOT_DIR/dev/AL/Network/faxofframp
#$ROOT_DIR/dev/AL/Network/USBPrintConnector
#$ROOT_DIR/dev/AL/Network/IPv6
#$ROOT_DIR/dev/AL/Network/FTPPrint
#$ROOT_DIR/dev/AL/Network/PingTraceroute
#$ROOT_DIR/dev/AL/Network/telnet
#$ROOT_DIR/dev/AL/Network/SLP
#$ROOT_DIR/dev/AL/Network/RemoteApplications
#$ROOT_DIR/dev/AL/Network/RemoteApplications/EFI
#$ROOT_DIR/dev/AL/Network/RemoteApplications/Common
#$ROOT_DIR/dev/AL/Network/RemoteApplications/Common/Parser
#$ROOT_DIR/dev/AL/Network/RemoteApplications/Common/Generator
#$ROOT_DIR/dev/AL/Network/RemoteApplications/RemoteIF
#$ROOT_DIR/dev/AL/Network/RemoteApplications/EFIWalkUPScan
#$ROOT_DIR/dev/AL/Network/RemoteApplications/Common/efiRemoteIFSR
#$ROOT_DIR/dev/AL/Network/eBR
#$ROOT_DIR/dev/AL/Network/llmnrInterface
#$ROOT_DIR/dev/AL/Network/llmnrInterface/src
#$ROOT_DIR/dev/AL/Network/llmnrInterface/src/getaddrinfo-hook
#$ROOT_DIR/dev/AL/Network/llmnrInterface/src/getnameinfo-hook
#$ROOT_DIR/dev/AL/SoftwareFunction
#$ROOT_DIR/dev/AL/SoftwareFunction/UserManagerLibrary
#$ROOT_DIR/dev/AL/SoftwareFunction/AccountingLibrary
#$ROOT_DIR/dev/AL/SoftwareFunction/Accounting
#$ROOT_DIR/dev/AL/SoftwareFunction/AddressBook
#$ROOT_DIR/dev/AL/SoftwareFunction/SoftwareUpgrade
#$ROOT_DIR/dev/AL/SoftwareFunction/eFiling
#$ROOT_DIR/dev/AL/SoftwareFunction/JobTemplates
#$ROOT_DIR/dev/AL/SoftwareFunction/MailBox/src
#$ROOT_DIR/dev/AL/SoftwareFunction/LogRetriever
#$ROOT_DIR/dev/AL/SoftwareFunction/RbacManagerLibrary
#$ROOT_DIR/dev/AL/SoftwareFunction/GroupManagerLibrary
#$ROOT_DIR/dev/AL/SoftwareFunction/UserExportImportLibrary
#$ROOT_DIR/dev/AL/SoftwareFunction/SecurityExportImportLibrary
#$ROOT_DIR/dev/AL/SoftwareFunction/Authentication
#$ROOT_DIR/dev/AL/SoftwareFunction/SecurePDFLibrary
#$ROOT_DIR/dev/AL/SoftwareFunction/USBApplication
#$ROOT_DIR/dev/AL/SoftwareFunction/SecuritySettings
#$ROOT_DIR/dev/AL/SoftwareFunction/RestrictionMode
#$ROOT_DIR/dev/AL/SoftwareFunction/UserManager
#$ROOT_DIR/dev/AL/SoftwareFunction/ExportImport
#$ROOT_DIR/dev/AL/SoftwareFunction/ViewPlugin
#$ROOT_DIR/dev/AL/SoftwareFunction/ViewPlugin/ViewPluginLib
#$ROOT_DIR/dev/AL/SoftwareFunction/GroupManager
#$ROOT_DIR/dev/AL/SoftwareFunction/RbacManager
#$ROOT_DIR/dev/AL/SoftwareFunction/LogManager
#$ROOT_DIR/dev/AL/SoftwareFunction/PDLFilters
#$ROOT_DIR/dev/AL/SoftwareFunction/CloningPlugin
#$ROOT_DIR/dev/AL/SoftwareFunction/PresentationResourceManager
#$ROOT_DIR/dev/AL/SoftwareFunction/LicenseMgmt
#$ROOT_DIR/dev/AL/SoftwareFunction/ServiceUIPlugin
#$ROOT_DIR/dev/AL/SoftwareFunction/IntegrityCheck
#$ROOT_DIR/dev/AL/SoftwareFunction/DevAuthMgmtPlugIn
#$ROOT_DIR/dev/AL/SoftwareFunction/PanelUIMessageHandler
#$ROOT_DIR/dev/AL/Reporting
#$ROOT_DIR/dev/AL/Reporting/ReportsMessenger
#$ROOT_DIR/dev/AL/Reporting/ReportManager
#$ROOT_DIR/dev/AL/Network/snmp
#$ROOT_DIR/dev/AL/Network/snmp/productiontoolmib
#$ROOT_DIR/dev/AL/DeviceInformationManager/PlugIns/DeviceService
#$ROOT_DIR/dev/AL/DeviceInformationManager/PlugIns/Configuration/src
#$ROOT_DIR/dev/AL/DeviceFunctions/Maintenance
#$ROOT_DIR/dev/AL/ApplicationServers
#$ROOT_DIR/dev/AL/ApplicationServers/ContentWebServer
#$ROOT_DIR/dev/AL/ApplicationServers/FileDownloader
#$ROOT_DIR/dev/AL/ApplicationServers/eFIRemoteIFServer
#$ROOT_DIR/dev/AL/TopAccess
#$ROOT_DIR/dev/AL/Stage2
#$ROOT_DIR/dev/AL/AppAccessor
#$ROOT_DIR/dev/AL/GSI
#$ROOT_DIR/Resources
#$ROOT_DIR/Resources/Templates
#$ROOT_DIR/dev/tool/GenNvmAll
)
for folder in ${folders[@]}
do
	FOLDER_TOP=`echo "$folder" | awk '{print substr($0,1,1);}'`
	if [ ! "$FOLDER_TOP" = "#" -a -d $folder ]; then
		cd $folder;
		TARGET_MODULE_NAME=`echo $folder | sed 's/.*\///'`
		echo -e "\n______________________________________________________" ; \
		echo -e ">>>>> Making all in ./$TARGET_MODULE_NAME  <<<<<\n" ;            \
		make clean all
	fi
done

else
	echo "#######################################################"
	echo "          Skipping ALCI build"
	echo "#######################################################"
fi


# SL_DL layer
if [ "$BUILD_SL" = "1" ]; then
echo "### Building SL and Mishima components ###"
folders=(
#$ROOT_DIR/dev/SL/ARMN
#$ROOT_DIR/dev/SL/NotificationService
#$ROOT_DIR/dev/SL/InformationService
#$ROOT_DIR/dev/SL/JobListManager
#$ROOT_DIR/dev/SL/SLInformationManager
#$ROOT_DIR/dev/SL/DefaultPDLParser
#$ROOT_DIR/dev/SL/WFCtrlService
#$ROOT_DIR/dev/SL/AE/AGTE_EmailSend
#$ROOT_DIR/dev/SL/AE/AGTE_FTPSend
#$ROOT_DIR/dev/SL/AE/AGTE_iFaxSend
#$ROOT_DIR/dev/SL/AE/AGTE_ListDataCreateToPDL
#$ROOT_DIR/dev/SL/AE/AGTE_MailDivide
#$ROOT_DIR/dev/SL/AE/AGTE_NetWareSend
#$ROOT_DIR/dev/SL/AE/AGTE_SambaSend
#$ROOT_DIR/dev/SL/AE/AGTE_SaveToLocalStorage
#$ROOT_DIR/dev/SL/AE/AGTE_SaveToRemovableStorage
#$ROOT_DIR/dev/SL/AE/AGTE_Scan
#$ROOT_DIR/dev/SL/AE/AGTE_FAXReceive
#$ROOT_DIR/dev/SL/AE/AGTE_RIP
#$ROOT_DIR/dev/SL/AE/AGTE_CnvtToTIFF
#$ROOT_DIR/dev/SL/AE/AGTE_Print
#$ROOT_DIR/dev/SL/AE/AGTE_FAXTransmit
#$ROOT_DIR/dev/SL/AE/AGTE_CnvtToPDL
#$ROOT_DIR/dev/SL/AE/AGTE_ImageEdit
#$ROOT_DIR/dev/SL/AE/AGTE_PreviewMake
#$ROOT_DIR/dev/SL/AE/AGTE_CnvtToPDF
#$ROOT_DIR/dev/SL/AE/AGTE_CnvtToXPS
#$ROOT_DIR/dev/SL/AE/AGTE_CnvtToSlimPDF
#$ROOT_DIR/dev/SL/AE/AGTE_CnvtToDIB
#$ROOT_DIR/dev/SL/AE/AGTE_ThumbnailMake
#$ROOT_DIR/dev/SL/AE/AGTE_EFI_Print
#$ROOT_DIR/dev/DL/PipeMN
#$ROOT_DIR/dev/DL/Pipe
#$ROOT_DIR/dev/DL/iccpLib
#$ROOT_DIR/dev/DL/sysASICLib
#$ROOT_DIR/dev/DL/cmnIPLib
#$ROOT_DIR/dev/DL/DIMLib
#$ROOT_DIR/dev/DL/fuLib
#$ROOT_DIR/dev/DL/AnnotationLibWrapper
#$ROOT_DIR/dev/DL/SkuldLib
#$ROOT_DIR/dev/DL/DIM
#$ROOT_DIR/dev/DL/CPE
#$ROOT_DIR/dev/DL/DEM
#$ROOT_DIR/dev/DL/PE/PE_EmailSend
#$ROOT_DIR/dev/DL/PE/PE_CnvtToTIFF
#$ROOT_DIR/dev/DL/PE/PE_RIP
#$ROOT_DIR/dev/DL/PE/PE_FTPSend
#$ROOT_DIR/dev/DL/PE/PE_iFaxSend
#$ROOT_DIR/dev/DL/PE/PE_NetWareSend
#$ROOT_DIR/dev/DL/PE/PE_SambaSend
#$ROOT_DIR/dev/DL/PE/PE_SaveToLocalStorage
#$ROOT_DIR/dev/DL/PE/PE_SaveToRemovableStorage
#$ROOT_DIR/dev/DL/PE/PE_CnvtToPDF
#$ROOT_DIR/dev/DL/PE/PE_MailDivide
#$ROOT_DIR/dev/DL/PE/PE_ThumbnailMake
#$ROOT_DIR/dev/DL/PE/PE_EFI_Print
#$ROOT_DIR/dev/DL/SE/SE_RIP
#$ROOT_DIR/dev/DL/SE/SE_CnvtToTIFF
#$ROOT_DIR/dev/DL/SE/SE_EmailSend
#$ROOT_DIR/dev/DL/SE/SE_FTPSend
#$ROOT_DIR/dev/DL/SE/SE_iFaxSend
#$ROOT_DIR/dev/DL/SE/SE_NetWareSend
#$ROOT_DIR/dev/DL/SE/SE_SambaSend
#$ROOT_DIR/dev/DL/SE/SE_SaveToLocalStorage
#$ROOT_DIR/dev/DL/SE/SE_SaveToRemovableStorage
#$ROOT_DIR/dev/DL/SE/SE_CnvtToPDF
#$ROOT_DIR/dev/DL/SE/SE_CnvtToPDL
#$ROOT_DIR/dev/DL/SE/SE_CodeCnvt
#$ROOT_DIR/dev/DL/SE/SE_MimeAnalyse
#$ROOT_DIR/dev/DL/SE/SE_CnvtToPNG
#$ROOT_DIR/dev/DL/SE/SE_ImgColorCnvt
#$ROOT_DIR/dev/DL/SE/SE_ImgRotate
#$ROOT_DIR/dev/DL/SE/SE_ImgShrink
#$ROOT_DIR/dev/DL/SE/SE_CnvtToPipe
#$ROOT_DIR/dev/DL/SE/SE_SaveToDocStore
#$ROOT_DIR/dev/DL/PE/PE_SaveToDocStore
#$ROOT_DIR/dev/SL/AE/AGTE_SaveToDocStore
#$ROOT_DIR/dev/DL/PE/PE_ImageEdit
#$ROOT_DIR/dev/DL/SE/SE_EncodeBWToMH
#$ROOT_DIR/dev/SL/AE/AGTE_CnvtToPipe
#$ROOT_DIR/dev/DL/PE/PE_CnvtToPipe
#$ROOT_DIR/dev/DL/SE/SE_ImageCnvt
#$ROOT_DIR/dev/DL/SE/SE_EncodeBWToMMR
#$ROOT_DIR/dev/DL/PE/PE_CnvtToXPS
#$ROOT_DIR/dev/DL/SE/SE_CnvtToXPS
#$ROOT_DIR/dev/DL/PE/PE_CnvtToSlimPDF
#$ROOT_DIR/dev/DL/SE/SE_DecodeJPEGToRaw
#$ROOT_DIR/dev/DL/SE/SE_CnvtToSlimPDF
#$ROOT_DIR/dev/DL/PE/PE_PreviewMake
#$ROOT_DIR/dev/DL/SE/SE_CnvtToDIB
#$ROOT_DIR/dev/DL/PE/PE_CnvtToDIB
#$ROOT_DIR/dev/DL/PE/PE_FaxReceive_Wrapper
#$ROOT_DIR/dev/DL/PE/PE_FaxTransmit_Wrapper
#$ROOT_DIR/dev/DL/PE/PE_Print_Wrapper
#$ROOT_DIR/dev/DL/PE/PE_Scan_Wrapper
#$ROOT_DIR/dev/DL/SE/SE_DecodeToRaw
#$ROOT_DIR/dev/SL/AE/AGTE_PrintPreProc
#$ROOT_DIR/dev/DL/PE/PE_PrintPreProc
#$ROOT_DIR/dev/DL/SE/SE_PrintPreProc
#$ROOT_DIR/dev/DL/DE/DE_MEP
#$ROOT_DIR/dev/DL/SE/SE_CopyProtection
)
for folder in ${folders[@]}
do
	cd $folder;
	if [ ! -d "obj" ]; then 
		mkdir obj;
	fi
	TARGET_MODULE_NAME=`echo $folder | sed 's/.*\///'`
	echo -e "\n______________________________________________________" ; \
	echo -e ">>>>> Making $TARGET_MODULE_NAME  <<<<<\n" ;            \
	export EBX_SL_OBJ_PATH=$ROOT_DIR/dev/SL/WFCtrlService/obj/
        mkdir $EBX_SL_OBJ_PATH
        $MAKE veryclean
        $MAKE MODWORK_DIR=$folder/obj/
done
else
	echo "#######################################################"
	echo "          Skipping SL and MSM Components build"
	echo "#######################################################"
fi

# DL_DL layer
if [ "$BUILD_DLDL" = "1" ]; then
	echo "#### Building DL-DL components ###"
	# eB3 (DL)
	TOP_DIR=$ROOT_DIR/dev/DL/eB3_Wrapper/
	export EBX_DL_OBJ_PATH=$ROOT_DIR/dev/DL/eB3_Wrapper/TEMP/
	mkdir $EBX_DL_OBJ_PATH
	cd $TOP_DIR
	$MAKE veryclean 
	$MAKE all

else
	echo "#######################################################"
	echo "          Skipping  DL-DL Components build"
	echo "#######################################################"
fi

cp -a $ROOT_DIR/build/$BUILD_TYPE/sbin/* $RELEASE_BINDIR

cd $ROOT_DIR
echo "### Build Finish ###"

./count.sh
