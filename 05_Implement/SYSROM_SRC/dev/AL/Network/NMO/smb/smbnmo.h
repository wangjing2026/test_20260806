/******************************************************************************
// Copyright 2008 TOSHIBA TEC CORPORATION All rights reserved //

*
* $Workfile$
* $Revision$
* $Date$
* $Author$
* $Logfile$
******************************************************************************/

#include <map>
#include <sys/types.h>
#include "../../NSM/nmo.h"	   
#include "../../NSM/cini.h"    
#include "cinfparser.h"
#include <fstream>
#include "../../NSM/cuiccommand.h"
#include "AL/UIController/boproxy.h"
#include <CI/SI/ssdksecuritymanagerinterface.h>
#include <CI/SI/ssdkusertokeninterface.h>
#define WINDOWSNT "$WINDOWS NT$"
#define WINDOWS95 "$WINDOWS 95$"
#define ALLWINDOWS "$CHICAGO$"
#define DEV_CONF_PLUG	"DEVICE_CONFIG_PLUGIN_SESSION"
#define OKI 1
#if 0
#define DRIVERPATH1 "/work/drivers/pointNprint/W32X86/"
#define DRIVERPATH2 "/work/drivers/pointNprint/WIN40/"
#define DRIVERPATH3 "/work/drivers/pointNprint/W32X86/2/"
#define DRIVERPATH4 "/work/drivers/pointNprint/W32X86/3/"
#define DRIVERPATH5 "/work/drivers/pointNprint/WIN40/0/"
#define DRIVERPATH6 "/work/drivers/pointNprint/x64/"
#define DRIVERPATH7 "/work/drivers/pointNprint/x64/2"
#define DRIVERPATH8 "/work/drivers/pointNprint/x64/3"
#define DRIVERPATHTMP "/work/drivers/pointNprint/tmp/"
#endif

#define SMB_DRIVER_PATH		"/work/drivers/pointNprint/"
#define SMB_DRIVER_PS		"/work/drivers/pointNprint/PS/"
#define SMB_DRIVER_UNI		"/work/drivers/pointNprint/UNI/"
#define SMB_DRIVER_XPS		"/work/drivers/pointNprint/XPS/"
#define SMB_DRIVER_PLUGIN	"/work/drivers/pointNprint/PLUGINS/"

#define SMB_XPS_DRIVER_PKG		"XPS.ZIP"
#define SMB_PS_DRIVER_PKG		"PS3.ZIP"
#define SMB_UNI_DRIVER_PKG		"UNIV.ZIP"
//For OKI driver name is PCL6.zip
#define SMB_UNI_DRIVER_PKG_OKI_C		"PCL6.ZIP"
#define SMB_DRIVER_PLUGIN_PKG		"PLUGINS.ZIP"

#define SMB_PS_CONFIG			"eSPDPS.cfg"
#define SMB_XPS_CONFIG			"eSPDXPS.cfg" 
#define SMB_UNI_CONFIG			"eSPDUNI.cfg" 
//Below are the driver configuration file for OKI_Mash
#define SMB_PS_CONFIG_OKI_M			"OKPDPS.cfg"
#define SMB_XPS_CONFIG_OKI_M			"OKPDXPS.cfg" 
#define SMB_UNI_CONFIG_OKI_M			"OKPDUNI.cfg" 
//Below are the driver configuration file for OKI_Loire
#define SMB_PS_CONFIG_OKI_L			"OLPDPS.cfg"
#define SMB_XPS_CONFIG_OKI_L			"OLPDXPS.cfg" 
#define SMB_UNI_CONFIG_OKI_L			"OLPDUNI.cfg" 

#define TCP_IP_CONF_ETH			"ifcfg-eth0"
#define TCP_IP_CONF_WLAN		"ifcfg-wln0"

#define SMB_GET_DRIVER_PATH		0
#define SMB_GET_DRIVER_FILE_NAME	1
#define SMB_GET_DRIVER_NAME		2

#define SMB_SNMP_VERSION_1		"1"
#define SMB_SNMP_VERSION_3              "3"

#define	SMB_READ_IP			"Protocols/TCP-IP/ip"
#define	SMB_WLN_ENABLE			"Adapters/Wireless/enabled"
#define SMB_READ_SNMP_V1_2		"Protocols/SNMP/snmpV1V2Enable"
#define SMB_READ_SNMP_V3		"Protocols/SNMP/snmpV3Enabled"
#define SMB_WRITE_SNMP_COM		"Protocols/SNMP/writeCommunityName"

#define DRIVERNOBUILDITEM "/NoBuildItems/AL/Network/pointNprint/"
#define DRIVERNTTDB	"/ramdisk/al/network/samba/"
#define DRIVERPRINTTDB "/ramdisk/al/network/samba/printing/"
#define DRIVER_64 "NTamd64" 
#define DRIVERFILEEXTENSION	"zip"

#define COMMAND_NODE                    "commandNode"
#define DOC_NAME                        "docName"
#define DELTA_DOC_NAME                  "deltaDocName"
#define DRIVER_UPDATE_CMD_DOC		"UpdateMSGDoc"

#define FILESTORAGEPATH "FileStorages/FileStorage[name='PointandPrint']/path"

#define PEDESTAL1_XPATH "MFP/Printer/PaperFeeder/Drawer3/Installation" 
#define PEDESTAL2_XPATH "MFP/Printer/PaperFeeder/Drawer4/Installation"
#define LCF_XPATH "MFP/Printer/PaperFeeder/LCF/Installation"

#define FINISHER_XPATH "MFP/Printer/Finisher/Installation"
#define STAPLE_XPATH "MFP/Printer/Finisher/StapleUnit/Installation"
#define SADDLESTITCH_XPATH "MFP/Printer/Finisher/SaddleStitchUnit/Installation"
#define HOLEPUNCH_XPATH "MFP/Printer/Finisher/HolePuncher/Installation"
#define INSERTER_XPATH "MFP/Printer/Finisher/Inserter/Installation"

#define FINISHER_NAME "MFP/Printer/Finisher/Name"
#define HOLEPUNCH_NAME "MFP/Printer/Finisher/HolePuncher/ModelName"

#define FINISHER_NONE "0"
#define HOLEPUNCH_NONE "0"

#define PEDESTAL_NONE "0"
#define PEDESTAL_PEDESTAL1 "1"
#define PEDESTAL_PEDESTAL2 "2"
#define PEDESTAL_LCF "3"

#define HOLE_PUNCH_NONE "0"

#define INDEX_ONE 1
#define INDEX_ZERO 0
#define INDEX_THREE 3
#define INDEX_FIVE 5
#define BUFFER_LIMIT 2000
#define MIN_SAMBA "1"
#define MAX_SAMBA "50"
#define DEFAULT_SAMBA "13"
#define DRIVER_INST_REQ	"1"	
#define DOMAINXPATH "Network/Protocols/SMB/enableDomainLogin"
#define PASSXPATH "Network/Protocols/SMB/password"
#define USERXPATH "Network/Protocols/SMB/logonUser"
#define SAMBA_ENABLE	"1"
#define SAMBA_DISABLE	"0"
#define NO_DRIVER 0
#define ONLY_XPS 1
#define ONLY_PS3 2
#define ONLY_PS3_XPS 3
#define ONLY_UNIV 4
#define ONLY_UNIV_XPS 5
#define ONLY_UNIV_PS3 6
#define ONLY_UNIV_PS3_XPS 7
#define ERROR -1
#define MAX_DRIVER_FILES 8

//20150109: To reduce HDD Access, log path is moved to /ramdisk
//#define SAMBA_LOG_PATH "/work/log/al/"
//#define SAMBA_LOG_FILE "/ramdisk/al/network/log/log.smbd"
/*20170620: when log file name is set, nmbd logs are dumping in log.smbd.
 *This exists in original samba. Better not to set log file name,
 *We may loose smbd informative logs if huge nmbd logs are dumped in log.smbd*/
#define SAMBA_LOG_FILE ""
#define SAMBA_VERSION_PATH "/ramdisk/al/network/samba/smb.version.conf"
#define SAMBA_VERSION_NOBUILD_PATH "/home/SYSROM_SRC/NoBuildItems/AL/Network/smb.version.conf"

/*20160614: send notification on xpath change ,snmp gets Notification from BOserver on below xpath change*/
#define SAMBA_NOTIFY_XPATH  "Network/Protocols/SMB/deviceName"

using namespace std;
using namespace ci::operatingenvironment;
using namespace ci::hierarchicaldb;
using namespace ssdk;
using namespace dom; // located in spec/ci/hierarchicaldb/dom
using namespace al::network;
using namespace al::uicontroller;
using namespace ci::codecs;

namespace al
{
namespace network
{
namespace nmo
{
//class CSmbNMO : public NMO
class CSmbNMO : public CNMOBase
{
		DocumentRef m_configDoc;
 		al::uicontroller::BOProxyRef m_pBO;
		static map<string, string>xPath_config_map;
		static map<string, string>xPath_Value_map;

	public:

		static void initXPathMapping(void);
		static CString mapXPath(CString xPath);
		static void initValueMapping(void);

		/*
		* Return mapped value from the "xPath_Value_map" table defined, if no match found return empty string
		*/
		static CString mapValue(CString value);

		/*
		* Return true if the status is "Installed" for the given xPath else false
		*/
		static bool isInstalled(CString xPath);

		/*
		* Return installed pedestal or "0"
		*/
		static CString getDrawers(void);

		/*
		* Return if the LCF is installed or empty string
		*/
		static CString getLCF(void);

		/*
		* Get the Name of Finisher installed
		* PS: Not all the finishers are implemented now
		*/
		static CString getFinishers(void);
		static CString getHolePuncher(void);

		Status SavetoConfig(CString xPath, CString val);
		void webInstallerDisable(CIniFile&); //WebInstaller Disable for SCC
		CString Update(dom::DocumentRef configDoc, ElementRef transRoot,
		CString &depResult,CString startMode);
                void UpdateSambaMap();
		void enableMFPFunction(ElementRef );
		void executePanelComand(DocumentRef ,CString);
		CString ValidateSmbConf();

		CString IdentifyDriver(CString, int);
		CString PrepareQuoteString(CString str);
		Status InstallPrinterDriver(CString driverFilePath, CString);
		void CleanDriverFolder (CString );
		Status ModifyConfiguration (CString);
		ElementRef getNetworkNodeNode (ElementRef);
		void ReadConfigParameter (ElementRef);
		/*RCR_587: Delete users for guestLogon */
		CString DBUsersDeletion(void);
		bool UpdateSambaLogInfo();
		Status SetDefaultValue(CString SerialNo,CString xpath,dom::DocumentRef configDoc);
		Status RestoreSMBSetting(DocumentRef);
		CString HandleUICCommand(CUICCommandRef command); 
		CString executeSSDCommand (CUICCommandRef );
		bool setSSCCommandPath(CString , CString , CString , CString);
		CString WritableName (CString);
		CString RestoreDefaults(dom::ElementRef defaultRoot, dom::ElementRef configRoot,CString startmode,CString SerialNo,CString xpath,CString &serviceStatus);;
		Status UpdateMFPDOM(CString xPath, CString val);
		HierarchicalDBRef m_hdb;
		ElementRef GetDocNode(CString , CString);
		Status installPnPDriver(ElementRef, int);
		CString m_univ_version;
                CString m_ps3_version;
                CString m_xps_version;
		int UpdateDriverVer(ElementRef);
		int checkVersion(ElementRef);
		CString readversionfile(int);
		CString getWinsAddress (ElementRef, CString &, CString &);
		/*Manoj 20100704 :: Added to crate map of all the basic samba configuration which is must for samba Operation*/
		map<CString , CString> mSambaMapGlobal;
		map<CString , CString> mSambaMapDrivShare;
		map<CString , CString> mSambaMapXPSDriverShare;
		map<CString , CString> mSambaMapPS3DriverShare;
		map<CString , CString> mSambaMapUNIVDriverShare;
		map<CString , CString>::iterator iSambaIterator;
		CString m_IPAddress;
		CString m_SnmpVersion;
		CString m_SnmpCommunity;
		/*Manoj 20100704 :: Added to push pcl6, ps3, xps and Print*/
		vector<CString> vPrinterType;
		vector<CString>::const_iterator viPrinterIterator;
		
		//For Checking the Line mode
		Status CheckLinemode(int &value);
		/*20160614: check smb conf for validation. EBX_DTFR_16193 F510 occurred*/
		Status ValidateConfOnBootup(void);

	
		//Status EncryptPassword(CString inputPassword,CString &outputEncryptedPassword);
		//Status DecryptPassword(CString inputPassword,CString &outputEncryptedPassword);
		CSmbNMO();
		~CSmbNMO(){}
};
map<string, string> CSmbNMO::xPath_config_map;
map<string, string> CSmbNMO::xPath_Value_map;

} //namespace AL
} //namespace Network
} //namespace NMO

