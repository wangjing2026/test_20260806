/* parasoft suppress item PB-33 */
/* parasoft suppress item METRICS-01_duplicated_2 */
/**************************************************************************************************************************
 *  Copyright 2008 TOSHIBA TEC CORPORATION All rights reserved
 * $Workfile: ccommand.cpp$
 * $Revision: 1 $
 * $Date: 06/15/2007$
 * Author: Manoj Saha/Ghaayathri
 *
 * Description:
 *****************************************************************************************************************************/

#include "CI/OperatingEnvironment/cexception.h"
#include "CI/OperatingEnvironment/interface.h"
#include "CI/OperatingEnvironment/cstring.h"
#include "CI/HierarchicalDB/DOM/node.h"
#include "CI/HierarchicalDB/DOM/document.h"
#include "CI/HierarchicalDB/hierarchicaldb.h"
#include "ccommand.h"
#include "CI/SI/ssdksecuritymanagerinterface.h"
#include "utils.h"
#include "croleexception.h"
#include "crolemanager.h"
#include "CI/SI/ebxssdkutil.h"
#include "AL/status.h"

using namespace dom;
using namespace ci::hierarchicaldb;
using namespace ci::operatingenvironment;

// namespaces
namespace al
{
	namespace softwarefunction
	{
		namespace rbacmanager 
		{
			
			CCommand::CCommand(dom::DocumentRef & pDocument,ci::hierarchicaldb::HierarchicalDBRef pHDB)
			{
				DEBUGL8("CComand::Constructor\n");
				m_pCommandTempDocument = pDocument;
				m_pCommandNode = pDocument->getDocumentElement()->getFirstChild();	
				m_pCommandXPathNode = NULL;
				CString nodeName = "";
				ci::operatingenvironment::Ref<NodeList> childNode = m_pCommandNode->getChildNodes();	
				for(unsigned int i=0; i<childNode->getLength();i++)
				{
					ci::operatingenvironment::Ref<Element> node=childNode->item(i);
					nodeName = node->getNodeName();
					if(nodeName == "sessionID")
					{
					DEBUGL8("CCommand::childNode = %s\n",nodeName.c_str());
						m_sessionID = node->getTextContent();
						DEBUGL8("CCommand::CCommand:SessionID is  = %s\n", m_sessionID.c_str());
					}
				}
				
				m_pHDB = pHDB;
			}
			const char * CCommand::GetCommandName()
			{
				DEBUGL8("CComand::GetCommand Name\n");
				if (m_pCommandNode)
				{
					return m_pCommandNode->getNodeName().c_str();			
				}
				else
					return NULL;
			}//end of GetCommandName()
			
			dom::NodeRef CCommand::GetCommandRoot()
			{
				DEBUGL8("In GetCommandRoot\n");
				return m_pCommandNode;
			}
			
			dom::NodeRef CCommand::GetCommandNode()
			{
				if (!m_pCommandXPathNode)
				{
					m_pCommandXPathNode = m_pHDB->BindToElement(m_pCommandNode,"commandNode");
				}
				return m_pCommandXPathNode;
			}
			
			NodeRef CCommand::GetParamNode(const char *pParamName)
			{
				CString sXPath = CString("Params/") + pParamName;
				DEBUGL8("SXPATH=%s,NAME=%s\n",sXPath.c_str(),m_pCommandNode->getNodeName().c_str());
				return m_pHDB->BindToElement(m_pCommandNode,sXPath);
			}
	
CCommandHandler::CCommandHandler(ci::operatingenvironment::Ref<CCommand> cmd) : m_command(cmd)
{
    CString cmdSessionId = cmd->GetSessionID();
    m_boProxy = BOProxy::Acquire(cmdSessionId);
    if(!m_boProxy)
    {
        DEBUGL1("Could not acquire BOProxy session %s\n", cmdSessionId.c_str());
        throw CRoleException("STATUS_BAD_REQUEST");
    }
}
#if 0
CString CRoleCommandHandler::GetValueFromPath(BOProxyRef boProxy, CString path, CString fieldName, CString statusXpath, CString cmdStatus, ERROR_STATUS errorCode,bool isReqd)
{
        NodeRef domNode = NULL;
        if(boProxy->GetValue(path.c_str(),domNode) == STATUS_FAILED)
        {
                DEBUGL1("Unable to get value of %s from bo server\n",fieldName.c_str());
                throw CRoleException("STATUS_FAILED");
        }

        CString value = domNode?domNode->getTextContent():"";
        domNode = NULL;
        if(value.empty() && isReqd && !statusXpath.empty() && !cmdStatus.empty())
        {
                DEBUGL1("%s is Empty\n",fieldName.c_str());
                throw CRoleException("STATUS_FAILED");
        }
        return value;

}
#endif
void CRoleCommandHandler::ExtractToken(CString& allToken,const char* del,vector<CString>& roleVector)
{
  char *myAllToken = new char[allToken.size() +1];
  if(myAllToken ==NULL)
  {
        DEBUGL1("memory allocation failed. Pointre is null");
        throw CRoleException("STATUS_BAD_REQUEST");
  }
memset(myAllToken,'\0',allToken.size() +1);
  strncpy(myAllToken,allToken.c_str(),allToken.size());
  char *singleToken = NULL;
  singleToken = strtok(myAllToken,del);
  if(singleToken != NULL)
        roleVector.push_back(CString(singleToken));
  while(singleToken != NULL)
  {
    singleToken = strtok(NULL,del);
    if(singleToken != NULL)
        roleVector.push_back(CString(singleToken));
  }
  if(myAllToken != NULL)
      delete [] myAllToken;
}

#if 0
Status CRoleCommandHandler::GetSystemUserToken(MyUserToken &myut,BOProxyRef &boProxy)
{
        boProxy = m_boProxy;
        if(!boProxy)
        {
          DEBUGL1("Unable to Acquire bo proxy server\n");
          return STATUS_FAILED;
        }
        CString Usertoken = GetValueFromPath(boProxy,XPATH_USERTOKEN_ID,"UserToken","",PLUGIN_STATUS_FAILED,ROLE_PLUGIN_USR_TOKEN_NOT_FOUND);
        if(Usertoken.empty())
        {
                DEBUGL1("\nUserToken is not found from boserver\n");
                return STATUS_FAILED;
        }
        if(myut.ut==NULL)
        {
            DEBUGL1("Invalid UserToken return from ssdk UserTokenInterface::DeSerialize()\n");
            return STATUS_FAILED;
        }

        return STATUS_OK;
}
#endif
Status CRoleCommandHandler::GetAllObjectOperationSet(MyUserToken &myut,const Ref<SSDKRBACMgmtInterface> & rbac,const CString &objectName,const CString &sObjectType,set< pair <CString,CString> > & objectPermSet)
{
	SSDKRBACMgmtInterface::UserInfo SSDKUserInfo;
	SSDKRBACMgmtInterface::RoleInfo SSDKRoleInfo;
	SSDKRBACMgmtInterface::GroupInfo SSDKGroupInfo;
	ssdk::SSDKStatus ssdkStatus;
	Status ret = STATUS_OK;
	if(objectName.empty() || sObjectType.empty())
	{
		DEBUGL1("Either objectname or Object Type is missing\n");
		return STATUS_AL_SEC_INVALID_INPUT;
	}
	if(sObjectType =="USER")
	{
		SSDKUserInfo=rbac->GetProperty(myut.ut,SSDKRBACMgmtInterface::USER_AUTHORIZED_ROLES_PROPERTY,objectName,ssdkStatus);
		DEBUGL8("GetProperty(USER_AUTHORIZED_ROLES_PROPERTY) Status :: %d\n",ssdkStatus);
		if(ssdkStatus != OK)
		{
			ret = SSDK_RBAC_MANAGER_PLUGIN_ERROR_MAP(ssdkStatus);
		}
		unsigned int len = SSDKUserInfo.m_UserAuthorizedRoles.size();
		for(unsigned _cnt=0;_cnt<len;++_cnt)
		{
			//For each role get permission set 
			SSDKRoleInfo = rbac->GetProperty(myut.ut,SSDKRBACMgmtInterface::ROLE_PERMISSIONS_PROPERTY,SSDKUserInfo.m_UserAuthorizedRoles[_cnt],ssdkStatus);//DBCORRCHK
			DEBUGL8("GetProperty(ROLE_PERMISSIONS_PROPERTY) Status :: %d\n",ssdkStatus);
			if(ssdkStatus == AUTH_RBAC_DB_CORRUPTED)
			{
				DEBUGL2("SSDK RBAC DB CORRUPTED\n");
				Utils::CheckForCorruption(ssdkStatus);
				return STATUS_AL_SEC_DB_CORRUPTED; 
			}	
			if(SSDKRoleInfo.m_RolePermissions.size()>0)
			{
				objectPermSet.insert(SSDKRoleInfo.m_RolePermissions.begin(),SSDKRoleInfo.m_RolePermissions.end());	
			}
			SSDKRoleInfo.m_RolePermissions.clear();
		}
	}
	else if(sObjectType =="ROLE")
	{
		SSDKRoleInfo = rbac->GetProperty(myut.ut,SSDKRBACMgmtInterface::ROLE_PERMISSIONS_PROPERTY,objectName,ssdkStatus);
		DEBUGL8("GetProperty(ROLE_PERMISSIONS_PROPERTY) Status :: %d\n",ssdkStatus);
		if(ssdkStatus != OK)
                {
                        ret = SSDK_RBAC_MANAGER_PLUGIN_ERROR_MAP(ssdkStatus);
                }
		if(SSDKRoleInfo.m_RolePermissions.size()>0)
		{
			objectPermSet.insert(SSDKRoleInfo.m_RolePermissions.begin(),SSDKRoleInfo.m_RolePermissions.end());
		}
	}
	else if(sObjectType =="GROUP")
	{
		SSDKGroupInfo = rbac->GetProperty(myut.ut,SSDKRBACMgmtInterface::GROUP_AUTHORIZED_ROLES_PROPERTY,objectName,ssdkStatus);
		DEBUGL8("GetProperty(GROUP_AUTHORIZED_ROLES_PROPERTY) Status :: %d\n",ssdkStatus);
		if(ssdkStatus != OK)
                {
                        ret = SSDK_RBAC_MANAGER_PLUGIN_ERROR_MAP(ssdkStatus);
                }
		unsigned int len = SSDKGroupInfo.m_GroupAuthorizedRoles.size();
		for(unsigned _cnt=0;_cnt<len;++_cnt)
		{
			//For each role get permission set
			SSDKRoleInfo = rbac->GetProperty(myut.ut,SSDKRBACMgmtInterface::ROLE_PERMISSIONS_PROPERTY,SSDKGroupInfo.m_GroupAuthorizedRoles[_cnt],ssdkStatus);//DBCORRCHK
			DEBUGL4("GetProperty(ROLE_PERMISSIONS_PROPERTY) Status :: %d\n",ssdkStatus);
			if(ssdkStatus == AUTH_RBAC_DB_CORRUPTED)
                        {
                                DEBUGL2("SSDK RBAC DB CRRUPTED\n");
				Utils::CheckForCorruption(ssdkStatus);
                                return STATUS_AL_SEC_DB_CORRUPTED;
                        }
			if(SSDKRoleInfo.m_RolePermissions.size()>0)
			{
				objectPermSet.insert(SSDKRoleInfo.m_RolePermissions.begin(),SSDKRoleInfo.m_RolePermissions.end());
			}
			SSDKRoleInfo.m_RolePermissions.clear();
		}
	}
	else
	{
		DEBUGL1("Unknown Object Type\n");
		return STATUS_AL_SEC_COMMAND_SCHEMA;
	}
	return ret;
}
Status CRoleCommandHandler::ReadPermissions(NodeRef pNewRoleData, CString sDataXPath,map<CString,bool> & permissionEnableDisableMap , bool checkAttribute, bool checkPermNodeReqd)
{
        Status ret=STATUS_OK;
        ci::operatingenvironment::Ref<dom::Element> pNewRoleEle=NULL;
        CString xPath = "Information/Permissions";
        CString tagName ="permission";
        ci::operatingenvironment::Ref<ci::hierarchicaldb::NodeList> permNodeList;
        ci::operatingenvironment::Ref<ci::hierarchicaldb::NodeList> elems;
        Ref<dom::Element> xPathNode = CRoleManager::m_pHDB->BindToElement(pNewRoleData,xPath);
        CString attribVal="";
        CString permName ="";
        if(xPathNode)
        {
                permNodeList=CRoleManager::m_pHDB->FindNodes(xPathNode,tagName,false);
                unsigned int permListLen = permNodeList?permNodeList->getLength():0;;
                if(permListLen>0)
                {
			CategoryDisplayNameMap::iterator iterDisObj;
                        for(unsigned int _cnt=0;_cnt<permListLen;_cnt++)
                        {
                                Ref<Element> node=permNodeList->item(_cnt);
                                if(!node) continue;
                                permName= Utils::getNodeText(node);
				iterDisObj = CRoleManager::displayObjectMap.find(permName);
				if(iterDisObj != CRoleManager::displayObjectMap.end())
				{
					if(!permName.empty())
					{
						DEBUGL8("Permission is = %s\n", permName.c_str());
						bool isCheckAttrReq = true;
						if(checkAttribute)
						{
							attribVal =node->getAttribute("isEnable");
							DEBUGL8("Attribute isEnable = %s\n",attribVal.c_str());
							if(attribVal != "")
							{
								bool bOut;
						                if(!EbxSSDKUtil::validateInput<bool>(attribVal,bOut))
        							{
					                       		DEBUGL1("Invalid Input to the bool Variable which decides Whether to Enable or Disable a Permission \n");
					                        	return STATUS_AL_SEC_INVALID_INPUT;
                						}
							}
							if(attribVal.empty()||((attribVal != "true")&&(attribVal != "false")))
								continue;
							isCheckAttrReq = (attribVal =="true")?true:false;
							DEBUGL8("Permission [%s] = [%d]\n",permName.c_str(),isCheckAttrReq);
						}
						permissionEnableDisableMap.insert(make_pair(permName,isCheckAttrReq));
					}
				}
				else
        			{
                			DEBUGL1("Unknown permission value ::%s\n",permName.c_str());
					node = NULL;
                			return STATUS_AL_SEC_INVALID_PERMISSION;
        			}

                                node = NULL;
                        }
                }
		else
                {
                        DEBUGL1("Permission List is Empty!!");
                        ret = STATUS_AL_SEC_EMPTY_PERMISSION_LIST;
                }
        }
	else
	{
		DEBUGL2("Permission List is Empty!!");
		if(checkPermNodeReqd)
			ret = STATUS_AL_SEC_EMPTY_PERMISSION_LIST;
	}
        return ret;
}
Status CRoleCommandHandler::SSDK_RBAC_MANAGER_PLUGIN_ERROR_MAP(SSDKStatus ssdkError)
{
	Status plugInError = STATUS_FAILED;
	switch(ssdkError)
	{
		case OK:
			plugInError= STATUS_OK;
			break;
		case AUTH_DUPLICATE:
			plugInError=STATUS_AL_SEC_OBJECT_ALREADY_EXISTS;
			break;
		case AUTH_BAD_AUTHORIZATION:
		case AUTH_AUTHORIZATION_DENIED:
		case AUTH_TOKEN_PERMISSION_NOT_FOUND:
			plugInError=STATUS_AL_SEC_USER_UNAUTHORIZED;
			break;
		case INVALID_ARGUMENT:
			plugInError=STATUS_AL_SEC_INVALID_INPUT;
			break;
		case AUTH_NOTFOUND:
			plugInError=STATUS_AL_SEC_OBJECT_NOT_FOUND;
			break;
		case AUTH_TOKEN_NOT_VALID:
		case AUTH_ANONYMOUS_UT_NOT_SUPPORTED_IN_CL:
		case AUTH_INVALID_TOKEN_CONTEXT:
		case AUTH_SERIALIZE_PROCESS_ID_NULL:
		case AUTH_PERSISTENT_TOKEN_WITH_PROCESSID:
			plugInError=STATUS_AL_SEC_INVALID_USERTOKEN;
			break;
		case AUTH_RBAC_DB_CORRUPTED:
			Utils::CheckForCorruption(ssdkError);
			plugInError=STATUS_AL_SEC_DB_CORRUPTED;
			break;
		case AUTH_RBAC_INVALID_PERMISSIONS_SET:
			plugInError = STATUS_AL_SEC_INVALID_PERMISSION;
			break;
		default:
			plugInError = STATUS_AL_SEC_UNKNOWN_ERROR;
			break;
	}
	return plugInError;
}

bool CRoleCommandHandler::CheckPermission()
{
	MyUserToken myut;
	BOProxyRef boProxyRef = NULL;
	Ref<SSDKLogInterface> LogExternalPtr=NULL;
	Authenticate(myut,boProxyRef,LogExternalPtr);
	if(myut.ut)
	{
		ssdk::SSDKStatus ssdkStatus = myut.ut->Authorize(ssdk::OBJ_USERMANAGEMENT, ssdk::OP_EXECUTE);
                DEBUGL8("Permission Check Status = %d\n", ssdkStatus);
                if(OK != ssdkStatus)
                {
                        DEBUGL1("User doesnt have permission\n");
			return false;
                }
		else
			return true;
	}	
	else
		return false;
}

Status CRoleCommandHandler::SetRoleRule(const SSDKUserTokenInterface* userToken,const map<CString,vector<CString> >& roleNameAttrMap)
{
        //Get the user token interface
        ssdk::SSDKStatus ssdkStatus=OK;
        Status ret=STATUS_OK;
        Ref<SSDKSecurityManagerInterface> ssdkSecurityManagerInterface = ssdk::GetSecurityManager();
        Ref<SSDKUserTokenInterface> userInterface = dynamic_cast<SSDKUserTokenInterface*> (ssdkSecurityManagerInterface->GetInterface(SSDKSecurityManagerInterface::USER_TOKEN_INTERFACE, ssdkStatus));
        if(userInterface == (void *)NULL || ssdkStatus != OK)
        {
                DEBUGL1("Casting to SSDKUserTokenInterface* failed:%d\n",ssdkStatus);
                ret = SSDK_RBAC_MANAGER_PLUGIN_ERROR_MAP(ssdkStatus);
                return ret;
        }

        //Get the rbac interface
        Ref<SSDKRBACMgmtInterface> rbac = dynamic_cast<SSDKRBACMgmtInterface*> (ssdkSecurityManagerInterface->GetInterface  (SSDKSecurityManagerInterface::RBAC_MGMT_INTERFACE,ssdkStatus));
        if(rbac==(void*) NULL || ssdkStatus != OK)
        {
                DEBUGL1("\n ssdk SecurityManager::SSDKRBACMgmtInterface Failed %d\n",ssdkStatus);
                ret = SSDK_RBAC_MANAGER_PLUGIN_ERROR_MAP(ssdkStatus);
                return ret;
        }

        map<CString,vector<CString> >::const_iterator it;
        for(it=roleNameAttrMap.begin();it!=roleNameAttrMap.end();it++)
        {
                //Get the role ALL Properties
                SSDKRBACMgmtInterface::RoleInfo roleInfoStructure;
                ssdk::SSDKStatus ssdkStatus;
                roleInfoStructure =  rbac->GetProperty(userToken, SSDKRBACMgmtInterface::ROLE_RULES_PROPERTY , (*it).first , ssdkStatus);
		if(ssdkStatus == AUTH_NOTFOUND)
		{
			DEBUGL8("Role Not Present in DB\n");
			continue;
		}
                if(OK!=ssdkStatus)
                {
                        DEBUGL1("SSDK::Get Role All Properties Failed Status:%d\n",ssdkStatus);
                        ret = SSDK_RBAC_MANAGER_PLUGIN_ERROR_MAP(ssdkStatus);
                        return ret;
                }
                vector<CString>::const_iterator attributeIterator;
                (roleInfoStructure.m_RoleRules).clear();
                for(attributeIterator=(it->second).begin();attributeIterator!=(it->second).end();attributeIterator++)
                {
                        DEBUGL8("Pushing back:%s\n",(*attributeIterator).c_str());
                        (roleInfoStructure.m_RoleRules).push_back(*attributeIterator);
                }
                DEBUGL6("Role Name from structure:%s\n",(roleInfoStructure.m_RoleId).c_str());
                //Set the role info to SSDK
                ssdkStatus = rbac->SetProperty(userToken, SSDKRBACMgmtInterface::ROLE_RULES_PROPERTY , roleInfoStructure);
                if(OK!=ssdkStatus)
                {
                        DEBUGL1("SSDK::Set Role All Properties Failed Status:%d\n",ssdkStatus);
                        ret = SSDK_RBAC_MANAGER_PLUGIN_ERROR_MAP(ssdkStatus);
                        return ret;
                }
        }
        return STATUS_OK;
}

Status CRoleCommandHandler::Authenticate(MyUserToken &myut, BOProxyRef &boProxy,  Ref<SSDKLogInterface> &LogExternalPtr)
{
	Status ebxStatus;
	boProxy = m_boProxy;
	if(!boProxy)
        {
            DEBUGL1("Unable to Acquire bo proxy server\n");
	    return STATUS_AL_SEC_INTERNAL_ERROR;	
        }

	SSDKStatus ssdkStatus;
 	Ref<SSDKSecurityManagerInterface> ssdkSecurityManagerInterface =ssdk::GetSecurityManager();
       	LogExternalPtr = dynamic_cast<SSDKLogInterface*> (ssdkSecurityManagerInterface->GetInterface(SSDKSecurityManagerInterface::LOG_INTERFACE, ssdkStatus));
        if(LogExternalPtr == (void*)NULL || OK!=ssdkStatus)
        {
        	DEBUGL1("\nFAILED:Creation of LOG Object is Failed    %s(%d) ",__FILE__,__LINE__);
        }

	ebxStatus = boProxy->GetUserToken(m_userToken);
	if(STATUS_OK!=ebxStatus)
	{
		DEBUGL1("UserToken is Not Available\n");				
		return STATUS_AL_SEC_USER_TOKEN_NOT_FOUND;
	}

        if(m_userToken)
        {
         	myut.ut = m_userToken.operator->();
                if(myut.ut == NULL)
                {
                	DEBUGL1("Actual UserToken is NULL\n");
                        return STATUS_AL_SEC_USER_TOKEN_NOT_FOUND;;
                }
        }
        else
        {
        	DEBUGL1("UserToken Ref Object is NULL\n");
                return STATUS_AL_SEC_INTERNAL_ERROR;
        }
	return STATUS_OK;
}

Status CRoleCommandHandler::ProcessTransDoc(CString cmdName,CString & sUserDataXPath, ElementRef pParamNode,DocumentRef & pNewUserDoc)
{
	Status retStat = STATUS_OK;
	NodeRef pParamText = NULL;

	if(strcmp(chelper::GetAttributeValue(pParamNode,"contentType").c_str(),"XPath") == 0)
	{
		DEBUGL8("RegisterUser::Execute::Content Type is XPath\n");
                        if (!(pParamNode) || (!(pParamText = pParamNode->getFirstChild()) || ((sUserDataXPath = pParamText->getNodeValue()).size() == 0) ) )
                        {
                                DEBUGL2("In CRoleCommandHandler::ProcessTransDoc::Failed To  node in Command XML\n");
				return STATUS_FAILED;
                        }
                        DEBUGL5("The Metadata XPath is %s\n",sUserDataXPath.c_str());
                        DEBUGL5("\n DELTA DOC NAME FOR PARAM = %s\n",pParamNode->getAttribute("deltaDocName").c_str());
                        if (STATUS_OK != (retStat = CRoleManager::m_pHDB->OpenDocument(pParamNode->getAttribute("deltaDocName"),pNewUserDoc)))
                        {
                                DEBUGL2("CRoleCommandHandler::ProcessTransDoc::Failed To Open Document\n");
				if(STATUS_HDB_ERROR_PARSING_XML == retStat)
					return STATUS_AL_SEC_BAD_REQUEST;
				return STATUS_FAILED;
                        }
			/*pNewUserData = CRoleManager::m_pHDB->BindToElement(pNewUserDoc,sUserDataXPath.c_str(),true);
                        if (!pNewUserData)
                        {
                                DEBUGL2("CRoleCommandHandler::ProcessTransDoc::Failed To Bind To Information Node (%s) Passed by Client Document\n",sUserDataXPath.c_str());
				return STATUS_FAILED;
                        }*/
			
		
	}
	else
	{
		DEBUGL1("In CRoleCommandHandler::ProcessTransDoc:: ContentType is Mismatched");
		return STATUS_FAILED;
	}
	return retStat;
}
Status CRoleCommandHandler::AddTheNewNode(CString& xml,  NodeRef pNewUserDoc, ci::operatingenvironment::Ref<dom::Element>& pNewUserEle)
{
	Status ret = STATUS_OK;
	if(pNewUserDoc && pNewUserDoc->getParentNode())
        {
                CString nodeText = pNewUserDoc->getParentNode()->getNodeName();
                DEBUGL8("NODE_TEXT::%s\n",nodeText.c_str());
                if(strcmp(nodeText.c_str(), "Roles") == 0)
                {
                        if((ret =CRoleManager::m_pHDB->DeserializeFromString(pNewUserDoc->getParentNode(), xml, pNewUserEle)) == STATUS_OK)
                        {
                                pNewUserDoc->getParentNode()->removeChild(pNewUserDoc);
                                return STATUS_OK;
                        }
			else
				DEBUGL2("DeserializeFromString Status :: %d\n",ret);
                }
                else
                {
                        if((ret =CRoleManager::m_pHDB->DeserializeFromString(pNewUserDoc, xml, pNewUserEle)) == STATUS_OK)
                        {
                                return STATUS_OK;
                        }
			else
				DEBUGL2("DeserializeFromString Status :: %d\n",ret);
                }
		
        }
        return STATUS_FAILED;
}
void CRoleCommandHandler::MakeTokenizeString(std::vector<string> users,CString & allUsers)
{
	std::vector<string>::iterator iter1,iter2;
	if(users.size()>0)
	{
		stringstream tempUsers;
		iter2=users.end();
		iter2--;
		for(iter1=users.begin();iter1!=users.end();iter1++)
		{
			if(iter1==iter2)
				tempUsers<<(*iter1);
			else
				tempUsers<<(*iter1)<<DELIM;
		}
	allUsers=tempUsers.str();
	}
	else
		allUsers=STRING_NULL;
}
CString  CRoleCommandHandler::CreateXMLNode(std::vector<string> strList,CString rootNodeName ,CString childNodeName)
{
	if(rootNodeName.empty() || childNodeName.empty())
		return "";
	std::vector<string>::iterator iter1;
	CString indString="";
	CString nodeNameBegin ="<" +childNodeName+">";
	CString nodeNameClose ="</" +childNodeName+">";
	CString tempXmlNode ="<"+rootNodeName+">"; 
	for(iter1=strList.begin();iter1!=strList.end();iter1++)
	{
		indString = *iter1;
		EbxSSDKUtil::CheckXMLEntityAndReplaceString(indString);	
		tempXmlNode = tempXmlNode + nodeNameBegin + indString + nodeNameClose;
	}
	tempXmlNode+= "</"+rootNodeName+">";
	return tempXmlNode;
}

void CRoleCommandHandler::UnassignPermission(map<CString,bool> & permissionEnableDisableMap)
{
        //Assign AddressBookReadEdit and JobTemplateReadEdit
        map<CString,bool>::iterator iter;
        /*
        iter = permissionEnableDisableMap.find(OPR_ADDRESSBOOK_READ_EDIT);
        if(iter == permissionEnableDisableMap.end())
                permissionEnableDisableMap.insert(make_pair(OPR_ADDRESSBOOK_READ_EDIT,true));
        else
                iter->second = true;

        iter = permissionEnableDisableMap.find(OPR_JOBTEMPLATE_READ_EDIT);
        if(iter == permissionEnableDisableMap.end())
                permissionEnableDisableMap.insert(make_pair(OPR_JOBTEMPLATE_READ_EDIT,true));
        else
                iter->second = true;*/

        vector<CString> disablePermission;
        disablePermission.push_back(OPR_DEVICE_SETTING);
        disablePermission.push_back(OPR_CLONE);
        disablePermission.push_back(OPR_ACCESS_DIAGONOSTICMODE);
        disablePermission.push_back(OPR_USER_DEPT_MGNT);
        disablePermission.push_back(OPR_LOG_READ);
        disablePermission.push_back(OPR_LOG_EXPORT);
        disablePermission.push_back(OPR_JOB_OPERATION);
        disablePermission.push_back(OPR_DEVICE_REMOTE_MAINTENANCE);

        vector<CString>::iterator iterPerm;
        for(iterPerm = disablePermission.begin();iterPerm != disablePermission.end(); ++iterPerm)
        {
                iter = permissionEnableDisableMap.find(*iterPerm);
                if(iter != permissionEnableDisableMap.end())
                {
                        iter->second = false;
                        DEBUGL8("PERMISSION NAME :: %s MUST BE DISABLE\n",(*iterPerm).c_str());
                }
        }
        return;
}

Status DocumentGuard::ReadLock(bool block)
{
        Status ret = STATUS_FAILED;
        if(m_pDeltaDoc != (void *)NULL)
        {
                try{
                        if(m_isWriteLock)
                        {
                                DEBUGL2("Document has write lock by same object, please release write lock before read lock\n");
                                return STATUS_FAILED;
                        }
                        else if (m_isReadLock)
                        {
                                DEBUGL2("Document has already read lock by same object\n");
                                return STATUS_OK;
                        }
                        ret = CRoleManager::m_pHDB->BeginTransaction(m_pDeltaDoc,eREAD,block);
                        DEBUGL2("BeginTransaction Status(READ):: %d\n",ret);
                        if(ret == STATUS_OK)
                                m_isReadLock = true;
                } catch(DOMException & ex)
                {
                        DEBUGL1("DOMException Caught in EndTransaction !!");
                        ret = STATUS_FAILED;
                }
        }
        return ret;
}

Status DocumentGuard::WriteLock(bool block)
{
        Status ret = STATUS_FAILED;
        if(m_pDeltaDoc != (void *)NULL)
        {
                try{
                        if (m_isReadLock)
                        {
                                 DEBUGL2("Document has read lock by same object, please release read lock before write lock\n");
                                 return STATUS_FAILED;
                        }
                        else if(m_isWriteLock)
                        {
                                DEBUGL2("Document has already write lock by same object\n");
                                return STATUS_OK;
                        }
                        ret = CRoleManager::m_pHDB->BeginTransaction(m_pDeltaDoc,eWRITE,block);
                        DEBUGL2("BeginTransaction(WRITE) Status :: %d\n",ret);
                        if(ret == STATUS_OK)
                                m_isWriteLock = true;
                } catch(DOMException & ex)
                {
                        DEBUGL1("DOMException Caught in EndTransaction !!");
                        ret = STATUS_FAILED;
                }
        }
        return ret;
}

Status DocumentGuard::Release()
{
        Status ret = STATUS_OK;
        if(m_pDeltaDoc != (void *)NULL)
        {
                try{
                        if(m_isWriteLock && m_isReadLock)
                        {
                                DEBUGL1("DOCUMENT HAS BOTH THE LOCK BY SAME INSTANCE\n");
                                return STATUS_FAILED;
                        }
                        else if(!m_isWriteLock && !m_isReadLock)
                        {
                                DEBUGL1("DOCUMENT HAS NO LOCK TO RELEASE BY SAME INSTANCE\n");
                                m_pDeltaDoc = NULL;
                                return STATUS_OK;
                        }
                        Status ret = STATUS_OK;
                        if(m_isWriteLock)
                        {
                                ret = CRoleManager::m_pHDB->EndTransaction(m_pDeltaDoc,eWRITE);
                                DEBUGL2("EndTransaction(WRITE) Status:: %d\n",ret);
                        }
                        else if(m_isReadLock)
                        {
                                ret = CRoleManager::m_pHDB->EndTransaction(m_pDeltaDoc,eREAD);
                                DEBUGL2("EndTransaction(READ) Status:: %d\n",ret);
                        }
                        if(ret == STATUS_OK)
                                m_pDeltaDoc = NULL;
                } catch(DOMException & ex)
                {
                        DEBUGL1("DOMException Caught in EndTransaction !!");
                        ret = STATUS_FAILED;
                }
        }
        return ret;
}
DocumentGuard::~DocumentGuard()
{
        Release();
}

Status CRoleCommandHandler::UpdateRoleRule(const CString& roleNameToMapping, SSDKRBACMgmtInterface::RoleInfo& SSDKRoleInfo)
{

	if(File::Exists(currentRoleSetting))
	{
		RoleRuleMap roleNameAttrMapFromFile;
		RoleRuleMap::iterator roleMapIterator;
		Status ret = STATUS_FAILED;
		CString otherUser;
		ret = Utils::ParseRoleMap(currentRoleSetting,roleNameAttrMapFromFile,otherUser,roleNameToMapping);
		if(ret != STATUS_OK)
			DEBUGL1("Parsing of xml failed:%s\n",currentRoleSetting.c_str());
		if(!roleNameAttrMapFromFile.empty())
		{
			map<CString,vector<CString> >::const_iterator it;
			for(it=roleNameAttrMapFromFile.begin();it!=roleNameAttrMapFromFile.end();it++)
			{
				vector<CString>::const_iterator attributeIterator;
					for(attributeIterator=(it->second).begin();attributeIterator!=(it->second).end();attributeIterator++)
					{
						DEBUGL8("Pushing back:%s\n",(*attributeIterator).c_str());
						//SSDKRoleInfo whcih will be passed to SSDK is modified here.
						(SSDKRoleInfo.m_RoleRules).push_back(*attributeIterator);

					}
			}
		}

	}


	return STATUS_OK;
}
		} //namespace RbacManager
	} // namespace SoftwareFunction
} // namespace AL


