/* parasoft suppress item PB-33 */
/* parasoft suppress item METRICS-01_duplicated_2 */
/**
 *@Copyright 2008 TOSHIBA TEC CORPORATION All rights reserved
 *@Workfile: utils.cpp$
 *@Revision: 1 $
 *@Date: 10/25/2007$ 
 *@Author: Manoj Saha 
 *@Description:
 *@Notes:
*/
#include "utils.h"
#include "CI/HierarchicalDB/DOM/node.h"
#include "CI/HierarchicalDB/DOM/document.h"
#include "CI/HierarchicalDB/DOM/element.h"
#include "CI/HierarchicalDB/hierarchicaldb.h"
#include "CI/MessagingSystem/msg.h"
#include "CI/LogManager/loginterface.h"
#include "CI/SI/ebxssdkutil.h"
#include <map>
#include "crolemanager.h"
#include <AL/al_service_name.h>

using namespace ci::logmanager;

// namespace
namespace al
{
namespace softwarefunction
{
namespace rbacmanager
{
CString Utils::getNodeText(Ref<Node> node)
{
	NodeRef txt = node->getFirstChild();
	if(txt && txt->getNodeType()==Node::TEXT_NODE) return txt->getNodeValue();
	return "";
}
//Sreekumar
Status Utils::setNodeText(Ref<Node> node, CString text)
{
	NodeRef txt = node->getFirstChild();
	if(txt && txt->getNodeType()==Node::TEXT_NODE) 
		txt->setNodeValue(text);
	else
		ci::hierarchicaldb::chelper::AppendTextNode(node, text);
	return STATUS_OK;
}
//Sreekumar
void Utils::StatusOfCommandOperation(dom::NodeRef pCommandNode,CString status)
{
	if (!pCommandNode)
	{
		DEBUGL2("BO::Failed To Append Status Node\n");
		return;
	}
	NodeRef pStatusNode = CRoleManager::m_pHDB->BindToElement(pCommandNode,"Response/statusOfOperation",true);
	if (!pStatusNode)
	{
		DEBUGL2("BO::Failed To Append Status Node\n");
		return;
	}
	Utils::setNodeText(pStatusNode, status);
}

ci::operatingenvironment::Ref<Node> Utils::GetChildByName(ci::operatingenvironment::Ref<Node> parent, CString childName)
{
        ci::operatingenvironment::Ref<NodeList> children = parent->getChildNodes();
        ci::operatingenvironment::Ref<Node> child = 0;
        int i = 0;
        for(; children &&  i < (int)children->getLength(); i++)
        {
                child = children->item(static_cast<unsigned long>(i));
                if(child->getNodeName() == childName)
                        return child;
        }
        return NULL;
}
Status Utils::RemoveChild(ci::operatingenvironment::Ref<dom::Element> pRemoveElem)
{
        ElementRef pParent = NULL;
        if(pRemoveElem)
        {
                pParent = pRemoveElem->getParentNode();
                if(pParent)
                {
                        //Remove the node pointing by pCurRoleElem
                        if(!pParent->removeChild(pRemoveElem))
                        {
                                DEBUGL1("\n Failed to remove child node <CurrentRole>\n");
                                return STATUS_FAILED;
                        }
                }
                else
                {
                        DEBUGL1("\n Failed to get Paranet node of <CurrentRole>\n");
                        return STATUS_FAILED;
                }
        }
        return STATUS_OK;
}

Status Utils::ParseRoleMap(const CString& xmlName,map<CString,vector<CString> >& roleNameAttrVector,CString& otherUser,const CString& roleNameToMapping)
{
	//Get the Element Node Ref
	CString path("RoleSetting");
	//Create a temp document
	dom::DocumentRef tempDoc=NULL;
	Status ret = STATUS_OK;
	ci::operatingenvironment::Ref<ci::hierarchicaldb::HierarchicalDB>  pHDB = NULL;
	pHDB = ci::hierarchicaldb::HierarchicalDB::Acquire(NULL);
	if (!pHDB)
	{
		DEBUGL1("HDB ACQUIRE FAILED\n");
		return STATUS_AL_SEC_INTERNAL_ERROR;
	}
	DEBUGL8("HDB ACQUIRE SUCCESS\n");
	if(STATUS_OK != (ret = pHDB->CreateTempDocumentFromFile(tempDoc,xmlName)))
	{
		DEBUGL1("CREATION OF TEMP DOC FAILED\n");
		if(STATUS_HDB_ERROR_PARSING_XML == ret)
			return STATUS_AL_SEC_BAD_REQUEST;
		return STATUS_AL_SEC_INTERNAL_ERROR;
	}
	DEBUGL8("TEMP DOC CREATION SUCCESS\n");
	dom::NodeRef domNode = pHDB->BindToElement(tempDoc,path.c_str());
	if(!domNode)
	{
		DEBUGL1("BIND TO ROLE SETTING FAILED\n");
		return STATUS_AL_SEC_BAD_REQUEST;
	}
	DEBUGL6("BINDING PASSED\n");
	ci::operatingenvironment::Ref<ci::hierarchicaldb::NodeList> roleNodeList;
	CString condition("Condition");
	CString role("Role");
	CString roleSet("RoleSet");
	CString anyOtherUser("AnyOtherUser");
	//Get any other user. Assuming any other user is not a must node.
	dom::NodeRef otherUserNode = pHDB->BindToElement(domNode,anyOtherUser,false);
	if(!otherUserNode)
	{
		DEBUGL1("Bind to AnyOtherUser failed\n");
		otherUser = "EMPTY";
	}
	else
		otherUser = otherUserNode->getTextContent();
	DEBUGL8("\nAny other user:%s\n",otherUser.c_str());
	roleNodeList=pHDB->FindNodes(domNode,roleSet);
	if(roleNodeList->getLength() == 0)
	{
		DEBUGL2("No RoleSet is present for Import\n");
		//return STATUS_AL_SEC_INTERNAL_ERROR;
	}
	unsigned int roleSetSize = roleNodeList->size();
	DEBUGL8("Number of role set:%d",roleSetSize);
	//Loop through the role set
	for(unsigned int i=0; i<roleSetSize; i++)
	{
		CString roleName;
		ci::hierarchicaldb::NodeRef roleSetNode = roleNodeList->item(static_cast<unsigned long>(i));
		//Get the role name
		dom::NodeRef targetRoleNode;
	        /*			
		targetRoleNode=pHDB->BindToElement(roleSetNode,role,false);
		if(targetRoleNode)
		{
			roleName = targetRoleNode->getTextContent();
			DEBUGL8("Role Name:%s\n",roleName.c_str());
			if( roleName.empty())
				continue;
		}
		else
		{
			DEBUGL1("Role tag is not present under RoleSet\n");
			return STATUS_AL_SEC_BAD_REQUEST;
		}
		*/
		ci::operatingenvironment::Ref<ci::hierarchicaldb::NodeList> conditionNodeList;
		conditionNodeList = pHDB->FindNodes(roleSetNode ,condition);
		if(conditionNodeList->getLength() == 0)
		{
			DEBUGL1("Condition tag is not present under RoleSet\n");
	                return STATUS_AL_SEC_BAD_REQUEST;
		}
		//Loop through the Conditions
		unsigned int conditionListSize = conditionNodeList->size();
		vector<CString> attrValueVector;
		CString temp;
		for(unsigned int j=0; j<conditionListSize; j++)
		{
			//Get the attribute and value
			CString attributeName;
			targetRoleNode=pHDB->BindToElement(conditionNodeList->item(static_cast<unsigned long>(j)),"AttributeName",false);
			if(!targetRoleNode)
			{
				DEBUGL1("AttributeName Tag is not present under Condition\n");
				return STATUS_AL_SEC_BAD_REQUEST;
			}
			attributeName = targetRoleNode->getTextContent();
			CString attributeValue;
			targetRoleNode=pHDB->BindToElement(conditionNodeList->item(static_cast<unsigned long>(j)),"AttributeValue",false);
			if(!targetRoleNode)
			{
				DEBUGL1("AttributeValue Tag is not present under Condition\n");
				return STATUS_AL_SEC_BAD_REQUEST;
			}
			attributeValue = targetRoleNode->getTextContent();
			if(attributeName.empty() || attributeValue.empty())
			{
				DEBUGL1("Either attribute name or value empty\n");
	                        return STATUS_AL_SEC_BAD_REQUEST;;
			}
			temp=attributeName+"="+attributeValue;
			DEBUGL8("ATTR:%s\n",temp.c_str());
			attrValueVector.push_back(temp);
		}
		CString encodedrule;
		EncodeDecode encodeObject;
		Status encodeStatus = encodeObject.Encode(attrValueVector,encodedrule);
				if(encodeStatus != STATUS_OK)
				{
						DEBUGL1("Encode failed\n");
						return STATUS_AL_SEC_BAD_REQUEST;
				}
		DEBUGL8("Encoded rule: %s \n",encodedrule.c_str());
               ci::operatingenvironment::Ref<ci::hierarchicaldb::NodeList> RoleNodeList;
               RoleNodeList = pHDB->FindNodes(roleSetNode ,role);
		
		unsigned int roleListSize = RoleNodeList->size();		
		if(roleListSize == 0)
                {
                        DEBUGL1("Role tag is not present under RoleSet\n");
                        return STATUS_AL_SEC_BAD_REQUEST;
		}
		
		for(unsigned int j=0; j<roleListSize; j++)
		{

			ci::hierarchicaldb::NodeRef targetRoleNodes = RoleNodeList->item(static_cast<unsigned long>(j));	
			if(targetRoleNodes)
			{
				roleName = targetRoleNodes->getTextContent();
				if( roleName.empty())
				{
					DEBUGL1("Role node is empty\n");
                                	return STATUS_AL_SEC_BAD_REQUEST;;
				}
				//this will be true for create role and modify role where roleNameToMapping is filled with new role name 
				if(roleName == roleNameToMapping)
				{
					DEBUGL8("Inserting for RoleName :%s\n",roleName.c_str());
					roleNameAttrVector[roleName].push_back(encodedrule);
					break;
				}
				//this will be true in case of import rolemap or reset
				if(roleNameToMapping.empty())
				{
					DEBUGL8("Inserting for RoleName :%s\n",roleName.c_str());
					roleNameAttrVector[roleName].push_back(encodedrule);
				}

			}
			
		}

	}

	for(map<CString,vector<CString> >::iterator it =roleNameAttrVector.begin();it !=roleNameAttrVector.end();it++)
	{
		DEBUGL8("RoleName from roleNameAttrVector:%s\n",(it->first).c_str());	

		for(vector<CString> ::iterator itr =(it->second).begin();itr != (it->second).end();itr++)
			DEBUGL8("Attributes for roleNameAttrVector:%s\n",(*itr).c_str());
	}
	return STATUS_OK;
}

set<CString> Utils::m_builtinRolesOrg;
set<CString> Utils::m_builtinRoles;

set<CString>& Utils::BuiltinRoles(bool searchByName)
{
	static bool firstTime = true;
	if(firstTime)
	{
		Ref<SSDKSecurityManagerInterface> ssdkSecurityManagerInterface =ssdk::GetSecurityManager();
		SSDKStatus statusCode = OK;
		Ref<SSDKUserTokenInterface> usertokenInterface = dynamic_cast<SSDKUserTokenInterface*> (ssdkSecurityManagerInterface->GetInterface(SSDKSecurityManagerInterface::USER_TOKEN_INTERFACE,statusCode));
		if((statusCode !=OK) || (usertokenInterface == (void*)NULL))
		{
			DEBUGL1("\n ssdk SecurityManager::getAuthInterface Failed \n");
			return m_builtinRoles;
		}
		MyUserToken tmput;
		statusCode=usertokenInterface->Authenticate(SSDKUserTokenInterface::TOKEN_CONTEXT_AUTOPROCESSING,tmput.ut);
		if(statusCode != OK)
		{
			DEBUGL1("\n ssdk SystemUser Unable to logged in \n");
		}

		Ref<SSDKRBACMgmtInterface> rbac= dynamic_cast<SSDKRBACMgmtInterface*>(ssdkSecurityManagerInterface->GetInterface(SSDKSecurityManagerInterface::RBAC_MGMT_INTERFACE,statusCode));
		if((statusCode !=OK) || (rbac == (void*)NULL))
		{
			DEBUGL1("\n ssdk SecurityManager::getAuthInterface Failed \n");
			return m_builtinRoles;
		}
		try
		{
			vector<SSDKRBACMgmtInterface::RoleInfo> roles;
			vector<SSDKRBACMgmtInterface::RoleInfo>::iterator roleIt;
			statusCode = rbac->GetProperty(tmput.ut,SSDKRBACMgmtInterface::ROLE_ALL_PROPERTIES,"",roles);
			DEBUGL5("GetProperty(ROLE_ID_PROPERTY) Status ::%d\n",statusCode);
			if(OK==statusCode)
			{
				for(roleIt = roles.begin(); roleIt != roles.end(); roleIt++)
				{
					//for case insensitive search
					string isAdminRole = rbac->GetProperty(tmput.ut,SSDKRBACMgmtInterface::RBAC_ROLE_OBJECT,roleIt->m_RoleId,IS_ADMIN_ROLE,statusCode);
					//if((isAdminRole != "true") && (temp != GUEST_ROLE_NAME))
					if(isAdminRole != "true")
					{
						SSDKRBACMgmtInterface::RoleInfo temproleInfo;
						temproleInfo = rbac->GetProperty(tmput.ut,SSDKRBACMgmtInterface::ROLE_ALL_PROPERTIES,roleIt->m_RoleId,statusCode);
						DEBUGL6("Get property status code for unique id:%d\n",statusCode);
						CString temp(roleIt->m_RoleId);
						std::transform(temp.begin(),temp.end(),temp.begin(), (int(*)(int)) tolower);
						m_builtinRolesOrg.insert(temp);
						m_builtinRoles.insert(temproleInfo.m_RoleUniqueID);
						CString temp1(roleIt->m_RoleUniqueID);
						DEBUGL6("Pushing role name:%s\n",temp.c_str());
					}
				}
			}
			else
				DEBUGL3("Get Roles Present in SSDK DB Failed\n");
		}
		catch(...)
		{
			DEBUGL3("Caught exception\n");
		}
		/*
		//Built-In Role IDs
		m_builtinRoles.insert("103");
		m_builtinRoles.insert("102");
		m_builtinRoles.insert("105");
		m_builtinRoles.insert("106");
		m_builtinRoles.insert("107");
		m_builtinRoles.insert("108");
		m_builtinRoles.insert("109");
		m_builtinRoles.insert("110");
		m_builtinRoles.insert("111");
		m_builtinRoles.insert("112");
		m_builtinRoles.insert("113");
		m_builtinRoles.insert("114");
		m_builtinRoles.insert("116");
		m_builtinRoles.insert("115");
		m_builtinRoles.insert("117");
		m_builtinRoles.insert("118");
		m_builtinRoles.insert("119");
		m_builtinRoles.insert("120");
		m_builtinRoles.insert("121");
		//m_builtinRolesOrg.insert("GuestUser");
		//Built-in Role Names
                m_builtinRolesOrg.insert("administrator");
		m_builtinRolesOrg.insert("servicetech");
                //m_builtinRolesOrg.insert("FaxReceiveOperator");
                m_builtinRolesOrg.insert("accountmanager");
                m_builtinRolesOrg.insert("copyoperator");
                m_builtinRolesOrg.insert("scanoperator");
                m_builtinRolesOrg.insert("print");
                m_builtinRolesOrg.insert("printoperator");
                m_builtinRolesOrg.insert("efilingoperator");
                m_builtinRolesOrg.insert("colorprintcopyoperator");
                m_builtinRolesOrg.insert("faxoperator");
                m_builtinRolesOrg.insert("auditor");
                m_builtinRolesOrg.insert("guest");
 		m_builtinRolesOrg.insert("fax");
                m_builtinRolesOrg.insert("user");
		m_builtinRolesOrg.insert("anonymous");
                m_builtinRolesOrg.insert("autoprocessing");
                m_builtinRolesOrg.insert("snmpwriteoperator");
                m_builtinRolesOrg.insert("snmpreadoperator");
		m_builtinRolesOrg.insert("nopermission");
		*/
                firstTime = false;
	}
	if(searchByName)
		return m_builtinRolesOrg;
	else
		return m_builtinRoles;
}

bool Utils::IsBuiltinRole(const CString& role,bool searchByName)
{
	/*set<CString> *roleSet = & BuiltinRoles();
        if(roleSet->end() != roleSet->find(role))
	        return true;
        else
        	return false;*/
	if(searchByName)
	{
		//Search Built-In roles based on Role Name
		CString tempName = role;
		std::transform(tempName.begin(),tempName.end(),tempName.begin(), (int(*)(int)) tolower);
		set<CString> *roleSet = & BuiltinRoles();
	        if(roleSet->end() != roleSet->find(tempName))
        	        return true;
	        else
        	        return false;
	}
	else
	{
		//Search Built-In roles based on Role ID
		set<CString> *roleSet = & BuiltinRoles(false);
	        if(roleSet->end() != roleSet->find(role))
        	        return true;
	        else
        	        return false;
	}
}	

bool Utils::SetValueToSRAM(const uint64 & key, const CString & value)
{
        bool ret = false;
        Status retStatus = STATUS_FAILED;
        try{
                ci::operatingenvironment::Ref<IndexedDB> indexDB=IndexedDB::Acquire();
                unsigned int length = value.length();
                char *data=NULL;
                DEBUGL8("Value to be set %s, length %d\n",value.c_str() , length);
                if(length ==0)
		{
                	data=new char[1]; 
		       *data='\0';
		}
                else
                        data = const_cast<char*>(value.c_str());
                retStatus=indexDB->SetValue(key,static_cast<void*>(data),static_cast<int>(length));
                DEBUGL8("Set Value into SRAM. Status: %d\n", retStatus);
                if(STATUS_OK==retStatus)
                        ret = true;
                else
                        ret = false;
        }catch(...)
        {
                DEBUGL2("Caught Unknown Exceptions!!\n");
                ret = false;
        }
        return ret;
}

bool Utils::GetValueFromSRAM(const uint64& key,CString& value)
{ 
	bool ret = false;
	Status retStatus=STATUS_OK;
	int length=0;
	void *p;
	try{
		ci::operatingenvironment::Ref<IndexedDB> indexDB=IndexedDB::Acquire();
		retStatus=indexDB->GetValue(key,length,p);
		value=reinterpret_cast<char*>(p);
		DEBUGL8("Value to be set %s, length %d\n", value.c_str(), value.length());
		DEBUGL8("Get Value from SRAM. Status: %d\n", retStatus);
		if(STATUS_OK==retStatus)
			ret = true;
		else
			ret = false;
        }catch(...)
	{
		DEBUGL2("Caught Unknown Exceptions!!\n");
                ret = false;
	}
	return ret;
}

bool Utils::SetInt32ValueToSRAM(const uint64 & key, const CString & value)
{
        bool ret = false;
        Status retStatus = STATUS_FAILED;
	istringstream temp;
	temp.str(value);
	int32 valueAsInt =0;
	temp>>valueAsInt;
	DEBUGL8("\n Value to set to SRAM:%d\n",valueAsInt);
        try{
                ci::operatingenvironment::Ref<IndexedDB> indexDB=IndexedDB::Acquire();
                DEBUGL8("Value to be set %d",valueAsInt);
                retStatus=indexDB->SetIntValue(key,valueAsInt);
                DEBUGL8("Set Value into SRAM. Status: %d\n", retStatus);
                if(STATUS_OK==retStatus)
                        ret = true;
                else
                        ret = false;
        }catch(...)
        {
                DEBUGL2("Caught Unknown Exceptions!!\n");
                ret = false;
        }
        return ret;
}
bool Utils::GetInt32ValueFromSRAM(const uint64 & key, CString & value)
{
        bool ret = true;
        int32 keyVal=0;
        try{
                ci::operatingenvironment::Ref<IndexedDB> indexDB=IndexedDB::Acquire();
                if(indexDB->KeyExists(key))
                {
                        DEBUGL8("\nKEY IS EXISTS\n");
                        keyVal = indexDB->GetIntValue(key);
			ostringstream temp;
			temp<<keyVal;
			value = temp.str();
			DEBUGL8("\n Value from SRAM:%d\n",keyVal);
                }
                else
                {
                        DEBUGL2("KEY DOES NOT EXISTS\n");
                        ret = false;
                }
        }catch(...)
        {
                DEBUGL2("Caught Unknown Exceptions!!\n");
                ret = false;
        }
        return ret;
}

Status Utils::SetValueToPath(BOProxyRef boProxy,const CString& path,const CString& pathValue)
{
	CString tempPathValue = pathValue;
	EbxSSDKUtil::CheckXMLEntityAndReplaceString(tempPathValue);
	CString setPath = "<SetValue><Payload><path>"+path+"</path><value>"+tempPathValue+"</value></Payload></SetValue>";
	if(boProxy->SetValue(setPath.c_str()) !=STATUS_OK)
	{
		DEBUGL1("Not able to set value on boproxy server %s\n", setPath.c_str());
		return STATUS_AL_SEC_INTERNAL_ERROR;
	}
	return STATUS_OK;
}

Status Utils::CheckForCorruption(SSDKStatus ssdkStatus)
{
	if (ssdkStatus == AUTH_RBAC_DB_CORRUPTED)
	{
	
		int iPort;
		//Creating MessagePort Parameters
		ci::messagingsystem::MsgPortRef g_InterfacePortRef;   

		CString sMessageBuff;
		MsgId iMsgId;
		Status status;

		/**
		 * Setting up message ports to communicate with SRM
		 */

		//Create Port to communicate with SSM, to get details of SRM
		ci::messagingsystem::MsgPortRef port = NULL;
		MsgPortId id = NULL;
		//MsgPortId id = MSGPORT_CI_SSM;

		Status ret = ci::messagingsystem::MsgPort::Create(port, id);
		if(ret!=STATUS_OK)
		{
			DEBUGL1("Failed to Create SSM Interface port\n");
			return ret; 
		}

		//Acquiring SSM port 
		CString name = SERVICE_AL_ROLEMANAGER_APPLICATION;
		Ref<ci::servicestartupmanager::Client> myClient;
		myClient = ci::servicestartupmanager::Client::Acquire(port, name.c_str());
		//Check For NULL
		if(!myClient)
		{
			DEBUGL2("Falied to acquire ssm client\n");
			return STATUS_FAILED;
		}
		//Get Service state of SystemResourceManager Process from SSM - "cisystemresourcemanager"
		ci::servicestartupmanager::SSMContracts::stServiceStateBus ssBus = myClient->GetServiceState(SRM_SERVICE_NAME);
		if(0 == strncmp(ssBus.sServiceName,"",1))
		{
			DEBUGL1("GetServiceState Failed\n");
			return STATUS_FAILED;
		}
		DEBUGL8("GetServiceState Passed\n");
		//Get the message port of SRM
		iPort = ssBus.portID;	//8193;   

		/**
		 * Sending a subscription message to SRM client
		 */

		//Setting the message id for Subscribe request
		iMsgId = ci::systemresourcemanager::SystemResourceManager::NotifyError;

		sMessageBuff = "<NotifyError>\
				<ErrorCode>F121</ErrorCode>\
				</NotifyError>";


		//Define the message parameters
		Msg msg(MSG_TYPE_REQUEST, sMessageBuff.c_str(), sMessageBuff.size());
		msg.SetId(iMsgId);
		msg.SetTarget(iPort);
		if(!port)
		{
			DEBUGL2("Failed to Acquire the port\n");
			return STATUS_FAILED;
		}
		//Send the message to the SRM port and Wait until the Reply is recieved
		status = port->Request(msg);
		if(status != STATUS_OK)
		{
			DEBUGL1("Failed to send subscription request to SRM \n");
			return status;
		}
		DEBUGL8("SendRequestToSRM: Successfully sent  \[%s]\n to SRM at port %d \n", sMessageBuff.c_str(),iPort);
	}

	return STATUS_OK;
}

Status Utils::WriteLog(const CString& errorCode,bool info,ci::logmanager::LogInterface::MsgOperationType oprType,ci::logmanager::LogInterface::MsgOperationTarget oprTarget,ci::logmanager::LogInterface::MessageType msgType,ci::logmanager::LogInterface::OperationApplicationType oprApplication, const CString& userName, const CString& operatedName,const CString& ipAddress,const CString& userDomainName)
{
	ssdk::SSDKStatus statusCode = OK;
	Ref<LogInterface> LogExternalPtr = LogInterface::getLogInterface(statusCode);
	if(!LogExternalPtr || (statusCode!=OK)) {
		DEBUGL2("\nFAILED:Creation of LOG Object is Failed    %s(%d) ",__FILE__,__LINE__);
		return STATUS_OK;
	}
	ci::logmanager::LogInterface::MsgLogRecord msgStruct;
	if(info)
		msgStruct.logLevel = ci::logmanager::LogInterface::llInfo;
	else
		msgStruct.logLevel = ci::logmanager::LogInterface::llError;
	msgStruct.ErrorCode = errorCode;
	//Type of event
                msgStruct.operationType = (oprType);
        //Message type
                msgStruct.messageType = (msgType);
        //operation targer
                msgStruct.operationTarget = (oprTarget);

	msgStruct.OperationApplication = oprApplication;
	msgStruct.userName = userName;
	msgStruct.modifiedUser = operatedName;
	msgStruct.DomainName = userDomainName;
	if(!ipAddress.empty())
	{
	        strncpy(msgStruct.ipAddr,ipAddress.c_str(),sizeof(msgStruct.ipAddr));
                msgStruct.ipAddr[sizeof(msgStruct.ipAddr) - 1] = '\0';
        }
        else
                msgStruct.ipAddr[0] = (char)NULL;
	/*
	if(status == true)
	{
		msgStruct.logLevel = ci::logmanager::LogInterface::llInfo;
		if(import)
			msgStruct.ErrorCode = "712A";
		else
			msgStruct.ErrorCode = "714A";
	}
	else
	{
		msgStruct.logLevel = ci::logmanager::LogInterface::llError;
		if(import)
                        msgStruct.ErrorCode = "712B";
                else
                        msgStruct.ErrorCode = "714B";
	}*/
	DEBUGL8("WriteMsg Log with error code:%s\n",msgStruct.ErrorCode.c_str());
	// Get the AutoProcessing user token
	Ref<SSDKSecurityManagerInterface> ssdkSecurityManagerInterface =ssdk::GetSecurityManager();

	Ref<SSDKUserTokenInterface> usertokenInterface = dynamic_cast<SSDKUserTokenInterface*> (ssdkSecurityManagerInterface->GetInterface(SSDKSecurityManagerInterface::USER_TOKEN_INTERFACE,statusCode));
	if((statusCode !=OK) || (usertokenInterface == (void*)NULL))
	{
		DEBUGL1("\n ssdk SecurityManager::getAuthInterface Failed \n");
		return STATUS_AL_SEC_INTERNAL_ERROR;
	}
	MyUserToken tmput;
	statusCode=usertokenInterface->Authenticate(SSDKUserTokenInterface::TOKEN_CONTEXT_AUTOPROCESSING,tmput.ut);
	if(statusCode != OK)
	{
		DEBUGL1("\n ssdk SystemUser Unable to logged in \n");
		//throw CUserAuthException("STATUS_FAILED");
		//	ret = SSDK_RBAC_MANAGER_PLUG_IN_ERROR_MAP(statusCode);
		return STATUS_AL_SEC_INTERNAL_ERROR;
	}	
	if(tmput.ut != NULL)
	{
		if(LogExternalPtr != (void *)NULL)
		{
			statusCode=LogExternalPtr->WriteMsgLogEntry(tmput.ut,msgStruct);
			DEBUGL8("STATUSCODE for WriteLogEntry%d\n", statusCode);
		}
	}
	// Delete the tmput.ut when it goes out of scope
	Ref<SSDKUserTokenInterface> autoProcessingTokenRef = tmput.ut;
	return STATUS_OK;	
}

bool Utils::CheckForNameValidity(const CString& name)
{
        set<char> nameChars(name.begin(),name.end());
        string notAllowedString("\"'()*+,/:;<=>?[\\]`{|}~");
        set<char> notAllowedChars(notAllowedString.begin(),notAllowedString.end());
        set<char> intersection;
        set_intersection(nameChars.begin(),nameChars.end(),notAllowedChars.begin(),notAllowedChars.end(),std::inserter(intersection,intersection.begin()));
        if(intersection.size() > 0)
                return false;
        else
                return true;
}
bool Utils::SortAscending(SSDKRBACMgmtInterface::RoleInfo const &a, SSDKRBACMgmtInterface::RoleInfo const &b )
{
	istringstream str(a.m_RoleUniqueID);
	int i = -1;
	str >> i;
	istringstream str2(b.m_RoleUniqueID);
	int j = -1;
	str2 >> j;
	return i < j ;
}
bool Utils::SortDescending(SSDKRBACMgmtInterface::RoleInfo const &a, SSDKRBACMgmtInterface::RoleInfo const &b )
{
        istringstream str(a.m_RoleUniqueID);
        int i = -1;
        str >> i;
        istringstream str2(b.m_RoleUniqueID);
        int j = -1;
        str2 >> j;
        return i > j ;
}
Status Utils::GetValuesFromRbacSettingsDOM(map<CString,CString> & inputMap)
{
	DEBUGL8("In GetValuesFromRbacSettingsDOM\n");	
	ci::operatingenvironment::Ref<dom::Element> nodeRef = NULL;
	DocumentRef pDeltaDoc = NULL;
	if(STATUS_OK != CRoleManager::m_pHDB->OpenDocument(RBACSETTINGS_DOM_PATH,RBACSETTINGS_DOM_NAME,pDeltaDoc))
	{
		DEBUGL1("HDB Open Document failed\n");
		return STATUS_FAILED;
	}
	if(STATUS_OK != CRoleManager::m_pHDB->BeginTransaction(pDeltaDoc,eREAD,true))
	{
		DEBUGL1("BeginTransaction of RbacSettings dom failed\n");
		return STATUS_FAILED;
	}
	for(map<CString,CString>::iterator itr=inputMap.begin();itr != inputMap.end();itr++)
	{
		nodeRef = CRoleManager::m_pHDB->BindToElement(pDeltaDoc,itr->first);
			if(!nodeRef)
			{
				DEBUGL1("\n Failed to bind to element in xpath: %s \n",(itr->first).c_str());
				if(STATUS_OK != CRoleManager::m_pHDB->EndTransaction(pDeltaDoc,eREAD))
				{
					DEBUGL1("EndTransaction of RbacSettings dom failed\n");
					return STATUS_FAILED;
				}
				return STATUS_FAILED;
			}
		itr->second = nodeRef->getTextContent();
		DEBUGL8("Value retrieved from RbacSettings dom: %s",(itr->second).c_str());
	}
	if(STATUS_OK != CRoleManager::m_pHDB->EndTransaction(pDeltaDoc,eREAD))
	{
		DEBUGL1("EndTransaction for RbacSettings dom failed\n");
		return STATUS_FAILED;
	}
	return STATUS_OK;
}

Status Utils::SetValuesToRbacSettingsDOM(BOProxyRef boProxy,map<CString,CString> & inputMap)
{
	DEBUGL8("In SetValuesToRbacSettingsDOM\n");	
	ci::operatingenvironment::Ref<dom::Element> nodeRef = NULL;
	DocumentRef pDeltaDoc = NULL;
	if(STATUS_OK != CRoleManager::m_pHDB->OpenDocument(RBACSETTINGS_DOM_PATH,RBACSETTINGS_DOM_NAME,pDeltaDoc))
	{
		DEBUGL1("HDB Open Document failed\n");
		return STATUS_FAILED;
	}
	if(STATUS_OK != CRoleManager::m_pHDB->BeginTransaction(pDeltaDoc,eWRITE,true))
	{
		DEBUGL1("BeginTransaction for RbacSettings dom failed\n");
		return STATUS_FAILED;
	}
	for(map<CString,CString>::iterator itr=inputMap.begin();itr != inputMap.end();itr++)
	{
		nodeRef = CRoleManager::m_pHDB->BindToElement(pDeltaDoc,itr->first);
			if(!nodeRef)
			{
				DEBUGL1("\n Failed to bind to element in xpath: %s \n",(itr->first).c_str());
				if(STATUS_OK != CRoleManager::m_pHDB->EndTransaction(pDeltaDoc,eWRITE))
				{
					DEBUGL1("EndTransaction of RbacSettings dom failed\n");
					return STATUS_FAILED;
				}
				return STATUS_FAILED;
			}
		nodeRef->setTextContent(itr->second);
	}
	if(STATUS_OK != CRoleManager::m_pHDB->EndTransaction(pDeltaDoc,eWRITE))
	{
		DEBUGL1("EndTransaction for RbacSettings dom failed\n");
		return STATUS_FAILED;
	}
	return STATUS_OK;
}

Status EncodeDecode::Encode(vector<string> inputVector, CString &encodedString)
{
	int size;
	string finalName="";
	string numSize="";
	if (inputVector.empty())
	{
		DEBUGL1("Input Vector is empty \n");
		return STATUS_FAILED;
	}

	for (std::vector<string>::iterator it =inputVector.begin();it!=inputVector.end();it++)
	{
		
		numSize="";
		size=it->size();
		if (size >999)
		{
		DEBUGL1("Size of ssdkrule is more than thousand characters \n");
                return STATUS_FAILED;
		}
		ostringstream o;
		o<<setfill('0')<<setw(3)<<size;
		numSize.append(o.str());
		string name1=numSize+((*it));
		finalName+=name1;

	}

	DEBUGL8("Encoded String is : %s \n",finalName.c_str());
	encodedString = finalName;
	return STATUS_OK;
}

bool Utils::IsMaxLengthExceeded(const CString& name, int maxlength)
{
        std::wstring output;
        //Added for UTF-8 Chracter support
        EbxSSDKUtil::LengthOfUTFString(reinterpret_cast<const unsigned char*> (name.c_str()),reinterpret_cast<const unsigned char*>(name.c_str()) + name.length(), std::back_inserter(output));
        int roleNameLength=output.length();
        DEBUGL8("Length of the RoleName = %d \n",roleNameLength);
        if( roleNameLength  > maxlength)
                return true;
	else
		return false;
}


//Added for EBX_STFR_18235 fix. called to check if ssdkrules are encoded
Status EncodeDecode::Decode(const string in,vector<string> &outvector)
{
	int index =0,fixedsize =3,tempsize;
	string temp,strname;
	int  strsize = in.size();
	if(!strsize)
	{
		DEBUGL1("Input string is empty\n");
		return STATUS_FAILED;
	}
	while(1)
	{
		temp = in.substr(index,fixedsize);
		tempsize = atoi(temp.c_str());
		if(!tempsize)
		{
			DEBUGL1("tempsize is empty\n");
			return STATUS_FAILED;
		}
		strname = in.substr(index+fixedsize,tempsize);
		if(strname.size() != tempsize)
		{
			DEBUGL1("substr() has failed\n");
			return STATUS_FAILED;
		}
		outvector.push_back(strname);
		index +=fixedsize+tempsize;
		if(index>=strsize)
			break;
	}
	return STATUS_OK;
}

}
}
}
// namespaces
