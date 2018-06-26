// Copyright 2008 TOSHIBA TEC CORPORATION All rights reserved //
#include <sys/types.h>
#include <sys/stat.h>
//#include "CI/OperatingEnvironment/debug.h"
#include "CI/OperatingEnvironment/ref.h"
#include "CI/Codecs/zip.h"
#include <CI/Codecs/passwordidentifier.h>
#include <CI/IndexedDB/indexeddb.h>
#include "smbnmo.h"

#define SMB_DEVICENAME "deviceName"
//#define DEFAULT_CONFIGURATION_PATH "/config/"
#define DEFAULT_CONFIGURATION_PATH AL_NETWORK_CONFIGPATH		//get from nmo.h

#define FIELD_VALUE 6
using namespace ci::hierarchicaldb;
using namespace ci::indexeddb;

namespace al
{
    namespace network
    {
        namespace nmo
        {


            /**
             * The SmbNMO manages samba config file.It needs to know
             * where the config file lives, the config file's format, and various commands
             * to start/stop/reload the service, and a command to get the service's running
             * status.
             * 
             In the configuration db, it maintains a copy of the values from the config
             * file (right now, all of them), and knows about three special XPaths: 
             * 'enabled' (should the service be running), 'active' (is the service actually
             * running), and 'error' (what went wrong in trying to update the service).  The
             * first of these must be passed to the NMO, either in the config db or in a 
             * transaction document, and the other two are set by the NMO itself based on 
             * the outcome of an update attempt.
             * 
             * The Acquire() function looks in NSM's config tree for a section called
             * "samba", which should list the services it is to create instances for.
             * There can also be some default settings such as the enable/active XPaths.
             * Then it looks for section named after each service to get the values it
             * needs to know about (listed above) in order to manage that service.
             */




            extern "C"
                void AL_Network_AcquireNMO(ElementRef conf, vector<NMOTraits> &instance)
                {
                    //Get Smb NMO objects node
					DEBUGL4("[%s] function enter\n",__FUNCTION__);
                    NMOTraits traits;
                    CSmbNMO *nmo = new CSmbNMO;
                    ElementRef gen = chelper::NavigatePath("NSM/NMObjects/NonGeneric",conf);
                    if (!gen || gen->getNodeType() != Node::ELEMENT_NODE)
					{
						if(nmo)
						{
							delete nmo;
						}
						DEBUGL1("[%s]  NavigatePath Failed to bind to the node\n",__FUNCTION__);
                        return;
					}

                    CString default_configFilePath = "";
                    CString default_svcXPath = "Services";
                    nmo->m_confFormat	= "INI";	
                    nmo->m_enabledXPath = "enabled";
                    nmo->m_activeXPath	= "active";
                    nmo->m_errorXPath	= "error";

                    /*20160614: Validate smb conf file before loading NMO. EBX_DTFR_16193 F510 occurred*/
                    Status cConfigFileRepair =  nmo->ValidateConfOnBootup();
                    if (STATUS_FAILED == cConfigFileRepair){
                        DEBUGL1("AcquireNMO: smb.conf file is corrupted, copy default conf file\n");
                        CString csCopyConf = "cp -p /home/SYSROM_SRC/NoBuildItems/AL/Network/smb.conf /encryption/al/network/config/smb.conf";
                        int iret = system(csCopyConf.c_str());
                        if(WEXITSTATUS(iret) != 0)
                            DEBUGL1("AcquireNMO: smb.conf copy failed\n");

                    }
                    else
                        DEBUGL5("AcquireNMO: smb.conf file not corrupted\n");

                    //	if(char* szEb2Path = getenv("EB2"))
                    //		default_configFilePath = CString(szEb2Path) + DEFAULT_CONFIGURATION_PATH;
                    default_configFilePath = AL_NETWORK_CONFIGFULLPATH;
                    //	DEBUGL5("samba.AcquireNMO() defaultConfigPath %s\n", default_configFilePath.c_str());

                    ElementRef smbNode; 
                    for (ElementRef t = gen->getFirstChild(); t; t = t->getNextSibling()) 
                    {
                        if (t->getNodeType() != Node::ELEMENT_NODE)
                            ;

                        ElementRef v = chelper::GetChildNode(t, "name");
                        if (v && v->getTextContent() == "Samba")
                        {
                            smbNode=t;
                            traits.service = v->getTextContent();
                        }
                    }
                    ElementRef v = chelper::GetChildNode(smbNode, "configFile"); 
                    if (v) 
                    {
                        // This NMO has configuration file 
                        // if config_path is not starting with / then assume that it is a relative path to EB2 config path.
                        CString configPath = v->getTextContent();
                        if (configPath.find("/") == 0)
                            nmo->m_confFile = configPath;
                        else
                            nmo->m_confFile = default_configFilePath + configPath;
                        //		DEBUGL5("AcquireNMO: Config file format = %s\n", nmo->m_confFormat.c_str());
                    }

                    ElementRef c = chelper::GetChildNode(smbNode, "startOrder"); 
                    if (c) 
                    {
                        nmo->orderno = atoi(c->getTextContent().c_str());
                    }

                    v = chelper::NavigatePath("dependents",smbNode);
                    if (v) 
                    {
                        for (ElementRef m = v->getFirstChild(); m; m = m->getNextSibling())
                        {
                            CString dependService;
                            dependService = m->getTextContent();
                            traits.dependsName.push_back(dependService);
                            DEBUGL5("generic.AquireNMO(): set to traits about dependency service [%s] in [%s]\n", dependService.c_str(),traits.service.c_str());
                            dependService = "";
                        }
                    }

                    traits.xpath = default_svcXPath + "/" + traits.service;
                    traits.nmo = nmo;

                    v = chelper::GetChildNode(smbNode, "serviceXpath");
                    if (v) 
                    {
                        traits.xpath = v->getTextContent();
                        nmo->m_serviceXpath = traits.xpath;
                    }
                    else
                    {
                        // no xpath given for the NMO, so it can't consider as valid NMO.
                        //		DEBUGL5("samba.AcquireNMO(): incomplete NMO configuration, No xpath specified\n");
                        // free the memory allocated for NMo object
                        delete nmo;
                    }

                    v = chelper::GetChildNode(smbNode, "startCmd");
                    if (v)
                    {
                        nmo->m_startCmd = v->getTextContent();
                        if (chelper::GetAttributeValue(v, "runBackground") == "1" || chelper::GetAttributeValue(v, "runBackground") == "true")
                            nmo->m_bStartCmdRunBGFlag = true;
                        else
                            nmo->m_bStartCmdRunBGFlag = false;

                        //nmo->m_startCmd += " &"; // run the script/process in background
                    }

                    // read Start command line arguments
                    v = chelper::GetChildNode(smbNode, "startCmdArguments");
                    if (v)
                    {
                        NodeListRef argsNodes = v->getChildNodes();
                        if(argsNodes)
                        {
                            // found an arguments node
                            for ( uint32 i = 0; i < argsNodes->getLength(); i++ )
                            {
                                NodeRef n = argsNodes->item(i);
                                if(n)
                                {
                                    CommandLineArgument arg;
                                    arg.cmdLineOption = chelper::GetNodeValue(n,"cmdLineOption");
                                    ElementRef velem = chelper::GetChildNode(n, "value");
                                    arg.value = velem->getTextContent();
                                    arg.valueType = chelper::GetAttributeValue(velem, "type"); 
                                    //					DEBUGL5("SmbNMO :: AL_Network_AcquireNMO  pushing a Start Cmd arg cmdopt=%s, type=%s,value=%s\n",arg.cmdLineOption.c_str(),arg.valueType.c_str(),arg.value.c_str());
                                    nmo->m_startCmdArguments.push_back(arg);
                                }
                            }
                        }
                    }

                    v = chelper::GetChildNode(smbNode, "stopCmd");
                    if (v)
                    {
                        nmo->m_stopCmd = v->getTextContent();
                        if (chelper::GetAttributeValue(v, "runBackground") == "1" || chelper::GetAttributeValue(v, "runBackground") == "true")
                            nmo->m_bStopCmdRunBGFlag = true;
                        else
                            nmo->m_bStopCmdRunBGFlag = false;
                        //nmo->m_stopCmd += " &"; // run the script/process in background
                    }

                    // read Stop command line arguments
                    v = chelper::GetChildNode(smbNode, "stopCmdArguments");
                    if (v)
                    {
                        NodeListRef argsNodes = v->getChildNodes();
                        if(argsNodes)
                        {
                            // found an arguments node
                            for ( uint32 i = 0; i < argsNodes->getLength(); i++ )
                            {
                                NodeRef n = argsNodes->item(i);
                                if(n)
                                {
                                    CommandLineArgument arg;
                                    arg.cmdLineOption = chelper::GetNodeValue(n,"cmdLineOption");
                                    ElementRef velem = chelper::GetChildNode(n, "value");
                                    arg.value = velem->getTextContent();
                                    arg.valueType = chelper::GetAttributeValue(velem, "type"); 
                                    //					DEBUGL5("SmbNMO :: AL_Network_AcquireNMO  pushing a Stop Cmd arg cmdopt=%s, type=%s,value=%s\n",arg.cmdLineOption.c_str(),arg.valueType.c_str(),arg.value.c_str());
                                    nmo->m_stopCmdArguments.push_back(arg);
                                }
                            }
                        }
                    }

                    v = chelper::GetChildNode(smbNode, "reloadCmd");
                    if (v)
                    {
                        nmo->m_reloadCmd = v->getTextContent();
                        if (chelper::GetAttributeValue(v, "runBackground") == "1" )
                            nmo->m_bReloadCmdRunBGFlag = true; // run the script/process in background
                    }

                    v = chelper::GetChildNode(smbNode, "restartCmdArguments");
                    if (v)
                    {
                        NodeListRef argsNodes = v->getChildNodes();
                        if(argsNodes)
                        {
                            // found an arguments node
                            for ( uint32 i = 0; i < argsNodes->getLength(); i++ )
                            {
                                NodeRef n = argsNodes->item(i);
                                if(n)
                                {
                                    CommandLineArgument arg;
                                    arg.cmdLineOption = chelper::GetNodeValue(n,"cmdLineOption");
                                    ElementRef velem = chelper::GetChildNode(n, "value");
                                    arg.value = velem->getTextContent();
                                    arg.valueType = chelper::GetAttributeValue(velem, "type");
                                    //                                      DEBUGL5("SmbNMO :: AL_Network_AcquireNMO  pushing a Stop Cmd arg cmdopt=%s, type=%s,value=%s\n",arg.cmdLineOption.c_str(),arg.valueType.c_str(),arg.value.c_str());
                                    nmo->m_reloadCmdArguments.push_back(arg);
                                }
                            }
                        }
                    }




                    v = chelper::GetChildNode(smbNode, "statusCmd");
                    if (v)
                    {
                        nmo->m_statusCmd = v->getTextContent();
                    }
                    v = chelper::GetChildNode(smbNode, "enabledXpath");
                    if (v) nmo->m_enabledXPath = v->getTextContent();

                    v = chelper::GetChildNode(smbNode, "activeXpath");
                    if (v) nmo->m_activeXPath = v->getTextContent();

                    v = chelper::GetChildNode(smbNode, "errorXpath");
                    if (v) nmo->m_errorXPath = v->getTextContent();

                    //	DEBUGL5("startCmd =%s, stopCmd=%s,reloadCmd=%s,statusCmd=%s,enabledXpath=%s,activexpath=%s,errorxpath=%s\n",
                    //			nmo->m_startCmd.c_str(),nmo->m_stopCmd.c_str(),nmo->m_reloadCmd.c_str(),nmo->m_statusCmd.c_str(),
                    //			nmo->m_enabledXPath.c_str(),nmo->m_activeXPath.c_str(),nmo->m_errorXPath.c_str());	

                    // read recovery mode settings
                    /*v = chelper::GetChildNode(smbNode, "recoveryMode");
                      if (v) 
                      {
                      if (v->getTextContent() == "noaction" )
                      nmo->m_recoveryMode = al::network::NMO::eRecoveryNoAction;
                      else if(v->getTextContent() == "restart")
                      nmo->m_recoveryMode = al::network::NMO::eRecoveryRestart;

                    //		DEBUGL5("AcquireNMO: v->getTextContent() = %sn",v->getTextContent().c_str());
                    }*/

                    v = chelper::NavigatePath("TranslationMap",smbNode);
                    if (v) 
                    {
                        for (ElementRef m = v->getFirstChild(); m; m = m->getNextSibling()) 
                        {
                            CString cfgXPath, iniXPath;
                            v = chelper::NavigatePath("iniXpath",m);
                            if (v) 
                                iniXPath = v->getTextContent();
                            v = chelper::NavigatePath("xpath",m);
                            if (v) 
                                cfgXPath = v->getTextContent();

                            //			DEBUGL5("samba.AquireNMO(): building xpath map : inserting [ %s:%s ]\n", cfgXPath.c_str(), iniXPath.c_str());
                            nmo->m_xpathMap[cfgXPath] = iniXPath;	
                        }
                    }
                    // if nmo->m_reloadCmd is empty, we'll just stop and start service
                    // daemon to get it to reload its config file.

                    v = chelper::GetChildNode(smbNode, "connectSSM");
                    if (v && v->getTextContent() != "1")
                        // don't pass service name back to NSM, so it won't tell SSM 
                        // about this service.
                        //			traits.service = CString();

                        instance.push_back(traits);

                    /*
                     * Initialize the status of Finisher to smb.conf in boot up
                     */
                    nmo->UpdateMFPDOM( PEDESTAL1_XPATH, "");
                    nmo->UpdateMFPDOM( PEDESTAL2_XPATH, "");
                    nmo->UpdateMFPDOM( LCF_XPATH, "");
                    nmo->UpdateMFPDOM( FINISHER_XPATH, "");
                    nmo->UpdateMFPDOM( STAPLE_XPATH, "");
                    nmo->UpdateMFPDOM( SADDLESTITCH_XPATH, "");
                    nmo->UpdateMFPDOM( HOLEPUNCH_XPATH, "");
                    nmo->UpdateMFPDOM( INSERTER_XPATH, "");

		//20160109: Deletion of samba_IP.log is not required, Because samba_IP.log files will not be created
		// to avoid HDD Access reduction. log path is moved to /ramdisk/al/network/log/
		#if 0
                    /* L4.8/L4.7315 : Since lot of thirdparty-samba logs are logged to /work/log/al folder,
                     * delete all samba_%I.log during boot-up.
                     */
                    CString csLogFile = "rm -rf " + CString(SAMBA_LOG_PATH) + "samba_*";
                    system(csLogFile.c_str());
                    DEBUGL5("AcquireNMO: deleting samba thirdparty logs from /work/log/al\n");
                    //delete samba_%I.log during boot-up ends here
		#endif
		//Copy smb.version.conf file from NoBuildItems to ramdisk folder on bootup.
		//Since ramdisk will be cleared on bootup
		    CString csVersionFile = "";
		    csVersionFile = "mkdir -p /ramdisk/al/network/samba";
		    int iRet = system(csVersionFile.c_str());
		    if(WEXITSTATUS(iRet) != 0)
			    DEBUGL1("AcquireNMO: /ramdisk/al/network/samba creation is failed [%d]\n",iRet);
		    csVersionFile.clear();
		    csVersionFile = "cp -f " + CString(SAMBA_VERSION_NOBUILD_PATH) + " " + CString(SAMBA_VERSION_PATH);
		    iRet = system(csVersionFile.c_str());
		    if(WEXITSTATUS(iRet) != 0)
			    DEBUGL1("AcquireNMO: smb.version.conf file copy to ramdisk is failed [%d]\n",iRet);
		    else
			    DEBUGL5("AcquireNMO: smb.version.conf file is copied to ramdisk path\n");

		    DEBUGL4("[%s] function exit\n",__FUNCTION__);
		}

            CSmbNMO::CSmbNMO()
            {
				DEBUGL4("[%s] function enter\n",__FUNCTION__);
                CString session = "SmbNMO";

                m_pBO = al::uicontroller::BOProxy::Acquire(session);
                if(!m_pBO)
                {
                    DEBUGL1("BOSERVER Acquire failed\n");
                }
                m_hdb = HierarchicalDB::Acquire(0);
                if(!m_hdb)
                {
                    DEBUGL1("HierarchicalDB Acquire failed\n");
                }
                CSmbNMO::initXPathMapping();
                CSmbNMO::initValueMapping();
				DEBUGL4("[%s] function exit\n",__FUNCTION__);
            }

            CString CSmbNMO::readversionfile(int line) 
            {
                DEBUGL4("[%s] function enter\n",__FUNCTION__);
                CString my_string;
                CString filePath = getenv("EB2");
                filePath += CString (DRIVERNOBUILDITEM) + "version.txt";
                ifstream myfile (filePath.c_str());
                if (myfile.is_open())
                {
                    if (1 == line)
                    {
                        CString str;
                        getline(myfile, my_string, '\n');
                        unsigned int position = my_string.find("=");
                        if(position != CString::npos)
                        {
                            str = my_string;
                            str.erase(position, my_string.length());
                        }
                       myfile.close();
						DEBUGL4("[%s] function exit\n",__FUNCTION__);
                        return str;

                    }
                    else if(2 == line)
                    {
                        CString str;
                        getline(myfile, my_string, '\n');
                        getline(myfile, my_string, '\n');
                        unsigned int position = my_string.find("=");
                        if(position != CString::npos)
                        {
                            str = my_string;
                            str.erase(position, my_string.length());
                        }
                       myfile.close();
						DEBUGL4("[%s] function exit\n",__FUNCTION__);
                        return str;
                    }
                    else if(3 == line)
                    {
                        CString str;
                        getline(myfile, my_string, '\n');
                        getline(myfile, my_string, '\n');
                        getline(myfile, my_string, '\n');
                        unsigned int position = my_string.find("=");
                        if(position != CString::npos)
                        {
                            str = my_string;
                            str.erase(position, my_string.length());
                        }
                       myfile.close();
						DEBUGL4("[%s] function exit\n",__FUNCTION__);
                        return str;
                    }
                    myfile.close();
                }
                 DEBUGL1("CSmbNMO::readversionfile, File open operation failed\n");
                 return "STATUS_FAILED";
            }

            void CSmbNMO::UpdateSambaMap()
            {
                DEBUGL4("[%s] function enter\n",__FUNCTION__);
				struct dirent **drivernamelist;
                int driverNumber = 0;
                const char *setup = "Setup.exe";
				/* 20161121: Fix for Vulnerability(JVNDB-2016-001684)
				 *  http://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2016-2542
				 */
                const char *essetup = "essetup.exe";

                DEBUGL5("Initilizing Samba Map for Validation of smb.conf\n");

#ifndef REMOVE_PNP //This is for the removal of the PNP driver installation at the time of first boot EBX 1.1
                vPrinterType.push_back("ps3");
                vPrinterType.push_back("univ");
#endif
                //vPrinterType.push_back("xps");
                vPrinterType.push_back("Print");

                /*Manoj::" Adding Global Section to map for validation*/
                mSambaMapGlobal["global/server~string"]		=	"SMB Server";
                mSambaMapGlobal["global/map~to~guest"]		=	"bad user";
                mSambaMapGlobal["global/log~file"]		=	"%${AL_NET_LOG}/samba_%I.log";
                mSambaMapGlobal["global/log~level"]		=	"3";
                mSambaMapGlobal["global/max~log~size"]		=	"5000";
                //mSambaMapGlobal["global/max~smbd~processes"]	=	"100";
                mSambaMapGlobal["global/max~ebx~connection"]	=	"13";
                mSambaMapGlobal["global/wins~proxy"]		=	"no";
                mSambaMapGlobal["global/dns~proxy"]		=	"no";
                mSambaMapGlobal["global/guest~account"]		=	"root";
                mSambaMapGlobal["global/load~printers"]		=	"yes";
                mSambaMapGlobal["global/SRAM~Logging"]          =       "no";
                mSambaMapGlobal["global/printcap~name"]		=	"/etc/printcap";
                mSambaMapGlobal["global/printing"]		=	"lprng";
                mSambaMapGlobal["global/local~master"]		=	"no";
                mSambaMapGlobal["global/encrypt~passwords"]	=	"yes";
                mSambaMapGlobal["global/deadtime"]		=	"60";
                mSambaMapGlobal["global/include"]		=	"%$(AL_NET_CONF)/smb_print_share.conf";
                mSambaMapGlobal["global/DeviceAuthTimeout"]     =	"10";       
                mSambaMapGlobal["global/name~resolve~order"]     =	"lmhosts host wins bcast purev6"; // code change for name resolve oreder
                /*Manoj:: Adding FILE_SHARE Section to map for validation*/	
                mSambaMapGlobal["FILE_SHARE/path"]		=	"/storage/sharedfolder";
                mSambaMapGlobal["FILE_SHARE/browseable"]	=	"yes";
                mSambaMapGlobal["FILE_SHARE/writeable"]		=	"no";
                mSambaMapGlobal["FILE_SHARE/max~connections"]	=	"13";
                mSambaMapGlobal["FILE_SHARE/guest~ok"]		=	"yes";
                mSambaMapGlobal["FILE_SHARE/available"]		=	"1";

                /*Manoj:: Adding ps3/univ/Print Section to map for validation*/
                mSambaMapDrivShare["/path"]			=	"/smbprefix";
                mSambaMapDrivShare["/comment"]			=	" Driver";
                mSambaMapDrivShare["/browseable"]		=	"yes";
                mSambaMapDrivShare["/printable"]		=	"yes";
                mSambaMapDrivShare["/writeable"]		=	"yes";
                mSambaMapDrivShare["/guest~ok"]			=	"yes";
                mSambaMapDrivShare["/available"]		=	"1";

                /* PnP support for webinstaller in samba, user will be able to install the printer similar to PnP with the below code change.
                 * samba is providing an interface for the user to directly install printer from the webinstaller drivers. 
                 * 1) User will not be able to delete any driver files.
                 * 2) All the other driver files will be hidden except Setup.exe / essetup.exe- Changed for Vulnerability(JVNDB-2016-001684)
                 * 3) When print share is disabled drivers folders will also be disabled.*/
                CString driv_str = readversionfile(1);
                if((driv_str != "STATUS_FAILED") && (driv_str != ""))
                {
                    mSambaMapUNIVDriverShare[driv_str + CString ("_driver/path")]            =       "/work/drivers/pointNprint/UNI/";
                    mSambaMapUNIVDriverShare[driv_str + CString ("_driver/available")]       =       "1";
                    mSambaMapUNIVDriverShare[driv_str + CString ("_driver/max~connections")] =        "13";
                    mSambaMapUNIVDriverShare[driv_str + CString ("_driver/browseable")]      =        "yes";
                    mSambaMapUNIVDriverShare[driv_str + CString ("_driver/writeable")]       =        "no";
                    mSambaMapUNIVDriverShare[driv_str + CString ("_driver/public")]          =        "yes";
                driverNumber = 0;
                    driverNumber = scandir("/work/drivers/pointNprint/UNI", &drivernamelist, 0, alphasort);
                if (driverNumber <= 0){
                        DEBUGL5("NO UNIV drivers1\n");
                        SavetoConfig((driv_str + CString ("_driver/available")),"0");
                }
                /* check added to avoid displaying the share folder if driver files are missing*/
                else if(driverNumber < MAX_DRIVER_FILES)
                {
                        DEBUGL5("NO UNIV drivers2\n");
                        SavetoConfig((driv_str + CString ("_driver/available")),"0");
                    while(driverNumber--){
                        if(drivernamelist[driverNumber])
                            free(drivernamelist[driverNumber]);
                    }
                    if(drivernamelist)
                        free(drivernamelist);
                }
                else{
                    CString hide_files = "";
                    while(driverNumber--){
                        size_t driverfile_size = strlen(drivernamelist[driverNumber]->d_name);
                        if((strncmp((drivernamelist[driverNumber]->d_name),setup,driverfile_size)== 0) ||
                        (strncmp((drivernamelist[driverNumber]->d_name),essetup,driverfile_size)== 0))
						{
							continue;
						}
						else
                        {
                                hide_files += "/work/drivers/pointNprint/UNI/" + CString(drivernamelist[driverNumber]->d_name);
                        }
                        if(drivernamelist[driverNumber])
                            free(drivernamelist[driverNumber]);
                    }
                    if(drivernamelist)
                        free(drivernamelist);
                        mSambaMapUNIVDriverShare[driv_str + CString ("_driver/hide~files")]  =       hide_files;
                }
                }
		else
		{
			/*In latest samba versions(from samba4.4.x??) if available= empty is there, then smbd will not start*/
			DEBUGL2("version.txt file is not present hence update UNIV driver availbale to 0\n");
			SavetoConfig(("univ" + CString ("_driver/available")),"0");
		}
                driv_str = readversionfile(2);
                if((driv_str != "STATUS_FAILED") && (driv_str != ""))
                {		
                    mSambaMapPS3DriverShare[driv_str + CString ("_driver/path")]            =       "/work/drivers/pointNprint/PS/";
                    mSambaMapPS3DriverShare[driv_str + CString ("_driver/available")]       =       "1";
                    mSambaMapPS3DriverShare[driv_str + CString ("_driver/max~connections")] =        "13";
                    mSambaMapPS3DriverShare[driv_str + CString ("_driver/browseable")]      =        "yes";
                    mSambaMapPS3DriverShare[driv_str + CString ("_driver/writeable")]       =        "no";
                    mSambaMapPS3DriverShare[driv_str + CString ("_driver/public")]          =        "yes";
                driverNumber = 0;
                    driverNumber = scandir("/work/drivers/pointNprint/PS", &drivernamelist, 0, alphasort);
                if (driverNumber <= 0){
                        DEBUGL5("NO PS drivers1\n");
                        SavetoConfig((driv_str + CString ("_driver/available")),"0");
                }
                else if(driverNumber < MAX_DRIVER_FILES)
                {
                        DEBUGL5("NO PS drivers2\n");
                        SavetoConfig((driv_str + CString ("_driver/available")),"0");
                    while(driverNumber--){
                        if(drivernamelist[driverNumber])
                            free(drivernamelist[driverNumber]);
                    }
                    if(drivernamelist)
                        free(drivernamelist);
                }
                else{
                    CString hide_files2 = "";
                    while(driverNumber--){
                        size_t driverfile_size = strlen(drivernamelist[driverNumber]->d_name);
                        if((strncmp((drivernamelist[driverNumber]->d_name),setup,driverfile_size)== 0) ||
                        (strncmp((drivernamelist[driverNumber]->d_name),essetup,driverfile_size)== 0))
						{
							continue;
						}
						else
                        {
                                hide_files2 += "/work/drivers/pointNprint/PS/" + CString(drivernamelist[driverNumber]->d_name);
                        }
                        if(drivernamelist[driverNumber])
                            free(drivernamelist[driverNumber]);
                    }
                    if(drivernamelist)
                        free(drivernamelist);
                        mSambaMapPS3DriverShare[driv_str + CString ("_driver/hide~files")]  =       hide_files2;
                }
                }
		else
		{
			/*In latest samba versions(from samba4.4.x??) if available= empty is there, then smbd will not start*/
			DEBUGL5("version.txt file is not present hence update PS3 driver availbale to 0\n");
			SavetoConfig(("ps3" + CString ("_driver/available")),"0");
		}
                driv_str = readversionfile(3);
                if((driv_str != "STATUS_FAILED") && (driv_str != ""))
                {
                    mSambaMapXPSDriverShare[driv_str + CString ("_driver/path")]            =       "/work/drivers/pointNprint/XPS/";
                    mSambaMapXPSDriverShare[driv_str + CString ("_driver/available")]       =       "1";
                    mSambaMapXPSDriverShare[driv_str + CString ("_driver/max~connections")] =        "13";
                    mSambaMapXPSDriverShare[driv_str + CString ("_driver/browseable")]      =        "yes";
                    mSambaMapXPSDriverShare[driv_str + CString ("_driver/writeable")]       =        "no";
                    mSambaMapXPSDriverShare[driv_str + CString ("_driver/public")]          =        "yes";
                driverNumber = 0;
                driverNumber = scandir("/work/drivers/pointNprint/XPS", &drivernamelist, 0, alphasort);
                if (driverNumber <= 0){
                    DEBUGL5("NO XPS drivers1\n");
                        SavetoConfig((driv_str + CString ("_driver/available")),"0");
                }
                else if(driverNumber < MAX_DRIVER_FILES)
                {
                    DEBUGL5("NO XPS drivers2\n");
                        SavetoConfig((driv_str + CString ("_driver/available")),"0");
                    while(driverNumber--){
                        if(drivernamelist[driverNumber])
                            free(drivernamelist[driverNumber]);
                    }
                    if(drivernamelist)
                        free(drivernamelist);
                }
                else{
                    CString hide_files3 = "";
                    while(driverNumber--){
                        size_t driverfile_size = strlen(drivernamelist[driverNumber]->d_name);
                        if((strncmp((drivernamelist[driverNumber]->d_name),setup,driverfile_size)== 0) ||
                        (strncmp((drivernamelist[driverNumber]->d_name),essetup,driverfile_size)== 0))
                        {
							continue;
                        }
						else
                        {
                            hide_files3 += "/work/drivers/pointNprint/XPS/" + CString(drivernamelist[driverNumber]->d_name);
                        }
                        if(drivernamelist[driverNumber])
                            free(drivernamelist[driverNumber]);
                    }
                    if(drivernamelist)
                        free(drivernamelist);
                        mSambaMapXPSDriverShare[driv_str + CString ("_driver/hide~files")]  =       hide_files3;
                    }
                }
		else
		{
			/*In latest samba versions(from samba4.4.x??) if available= empty is there, then smbd will not start*/
			DEBUGL5("version.txt file is not present hence update XPS driver availbale to 0\n");
			SavetoConfig(("xps" + CString ("_driver/available")),"0");
		}
                /* webinstaller PnP end*/
				DEBUGL4("[%s] function exit\n",__FUNCTION__);
                return ;
            }
            /*
               Manoj :: 20100704 
               ValidateSmbConf (void)		
               @In	:: Void
               @Out	:: CString containing Differet config file error status

               Defination	:: This Function will recover smb.conf file if any/few of samba configuration parameter got corrupted
               It will compare smb.conf with some MUST have smb configuration parameter and in case if those are missing 
               he will add the same paramter with predefined default value
               Current configuration updation will be taken care by Update Function.

               In case developer want to modify/Add more paremeter please add/modify global maps defined in CSmbNMO class
               map mSambaMapGlobal 	-> For global of FILE_SHARE
               map mSambaMapDrivShare  -> For ps3, univ and print share
               */
            CString CSmbNMO::ValidateSmbConf()
            {
                DEBUGL4("[%s] function enter\n",__FUNCTION__);
				ElementRef ptempNode	= NULL;	
                ElementRef confRoot	= NULL;
                bool bRecoveryDone	= false;
                CString cPrintName	= "";	


                CIniFile ini(m_confFile);
                HierarchicalDBRef m_hdb = HierarchicalDB::Acquire(0);
                /*Manoj :: Veryfing and reparing smb.conf*/
                confRoot = ini.GetRoot();
                if(!confRoot)
                {
                    DEBUGL1("[%s] Unable to get ConfigRoot \n", __FUNCTION__);
                    return "NO_CONFIG_ROOT";
                }

                /*Manoj  :: Reparing Global and Print section of smb.conf*/
                for(iSambaIterator = mSambaMapGlobal.begin(); iSambaIterator != mSambaMapGlobal.end(); iSambaIterator++)
                {
                    DEBUGL5("[%s] Reparing/Veryfing %s\n",__FUNCTION__,((*iSambaIterator).first).c_str());
                    ptempNode = chelper::NavigatePath((*iSambaIterator).first, confRoot);
                    if (!ptempNode){
                        ptempNode = chelper::NavigatePath((*iSambaIterator).first, confRoot,true);
                        if (!ptempNode){
                            DEBUGL1("Unable to recover %s with value %s\n",((*iSambaIterator).first).c_str(),((*iSambaIterator).second).c_str());
                            continue ;
                        }
                        ptempNode->setTextContent((*iSambaIterator).second);
                        bRecoveryDone = true;
                        ini.SetModified();
                        DEBUGL5("smb.config Corrupted, repared %s with value %s\n",((*iSambaIterator).first).c_str(),((*iSambaIterator).second).c_str());
                    }
                    ptempNode = NULL;
                }

                /*Manoj :: Reparing  ps3 section of smb.conf*/
                for (viPrinterIterator = vPrinterType.begin(); viPrinterIterator != vPrinterType.end(); viPrinterIterator++)	
                {
                    for(iSambaIterator = mSambaMapDrivShare.begin(); iSambaIterator != mSambaMapDrivShare.end(); iSambaIterator++)
                    {
                        cPrintName = *viPrinterIterator + (*iSambaIterator).first;
                        DEBUGL5("[%s] Reparing/Veryfing %s\n",__FUNCTION__,cPrintName.c_str());
                        ptempNode = chelper::NavigatePath(cPrintName, confRoot);
                        if (!ptempNode){
                            ptempNode = chelper::NavigatePath(cPrintName, confRoot,true);
                            if (!ptempNode){
                                DEBUGL1("Unable to recover %s with value %s\n",cPrintName.c_str(),((*iSambaIterator).second).c_str());
                                continue ;
                            }
                            if ("/comment" == (*iSambaIterator).first) ptempNode->setTextContent(*viPrinterIterator + (*iSambaIterator).second);
                            else ptempNode->setTextContent((*iSambaIterator).second);
                            bRecoveryDone = true;
                            ini.SetModified();
                            DEBUGL5("smb.config Corrupted, repared %s with value %s\n",cPrintName.c_str(),((*iSambaIterator).second).c_str());
                        }
                        ptempNode = NULL;
                    }
                }	
                /* webinstaller PnP */
                for(iSambaIterator = mSambaMapUNIVDriverShare.begin(); iSambaIterator != mSambaMapUNIVDriverShare.end(); iSambaIterator++)
                {
                    DEBUGL5("[%s] Reparing/Veryfing %s\n",__FUNCTION__,((*iSambaIterator).first).c_str());
                    ptempNode = chelper::NavigatePath((*iSambaIterator).first, confRoot);
                    if (!ptempNode){
                        ptempNode = chelper::NavigatePath((*iSambaIterator).first, confRoot,true);
                        if (!ptempNode){
                            DEBUGL1("Unable to recover %s with value %s\n",((*iSambaIterator).first).c_str(),((*iSambaIterator).second).c_str());
                            continue ;
                        }
                        ptempNode->setTextContent((*iSambaIterator).second);
                        bRecoveryDone = true;
                        ini.SetModified();
                        DEBUGL5("smb.config Corrupted, repared %s with value %s\n",((*iSambaIterator).first).c_str(),((*iSambaIterator).second).c_str());
                    }
                    else{
                        CString driv_str = readversionfile(1);
						if((driv_str != "STATUS_FAILED") && (driv_str != "")){
                        	if((driv_str + CString ("_driver/hide~files")) == (*iSambaIterator).first){
							
							/*20131031: Fix for samba starting twice when wakes up from hibernation mode changes starts here.*/
								CString driverHideFiles = ptempNode->getTextContent();
                            	if(driverHideFiles != (*iSambaIterator).second)
                            	{
                               		DEBUGL5("%s : smb.conf file recovered\n",driv_str.c_str());
	                            	ptempNode->setTextContent((*iSambaIterator).second);
    	                        	bRecoveryDone = true;
        	                    	ini.SetModified();         
                            	}
                            	else
                                	DEBUGL5("%s : No need to recover the smb.conf file\n",driv_str.c_str());
							/*Fix for hibernation issue changes ends here.*/
                        	}	
                    	}
		  			}
                    ptempNode = NULL;
                }

                for(iSambaIterator = mSambaMapPS3DriverShare.begin(); iSambaIterator != mSambaMapPS3DriverShare.end(); iSambaIterator++)
                {
                    DEBUGL5("[%s] Reparing/Veryfing %s\n",__FUNCTION__,((*iSambaIterator).first).c_str());
                    ptempNode = chelper::NavigatePath((*iSambaIterator).first, confRoot);
                    if (!ptempNode){
                        ptempNode = chelper::NavigatePath((*iSambaIterator).first, confRoot,true);
                        if (!ptempNode){
                            DEBUGL1("Unable to recover %s with value %s\n",((*iSambaIterator).first).c_str(),((*iSambaIterator).second).c_str());
                            continue ;
                        }
                        ptempNode->setTextContent((*iSambaIterator).second);
                        bRecoveryDone = true;
                        ini.SetModified();
                        DEBUGL5("smb.config Corrupted, repared %s with value %s\n",((*iSambaIterator).first).c_str(),((*iSambaIterator).second).c_str());
                    }
                    else{
                        CString driv_str = readversionfile(2);
						if((driv_str != "STATUS_FAILED") && (driv_str != "")){
                        	if((driv_str + CString ("_driver/hide~files")) == (*iSambaIterator).first){
					
							/*20131031: Fix for samba starting twice when wakes up from hibernation mode changes starts here.*/
					        	CString driverHideFiles = ptempNode->getTextContent();
                            	if(driverHideFiles != (*iSambaIterator).second)
                            	{
                                	DEBUGL5("%s : smb.conf file recovered\n",driv_str.c_str());
	                            	ptempNode->setTextContent((*iSambaIterator).second);
    	                        	bRecoveryDone = true;
        	                    	ini.SetModified();
                            	}
                            	else
                                	DEBUGL5("%s : No need to recover the smb.conf file\n",driv_str.c_str());
							/*Fix for hibernation issue changes ends here.*/
                        		
								}
                    		}
					}
                    ptempNode = NULL;
                }

                for(iSambaIterator = mSambaMapXPSDriverShare.begin(); iSambaIterator != mSambaMapXPSDriverShare.end(); iSambaIterator++)
                {
                    DEBUGL5("[%s] Reparing/Veryfing %s\n",__FUNCTION__,((*iSambaIterator).first).c_str());
                    ptempNode = chelper::NavigatePath((*iSambaIterator).first, confRoot);
                    if (!ptempNode){
                        ptempNode = chelper::NavigatePath((*iSambaIterator).first, confRoot,true);
                        if (!ptempNode){
                            DEBUGL1("Unable to recover %s with value %s\n",((*iSambaIterator).first).c_str(),((*iSambaIterator).second).c_str());
                            continue ;
                        }
                        ptempNode->setTextContent((*iSambaIterator).second);
                        bRecoveryDone = true;
                        ini.SetModified();
                        DEBUGL5("smb.config Corrupted, repared %s with value %s\n",((*iSambaIterator).first).c_str(),((*iSambaIterator).second).c_str());
                    }
                    else{
                        CString driv_str = readversionfile(3);
						if((driv_str != "STATUS_FAILED") && (driv_str != "")){
                        	if((driv_str + CString ("_driver/hide~files")) == (*iSambaIterator).first){
							
							/*20131031: Fix for samba starting twice when wakes up from hibernation mode changes starts here.*/
                            	CString driverHideFiles = ptempNode->getTextContent();
                            	if(driverHideFiles != (*iSambaIterator).second)
                            	{
                                	DEBUGL5("%s : smb.conf file recovered\n",driv_str.c_str());
	                            	ptempNode->setTextContent((*iSambaIterator).second);
    	                        	bRecoveryDone = true;
        	                    	ini.SetModified();
                            	}
								else
                                	DEBUGL5("%s : No need to recover the smb.conf file\n",driv_str.c_str());				
							/*Fix for hibernation issue changes ends here.*/
                        
							}
                    	}
					}
                    ptempNode = NULL;
                }
                /* webinstaller PnP end*/
                /*
                   Manoj ::	
                   Need to Add Include At the End of the config file
                   Reason of Change ::
                   if "include" comes above "map to guest", then fileshare will prompt for UN/PW even if MFP is in WG
                   Need to find the reason in OS samba.
                   */
                if (bRecoveryDone == true )
                {
                    DEBUGL5("[%s] Making include as last Node\n",__FUNCTION__);
                    NodeRef pElementNode = m_hdb->BindToElement(confRoot, "global/include");
                    if(pElementNode)
                    {
                        NodeRef pParentNode = pElementNode->getParentNode();
                        if (pParentNode)	
                        {
                            pParentNode->removeChild(pElementNode);

                            ptempNode = chelper::NavigatePath("global/include", confRoot, true);
                            if (ptempNode)		
                            {
                                ptempNode->setTextContent("%$(AL_NET_CONF)/smb_print_share.conf");
                                DEBUGL5("[%s] Set Default value of include\n",__FUNCTION__);
                            }
                            else
                                DEBUGL1("[%s]Unable to find global/include\n",__FUNCTION__);
                        }
                        else
                            DEBUGL1("[%s] Unable to get parent Node of include\n",__FUNCTION__);
                    }
                    else
                        DEBUGL1("[%s] Unable to find include Node, Return ERROR",__FUNCTION__);
                }

                bool bSaveConfigFile = ini.IsModified();
                if (bSaveConfigFile){
                    if(ini.Save() != STATUS_OK){
                        DEBUGL1("[%s] : Unable to save recovered file\n",__FUNCTION__);
                    }
                    else
                    {
                        DEBUGL5("Update: config file update done.reloading.\n");
                    }

                }
                if ( true == bRecoveryDone ){
					DEBUGL4("[%s] function exit\n",__FUNCTION__);
                    return ("CONFIG_FILE_RECOVERED");
				}
				DEBUGL4("[%s] function exit\n",__FUNCTION__);
                return ("NO_RECOVERY_REQUIRED");
            }/*End of ValidateSmbConf()*/

            /*
               Manoj :: 20110404
               UpdateSambaLogInfo (void)		
               @In	:: void
               @Out	:: void

               Defination	:: This Function will will  modify samba log level (depricate smb script for the same)
               */
            bool CSmbNMO::UpdateSambaLogInfo()
            {
				DEBUGL4("[%s] function enter\n",__FUNCTION__);
#if 0
                SSDKStatus returnstatus=ssdk::OK;
                Ref<SSDKSecurityManagerInterface> ssdkSecurityManagerInterface=GetSecurityManager();
                if(ssdkSecurityManagerInterface==(Ref<SSDKSecurityManagerInterface>)NULL)
                {
                    DEBUGL2("[%s]Scanner Info Getting ssdkSecurityManagerInterface failed \n",__FUNCTION__);
                    return ;
                }
                Ref<SSDKUserTokenInterface> usertokenInterface=dynamic_cast<SSDKUserTokenInterface*>
                    (ssdkSecurityManagerInterface->GetInterface(SSDKSecurityManagerInterface::USER_TOKEN_INTERFACE, returnstatus));
                if(usertokenInterface==(Ref<SSDKUserTokenInterface>)NULL || returnstatus!=ssdk::OK)
                {
                    DEBUGL2("[%s]Scanner Info failed to cast SSDKUserTokenInterface* with errorCode=%d\n",__FUNCTION__,returnstatus);
                    return ;
                }
                SSDKUserTokenInterface *userToken = NULL;
                returnstatus = usertokenInterface->Authenticate(SSDKUserTokenInterface::TOKEN_CONTEXT_AUTOPROCESSING,userToken);

                if(((returnstatus != ssdk::OK) && (returnstatus != ssdk::AUTH_GOOD_AUTHENTICATION_NEEDS_PASSWORD_CHANGE)) ||
                        userToken==(SSDKUserTokenInterface*)NULL)
                {
                    DEBUGL2("[%s]Scanner Authenticate Function Call Failed with errorCode=%d\n", __FUNCTION__,returnstatus);
                    return ;
                }
                Status retStatus = m_pBO->SetUserToken(userToken);
                if(returnstatus != STATUS_OK)
                {
                    DEBUGL2("[%s]Scanner SetUserToken Call Failed with errorCode=%d\n",__FUNCTION__ ,retStatus);
                    return ;
                }


                CString setXmlValue = "<SetValue><Payload XMLPayLoad = \"true\" overrideDelta = \"true\"><path></path><value><DiagnosticMode><Category><Mode>08</Mode><MainCode><Code>8942</Code></MainCode></Category></DiagnosticMode></value></Payload></SetValue>";	
                DEBUGL5("[%s]setXmlValue = %s\n",__FUNCTION__, setXmlValue.c_str());

                retStatus = m_pBO->SetValue(setXmlValue.c_str());
                if(retStatus != STATUS_OK)
                {
                    DEBUGL2("[%s]SetValue failed\n", __FUNCTION__);
                    return ;
                }
                CString strCommandMsg = "<Command><GetDiagnosticMode><commandNode>DiagnosticMode</commandNode></GetDiagnosticMode></Command>";
                CString sResponse;
                retStatus = m_pBO->ExecuteCommand(strCommandMsg.c_str(), sResponse);
                if(retStatus != STATUS_OK)
                {
                    DEBUGL2("[%s]Execute failed\n", __FUNCTION__);
                    return ;
                }
                DEBUGL5("[%s]Execute Success with [%s]\n", __FUNCTION__, sResponse.c_str());
                /*Read 08-8942*/
                NodeRef pNode = NULL;
                CString xpath = "DiagnosticMode/Category/MainCode/ValueList/Value/Value";
                std::string value;
                m_pBO->GetValue(xpath.c_str(), pNode);
                if(!pNode)
                {
                    DEBUGL2("[%s]read DOM failed\n", __FUNCTION__);
                    return ;
                }
#endif
                //int retValue = atoi((pNode->getNodeValue()).c_str());
                bool bSaveConfigFile = false;
                //bool bSSDModel = false;
                CString csLogLevel;

                if (!getenv("DEBUG_LEVEL"))
                    csLogLevel = "";
                else
                    csLogLevel = CString(getenv("DEBUG_LEVEL"));

                int retValue = atoi(csLogLevel.c_str());

                DEBUGL5("[%s]: Current Log Level = %d\n", __FUNCTION__, retValue);

		//20160109: HDD Access Reduction.
		//Unusable Code	
#if 0
                if(getenv("PRODUCT")){
                    CString csProductType=CString(getenv("PRODUCT"));
                    if(csProductType=="WEISS")
                    {
                if (getenv("SUB_PRODUCT")){
                    CString csModelType = CString(getenv("SUB_PRODUCT"));
                if((csModelType =="WEISS_L_SSD") || (csModelType =="WEISS_LL")){

                        bSSDModel = true;
                        DEBUGL5("%s Found SSD model, Optimized logs will be saved\n", __FUNCTION__);
                } 
                    }
                }
                }
#endif		

                CIniFile ini(m_confFile);
                ElementRef confRoot = ini.GetRoot();
                if(!confRoot)
                {
                    DEBUGL1("[%s] Unable to get ConfigRoot \n", __FUNCTION__);
				    return bSaveConfigFile;
                }

                ElementRef logLevelRef = chelper::NavigatePath("global/log~level", ini.GetRoot(), true);
                ElementRef logFileRef = chelper::NavigatePath("global/log~file", ini.GetRoot(), true);
                ElementRef logSizeRef = chelper::NavigatePath("global/max~log~size", ini.GetRoot(), true);

                /*Log Optimization for SSD model*/
                if (logSizeRef)
                {
                    CString csTmpLogSize;
                    csTmpLogSize = logSizeRef->getTextContent();
			/*20160109: HDD Access Reduction
			 * Always log size is 2MB
			 */
			if ("2000" != csTmpLogSize)
			{
				logSizeRef->setTextContent("2000");
				ini.SetModified();
			}
		}
#if 0
                    if (bSSDModel){
                        if ("1000" != csTmpLogSize){
                            /*20120319 Setting log size to 1MB for Weiss SSD model*/
                            logSizeRef->setTextContent("1000");
                            ini.SetModified();
                        }
                    }
                    else {
                        if ("10000" != csTmpLogSize){
                            /*20120319 Setting log size to 10MB for other then Weiss SSD model*/
                            logSizeRef->setTextContent("10000");
                            ini.SetModified();
                        }
                    }
#endif

                if (logLevelRef && logFileRef)
                {
                    CString csTmpLog;
                    if ((7 >= retValue) && (0 < retValue))
                    {
                        csTmpLog = logLevelRef->getTextContent();
                        DEBUGL5("[%s]: Current Log Level = %d setting log level = 1 in conf old [%s]\n", __FUNCTION__,retValue,(csTmpLog).c_str());
                        if ("1" != csTmpLog){
                            logLevelRef->setTextContent("1");
                            logFileRef->setTextContent("");
                            ini.SetModified();
                        }csTmpLog="";
                    }
                    else if ((15 == retValue) || (0 == retValue))
                    {
                        csTmpLog = logLevelRef->getTextContent();
                        DEBUGL5("[%s]: Current Log Level = %d setting log level = 6 in conf old [%s]\n", __FUNCTION__,retValue,(csTmpLog).c_str());
                        if ("6" != csTmpLog){
                            logLevelRef->setTextContent("6");
				/*20160109: HDD Access Reduction
				 * Always log file name is log.smbd for smbd process

				 if (bSSDModel)
				 	logFileRef->setTextContent("%$(AL_NET_LOG)/samba_client.log");
				 else
				 	logFileRef->setTextContent("%$(AL_NET_LOG)/samba_%I.log");
				 */

				logFileRef->setTextContent(CString(SAMBA_LOG_FILE));
                            ini.SetModified();
                        }csTmpLog="";
                    }
                    else if (31 == retValue)
                    {
                        csTmpLog = logLevelRef->getTextContent();
                        DEBUGL5("[%s]: Current Log Level = %d setting log level = 7 in conf old [%s]\n", __FUNCTION__,retValue,(csTmpLog).c_str());
                        if ("7" != csTmpLog){
                            logLevelRef->setTextContent("7");
				/*20160109: HDD Access Reduction
				 * Always log file name is log.smbd for smbd process

				 if (bSSDModel)
				 	logFileRef->setTextContent("%$(AL_NET_LOG)/samba_client.log");
				 else
				 	logFileRef->setTextContent("%$(AL_NET_LOG)/samba_%I.log");
				 */

				logFileRef->setTextContent(CString(SAMBA_LOG_FILE));
                            ini.SetModified();
                        }csTmpLog="";
                    }
                    else if (63 == retValue)
                    {
                        csTmpLog = logLevelRef->getTextContent();
                        DEBUGL5("[%s]: Current Log Level = %d setting log level = 9 in conf old [%s]\n", __FUNCTION__,retValue,(csTmpLog).c_str());
                        if ("9" != csTmpLog){
                            logLevelRef->setTextContent("9");
				/*20160109: HDD Access Reduction
				 * Always log file name is log.smbd for smbd process

				 if (bSSDModel)
				 	logFileRef->setTextContent("%$(AL_NET_LOG)/samba_client.log");
				 else
				 	logFileRef->setTextContent("%$(AL_NET_LOG)/samba_%I.log");
				 */

				logFileRef->setTextContent(CString(SAMBA_LOG_FILE));
                            ini.SetModified();
                        }csTmpLog="";
                    }
                    else if (127 == retValue || 255 == retValue)
                    {
                        csTmpLog = logLevelRef->getTextContent();
                        DEBUGL5("[%s]: Current Log Level = %d setting log level = 10 in conf old [%s]\n", __FUNCTION__,retValue,(csTmpLog).c_str());
                        if ("10" != csTmpLog){
                            logLevelRef->setTextContent("10");
				/*20160109: HDD Access Reduction
				 * Always log file name is log.smbd for smbd process

				 if (bSSDModel)
				 	logFileRef->setTextContent("%$(AL_NET_LOG)/samba_client.log");
				 else
				 	logFileRef->setTextContent("%$(AL_NET_LOG)/samba_%I.log");
				 */

				logFileRef->setTextContent(CString(SAMBA_LOG_FILE));
                            ini.SetModified();
                        }csTmpLog="";
                    }
                    else
                    {
                        DEBUGL5("[%s]: Current Log Level = %d (Undefined) disabling logs in conf\n",  __FUNCTION__,retValue);
                        logLevelRef->setTextContent("0");
                        logFileRef->setTextContent("");
                        ini.SetModified();
                    }	
                }
                bSaveConfigFile = ini.IsModified();
                if (bSaveConfigFile){
                    if(ini.Save() != STATUS_OK){
                        DEBUGL1("[%s] : Unable to save log file and log level in smb.log\n",__FUNCTION__);
                    }
                    else
                    {
                        DEBUGL5("Update: config file updated with log file and log level in smb.log\n");
                    }
                }
				DEBUGL4("[%s] configfile saved. function exit\n",__FUNCTION__);
                return bSaveConfigFile;
            }

            /*
               Manoj :: 201008019 
               readDriverFileType (void)		
               @In	:: void
               @Out	:: return file type filter, currently it is case in-sensetive as zip file name can be ZIP

               Defination	:: This Function will create filter for scandir() and return required value to scandir for the files present 
               in EB2/NoBuildItems/AL/Network/PnPData/
               */

            int readDriverFileType(const struct dirent *entry)
            {
				DEBUGL4("[%s] function enter\n",__FUNCTION__);
                if ((strcmp(entry->d_name, ".") == 0) || (strcmp(entry->d_name, "..") == 0)){
					DEBUGL4("[%s] function exit\n",__FUNCTION__);
                    return (0);
				}
                else if(strcasestr(entry->d_name, "zip") == 0){
					DEBUGL4("[%s] function exit\n",__FUNCTION__);
                    return (0);
				}
                else{
					DEBUGL4("[%s] function exit\n",__FUNCTION__);
                    return (1);
				}
            }

            /*
               Manoj :: 201008019 
               installPnPDriver (ElementRef)		
               @In	:: ElementRef
               @Out	:: void

               Defination	:: This Function will intall PnP driver present NoBuildItems/PnPData only for first boot.
               This function returns void, because samba startup dosent depends on driver instillation, if instillation fails
               then this function will update dom and throw error in nsm.log   	
               */
            Status CSmbNMO::installPnPDriver(ElementRef configRoot, int driverInst)
            {
                DEBUGL4("[%s] function enter\n",__FUNCTION__);
				ElementRef pDriverIntNodet = NULL ;
                DocumentRef msgDoc = NULL ;

                CString cDriverInstRequired;
                CString driverPath;
                CString cCopyCommand;
                CString szCmd ;

                CString cFileNames ;
				uint16 brand = 1;
                struct dirent **filelist = NULL;
                int iFcount = -1;
                int iIndex = 0 ;

                CString directory = getenv("EB2");
                directory += CString (DRIVERNOBUILDITEM);

                /*UIC command object used to process UIC messages*/
                Ref<CUICCommand> pUICCommand = NULL;

                DEBUGL5("Inside %s\n",__FUNCTION__);

                /*Now best time to read configuration parameter*/
                ReadConfigParameter(configRoot);

                /*Modify ConfigParameter for Driver ps3/univ/xps*/
                ModifyConfiguration(SMB_XPS_DRIVER_PKG);
                ModifyConfiguration(SMB_PS_DRIVER_PKG);
                int flagInstallDriver = 0;
				CString csBrandType = "";
                CString csProductType = "";
                if(getenv("EQUIPMENT_BRAND"))
                {
                    csBrandType = CString(getenv("EQUIPMENT_BRAND"));
                }
                if(getenv("PRODUCT"))
                {
                    csProductType = CString(getenv("PRODUCT"));
                }
				if(csProductType == "S2_PRODUCT") //common code (IndexedDB check for S2_PRODUCT)
				{
                	IndexedDB_ReadContent(8, 9003, 0, brand, false);
					DEBUGL5("Value of 9003 is %d\n",brand);
					flagInstallDriver = 1;
                }
				if(((flagInstallDriver == 1) && (OKI != brand)) || ((csProductType != "S2_PRODUCT") && (csBrandType != "OKI")))
                ModifyConfiguration(SMB_UNI_DRIVER_PKG);
                else
                        ModifyConfiguration(SMB_UNI_DRIVER_PKG_OKI_C);
                /*Check for the first instillation of eBX, Driver instilltion is only for first boot*/
                /*Manoj 20110615 As per new specification modify driver INF file whenever NIC reset is happening*/
                //pDriverIntNodet = chelper::NavigatePath("UpdateDriver", configRoot, false);
                //if (!pDriverIntNodet)
                //{
                //   DEBUGL1("Unable to retrive first boot info, returning\n");
                //    return STATUS_FAILED;	
                //}
                /*Checking for the first boot, if not continue instillation*/
                //cDriverInstRequired  = pDriverIntNodet->getTextContent();				
                /*if (DRIVER_INST_REQ == cDriverInstRequired)
                {
                    DEBUGL1("Driver is already installed and this is not first boot\n");
                    return STATUS_OK;
                }*/
		if( NO_DRIVER == driverInst)
		{
			DEBUGL4("CSmbNMO::installPnPDriver,function exit Driver is already installed\n");
			return STATUS_OK;
        }
                /*Scanning directory for file $EB2/NoBuildItems/AL/Network/pointNprint */
                iFcount = scandir(directory.c_str(), &filelist, readDriverFileType, alphasort);
                if(iFcount < 0) {
                    DEBUGL1("No files found under f0lder %s, Failed to install\n",directory.c_str());
                    return STATUS_FAILED;
                }

                cFileNames.clear();
                cCopyCommand.clear();

                /*Creating parameter list for File node of UploadPnP command*/
                for(iIndex = 0; iIndex < iFcount; iIndex++)  {
		    
                    DEBUGL5(" Currently creating string for %s\n", filelist[iIndex]->d_name);
		    CString driverName = CString(filelist[iIndex]->d_name);
		    DEBUGL5(" Currently creating string for %s\n", driverName.c_str());
		    if("ps3.zip" == driverName)
		    {
			if((ONLY_PS3 == driverInst)||(ONLY_PS3_XPS == driverInst)||(ONLY_UNIV_PS3 ==driverInst) || (ONLY_UNIV_PS3_XPS == driverInst))
			{
                    cFileNames += "<File>"+ CString(filelist[iIndex]->d_name) +"</File>";
                    		/*Copy the data from NoBuildItems/AL/Network/PnPData to tmp folder*/ 
                    		driverPath.clear();
				driverPath = getenv("EB2");
				driverPath += DRIVERNOBUILDITEM + CString(filelist[iIndex]->d_name) + " ";
				cCopyCommand  = "cp -f " + driverPath + SMB_DRIVER_PATH ;
       	       	      		system (cCopyCommand.c_str());
                    		free(filelist[iIndex]);
			}	
			else
				continue;

		    }
//code change to make OKI and TTEC common code base - 12072012
		    else if("univ.zip" == driverName)
		    {
			if((ONLY_UNIV == driverInst)||(ONLY_UNIV_XPS == driverInst)||(ONLY_UNIV_PS3 ==driverInst) || (ONLY_UNIV_PS3_XPS == driverInst))
			{
                    		cFileNames += "<File>"+ CString(filelist[iIndex]->d_name) +"</File>";
                    /*Copy the data from NoBuildItems/AL/Network/PnPData to tmp folder*/ 
                    driverPath.clear();
				driverPath = getenv("EB2");
				driverPath += DRIVERNOBUILDITEM + CString(filelist[iIndex]->d_name) + " ";
				cCopyCommand  = "cp -f " + driverPath + SMB_DRIVER_PATH ;
       	       	      		system (cCopyCommand.c_str());
                    		free(filelist[iIndex]);
			}	
			else
				continue;
		    }
		    else if("xps.zip" == driverName)
		    {
			if((ONLY_XPS == driverInst)||(ONLY_PS3_XPS == driverInst)||(ONLY_UNIV_XPS ==driverInst) || (ONLY_UNIV_PS3_XPS == driverInst))
			{
                    		cFileNames += "<File>"+ CString(filelist[iIndex]->d_name) +"</File>";
                    		/*Copy the data from NoBuildItems/AL/Network/PnPData to tmp folder*/ 
                    		driverPath.clear();
				driverPath = getenv("EB2");
				driverPath += DRIVERNOBUILDITEM + CString(filelist[iIndex]->d_name) + " ";
				cCopyCommand  = "cp -f " + driverPath + SMB_DRIVER_PATH ;
       	       	      		system (cCopyCommand.c_str());
                    		free(filelist[iIndex]);
			}	
			else
				continue;
		    }
		    else
		    {
                    	cFileNames += "<File>"+ CString(filelist[iIndex]->d_name) +"</File>";

                    	/*Copy the data from NoBuildItems/AL/Network/PnPData to tmp folder*/ 
                    	driverPath.clear();
                    driverPath = getenv("EB2");
                    driverPath += DRIVERNOBUILDITEM + CString(filelist[iIndex]->d_name) + " ";
                    cCopyCommand  = "cp -f " + driverPath + SMB_DRIVER_PATH ;
                    system (cCopyCommand.c_str());
                    free(filelist[iIndex]);
                }
                }
                /*Free the file list structure, as scan directory internally use malloc system call*/
                free(filelist);

                /*Clearing All the temp veriable, for reuse*/
                szCmd.clear();

                DEBUGL5("Installing %s Samba PnP driver in MFP form NoBuildItem \n" ,cFileNames.c_str());

                /*Command creation for UploadPnP*/
                szCmd = "<Command>\
                         <UploadPnP>\
                         <commandNode>Network</commandNode>\
                         <Params>\
                         <source>"\
                         + cFileNames +
                         "<name>Session</name>\
                         </source>\
                         <destination>\
                         <name>PointandPrint</name>\
                         </destination>\
                         </Params>\
                         </UploadPnP>\
                         </Command>";
                DEBUGL5("Execute command   = [%s]\n",szCmd.c_str());

                msgDoc = NULL ;
                Status stat = m_hdb->CreateTempDocument(DRIVER_UPDATE_CMD_DOC, msgDoc, szCmd.c_str());
                if (stat != STATUS_OK || !msgDoc) {
                    DEBUGL1("CSmbNMO::installPnPDriver, Could not create tmp doc for update Driver, continue \n");
                    return STATUS_FAILED;
                }

                /*Creating CUICCommandRef class for HandleUICCommand command for updating driver*/
                pUICCommand = new CUICCommand(msgDoc, m_hdb);
                if (!pUICCommand)
                {
                    DEBUGL1("CSmbNMO::installPnPDriver,Could not create message command for update Driver, continue \n");
                    return STATUS_FAILED;	
                }

                /*Call HandleUICCommand for updating driver in device, for download*/
                CString sRetValue = HandleUICCommand(pUICCommand) ;
                if ("STATUS_FAILED" == sRetValue)
                {
                    DEBUGL1("CSmbNMO::installPnPDriver,Driver update Failed \n");	
                    return STATUS_FAILED;
                }
                DEBUGL5("Succesfully installed , PnP driver\n");
                /* webinstaller PnP: drivers share to be made available only if print share is available*/
                CIniFile ini(m_confFile);
                CString pri_available = "";
                ElementRef pri = chelper::NavigatePath("Print/available",ini.GetRoot(), true);
                if(pri){
                    pri_available = pri->getTextContent();
                    if("1" == pri_available) {
                        for (int line = 1; line < 4; line++ ){
                            CString driv_str = readversionfile(line);
                            if((driv_str != "STATUS_FAILED") && (driv_str != ""))
                                SavetoConfig((driv_str + CString ("_driver/available")),"1");
                        }
                    }
                }
                /* webinstaller PnP end */
                /*Setting driver instillation done flag in Network DONE*/
                //pDriverIntNodet->setTextContent(DRIVER_INST_REQ);
				DEBUGL4("[%s] function exit\n",__FUNCTION__);
                return STATUS_OK;	
            }


            /*
             * 20101006 Manoj: 
             * executePanelComand()
             * Finction discription : This API will set Mode_08/Code_8914/SubCode_5 for MFP FILE_SHARE function based on request	
             * 			This will be called from enableMFPFunction()
             */
            void CSmbNMO::executePanelComand(DocumentRef pPanelRef, CString cValue)
            {
                DEBUGL4("[%s] function enter\n",__FUNCTION__);
				CString cPathToSet = "DiagnosticMode/Mode_08/Code_8914/SubCode_5";
                ElementRef pStatusNode = m_hdb->BindToElement(pPanelRef, cPathToSet);
                if (!pStatusNode)	
                {
                    DEBUGL1("CSmbNMO::%s  Unable to bind %s in Panel Doc returning\n",__FUNCTION__,cPathToSet.c_str());
                    return ;
                }
                DEBUGL4("CSmbNMO::%s function exit Setting %s in %s \n",__FUNCTION__,cValue.c_str(), cPathToSet.c_str());
                pStatusNode->setTextContent(cValue);
				
                return ;
            }

            /*
             * 20101006 Manoj: 
             * enableMFPFunction()
             * Finction discription : This API will set Mode_08/Code_8914/SubCode_5 for MFP FILE_SHARE function 	
             * 			Code_8914/SubCode_5 0->	MFP function FILE_SHARE will be disabled from Panel UI
             * 			Code_8914/SubCode_5 1-> MFP function FILE_SHARE will be enabled from Panel UI
             */
            void CSmbNMO::enableMFPFunction(ElementRef configDoc)
            {
				DEBUGL4("[%s] function enter\n",__FUNCTION__);
				HierarchicalDBRef hdb = HierarchicalDB::Acquire(0);
                NodeRef pNodeRef	= NULL;

                ElementRef pElementNode = NULL;
                DocumentRef pDocNode = NULL;

                CString cSambaStatus	= "";
                CString cPrintShare	= "";
                CString cFileShare	= "";

                //Status sGetStatus	= STATUS_FAILED;

                if(m_hdb->OpenDocument("Panel", pDocNode, getenv("AL_NET_PERMANENT_DOM")) != STATUS_OK)
                {
                    DEBUGL1("CSmbNMO::%s  Unable to open Panel Document returning\n",__FUNCTION__);
                    return ;
                }
		hdb->BeginTransaction(pDocNode,eWRITE,true);
                pElementNode =m_hdb->BindToElement(configDoc, "enabled");
                if (pElementNode)
                {
                    cSambaStatus = pElementNode->getTextContent();
                    pElementNode = NULL;
                }
                else
                    DEBUGL1("CSmbNMO::%s Failed to get enabled \n",__FUNCTION__);

                pElementNode =m_hdb->BindToElement(configDoc, "printingEnabled");
                if (pElementNode)
                {
                    cPrintShare = pElementNode->getTextContent();
                    pElementNode = NULL;
                }
                else
                    DEBUGL1("CSmbNMO::%s Failed to get printingenabled \n",__FUNCTION__);

                pElementNode =m_hdb->BindToElement(configDoc, "fileshareEnabled");
                if (pElementNode)
                {
                    cFileShare = pElementNode->getTextContent();
                    pElementNode = NULL;
                }
                else
                    DEBUGL1("CSmbNMO::%s Failed to get fileshareenabled \n",__FUNCTION__);

                DEBUGL5("CSmbNMO::%s cSambaStatus = %s, cPrintShare = %s and cFileShare = %s \n"\
                        ,__FUNCTION__, cSambaStatus.c_str(), cPrintShare.c_str(), cFileShare.c_str());

                if ((SAMBA_ENABLE == cSambaStatus) && (SAMBA_ENABLE == cPrintShare) && (SAMBA_ENABLE == cFileShare))
                {
                    DEBUGL5("CSmbNMO::%s cSambaStatus:cPrintShare:cFileShare = 1:1:1 Enabling MFP function Category-08/C-8914/S-5\n",__FUNCTION__);
                    executePanelComand(pDocNode, SAMBA_ENABLE);
                }
                //else if ((SAMBA_DISABLE == cSambaStatus) && (SAMBA_DISABLE == cPrintShare) && (SAMBA_DISABLE == cFileShare))
                else if ((SAMBA_DISABLE == cSambaStatus)) /*TODO MANOJ 20101006 to check condition when all will be disabled*/
                {
                    DEBUGL5("CSmbNMO::%s cSambaStatus:cPrintShare:cFileShare = 0:0:0 Disabling MFP function Category-08/C-8914/S-5\n",__FUNCTION__);
                    executePanelComand(pDocNode, SAMBA_DISABLE);
                }
                else if ((SAMBA_ENABLE == cSambaStatus) && (SAMBA_DISABLE == cPrintShare) && (SAMBA_ENABLE == cFileShare))
                {
                    DEBUGL5("CSmbNMO::%s cSambaStatus:cPrintShare:cFileShare = 1:0:1 Enabling MFP function Category-08/C-8914/S-5\n",__FUNCTION__);
                    executePanelComand(pDocNode, SAMBA_ENABLE);
                }
                else if ((SAMBA_ENABLE == cSambaStatus) && (SAMBA_ENABLE == cPrintShare) && (SAMBA_DISABLE == cFileShare))
                {
                    DEBUGL5("CSmbNMO::%s cSambaStatus:cPrintShare:cFileShare = 1:1:0 Disabling MFP function Category-08/C-8914/S-5\n",__FUNCTION__);
                    executePanelComand(pDocNode, SAMBA_DISABLE);
                }
                else
                    DEBUGL1("CSmbNMO::%s None of the condition satisfied return\n", __FUNCTION__);	
				hdb->EndTransaction(pDocNode,eWRITE);
            	DEBUGL4("[%s] function exit\n",__FUNCTION__);
			    return ;
            }
            /*
             * 20110119 Manoj: 
             * getWinsAddress()
             * Finction discription : This API will set wins IP address in proper format (Removing wins server2 from config file)	
             * @in : configRoot on Network DOM, starting from SMB
             */
            CString CSmbNMO::getWinsAddress(ElementRef configRoot, CString &wins1, CString &wins2)
            {
				DEBUGL4("[%s] function enter\n",__FUNCTION__);
				if (!configRoot)
                {
                    DEBUGL1("[%s] Unable to find root node, verify input\n",__FUNCTION__);
                    return "";
                }	
                CString wsWinsAdd1;
                CString wsWinsAdd2;
                CString csRetString;

                ElementRef pWinsServerNode = NULL;

                pWinsServerNode = chelper::NavigatePath(CString("wins"), configRoot, true);
                if (pWinsServerNode)
                {
                    wsWinsAdd1=pWinsServerNode->getTextContent();
                    wins1 = wsWinsAdd1;
                    pWinsServerNode = NULL;
                }
                DEBUGL5("[%s] Address of wins server1 = [%s]\n",__FUNCTION__, wsWinsAdd1.c_str());
                pWinsServerNode = chelper::NavigatePath( CString("wins2"), configRoot, true);
                if (pWinsServerNode)
                {
                    wsWinsAdd2=pWinsServerNode->getTextContent();
                    wins2 = wsWinsAdd2;
                    pWinsServerNode = NULL;
                }
                DEBUGL5("[%s] Address of wins server2 = [%s]\n",__FUNCTION__, wsWinsAdd2.c_str());
                csRetString = wsWinsAdd1;
                if ("" != wsWinsAdd2)
                {
                    csRetString = wsWinsAdd1;
                    if ("" != wsWinsAdd1)
                        csRetString += ", ";
                    csRetString += wsWinsAdd2;
                }
                DEBUGL4("[%s]function exit returning wins address = %s\n",__FUNCTION__,csRetString.c_str());
                return csRetString;
            }

            CString CSmbNMO::PrepareQuoteString(CString str)
            {
                DEBUGL4("Inside PrepareQuoteString():function enter String for conversion is %s \n",str.c_str());
                string szToFind = "\"";
                string szToFind1 = "`";
                string szToFind2 = "$";
                string szToReplace = "\\\"";
                string szToReplace1 = "\\`";
                string szToReplace2 = "\\$";
                size_t found=0;
                size_t found0 =0;

                if (str.length() == 0){
					DEBUGL4("[%s] function exit\n",__FUNCTION__);
                    return str;
                }
                while(string::npos != (found = str.find(szToFind,found0)))
                {
                    str.replace(found, szToFind.length(), szToReplace);
                    found0 = found + szToReplace.size();
                }
                found=0;
                found0=0;
                while(string::npos != (found = str.find(szToFind1,found0)))
                {
                    str.replace(found, szToFind1.length(), szToReplace1);
                    found0 = found + szToReplace1.size();
                }
                found=0;
                found0=0;
                while(string::npos != (found = str.find(szToFind2,found0)))
                {
                    str.replace(found, szToFind2.length(), szToReplace2);
                    found0 = found + szToReplace2.size();
                }
                str = "\"" + str + "\"";
				DEBUGL4("[%s] function exit\n",__FUNCTION__);
                return str;
            }

	/*Disable Webinstaller folders for SCC starts here 20130827*/
	 void CSmbNMO::webInstallerDisable(CIniFile &ini)
            {
                DEBUGL4("[%s] function enter\n",__FUNCTION__);
				CString driv_str = "univ";
                ElementRef uni = chelper::NavigatePath((driv_str + CString ("_driver/available")),ini.GetRoot(),true);
                if(uni)
                        uni->setTextContent("0");
                driv_str = "xps";
                ElementRef xps = chelper::NavigatePath((driv_str + CString ("_driver/available")),ini.GetRoot(),true);
                if(xps)
                        xps->setTextContent("0");
                driv_str = "ps3";
                ElementRef ps3 = chelper::NavigatePath((driv_str + CString ("_driver/available")),ini.GetRoot(),true);
                if(ps3)
                        ps3->setTextContent("0");
                ini.SetModified();
				DEBUGL4("[%s] function exit\n",__FUNCTION__);
             }
	/*Disable Webinstaller folders for SCC ends here 20130827*/
            /* Delete existing users from /etc/passwd and passdb.tdb file
             * when GuestLogon is Disabled or AD/Domain enabled
             * This function deletes all users from passdb.tdb file but only users having 9998 or 9999 from /etc/passwd file.
             * because other users are built-in or added by some other application.*/

            CString CSmbNMO::DBUsersDeletion(void)
            {
                DEBUGL4("[%s] function enter\n",__FUNCTION__);
                CString csRet = "STATUS_OK";

                CString cmd = getenv("EB2");
                cmd += "/bin/pdbedit -L -d0 | awk -F :  '{ print $1\" \"$2 }'";
                FILE *fp = popen(cmd.c_str(), "r");
                if(fp)
                {
                    char username[128]={0};
                    char userid[10] = {0};
                    while(fscanf(fp,"%s %s\n",username, userid) != EOF) //get the users from passdb
                    {
                        CString csUsername = username;
                        CString csUserID = userid;
                        if("" != csUsername)
                        {
                            DEBUGL2("SMBNMO:DBUsersDeletion(): deleting user:[%s] having id[%s]\n",csUsername.c_str(), csUserID.c_str());
                            if(("9998" == csUserID) || ("9999" == csUserID)) //delete from both passwd and passdb.tdb
                            {
                                CString csDeleteCmd = getenv("EB2");
                                csUsername=PrepareQuoteString(csUsername);
                                csDeleteCmd += "/bin/smbpasswd -x " + csUsername;
                                int iRet = system(csDeleteCmd.c_str());
                                int iErr = WEXITSTATUS(iRet);
                                if(iErr != 0)
                                {
                                    DEBUGL1("SMBNMO:DBUsersDeletion() return value of smbpasswd delete user command [%s] is [%d]\n",csDeleteCmd.c_str(),iErr);
                                    /*If user deletion failed, it is better to retry and delete. Sometimes system call may fail.
                                     * So even after 3 retry it failed to delete , then old user is retained,
                                     * Since we are returning "STATUS_FAILED" from Update() function to NSM.*/
                                    for(int i = 1; i <=3; i++)
                                    {
                                        iRet = system(csDeleteCmd.c_str());
                                        iErr = WEXITSTATUS(iRet);
                                        DEBUGL1("SMBNMO:DBUsersDeletion() return value after retry[%d] for deletecommand [%s] is [%d]\n",i,csDeleteCmd.c_str(),iErr);
                                        if(iErr == 0)
                                        {
                                            DEBUGL5("SMBNMO:DBUsersDeletion(): user deletion success after [%d] times\n",i);
                                            csRet = "STATUS_OK";
                                            break;
                                        }
                                        else
                                        {
                                            DEBUGL1("SMBNMO:DBUsersDeletion(): user deletion failed for [%d] times\n",i);
                                            csRet = "STATUS_FAILED";
                                        }
                                    }
                                    if(csRet != "STATUS_OK")
                                    {
                                            DEBUGL1("SMBNMO:DBUsersDeletion(): user deletion failed for all tries\n");
                                            pclose(fp);
                                            return csRet; //return only if failed, else return at the end of function
                                    }
                                }
                                csDeleteCmd.clear();
                                /*20180411: STFR_19384, there is possibility that user can hold file_share process longer time,
                                 * So when new user is getting added, we need to remove old user with -f option(forcefully deletion)
                                 */
                                csDeleteCmd = "/usr/sbin/userdel -f " + csUsername; // delete user from /etc/passwd
                                DEBUGL5("SMBNMO:DBUsersDeletion() Delete command is [%s]\n",csDeleteCmd.c_str());
                                iRet = system(csDeleteCmd.c_str());
                                iErr = WEXITSTATUS(iRet);
                                if(iErr != 0)
                                {
                                    DEBUGL1("SMBNMO:DBUsersDeletion() return value of delete user command [%s] is [%d]\n",csDeleteCmd.c_str(),iErr);
                                    /*If user deletion failed, it is better to retry and delete. Sometimes system call may fail.
                                     * So even after 3 retry it failed to delete , then old user is retained,
                                     * Since we are returning "STATUS_FAILED" from Update() function to NSM.*/
                                    for(int i = 1; i <=3; i++)
                                    {
                                        iRet = system(csDeleteCmd.c_str());
                                        iErr = WEXITSTATUS(iRet);
                                        DEBUGL1("SMBNMO:DBUsersDeletion() return value after retry[%d] for deletecommand [%s] is [%d]\n",i,csDeleteCmd.c_str(),iErr);
                                        if(iErr == 0)
                                        {
                                            DEBUGL5("SMBNMO:DBUsersDeletion(): user deletion success after [%d] times\n",i);
                                            csRet = "STATUS_OK";
                                            break;
                                        }
                                        else
                                        {
                                            DEBUGL1("SMBNMO:DBUsersDeletion(): user deletion failed for [%d] times\n",i);
                                            csRet = "STATUS_FAILED";
                                        }
                                    }
                                    if(csRet != "STATUS_OK")
                                    {
                                            DEBUGL1("SMBNMO:DBUsersDeletion(): user deletion failed for all tries\n");
                                            pclose(fp);
                                            return csRet; //return only if failed, else return at the end of function
                                    }
                                }
                            }
                            else
                            {
                                CString csDeleteCmd = getenv("EB2");
                                csUsername=PrepareQuoteString(csUsername);
                                csDeleteCmd += "/bin/smbpasswd -x " + csUsername;
                                int iRet = system(csDeleteCmd.c_str());
                                int iErr = WEXITSTATUS(iRet);
                                if(iErr != 0)
                                {
                                    DEBUGL1("SMBNMO:DBUsersDeletion() return value of smbpasswd delete user command [%s] is [%d]\n",csDeleteCmd.c_str(),iErr);
                                    /*If user deletion failed, it is better to retry and delete. Sometimes system call may fail.
                                     * So even after 3 retry it failed to delete , then old user is retained,
                                     * Since we are returning "STATUS_FAILED" from Update() function to NSM.*/
                                    for(int i = 1; i <=3; i++)
                                    {
                                        iRet = system(csDeleteCmd.c_str());
                                        iErr = WEXITSTATUS(iRet);
                                        DEBUGL1("SMBNMO:DBUsersDeletion() return value after retry[%d] for deletecommand [%s] is [%d]\n",i,csDeleteCmd.c_str(),iErr);
                                        if(iErr == 0)
                                        {
                                            DEBUGL5("SMBNMO:DBUsersDeletion(): user deletion success after [%d] times\n",i);
                                            csRet = "STATUS_OK";
                                            break;
                                        }
                                        else
                                        {
                                            DEBUGL1("SMBNMO:DBUsersDeletion(): user deletion failed for [%d] times\n",i);
                                            csRet = "STATUS_FAILED";
                                        }
                                    }
                                    if(csRet != "STATUS_OK")
                                    {
                                            DEBUGL1("SMBNMO:DBUsersDeletion(): user deletion failed for all tries\n");
                                            pclose(fp);
                                            return csRet; //return only if failed, else return at the end of function
                                    }
                                }
                                csDeleteCmd.clear();
                            }
                        }
                        else
                        {
                            DEBUGL5("SMBNMO:DBUsersDeletion() SMBserverAuthentication No guest users present\n");
                            break;
                        }
                    }
                    pclose(fp);
                }
                else
                {
                    DEBUGL1("SMBNMO:DBUsersDeletion() popen failed\n");
                    csRet = "STATUS_FAILED";
                }

                DEBUGL4("[%s] function exit\n",__FUNCTION__);
                return csRet;
            }

            CString CSmbNMO::Update(dom::DocumentRef configDoc, ElementRef transRoot,CString &depResult, CString startMode)
            {
                DEBUGL4("[%s] function enter\n",__FUNCTION__);
				HierarchicalDBRef hdb = HierarchicalDB::Acquire(0);
                hdb->BeginTransaction(configDoc,eWRITE,true);
                DEBUGL5("Update: BeginTransaction with eWRITE is called for configDoc\n");
                ElementRef configRoot = hdb->BindToElement(configDoc,m_serviceXpath);
                if(!configRoot)
                {
                    DEBUGL1("Update: could not get configRoot for [%s]\n",m_serviceXpath.c_str());
					DEBUGL5("Update: EndTransaction with eWRITE is called for configDoc\n");
                    hdb->EndTransaction(configDoc,eWRITE);
                    return "STATUS_FAILED";
                }

                //DEBUGL4("Update: update called for config db path '%s', xpath = '%s'\n",
                //configRoot->ComputeFullPath().c_str(),configRoot->getCurrentPath().c_str());
                bool bRestartRequired = false;
                if(transRoot)
                    DEBUGL5("Update: update called for trans doc path '%s'\n",
                            transRoot->ComputeFullPath().c_str());

                CString error, tmp;

                CIniFile::Traits traits;
                needDependRst=true;
                needNotify=false;
                bool check=false;
                bool reload = false;
                // this flag will set to true in case there is no conf file key for xpath and yet there is change in configuration

                CString xpathForPDC;
                CString xpathForBDC;
                CString tmpConfPath;

                //File exist check.
                CheckConfile(m_confFile);

                /*Updating samba Map for validating Samba conf file*/
                UpdateSambaMap();
                CString cConfigFileRepairStatus =  ValidateSmbConf();
                if ("CONFIG_FILE_RECOVERED" == cConfigFileRepairStatus){
                    DEBUGL5("Update: CONFIG_FILE_RECOVERED \n");
					bRestartRequired = true;
                }

                if (startMode == "00") {
                    if (true == UpdateSambaLogInfo()){
                        bRestartRequired = true;
                    }
                }


                CIniFile ini(m_confFile);

                DEBUGL5("[%s] Config file  %s \n",__FUNCTION__, cConfigFileRepairStatus.c_str());

                ElementRef root = transRoot;
                CString domainCntrl="0.0.0.0",domain,group,activeDirectory, Fulldomain, Full_AD,clientAuth,enableVar,serverAuth;
                // apply mapping to select (and possibly rename) specific paths.

                ElementRef enableDomainRef = chelper::NavigatePath( CString("enableDomainLogin"), configRoot, true);
                ElementRef pPasswordReset = chelper::NavigatePath("password", configRoot, true);
                ElementRef LogUser = chelper::NavigatePath(CString("logonUser"), configRoot, true);
                //ElementRef winDomainRef= chelper::NavigatePath(CString("winDomainAuthMethod"), configRoot, true);
                ElementRef authLoginRef= chelper::NavigatePath(CString("enableAuthLogin"), configRoot, true);
                ElementRef clientAuthRef= chelper::NavigatePath(CString("clientAuthMethod"), configRoot, true);
                ElementRef serverAuthRef= chelper::NavigatePath(CString("serverAuthGuestLogon"), configRoot, true);
                ElementRef serverAuthUserRef = chelper::NavigatePath(CString("serverAuthUser"), configRoot, true);
                ElementRef serverAuthPasswdRef = chelper::NavigatePath("serverAuthPassword", configRoot, true);


                //Manoj 20110121 Only one node is possible in driver cleanup for samba
                ElementRef DrivercleanupRef = chelper::NavigatePath( CString("driverCleanup"), transRoot, true);
                if (DrivercleanupRef)
                {
                    CString drivercleanup;
                    drivercleanup=DrivercleanupRef->getTextContent();
                    DEBUGL3("08 - 8975 code value is %s ",drivercleanup.c_str());
                    if("1" == drivercleanup)
                    {
                        /*2012-sep-04 driver clean up of back files and version.txt files existing in NoBuildItems path should not be done*/
                        /*CString fNAME="rm -rf " + (CString)getenv("EB2") +  CString(DRIVERNOBUILDITEM) + "*";
                        system(fNAME.c_str());
                          fNAME.clear();*/

                        CString fNAME="rm -rf " + CString(SMB_DRIVER_PATH) + "*";
                        system(fNAME.c_str());
                        fNAME.clear();

                        fNAME="rm -rf " + CString(DRIVERNTTDB) +"nt*";
                        system(fNAME.c_str());
                        fNAME.clear();

                        fNAME="rm -rf " + CString(DRIVERPRINTTDB) +"*";
                        system(fNAME.c_str());
                        fNAME.clear();
						DEBUGL5("update: EndTransaction with eWRITE was called for configDoc\n");
                        hdb->EndTransaction(configDoc,eWRITE);
                        //DrivercleanupRef->setTextContent("0");
						DEBUGL4("[%s] function exit\n",__FUNCTION__);
                        return "STATUS_OK";
                    }
                    else
                    {
                        DEBUGL3("08 - 8975 code value set to zero");
                    }
                }

		/*fix for stae2 -08-9599 code start*/
                bool samba_enabled=0;  //to change from 1,0,0 combination to 1,1,1
                CString arr[]={m_enabledXPath,CString("printingEnabled"),CString("fileshareEnabled")};
                DEBUGL5("arr[0]=%s::,  arr[1]=%s::, arr[2]=%s::i\n",arr[0].c_str(),arr[1].c_str(),arr[2].c_str());

				ElementRef conf_arr[3];
				ElementRef trans_arr[3];
                for(int i=0;i<=2;i++){
                        conf_arr[i] = chelper::NavigatePath(arr[i], configRoot, true);
                        trans_arr[i] = transRoot ? chelper::NavigatePath(arr[i], transRoot) : NodeRef();
                        if (!conf_arr[i]) {
                                error = "Unable to get 'enable' setting under config db path for samba " +
                                configRoot->ComputeFullPath();
								DEBUGL1("[%s]CONF ARR ERROR",__FUNCTION__);
                        }
                        else
                        {
                                if (trans_arr[i]){
										DEBUGL5("trans_arr[i]=%s\n",trans_arr[i]->getTextContent().c_str());
                                        conf_arr[i]->setTextContent(trans_arr[i]->getTextContent());
								}
                                if (conf_arr[i]->getTextContent().empty()){
				        				conf_arr[i]->setTextContent("0");
								}
                        }
                }
				if((conf_arr[0])&&(conf_arr[1])&&(conf_arr[2])){
                	samba_enabled=conf_arr[0]->getTextContent() == "1" && conf_arr[1]->getTextContent() == "0" && conf_arr[2]->getTextContent() == "0";
					DEBUGL5("Update() : samba_enabled : %s\n", samba_enabled ? "yes" : "no");
                }
                if(samba_enabled)
                {
						conf_arr[1]->setTextContent(SAMBA_ENABLE);
                        conf_arr[2]->setTextContent(SAMBA_ENABLE);
                }
		/*fix for stae2 -08-9599 code end*/       
                for (map<CString,CString>::iterator i = m_xpathMap.begin();i != m_xpathMap.end(); i++)
                {
                    ////////////////////////////////////////////////////////////////
                    //Update Permanent DOM. and Get newest value from Permanent DOM.
                    ////////////////////////////////////////////////////////////////
                    CString TransVal;//Trans val.If trans is't present,this value should be set Null
                    CString PermVal;//Permanent val
                    CString domainCntrl="0.0.0.0",domain,group,encFlag,outputDecryptPassword,outputEncryptedPassword,winDomain;

                    DEBUGL5("The NSM.XML is as below ::: \n <--[  %s  ]-->\n",i->second.c_str());

                    //For keep PDC and BDC data.
                    DEBUGL5("Update: sec[%s] first[%s]\n",i->second.c_str(),i->first.c_str());
                    if(i->second == "global/password~server"){
                        //save conf path.
                        tmpConfPath = i->second;
                        if(i->first == "primaryDomainController"){
                            //save xpath
                            xpathForPDC = i->first;
                        }
                        else if(i->first == "backupDomainController"){
                            //save xpath
                            xpathForBDC = i->first;
                        }
                    }
                    DEBUGL5("Update: xpathForPDC[%s] xpathForBDC[%s]\n",xpathForPDC.c_str(),xpathForBDC.c_str());

                    //Get Item value from Permanent Dom.
                    ElementRef permRef = chelper::NavigatePath(i->first, configRoot, true);
                    if(!permRef){
                        DEBUGL1("Update: not present[%s] in Permanent Dom.It might be bug.\n", i->first.c_str());
                        continue;
                    }
                    PermVal = permRef->getTextContent();
                    DEBUGL5("Update: Get permanent val.[%s:%s]\n",i->first.c_str(),PermVal.c_str());
                    if (chelper::GetAttributeValue(permRef, "encryption") == "1"){
                        DEBUGL5("Update: Found attribute [before decryption] -%s\n", PermVal.c_str());
                        DecryptPassword(PermVal,outputDecryptPassword);
                        encFlag = "1";
                        PermVal = outputDecryptPassword;
                        DEBUGL5("Update: Found attribute [after decryption] -%s\n", PermVal.c_str()); 

                    }
                    if(root){//There is item in TransDoc.
                        //Get Item value from TransDoc.
                        needNotify=true; 
                        ElementRef transRef = chelper::NavigatePath(i->first, root);
                        if (!transRef){//If item is not there in TransDoc.
                            DEBUGL5("Update: not present[%s] in TransDom. skpping\n", i->first.c_str());
                            continue;
                        }
                        TransVal = transRef->getTextContent();

                        DEBUGL5("Update: Tere is Trans data.i->first[%s]\n",i->first.c_str());
                        if(i->first == "domainName" && TransVal!=""){
                            domain = TransVal;
                            Fulldomain=TransVal;
                            /* To remove the trailing .com/.org/.net etc from the domain name */
                            string whitespaces (".");
                            size_t found;
                            found = domain.find_first_of(whitespaces);
                            if (found!=string::npos)
                                domain.erase(found);
                            enableDomainRef->setTextContent("1");
                            authLoginRef->setTextContent("1");
                            DEBUGL3("Set enableDomainRef and  to '1' -> AD/Domain \n");
                        }			
#if 1
                        if(i->first == "groupName" && domain==""  && activeDirectory=="" && TransVal!=""){
                            DEBUGL5("Set enableDomainRef to '0' -> WorkGroup \n");       
                            group = TransVal;
                            enableDomainRef->setTextContent("0");         
                            authLoginRef->setTextContent("0");
                            /* 20100726 Manoj :: TA is not sending password field in case we jump to WG from Domain
                               explicitly reseting passwd to NULL 
                               */
                            if (pPasswordReset){
                                DEBUGL5("[%s] Resetting password as it workgroup \n",__FUNCTION__);
                                pPasswordReset->setTextContent("");
                            }

                        }
#endif

                        if(i->first == "adsName"){
                            activeDirectory = TransVal;
                            Full_AD=TransVal;

                            /* To remove the trailing .com/.org/.net etc from the domain name */
                            string whitespaces (".");
                            size_t found;
                            found = activeDirectory.find_first_of(whitespaces);
                            if (found!=string::npos)
                                activeDirectory.erase(found);
                            else
                                activeDirectory.clear();         /* str is all whitespace*/


                        }

                        if(i->first == "deviceName"){
                            if(TransVal == ""){//Set default value,when trans data is NULL value. 
                                CString serialno;
                                //Get serial Number. 
                                createSerialNo(serialno);
                                TransVal = "MFP"+serialno;
                                DEBUGL5("Update: transVal[(Null)] is NULL.Change to default value[%s]\n",TransVal.c_str());
                            }
                        }
                        if (i->first == "maxConnection")
                        {	
							//Changes for Invalid String comparison
                            //if (TransVal < MIN_SAMBA && TransVal >MAX_SAMBA)

							int iTransValConn = atoi(TransVal.c_str()); //transval of maxConnection
							int iMinSambaConn = atoi(MIN_SAMBA); //Min samba connection
							int iMaxSambaConn = atoi(MAX_SAMBA); //max samba connection

                            if ((iTransValConn < iMinSambaConn) || (iTransValConn > iMaxSambaConn))
                            {

                                TransVal = DEFAULT_SAMBA;
                                DEBUGL5("Update: Default Connections(13) is set now...Check SMB.CONF\n");
                            }
                            else 
                            {   	
                                DEBUGL5("Update: Connections(as per 08 mode) is set now...Check SMB.CONF\n");
                            }
                        }		

			if(samba_enabled && ((i->first =="printingEnabled") || (i->first =="fileshareEnabled"))){
                                TransVal=PermVal;
                                DEBUGL5(" 08-9599 condition check");
			}

                        CString tmppasswd;
                        if(TransVal != PermVal){
                            DEBUGL5("Update: transVal[%s] PermVal[%s] are different for i->first [%s].\n",TransVal.c_str(),PermVal.c_str(),(i->first).c_str());
                            if (encFlag == "1"){
                                DEBUGL5("Update: Found a node that needs to be encrypted -%s\n", TransVal.c_str());
                                // Encrypt trans val and copied it in transVal
                                Status stat = EncryptPassword(TransVal,outputEncryptedPassword);
                                if(stat == STATUS_FAILED){
                                    DEBUGL1("Update: Failed EncryptPassword()\n");
                                    //	error = "Password encryption is failed";
                                }

                                TransVal = outputEncryptedPassword;
                                DEBUGL5("Update: Found a node that needs to be encrypted -%s\n", TransVal.c_str());
                                DecryptPassword(outputDecryptPassword, tmppasswd);
                                DEBUGL5("Decrypt password -- %s\n", tmppasswd.c_str());
                                /*20120126 Password wil not be present in conf file, In case user is modified passws restart is required*/
                                bRestartRequired = true;
                                /*20120124 : User has changed password*/
                                DEBUGL5("[%s] User have changed password bRestartRequired[%d] \n",__FUNCTION__, bRestartRequired);
                            }
                            if(i->first == "logonUser"){
                                /*20120126 username wil not be present in conf file, In case user is modified username restart is required*/
                                DEBUGL5("[%s] User Has changed username Restart is required\n",__FUNCTION__);
                                bRestartRequired = true;
                            }	
			    if(i->first == "serverAuthUser")
			    {
				    /*we can allow only one guest user. so , we are checking whether new user name is entered.
				     *                                  * If so, delete the old user from /etc/passwd file.*/
				    /* empty user is not valid: if 8852 is "0" there must be user in 8853, but 8854 empty is OK.
				     * If GuestLogon is disabled for first time and no user is set in 8853,
				     * empty user will be passed to script and no user will be added to passwd file.
				     * */
				    if(TransVal == "")
				    {
					    needDependRst=false;
					    DEBUGL2("Update: Empty user is invalid input, SelfDiagnostic mode allows empty values\n");
					    return "STATUS_FAILED";
				    }
				    else
				    {
					    DEBUGL2("Update: serverAuthUser: Deleting users\n");
					    CString csDeleteRes = DBUsersDeletion();//delete user from passwd and passdb.tdb files
					    if(csDeleteRes != "STATUS_OK")
					    {
						    DEBUGL2("Update: serverAuthUser: User deletion failed. Hence we need to return error to TA or Self-Diagnostic\n");
						    return "STATUS_FAILED";
					    }
					    else
						    DEBUGL5("Update: serverAuthUser: successfully deleted users\n");
				    }
			    }

                            /*Update Permanent Dom.And get newest value.*/
                            permRef->setTextContent(TransVal.c_str());
                            outputEncryptedPassword = TransVal;
                            DEBUGL5("outputEncryptedPassword.c_str() =%s\n", outputEncryptedPassword.c_str());
                            DecryptPassword(outputDecryptPassword, tmppasswd);
                            DEBUGL5("Decrypt password -- %s\n", tmppasswd.c_str());
                            /*reset permanent value.*/
                            PermVal = TransVal;
                        }else{
                            DEBUGL5("Update: transVal[%s] PermVal[%s] are same.\n",TransVal.c_str(),PermVal.c_str());
                        }

                        /*For Suporting 08-settings[3724 code]:  1->"Auto" 2->"Kerberos" 3->"NTLMv2" 
                        */			
                    /*FCH Issue FCH0001-MS009060 Scan to file generates error 
					when "Send NTLMv2 response only. Refuse LM & NTLM" Has Been Enabled in server*/   
					
		/*	enableVar = enableDomainRef->getTextContent();
                        DEBUGL5("enableDomainLogin ={ %s }\n",enableVar.c_str());
                        if ( enableVar == "1")
                        {
                            winDomain=winDomainRef->getTextContent();
                            DEBUGL5("winDomain now %s -----\n",winDomain.c_str());
                            if( winDomain == "Auto" || winDomain == "auto" ){
                                DEBUGL5("enableAuthLogin setting to 1[auto] \n");
                                authLoginRef->setTextContent("1");
                            }
                            else if (winDomain == "Kerberos" || winDomain == "kerberos" ) {
                                DEBUGL5("enableAuthLogin setting to 2[kerberos]\n");
                                authLoginRef->setTextContent("2");
                            }
                            else if (winDomain == "NTLMv2"){
                                DEBUGL5("enableAuthLogin setting  to 3[ntlmv2]\n");
                                authLoginRef->setTextContent("3");
                            }
                            else if (winDomain == "NTLMv1"){
                                DEBUGL5("enableAuthLogin setting  to 4[ntlmv1]\n");
                                authLoginRef->setTextContent("4");
                            }
                            DEBUGL5("enableAuthLogin is set......\n");
                        }
						else{
							DEBUGL5("enableAuthLogin setting  to 0[Workgroup]\n");
							winDomain=winDomainRef->getTextContent();
							authLoginRef->setTextContent("0");

							ElementRef clientref = chelper::NavigatePath("global/client~NTLMv2~auth", ini.GetRoot());
							if(!clientref)
							{
									DEBUGL2("client~NTLMv2~auth parameter not present\n");
							}
							else{
									if(winDomain == "NTLMv2")
									{
											DEBUGL5("enableAuthLogin setting  to 3[ntlmv2] in Workgroup mode\n");
											clientref->setTextContent("Yes");
									}
									else{
											DEBUGL5("Other than NTLMv2 enableAuthLogin setting in Workgroup mode\n");
											clientref->setTextContent("No");
										}
										ini.SetModified();
							}
						} */
                        /*      08-3724-subcode-1 clientAuthMethod
                                FCH "Scan to SMB" fails in MAC OS Yosemite since "client NTLMv2 auth=No"
                                1->Auto1 (kerberos/NTLMv1) 2->Kerberos  3->NTLMv2 4->NTLMv1 5->Auto2(Kerberos/NTLMv2) */
                        enableVar = enableDomainRef->getTextContent();
                        DEBUGL5("enableDomainLogin =[ %s ] \n",enableVar.c_str());
                        clientAuth=clientAuthRef->getTextContent();
                        if ( enableVar == "1")
                        {
                            DEBUGL5("clientAuthRef now %s -----\n",clientAuth.c_str());
                            if(clientAuth == "Auto1" || clientAuth == "auto1")
                            {
                                DEBUGL5("enableAuthLogin setting to 1[Auto1]\n");
                                authLoginRef->setTextContent("1");
                            }
                            else if ( clientAuth == "Kerberos" || clientAuth == "kerberos" ) {
                                DEBUGL5("enableAuthLogin setting to 2[kerberos]\n");
                                authLoginRef->setTextContent("2");
                            }
                            else if ( clientAuth == "NTLMv2"){
                                DEBUGL5("enableAuthLogin setting  to 3[ntlmv2]\n");
                                authLoginRef->setTextContent("3");
                            }
                            else if (clientAuth == "NTLMv1"){
                                DEBUGL5("enableAuthLogin setting  to 4[ntlmv1]\n");
                                authLoginRef->setTextContent("4");
                            }
                            else if(clientAuth == "Auto2" || clientAuth == "auto2" ){
                                DEBUGL5("enableAuthLogin setting to 5[Auto2] \n");
                                authLoginRef->setTextContent("5");
                            }
			    DEBUGL5("enableAuthLogin is set......\n");
			    ini.SetModified();
                        }
                        else{
                            DEBUGL5("enableAuthLogin setting  to 0[Workgroup]\n");
                            authLoginRef->setTextContent("0");

                             ElementRef clientref = chelper::NavigatePath("global/client~NTLMv2~auth", ini.GetRoot());
                            if(!clientref)
                            {
                                    DEBUGL2("client~NTLMv2~auth parameter not present\n");
                            }
                            else{
                                    if(clientAuth == "NTLMv2" || clientAuth == "Auto2" || clientAuth == "auto2" )
                                    {
                                            DEBUGL5("enableAuthLogin setting to 3 or 5 [ntlmv2/auto2] client~NTLMv2~auth=yes in Workgroup mode\n");
                                            clientref->setTextContent("Yes");
                                    }
                                    else{
                                            DEBUGL5("Other than NTLMv2 and Auto2 enableAuthLogin setting in Workgroup mode\n");
                                            clientref->setTextContent("No");
                                        }
                                        ini.SetModified();
                            }
                        }


                        if (encFlag == "1"){
                            encFlag = "0";
                        }
                        /*Device Auth Timeout*/
                        CString timeoutVar;
                        ElementRef timeoutref= chelper::NavigatePath(CString("deviceAuthTimeout"), configRoot, true);
                        timeoutVar = timeoutref->getTextContent();
                    }

                    ////////////////////////////////////////////
                    //Update config file from PermanentDOM data.
                    ////////////////////////////////////////////
                    ElementRef confRef;
                    if(i->second == ""){
                        DEBUGL5("Update: no conf item.[%s]skip\n",i->second.c_str());
                        continue;
                    }
                    //Serch item from ConfigFile.
                    confRef = chelper::NavigatePath(i->second, ini.GetRoot(), true);
                    if (!confRef)
                    {
                        DEBUGL5("Update: unable to serch xpath.[%s]\n",i->second.c_str());
                        continue;
                    }
	    
                    if(confRef->getTextContent() != PermVal ){
                        DEBUGL5("Update: values of [%s] are different between permVal [%s] and confVal[%s]\n", i->second.c_str(), PermVal.c_str(), confRef->getTextContent().c_str());

                        if(i->second == "global/netbios~name"){
                            //Update config file.
                            confRef->setTextContent(PermVal);
                            DEBUGL5("Update: New value = [%s] for [%s]\n", PermVal.c_str(), i->second.c_str());
                            ini.SetModified();
                            /* 20160614: Send notification to BOServer if deviceName(netbios-nsame) changes,
                             *  Since snmp gets notification from BOServer for deviceName xpath
                             */
                            DEBUGL5("update: EndTransaction with eWRITE was called for configDoc\n");
                            hdb->EndTransaction(configDoc,eWRITE);
                            CString xpath = SAMBA_NOTIFY_XPATH;
                            sendGenerateNotification(xpath.c_str());
                            DEBUGL5("Update: BeginTransaction with eWRITE is called for configDoc\n");
                            hdb->BeginTransaction(configDoc,eWRITE,true);
                        }
                        else if(i->second == "global/hostAnnouncementSSL"){
                            //Update config file.
                            confRef->setTextContent(PermVal);
                            DEBUGL5("Update: New value = [%s] for [%s]\n", PermVal.c_str(), i->second.c_str());
                            ini.SetModified();
                        }
			/*20170620: eBX WannaCry Support:
			 * server max protocol Support 08-8833 1-NT1, 2-SMB2, 3-SMB3(newly supported)
			 * client max protocol Support 08-8834 1-NT1, 2-SMB2, 3-SMB3
			 */
			else if(i->second == "global/client~max~protocol"){
                            //Update config file.
						#if 0
							if(PermVal=="1")
                                confRef->setTextContent("NT1");
                            else if(PermVal=="2")
                                confRef->setTextContent("SMB2");
							else
                                confRef->setTextContent("NT1");
                            DEBUGL5("Update: New value = [%s] for [%s]\n", PermVal.c_str(), i->second.c_str());
                            ini.SetModified();
						#endif
					        //Fix for samba starting twice from hibernation mode after 08-8833 support.
			        ElementRef clientMaxProtocolConf = chelper::NavigatePath("global/client~max~protocol", ini.GetRoot(),true);
                                if(!clientMaxProtocolConf)
                                        DEBUGL2("Update: client~max~protocol node not present\n");
                                else
                                {
                                        CString clientMaxProtoValue = clientMaxProtocolConf->getTextContent();
                                        if(PermVal == "1")
                                        {
                                                if(clientMaxProtoValue == "NT1")
                                                        DEBUGL5("Update: No Update required for client max~protocol,  NT1 is present\n");
                                                else
                                                {
                                                        confRef->setTextContent("NT1");
                                                        DEBUGL5("Update: New value = [%s] for [%s]\n", PermVal.c_str(), i->second.c_str());
                                                        ini.SetModified();
                                                }
                                        }
                                        else if(PermVal == "2")
                                        {
                                                if(clientMaxProtoValue == "SMB2")
                                                        DEBUGL5("Update: No Update required for client max~protocol, SMB2 is present\n");
                                                else
                                                {
                                                        confRef->setTextContent("SMB2");
                                                        DEBUGL5("Update: New value = [%s] for [%s]\n", PermVal.c_str(), i->second.c_str());
                                                        ini.SetModified();
                                                }
                                        }
                                        else if(PermVal == "3")
                                        {
                                                if(clientMaxProtoValue == "SMB3")
                                                        DEBUGL5("Update: No Update required for client max~protocol, SMB3 is present\n");
                                                else
                                                {
                                                        confRef->setTextContent("SMB3");
                                                        DEBUGL5("Update: New value = [%s] for [%s]\n", PermVal.c_str(), i->second.c_str());
                                                        ini.SetModified();
                                                }
                                        }
                                        else
                                        {
                                                confRef->setTextContent("SMB3");
                                                DEBUGL5("Update: New value = [%s] for [%s]\n", PermVal.c_str(), i->second.c_str());
                                                ini.SetModified();
                                        }
                                }
                                //fix for samba starting twice ends here
                        }
                        /*20170620: eBX WannaCry Support:
                         * server max protocol Support 08-8833 1-NT1, 2-SMB2, 3-SMB3(newly supported)
                         */
                        else if(i->second == "global/server~max~protocol")
                        {
                                ElementRef serverMaxProtocolConf = chelper::NavigatePath("global/server~max~protocol", ini.GetRoot(),true);
                                if(!serverMaxProtocolConf)
                                        DEBUGL2("Update: server~max~protocol node not present\n");
                                else
                                {
                                        CString serverMaxProtoValue = serverMaxProtocolConf->getTextContent();
                                        if(PermVal == "1")
                                        {
                                                if(serverMaxProtoValue == "NT1")
                                                        DEBUGL5("Update: No Update required for server max~protocol,  NT1 is present\n");
                                                else
                                                {
                                                        confRef->setTextContent("NT1");
                                                        DEBUGL5("Update: New value = [%s] for [%s]\n", PermVal.c_str(), i->second.c_str());
                                                        ini.SetModified();
                                                }
                                        }
                                        else if(PermVal == "2")
                                        {
                                                if(serverMaxProtoValue == "SMB2")
                                                        DEBUGL5("Update: No Update required for server max~protocol, SMB2 is present\n");
                                                else
                                                {
                                                        confRef->setTextContent("SMB2");
                                                        DEBUGL5("Update: New value = [%s] for [%s]\n", PermVal.c_str(), i->second.c_str());
                                                        ini.SetModified();
                                                }
                                        }
                                        else if(PermVal == "3")
                                        {
                                                if(serverMaxProtoValue == "SMB3")
                                                        DEBUGL5("Update: No Update required for server max~protocol, SMB3 is present\n");
                                                else
                                                {
                                                        confRef->setTextContent("SMB3");
                                                        DEBUGL5("Update: New value = [%s] for [%s]\n", PermVal.c_str(), i->second.c_str());
                                                        ini.SetModified();
                                                }
                                        }
                                        else
                                        {
                                                confRef->setTextContent("SMB3");
                                                DEBUGL5("Update: New value = [%s] for [%s]\n", PermVal.c_str(), i->second.c_str());
                                                ini.SetModified();
                                        }
                                }
                        }
                        else if(i->second == "global/sambaCliConnectionTimeout"){
                            //Update config file.
                            confRef->setTextContent(PermVal);
                            DEBUGL5("Update: New value = [%s] for [%s]\n", PermVal.c_str(), i->second.c_str());
                            ini.SetModified();
                        }
                        else if (i->second == "FILE_SHARE/max~connections"){
                            confRef->setTextContent(PermVal);
                            ElementRef smbref = chelper::NavigatePath("Print/max~connections", ini.GetRoot());
                            if(smbref){
                                smbref->setTextContent(PermVal);
                            }
                            smbref = chelper::NavigatePath("DRIVER_INST/max~connections", ini.GetRoot());
                            if(smbref){
                                smbref->setTextContent(PermVal);
                            }
                            DEBUGL5("Update: File share Max connections are set from DOM..\n");
                            ini.SetModified();
                        }
                        /*Device Auth Timeout*/
                        else if (i->second == "global/DeviceAuthTimeout"){
                            confRef->setTextContent(PermVal);
                            DEBUGL5("Update: Connecttion Timeout defined by the user..\n");
                            ini.SetModified();
                        }
                        else if (i->second == "global/name_resolve_order"){
                            confRef->setTextContent("lmhosts host wins bcast purev6");
                            ini.SetModified();
                        }

                        else if(i->second == "global/workgroup"){
                            ElementRef t = chelper::NavigatePath("global/security", ini.GetRoot());
                            if (!t)	// 20100720 Manoj  Fix for DTFR 5311
                            {
                                DEBUGL5("Update: no global/security  present in config file \n");
                                continue ;
                            }
                            ElementRef realmRef = chelper::NavigatePath("global/realm", ini.GetRoot());
                            if (!realmRef) // 20100720 Manoj  Fix for DTFR 5311
                            {
                                DEBUGL5("Update: no global/realm  present in config file \n");
                                continue ;
                            }
                            DEBUGL5("Update: i->first[%s] domain[%s] group[%s]\n",i->first.c_str(),domain.c_str(),group.c_str());
                            if(i->first == "domainName" && domain == "")
                            {
                                //t->setTextContent("share");
                                t->setTextContent("user");
                                //confRef->setTextContent("Workgroup"); #stfr 7456 fix
                                /*20120124 Removed removel of Domain Name from Conf file */
                                /*WARINIG : THIS TESTING IS MUST*/	
                                /*realmRef->setTextContent("");*/
                            }
                            else if(i->first == "domainName"){
                                //	domain_upper=ChangeStringCase(domain.c_str(),false);
                                //	Fulldomain_upper=ChangeStringCase(Fulldomain.c_str(),false);

                                std::transform(domain.begin(),domain.end(),domain.begin(),(int(*)(int))::toupper);
                                std::transform(Fulldomain.begin(),Fulldomain.end(),Fulldomain.begin(),(int(*)(int))::toupper);
                                confRef->setTextContent(domain.c_str());
                                ini.SetModified();
                                realmRef->setTextContent(Fulldomain.c_str());
                                t->setTextContent("domain");
                            }

                            if(i->first == "groupName" && group == "")
                            {                           
                                //t->setTextContent("domain");
                                if(PermVal != "")
                                {
                                    confRef->setTextContent(PermVal);
                                    ini.SetModified();
                                }
                            }
                            else if(i->first == "groupName"){
                                confRef->setTextContent(group.c_str());
                                ini.SetModified();
                                //t->setTextContent("share");
                                t->setTextContent("user");
                                realmRef->setTextContent("");
                            }

                            if(i->first == "adsName" && activeDirectory == "")
                            {

                                //t->setTextContent("ads");
                                //confRef->setTextContent("ads");
                            }
                            else if(i->first == "adsName"){

                                confRef->setTextContent(activeDirectory.c_str());
                                ini.SetModified();
                                realmRef->setTextContent(Full_AD.c_str());
                                t->setTextContent("ads"); 
                            }
                            //ini.SetModified();
                        }
#if 0
                        else if(i->second == "global/wins~server"){
                            //Update config file.
                            confRef->setTextContent(PermVal);
                            DEBUGL5("Update: New value = [%s] for [%s]\n", PermVal.c_str(), i->second.c_str());
                            ini.SetModified();
                        }
                        else if(i->second == "global/wins~server2"){
                            //Update config file.
                            confRef->setTextContent(PermVal);
                            DEBUGL5("Update: New value = [%s] for [%s]\n", PermVal.c_str(), i->second.c_str());
                            ini.SetModified();
                        }
#endif
                        else if(i->second == "Print/available"){
                            //Update config file.
                            /* webinstaller PnP: depending on print share driver shares are to be made available ot disabled.*/
                            struct dirent **drivernamelist;
                            int files = 0;


							
                            CString driv_str = readversionfile(1);
                            if((driv_str != "STATUS_FAILED") && (driv_str != "")){
                                ElementRef uni = chelper::NavigatePath((driv_str + CString ("_driver/available")),ini.GetRoot(), true);
                                if(uni){
                                    files=scandir("/work/drivers/pointNprint/UNI", &drivernamelist, 0, alphasort);
                                if(files > 0){
                                    if(files < MAX_DRIVER_FILES){
                                            uni->setTextContent("0");
                                    }
                                    else{
                                            uni->setTextContent(PermVal);
                                    }
                                    while(files--){
                                        if(drivernamelist[files])
                                            free(drivernamelist[files]);
                                    }
                                    if(drivernamelist)
                                        free(drivernamelist);
                                }
                            }
                            }
                            driv_str = readversionfile(2);
                            if((driv_str != "STATUS_FAILED") && (driv_str != "")){
                                ElementRef ps3 = chelper::NavigatePath((driv_str + CString ("_driver/available")),ini.GetRoot(), true);
                                if(ps3){
                                    files=scandir("/work/drivers/pointNprint/PS", &drivernamelist, 0, alphasort);
                                if(files > 0){
                                    if(files < MAX_DRIVER_FILES){
                                            ps3->setTextContent("0");
                                    }
                                    else{
                                            ps3->setTextContent(PermVal);
                                    }
                                    while(files--){
                                        if(drivernamelist[files])
                                            free(drivernamelist[files]);
                                    }
                                    if(drivernamelist)
                                        free(drivernamelist);
                                }
                            }
                            }
                            driv_str = readversionfile(3);
                            if((driv_str != "STATUS_FAILED") && (driv_str != "")){
                                ElementRef rxps = chelper::NavigatePath((driv_str + CString("_driver/available")),ini.GetRoot(), true);
                            if(rxps){
                                files=scandir("/work/drivers/pointNprint/XPS", &drivernamelist, 0, alphasort);
                                if(files > 0){
                                    if(files < MAX_DRIVER_FILES){
                                        rxps->setTextContent("0");
                                    }
                                    else{
                                        rxps->setTextContent(PermVal);
                                    }
                                    while(files--){
                                        if(drivernamelist[files])
                                            free(drivernamelist[files]);
                                    }
                                    if(drivernamelist)
                                        free(drivernamelist);
                                }
                            }
                            }
                            /* webinstaller PnP end*/
                            confRef->setTextContent(PermVal);
                            DEBUGL5("Update: New value = [%s] for [%s]\n", PermVal.c_str(), i->second.c_str());
                            ini.SetModified();
                        }
                        else if(i->second == "FILE_SHARE/available"){
                            //Update config file.
                            confRef->setTextContent(PermVal);
                            DEBUGL5("Update: New value = [%s] for [%s]\n", PermVal.c_str(), i->second.c_str());
                            ini.SetModified();
                        }
                        else if(i->second == "global/server~signing"){
                            //Update config file.
                            confRef->setTextContent(PermVal);
                            DEBUGL5("Update: New value = [%s] for [%s]\n", PermVal.c_str(), i->second.c_str());
                            ini.SetModified();
                        }
                        else if(i->second == "global/client~signing"){
                            //Update config file.
                            confRef->setTextContent(PermVal);
                            DEBUGL5("Update: New value = [%s] for [%s]\n", PermVal.c_str(), i->second.c_str());
                            ini.SetModified();
                        }
                    }
                }

		/* 20160615: We have supported 08-3724-1 from L4.8x or L4.732x. we need to consider below scenarios during ROM update.
		 * 	a)If user update ROM from L4.8x or L4.732x to L4.9 or L4.733, 
		 *	  then after ROM update we need to update value of 08-3724-1(clientAuth).
		 *	b)If user update ROM from L4.7311 or L4.73 (from any version which are not supported 08-3724-1),
		 *	  then after ROM update we need to update value of 08-3724-0(windomainAuth).
		 *     	
		 *      During ROM update if 08-3724-1 is not supported	in previous ROM, 
		 * 	We will set clientAuthFlag=NotSupported in smb.conf using configure_samba script present in $EB2/build/commmon/bin/
		 * 	Please see configure_samba script for details($EB2/build/commmon/bin/).
		 */

	            // when user update ROM from L4.73x to L4.8 3724-1 should same as 3724-0 value
                ElementRef clientAuthFlagref = chelper::NavigatePath("global/clientAuthFlag", ini.GetRoot());
                ElementRef clientMethodRef= chelper::NavigatePath(CString("clientAuthMethod"), configRoot, true);
                if(!clientAuthFlagref)
                {
                    DEBUGL5("Update(): global/clientAuthFlag parameter not present\n");
                }
                else
                {
                    DEBUGL5("Update(): global/clientAuthFlag parameter is present\n");
                    CString clientFlag = clientAuthFlagref->getTextContent();
		    ElementRef clientref = chelper::NavigatePath("global/client~NTLMv2~auth", ini.GetRoot());
	   	    if(!clientref)
		    {
	 	    	DEBUGL2("Update(): client~NTLMv2~auth parameter not present\n");
		    }
		    else
		    {
			    /*configure_samba script will set to "NotSupported" if 3724-1 was not supported in previous ROM*/
			    if(clientFlag == "NotSupported")
			    {
				    ElementRef winDomainRef= chelper::NavigatePath(CString("winDomainAuthMethod"), configRoot, true);
				    CString winDomain = winDomainRef->getTextContent();
				    //Safer-side, set it if any of below values present only.
				    //if( winDomain == "auto") || (winDomain == "Kerberos") || (winDomain == "NTLMv2") || (winDomain == "NTLMv1"))
				    DEBUGL5("Update():  Set value of clientAuthMethod as winDomainAuthMethod\n");
				    if((winDomain == "Auto") || (winDomain == "auto"))
					    clientMethodRef->setTextContent("auto1");
				    else if((winDomain == "NTLMv2") || (winDomain == "ntlmv2"))
				    {
					    DEBUGL5("Update(): After ROM update client~NTLMv2~auth=yes is set in samba conf,previous value was NTLMv2\n");
					    clientMethodRef->setTextContent(winDomain);
					    clientref->setTextContent("Yes");
				    }
				    else
					    clientMethodRef->setTextContent(winDomain);

				    clientAuthFlagref->setTextContent("Yes");
				    ini.SetModified();
			    }
			    else if(clientFlag == "No")
			    {
				    clientAuth=clientAuthRef->getTextContent();
				    if((clientAuth == "NTLMv2") || (clientAuth == "Auto2") || (clientAuth == "auto2" ))
				    {
					    DEBUGL5("Update(): After ROM update client~NTLMv2~auth=yes is set in samba conf\n");
					    clientref->setTextContent("Yes");
				    }

				    clientAuthFlagref->setTextContent("Yes");
				    ini.SetModified();
			    }
			    else
				    DEBUGL5("Update(): clientAuthFlag=Yes is present.Hence dont change client ntlm auth value\n");
		    }
		}
                //ROM Update issue ends here

	/*Disable Webinstaller folders for SCC starts here 20130827*/
				
                CString csBrandType = "";
                CString csProductType = "";
                CString csModelType = "";               //Making modeltype=NULL since all Product type doesn't have a ModelType.
                if(getenv("EQUIPMENT_BRAND"))
                {
                    csBrandType = CString(getenv("EQUIPMENT_BRAND"));
                }
                if(getenv("PRODUCT"))
                {
                    csProductType = CString(getenv("PRODUCT"));
                }
                if(getenv("SUB_PRODUCT"))
                {
                    csModelType = CString(getenv("SUB_PRODUCT"));
                }
				uint16 brand = 0;
				if(csProductType == "S2_PRODUCT") //brand check for s2_product(common code)
				{					
					IndexedDB_ReadContent(8, 9003, 0, brand, false);
					DEBUGL5("Update(): Value of 9003 is %d\n",brand);
				}
				
				 //disable webinstaller for SCC
					CString univ_driv = "univ";
					CString xps_driv = "xps";
					CString ps3_driv = "ps3";
					ElementRef univ = chelper::NavigatePath((univ_driv + CString("_driver/available")),ini.GetRoot(), true);
					ElementRef xps = chelper::NavigatePath((xps_driv + CString("_driver/available")),ini.GetRoot(), true);
					ElementRef ps3 = chelper::NavigatePath((ps3_driv + CString("_driver/available")),ini.GetRoot(), true);
					if(univ || xps || ps3)
					{
						if(csBrandType == "TOSHIBASS")
						{           
							if(("0" != univ->getTextContent()) || ("0" != xps->getTextContent()) || ("0" != ps3->getTextContent()))       	
							{	
								DEBUGL5("[%s]: UNIV/XPS/PS3 Disabling webinstaller for TOSHIBASS \n" ,__FUNCTION__);
								webInstallerDisable(ini);                        
							}
						}
						else if((csBrandType == "OKI") || (OKI == brand))
						{
							if((csProductType == "WEISS") || (csProductType == "S2_PRODUCT"))
							{
								if(("0" != univ->getTextContent()) || ("0" != xps->getTextContent()) || ("0" != ps3->getTextContent()))       	
								{	
									DEBUGL5("[%s]: UNIV/XPS/PS3 Disabling webinstaller for OKI-WEISS/S2_PRODUCT \n" ,__FUNCTION__);
									webInstallerDisable(ini);                        
								}
							}
						}		

						else if(csBrandType == "TOSHIBA")
						{
							if(csProductType == "WEISS")
							{
								if((csModelType == "WEISS_L_SSD") || (csModelType == "WEISS_LL"))
								{
									if(("0" != univ->getTextContent()) || ("0" != xps->getTextContent()) || ("0" != ps3->getTextContent()))       	
									{	
										DEBUGL5("[%s]: UNIV/XPS/PS3 Disabling webinstaller for WEISS_LL/WEISS_L_SSD \n" ,__FUNCTION__);
										webInstallerDisable(ini);                        
									}
								}
							}
						}
					}
					else
						DEBUGL5("[%s]:  UNIV/XPS/PS3 nodes not present \n" ,__FUNCTION__);
					
	/*Disable Webinstaller folders for SCC ends here 20130827*/
		
				/* DHEERAJ WEISS_LL_SSD PNP issue */
             /*   if(getenv("PRODUCT"))
				{
			        CString csProductType=CString(getenv("PRODUCT"));
			        if(csProductType=="WEISS")
			        {
					    if (getenv("SUB_PRODUCT"))
						{
							CString csModelType = CString(getenv("SUB_PRODUCT"));
						    if((csModelType =="WEISS_L_SSD") || (csModelType =="WEISS_LL"))
							{
								CString driv_str = "univ";
								ElementRef uni = chelper::NavigatePath((driv_str + CString ("_driver/available")),ini.GetRoot(),true);
								if(uni)
                                    uni->setTextContent("0");
								driv_str = "xps";
								ElementRef xps = chelper::NavigatePath((driv_str + CString ("_driver/available")),ini.GetRoot(),true);
								if(xps)
                                    xps->setTextContent("0");
								driv_str = "ps3";
								ElementRef ps3 = chelper::NavigatePath((driv_str + CString ("_driver/available")),ini.GetRoot(),true);
								if(ps3)
                                    ps3->setTextContent("0");
								ini.SetModified();	
						    }
			            }
			        }
			    }    */
				ElementRef print = chelper::NavigatePath("Print/browseable",ini.GetRoot(),true);
				if(print)
					print->setTextContent("no");
				/* DHEERAJ WEISS_LL_SSD PNP issue  End*/	
		
		/*  PRODUCT DEPENDENT CODE Start 20130911*/
        /*08-9599(Samba Server) code changes */
            if(csProductType == "WEISS")
            {
            	if((csModelType == "WEISS_L_SSD") || (csModelType == "WEISS_LL"))
                {
                    ElementRef fileshare_dis = chelper::NavigatePath(CString("fileshareEnabled"), configRoot, true);
                    if(fileshare_dis)
                        fileshare_dis->setTextContent("0");
                }
            }
#if 0        /* 08-8825(hostAnnouncement in Super sleep mode) code changes*/
            if(csProductType=="LOIRE")
            {
                if(csModelType =="ECO_LOIRE")
                {
                    ElementRef hostAnnounce = chelper::NavigatePath(CString("hostAnnouncementSSL"), configRoot, true);
                    if(hostAnnounce)
                        hostAnnounce->setTextContent("0");
                }
            }
 #endif     /* PRODUCT DEPENDENT CODE End*/

                /*******Syncing config file and Dom in case of fall back *****/
                ElementRef cSecurity = chelper::NavigatePath("global/security", ini.GetRoot());  
                if(cSecurity) {
                    if("1" == enableDomainRef->getTextContent()  && "user" == cSecurity->getTextContent() ) {
                        ElementRef domDomainRef = chelper::NavigatePath( CString("domainName"), configRoot, true);
                        if (domDomainRef)
                        {
                            CString domDomain=domDomainRef->getTextContent();
                            if(!(domDomain.empty()))	
                            {
                                CString domFullDomain=domDomain;
                                ElementRef cWorkgroupRef = chelper::NavigatePath("global/workgroup", ini.GetRoot());
                                ElementRef cRealmRef = chelper::NavigatePath("global/realm" , ini.GetRoot());

                                if(cWorkgroupRef && cRealmRef){
                                    string whitespaces ("."); 
                                    size_t found;
                                    if(domDomain!=""){
                                        found = domDomain.find_first_of(whitespaces);
                                        if (found!=string::npos)
                                            domDomain.erase(found);
                                        std::transform(domDomain.begin(),domDomain.end(),domDomain.begin(),(int(*)(int))::toupper);
                                        std::transform(domFullDomain.begin(),domFullDomain.end(),domFullDomain.begin(),(int(*)(int))::toupper);
                                        CString cstmpGroupName;
                                        cstmpGroupName = cWorkgroupRef->getTextContent();
                                        if (domDomain != cstmpGroupName){
                                            DEBUGL5("Writing conf WG old[%s] New[%s]\n",cstmpGroupName.c_str(),domDomain.c_str());
                                            cWorkgroupRef->setTextContent(domDomain);
                                            ini.SetModified();
                                        }/*20120125 End of Domain (removed ".")Check*/
                                        cstmpGroupName = cRealmRef->getTextContent();
                                        if (domFullDomain != cstmpGroupName){
                                            DEBUGL5("Writing conf RE old[%s] New[%s]\n",cstmpGroupName.c_str(),domFullDomain.c_str());
                                            cRealmRef->setTextContent(domFullDomain);
                                            ini.SetModified();
                                        }/*20120125 End of fulldomain namecheck*/
                                    }
                                }
                            }//end of if(!(domDomain.empty()))	
                        }//end of if (domDomainRef)
                    }
                }


                CString tmpPDC,tmpBDC;
                CString tmpPasswdServer;
                //Get PDC value in PermanentDOM.
                ElementRef permRef = chelper::NavigatePath(xpathForPDC.c_str(), configRoot, true);
                if(!permRef){
                    DEBUGL1("Update: not present[%s] in Permanent Dom.It might be bug.\n", xpathForPDC.c_str());
                }
                else
                    tmpPDC = permRef->getTextContent();
                DEBUGL5("Update: Perm PDC data[%s] path[%s].\n",tmpPDC.c_str(),xpathForPDC.c_str());
                //Get BDC value in PermanentDOM.
                permRef = chelper::NavigatePath(xpathForBDC.c_str(), configRoot, true);
                if(!permRef){
                    DEBUGL1("Update: not present[%s] in Permanent Dom.It might be bug.\n", xpathForBDC.c_str());
                }
                else
                    tmpBDC = permRef->getTextContent();
                DEBUGL5("Update: Perm BDC data[%s] path[%s].\n",tmpBDC.c_str(),xpathForBDC.c_str());

                if(tmpPDC != ""){
                    if(tmpBDC != ""){
                        tmpPasswdServer = tmpPDC + ", " + tmpBDC;
                    }
                    else{
                        tmpPasswdServer = tmpPDC;
                    }
                }
                else{
                    if(tmpBDC != ""){
                        tmpPasswdServer = tmpBDC;
                    }
                    else{
                        tmpPasswdServer = "";
                    }
                }

                ElementRef confRef;
                confRef = chelper::NavigatePath("global/wins~server", ini.GetRoot(), true);
                if (confRef)
                {
                    CString csTempAddress = confRef->getTextContent();
                    CString csWins1, csWins2;
                    CString CSWinsJoinedAddress = getWinsAddress(configRoot, csWins1, csWins2);
                    DEBUGL5("[%s]Wins conf server Address - [%s] w1[%s] w2[%s]\n",__FUNCTION__,csTempAddress.c_str(), csWins1.c_str(),csWins2.c_str());
                    size_t sw1Found = csTempAddress.find(csWins1);
                    size_t sw2Found = csTempAddress.find(csWins2);
                    DEBUGL5("[%s] w1 Find Status = [%d] , w2 Find Status = [%d]\n", __FUNCTION__, sw1Found, sw2Found);
                    if ((sw1Found == string::npos) || (sw2Found == string::npos)){
                        DEBUGL5("[%s] Wins address changed updatings [%s] smb.conf\n",__FUNCTION__,CSWinsJoinedAddress.c_str());
                        confRef->setTextContent(CSWinsJoinedAddress);
                        ini.SetModified();
                    }
                    else
                        DEBUGL5("[%s]No change in WINS SERVER Address\n",__FUNCTION__);
                    confRef = NULL;	
                }
                DEBUGL5("Update: Make password server from current PermDOM data[%s]\n",tmpPasswdServer.c_str());
                confRef = chelper::NavigatePath(tmpConfPath, ini.GetRoot(), true);
                if (!confRef){
                    DEBUGL1("Update: unable to search xpath.[%s]\n",tmpConfPath.c_str());
                }
                if(confRef){
                    CString csConfPasswdServer = confRef->getTextContent();
                    DEBUGL5("Update: Password server data is different with Permanent data and conf data.conf[%s]\n",(csConfPasswdServer).c_str());
                    DEBUGL5("[%s]PASSWD Server conf Address - [%s] PD1[%s] PD2[%s]\n",__FUNCTION__,csConfPasswdServer.c_str(), tmpPDC.c_str(),tmpBDC.c_str());
                    size_t spdcFound = csConfPasswdServer.find(tmpPDC);
                    size_t sbdcFound = csConfPasswdServer.find(tmpBDC);
                    DEBUGL5("[%s] pdc Find Status = [%d] , bdc Find Status = [%d]\n", __FUNCTION__, spdcFound, sbdcFound);
                    if ((spdcFound == string::npos) || (sbdcFound == string::npos)){
                        confRef->setTextContent(tmpPasswdServer);
                        ini.SetModified();
                    }
                    else
                        DEBUGL5("[%s]No change in PASSWORD Server\n",__FUNCTION__);
                    confRef = NULL;
                }
                /*Moved here for EBX_DTFR_18229: smb.conf settings are not saved properly. if this part is after check = ini.IsModified(); */
                /* In Windows 10 Enterprise/Education Edition version 1709 (RS3) or later, for Windows Server version 1709 or later,
                 * specification was changed not to accept Guest Logon for SMB server.
                 * Hence,08-8852 is supported to resolve this.
                 * 02_Requirement/SRS/JEA-03880_SMBServerAuthentication_Specifications_Eng.doc
                 */
                CString csGuestLogonUser = "", csGuestUserPasswd = "",csDomainenabled ="";
                serverAuth=serverAuthRef->getTextContent();
                csDomainenabled= enableDomainRef->getTextContent();
                CString csServerAuthFlag = "GuestEnabled"; //guest Logon enabled
                if( "0" == csDomainenabled &&  "0" == serverAuth)
                {
                    DEBUGL5("Update:Domain Login is Disabled and Guest Logon is disabled \n");
                    csServerAuthFlag = "WGGuestDisabled";
                    csGuestLogonUser= serverAuthUserRef->getTextContent();
                    csGuestUserPasswd= serverAuthPasswdRef->getTextContent();
                    CString csTempGuestUserPasswd ="";
                    DecryptPassword(csGuestUserPasswd, csTempGuestUserPasswd);
                    csGuestUserPasswd = csTempGuestUserPasswd;
                    DEBUGL5("Update:Server Authentication Logon UserName is %s\n ",csGuestLogonUser.c_str());
                }
                else if ("1" == csDomainenabled)
                {
                    /* 20180217: When AD/Domain is enabled we add logon locally(Since we send guest logon flag to true even in Ad/Domain).
                     * Because: if Logon user is not added in smbpasswd, user cannot access fileshare/print
                     * from Windows10-1709 or later versions.
                     * We distinguish domain user(uid-9998) and GuestLogon Disabled option user(uid-9999) using uid.
                     */
                    //Delete if any Domain guest user already exists from passdb and /etc/passwd file

                    DEBUGL5("Update:adding temporary user in db\n");
                    CString csDeleteRes = DBUsersDeletion();//delete user from passwd and passdb.tdb files
                    if(csDeleteRes != "STATUS_OK")
                    {
                        DEBUGL2("Update: serverAuthUser: User deletion failed. Hence we need to return error to TA or Self-Diagnostic\n");
                        return "STATUS_FAILED";
                    }
                    else
                        DEBUGL5("Update: serverAuthUser: successfully deleted users\n");

                    CString csDomainLogonUser = LogUser->getTextContent();
                    CString csDomainLogonUserPasswd = pPasswordReset->getTextContent();
                    CString csTempGuestUserPasswd ="";
                    DecryptPassword(csDomainLogonUserPasswd, csTempGuestUserPasswd);
                    //Add AD/Domain user to /etc/password file and smbpasswd(passdb) file
                    CString csAddUserCmd = "";
                    csAddUserCmd = "/usr/sbin/useradd -u 9998 -M ";
                    csDomainLogonUser = PrepareQuoteString(csDomainLogonUser);
                    DEBUGL5("Update():SMBserverAuthentication logon user adding to db is:[%s]\n",csDomainLogonUser.c_str());
                    csAddUserCmd += csDomainLogonUser;
                    int iRet = system(csAddUserCmd.c_str());
                    int iErr = WEXITSTATUS(iRet);
                    if(iErr != 0)
                        DEBUGL1("Update:SMBserverAuthentication: return value of adding domain user to /etc/passwd file [%s] is [%d]\n",csAddUserCmd.c_str(),iErr);
                    csAddUserCmd.clear();
                    CString csDomainGuestUserPasswd = PrepareQuoteString(csTempGuestUserPasswd);
                    csAddUserCmd="echo -e \"" + csDomainGuestUserPasswd;
                    csAddUserCmd+="\\n" + csDomainGuestUserPasswd;
                    csAddUserCmd+="\" | ";
                    csAddUserCmd+=getenv("EB2");
                    csAddUserCmd+= "/bin/smbpasswd -a " + csDomainLogonUser;
                    DEBUGL5("Update:SMBserverAuthentication: Executing system command [%s] and the return value is [%d]\n",csAddUserCmd.c_str(),iErr);
                    iRet = system(csAddUserCmd.c_str());
                    iErr = WEXITSTATUS(iRet);
                    if(iErr != 0)
                        DEBUGL1("Update:SMBserverAuthentication:return value of adding domain user to smbpasswd(passdb) file [%s] is [%d]\n",csAddUserCmd.c_str(),iErr);
                    csAddUserCmd.clear();
                    //set "guest only" to yes to delete files since we are adding user to local db,
                    //eventhough MFP is in AD/Domain, Else we cannot delete file/folder from file_share due to permission issue
                    ElementRef guestref = chelper::NavigatePath("global/guest~only", ini.GetRoot(),true);
                    if(!guestref)
                        DEBUGL2("guest~only parameter not present\n");
                    else
                    {
                        DEBUGL5("Update:setting guest only flag to yes since AD/Domain enabled and the user is added locally\n");
                        guestref->setTextContent("yes");
                        ini.SetModified();
                    }
                }
                else
                {
                    DEBUGL5("Update:Guest Logon is Enabled  and  MFP is Workgroup\n");
                    ElementRef guestref = chelper::NavigatePath("global/guest~only", ini.GetRoot(),true);
                    if(!guestref)
                        DEBUGL2("guest~only parameter not present\n");
                    else
                    {
                        DEBUGL5("Update:setting guest only flag to no since AD/Domain enabled and Guest Logon Disabled\n");
                        guestref->setTextContent("no");
                        ini.SetModified();
                    }
		}
                //Rangan  changes done to have a check on wrong username and password , when controller in domain join
                check = ini.IsModified();

                // save (and reload) the ini file if it was modified.
                reload = bRestartRequired;
                if (check ||reload){
                    if(ini.Save() != STATUS_OK) {
                        DEBUGL1("Update: config file update error!!!\n");
                        error = "Unable to save config file " + m_confFile;
                    }
                    else{
                        DEBUGL5("Update: config file update done.reloading.\n");
                        ini.Load();
                    }
                }

                ElementRef oe = chelper::NavigatePath(m_enabledXPath, configRoot, true);
                ElementRef ne = transRoot ? chelper::NavigatePath(m_enabledXPath, transRoot) 
                    : NodeRef();
                if (!oe) {
                    error = "Unable to get 'enable' setting under config db path " +
                        configRoot->ComputeFullPath();
                }
                else
                {
                    if (ne)
                        oe->setTextContent(ne->getTextContent());
                    if (oe->getTextContent().empty())
                        oe->setTextContent("0");
                }

                bool enabled = oe && oe->getTextContent() == "1";

                DEBUGL5("Update() : enabled : %s\n", enabled ? "yes" : "no");
                DEBUGL5("Update() : m_statusCmd : [%s] m_startCmd [%s] m_reloadCmd [%s]\n", m_statusCmd.c_str(),m_startCmd.c_str(),m_reloadCmd.c_str());
                CString statstr = getProcStatus(m_statusCmd);
                depResult = statstr;
                DEBUGL5("update: EndTransaction with eWRITE was called for configDoc\n");
                hdb->EndTransaction(configDoc,eWRITE);
                if (error.empty() && (startMode == "00")  && (sCallingModeNMO != "ApplyClone"))
                {
                    bool restat = false;
                    size_t found1=0;
                    size_t found2=0;
                    size_t found3=0;
                    size_t found4=0;
                    size_t found=0;
                    char TempCmd[BUFFER_LIMIT];
                    CString TmpUserStr;
                    CString TmpPswdStr;
                    CString search_Str("\\");
                    CString ValueStr ="";
                    CString ValueStr1 ="";
                    hdb->BeginTransaction(configDoc,eWRITE,true);
                    DEBUGL5("Update: BeginTransaction with eWRITE is called for configDoc\n");	
                    ElementRef configRoot = hdb->BindToElement(configDoc,m_serviceXpath);

                    memset(TempCmd,'\0',BUFFER_LIMIT);
                    ElementRef domDomainRef = chelper::NavigatePath( CString("domainName"), configRoot, true);
                    /*20101006 Manoj VCON #105 in request is not from DEVICE _CONFIG_PLUGION enable/disable MFP function*/
                    if (!domDomainRef && !LogUser) {
                        DEBUGL1("Unable to fetch from Permanent DOM...BUG Present\n");
                    }
                    if(domDomainRef && LogUser){
                        ValueStr = LogUser->getTextContent();
                        ValueStr1= domDomainRef->getTextContent();
                    }
                    enableMFPFunction(configRoot);	
                    DEBUGL5("update: EndTransaction with eWRITE was called for configDoc\n");
                    hdb->EndTransaction(configDoc,eWRITE);

                    if( depResult =="running" && !enabled)
                    {
                        // Service restart required hence stop the service first
                        CString stopCmd = PrepareCommandString(eStopCommand,configRoot);
                        DEBUGL5("Update() : Stopping service[%s]\n",stopCmd.c_str());
                        system(stopCmd.c_str());
                        restat = true;
                    }
                    else if (depResult != "running" && enabled) // not active
                    {
                        CString startCmd = PrepareCommandString(eStartCommand,configRoot);
                        DEBUGL5("Update : Starting service[%s]\n",startCmd.c_str());
                        /*When user enters the username of form "USER_NAME@DOMAIN_NAME */
                        startCmd.copy(TempCmd,startCmd.length(),INDEX_ZERO);
                        CString StrTempCmd(TempCmd);
                        if(domDomainRef && LogUser){
                            CString search_char("@");

                            /*In TA->SMB Session : Checking weather Domain name and Username are with proper domain*/
                            DEBUGL5("DOM values -> UserName :%s and domain name: %s\n ",ValueStr.c_str(),ValueStr1.c_str());
                            found1 = ValueStr.find_first_of(search_char,INDEX_ZERO);
                            if (found1!=string::npos)
                            {
                                ValueStr.erase(INDEX_ZERO,found1+INDEX_ONE);
                                /*In TA->SMB Session:Checking DOM,weather Domainname and Username(uname@Domainname)are matching */
                                if(ValueStr.compare(INDEX_ZERO,ValueStr.length(),ValueStr1,INDEX_ZERO,ValueStr1.length())==0)
                                {
                                    found3=startCmd.find_first_of(search_char);
                                    if (found3 != string::npos)
                                    {

                                        found2 = startCmd.find('-', found3 +INDEX_ONE);
                                        startCmd = startCmd.substr(INDEX_ZERO,found3) + startCmd.substr(found2-INDEX_ONE);
                                    }
                                    DEBUGL5("DOMdomain and DOMuser matching...\n So,COMMANDSTRING is :%s\n",startCmd.c_str());
                                    found4 = startCmd.find_first_of("u",INDEX_ZERO);
                                    found3 = startCmd.find_first_of(" ",found4);
                                    TmpUserStr = startCmd.substr((found4 + INDEX_ONE),(found3 - found4 - INDEX_ONE));
                                    while((found = TmpUserStr.find_first_of(search_Str,INDEX_ZERO)) != string::npos)
                                    {
                                        TmpUserStr.erase(INDEX_ZERO,(found+INDEX_ONE));
                                    }				
                                    TmpUserStr = PrepareQuoteString(TmpUserStr);
                                    TmpPswdStr = startCmd.substr(found3 + INDEX_THREE);
                                    TmpPswdStr = TmpPswdStr.substr(INDEX_ZERO,(TmpPswdStr.length() - INDEX_FIVE));
                                    TmpPswdStr = PrepareQuoteString(TmpPswdStr);

                                    startCmd = startCmd.erase((found4 + INDEX_ONE),startCmd.length());
                                    startCmd = startCmd  + TmpUserStr + " -p" + TmpPswdStr + " -v" + " &";
                                    DEBUGL5("The replaced string : %s \n",startCmd.c_str());				

                                    system(startCmd.c_str());
                                    restat = true;
                                }
                                else  /*Putting back to WG*/
                                {
                                    DEBUGL5("DOM values-> UserName:%s & domain name: %s\n ",ValueStr.c_str(),ValueStr1.c_str());
                                    found4 =StrTempCmd.find_first_of("u",INDEX_ZERO);
                                    if (found4!=string::npos)  {
                                        /*Making the username BLANK in the Command String*/
                                        StrTempCmd.replace(found4+INDEX_ONE,((StrTempCmd.find("-",found4))- found4) -INDEX_ONE," ");
                                    }
                                    else {
                                        //do nothing
                                    }
                                    DEBUGL5("DOMdomain and DOMuser not matching...\n So,COMMANDSTRING():%s\n",StrTempCmd.c_str());
                                    found4 = StrTempCmd.find_first_of("u",INDEX_ZERO);
                                    found3 = StrTempCmd.find_first_of(" ",found4);
                                    TmpUserStr = StrTempCmd.substr((found4 + INDEX_ONE),(found3 - found4 - INDEX_ONE));
                                    while((found = TmpUserStr.find_first_of(search_Str,INDEX_ZERO)) != string::npos)
                                    {
                                        TmpUserStr.erase(INDEX_ZERO,(found + INDEX_ONE));
                                    }
                                    TmpUserStr = PrepareQuoteString(TmpUserStr);
                                    TmpPswdStr = StrTempCmd.substr(found3 + INDEX_THREE);
                                    TmpPswdStr = TmpPswdStr.substr(INDEX_ZERO,(TmpPswdStr.length() - INDEX_FIVE));
                                    TmpPswdStr = PrepareQuoteString(TmpPswdStr);

                                    StrTempCmd = StrTempCmd.erase((found4 + INDEX_ONE),StrTempCmd.length());
                                    StrTempCmd = StrTempCmd  + TmpUserStr + " -p" + TmpPswdStr + " -v" + " &";
                                    DEBUGL5("The replaced string : %s \n",startCmd.c_str());

                                    system(StrTempCmd.c_str());
                                    restat=true;                                                               
                                }//end of WG check
                            }//end of if(search-char)
                            else { /*if user enters username in format 'username' only(without @) */
                                DEBUGL5("Username in TA is not of form userName(@)domainName : %s \n",startCmd.c_str());
                                if("WGGuestDisabled" == csServerAuthFlag)//Guest Logon Disabled + Workgroup
                                {
                                    DEBUGL5("Update :value of csServerAuthFlag %s \n",csServerAuthFlag.c_str());
                                    startCmd = "$EB2/bin/networkservice/smb start -l6";//option 6 is added in smb script to add user and password to smbpasswd
                                    /*EBX_DTFR_18233: we should escape special chars by using quotes*/
                                    csGuestLogonUser = PrepareQuoteString(csGuestLogonUser);
                                    startCmd += " -u" + csGuestLogonUser;  //username of Guest logon
                                    csGuestUserPasswd = PrepareQuoteString(csGuestUserPasswd);
                                    startCmd += " -p" + csGuestUserPasswd; //Password of guest logon
                                    startCmd += " -v &";
                                    DEBUGL5("Update:Guest Logon: Username in TA is not of form userName(@)domainName : %s \n",startCmd.c_str());
                                }
                                /*For Guest Enabled or AD/Domain*/
                                else
				{
					found4 = startCmd.find_first_of("u",INDEX_ZERO);
					found3 = startCmd.find_first_of(" ",found4);
					TmpUserStr = startCmd.substr((found4 + INDEX_ONE),(found3 - found4 - INDEX_ONE));
					while((found = TmpUserStr.find_first_of(search_Str,INDEX_ZERO)) != string::npos)
					{
						TmpUserStr.erase(INDEX_ZERO,found + INDEX_ONE);
					}
					TmpUserStr = PrepareQuoteString(TmpUserStr);
					TmpPswdStr = startCmd.substr(found3 + INDEX_THREE);
					TmpPswdStr = TmpPswdStr.substr(INDEX_ZERO,(TmpPswdStr.length()-INDEX_FIVE));
					TmpPswdStr = PrepareQuoteString(TmpPswdStr);

					startCmd = startCmd.erase((found4 + INDEX_ONE),startCmd.length());
					startCmd = startCmd + TmpUserStr + " -p" + TmpPswdStr + " -v" + " &";
				}
                                DEBUGL5("The replaced string : %s \n",startCmd.c_str());

                                system(startCmd.c_str());
                                restat = true;
                            } 
                        }//end of if(domDomainRef && LogUser)
                    }
                    //	hdb->EndTransaction(configDoc,eWRITE);
                    //	DEBUGL5("update: EndTransaction with eWRITE was called for configDoc\n");
                    else if ((check ||reload) && enabled)
                    { 
                        CString reloadCmd = PrepareCommandString(eReloadCommand,configRoot);
                        DEBUGL5("Update() : Restarting service[%s]\n",reloadCmd.c_str());

                        /*When user enters the username of form "USER_NAME@DOMAIN_NAME */
                        reloadCmd.copy(TempCmd,reloadCmd.length(),INDEX_ZERO);
                        CString StrTempCmd(TempCmd);
                        /*  		 	CString ValueStr ="";
                                        CString ValueStr1 ="";
                                        if (!domDomainRef && !LogUser) {
                                        DEBUGL1("Unable to fetch from Permanent DOM...BUG Present\n");
                                        }

                                        if(domDomainRef && LogUser)
                                        ValueStr = LogUser->getTextContent();
                                        ValueStr1= domDomainRef->getTextContent();*/
                        if(domDomainRef && LogUser){
                            string search_char("@");

                            /*In TA->SMB Session : Checking weather Domain name and Username are with proper domain*/
                            DEBUGL5("DOM values -> UserName :%s and domain name: %s\n ",ValueStr.c_str(),ValueStr1.c_str());
                            found1 = ValueStr.find_first_of(search_char);
                            if (found1!=string::npos)
                            {	
                                ValueStr.erase(INDEX_ZERO,found1+INDEX_ONE);
                                /*In TA->SMB Session : Checking weather Domainname and Username(uname@Domainname)are matching */
                                if(ValueStr.compare(INDEX_ZERO,ValueStr.length(),ValueStr1,INDEX_ZERO,ValueStr1.length())==0)
                                {	
                                    found3=reloadCmd.find_first_of(search_char);
                                    if (found3 != string::npos)
                                    {
                                        found2 = reloadCmd.find('-', found3 +INDEX_ONE); 
                                        reloadCmd = reloadCmd.substr(INDEX_ZERO,found3) + reloadCmd.substr(found2-INDEX_ONE);
                                    }
                                    DEBUGL5("DOMdomain and DOMuser are matching...\nSo,COMMANDSTRING:%s\n",reloadCmd.c_str());
                                    found4 = reloadCmd.find_first_of("u",INDEX_ZERO);
                                    found3 = reloadCmd.find_first_of(" ",found4);
                                    TmpUserStr = reloadCmd.substr((found4 + INDEX_ONE),(found3 - found4 - INDEX_ONE));
                                    while((found = TmpUserStr.find_first_of(search_Str,INDEX_ZERO)) != string::npos)
                                    {
                                        TmpUserStr.erase(INDEX_ZERO,(found+INDEX_ONE));
                                    }
                                    TmpUserStr = PrepareQuoteString(TmpUserStr);
                                    TmpPswdStr = reloadCmd.substr(found3 + INDEX_THREE);
                                    TmpPswdStr = TmpPswdStr.substr(INDEX_ZERO,(TmpPswdStr.length() - INDEX_THREE));
                                    TmpPswdStr = PrepareQuoteString(TmpPswdStr);

                                    reloadCmd = reloadCmd.erase((found4 + INDEX_ONE),reloadCmd.length());
                                    reloadCmd = reloadCmd + TmpUserStr + " -p" + TmpPswdStr + " -v";
                                    DEBUGL5("The replaced string : %s \n",reloadCmd.c_str());

                                    system(reloadCmd.c_str());
                                    restat = true;
                                }
                                else  /*Putting back to WG*/
                                {  

                                    DEBUGL5("DOM values-> UserName:%s & domain name: %s\n ",ValueStr.c_str(),ValueStr1.c_str()); 
                                    found4 =StrTempCmd.find_first_of("u",INDEX_ZERO);
                                    if (found4!=string::npos)  {
                                        /*Making the username BLANK in the Command String*/
                                        StrTempCmd.replace(found4+INDEX_ONE,((StrTempCmd.find("-",found4))- found4) -INDEX_ONE," ");
                                    }   
                                    else {
                                        //do nothing
                                    }
                                    DEBUGL5("DOMdomain and DOMuser not matching...\n So,COMMANDSTRING():%s\n",StrTempCmd.c_str());
                                    found4 = StrTempCmd.find_first_of("u",INDEX_ZERO);
                                    found3 = StrTempCmd.find_first_of(" ",found4);
                                    TmpUserStr = StrTempCmd.substr((found4 + INDEX_ONE),(found3 - found4 - INDEX_ONE));
                                    while((found = TmpUserStr.find_first_of(search_Str,INDEX_ZERO)) != string::npos)
                                    {
                                        TmpUserStr.erase(INDEX_ZERO,(found + INDEX_ONE));
                                    }
                                    TmpUserStr = PrepareQuoteString(TmpUserStr);
                                    TmpPswdStr = StrTempCmd.substr(found3 + INDEX_THREE);
                                    TmpPswdStr = TmpPswdStr.substr(INDEX_ZERO,(TmpPswdStr.length() - INDEX_THREE));
                                    TmpPswdStr = PrepareQuoteString(TmpPswdStr);

                                    StrTempCmd = StrTempCmd.erase((found4 + INDEX_ONE),StrTempCmd.length());
                                    StrTempCmd = StrTempCmd + TmpUserStr + " -p" + TmpPswdStr + " -v";
                                    DEBUGL5("The replaced string : %s \n",StrTempCmd.c_str());

                                    system(StrTempCmd.c_str());
                                    restat = true;
                                }//end of WG check
                            }//end of if (search_char)
                            else {/*if user enters username in format 'username' only(without @) */
                                DEBUGL5("Username in TA is not of form userName(@)domainName : %s \n",reloadCmd.c_str());
                               if("WGGuestDisabled" == csServerAuthFlag)//Guest Logon Disabled + Workgroup
                                {
                                    DEBUGL5("Update :value of csServerAuthFlag %s \n",csServerAuthFlag.c_str());
                                    reloadCmd = "$EB2/bin/networkservice/smb restart -l6"; //option 6 is added in smb script to add user and password to smbpasswd
                                    /*EBX_DTFR_18233: we should escape special chars by using quotes*/
                                    csGuestLogonUser = PrepareQuoteString(csGuestLogonUser);
                                    reloadCmd += " -u" + csGuestLogonUser;  //username of Guest logon
                                    csGuestUserPasswd = PrepareQuoteString(csGuestUserPasswd);
                                    reloadCmd += " -p" + csGuestUserPasswd; //Password of guest logon
                                    reloadCmd += " -v &";
                                    DEBUGL5("Update:Guest Logon: Username in TA is not of form userName(@)domainName : %s \n",reloadCmd.c_str());
                                }
                                /*For Guest Enabled or AD/Domain*/
                                else
                                {
					found4 = reloadCmd.find_first_of("u",INDEX_ZERO);
					found3 = reloadCmd.find_first_of(" ",found4);
					TmpUserStr = reloadCmd.substr((found4 + INDEX_ONE),(found3 - found4 - INDEX_ONE));
					while((found = TmpUserStr.find_first_of(search_Str,INDEX_ZERO)) != string::npos)
					{
						TmpUserStr.erase(INDEX_ZERO,(found + INDEX_ONE));
					}
					TmpUserStr = PrepareQuoteString(TmpUserStr);
					TmpPswdStr = reloadCmd.substr(found3 + INDEX_THREE);
					TmpPswdStr = TmpPswdStr.substr(INDEX_ZERO,(TmpPswdStr.length() - INDEX_THREE));
					TmpPswdStr = PrepareQuoteString(TmpPswdStr);

					reloadCmd = reloadCmd.erase((found4 + INDEX_ONE),reloadCmd.length());
					reloadCmd = reloadCmd + TmpUserStr + " -p" + TmpPswdStr + " -v";
				}
				DEBUGL5("The replaced string : %s \n",reloadCmd.c_str());

                                system(reloadCmd.c_str());
                                restat = true;
                            }
                        }//end of if(domDomainRef && LogUser)
                    }
		    else
				DEBUGL1("Update: Not matched for any condition depResult [%s] restat [%d] \n",depResult.c_str() , restat);
                    if (restat) {
                        statstr = getProcStatus(m_statusCmd);
                    }
                    DEBUGL5("update: EndTransaction with eWRITE was called for configDoc\n");
                    hdb->EndTransaction(configDoc,eWRITE);

                }else{
                    DEBUGL5("SambaNMO::Update serivce [%s] not stopped or restated because no change in settings and not disabled.startMode[%s] error = [%s]\n",m_serviceXpath.c_str(),startMode.c_str(), error.c_str());
                    needNotify = false;
                }

#if (0)/*Manoj 20110119 commented because script will run in background, so panel will not get proper status*/
                /*.. updating the PaneL weather MFP is in Domain or Workgroup .. */
                CIniFile IniT(m_confFile);
                ElementRef panelDomainRef = chelper::NavigatePath( CString("currentStatus"), configRoot, true);
                ElementRef cSecurity1 = chelper::NavigatePath("global/security", IniT.GetRoot(),true);

                if (panelDomainRef && cSecurity1) {
                    if ( "user" == cSecurity1->getTextContent() ){
                        ElementRef cWorkgroup = chelper::NavigatePath("global/workgroup", IniT.GetRoot(),true);
                        if (cWorkgroup){
                            CString confWorkgroup = cWorkgroup->getTextContent();
                            panelDomainRef->setTextContent(confWorkgroup.c_str());
                        }
                    }
                    else{
                        ElementRef cRealmname = chelper::NavigatePath("global/realm", IniT.GetRoot(),true);
                        if (cRealmname){
                            CString confRealm = cRealmname->getTextContent();
                            std::transform(confRealm.begin(),confRealm.end(),confRealm.begin(),(int(*)(int))::tolower);
                            panelDomainRef->setTextContent(confRealm.c_str());
                        }
                    }
                }
#endif
                DEBUGL5("Panel is Notified about the MFP level...!!!\n");
                /*...End of updating the PaneL...*/
                depResult = statstr;
                //For getting Linemode status
                //Status lineModeStatus = STATUS_FAILED;
                
				int flagLineMode = 0;
				int flagLineModeSP = 0;
				int lineModeValue = 1;
				if(csProductType == "S2_PRODUCT")
				{
					uint16 lineModeValueSP = 1;
					IndexedDB_ReadContent(5000,0,0,lineModeValueSP,true);
					lineModeValue = lineModeValueSP;
					flagLineModeSP = 1;
				}
				else
				{
	                Status lineModeStatus = STATUS_FAILED;
    	      //      int lineModeValue = 1;
	                lineModeStatus = CheckLinemode(lineModeValue);
    	            if (lineModeStatus == STATUS_OK)
        	        {
						flagLineMode = 1;
					}
				}
				if(((csProductType == "S2_PRODUCT") && (flagLineModeSP == 1)) || ((csProductType != "S2_PRODUCT") && (flagLineMode == 1)))
                {
					if (startMode == "00" && lineModeValue == 0)
                    {
                        //Manoj 20100819 : all config file update done installing driver for the 
                        //first boot.
                        Status installStatus = STATUS_FAILED;
			/*[Arnab]20120531 Updating Network DOM with webinstaller version and checking if installation is required*/
                        hdb->BeginTransaction(configDoc,eWRITE,true);
                        DEBUGL5("Update: BeginTransaction with eWRITE is called for configDoc\n");
			int installReq = checkVersion(configRoot);
                        DEBUGL5("update: EndTransaction with eWRITE was called for configDoc\n");
                        hdb->EndTransaction(configDoc,eWRITE);

			if(ERROR == installReq){
				DEBUGL1("[%s] Installl driver request error\n",__FUNCTION__);
				return "STATUS_FAILED";	
			}
                        installStatus = installPnPDriver(configRoot,installReq);
                        if (installStatus != STATUS_OK)
                            DEBUGL1("SMB NMO installPnPDriver failed !!!\n");
				/* Added work-around for STFR_13236, Samba is setting driver version information in Network DOM 
 				* before Unzip. We need to improve logic to set version information after Unzip, so that we can 
 				* update version in Network DOM only if unzip succeed. */	
			else{
	                        hdb->BeginTransaction(configDoc,eWRITE,true);
        	                DEBUGL5("Update: BeginTransaction with eWRITE is called for configDoc\n");
				int updated = UpdateDriverVer(configRoot);
				 DEBUGL5("update: EndTransaction with eWRITE was called for configDoc\n");
				hdb->EndTransaction(configDoc,eWRITE);
	                       
				if( ERROR == updated){
					DEBUGL1("[%s] Failed to update driver\n",__FUNCTION__);
					return "STATUS_FAILED";
				}
                        }
                    }
                    else
                        DEBUGL2("SMB NMO No need to install pnp driver error[%s] startMode[%s] lineModeValue[%d]\n", 
                                error.c_str(), startMode.c_str(), lineModeValue);

                }
                else
                {
                    DEBUGL1("SMB NMO CheckLinemode function failed\n");
                }
			/*[Arnab] Added work-around for STFR_13236 - WebInstaller icons do not exist in file 
 			* share folder Changes have been added to avoid mentioned scenario, If Network DOM driver 
 			* version and version.txt version are same and driver files are not present in EworkE
 			* partition. We need to extract it in work partition. */
		hdb->BeginTransaction(configDoc,eWRITE,true);
		DEBUGL5("Update: BeginTransaction with eWRITE is called for configDoc\n");
		int driverReq = checkVersion(configRoot);
		DEBUGL5("update: EndTransaction with eWRITE was called for configDoc\n");
		hdb->EndTransaction(configDoc,eWRITE);

		if(ERROR == driverReq)
		{
			DEBUGL1("Update: checkVersion failed \n");
			return "STATUS_FAILED";	
		}
		else if(NO_DRIVER == driverReq)
		{
			Status sInstallStatus = STATUS_FAILED;
			struct dirent **drivernamelist;
			int iDriverNumber = scandir("/work/drivers/pointNprint/UNI", &drivernamelist, 0, alphasort);
                        if (iDriverNumber <= MAX_DRIVER_FILES)
			{
                            	DEBUGL1("NO UNI drivers available\n");
				sInstallStatus = installPnPDriver(configRoot,ONLY_UNIV);
				if (sInstallStatus != STATUS_OK)
					DEBUGL1("SMB NMO installPnPDriver for UNIV failed !!!\n");
                        }
			iDriverNumber = scandir("/work/drivers/pointNprint/PS", &drivernamelist, 0, alphasort);
		        if (iDriverNumber <= MAX_DRIVER_FILES)
                        {
                            	DEBUGL1("NO PS3 drivers available\n");
				sInstallStatus = installPnPDriver(configRoot,ONLY_PS3);
				if (sInstallStatus != STATUS_OK)
					DEBUGL1("SMB NMO installPnPDriver for PS3 failed !!!\n");
                        }
			iDriverNumber = scandir("/work/drivers/pointNprint/XPS", &drivernamelist, 0, alphasort);
                        if (iDriverNumber <= MAX_DRIVER_FILES)
			{
                            	DEBUGL1("NO XPS drivers available\n");
				sInstallStatus = installPnPDriver(configRoot,ONLY_XPS);
				if (sInstallStatus != STATUS_OK)
					DEBUGL1("SMB NMO installPnPDriver for XPS failed !!!\n");
                        }


		}
                if (error.empty()){
					DEBUGL4("[%s] Success, function exit\n",__FUNCTION__);
                    return "STATUS_OK";
				}
                else{
					DEBUGL1("[%s] ERROR Encountered exiting\n",__FUNCTION__);
                    return "STATUS_FAILED";
				}
            }
/*[Arnab]Comment for the API*/
/*Return :: int; In error case returns ERROR(-1)*/
/*UNIV -> 4 ; PS3 -> 2; XPS -> 1*/
/*If only UNIV install req returns 4, If XPS and UNIV this function returns (4+1) -> 5 etc*/
/*Argument :: Network DOM reference*/


int CSmbNMO::checkVersion(ElementRef configRoot)
{
	DEBUGL4("[%s] function enter\n",__FUNCTION__);
	CString csline;
	CString univVersion;
	CString xpsVersion;
	CString ps3Version;
	CString univVersionNet;
	CString xpsVersionNet;
	CString ps3VersionNet;
	int retValue;
	ElementRef pRootNode = NULL;
	ElementRef pCurrentVersionNode  = NULL;
	HierarchicalDBRef hdb = HierarchicalDB::Acquire(0);
	if (!hdb)
	{
		DEBUGL1("CSmbNMO::checkVersion, RestoreSMBSetting: Unable to Acquire HDB return STATUS_FAILED\n");
		return ERROR;;
	}
	CString filePath = getenv("EB2");
	filePath += CString (DRIVERNOBUILDITEM) + "version.txt";
	ifstream myfile (filePath.c_str());
	if (myfile.is_open())
	{
		while (!myfile.eof())
		{
			getline (myfile,csline);
			unsigned int position = csline.find("univ");
			if(position != string::npos)
			{
				int  pos = csline.find("=");
				string  str = csline.substr (pos+1);
				bool isSpaces = true;
				while( isSpaces )
				{
					unsigned int posS = str.find(" ");
					if( posS != string::npos )
						str.erase( posS, 1 );
					else
						isSpaces = false;
				}
				univVersion = str;
				/*Changes made for STFR_13236*/
				m_univ_version = univVersion;
				DEBUGL5("univ version is :: %s \n", str.c_str());
			}
			position = csline.find("ps3");
			if(position != string::npos)
			{
				int  pos = csline.find("=");
				string  str = csline.substr (pos+1);
				bool isSpaces = true;
				while( isSpaces )
				{
					unsigned int posS = str.find(" ");
					if( posS != string::npos )
						str.erase( posS, 1 );
					else
						isSpaces = false;
				}
				ps3Version = str;
				/*Changes made for STFR_13236*/
				m_ps3_version = ps3Version;
				DEBUGL5(" ps3 version is :: %s \n", str.c_str());
			}
			position = csline.find("xps");
			if(position != string::npos)
			{
				int  pos = csline.find("=");
				string  str = csline.substr (pos+1);
				bool isSpaces = true;
				while( isSpaces )
				{
					unsigned int posS = str.find(" ");
					if( posS != string::npos )
						str.erase( posS, 1 );
					else
						isSpaces = false;
				}
				xpsVersion = str;
				/*Changes made for STFR_13236*/
				m_xps_version = xpsVersion;
				DEBUGL5(" xps version is :: %s \n", str.c_str());
			}
		}
		myfile.close();
	}
	else
		DEBUGL1("Unable to open version.txt file\n");
	if (!configRoot)
	{
		DEBUGL1("checkVersion : No Config root Present Return STATUS_FAILED\n");
		return ERROR;
	}
	pCurrentVersionNode = hdb->BindToElement(configRoot, "univVersion");
	if (!pCurrentVersionNode)
	{
		DEBUGL1("checkVersion: Unable to BIND univVersion,  Return STATUS_FAILED\n");
		return ERROR;
	}
	univVersionNet = pCurrentVersionNode->getTextContent();
	/*Changes made for STFR_13236, Samba is setting driver version information in Network DOM before Unzip. 
 	* We need to improve logic to set version information after Unzip, so that we can update version in 
 	* Network DOM only if unzip succeed. */
	/*if(univVersionNet != univVersion)
		pCurrentVersionNode->setTextContent(univVersion);*/
	pCurrentVersionNode = hdb->BindToElement(configRoot, "ps3Version");
	if (!pCurrentVersionNode)
	{
		DEBUGL1("checkVersion: Unable to BIND ps3Version,  Return STATUS_FAILED\n");
		return ERROR;
	}
	ps3VersionNet = pCurrentVersionNode->getTextContent();
	/*Changes made for STFR_13236*/
	/*if(ps3VersionNet != ps3Version)
		pCurrentVersionNode->setTextContent(ps3Version);*/
	pCurrentVersionNode = hdb->BindToElement(configRoot, "xpsVersion");
	if (!pCurrentVersionNode)
	{
		DEBUGL1("checkVersion: Unable to BIND xpsVersion,  Return STATUS_FAILED\n");
		return ERROR;
	}
	xpsVersionNet = pCurrentVersionNode->getTextContent();
	/*Changes made for STFR_13236*/
	/*if(xpsVersionNet != xpsVersion)
		pCurrentVersionNode->setTextContent(xpsVersion);*/

	retValue = ((univVersion != univVersionNet)?((ps3Version != ps3VersionNet)?((xpsVersion != xpsVersionNet)?(ONLY_UNIV_PS3_XPS):(ONLY_UNIV_PS3)):((xpsVersion != xpsVersionNet)?(ONLY_UNIV_XPS):(ONLY_UNIV))):((ps3Version != ps3VersionNet)?((xpsVersion != xpsVersionNet)?(ONLY_PS3_XPS):(ONLY_PS3)):((xpsVersion != xpsVersionNet)?(ONLY_XPS):(NO_DRIVER))));
	DEBUGL4("[%s] function exit\n",__FUNCTION__);
	return retValue ;

}
            /*
 * Work-around for STFR_13236
*   UpdateDriverVer()
*   Function discription : This API will be called from NMO Base UpdateDriverVer() 
*   will compare Network DOM driver version and version.txt version and update if it
*   is different.
*   Input Parameter: ConfigRoot
*   OutPut parameter: Status (if driver version update in DOM succeeded or failed
*/
int CSmbNMO::UpdateDriverVer(ElementRef configRoot)
{
	DEBUGL4("[%s] function enter\n",__FUNCTION__);
	ElementRef pCurrentVersionNode  = NULL;
        CString univVersionNet;
        CString xpsVersionNet;
        CString ps3VersionNet;
	
        if (!configRoot)
        {
                DEBUGL1("checkVersion : No Config root Present Return STATUS_FAILED\n");
				return ERROR;
        }
	HierarchicalDBRef hdb = HierarchicalDB::Acquire(0);
        if (!hdb)
        {
                DEBUGL1("RestoreSMBSetting: Unable to Acquire HDB return STATUS_FAILED\n");
                return ERROR;;
        }
        pCurrentVersionNode = hdb->BindToElement(configRoot, "univVersion");
        if (!pCurrentVersionNode)
        {
                DEBUGL1("checkVersion: Unable to BIND univVersion,  Return STATUS_FAILED\n");
			    return ERROR;
        }
        univVersionNet = pCurrentVersionNode->getTextContent();
        if(univVersionNet != m_univ_version)
                 pCurrentVersionNode->setTextContent(m_univ_version);
        pCurrentVersionNode = hdb->BindToElement(configRoot, "ps3Version");
        if (!pCurrentVersionNode)
        {
                DEBUGL1("checkVersion: Unable to BIND ps3Version,  Return STATUS_FAILED\n");
			    return ERROR;
        }
        ps3VersionNet = pCurrentVersionNode->getTextContent();
      if(ps3VersionNet != m_ps3_version)
                    pCurrentVersionNode->setTextContent(m_ps3_version);
        pCurrentVersionNode = hdb->BindToElement(configRoot, "xpsVersion");
        if (!pCurrentVersionNode)
        {
                DEBUGL1("checkVersion: Unable to BIND xpsVersion,  Return STATUS_FAILED\n");
			    return ERROR;
        }
        xpsVersionNet = pCurrentVersionNode->getTextContent();
        if(xpsVersionNet != m_xps_version)
           pCurrentVersionNode->setTextContent(m_xps_version);
		DEBUGL4("[%s] function exit\n",__FUNCTION__);
	return STATUS_OK;
}
            /*
             *   Manoj 20110301
             *   RestoreSMBSetting()
             *   Finction discription : This API will be called from NMO Base
             *   			RestoreSMBSetting() will compare currentStatus with groupName and domainName.
             *   			Match will kept same, and difference will be removed
             *   			if WG (or relevent) 	 
             *   				SET primaryDomainController	= "" 
             *  				SET backupDomainController	= ""
             *      				SET password			= ""
             *      			else if DOMIAN (or relevent)
             *      				SET groupName			= ""
             *   Assumption : Last Setup has currentStatus updated Properly			
             */
            Status CSmbNMO::RestoreSMBSetting(DocumentRef configRoot)
            {
                DEBUGL4("[%s] function enter\n",__FUNCTION__);
				if (!configRoot)
                {
                    DEBUGL1("RestoreSMBSetting: No Config root Present Return STATUS_FAILED\n");
					return STATUS_FAILED;
                }
                HierarchicalDBRef hdb = HierarchicalDB::Acquire(0);
                if (!hdb)
                {
                    DEBUGL1("RestoreSMBSetting: Unable to Acquire HDB return STATUS_FAILED\n");
	                return STATUS_FAILED;
                }

                ElementRef pRootNode		= NULL;
                ElementRef pCurrentStatusNode	= NULL;
                ElementRef pdomainNameNode	= NULL;
                ElementRef pgroupNameNode	= NULL;
                ElementRef ptempNode		= NULL;

                CString csCurrentStatus;
                CString csdomainName;
                CString csgroupName;

                pRootNode = hdb->BindToElement(configRoot, "Network/Protocols/SMB");
                if (!pRootNode)
                {
                    DEBUGL1("RestoreSMBSetting: Unable to BIND SMB,  Return STATUS_FAILED\n");
					return STATUS_FAILED;
                }

                /*Check Current Status, domain and WorkGroup Information  of the MFP,*/
                /*Check Current Status*/
                pCurrentStatusNode = hdb->BindToElement(pRootNode, "currentStatus");
                if (!pCurrentStatusNode)
                {
                    DEBUGL1("RestoreSMBSetting: Unable to BIND currentStatus,  Return STATUS_FAILED\n");
					return STATUS_FAILED;
                }
                csCurrentStatus = pCurrentStatusNode->getTextContent();
                std::transform(csCurrentStatus.begin(),csCurrentStatus.end(),csCurrentStatus.begin(),(int(*)(int))::tolower);
                DEBUGL5("RestoreSMBSetting: Current Status for samba = [%s]\n", csCurrentStatus.c_str());

                /*Check Domain Name of the MFP*/
                pdomainNameNode = hdb->BindToElement(pRootNode, "domainName"); 
                if (!pdomainNameNode)
                {
                    DEBUGL1("RestoreSMBSetting: Unable to BIND currentStatus,  Return STATUS_FAILED\n");
					return STATUS_FAILED;
                }
                csdomainName = pdomainNameNode->getTextContent();
                std::transform(csdomainName.begin(), csdomainName.end(), csdomainName.begin(),(int(*)(int))::tolower);
                DEBUGL5("RestoreSMBSetting: Domain Status for samba = [%s]\n", csdomainName.c_str());

                /*Check Workgroup Name of the MFP*/
                pgroupNameNode = hdb->BindToElement(pRootNode, "groupName"); 
                if (!pgroupNameNode)
                {
                    DEBUGL1("RestoreSMBSetting: Unable to BIND currentStatus,  Return STATUS_FAILED\n");
		            return STATUS_FAILED;
                }
                csgroupName = pgroupNameNode->getTextContent();
                std::transform(csgroupName.begin(), csgroupName.end(), csgroupName.begin(),(int(*)(int))::tolower);
                DEBUGL5("RestoreSMBSetting: group name Status for samba = [%s]\n", csgroupName.c_str());

                /*Now we have all the data to Judge SMB status, we will start calculating actual result*/

                if (csCurrentStatus == csgroupName)
                {
                    DEBUGL5("RestoreSMBSetting: Current Status is same As workgroup Information, Delete Doamin Information\n");
                    /*Set domainName parameter to NULL ("")*/
                    pdomainNameNode->setTextContent("");

                    /*Set Samba PDC  to NULL ("")*/
                    ptempNode = hdb->BindToElement(pRootNode, "primaryDomainController");
                    if (ptempNode)
                        ptempNode->setTextContent("");

                    /*Set Samba BDC  to NULL ("")*/
                    ptempNode = hdb->BindToElement(pRootNode, "backupDomainController");
                    if (ptempNode)
                        ptempNode->setTextContent("");

                    /*Set Samba Password to NULL ("")*/
                    ptempNode = hdb->BindToElement(pRootNode, "password");
                    if (ptempNode)
                        ptempNode->setTextContent("");
					DEBUGL4("[%s] function exit\n",__FUNCTION__);
                    return STATUS_OK;
                }
                else if (csCurrentStatus == csdomainName)
                {
                    DEBUGL4("RestoreSMBSetting:function exit Current Status is same As Domain Information, Delete WorkGroup Information\n");
                    /*Set Samba workgroup to NULL ("")*/
                    pgroupNameNode->setTextContent("");
                    return STATUS_OK;
                }
                else
                    DEBUGL5("RestoreSMBSetting: This is not desirable condition, need to check the issue\n");
				DEBUGL4("[%s] function exit\n",__FUNCTION__);
                return STATUS_FAILED;
            }
            /*
             *	RestoreDefaults()
             */

            CString CSmbNMO::RestoreDefaults(dom::ElementRef defaultRoot, dom::ElementRef configRoot,CString startMode,CString SerialNo,CString xpath,CString &serviceStatus)
            {
				DEBUGL4("[%s] function enter\n",__FUNCTION__);				
#ifdef MORE_DEBUG
                CString t;
                if( configRoot && HierarchicalDB::Acquire(0)->SerializeToString(configRoot,t) == STATUS_OK)
                    DEBUGL5("RestoreDefaults: current config node [%s]\n", t.c_str());
                if( defaultRoot && HierarchicalDB::Acquire(0)->SerializeToString(defaultRoot,t) == STATUS_OK)
                    DEBUGL5("RestoreDefaults: DefaultNode [%s]\n", t.c_str());
#endif

                // 
                //update config 
                //	depResults depResults;
                CString statstr = getProcStatus(m_statusCmd); 
                //pair<bool,bool> present_active= interpretProcStatus(statstr);

                // Setting hostname
                Status sret = SetDefaultValue(SerialNo,xpath,configRoot);
                DEBUGL5("RestoreDefaults: Called with SerialNo = [%s] and xpath [%s]\n",SerialNo.c_str(),xpath.c_str());
                if(sret == STATUS_FAILED)
                    DEBUGL1("RestoreDefaults: Failed to set default hostnames\n");

                //bool needDependRst=false; 
                CString result = CSmbNMO::Update(configRoot, defaultRoot,serviceStatus,startMode);
				DEBUGL4("[%s] function exit\n",__FUNCTION__);
                return result;
            }

            Status CSmbNMO::SetDefaultValue(CString SerialNo,CString xpath,dom::DocumentRef configDoc)
            {
               	DEBUGL4("[%s] function enter\n",__FUNCTION__); 
				HierarchicalDBRef hdb = HierarchicalDB::Acquire(0);
                DEBUGL5("SetDefaultValue: BeginTransaction with eWRITE is called for configDoc\n");
                hdb->BeginTransaction(configDoc,eWRITE,true);
                ElementRef configRoot = hdb->BindToElement(configDoc,m_serviceXpath);
                if(!configRoot)
                {
                    DEBUGL1("CSmbNMO::SetDefaultValue Unable to bind, CVonfigRoot is NULL\n");
                    DEBUGL5("SetDefault: EndTransaction with eWRITE was called for configDoc\n");
                    hdb->EndTransaction(configDoc,eWRITE);
                    return (STATUS_FAILED);
                }
                CString fNAME="rm -rf " +CString(SMB_DRIVER_PATH) + "*";
                system(fNAME.c_str());
                fNAME.clear();

                fNAME="rm -rf " + CString(DRIVERNTTDB) +"nt*";
                system(fNAME.c_str());
                fNAME.clear();

                fNAME="rm -rf " + CString(DRIVERPRINTTDB) +"*";
                system(fNAME.c_str());
                fNAME.clear();

                // Setting Default device name.
                if (xpath == ""){
                    DEBUGL1("SetDefaultValue: Failed to get service name [%s]\n",xpath.c_str());
                }
                else if (xpath == "Protocols/SMB"){
                    // For SMB devicename
                    CString deviceName = "MFP"+SerialNo;
                    ElementRef nmoConfigNode = chelper::NavigatePath(SMB_DEVICENAME,configRoot);
                    if(!nmoConfigNode){
                        DEBUGL1("SetDefaultValue: Failed to obtain object for [%s]\n", SMB_DEVICENAME);
     					 DEBUGL5("SetDefault: EndTransaction with eWRITE was called for configDoc\n");
                        hdb->EndTransaction(configDoc,eWRITE);
                       	return (STATUS_FAILED);
                    }
                    nmoConfigNode->setTextContent(deviceName);
                    DEBUGL5("SetDefaultValue: [%s] is updated hostname [%s]\n",SMB_DEVICENAME, nmoConfigNode->getTextContent().c_str());
                    nmoConfigNode = NULL ;

                    nmoConfigNode = chelper::NavigatePath("UpdateDriver",	configRoot);
                    if(!nmoConfigNode){
                        DEBUGL1("SetDefaultValue: Failed to obtain object for [%s]\n", "UpdateDriver");
						DEBUGL5("SetDefault: EndTransaction with eWRITE was called for configDoc\n");
                        hdb->EndTransaction(configDoc,eWRITE);
                        return (STATUS_FAILED);
                    }
                    nmoConfigNode->setTextContent("0");
                    DEBUGL5("SetDefaultValue: [%s] is updated UpdateDriver [%s]\n",SMB_DEVICENAME, nmoConfigNode->getTextContent().c_str());
					
        /* 08-8825(hostAnnouncement in Super sleep mode) code changes for Baikal(L4.625)*/
                	CString csProductType = "";
                	CString csModelType = "";
                	if(getenv("PRODUCT"))
                	{
                    	csProductType = CString(getenv("PRODUCT"));
                	}
                	if(getenv("SUB_PRODUCT"))
                	{
                    	csModelType = CString(getenv("SUB_PRODUCT"));
					}
					if(csProductType == "LOIRE")
					{
					    if(csModelType == "ECO_LOIRE")
					    {
							ElementRef hostAnnounce = chelper::NavigatePath(CString("hostAnnouncementSSL"), configRoot, true);
							if(hostAnnounce)
								hostAnnounce->setTextContent("0");
				 		}
					}
			/* 08-8825(hostAnnouncement in Super sleep mode) code changes ends here*/
			
			    /*08-9599(Samba Server) code changes : for WEISS_LL and WEISS_L_SSD models the default 
														value of 08-9599 is made "4" */
            	if(csProductType == "WEISS")
            	{
                	if((csModelType == "WEISS_L_SSD") || (csModelType == "WEISS_LL"))
                	{
                   		ElementRef fileshare_dis = chelper::NavigatePath(CString("fileshareEnabled"), configRoot, true);
                    	if(fileshare_dis)
                        	fileshare_dis->setTextContent("0");
                	}
            	}
			    /*08-9599(Samba Server) code changes ends here */

				}
                else{
                    DEBUGL5("SetDefaultValue: No need to craete default hostname or service name for [%s]",xpath.c_str());
                }
		/* 08-3724-1 ode changes for setting Default value of client NTLMv2 Auth= "yes" */
		CIniFile ini(m_confFile);
		ElementRef smbConfRef = chelper::NavigatePath("global/client~NTLMv2~auth", ini.GetRoot());
		if(smbConfRef)
		{
			DEBUGL5("SetDefaultValue: setting client~NTLMv2~auth = Yes\n");
			smbConfRef->setTextContent("Yes");
			ini.SetModified();
		}
		else
			DEBUGL2("SetDefaultValue: smbConfRef node not present\n");

		// On 08-9083, set clientAuthFlag flag to "Yes"

		ElementRef clientAuthFlagref = chelper::NavigatePath("global/clientAuthFlag", ini.GetRoot());
		if(clientAuthFlagref)
		{
			DEBUGL5("SetDefaultValue: setting clientAuthFlag = Yes\n");
			clientAuthFlagref->setTextContent("Yes");
			ini.SetModified();
		}
		else
			DEBUGL2("SetDefaultValue: clientAuthFlagref node not present\n");


		/*08-3724-1 code changes ends here*/
                /*RCR_587: 2018115: Delete existing user from /etc/passwd file and passdb.tdb if 9083 is performed.
                 * If not deleted, new user cannot be added after 9083, bcz already added uid 9999/9998 will be present for a user*/
                DEBUGL5("[%s] deleting all users present in passdb.tdb\n",__FUNCTION__);
                CString csDeleteRes = DBUsersDeletion();//delete user from passwd and passdb.tdb files
                if(csDeleteRes != "STATUS_OK")
                {
                    DEBUGL2("SetDefaultValue: serverAuthUser: User deletion failed. Hence we need to return error to TA or Self-Diagnostic\n");
                    return STATUS_FAILED;
                }
                else
                    DEBUGL5("SetDefaultValue: serverAuthUser: successfully deleted users\n");

                DEBUGL5("SetDefault: EndTransaction with eWRITE was called for configDoc\n");
                hdb->EndTransaction(configDoc,eWRITE);
		DEBUGL4("[%s] function exit\n",__FUNCTION__);
                return STATUS_OK;
            }
            void CSmbNMO::ReadConfigParameter(ElementRef pTempRootPart)
            {
                DEBUGL4("[%s] function enter\n",__FUNCTION__);
				CString csComV12;
                CString csComV3;
                CString csConfigFile;
                CString csWirelessEnable;


                ElementRef pTempRoot = getNetworkNodeNode(pTempRootPart);
                if (!pTempRoot)
                {
                    DEBUGL1("[%s]Unable to getParentNode, return error\n",__FUNCTION__);
                }

                HierarchicalDBRef m_hdb = HierarchicalDB::Acquire(0);
                if (!m_hdb)
                {
                    DEBUGL1("[%s]Unable to Axcquire hdb, return error\n",__FUNCTION__);
				    return ;
                }
                ElementRef pReadNode = m_hdb->BindToElement(pTempRoot, SMB_WLN_ENABLE);
                if (pReadNode)
                {
                    csWirelessEnable =  pReadNode->getTextContent();
                    DEBUGL5("[%s]wireless Enable Status [%s]from [%s]\n",__FUNCTION__, csWirelessEnable.c_str(), SMB_WLN_ENABLE);
                    pReadNode = NULL;
                }
                else
                {
                    DEBUGL1("[%s]Unable to read from [%s], No return\n",__FUNCTION__, SMB_WLN_ENABLE);
                    pReadNode = NULL;
                }

                if (SAMBA_ENABLE == csWirelessEnable)
                    csConfigFile = CString (TCP_IP_CONF_WLAN);
                else
                    csConfigFile = CString (TCP_IP_CONF_ETH);

                CString csConfigFilePath = CString (getenv ("AL_NET_CONF")) + "/" + csConfigFile;

                DEBUGL5("[%s]Complete config file [%s],\n",__FUNCTION__, csConfigFilePath.c_str());
                CIniFile ini(csConfigFilePath);

                pReadNode = chelper::NavigatePath("IPADDR", ini.GetRoot(), true);
                if (pReadNode)
                {
                    m_IPAddress = pReadNode->getTextContent();
                    DEBUGL5("[%s]read IPaddress [%s]from [%s]\n",__FUNCTION__,m_IPAddress.c_str(),SMB_READ_IP);
                    pReadNode = NULL;
                }
                else
                {
                    DEBUGL1("[%s]Unable to read from [%s], No return\n",__FUNCTION__,SMB_READ_IP);
                    pReadNode = NULL;
                }

                pReadNode = m_hdb->BindToElement(pTempRoot, SMB_READ_SNMP_V1_2);
                if (pReadNode)
                {
                    csComV12 = pReadNode->getTextContent();
                    DEBUGL5("[%s]read snmvv1_2 version as [%s] from [%s]\n",__FUNCTION__,csComV12.c_str(),SMB_READ_SNMP_V1_2);
                    pReadNode = NULL;
                }
                else
                {
                    DEBUGL1("[%s]Unable to read from [%s], No return\n",__FUNCTION__,SMB_READ_SNMP_V1_2);
                    pReadNode = NULL;
                }


                pReadNode = m_hdb->BindToElement(pTempRoot, SMB_READ_SNMP_V3);
                if (pReadNode)
                {
                    csComV3 = pReadNode->getTextContent();
                    DEBUGL5("[%s]read snmvv3 version as [%s]\n",__FUNCTION__,csComV3.c_str(),SMB_READ_SNMP_V3);
                    pReadNode = NULL;
                }
                else
                {
                    DEBUGL1("[%s]Unable to read from [%s], No return\n",__FUNCTION__,SMB_READ_SNMP_V3);
                    pReadNode = NULL;
                }

                /*Calculate Actual SNMP version*/
                if ((SAMBA_ENABLE == csComV3) && (SAMBA_ENABLE == csComV12))
                    m_SnmpVersion = CString (SMB_SNMP_VERSION_1);
                else if (SAMBA_ENABLE == csComV12)
                    m_SnmpVersion = CString (SMB_SNMP_VERSION_1);
                else if (SAMBA_ENABLE == csComV3)
                    m_SnmpVersion = CString (SMB_SNMP_VERSION_3);
                else
                    DEBUGL1("[%s]Undefined SNMP version\n",__FUNCTION__);

                DEBUGL5("[%s]read SNMP [%s]\n",__FUNCTION__,m_SnmpVersion.c_str());

                pReadNode = m_hdb->BindToElement(pTempRoot, SMB_WRITE_SNMP_COM);
                if (pReadNode)
                {
                    m_SnmpCommunity = pReadNode->getTextContent();
                    DEBUGL5("[%s]read snmv community version as [%s]\n",__FUNCTION__,m_SnmpCommunity.c_str(),SMB_WRITE_SNMP_COM);
                    pReadNode = NULL;
                }
                else{
                    DEBUGL1("[%s]Unable to read from [%s], No return\n",__FUNCTION__,SMB_WRITE_SNMP_COM);
                    pReadNode = NULL;
                }
				DEBUGL4("[%s] function exit\n",__FUNCTION__);
                return ;
            }

            /*
             * 20110615 Manoj: 
             * getNetworkNodeNode()
             * Finction discription : GetParentNode from childNode
             * 			Please refer TA specification DOC "TopAccessInstaller_SRS_en_110602.doc"
             */
            ElementRef CSmbNMO::getNetworkNodeNode(ElementRef pTempRoot)
            {
                DEBUGL4("[%s] function enter\n",__FUNCTION__);
				if (!pTempRoot)
                {
                    DEBUGL1("CSmbNMO::getNetworkNodeNode, Invalid input parameter returning NULL\n");
                    return (NULL);
                }
                ElementRef pTempNode = pTempRoot->getParentNode();
                if (!pTempNode)
                {
                    DEBUGL1("CSmbNMO::getNetworkNodeNode, Unable to get ParentNode returning [%d]\n",__LINE__);
                    return (NULL);
                }
                DEBUGL5("parent node of krb = [%s]\n",(pTempNode->getNodeName()).c_str());
                ElementRef pNetworkNode  = pTempNode->getParentNode();
                if (!pNetworkNode)
                {
                    DEBUGL1("CSmbNMO::getNetworkNodeNode,Unable to get ParentNode returning [%d]\n",__LINE__);
					return (NULL);
                }
                DEBUGL4("getNetworkNodeNode function exit NetworkNode = [%s]\n",(pNetworkNode->getNodeName()).c_str());
                return (pNetworkNode);
            }


            CString CSmbNMO::WritableName (CString csOrgName)
            {
                DEBUGL4("[%s] function enter\n",__FUNCTION__);
				CString csWritableName;
				uint16 brand = 0;
				int flagWriteName = 0;
				CString csBrandType = "";
				CString csProductType = "";
				if(getenv("EQUIPMENT_BRAND"))
                {
                    csBrandType = CString(getenv("EQUIPMENT_BRAND"));
                }
                if(getenv("PRODUCT"))
                {
                    csProductType = CString(getenv("PRODUCT"));
                }
                if(csProductType == "S2_PRODUCT")
            	{
                	IndexedDB_ReadContent(8, 9003, 0, brand, false);
                	DEBUGL5("Value of 9003 is %d\n",brand);
					flagWriteName = 1;
 				}
                if ("XPS" == csOrgName)
                    csWritableName = csOrgName;
                else if ("PS3" == csOrgName)
                    csWritableName = csOrgName;
                else if ("UNI" == csOrgName)
				{
                /*For L5.0 we need to read EQUIPMENT_BRAND from 08-9003. It returns either 0(Toshiba) or 1(OKI). 
		 * for the above change,EQUIPMENT_BRAND env variable in smbnmo will be changed to 
		 * reading 08-9003 value using IndexDB function*/

			    if(((flagWriteName == 1) && (OKI == brand)) || ((csProductType != "S2_PRODUCT") && (csBrandType == "OKI")))
					csWritableName = "PCL6";
				else
                    csWritableName = "UNIV";
				}
                else if ("PLUGIN" == csOrgName)	/*Dead Code blocked in ModifyConfiguration ()*/
                    csWritableName = csOrgName;

                DEBUGL4("[%s] function exit, ConfigFile Param to write[%s] for [%s]\n",__FUNCTION__, csWritableName.c_str(), csOrgName.c_str());
                return csWritableName;
            }

            /*
             * 20110615 Manoj: 
             * ModifyConfiguration()
             * Finction discription : This API will modify configuration
             * 			Please refer TA specification DOC "TopAccessInstaller_SRS_en_110602.doc"
             */
            Status CSmbNMO::ModifyConfiguration (CString csFileName)
            {
				DEBUGL4("[%s] function enter\n",__FUNCTION__);
				/*CString csCfgFile;
                ofstream File;
                */
                CString csCfgFile;
                CString dupcsCfgFile;
                CString readline;
				CString szCommand;
                ifstream Read_File;
                ofstream Write_File;
                int retStatus=-1;
				const char *configField[FIELD_VALUE]={"PRINTER_NAME","PORT_NAME","DEFAULT_PRINTER","MODE","SNMP_VER","SNMP_COMMUNITY"};
				int checkField[FIELD_VALUE]={0};

                if ( "PLUGIN" == IdentifyDriver(csFileName, SMB_GET_DRIVER_NAME))
                {
                    DEBUGL4("[%s]  function exit, No need to modify [%s]\n",__FUNCTION__, (IdentifyDriver(csFileName, SMB_GET_DRIVER_NAME)).c_str());
                    return STATUS_OK;
                }

                csCfgFile = IdentifyDriver(csFileName, SMB_GET_DRIVER_PATH) + IdentifyDriver(csFileName, SMB_GET_DRIVER_FILE_NAME);


                DEBUGL5("[%s] ConfigFile to modify [%s]\n",__FUNCTION__, csCfgFile.c_str());
                if (csCfgFile == "")
                {
                    DEBUGL4("[%s] function exit, No need to modify NULL CONFIG FILE [%s]\n",__FUNCTION__, csFileName.c_str());
                    return STATUS_OK;
                }

                
                 Read_File.open (csCfgFile.c_str(), ios_base::in|ios_base::binary);

				 if (Read_File.is_open())
                {
					dupcsCfgFile=csCfgFile+".cfg";
                	szCommand="touch "+dupcsCfgFile;
                	retStatus = system(szCommand.c_str());
                	if(WEXITSTATUS(retStatus)!=0){
                    	return STATUS_FAILED;
					}
					
					Write_File.open (dupcsCfgFile.c_str(), ios_base::trunc|ios_base::binary);
                    while(getline(Read_File,readline,'\n'))
                    {
                        if(Write_File.is_open())
                        {
                                size_t pos=-1;

                                pos=readline.find("=");
                                string str(readline,0,pos);
								DEBUGL5("READ LINE FROM CONFIG FILE===%s\n",readline.c_str());


                                if(str=="PRINTER_NAME"){
                                        Write_File<< "PRINTER_NAME="<< WritableName(IdentifyDriver(csFileName, SMB_GET_DRIVER_NAME)) <<" on MFP " << m_IPAddress  << endl ;
										checkField[0]=1;
								}
                                else if(str=="PORT_NAME"){
                                        Write_File << "PORT_NAME=IP_" << m_IPAddress << endl ;
										checkField[1]=1;
								}
                                else if(str=="DEFAULT_PRINTER"){
                                        Write_File << "DEFAULT_PRINTER=Y" << endl;
										checkField[2]=1;
								}
                                else if(str=="MODE"){
                                        Write_File << "MODE=SILENT" << endl;
										checkField[3]=1;
								}
                                else if(str=="SNMP_VER"){
                                        Write_File << "SNMP_VER=" << m_SnmpVersion << endl;
										checkField[4]=1;
								}
                                else if(str=="SNMP_COMMUNITY"){
                                        Write_File << "SNMP_COMMUNITY=" << m_SnmpCommunity << endl;
										checkField[5]=1;
								}
                                else{
										readline.erase(readline.find_last_not_of("\n\r\t")+1);
										Write_File << readline << endl;
								}
                        }
						else
						{
								DEBUGL2("[%s] Unable to open WriteFile modify [%s]\n",__FUNCTION__, dupcsCfgFile.c_str());
			                    return STATUS_FAILED;
                    	}

                	}
				}
                else {
                    DEBUGL2("[%s] Unable to open Readfile modify [%s]\n",__FUNCTION__, csCfgFile.c_str());
                    return STATUS_FAILED;
                }
				for(int index=0;index<FIELD_VALUE;index++)
				{
					if(checkField[index]==0)
						DEBUGL2("[%s]--->[%s] parameter not present in [%s] config file\n",__FUNCTION__,configField[index],csCfgFile.c_str());
				}
                if (Read_File.is_open())
                    Read_File.close();
                if (Write_File.is_open())
                    Write_File.close();

                szCommand="mv "+dupcsCfgFile+" "+csCfgFile;
                retStatus = system(szCommand.c_str());
                if(WEXITSTATUS(retStatus)!=0){
					DEBUGL1("[%s] ERROR Moving the config  file\n",__FUNCTION__);
                    return STATUS_FAILED;
                }
				DEBUGL4("[%s] function exit\n",__FUNCTION__);
                return STATUS_OK;
            }
            /*
             * 20110615 Manoj: 
             * IdentifyDriver()
             * Finction discription : This API will Idenntify driver and return folder PATH 
             * 			Please refer TA specification DOC "TopAccessInstaller_SRS_en_110602.doc"
             */
            CString  CSmbNMO::IdentifyDriver (CString csFileName, int iReturnConfigName)
            {
                DEBUGL4("[%s] function enter\n",__FUNCTION__);
				CString csReturnString;
				uint16 brand = 1;
		
				int flagDriver = 0;
				CString csBrandType = "";
                CString csProductType = "";
                if(getenv("EQUIPMENT_BRAND"))
                {
                    csBrandType = CString(getenv("EQUIPMENT_BRAND"));
                }
                if(getenv("PRODUCT"))
                {
                    csProductType = CString(getenv("PRODUCT"));
                }

                /*Change the case of read File, to avoid case issues*/
                std::transform(csFileName.begin(), csFileName.end(), csFileName.begin(), (int(*)(int))::toupper );

                DEBUGL5("[%s] Processing  string [%s] ->[%s][%s][%s][%s] mode [%d]\n",
                        __FUNCTION__, csFileName.c_str(), SMB_XPS_DRIVER_PKG, SMB_PS_DRIVER_PKG, SMB_UNI_DRIVER_PKG, SMB_DRIVER_PLUGIN_PKG,iReturnConfigName);
			if(csProductType == "S2_PRODUCT")
			{
				IndexedDB_ReadContent(8, 9003, 0, brand, false);
				DEBUGL5("Value of 9003 is %d\n",brand);
				flagDriver = 1;
			}
                if (CString (SMB_XPS_DRIVER_PKG) == csFileName)
				{
                    if (iReturnConfigName == SMB_GET_DRIVER_FILE_NAME)
		    {
						if(((flagDriver == 1) && (OKI != brand)) || ((csProductType != "S2_PRODUCT") && (csBrandType != "OKI")))
                        csReturnString =  CString (SMB_XPS_CONFIG);
			else
				if(CString(getenv("PRODUCT")) != "MASH" || CString(getenv("PRODUCT")) != "BP")
					csReturnString =  CString (SMB_XPS_CONFIG_OKI_M);
				else if(CString(getenv("PRODUCT")) != "LOIRE" || CString(getenv("PRODUCT")) != "ALABAMA")
					csReturnString =  CString (SMB_XPS_CONFIG_OKI_L);
				else
                        csReturnString =  CString (SMB_XPS_CONFIG);
				
		    }
                    else if (iReturnConfigName == SMB_GET_DRIVER_PATH)
                        csReturnString =   CString(SMB_DRIVER_XPS);
                    else if (iReturnConfigName == SMB_GET_DRIVER_NAME)
                        csReturnString = CString("XPS");
                    else	
                        csReturnString = "";

                    DEBUGL5("[%s] Return string [%s]\n",__FUNCTION__, csReturnString.c_str());
                    return csReturnString;
                }
                else if (CString (SMB_PS_DRIVER_PKG) == csFileName)
				{
                    if (iReturnConfigName == (SMB_GET_DRIVER_FILE_NAME))
		    {
                        if(((flagDriver == 1) && (OKI != brand)) || ((csProductType != "S2_PRODUCT") && (csBrandType != "OKI")))
                        csReturnString =  CString (SMB_PS_CONFIG);
			else
				if(CString(getenv("PRODUCT")) != "MASH" || CString(getenv("PRODUCT")) != "BP")
                                        csReturnString =  CString (SMB_PS_CONFIG_OKI_M);
                                else if(CString(getenv("PRODUCT")) != "LOIRE" || CString(getenv("PRODUCT")) != "ALABAMA")
                                        csReturnString =  CString (SMB_PS_CONFIG_OKI_L);
                                else
                        csReturnString =  CString (SMB_PS_CONFIG);
		    }
                    else  if (iReturnConfigName ==  (SMB_GET_DRIVER_PATH))
                        csReturnString =  CString (SMB_DRIVER_PS);
                    else if (iReturnConfigName ==  (SMB_GET_DRIVER_NAME))
                        csReturnString = CString("PS3");
                    else
                        csReturnString = "";

                    DEBUGL5("[%s] Return string [%s]\n",__FUNCTION__, csReturnString.c_str());
                    return csReturnString;
                }
               else if ((CString (SMB_UNI_DRIVER_PKG) == csFileName) || ((((flagDriver == 1) && (OKI == brand)) || ((csProductType != "S2_PRODUCT") && (csBrandType == "OKI"))) && ((CString (SMB_UNI_DRIVER_PKG_OKI_C) == csFileName)))){
                    if (iReturnConfigName == (SMB_GET_DRIVER_FILE_NAME))
		    {
                        if(((flagDriver == 1) && (OKI != brand)) || ((csProductType != "S2_PRODUCT") && (csBrandType != "OKI")))
                        csReturnString = CString (SMB_UNI_CONFIG);
			else
				if(CString(getenv("PRODUCT")) != "MASH" || CString(getenv("PRODUCT")) != "BP")
					csReturnString = CString (SMB_UNI_CONFIG_OKI_M);
				else if(CString(getenv("PRODUCT")) != "LOIRE" || CString(getenv("PRODUCT")) != "ALABAMA")
					csReturnString = CString (SMB_UNI_CONFIG_OKI_L);
				else
                        csReturnString = CString (SMB_UNI_CONFIG);
		    }
                    else if (iReturnConfigName == (SMB_GET_DRIVER_PATH))
                        csReturnString = CString (SMB_DRIVER_UNI);
                    else if (iReturnConfigName == (SMB_GET_DRIVER_NAME))
                        csReturnString = CString("UNI");
                    else
                        csReturnString = "";
                    DEBUGL5("[%s] Return string [%s]\n",__FUNCTION__, csReturnString.c_str());
                    return csReturnString;
                }
                else if (CString (SMB_DRIVER_PLUGIN_PKG) == csFileName){
                    if (iReturnConfigName==  (SMB_GET_DRIVER_PATH))
                        csReturnString = CString (SMB_DRIVER_PLUGIN);
                    else if (iReturnConfigName ==  (SMB_GET_DRIVER_NAME))
                        csReturnString = CString("PLUGIN");
                    else
                        csReturnString = ""; /*No config file for PLUGIN*/

                    DEBUGL5("[%s] Return string [%s]\n",__FUNCTION__, csReturnString.c_str());
                    return csReturnString;
                }

                DEBUGL4("[%s]  function exit Return folder NULL for request [%s], error \n",__FUNCTION__, csFileName.c_str());
				return "";
            }

            /*
             * 20110615 Manoj: 
             * CleanDriverFolder()
             * Finction discription : We Need to clean driver first before installing 
             * 			Please refer TA specification DOC "TopAccessInstaller_SRS_en_110602.doc"
             */
            void CSmbNMO::CleanDriverFolder(CString csFileName)
            {
                DEBUGL4("[%s] function enter\n",__FUNCTION__);
				CString csCleanDirectory;
                CString csCleanFile;
                int retStatus = -1;

                /*Create folder to delete*/
                csCleanDirectory = "rm -rf ";
                csCleanFile = IdentifyDriver(csFileName, SMB_GET_DRIVER_PATH);	

                if (csCleanFile == "")
                {
                    DEBUGL5("[%s] Returned PATH is NULL, No Clean folder for [%s]\n",__FUNCTION__,csFileName.c_str());
                }
                csCleanDirectory += csCleanFile;

                DEBUGL5("[%s] Clean folder [%s]\n",__FUNCTION__,csCleanDirectory.c_str());

                /*Finally Delete folder*/
                retStatus = system(csCleanDirectory.c_str());
                if(WEXITSTATUS(retStatus)!=0)
                    DEBUGL1("[%s]Remove directory for [%s] driver with command [%s] failed!!!\n",__FUNCTION__,csFileName.c_str(),csCleanDirectory.c_str());
				DEBUGL4("[%s] function exit\n",__FUNCTION__);
                return ;
            }	

            /*
             * Install Point and print driver in /work/drivers/pointNprint folder. Unzips the uploaded driver file
             * parses .inf file to get the driver details and executes rpcclient adddriver and setdriver commands
             * to install the printer driver.
             * @param[in] driverFilePath : Absolute path of the zip file uploaded from Top access
             * @return : int
             */

            Status CSmbNMO::InstallPrinterDriver(CString driverFilePath, CString csFileName)
            {
				DEBUGL4("[%s] function enter\n",__FUNCTION__);
                CString csMakeDirectory;
                CString csDriverName;
                int retStatus = -1;
                Status sRetStatus = STATUS_FAILED;
                /* webinstaller PnP*/
                int driverNumber = 0;
                struct dirent **drivernamelist;
                CString hide_files = "";
                const char *setup = "Setup.exe";	
                const char *essetup = "essetup.exe";	
                CString pri_available = "";
                /* webinstaller PnP end*/

                /*Remove directory for specified driver, return STATUS not required*/
                CleanDriverFolder(csFileName);		

                /*Create Valid directory for driver*/
                csDriverName = IdentifyDriver(csFileName, SMB_GET_DRIVER_PATH);
                csMakeDirectory="mkdir -p ";
                csMakeDirectory += csDriverName;
                DEBUGL5("[%s]Creating directory for [%s]  driver with command [%s]\n",__FUNCTION__,csDriverName.c_str(), csMakeDirectory.c_str());	
                retStatus = system(csMakeDirectory.c_str());
                if(WEXITSTATUS(retStatus) != 0)
                    DEBUGL1("[%s]Creating directory for driver with command [%s] failed!!!\n",__FUNCTION__,csMakeDirectory.c_str());
                retStatus = -1;
                csMakeDirectory.clear();

                /*Folder Creation is now complete, start extraction*/
                Ref<ci::codecs::Zip> ciZipref = ci::codecs::Zip::Acquire();
                if(!ciZipref){
                    DEBUGL1("[%s]:ciZipref Acquire Fail, cannot extract Driver, return error\n",__FUNCTION__);
                    return STATUS_FAILED;
                }

                sRetStatus = ciZipref->ExtractZip(driverFilePath.c_str(), SMB_DRIVER_PATH, "",true);
                if (STATUS_FAILED == sRetStatus)
                {
                    DEBUGL1("[%s]:ciZipref extract Failed, cannot extract Driver [%s], return error\n",__FUNCTION__,driverFilePath.c_str());
                    return STATUS_FAILED;
                }
                /*Driver Instillation Complete Time to modify configFile*/
                if (STATUS_FAILED == ModifyConfiguration (csFileName))
                {
                    DEBUGL1("[%s]:ModifyConfig Failed, return error\n",__FUNCTION__,driverFilePath.c_str());
                    return STATUS_FAILED;
                }
                /* webinstaller PnP: The below section takes care of updating the driver share and hide file details when the user uploads
                 * drivers from TA*/ 
                CIniFile ini(m_confFile);
                ElementRef pri = chelper::NavigatePath("Print/available",ini.GetRoot(), true);
                if(pri)
                    pri_available = pri->getTextContent();
                if("1" == pri_available) {
                    if(csFileName == "univ.zip"){
                        CString driv_str = readversionfile(1);
                        if((driv_str != "STATUS_FAILED") && (driv_str != "")){
                        driverNumber = scandir("/work/drivers/pointNprint/UNI", &drivernamelist, 0, alphasort);
                        if (driverNumber <= 0){
                            DEBUGL5("NO UNI drivers1\n");
                                SavetoConfig((driv_str + CString ("_driver/available")),"0");
                        }
                        else{
                            hide_files = "";
                            while(driverNumber--){
                                size_t driverfile_size = strlen(drivernamelist[driverNumber]->d_name);
                                if((strncmp((drivernamelist[driverNumber]->d_name),setup,driverfile_size)== 0) ||
								(strncmp((drivernamelist[driverNumber]->d_name),essetup,driverfile_size)== 0))
                                {
									continue;
								}
								else
								{
                                    hide_files += "/work/drivers/pointNprint/UNI/" + CString(drivernamelist[driverNumber]->d_name);
                                }
                                if(drivernamelist[driverNumber])
                                    free(drivernamelist[driverNumber]);
                            }
                            if(drivernamelist)
                                free(drivernamelist);
                                SavetoConfig((driv_str + CString ("_driver/hide~files")),hide_files);
                                SavetoConfig((driv_str + CString ("_driver/available")),"1");
                            }
                        }
                    }
                    else if(csFileName == "ps3.zip"){
                        CString driv_str = readversionfile(2);
                        if((driv_str != "STATUS_FAILED") && (driv_str != "")){
                        driverNumber = 0;
                            driverNumber = scandir("/work/drivers/pointNprint/PS", &drivernamelist, 0, alphasort);
                        if (driverNumber <= 0){
                                DEBUGL5("NO PS drivers1\n");
                                SavetoConfig((driv_str + CString ("_driver/available")),"0");
                        }
                        else{
                            hide_files = "";
                            while(driverNumber--){
                                size_t driverfile_size = strlen(drivernamelist[driverNumber]->d_name);
                                if((strncmp((drivernamelist[driverNumber]->d_name),setup,driverfile_size)== 0) || 
								(strncmp((drivernamelist[driverNumber]->d_name),essetup,driverfile_size)== 0))
                                {
									continue;
								}
								else
                                {
                                        hide_files += "/work/drivers/pointNprint/PS/" + CString(drivernamelist[driverNumber]->d_name);
                                }
                                if(drivernamelist[driverNumber])
                                    free(drivernamelist[driverNumber]);
                            }
                                if(drivernamelist)
                                free(drivernamelist);
                                SavetoConfig((driv_str + CString ("_driver/hide~files")),hide_files);
                                SavetoConfig((driv_str + CString ("_driver/available")),"1");
                        }
                    }
                    }
                    else if(csFileName == "xps.zip"){
			CString driv_str = readversionfile(3);
                        if((driv_str != "STATUS_FAILED") && (driv_str != "")){
                        driverNumber = 0;			
                        driverNumber = scandir("/work/drivers/pointNprint/XPS", &drivernamelist, 0, alphasort);
                        if (driverNumber <= 0){
                            DEBUGL5("NO XPS drivers1\n");
                            SavetoConfig((driv_str + CString ("_driver/available")),"0");
                        }
                        else{
                            hide_files = "";
                            while(driverNumber--){
                                size_t driverfile_size = strlen(drivernamelist[driverNumber]->d_name);
                                if((strncmp((drivernamelist[driverNumber]->d_name),setup,driverfile_size)== 0) ||
								(strncmp((drivernamelist[driverNumber]->d_name),essetup,driverfile_size)== 0))
                                {
									continue;
								}
								else
                                {
                                    hide_files += "/work/drivers/pointNprint/XPS/" + CString(drivernamelist[driverNumber]->d_name);
                                }
                                if(drivernamelist[driverNumber])
                                    free(drivernamelist[driverNumber]);
                            }
                            if(drivernamelist)
                                free(drivernamelist);
                            SavetoConfig((driv_str + CString ("_driver/hide~files")),hide_files);
                            SavetoConfig((driv_str + CString ("_driver/available")),"1");
                        }
                        }
                    }
                }
                /* webinstaller PnP end */
				DEBUGL4("[%s] function exit\n",__FUNCTION__);
                return STATUS_OK;
            }

            bool CSmbNMO::setSSCCommandPath(CString docNameValue, CString cXpath, CString csConfPath, CString csValue)
            {
                DEBUGL4("[%s] function enter\n",__FUNCTION__);
				HierarchicalDBRef hdb = HierarchicalDB::Acquire(0);
                if(!hdb){
					DEBUGL1("[%s] Acquire HDB Failed\n",__FUNCTION__);
                    return false;
				}
                ElementRef configNode = GetDocNode(docNameValue, "Network/Protocols/SMB/"+cXpath);
                if (!configNode)
                {
                    DEBUGL1("[%s]unable to bind fileshare in Network: failed to get data from command\n", __FUNCTION__);
                	return false;
                }
                DEBUGL5("[%s]: BeginTransaction with eWRITE is called for configDoc\n",__FUNCTION__);
                hdb->BeginTransaction(m_configDoc,eWRITE,true);

                configNode->setTextContent(csValue.c_str());
				DEBUGL5("[%s]: EndTransaction with eWRITE was called for configDoc\n",__FUNCTION__);
                hdb->EndTransaction(m_configDoc,eWRITE);
               

                if (!csConfPath.empty())
                    SavetoConfig(csConfPath, csValue);
				DEBUGL4("[%s] function exit\n",__FUNCTION__);
                return true;      
            }

            CString CSmbNMO::executeSSDCommand (CUICCommandRef command)
            {
				DEBUGL4("[%s] function enter\n",__FUNCTION__);
                bool bStatus = false;
                NodeRef pSMBNode  = command->GetCommandRoot();
                if(!pSMBNode)
                {
                    DEBUGL1("CSmbNMO::executeSSDCommand, unable to get root node of command returning\n");
					return "STATUS_FAILED";
                }

                NodeRef pCommandNodeNode = m_hdb->BindToElement(pSMBNode,COMMAND_NODE);
                if (!pCommandNodeNode)
                {
                    DEBUGL1("CSmbNMO::executeSSDCommand,unable to bind pCommandNodeNode: failed to get data from command\n");					
                    return "STATUS_FAILED";
                }

                CString docNameValue = ((ElementRef)pCommandNodeNode)->getAttribute(DOC_NAME);
                if (docNameValue.empty())
                {
                    DEBUGL1("[%s]unable to get document Name from Command\n", __FUNCTION__);
                    return "STATUS_FAILED";
                }
                if("InstallHdd" == command->GetCommandName())
                {
                    bStatus =  setSSCCommandPath (docNameValue, "enabled",             "",                     "1");
                    bStatus &= setSSCCommandPath (docNameValue, "fileshareEnabled",    "FILE_SHARE/available", "1");
                    bStatus &= setSSCCommandPath (docNameValue, "printingEnabled",     "Print/available",      "1");
                    if (!bStatus)
                        DEBUGL1("[%s]unable to set ConfigNode or configFile\n", __FUNCTION__);
                }

                if("RemoveHdd" == command->GetCommandName())
                {
                    bStatus =  setSSCCommandPath (docNameValue, "enabled",             "",                     "1");
                    bStatus &= setSSCCommandPath (docNameValue, "fileshareEnabled",    "FILE_SHARE/available", "0");
                    bStatus &= setSSCCommandPath (docNameValue, "printingEnabled",     "Print/available",      "1");
                    if (!bStatus)
                        DEBUGL1("[%s]unable to set ConfigNode or configFile\n", __FUNCTION__);
                }
				DEBUGL4("[%s] function exit\n",__FUNCTION__);
                return "STATUS_OK";
            }

            CString CSmbNMO::HandleUICCommand(CUICCommandRef command) 
            {
                DEBUGL4("[%s] function enter\n",__FUNCTION__);
				bool bDriverUpload  = false ;
                if("UploadPnP" == command->GetCommandName())
                {
                    //Get the driver uploaded path from FSM Dom
                    ci::operatingenvironment::Ref<ci::hierarchicaldb::HierarchicalDB> pHDB = HierarchicalDB::Acquire(NULL);
                    if(!pHDB)
                    {
                        DEBUGL1("HandleUICCommand:Failed to acquire hdb\n");
                        return "STATUS_FAILED";	
                    }
                    if (!m_pBO)
                    {
                        DEBUGL1("HandleUICCommands::-->No boproxy reference\n");
                        return "STATUS_FAILED";
                    }
                    NodeRef pNode = NULL;
                    CString path;

                    NodeRef fileNode = command->GetParamNode("source/File");
                    if(!fileNode)
                    {
                        DEBUGL1("HandleUICCommand:Failed to bind to node\n");
                        return "STATUS_FAILED";
                    }
                    //Fetching driver file path from FSM DOM
                    CString xpath = FILESTORAGEPATH;
                    //Get the value from XPath
                    m_pBO->GetValue(xpath.c_str(),pNode);
                    if (!pNode)
                    {
                        DEBUGL1("HandleUICCommands::-->Fetch value for xpath ->%s failed\n",xpath.c_str());
                        return "STATUS_FAILED";
                    }
                    path = pNode->getNodeValue();
                    if(path.empty())
                    {
                        DEBUGL1("HandleUICCommands::-->Fetch value for xpath ->%s failed\n",xpath.c_str());
                        return "STATUS_FAILED";
                    }

                    //Get all the driver file names from command payload
                    ElementRef parentNode = fileNode->getParentNode();
                    CString fileName;
                    if(parentNode)
                    {
                        CString nodeName = fileNode->getNodeName();
                        DEBUGL5("HandleUICCommand: Node = %s\n",nodeName.c_str());
                        NodeListRef nodeList = parentNode->getElementsByTagName(nodeName.c_str());
                        if (0 == nodeList->getLength())
                        {
                            DEBUGL1("[%s] Unable to get the list of file name from command Node\n",__FUNCTION__);
							return "STATUS_FAILED";
                        }
                        for(unsigned i=0;i<(nodeList->getLength());i++)
                        {
                            fileNode = nodeList->item(i);
                            if(fileNode)
                            {
                                fileName = fileNode->getTextContent();
                                //form fully qualified driver path
                                CString driverURI = path + "/" + fileName;
                                DEBUGL5("HandleUICCommand:Driver URI = %s\n",driverURI.c_str());
                                if (STATUS_FAILED == InstallPrinterDriver(driverURI, fileName))
                                {
                                    DEBUGL1("HandleUICCommand:Driver URI = %s failed!!!\n",driverURI.c_str());
                                    bDriverUpload = true ;
                                }
                            }
                            else
                            {
                                DEBUGL1("[%s] Unable to get driver name from command Node [%d]\n",__FUNCTION__,i);
                                bDriverUpload = true ;
                            }
                        }
                    }
                    else
                    { 
                        DEBUGL1("[%s]HandleUICCommand:Couldnt fetch the fileName\n",__FUNCTION__);
                        return "STATUS_FAILED";
                    }
                    if (true == bDriverUpload){	
                        DEBUGL1("[%s] Unable to get driver name from command Node\n",__FUNCTION__);	
                        return "STATUS_FAILED";
                    }
                }

                else if(("InstallHdd" == command->GetCommandName())  || ("RemoveHdd" == command->GetCommandName())){
					DEBUGL4("[%s] function exit\n",__FUNCTION__);
                    return executeSSDCommand (command);
                }
                else{
                    DEBUGL5("HandleUICCommands::Cannot handle the command\n");
                    return "STATUS_OK";
                }
				DEBUGL4("[%s] function exit\n",__FUNCTION__);
                return "STATUS_OK";
            }

            ElementRef CSmbNMO::GetDocNode(CString cPath,CString cSMBxPath)
            {
            	DEBUGL4("[%s] function enter\n",__FUNCTION__);
			    HierarchicalDBRef hdb = HierarchicalDB::Acquire(0);
                Status stat = STATUS_FAILED;
                ElementRef pElementNode = NULL;
                DocumentRef pDocNode = NULL;

                /** samba xPath starting from Network*/
                stat = m_hdb->OpenDocument(cPath, pDocNode);
                m_configDoc = pDocNode;
                DEBUGL5("[%s]: BeginTransaction with eWRITE is called for configDoc\n",__FUNCTION__);
                hdb->BeginTransaction(m_configDoc,eWRITE,true);
                if (STATUS_OK == stat)
                {
                    pElementNode = m_hdb->BindToElement(pDocNode, cSMBxPath);
                    if (!pElementNode)
                    {
                        DEBUGL1("CSmbNMO::GetDocNode, unable to bind [%s] in trans DOM returning\n",cSMBxPath.c_str());
						DEBUGL5("[%s]: EndTransaction with eWRITE was called for configDoc\n",__FUNCTION__);
                        hdb->EndTransaction(m_configDoc,eWRITE);
                        return (NULL);
                    }
                    else{				
                        DEBUGL5("[%s]: returning pElementNode EndTransaction with eWRITE was called for configDoc\n",__FUNCTION__);
                        hdb->EndTransaction(m_configDoc,eWRITE);
                        return(pElementNode);
                    }
                }
                else
                {
                    DEBUGL1("unable to open trans DOM returning\n");
   				    DEBUGL5("[%s]: EndTransaction with eWRITE was called for configDoc\n", __FUNCTION__);	
                    hdb->EndTransaction(m_configDoc,eWRITE);                   
                    return (NULL);
                }
                DEBUGL5("[%s]: EndTransaction with eWRITE was called for configDoc\n",__FUNCTION__);
                hdb->EndTransaction(m_configDoc,eWRITE);
				DEBUGL4("[%s] function exit\n",__FUNCTION__);
                return (NULL);
            }


            Status CSmbNMO::UpdateMFPDOM(CString xPath, CString val)
            {
                DEBUGL4("[%s] function enter\n",__FUNCTION__);
				CString confPath;


                if(xPath == PEDESTAL1_XPATH ) {
                    val = CSmbNMO::getDrawers();
                    SavetoConfig("global/Pedestal", val);
                }
                else if(xPath == LCF_XPATH ){
                    val = CSmbNMO::getLCF();
                    SavetoConfig("global/Pedestal", val);
                }
                else if( xPath == FINISHER_XPATH){
                    val = CSmbNMO::getFinishers();
                    SavetoConfig("global/FinisherUnit", val);
                }
                else if ( xPath == HOLEPUNCH_XPATH ){
                    val = CSmbNMO::getHolePuncher();
                    SavetoConfig("global/PunchUnit", val);
                }

                val = CSmbNMO::mapValue(val);
                confPath=CSmbNMO::mapXPath(xPath);
				DEBUGL4("[%s] function exit\n",__FUNCTION__);
                return (SavetoConfig(confPath, val));
            }

            Status CSmbNMO::SavetoConfig(CString cPath, CString mapVal)
            {
                DEBUGL4("[%s] function enter\n",__FUNCTION__);
				CIniFile ini(m_confFile);
                if (!ini){
					DEBUGL1("CSmbNMO::SavetoConfig, Config file not found\n");
 					return STATUS_FAILED;
				}

                ElementRef v ;

                if(cPath!="")
                    DEBUGL5("SavetoConfig:Config Path = %s\n", cPath.c_str());
                else{
					DEBUGL1("CSmbNMO::SavetoConfig, Config file Path not found\n");
                    return STATUS_FAILED;
				}

                DEBUGL5("SavetoConfig:Mapped Value [%s]= %s\n",cPath.c_str(), mapVal.c_str());
                v = chelper::NavigatePath(cPath, ini.GetRoot(), true);

                if(!v){
                    DEBUGL1("SavetoConfig:NavigatePath function fail[%s]\n",cPath.c_str());
                    return STATUS_FAILED;
                }

                if(mapVal != "")
                    DEBUGL5("SavetoConfig:Mapped Value = %s\n", mapVal.c_str());
                else{
					DEBUGL1("CSmbNMO::SavetoConfig, mapped value not found\n");
                    return STATUS_FAILED;
				}

                v->setTextContent(mapVal);
                ini.SetModified();
                ini.Save();
				DEBUGL4("[%s] function exit\n",__FUNCTION__);
                return STATUS_OK;
            }

            void CSmbNMO::initXPathMapping(void)
            {
                DEBUGL4("[%s] function enter\n",__FUNCTION__);
				xPath_config_map["MFP/Printer/PaperFeeder/Drawer1/PaperSize"]="global/UpperPaper";
                xPath_config_map["MFP/Printer/PaperFeeder/Drawer2/PaperSize"]="global/LowerPaper";
                xPath_config_map["MFP/Printer/PaperFeeder/Drawer3/PaperSize"]="global/Pedestal1Paper";
                xPath_config_map["MFP/Printer/PaperFeeder/Drawer4/PaperSize"]="global/Pedestal2Paper";
                xPath_config_map["MFP/Printer/PaperFeeder/LCF/PaperSize"]="global/LCFPaper";
                xPath_config_map["MFP/Printer/PaperFeeder/ExternalLCF/PaperSize"]="global/ExternalLCFPaper";

                xPath_config_map["MFP/Printer/PaperFeeder/Drawer1/MediaType/PaperType"]="global/UpperMedia";
                xPath_config_map["MFP/Printer/PaperFeeder/Drawer2/MediaType/PaperType"]="global/LowerMedia";
                xPath_config_map["MFP/Printer/PaperFeeder/Drawer3/MediaType/PaperType"]="global/Pedestal1Media";
                xPath_config_map["MFP/Printer/PaperFeeder/Drawer4/MediaType/PaperType"]="global/Pedestal2Media";
                xPath_config_map["MFP/Printer/PaperFeeder/LCF/MediaType/PaperType"]="global/LCFMedia";
                xPath_config_map["MFP/Printer/PaperFeeder/ExternalLCF/MediaType/PaperType"]="global/ExternalLCFMedia";
                xPath_config_map["MFP/Printer/PaperFeeder/Drawer1/Attribute"] = "global/UpperInputKind";
                xPath_config_map["MFP/Printer/PaperFeeder/Drawer2/Attribute"] = "global/LowerInputKind";
                xPath_config_map["MFP/Printer/PaperFeeder/Drawer3/Attribute"] = "global/Pedestal1InputKind";
                xPath_config_map["MFP/Printer/PaperFeeder/Drawer4/Attribute"] = "global/Pedestal2InputKind";
                xPath_config_map["MFP/Printer/PaperFeeder/LCF/Attribute"] = "global/LCFInputKind";
                xPath_config_map["MFP/Printer/PaperFeeder/ExternalLCF/Attribute"] = "global/ExternalLCFInputKind";

                xPath_config_map[PEDESTAL1_XPATH] = "global/Pedestal";
                xPath_config_map[PEDESTAL2_XPATH] = "global/Pedestal";
                xPath_config_map[LCF_XPATH] = "global/Pedestal";
                xPath_config_map[FINISHER_XPATH] = "global/FinisherUnit";
                xPath_config_map[FINISHER_NAME] = "global/FinisherUnit";
                xPath_config_map[HOLEPUNCH_XPATH] = "global/PunchUnit";
                xPath_config_map[HOLEPUNCH_NAME] = "global/PunchUnit";
				DEBUGL4("[%s] function exit\n",__FUNCTION__);
            }

            CString CSmbNMO::mapXPath(CString xPath)
            {
                DEBUGL4("[%s] function enter\n",__FUNCTION__);
				map<string,string>::iterator iter = xPath_config_map.find(xPath);
                if (iter == xPath_config_map.end()) {
                    DEBUGL2("mapXPath:%s String Not found in the Map Table\n", xPath.c_str());
                    return "";
                }
				DEBUGL4("[%s] function exit\n",__FUNCTION__);
                return iter->second;
            }

            void CSmbNMO::initValueMapping()
            {
            	DEBUGL4("[%s] function enter\n",__FUNCTION__);
			    xPath_Value_map["Installed"] = "1";
                xPath_Value_map["NotInstalled"] = "-998";
                //Paper Size Values follows
                xPath_Value_map["LETTER"] = "1";
                xPath_Value_map["TABLOID"] = "3";
                xPath_Value_map["LEDGER"] = "4";
                xPath_Value_map["LEDGER-R"] = "4";
                xPath_Value_map["LEGAL"] = "5";
                xPath_Value_map["LEGAL-R"] = "5";
                xPath_Value_map["STATEMENT-R"] = "6";
                xPath_Value_map["A3"] = "8";
                xPath_Value_map["A4"] = "9";
                xPath_Value_map["A5"] = "11";
                xPath_Value_map["A5-R"] = "11";
                xPath_Value_map["B4"] = "12";
                xPath_Value_map["B4-R"] = "12";
                xPath_Value_map["B5"] = "13";
                xPath_Value_map["FOLIO-R"] = "60";
                xPath_Value_map["USER"] = "256";
                xPath_Value_map["COMPUTER-R"] = "258";
                xPath_Value_map["A3-R"] = "259";
                xPath_Value_map["13inchLEGAL-R"] = "261";
                xPath_Value_map["8.5SQ"] = "262";
                xPath_Value_map["8K-R"] = "263";
                xPath_Value_map["16K"] = "264";
                xPath_Value_map["LETTERTAB_OTH"] = "266";
                xPath_Value_map["A4TAB_OTH"] = "267";
                xPath_Value_map["LDWR"] = "269";
                xPath_Value_map["LETTER-R"] = "270";
                xPath_Value_map["A4-R"] = "271";
                xPath_Value_map["B5-R"] = "272";
                xPath_Value_map["16K-R"] = "273";
                xPath_Value_map["SRA3_450R"] = "276";
                xPath_Value_map["SRA3_460R"] = "277";
                xPath_Value_map["SELECT"] = "512";
                xPath_Value_map["LETTERTAB_PCL5C"] = "514";
                xPath_Value_map["A4TAB_PCL5C"] = "538";

                //Paper Type Value Follows

                xPath_Value_map["Plain"] = "1";
                xPath_Value_map["Thick1"] = "2";
                xPath_Value_map["Thick2"] = "3";
                xPath_Value_map["Thick3"] = "5";
                xPath_Value_map["Recycled"] = "11";

                //Paper Attribute Follows

                xPath_Value_map["Normal"] = "3";
                xPath_Value_map["Cover"] = "5";
                xPath_Value_map["Insert"] = "6";
                xPath_Value_map["Fax"] = "8";
                xPath_Value_map["Special"] = "9";

                //Finisher Value Mapping Follows

                xPath_Value_map["Console"] = "2";
                xPath_Value_map["SaddleStitchFinisher"]="0";
                xPath_Value_map["ConsoleOneBin"]="6";
                xPath_Value_map["HangingFinisher"]= "3";

                xPath_Value_map["Console1_Mash"]= "5";
                xPath_Value_map["Console2_Mash"]= "5";
                xPath_Value_map["Console1_BP"]= "4";
                xPath_Value_map["Console2_BP"]= "4";
                xPath_Value_map["Console1_Loire"]= "6";
                xPath_Value_map["Console2_Loire"]= "6";
                xPath_Value_map["Console1_Alabama"]= "7";
                xPath_Value_map["Console2_Alabama"]= "7";

                //HolePuncher Name Mapping Follows

                xPath_Value_map["2Holes"] = "1";
                xPath_Value_map["23Holes"] = "2";
                xPath_Value_map["4HolesF"] = "3";
                xPath_Value_map["4HolesS"] = "4";
				DEBUGL4("[%s] function exit\n",__FUNCTION__);
            }
            /*
             * Return mapped value from the "xPath_Value_map" table defined, if no match found return empty string
             */
            CString CSmbNMO::mapValue(CString value)
            {
				DEBUGL4("mapValue: function enter Value to Map %s\n", value.c_str());
                map<string,string>::iterator iter = xPath_Value_map.find(value);
                if (iter == xPath_Value_map.end()) {
                    DEBUGL2("mapValue:%s String Not found in Value Map Table\n", value.c_str());
                    return "";
                }
				DEBUGL4("[%s] function exit\n",__FUNCTION__);
                return iter->second;
            }

            /*
             * Return true if the status is "Installed" for the given xPath else false
             */

            bool CSmbNMO::isInstalled(CString xPath)
            {
                DEBUGL4("[%s] function enter\n",__FUNCTION__);
				NodeRef pNode;
                CString sessionid="";
                BOProxyRef bop = BOProxy::Acquire(sessionid);
                if(!bop){
                    DEBUGL1("isInstalled:BOProxy::Acquire function fail\n");					
                    return 0;
                }
                CString szNodeValue;
                bop->GetValue(xPath.c_str(),pNode);
                if(!pNode){
                    DEBUGL1("Get Value failed \n");
                    return 0;
                }

                szNodeValue = pNode->getNodeValue();
                DEBUGL5("isInstalled:Value for %s= %s\n",xPath.c_str(), szNodeValue.c_str());
                if (szNodeValue == "Installed")
                {
                	DEBUGL4("[%s] function exit\n",__FUNCTION__);
					return (1);
                }
                else
                {
                	DEBUGL4("[%s] function exit\n",__FUNCTION__);
					return (0);
                }

            }
            /*
             * Return installed pedestal or "0"
             */
            CString CSmbNMO::getDrawers(void)
            {
                if(isInstalled(PEDESTAL1_XPATH) && isInstalled(PEDESTAL2_XPATH))
                    return PEDESTAL_PEDESTAL2;
                else if(isInstalled(PEDESTAL1_XPATH) && !(isInstalled(PEDESTAL2_XPATH)) )
                    return PEDESTAL_PEDESTAL1;
                else if(!(isInstalled(PEDESTAL1_XPATH)) && isInstalled(PEDESTAL2_XPATH))
                    return PEDESTAL_PEDESTAL2;
                else {
                    return PEDESTAL_NONE;
                }
            }
            /*
             * Return if the LCF is installed or empty string
             */

            CString CSmbNMO::getLCF(void)
            {
                if(isInstalled(LCF_XPATH))
                    return PEDESTAL_LCF;
                else {
                    return "";
                }
            }

            /*
             * Get the Name of Finisher installed
             * PS: Not all the finishers are implemented now
             */
            CString CSmbNMO::getFinishers(void)
            {
                DEBUGL4("[%s] function enter\n",__FUNCTION__);
				if(!(isInstalled(FINISHER_XPATH))){
					DEBUGL4("[%s] function exit\n",__FUNCTION__);
                    return FINISHER_NONE;
				}

                NodeRef pNode;
                CString sessionid="";
                BOProxyRef bop = BOProxy::Acquire(sessionid);
                if(!bop){
                    DEBUGL1("getFinishers:BOProxy::Acquire function fail\n");
				   return 0;
                }
                CString szFinisherValue, szSaddleStitch, szOutputTray, szOutputOffsetStacking, szNodeValue;
                bop->GetValue(FINISHER_NAME,pNode);
                if (!pNode){
                    DEBUGL1("getFinishers: Get Value failed \n");
				  return 0;       
				}

                szFinisherValue = pNode->getNodeValue();
                char product[75];

                strcpy(product, "MASH"); //Default value
                size_t product_size = strlen(getenv("PRODUCT"));
                strncpy(product, getenv("PRODUCT"),(product_size+1));
                if(szFinisherValue == "HangingFinisher") szNodeValue="HangingFinisher";
                else if (isInstalled(SADDLESTITCH_XPATH)) szNodeValue="SaddleStitch";

                if(!strcmp(product , "MASH")){
                    if((szFinisherValue == "Console1")  ) szNodeValue = "Console1_Mash";
                    else if((szFinisherValue == "Console2") ) szNodeValue = "Console2_Mash";
                }
                else if (!strcmp(product, "BP")) { 
                    if((szFinisherValue == "Console1")  ) szNodeValue = "Console1_BP";
                    else if((szFinisherValue == "Console2") ) szNodeValue = "Console2_BP";
                }         
                else if (!strcmp(product, "LOIRE")) {
                    if((szFinisherValue == "Console1")  ) szNodeValue = "Console1_Loire";
                    else if((szFinisherValue == "Console2") ) szNodeValue = "Console2_Loire";
                }
                else if (!strcmp(product, "ALABAMA")) {
                    if((szFinisherValue == "Console1")  ) szNodeValue = "Console1_Alabama";
                    else if((szFinisherValue == "Console2") ) szNodeValue = "Console2_Alabama";
                } 
                else if (szFinisherValue == "Console") szNodeValue="Console";

                DEBUGL5("getFinishers:Value for %s= %s\n",FINISHER_NAME, szNodeValue.c_str());
                if ( mapValue(szFinisherValue) == "" ) 
					szNodeValue="Finisher";
				DEBUGL4("[%s] function exit\n",__FUNCTION__);
                return (mapValue(szNodeValue));

            }
            CString CSmbNMO::getHolePuncher(void)
            {
                DEBUGL4("[%s] function enter\n",__FUNCTION__);
				if(!(isInstalled(HOLEPUNCH_XPATH))){
					 return HOLEPUNCH_NONE;
				}

                NodeRef pNode;
                CString sessionid="";
                BOProxyRef bop = BOProxy::Acquire(sessionid);
                if(!bop){
                    DEBUGL1("getHolePuncher:BOProxy::Acquire function fail\n");
					return CString("");
                }
                CString szNodeValue;
                bop->GetValue(HOLEPUNCH_NAME,pNode);
                if(!pNode) {
                    DEBUGL1("getHolePuncher::get value failed\n");
                    return CString("");
                }
                szNodeValue = pNode->getNodeValue();
                DEBUGL4("getHolePuncher: function exit Value for %s= %s\n",HOLEPUNCH_NAME, szNodeValue.c_str());
                return (mapValue(szNodeValue));	
            }

            Status CSmbNMO::CheckLinemode(int &value)
            {
				DEBUGL4("[%s] function enter\n",__FUNCTION__);
                Ref<IndexedDB> idb = IndexedDB::Acquire();
                if (idb)
                {
                    uint64 key = IndexedDB::CreateKey(5000, 0, 0);
                    value = idb->GetIntValue(key, -1);
                }
                else{
					DEBUGL1("[%s] Acquire() IndexedDB failed \n",__FUNCTION__);
                    return (STATUS_FAILED);
				}

                DEBUGL4("SMB NMO::CheckLinemode:function exit value is %d\n", value);
                return (STATUS_OK);
            }

	/*20160614: Validate smb.conf file during bootup*/
            Status CSmbNMO::ValidateConfOnBootup(void)
            {
                DEBUGL4("[%s] function enter\n",__FUNCTION__);
                CString path= "/encryption/al/network/config/smb.conf";
                ifstream in(path.c_str());
                /*check if smb.conf file present*/
                if(!in) {
                    DEBUGL1("CSmbNMO::ValidateConfOnBootup() unable to find smb.conf file, copy default conf file\n");
                    return STATUS_FAILED;
                }
                struct stat st;
                if ((stat(path.c_str(), &st) == 0) && (!st.st_size))  {
                    DEBUGL1("CSmbNMO::ValidateConfOnBootup()size of smb.conf is %d, copy default conf file\n",st.st_size);
                    return STATUS_FAILED;
                }

                CString line;
                unsigned int linecnt = 0;
                while (getline(in, line)) {
                    linecnt++;
                    size_t lbrack = line.size(), rbrack, nstart, nend, vstart, vend, eq, lquote, rquote, comment, iLineLength;
                    rbrack = nstart = nend = eq = vstart = vend = lquote = rquote = comment = iLineLength = lbrack;

                    bool error = false;
                    bool escapeChar = false;
                    /*Exception of length for hide files parameter, in case of valid smb.conf file*/
                    CString csReadline=line;
                    size_t linepos = 0;
                    linepos=csReadline.find("=");
                    CString csHideFiles(csReadline,0,linepos);
                    if("hide files" == csHideFiles)
                        linepos = 1;

                    for (size_t pos = 0; pos < comment && !error; pos++) {
                        // spaces are always ok.
                        if (isspace(line[pos]))
                            ;
                        // at start of line, only want a name, left-bracket, or comment.
                        else if ((nstart > pos) && (!isalpha(line[pos])) &&
                                (!strchr("[#;", line[pos])))
                            error = true;
                        /* once we've started getting the name, we want only valid name
                         * chars or a name-terminator (right-bracket, equals, or comment) or special characters such as @,/.
                         * char ' (single quote) also consider as valid character in name.
                         */
                        else if (nstart < pos && nend > pos &&
                                (!isalnum(line[pos]) && line[pos] != '.' ) && (!escapeChar && !strchr("\\-_=];#/@'", line[pos])))
                            error = true;
                        // after right-bracket, only a comment is ok.
                        else if ((rbrack < pos) && line[pos] != '#' && line[pos] != ';')
                            error = true;
                        else if((iLineLength > 160) && (1 != linepos)) //domain and other parameters may contain 128 chars
                            error = true;
                        else{
                            // now we do character-specific state transitions
                            switch (line[pos]) {
                                case '#': case ';':
                                    comment = pos;
                                    break;
                                case '\\':
                                    escapeChar = true;
                                    break;
                                case '[':
                                    if(! escapeChar)
                                    {
                                        if (lbrack > pos && nstart > pos)
                                            lbrack = pos;
                                        if (eq < pos && vstart > pos)
                                            vstart = pos;
                                    }
                                    else
                                        escapeChar = false;
                                    break;
                                case ']':
                                    if(! escapeChar)
                                    {
                                        if (lbrack < pos && nstart < pos)
                                            rbrack = nend = pos;
                                        if (eq < pos && vstart > pos)
                                            vstart = pos;
                                    }
                                    else
                                        escapeChar = false;
                                    break;
                                case '=':
                                    if( ! escapeChar)
                                    {
                                        if (lbrack > pos && nstart < pos)
                                            eq = nend = pos;
                                        else
                                            error = true;
                                    }
                                    else
                                        escapeChar = false;
                                    break;
                                default:
                                    if (nstart > pos)
                                        nstart = pos;
                                    else if (eq < pos && vstart > pos)
                                        vstart = pos;
                                    break;
                            }
                        }
                    }

                    if (lbrack < comment && rbrack >= comment)
                        error = true;

                    if (error)
                    {
                        DEBUGL1("CSmbNmo::ValidateConfOnBootup(), file '%s' has invalid line %u='%s'\n",
                                path.c_str(), linecnt, line.c_str());
                        return STATUS_FAILED;
                    }
                }
                DEBUGL4("[%s] function exit\n",__FUNCTION__);
                return STATUS_OK;
            }
        }
    }
}
