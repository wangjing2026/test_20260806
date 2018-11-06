/* parasoft suppress item PB-33 */
/* parasoft suppress item METRICS-01_duplicated_2 */

/**
* @Copyright 2008 TOSHIBA TEC CORPORATION All rights reserved
 * @Workfile: crolemanager.cpp$
 * @Revision: 1 $
 * @Date: 07/30/2008$ 
 * @Author: Manoj Saha / Ghaayathri
 * @Description:This file implements Listening to the port and managing the commands received from the UpperLayer
 * @Notes:
*/
#include "crolemanager.h"
#include "CI/OperatingEnvironment/debug.h"
#include "portids.h"
#include "croleexception.h"
#include "utils.h"
#include "CI/ServiceStartupManager/ssmcontracts.h" 
#include "CI/ServiceStartupManager/client.h" 
#include <AL/UIController/boproxy.h>
#include "AL/UIController/utility.h"
#include "ccommandhandler.h"
#include <AL/al_service_name.h>
#include "CI/SecureMessaging/securemessaging.h"
#include "CI/SI/rbac_objects.h"
#include <AL/status.h>
using namespace ci::servicestartupmanager; 
using namespace ci::hierarchicaldb;
using namespace dom;



namespace al
{
namespace softwarefunction
{
namespace rbacmanager
{

	//MsgPortRef CRoleManager::m_port; 
	MsgPortRef	CRoleManager::m_pAccsgPort; 
	HierarchicalDBRef CRoleManager::m_pHDB;
	CategoryDisplayNameMap CRoleManager::categoryDisplayMap;
	ObjectOperationMap       CRoleManager::objectOperMap;
	ObjectDisplayNameMap     CRoleManager::objectDisplayMap;
	DisplayNameObjectMap     CRoleManager::displayObjectMap;
	ObjectCategoryMap        CRoleManager::objectCategoryMap;
	ObjectCategoryMap		 CRoleManager::displayCatMap;
	RbacManagerErrorCodeMap CRoleManager::m_errorMap;

/**
* Normal constructor the members are initialized and setting the document path
*/ 
CRoleManager::CRoleManager()
{				
	m_bAllDependentServicesUpAndRunning=false;
	/*if(STATUS_OK != MsgPort::Create(m_port,MSGPORT_AL_ROLEMANAGER_APPLICATION ))
		throw CRoleException("Failed to create port for RoleManager.\n");*/ 
	if(STATUS_OK != MsgPort::Create(m_pAccsgPort,MSGPORT_AL_ROLEMANAGER_APPLICATION ))
                throw CRoleException("Failed to create port for RoleManager.\n");   
	//if(STATUS_OK !=ci::securemessaging::CreateMsgPort(m_port,MSGPORT_AL_ROLEMANAGER_APPLICATION))
	//	throw CRoleException("Failed to create port for RoleManager.\n");
	if (!(m_pHDB = HierarchicalDB::Acquire(NULL))) 
		throw CRoleException("Failed to Acquire HDB\n");
       // m_pAccsgPort = NULL; 
}

Status CRoleManager::Param()
        {
                Status retStatus = STATUS_OK;
                try
                {
                        /*if (MsgPort::Create(m_pAccsgPort,MSGPORT_AL_ROLEMANAGER_APPLICATION) != STATUS_OK)
                        {

                                throw CRoleException(STATUS_FAILED);
                        }*/ 

#ifndef NO_SSM

                        Ref<ci::servicestartupmanager::Client> ssmClient;
                        ssmClient = ci::servicestartupmanager::Client::Acquire(m_pAccsgPort,SERVICE_AL_ROLEMANAGER_APPLICATION);
			//Check for NULL
			if ( !ssmClient)
			{
				DEBUGL2("RoleManager::Failed to Acquire ssm client \n");
				throw CRoleException(STATUS_FAILED);
			}
                        Status ret = ssmClient->Notify((SSMContracts::ecNotifications)SSMContracts::ecStartPending);
                        if (ret != STATUS_OK)
                        {
                                DEBUGL2("RoleManager::Param:Failed to send NOTIFICATION messages to SSM\n");
                                throw CRoleException(STATUS_FAILED);
                        }
			ret = ssmClient->Notify((SSMContracts::ecNotifications)SSMContracts::ecReady);

        		if (ret != STATUS_OK)
        		{
                		DEBUGL2("CRoleManager::Param:Failed to send NOTIFICATION messages to SSM\n");
                		throw CRoleException(STATUS_FAILED);
        		}

#endif
                }
                catch(CRoleException& except)
                {
                        DEBUGL2("RoleManager::CAccManager::Param-->Failed\n");
                        retStatus = (Status)(except);
                }
                return retStatus;
        }
void CRoleManager::LoadRbacManagerErrorCodes()
{
	CRoleManager::m_errorMap.insert(RbacManagerErrorCodeMap::value_type(STATUS_OK,RBAC_SEC_STATUS_OK));
	CRoleManager::m_errorMap.insert(RbacManagerErrorCodeMap::value_type(STATUS_FAILED,RBAC_SEC_STATUS_FAILED));
	CRoleManager::m_errorMap.insert(RbacManagerErrorCodeMap::value_type(STATUS_AL_SEC_INVALID_INPUT,RBAC_SEC_STATUS_INVALID_INPUT));
	CRoleManager::m_errorMap.insert(RbacManagerErrorCodeMap::value_type(STATUS_AL_SEC_USER_NOT_FOUND,RBAC_SEC_STATUS_USER_NOT_FOUND));
	CRoleManager::m_errorMap.insert(RbacManagerErrorCodeMap::value_type(STATUS_AL_SEC_OBJECT_MAX_REACHED,RBAC_SEC_STATUS_OBJECT_MAX_REACHED));
	CRoleManager::m_errorMap.insert(RbacManagerErrorCodeMap::value_type(STATUS_AL_SEC_OBJECT_ALREADY_EXISTS,RBAC_SEC_STATUS_OBJECT_ALREADY_EXISTS));
	CRoleManager::m_errorMap.insert(RbacManagerErrorCodeMap::value_type(STATUS_AL_SEC_INTERNAL_ERROR,RBAC_SEC_STATUS_INTERNAL_ERROR));
	CRoleManager::m_errorMap.insert(RbacManagerErrorCodeMap::value_type(STATUS_AL_SEC_USER_TOKEN_NOT_FOUND,RBAC_SEC_STATUS_USER_TOKEN_NOT_FOUND));
	CRoleManager::m_errorMap.insert(RbacManagerErrorCodeMap::value_type(STATUS_AL_SEC_ROLE_NOT_FOUND,RBAC_SEC_STATUS_ROLE_NOT_FOUND));
	CRoleManager::m_errorMap.insert(RbacManagerErrorCodeMap::value_type(STATUS_AL_SEC_COMMAND_SCHEMA,RBAC_SEC_STATUS_INVALID_COMMAND_SCHEMA));
	CRoleManager::m_errorMap.insert(RbacManagerErrorCodeMap::value_type(STATUS_AL_SEC_USER_UNAUTHORIZED,RBAC_SEC_STATUS_USER_UNAUTHORIZED));
	CRoleManager::m_errorMap.insert(RbacManagerErrorCodeMap::value_type(STATUS_AL_SEC_GROUP_NOT_FOUND,RBAC_SEC_STATUS_GROUP_NOT_FOUND));
	CRoleManager::m_errorMap.insert(RbacManagerErrorCodeMap::value_type(STATUS_AL_SEC_INVALID_USERTOKEN,RBAC_SEC_STATUS_INVALID_USER_TOKEN));
	CRoleManager::m_errorMap.insert(RbacManagerErrorCodeMap::value_type(STATUS_AL_SEC_UNKNOWN_ERROR,RBAC_SEC_STATUS_UNKNOWN_ERROR));
	CRoleManager::m_errorMap.insert(RbacManagerErrorCodeMap::value_type(STATUS_AL_SEC_OBJECT_NOT_FOUND,RBAC_SEC_STATUS_OBJECT_NOT_FOUND));
	CRoleManager::m_errorMap.insert(RbacManagerErrorCodeMap::value_type(STATUS_AL_SEC_OBJECT_MAX_LENGTH_EXCEEDED,RBAC_SEC_STATUS_OBJECT_MAX_LEN_EXCEDDED));
	CRoleManager::m_errorMap.insert(RbacManagerErrorCodeMap::value_type(STATUS_AL_SEC_EMPTY_PERMISSION_LIST,RBAC_SEC_STATUS_EMPTY_PERMISSION_LIST));
	CRoleManager::m_errorMap.insert(RbacManagerErrorCodeMap::value_type(STATUS_AL_SEC_EMPTY_ROLE_LIST,RBAC_SEC_STATUS_EMPTY_ROLE_LIST));
	CRoleManager::m_errorMap.insert(RbacManagerErrorCodeMap::value_type(STATUS_AL_SEC_INVALID_PERMISSION,RBAC_SEC_STATUS_INVALID_PERMISSION));
	CRoleManager::m_errorMap.insert(RbacManagerErrorCodeMap::value_type(STATUS_AL_SEC_IMPORT_FILE_NOT_FOUND,RBAC_SEC_STATUS_IMPORT_FILE_NOT_FOUND));
	CRoleManager::m_errorMap.insert(RbacManagerErrorCodeMap::value_type(STATUS_AL_SEC_EXPORT_FILE_NOT_FOUND,RBAC_SEC_STATUS_EXPORT_FILE_NOT_FOUND));
	CRoleManager::m_errorMap.insert(RbacManagerErrorCodeMap::value_type(STATUS_AL_SEC_DB_CORRUPTED,RBAC_SEC_STATUS_DB_CORRUPTED));
	CRoleManager::m_errorMap.insert(RbacManagerErrorCodeMap::value_type(STATUS_AL_SEC_PERMISSION_CHECK_ERROR,RBAC_SEC_STATUS_PERMISSION_CHECK_ERROR));
	CRoleManager::m_errorMap.insert(RbacManagerErrorCodeMap::value_type(STATUS_AL_SEC_ROLENAME_INVALID_INPUT,RBAC_SEC_STATUS_ROLENAME_INVALID_INPUT));
	CRoleManager::m_errorMap.insert(RbacManagerErrorCodeMap::value_type(STATUS_AL_SEC_IMPORT_PARTIAL_ROLES_SUCCESS,RBAC_SEC_STATUS_IMPORT_PARTIAL_ROLES_SUCCESS));
	CRoleManager::m_errorMap.insert(RbacManagerErrorCodeMap::value_type(STATUS_DISK_FULL,RBAC_SEC_STATUS_DISK_FULL));
	CRoleManager::m_errorMap.insert(RbacManagerErrorCodeMap::value_type(STATUS_AL_SEC_BUILTIN_ROLE_CANNOT_DELETE,RBAC_SEC_STATUS_BUILTIN_ROLE_CANNOT_DELETE));
	CRoleManager::m_errorMap.insert(RbacManagerErrorCodeMap::value_type(STATUS_AL_SEC_BAD_REQUEST, RBAC_SEC_STATUS_BAD_REQUEST));
	return ;
}
void CRoleManager::LoadDisplayObjectMap()
{
	//Load the DisplayNameObjectMap
	//CopyJob
	set<CString> objectSet;
	objectSet.insert(OBJ_COPYJOB);
	displayObjectMap.insert(DisplayNameObjectMap::value_type(OPR_COPY_JOB,objectSet));
	objectSet.clear();

	//PrintJob
	objectSet.insert(OBJ_PRINTJOB);
	//objectSet.insert(OBJ_PRINT_USBSTORAGE); //As per the new Permission_Group.xls on 31/05/2010
	CRoleManager::displayObjectMap.insert(DisplayNameObjectMap::value_type(OPR_PRINT_JOB,objectSet));
	objectSet.clear();

	//Print Management
	objectSet.insert(OBJ_PRINT_MANAGEMENT);
	CRoleManager::displayObjectMap.insert(DisplayNameObjectMap::value_type(OPR_PRINT_MGNT,objectSet));
	objectSet.clear();

	//Remote Scan 
	objectSet.insert(OBJ_REMOTESCAN_JOB);
	CRoleManager::displayObjectMap.insert(DisplayNameObjectMap::value_type(OPR_REMOTESCAN,objectSet));
	objectSet.clear();

	//Internet Fax Transmission
	objectSet.insert(OBJ_IFAXSEND);
	//objectSet.insert(OBJ_IFAXSEND_PC); //As per the new Permission_Group.xls on 31/05/2010
	CRoleManager::displayObjectMap.insert(DisplayNameObjectMap::value_type(OPR_INTERNET_FAX_TRANS,objectSet));
	objectSet.clear();

	//Fax Transmission
	//objectSet.insert(OBJ_FAX_SEND_LONGDISTANCE); //As per the new Permission_Group.xls on 31/05/2010
	//objectSet.insert(OBJ_FAX_SEND_QUALITY); //As per the new Permission_Group.xls on 31/05/2010
	objectSet.insert(OBJ_FAXSEND);
	//objectSet.insert(OBJ_FAXSEND_PC); //As per the new Permission_Group.xls on 31/05/2010
	CRoleManager::displayObjectMap.insert(DisplayNameObjectMap::value_type(OPR_FAX_TRANS,objectSet));
	objectSet.clear();

	//Fax RecievedPrint
	objectSet.insert(OBJ_FAXRECEIVEDPRINT);
	CRoleManager::displayObjectMap.insert(DisplayNameObjectMap::value_type(OPR_FAX_RECVD_PRINT,objectSet));
	objectSet.clear();
	/*Macro is introduced to identify separate product.Conditional compilation is
	done.Below 3 lines are not executed in LOIRE/ALABAMA/MOSEL/ECOLOIRE*/
	#if !defined (PRODUCT_LOIRE) && !defined(PRODUCT_ALABAMA) && !defined(PRODUCT_MOSEL) 
	//Color print
	objectSet.insert(OBJ_COLOR_PRINT);
	//objectSet.insert(OBJ_TWINCOLOR_PRINT); //As per the new Permission_Group.xls on 31/05/2010
	CRoleManager::displayObjectMap.insert(DisplayNameObjectMap::value_type(OPR_COLOR_PRINT,objectSet));
	objectSet.clear();

   #endif

	//Various Print Settings //As per the new Permission_Group.xls on 31/05/2010
	/*objectSet.insert(OBJ_OUTPUT_SIMPLEX);
	objectSet.insert(OBJ_OUTPUT_LARGESIZE);
	objectSet.insert(OBJ_OUTPUT_SPECIALPAPER);
	objectSet.insert(OBJ_OUTPUT_1N1);
	CRoleManager::displayObjectMap.insert(DisplayNameObjectMap::value_type(OPR_VARIOUS_PRINT_SETTING,objectSet));
	objectSet.clear();*/

	//Store To Local Storage
	//objectSet.insert(OBJ_INTERNALSTORE); //this is commented as per new Permission_Group file
	objectSet.insert(OBJ_INTERNALSTORE_FILESHARE_LOCAL);
	CRoleManager::displayObjectMap.insert(DisplayNameObjectMap::value_type(OPR_LOCAL_STORAGE,objectSet));
	objectSet.clear();

	//Store To USB Device
	objectSet.insert(OBJ_INTERNALSTORE_USBSTORAGE);
	CRoleManager::displayObjectMap.insert(DisplayNameObjectMap::value_type(OPR_USB_STORAGE,objectSet));
	objectSet.clear();

	//Send Email
	objectSet.insert(OBJ_EMAILSEND);
	CRoleManager::displayObjectMap.insert(DisplayNameObjectMap::value_type(OPR_SEND_EMAIL,objectSet));
	objectSet.clear();

	//Store To Remote Server
	objectSet.insert(OBJ_EXTERNALSTORE); //this is commented as per new Permission_Group file
	//objectSet.insert(OBJ_EXTERNALSTORE_FILESHARE);
	//objectSet.insert(OBJ_EXTERNALSTORE_FILESHARE_SMB);
	//objectSet.insert(OBJ_EXTERNALSTORE_FILESHARE_HOME);
	//objectSet.insert(OBJ_EXTERNALSTORE_FTP);
	//objectSet.insert(OBJ_EXTERNALSTORE_NETWARE);
	CRoleManager::displayObjectMap.insert(DisplayNameObjectMap::value_type(OPR_REMOTE_STORE,objectSet));
	objectSet.clear();


	//Distributed Scan
	/*objectSet.insert(OBJ_EXTERNALSTORE_DISTRIBUTEDSCAN);
	CRoleManager::displayObjectMap.insert(DisplayNameObjectMap::value_type(OPR_DISTRIBUTED_SCAN,objectSet));
	objectSet.clear();*/

	//WSScanPush
	objectSet.insert(OBJ_EXTERNALSTORE_WSPUSHSCAN);
	CRoleManager::displayObjectMap.insert(DisplayNameObjectMap::value_type(OPR_WSSCAN,objectSet));
	objectSet.clear();

	//Object E-Filing
	objectSet.insert(OBJ_EFILING);
	CRoleManager::displayObjectMap.insert(DisplayNameObjectMap::value_type(OPR_EFILING_ACCESS,objectSet));
	objectSet.clear();

	//Device Setting
	objectSet.insert(OBJ_DEVICESETTING);
	CRoleManager::displayObjectMap.insert(DisplayNameObjectMap::value_type(OPR_DEVICE_SETTING,objectSet));
	objectSet.clear();

	//Cloning
	/*objectSet.insert(OBJ_DEVICE_CLONE);
	CRoleManager::displayObjectMap.insert(DisplayNameObjectMap::value_type(OPR_CLONE,objectSet));
	objectSet.clear();*/

	//User/Department Management
	objectSet.insert(OBJ_USERMANAGEMENT);
	CRoleManager::displayObjectMap.insert(DisplayNameObjectMap::value_type(OPR_USER_DEPT_MGNT,objectSet));
	objectSet.clear();

	//Job Operation
	objectSet.insert(OBJ_JOB_MANAGEMENT);
	CRoleManager::displayObjectMap.insert(DisplayNameObjectMap::value_type(OPR_JOB_OPERATION,objectSet));
	objectSet.clear();

	//LogRead
	objectSet.insert(OBJ_LOG_ACCESS); //this is commented as per new Permission_Group file
	//objectSet.insert(OBJ_JOB_LOG_ACCESS);
	//objectSet.insert(OBJ_MESSAGE_LOG_ACCESS);
	CRoleManager::displayObjectMap.insert(DisplayNameObjectMap::value_type(OPR_LOG_READ,objectSet));
	objectSet.clear();

	//Log Export
	objectSet.insert(OBJ_LOG_EXPORT); //this is commented as per new Permission_Group file
	//objectSet.insert(OBJ_JOB_LOG_EXPORT);
	//objectSet.insert(OBJ_MESSAGE_LOG_EXPORT);
	CRoleManager::displayObjectMap.insert(DisplayNameObjectMap::value_type(OPR_LOG_EXPORT,objectSet));
	objectSet.clear();

	//AdressBook
	//objectSet.insert(OBJ_ADDRESSBOOK);
	//CRoleManager::displayObjectMap.insert(DisplayNameObjectMap::value_type(OPR_ADDRESSBOOK_READ_EDIT,objectSet));
	objectSet.clear();

	//JobTemplate
	//objectSet.insert(OBJ_JOBTEMPLATE);
	//CRoleManager::displayObjectMap.insert(DisplayNameObjectMap::value_type(OPR_JOBTEMPLATE_READ_EDIT,objectSet));
	objectSet.clear();

	//DiagonosticMode
	objectSet.insert(OBJ_DEVICE_MEINTENANCE);
	CRoleManager::displayObjectMap.insert(DisplayNameObjectMap::value_type(OPR_ACCESS_DIAGONOSTICMODE,objectSet));
	objectSet.clear();
	
	//EWBAccess
	objectSet.insert(OBJ_EWB_ACCESS);
        CRoleManager::displayObjectMap.insert(DisplayNameObjectMap::value_type(OPR_EWB_ACCESS,objectSet));
        objectSet.clear();
	
	//USBDirectPrint
	objectSet.insert(OBJ_USBDIRECTPRINT);
        CRoleManager::displayObjectMap.insert(DisplayNameObjectMap::value_type(OPR_USBDIRECTPRINT,objectSet));
        objectSet.clear();

	//Remote service technician
	objectSet.insert (OBJ_DEVICE_REMOTE_MAINTENANCE);
	CRoleManager::displayObjectMap.insert (DisplayNameObjectMap::value_type (OPR_DEVICE_REMOTE_MAINTENANCE, objectSet));
	objectSet.clear ();
	
	return ;
}

void CRoleManager::LoadObjectDisplayMap()
{
	//Load the ObjectDisplayNameMap
	CRoleManager::objectDisplayMap.insert(ObjectDisplayNameMap::value_type(OBJ_COPYJOB,OPR_COPY_JOB));
	CRoleManager::objectDisplayMap.insert(ObjectDisplayNameMap::value_type(OBJ_PRINTJOB,OPR_PRINT_JOB));
	CRoleManager::objectDisplayMap.insert(ObjectDisplayNameMap::value_type(OBJ_PRINT_MANAGEMENT,OPR_PRINT_MGNT));
	CRoleManager::objectDisplayMap.insert(ObjectDisplayNameMap::value_type(OBJ_REMOTESCAN_JOB,OPR_REMOTESCAN));
	CRoleManager::objectDisplayMap.insert(ObjectDisplayNameMap::value_type(OBJ_IFAXSEND,OPR_INTERNET_FAX_TRANS));
	CRoleManager::objectDisplayMap.insert(ObjectDisplayNameMap::value_type(OBJ_FAXSEND,OPR_FAX_TRANS));
	CRoleManager::objectDisplayMap.insert(ObjectDisplayNameMap::value_type(OBJ_FAXRECEIVEDPRINT,OPR_FAX_RECVD_PRINT));
	/*Macro is introduced to identify separate product.Conditional compilation is
	done.Below 3 lines are not executed in LOIRE/ALABAMA/MOSEL/ECOLOIRE*/
	#if !defined (PRODUCT_LOIRE) && !defined(PRODUCT_ALABAMA) && !defined(PRODUCT_MOSEL) 
		CRoleManager::objectDisplayMap.insert(ObjectDisplayNameMap::value_type(OBJ_COLOR_PRINT,OPR_COLOR_PRINT));
	#endif
	CRoleManager::objectDisplayMap.insert(ObjectDisplayNameMap::value_type(OBJ_INTERNALSTORE_FILESHARE_LOCAL,OPR_LOCAL_STORAGE));
	CRoleManager::objectDisplayMap.insert(ObjectDisplayNameMap::value_type(OBJ_INTERNALSTORE_USBSTORAGE,OPR_USB_STORAGE));
	CRoleManager::objectDisplayMap.insert(ObjectDisplayNameMap::value_type(OBJ_EMAILSEND,OPR_SEND_EMAIL));
	CRoleManager::objectDisplayMap.insert(ObjectDisplayNameMap::value_type(OBJ_EXTERNALSTORE,OPR_REMOTE_STORE));
	//CRoleManager::objectDisplayMap.insert(ObjectDisplayNameMap::value_type(OBJ_EXTERNALSTORE_DISTRIBUTEDSCAN,OPR_DISTRIBUTED_SCAN));
	CRoleManager::objectDisplayMap.insert(ObjectDisplayNameMap::value_type(OBJ_EXTERNALSTORE_WSPUSHSCAN,OPR_WSSCAN));
	CRoleManager::objectDisplayMap.insert(ObjectDisplayNameMap::value_type(OBJ_EFILING,OPR_EFILING_ACCESS));
//	CRoleManager::objectDisplayMap.insert(ObjectDisplayNameMap::value_type(OBJ_ADDRESSBOOK,OPR_COLOR_PRINT));
//	CRoleManager::objectDisplayMap.insert(ObjectDisplayNameMap::value_type(OBJ_JOBTEMPLATE,OPR_COLOR_PRINT));
	CRoleManager::objectDisplayMap.insert(ObjectDisplayNameMap::value_type(OBJ_DEVICESETTING,OPR_DEVICE_SETTING));
	//CRoleManager::objectDisplayMap.insert(ObjectDisplayNameMap::value_type(OBJ_DEVICE_CLONE,OPR_CLONE));
	CRoleManager::objectDisplayMap.insert(ObjectDisplayNameMap::value_type(OBJ_USERMANAGEMENT,OPR_USER_DEPT_MGNT));
	//CRoleManager::objectDisplayMap.insert(ObjectDisplayNameMap::value_type(OBJ_MESSAGE_LOG_ACCESS,OPR_READ));
	CRoleManager::objectDisplayMap.insert(ObjectDisplayNameMap::value_type(OBJ_LOG_ACCESS,OPR_LOG_READ));
	//CRoleManager::objectDisplayMap.insert(ObjectDisplayNameMap::value_type(OBJ_MESSAGE_LOG_EXPORT,OPR_EXPORT));
	CRoleManager::objectDisplayMap.insert(ObjectDisplayNameMap::value_type(OBJ_LOG_EXPORT,OPR_LOG_EXPORT));
	//CRoleManager::objectDisplayMap.insert(ObjectDisplayNameMap::value_type(OBJ_JOB_DELETE,OPR_JOB_OPERATION));

	return ;
}
void CRoleManager::LoadObjectCategoryMap()
{
	//Load the ObjectCategoryMap
	CRoleManager::objectCategoryMap.insert(ObjectCategoryMap::value_type(OBJ_COPYJOB,CAT_COPY_FUNC));
	CRoleManager::objectCategoryMap.insert(ObjectCategoryMap::value_type(OBJ_PRINTJOB,CAT_PRINT_FUNC));
	CRoleManager::objectCategoryMap.insert(ObjectCategoryMap::value_type(OBJ_PRINT_MANAGEMENT,CAT_PRINT_FUNC));
	CRoleManager::objectCategoryMap.insert(ObjectCategoryMap::value_type(OBJ_REMOTESCAN_JOB,CAT_SCAN_FUNC));
	CRoleManager::objectCategoryMap.insert(ObjectCategoryMap::value_type(OBJ_IFAXSEND,CAT_FAX_IFAX_FUNC));
	CRoleManager::objectCategoryMap.insert(ObjectCategoryMap::value_type(OBJ_FAXSEND,CAT_FAX_IFAX_FUNC));
	CRoleManager::objectCategoryMap.insert(ObjectCategoryMap::value_type(OBJ_FAXRECEIVEDPRINT,CAT_FAX_IFAX_FUNC));
	CRoleManager::objectCategoryMap.insert(ObjectCategoryMap::value_type(OBJ_COLOR_PRINT,CAT_OUTPUT_SETTING));
	//CRoleManager::objectCategoryMap.insert(ObjectCategoryMap::value_type(OBJ_TWINCOLOR_PRINT,CAT_OUTPUT_SETTING));
	CRoleManager::objectCategoryMap.insert(ObjectCategoryMap::value_type(OBJ_INTERNALSTORE_FILESHARE_LOCAL,CAT_LOCAL_FILE_SHARE));
	CRoleManager::objectCategoryMap.insert(ObjectCategoryMap::value_type(OBJ_INTERNALSTORE_USBSTORAGE,CAT_LOCAL_FILE_SHARE));
	CRoleManager::objectCategoryMap.insert(ObjectCategoryMap::value_type(OBJ_EMAILSEND,CAT_REMOTE));
	CRoleManager::objectCategoryMap.insert(ObjectCategoryMap::value_type(OBJ_EXTERNALSTORE,CAT_REMOTE));
	//CRoleManager::objectCategoryMap.insert(ObjectCategoryMap::value_type(OBJ_EXTERNALSTORE_DISTRIBUTEDSCAN,CAT_REMOTE));
	CRoleManager::objectCategoryMap.insert(ObjectCategoryMap::value_type(OBJ_EXTERNALSTORE_WSPUSHSCAN,CAT_REMOTE));
	CRoleManager::objectCategoryMap.insert(ObjectCategoryMap::value_type(OBJ_EFILING,CAT_EFILING));
	CRoleManager::objectCategoryMap.insert(ObjectCategoryMap::value_type(OBJ_DEVICESETTING,CAT_DEVICE_SETTING));
	//CRoleManager::objectCategoryMap.insert(ObjectCategoryMap::value_type(OBJ_DEVICE_CLONE,CAT_DEVICE_SETTING));	
	CRoleManager::objectCategoryMap.insert(ObjectCategoryMap::value_type(OBJ_USERMANAGEMENT,CAT_USER_DEPT_MGNT));
	//CRoleManager::objectCategoryMap.insert(ObjectCategoryMap::value_type(OBJ_JOB_DELETE,CAT_JOB_MGNT));
	//CRoleManager::objectCategoryMap.insert(ObjectCategoryMap::value_type(OBJ_MESSAGE_LOG_ACCESS,CAT_LOG_MGNT));
	CRoleManager::objectCategoryMap.insert(ObjectCategoryMap::value_type(OBJ_LOG_ACCESS,CAT_LOG_MGNT));
	//CRoleManager::objectCategoryMap.insert(ObjectCategoryMap::value_type(OBJ_MESSAGE_LOG_EXPORT,CAT_LOG_MGNT));
	CRoleManager::objectCategoryMap.insert(ObjectCategoryMap::value_type(OBJ_LOG_EXPORT,CAT_LOG_MGNT));
	//CRoleManager::objectCategoryMap.insert(ObjectCategoryMap::value_type(OBJ_ADDRESSBOOK,CAT_ADDRS_BOOK));
	//CRoleManager::objectCategoryMap.insert(ObjectCategoryMap::value_type(OBJ_JOBTEMPLATE,CAT_JOB_TEMPLATE));
	//CRoleManager::objectCategoryMap.insert(ObjectCategoryMap::value_type(OBJ_OUTPUT_SIMPLEX,CAT_OUTPUT_SETTING));
	//CRoleManager::objectCategoryMap.insert(ObjectCategoryMap::value_type(OBJ_OUTPUT_LARGESIZE,CAT_OUTPUT_SETTING));
	//CRoleManager::objectCategoryMap.insert(ObjectCategoryMap::value_type(OBJ_OUTPUT_SPECIALPAPER,CAT_OUTPUT_SETTING));
	//CRoleManager::objectCategoryMap.insert(ObjectCategoryMap::value_type(OBJ_OUTPUT_1N1,CAT_OUTPUT_SETTING));
}

void CRoleManager::LoadDisplayCategoryMap()
{
	//CopyFunction
	CRoleManager::displayCatMap.insert(ObjectCategoryMap::value_type(OPR_COPY_JOB,CAT_COPY_FUNC));
	//Print Function
	CRoleManager::displayCatMap.insert(ObjectCategoryMap::value_type(OPR_PRINT_JOB,CAT_PRINT_FUNC));
	CRoleManager::displayCatMap.insert(ObjectCategoryMap::value_type(OPR_PRINT_MGNT,CAT_PRINT_FUNC));
	//Scan function
	CRoleManager::displayCatMap.insert(ObjectCategoryMap::value_type(OPR_REMOTESCAN,CAT_SCAN_FUNC));
	//Fax/iFax Function
	CRoleManager::displayCatMap.insert(ObjectCategoryMap::value_type(OPR_INTERNET_FAX_TRANS,CAT_FAX_IFAX_FUNC));
	CRoleManager::displayCatMap.insert(ObjectCategoryMap::value_type(OPR_FAX_TRANS,CAT_FAX_IFAX_FUNC));
	CRoleManager::displayCatMap.insert(ObjectCategoryMap::value_type(OPR_FAX_RECVD_PRINT,CAT_FAX_IFAX_FUNC));
	//Output Setting
	CRoleManager::displayCatMap.insert(ObjectCategoryMap::value_type(OPR_COLOR_PRINT,CAT_OUTPUT_SETTING));
	CRoleManager::displayCatMap.insert(ObjectCategoryMap::value_type(OPR_VARIOUS_PRINT_SETTING,CAT_OUTPUT_SETTING));
	//Local File Share
	CRoleManager::displayCatMap.insert(ObjectCategoryMap::value_type(OPR_LOCAL_STORAGE,CAT_LOCAL_FILE_SHARE));
	CRoleManager::displayCatMap.insert(ObjectCategoryMap::value_type(OPR_USB_STORAGE,CAT_LOCAL_FILE_SHARE));
	//Remote
	CRoleManager::displayCatMap.insert(ObjectCategoryMap::value_type(OPR_SEND_EMAIL,CAT_REMOTE));
	CRoleManager::displayCatMap.insert(ObjectCategoryMap::value_type(OPR_REMOTE_STORE,CAT_REMOTE));
	CRoleManager::displayCatMap.insert(ObjectCategoryMap::value_type(OPR_DISTRIBUTED_SCAN,CAT_REMOTE));
	CRoleManager::displayCatMap.insert(ObjectCategoryMap::value_type(OPR_WSSCAN,CAT_REMOTE));
	//E-Filing
	CRoleManager::displayCatMap.insert(ObjectCategoryMap::value_type(OPR_EFILING_ACCESS,CAT_EFILING));
	//Device settings
	CRoleManager::displayCatMap.insert(ObjectCategoryMap::value_type(OPR_DEVICE_SETTING,CAT_DEVICE_SETTING));
	CRoleManager::displayCatMap.insert(ObjectCategoryMap::value_type(OPR_CLONE,CAT_DEVICE_SETTING));
	//User/Department Management
	CRoleManager::displayCatMap.insert(ObjectCategoryMap::value_type(OPR_USER_DEPT_MGNT,CAT_USER_DEPT_MGNT));
	//Job Management
	CRoleManager::displayCatMap.insert(ObjectCategoryMap::value_type(OPR_JOB_OPERATION,CAT_JOB_MGNT));
	//Log Management
	CRoleManager::displayCatMap.insert(ObjectCategoryMap::value_type(OPR_LOG_READ,CAT_LOG_MGNT));
	CRoleManager::displayCatMap.insert(ObjectCategoryMap::value_type(OPR_LOG_EXPORT,CAT_LOG_MGNT));
	//AddressBookReadEdit
	CRoleManager::displayCatMap.insert(ObjectCategoryMap::value_type(OPR_ADDRESSBOOK_READ_EDIT,CAT_ADDRS_BOOK));
	//JobTemplateReadEdit
	CRoleManager::displayCatMap.insert(ObjectCategoryMap::value_type(OPR_JOBTEMPLATE_READ_EDIT,CAT_JOB_TEMPLATE));
	//AccessDiagonosticMode
	CRoleManager::displayCatMap.insert(ObjectCategoryMap::value_type(OPR_ACCESS_DIAGONOSTICMODE,CAT_DIAGONOSTIC));
	return ;
}
Status CRoleManager::LoadObjectOperationMap()
{
	set<CString> oprSet;

	//CopyJob
	oprSet.insert(OP_EXECUTE);
	objectOperMap.insert(ObjectOperationMap::value_type(OBJ_COPYJOB,oprSet));
	oprSet.clear();

	//PrintJob
	oprSet.insert(OP_EXECUTE);
	objectOperMap.insert(ObjectOperationMap::value_type(OBJ_PRINTJOB,oprSet));
	//objectOperMap.insert(ObjectOperationMap::value_type(OBJ_PRINT_USBSTORAGE,oprSet));
	oprSet.clear();

	//PrintManagement
	oprSet.insert(OP_EXECUTE);
	objectOperMap.insert(ObjectOperationMap::value_type(OBJ_PRINT_MANAGEMENT,oprSet));
	oprSet.clear();
	
	//Remote Scan
	oprSet.insert(OP_EXECUTE);
	objectOperMap.insert(ObjectOperationMap::value_type(OBJ_REMOTESCAN_JOB,oprSet));
	oprSet.clear();

	//InternetFaxTransmission
	oprSet.insert(OP_EXECUTE);
	objectOperMap.insert(ObjectOperationMap::value_type(OBJ_IFAXSEND,oprSet));
	//objectOperMap.insert(ObjectOperationMap::value_type(OBJ_IFAXSEND_PC,oprSet));	
	oprSet.clear();

	//Fax Transmission
	oprSet.insert(OP_EXECUTE);
	//objectOperMap.insert(ObjectOperationMap::value_type(OBJ_FAX_SEND_LONGDISTANCE,oprSet));
	//objectOperMap.insert(ObjectOperationMap::value_type(OBJ_FAX_SEND_QUALITY,oprSet));
	objectOperMap.insert(ObjectOperationMap::value_type(OBJ_FAXSEND,oprSet));
	//objectOperMap.insert(ObjectOperationMap::value_type(OBJ_FAXSEND_PC,oprSet));
	oprSet.clear();

	//FaxReceivedPrint
	oprSet.insert(OP_EXECUTE);
	objectOperMap.insert(ObjectOperationMap::value_type(OBJ_FAXRECEIVEDPRINT,oprSet));
	oprSet.clear();
	/*Macro is introduced to identify separate product.Conditional compilation is
	done.Below 3 lines are not executed in LOIRE/ALABAMA/MOSEL/ECOLOIRE*/
	#if !defined (PRODUCT_LOIRE) && !defined(PRODUCT_ALABAMA) && !defined(PRODUCT_MOSEL) 
	//Color Print
	oprSet.insert(OP_EXECUTE);
	objectOperMap.insert(ObjectOperationMap::value_type(OBJ_COLOR_PRINT,oprSet));
	//objectOperMap.insert(ObjectOperationMap::value_type(OBJ_TWINCOLOR_PRINT,oprSet));
	oprSet.clear();
	#endif
	//VArious print settings
	/*oprSet.insert(OP_EXECUTE);
	objectOperMap.insert(ObjectOperationMap::value_type(OBJ_OUTPUT_SIMPLEX,oprSet));
	objectOperMap.insert(ObjectOperationMap::value_type(OBJ_OUTPUT_LARGESIZE,oprSet));
	objectOperMap.insert(ObjectOperationMap::value_type(OBJ_OUTPUT_SPECIALPAPER,oprSet));
	objectOperMap.insert(ObjectOperationMap::value_type(OBJ_OUTPUT_1N1,oprSet));	
	opSet.clear();*/

	//StoreToLocalStorage
	oprSet.insert(OP_EXECUTE);
	//objectOperMap.insert(ObjectOperationMap::value_type(OBJ_INTERNALSTORE,oprSet)); //This is modified as per new Permission_Group file
	objectOperMap.insert(ObjectOperationMap::value_type(OBJ_INTERNALSTORE_FILESHARE_LOCAL,oprSet));
	oprSet.clear();

	//StoreToUSBDevice
	oprSet.insert(OP_EXECUTE);
	objectOperMap.insert(ObjectOperationMap::value_type(OBJ_INTERNALSTORE_USBSTORAGE,oprSet));
	oprSet.clear();

	//SendEmail
	oprSet.insert(OP_EXECUTE);
	objectOperMap.insert(ObjectOperationMap::value_type(OBJ_EMAILSEND,oprSet));
	oprSet.clear();

	//StoreToRemoteServer
	oprSet.insert(OP_EXECUTE);
	objectOperMap.insert(ObjectOperationMap::value_type(OBJ_EXTERNALSTORE,oprSet)); 
	//This is modified as per new Permission_Group file
	//objectOperMap.insert(ObjectOperationMap::value_type(OBJ_EXTERNALSTORE_FILESHARE,oprSet));
	//objectOperMap.insert(ObjectOperationMap::value_type(OBJ_EXTERNALSTORE_FILESHARE_SMB,oprSet));
	//objectOperMap.insert(ObjectOperationMap::value_type(OBJ_EXTERNALSTORE_FILESHARE_HOME,oprSet));
	//objectOperMap.insert(ObjectOperationMap::value_type(OBJ_EXTERNALSTORE_FTP,oprSet));
	//objectOperMap.insert(ObjectOperationMap::value_type(OBJ_EXTERNALSTORE_NETWARE,oprSet));
	//oprSet.clear();

	//DistributedScan
	/*oprSet.insert(OP_EXECUTE);
	objectOperMap.insert(ObjectOperationMap::value_type(OBJ_EXTERNALSTORE_DISTRIBUTEDSCAN,oprSet));
	oprSet.clear();*/
	
	//WSSCANPUSH
	oprSet.insert(OP_EXECUTE);
	objectOperMap.insert(ObjectOperationMap::value_type(OBJ_EXTERNALSTORE_WSPUSHSCAN,oprSet));
	oprSet.clear();

	//DeviceSetting
	oprSet.insert(OP_EXECUTE);
	objectOperMap.insert(ObjectOperationMap::value_type(OBJ_DEVICESETTING,oprSet));
	oprSet.clear();
	
	//Cloning
	/*oprSet.insert(OP_EXECUTE);
	objectOperMap.insert(ObjectOperationMap::value_type(OBJ_DEVICE_CLONE,oprSet));
	oprSet.clear();*/

	//User/Dept Management
	oprSet.insert(OP_EXECUTE);
	objectOperMap.insert(ObjectOperationMap::value_type(OBJ_USERMANAGEMENT,oprSet));
	oprSet.clear();

	//Job Operation
	oprSet.insert(OP_EXECUTE);
	objectOperMap.insert(ObjectOperationMap::value_type(OBJ_JOB_MANAGEMENT,oprSet));
	//objectOperMap.insert(ObjectOperationMap::value_type(OBJ_JOB_DELETE,oprSet));
	oprSet.clear();

	//Export Log
	oprSet.insert(OP_EXECUTE);
	//objectOperMap.insert(ObjectOperationMap::value_type(OBJ_LOG_EXPORT,oprSet)); //This is modified as per new Permission_Group file
	objectOperMap.insert(ObjectOperationMap::value_type(OBJ_LOG_EXPORT,oprSet));
	//objectOperMap.insert(ObjectOperationMap::value_type(OBJ_MESSAGE_LOG_EXPORT,oprSet));
	oprSet.clear();

	//Read Log
	oprSet.insert(OP_EXECUTE);
	//objectOperMap.insert(ObjectOperationMap::value_type(OBJ_LOG_ACCESS,oprSet)); //This is modified as per new Permission_Group file
	objectOperMap.insert(ObjectOperationMap::value_type(OBJ_LOG_ACCESS,oprSet));
	//objectOperMap.insert(ObjectOperationMap::value_type(OBJ_MESSAGE_LOG_ACCESS,oprSet));
	oprSet.clear();

	//E-Filing
	//oprSet.insert(OP_EDIT);
	oprSet.insert(OP_READ);
	//oprSet.insert(OP_EXPORT); //This is modified as per new Permission_Group file
	//oprSet.insert(OP_SAVE);
	//oprSet.insert(OP_PRINT);
	//oprSet.insert(OP_RETRIEVE);
	objectOperMap.insert(ObjectOperationMap::value_type(OBJ_EFILING,oprSet));
	oprSet.clear();

	//AddressBook
	//oprSet.insert(OP_EDIT);
	oprSet.insert(OP_READ);
	//objectOperMap.insert(ObjectOperationMap::value_type(OBJ_ADDRESSBOOK,oprSet));
	oprSet.clear();

	//JobTemplate
	//oprSet.insert(OP_EDIT);
	oprSet.insert(OP_READ);
	//objectOperMap.insert(ObjectOperationMap::value_type(OBJ_JOBTEMPLATE,oprSet));
	oprSet.clear();

	//AccessDiagonosticMode
	oprSet.insert(OP_EXECUTE);
	objectOperMap.insert(ObjectOperationMap::value_type(OBJ_DEVICE_MEINTENANCE,oprSet));
	oprSet.clear();
	
	//EWBAccess
	oprSet.insert(OP_EXECUTE);
        objectOperMap.insert(ObjectOperationMap::value_type(OBJ_EWB_ACCESS,oprSet));
        oprSet.clear();
	
	//USBDirectPrint
	oprSet.insert(OP_EXECUTE);
        objectOperMap.insert(ObjectOperationMap::value_type(OBJ_USBDIRECTPRINT,oprSet));
        oprSet.clear();

	//Remote service technician
	oprSet.insert(OP_EXECUTE);
	objectOperMap.insert(ObjectOperationMap::value_type(OBJ_DEVICE_REMOTE_MAINTENANCE ,oprSet));
	oprSet.clear();
	
	return STATUS_OK;
}
Status CRoleManager::GetSystemUserToken(SSDKUserTokenInterface* & userToken)
{
	//Get the AuToProcessing usertoken
	 ssdk::SSDKStatus status;

         Ref<SSDKSecurityManagerInterface> ssdkSecurityManagerInterface =ssdk::GetSecurityManager();

         Ref<SSDKUserTokenInterface> usertokenInterface = dynamic_cast<SSDKUserTokenInterface*> (ssdkSecurityManagerInterface->GetInterface(SSDKSecurityManagerInterface::USER_TOKEN_INTERFACE,status));
         if((status!=OK) || (usertokenInterface == (void*)NULL))
         {
         	DEBUGL1("\n ssdk SecurityManager::GetInterface(USER_TOKEN_INTERFACE) Failed \n");
                return STATUS_FAILED;
         }
         SSDKUserTokenInterface::AuthenticateUserInfo emptyuserInfo;
         emptyuserInfo.m_UserId  = "";
         emptyuserInfo.m_UserPassword= "";
         status=usertokenInterface->Authenticate(SSDKUserTokenInterface::TOKEN_CONTEXT_AUTOPROCESSING,userToken);
         if(userToken==NULL)
         {
         	DEBUGL1("Invalid UserToken, UserToken is NULL\n");
         	return STATUS_FAILED;
         }
	return STATUS_OK;
}


void CRoleManager::LoadCategoryDisplayMap()
{
	//Load the CategoryDisplayNameMap
	//CopyFunction
	set<CString> oprSet;
	oprSet.insert(OPR_COPY_JOB);
	CRoleManager::categoryDisplayMap.insert(CategoryDisplayNameMap::value_type(CAT_COPY_FUNC,oprSet));
	oprSet.clear();

	//PrintFunction
	oprSet.insert(OPR_PRINT_JOB);
	oprSet.insert(OPR_PRINT_MGNT);
	CRoleManager::categoryDisplayMap.insert(CategoryDisplayNameMap::value_type(CAT_PRINT_FUNC,oprSet)); 
	oprSet.clear();

	//ScanFunction
	oprSet.insert(OPR_REMOTESCAN);
	CRoleManager::categoryDisplayMap.insert(CategoryDisplayNameMap::value_type(CAT_SCAN_FUNC,oprSet));
	oprSet.clear();

	//Fax/iFaxFunction
	oprSet.insert(OPR_INTERNET_FAX_TRANS);
	oprSet.insert(OPR_FAX_TRANS);
	oprSet.insert(OPR_FAX_RECVD_PRINT);
	CRoleManager::categoryDisplayMap.insert(CategoryDisplayNameMap::value_type(CAT_FAX_IFAX_FUNC,oprSet));
	oprSet.clear();

	//OutputSetting
	oprSet.insert(OPR_COLOR_PRINT);
	oprSet.insert(OPR_VARIOUS_PRINT_SETTING);
	CRoleManager::categoryDisplayMap.insert(CategoryDisplayNameMap::value_type(CAT_OUTPUT_SETTING,oprSet));
	oprSet.clear();

	//LocalFileShare
	oprSet.insert(OPR_LOCAL_STORAGE);
	oprSet.insert(OPR_USB_STORAGE);
	CRoleManager::categoryDisplayMap.insert(CategoryDisplayNameMap::value_type(CAT_LOCAL_FILE_SHARE,oprSet));
	oprSet.clear();

	//Remote
	oprSet.insert(OPR_SEND_EMAIL);
	oprSet.insert(OPR_REMOTE_STORE);
	oprSet.insert(OPR_DISTRIBUTED_SCAN);
	oprSet.insert(OPR_WSSCAN);
	CRoleManager::categoryDisplayMap.insert(CategoryDisplayNameMap::value_type(CAT_REMOTE,oprSet));
	oprSet.clear();
	
	//e-Filing
	oprSet.insert(OPR_EFILING_ACCESS);
	CRoleManager::categoryDisplayMap.insert(CategoryDisplayNameMap::value_type(CAT_EFILING,oprSet));
	oprSet.clear();

	//AddressBook
	oprSet.insert(OPR_READ);
	//oprSet.insert(OPR_EDIT);
	//CRoleManager::categoryDisplayMap.insert(CategoryDisplayNameMap::value_type(CAT_ADDRS_BOOK,oprSet));
	oprSet.clear();

	//JobTemplate
	oprSet.insert(OPR_READ);
	//oprSet.insert(OPR_EDIT);
	//CRoleManager::categoryDisplayMap.insert(CategoryDisplayNameMap::value_type(CAT_JOB_TEMPLATE,oprSet));
	oprSet.clear();

	//DeviceSetting
	oprSet.insert(OPR_DEVICE_SETTING);
	oprSet.insert(OPR_CLONE);
	CRoleManager::categoryDisplayMap.insert(CategoryDisplayNameMap::value_type(CAT_DEVICE_SETTING,oprSet));	
	oprSet.clear();

	//User/DepartmentManagement
	oprSet.insert(OPR_USER_DEPT_MGNT);
	CRoleManager::categoryDisplayMap.insert(CategoryDisplayNameMap::value_type(CAT_USER_DEPT_MGNT,oprSet));
	oprSet.clear();

	//LogManagement
	oprSet.insert(OPR_LOG_READ);
	oprSet.insert(OPR_LOG_EXPORT);
	CRoleManager::categoryDisplayMap.insert(CategoryDisplayNameMap::value_type(CAT_LOG_MGNT,oprSet));
	oprSet.clear();

	//JobManagement	
	oprSet.insert(OPR_JOB_OPERATION);
	CRoleManager::categoryDisplayMap.insert(CategoryDisplayNameMap::value_type(CAT_JOB_MGNT,oprSet));
	oprSet.clear();
	return ;
}
Status CRoleManager::Run()
{
	Status retStatus = STATUS_OK;
	CString sPath = RBACSETTINGS_DOM_PATH;
	CString sDocName = RBACSETTINGS_DOM_NAME;
	DocumentRef pDoc = NULL;
	retStatus = m_pHDB->OpenDocument(sDocName, pDoc, sPath);
	if(STATUS_OK != retStatus) // DOM not present or Corrupted.
	{
		DEBUGL6("Open RbacSettings dom failed with status %d. Creating dom with settings from config\n",retStatus);
		retStatus = m_pHDB->DeleteDocument(sDocName, sPath);
		if(STATUS_OK != retStatus)
		{
			DEBUGL1("Failed to delete RbacSettings DOM during bootup check. Status->%d\n", retStatus);
		}
		if(STATUS_OK != CreateRbacSettingsDom())
		{
			DEBUGL1("Failed to create RbacSettings dom\n");
		}
	}

	/* msgPort deletion issue fix by calling acquire which will increase msgport ref count */
	 Ref<ci::servicestartupmanager::Client> ssmClient;
         ssmClient = ci::servicestartupmanager::Client::Acquire(m_pAccsgPort,SERVICE_AL_ROLEMANAGER_APPLICATION);
         //Check for NULL
         if ( !ssmClient)
         {
         	DEBUGL2("RoleManager::Failed to Acquire ssm client \n");
         	throw CRoleException(STATUS_FAILED);
         }

	//Importrolemapping is called during bootup  when rolerulemapping file exists and upgradeFileForRuleFlag file is present. fixed for EBX_STFR_18235
	//ImportRoleRuleMapping::ExecuteSSDK() is called directly with dummy command object

	try
	{
		if(File::Exists(upgradeFileForRuleFlag))
		{
			SSDKStatus status = OK;
			SSDKUserTokenInterface * sysToken = NULL;
			if(STATUS_OK != GetSystemUserToken(sysToken))
			{
				DEBUGL1("GetSystemUserToken Failed.\n");	
			}
			else
			{	
				Ref<SSDKSecurityManagerInterface> ssdkSecurityManagerInterface =ssdk::GetSecurityManager();	
				Ref<SSDKRBACMgmtInterface> rbacRef = dynamic_cast<SSDKRBACMgmtInterface*>(ssdkSecurityManagerInterface->GetInterface(SSDKSecurityManagerInterface::RBAC_MGMT_INTERFACE, status));
				if(rbacRef == (void*)NULL || status != OK)
				{
					DEBUGL1("Getting SSDKRBACMgmtInterface Failed. Status:%d\n", status);
				}
				else
				{
					//Get SSDK rule attribute from DB and check if it already encoded(upgrade case from supported ROM to Suppoerted ROM Ex: L6.01 to L6.02)
					//If rule is encoded then don't call ExecuteSSDK
					bool ruledecoded = true;
					vector<SSDKRBACMgmtInterface::RoleInfo> roleInfos;
					status = rbacRef->GetProperty(sysToken,SSDKRBACMgmtInterface::ROLE_RULES_PROPERTY,"",roleInfos);
					if(roleInfos.size())
					{
						vector<SSDKRBACMgmtInterface::RoleInfo>::iterator it;
						for (it=roleInfos.begin();it != roleInfos.end();it++)
						{
							if((it->m_RoleRules).size())
							{
								//If rules are found in DB,then rules will be decoded and checked if it is in correct encoded format
								//Only one rule is considered. If rule is not encoded then, ruledecoded is false and ImportRolerule will be called
								//otherwise it will not be called and upgradeFileForRuleFlag will be deleted
								vector<string> decodevector;
								EncodeDecode decodeObject;
								if(STATUS_OK != decodeObject.Decode((it->m_RoleRules).front(),decodevector))
								{
									ruledecoded = false;
									DEBUGL2("ssdkrule Decode failed. Importing rolerulemapping file again!\n");
								}
								else
								{
									DEBUGL8("Decode successful. No need to Import rolerulemapping file\n");
									if(STATUS_OK !=  (retStatus=File::DeleteFile(upgradeFileForRuleFlag)))
										DEBUGL2("Failed to Delete the upgradeFileForRule File. Status : %d\n",retStatus);
								}
								break;
							}
						}
					}
					//If ruledecoded is false that means ssdkrules in DB are not encoded
					//So calling ImportRoleRuleMapping to Import the ssdkrule
					if(!ruledecoded)
					{	
						const char*  pMsgXML = "<Command uuid='dummy'><dummyCommand><commandNode docName='hdb:/work/al/etc/dom/RBACManager' deltaDocName='dummy'>RBACManager/Roles</commandNode><Params><roleDetails contentType='XPath' docName='hdb:/work/al/etc/dom/RBACManager' deltaDocName='dummy'>RBACManager/View</roleDetails></Params><sessionID>DummySession</sessionID></dummyCommand></Command>";
						if (STATUS_OK !=(retStatus = m_pHDB->CreateTempDocument("CommandDocforrolerule",pDoc,pMsgXML)))
						{
							DEBUGL2("CRoleManager::Failed To CREATE TEMP DOCUMENT FROM XML. Status : %d\n",retStatus);
						}
						else
						{
							Ref<CCommand> pCommand = NULL;
							pCommand = new CCommand(pDoc,m_pHDB);
							Ref<ImportRoleRuleMapping> Iobj = new ImportRoleRuleMapping(pCommand);
							if(Iobj == (void*)NULL)
								DEBUGL1("Creating ImportRoleRuleMapping object failed");
							else
							{
								//making m_upgradeRoleMappingFlag member variable value to true. By default it will be false
								Iobj->m_upgradeRoleMappingFlag = true;
								retStatus = Iobj->ExecuteSSDK(currentRoleSetting);
								DEBUGL8("ExecuteSSDK status is %d\n",retStatus);
							}

						}
					}
					//If ruledecoded is true that means ssdkrules in DB are encoded or ssdkrules are not present in DB. In both cases ImportRoleRuleMapping is not called and upgradeFileForRuleFlag file is deleted.
					else
					{
						DEBUGL8("Decode successful(or ssdkrule is not present in DB). No need to Import rolerulemapping file\n");
						if(STATUS_OK !=  (retStatus=File::DeleteFile(upgradeFileForRuleFlag)))
							DEBUGL2("Failed to Delete the upgradeFileForRule File. Status : %d\n",retStatus);
					}
				}
			}

		}
	}

	catch(CRoleException& except)
	{
		retStatus = (Status)(except);
		DEBUGL1("Exception for ImportRoleRuleMapping In RoleManager::Run().... Continuing  with Msg Receive. retStatus : %d\n",retStatus);

	}
		

	catch(...)
	{
		DEBUGL1("Unknown Exception for ImportRoleRuleMapping In RoleManager::Run().... Continuing  with Msg Receive\n");
	}


	try
	{ 
		Ref<Msg> pMsg = new Msg();
		while(1)
		{
			DEBUGL8("IN RoleManager::Run ..WAITING FOR MESSAGES\n"); 			
			if (m_pAccsgPort->Receive(*pMsg) != STATUS_OK)
			{
				DEBUGL2("RoleManager::Run()->Failed in ReceiveDirectMsgs\n");
				continue;
			}
			DEBUGL8("\n IN ReceiveDirectMsgs() ..GOT MESSAGE WITH ID = '0x%x', FROM PORT = '0x%x'\n",pMsg->GetId(),pMsg->GetSender());
			ProcessMessage(pMsg);
		}//end of while()
	}
	catch(CRoleException& except)
	{
		DEBUGL2("RoleManager::Failed in RoleManager::Run()\n");
		retStatus = (Status)(except);
	}
	return retStatus;
}//end of Run()


Status CRoleManager::ProcessMessage(ci::operatingenvironment::Ref<ci::messagingsystem::Msg> & pMsg)
{
	Status retStatus = STATUS_OK;
	try
	{
			
#if 0				
		if(m_bAllDependentServicesUpAndRunning || (pMsg->GetId() & CONTRACT_ID_MASK) == CI::CID_CI_SSM)
		{
#endif
			switch(pMsg->GetId() & CONTRACT_ID_MASK )
			{
				
#ifndef NO_SSM
			case ci::CID_CI_SSM:
				{
					
					retStatus = ProcessSSMMessage(pMsg);
				}
				break;
#endif
			case CID_AL_BO_SERVER:  // Handle messages from BOServer
				{
					DEBUGL8("RoleManager::ProcessMessage--> From BOContracts : %d\n ",pMsg->GetId());
					if(pMsg->GetId() == BOContracts::COMMAND){
						ProcessUICMsg(pMsg);	
					}
				}
			break;
				default:
				{
					DEBUGL2("\n BO::CBOREPOSITORY::ProcessMessage(). CANNOT HANDLE MSG '0x%x' FROM PORT '0x%x'\n",pMsg->GetId(),pMsg->GetSender());
					throw CRoleException(STATUS_FAILED);						
				}	
				break;
			}//end of switch()
#if 0
		}
		else
		{
			DEBUGL2("RoleManager::ProcessMessage: Not processing this message because all dependent services \
					are not ready m_bAllDependentServicesUpAndRunning = %d\n",m_bAllDependentServicesUpAndRunning);
		}
#endif			
	}
	catch(CRoleException& except)
	{
		DEBUGL2("RoleManager::Failed in CRoleManager::ProcessMessage()\n");
		retStatus = (Status)(except);
	}
	return retStatus;
}//end of ProcessMessage()

Status CRoleManager::ProcessSSMMessage(ci::operatingenvironment::Ref<ci::messagingsystem::Msg> & pMsg)
{
	Status retStatus = STATUS_OK;
	uint32 uMsgId = pMsg->GetId();
	try
	{
		if(uMsgId==ci::servicestartupmanager::SSMContracts::STOP)
		{
			//Got shut down message from SSM... So Send ShutDown message back to SSM
						DEBUGL8("RoleManager::ProcessSSMMsg: received SSMContracts::STOP message from SSM\n");
#ifndef NO_SSM

						ci::servicestartupmanager::SSMContracts::stServiceStateBus stShutDownBus;
						stShutDownBus.serviceState = (SSMContracts::eServiceState)SSMContracts::eShuttingDown;
						size_t length = static_cast<size_t>(static_cast<int>(CString(SERVICE_AL_ROLEMANAGER_APPLICATION).length()+1));
						strncpy(stShutDownBus.sServiceName,SERVICE_AL_ROLEMANAGER_APPLICATION,length);
						stShutDownBus.sServiceName[strnlen(SERVICE_AL_ROLEMANAGER_APPLICATION,12)]='\0';
						stShutDownBus.portID = MSGPORT_AL_ROLEMANAGER_APPLICATION ;
						stShutDownBus.iNextMessageInterval = 0;
						stShutDownBus.noOfReadyPendingServices = 0;
						stShutDownBus.iDataSchemaVersion = 1;
						stShutDownBus.iDataLength = 0;
						*stShutDownBus.data ='\0';

						DEBUGL8("RoleManager::ProcessSSMMsg: Sending SHUTTINGDOWN to SSM\n");
						//Ref<ci::servicestartupmanager::Client> ssmClient=ci::servicestartupmanager::Client::Acquire(m_port,SERVICE_AL_ROLEMANAGER_APPLICATION);  
						Ref<ci::servicestartupmanager::Client> ssmClient=ci::servicestartupmanager::Client::Acquire(m_pAccsgPort,SERVICE_AL_ROLEMANAGER_APPLICATION);
						//Check for NULL
						if(!ssmClient)
						{
							DEBUGL2("Failed to Acquire ssm client\n");
							throw CException(STATUS_FAILED);
						}
						Status ret = ssmClient->SendStateNotification(stShutDownBus);
						if (ret != STATUS_OK) 
						{
							DEBUGL2("RoleManager::ProcessSSMMsg: Failed to send eShuttingDown messages to SSM\n");
							throw CException(STATUS_FAILED);
						}
						/** After Sending ShutDown Messages to all the Subscribers, send Shutdown message to SSM **/
				
						DEBUGL8(" RoleManager::Sending SHUTTINGDOWN to SSM\n");
						ret = ssmClient->SendStateNotification(stShutDownBus);
						if (ret != STATUS_OK)
						{
							DEBUGL2(" RoleManager::Receive Failed to send eShuttingDown messages to SSM\n");
							throw CException(STATUS_FAILED);
						}
						//Exit the application
						exit(0);
#endif
		}


	
		else if (uMsgId == ci::servicestartupmanager::SSMContracts::NOTIFY_SERVICE_STATE)
		{
			SSMContracts::stStateNotificationBus *pSSMBus = (SSMContracts::stStateNotificationBus *)pMsg->GetContentPtr();
			if (pSSMBus)
			{
				switch(pSSMBus->serviceState)
				{
					case SSMContracts::ecStartPending:
					break;
					case SSMContracts::ecReady:
					{
						DEBUGL8("CRoleManager::ProcessSSMMessage: Number of Ready-Pending Services is %d \n",pSSMBus->noOfReadyPendingServices);
						if ((pSSMBus->noOfReadyPendingServices == 0) && !m_bAllDependentServicesUpAndRunning)
						{
						// Acquire SSM  client
							Ref<ci::servicestartupmanager::Client> ssmClient;
							ssmClient = ci::servicestartupmanager::Client::Acquire(m_pAccsgPort,SERVICE_AL_ROLEMANAGER_APPLICATION);
							if(!ssmClient)
							{
								DEBUGL1("CRoleManager::ProcessSSMMessage SSMClient::Acquire Failed\n");
								throw CRoleException(STATUS_FAILED);
							}
	
							// Send ecReady notification to SSM
							DEBUGL8("CRoleManager::ProcessSSMMessage::Requesting Client library to Notify SSM, status = ecReady\n");
							if(STATUS_OK == ssmClient->Notify((SSMContracts::ecNotifications)SSMContracts::ecReady))
							{
								DEBUGL8("CRoleManager::ProcessSSMMessage::Client library Notified SSM successfully, status = ecReady\n");
							}
							else
							{
								DEBUGL1("CRoleManager::ProcessSSMMessage::Client library failed to Notify SSM, status = ecReady\n");
								throw CException(STATUS_FAILED);
							}
							m_bAllDependentServicesUpAndRunning = true;
							DEBUGL8("CRoleManager::ProcessSSMMessage::CRoleManagerApplication is Up and Ready!!!...\n");
							fflush(stdout);
						}
					}
					break;
				
					case SSMContracts::ecShuttingDownPending:
					case SSMContracts::ecShuttingDown:
					case SSMContracts::ecHeartBeat:
					break;
					
					case SSMContracts::eSubscribedServiceDead:
					{
						if(m_bAllDependentServicesUpAndRunning)
						{
							DEBUGL8("CRoleManager::ProcessSSMMessage::Received eSubscribedServiceDead for %s \n", pSSMBus->sServiceName);
							DEBUGL8("CRoleManager::ProcessSSMMessage::Now switching to ecStartPending state \n");
							// Acquire SSM  client
							Ref<ci::servicestartupmanager::Client> ssmClient;
							ssmClient = ci::servicestartupmanager::Client::Acquire(m_pAccsgPort,SERVICE_AL_ROLEMANAGER_APPLICATION);
							if(!ssmClient)
							{
								DEBUGL1("CRoleManager::ProcessSSMMessage SSMClient::Acquire Failed\n");
								throw CException(STATUS_FAILED);
							}
							if (ssmClient->Notify((SSMContracts::ecNotifications)SSMContracts::ecStartPending) != STATUS_OK)
							{
								DEBUGL1("CRoleManager::ProcessSSMMessage Failed to send NOTIFICATION (ecStartPending) message to SSM\n");
								throw CException(STATUS_FAILED);
							}
							else
							{
								DEBUGL8("CRoleManager::ProcessSSMMessage Sent NOTIFICATION (ecStartPending) message to SSM\n");
							}
							// No more i/p messages will be processed till dependent services are up and running (ecReady)
							m_bAllDependentServicesUpAndRunning = false;
						}
						
					}
					break;
					default:
					DEBUGL4("DEFAULT\n");
					break;
				}
			}
			else
			{
				DEBUGL1("CRoleManager::ProcessSSMMessage ssmBus is NULL\n");
				throw CException(STATUS_FAILED);
			}
		}
				
	}
	catch(CRoleException& except)
	{
		DEBUGL2("RoleManager::Failed in CRoleManager::ProcessMessage()\n");
		retStatus = (Status)(except);
	}
				
	return retStatus;
				
}
				
/**
*	Process the UIController messages and call the apropriate command handlers
* 	o handle that message.
*/
Status CRoleManager::ProcessUICMsg(ci::operatingenvironment::Ref<ci::messagingsystem::Msg> & pMsg)
{


  Status retStatus = STATUS_OK;
  DocumentRef pDoc = NULL;
  Ref<CCommand> pCommand = NULL;
  Ref<CCommandHandler> pObj = 0;


  try
     {

        const char *pMsgXML = reinterpret_cast<const char *>(pMsg->GetContentPtr());
	DEBUGL8("THE INCOMMING MESSAGE ::%s",pMsgXML);
          if (!pMsgXML)
          {
               DEBUGL2("CRoleManager::COMMAND XML IS NULL\n");
              throw CRoleException(STATUS_FAILED);
          }

         if (STATUS_OK !=(retStatus = m_pHDB->CreateTempDocument("CommandDocument",pDoc,pMsgXML)))
          {
               DEBUGL2("CRoleManager::Failed To CREATE TEMP DOCUMENT FROM THIS '%s' XML\n",pMsgXML);
               throw CRoleException(STATUS_FAILED);
          }

        //Create a Command Object to decode the contents of Incoming command.
         pCommand = new CCommand(pDoc,m_pHDB);
	 CString cmdName = pCommand->GetCommandName();
	 DEBUGL8("\n Command Name :: %s \n",cmdName.c_str());
//Ref<SSDKUserTokenInterface> userToken = pMsg->GetToken();
	CRoleCommandHandler* permCheckObj; 
         if (pCommand->GetCommandName() == CMD_GET_ROLES)
	{
                pObj = new GetRoles(pCommand);
//if(pObj)
//	pObj->SetUserToken(userToken);		
         }
	else if (pCommand->GetCommandName() == CMD_CREATE_ROLE)
	{
	      pObj = new CreateRole(pCommand);
	      permCheckObj = dynamic_cast<CRoleCommandHandler*>(pObj.operator->());
	      if(permCheckObj && !permCheckObj->CheckPermission())
			throw CRoleException(STATUS_AL_SEC_PERMISSION_CHECK_ERROR);
//     if(pObj)
//			pObj->SetUserToken(userToken);
         }
	else if (pCommand->GetCommandName() == CMD_DELETE_ROLE)
	{
	      pObj = new DeleteRole(pCommand);
	      permCheckObj = dynamic_cast<CRoleCommandHandler*>(pObj.operator->());
	      if(permCheckObj && !permCheckObj->CheckPermission())
			throw CRoleException(STATUS_AL_SEC_PERMISSION_CHECK_ERROR);
//	      if(pObj)
//			pObj->SetUserToken(userToken);
         }
	else if (pCommand->GetCommandName() == CMD_TRANSFER_TO_ROLE)
	{
		//This feature is not applicable but did not remove as we are not sure about whether anyone is calling or not.
	      pObj = new TransferToRole(pCommand);
	      permCheckObj = dynamic_cast<CRoleCommandHandler*>(pObj.operator->());
              if(permCheckObj && !permCheckObj->CheckPermission())
                        throw CRoleException(STATUS_AL_SEC_PERMISSION_CHECK_ERROR);
//		if(pObj)
//			pObj->SetUserToken(userToken);
         }
	else if (pCommand->GetCommandName() == CMD_MODIFY_ROLE)
	{
	      pObj = new ModifyRole(pCommand);
	      permCheckObj = dynamic_cast<CRoleCommandHandler*>(pObj.operator->());
              if(permCheckObj && !permCheckObj->CheckPermission())
			throw CRoleException(STATUS_AL_SEC_PERMISSION_CHECK_ERROR);
//		if(pObj)
//			pObj->SetUserToken(userToken);
         }
	else if (pCommand->GetCommandName()==CMD_GET_ROLE_INFO)
	{
	      pObj = new GetRoleInfo(pCommand);
//		if(pObj)
//			pObj->SetUserToken(userToken);
         }
	else if (pCommand->GetCommandName() == CMD_GET_ONLY_ROLE_NAMES)
	{
	      pObj = new GetOnlyRoleNames(pCommand);
//		if(pObj)
//			pObj->SetUserToken(userToken);
         }
	else if (pCommand->GetCommandName() == CMD_GET_ALL_OBJECTS)
	{
		//This feature is not applocable. We must remove it once we make sure nonoby is calling it.
	      pObj = new GetAllObjects(pCommand);
//		if(pObj)
//			pObj->SetUserToken(userToken);
         }
	else if (pCommand->GetCommandName() == CMD_GET_RBAC_CUSTOM_PROPERTY)
	{
		pObj = new RoleGetCustomProperty(pCommand);
//		if(pObj)
//			pObj->SetUserToken(userToken);
	}
	else if (pCommand->GetCommandName() == CMD_SET_RBAC_CUSTOM_PROPERTY)
        {
                pObj = new RoleSetCustomProperty(pCommand);
		permCheckObj = dynamic_cast<CRoleCommandHandler*>(pObj.operator->());
	        if(permCheckObj && !permCheckObj->CheckPermission())
                        throw CRoleException(STATUS_AL_SEC_PERMISSION_CHECK_ERROR);
//		if(pObj)
//			pObj->SetUserToken(userToken);
        }
	else if(cmdName=="GetPermission")
	{
		pObj = new GetPermission(pCommand);
//		if(pObj)
//           	pObj->SetUserToken(userToken);
	}
	else if(cmdName=="GetPermissionList")
	{
		pObj = new GetPermissionList(pCommand);
//if(pObj)
//	pObj->SetUserToken(userToken);
	}
	else if(cmdName=="SetPermission")
	{
		pObj = new SetPermission(pCommand);
		permCheckObj = dynamic_cast<CRoleCommandHandler*>(pObj.operator->());
	        if(permCheckObj && !permCheckObj->CheckPermission())
                        throw CRoleException(STATUS_AL_SEC_PERMISSION_CHECK_ERROR);
//if(pObj)
//	pObj->SetUserToken(userToken);
	}
	else if(cmdName=="Reset")
        {
               pObj = new Reset(pCommand);
	       permCheckObj = dynamic_cast<CRoleCommandHandler*>(pObj.operator->());
	       if(permCheckObj && !permCheckObj->CheckPermission())
			throw CRoleException(STATUS_AL_SEC_PERMISSION_CHECK_ERROR);
//              if(pObj)
//                      pObj->SetUserToken(userToken);
        }
	else if(cmdName=="ImportMappingXML")
	{
		pObj = new ImportRoleRuleMapping(pCommand);
		permCheckObj = dynamic_cast<CRoleCommandHandler*>(pObj.operator->());
		if(permCheckObj && !permCheckObj->CheckPermission())
			throw CRoleException(STATUS_AL_SEC_PERMISSION_CHECK_ERROR);
	}
	else if(cmdName=="ExportMappingXML")
	{
		pObj = new ExportRoleRuleMapping(pCommand);
		permCheckObj = dynamic_cast<CRoleCommandHandler*>(pObj.operator->());
		if(permCheckObj && !permCheckObj->CheckPermission())
			throw CRoleException(STATUS_AL_SEC_PERMISSION_CHECK_ERROR);
	}
	else if(cmdName == "ImportRoleInfo")
        {
                pObj = new ImportRoleInfo(pCommand);
		permCheckObj = dynamic_cast<CRoleCommandHandler*>(pObj.operator->());
		if(permCheckObj && !permCheckObj->CheckPermission())
			throw CRoleException(STATUS_AL_SEC_PERMISSION_CHECK_ERROR);
        }
	else if(cmdName == "ExportRoleInfo")
	{
		pObj = new ExportRoleInfo(pCommand);
		permCheckObj = dynamic_cast<CRoleCommandHandler*>(pObj.operator->());
		if(permCheckObj && !permCheckObj->CheckPermission())
			throw CRoleException(STATUS_AL_SEC_PERMISSION_CHECK_ERROR);
	}
	else if(cmdName == "GetRbacSettings")
	{
		pObj = new GetRbacServerSettings(pCommand);
	}
	else if(cmdName == "SetRbacSettings")
	{
		pObj = new SetRbacServerSettings(pCommand);
		permCheckObj = dynamic_cast<CRoleCommandHandler*>(pObj.operator->());
	        if(permCheckObj && !permCheckObj->CheckPermission())
                        throw CRoleException(STATUS_AL_SEC_PERMISSION_CHECK_ERROR);
	}
	else if(cmdName == "GenerateClone")
	{
		pObj = new GenerateClone(pCommand);
	}
	else if(cmdName == "ApplyClone")
	{
		pObj = new ApplyClone(pCommand);
	}
        else
        {
              DEBUGL2("CRoleManager::ProcessUICmessage()-->CANNOT HANDLE COMMAND '%s' \n",pCommand->GetCommandName());
              throw CRoleException(STATUS_FAILED);
        }
    }
    catch(DOMException & domExcept)
    {
                DEBUGL1("\n DOM Exception..\n");
                retStatus = STATUS_FAILED;
    }
    catch(CException& except)
    {
             DEBUGL2("CRoleManager::CRequestProcessor::ProcessUICMsg()-->Failed\n");
             retStatus = except;
             AppendStatusOfCommandOperation(pCommand->GetCommandRoot(),retStatus);
    }
    catch(...)
    {
                DEBUGL1("\n CRoleManager::ProcessUICMsg:: Caught ALL Exception !! \n");
                retStatus = STATUS_FAILED;
    }

     if(retStatus == STATUS_OK)
     {
	try{
        	retStatus = pObj->Execute();
	}
	catch(DOMException & domExcept)
        {
                DEBUGL1("\n DOM Exception..\n");
                retStatus = STATUS_FAILED;
        }
	catch(CRoleException& userExcept)
	{
		DEBUGL1("\n CRoleManager::ProcessUICMsg::Caught CRoleException ...!! \n");
		retStatus = STATUS_FAILED;
	}
	catch(...)
	{
		DEBUGL1("\n CRoleManager::ProcessUICMsg:: Caught ALL Exception !! \n");
		retStatus = STATUS_FAILED;	
	}
        //AppendStatusOfCommandOperation(pCommand->GetCommandRoot(),retStatus);
     }
     	if (retStatus == STATUS_OK)
        	Utils::StatusOfCommandOperation(pCommand->GetCommandRoot(),"STATUS_OK");
	else
	{
		RbacManagerErrorCodeMap::iterator iter;
                iter = CRoleManager::m_errorMap.find(retStatus);
                DEBUGL8("RBAC_MANAGER RETURN STATUS :: %u\n",retStatus);
                if(iter!=CRoleManager::m_errorMap.end())
                {
			//Utils::StatusOfCommandOperation(pCommand->GetCommandRoot(),"STATUS_FAILED");
			Utils::StatusOfCommandOperation(pCommand->GetCommandRoot(),(iter->second).c_str());
                        DEBUGL8("WE ARE HERE\n");
                }
		else
                {
                        Utils::StatusOfCommandOperation(pCommand->GetCommandRoot(),"STATUS_FAILED");
                        DEBUGL8("WE ARE HERE 1\n");
                }
	}
        CString sResponse = "";
     	if (m_pHDB->Serialize(pDoc,sResponse) != STATUS_OK)
     	{
            	DEBUGL2("CRequestProcessor::Param::Failed To Serialize Response\n");
     	}
      //Send the Command Response back.
      Msg mResponse = pMsg->Reply(static_cast<void*>(const_cast<char*>(sResponse.c_str())),static_cast<unsigned int>(static_cast<int>(sResponse.size()) + 1));
      DEBUGL6("CRoleManager::HandleCommand()--> SENDING RESPONSE FROM JOBSPLUGIN FOR EXECUTECOMMMAND REQUEST. RESPONSE = '%s' \n",sResponse.c_str());
     if(m_pAccsgPort->Send(mResponse) != STATUS_OK)
       DEBUGL2("CRoleManager::FAILED TO SEND COMMAND RESPONSE\n");

         return retStatus;

}//end of 

/* GetAutoProcessingToken
 * Description : Get the auto processing user token
 */
Status CRoleManager::GetAutoProcessingToken(SSDKUserTokenInterface* & userToken)
{
	//Get the user token interface
	ssdk::SSDKStatus status;
	Status ret = STATUS_AL_SEC_INTERNAL_ERROR;
	Ref<SSDKSecurityManagerInterface> ssdkSecurityManagerInterface = ssdk::GetSecurityManager();
	Ref<SSDKUserTokenInterface> userInterface = dynamic_cast<SSDKUserTokenInterface*> (ssdkSecurityManagerInterface->GetInterface(SSDKSecurityManagerInterface::USER_TOKEN_INTERFACE, status));
	if(userInterface == (void *)NULL || status != OK)
	{
		DEBUGL1("Casting to SSDKUserTokenInterface* failed\n");
		return STATUS_AL_SEC_INTERNAL_ERROR;
	}
	//Get the auto processing user token
	status = userInterface->Authenticate(SSDKUserTokenInterface::TOKEN_CONTEXT_AUTOPROCESSING, userToken);
	if(OK != status  && AUTH_GOOD_AUTHENTICATION_NEEDS_PASSWORD_CHANGE != status)
	{
		DEBUGL1("::UNABLE TO AUTHENTICATE AND GET SYSTEM USER TOKEN\n");
		//ret = SSDK_RBAC_MANAGER_PLUGIN_ERROR_MAP(status);
		return ret;
	}
	DEBUGL8("Authenticated System User Token %d\n",status);
	return STATUS_OK;	
}

Status CRoleManager::CreateRbacSettingsDom()
{
	DEBUGL8("CreateRbacSettingsDom called\n");	
	ssdk::SSDKStatus ssdkStatus;
	Ref<SSDKSecurityManagerInterface> ssdkSecurityManagerInterface = ssdk::GetSecurityManager();
	Ref<SSDKConfigInterface> configPtr = dynamic_cast<SSDKConfigInterface*> (ssdkSecurityManagerInterface->GetInterface  (SSDKSecurityManagerInterface::CONFIG_INTERFACE,ssdkStatus));
	if(configPtr ==(void*) NULL || ssdkStatus != OK)
	{
		DEBUGL1("\n ssdk SecurityManager::SSDKConfigInterface Failed \n");
		return STATUS_AL_SEC_INTERNAL_ERROR;
	}
	SSDKUserTokenInterface * sysToken = NULL;
	if(STATUS_OK != GetSystemUserToken(sysToken))
	{
		DEBUGL1("GetSystemUserToken failed with status");	
		return STATUS_FAILED;
	}
	CString ewbAccessPermFromConfig = configPtr->GetProperty(sysToken, SSDKConfigInterface::SA_RBACPERMEWBACCESSENABLE, ssdkStatus);
	if(OK != ssdkStatus)
	{
		DEBUGL1("Failed to get value from config for SA_RBACPERMEWBACCESSENABLE\n. Status: %d\n",ssdkStatus);
		return STATUS_FAILED;
	}
	CString usbPrintPermFromConfig = configPtr->GetProperty(sysToken, SSDKConfigInterface::SA_RBACPERMUSBPRINTENABLE, ssdkStatus);
	if(OK != ssdkStatus)
	{
		DEBUGL1("Failed to get value from config for SA_RBACPERMUSBPRINTENABLE  \n. Status: %d\n",ssdkStatus);
		return STATUS_FAILED;
	}
        CString ewbAccessPerm = (ewbAccessPermFromConfig == "1")?"true":"false";
        CString usbPrintPerm = (usbPrintPermFromConfig == "1")?"true":"false";
	DocumentRef pDoc = NULL;
	Status retStatus;
	char tempString[MAX_DATA_SIZE];
	snprintf(tempString,MAX_DATA_SIZE,RBACSETTINGS_XML.c_str(),ewbAccessPerm.c_str(),usbPrintPerm.c_str());
	CString domString = tempString;
	if(STATUS_OK != (retStatus = m_pHDB->CreateDocumentFromString(RBACSETTINGS_DOM_PATH,RBACSETTINGS_DOM_NAME,pDoc,domString)))
	{
		DEBUGL1("Unable to create RbacSettings DOM\n");
		return retStatus;
	}
	return STATUS_OK;

}
};
};
};
