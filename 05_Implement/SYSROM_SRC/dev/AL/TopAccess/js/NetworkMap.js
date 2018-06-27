  /** <pre>
    *-------------------------------------------------------------------------------

    *

    *-------------------------------------------------------------------------------
    * File: NetworkMap.js
    *-------------------------------------------------------------------------------
    * DESCRIPTION:
    * This file contains the widget xpath for Device, General, Network, Copier, 
     * Email, InternetFax, and PrintService.
    *
    * AUTHOR:       Kalyan,Sangeetha,Tarkeshwar and DurgaPrasad
    * PROJECT:		[TOPACCESS]
    *
    * Date Of Creation	: 30/AUG/2007
    * Version No.		: 1.0
    *-------------------------------------------------------------------------------

    * DEPENDENCIES
    *
    *-------------------------------------------------------------------------------
    * </pre>

    */

       /******************** Netork setting Xpath Widget *************************/

        var Network_Xpath_Widget_HashTable = {
          "Network/Adapters/Wire/speed"				                            : "TCPIP_ether",

          "Network/Protocols/TCP-IP/addressMode"		                        : "TCPIP_AddressMode" ,
          "Network/Protocols/TCP-IP/DHCP/autoDDNSUpdate"			            : "TCPIP_DomainName" ,
          "Network/Protocols/TCP-IP/ip"					                        : "TCPIP_IPAddress" ,
          "Network/Protocols/TCP-IP/subnet"				                        : "TCPIP_SubnetMask" ,
          "Network/Protocols/TCP-IP/gw"					                        : "TCPIP_DefaultGateway",

          "Network/Protocols/TCP-IP/DHCP/autoDNSUpdate"                         : "TCPIP_DomainServerAddress",
          "Network/Protocols/TCP-IP/DHCP/autoWINSUpdate"                        : "TCPIP_WINS",
          "Network/Protocols/TCP-IP/DHCP/autoSMTPUpdate"                        : "TCPIP_SMTP",
          "Network/Protocols/TCP-IP/DHCP/autoPOP3Update"                        : "TCPIP_POP3",
          "Network/Protocols/TCP-IP/DHCP/autoSNTPUpdate"                        : "TCPIP_SNTP",
          "Network/Protocols/TCP-IP/enableIPConflictDetect"                     : "TCPIP_IPConflictDetect",
          "Network/Protocols/TCP-IP/hostName"                                   : "TCPIP_HostName" ,

          "Network/Services/LLMNR/enabled"                                      : "IPv6_LLMNR",

          "Network/Protocols/IPX-SPX/enabled"			                        : "IPXSPX_Enable" ,
          "Network/Protocols/IPX-SPX/frameType"	                                : "IPXSPX_FrameType" ,
		  "Network/Protocols/IPX-SPX/actualFrameType"	                        : "IPXSPX_ActualFrameType" ,

          "Network/Protocols/Appletalk/enabled"			                        : "APPLETALK_Enable" ,
          "Network/Protocols/Appletalk/deviceName"		                        : "APPLETALK_DeviceName",
          "Network/Protocols/Appletalk/zone"			                        : "APPLETALK_Zone",

          "Network/Services/Bonjour/enabled"		                            : "BONJOUR_Enable",
          "Network/Services/Bonjour/hostName"			                        : "BONJOUR_HostName" ,
          "Network/Services/Bonjour/serviceName"		                        : "BONJOUR_ServiceName" ,

          "Network/Services/LDAP/enabled"				                        : "LDAP_Enable" ,
          "Network/Services/LDAP/attribute1"                                    : "LDAP_Attribute1",
          "Network/Services/LDAP/attribute2"                                    : "LDAP_Attribute2",
          "Network/Services/LDAP/searchMethod"                                  : "LDAP_SearchMethod",

          "Network/Protocols/DNS/enabled"                                       : "DNS_Enable",
          "Network/Protocols/DNS/primaryServer"                                 : "DNS_Primary",
          "Network/Protocols/DNS/secondaryServer"                               : "DNS_Secondary",

          "Network/Protocols/DDNS/enabled"                                      : "DDNS_Enable" ,
          "Network/Protocols/DDNS/domainName"                                   : "DDNS_DomainName" ,
          "Network/Protocols/DDNS/primaryLoginName"                             : "DDNS_PrimaryLoginName",
          "Network/Protocols/DDNS/primaryPassword"                              : "DDNSPri_Password",
          "Network/Protocols/DDNS/secondaryLoginName"                           : "DDNS_SecondaryLoginName",
          "Network/Protocols/DDNS/secondaryPassword"                            : "DDNSSec_Password",
          "Network/Protocols/DDNS/securityMethod"                               : "DDNS_SecurityMethod",

          "Network/Protocols/SMB/enabled"			                            : "SMB_EnableXpath" ,
          "Network/Protocols/SMB/printingEnabled"			                    : "SMB_Print_Enable" ,
          "Network/Protocols/SMB/fileshareEnabled"			                    : "SMB_FileShare_Enable" ,
          "Network/Protocols/SMB/domainName"                                    : "SMB_Domain",
          "Network/Protocols/SMB/primaryDomainController"                       : "SMB_Primary",
          "Network/Protocols/SMB/backupDomainController"                        : "SMB_Controller",
          "Network/Protocols/SMB/logonUser"                                     : "SMB_UserName",
          "Network/Protocols/SMB/password"                                      : "SMB_Pwd",
          "Network/Protocols/SMB/digitalSigningSMBServer"                       : "SMB_Server",
          "Network/Protocols/SMB/digitalSigningSMBClient"                       : "SMB_Client",
          "Network/Protocols/SMB/deviceName"			                        : "SMB_NetBIOSName" ,
          "Network/Protocols/SMB/groupName"				                        : "SMB_Workgroup" ,
          "Network/Protocols/SMB/wins"					                        : "SMB_PrimaryWins" ,
          "Network/Protocols/SMB/wins2"					                        : "SMB_SecondaryWins" ,
          "Network/Protocols/SMB/serverAuthGuestLogon"                  : "SMB_GuestLogon",
          "Network/Protocols/SMB/serverAuthUser"                        : "SMB_AuthUserName",
          "Network/Protocols/SMB/serverAuthPassword"                    : "SMB_Password",

          "Network/Services/NetWare/Environment/NDS"                            : "NETWARE_NDS" ,
          "Network/Services/NetWare/enabled"                                    : "NETWARE_Enable" ,
		  "Network/Services/NetWare/Environment/bindery"			            : "NETWARE_Bindery" ,
          "Network/Services/NetWare/context"			                        : "NETWARE_Context" ,
          "Network/Services/NetWare/tree"				                        : "NETWARE_tree" ,
          "Network/Services/NetWare/searchRoot"			                        : "NETWARE_SearchRoot" ,

          "Network/Services/HTTPServer/enabled"			                        : "HTTP_Enabled" ,
          "Network/Services/HTTPServer/SSLEnabled"		                        : "HTTP_SSLEnabled" ,
          "Network/Services/HTTPServer/primaryPortNo"	                        : "HTTP_PrimaryPort" ,
          "Network/Services/HTTPServer/secondaryPortNo"	                        : "HTTP_SecondaryPort" ,
          "Network/Services/HTTPServer/SSLPortNo"                               : "HTTP_SSLPort",

          "Network/Protocols/SMTP/Client/enabled"		                        : "SMTPClient_Enable" ,
          "Network/Protocols/SMTP/Client/enableSSL"                             : "SMTPClient_EnableSSL",
          "Network/Protocols/SMTP/Client/sslProtocol"                           : "SMTPClient_SSLTLS",
          "Network/Protocols/SMTP/Client/server"		                        : "SMTPClient_ServerAddress" ,
          "Network/Protocols/SMTP/Client/popBeforeSMTP"                         : "SMTPClient_POPSMTP",
          "Network/Protocols/SMTP/Client/Authentication/desiredType"            : "SMTPClient_Authentication" ,
          "Network/Protocols/SMTP/Client/Authentication/userName"	            : "SMTPClient_LoginName" ,
          "Network/Protocols/SMTP/Client/Authentication/password"	            : "SMTPClient_Password" ,
          "Network/Protocols/SMTP/Client/port"			                        : "SMTPClient_PortNum" ,
          "Network/Protocols/SMTP/Client/connectionTimeOut"			            : "SMTPClient_TimeOut" ,

          "Network/Protocols/SMTP/Server/enabled"					            : "SMTPServer_Enable" ,
          "Network/Protocols/SMTP/Server/portNo"                                : "SMTPServer_PortNum" ,
          "Network/Protocols/SMTP/Server/emailAddress"			                : "SMTPServer_EmailAddress" ,
          "Network/Protocols/SMTP/Server/OffRamp/gwEnabled"		                : "SMTPServer_Gateway" ,
          "Network/Protocols/SMTP/Server/OffRamp/securityEnabled"	            : "SMTPServer_Security" ,
          "Network/Protocols/SMTP/Server/OffRamp/printEnabled"	                : "SMTPServer_Print" ,

          "Network/Protocols/POP3/enabled"						                : "POP3_Enable" ,
          "Network/Protocols/POP3/SSLEnabled"                                   : "POP3_SSL",
          "Network/Protocols/POP3/server"							            : "POP3_ServerAddress" ,
          "Network/Protocols/POP3/Authentication/loginType"		                : "POP3_LoginType" ,
          "Network/Protocols/POP3/Authentication/userName"		                : "POP3_AccountName" ,
          "Network/Protocols/POP3/Authentication/password"		                : "POP3_Password" ,
          "Network/Protocols/POP3/pollingRate"					                : "POP3_ScanRate" ,
          "Network/Protocols/POP3/port"							                : "POP3_PortNumber" ,
          "Network/Protocols/POP3/SSLPortNo"                                    : "POP3_SSLPortNum",
          "Network/Protocols/POP3/Authentication/authType"                      : "POP3_Auth",
          "Network/Protocols/POP3/connectionTimeOut"                            : "POP3_TimeOut",

          "Network/Protocols/FTPClient/portNo"                                  : "FTPCLIENT_PortNum",
          "Network/Protocols/FTPClient/SSLSetting"                              : "FTPCLIENT_SSLSetting",

          "Network/Services/FTPServer/enabled"                                  : "FTPSERVER_Enable",
		  "Network/Services/FTPServer/SSLEnabled"                               : "FTPSERVER_SSL",
          "Network/Services/FTPServer/portNo"                                   : "FTPSERVER_PortNum",
          "Network/Services/FTPServer/SSLPortNo"                                : "FTPSERVER_SSLPortNum",

          "Network/Protocols/SLP/enabled"                                       :  "SLPSession_Enable",
          "Network/Protocols/SLP/scope"                                         :  "SLPSession_Scope",
          "Network/Protocols/SLP/ttl"                                           :  "SLPSessionTTL_id",


          "Network/Protocols/SNMP/snmpV1V2Enable"						        : "SNMP_Enable" ,
          "Network/Protocols/SNMP/readCommunityName"                            : "SNMP_Community",
          "Network/Protocols/SNMP/writeCommunityName"                           : "SNMP_RWCommunity",
          // SNMP V3 Xpaths here..
          "Network/Protocols/SNMP/snmpV3Enabled"				                : "SNMP_EnableV3" ,

          "Network/Protocols/SNMP/snmpV3TrapEnabled"				            : "SNMP_EnableV3Trap" ,
          "Network/Protocols/SNMP/snmpV3TrapUserName"				            : "SNMP_V3TrapUserName" ,
          "Network/Protocols/SNMP/snmpV3TrapAuthenticationProtocol"				: "SNMP_V3TrapAuthProtocol" ,
          "Network/Protocols/SNMP/snmpV3TrapAuthenticationPassword"				: "SNMP_V3TRAPAUTHPWD" ,
          "Network/Protocols/SNMP/snmpV3TrapPrivacyProtocol"				    : "SNMP_V3TrapPrivacyProtocol" ,
          "Network/Protocols/SNMP/snmpV3TrapPrivacyPassword"				    : "SNMP_V3TRAPPRVPWD" ,

          "Network/Protocols/SNMP/authTrapEnabled"				                : "SNMP_EnableAuthTrap" ,
          "Network/Protocols/SNMP/alertTrapEnabled"				                : "SNMP_EnableAlertTrap" ,
          "Network/Protocols/SNMP/Trap[@index=0]/trapIPAddress"					: "SNMP_TrapIP1",
          "Network/Protocols/SNMP/Trap[@index=1]/trapIPAddress"					: "SNMP_TrapIP2",
          "Network/Protocols/SNMP/Trap[@index=2]/trapIPAddress"					: "SNMP_TrapIP3",
          "Network/Protocols/SNMP/Trap[@index=3]/trapIPAddress"					: "SNMP_TrapIP4",
          "Network/Protocols/SNMP/Trap[@index=4]/trapIPAddress"					: "SNMP_TrapIP5",
          "Network/Protocols/SNMP/Trap[@index=5]/trapIPAddress"					: "SNMP_TrapIP6",
          "Network/Protocols/SNMP/Trap[@index=6]/trapIPAddress"					: "SNMP_TrapIP7",
          "Network/Protocols/SNMP/Trap[@index=7]/trapIPAddress"					: "SNMP_TrapIP8",
          "Network/Protocols/SNMP/Trap[@index=8]/trapIPAddress"					: "SNMP_TrapIP9",
          "Network/Protocols/SNMP/Trap[@index=9]/trapIPAddress"					: "SNMP_TrapIP10",
          "Network/Protocols/SNMP/ipxTrapAddress"					            : "SNMP_IPXTrapAddress" ,
          "Network/Protocols/SNMP/ipTrapCommunityName"		    	            : "SNMP_IPTrapCommunity",

          "Network/Protocols/DPWS/friendlyName"                                 : "WEBSERVICE_friendlyName",
          "Network/Protocols/DPWS/printServiceStatus"                           : "WEBSERVICE_print",
          "Network/Protocols/DPWS/printerName"                                  : "WEBSERVICE_printerName",
          "Network/Protocols/DPWS/scanServiceStatus"                            : "WEBSERVICE_scan",
          "Network/Protocols/DPWS/scannerName"                                  : "WEBSERVICE_scannerName",
          "Network/Protocols/DPWS/printerInformation"                           : "WEBSERVICE_print_inf",
          "Network/Protocols/DPWS/scannerInformation"                           : "WEBSERVICE_scan_inf",
          "Network/Protocols/DPWS/SSLEnabled"                                   : "ENABLE_SSL",
          "Network/Protocols/DPWS/JobExecutionMode"                              : "WEBSERVICE_JobExecMode" ,

          "Network/Services/LLTD/enabled"                                       : "LLTD_enable",
          "Network/Services/LLTD/deviceName"                                    : "LLTD_deviceName" ,

            // Wake up setting
            "Network/Services/PowerManagement/PMWakeUpFilterSNMPv6"                              : "vr_SNMPIPV6" ,
            "Network/Services/PowerManagement/PMWakeUpFilterSNMPv4"                              : "vr_SNMPIPV4" ,
            "Network/Services/PowerManagement/PMWakeUpFilterSMB"                                 : "vr_SMB" ,
            "Network/Services/PowerManagement/PMWakeUpFilterNeighborDiscovery"                   : "vr_ND_LinkLocalAddr" ,
            "Network/Services/PowerManagement/PMWakeUpFilterNeighborDiscoveryForManual"          : "vr_ND_Manual" ,
            "Network/Services/PowerManagement/PMWakeUpFilterLLMNR"                               : "vr_LLMNR" ,
            "Network/Services/PowerManagement/PMWakeUpFilterLLTD"                                : "vr_LLTD" ,
            "Network/Services/PowerManagement/PMWakeUpFilterDPWSIPv6"                            : "vr_WebService_IPV6" ,
            "Network/Services/PowerManagement/PMWakeUpFilterBonjour"                             : "vr_Bonjor" ,
            "Network/Services/PowerManagement/PMWakeUpFilterDPWSIPv4"                            : "vr_WebService_IPV4" ,
            "Network/Services/PowerManagement/PMWakeUpFilterARP"                                 : "vr_ARP"  ,
            "Network/Services/PowerManagement/PMWakeUpFilterSLP"                                 : "vr_SLP" ,
            // for SMB domian setting
            "Network/Protocols/SMB/enableDomainLogin"                                            : "SMB_DomainLogin",
            "Network/Services/PowerManagement/PMWakeUpFilterBonjourIPV6"                         : "vr_Bonjor_ipv6" ,
            "Network/Services/PowerManagement/PMWakeUpFilterAirPrint"                            : "vr_Airprint" ,
            "Network/Services/PowerManagement/PMWakeUpFilterAirPrintIPV6"                        : "vr_Airprint_ipv6"
        };

        var chkbxHashMap ={
          "ARP_ID"                      : fnGetLocaleString("103336","ARP") ,
          "SNMP_IPV4_ID"                : fnGetLocaleString("103337","SNMP for IPV4"),
          "BONJOR_ID"                   : fnGetLocaleString("104493","Bonjour for IPv4"),
          "BONJOR_ID_IPV6"              : fnGetLocaleString("104494","Bonjour for IPv6"),
          "AIRPRINT_ID"                 : fnGetLocaleString("104453","AirPrint Discovery for IPv4"),
          "AIRPRINT_ID_IPV6"            : fnGetLocaleString("104495","AirPrint Discovery for IPv6"),
          "SNMP_IPV6_ID"                : fnGetLocaleString("103338","SNMP for IPV6"),
          "SMB_ID"                      : fnGetLocaleString("101731","SMB"),
          "NEIGHBORDISCOVERY_LL_ID"     : fnGetLocaleString("103339","Neighbor Discovery(Link Local Address)"),
          "NEIGHBORDISCOVERY_M_ID"      : fnGetLocaleString("103340","Neighbor Discovery(Manual / Stateful Address)"),
          "LLMR_ID"                     : fnGetLocaleString("101074","LLMNR") ,
          "SLP_ID"                      : fnGetLocaleString("103342","SLP"),
          "LLTD_ID"                     : fnGetLocaleString("103341","LLTD"),
          "WEB_SERVICE_IPV6_ID"         : fnGetLocaleString("103344","Web Services for IPV6"),
          "WEB_SERVICE_IPV4_ID"         : fnGetLocaleString("103343","Web Services for IPV4")
        }
        var strProtoHashMap = {
          "vr_ARP"                  : fnGetLocaleString("103336","ARP") ,
          "vr_SNMPIPV4"             : fnGetLocaleString("103337","SNMP for IPV4"),
          "vr_SNMPIPV6"             :  fnGetLocaleString("103338","SNMP for IPV6"),
          "vr_SMB"                  :  fnGetLocaleString("101731","SMB"),
          "vr_Bonjor"               :  fnGetLocaleString("104493","Bonjour for IPv4") ,
          "vr_Bonjor_ipv6"          :  fnGetLocaleString("104494","Bonjour for IPv6") ,
          "vr_Airprint"             :  fnGetLocaleString("104453","AirPrint Discovery for IPv4") ,
          "vr_Airprint_ipv6"        :  fnGetLocaleString("104495","AirPrint Discovery for IPv6") ,
          "vr_ND_LinkLocalAddr"     :  fnGetLocaleString("103339","Neighbor Discovery(Link Local Address)"),
          "vr_ND_Manual"            :  fnGetLocaleString("103340","Neighbor Discovery(Manual / Stateful Address)"),
          "vr_LLMNR"                :  fnGetLocaleString("101074","LLMNR"),
          "vr_LLTD"                 :  fnGetLocaleString("103341","LLTD"),
          "vr_SLP"                  :  fnGetLocaleString("103342","SLP"),
          "vr_WebService_IPV4"      :  fnGetLocaleString("103343","Web Services for IPV4"),
          "vr_WebService_IPV6"      :  fnGetLocaleString("103344","Web Services for IPV6")
        };
        var gblStateLessIpTableArray = new Array(7);
        var gblStateFulIpTableArray = new Array();
        var gblTempNtwDOMArr = new Array();

        function fnChkNtwStatus(node){
            var ntwStatus = getXpathValue(node,"Network/Status/state");
            if(ntwStatus == "NetworkSettingsUpdated" || ntwStatus == "NetworkInitialized"){
                for(var i = 0; i < gblHashMapArray.length; i++) {
                    BindToWidgets(node,gblHashMapArray[i]);
                }
            }else{
                gblNtwLoadFlag = false;
                fnShowNicInitMsg();
            }
        }
        function fillNetworkPage(){
            try{
                var node = xmlDoc;

                setValue("SMTPClient_MaxSize",((temp = getXpathValue(node,"Controller/Settings/AdminSystemSettings/EMail/SendMaxSize"))==null)?"":temp);
                fnFillSNMP();
                // IPSec License Enable Check
                if(!isEFI){  //Ipsec will be disabled if EFI is Installed
                    if(ipsecExists == ""){
                        var ipsecLicense = getXpathValue(node,"LicenseEnableCheck/Response/statusOfOperation");
                        if(ipsecLicense == "STATUS_OK"){
                            top.fnTrackCookie("LICENSE_SETTINGS","SET","IPSEC","YES");
                            fillIPSec();
                            showDivs("IPSEC_ID~IPSEC_LINKID");
                        }else{
                            top.fnTrackCookie("LICENSE_SETTINGS","SET","IPSEC","NO");
                            hideDivs("IPSEC_ID~IPSEC_LINKID");
                        }
                    }else if(ipsecExists == "YES"){
                        fillIPSec();
                        showDivs("IPSEC_ID~IPSEC_LINKID");
                    }else if(ipsecExists == "NO"){
                        hideDivs("IPSEC_ID~IPSEC_LINKID");
                    }
                }
                // IPV6 Settings
                setValue("IPv6_Enable",((temp = getXpathValue(node,"Network/Protocols/IPV6/enabled"))==null)?"":temp);
                setValue("IPv6_LinkLocalAddr",((temp = getXpathValue(node,"Network/Protocols/IPV6/linkLocalAddress"))==null)?"":temp);
                setValue("IPv6_Selected",((temp = getXpathValue(node,"Network/Protocols/IPV6/addressMode"))==null)?"":temp);
                setValue("DNS_PrimaryIPv6",((temp = getXpathValue(node,"Network/Protocols/IPV6/primaryDNSIP"))==null)?"":temp);
                setValue("DNS_SecondaryIPv6",((temp = getXpathValue(node,"Network/Protocols/IPV6/secondaryDNSIP"))==null)?"":temp);

                if(!(top.gblDeviceModelId == "STHELENS" || top.gblDeviceModelId == "MOSEL") && !isEFI && (getXpathValue(node,"DiagnosticMode/Category/MainCode[@MainCode='9060']/ValueList/Value/Value")!="0")){
                    //adding to set super seep widget value in network page

                    var sleepState = ((temp = getXpathValue(node,"Controller/Settings/PowerSettings/EnableSuperSleep"))==null)?"":temp;
                    if(sleepState == "true")
                        sleepState = fnGetLocaleString("100604","Enable");
                    else
                        sleepState = fnGetLocaleString("100526","Disable");

                    setValue("SuperSleep_enable_id",sleepState);

                    document.getElementById("ID_SUPER_SLEEP").style.display = "";
                }
                else
                    document.getElementById("ID_SUPER_SLEEP").style.display = "none";

                fnconstructValueForPortocolStr();

                var addrModeIPV6 = getXpathValue(node,"Network/Protocols/IPV6/addressMode");
                setValue("IPv6_Selected",addrModeIPV6); 
               //var ipAddress = ((temp = getXpathValue(node,"Network/Protocols/IPV6/Manual/IPAddress/ipAddress"))==null)?"":temp;
               // if(ipAddress != "" && ipAddress != null && ipAddress !== undefined){
                if(addrModeIPV6 == "Manual"){
                    setValue("IPv6_IPAddr",((temp = getXpathValue(node,"Network/Protocols/IPV6/Manual/IPAddress/ipAddress"))==null)?"":temp);
                    setValue("IPv6_PrefixLength",((temp = getXpathValue(node,"Network/Protocols/IPV6/Manual/IPAddress/prefix"))==null)?0:temp);
                    setValue("IPv6_Gateway",((temp = getXpathValue(node,"Network/Protocols/IPV6/Manual/IPAddress/gateway"))==null)?"":temp);
                    setValue("IPv6_Manual_DHCP",((temp = getXpathValue(node,"Network/Protocols/IPV6/Manual/useDHCPV6Options"))==null)?"":temp);
                }else{
                    setValue("IPv6_IPAddr",((temp = getXpathValue(node,"Network/Protocols/IPV6/Stateful/IPAddress/ipAddress"))==null)?"":temp);
                    setValue("IPv6_PrefixLength",((temp = getXpathValue(node,"Network/Protocols/IPV6/Stateful/IPAddress/prefix"))==null)?0:temp);
                    setValue("IPv6_Gateway",((temp = getXpathValue(node,"Network/Protocols/IPV6/Stateful/IPAddress/gateway"))==null)?"":temp);

                    if(addrModeIPV6 == "Stateless"){
                        setValue("IPv6_Stateless__DHCPIP",((temp = getXpathValue(node,"Network/Protocols/IPV6/Stateless/useDHCPV6IPAddress"))==null)?"":temp);
                        setValue("IPv6_Stateless_DHCPOptions",((temp = getXpathValue(node,"Network/Protocols/IPV6/Stateless/useDHCPV6Options/enable"))==null)?"":temp);
                        setValue("IPv6_Stateless_FQDNOption",((temp = getXpathValue(node,"Network/Protocols/IPV6/Stateless/useDHCPV6Options/fqdnOptionFlag/enable"))==null)?"":temp);
                        setValue("IPv6_Stateless_UpdateMethod",((temp = getXpathValue(node,"Network/Protocols/IPV6/Stateless/useDHCPV6Options/fqdnOptionFlag/updateMethod"))==null)?"":temp);

                    }else if(addrModeIPV6 == "Stateful"){
                        setValue("IPv6_Stateful__DHCPIP",((temp = getXpathValue(node,"Network/Protocols/IPV6/Stateful/useDHCPV6IPAddress"))==null)?"":temp);
                        setValue("IPv6_Stateful__DHCP_Options",((temp = getXpathValue(node,"Network/Protocols/IPV6/Stateful/useDHCPV6Options/enable"))==null)?"":temp);
                        setValue("IPv6_Stateful_FQDNOption",((temp = getXpathValue(node,"Network/Protocols/IPV6/Stateful/useDHCPV6Options/fqdnOptionFlag/enable"))==null)?"":temp);
                        setValue("IPv6_Stateful_UpdateMethod",((temp = getXpathValue(node,"Network/Protocols/IPV6/Stateful/useDHCPV6Options/fqdnOptionFlag/updateMethod"))==null)?"":temp);

                    }
                }

                //for Stateless IP Addr Table
                var stateLessObj = evaluateXpath(node,"Network/Protocols/IPV6/Stateless");
                var stateLessIPTableObj = stateLessObj.getElementsByTagName("IPAddress");
                for(var i=0;i<stateLessIPTableObj.length;i++){
                    gblStateLessIpTableArray[i] = new Array(3);
                    var ipAddr = ((temp = getXpathValue(stateLessIPTableObj[i],"ipAddress"))==null)?"":temp;
                    gblStateLessIpTableArray[i][0] = parseInt(i+1)+":  "+ipAddr;
                    gblStateLessIpTableArray[i][1] = ((temp = getXpathValue(stateLessIPTableObj[i],"prefix"))==null)?0:temp;
                    if(gblStateLessIpTableArray[i][1] == "")
                        gblStateLessIpTableArray[i][1] = 0;
                    gblStateLessIpTableArray[i][2] = ((temp = getXpathValue(stateLessIPTableObj[i],"gateway"))==null)?"":temp;
                }
                //for Stateful IPTable
                var stateFulObj = evaluateXpath(node,"Network/Protocols/IPV6/Stateful");

                gblStateFulIpTableArray = new Array(3);
                gblStateFulIpTableArray[0] = ((temp = getXpathValue(stateFulObj,"IPAddress/ipAddress"))==null)?"&nbsp;":temp;
                gblStateFulIpTableArray[1] = ((temp = getXpathValue(stateFulObj,"IPAddress/prefix"))==null)?0:temp;
                if(gblStateFulIpTableArray[1] == "")
                    gblStateFulIpTableArray[1] = 0;
                gblStateFulIpTableArray[2] = ((temp = getXpathValue(stateFulObj,"IPAddress/gateway"))==null)?"&nbsp;":temp;

                // SMB Enable Xpath mapping
                var smbEnablerXpath = getValue("SMB_EnableXpath");
                var smbFileEnable = getValue("SMB_FileShare_Enable");
                var smbPrintEnable= getValue("SMB_Print_Enable");
                var guestLogonUsername_xpath = getXpathValue(node,"Network/Protocols/SMB/serverAuthUser");
                var guestLogonPassword_xpath = getXpathValue(node,"Network/Protocols/SMB/serverAuthPassword");
                $("#SMB_AuthUserName").data("onLoad",guestLogonUsername_xpath);
                $("#SMB_Password").data("onLoad",guestLogonPassword_xpath);
                if(smbEnablerXpath == "0") {
                    setValue("SMB_Enable","0");
                }
                else {
                   setValue("SMB_Enable","1");
                }

                if(smbFileEnable == "0" && smbPrintEnable == "1") {
                    setValue("SMB_Restriction","2");
                }
                else if(smbPrintEnable == "0" && smbFileEnable == "1") {
                    setValue("SMB_Restriction","1");
                }
                else {
                    setValue("SMB_Restriction","0");
                }

                parent.frames[0].document.getElementById("btnNetSave").disabled= false;
            }  catch(e) {errHandler(e,'fillNetworkPage()','NetworkMap.js',"");}
        }
        //IP Security handling..
        function fillIPSec(){
            try{
            var node = xmlDoc;
            var ipsecObj = evaluateXpath(node,"Network/Protocols/IPSec");
            if(ipsecObj != null){
                setValue("IPSec_Enable",((temp = getXpathValue(ipsecObj,"enabled"))==null)? "0" :temp);
                setValue("Policy_Name",((temp = getXpathValue(ipsecObj,"selectedPolicy"))==null)? "0" :temp);
                if(ipsecObj != null){
                    var filtersObj = evaluateXpath(ipsecObj,"Filters");
                    if(filtersObj == null || filtersObj === undefined){
                    }else{
                        var filterObj = filtersObj.getElementsByTagName("Filter");
                        if(filterObj == null || filterObj === undefined){
                        }else{
                            fnExtractAndBindFilterData(filterObj);
                        }
                    }
                    var manualKeysObj = evaluateXpath(ipsecObj,"ManualKeys");
                    if(manualKeysObj == null || manualKeysObj === undefined){
                    }else{
                        var manualKeyObj = manualKeysObj.getElementsByTagName("ManualKey");
                        if(manualKeyObj == null || manualKeyObj === undefined){
                        }else{
                            fnExtractAndBindManualKeyData(manualKeyObj);
                        }
                    }
                    var ikeKeysObj = evaluateXpath(ipsecObj,"IKEKeys");
                    if(ikeKeysObj == null || ikeKeysObj === undefined){
                    }else{
                        var ikeObj = ikeKeysObj.getElementsByTagName("IKEKey");
                        if(ikeObj == null || ikeObj === undefined){
                        }else{
                            fnExtractAndBindIKEData(ikeObj);
                        }
                    }
                    var profilesObj = evaluateXpath(ipsecObj,"Profiles");
                    if(profilesObj == null || profilesObj === undefined){
                    }else{
                        var profileObj = ipsecObj.getElementsByTagName("Profile");

                        if(profileObj == null || profileObj === undefined){
                        }else{
                            fnExtractAndBindProfileData(profileObj);
                        }
                    }
                    //Policy
                    var policiesObj = evaluateXpath(ipsecObj,"Policies");
                    if(policiesObj == null || policiesObj === undefined){
                    }else{
                        var policyObj = ipsecObj.getElementsByTagName("Policy");

                        if(policyObj == null || policyObj === undefined){
                        }else{
                            fnExtractAndBindPolicyData(policyObj);
                        }
                    }
                }
            }
           } catch(e){errHandler(e,'fillIPSec()','NetworkMap.js',"")}
        }
        //SNMP handling..
        function fnFillSNMP(){
            fnExtractSNMPUsrStr();
            // after binding to SNMPUserStr call this function for SNMP v3 user table formation.
            fnConstSNMPV3UsrList();
        }
        function fnExtractSNMPUsrStr(){
            // code for constructing and binding to SNMPUserStr and savedSNMPUserStr
            var usrTableStr = "";
            var node = xmlDoc;
            var usrDataObj = node.getElementsByTagName("otherUsers");
            var usrTableArr = new Array(usrDataObj.length);
            for(var i=0;i<usrDataObj.length;i++){
                var eachUsrData = new Array (7);
                eachUsrData[0] = usrDataObj[i].getAttribute("index");
                if(eachUsrData[0] == null || eachUsrData[0] === undefined)
                    continue;    
                eachUsrData[1] = ((temp = getXpathValue(usrDataObj[i],"snmpV3UserName")) == null)? "" : temp;
                eachUsrData[2] = ((temp = getXpathValue(usrDataObj[i],"snmpV3AuthenticationProtocol")) == null)? "" : temp;
                if(eachUsrData[2] == "MD5")
                    eachUsrData[2] = "HMAC-MD5";
                else if(eachUsrData[2] == "SHA")
                    eachUsrData[2] = "HMAC-SHA";
                eachUsrData[3] = ((temp = getXpathValue(usrDataObj[i],"snmpV3AuthenticationPassword")) == null)? "" : temp;
                eachUsrData[4] = ((temp = getXpathValue(usrDataObj[i],"snmpV3PrivacyProtocol")) == null)? "" : temp;
                if(eachUsrData[4] == "None")
                    eachUsrData[4] = "None";
                else if(eachUsrData[4] == "DES")
                    eachUsrData[4] = "CBC-DES";
                else if(eachUsrData[4] == "AES")
                    eachUsrData[4] = "CFB-AES-128";
                eachUsrData[5] = ((temp = getXpathValue(usrDataObj[i],"snmpV3PrivacyPassword")) == null)? "" : temp;
                eachUsrData[6] = ((temp = getXpathValue(usrDataObj[i],"snmpV3PermissionLevel")) == null)? "" : temp;
                eachUsrData[7] = "SAVED";
                eachUsrData[8] = ((temp = getXpathValue(usrDataObj[i],"contextName")) == null)? "" : temp;
                usrTableArr[i] = eachUsrData.join(gblDestSep1);
            }
            usrTableStr = usrTableArr.join(gblDestSep2);
            //sorting the table data..
            var sTableData = "";
            var tmpArr = usrTableArr;
            if(tmpArr.length > 1){
                sTableData = "";
                var tmpUsrData;
                for(var i=0;i<tmpArr.length;i++){
                    for(var j=i+1;j<tmpArr.length;j++){     //bubble sort
                        if(parseInt(tmpArr[i].split(gblDestSep1)[0],10) > parseInt(tmpArr[j].split(gblDestSep1)[0],10)){
                            tmpUsrData = tmpArr[i];
                            tmpArr[i] = tmpArr[j];
                            tmpArr[j] = tmpUsrData;
                        }
                    }
                    sTableData += tmpArr[i];
                    if(i!=tmpArr.length-1){
                        sTableData += gblDestSep2;
                    }
                }
                usrTableStr = sTableData;
            }
            //sorting ends..
            setValue("SNMPUserStr",usrTableStr);
            setValue("savedSNMPUserStr",usrTableStr);
        }
        // this function should be called on click of network page save button from SetUpTitle.html.
        function fnConstSNMPXML(){
            // Construct SNMPV3 User Xml from snmp user Table and set to SNMPCmdXml hidden field.
            var userStr = "";
            var userData = getValue("SNMPUserStr");
            userData = userData.split(gblDestSep2);
            for(var i=0;i<userData.length;i++){
                var eachUsrData = userData[i].split(gblDestSep1);

                if(eachUsrData == "" ||eachUsrData == null ||eachUsrData === undefined)
                    continue;
                if(eachUsrData[0] == "" ||eachUsrData[0] == null ||eachUsrData[0] === undefined)
                    continue;
                if(eachUsrData[7] == "NEW"){
                    userStr += "<otherUsers index='' markToDelete='0' markToExport='0' >" +
                               "<snmpV3UserName>"+fnConvertXMLEntities(eachUsrData[1])+"</snmpV3UserName>"+
                               "<contextName>"+eachUsrData[8]+"</contextName>";
                    if(eachUsrData[2] == "HMAC-MD5"){
                        userStr +="<snmpV3AuthenticationProtocol>MD5</snmpV3AuthenticationProtocol>";
                    }else if(eachUsrData[2] == "HMAC-SHA"){
                        userStr +="<snmpV3AuthenticationProtocol>SHA</snmpV3AuthenticationProtocol>";
                    }
                    userStr +="<snmpV3AuthenticationPassword>"+fnConvertXMLEntities(eachUsrData[3])+"</snmpV3AuthenticationPassword>";
                    if(eachUsrData[4] == "None"){
                        userStr +="<snmpV3PrivacyProtocol>None</snmpV3PrivacyProtocol>";
                    }else if(eachUsrData[4] == "CBC-DES"){
                        userStr +="<snmpV3PrivacyProtocol>DES</snmpV3PrivacyProtocol>";
                    }else if(eachUsrData[4] == "CFB-AES-128"){
                        userStr +="<snmpV3PrivacyProtocol>AES</snmpV3PrivacyProtocol>";
                    }
                    userStr +="<snmpV3PrivacyPassword>"+fnConvertXMLEntities(eachUsrData[5])+"</snmpV3PrivacyPassword>";
                    userStr +="<snmpV3PermissionLevel>"+eachUsrData[6]+"</snmpV3PermissionLevel></otherUsers>";
                }else if(eachUsrData[7] == "SAVED"){
                    var tmpUsrStr ="";
                    var snmpUsrObj = null ;
                    var snmpUsrDataObj = gblDomObjArray[0].getElementsByTagName("otherUsers");
                    for(var j=0;j<snmpUsrDataObj.length;j++){
                        if(snmpUsrDataObj[j].getAttribute("index") == eachUsrData[0]){
                            snmpUsrObj = snmpUsrDataObj[j];
                             break;
                         }
                    }
                    if(snmpUsrObj == null) continue;
                    tmpUsrStr = "";
                    var savedUserName = getXpathValue(snmpUsrObj,"snmpV3UserName");
                    if(savedUserName != eachUsrData[1]){
                        tmpUsrStr +=  "<snmpV3UserName>"+fnConvertXMLEntities(eachUsrData[1])+"</snmpV3UserName>";
                    }
                    var savedAuthProt = getXpathValue(snmpUsrObj,"snmpV3AuthenticationProtocol");
                    if(eachUsrData[2] == "HMAC-MD5"){
                        eachUsrData[2] = "MD5";
                    }else if(eachUsrData[2] == "HMAC-SHA"){
                        eachUsrData[2] = "SHA";
                    }
                    if(savedAuthProt != eachUsrData[2]){
                        tmpUsrStr +=  "<snmpV3AuthenticationProtocol>"+eachUsrData[2]+"</snmpV3AuthenticationProtocol>";
                    }
                    var savedAuthPwd = getXpathValue(snmpUsrObj,"snmpV3AuthenticationPassword");
                    if(savedAuthPwd != eachUsrData[3]){
                        tmpUsrStr +=  "<snmpV3AuthenticationPassword>"+fnConvertXMLEntities(eachUsrData[3])+"</snmpV3AuthenticationPassword>";
                    }
                    var savedPrivacyProt = getXpathValue(snmpUsrObj,"snmpV3PrivacyProtocol");
                    if(eachUsrData[4] == "CBC-DES"){
                        eachUsrData[4] = "DES";
                    }else if(eachUsrData[4] == "CFB-AES-128"){
                        eachUsrData[4] = "AES";
                    }
                    if(savedPrivacyProt != eachUsrData[4]){
                        tmpUsrStr +=  "<snmpV3PrivacyProtocol>"+eachUsrData[4]+"</snmpV3PrivacyProtocol>";
                    }
                    var savedPrivacyPwd = getXpathValue(snmpUsrObj,"snmpV3PrivacyPassword");
                    if(savedPrivacyPwd != eachUsrData[5]){
                        tmpUsrStr +=  "<snmpV3PrivacyPassword>"+fnConvertXMLEntities(eachUsrData[5])+"</snmpV3PrivacyPassword>";
                    }
                    var savedPermLevel = getXpathValue(snmpUsrObj,"snmpV3PermissionLevel");
                    if(savedPermLevel != eachUsrData[6]){
                        tmpUsrStr +=  "<snmpV3PermissionLevel>"+eachUsrData[6]+"</snmpV3PermissionLevel>";
                    }
                    if(tmpUsrStr != ""){
                        userStr += "<otherUsers index='"+eachUsrData[0]+"' markToDelete='0' markToExport='0' ><contextName>"+eachUsrData[8]+"</contextName>"+tmpUsrStr+"</otherUsers>";
                    }

                }

            }
            var delUsrStr = fnAppndDelUsrDataInCmdXml();
            if(userStr != "" || delUsrStr != ""){
                setValue("SNMPCmdXml","<snmpV3UserTable>"+userStr+delUsrStr+"</snmpV3UserTable>");
            }
        }
        function fnAppndDelUsrDataInCmdXml(){
            // Construct SNMPV3 User Xml from snmp user Table and set to SNMPCmdXml hidden field.
            var userStr = "";
            var userData = getValue("DelSNMPUserStr");
            userData = userData.split(gblDestSep2);
            for(var i=0;i<userData.length;i++){
                var eachUsrData = userData[i].split(gblDestSep1);

                if(eachUsrData == "" ||eachUsrData == null ||eachUsrData === undefined)
                    continue;
                if(eachUsrData[0] == "" ||eachUsrData[0] == null ||eachUsrData[0] === undefined)
                    continue;
                userStr += "<otherUsers index='"+eachUsrData[0]+"' markToDelete='1' markToExport='0'/>";
            }
            return userStr;
        }
        
        // SNMP handling ends here..
        // this function should be called on click of network page save button from SetUpTitle.html.
        function fnConstIPV6XML(){
            var setXmlIPV6 = "";
            var initialIPV6SettingsObj = evaluateXpath(gblDomObjArray[0],"Network/Protocols/IPV6");

            var enable = getValue("IPv6_Enable");
            var initEnable = getXpathValue(initialIPV6SettingsObj,"enabled");
            if(enable != initEnable){
                setXmlIPV6 +="<enabled>"+enable+"</enabled>";
            }
            var addrMode = getValue("IPv6_Selected");
            var initaddrMode = getXpathValue(initialIPV6SettingsObj,"addressMode");
            if(addrMode != initaddrMode){
                setXmlIPV6 +="<addressMode>"+addrMode+"</addressMode>";
            }
            var priDNS = getValue("DNS_PrimaryIPv6");
            var initpriDNS = getXpathValue(initialIPV6SettingsObj,"primaryDNSIP");
            if(priDNS != initpriDNS){
                setXmlIPV6 +="<primaryDNSIP>"+priDNS+"</primaryDNSIP>";
            }
            var secDNS = getValue("DNS_SecondaryIPv6");
            var initsecDNS = getXpathValue(initialIPV6SettingsObj,"secondaryDNSIP");
            if(secDNS != initsecDNS){
                setXmlIPV6 +="<secondaryDNSIP>"+secDNS+"</secondaryDNSIP>";
            }
            switch(addrMode){
                case "Manual":
                    var manualStr = "";
                    var initIpAddr = getXpathValue(initialIPV6SettingsObj,"ipAddress");
                    var ipAddr = getValue("IPv6_IPAddr");
                    if(ipAddr != initIpAddr){
                        manualStr +="<ipAddress>"+ipAddr+"</ipAddress>";
                    }
                    var initPrefix= getXpathValue(initialIPV6SettingsObj,"prefix");
                    var prefix = getValue("IPv6_PrefixLength");
                    if(prefix != initPrefix){
                        manualStr +="<prefix>"+prefix+"</prefix>";
                    }
                    var initDefGw = getXpathValue(initialIPV6SettingsObj,"gateway");
                    var defGw  = getValue("IPv6_Gateway");
                    if(defGw != initDefGw){
                        manualStr +="<gateway>"+defGw+"</gateway>";
                    }
                    if(manualStr != "")
                        manualStr = "<IPAddress>"+manualStr+"</IPAddress>";
                    var initDhcpOptions = getXpathValue(initialIPV6SettingsObj,"useDHCPV6Options");
                    var dhcpOptions  = ((temp = getValue("IPv6_Manual_DHCP")) == "")?"0":temp;
                    if(dhcpOptions != initDhcpOptions){
                        manualStr +="<useDHCPV6Options>"+dhcpOptions+"</useDHCPV6Options>";
                    }
                    if(manualStr != "")
                        manualStr = "<Manual>"+manualStr+"</Manual>";
                    setXmlIPV6 += manualStr;
                    break;

                case "Stateless":
                    var statelessStr = "";
                    var initDHCPStateLess = getXpathValue(initialIPV6SettingsObj,"Stateless/useDHCPV6IPAddress");
                    var stateLessDHCP = ((temp = getValue("IPv6_Stateless__DHCPIP")) == "")?"0":temp;
                    if(stateLessDHCP != initDHCPStateLess){
                        statelessStr += "<useDHCPV6IPAddress>"+stateLessDHCP+"</useDHCPV6IPAddress>";
                    }
                    var optionStr = "";
                    var initDHCPOption = getXpathValue(initialIPV6SettingsObj,"Stateless/useDHCPV6Options/enable");
                    var stateLessDHCPOptions = ((temp = getValue("IPv6_Stateless_DHCPOptions")) == "")?"false":temp;
                    if(stateLessDHCPOptions != initDHCPOption){
                        optionStr += "<enable>"+stateLessDHCPOptions+"</enable>";
                    }
                    var fqdnStr = "";
                    var initfqdnEnable = getXpathValue(initialIPV6SettingsObj,"Stateless/useDHCPV6Options/fqdnOptionFlag/enable");
                    var stateLessFqdnEnable = ((temp = getValue("IPv6_Stateless_FQDNOption")) == "")?"false":temp;
                    if(stateLessFqdnEnable != initfqdnEnable){
                        fqdnStr += "<enable>"+stateLessFqdnEnable+"</enable>";
                    }
                    var initfqdnUpdateMethod = getXpathValue(initialIPV6SettingsObj,"Stateless/useDHCPV6Options/fqdnOptionFlag/updateMethod");
                    var stateLessFqdnUpdateMethod = getValue("IPv6_Stateless_UpdateMethod");
                    if(stateLessFqdnUpdateMethod != initfqdnUpdateMethod){
                        fqdnStr += "<updateMethod>"+stateLessFqdnUpdateMethod+"</updateMethod>";
                    }
                    if(fqdnStr  != ""){
                        fqdnStr = "<fqdnOptionFlag>"+fqdnStr+"</fqdnOptionFlag>"
                    }
                    optionStr += fqdnStr;
                    if(optionStr != ""){
                        statelessStr += "<useDHCPV6Options>"+optionStr+"</useDHCPV6Options>";
                    }
                    if(statelessStr != ""){
                        setXmlIPV6 += "<Stateless>"+statelessStr+"</Stateless>";
                    }
                    break;

                case "Stateful":
                    var stateFulStr = "";
                    var initstateFulDHCPIP = getXpathValue(initialIPV6SettingsObj,"Stateful/useDHCPV6IPAddress");
                    var stateFulDHCPIP = ((temp = getValue("IPv6_Stateful__DHCPIP")) == "")?"0":temp;
                    if(stateFulDHCPIP != initstateFulDHCPIP){
                        stateFulStr += "<useDHCPV6IPAddress>"+stateFulDHCPIP+"</useDHCPV6IPAddress>";
                    }
                    var optionStr = "";
                    var initstateFulDHCPOptions = getXpathValue(initialIPV6SettingsObj,"Stateful/useDHCPV6Options/enable");
                    var stateFulDHCPOptions = ((temp = getValue("IPv6_Stateful__DHCP_Options")) == "")?"false":temp;
                    if(stateFulDHCPOptions != initstateFulDHCPOptions){
                        optionStr += "<enable>"+stateFulDHCPOptions+"</enable>";
                    }
                    var fqdnStr = "";
                    var initfqdnEnable = getXpathValue(initialIPV6SettingsObj,"Stateful/useDHCPV6Options/fqdnOptionFlag/enable");
                    var stateFulFqdnEnable = ((temp = getValue("IPv6_Stateful_FQDNOption")) == "")?"false":temp;
                    if(stateFulFqdnEnable != initfqdnEnable){
                        fqdnStr += "<enable>"+stateFulFqdnEnable+"</enable>";
                    }
                    var initfqdnUpdateMethod = getXpathValue(initialIPV6SettingsObj,"Stateful/useDHCPV6Options/fqdnOptionFlag/updateMethod");
                    var stateFulFqdnUpdateMethod = getValue("IPv6_Stateful_UpdateMethod");
                    if(stateFulFqdnUpdateMethod != initfqdnUpdateMethod){
                        fqdnStr += "<updateMethod>"+stateFulFqdnUpdateMethod+"</updateMethod>";
                    }
                    if(fqdnStr  != ""){
                        fqdnStr = "<fqdnOptionFlag>"+fqdnStr+"</fqdnOptionFlag>"
                    }
                    optionStr += fqdnStr;

                    if(optionStr != ""){
                        stateFulStr += "<useDHCPV6Options>"+optionStr+"</useDHCPV6Options>";
                    }
                    if(stateFulStr != ""){
                        setXmlIPV6 += "<Stateful>"+stateFulStr+"</Stateful>";
                    }
                    break;
            }
            if(setXmlIPV6 != ""){
                setXmlIPV6 = "<IPV6>"+setXmlIPV6+"</IPV6>";
                setValue("IPV6CmdXml",setXmlIPV6);
            }
        }
        function fnExtractAndBindFilterData(filterObj){
            var filterData = new Array();
            var strFilterData = new Array();
            for(var i=0;i<filterObj.length;i++){

                var filterIndex = filterObj[i].getAttribute("index");
                if(filterIndex == "" || filterIndex == null)
                    continue;
                filterData[i] = new Array();
                filterData[i][0] = filterIndex;
                filterData[i][1] = filterObj[i].getAttribute("name");
                filterData[i][1] = filterData[i][1].replace(/\"/g,"&#34;");
                //filterData[i][1] = filterData[i][1].replace(/~/g,"&#126;");
                filterData[i][2] = ((temp = getXpathValue(filterObj[i],"inetProtocolVersion")) == null)?"":temp;
                // filterData[i][3] is MyIpAddress which is not required to send to Backend 
                filterData[i][4] = ((temp = getXpathValue(filterObj[i],"destinationAddress/addressType")) == null)?"":temp;
                filterData[i][5] = ((temp = getXpathValue(filterObj[i],"destinationAddress/address")) == null)?"":temp;
                filterData[i][6] = ((temp = getXpathValue(filterObj[i],"protocolType")) == null)?"":temp;
                filterData[i][7] = ((temp = getXpathValue(filterObj[i],"sourcePort/portType")) == null)?"":temp;
                filterData[i][8] = ((temp = getXpathValue(filterObj[i],"sourcePort/portNo")) == null)?"":temp;
                filterData[i][9] = ((temp = getXpathValue(filterObj[i],"destinationPort/portType")) == null)?"":temp;
                filterData[i][10] = ((temp = getXpathValue(filterObj[i],"destinationPort/portNo")) == null)?"":temp;
                filterData[i][11] = ((temp = getXpathValue(filterObj[i],"filterAction")) == null)?"":temp;
                if(filterData[i][11] == "NegotiateESP"){
                    filterData[i][11] = "Negotiate Security";
                    filterData[i][12] = "ESP";          
                }
                else if(filterData[i][11] == "NegotiateAH"){
                    filterData[i][11] = "Negotiate Security";
                    filterData[i][12] = "AH";
                }
                else
                    filterData[i][12] = "";
                filterData[i][13] = filterObj[i].getAttribute("inUse");
                if(filterData[i][13] == "" || filterData[i][13] == null ||filterData[i][13]===undefined)
                    filterData[i][13] = 0;
                filterData[i][14] = "SAVED";

                strFilterData[i] = filterData[i].join(gblSep2);
            }
            setValue("IPSECFILTERDATA",strFilterData.join(gblSep1));
        }
        function fnExtractAndBindManualKeyData(manualKeyObj){
            var manualKeyData = new Array();
            var strManualKeyData = new Array();
            for(var i=0;i<manualKeyObj.length;i++){
                var manualKeyIndex = manualKeyObj[i].getAttribute("index");
                if(manualKeyIndex == "" || manualKeyIndex == null)
                    continue;
                manualKeyData[i] = new Array();
                manualKeyData[i][0] = manualKeyIndex;
                manualKeyData[i][1] = manualKeyObj[i].getAttribute("name");
                manualKeyData[i][1] = manualKeyData[i][1].replace(/\"/g,"&#34;");
                //manualKeyData[i][1] = manualKeyData[i][1].replace(/~/g,"&#126;");
                manualKeyData[i][2] = ((temp = getXpathValue(manualKeyObj[i],"EncryptionAlgorithm")) == null)?"":temp;
                manualKeyData[i][3] = ((temp = getXpathValue(manualKeyObj[i],"IntegrityAlgorithm")) == null)?"":temp;
                manualKeyData[i][4] = ((temp = getXpathValue(manualKeyObj[i],"InboundKey/spi")) == null)?"":temp;
                manualKeyData[i][5] = ((temp = getXpathValue(manualKeyObj[i],"InboundKey/ESPEncryKey")) == null)?"":temp;
                manualKeyData[i][6] = ((temp = getXpathValue(manualKeyObj[i],"InboundKey/ESPAuthKey")) == null)?"":temp;
                manualKeyData[i][7] = ((temp = getXpathValue(manualKeyObj[i],"InboundKey/AHAuthKey")) == null)?"":temp;
                manualKeyData[i][8] = ((temp = getXpathValue(manualKeyObj[i],"OutboundKey/spi")) == null)?"":temp;
                manualKeyData[i][9] = ((temp = getXpathValue(manualKeyObj[i],"OutboundKey/ESPEncryKey")) == null)?"":temp;
                manualKeyData[i][10] = ((temp = getXpathValue(manualKeyObj[i],"OutboundKey/ESPAuthKey")) == null)?"":temp;
                manualKeyData[i][11] = ((temp = getXpathValue(manualKeyObj[i],"OutboundKey/AHAuthKey")) == null)?"":temp;

                manualKeyData[i][12] = manualKeyObj[i].getAttribute("inUse");
                if(manualKeyData[i][12] == "" || manualKeyData[i][12] == null ||manualKeyData[i][12]===undefined)
                    manualKeyData[i][12] = 0;
                manualKeyData[i][13] = "SAVED";

                strManualKeyData[i] = manualKeyData[i].join(gblSep2);
            }
            setValue("IPSEC_MANUAL_DATA",strManualKeyData.join(gblSep1));
        }
        function fnExtractAndBindIKEData(ikeObj){
            var ikeData = new Array();
            var strIKEData = new Array();
            for(var i=0;i<ikeObj.length;i++){
                var ikeIndex = ikeObj[i].getAttribute("index");
                if(ikeIndex == "" || ikeIndex == null)
                    continue;
                ikeData[i] = new Array();
                ikeData[i][0] = ikeIndex;
                ikeData[i][1] = ikeObj[i].getAttribute("name");
                ikeData[i][1] = ikeData[i][1].replace(/\"/g,"&#34;");
                //ikeData[i][1] = ikeData[i][1].replace(/~/g,"&#126;");
                ikeData[i][2] = ((temp = getXpathValue(ikeObj[i],"version")) == null)?"":temp;
                if(ikeData[i][2] == "IKEv1")
                    ikeData[i][3] = ((temp = getXpathValue(ikeObj[i],"authenticationMethod")) == null)?"":temp;
                else if(ikeData[i][2] == "IKEv2")
                    ikeData[i][5] = ((temp = getXpathValue(ikeObj[i],"authenticationMethod")) == null)?"":temp;
                ikeData[i][4] = ((temp = getXpathValue(ikeObj[i],"preHashKey")) == null)?"":temp;
                ikeData[i][4] = ikeData[i][4].replace(/~/g,"&#126;");                    
                ikeData[i][6] = ((temp = getXpathValue(ikeObj[i],"srcID")) == null)?"":temp;
                ikeData[i][7] = ((temp = getXpathValue(ikeObj[i],"srcIDVal")) == null)?"":temp;
                ikeData[i][8] = ((temp = getXpathValue(ikeObj[i],"remoteID")) == null)?"":temp;
                ikeData[i][9] = ((temp = getXpathValue(ikeObj[i],"remoteIDVal")) == null)?"":temp;
                ikeData[i][10] = ((temp = getXpathValue(ikeObj[i],"generateNewKeyDelay")) == null)?"":temp;
                ikeData[i][11] = ((temp = getXpathValue(ikeObj[i],"enablePFS")) == null)?0:temp;
                ikeData[i][12] = ((temp = getXpathValue(ikeObj[i],"Transformations/Integrity/enableSHA1")) == null)?0:temp;
                ikeData[i][13] = ((temp = getXpathValue(ikeObj[i],"Transformations/Integrity/enableMD5")) == null)?0:temp;
                ikeData[i][14] = ((temp = getXpathValue(ikeObj[i],"Transformations/Integrity/enableAESXCBC")) == null)?0:temp;
                ikeData[i][15] = ((temp = getXpathValue(ikeObj[i],"Transformations/Encryption/enableAES256CBC")) == null)?0:temp;
                ikeData[i][16] = ((temp = getXpathValue(ikeObj[i],"Transformations/Encryption/enableAES192CBC")) == null)?0:temp;
                ikeData[i][17] = ((temp = getXpathValue(ikeObj[i],"Transformations/Encryption/enableAES128CBC")) == null)?0:temp;
                ikeData[i][18] = ((temp = getXpathValue(ikeObj[i],"Transformations/Encryption/enableAESCTR")) == null)?0:temp;
                ikeData[i][19] = ((temp = getXpathValue(ikeObj[i],"Transformations/Encryption/enable3DESCBC")) == null)?0:temp;
                ikeData[i][20] = ((temp = getXpathValue(ikeObj[i],"Transformations/Encryption/enableDESCBC")) == null)?0:temp;
                ikeData[i][21] = ((temp = getXpathValue(ikeObj[i],"Transformations/diffieHellmanAlgorithm")) == null)?0:temp;

                ikeData[i][22] = ikeObj[i].getAttribute("inUse");
                if(ikeData[i][23] == "" || ikeData[i][23] == null ||ikeData[i][23]===undefined)
                    ikeData[i][23] = 0;
                ikeData[i][23] = "SAVED";

                strIKEData[i] = ikeData[i].join(gblSep2);
            }
            setValue("IPSECIKEDATA",strIKEData.join(gblSep1));
        }
        function fnExtractAndBindProfileData(profileObj){
            var profileData = new Array();
            var strProfileData = new Array();
            for(var i=0;i<profileObj.length;i++){
                var profileIndex = profileObj[i].getAttribute("index");
                if(profileIndex == "" || profileIndex == null){
                    continue;
                }
                profileData[i] = new Array();
                profileData[i][0] = profileIndex;
                profileData[i][1] = profileObj[i].getAttribute("name");
                profileData[i][1] = profileData[i][1].replace(/\"/g,"&#34;");
                //profileData[i][1] = profileData[i][1].replace(/~/g,"&#126;");
                profileData[i][2] = ((temp = getXpathValue(profileObj[i],"tunnelMode")) == null)?0:temp;
                profileData[i][3] = ((temp = getXpathValue(profileObj[i],"inetAddress")) == null)?"":temp;
                var keySelected = ((temp = getXpathValue(profileObj[i],"keySelected")) == null)?"":temp;
                var keyType = profileObj[i].getElementsByTagName("keySelected")[0].getAttribute("keyType");
                profileData[i][4] = keySelected+"#"+keyType;
                var espTransformObj = evaluateXpath(profileObj[i],"Proposals/ESPTransformation");
                profileData[i][5] = ((temp = getXpathValue(espTransformObj,"enabledSHA1")) == null)?0:temp;
                profileData[i][6] = ((temp = getXpathValue(espTransformObj,"enableMD5")) == null)?0:temp;
                profileData[i][7] = ((temp = getXpathValue(espTransformObj,"enableAESXCBC")) == null)?0:temp;
                profileData[i][8] = ((temp = getXpathValue(espTransformObj,"enableAES256CBC")) == null)?0:temp;
                profileData[i][9] = ((temp = getXpathValue(espTransformObj,"enableAES192CBC")) == null)?0:temp;
                profileData[i][10] = ((temp = getXpathValue(espTransformObj,"enableAES128CBC")) == null)?0:temp;
                profileData[i][11] = ((temp = getXpathValue(espTransformObj,"enableAESCTR")) == null)?0:temp;
                profileData[i][12] = ((temp = getXpathValue(espTransformObj,"enable3DESCBC")) == null)?0:temp;
                profileData[i][13] = ((temp = getXpathValue(espTransformObj,"enableDESCBC")) == null)?0:temp;
                profileData[i][14] = ((temp = getXpathValue(espTransformObj,"noEncryption")) == null)?0:temp;

                profileData[i][15] = ((temp = getXpathValue(profileObj[i],"Proposals/AHTransformation/enableSH1")) == null)?0:temp;
                profileData[i][16] = ((temp = getXpathValue(profileObj[i],"Proposals/AHTransformation/enableMD5")) == null)?0:temp;
                profileData[i][17] = ((temp = getXpathValue(profileObj[i],"Proposals/AHTransformation/enableAESXCBC")) == null)?0:temp;
                profileData[i][18] = ((temp = getXpathValue(profileObj[i],"Proposals/enableTimeInterval")) == null)?0:temp;
                profileData[i][19] = ((temp = getXpathValue(profileObj[i],"Proposals/sessionKeyGenTimeInterval")) == null)?0:temp;
                profileData[i][20] = ((temp = getXpathValue(profileObj[i],"Proposals/enableSizeInterval")) == null)?0:temp;
                profileData[i][21] = ((temp = getXpathValue(profileObj[i],"Proposals/sessionKeyGenSizeInterval")) == null)?0:temp;
                profileData[i][22] = ((temp = getXpathValue(profileObj[i],"Proposals/IPCOMPTransform")) == null)?0:temp;
                //Filter Data..
                var filterObj = profileObj[i].getElementsByTagName("Filter");
                var filterIndexList = new Array();
                for(var j=0;j<filterObj.length;j++){
                    filterIndexList[j] = filterObj[j].getAttribute("index");
                }
                profileData[i][23] = filterIndexList.join(",");
                profileData[i][24] = profileObj[i].getAttribute("inUse");
                if(profileData[i][24] == "" || profileData[i][24] == null ||profileData[i][24]===undefined)
                    profileData[i][24] = 0;
                profileData[i][25] = "SAVED";

                strProfileData[i] = profileData[i].join(gblSep2);
            }
            setValue("IPSEC_PROFILE_DATA",strProfileData.join(gblSep1));
        }
        function fnExtractAndBindPolicyData(policyObj){
            var policyData = new Array();
            var strPolicyData = new Array();
            for(var i=0;i<policyObj.length;i++){
                var policyIndex = policyObj[i].getAttribute("index");
                if(policyIndex == "" || policyIndex == null){
                    continue;
                }
                policyData[i] = new Array();
                policyData[i][0] = policyIndex;
                policyData[i][1] = policyObj[i].getAttribute("name");
                policyData[i][1] = policyData[i][1].replace(/\"/g,"&#34;");
                //policyData[i][1] = policyData[i][1].replace(/~/g,"&#126;");
                //Profile Data..
                var profileObj = policyObj[i].getElementsByTagName("selectedProfile");
                var profileIndexList = new Array();
                for(var j=0;j<profileObj.length;j++){
                    profileIndexList[j] = profileObj[j].getAttribute("index");
                }
                policyData[i][2] = profileIndexList.join(",");
                policyData[i][3] = policyObj[i].getAttribute("inUse");
                if(policyData[i][3] == "" || policyData[i][3] == null || policyData[i][3]===undefined)
                    policyData[i][3] = 0;
                policyData[i][4] = "SAVED";

                strPolicyData[i] = policyData[i].join(gblSep2);
            }
            setValue("IPSEC_POLICY_DATA",strPolicyData.join(gblSep1));
        }
        function fnConstIPSecXML(){
            var ipsecXml = "";
            var initialIPSecSettings = evaluateXpath(gblDomObjArray[0],"Network/Protocols/IPSec");
            //Filters
            var orgFiltersNodeObj = gblDomObjArray[0].getElementsByTagName("Filters");
            var orgFilterNodeObj;
            if(orgFiltersNodeObj[0] != undefined && orgFiltersNodeObj[0] != null){
                orgFilterNodeObj = orgFiltersNodeObj[0].getElementsByTagName("Filter");
            }
            //Manual Key
            var orgManualKeysNodeObj = gblDomObjArray[0].getElementsByTagName("ManualKeys");
            var orgManualKeyNodeObj;
            if(orgManualKeysNodeObj[0] != undefined && orgManualKeysNodeObj[0] != null){
                orgManualKeyNodeObj = orgManualKeysNodeObj[0].getElementsByTagName("ManualKey");
            }
            //Ike
            var orgIKEsNodeObj = gblDomObjArray[0].getElementsByTagName("IKEKeys");
            var orgIKENodeObj;
            if(orgIKEsNodeObj[0] != undefined && orgIKEsNodeObj[0] != null){
                orgIKENodeObj = orgIKEsNodeObj[0].getElementsByTagName("IKEKey");
            }
            //Profile
            var orgProfilesNodeObj = gblDomObjArray[0].getElementsByTagName("Profiles");
            var orgProfileNodeObj;
            if(orgProfilesNodeObj[0] != undefined && orgProfilesNodeObj[0] != null){
                orgProfileNodeObj = orgProfilesNodeObj[0].getElementsByTagName("Profile");
            }
            //Policy
            var orgPoliciesNodeObj = gblDomObjArray[0].getElementsByTagName("Policies");
            var orgPolicyNodeObj;
            if(orgPoliciesNodeObj[0] != undefined && orgPoliciesNodeObj[0] != null){
                orgPolicyNodeObj = orgPoliciesNodeObj[0].getElementsByTagName("Policy");
            }

            var initialIpSecEnable = getXpathValue(initialIPSecSettings,"enabled");
            var ipSecEnable = getValue("IPSec_Enable");
            if(ipSecEnable != initialIpSecEnable){
                ipsecXml +="<enabled>"+ipSecEnable+"</enabled>";
            }
            var initialIpSecSelPolicy = getXpathValue(initialIPSecSettings,"selectedPolicy");
            var ipSecSelPolicy = getValue("Policy_Name");
            if(ipSecSelPolicy != initialIpSecSelPolicy){
                ipsecXml +="<selectedPolicy>"+fnConvertXMLEntities(ipSecSelPolicy)+"</selectedPolicy>";
            }
            ipsecXml += fnConstIPSecFilterXML(orgFilterNodeObj)+ fnConstIPSecManualKeyXML(orgManualKeyNodeObj)+fnConstIPSecIKEXML(orgIKENodeObj)+ fnConstIPSecProfileXML(orgProfileNodeObj)+ fnConstIPSecPolicyXML(orgPolicyNodeObj);
            if(ipsecXml != "")
                ipsecXml = "<IPSec>"+ipsecXml+"</IPSec>" ;
            setValue("IPSecCmdXml",ipsecXml);
        }
        function fnConstIPSecFilterXML(orgFilterNodeObj){

            var finalFilterXml = "";
            var filterCount = 0;
            var orgFilterNode = null;
            var eachFilterdata;
            var filterData = getValue("IPSECFILTERDATA");
            if(filterData == ""){
                filterCount = 0;
            }else{
                filterCount = filterData.split(gblSep1).length;
            }
            filterData = filterData.split(gblSep1);
            var ipsecFilterXml = "";
            ipsecFilterXml += fnConstDelFilterStr();
            for(var i=0;i<filterData.length;i++){
                if(filterData[i] == ""){
                    continue;
                }
                eachFilterdata = filterData[i].split(gblSep2);
                if(eachFilterdata[14] == "SAVED"){ // modifying the existing Filters..
                    for(var j=0;j<orgFilterNodeObj.length;j++){
                        if(orgFilterNodeObj[j].getAttribute("index") == eachFilterdata[0]){
                            orgFilterNode = orgFilterNodeObj[j];
                            break;
                        }
                    }
                    if(orgFilterNode == null){
                        continue;
                    }
                    var eachFilterXml = "";
                    var eachFilterAddrXml = "";
                    var eachFilterSrcPortXml = "";
                    var eachFilterDestPortXml = "";

                    var filterProt = getXpathValue(orgFilterNode,"inetProtocolVersion");
                    if(filterProt != eachFilterdata[2]){
                        eachFilterXml += '<inetProtocolVersion>'+eachFilterdata[2]+'</inetProtocolVersion>';
                    }
                    var addrType = getXpathValue(orgFilterNode,"destinationAddress/addressType");
                    var address = getXpathValue(orgFilterNode,"destinationAddress/address");
                    if(addrType != eachFilterdata[4]){
                        eachFilterAddrXml += '<addressType>'+eachFilterdata[4]+'</addressType>';
                    }
                    if(address != eachFilterdata[5]){
                        eachFilterAddrXml +='<address>'+eachFilterdata[5]+'</address>';
                    }
                    if(eachFilterAddrXml != ""){
                        eachFilterXml += "<destinationAddress>"+eachFilterAddrXml+"</destinationAddress>"
                    }
                    var protocolType = getXpathValue(orgFilterNode,"protocolType");
                    if(protocolType  != eachFilterdata[6]){
                        eachFilterXml += '<protocolType>'+eachFilterdata[6]+'</protocolType>';
                    }
                    var srcPortType = getXpathValue(orgFilterNode,"sourcePort/portType");
                    var srcPortNo= getXpathValue(orgFilterNode,"sourcePort/portNo");
                    if(srcPortType != eachFilterdata[7]){
                        eachFilterSrcPortXml += '<portType>'+eachFilterdata[7]+'</portType>';
                    }
                    if(srcPortNo != eachFilterdata[8]){
                        eachFilterSrcPortXml +='<portNo>'+eachFilterdata[8]+'</portNo>';
                    }
                    if(eachFilterSrcPortXml != ""){
                        eachFilterXml += "<sourcePort>"+eachFilterSrcPortXml+"</sourcePort>"
                    }
                    var destPortType = getXpathValue(orgFilterNode,"destinationPort/portType");
                    var destPortNo= getXpathValue(orgFilterNode,"destinationPort/portNo");
                    if(destPortType != eachFilterdata[9]){
                        eachFilterDestPortXml += '<portType>'+eachFilterdata[9]+'</portType>';
                    }
                    if(destPortNo != eachFilterdata[10]){
                        eachFilterDestPortXml +='<portNo>'+eachFilterdata[10]+'</portNo>';
                    }
                    if(eachFilterDestPortXml != ""){
                        eachFilterXml += "<destinationPort>"+eachFilterDestPortXml+"</destinationPort>"
                    }
                    var filtAction = getXpathValue(orgFilterNode,"filterAction");
                    if(eachFilterdata[11] == "Negotiate Security")
                        eachFilterdata[11] = 'Negotiate'+eachFilterdata[12];
                    if(filtAction != eachFilterdata[11])
                        eachFilterXml += '<filterAction>'+eachFilterdata[11]+'</filterAction>';
                    var orgFilterName = orgFilterNode.getAttribute("name");
                    var orgFilterInUse = orgFilterNode.getAttribute("inUse");
                    eachFilterdata[1] = eachFilterdata[1].replace(/(&#34;)/g,'\"');
                    //eachFilterdata[1] = eachFilterdata[1].replace(/(&#126;)/g,'~');
                    if(eachFilterXml != ""){
                        ipsecFilterXml += '<Filter markToDelete="0" inUse="'+eachFilterdata[13]+'" index="'+eachFilterdata[0]+'" name="'+fnConvertXMLEntities(eachFilterdata[1])+'">'+eachFilterXml+'</Filter>' ;
                    }else if(orgFilterName != eachFilterdata[1] || orgFilterInUse != eachFilterdata[13]){
                        ipsecFilterXml += '<Filter markToDelete="0" inUse="'+eachFilterdata[13]+'" index="'+eachFilterdata[0]+'" name="'+fnConvertXMLEntities(eachFilterdata[1])+'"/>' ;
                    }
                }else if(eachFilterdata[14] == "NEW"){ // For New Filters...
                    eachFilterdata[1] = eachFilterdata[1].replace(/(&#34;)/g,'\"');
                    //eachFilterdata[1] = eachFilterdata[1].replace(/(&#126;)/g,'~');
                    ipsecFilterXml += '<Filter markToDelete="0" inUse="'+eachFilterdata[13]+'" index="" name="'+fnConvertXMLEntities(eachFilterdata[1])+'">' ;
                    ipsecFilterXml += '<inetProtocolVersion>'+eachFilterdata[2]+'</inetProtocolVersion>';
                    ipsecFilterXml += '<destinationAddress><addressType>'+eachFilterdata[4]+'</addressType><address>'+eachFilterdata[5]+'</address></destinationAddress>';
                    ipsecFilterXml += '<protocolType>'+eachFilterdata[6]+'</protocolType>';
                    ipsecFilterXml += '<sourcePort><portType>'+eachFilterdata[7]+'</portType><portNo>'+eachFilterdata[8]+'</portNo></sourcePort>';
                    ipsecFilterXml += '<destinationPort><portType>'+eachFilterdata[9]+'</portType><portNo>'+eachFilterdata[10]+'</portNo></destinationPort>';
                    if(eachFilterdata[11] == "Negotiate Security")
                        ipsecFilterXml += '<filterAction>Negotiate'+eachFilterdata[12]+'</filterAction>';
                    else
                        ipsecFilterXml += '<filterAction>'+eachFilterdata[11]+'</filterAction>';
                    ipsecFilterXml += "</Filter>"
                }
            }
            if(ipsecFilterXml != ""){
                finalFilterXml = "<Filters>"+ipsecFilterXml+"</Filters>";
            }

            return finalFilterXml;
        }
        function fnConstDelFilterStr(){
            var finalDelFilterStr = "";
            var delFilterData = getValue("IPSEC_DELETED_FILTERS");
            if(delFilterData == "" || delFilterData == null || delFilterData === undefined){
                return "";
            }
            delFilterData = delFilterData.split(gblSep1);
            for(var i=0;i<delFilterData.length;i++){
                var eachFilterData = delFilterData[i].split(gblSep2);
                eachFilterData[1] = eachFilterData[1].replace(/(&#34;)/g,'\"');
                //eachFilterData[1] = eachFilterData[1].replace(/(&#126;)/g,'~');
                finalDelFilterStr += '<Filter markToDelete="1" inUse="0" index="'+eachFilterData[0]+'" name="'+fnConvertXMLEntities(eachFilterData[1])+'" />' ;
            }
            return finalDelFilterStr ;
        }
        function fnConstIPSecManualKeyXML(orgManualKeyNodeObj){
            var finalManualKeyXml = "";
            var manualkeyCount = 0;
            var orgManualKeyNode = null;
            var eachManualKeydata;
            var manualKeyData = getValue("IPSEC_MANUAL_DATA");
            if(manualKeyData == ""){
                manualkeyCount = 0;
            }else{
                manualkeyCount = manualKeyData.split(gblSep1).length;
            }
            manualKeyData = manualKeyData.split(gblSep1);
            var ipsecManualKeyXml = "";
            ipsecManualKeyXml += fnConstDelManualKeyStr();
            for(var i=0;i<manualKeyData.length;i++){
                if(manualKeyData[i] == ""){
                    continue;
                }
                eachManualKeydata = manualKeyData[i].split(gblSep2);
                if(eachManualKeydata[13] == "SAVED"){ // modifying the existing Manual Keys..
                    for(var j=0;j<orgManualKeyNodeObj.length;j++){
                        if(orgManualKeyNodeObj[j].getAttribute("index") == eachManualKeydata[0]){
                            orgManualKeyNode = orgManualKeyNodeObj[j];
                            break;
                        }
                    }
                    if(orgManualKeyNode == null){
                        continue;
                    }
                    var eachManualKeyXml = "";
                    var eachManualKeyInBoundXml = "";
                    var eachManualKeyOutBoundXml = "";

                    var encryptAlg = getXpathValue(orgManualKeyNode,"EncryptionAlgorithm");
                    if(encryptAlg != eachManualKeydata[2]){
                        eachManualKeyXml += '<EncryptionAlgorithm>'+eachManualKeydata[2]+'</EncryptionAlgorithm>';
                    }
                    var integrityAlg = getXpathValue(orgManualKeyNode,"IntegrityAlgorithm");
                    if(integrityAlg != eachManualKeydata[3]){
                        eachManualKeyXml += '<IntegrityAlgorithm>'+eachManualKeydata[3]+'</IntegrityAlgorithm>';
                    }
                    var inboundSPI = getXpathValue(orgManualKeyNode,"InboundKey/spi");
                    if(inboundSPI != eachManualKeydata[4]){
                        eachManualKeyInBoundXml += '<spi>'+eachManualKeydata[4]+'</spi>';
                    }
                    var inboundESPEncryptionKey = getXpathValue(orgManualKeyNode,"InboundKey/ESPEncryKey");
                    if(inboundESPEncryptionKey != eachManualKeydata[5]){
                        eachManualKeyInBoundXml += '<ESPEncryKey>'+fnConvertXMLEntities(eachManualKeydata[5])+'</ESPEncryKey>';
                    }
                    var inboundESPAuthKey = getXpathValue(orgManualKeyNode,"InboundKey/ESPAuthKey");
                    if(inboundESPAuthKey != eachManualKeydata[6]){
                        eachManualKeyInBoundXml += '<ESPAuthKey>'+fnConvertXMLEntities(eachManualKeydata[6])+'</ESPAuthKey>';
                    }
                    var inboundAHAuthKey = getXpathValue(orgManualKeyNode,"InboundKey/AHAuthKey");
                    if(inboundAHAuthKey != eachManualKeydata[7]){
                        eachManualKeyInBoundXml += '<AHAuthKey>'+fnConvertXMLEntities(eachManualKeydata[7])+'</AHAuthKey>';
                    }

                    if(eachManualKeyInBoundXml != ""){
                        eachManualKeyXml += "<InboundKey>"+eachManualKeyInBoundXml+"</InboundKey>";
                    }
                    var outboundSPI = getXpathValue(orgManualKeyNode,"OutboundKey/spi");
                    if(outboundSPI != eachManualKeydata[8]){
                        eachManualKeyOutBoundXml += '<spi>'+eachManualKeydata[8]+'</spi>';
                    }
                    var outboundESPEncryptionKey = getXpathValue(orgManualKeyNode,"OutboundKey/ESPEncryKey");
                    if(outboundESPEncryptionKey != eachManualKeydata[9]){
                        eachManualKeyOutBoundXml += '<ESPEncryKey>'+fnConvertXMLEntities(eachManualKeydata[9])+'</ESPEncryKey>';
                    }
                    var outboundESPAuthKey = getXpathValue(orgManualKeyNode,"OutboundKey/ESPAuthKey");
                    if(outboundESPAuthKey != eachManualKeydata[10]){
                        eachManualKeyOutBoundXml += '<ESPAuthKey>'+fnConvertXMLEntities(eachManualKeydata[10])+'</ESPAuthKey>';
                    }
                    var outboundAHAuthKey = getXpathValue(orgManualKeyNode,"OutboundKey/AHAuthKey");
                    if(outboundAHAuthKey != eachManualKeydata[11]){
                        eachManualKeyOutBoundXml += '<AHAuthKey>'+fnConvertXMLEntities(eachManualKeydata[11])+'</AHAuthKey>';
                    }

                    if(eachManualKeyOutBoundXml != ""){
                        eachManualKeyXml += "<OutboundKey>"+eachManualKeyOutBoundXml+"</OutboundKey>";
                    }
                    eachManualKeydata[1] = eachManualKeydata[1].replace(/(&#34;)/g,'\"');
                    //eachManualKeydata[1] = eachManualKeydata[1].replace(/(&#126;)/g,'~');
                    var orgManualKeyName = orgManualKeyNode.getAttribute("name");
                    var orgManualKeyInUse = orgManualKeyNode.getAttribute("inUse");
                    if(eachManualKeyXml != ""){
                        ipsecManualKeyXml += '<ManualKey markToDelete="0" inUse="'+eachManualKeydata[12]+'" index="'+eachManualKeydata[0]+'" name="'+fnConvertXMLEntities(eachManualKeydata[1])+'">'+eachManualKeyXml+'</ManualKey>' ;
                    }else if(orgManualKeyName != eachManualKeydata[1] || orgManualKeyInUse != eachManualKeydata[12]){
                        ipsecManualKeyXml += '<ManualKey markToDelete="0" inUse="'+eachManualKeydata[12]+'" index="'+eachManualKeydata[0]+'" name="'+fnConvertXMLEntities(eachManualKeydata[1])+'" />' ;
                    }

                }else if(eachManualKeydata[13] == "NEW"){ // For New ManualKeys...
                    eachManualKeydata[1] = eachManualKeydata[1].replace(/(&#34;)/g,'\"');
                    //eachManualKeydata[1] = eachManualKeydata[1].replace(/(&#126;)/g,'~');
                    ipsecManualKeyXml += '<ManualKey markToDelete="0" inUse="'+eachManualKeydata[12]+'" index="" name="'+fnConvertXMLEntities(eachManualKeydata[1])+'">' ;
                    ipsecManualKeyXml += '<EncryptionAlgorithm>'+eachManualKeydata[2]+'</EncryptionAlgorithm>';
                    ipsecManualKeyXml += '<IntegrityAlgorithm>'+eachManualKeydata[3]+'</IntegrityAlgorithm>';
                    ipsecManualKeyXml += '<InboundKey>';
                    ipsecManualKeyXml += '<spi>'+eachManualKeydata[4]+'</spi>';
                    ipsecManualKeyXml += '<ESPEncryKey>'+fnConvertXMLEntities(eachManualKeydata[5])+'</ESPEncryKey>';
                    ipsecManualKeyXml += '<ESPAuthKey>'+fnConvertXMLEntities(eachManualKeydata[6])+'</ESPAuthKey>';
                    ipsecManualKeyXml += '<AHAuthKey>'+fnConvertXMLEntities(eachManualKeydata[7])+'</AHAuthKey>';
                    ipsecManualKeyXml += '</InboundKey>';
                    ipsecManualKeyXml += '<OutboundKey>';
                    ipsecManualKeyXml += '<spi>'+eachManualKeydata[8]+'</spi>';
                    ipsecManualKeyXml += '<ESPEncryKey>'+fnConvertXMLEntities(eachManualKeydata[9])+'</ESPEncryKey>';
                    ipsecManualKeyXml += '<ESPAuthKey>'+fnConvertXMLEntities(eachManualKeydata[10])+'</ESPAuthKey>';
                    ipsecManualKeyXml += '<AHAuthKey>'+fnConvertXMLEntities(eachManualKeydata[11])+'</AHAuthKey>';
                    ipsecManualKeyXml += '</OutboundKey>';
                    ipsecManualKeyXml += "</ManualKey>"
                }
            }
            if(ipsecManualKeyXml != ""){
                finalManualKeyXml = "<ManualKeys>"+ipsecManualKeyXml+"</ManualKeys>";
            }
            return finalManualKeyXml;
        }
        function fnConstDelManualKeyStr(){
            var finalDelManualKeyStr = "";
            var delManualKeyData = getValue("IPSEC_DELETED_MANUAL");
            if(delManualKeyData == "" || delManualKeyData == null || delManualKeyData === undefined){
                return "";
            }
            delManualKeyData = delManualKeyData.split(gblSep1);
            for(var i=0;i<delManualKeyData.length;i++){
                var eachManualKeydata = delManualKeyData[i].split(gblSep2);
                eachManualKeydata[1] = eachManualKeydata[1].replace(/(&#34;)/g,'\"');
                //eachManualKeydata[1] = eachManualKeydata[1].replace(/(&#126;)/g,'~');
                finalDelManualKeyStr += '<ManualKey markToDelete="1" inUse="0" index="'+eachManualKeydata[0]+'" name="'+fnConvertXMLEntities(eachManualKeydata[1])+'" />' ;
            }
            return finalDelManualKeyStr ;
        }
  
        function fnConstIPSecIKEXML(orgIKENodeObj){
            var finalIKEXml = "";
            var ikeCount = 0;
            var orgIKENode = null;
            var eachIKEdata;
            var ikeData = getValue("IPSECIKEDATA");
            if(ikeData == ""){
                ikeCount = 0;
            }else{
                ikeCount = ikeData.split(gblSep1).length;
            }
            ikeData = ikeData.split(gblSep1);
            var ipsecIKEXml = "";
            ipsecIKEXml += fnConstDelIKEStr();
            for(var i=0;i<ikeData.length;i++){
                if(ikeData[i] == ""){
                    continue;
                }
                eachIKEdata = ikeData[i].split(gblSep2);
                if(eachIKEdata[23] == "SAVED"){ // modifying the existing Filters..
                    for(var j=0;j<orgIKENodeObj.length;j++){
                        if(orgIKENodeObj[j].getAttribute("index") == eachIKEdata[0]){
                            orgIKENode = orgIKENodeObj[j];
                            break;
                        }
                    }
                    if(orgIKENode == null){
                        continue;
                    }
                    var eachIKEXml = "";

                    var ikeVersion = getXpathValue(orgIKENode,"version");
                    if(ikeVersion != eachIKEdata[2]){
                        eachIKEXml += '<version>'+eachIKEdata[2]+'</version>';
                    }
                    var ikeAuthType = getXpathValue(orgIKENode,"authenticationMethod");
                    if(ikeVersion == "IKEv1"){
                        if(ikeAuthType != eachIKEdata[3]){
                            eachIKEXml += '<authenticationMethod>'+eachIKEdata[3]+'</authenticationMethod>';
                        }
                    }else if(ikeVersion == "IKEv2"){
                        if(ikeAuthType != eachIKEdata[5]){
                            eachIKEXml += '<authenticationMethod>'+eachIKEdata[5]+'</authenticationMethod>';
                        }
                    }

                    var preHashKey = getXpathValue(orgIKENode,"preHashKey");
                    if(preHashKey != eachIKEdata[4]){
                        eachIKEdata[4] = eachIKEdata[4].replace(/(&#126;)/g,"~");
                        eachIKEXml += '<preHashKey>'+eachIKEdata[4]+'</preHashKey>';
                    }
                    var srcIdType = getXpathValue(orgIKENode,"srcID");
                    if(srcIdType != eachIKEdata[6]){
                        eachIKEXml += '<srcID>'+eachIKEdata[6]+'</srcID>';
                    }
                    var srcId = getXpathValue(orgIKENode,"srcIDVal");
                    if(srcId != eachIKEdata[7]){
                        eachIKEXml += '<srcIDVal>'+eachIKEdata[7]+'</srcIDVal>';
                    }
                    var remoteIdType = getXpathValue(orgIKENode,"remoteID");
                    if(remoteIdType != eachIKEdata[8]){
                        eachIKEXml += '<remoteID>'+eachIKEdata[8]+'</remoteID>';
                    }
                    var remoteId = getXpathValue(orgIKENode,"remoteIDVal");
                    if(remoteId != eachIKEdata[9]){
                        eachIKEXml += '<remoteIDVal>'+eachIKEdata[9]+'</remoteIDVal>';
                    }
                    var newKeyDelay = getXpathValue(orgIKENode,"generateNewKeyDelay");
                    if(newKeyDelay != eachIKEdata[10]){
                        eachIKEXml += '<generateNewKeyDelay>'+eachIKEdata[10]+'</generateNewKeyDelay>';
                    }
                    var enablePFS = getXpathValue(orgIKENode,"enablePFS");
                    if(enablePFS != eachIKEdata[11]){
                        eachIKEXml += '<enablePFS>'+eachIKEdata[11]+'</enablePFS>';
                    }

                    var integrityXml = "";
                    var enableAES = getXpathValue(orgIKENode,"enableAESXCBC");
                    if(enableAES != eachIKEdata[14]){
                        integrityXml += '<enableAESXCBC>'+eachIKEdata[14]+'</enableAESXCBC>';
                    }
                    var enableSHA1 = getXpathValue(orgIKENode,"enableSHA1");
                    if(enableSHA1 != eachIKEdata[12]){
                        integrityXml += '<enableSHA1>'+eachIKEdata[12]+'</enableSHA1>';
                    }
                    var enableMD5 = getXpathValue(orgIKENode,"enableMD5");
                    if(enableMD5 != eachIKEdata[13]){
                        integrityXml += '<enableMD5>'+eachIKEdata[13]+'</enableMD5>';
                    }
                    if(integrityXml != ""){
                        integrityXml = "<Integrity>"+integrityXml+"</Integrity>";
                    }
                    
                    var encryptXml = "";
                    var enableAESCTR = getXpathValue(orgIKENode,"enableAESCTR");
                    if(enableAESCTR != eachIKEdata[18]){
                        encryptXml += '<enableAESCTR>'+eachIKEdata[18]+'</enableAESCTR>';
                    }
                    var enableAES256 = getXpathValue(orgIKENode,"enableAES256CBC");
                    if(enableAES256 != eachIKEdata[15]){
                        encryptXml += '<enableAES256CBC>'+eachIKEdata[15]+'</enableAES256CBC>';
                    }
                    var enableAES192 = getXpathValue(orgIKENode,"enableAES192CBC");
                    if(enableAES192 != eachIKEdata[16]){
                        encryptXml += '<enableAES192CBC>'+eachIKEdata[16]+'</enableAES192CBC>';
                    }
                    var enableAES128 = getXpathValue(orgIKENode,"enableAES128CBC");
                    if(enableAES128 != eachIKEdata[17]){
                        encryptXml += '<enableAES128CBC>'+eachIKEdata[17]+'</enableAES128CBC>';
                    }
                    var enable3DES = getXpathValue(orgIKENode,"enable3DESCBC");
                    if(enable3DES != eachIKEdata[19]){
                        encryptXml += '<enable3DESCBC>'+eachIKEdata[19]+'</enable3DESCBC>';
                    }
                    var enableDES = getXpathValue(orgIKENode,"enableDESCBC");
                    if(enableDES != eachIKEdata[20]){
                        encryptXml += '<enableDESCBC>'+eachIKEdata[20]+'</enableDESCBC>';
                    }
                    if(encryptXml != ""){
                        encryptXml = "<Encryption>"+encryptXml+"</Encryption>";
                    }
                    var difHelmanAlgXml = "";
                    var difHelmanAlg = getXpathValue(orgIKENode,"diffieHellmanAlgorithm");
                    if(difHelmanAlg != eachIKEdata[21]){
                        difHelmanAlgXml += '<diffieHellmanAlgorithm>'+eachIKEdata[21]+'</diffieHellmanAlgorithm>';
                    }
                    var transformXml = "";
                    transformXml = integrityXml+encryptXml+difHelmanAlgXml;
                    if(transformXml != ""){
                        eachIKEXml += "<Transformations>"+transformXml+"</Transformations>";
                    }
                    eachIKEdata[1] = eachIKEdata[1].replace(/(&#34;)/g,'\"');
                    //eachIKEdata[1] = eachIKEdata[1].replace(/(&#126;)/g,'~');
                    var orgIKEName = orgIKENode.getAttribute("name");
                    var orgIKEInUse = orgIKENode.getAttribute("inUse");
                    if(eachIKEXml != ""){
                        ipsecIKEXml += '<IKEKey markToDelete="0" inUse="'+eachIKEdata[22]+'" index="'+eachIKEdata[0]+'" name="'+fnConvertXMLEntities(eachIKEdata[1])+'">'+eachIKEXml+'</IKEKey>' ;
                    }else if(orgIKEName != eachIKEdata[1] || orgIKEInUse != eachIKEdata[22]){
                        ipsecIKEXml += '<IKEKey markToDelete="0" inUse="'+eachIKEdata[22]+'" index="'+eachIKEdata[0]+'" name="'+fnConvertXMLEntities(eachIKEdata[1])+'" />' ;
                    }
                }else if(eachIKEdata[23] == "NEW"){ // For New IKE Keys...
                    eachIKEdata[1] = eachIKEdata[1].replace(/(&#34;)/g,'\"');
                    //eachIKEdata[1] = eachIKEdata[1].replace(/(&#126;)/g,'~');
                    ipsecIKEXml += '<IKEKey markToDelete="0" inUse="'+eachIKEdata[22]+'" index="" name="'+fnConvertXMLEntities(eachIKEdata[1])+'">' ;
                    var version = eachIKEdata[2];
                    ipsecIKEXml += '<version>'+eachIKEdata[2]+'</version>';
                    if(version == "IKEv1"){
                        ipsecIKEXml += '<authenticationMethod>'+eachIKEdata[3]+'</authenticationMethod>';
                    }else if(version == "IKEv2"){
                        ipsecIKEXml += '<authenticationMethod>'+eachIKEdata[5]+'</authenticationMethod>';
                    }
                    eachIKEdata[4] = eachIKEdata[4].replace(/(&#126;)/g,"~");
                    ipsecIKEXml += '<preHashKey>'+eachIKEdata[4]+'</preHashKey>';
                    ipsecIKEXml += '<srcID>'+eachIKEdata[6]+'</srcID>';
                    ipsecIKEXml += '<srcIDVal>'+eachIKEdata[7]+'</srcIDVal>';
                    ipsecIKEXml += '<remoteID>'+eachIKEdata[8]+'</remoteID>';
                    ipsecIKEXml += '<remoteIDVal>'+eachIKEdata[9]+'</remoteIDVal>';
                    ipsecIKEXml += '<generateNewKeyDelay>'+eachIKEdata[10]+'</generateNewKeyDelay>';
                    ipsecIKEXml += '<enablePFS>'+eachIKEdata[11]+'</enablePFS>';
                    ipsecIKEXml += '<Transformations><Integrity><enableAESXCBC>'+eachIKEdata[14]+'</enableAESXCBC>' +
                                  '<enableSHA1>'+eachIKEdata[12]+'</enableSHA1>' +
                                  '<enableMD5>'+eachIKEdata[13]+'</enableMD5></Integrity>';
                    ipsecIKEXml += '<Encryption><enableAESCTR>'+eachIKEdata[18]+'</enableAESCTR>' +
                                  '<enableAES256CBC>'+eachIKEdata[15]+'</enableAES256CBC>' +
                                  '<enableAES192CBC>'+eachIKEdata[16]+'</enableAES192CBC>' +
                                  '<enableAES128CBC>'+eachIKEdata[17]+'</enableAES128CBC>' +
                                  '<enable3DESCBC>'+eachIKEdata[19]+'</enable3DESCBC>' +
                                  '<enableDESCBC>'+eachIKEdata[20]+'</enableDESCBC>' +
                                  '</Encryption>';
                    ipsecIKEXml += '<diffieHellmanAlgorithm>'+eachIKEdata[21]+'</diffieHellmanAlgorithm></Transformations>';
                    ipsecIKEXml += '</IKEKey>';
                }
            }
            if(ipsecIKEXml != ""){
                finalIKEXml = "<IKEKeys>"+ipsecIKEXml+"</IKEKeys>";
            }
            return finalIKEXml;
        }
        function fnConstDelIKEStr(){
            var finalDelIKEStr = "";
            var delIKEData = getValue("IPSEC_DELETED_IKE");
            if(delIKEData == "" || delIKEData == null || delIKEData === undefined){
                return "";
            }
            delIKEData = delIKEData.split(gblSep1);
            for(var i=0;i<delIKEData.length;i++){
                var eachIKEData = delIKEData[i].split(gblSep2);
                eachIKEData[1] = eachIKEData[1].replace(/(&#34;)/g,'\"');
                //eachIKEData[1] = eachIKEData[1].replace(/(&#126;)/g,'~');
                finalDelIKEStr += '<IKEKey markToDelete="1" inUse="0" index="'+eachIKEData[0]+'" name="'+fnConvertXMLEntities(eachIKEData[1])+'" />' ;
            }
            return finalDelIKEStr ;
        }
        function fnConstIPSecProfileXML(orgProfileNodeObj){
            var finalProfileXml = "";
            var profileCount = 0;
            var orgProfileNode = null;
            var eachProfiledata;
            var profileData = getValue("IPSEC_PROFILE_DATA");
            var filterData = getValue("IPSECFILTERDATA");
            filterData = filterData.split(gblSep1);
            if(profileData == ""){
                profileCount = 0;
            }else{
                profileCount = profileData.split(gblSep1).length;
            }
            profileData = profileData.split(gblSep1);
            var ipsecProfileXml = "";
            ipsecProfileXml += fnConstDelProfileStr();
            for(var i=0;i<profileData.length;i++){
                if(profileData[i] == ""){
                    continue;
                }
                eachProfiledata = profileData[i].split(gblSep2);
                if(eachProfiledata[25] == "SAVED"){ // modifying the existing Profiles..
                    for(var j=0;j<orgProfileNodeObj.length;j++){
                        if(orgProfileNodeObj[j].getAttribute("index") == eachProfiledata[0]){
                            orgProfileNode = orgProfileNodeObj[j];
                            break;
                        }
                    }
                    if(orgProfileNode == null){
                        continue;
                    }
                    var eachProfileXml = "";

                    var tunnelMode = getXpathValue(orgProfileNode,"tunnelMode");
                    if(tunnelMode != eachProfiledata[2]){
                        eachProfileXml += '<tunnelMode>'+eachProfiledata[2]+'</tunnelMode>';
                    }
                    var inetAddr = getXpathValue(orgProfileNode,"inetAddress");
                    if(inetAddr != eachProfiledata[3]){
                        eachProfileXml += '<inetAddress>'+eachProfiledata[3]+'</inetAddress>';
                    }

                    var keySelected = getXpathValue(orgProfileNode,"keySelected");
                    var keyType = orgProfileNode.getElementsByTagName("keySelected")[0].getAttribute("keyType");
                    var arrKey = eachProfiledata[4].split("#");
                    if(keySelected != arrKey[0] || keyType != arrKey[1]){
                        eachProfileXml += '<keySelected keyType="'+arrKey[1]+'">'+fnConvertXMLEntities(arrKey[0])+'</keySelected>';
                    }

                    var espTransformXml = "";
                    var enableSHA1 = getXpathValue(orgProfileNode,"enabledSHA1");
                    if(enableSHA1 != eachProfiledata[5]){
                        espTransformXml += '<enabledSHA1>'+eachProfiledata[5]+'</enabledSHA1>';
                    }
                    var enableMD5 = getXpathValue(orgProfileNode,"enableMD5");
                    if(enableMD5 != eachProfiledata[6]){
                        espTransformXml += '<enableMD5>'+eachProfiledata[6]+'</enableMD5>';
                    }
                    var enableAESXCBC = getXpathValue(orgProfileNode,"enableAESXCBC");
                    if(enableAESXCBC != eachProfiledata[7]){
                        espTransformXml += '<enableAESXCBC>'+eachProfiledata[7]+'</enableAESXCBC>';
                    }
                    var enableAES256CBC = getXpathValue(orgProfileNode,"enableAES256CBC");
                    if(enableAES256CBC != eachProfiledata[8]){
                        espTransformXml += '<enableAES256CBC>'+eachProfiledata[8]+'</enableAES256CBC>';
                    }
                    var enableAES192CBC = getXpathValue(orgProfileNode,"enableAES192CBC");
                    if(enableAES192CBC != eachProfiledata[9]){
                        espTransformXml += '<enableAES192CBC>'+eachProfiledata[9]+'</enableAES192CBC>';
                    }
                    var enableAES128CBC = getXpathValue(orgProfileNode,"enableAES128CBC");
                    if(enableAES128CBC != eachProfiledata[10]){
                        espTransformXml += '<enableAES128CBC>'+eachProfiledata[10]+'</enableAES128CBC>';
                    }
                    var enableAESCTR = getXpathValue(orgProfileNode,"enableAESCTR");
                    if(enableAESCTR != eachProfiledata[11]){
                        espTransformXml += '<enableAESCTR>'+eachProfiledata[11]+'</enableAESCTR>';
                    }
                    var enable3DESCBC = getXpathValue(orgProfileNode,"enable3DESCBC");
                    if(enable3DESCBC != eachProfiledata[12]){
                        espTransformXml += '<enable3DESCBC>'+eachProfiledata[12]+'</enable3DESCBC>';
                    }
                    var enableDESCBC = getXpathValue(orgProfileNode,"enableDESCBC");
                    if(enableDESCBC != eachProfiledata[13]){
                        espTransformXml += '<enableDESCBC>'+eachProfiledata[13]+'</enableDESCBC>';
                    }
                    var noEncryption = getXpathValue(orgProfileNode,"noEncryption");
                    if(noEncryption != eachProfiledata[14]){
                        espTransformXml += '<noEncryption>'+eachProfiledata[14]+'</noEncryption>';
                    }
                    if(espTransformXml != ""){
                        espTransformXml = "<ESPTransformation>"+espTransformXml+"</ESPTransformation>";
                    }

                    var ahTransformXml = "";
                    var enableSH1 = getXpathValue(orgProfileNode,"enableSH1");
                    if(enableSH1 != eachProfiledata[15]){
                        ahTransformXml += '<enableSH1>'+eachProfiledata[15]+'</enableSH1>';
                    }
                    var enableAHMD5 = getXpathValue(orgProfileNode,"enableMD5");
                    if(enableAHMD5 != eachProfiledata[16]){
                        ahTransformXml += '<enableMD5>'+eachProfiledata[16]+'</enableMD5>';
                    }
                    var enableAHAESXCBC = getXpathValue(orgProfileNode,"enableAESXCBC");
                    if(enableAHAESXCBC != eachProfiledata[17]){
                        ahTransformXml += '<enableAESXCBC>'+eachProfiledata[17]+'</enableAESXCBC>';
                    }
                    if(ahTransformXml != ""){
                        ahTransformXml = "<AHTransformation>"+ahTransformXml+"</AHTransformation>";
                    }

                    var proposalXml = "";

                    var keySize = getXpathValue(orgProfileNode,"sessionKeyGenTimeInterval");
                    if(keySize != eachProfiledata[19]){
                        proposalXml += '<sessionKeyGenTimeInterval>'+eachProfiledata[19]+'</sessionKeyGenTimeInterval>';
                    }
                    var enableTimeInterval = getXpathValue(orgProfileNode,"enableTimeInterval");
                    if(enableTimeInterval != eachProfiledata[18]){
                        proposalXml += '<enableTimeInterval>'+eachProfiledata[18]+'</enableTimeInterval>';
                    }
                    var keyTime = getXpathValue(orgProfileNode,"sessionKeyGenSizeInterval");
                    if(keyTime != eachProfiledata[21]){
                        proposalXml += '<sessionKeyGenSizeInterval>'+eachProfiledata[21]+'</sessionKeyGenSizeInterval>';
                    }
                    var enableSizeInterval = getXpathValue(orgProfileNode,"enableSizeInterval");
                    if(enableSizeInterval != eachProfiledata[20]){
                        proposalXml += '<enableSizeInterval>'+eachProfiledata[20]+'</enableSizeInterval>';
                    }
                    var transformIPCOMP = getXpathValue(orgProfileNode,"IPCOMPTransform");
                    if(transformIPCOMP != eachProfiledata[22]){
                        proposalXml += '<IPCOMPTransform>'+eachProfiledata[22]+'</IPCOMPTransform>';
                    }
                    proposalXml += espTransformXml+ahTransformXml;

                    if(proposalXml != ""){
                        eachProfileXml += "<Proposals>"+proposalXml+"</Proposals>";
                    }
                    // Filter Data..
                    var filterXml = "";
                    if(eachProfiledata[23] != "") {
                        var profileFilterIndexArr = eachProfiledata[23].split(",");
                        for(var k=0;k<profileFilterIndexArr.length;k++){
                            if(profileFilterIndexArr[k] == "")
                                continue;
                            var filterIndex = 0;
                            for(var j=0;j<filterData.length;j++){
                                if(profileFilterIndexArr[k] ==filterData[j].split(gblSep2)[0]){
                                    filterIndex = j;
                                    break;
                                }
                            }
                            var reqdFilterArr = filterData[filterIndex];
                            reqdFilterArr = reqdFilterArr.split(gblSep2);
                            reqdFilterArr[1] = reqdFilterArr[1].replace(/(&#34;)/g,'\"');
                            //reqdFilterArr[1] = reqdFilterArr[1].replace(/(&#126;)/g,'~');
                            if(reqdFilterArr[14] == "NEW")
                                filterXml += '<Filter index="" name="'+fnConvertXMLEntities(reqdFilterArr[1])+'" />';
                            else if(reqdFilterArr[14] == "SAVED")
                                filterXml += '<Filter index="'+reqdFilterArr[0]+'" name="'+fnConvertXMLEntities(reqdFilterArr[1])+'" />';
                        }

                    }
                    if(filterXml != "")
                        eachProfileXml += "<Filters>"+filterXml+"</Filters>";
                    eachProfiledata[1] = eachProfiledata[1].replace(/(&#34;)/g,'\"');
                    //eachProfiledata[1] = eachProfiledata[1].replace(/(&#126;)/g,'~');
                    var orgProfileName = orgProfileNode.getAttribute("name");
                    var orgProfileInUse = orgProfileNode.getAttribute("inUse");
                    if(eachProfileXml != ""){
                        ipsecProfileXml += '<Profile markToDelete="0" inUse="'+eachProfiledata[24]+'" index="'+eachProfiledata[0]+'" name="'+fnConvertXMLEntities(eachProfiledata[1])+'">'+eachProfileXml+'</Profile>' ;
                    }else if(orgProfileName != eachProfiledata[1] || orgProfileInUse != eachProfiledata[24]){
                        ipsecProfileXml += '<Profile markToDelete="0" inUse="'+eachProfiledata[24]+'" index="'+eachProfiledata[0]+'" name="'+fnConvertXMLEntities(eachProfiledata[1])+'" />';
                    }

                }else if(eachProfiledata[25] == "NEW"){ // For New Profiles...
                    eachProfiledata[1] = eachProfiledata[1].replace(/(&#34;)/g,'\"');
                    //eachProfiledata[1] = eachProfiledata[1].replace(/(&#126;)/g,'~');
                    ipsecProfileXml += '<Profile markToDelete="0" inUse="'+eachProfiledata[24]+'" index="" name="'+fnConvertXMLEntities(eachProfiledata[1])+'">' ;
                    ipsecProfileXml += '<tunnelMode>'+eachProfiledata[2]+'</tunnelMode>';
                    ipsecProfileXml += '<inetAddress>'+eachProfiledata[3]+'</inetAddress>';
                    var arrKey = eachProfiledata[4].split("#");
                    ipsecProfileXml += '<keySelected keyType="'+arrKey[1]+'">'+fnConvertXMLEntities(arrKey[0])+'</keySelected>';
                    ipsecProfileXml += '<Proposals>';
                    ipsecProfileXml += '<ESPTransformation>';
                    ipsecProfileXml += '<enabledSHA1>'+eachProfiledata[5]+'</enabledSHA1>';
                    ipsecProfileXml += '<enableMD5>'+eachProfiledata[6]+'</enableMD5>';
                    ipsecProfileXml += '<enableAESXCBC>'+eachProfiledata[7]+'</enableAESXCBC>';
                    ipsecProfileXml += '<enableAES256CBC>'+eachProfiledata[8]+'</enableAES256CBC>';
                    ipsecProfileXml += '<enableAES192CBC>'+eachProfiledata[9]+'</enableAES192CBC>';
                    ipsecProfileXml += '<enableAES128CBC>'+eachProfiledata[10]+'</enableAES128CBC>';
                    ipsecProfileXml += '<enableAESCTR>'+eachProfiledata[11]+'</enableAESCTR>';
                    ipsecProfileXml += '<enable3DESCBC>'+eachProfiledata[12]+'</enable3DESCBC>';
                    ipsecProfileXml += '<enableDESCBC>'+eachProfiledata[13]+'</enableDESCBC>';
                    ipsecProfileXml += '<noEncryption>'+eachProfiledata[14]+'</noEncryption>';
                    ipsecProfileXml += "</ESPTransformation>";
                    ipsecProfileXml += "<AHTransformation>";
                    ipsecProfileXml += '<enableSH1>'+eachProfiledata[15]+'</enableSH1>';
                    ipsecProfileXml += '<enableMD5>'+eachProfiledata[16]+'</enableMD5>';
                    ipsecProfileXml += '<enableAESXCBC>'+eachProfiledata[17]+'</enableAESXCBC>';
                    ipsecProfileXml += "</AHTransformation>";
                    ipsecProfileXml += '<sessionKeyGenTimeInterval>'+eachProfiledata[19]+'</sessionKeyGenTimeInterval>';
                    ipsecProfileXml += '<enableTimeInterval>'+eachProfiledata[18]+'</enableTimeInterval>';
                    ipsecProfileXml += '<sessionKeyGenSizeInterval>'+eachProfiledata[21]+'</sessionKeyGenSizeInterval>';
                    ipsecProfileXml += '<enableSizeInterval>'+eachProfiledata[20]+'</enableSizeInterval>';
                    ipsecProfileXml += '<IPCOMPTransform>'+eachProfiledata[22]+'</IPCOMPTransform>';
                    ipsecProfileXml += "</Proposals>";
                    ipsecProfileXml += "<Filters>";
                    // Filter Data..
                    if(eachProfiledata[23] != "") {
                        var profileFilterIndexArr = eachProfiledata[23].split(",");
                        for(var m=0;m<profileFilterIndexArr.length;m++){
                            if(profileFilterIndexArr[m] == "")
                                continue;
                            var filterIndex = 0;
                             for(var j=0;j<filterData.length;j++){
                                if(profileFilterIndexArr[m] ==filterData[j].split(gblSep2)[0]){
                                    filterIndex = j;
                                    break;
                                }
                            }
                            var reqdFilterArr = filterData[filterIndex];
                            reqdFilterArr = reqdFilterArr.split(gblSep2);
                            reqdFilterArr[1] = reqdFilterArr[1].replace(/(&#34;)/g,'\"');
                            //reqdFilterArr[1] = reqdFilterArr[1].replace(/(&#126;)/g,'~');
                            if(reqdFilterArr[14] == "NEW")
                                ipsecProfileXml += '<Filter index="" name="'+fnConvertXMLEntities(reqdFilterArr[1])+'" />';
                            else if(reqdFilterArr[14] == "SAVED")
                                ipsecProfileXml += '<Filter index="'+reqdFilterArr[0]+'" name="'+fnConvertXMLEntities(reqdFilterArr[1])+'" />';
                        }

                    }
                    ipsecProfileXml += "</Filters>";
                    ipsecProfileXml += '</Profile>' ;

                }
            }
            if(ipsecProfileXml != ""){
                finalProfileXml = "<Profiles>"+ipsecProfileXml+"</Profiles>";
            }

            return finalProfileXml;
        }
        function fnConstDelProfileStr(){
            var finalDelProfileStr = "";
            var delProfileData = getValue("IPSEC_DELETED_PROFILE");
            if(delProfileData == "" || delProfileData == null || delProfileData === undefined){
                return "";
            }
            delProfileData = delProfileData.split(gblSep1);
            for(var i=0;i<delProfileData.length;i++){
                var eachProfiledata = delProfileData[i].split(gblSep2);
                eachProfiledata[1] = eachProfiledata[1].replace(/(&#34;)/g,'\"');
                //eachProfiledata[1] = eachProfiledata[1].replace(/(&#126;)/g,'~');
                finalDelProfileStr += '<Profile markToDelete="1" inUse="0" index="'+eachProfiledata[0]+'" name="'+fnConvertXMLEntities(eachProfiledata[1])+'" />' ;
            }
            return finalDelProfileStr ;
        }
        function fnConstIPSecPolicyXML(orgPolicyNodeObj){

            var finalPolicyXml = "";
            var policyCount = 0;
            var orgPolicyNode = null;
            var eachPolicydata;
            var policyData = getValue("IPSEC_POLICY_DATA");
            var profileData = getValue("IPSEC_PROFILE_DATA");
            profileData = profileData.split(gblSep1);
            if(policyData == ""){
                policyCount = 0;
            }else{
                policyCount = policyData.split(gblSep1).length;
            }
            policyData = policyData.split(gblSep1);
            var ipsecPolicyXml = "";
            ipsecPolicyXml += fnConstDelPolicyStr();
            for(var i=0;i<policyData.length;i++){
                if(policyData[i] == ""){
                    continue;
                }
                eachPolicydata = policyData[i].split(gblSep2);
                if(eachPolicydata[4] == "SAVED"){ // modifying the existing Policies..
                    for(var j=0;j<orgPolicyNodeObj.length;j++){
                        if(orgPolicyNodeObj[j].getAttribute("index") == eachPolicydata[0]){
                            orgPolicyNode = orgPolicyNodeObj[j];
                            break;
                        }
                    }
                    if(orgPolicyNode == null){
                        continue;
                    }
                    var eachPolicyXml = "";
                    var selProfileIndexArr = eachPolicydata[2];
                    selProfileIndexArr = selProfileIndexArr.split(",");
                    var orgSelectedProfileObj = orgPolicyNode.getElementsByTagName("selectedProfile");
                    for(var m=0;m<selProfileIndexArr.length;m++){
                        if(selProfileIndexArr[m] == "" || selProfileIndexArr[m] == null || selProfileIndexArr[m] === undefined){
                            continue;
                        }
                        var profileIndex = 0;
                        for(var k=0;k<profileData.length;k++){
                            if(selProfileIndexArr[m] ==profileData[k].split(gblSep2)[0]){
                                profileIndex = k;
                                break;
                            }
                        }
                        var reqdProfileArr = profileData[profileIndex];
                        reqdProfileArr = reqdProfileArr.split(gblSep2);
                        reqdProfileArr[1] = reqdProfileArr[1].replace(/(&#34;)/g,'\"');
                        //reqdProfileArr[1] = reqdProfileArr[1].replace(/(&#126;)/g,'~');
                        if(reqdProfileArr[25] == "NEW")
                            eachPolicyXml += '<selectedProfile index="" name="'+fnConvertXMLEntities(reqdProfileArr[1])+'" />';
                        else if(reqdProfileArr[25] == "SAVED")
                            eachPolicyXml += '<selectedProfile index="'+reqdProfileArr[0]+'" name="'+fnConvertXMLEntities(reqdProfileArr[1])+'" />';
                    }
                    eachPolicydata[1] = eachPolicydata[1].replace(/(&#34;)/g,'\"');
                    //eachPolicydata[1] = eachPolicydata[1].replace(/(&#126;)/g,'~');
                    var orgPolicyName = orgPolicyNode.getAttribute("name");
                    var orgPolicyInUse = orgPolicyNode.getAttribute("inUse");
                    if(eachPolicyXml != ""){
                        ipsecPolicyXml += '<Policy markToDelete="0" inUse="'+eachPolicydata[3]+'" index="'+eachPolicydata[0]+'" name="'+fnConvertXMLEntities(eachPolicydata[1])+'">'+eachPolicyXml+'</Policy>';
                    }else if(orgPolicyName != eachPolicydata[1] || orgPolicyInUse != eachPolicydata[3]){
                        ipsecPolicyXml += '<Policy markToDelete="0" inUse="'+eachPolicydata[3]+'" index="'+eachPolicydata[0]+'" name="'+fnConvertXMLEntities(eachPolicydata[1])+'" />';
                    }
                }else if(eachPolicydata[4] == "NEW"){ // For New Policies...
                    eachPolicydata[1] = eachPolicydata[1].replace(/(&#34;)/g,'\"');
                    //eachPolicydata[1] = eachPolicydata[1].replace(/(&#126;)/g,'~');
                    ipsecPolicyXml += '<Policy markToDelete="0" inUse="'+eachPolicydata[3]+'" index="" name="'+fnConvertXMLEntities(eachPolicydata[1])+'">' ;
                    var selProfileIndexArr = eachPolicydata[2];
                    selProfileIndexArr = selProfileIndexArr.split(",");
                    for(var n=0;n<selProfileIndexArr.length;n++){
                        if(selProfileIndexArr[n] == null || selProfileIndexArr[n] === undefined || selProfileIndexArr[n] == "")
                            continue;        
                        var profileIndex = 0;
                        for(var k=0;k<profileData.length;k++){
                            if(selProfileIndexArr[n] ==profileData[k].split(gblSep2)[0]){
                                profileIndex = k;
                                break;
                            }
                        }

                        var reqdProfileArr = profileData[profileIndex];
                        reqdProfileArr = reqdProfileArr.split(gblSep2);
                        reqdProfileArr[1] = reqdProfileArr[1].replace(/(&#34;)/g,'\"');
                        //reqdProfileArr[1] = reqdProfileArr[1].replace(/(&#126;)/g,'~');
                        if(reqdProfileArr[25] == "NEW")
                            ipsecPolicyXml += '<selectedProfile index="" name="'+fnConvertXMLEntities(reqdProfileArr[1])+'" />';
                        else if(reqdProfileArr[25] == "SAVED")
                            ipsecPolicyXml += '<selectedProfile index="'+reqdProfileArr[0]+'" name="'+fnConvertXMLEntities(reqdProfileArr[1])+'" />';
                    }
                    ipsecPolicyXml += '</Policy>';
                }
            }
            if(ipsecPolicyXml != ""){
                finalPolicyXml = "<Policies>"+ipsecPolicyXml+"</Policies>";
            }

            return finalPolicyXml;
        }
        function fnConstDelPolicyStr(){
            var finalDelPolicyStr = "";
            var delPolicyData = getValue("IPSEC_DELETED_POLICY");
            if(delPolicyData == "" || delPolicyData == null || delPolicyData === undefined){
                return "";
            }
            delPolicyData = delPolicyData.split(gblSep1);
            for(var i=0;i<delPolicyData.length;i++){
                var eachPolicydata = delPolicyData[i].split(gblSep2);
                eachPolicydata[1] = eachPolicydata[1].replace(/(&#34;)/g,'\"');
                //eachPolicydata[1] = eachPolicydata[1].replace(/(&#126;)/g,'~');
                finalDelPolicyStr += '<Policy markToDelete="1" inUse="0" index="'+eachPolicydata[0]+'" name="'+fnConvertXMLEntities(eachPolicydata[1])+'" />' ;
            }
            return finalDelPolicyStr ;
        }
        function fnFlushConnections(){
            calledFromPage = "IPSecFlushConnections";
            glbContentWebServerCmdArray = ["<Commit><commandNode>Network</commandNode><SetValue><Network><Protocols><IPSec><flushConnection>true</flushConnection></IPSec></Protocols></Network></SetValue></Commit>"];
            gblBoolHandleRespArray = [true];
            InitiateServerRequest("CMD");
        }
        function fnHandleFlushConnections(xmlObj){
            var respVal = getXpathValue(xmlObj,"Commit/Response/statusOfOperation");
            if (respVal != "STATUS_OK"){
                alert(fnGetLocaleString("100701","Failed to clear the connections."));
            }else{
                alert(fnGetLocaleString("100365","Connections were cleared successfully."));
            }
        }
        function fnGetSNMPExportInfo(userType,arrUserID){
            var otherUsrStr = "";
            calledFromPage = "SNMPExport";
            gblSETRequestXMLArray = ["<Network/>"];
            if(userType == "OTHER"){
                if(getQueryStringValue("COUNT") == 0){
                    glbContentWebServerCmdArray = ["<Commit><commandNode>Network</commandNode><SetValue><Network><Protocols><SNMP><snmpV3UserTable><otherUsers index='"+arrUserID+"' markToExport='1' /></snmpV3UserTable></SNMP></Protocols></Network></SetValue></Commit>"];
                    gblGETRequestXMLArray = ["<Network><Protocols><SNMP><snmpV3UserTable></snmpV3UserTable></SNMP></Protocols></Network>"];
                }else{
                    var arrUsrid = opener.arrUserID;
                    for(var i=0;i<arrUsrid.length;i++){
                        if(arrUsrid[i] != "" && arrUsrid[i] !== undefined){
                            otherUsrStr += "<otherUsers index='"+arrUsrid[i]+"' markToExport='1' />";
                        }
                    }
                glbContentWebServerCmdArray = ["<Commit><commandNode>Network</commandNode><SetValue><Network><Protocols><SNMP><snmpV3UserTable>"+otherUsrStr+"</snmpV3UserTable></SNMP></Protocols></Network></SetValue></Commit>"];
                gblGETRequestXMLArray = ["<Network><Protocols><SNMP><snmpV3UserTable/></SNMP></Protocols></Network>"];
               }
            }else
                return;
            gblBoolHandleRespArray = [true];
            InitiateServerRequest("SETCMDGET");
        }
        function fnHandleSNMPExportInfo(node){
            var respVal = getXpathValue(node,"Commit/Response/statusOfOperation");
            var eachFileName = "";
            var eachFileSize = "";
            var newRow;
            var oCell;
            var emptyFlag = false;
            if (respVal != "STATUS_OK"){
                alert(fnGetLocaleString("103472","Internal Error has occurred. Please try again."));
                return false;
            }else{
                if(gblUser == "OTHER"){
                    var arrOtherUsrObj = node.getElementsByTagName("otherUsers");
                    var tableObj = document.getElementById("tblSNMPExportID");
                    if(getQueryStringValue("COUNT") == 0){
                            eachFileName = "";
                            eachFileName = getXpathValue(arrOtherUsrObj[0],"exportFileName");
                            if(eachFileName == "" || eachFileName == null || eachFileName === undefined){
                                emptyFlag = true;
                            }else{
                                emptyFlag = false;

                            }
                            eachFileSize = "";
                            eachFileSize = getXpathValue(arrOtherUsrObj[0],"fileSize");
                            newRow = tableObj.insertRow(1);
                            oCell = newRow.insertCell(0);
                            oCell.className = "clsTableElement";
                            oCell.innerHTML = "<A HREF='/contentwebserver/download/SNMP/"+fnExtractFileName(eachFileName)+"' CLASS=clsBTableElement >"+fnExtractFileName(eachFileName)+"</A>";
                            oCell = newRow.insertCell(1);
                            oCell.className = "clsTableElement";
                            oCell.innerHTML = eachFileSize ;
                    }else{
                        var arrUsrid = opener.arrUserID;
                        var strUsrid = ","+arrUsrid.join()+",";
                        for(var i=0,k=0;i<arrOtherUsrObj.length;i++){
                            eachFileName = "";
                            var eachUsrId = arrOtherUsrObj[i].getAttribute("index");
                            if(strUsrid.indexOf(","+eachUsrId+",") == -1){
                                continue;
                            }
                            eachFileName = getXpathValue(arrOtherUsrObj[i],"exportFileName");
                            if(eachFileName == "" || eachFileName == null || eachFileName === undefined){
                                emptyFlag = true;
                                continue;
                            }else{
                                emptyFlag = false;

                            }
                            eachFileSize = "";
                            eachFileSize = getXpathValue(arrOtherUsrObj[i],"fileSize");
                            newRow = tableObj.insertRow(++k);
                            oCell = newRow.insertCell(0);
                            oCell.className = "clsTableElement";
                            oCell.innerHTML = "<A HREF='/contentwebserver/download/SNMP/"+fnExtractFileName(eachFileName)+"' CLASS=clsBTableElement >"+fnExtractFileName(eachFileName)+"</A>";
                            oCell = newRow.insertCell(1);
                            oCell.className = "clsTableElement";
                            oCell.innerHTML = eachFileSize ;
                        }
                    }
                    if(emptyFlag){
                        alert(fnGetLocaleString("103472","Internal Error has occurred. Please try again."));
                    }

                }
            }
        }
        function fnconstructValueForPortocolStr() {
            var temp_Protocol_Str  = "";
              for(var key in strProtoHashMap){
                if(getValue(key) == "1"){
                    temp_Protocol_Str += strProtoHashMap[key] + "," ;
                }
              }

            temp_Protocol_Str = temp_Protocol_Str.slice(0,temp_Protocol_Str.length-1);
            setValue("WakeUpSetting_Protocol",temp_Protocol_Str);
        }
        function fnGetSNMPUserInfo(){
            gblTempNtwDOMArr = gblDomObjArray[0];
            gblGETRequestXMLArray = ["<Network><Protocols><SNMP><snmpV3UserTable/></SNMP></Protocols></Network>"];
            calledFromPage = "Network-GetSNMPV3UserInfo";
            gblBoolHandleRespArray = [true];
            InitiateServerRequest("GET");
        }
        function fnUpdateSNMPV3UserInfo(){
            fnFillSNMP();
            gblDomObjArray[0] = gblTempNtwDOMArr ;
        }
