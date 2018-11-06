/* parasoft suppress item PB-33 */
/* parasoft suppress item METRICS-01_duplicated_2 */
/**
*@Copyright 2008 TOSHIBA TEC CORPORATION All rights reserved
 * @Workfile: ccommandhandler.cpp$
 * @Revision: 1 $
 * @Date: 01/10/2008$
 *@ Author : Ghaayathri/ Manoj Saha
 * @ Notes:
 */
#include "ccommandhandler.h"
#include "croleexception.h"
#include "crolemanager.h"
#include "crole.h"
#include <algorithm>
#include <set>
#include <CI/SI/ebxssdkutil.h>
#include <CI/OperatingEnvironment/file.h>

using namespace std;

namespace al
{
namespace softwarefunction
{
namespace rbacmanager
{

using namespace dom;
using namespace ci::operatingenvironment;

using namespace al::softwarefunction::rbacmanager;

Status GetPermissionList::ExecuteSSDK(NodeRef pNewRoleData, CString sDataXPath, CString sObjectType)
{
	MyUserToken myut;
	BOProxyRef boProxy;
	Ref<SSDKLogInterface> LogExternalPtr=NULL;
	ssdk::SSDKStatus ssdkStatus;
	Status ret=STATUS_FAILED;
	//Get the patams checkType
	ElementRef pParamNode = NULL;
	NodeRef pParamText = NULL;
	CString checkType;
	pParamNode = m_command->GetParamNode("checkDetails");
	
	if (!(pParamNode) || (!(pParamText = pParamNode->getFirstChild()) || ((checkType= pParamText->getNodeValue()).size() == 0) ) )
      {
      	DEBUGL1("In GetEnabledPermissionList::ProcessTransDoc::Failed to get param value in Command XML\n");
      	return STATUS_AL_SEC_COMMAND_SCHEMA;
      }
	bool checkEnable = true;
	if(checkType =="ENABLE")
		checkEnable = true;
	else if(checkType =="DISABLE")
		checkEnable = false;
	else
	{
		DEBUGL1("Unknown value for params \"checkDetails\"\n");
		return STATUS_AL_SEC_COMMAND_SCHEMA;
	}
	ci::operatingenvironment::Ref<dom::Element> pNewRoleEle=NULL;
	//Authenticate the user
	try{
	ret =Authenticate(myut, boProxy, LogExternalPtr);
	if(ret != STATUS_OK)
      	{
      		DEBUGL1("GetEnabledPermissionList ::Authenticate::Failed \n");
      		return ret;
      	}
      	Ref<SSDKSecurityManagerInterface> ssdkSecurityManagerInterface =ssdk::GetSecurityManager();
      	Ref<SSDKRBACMgmtInterface> rbac = dynamic_cast<SSDKRBACMgmtInterface*> (ssdkSecurityManagerInterface->GetInterface  (SSDKSecurityManagerInterface::RBAC_MGMT_INTERFACE,ssdkStatus));
      	if(rbac==(void*) NULL || ssdkStatus != OK)
      	{
               DEBUGL1("\n ssdk SecurityManager::SSDKRBACMgmtInterface Failed \n");
               return STATUS_AL_SEC_INTERNAL_ERROR;
      	}
	Role role;
	CString xPath = "Information/Permissions";
	Ref<dom::Element> _pNewRoleData = CRoleManager::m_pHDB->BindToElement(pNewRoleData,xPath);
	ElementRef pParent = NULL; 
	if(_pNewRoleData )
	{
		//remove the <Permissions> node 
		pParent = _pNewRoleData->getParentNode();
		if(pParent)
		{
			//Remove the node pointing by _pNewUserData
			if(!pParent->removeChild(_pNewRoleData))
			{
				DEBUGL1("\n Failed to remove child node <Roles>\n");
				return STATUS_FAILED;
			}
		}
		else
		{
			DEBUGL1("\n Failed to get Paranet node of <Roles>\n");
			return STATUS_AL_SEC_INTERNAL_ERROR;
		}
	}
	else
		pParent =CRoleManager::m_pHDB->BindToElement(pNewRoleData,"Information",true);
	CString objectName = role.GetName(sDataXPath,pNewRoleData);
	//Get all SSDK permission list for a given role/user/group
	set< pair <CString,CString> > objectPermSet;
	ret=GetAllObjectOperationSet(myut,rbac,objectName,sObjectType,objectPermSet);
	if(ret != STATUS_OK)
	{
		DEBUGL1("Failed to get ObjectPermission Set\n");
		return ret;
	}
	
	//Now for each DisplayObject, check for permission enable/disable
	CategoryDisplayNameMap::iterator iterDisObj;
	CString permXML ="<Permissions>";
	for(iterDisObj=CRoleManager::displayObjectMap.begin();iterDisObj!=CRoleManager::displayObjectMap.end();++iterDisObj)
	{
		DEBUGL8("Permission is = %s\n", (iterDisObj->first).c_str());
		Status status = STATUS_OK;
		if(role.CheckPermissionEnable(objectPermSet,iterDisObj->first,status))
		{
			//enable
			if(checkEnable)	
				permXML += "<permission isEnable=\"true\">" + iterDisObj->first + "</permission>";
		}
		else
		{
			//disable
			if(!checkEnable)
				permXML += "<permission isEnable=\"false\">" + iterDisObj->first + "</permission>";
		}
	}
	permXML = permXML + "</Permissions>";
	DEBUGL8("Permission XML :: %s\n",permXML.c_str());
	ElementRef pNewUserEle = NULL;
	if(AddTheNewNode(permXML, pParent , pNewUserEle) != STATUS_OK)
	{
		DEBUGL1("Failed to DeSerialized the string :: %s\n",permXML.c_str());
		return STATUS_FAILED;
	}
	}
	catch(DOMException& domExcept)
        {
                DEBUGL1("\n Caught DOMException !! \n");
                return STATUS_FAILED;
        }
	catch(...)
	{
		DEBUGL1("Caught Exception!!\n");
		return STATUS_AL_SEC_UNKNOWN_ERROR;
	}
	return STATUS_OK;
}
#if 0
Status Permission::ReadPermissions(NodeRef pNewRoleData, CString sDataXPath,map<CString,bool> & permissionEnableDisableMap , bool checkAttribute)
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
                        for(unsigned int _cnt=0;_cnt<permListLen;_cnt++)
                        {
                                Ref<Element> node=permNodeList->item(_cnt);
                                if(!node) continue;
                                permName= Utils::getNodeText(node);
                                if(!permName.empty())
                                {
                                        DEBUGL8("Permission is = %s\n", permName.c_str());
					bool isCheckAttrReq = true;
					if(checkAttribute)
					{	
						attribVal =node->getAttribute("isEnable");
						DEBUGL8("Attribute isEnable = %s\n",attribVal.c_str());
						if(attribVal.empty()||((attribVal != "true")&&(attribVal != "false")))
							continue;
						isCheckAttrReq = (attribVal =="true")?true:false;
						DEBUGL8("Permission [%s] = [%d]\n",permName.c_str(),isCheckAttrReq);
					}	
					permissionEnableDisableMap.insert(make_pair(permName,isCheckAttrReq));
                                }
                                node = NULL;
                        }
                }
                else
                {
                        DEBUGL1("Permission List is Empty!!");
                        ret = STATUS_FAILED;
                }
        }
        else
        {
                DEBUGL1("Permissions node is not present");
                ret = STATUS_FAILED;
        }
	return ret;
}
#endif
Status Permission::Execute()
{
	ElementRef pParamNode = NULL;
	ElementRef pParamNode2 = NULL;
	ElementRef pNewUserData = NULL;
      	CString sUserDataXPath = STRING_NULL;
	CString sUserDataXPath2 ="";
	NodeRef pParamText2 = NULL;
      	Status ret = STATUS_OK;
	pParamNode = m_command->GetParamNode("permissionDetails");
	pParamNode2 = m_command->GetParamNode("objectType");
	DocumentRef pNewUserDoc;
	if(pParamNode && pParamNode2)
        {
                ret = ProcessTransDoc("GetPermission",sUserDataXPath,pParamNode,pNewUserDoc);
                if (ret != STATUS_OK)
                {
                        DEBUGL1("In GetRoles::Execute:: Failed \n");
                        ret = STATUS_AL_SEC_COMMAND_SCHEMA;
                        return ret;
                }
		DocumentGuard domGuard(pNewUserDoc);
		if(STATUS_OK != domGuard.WriteLock(false))
                {
                        DEBUGL1("Failed to Acquire Write Lock on Rbac Session DOM\n");
                        return STATUS_FAILED;
                }
		pNewUserData = CRoleManager::m_pHDB->BindToElement(pNewUserDoc,sUserDataXPath.c_str(),true);
		if (!pNewUserData)
		{
			DEBUGL2("Permission::Execute::Failed To Bind To Information Node (%s) Passed by Client Document\n",sUserDataXPath.c_str());
			return STATUS_AL_SEC_COMMAND_SCHEMA;
		}
		if (!(pParamNode2) || (!(pParamText2 = pParamNode2->getFirstChild()) || ((sUserDataXPath2 = pParamText2->getNodeValue()).size() == 0) ) )
                {
                	DEBUGL2("In CRoleCommandHandler::ProcessTransDoc::Failed To  node in Command XML\n");
                	return STATUS_AL_SEC_COMMAND_SCHEMA;
                }
                ret = ExecuteSSDK(pNewUserData,sUserDataXPath,sUserDataXPath2);
        }
        else
        {
                DEBUGL1("In GetRoles::Execute:: Failed \n");
                ret = STATUS_AL_SEC_COMMAND_SCHEMA;
        }
	return ret;
}
Status SetPermission::ExecuteSSDK(NodeRef pNewRoleData, CString sDataXPath, CString sObjectType)
{
	MyUserToken myut;
	BOProxyRef boProxy;
	Ref<SSDKLogInterface> LogExternalPtr=NULL;
	ssdk::SSDKStatus ssdkStatus;
	Status ret=STATUS_FAILED;
	ci::operatingenvironment::Ref<dom::Element> pNewRoleEle=NULL;

	if(sObjectType != "ROLE")
	{
		DEBUGL1("Invalid Object Type\n");
		return STATUS_AL_SEC_COMMAND_SCHEMA;
	}
	//Authenticate the user
	ret = Authenticate(myut, boProxy, LogExternalPtr);
	if(ret != STATUS_OK)
        {
                DEBUGL1("GetPermission::Authenticate::Failed \n");
                return ret;
        }
	Ref<SSDKSecurityManagerInterface> ssdkSecurityManagerInterface =ssdk::GetSecurityManager();
	Ref<SSDKRBACMgmtInterface> rbac = dynamic_cast<SSDKRBACMgmtInterface*> (ssdkSecurityManagerInterface->GetInterface  (SSDKSecurityManagerInterface::RBAC_MGMT_INTERFACE,ssdkStatus));
        if(rbac==(void*) NULL || ssdkStatus != OK)
        {
                DEBUGL1("\n ssdk SecurityManager::SSDKRBACMgmtInterface Failed \n");
                return STATUS_AL_SEC_INTERNAL_ERROR;
        }
	Role role;
        CString objectName = "";
        objectName = role.GetName(sDataXPath,pNewRoleData);
	if(objectName.empty())
        {
                DEBUGL1("Object name is empty\n");
                return STATUS_AL_SEC_INVALID_INPUT;
        }
	//Get all SSDK permission list for a given role/user/group
	//set< pair <CString,CString> > objectPermSet;
	SSDKRBACMgmtInterface::RoleInfo SSDKRoleInfo;
	SSDKRoleInfo.m_RoleId = objectName; 
        ret = GetAllObjectOperationSet(myut,rbac,objectName,sObjectType,SSDKRoleInfo.m_RolePermissions);
	if(ret != STATUS_OK)
	{
		DEBUGL1("Invalid object type \n");
		return ret;
	}
	map<CString,bool> permissionEnableDisableMap;
	ret = ReadPermissions(pNewRoleData,sDataXPath,permissionEnableDisableMap,true);	
	if(ret != STATUS_OK)
	{
		DEBUGL1("Invalid Permission List \n");
		return ret;
	}
	map<CString,bool>::iterator iter;
	for(iter=permissionEnableDisableMap.begin();iter!=permissionEnableDisableMap.end();++iter)
	{
		ret=role.SetEnableDisablePermission(SSDKRoleInfo.m_RolePermissions,iter->first,iter->second);
		if(ret != STATUS_OK)
		{
			DEBUGL2("Failed to set permission :: %s\n",(iter->first).c_str());
			return ret;
		}
	}
		
	ssdkStatus = rbac->SetProperty(myut.ut,SSDKRBACMgmtInterface::ROLE_PERMISSIONS_PROPERTY,SSDKRoleInfo);
	if(ssdkStatus != OK)
	{
		DEBUGL1("Failed to Enable/Disable the permission(s)\n");
		ret = SSDK_RBAC_MANAGER_PLUGIN_ERROR_MAP(ssdkStatus); 
		return  ret;
	}	
	return STATUS_OK;
}
Status GetPermission::ExecuteSSDK(NodeRef pNewRoleData, CString sDataXPath, CString sObjectType)
{
	MyUserToken myut;
	BOProxyRef boProxy;
	Ref<SSDKLogInterface> LogExternalPtr=NULL;
	ssdk::SSDKStatus ssdkStatus;
	Status ret=STATUS_FAILED;
	ci::operatingenvironment::Ref<dom::Element> pNewRoleEle=NULL;
	//Authenticate the user
	ret=Authenticate(myut, boProxy, LogExternalPtr);
	if(ret != STATUS_OK)
        {
                DEBUGL2("GetPermission::Authenticate::Failed \n");
                return ret;
        }
	Ref<SSDKSecurityManagerInterface> ssdkSecurityManagerInterface =ssdk::GetSecurityManager();
        Ref<SSDKRBACMgmtInterface> rbac = dynamic_cast<SSDKRBACMgmtInterface*> (ssdkSecurityManagerInterface->GetInterface  (SSDKSecurityManagerInterface::RBAC_MGMT_INTERFACE,ssdkStatus));
        if(rbac==(void*) NULL || ssdkStatus != OK)
        {
                DEBUGL1("\n ssdk SecurityManager::SSDKRBACMgmtInterface Failed \n");
                return STATUS_AL_SEC_INTERNAL_ERROR;
        }
	Role role;
	CString objectName = "";
	objectName = role.GetName(sDataXPath,pNewRoleData);
	if(objectName.empty())
	{
		DEBUGL1("Object name is empty\n");
		return STATUS_AL_SEC_INVALID_INPUT;
	}
	//Get all SSDK permission list for a given role/user/group
	set< pair <CString,CString> > objectPermSet;
	ret = GetAllObjectOperationSet(myut,rbac,objectName,sObjectType,objectPermSet);
	if(ret != STATUS_OK)
	{
		DEBUGL1("Failed to get Object/Operation set\n");
		return ret;
	}	
	//Bind to Information/Permissions
	CString xPath = "Information/Permissions";
	CString tagName ="permission";
	CString operations= "Operations";
	Ref<dom::Element> objNameRef;
	ci::operatingenvironment::Ref<ci::hierarchicaldb::NodeList> permNodeList;
	ci::operatingenvironment::Ref<ci::hierarchicaldb::NodeList> elems;
	Ref<dom::Element> xPathNode = CRoleManager::m_pHDB->BindToElement(pNewRoleData,xPath);
	CString permName ="";
	if(xPathNode)
	{
		permNodeList=CRoleManager::m_pHDB->FindNodes(xPathNode,tagName,false);
		//elems = xPathNode->getChildNodes();
		unsigned int permListLen = permNodeList?permNodeList->getLength():0;;
		if(permListLen>0)
		{
			for(unsigned int _cnt=0;_cnt<permListLen;_cnt++)
			{
				Ref<Element> node=permNodeList->item(static_cast<unsigned long>(_cnt));
				if(!node) continue;
				permName= Utils::getNodeText(node);
				if(!permName.empty())
				{
					DEBUGL8("Permission is = %s\n", permName.c_str());
					Status status = STATUS_OK;
					if(role.CheckPermissionEnable(objectPermSet,permName,status))
						//enable
						node->setAttribute("isEnable","true");
					else
						node->setAttribute("isEnable","false");
					if(status == STATUS_AL_SEC_INVALID_PERMISSION)
					{
						DEBUGL1("Permission is not found :: %s\n",permName.c_str());
						return status;
					}
				}
				node = NULL;
			}
		}
		else
		{
			DEBUGL1("Permission List is Empty!!");
			return STATUS_AL_SEC_EMPTY_PERMISSION_LIST;
		}
	}
	else
	{
		DEBUGL1("Permissions node is not present");
		return STATUS_AL_SEC_INTERNAL_ERROR;
	}
	return STATUS_OK;	
}
Status GetRoles::Execute()
{
	ElementRef pParamNode = NULL;
	ElementRef pNewUserData = NULL; 
	CString sUserDataXPath = STRING_NULL;
	Status ret = STATUS_FAILED;
	pParamNode = m_command->GetParamNode(ROLE_DETAILS);
	DocumentRef pNewUserDoc;	
	if(pParamNode)
	{
		ret = ProcessTransDoc("GetRoles",sUserDataXPath,pParamNode,pNewUserDoc);
		if (ret != STATUS_OK)
		{
			DEBUGL1("In GetRoles::Execute:: Failed \n");
			ret = STATUS_AL_SEC_COMMAND_SCHEMA;
			return ret;
		}
		DocumentGuard domGuard(pNewUserDoc);
		if(STATUS_OK != domGuard.WriteLock(false))
		{
			DEBUGL1("Failed to Acquire Write Lock on Rbac Session DOM\n");
			return STATUS_FAILED;
		}
		pNewUserData = CRoleManager::m_pHDB->BindToElement(pNewUserDoc,sUserDataXPath.c_str(),true);
		if (!pNewUserData)
		{
			DEBUGL2("GetRoles::Execute::Failed To Bind To Information Node (%s) Passed by Client Document\n",sUserDataXPath.c_str());
			return STATUS_AL_SEC_COMMAND_SCHEMA;
		}
		ret = ExecuteGetRoles(pNewUserData,sUserDataXPath);	
	}
	else
	{
		DEBUGL1("In GetRoles::Execute:: Failed \n");
		 ret = STATUS_AL_SEC_COMMAND_SCHEMA;
	}

	return ret;	
}

Status GetRoles::ExecuteGetRoles(ElementRef pNewUserData , CString sUserDataXPath)
{
	Status ret = STATUS_FAILED;
	CString xml="";
	DEBUGL8("GETNODENAME::%s\n",pNewUserData->getNodeName().c_str());
	CString bindNode = pNewUserData->getNodeName();	
	if(bindNode == "Role")
	{
		// Get a single role
		ret =GetIndividualRole(pNewUserData, sUserDataXPath, xml);
		if(ret != STATUS_OK)
		{
			DEBUGL1("In GetRoles::ExecuteGetRoles failed\n");
			return ret;
		}	
	}
	else if((bindNode == "RBACManager") || (bindNode == "Roles")|| (!bindNode.empty()))
	{
		//Get All roles
		CString bindPath ;
		ElementRef _pNewUserData= NULL;
		bindPath = sUserDataXPath + "/Roles" ;
		try{
		if(bindNode == "Roles")	
			_pNewUserData = pNewUserData;
		else
			_pNewUserData = CRoleManager::m_pHDB->BindToElement(pNewUserData, "Roles");
		ElementRef pParent = NULL; 
		if(_pNewUserData)
                {
                	//Remove Existing <Roles> element node
			pParent = _pNewUserData->getParentNode();
			if(pParent)
			{
				//Remove the node pointing by _pNewUserData
				if(!pParent->removeChild(_pNewUserData))
				{
					DEBUGL1("\n Failed to remove child node <Roles>\n");
					return STATUS_AL_SEC_INTERNAL_ERROR;
				}
			}
			else
			{
				DEBUGL1("\n Failed to get Paranet node of <Roles>\n");
				return STATUS_AL_SEC_INTERNAL_ERROR;
			}
                }
		else
			pParent =pNewUserData;

                CString xml = "<Roles/>";
                ElementRef pNewUserEle = NULL;
		
                if(AddTheNewNode(xml, pParent , pNewUserEle) != STATUS_OK)
		{
			DEBUGL1("Failed to DeSerialized the string :: %s\n",xml.c_str());
                	return STATUS_FAILED;
		}
                if(pNewUserEle)
                {
                	CString xml;
                	try{
				DEBUGL8("\n BIND PATH :: %s\n",bindPath.c_str());
                		ret=GetAllRoles(pNewUserEle,  bindPath , xml);
				if(ret != STATUS_OK)
                		{
                			DEBUGL2("GetRoles::Execute::Failed \n");
                			return ret;	
                		}
                	}
                	catch(CRoleException& userExcept){
                		DEBUGL1("\n GetRoles::ExecuteIndividualRole:: CRoleException caught !! \n");
                		return STATUS_FAILED;
                	}
                }
		}catch(DOMException& domExcept)
		{
			DEBUGL1("\n Caught DOMException !!!\n");
			return STATUS_FAILED;
		}
		catch(...)
		{
			DEBUGL1("Caught UnKnown Exception!!");
			return STATUS_AL_SEC_UNKNOWN_ERROR; 
		}
	}
	else
	{
        	ret = STATUS_AL_SEC_COMMAND_SCHEMA;
                DEBUGL1("\nGetRoles:: Invalid Bind Node\n");
        }

	return ret;
}

Status GetRoles::GetAllRoles(ElementRef pNewUserDoc,  CString sUserDataXPath, CString &xml)
{
	Status ret = STATUS_FAILED;
	
	xml = "<Role><Information>";
	xml += "<name></name>";
	xml += "<type></type>";
	xml += "<desc></desc>";
	xml += "</Information></Role>";
	
	ci::operatingenvironment::Ref<dom::Element> pNewUserEle = NULL;
	ret = ExecuteSSDK(sUserDataXPath.c_str(), pNewUserDoc, STRING_NULL,xml, pNewUserEle);	

	return ret;
}

Status GetRoles::GetIndividualRole(ElementRef pNewUserDoc,  CString sUserDataXPath, CString &xml)
{
	Status ret = STATUS_OK;
	CString roleXML ="";
	roleXML = "<Information>";
        roleXML += "<name></name>";
        roleXML += "<type></type>";
        roleXML += "<desc></desc>";
        roleXML += "</Information>";
	Role role;
	CString rName = role.GetName(sUserDataXPath,pNewUserDoc);
	if(rName.empty())
	{
		DEBUGL1("\n Role Name is empty\n");
		return STATUS_AL_SEC_INVALID_INPUT;
	}
	ElementRef pNewUserEle = NULL;
	ElementRef pParent = NULL;
	//Remove existing <Information> node
	ElementRef pInfoElem = CRoleManager::m_pHDB->BindToElement(pNewUserDoc,"Information");
	if(pInfoElem)
	{
		//Remove the <Information> node
		pParent = pInfoElem->getParentNode();
		if(pParent)
		{
			if(!pParent->removeChild(pInfoElem))
			{
				DEBUGL1("\n Failed to remove node <Information>\n");
				return STATUS_FAILED;
			}
		}
		else
		{	
			DEBUGL1("\n Failed to Get Parent node of <Information>\n");
			return STATUS_AL_SEC_INTERNAL_ERROR;
		}
	}
	else
	{
		DEBUGL1("Failed to Bind on <Information>\n");
		return STATUS_AL_SEC_INTERNAL_ERROR;
	}	

	ret = ExecuteSSDK(sUserDataXPath,pNewUserDoc,rName,roleXML,pNewUserEle);
	if(ret != STATUS_OK)
		DEBUGL1("\nGetRoles::GetIndividualRole Failed\n");
	return ret;
}

Status GetRoles::ExecuteSSDK(ci::operatingenvironment::CString sUserDataXPath, NodeRef pNewUserDoc, CString rName,CString &xml, ci::operatingenvironment::Ref<dom::Element> &pNewUserEle)
{
	MyUserToken myut;
	BOProxyRef boProxy;
	Ref<SSDKLogInterface> LogExternalPtr=NULL;
	ssdk::SSDKStatus ssdkStatus;	
	CString userXml =USER_XML;

	NodeRef pNewEle, pNewObjEle;
	Status ret = STATUS_FAILED;
	ci::operatingenvironment::Ref<dom::Element> pNewUserInfoEle,pNewObjInfoEle; 
	
	ret=Authenticate(myut, boProxy, LogExternalPtr);
	if(ret != STATUS_OK)
        {
           DEBUGL2("GetUsers::Authenticate::Failed \n");
           return ret;
        }
	 Ref<SSDKSecurityManagerInterface> ssdkSecurityManagerInterface =ssdk::GetSecurityManager();
	 Ref<SSDKRBACMgmtInterface> rbac = dynamic_cast<SSDKRBACMgmtInterface*> (ssdkSecurityManagerInterface->GetInterface  (SSDKSecurityManagerInterface::RBAC_MGMT_INTERFACE,ssdkStatus));
	if(rbac==(void*) NULL || ssdkStatus != OK)
        {
                DEBUGL1("\n ssdk SecurityManager::SSDKRBACMgmtInterface Failed \n");
                return STATUS_AL_SEC_INTERNAL_ERROR;
        }

	SSDKRBACMgmtInterface::UserInfo SSDKUserInfo;
		
	
	bool create = false;
	Role role;
	try{
		vector<SSDKRBACMgmtInterface::RoleInfo> roles,sortedRoles;
		set< pair <CString,CString> > ::iterator it;
		ssdkStatus=rbac->GetProperty(myut.ut,SSDKRBACMgmtInterface::ROLE_ALL_PROPERTIES,rName,roles);
		if(OK == ssdkStatus)
		{
			DEBUGL8("\nGet all roles .......\n");
			unsigned int size= roles.size();
			CString roleName=STRING_NULL;
			string isAdminRole ="false";
			for(unsigned int _cnt=0;_cnt<size;++_cnt)
			{
				SSDKRBACMgmtInterface::RoleInfo tempRoleInfo;
				tempRoleInfo =rbac->GetProperty(myut.ut,SSDKRBACMgmtInterface::ROLE_ALL_PROPERTIES,roles[_cnt].m_RoleId,ssdkStatus);
				if(OK != ssdkStatus)
					DEBUGL1("Failed to fetch unique id for role:%s\n",roles[_cnt].m_RoleId.c_str());
				sortedRoles.push_back(tempRoleInfo);
			}
			bool sortAscending = true;
			if(sortAscending)
				sort(sortedRoles.begin(),sortedRoles.end(),Utils::SortAscending);
			else
				sort(sortedRoles.begin(),sortedRoles.end(),Utils::SortDescending);
			for(unsigned int _cnt=0;_cnt<size;++_cnt)
                        {
				if(rName.empty())
                                {
                                        if(AddTheNewNode(xml, pNewUserDoc, pNewUserEle) != STATUS_OK)
					{
						DEBUGL1("\nFailed to DeSerialize the string :: %s\n",xml.c_str());
                                               	return STATUS_FAILED; 
					}
                                }
                                else
                                {
                                   pNewUserEle = pNewUserDoc;
                                   create = true;
                                }
				DEBUGL8("\n DeSerialize the string ::%s\n",xml.c_str());
				DEBUGL8("\n ROLE NAME ::%s\n",sortedRoles[_cnt].m_RoleId.c_str());
				DEBUGL8("\n USER_SIZE ::%d\n",sortedRoles[_cnt].m_RoleUsers.size());
				roleName = sortedRoles[_cnt].m_RoleId.c_str();
				//If role name is "Administrator" or "Auditor" or "ServiceTech" then we should not return its permission list
				//if((roleName == "Administrator") || (roleName == "Auditor")||(roleName == "ServiceTech"))
				//	continue;
				//Set the role name and the description
				role.SetName(sUserDataXPath.c_str(), pNewUserEle,roleName.c_str(),create);
				//Set the role description
				role.SetDesc( sUserDataXPath.c_str(), pNewUserEle, sortedRoles[_cnt].m_RoleDescription.c_str(),  create);
				DEBUGL8("Role Unique id:%s\n",sortedRoles[_cnt].m_RoleUniqueID.c_str());
				//Set the unique id
				role.SetUniqueId(sUserDataXPath.c_str(),pNewUserEle,sortedRoles[_cnt].m_RoleUniqueID,create);
				//Get the objects
				DEBUGL8("\n Objects and Permissions...............%d \n",sortedRoles[_cnt].m_RolePermissions.size());
				CString tempObjName,ObjOpers;
				vector<CString> perms;
				CString parentNode = "Operations";
                                CString childNode="operation";
				CString oprXmlNode ="";
				if(sortedRoles[_cnt].m_RolePermissions.size()>0)
				{
				
					//Bind to the node "Objects"
					CString objectsXml = "Information";
					CString objXml=OBJ_XML;
					NodeRef ObjRef =pNewUserEle;
					pNewObjEle=CRoleManager::m_pHDB->BindToElement(ObjRef, objectsXml, false);
				 	if(!pNewObjEle)
					{
						DEBUGL1("\n Failed to Bind to Node <Information>\n");
						return STATUS_AL_SEC_INTERNAL_ERROR;
					}
					/* 
					it=roles[_cnt].m_RolePermissions.begin();
					tempObjName=(*it).first.c_str();
					CString objectXML="";*/
					CString permissionXML="";
					ObjectCategoryMap::iterator iterCatMap;
					ObjectDisplayNameMap::iterator iterDisplayMap;
					CategoryDisplayNameMap::iterator iterCatDisMap;
					CategoryDisplayNameMap::iterator iterDisObj;
					ObjectOperationMap::iterator iterObjOprMap;
					CategoryDisplayNameMap catDisMap;
					SetString displaySet; 
					/*
					for(it=roles[_cnt].m_RolePermissions.begin();it!=roles[_cnt].m_RolePermissions.end();it++)
					{
						DEBUGL8("ObjName %s   Oper   %s \n",(*it).first.c_str(),(*it).second.c_str());
					
						if(tempObjName.compare((*it).first)==0)
						{
							perms.push_back((*it).second);
						}
						else
						{
							//MakeTokenizeString( perms, ObjOpers);
							oprXmlNode=CreateXMLNode(perms,parentNode,childNode);
							EbxSSDKUtil::CheckXMLEntityAndReplaceString(tempObjName);
							//EbxSSDKUtil::CheckXMLEntityAndReplaceString(ObjOpers);
							objectXML = objectXML +"<Object><name>"+ tempObjName +"</name>"+oprXmlNode+"</Object>";
							tempObjName=(*it).first.c_str();
							perms.clear();
							perms.push_back((*it).second);
						}
					}
					//MakeTokenizeString( perms, ObjOpers);
					
					oprXmlNode=CreateXMLNode(perms,parentNode,childNode);
					EbxSSDKUtil::CheckXMLEntityAndReplaceString(tempObjName);
					//EbxSSDKUtil::CheckXMLEntityAndReplaceString(ObjOpers);
					objectXML = objectXML +"<Object><name>"+ tempObjName +"</name>"+oprXmlNode+"</Object>";
					objectXML = "<Objects>"+ objectXML +"</Objects>";
					if(AddTheNewNode(objectXML, pNewObjEle, pNewObjInfoEle) == STATUS_FAILED)
					{
						DEBUGL1("\nFailed to DeSerialize the string :: %s\n",objectXML.c_str());
						throw CRoleException("STATUS_FAILED");
					}*/
					pNewObjInfoEle = NULL;
					//Read the DisplayObjectMap, for each display get the object list and then search for each object in object list in Role ObjectList return from SSDK and check the opeartion
					for(iterDisObj=CRoleManager::displayObjectMap.begin();iterDisObj!=CRoleManager::displayObjectMap.end();++iterDisObj)
					{
						SetString objectSet;
						SetString::iterator iterObjSet;
						SetString::iterator iterOprSet; 
						objectSet = iterDisObj->second;
						//For Each Object
						bool allObjPresent= false; 
						for(iterObjSet=objectSet.begin();iterObjSet!=objectSet.end();++iterObjSet)
						{
							//Get All operation for each object
							iterObjOprMap = CRoleManager::objectOperMap.find(*iterObjSet);
							if(iterObjOprMap != CRoleManager::objectOperMap.end())
							{
								for(iterOprSet=(iterObjOprMap->second).begin();iterOprSet!=(iterObjOprMap->second).end();++iterOprSet)
								{
									pair <CString,CString> objPair;
									objPair = make_pair(*iterObjSet,*iterOprSet);
									set< pair <CString,CString> > ::iterator iterObjOprSet;
									iterObjOprSet =	sortedRoles[_cnt].m_RolePermissions.find(objPair);
									if(iterObjOprSet!=sortedRoles[_cnt].m_RolePermissions.end())
									{
										DEBUGL8("Object ::%s Operation :: %s is present\n",(*iterObjSet).c_str(),(*iterOprSet).c_str());
										allObjPresent = true;
									}
									else
									{
										allObjPresent = false;
										break;
									}
								}
								if(!allObjPresent)
									break;
							}
							else
							{
								allObjPresent = false;
								break;
							}
						}
						//Get the CategoryName for each displayName
						if(allObjPresent)
						{
							/*ObjectCategoryMap::iterator disCatIter;
                                                	disCatIter = CRoleManager::displayCatMap.find(iterDisObj->first);
							if(disCatIter != CRoleManager::displayCatMap.end())
							{
								//Put the display name in CategoryDisplayNameMap
								CategoryDisplayNameMap::iterator iterCatDisMap;
								if((iterCatDisMap= catDisMap.find(disCatIter->second))!=catDisMap.end())
                                                                {
                                                                        DEBUGL5("CATEGORY NAME IS ALREADY PRESENT\n");
                                                                        (iterCatDisMap->second).insert(iterDisObj->first);
                                                                }
                                                                else
                                                                {
                                                                        DEBUGL5("NEW CATEGORY NAME\n");
                                                                        displaySet.insert(iterDisObj->first);
                                                                        catDisMap.insert(CategoryDisplayNameMap::value_type(disCatIter->second,displaySet));
                                                                        displaySet.clear();
                                                                }
							}*/
							displaySet.insert(iterDisObj->first);
						}
					}
					//Handle AddressBook and JobTemplate object
					/*
					vector<CString>oprVec;
					oprVec.push_back(OP_READ);	
					oprVec.push_back(OP_EDIT);
					vector<CString> displayVec;
					displayVec.push_back(OPR_READ);
					displayVec.push_back(OPR_EDIT);*/
					//HandleAddressBook(roles[_cnt].m_RolePermissions,catDisMap,OBJ_ADDRESSBOOK,oprVec,displayVec);
					//HandleJobTemplate(roles[_cnt].m_RolePermissions,catDisMap,OBJ_JOBTEMPLATE,oprVec,displayVec);
					//Display the CategoryDisplayNameMap
					permissionXML.clear();	
					/*for(iterCatDisMap=catDisMap.begin();iterCatDisMap!=catDisMap.end();++iterCatDisMap)
                                        {
                                                DEBUGL8("\nSTART \n");
                                                DEBUGL8("CATEGORY_NAME :: %s",(iterCatDisMap->first).c_str());
                                                SetString::iterator iterSet;
                                                oprsXml="";
                                                for(iterSet=(iterCatDisMap->second).begin();iterSet!=(iterCatDisMap->second).end();++iterSet)
                                                {
                                                        oprsXml += "<operation>"+*iterSet +"</operation>";
                                                        DEBUGL8("DISPLAY NAME :: %s \t",(*iterSet).c_str());
                                                }
						if(!oprsXml.empty())
						{
							oprsXml = ALL_OPR_START_TAG + oprsXml + ALL_OPR_END_TAG;
							permissionXML = permissionXML + "<Permission><name>" + (iterCatDisMap->first).c_str() +"</name>"+ oprsXml + "</Permission>";
						}
                                                DEBUGL8("\nEND\n");
						DEBUGL8("PERMISSION XML:: %s\n",permissionXML.c_str());
                                        }*/
					SetString::iterator permissionIter;
					permissionXML="";
					for(permissionIter=displaySet.begin();permissionIter!=displaySet.end();++permissionIter)
					{
						DEBUGL8("\nSTART \n");
						DEBUGL8("Permission Name :: %s\n",(*permissionIter).c_str());
						permissionXML+= "<permission>"+ *permissionIter + "</permission>";
						DEBUGL8("\nEND\n");
						DEBUGL8("PERMISSION XML:: %s\n",permissionXML.c_str());
					}
					if(!permissionXML.empty())
					{
						permissionXML = ALL_PERM_START_TAG  + permissionXML + ALL_PERM_END_TAG;
						DEBUGL4("Complete Permission List ::%s\n",permissionXML.c_str());
						if(AddTheNewNode(permissionXML, pNewObjEle, pNewObjInfoEle) != STATUS_OK)
                                        	{
                                                	DEBUGL1("\nFailed to DeSerialize the string :: %s\n",permissionXML.c_str());
                                                	return STATUS_FAILED;	
                                        	}
                                        	pNewObjInfoEle = NULL;
					}
					isAdminRole = rbac->GetProperty(myut.ut,SSDKRBACMgmtInterface::RBAC_ROLE_OBJECT,roleName,IS_ADMIN_ROLE,ssdkStatus);//DBCORR
					if(ssdkStatus == AUTH_RBAC_DB_CORRUPTED)
					{
						DEBUGL2("SSDK RBAC DB CRRUPTED\n");
						Utils::CheckForCorruption(ssdkStatus);
						return STATUS_AL_SEC_DB_CORRUPTED;
					}

					if(isAdminRole == "true")
						role.SetRoleType(sUserDataXPath.c_str(), pNewUserEle,ADMIN_ROLE,create);	
					else
						role.SetRoleType(sUserDataXPath.c_str(), pNewUserEle,SYSTEM_ROLE,create);
					roleName =STRING_NULL;
					pNewUserEle = NULL;
				}
			}
				
				//Get All the custom property for this object id, currently ssdk does not support to get all the custom properties for a object id and also nor sure whether EBX wants all these information during GetRoles 
		}
		else
		{
			DEBUGL1("\nGetProperty(ROLE_ALL_PROPERTIES) Status :: %d\n",ssdkStatus);
			ret = SSDK_RBAC_MANAGER_PLUGIN_ERROR_MAP(ssdkStatus);
			return ret;
		}
	}
	catch(DOMException& domExcept)
	{
		DEBUGL1("\n Caught DOMException !! \n");
		return STATUS_FAILED;
	}
	catch(...)
	{
		DEBUGL1("Some Exceptions from ssdk GetProperty() call");
		return STATUS_AL_SEC_UNKNOWN_ERROR;
	}		
	return STATUS_OK;
}
void GetRoles::HandleAddressBook(const std::set<std::pair<std::string,std::string> > & permissionSet,CategoryDisplayNameMap & catDisMap,const CString & objectVec, vector<CString> & oprVec, vector<CString> & displayVec)
{
	pair <CString,CString> objPair;
	for(unsigned int i=0;i<oprVec.size();++i)
	{
        	//objPair = make_pair(OBJ_ADDRESSBOOK,oprVec[i]);
		SetString displaySet;
		CategoryDisplayNameMap::iterator iterCatDisMap;
        	set< pair <CString,CString> > ::const_iterator iterObjOprSet = permissionSet.find(objPair);
		if(iterObjOprSet!=permissionSet.end())
		{
			//Put the display name in CategoryDisplayNameMap
			iterCatDisMap= catDisMap.find(CAT_ADDRS_BOOK);
			if(iterCatDisMap !=catDisMap.end())
                	{
                		DEBUGL8("CATEGORY NAME IS ALREADY PRESENT\n");
                		(iterCatDisMap->second).insert(displayVec[i]);
                	}
                	else
                	{
                		DEBUGL8("NEW CATEGORY NAME\n");
                		displaySet.insert(displayVec[i]);
                		catDisMap.insert(CategoryDisplayNameMap::value_type(CAT_ADDRS_BOOK,displaySet));
                		displaySet.clear();
                	}
		}
	}
	return;
}

void GetRoles::HandleJobTemplate(const std::set<std::pair<std::string,std::string> > & permissionSet,CategoryDisplayNameMap & catDisMap,const CString & objectVec, vector<CString> & oprVec, vector<CString> & displayVec)
{
        pair <CString,CString> objPair;
        for(unsigned int i=0;i<oprVec.size();++i)
        {
                //objPair = make_pair(OBJ_ADDRESSBOOK,oprVec[i]);
                SetString displaySet;
                CategoryDisplayNameMap::iterator iterCatDisMap;
                set< pair <CString,CString> > ::const_iterator iterObjOprSet = permissionSet.find(objPair);
                if(iterObjOprSet!=permissionSet.end())
                {
                       // Put the display name in CategoryDisplayNameMap
                        iterCatDisMap= catDisMap.find(CAT_JOB_TEMPLATE);
			if(iterCatDisMap !=catDisMap.end())
                        {
                                DEBUGL8("CATEGORY NAME IS ALREADY PRESENT\n");
                                (iterCatDisMap->second).insert(displayVec[i]);
                        }
                        else
                        {
                                DEBUGL8("NEW CATEGORY NAME\n");
                                displaySet.insert(displayVec[i]);
                                catDisMap.insert(CategoryDisplayNameMap::value_type(CAT_JOB_TEMPLATE,displaySet));
                                displaySet.clear();
                        }
                }
        }
        return;
}

Status CreateRole::Execute()
{
	ElementRef pParamNode = NULL;
	ElementRef pNewUserData = NULL; 
	CString sUserDataXPath = STRING_NULL;
	Status ret = STATUS_OK;
	pParamNode = m_command->GetParamNode(ROLE_DETAILS);
	DocumentRef pNewUserDoc;
	if(pParamNode)
	{
		//convert from string xpath to element ref
		ret = ProcessTransDoc("CreateRole",sUserDataXPath,pParamNode,pNewUserDoc);	
		if (ret != STATUS_OK)
		{
			DEBUGL1("In CreateRole::Execute:: Failed \n");
			ret = STATUS_AL_SEC_COMMAND_SCHEMA;
			return ret;
		}
		DocumentGuard domGuard(pNewUserDoc);
		if(STATUS_OK != domGuard.WriteLock(false))
                {
                        DEBUGL1("Failed to Acquire Write Lock on Rbac Session DOM\n");
                        return STATUS_FAILED;
                }
		pNewUserData = CRoleManager::m_pHDB->BindToElement(pNewUserDoc,sUserDataXPath.c_str(),true);
		if (!pNewUserData)                      
		{
			DEBUGL2("CreateRole::Execute::Failed To Bind To Information Node (%s) Passed by Client Document\n",sUserDataXPath.c_str());
        		return STATUS_AL_SEC_COMMAND_SCHEMA;
		}	
		ret = ExecuteSSDK(sUserDataXPath,pNewUserData);	
	}
	else
	{
		DEBUGL1("In CreateRole::Execute:: Failed \n");
		ret = STATUS_AL_SEC_COMMAND_SCHEMA;
	}

	return ret;	
}

/*void CreateRole::AssignUnassignPermission(map<CString,bool> & permissionEnableDisableMap)
{
	//Assign AddressBookReadEdit and JobTemplateReadEdit
	map<CString,bool>::iterator iter;
	vector<CString> disablePermission;
	disablePermission.push_back(OPR_DEVICE_SETTING);
	disablePermission.push_back(OPR_CLONE);
	disablePermission.push_back(OPR_ACCESS_DIAGONOSTICMODE);
	disablePermission.push_back(OPR_USER_DEPT_MGNT);
	disablePermission.push_back(OPR_LOG_READ);
	disablePermission.push_back(OPR_LOG_EXPORT);
	disablePermission.push_back(OPR_JOB_OPERATION);

	vector<CString>::iterator iterPerm;
	for(iterPerm = disablePermission.begin();iterPerm != disablePermission.end(); ++iterPerm)
	{
		iter = permissionEnableDisableMap.find(*iterPerm);
		if(iter != permissionEnableDisableMap.end())
		{
			iter->second = false;
			DEBUGL4("PERMISSION NAME :: %s MUST BE DISABLE\n",(*iterPerm).c_str());
		}
	}
	return;
}*/
Status CreateRole::ExecuteSSDK(ci::operatingenvironment::CString sUserDataXPath, NodeRef pNewUserDoc)
{
	MyUserToken myut;
	BOProxyRef boProxy;
	Ref<SSDKLogInterface> LogExternalPtr=NULL;
	ssdk::SSDKStatus ssdkStatus;
	Status ret=STATUS_FAILED;
	ci::operatingenvironment::Ref<dom::Element> pNewUserEle=NULL;
	//Authenticate the user
	ret=Authenticate(myut, boProxy, LogExternalPtr);
	if(ret != STATUS_OK)
	{
		DEBUGL2("Create Role ::Authenticate::Failed \n");
		//throw CRoleException(STATUS_FAILED);
		return ret;
	}
	Ref<SSDKSecurityManagerInterface> ssdkSecurityManagerInterface =ssdk::GetSecurityManager();
	Ref<SSDKRBACMgmtInterface> rbac = dynamic_cast<SSDKRBACMgmtInterface*> (ssdkSecurityManagerInterface->GetInterface  (SSDKSecurityManagerInterface::RBAC_MGMT_INTERFACE,ssdkStatus));
	if(rbac==(void*) NULL || ssdkStatus != OK)
	{
		DEBUGL1("\n ssdk SecurityManager::SSDKRBACMgmtInterface Failed \n");
		return STATUS_AL_SEC_INTERNAL_ERROR;
	}
	try
	{
		//Check number of roles
		vector<SSDKRBACMgmtInterface::RoleInfo> allRoles;
		vector<SSDKRBACMgmtInterface::RoleInfo>::iterator roleIt;
		ssdkStatus = rbac->GetProperty(myut.ut,SSDKRBACMgmtInterface::ROLE_ALL_PROPERTIES,SSDKRBACMgmtInterface::ROLE_UNIQUE_ID_PROPERTY,"",allRoles);
		if(ssdkStatus !=OK)
		{
			DEBUGL1("Failed in GetProperty(ROLE_ID_PROPERTY) with Status ::%d\n",ssdkStatus);
			ret = SSDK_RBAC_MANAGER_PLUGIN_ERROR_MAP(ssdkStatus); 
			return ret;
		}
		unsigned int totalRole = 0;
		for(roleIt = allRoles.begin(); roleIt != allRoles.end(); roleIt++)
		{
			istringstream temp;
			temp.str(roleIt->m_RoleUniqueID);
			int uniqueId = -1;
			temp>>uniqueId;
			if(uniqueId <= 30)
				totalRole++;
		}
		if(totalRole >= MAX_POSSIBLE_ROLE)
		{
			DEBUGL2("Maximum Role Creation Limit is reached\n");
			return STATUS_AL_SEC_OBJECT_MAX_REACHED;
		}
		bool create = false;
		Role userRole; //to get information from UI
		CString roleName,strstatus;
		CString InheritRoles;
		CString ldapEnable,ldapRule,userList,groupList;
		//Get the Role name from UI
		roleName = userRole.GetName( sUserDataXPath,  pNewUserDoc, create);
		if(roleName.empty())
			return STATUS_AL_SEC_INVALID_INPUT;
		if(!Utils::CheckForNameValidity(roleName))
		{
			DEBUGL1("Role name invalid\n");
			return STATUS_AL_SEC_ROLENAME_INVALID_INPUT;
		}

		//The below function considers multibyte characters also , during length check.
		if(Utils::IsMaxLengthExceeded(roleName,MAX_ROLE_NAME_SIZE))
		{
			DEBUGL1("Rolename length has exceeded the maximum length\n");
			return STATUS_AL_SEC_OBJECT_MAX_LENGTH_EXCEEDED;	
		}


		vector<CString> roleVector;
		SSDKRBACMgmtInterface::RoleInfo SSDKRoleInfo;	
		set< pair <CString,CString> > ::iterator it;
		/*
		userRole.GetInhRoles( sUserDataXPath,  pNewUserDoc,roleVector, create);
		DEBUGL5("\n INHERIT_ROLE_LIST ::%s\n",InheritRoles.c_str());
		if(roleVector.size()>0)
		{
			//Get the roles into SSDK Role Info
			SSDKRoleInfo.m_RoleRoles=roleVector;
		}

		// get all objects
		strstatus = userRole.GetObjectPermPair( sUserDataXPath, pNewUserDoc,SSDKRoleInfo.m_RolePermissions,create);
		if (!strstatus.empty())
		{
			DEBUGL8("\n Objects and Permissions............... \n");
			for(it=SSDKRoleInfo.m_RolePermissions.begin();it!=SSDKRoleInfo.m_RolePermissions.end();it++)
				DEBUGL8("%s      %s \n",(*it).first.c_str(),(*it).second.c_str());
		}*/
		/*strstatus = userRole.GetPermission( sUserDataXPath, pNewUserDoc,SSDKRoleInfo.m_RolePermissions,create);
		if (!strstatus.empty())
                {
                        DEBUGL8("\n Objects and Permissions............... \n");
                        for(it=SSDKRoleInfo.m_RolePermissions.begin();it!=SSDKRoleInfo.m_RolePermissions.end();it++)
                                DEBUGL8("%s      %s \n",(*it).first.c_str(),(*it).second.c_str());
                }*/

		map<CString,bool> permissionEnableDisableMap;
		ret = ReadPermissions(pNewUserDoc,sUserDataXPath,permissionEnableDisableMap,true);
		if(ret != STATUS_OK)
		{
			DEBUGL1("Invalid Permissions List\n");
			return ret;
		}
		//unassign automatically few deafult permision name 
		UnassignPermission(permissionEnableDisableMap);
		map<CString,bool>::iterator iter;
		for(iter=permissionEnableDisableMap.begin();iter!=permissionEnableDisableMap.end();++iter)
		{
			ret = userRole.SetEnableDisablePermission(SSDKRoleInfo.m_RolePermissions,iter->first,iter->second);
			if(ret !=STATUS_OK)
			{
				DEBUGL2("Failed to set permission :: %s\n",(iter->first).c_str());
				return ret;	
			}
		}
		if(SSDKRoleInfo.m_RolePermissions.size()<=0)
                {
                        DEBUGL1("\nInvalid Permission List \n");
                        return STATUS_AL_SEC_EMPTY_PERMISSION_LIST;
                }
		//Assigning default permission
		SSDKRoleInfo.m_RolePermissions.insert(pair<CString,CString>(OBJ_DEFAULT,OP_DEFAULT));
			
		vector <CString > tempVector;
		//Get the name for role name 
		SSDKRoleInfo.m_RoleId = roleName;

		//Assign ssdkrule if role mapping file exists 
		UpdateRoleRule(roleName,SSDKRoleInfo);

		//Create Role 
		ssdkStatus=rbac->RBACRoleOperation(myut.ut,SSDKRBACMgmtInterface::ROLE_CREATE,SSDKRoleInfo);
		if(OK!=ssdkStatus)
		{
			DEBUGL1("CREATE ROLE FAILED\n.....%d",ssdkStatus);
			ret = SSDK_RBAC_MANAGER_PLUGIN_ERROR_MAP(ssdkStatus);	
			return ret;
		}
		//Get the role unique id and set it to transaction document
		userRole.SetUniqueId("",pNewUserDoc,SSDKRoleInfo.m_RoleUniqueID,true);
		//Set the type of role as Admin role
		tempVector.clear();
		bool isAllDataSet = true;	
		ssdkStatus=rbac->SetProperty(myut.ut,SSDKRBACMgmtInterface::RBAC_ROLE_OBJECT,roleName,IS_ADMIN_ROLE,"true");//DBCORR
		if(ssdkStatus == AUTH_RBAC_DB_CORRUPTED)
		{
			DEBUGL2("SSDK RBAC DB CRRUPTED\n");
			Utils::CheckForCorruption(ssdkStatus);
			return STATUS_AL_SEC_DB_CORRUPTED;
		}
		if(OK!=ssdkStatus)
		{
			DEBUGL2("SETTING IsAdminRole Property Failed with Status :: %d\n",ssdkStatus);
			isAllDataSet = false ;
		}
		if(!isAllDataSet)
		{
			//Remove the newly created role
			ssdkStatus =rbac->RBACRoleOperation(myut.ut,SSDKRBACMgmtInterface::ROLE_REMOVE,SSDKRoleInfo);
			DEBUGL3("\n RBACRoleOperation(ROLE_REMOVE) Status :: %d\n",ssdkStatus);	
			ret = SSDK_RBAC_MANAGER_PLUGIN_ERROR_MAP(ssdkStatus);
			return ret;	
		}
		//Setting RBAC Custom Properties if any specify by client
		ElementRef pNewEle = NULL;
		NodeRef RbacRef =pNewUserDoc;
		CString cusPropXml =    "Information/CustomProperties/AddViewProperty";
		pNewEle=CRoleManager::m_pHDB->BindToElement(RbacRef, cusPropXml);
		if(pNewEle)
		{
			CString nodeText = pNewEle->getParentNode()->getNodeName();
			DEBUGL8("NODE_TEXT::%s\n",nodeText.c_str());
			Ref<NodeList> elems = pNewEle->getChildNodes();
			unsigned int len = elems->getLength();
			for(unsigned int _cnt=0;_cnt<len;_cnt++)
			{
				Ref<Element> node=elems->item(static_cast<unsigned long>(_cnt));
				CString propName = userRole.GetRoleCusPropName(sUserDataXPath,node);
				CString propValue = userRole.GetRoleCusPropValue(sUserDataXPath,node);
				ssdkStatus= rbac->SetProperty(myut.ut,SSDKRBACMgmtInterface::RBAC_ROLE_OBJECT,roleName,propName,propValue);
				DEBUGL8("\nSSDK SetProperty(RBAC_ROLE_OBJECT) status is :: %d\n",ssdkStatus);
				node = NULL;
			}
		}
		ci::logmanager::LogInterface::OperationApplicationType oprApplication;
		CString loggedInUser = myut.ut->GetProperty(SSDKUserTokenInterface::USER_ID_PROPERTY, ssdkStatus);
		if(ssdkStatus != OK)
		{
			DEBUGL2("Failed to Get the user name from user token %d\n", ssdkStatus);
		}
		DEBUGL8("The Logged in User is %s\n", loggedInUser.c_str());
		CString loggedInUserDomainName = myut.ut->GetProperty(SSDKUserTokenInterface::USER_DOMAIN_PROPERTY, ssdkStatus);
		if(ssdkStatus != OK)
                {
                        DEBUGL2("Failed to Get the Domain name from user token %d\n", ssdkStatus);
                }
                DEBUGL8("The Logged in User Domain Name is %s\n", loggedInUserDomainName.c_str());
                CString ipAddr; 
                boProxy->GetApplicationInformation(oprApplication,ipAddr);
		Utils::WriteLog(NOTIFICATION_ADD_ROLE,true,ci::logmanager::LogInterface::opAdd,ci::logmanager::LogInterface::otRole,ci::logmanager::LogInterface::mtMaintenance,oprApplication, loggedInUser, roleName,ipAddr,loggedInUserDomainName);
	}
	catch(DOMException& domExcept)
        {
                DEBUGL1("\n Caught DOMException !! \n");
                return STATUS_FAILED;
        }
	catch(...)
	{
		DEBUGL1("Some Exceptions from ssdk GetProperty() call");
		ret = STATUS_AL_SEC_UNKNOWN_ERROR;
		return ret;
	}		
	return STATUS_OK;
}

Status DeleteRole::Execute()
{
	ElementRef pParamNode = NULL;
	ElementRef pNewUserData = NULL; 
	CString sUserDataXPath = STRING_NULL;
	Status ret = STATUS_OK;
	bool searchByName = true;
        pParamNode = m_command->GetParamNode(SEARCH_BY.c_str());
        NodeRef pParamSearchByText;
	DocumentRef pNewUserDoc;
        if(pParamNode)
        {
                pParamSearchByText = pParamNode->getFirstChild();
                CString searchBy;
                if(pParamSearchByText)
                        searchBy = pParamSearchByText->getNodeValue();
                if(searchBy == "ID")
                        searchByName = false;
                else if(searchBy == "NAME")
                        searchByName = true;

                DEBUGL8("Search By Name:%s\n",searchBy.c_str());
        }
	pParamNode = m_command->GetParamNode(ROLE_DETAILS);
	if(pParamNode)
	{
		//convert from string xpath to element ref
		ret = ProcessTransDoc("DeleteRole",sUserDataXPath,pParamNode,pNewUserDoc);
		if (ret != STATUS_OK)
		{
			DEBUGL1("In DeleteRole::Execute:: Failed \n");
			ret = STATUS_AL_SEC_COMMAND_SCHEMA;
			return ret;
		}
		DocumentGuard domGuard(pNewUserDoc);
		if(STATUS_OK != domGuard.ReadLock(false))
                {
                        DEBUGL1("Failed to Acquire Read Lock on Rbac Session DOM\n");
                        return STATUS_FAILED;
                }
		pNewUserData = CRoleManager::m_pHDB->BindToElement(pNewUserDoc,sUserDataXPath.c_str(),true);
		if (!pNewUserData)                      
		{
			DEBUGL2("DeleteRole::Execute::Failed To Bind To Information Node (%s) Passed by Client Document\n",sUserDataXPath.c_str());
        		return STATUS_AL_SEC_COMMAND_SCHEMA;
		}
	  	ret = ExecuteIndividualRole(pNewUserData, sUserDataXPath,searchByName);
	}
	else
	{
		MyUserToken myut;
	        BOProxyRef boProxy;
	        Ref<SSDKLogInterface> LogExternalPtr=NULL;
		ssdk::SSDKStatus ssdkStatus;
		ret = Authenticate(myut, boProxy, LogExternalPtr);
		if(ret  != STATUS_OK)
	        {
        	   DEBUGL2("DeleteRole::Authenticate::Failed \n");
	           return ret;
       		}
		 Ref<SSDKSecurityManagerInterface> ssdkSecurityManagerInterface =ssdk::GetSecurityManager();
        Ref<SSDKRBACMgmtInterface> rbac = dynamic_cast<SSDKRBACMgmtInterface*> (ssdkSecurityManagerInterface->GetInterface  (SSDKSecurityManagerInterface::RBAC_MGMT_INTERFACE,ssdkStatus));
	        if(rbac==(void*) NULL || ssdkStatus != OK)
        	{
                	DEBUGL1("\n ssdk SecurityManager::SSDKRBACMgmtInterface Failed \n");
	                return STATUS_AL_SEC_INTERNAL_ERROR;
        	}
		vector<SSDKRBACMgmtInterface::RoleInfo> roles;
		vector<SSDKRBACMgmtInterface::RoleInfo>::iterator it;
                ssdkStatus=rbac->GetProperty(myut.ut,SSDKRBACMgmtInterface::ROLE_ID_PROPERTY,STRING_NULL,roles);
                DEBUGL8("GetProperty(ROLE_ID_PROPERTY) Status ::%d\n",ssdkStatus);
		if(OK != ssdkStatus)
		{
			ret=SSDK_RBAC_MANAGER_PLUGIN_ERROR_MAP(ssdkStatus);	
			return ret;
		}
		vector<CString> roleID;
		for(it=roles.begin();it!=roles.end();it++)
			roleID.push_back((*it).m_RoleId);
		ret = ExecuteSSDK("",NULL,roleID,true,true) ;
	}
	return ret;	
}
Status DeleteRole::ExecuteIndividualRole(NodeRef pNewUserData, CString sUserDataXPath,bool searchByName)
{
	Status ret = STATUS_OK;
	DEBUGL8("IN DeleteRole::ExecuteIndividualRole ....%s\n",pNewUserData->getNodeName().c_str());
	vector<CString>roleList;
	bool multipleRoles = false;
	if(pNewUserData->getNodeName() ==  "Roles")
	{
		ci::operatingenvironment::Ref<NodeList> paramNodes = pNewUserData->getChildNodes();
		DEBUGL8("TOTAL_LENGTH::%d\n",paramNodes->getLength());
		multipleRoles = true;
		if(paramNodes)
		{
			for(unsigned int i=0; i<paramNodes->getLength(); i++)
			{
				if(GetIndividualRole(paramNodes->item(static_cast<unsigned long>(i)), paramNodes->item(static_cast<unsigned long>(i))->getNodeValue(),roleList,searchByName) != STATUS_OK)
				{
					DEBUGL2("In DeleteRole::ExecuteIndividualRole::Deletion of role failed\n");
				}
			}
			if(roleList.size()<=0)
			{
				DEBUGL1("Empty Role List\n");
				return STATUS_AL_SEC_EMPTY_ROLE_LIST;
			}
		}
		else
		{
			ret = STATUS_AL_SEC_INTERNAL_ERROR;
			return ret;
		}
	}
	else
	{
		ret =GetIndividualRole(pNewUserData, sUserDataXPath,roleList,searchByName);
		if(ret  != STATUS_OK)
		{
			DEBUGL1("Deletion of role failed\n");
			return ret;
		}
	}
	if(roleList.size()>0)
        {
        	ret = ExecuteSSDK(sUserDataXPath,pNewUserData,roleList,multipleRoles,searchByName);
        }
	return ret;
}
Status DeleteRole::GetIndividualRole(NodeRef pNewUserData,  CString sUserDataXPath, vector<CString> & roleList,bool searchByName)
{
	Status ret = STATUS_OK;
	Role role;
	CString rName = role.GetName(sUserDataXPath, pNewUserData);
	if(rName.empty() && searchByName)
	{
		DEBUGL2("Role Name is Empty \n");
		return STATUS_AL_SEC_INVALID_INPUT;
	}
	CString roleUniqueId = role.GetUniqueID(sUserDataXPath, pNewUserData);
	if(roleUniqueId != "")
	{
		int iOut;
	        if(!EbxSSDKUtil::validateInput<int>(roleUniqueId,iOut))
        	{
        		DEBUGL1("Invalid Role UniqueId \n");
	                return STATUS_AL_SEC_INVALID_INPUT;
        	}
	}
	if(roleUniqueId.empty() && !searchByName)
	{
		DEBUGL2("Role Unique id is Empty \n");
                return STATUS_AL_SEC_INVALID_INPUT;
	}
	if(searchByName)
		roleList.push_back(rName);
	else
		roleList.push_back(roleUniqueId);
	return ret;
}

Status DeleteRole::ExecuteSSDK(ci::operatingenvironment::CString sUserDataXPath, NodeRef pNewUserDoc, vector<CString> roleVector, bool multipleRoles,bool searchByName)
{
	MyUserToken myut;
	BOProxyRef boProxy;
	Ref<SSDKLogInterface> LogExternalPtr=NULL;
	ssdk::SSDKStatus ssdkStatus;
	Status ret = STATUS_FAILED;
	//Authenticate the user
	ret = Authenticate(myut, boProxy, LogExternalPtr);
	if(ret != STATUS_OK)
        {
           DEBUGL2("DeleteRole::Authenticate::Failed \n");
           return ret;
        }
	 Ref<SSDKSecurityManagerInterface> ssdkSecurityManagerInterface =ssdk::GetSecurityManager();
	 Ref<SSDKRBACMgmtInterface> rbac = dynamic_cast<SSDKRBACMgmtInterface*> (ssdkSecurityManagerInterface->GetInterface  (SSDKSecurityManagerInterface::RBAC_MGMT_INTERFACE,ssdkStatus));
	if(rbac==(void*) NULL || ssdkStatus != OK)
        {
                DEBUGL1("\n ssdk SecurityManager::SSDKRBACMgmtInterface Failed \n");
                return STATUS_AL_SEC_INTERNAL_ERROR;
        }
	
	//Role userRole; //to set user info back to UI
	//CString allUsers=STRING_NULL;
	//CString isAdminRole =STRING_NULL;
	//Extract the roles from the string 
	//ExtractToken(rList, DELIM, roleVector);
	//CString newRole = "";
	//if(multipleRoles)
	//	newRole.append("<Roles>");
	try
	{
		SSDKRBACMgmtInterface::RoleInfo SSDKRoleInfo;
		//unsigned int roleUsers =0;
		int numOfRoles = roleVector.size();
		//vector<CString> undeletedRoles;
		//CString setRoles=STRING_NULL;
		//vector<CString> userList;
		DEBUGL8("Number of roles to be deleted ::%d\n",numOfRoles);
		SSDKRBACMgmtInterface::RolePropertyName roleProperty = SSDKRBACMgmtInterface::ROLE_ID_PROPERTY;
		if(!searchByName)
		{
			roleProperty = SSDKRBACMgmtInterface::ROLE_UNIQUE_ID_PROPERTY;
		}
		for (int cnt =0; cnt <numOfRoles; cnt ++)
		{
			//Get the information for that roleid
			DEBUGL8("Role Name From List:%s",roleVector[cnt].c_str());
			SSDKRoleInfo=rbac->GetProperty(myut.ut,SSDKRBACMgmtInterface::ROLE_ALL_PROPERTIES,roleProperty,roleVector[cnt],ssdkStatus);
			//SSDKRoleInfo=rbac->GetProperty(myut.ut, SSDKRBACMgmtInterface::ROLE_ALL_PROPERTIES,roleVector[cnt], ssdkStatus);
			DEBUGL8("\n GetProperty(ROLE_ALL_PROPERTIES) Status :: %d\n",ssdkStatus);
			if(ssdkStatus == AUTH_RBAC_DB_CORRUPTED)
                        {
                                DEBUGL2("SSDK RBAC DB CRRUPTED\n");
                                Utils::CheckForCorruption(ssdkStatus);
                                return STATUS_AL_SEC_DB_CORRUPTED;
                        }

			/** If user send multiple roles to be deleted and if some roles does not exists then this fucntion will continue and  pick up the next role to be deleted but if user sends only single role to be deleted and if that role doest not exists then this function will return error**/
			if((OK!=ssdkStatus))//DBCORR
			{
				if(numOfRoles>1)
					continue;
				else
				{
					ret = SSDK_RBAC_MANAGER_PLUGIN_ERROR_MAP(ssdkStatus);	
					return ret;
				}
			}
			
			/*isAdminRole = rbac->GetProperty(myut.ut,SSDKRBACMgmtInterface::RBAC_ROLE_OBJECT,SSDKRoleInfo.m_RoleId,IS_ADMIN_ROLE,ssdkStatus);
			if(ssdkStatus == AUTH_RBAC_DB_CORRUPTED)
                        {
                                DEBUGL2("SSDK RBAC DB CRRUPTED\n");
                                Utils::CheckForCorruption(ssdkStatus);
                                return STATUS_AL_SEC_DB_CORRUPTED;
                        }
			if(isAdminRole != "true")
			{
				if(numOfRoles>1)//DBCORR
				{
					DEBUGL2( "SYSTEM ROLE CANT BE DELETED............\n");
					continue;
				}
				else
				{
					DEBUGL1( "SYSTEM ROLE CANT BE DELETED............\n");
					return STATUS_AL_SEC_BUILTIN_ROLE_CANNOT_DELETE;
				}
			}*/
			// Check for Deletion of Built-in Role(Built-In roles cannot be deleted)
			if(Utils::IsBuiltinRole(roleVector[cnt],searchByName))
			{
				if(numOfRoles>1)//DBCORR
                                {
                                        DEBUGL2( "SYSTEM ROLE CANT BE DELETED............\n");
                                        continue;
                                }
                                else
				{
                                	DEBUGL1( "SYSTEM ROLE CANT BE DELETED............\n");
	                                return STATUS_AL_SEC_BUILTIN_ROLE_CANNOT_DELETE;
				}			
			}
			//Removed code because assigning default role to a user is taken care by ssdk	
			//if there are users then get the num of users
			#if 0
			roleUsers=0;
			roleUsers= static_cast<unsigned int>( SSDKRoleInfo.m_RoleUsers.size());
			bool isRoleCanbeDeleted = true;
			if(roleUsers>0)
			{
				/*char numOfUsers[10];
				memset(numOfUsers,'\0',sizeof(numOfUsers));
				sprintf(numOfUsers,"%d",roleUsers);
				CString tempRole = roleVector[cnt];
				EbxSSDKUtil::CheckXMLEntityAndReplaceString(tempRole);	
				if(multipleRoles) 
					newRole = newRole + "<Role><Information><name>" + tempRole + "</name><userNum>" + numOfUsers + "</userNum></Information></Role>";
				else
				{
					status=userRole.SetUserNum(sUserDataXPath.c_str(), pNewUserDoc,numOfUsers,true);
                			if (OK!=status)
                			{
                        			DEBUGL1( "setting UserNum to UI failed............");
                        			throw CRoleException("STATUS_FAILED");
                			}
					return STATUS_OK;
				}*/
				//For each user of this role check the role property, if any user is having only this role then assign the Default Role [User Role] to that user
				SSDKRBACMgmtInterface::UserInfo SSDKUserInfo;
				for(unsigned int _cnt=0;_cnt<roleUsers;++_cnt)
				{
					SSDKUserInfo=rbac->GetProperty(myut.ut,SSDKRBACMgmtInterface::USER_ROLES_PROPERTY,SSDKRoleInfo.m_RoleUsers[_cnt],ssdkStatus);
					if(ssdkStatus == AUTH_RBAC_DB_CORRUPTED)
					{
						DEBUGL2("SSDK RBAC DB CRRUPTED\n");
						Utils::CheckForCorruption(ssdkStatus);
						return STATUS_AL_SEC_DB_CORRUPTED;
					}
					if(ssdkStatus !=OK)//DBCORR
					{
						DEBUGL1("GetProperty(USER_ROLES_PROPERTY) Status ::%d\n",ssdkStatus);
						isRoleCanbeDeleted = false;
						break;
					}
					if(SSDKUserInfo.m_UserRoles.size() <= 1)
						userList.push_back(SSDKRoleInfo.m_RoleUsers[_cnt]);
				}
				if(!isRoleCanbeDeleted)
					continue;
			}
			#endif
			ssdkStatus=rbac->RBACRoleOperation(myut.ut,SSDKRBACMgmtInterface::ROLE_REMOVE,SSDKRoleInfo);
			if(ssdkStatus == AUTH_RBAC_DB_CORRUPTED)
                        {
                                DEBUGL2("SSDK RBAC DB CRRUPTED\n");
                                Utils::CheckForCorruption(ssdkStatus);
                                return STATUS_AL_SEC_DB_CORRUPTED;
                        }
			DEBUGL3("\nRBACRoleOperation(ROLE_REMOVE) Status :: %d\n",ssdkStatus);
			if(OK!=ssdkStatus)//DBCORR
			{		
		        	DEBUGL2( "ROLE_REMOVE failed\n");
				if(numOfRoles <=1)
				{
					ret =SSDK_RBAC_MANAGER_PLUGIN_ERROR_MAP(ssdkStatus); 
					return ret;	
				}
			}	
			else
			{
				//Write Log success case	
				ci::logmanager::LogInterface::OperationApplicationType oprApplication;
				CString loggedInUser = myut.ut->GetProperty(SSDKUserTokenInterface::USER_ID_PROPERTY, ssdkStatus);
				if(ssdkStatus != OK)
				{
					DEBUGL2("Failed to Get the user name from user token %d\n", ssdkStatus);
				}
				DEBUGL8("The Logged in User is %s\n", loggedInUser.c_str());		
				CString loggedInUserDomainName = myut.ut->GetProperty(SSDKUserTokenInterface::USER_DOMAIN_PROPERTY, ssdkStatus);
                                if(ssdkStatus != OK)
                                {
                                        DEBUGL2("Failed to Get the Domain name from user token %d\n", ssdkStatus);
                                }
                                DEBUGL8("The Logged in User Domain Name is %s\n", loggedInUserDomainName.c_str());
				
				CString ipAddr; 
				boProxy->GetApplicationInformation(oprApplication,ipAddr);
				Utils::WriteLog(NOTIFICATION_DELETE_ROLE,true,ci::logmanager::LogInterface::opDelete,ci::logmanager::LogInterface::otRole,ci::logmanager::LogInterface::mtMaintenance,oprApplication, loggedInUser,SSDKRoleInfo.m_RoleId,ipAddr,loggedInUserDomainName);				

			}
			DEBUGL8("Role deleted succesfully %s \n", SSDKRoleInfo.m_RoleId.c_str());
		}
		/*unsigned int totalUser = userList.size();
		if(totalUser>0)
		{
			SSDKRoleInfo=rbac->GetProperty(myut.ut, SSDKRBACMgmtInterface::ROLE_USERS_PROPERTY,DEFAULT_ROLE_NAME, ssdkStatus);//DBCORR
			if(ssdkStatus == AUTH_RBAC_DB_CORRUPTED)
                        {
                                DEBUGL2("SSDK RBAC DB CRRUPTED\n");
                                Utils::CheckForCorruption(ssdkStatus);
                                return STATUS_AL_SEC_DB_CORRUPTED;
                        }
			for(unsigned _cnt=0;_cnt<totalUser;++_cnt)
				SSDKRoleInfo.m_RoleUsers.push_back(userList[_cnt]);

			ssdk::SSDKStatus ssdkStatus =OK;
			ssdkStatus = rbac->SetProperty(myut.ut, SSDKRBACMgmtInterface::ROLE_USERS_PROPERTY,SSDKRoleInfo);
			if(ssdkStatus != OK)
			{
				DEBUGL3("SetProperty(ROLE_USERS_PROPERTY) Status :: %d\n",ssdkStatus);
				DEBUGL2("Failed to Assigned Default Role to User\n");
			}
		}*/
		/*
		if(multipleRoles)
			newRole.append("</Roles>");
		//Now DeSerialize the newly created xml string in Transaction doc
		if(pNewUserDoc)
		{
			if(multipleRoles)
			{
				ElementRef pParent = pNewUserDoc->getParentNode();	
				if(STATUS_OK != Utils::RemoveChild(pNewUserDoc))
					return STATUS_FAILED;
				if(CRoleManager::m_pHDB->DeserializeFromString(pParent,newRole)!=STATUS_OK)
				{
					DEBUGL1("\n FAILED TO DESERIALIZE THE STRING:: %s \n",newRole.c_str());
					return STATUS_FAILED;
				}	
				pParent = NULL;
			}
		}
		else
		{
			DEBUGL1("\n DOM node is NULL \n");
			return STATUS_FAILED;
		}*/
	}
	catch(...)
	{
		DEBUGL1("Caught Unknown Exception!!");
		return STATUS_AL_SEC_UNKNOWN_ERROR;
	}		
	return STATUS_OK;
}

Status Reset::Execute()
{
	Status ret = STATUS_OK;
	MyUserToken myut;
	BOProxyRef boProxy;
	Ref<SSDKLogInterface> LogExternalPtr=NULL;
	ssdk::SSDKStatus ssdkStatus;
	ret = Authenticate(myut, boProxy, LogExternalPtr);
	if(ret != STATUS_OK)
	{
		DEBUGL2("Reset::Authenticate::Failed \n");
		return ret;
	}
	Ref<SSDKSecurityManagerInterface> ssdkSecurityManagerInterface =ssdk::GetSecurityManager();
	Ref<SSDKRBACMgmtInterface> rbac = dynamic_cast<SSDKRBACMgmtInterface*> (ssdkSecurityManagerInterface->GetInterface  (SSDKSecurityManagerInterface::RBAC_MGMT_INTERFACE,ssdkStatus));
	if(rbac==(void*) NULL || ssdkStatus != OK)
	{
		DEBUGL1("\n ssdk SecurityManager::SSDKRBACMgmtInterface Failed \n");
		return STATUS_AL_SEC_INTERNAL_ERROR;
	}
	vector<SSDKRBACMgmtInterface::RoleInfo> roles;
	vector<SSDKRBACMgmtInterface::RoleInfo>::iterator it;
	ssdkStatus=rbac->GetProperty(myut.ut,SSDKRBACMgmtInterface::ROLE_ID_PROPERTY,STRING_NULL,roles);
	DEBUGL8("GetProperty(ROLE_ID_PROPERTY) Status ::%d\n",ssdkStatus);
	if(OK != ssdkStatus)
	{
		ret=SSDK_RBAC_MANAGER_PLUGIN_ERROR_MAP(ssdkStatus);
		return ret;
	}
	vector<CString> roleID;
	for(it=roles.begin();it!=roles.end();it++)
		roleID.push_back((*it).m_RoleId);
	ret = ExecuteSSDK("",NULL,roleID,true,true) ;
	if(STATUS_OK != ret)
	{
		DEBUGL1("\n Reset::Execute SSDK failed\n");
		return ret;
	}
	//Reset Guest User Permissions:
	//Added After ExecuteSSDK because ExecuteSSDK is member of DeleteRole Class
	SSDKRBACMgmtInterface::RoleInfo SSDKRoleInfo;
	//Please note "Guest" is used because in SSDK call this is hardcoded and is case sensitive
	SSDKRoleInfo.m_RoleId = "Guest";
	SSDKRoleInfo.m_RolePermissions.clear();
	SSDKRoleInfo.m_RolePermissions.insert(pair<CString,CString>(OBJ_DEFAULT,OP_DEFAULT));
	ssdkStatus = rbac->SetProperty(myut.ut,SSDKRBACMgmtInterface::ROLE_PERMISSIONS_PROPERTY,SSDKRoleInfo);
	if(OK != ssdkStatus)
	{
		DEBUGL1("\n Reset Guest User permissions Failed :  %d\n",ssdkStatus);
		return SSDK_RBAC_MANAGER_PLUGIN_ERROR_MAP(ssdkStatus);
		
	}
	return ResetRuleMapping(myut.ut);
}

Status Reset::ResetRuleMapping(const SSDKUserTokenInterface* userToken)
{
	//Check ImportRoleMapping file exists
        RoleRuleMap roleNameAttrVectorFromFile;
	CString otherUser;
        RoleRuleMap::iterator roleMapIterator;
	Status ret = STATUS_FAILED;
        if(File::Exists(currentRoleSetting))
        {
                //Parse the xml file
                ret = Utils::ParseRoleMap(currentRoleSetting,roleNameAttrVectorFromFile,otherUser);
		if(ret != STATUS_OK)
                {
                        DEBUGL1("Parsing of xml failed:%s\n",currentRoleSetting.c_str());
                        return ret;
                }
                //Remove the entry if any role other than built-in role is there.
                RoleRuleMap roleNameAttrVectorBuiltinRoles;
                for(roleMapIterator=roleNameAttrVectorFromFile.begin();roleMapIterator!=roleNameAttrVectorFromFile.end();roleMapIterator++)
                {
                        if(Utils::IsBuiltinRole(roleMapIterator -> first))
                        {
				roleNameAttrVectorBuiltinRoles.insert(RoleRuleMap::value_type(roleMapIterator -> first,vector<CString>()));
                        }
			//Set the rules to SSDK
			ret = SetRoleRule(userToken,roleNameAttrVectorBuiltinRoles);
			if(ret != STATUS_OK)
			{
				DEBUGL1("\n Reset::Set Rules to SSDK failed\n");
				return ret;
			}
                }
		//Delete the imported file
		if(STATUS_OK != File::DeleteFile(currentRoleSetting))
		{		
			DEBUGL1("Reset::Imported rule file deletion failed\n");	
			return STATUS_AL_SEC_INTERNAL_ERROR;
		}	
        }
	return STATUS_OK;
}

Status TransferToRole::Execute()
{
	ElementRef pParamNode = NULL;
	ElementRef pNewUserData = NULL; 
	CString sUserDataXPath = STRING_NULL;
	Status ret = STATUS_OK;
	pParamNode = m_command->GetParamNode(ROLE_DETAILS);
	DocumentRef pNewUserDoc;	
	if(pParamNode)
	{
		//convert from string xpath to element ref
		ret = ProcessTransDoc("TransferToRole",sUserDataXPath,pParamNode,pNewUserDoc);	
		if (ret != STATUS_OK)
		{
			DEBUGL1("In TransferToRole::Execute:: Failed \n");
			ret = STATUS_FAILED;
			return ret;
		}
		pNewUserData = CRoleManager::m_pHDB->BindToElement(pNewUserDoc,sUserDataXPath.c_str(),true);
		if (!pNewUserData)                      
		{
			DEBUGL2("TransferToRole::Execute::Failed To Bind To Information Node (%s) Passed by Client Document\n",sUserDataXPath.c_str());
        		return STATUS_FAILED;
		}
		
	//ret = ExecuteGetRoleName(pNewUserData,sUserDataXPath);	
	ret = ExecuteIndividualRole(pNewUserData, sUserDataXPath);
	}
	else
	{
		DEBUGL1("In TransferToRole::Execute:: Failed \n");
		 ret = STATUS_FAILED;
	}

	return ret;	
}
Status TransferToRole::ExecuteIndividualRole(NodeRef pNewUserData, CString sUserDataXPath)
{
        Status ret = STATUS_OK;
        DEBUGL8("IN TransferToRole::ExecuteIndividualRole ....%s\n",pNewUserData->getNodeName().c_str());
        if(pNewUserData->getNodeName() == "Roles")
        {
                ci::operatingenvironment::Ref<NodeList> paramNodes = pNewUserData->getChildNodes();
                DEBUGL8("TOTAL_LENGTH::%d\n",paramNodes->getLength());
                if(paramNodes)
                {
                        for(unsigned int i=0; i<paramNodes->getLength(); i++)
                        {
                                if(TransferIndividualRole(paramNodes->item(static_cast<unsigned long>(i)), paramNodes->item(static_cast<unsigned long>(i))->getNodeValue()) != STATUS_OK)
                                {
                                        DEBUGL1("In TransferToRole::ExecuteIndividualRole::Transfer of role failed\n");
                                }
                        }
                }
                else
                {
                        ret = STATUS_FAILED;
                        return ret;
                }
        }
        else
        {
                if(TransferIndividualRole(pNewUserData, sUserDataXPath) != STATUS_OK)
                {
                        ret = STATUS_FAILED;
                        DEBUGL1("Transfer of Role failed\n");
                        return ret;
                }
        }
        return ret;
}
Status TransferToRole::TransferIndividualRole(ElementRef pNewUserData , CString sUserDataXPath)
{
	Status ret = STATUS_OK;
	Role role;
	CString roleName = role.GetName(sUserDataXPath,pNewUserData);
	CString transferRoleName = role.GetTransferToRole(sUserDataXPath,pNewUserData,false);
	if(roleName.empty() || transferRoleName.empty())
	{
		DEBUGL1("\n RoleName or Transfer role name is missing \n");
		return STATUS_FAILED;
	}
	ret = ExecuteSSDK(sUserDataXPath.c_str(), pNewUserData, roleName ,transferRoleName);
	return ret;	
}
/*
Status TransferToRole::ExecuteGetRoleName(ElementRef pNewUserData , CString sUserDataXPath)
{
	Status ret = STATUS_OK;
	CString xml,bindPath;
	DEBUGL5("GETNODENAME::%s\n",pNewUserData->getNodeName().c_str());	
	bool create = false;
	Role role;
	CString roleList,transferToRole;
	roleList=role.GetInhRoles(sUserDataXPath, pNewUserData, create);
	if(roleList.empty())
	{
		DEBUGL2("Role Name is Empty \n");
		return STATUS_FAILED;
	}
	transferToRole=role.GetTransferToRole( sUserDataXPath, pNewUserData, create);
	if(transferToRole.empty())
	{
		DEBUGL2("Role Name is Empty \n");
		return STATUS_FAILED;
	}
	ret = ExecuteSSDK(sUserDataXPath.c_str(), pNewUserData, roleList,transferToRole);
	return ret;
}
*/

Status TransferToRole::ExecuteSSDK(ci::operatingenvironment::CString sUserDataXPath, NodeRef pNewUserDoc, CString roleName,CString tName)
{
	MyUserToken myut;
	BOProxyRef boProxy;
	Ref<SSDKLogInterface> LogExternalPtr=NULL;
	ssdk::SSDKStatus ssdkStatus;
	//Authenticate the user
	if(Authenticate(myut, boProxy, LogExternalPtr) == STATUS_FAILED)
        {
           DEBUGL2("GetUsers::Authenticate::Failed \n");
           return STATUS_FAILED;
        }
	 Ref<SSDKSecurityManagerInterface> ssdkSecurityManagerInterface =ssdk::GetSecurityManager();
	 Ref<SSDKRBACMgmtInterface> rbac = dynamic_cast<SSDKRBACMgmtInterface*> (ssdkSecurityManagerInterface->GetInterface  (SSDKSecurityManagerInterface::RBAC_MGMT_INTERFACE,ssdkStatus));
	if(rbac==(void*) NULL || ssdkStatus != OK)
        {
                DEBUGL1("\n ssdk SecurityManager::SSDKRBACMgmtInterface Failed \n");
                return STATUS_FAILED;
        }
	
	CString isAdminRole =STRING_NULL;
	//vector <CString> roleVector;
	//Extract the roles from the string 
	//ExtractToken(rList, DELIM, roleVector);
	bool check =false;
	unsigned int i=0,j=0,k=0;
	try
	{
	
		SSDKRBACMgmtInterface::RoleInfo deleteRoleInfo,transferRoleInfo;
		vector<CString> users;
		//Get the information for that roleid
		//for ( cnt=0;cnt<roleVector.size();cnt++)
		//{
		deleteRoleInfo=rbac->GetProperty(myut.ut, SSDKRBACMgmtInterface::ROLE_ALL_PROPERTIES,roleName, ssdkStatus);
		if(ssdkStatus == AUTH_RBAC_DB_CORRUPTED)
		{
			DEBUGL2("SSDK RBAC DB CRRUPTED\n");
			Utils::CheckForCorruption(ssdkStatus);
			return STATUS_AL_SEC_DB_CORRUPTED;
		}

		if(OK!=ssdkStatus)//DBCORR
		{		
	        	DEBUGL1( "GetProperty() failed...%d",ssdkStatus);
	       		throw CRoleException("STATUS_FAILED");
		}
			
		isAdminRole = rbac->GetProperty(myut.ut,SSDKRBACMgmtInterface::RBAC_ROLE_OBJECT,roleName,IS_ADMIN_ROLE,ssdkStatus);
		if(ssdkStatus == AUTH_RBAC_DB_CORRUPTED)
		{
			DEBUGL2("SSDK RBAC DB CRRUPTED\n");
			Utils::CheckForCorruption(ssdkStatus);
			return STATUS_AL_SEC_DB_CORRUPTED;
		}

		if(isAdminRole != "true")//DBCORR
		{
			DEBUGL1( "SYSTEM ROLE CANT BE DELETED............");
	        	throw CRoleException("STATUS_FAILED");
		}
		//Get all the users
		for (i =0; i <deleteRoleInfo.m_RoleUsers.size();i++)
		{
			check = false;
			for ( j= 0; j<users.size();j++)
			{
				if (users[j]==deleteRoleInfo.m_RoleUsers[i])
				{
					check = true;
					break;
				}
			}
			if (false==check)
				users.push_back(deleteRoleInfo.m_RoleUsers[i]);
		}
		
		//}
		//Only for testing
		for ( k= 0; k<users.size();k++)
			DEBUGL8("Users = %s \n",users[k].c_str());

		
		//Transfer the users to tRole
		if(users.size()>0)
		{
			transferRoleInfo=rbac->GetProperty(myut.ut, SSDKRBACMgmtInterface::ROLE_ALL_PROPERTIES,tName, ssdkStatus);
			if(ssdkStatus == AUTH_RBAC_DB_CORRUPTED)
                        {
                                DEBUGL2("SSDK RBAC DB CRRUPTED\n");
                                Utils::CheckForCorruption(ssdkStatus);
                                return STATUS_AL_SEC_DB_CORRUPTED;
                        }
			if(OK!=ssdkStatus)//DBCORR
			{		
		        	DEBUGL1( "GetProperty() failed");
		               	throw CRoleException("STATUS_FAILED");
			}

		 	if(transferRoleInfo.m_RoleUsers.size()==0)
			 	for(unsigned int count =0 ; count < users.size();count++)
			 		transferRoleInfo.m_RoleUsers.push_back(users[count]);
			else
			{
				for(unsigned int i =0;i< users.size();i++)
				{
					check =false;
					for (unsigned int j= 0; j<transferRoleInfo.m_RoleUsers.size();j++)
					{
						if (transferRoleInfo.m_RoleUsers[j]==users[i])
						{
							check = true;
							break;
						}
					}
					if (false==check)
						transferRoleInfo.m_RoleUsers.push_back(users[i]);

				}
			}
			//Only for testing
			for ( k= 0; k<transferRoleInfo.m_RoleUsers.size();k++)
				DEBUGL8("Transfer Users = %s \n",transferRoleInfo.m_RoleUsers[k].c_str());

		}	

		//Transfer the users
		DEBUGL8("\n ROLE_ID_TO_BE_TRANSFERED :: %s\n",transferRoleInfo.m_RoleId.c_str());
		ssdkStatus=rbac->SetProperty(myut.ut,SSDKRBACMgmtInterface::ROLE_ALL_PROPERTIES,transferRoleInfo);
		if(ssdkStatus == AUTH_RBAC_DB_CORRUPTED)
		{
			DEBUGL2("SSDK RBAC DB CRRUPTED\n");
			Utils::CheckForCorruption(ssdkStatus);
			return STATUS_AL_SEC_DB_CORRUPTED;
		}
		if(OK!=ssdkStatus)//DBCORR
		{
			DEBUGL1("SETTING  ROLE TYPE FAILED   %d\n",ssdkStatus);
			throw CRoleException("STATUS_FAILED");
		}
		
			
		 // delete the roles
		//for (cnt=0;cnt<roleVector.size();cnt++)
		//{
		deleteRoleInfo=rbac->GetProperty(myut.ut, SSDKRBACMgmtInterface::ROLE_ALL_PROPERTIES,roleName, ssdkStatus);
		if(ssdkStatus == AUTH_RBAC_DB_CORRUPTED)
		{
			DEBUGL2("SSDK RBAC DB CRRUPTED\n");
			Utils::CheckForCorruption(ssdkStatus);
			return STATUS_AL_SEC_DB_CORRUPTED;
		}
		if(OK!=ssdkStatus)//DBCORR
		{		
	        	DEBUGL1( "GetProperty(ROLE_ALL_PROPERTIES) failed...%d",ssdkStatus);
	       		throw CRoleException("STATUS_FAILED");
		}		
		ssdkStatus=rbac->RBACRoleOperation(myut.ut,SSDKRBACMgmtInterface::ROLE_REMOVE,deleteRoleInfo);
		if(ssdkStatus == AUTH_RBAC_DB_CORRUPTED)
		{
			DEBUGL2("SSDK RBAC DB CRRUPTED\n");
			Utils::CheckForCorruption(ssdkStatus);
			return STATUS_AL_SEC_DB_CORRUPTED;
		}
		if(OK!=ssdkStatus)//DBCORR
		{		
	        	DEBUGL1( "\nRBACRoleOperation(ROLE_REMOVE) failed status :: %d\n",ssdkStatus);
	                throw CRoleException("STATUS_FAILED");
		}	
		//}	                   
	}
	catch(...)
	{
		DEBUGL1("Some Exceptions from ssdk GetProperty() call");
		return STATUS_FAILED;
	}		
	return STATUS_OK;
}
Status ModifyRole::Execute()
{
	ElementRef pParamNode = NULL;
	ElementRef pNewUserData = NULL; 
	CString sUserDataXPath = STRING_NULL;
	Status ret = STATUS_OK;
	bool searchByName = true;
        pParamNode = m_command->GetParamNode(SEARCH_BY.c_str());
        NodeRef pParamSearchByText;
        if(pParamNode)
        {
                pParamSearchByText = pParamNode->getFirstChild();
                CString searchBy;
                if(pParamSearchByText)
                        searchBy = pParamSearchByText->getNodeValue();
                if(searchBy == "ID")
                        searchByName = false;
                else if(searchBy == "NAME")
                        searchByName = true;

                DEBUGL8("Search By Name:%s\n",searchBy.c_str());
        }

	pParamNode = m_command->GetParamNode(ROLE_DETAILS);
	DocumentRef pNewUserDoc;
	if(pParamNode)
	{
		//convert from string xpath to element ref
		ret = ProcessTransDoc("ModifyRole",sUserDataXPath,pParamNode,pNewUserDoc);	
		if (ret != STATUS_OK)
		{
			DEBUGL1("In ModifyRole::Execute:: Failed \n");
			ret = STATUS_AL_SEC_COMMAND_SCHEMA;
			return ret;
		}
		DocumentGuard domGuard(pNewUserDoc);
		if(STATUS_OK != domGuard.ReadLock(false))
                {
                        DEBUGL1("Failed to Acquire Read Lock on Rbac Session DOM\n");
                        return STATUS_FAILED;
                }
		pNewUserData = CRoleManager::m_pHDB->BindToElement(pNewUserDoc,sUserDataXPath.c_str(),true);
		if (!pNewUserData)                      
		{
			DEBUGL2("ModifyRole::Execute::ProcessTransDoc::Failed To Bind To Information Node (%s) Passed by Client Document\n",sUserDataXPath.c_str());
        		return STATUS_AL_SEC_COMMAND_SCHEMA;
		}
		ret = ExecuteSSDK(sUserDataXPath,pNewUserData,searchByName);	
	}
	else
	{
		DEBUGL1("In ModifyRoles::Execute:: Failed \n");
		ret = STATUS_AL_SEC_INTERNAL_ERROR;
	}
	return ret;	
}

Status ModifyRole::ExecuteSSDK(ci::operatingenvironment::CString sUserDataXPath, NodeRef pNewUserDoc,bool searchByName)
{
	MyUserToken myut;
	BOProxyRef boProxy;
	Ref<SSDKLogInterface> LogExternalPtr=NULL;
	ssdk::SSDKStatus ssdkStatus;
	Status ret=STATUS_FAILED;
	ci::operatingenvironment::Ref<dom::Element> pNewUserEle=NULL;
	//Authenticate the user
	ret =Authenticate(myut, boProxy, LogExternalPtr);
	if(ret != STATUS_OK)
        {
           DEBUGL2("Modify Role ::Authenticate::Failed \n");
           //throw CRoleException(STATUS_FAILED);
           return ret;
        }
	 Ref<SSDKSecurityManagerInterface> ssdkSecurityManagerInterface =ssdk::GetSecurityManager();
	 Ref<SSDKRBACMgmtInterface> rbac = dynamic_cast<SSDKRBACMgmtInterface*> (ssdkSecurityManagerInterface->GetInterface  (SSDKSecurityManagerInterface::RBAC_MGMT_INTERFACE,ssdkStatus));
	if(rbac==(void*) NULL || ssdkStatus != OK)
        {
                DEBUGL1("\n ssdk SecurityManager::SSDKRBACMgmtInterface Failed \n");
                return STATUS_AL_SEC_INTERNAL_ERROR;
        }
	
	try
	{
	bool create = false;
	Role userRole; //to get information from UI
	CString roleName,status;
	CString InheritRoles;
	CString ldapRule,userList,ldapEnable,groupList;
	//CString creatMode;
	set< pair <CString,CString> > objSet;
	set< pair <CString,CString> > ::iterator it;
	SSDKRBACMgmtInterface::RoleInfo SSDKRoleInfo;	

	
	//Get the Role name from UI
	roleName = userRole.GetName( sUserDataXPath,  pNewUserDoc, create);
	if(roleName.empty() && searchByName)
	{
		DEBUGL1("Empty Role Name\n");
		return STATUS_AL_SEC_INVALID_INPUT;
	}
	if(!Utils::CheckForNameValidity(roleName))
	{
		DEBUGL1("Role name invalid\n");
		return STATUS_AL_SEC_ROLENAME_INVALID_INPUT;
	}
	//The below function considers multibyte characters also , during length check.
	if(Utils::IsMaxLengthExceeded(roleName,MAX_ROLE_NAME_SIZE))
	{
		DEBUGL1("Rolename length has exceeded the maximum length\n");
		return STATUS_AL_SEC_OBJECT_MAX_LENGTH_EXCEEDED;
	}

	CString roleUniqueId;
	roleUniqueId = userRole.GetUniqueID(sUserDataXPath, pNewUserDoc,create);
	if(roleUniqueId != "")
	{
		int iOut;
	        if(!EbxSSDKUtil::validateInput<int>(roleUniqueId,iOut))
        	{
               		DEBUGL1("Invalid Role UniqueId \n");
                	return STATUS_AL_SEC_INVALID_INPUT;
        	}
	}
	if(roleUniqueId.empty() && !searchByName)
        {
                DEBUGL1("Empty Role Unique iD\n");
                return STATUS_AL_SEC_INVALID_INPUT;
        }

	bool isOldNameExits = false;
	CString oldRoleName = userRole.GetOldName( sUserDataXPath,  pNewUserDoc,isOldNameExits);
	if(isOldNameExits && oldRoleName.empty() && searchByName)
	{
		
		DEBUGL1("Old name is Empty\n");
		return STATUS_AL_SEC_INVALID_INPUT;
	}
	//Get the old role name from SSDK if oldRoleName is not specified in DOM while Modify request comes on ID
	CString oldRoleNameInDB;
	if(!searchByName)
	{
		SSDKRoleInfo=rbac->GetProperty(myut.ut,SSDKRBACMgmtInterface::ROLE_ID_PROPERTY,SSDKRBACMgmtInterface::ROLE_UNIQUE_ID_PROPERTY,roleUniqueId,ssdkStatus);
                if(ssdkStatus != OK)
                {
                        DEBUGL1("Failed to get Role Name for role unique Id :: %d\n",roleUniqueId.c_str());
                        ret = SSDK_RBAC_MANAGER_PLUGIN_ERROR_MAP(ssdkStatus);
                        return ret;
                }
		oldRoleNameInDB =  SSDKRoleInfo.m_RoleId;
	}
	if(isOldNameExits && !oldRoleName.empty() && !searchByName)
	{
		if(oldRoleName != oldRoleNameInDB)
		{
			DEBUGL1("Specified old role name is not matched for role id :: %s\n",roleUniqueId.c_str());
			return STATUS_AL_SEC_INVALID_INPUT; 
		}
	}
	if(isOldNameExits && oldRoleName.empty() && !searchByName)
	{
		oldRoleName =  oldRoleNameInDB;
	}
	DEBUGL8("IsOldRoleName Tag exists in DOM :: %d\n",isOldNameExits);
	DEBUGL8("Search By Name :: %d\n",searchByName);
	DEBUGL8("Role Unique ID :: %s\n",roleUniqueId.c_str());
	DEBUGL8("Role name :: %s\n",roleName.c_str());
	DEBUGL8("Role old name :; %s\n",oldRoleName.c_str());

	
	CString roleToQuery(roleName);
        SSDKRBACMgmtInterface::RolePropertyName roleProperty = SSDKRBACMgmtInterface::ROLE_ID_PROPERTY;

	if(isOldNameExits && searchByName)
		roleToQuery = oldRoleName;

        if(!roleUniqueId.empty() && !searchByName)
        {
                roleToQuery = roleUniqueId;
                roleProperty = SSDKRBACMgmtInterface::ROLE_UNIQUE_ID_PROPERTY;
        }
		
        SSDKRoleInfo=rbac->GetProperty(myut.ut,SSDKRBACMgmtInterface::ROLE_ALL_PROPERTIES,roleProperty,roleToQuery,ssdkStatus);
        if(OK!=ssdkStatus)
        {
                DEBUGL1("Getting  ROLE TYPE FAILED   %d\n",ssdkStatus);
                ret = SSDK_RBAC_MANAGER_PLUGIN_ERROR_MAP(ssdkStatus);
                return ret;
        }

	map<CString,bool> permissionEnableDisableMap; 
        ret = ReadPermissions(pNewUserDoc,sUserDataXPath,permissionEnableDisableMap,true,false); 
        if(ret != STATUS_OK) 
        { 
                DEBUGL1("Invalid Permissions List\n"); 
                return ret; 
        } 

	map<CString,bool>::iterator iter;
	//unassign automatically few restricted permision name
	UnassignPermission(permissionEnableDisableMap);
        for(iter=permissionEnableDisableMap.begin();iter!=permissionEnableDisableMap.end();++iter)
        {
                ret=userRole.SetEnableDisablePermission(SSDKRoleInfo.m_RolePermissions,iter->first,iter->second);
                if(ret!=STATUS_OK)
                {
                        DEBUGL2("Failed to set permission :: %s\n",(iter->first).c_str());
                        return ret;
                }
        }

        set<pair<CString,CString> >::iterator dispIter;
        for(dispIter = SSDKRoleInfo.m_RolePermissions.begin();dispIter != SSDKRoleInfo.m_RolePermissions.end();dispIter++)
        {
                DEBUGL8("Object:%s Permission:%s\n",(*dispIter).first.c_str(),(*dispIter).second.c_str());
        }

        CString tempName = roleName;
        std::transform(tempName.begin(),tempName.end(),tempName.begin(), (int(*)(int)) tolower);

        if(((!roleName.empty()) && (tempName != GUEST_ROLE_NAME)) || ((!roleUniqueId.empty()) && (roleUniqueId != GUEST_ROLE_UNIQUE_ID)))
        {
                if(SSDKRoleInfo.m_RolePermissions.size()==1)
                {
                        DEBUGL1("\nRole has no permission \n");
                        return STATUS_AL_SEC_EMPTY_PERMISSION_LIST;
                }
        }

	if(isOldNameExits && (roleName!=oldRoleName))
	{
		//Change The Role Name
		SSDKRBACMgmtInterface::RoleInfo newRoleInfo;
		newRoleInfo.m_NewRoleId = roleName;
		newRoleInfo.m_RoleId = oldRoleName;
		ssdkStatus = rbac->RBACRoleOperation(myut.ut,SSDKRBACMgmtInterface::ROLE_CHANGE_NAME,newRoleInfo);
		DEBUGL4("RBACRoleOperation(ROLE_CHANGE_NAME) Status :: %d\n",ssdkStatus);
		if(ssdkStatus !=OK)

		{
			DEBUGL1("Failed to Rename the role\n");
			ret = SSDK_RBAC_MANAGER_PLUGIN_ERROR_MAP(ssdkStatus);
			return ret;	
		}		
		//Set rolerule property to roles whose name is present in rolemapping file when role name is modified.
		//Clearing the old rules for the role.
		SSDKRoleInfo.m_RoleRules.clear();
		UpdateRoleRule(roleName,SSDKRoleInfo);
	}

	/*CString roleToQuery(roleName);
	SSDKRBACMgmtInterface::RolePropertyName roleProperty = SSDKRBACMgmtInterface::ROLE_ID_PROPERTY;
	if(!roleUniqueId.empty())
	{
		roleToQuery = roleUniqueId;
		roleProperty = SSDKRBACMgmtInterface::ROLE_UNIQUE_ID_PROPERTY;
	}
	SSDKRoleInfo=rbac->GetProperty(myut.ut,SSDKRBACMgmtInterface::ROLE_ALL_PROPERTIES,roleProperty,roleToQuery,ssdkStatus);
	if(OK!=ssdkStatus)
	{
		DEBUGL1("Getting  ROLE TYPE FAILED   %d\n",ssdkStatus);
		ret = SSDK_RBAC_MANAGER_PLUGIN_ERROR_MAP(ssdkStatus);
		return ret;
	}*/
	/*CString roleNameFromSSDK = SSDKRoleInfo.m_RoleId;
	if(!searchByName && (roleNameFromSSDK != roleName) )
	{
		//Change The Role Name
		if(oldRoleName == "-1")
	        {
                	DEBUGL4("Old name is Empty\n");
        
       		        SSDKRBACMgmtInterface::RoleInfo newRoleInfo;
                	newRoleInfo.m_NewRoleId = roleName;
                	newRoleInfo.m_RoleId = SSDKRoleInfo.m_RoleId;
                	ssdkStatus = rbac->RBACRoleOperation(myut.ut,SSDKRBACMgmtInterface::ROLE_CHANGE_NAME,newRoleInfo);
                	if(ssdkStatus !=OK)
                	{
                        	DEBUGL1("Failed to Rename the role\n");
                        	ret = SSDK_RBAC_MANAGER_PLUGIN_ERROR_MAP(ssdkStatus);
                        	return ret;
                	}
		}
	}*/

	/*map<CString,bool> permissionEnableDisableMap;
	ret = ReadPermissions(pNewUserDoc,sUserDataXPath,permissionEnableDisableMap,true,false);
	if(ret != STATUS_OK)
	{
		DEBUGL1("Invalid Permissions List\n");
		return ret;
	}*/

	/*map<CString,bool>::iterator iter;
	//unassign automatically few restricted permision name
	UnassignPermission(permissionEnableDisableMap);	
	for(iter=permissionEnableDisableMap.begin();iter!=permissionEnableDisableMap.end();++iter)
	{
		ret=userRole.SetEnableDisablePermission(SSDKRoleInfo.m_RolePermissions,iter->first,iter->second);
		if(ret!=STATUS_OK)
		{
			DEBUGL2("Failed to set permission :: %s\n",(iter->first).c_str());
			return ret;
		}
	}

	set<pair<CString,CString> >::iterator dispIter;
	for(dispIter = SSDKRoleInfo.m_RolePermissions.begin();dispIter != SSDKRoleInfo.m_RolePermissions.end();dispIter++)
	{
		DEBUGL8("Object:%s Permission:%s\n",(*dispIter).first.c_str(),(*dispIter).second.c_str());
	}	

	CString tempName = roleName;
	std::transform(tempName.begin(),tempName.end(),tempName.begin(), (int(*)(int)) tolower);

	if(((!roleName.empty()) && (tempName != GUEST_ROLE_NAME)) || ((!roleUniqueId.empty()) && (roleUniqueId != GUEST_ROLE_UNIQUE_ID)))
	{
		if(SSDKRoleInfo.m_RolePermissions.size()==1)
        	{
        		DEBUGL1("\nRole has no permission \n");
                	return STATUS_AL_SEC_EMPTY_PERMISSION_LIST;
        	}
	}*/
	vector <CString > tempVector;
	//Get the name for role name 
	if(!roleName.empty())
		SSDKRoleInfo.m_RoleId = roleName;
	if(!roleUniqueId.empty())
		SSDKRoleInfo.m_RoleUniqueID = roleUniqueId;
	ssdkStatus=rbac->SetProperty(myut.ut,SSDKRBACMgmtInterface::ROLE_ALL_PROPERTIES,SSDKRoleInfo);
	if(OK!=ssdkStatus)
	{
		DEBUGL1("SETTING  ROLE TYPE FAILED   %d\n",ssdkStatus);
		ret = SSDK_RBAC_MANAGER_PLUGIN_ERROR_MAP(ssdkStatus);
		return ret;	
	}
	//Get Role Custom Property and Bind to Information/CustomProperties
	CString cusPropXml =    "Information/CustomProperties/AddViewProperty";
	ElementRef pNewEle= NULL;
	NodeRef RoleRef =pNewUserDoc;
	pNewEle=CRoleManager::m_pHDB->BindToElement(RoleRef, cusPropXml);
	if(pNewEle)
	{
		CString nodeText = pNewEle->getParentNode()->getNodeName();
		DEBUGL8("NODE_TEXT::%s\n",nodeText.c_str());
		//Traverse each child nodes
		Ref<NodeList> elems = pNewEle->getChildNodes();
		unsigned int len = elems->getLength();
		for(unsigned int _cnt=0;_cnt<len;_cnt++)
		{
			Ref<Element> node=elems->item(static_cast<unsigned long>(_cnt));
			CString propName = userRole.GetRoleCusPropName(sUserDataXPath,node);
			CString propValue = userRole.GetRoleCusPropValue(sUserDataXPath,node);
			ssdkStatus= rbac->SetProperty(myut.ut,SSDKRBACMgmtInterface::RBAC_ROLE_OBJECT,roleName,propName,propValue);
			DEBUGL8("\nSSDK SetProperty(RBAC_USER_OBJECT) status is :: %d\n",ssdkStatus);
			node = NULL;
		}	
	}

	//Bind to read property name to be removed
	CString cusPropRemXml = "Information/CustomProperties/RemoveProperty";
	ElementRef pNewEleRem = NULL;
	pNewEleRem=CRoleManager::m_pHDB->BindToElement(RoleRef, cusPropRemXml);
	if(pNewEleRem)
	{
		CString nodeTextRem = pNewEleRem->getParentNode()->getNodeName();
		DEBUGL8("NODE_TEXT::%s\n",nodeTextRem.c_str());
		Ref<NodeList> elemsRem = pNewEleRem->getChildNodes();
		unsigned int len2 = elemsRem->getLength();
		for(unsigned int _cnt=0;_cnt<len2;_cnt++)
		{
			Ref<Element> node=elemsRem->item(static_cast<unsigned long>(_cnt));
			CString propName = userRole.GetRoleCusPropName(sUserDataXPath,node);
			DEBUGL8("Property Name to be removed :: %s\n",propName.c_str());
			//Currently SSDK does not support for removing of custom property for a object, once ssdk supports, plug-in code will be modify accordingly
			node = NULL;
		}
	}
	ci::logmanager::LogInterface::OperationApplicationType oprApplication;
	CString loggedInUser = myut.ut->GetProperty(SSDKUserTokenInterface::USER_ID_PROPERTY, ssdkStatus);
	if(ssdkStatus != OK)
	{
		DEBUGL2("Failed to Get the user name from user token %d\n", ssdkStatus);
	}
	DEBUGL8("The Logged in User is %s\n", loggedInUser.c_str());		
	CString loggedInUserDomainName = myut.ut->GetProperty(SSDKUserTokenInterface::USER_DOMAIN_PROPERTY, ssdkStatus);
        if(ssdkStatus != OK)
        {
                DEBUGL2("Failed to Get the Domain name from user token %d\n", ssdkStatus);
        }
        DEBUGL8("The Logged in UserDomain name is %s\n", loggedInUserDomainName.c_str());
	
	CString ipAddr; 
	boProxy->GetApplicationInformation(oprApplication,ipAddr);
	Utils::WriteLog(NOTIFICATION_UPDATE_ROLE,true,ci::logmanager::LogInterface::opEdit,ci::logmanager::LogInterface::otRole,ci::logmanager::LogInterface::mtMaintenance,oprApplication, loggedInUser,roleName,ipAddr,loggedInUserDomainName);
	}
	catch(DOMException& domExcept)
        {
                DEBUGL1("\n Caught DOMException !! \n");
                return STATUS_FAILED;
        }
	catch(...)
	{
		DEBUGL1("Caught UnKnown Exceptions\n");
		return STATUS_AL_SEC_UNKNOWN_ERROR;
	}		
	return STATUS_OK;
}




Status GetRoleInfo::Execute()
{
	ElementRef pParamNode = NULL;
	ElementRef pNewUserData = NULL; 
	CString sUserDataXPath = STRING_NULL;
	Status ret = STATUS_OK;
	bool searchByName = true;
	pParamNode = m_command->GetParamNode(SEARCH_BY.c_str());
	NodeRef pParamSearchByText;
	if(pParamNode)
	{
		pParamSearchByText = pParamNode->getFirstChild();
		CString searchBy;
		if(pParamSearchByText)
			searchBy = pParamSearchByText->getNodeValue();
		if(searchBy == "ID")
			searchByName = false;
		else if(searchBy == "NAME")
			searchByName = true;

		DEBUGL8("Search By Name:%s\n",searchBy.c_str());
		/*
		else if("NAME" == pParamNode->getNodeName())
                        searchByName = true;*/
	}
	
	pParamNode = m_command->GetParamNode(ROLE_DETAILS);
	DocumentRef pNewUserDoc;
	if(pParamNode)
	{
		//convert from string xpath to element ref
		
		ret = ProcessTransDoc("GetRoleInfo",sUserDataXPath,pParamNode,pNewUserDoc);	
		if ( STATUS_OK == ret)
		{
			DocumentGuard domGuard(pNewUserDoc);
			if(STATUS_OK != domGuard.WriteLock(false))
                	{
                        	DEBUGL1("Failed to Acquire Write Lock on Rbac Session DOM\n");
                        	return STATUS_FAILED;
                	}
			pNewUserData = CRoleManager::m_pHDB->BindToElement(pNewUserDoc,sUserDataXPath.c_str(),true);
			if (!pNewUserData)                      
			{
				DEBUGL2("GetRoleInfo::Execute::Failed To Bind To Information Node (%s) Passed by Client Document\n",sUserDataXPath.c_str());
        			return STATUS_FAILED;
			}
			ret = ExecuteGetRoleName(pNewUserData,sUserDataXPath,searchByName);
		}
		else
		{
			DEBUGL1("In GetRoleInfo::Execute:: Failed \n");
			return STATUS_FAILED;
		}
	}
	else
	{
		DEBUGL1("In GetRoleInfo::Execute:: Failed \n");
		ret = STATUS_FAILED;
	}
	return ret;	
}
Status GetRoleInfo::ExecuteGetRoleName(ElementRef pNewUserData , CString sUserDataXPath,bool searchByName)
{
	Status ret = STATUS_OK;
	CString xml,bindPath;
	DEBUGL8("GETNODENAME::%s\n",pNewUserData->getNodeName().c_str());	
	bool create = false;
	Role role;
	CString roleName,roleUniqueId;
	roleName=role.GetName(sUserDataXPath, pNewUserData, create);
	DEBUGL8("ROLE_NAME_IS :: %s\n",roleName.c_str());
	roleUniqueId = role.GetUniqueID(sUserDataXPath,pNewUserData,create);
	DEBUGL8("ROLE_UNIQUE_ID IS :: %s\n",roleUniqueId.c_str());
	if(roleName.empty() && searchByName)
	{
		DEBUGL2("Role Name is Empty \n");
		return STATUS_FAILED;
	}
	if(!searchByName && roleUniqueId.empty())
	{
                DEBUGL2("Role UniqueId is Empty \n");
                return STATUS_FAILED;
        }
	if(WriteXML(pNewUserData,sUserDataXPath,xml,roleName,roleUniqueId)!= STATUS_OK)
	{
		DEBUGL2("GetRoleInfo ::Execute::Failed \n");
		ret = STATUS_FAILED;
	}
	return ret;
}
Status GetRoleInfo::WriteXML(ElementRef pNewUserDoc,  CString sUserDataXPath, CString &xml,const CString& RName,const CString& roleID)
{
	Status ret = STATUS_FAILED;

	xml = USER_XML;

	ret = ExecuteSSDK(sUserDataXPath.c_str(), pNewUserDoc, RName,roleID,xml);

	return ret;
}

Status GetRoleInfo::ExecuteSSDK(ci::operatingenvironment::CString sUserDataXPath, NodeRef pNewUserDoc,const CString& rName,const CString& roleID,CString &xml)
{
	MyUserToken myut;
	BOProxyRef boProxy;
	Ref<SSDKLogInterface> LogExternalPtr=NULL;
	ssdk::SSDKStatus ssdkStatus;
	ci::operatingenvironment::Ref<dom::Element> pNewUserEle=NULL;
	ci::operatingenvironment::Ref<dom::Element> tempEle=NULL;
	ElementRef pObjectElem = NULL;
	ElementRef pObjectElem1 = NULL;
	//Authenticate the user
	if(Authenticate(myut, boProxy, LogExternalPtr) == STATUS_FAILED)
        {
           DEBUGL2("GetRoleInfo::Authenticate::Failed \n");
           return STATUS_FAILED;
        }
	 Ref<SSDKSecurityManagerInterface> ssdkSecurityManagerInterface =ssdk::GetSecurityManager();
	 Ref<SSDKRBACMgmtInterface> rbac = dynamic_cast<SSDKRBACMgmtInterface*> (ssdkSecurityManagerInterface->GetInterface  (SSDKSecurityManagerInterface::RBAC_MGMT_INTERFACE,ssdkStatus));
	if(rbac==(void*) NULL || ssdkStatus != OK)
        {
                DEBUGL1("\n ssdk SecurityManager::SSDKRBACMgmtInterface Failed \n");
                return STATUS_FAILED;
        }
	bool create = true;
	unsigned int size =0;
	Role userRole; //to set user info back to UI

	try
	{
		set< pair <CString,CString> > ::iterator it;
		CString allUsers =STRING_NULL;
		SSDKRBACMgmtInterface::RoleInfo SSDKRoleInfo;
		SSDKRBACMgmtInterface::UserInfo SSDKUserInfo;
		//Get the information for that roleid
		//SSDKRoleInfo=rbac->GetProperty(myut.ut, SSDKRBACMgmtInterface::ROLE_ALL_PROPERTIES,rName, ssdkStatus);
		CString roleToQuery(rName);
		SSDKRBACMgmtInterface::RolePropertyName roleProperty = SSDKRBACMgmtInterface::ROLE_ID_PROPERTY;
		if(!roleID.empty())
		{
			roleToQuery = roleID;
			roleProperty = SSDKRBACMgmtInterface::ROLE_UNIQUE_ID_PROPERTY;
		}
		SSDKRoleInfo=rbac->GetProperty(myut.ut,SSDKRBACMgmtInterface::ROLE_ALL_PROPERTIES,roleProperty,roleToQuery,ssdkStatus);
		if(ssdkStatus == AUTH_RBAC_DB_CORRUPTED)
		{
			DEBUGL2("SSDK RBAC DB CRRUPTED\n");
			Utils::CheckForCorruption(ssdkStatus);
			return STATUS_AL_SEC_DB_CORRUPTED;
		}

		if(OK==ssdkStatus)//DBCORR
		{
		
		userRole.SetName(sUserDataXPath,  pNewUserDoc,SSDKRoleInfo.m_RoleId,create);
		DEBUGL8("Role Unique id:%s\n",SSDKRoleInfo.m_RoleId.c_str());
		userRole.SetUniqueId(sUserDataXPath,  pNewUserDoc,SSDKRoleInfo.m_RoleUniqueID,create);
		CString ldapRule=STRING_NULL,inhRoles=STRING_NULL,groupList=STRING_NULL;
				
		//Set the Inherited Roles
		//MakeTokenizeString( SSDKRoleInfo.m_RoleRoles, inhRoles);
		inhRoles = "<InheritRoles>";
		CString roles="";
		std::vector<std::string>::iterator iter1;
		for(iter1=SSDKRoleInfo.m_RoleRoles.begin();iter1!=SSDKRoleInfo.m_RoleRoles.end();++iter1)
		{
			EbxSSDKUtil::CheckXMLEntityAndReplaceString(*iter1);
			roles = roles +"<role>" + *iter1 + "</role>";
		}
		if(!roles.empty())
		{
			inhRoles.append(roles);
			inhRoles.append("</InheritRoles>");
			userRole.SetInhRole(sUserDataXPath.c_str(), pNewUserDoc,inhRoles, create);
		}
		//Set the description
		 userRole.SetDesc( sUserDataXPath,  pNewUserDoc, SSDKRoleInfo.m_RoleDescription ,create);
		
		//Set the groups
		//MakeTokenizeString( SSDKRoleInfo.m_RoleGroups, groupList);
		CString groups="";
		groupList = "<Groups>";
		for(iter1=SSDKRoleInfo.m_RoleGroups.begin();iter1!=SSDKRoleInfo.m_RoleGroups.end();++iter1)
		{
			EbxSSDKUtil::CheckXMLEntityAndReplaceString(*iter1);
			groups = groups +"<group>" + *iter1 + "</group>";
		}
		if(!groupList.empty())
		{	
			groupList.append(groups);
			groupList.append("</Groups>");	
			userRole.SetGroup(sUserDataXPath.c_str(), pNewUserDoc,groupList,create);
			
		}
		//Check if the ldap rule is enabled 
		if (0==SSDKRoleInfo.m_RoleRules.size())
			userRole.SetLdapEnable(sUserDataXPath.c_str(), pNewUserDoc,"0", create);			
		else	
		{
			MakeTokenizeString( SSDKRoleInfo.m_RoleRules, ldapRule);
			userRole.SetLdapRule(sUserDataXPath.c_str(), pNewUserDoc,ldapRule, create);
			userRole.SetLdapEnable(sUserDataXPath.c_str(), pNewUserDoc,"1", create);
		}
		size =static_cast<unsigned int>(SSDKRoleInfo.m_RoleUsers.size());
		if(0 == size)
		 {   
			 DEBUGL8( "No Users\n");
		 }
 		else {
				CString users="";
				allUsers ="<Users>";
				//MakeTokenizeString(SSDKRoleInfo.m_RoleUsers,allUsers);
				for(iter1=SSDKRoleInfo.m_RoleUsers.begin();iter1!=SSDKRoleInfo.m_RoleUsers.end();++iter1)
				{
					EbxSSDKUtil::CheckXMLEntityAndReplaceString(*iter1);
					users = users + "<User><Information><name>" + *iter1 + "</name></Information></User>";
				}
				if(!users.empty())
				{
					allUsers.append(users);
					allUsers.append("</Users>");
					userRole.SetUsers(sUserDataXPath.c_str(), pNewUserDoc,allUsers,create);
				}
 		}
		//Get the objects
		DEBUGL8("\n Objects and Permissions...............%d \n",SSDKRoleInfo.m_RolePermissions.size());
		CString tempObjName,ObjOpers;
		vector<CString> perms;
		
		//Check for existing <Objects> element and if found delete the element and create a new <Objects>
		try{
			pObjectElem = CRoleManager::m_pHDB->BindToElement(pNewUserDoc,"Information/Objects");
			if(pObjectElem)
			{
				//Remove the <Objects> Element
				ElementRef pParent = pObjectElem->getParentNode();
				if(!pParent)
				{
					DEBUGL1("Failed to get the parent Node of <Objects>\n");
					return STATUS_FAILED;
				}
				if(!pParent->removeChild(pObjectElem))
				{
					DEBUGL1("Failed to remove the Node <Objects>\n");
					return STATUS_FAILED;
				}
			}
			pObjectElem1 = CRoleManager::m_pHDB->BindToElement(pNewUserDoc,"Information");
			if(!pObjectElem1)
			{
				DEBUGL1("\nFailed to bind to node <Information>\n");
				return STATUS_FAILED;
			}
		}catch(DOMException& except)
		{
			DEBUGL1("\n Caught DOMException !!!\n");
			return STATUS_FAILED;
		}
		if(SSDKRoleInfo.m_RolePermissions.size()>0)
		{
			
			it=SSDKRoleInfo.m_RolePermissions.begin();
			tempObjName=(*it).first.c_str();
			CString objectsXml = "";
			CString parentNode = "Operations";
			CString childNode="operation";
			CString oprXmlNode ="";

			for(it=SSDKRoleInfo.m_RolePermissions.begin();it!=SSDKRoleInfo.m_RolePermissions.end();it++)
			{
				DEBUGL8("ObjName %s   Oper   %s \n",(*it).first.c_str(),(*it).second.c_str());
				if(tempObjName.compare((*it).first)==0)
				{
					perms.push_back((*it).second);
				}
				else
				{
					//MakeTokenizeString( perms, ObjOpers);
					oprXmlNode=CreateXMLNode(perms,parentNode,childNode);
					EbxSSDKUtil::CheckXMLEntityAndReplaceString(tempObjName);
					objectsXml = objectsXml +"<Object><name>"+ tempObjName +"</name>"+oprXmlNode+"</Object>";
					DEBUGL8("\nMY OBJ SRING2 ::%s\n",objectsXml.c_str());
					tempObjName=(*it).first.c_str();
					perms.clear();
					perms.push_back((*it).second);
				}
			}
			//MakeTokenizeString( perms, ObjOpers);
			oprXmlNode=CreateXMLNode(perms,parentNode,childNode);
			EbxSSDKUtil::CheckXMLEntityAndReplaceString(tempObjName);
			//EbxSSDKUtil::CheckXMLEntityAndReplaceString(ObjOpers);
			objectsXml = objectsXml +"<Object><name>"+ tempObjName +"</name>"+ oprXmlNode +"</Object>";
			objectsXml ="<Objects>"+ objectsXml +"</Objects>";
			DEBUGL8("\nMY OBJ SRING ::%s\n",objectsXml.c_str());
			if(AddTheNewNode(objectsXml,pObjectElem1,pNewUserEle) != STATUS_OK)
			{
				DEBUGL1("\n Failed to DeSerialize the string :: %s\n",objectsXml.c_str());
				return STATUS_FAILED;
			}		
		}
		}
		else 
                {		
                   DEBUGL1( "\nGetProperty(ROLE_ALL_PROPERTIES)  Status %d::\n",ssdkStatus);
                   throw CRoleException("STATUS_FAILED");
		}
	}
	catch(...)
	{
		DEBUGL1("Some Exceptions from GetRoleInfo  ssdk GetProperty() call");
		return STATUS_FAILED;
	}		
	return STATUS_OK;
}
Status GetOnlyRoleNames::Execute()
{
	ElementRef pParamNode = NULL;
	ElementRef pNewUserData = NULL; 
	CString sUserDataXPath = STRING_NULL;
	Status ret = STATUS_OK;
	pParamNode = m_command->GetParamNode(ROLE_DETAILS);
	DocumentRef pNewUserDoc;	
	if(pParamNode)
	{
		ret = ProcessTransDoc("GetOnlyRoleNames",sUserDataXPath,pParamNode,pNewUserDoc);	
		if (STATUS_OK==ret)
		{
			DocumentGuard domGuard(pNewUserDoc);
			if(STATUS_OK != domGuard.WriteLock(false))
                	{
                        	DEBUGL1("Failed to Acquire Write Lock on Rbac Session DOM\n");
                        	return STATUS_FAILED;
                	}
			pNewUserData = CRoleManager::m_pHDB->BindToElement(pNewUserDoc,sUserDataXPath.c_str(),true);
			if (!pNewUserData)                      
			{
				DEBUGL2("CRoleCommandHandler::ProcessTransDoc::Failed To Bind To Information Node (%s) Passed by Client Document\n",sUserDataXPath.c_str());
        			return STATUS_FAILED;
			}
			ret = ExecuteGetRoles(pNewUserData,sUserDataXPath);

		}
		else
		{
			ret = STATUS_AL_SEC_COMMAND_SCHEMA;
			DEBUGL1("In GetOnlyRoleNames::Execute:: Failed \n");
		}
	}
	else
	{
		DEBUGL1("In GetOnlyRoleNames::Execute:: Failed \n");
		 ret = STATUS_AL_SEC_COMMAND_SCHEMA;
	}

	return ret;	
}

Status GetOnlyRoleNames::ExecuteGetRoles(ElementRef pNewUserData , CString sUserDataXPath)
{
		Status ret = STATUS_OK;
		CString xml;
		DEBUGL8("GETNODENAME::%s\n",pNewUserData->getNodeName().c_str());
		CString bindNode = pNewUserData->getNodeName();
		
		//Get All roles
		CString bindPath ;
	 	//Get All roles
                ElementRef _pNewUserData= NULL;
                bindPath = sUserDataXPath + "/Roles" ;
                try{
                if(bindNode == "Roles")
                        _pNewUserData = pNewUserData;
                else
                        _pNewUserData = CRoleManager::m_pHDB->BindToElement(pNewUserData, "Roles");
                ElementRef pParent = NULL;
                if(_pNewUserData)
                {
                        //Remove Existing <Roles> element node
                        pParent = _pNewUserData->getParentNode();
                        if(pParent)
                        {
                                //Remove the node pointing by _pNewUserData
                                if(!pParent->removeChild(_pNewUserData))
                                {
                                        DEBUGL1("\n Failed to remove child node <Roles>\n");
                                        return STATUS_FAILED;
                                }
                        }
                        else
                        {
                                DEBUGL1("\n Failed to get Paranet node of <Roles>\n");
                                return STATUS_AL_SEC_INTERNAL_ERROR;
                        }
                }
                else
                        pParent =pNewUserData;
		CString xml = "<Roles/>";
                ElementRef pNewUserEle = pParent;
                if(pNewUserEle)
                {
                        CString xml;
                        try{
                                DEBUGL8("\n BIND PATH :: %s\n",bindPath.c_str());
                                ret = GetAllRoles(pNewUserEle,  bindPath , xml);
				if(ret  != STATUS_OK)
                                {
                                        DEBUGL2("GetRoles::Execute::Failed \n");
                                      	return ret; 
                                }
                        }
                        catch(CRoleException& userExcept){
                                DEBUGL1("\n GetRoles::ExecuteIndividualRole:: CRoleException caught !! \n");
                                return STATUS_FAILED;
                        }
                }
                }catch(DOMException& domExcept)
                {
                        DEBUGL1("\n Caught DOMException !!!\n");
                        return STATUS_FAILED;
                }
		catch(...)
		{
			DEBUGL1("Unknown Error\n");
			ret = STATUS_AL_SEC_UNKNOWN_ERROR;
		}
	
	return ret;
}

Status GetOnlyRoleNames::GetAllRoles(ElementRef pNewUserDoc,  CString sUserDataXPath, CString &xml)
{
	Status ret = STATUS_FAILED;
	
	xml = "<Role><Information>";
	xml += "<name></name>";
	xml += "</Information></Role>";
	
	ci::operatingenvironment::Ref<dom::Element> pNewUserEle = NULL;
	ret = ExecuteSSDK(sUserDataXPath.c_str(), pNewUserDoc, STRING_NULL,xml, pNewUserEle);	

	return ret;
}

Status GetOnlyRoleNames::ExecuteSSDK(ci::operatingenvironment::CString sUserDataXPath, NodeRef pNewUserDoc, CString rName,CString &xml, ci::operatingenvironment::Ref<dom::Element> &pNewUserEle)
{
	MyUserToken myut;
	BOProxyRef boProxy;
	Ref<SSDKLogInterface> LogExternalPtr=NULL;
	ssdk::SSDKStatus ssdkStatus;	

	NodeRef pNewEle;
	Status ret = STATUS_FAILED;
	ci::operatingenvironment::Ref<dom::Element> pNewUserInfoEle; 
	
	ret = Authenticate(myut, boProxy, LogExternalPtr);
	if(ret!= STATUS_OK) 
        {
           	DEBUGL2("GetUsers::Authenticate::Failed \n");
	  	return ret;
        }
	Ref<SSDKSecurityManagerInterface> ssdkSecurityManagerInterface =ssdk::GetSecurityManager();
	Ref<SSDKRBACMgmtInterface> rbac = dynamic_cast<SSDKRBACMgmtInterface*> (ssdkSecurityManagerInterface->GetInterface  (SSDKSecurityManagerInterface::RBAC_MGMT_INTERFACE,ssdkStatus));
	if(rbac==(void*) NULL || ssdkStatus != OK)
        {
                DEBUGL1("\n ssdk SecurityManager::SSDKRBACMgmtInterface Failed \n");
                return STATUS_AL_SEC_INTERNAL_ERROR;
        }

	SSDKRBACMgmtInterface::UserInfo SSDKUserInfo;
		
	CString finalXml("<Roles>");
	Role role;
	try{
		vector<SSDKRBACMgmtInterface::RoleInfo> roles;
		ssdkStatus=rbac->GetProperty(myut.ut,SSDKRBACMgmtInterface::ROLE_ID_PROPERTY,STRING_NULL,roles);
		DEBUGL8("GetProperty(ROLE_ID_PROPERTY) Status ::%d\n",ssdkStatus);
		if(OK==ssdkStatus)
		{
			DEBUGL8("\nGet all roles .......\n");
			unsigned int size= roles.size();
			CString roleName=STRING_NULL;
			CString isAdminRole = "false";
			vector<SSDKRBACMgmtInterface::RoleInfo> sortedRoles;
			for(unsigned int _cnt=0;_cnt<size;++_cnt)
			{
				SSDKRBACMgmtInterface::RoleInfo tempRoleInfo;
                                tempRoleInfo =rbac->GetProperty(myut.ut,SSDKRBACMgmtInterface::ROLE_ALL_PROPERTIES,roles[_cnt].m_RoleId,ssdkStatus);
                                if(OK != ssdkStatus)
                                        DEBUGL8("Failed to fetch unique id for role:%s\n",roles[_cnt].m_RoleId.c_str());
				DEBUGL8("\n ROLE NAME ::%s\n",roles[_cnt].m_RoleId.c_str());
				sortedRoles.push_back(tempRoleInfo);
			}
			bool sortAscending = true;
			if(sortAscending)
				sort(sortedRoles.begin(),sortedRoles.end(),Utils::SortAscending);
			else
				sort(sortedRoles.begin(),sortedRoles.end(),Utils::SortDescending);
			size = sortedRoles.size();
			for(unsigned int _cnt=0;_cnt<size;++_cnt)
			{
				roleName = sortedRoles[_cnt].m_RoleId.c_str();
				/*
				//Set the role name and the description
				role.SetName(sUserDataXPath.c_str(), pNewUserEle,roleName.c_str(),create);
				//Set the unique id	
				role.SetUniqueId(sUserDataXPath.c_str(), pNewUserEle,tempRoleInfo.m_RoleUniqueID,create);*/

				isAdminRole = rbac->GetProperty(myut.ut,SSDKRBACMgmtInterface::RBAC_ROLE_OBJECT,roleName,IS_ADMIN_ROLE,ssdkStatus);
				EbxSSDKUtil::CheckXMLEntityAndReplaceString(roleName);
				finalXml += "<Role ID='" + sortedRoles[_cnt].m_RoleUniqueID + "'><Information><name>" + roleName + "</name>";
				if(isAdminRole == "true")
					finalXml += "<type>" + CString(ADMIN_ROLE) + "</type>"; 
				else
					finalXml += "<type>" + CString(SYSTEM_ROLE) + "</type>";
				finalXml += "</Information></Role>";
			}
			finalXml += "</Roles>";
			DEBUGL8("Role Name xml:%s\n",finalXml.c_str());	
			if(AddTheNewNode(finalXml, pNewUserDoc, pNewUserEle) != STATUS_OK)
			{
				DEBUGL1("Failed to DeSerialized the string:: %s\n",finalXml.c_str());
				return STATUS_FAILED;
			}
		}
		else
		{
			DEBUGL1("Failed to get roles\n");
			ret = SSDK_RBAC_MANAGER_PLUGIN_ERROR_MAP(ssdkStatus);
			return ret; 
		}
	}
	catch(DOMException& domExcept)
        {
                DEBUGL1("\n Caught DOMException !! \n");
                return STATUS_FAILED;
        }
	catch(...)
	{
		DEBUGL1("UnKnown Exceptions!!");
		return STATUS_AL_SEC_UNKNOWN_ERROR;
	}		
	return STATUS_OK;
}

Status GetAllObjects::Execute()
{
	ElementRef pParamNode = NULL;
	ElementRef pNewUserData = NULL; 
	CString sUserDataXPath = STRING_NULL;
	Status ret = STATUS_OK;
	pParamNode = m_command->GetParamNode(OBJECT_DETAILS);
	DocumentRef pNewUserDoc;	
	if(pParamNode)
	{
		ret = ProcessTransDoc("GetAllObjects",sUserDataXPath,pParamNode,pNewUserDoc);	
		if ( STATUS_OK==ret)
		{
			pNewUserData = CRoleManager::m_pHDB->BindToElement(pNewUserDoc,sUserDataXPath.c_str(),true);
			if (!pNewUserData)                      
			{
				DEBUGL2("GetAllObjects::Execute::Failed To Bind To Information Node (%s) Passed by Client Document\n",sUserDataXPath.c_str());
        			return STATUS_FAILED;
			}
			ret = ExecuteGetObjects(pNewUserData,sUserDataXPath);	
		}
		else
			DEBUGL1("In GetAllObjects::Execute:: Failed \n");
	}
	else
	{
		DEBUGL1("In GetAllObjects::Execute:: Failed \n");
		 ret = STATUS_FAILED;
	}

	return ret;	
}

Status GetAllObjects::ExecuteGetObjects(ElementRef pNewUserData , CString sUserDataXPath)
{
	Status ret = STATUS_OK;
	CString xml,xml2;
	CString bindPath ="";
	ElementRef _pNewUserData= NULL;
	ElementRef _pNewUserData2= NULL;
	DEBUGL8("GETNODENAME::%s\n",pNewUserData->getNodeName().c_str());	
	CString bindNode = pNewUserData->getNodeName();
	if(bindNode == "RBACObjects")
	{
		_pNewUserData = pNewUserData;
	}	
	else
		_pNewUserData = CRoleManager::m_pHDB->BindToElement(pNewUserData, "RBACObjects");

	ElementRef pParent = NULL;
	ElementRef pParent2 = NULL;
	if(_pNewUserData)
	{
		//Remove Existing <RBACObjects> element node
		pParent = _pNewUserData->getParentNode();
		if(pParent)
		{
			//Remove the node pointing by _pNewUserData
			if(!pParent->removeChild(_pNewUserData))
			{
				DEBUGL1("\n Failed to remove child node <RBACObjects>\n");
				return STATUS_FAILED;
			}
			_pNewUserData2 = CRoleManager::m_pHDB->BindToElement(pParent, "RBACPermissions");
			if(_pNewUserData2)
			{
				pParent2 = _pNewUserData2->getParentNode();
			   	if(pParent2)
				{
					if(!pParent2->removeChild(_pNewUserData2))
					{
						DEBUGL1("\n Failed to remove child node <RBACPermissions>\n");
						return STATUS_FAILED;
					}
				}
				else
                		{
                        		DEBUGL1("\n Failed to get Paranet node of <RBACPermissions>\n");
                        		return STATUS_FAILED;
                		}
			}
		}
		else
                {
                	DEBUGL1("\n Failed to get Paranet node of <RBACObjects>\n");
               	 	return STATUS_FAILED;
                }
	}
	else
		pParent = pNewUserData;
	
        bindPath = sUserDataXPath + "/RBACObjects" ;
        xml = "<RBACObjects/>";
        ElementRef pNewUserEle = NULL;
	ElementRef pNewUserEle2 = NULL;
	xml2 = "<RBACPermissions/>";	
        //if(AddTheNewNode(xml, pNewUserData, pNewUserEle) == STATUS_FAILED)
        if(AddTheNewNode(xml, pParent, pNewUserEle) == STATUS_FAILED)
	{
        	DEBUGL1("\n Failed to DeSerialized the string :: %s\n",xml.c_str());
		throw CRoleException("STATUS_FAILED");
	}
	//if(AddTheNewNode(xml2, pNewUserData, pNewUserEle2) == STATUS_FAILED)
	if(AddTheNewNode(xml2, pParent , pNewUserEle2) == STATUS_FAILED)
        {
                DEBUGL1("\n Failed to DeSerialized the string :: %s\n",xml.c_str());
                throw CRoleException("STATUS_FAILED");
        }

        if(pNewUserEle && pNewUserEle2)
        {
        	try{
			DEBUGL8("\n BIND PATH :: %s\n",bindPath.c_str());
			if(ExecuteSSDK(pNewUserEle,pNewUserEle2,bindPath ) != STATUS_OK)
			{
				DEBUGL2("GetAllObjects::Execute::Failed \n");
				ret = STATUS_FAILED;
			}	
        	}
		catch(DOMException& domExcept)
		{
			DEBUGL1("\n Caught dom exception !! \n");
			ret = STATUS_FAILED;
		}
        	catch(CRoleException& userExcept)
		{
        		DEBUGL1("\n GetAllObjects::Execute CRoleException caught !! \n");
        		return STATUS_FAILED;
		}
        }
	
	return ret;
}


Status GetAllObjects::ExecuteSSDK(NodeRef pNewUserDoc, NodeRef pNewUserDoc2, ci::operatingenvironment::CString sUserDataXPath)
{
	MyUserToken myut;
	BOProxyRef boProxy;
	Ref<SSDKLogInterface> LogExternalPtr=NULL;
	ssdk::SSDKStatus ssdkStatus;	

	NodeRef pNewEle;
	ci::operatingenvironment::Ref<dom::Element> pNewUserEle; 
	CString xml = RBACOBJ_XML;
	if(Authenticate(myut, boProxy, LogExternalPtr) == STATUS_FAILED)
        {
           DEBUGL2("GetAllObjects::Authenticate::Failed \n");
           return STATUS_FAILED;
        }
	 Ref<SSDKSecurityManagerInterface> ssdkSecurityManagerInterface =ssdk::GetSecurityManager();
	 Ref<SSDKRBACMgmtInterface> rbac = dynamic_cast<SSDKRBACMgmtInterface*> (ssdkSecurityManagerInterface->GetInterface  (SSDKSecurityManagerInterface::RBAC_MGMT_INTERFACE,ssdkStatus));
	if(rbac==(void*) NULL || ssdkStatus != OK)
        {
                DEBUGL1("\n ssdk SecurityManager::SSDKRBACMgmtInterface Failed \n");
                return STATUS_FAILED;
        }

		
	
	bool create = false;
	Role role;
	try{
		vector<SSDKRBACMgmtInterface::ObjectTypeInfo> objects;
		SSDKRBACMgmtInterface::ObjectTypeInfo parentObj;
		set <CString> operset;
		vector <CString> operVec;
		set<CString>::iterator it;
		unsigned int  i=0;

		ssdkStatus=rbac->GetProperty(myut.ut,SSDKRBACMgmtInterface::OBJECT_TYPE_ALL_PROPERTIES,STRING_NULL, objects);
		if(ssdkStatus == AUTH_RBAC_DB_CORRUPTED)
		{
			DEBUGL2("SSDK RBAC DB CRRUPTED\n");
			Utils::CheckForCorruption(ssdkStatus);
			return STATUS_AL_SEC_DB_CORRUPTED;
		}

		if(OK == ssdkStatus)//DBCORR
		{
			DEBUGL8("\nGet all objects .......\n");
			unsigned int size= objects.size();
			CString objectName=STRING_NULL;
			CString opers =STRING_NULL;
			CString parent =STRING_NULL;
			for(unsigned int _cnt=0;_cnt<size;++_cnt)
			{
			          if(AddTheNewNode(xml, pNewUserDoc, pNewUserEle) == STATUS_FAILED)
                                               throw CRoleException("STATUS_FAILED");
				objectName = objects[_cnt].m_ObjectType.c_str();
				DEBUGL8("\n OBJECT NAME ::%s",objectName.c_str());
				opers="<Operations>";
				CString indOpr="";
				//Get all the operations
				for (i =0;i< objects[_cnt].m_ObjectTypeOperations.size();i++)
					operset.insert( objects[_cnt].m_ObjectTypeOperations[i]);
				parent=objects[_cnt].m_ParentType;
				while (!parent.empty())
				{
					parentObj=rbac->GetProperty(myut.ut,SSDKRBACMgmtInterface::OBJECT_TYPE_ALL_PROPERTIES,parent,ssdkStatus);//DBCORR
					if(ssdkStatus == AUTH_RBAC_DB_CORRUPTED)
					{
						DEBUGL2("SSDK RBAC DB CRRUPTED\n");
						Utils::CheckForCorruption(ssdkStatus);
						return STATUS_AL_SEC_DB_CORRUPTED;
					}

					for (i =0;i< parentObj.m_ObjectTypeOperations.size();i++)
					{
						operset.insert( parentObj.m_ObjectTypeOperations[i]);
					}
					parent = parentObj.m_ParentType;
				}
				parent.clear();
				for (it=operset.begin(); it!=operset.end(); it++)
				{
					   //operVec.push_back(*it);
					CString oprName= *it;
					EbxSSDKUtil::CheckXMLEntityAndReplaceString(oprName);	
					indOpr = indOpr + "<operation>" + oprName +"</operation>";
				}
				//Set the object Name
				EbxSSDKUtil::CheckXMLEntityAndReplaceString(objectName);
				role.SetObjectName(sUserDataXPath.c_str(), pNewUserEle,objectName.c_str(),create);
				//Tokenize the operations
				//MakeTokenizeString( operVec,opers);
				//Set the Opers
				if(!indOpr.empty())
                                {
                                        opers.append(indOpr);
                                        opers.append("</Operations>");
					role.SetObjectOpers( sUserDataXPath.c_str(), pNewUserEle,opers.c_str(),create);
				}
				objectName.clear();
				opers.clear();
				//operVec.clear();
				operset.clear();
				pNewUserEle = NULL;
			}
			
		}
		/* 
		CategoryDisplayNameMap::iterator iterMap;
		pNewUserEle=NULL;
		CString oprList="";
		for(iterMap = CRoleManager::categoryDisplayMap.begin();iterMap!=CRoleManager::categoryDisplayMap.end();++iterMap)
		{
			CString permXml = RBACPERMS_XML;
			if(AddTheNewNode(permXml, pNewUserDoc2, pNewUserEle) == STATUS_FAILED)
                        	throw CRoleException("STATUS_FAILED");
			oprList.clear();
			CString categoryName = iterMap->first;
			set<CString>::iterator iterSet;
			CString allOpr="<Operations>"; 
			for(iterSet=(iterMap->second).begin();iterSet!=(iterMap->second).end();++iterSet)
			{
				oprList += "<operation>" + *iterSet + "</operation>";	
			}
			if(!oprList.empty())
			{	allOpr.append(oprList);
				allOpr.append("</Operations>");
				role.SetObjectName(sUserDataXPath.c_str(),pNewUserEle,categoryName,true);
				role.SetObjectOpers(sUserDataXPath.c_str(),pNewUserEle,allOpr,true);
			}
			allOpr.clear();
			pNewUserEle = NULL;	
		}*/
	}catch(...)
	{
		DEBUGL1("Some Exceptions from ssdk GetAllObjects");
		return STATUS_FAILED;
	}		
	return STATUS_OK;
}

Status RoleCustomProperty::Execute()
{
	ElementRef pParamNode = NULL;
	ElementRef pNewRoleData = NULL;
        CString sRoleDataXPath = STRING_NULL;
        Status ret = STATUS_OK;
        pParamNode = m_command->GetParamNode(ROLE_DETAILS);
	DocumentRef pNewUserDoc;

	if(pParamNode)
        {
		//convert from string xpath to element ref
		ret = ProcessTransDoc("SET_GET_ROLE_CUS_PROP",sRoleDataXPath,pParamNode,pNewUserDoc);
                if (ret != STATUS_OK)
                {
                        DEBUGL1("In RoleCustomProperty::Execute:: Failed \n");
                        ret = STATUS_AL_SEC_COMMAND_SCHEMA;
                        return ret;
                }
		DocumentGuard domGuard(pNewUserDoc);
		if(STATUS_OK != domGuard.WriteLock(false))
                {
                        DEBUGL1("Failed to Acquire Write Lock on Rbac Session DOM\n");
                        return STATUS_FAILED;
                }
		pNewRoleData = CRoleManager::m_pHDB->BindToElement(pNewUserDoc,sRoleDataXPath.c_str(),true);
		if (!pNewRoleData)                      
		{
			DEBUGL2("RoleCustomProperty::Execute::Failed To Bind To Information Node (%s) Passed by Client Document\n",sRoleDataXPath.c_str());
        		return STATUS_AL_SEC_COMMAND_SCHEMA;
		}
                ret = ExecuteSSDK(pNewRoleData,sRoleDataXPath);
	}
	else
        {
                DEBUGL1("In RoleCustomProperty::Execute:: Failed \n");
                ret = STATUS_AL_SEC_COMMAND_SCHEMA;
        }
        return ret;
}

Status RoleSetCustomProperty::ExecuteSSDK(NodeRef pNewRoleData, CString sRoleDataXPath)
{
	MyUserToken myut;
        BOProxyRef boProxy;
        Ref<SSDKLogInterface> LogExternalPtr=NULL;
        ssdk::SSDKStatus ssdkStatus;
        Status ret=STATUS_FAILED;
        ci::operatingenvironment::Ref<dom::Element> pNewRoleEle=NULL;
	//Authenticate the user
	ret =Authenticate(myut, boProxy, LogExternalPtr);
	if(ret!= STATUS_OK) 
        {
		DEBUGL2("RoleSetCustomProperty ::Authenticate::Failed \n");
		return ret;
	}
	Ref<SSDKSecurityManagerInterface> ssdkSecurityManagerInterface =ssdk::GetSecurityManager();
        Ref<SSDKRBACMgmtInterface> rbac = dynamic_cast<SSDKRBACMgmtInterface*> (ssdkSecurityManagerInterface->GetInterface  (SSDKSecurityManagerInterface::RBAC_MGMT_INTERFACE,ssdkStatus));
        if(rbac==(void*) NULL || ssdkStatus != OK)
        {
                DEBUGL1("\n ssdk SecurityManager::SSDKRBACMgmtInterface Failed \n");
                return STATUS_AL_SEC_INTERNAL_ERROR;
        }
	CString rName ="";
	Role role;
	if(sRoleDataXPath.c_str())	
	{
		rName = role.GetName(sRoleDataXPath.c_str(),pNewRoleData);
		DEBUGL8("\nRole Name :: %sn",rName.c_str());
		if(rName.empty()) return STATUS_AL_SEC_INVALID_INPUT;

		//Adding Custom Property to Role
		ElementRef pNewEle;
		NodeRef RoleRef = pNewRoleData;
		//Get Role Custom Property
		//Bind to Information/CustomProperties
		CString cusPropXml =    "Information/CustomProperties/AddViewProperty";
		pNewEle=CRoleManager::m_pHDB->BindToElement(RoleRef, cusPropXml);
		CString nodeText = pNewEle->getParentNode()->getNodeName();
		DEBUGL8("NODE_TEXT::%s\n",nodeText.c_str());
		//Traverse each child nodes
		Ref<NodeList> elems = pNewEle->getChildNodes();
		unsigned int len = elems->getLength();
		for(unsigned int _cnt=0;_cnt<len;_cnt++)
		{
			Ref<Element> node=elems->item(static_cast<unsigned long>(_cnt));
                        CString propName = role.GetRoleCusPropName(sRoleDataXPath,node);
                        CString propValue = role.GetRoleCusPropValue(sRoleDataXPath,node);
                        if(!propName.empty() && !propValue.empty())
			{
                        	ssdkStatus = rbac->SetProperty(myut.ut,SSDKRBACMgmtInterface::RBAC_ROLE_OBJECT,rName,propName,propValue);//DBCORR
				if(ssdkStatus == AUTH_RBAC_DB_CORRUPTED)
				{
					DEBUGL2("SSDK RBAC DB CRRUPTED\n");
					Utils::CheckForCorruption(ssdkStatus);
					return STATUS_AL_SEC_DB_CORRUPTED;
				}

				DEBUGL8("\nSSDK SetProperty(RBAC_ROLE_OBJECT) status is :: %d\n",ssdkStatus);
			}
                        else
                                DEBUGL5("\n PropertyName or PropertyValue is empty\n");
                        node = NULL;
		}
	}
	else
        {
        	DEBUGL2("RoleSetCustomProperty::ExecuteSSDK::Failed \n");
        	return STATUS_AL_SEC_INTERNAL_ERROR;
        }
        DEBUGL8("\nEND OF ADDING ROLE CUSTOM PROPERTY \n");
        return STATUS_OK;
}
Status RoleGetCustomProperty::ExecuteSSDK(NodeRef pNewRoleData, CString sRoleDataXPath)
{
	MyUserToken myut;
        BOProxyRef boProxy;
        Ref<SSDKLogInterface> LogExternalPtr=NULL;
        ssdk::SSDKStatus ssdkStatus;
        Status ret=STATUS_FAILED;
        ci::operatingenvironment::Ref<dom::Element> pNewRoleEle=NULL;
	//Authenticate the user
	ret = Authenticate(myut, boProxy, LogExternalPtr);
	if(ret!= STATUS_OK)
        {
                DEBUGL2("RoleGetCustomProperty ::Authenticate::Failed \n");
                return ret;
        }
	Ref<SSDKSecurityManagerInterface> ssdkSecurityManagerInterface =ssdk::GetSecurityManager();
        Ref<SSDKRBACMgmtInterface> rbac = dynamic_cast<SSDKRBACMgmtInterface*> (ssdkSecurityManagerInterface->GetInterface  (SSDKSecurityManagerInterface::RBAC_MGMT_INTERFACE,ssdkStatus));
        if(rbac==(void*) NULL || ssdkStatus != OK)
        {
                DEBUGL1("\n ssdk SecurityManager::SSDKRBACMgmtInterface Failed \n");
                return STATUS_AL_SEC_INTERNAL_ERROR;
        }
        CString rName ="";
        Role role;
	if(sRoleDataXPath.c_str())
        {
		//Get Role Name
		rName = role.GetName(sRoleDataXPath.c_str(),pNewRoleData);
		DEBUGL8("\n Role Name = %s\n",rName.c_str());
		if(rName.empty()) return ret;
		ElementRef pNewEle;
                NodeRef RoleRef =pNewRoleData;
		//Get User Custom Property
		//Bind to Information/CustomProperties
		CString cusPropXml =    "Information/CustomProperties/AddViewProperty";
		pNewEle=CRoleManager::m_pHDB->BindToElement(RoleRef, cusPropXml,true);
		CString nodeText = pNewEle->getParentNode()->getNodeName();
		DEBUGL8("NODE_TEXT::%s\n",nodeText.c_str());
		//Traverse each child nodes
		Ref<NodeList> elems = pNewEle->getChildNodes();
		unsigned int len = elems->getLength();
		for(unsigned int _cnt=0;_cnt<len;_cnt++)
		{
			Ref<Element> node=elems->item(static_cast<unsigned long>(_cnt));
                        CString propName = role.GetRoleCusPropName(sRoleDataXPath,node,true);
                        CString propVal = rbac->GetProperty(myut.ut,SSDKRBACMgmtInterface::RBAC_ROLE_OBJECT,rName,propName,ssdkStatus);//DBCORR
			if(ssdkStatus == AUTH_RBAC_DB_CORRUPTED)
                        {
                                DEBUGL2("SSDK RBAC DB CRRUPTED\n");
                                Utils::CheckForCorruption(ssdkStatus);
                                return STATUS_AL_SEC_DB_CORRUPTED;
                        }
			DEBUGL5("\n PROP_NAME :: %s   PROP_VAL :: %s\n",propName.c_str(),propVal.c_str());
                        if(!propVal.empty())
                        	role.SetRoleCusPropName(sRoleDataXPath,node,propName,true);
                        DEBUGL8("\nSSDK GetProperty(RBAC_ROLE_OBJECT) status is :: %d\n",ssdkStatus);
                        if(ssdkStatus == OK)
                                role.SetRoleCusPropValue(sRoleDataXPath,node,propVal,true);
                        node = NULL;
		}	
	}
	else
        {
        	DEBUGL2("RoleGetCustomProperty::ExecuteSSDK::Failed \n");
        	return STATUS_AL_SEC_INTERNAL_ERROR;
        }
        return STATUS_OK;					
}

Status ImportExportRoleRuleMapping::Execute()
{
	MyUserToken myut;
        BOProxyRef boProxy;
        Ref<SSDKLogInterface> LogExternalPtr=NULL;
	//Authenticate the user
	Status ret=STATUS_OK;
        ret = Authenticate(myut, boProxy, LogExternalPtr);
	if(ret!= STATUS_OK)
        {
                DEBUGL2("ImportExportRoleRule ::Authenticate::Failed \n");
                return ret;
        }
	//Get the absolute file path
	//Get the file storage path. Previously need to read from fileStorage Path
	CString sFileStoragePath("DataImport");
	
	ElementRef pfileStorageNode = NULL;
        NodeRef pfileStorageText = NULL;
	pfileStorageNode = m_command->GetParamNode("filePath");
        if (!(pfileStorageNode) || (!(pfileStorageText = pfileStorageNode->getFirstChild()) || ((sFileStoragePath= pfileStorageText->getNodeValue()).size() == 0) ) )
        	DEBUGL2("ImportExportMapping::Execute::::Failed To  node in Command XML\n");
        DEBUGL8("Params fileStoragePath value:: %s\n",sFileStoragePath.c_str());
	if(sFileStoragePath.empty())
	{
		DEBUGL1("\n fileStorage Path is empty\n");
		return STATUS_AL_SEC_COMMAND_SCHEMA;
	}
	//Get the relative path
	ElementRef fileNameNode = NULL;
        NodeRef fileNameText = NULL;
        fileNameNode = m_command->GetParamNode("fileName");
        CString sRelativePath;
        if (!(fileNameNode) || (!(fileNameText = fileNameNode->getFirstChild()) || ((sRelativePath= fileNameText->getNodeValue()).size() == 0) ) )
                DEBUGL2("ImportExportMapping::Execute::::Failed To  node in Command XML\n");
	if(sRelativePath.empty())
	{
		DEBUGL1("\nFile name is empty\n");
                return STATUS_AL_SEC_COMMAND_SCHEMA;
	}
        DEBUGL8("Params relativePath value:: %s\n",sRelativePath.c_str());
	/*
 	//Commented because of requirement change.Previously refered from file storage DOM
	//Get the command name
	CString cmdName = m_command->GetCommandName();
	if("ImportMappingXML" != cmdName)
		sFileStoragePath = "DataExport";	
	//Get the path from file storage DOM
	CString absFolderPath;
	CString xPath = "FileStorages/FileStorage[@selected = '1'][name = '" + sFileStoragePath + "']/path";
	NodeRef domNode = NULL;
	if(boProxy->GetValue(xPath.c_str(),domNode) != STATUS_FAILED)
	{
		if(domNode)
		{
			absFolderPath = domNode->getNodeValue();
			DEBUGL5("\n FileStoragePath :: %s\n",absFolderPath.c_str());
		}
	}
	if(absFolderPath.empty())
	{
		DEBUGL1("No value for XPATH FileStoragePath ::%s \n",xPath.c_str());
		if(m_command)
		{
			if("ImportMappingXML" == cmdName)
				return STATUS_AL_SEC_IMPORT_FILE_NOT_FOUND;
			else
				return STATUS_AL_SEC_EXPORT_FILE_NOT_FOUND;
		}
		else
		{
			DEBUGL1("Unable to get the command name\n");
			return STATUS_AL_SEC_INTERNAL_ERROR;
		}
		
	}*/
	CString absFilePath;
	absFilePath=sFileStoragePath+"/"+sRelativePath;
	return ExecuteSSDK(absFilePath);
}

Status ImportRoleRuleMapping::SetDefaultRole(const SSDKUserTokenInterface* userToken,const CString& otherUser)
{
        ssdk::SSDKStatus ssdkStatus;
        Ref<SSDKSecurityManagerInterface> ssdkSecurityManagerInterface = ssdk::GetSecurityManager();
        Status ret = STATUS_FAILED;
        //Get the Config interface
        Ref<SSDKConfigInterface> configPtr = dynamic_cast<SSDKConfigInterface*> (ssdkSecurityManagerInterface->GetInterface  (SSDKSecurityManagerInterface::CONFIG_INTERFACE,ssdkStatus));
        if(configPtr ==(void*) NULL || ssdkStatus != OK)
        {
                DEBUGL1("\n ssdk SecurityManager::SSDKConfigInterface Failed \n");
		return STATUS_AL_SEC_INTERNAL_ERROR;
        }
	//Get the rbacType
        CString rbacTypeValue = configPtr->GetProperty(userToken,SSDKConfigInterface::SA_SERVAUTHEXTERNRBACENABLE,ssdkStatus);
        if(OK != ssdkStatus)
        {
                DEBUGL1("Get value from config failed:%d\n",ssdkStatus);
                ret = SSDK_RBAC_MANAGER_PLUGIN_ERROR_MAP(ssdkStatus);
                return ret;
        }
	CString otherUserToSet(EXTERNAL_RBAC_DEFAULT_ROLE);
	if(rbacTypeValue == "1")
	{
		if(otherUser != "EMPTY" && !otherUser.empty())
			otherUserToSet = otherUser;
		//Set the value to SSDK. If no AnyOtherUser tag is present, set it to User as info given by MSM
		ssdkStatus = configPtr->SetProperty(userToken,SSDKConfigInterface::SA_SERVAUTHRBACDEFAULTROLE,otherUserToSet);
		if(OK!=ssdkStatus)
		{
			DEBUGL1("Not able to set Err::%d \n",ssdkStatus);
			ret = SSDK_RBAC_MANAGER_PLUGIN_ERROR_MAP(ssdkStatus);
			return ret;
		}
	}
        return STATUS_OK;
}

Status ImportRoleRuleMapping::ExecuteSSDK(const CString& absFilePath)
{
	MyUserToken myut;
        BOProxyRef boProxy;
        Ref<SSDKLogInterface> LogExternalPtr=NULL;
	Status ret=STATUS_FAILED;
	
	//getting Autoprocessing token
	//flow for ImportroleRule mapping during first boot.m_upgradeRoleMappingFlag will be true only when rolerulemapping file exists and upgradeFileForRuleFlag file is present.
	//fix for EBX_STFR_18235
	if(m_upgradeRoleMappingFlag)
	{
		ssdk::SSDKStatus status;
		Ref<SSDKSecurityManagerInterface> ssdkSecurityManagerInterface = ssdk::GetSecurityManager();
		Ref<SSDKUserTokenInterface> userInterface = dynamic_cast<SSDKUserTokenInterface*> (ssdkSecurityManagerInterface->GetInterface(SSDKSecurityManagerInterface::USER_TOKEN_INTERFACE, status));
		if(userInterface == (void *)NULL || status != OK)
		{
			DEBUGL1("Casting to SSDKUserTokenInterface* failed\n");
			return STATUS_AL_SEC_INTERNAL_ERROR;
		}
		//Get the auto processing user token
		SSDKUserTokenInterface* userToken = NULL;
		status = userInterface->Authenticate(SSDKUserTokenInterface::TOKEN_CONTEXT_AUTOPROCESSING, userToken);
		if(OK != status )
		{
			DEBUGL1("UNABLE TO AUTHENTICATE AND GET SYSTEM USER TOKEN\n");
			return ret;
		}
		myut.ut = userToken;		
	}
	//Normal flow for Authenticate
	else
	{
		//Authenticate the user
		ret = Authenticate(myut, boProxy, LogExternalPtr);
		if(ret!= STATUS_OK)
		{
			DEBUGL2("Import::Authenticate::Failed \n");
			return ret;
		}
	}
	//Parse the xml file
	map<CString,vector<CString> > roleNameAttrVector;
	//Get the xml file name.Name assumed to be DefaultRoleSetting.xml
	CString xmlPath(defaultRoleSetting);
	xmlPath = absFilePath;
	//Check file exists or not
	if(!File::Exists(xmlPath))
	{
		DEBUGL2("Import::File Not found \n");
                return STATUS_AL_SEC_IMPORT_FILE_NOT_FOUND;
	}
	CString otherUser; 
	ret =Utils::ParseRoleMap(xmlPath,roleNameAttrVector,otherUser);
	if(ret != STATUS_OK)
	{
		DEBUGL6("Parsing of xml failed:%s\n",xmlPath.c_str());
		return ret;
	}
	//Clear all old rules
	ret = ClearRules(myut.ut);
        if(STATUS_OK != ret)
        {
                DEBUGL2("Failed to clear old rules\n");
                return ret;
        }
	//Set the other user to SSDK
	ret = SetDefaultRole(myut.ut,otherUser);
	if(ret != STATUS_OK)
	{
		DEBUGL6("Setting other user to SSDK failed:\n");
                return ret;
	}
	//Set the role setting to SSDK
	ret = SetRoleRule(myut.ut,roleNameAttrVector);
	if(ret != STATUS_OK)
	{
		DEBUGL6("Importing to SSDK failed:%s\n",xmlPath.c_str());
                return ret;
	}
	
	
	//Deleting upgradeFileForRuleFlag file. Continued fix for EBX_STFR_18235,EBX_DCR_52523
	if(m_upgradeRoleMappingFlag)
        {
		if(STATUS_OK !=  (ret=File::DeleteFile(upgradeFileForRuleFlag)))
                {
                        DEBUGL2("Failed to create the upgradeFileForRule File. Status : %d \n",ret);
                }
        }
	
	else
	{		
		//Copy the file
		Status fileStatus = STATUS_OK;
		fileStatus =File::CopyFile(xmlPath,currentRoleSetting,true);
		if(STATUS_OK != fileStatus)
		{
			DEBUGL2("Import::File Copy failed \n");
			if(STATUS_DISK_FULL == fileStatus)
				return fileStatus;
			return STATUS_AL_SEC_INTERNAL_ERROR;
		}
	}
	return STATUS_OK;
	
}

Status ImportRoleRuleMapping::ClearRules(const SSDKUserTokenInterface* ut)
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
	vector<SSDKRBACMgmtInterface::RoleInfo> roles;
	//Get all the roles from the ssdk	
	ssdkStatus=rbac->GetProperty(ut,SSDKRBACMgmtInterface::ROLE_ALL_PROPERTIES,"",roles);
	if(OK != ssdkStatus)
	{
		DEBUGL1("\n ssdk SecurityManager::SSDKRBACMgmtInterface GetProperty %d\n",ssdkStatus);
                ret = SSDK_RBAC_MANAGER_PLUGIN_ERROR_MAP(ssdkStatus);
                return ret;
	}
	vector<SSDKRBACMgmtInterface::RoleInfo>::iterator it;
	map<CString,vector<CString> > roleRuleMap;
	for(it = roles.begin(); it != roles.end(); it++)
	{
		roleRuleMap.insert(make_pair<CString,vector<CString> >((*it).m_RoleId,vector<CString>()));
	}
	return SetRoleRule(ut,roleRuleMap);
}
Status ExportRoleRuleMapping::ExecuteSSDK(const CString& absFilePath)
{
	CString exportFileName(absFilePath);
	Status fileStatus = STATUS_OK;
        if(File::Exists(currentRoleSetting))
        {
                fileStatus = File::CopyFile(currentRoleSetting,exportFileName,true);
                if(STATUS_OK != fileStatus)
                {
                        DEBUGL2("Import::File Copy failed \n");
                        if(STATUS_DISK_FULL == fileStatus)
                                return fileStatus;
                        return STATUS_AL_SEC_INTERNAL_ERROR;
                }
        }
        else
        {
                fileStatus = File::CopyFile(defaultRoleSetting,exportFileName,true);
                if(STATUS_OK != fileStatus)
                {
                        DEBUGL2("Import::File Copy failed \n");
                        if(STATUS_DISK_FULL == fileStatus)
                                return fileStatus;
                        return STATUS_AL_SEC_INTERNAL_ERROR;
                }
        }
	return STATUS_OK;
}
Status ImportExportRoleInfo::Execute()
{
	MyUserToken myut;
        BOProxyRef boProxy;
        Ref<SSDKLogInterface> LogExternalPtr=NULL;
        //Authenticate the user
        Status ret=STATUS_OK;
        ret = Authenticate(myut, boProxy, LogExternalPtr);
	if(ret != STATUS_OK)
        {
                DEBUGL2("RoleGetCustomProperty ::Authenticate::Failed \n");
                return ret;
        }
        //Get the absolute file path
        //Get the file storage path
	//Previous implementation
	/*
        ElementRef pfileStorageNode = NULL;
        NodeRef pfileStorageText = NULL;
        pfileStorageNode = m_command->GetParamNode("fileStoragePath");
        if (!(pfileStorageNode) || (!(pfileStorageText = pfileStorageNode->getFirstChild()) || ((sFileStoragePath= pfileStorageText->getNodeValue()).size() == 0) ) )
                DEBUGL2("ImportExportRoleInfo::Execute::::Failed To  node in Command XML\n");
	if(sFileStoragePath.empty())
        {
                DEBUGL1("\n fileStorage Path is empty\n");
                return STATUS_AL_SEC_COMMAND_SCHEMA;
        }
        DEBUGL5("Params fileStoragePath value:: %s\n",sFileStoragePath.c_str());
        //Get the relative path
        ElementRef fileNameNode = NULL;
        NodeRef fileNameText = NULL;
        fileNameNode = m_command->GetParamNode("filename");
        CString sRelativePath;
        if (!(fileNameNode) || (!(fileNameText = fileNameNode->getFirstChild()) || ((sRelativePath= fileNameText->getNodeValue()).size() == 0) ) )
                DEBUGL2("ImportExportRoleInfo::Execute::::Failed To  node in Command XML\n");
	if(sRelativePath.empty())
        {
                DEBUGL1("\n file name is empty\n");
                return STATUS_AL_SEC_COMMAND_SCHEMA;
        }
        DEBUGL5("Params File Name:: %s\n",sRelativePath.c_str());
	//Get the command name
	CString cmdName = m_command -> GetCommandName();
	if("ImportRoleInfo" != cmdName)
		sFileStoragePath = "DataExport";
        //Get the path from file storage DOM
        CString absFolderPath;
	CString xPath = "FileStorages/FileStorage[@selected = '1'][name = '" + sFileStoragePath + "']/path";
	NodeRef domNode = NULL;
	if(boProxy->GetValue(xPath.c_str(),domNode) != STATUS_FAILED)
	{
		if(domNode)
		{
			absFolderPath = domNode->getNodeValue();
			DEBUGL5("\n FileStoragePath :: %s\n",absFolderPath.c_str());
		}
	}
	if(absFolderPath.empty())
	{
		DEBUGL1("No value for XPATH FileStoragePath ::%s \n",xPath.c_str());
                if(m_command)
                {
			if("ImportRoleInfo" == cmdName)
				return STATUS_AL_SEC_IMPORT_FILE_NOT_FOUND;
			else
				return STATUS_AL_SEC_EXPORT_FILE_NOT_FOUND;	
		}
		else
		{
			DEBUGL1("Unable to get the command name\n");
			return STATUS_AL_SEC_INTERNAL_ERROR;	
		}	
	}*/
	CString sFileStoragePath("DataImport");
        ElementRef pfileStorageNode = NULL;
        NodeRef pfileStorageText = NULL;
        pfileStorageNode = m_command->GetParamNode("filePath");
        if (!(pfileStorageNode) || (!(pfileStorageText = pfileStorageNode->getFirstChild()) || ((sFileStoragePath= pfileStorageText->getNodeValue()).size() == 0) ) )
                DEBUGL2("ImportExportRole::Execute::::Failed To  node in Command XML\n");
        DEBUGL8("Params fileStoragePath value:: %s\n",sFileStoragePath.c_str());
        if(sFileStoragePath.empty())
        {
                DEBUGL1("\n fileStorage Path is empty\n");
                return STATUS_AL_SEC_COMMAND_SCHEMA;
        }
        //Get the relative path
        ElementRef fileNameNode = NULL;
        NodeRef fileNameText = NULL;
        fileNameNode = m_command->GetParamNode("fileName");
        CString sRelativePath;
        if (!(fileNameNode) || (!(fileNameText = fileNameNode->getFirstChild()) || ((sRelativePath= fileNameText->getNodeValue()).size() == 0) ) )
                DEBUGL2("ImportExportRole::Execute::::Failed To  node in Command XML\n");
        if(sRelativePath.empty())
        {
                DEBUGL1("\nFile name is empty\n");
                return STATUS_AL_SEC_COMMAND_SCHEMA;
        }
        DEBUGL8("Params relativePath value:: %s\n",sRelativePath.c_str());

        CString absFilePath;
        absFilePath=sFileStoragePath+"/"+sRelativePath;
        ret = ExecuteSSDK(absFilePath);
	if(STATUS_OK == ret)
	{
		/*
		if(operationType == "Import")
			Utils::WriteLog(NOTIFICATION_IMPORT_SUCCESS,true,ci::logmanager::LogInterface::opAdd,ci::logmanager::LogInterface::otRole,ci::logmanager::LogInterface::mtMaintenance);
		else if(operationType == "Export")	
			Utils::WriteLog(NOTIFICATION_EXPORT_SUCCESS,true,ci::logmanager::LogInterface::opAdd,ci::logmanager::LogInterface::otRole,ci::logmanager::LogInterface::mtMaintenance);*/
	}
	else
	{
		/*
		if(operationType == "Import")
			Utils::WriteLog(NOTIFICATION_IMPORT_FAILURE,false,ci::logmanager::LogInterface::opAdd,ci::logmanager::LogInterface::otRole,ci::logmanager::LogInterface::mtMaintenance);
                else if(operationType == "Export")
			Utils::WriteLog(NOTIFICATION_EXPORT_FAILURE,false,ci::logmanager::LogInterface::opAdd,ci::logmanager::LogInterface::otRole,ci::logmanager::LogInterface::mtMaintenance);*/
	}
	return ret;
}

Status ImportExportRoleInfo::GetRoleInfo(const SSDKUserTokenInterface* userToken,vector<SSDKRBACMgmtInterface::RoleInfo>& roleInfoAllRoles)
{
        ssdk::SSDKStatus ssdkStatus;
	Status ret = STATUS_FAILED;
        Ref<SSDKSecurityManagerInterface> ssdkSecurityManagerInterface = ssdk::GetSecurityManager();
        //Get the rbac interface
        Ref<SSDKRBACMgmtInterface> rbac = dynamic_cast<SSDKRBACMgmtInterface*> (ssdkSecurityManagerInterface->GetInterface  (SSDKSecurityManagerInterface::RBAC_MGMT_INTERFACE,ssdkStatus));
        if(rbac==(void*) NULL || ssdkStatus != OK)
        {
                DEBUGL1("\n ssdk SecurityManager::SSDKRBACMgmtInterface Failed \n");
                return STATUS_AL_SEC_INTERNAL_ERROR;
        }
	//Get the role all properties
	ssdkStatus = rbac->GetProperty(userToken, SSDKRBACMgmtInterface::ROLE_ALL_PROPERTIES,CString(""), roleInfoAllRoles);
	if(OK!=ssdkStatus)
	{
		DEBUGL1("SSDK::Get Role All Properties Failed Status:%d\n",ssdkStatus);
		ret = SSDK_RBAC_MANAGER_PLUGIN_ERROR_MAP(ssdkStatus);
		return ret;
	}
	return STATUS_OK;
}

Status ImportRoleInfo::ExecuteSSDK(const CString& absFilePath)
{
	MyUserToken myut;
        BOProxyRef boProxy;
        Ref<SSDKLogInterface> LogExternalPtr=NULL;
	Status ret = STATUS_FAILED;
	ssdk::SSDKStatus ssdkStatus = OK;
	ssdk::SSDKStatus roleOpStatus = OK;

        //Authenticate the user
        ret = Authenticate(myut, boProxy, LogExternalPtr);
	if(ret != STATUS_OK)
        {
                DEBUGL2("RoleGetCustomProperty ::Authenticate::Failed \n");
                return ret;
        }
	Ref<SSDKSecurityManagerInterface> ssdkSecurityManagerInterface = ssdk::GetSecurityManager();
        //Get the rbac interface
        Ref<SSDKRBACMgmtInterface> rbac = dynamic_cast<SSDKRBACMgmtInterface*> (ssdkSecurityManagerInterface->GetInterface  (SSDKSecurityManagerInterface::RBAC_MGMT_INTERFACE,ssdkStatus));
        if(rbac==(void*) NULL || ssdkStatus != OK)
        {
                DEBUGL1("\n ssdk SecurityManager::SSDKRBACMgmtInterface Failed \n");
                return STATUS_AL_SEC_INTERNAL_ERROR;
        }

	//Get all the roles. If not built-in delete it
	vector<SSDKRBACMgmtInterface::RoleInfo> roleInfoAllRoles;
	ret = GetRoleInfo(myut.ut,roleInfoAllRoles);
	if(ret != STATUS_OK)
	{
		DEBUGL1("\n Get role all properties failed\n");
		return ret;
	}
	vector<SSDKRBACMgmtInterface::RoleInfo>::iterator roleInfoIterator;
	for(roleInfoIterator = roleInfoAllRoles.begin();roleInfoIterator != roleInfoAllRoles.end() ; roleInfoIterator++)
	{
		if(!Utils::IsBuiltinRole(roleInfoIterator->m_RoleId))
		{
			//Delete the non built in roles
			DEBUGL1("Deleting role roleID:%s\n",(*roleInfoIterator).m_RoleId.c_str());
			ssdkStatus = rbac->RBACRoleOperation(myut.ut,SSDKRBACMgmtInterface::ROLE_REMOVE,(*roleInfoIterator));
			if(OK != ssdkStatus)
			{
				DEBUGL1("\n Remove role failed::%d\n",ssdkStatus);
				ret = SSDK_RBAC_MANAGER_PLUGIN_ERROR_MAP(ssdkStatus);
				return ret;
			}	
		}
	}	
	
	//Parse the XML file
	
	CString path("RBACManager/Roles");
        //Create a temp document
        dom::DocumentRef tempDoc=NULL;
        ci::operatingenvironment::Ref<ci::hierarchicaldb::HierarchicalDB>  pHDB = NULL;
        pHDB = ci::hierarchicaldb::HierarchicalDB::Acquire(NULL);
        if (!pHDB)
        {
                DEBUGL1("HDB ACQUIRE FAILED\n");
                return STATUS_AL_SEC_INTERNAL_ERROR;
        }
        DEBUGL6("HDB ACQUIRE SUCCESS\n");
        if(STATUS_OK != (ret = pHDB->CreateTempDocumentFromFile(tempDoc,absFilePath)))
        {		
                DEBUGL1("CREATION OF TEMP DOC FAILED\n");
		if(STATUS_HDB_ERROR_PARSING_XML == ret)
			return	STATUS_AL_SEC_BAD_REQUEST;
                return STATUS_AL_SEC_INTERNAL_ERROR;
        }
        DEBUGL6("TEMP DOC CREATION SUCCESS\n");
        dom::NodeRef domNode = pHDB->BindToElement(tempDoc,path.c_str());
        if(!domNode)
        {
                DEBUGL1("BIND TO ROLE FAILED\n");
                return STATUS_AL_SEC_INTERNAL_ERROR;
        }
        DEBUGL6("BINDING PASSED\n");
	ci::operatingenvironment::Ref<ci::hierarchicaldb::NodeList> roleNodeList;
	CString role("Role");
	roleNodeList=pHDB->FindNodes(domNode,role);
        if(!roleNodeList)
        {
                DEBUGL1("Bind to RoleSet failed\n");
                return STATUS_AL_SEC_INTERNAL_ERROR;
        }
	int roleListSize = static_cast<int>(roleNodeList ->size());
	Role roleObj;
	for(int i=0;i<roleListSize;i++)
	{
		//Check number of roles
		vector<SSDKRBACMgmtInterface::RoleInfo> allRoles;
		ssdkStatus = rbac->GetProperty(myut.ut,SSDKRBACMgmtInterface::ROLE_ID_PROPERTY,"",allRoles);
		if(ssdkStatus !=OK)
		{
			DEBUGL1("Failed in GetProperty(ROLE_ID_PROPERTY) with Status ::%d\n",ssdkStatus);
			ret = SSDK_RBAC_MANAGER_PLUGIN_ERROR_MAP(ssdkStatus); 
			return ret;
		}
		unsigned int totalRole = allRoles.size();
		if(totalRole >= MAX_POSSIBLE_ROLE)
		{
			DEBUGL2("Maximum Role Creation Limit is reached\n");
			return STATUS_AL_SEC_IMPORT_PARTIAL_ROLES_SUCCESS;
		}

		NodeRef roleNode = roleNodeList->item(static_cast<unsigned long>(i));
		//Set the roleID
		SSDKRBACMgmtInterface::RoleInfo roleInfoStructure;
		NodeRef roleName = pHDB->BindToElement(roleNode,"Information/name",false);
		if(!roleName)
		{
			DEBUGL1("ROle Name not found\n");
			return STATUS_AL_SEC_INTERNAL_ERROR;
		}
		roleInfoStructure.m_RoleId = roleName->getTextContent();
		DEBUGL8("Role Name :%s\n",roleInfoStructure.m_RoleId.c_str());
		if(Utils::IsBuiltinRole(roleInfoStructure.m_RoleId))
			continue;
		ElementRef tempElem = roleNode;
		CString roleUniqueID = tempElem->getAttribute("ID");
                if(roleUniqueID.empty())
                {
                        DEBUGL1("roleID not found\n");
                        return STATUS_AL_SEC_INTERNAL_ERROR;
                }
		roleInfoStructure.m_RoleUniqueID = roleUniqueID;
		//Get the permission map
		map<CString,bool> permissionsMap;
		ret = ReadPermissions(roleNode,"",permissionsMap,false);
		if(ret != STATUS_OK)
		{
			DEBUGL1("\n Invalid Permission List\n");
			return ret;		
		}
		map<CString,bool>::iterator permIterator;
		for(permIterator=permissionsMap.begin();permIterator!=permissionsMap.end();permIterator++)
		{
			ret = roleObj.SetEnableDisablePermission(roleInfoStructure.m_RolePermissions,permIterator->first,permIterator->second);
			if(ret != STATUS_OK)
			{
				DEBUGL1("\n SetEnableDisablePermission failed\n");
				return ret;
			}
		}
		//Set the permission to SSDK
		ssdkStatus = rbac->RBACRoleOperation(myut.ut,SSDKRBACMgmtInterface::ROLE_CREATE,roleInfoStructure);
		if(OK != ssdkStatus)
		{
			DEBUGL2("Role creation failed for role:%s\n",roleInfoStructure.m_RoleId.c_str());
			DEBUGL2("\n SSDK Role Creation failed. Status:%d\n", ssdkStatus);
			roleOpStatus = ssdkStatus;
			continue;
			//ret = SSDK_RBAC_MANAGER_PLUGIN_ERROR_MAP(ssdkStatus);
			//return ret;
		}
		//Set as admin created role
		ssdkStatus=rbac->SetProperty(myut.ut,SSDKRBACMgmtInterface::RBAC_ROLE_OBJECT,roleInfoStructure.m_RoleId,IS_ADMIN_ROLE,"true");//DBCORR
                if(ssdkStatus == AUTH_RBAC_DB_CORRUPTED)
                {
                        DEBUGL2("SSDK RBAC DB CRRUPTED\n");
                        Utils::CheckForCorruption(ssdkStatus);
                        return STATUS_AL_SEC_DB_CORRUPTED;
                }
                if(OK!=ssdkStatus)
                {
                        DEBUGL2("SETTING IsAdminRole Property Failed with Status :: %d\n",ssdkStatus);
                }
	}
	if(OK != roleOpStatus)
	{
		return STATUS_AL_SEC_IMPORT_PARTIAL_ROLES_SUCCESS;
	}
		
	return STATUS_OK;
}

Status ExportRoleInfo::ExecuteSSDK(const CString& absFilePath)
{
	MyUserToken myut;
        BOProxyRef boProxy;
        Ref<SSDKLogInterface> LogExternalPtr=NULL;
        Status ret=STATUS_FAILED;
        //Authenticate the user
        ret = Authenticate(myut, boProxy, LogExternalPtr);
	if(ret != STATUS_OK)
        {
                DEBUGL2("Import::Authenticate::Failed \n");
                return ret;
        }
	ssdk::SSDKStatus ssdkStatus;
        Ref<SSDKSecurityManagerInterface> ssdkSecurityManagerInterface = ssdk::GetSecurityManager();
        //Get the rbac interface
        Ref<SSDKRBACMgmtInterface> rbac = dynamic_cast<SSDKRBACMgmtInterface*> (ssdkSecurityManagerInterface->GetInterface  (SSDKSecurityManagerInterface::RBAC_MGMT_INTERFACE,ssdkStatus));
        if(rbac==(void*) NULL || ssdkStatus != OK)
        {
                DEBUGL1("\n ssdk SecurityManager::SSDKRBACMgmtInterface Failed \n");
                return STATUS_AL_SEC_INTERNAL_ERROR;
        }
	//Get All the role all Properties
	Role roleObj;
	vector<SSDKRBACMgmtInterface::RoleInfo> roleInfoAllRoles;
	ret = GetRoleInfo(myut.ut, roleInfoAllRoles);
	if(ret != STATUS_OK)
	{
		DEBUGL1("\n Get ALL role properties failed\n");
		return ret;
	}
	vector<SSDKRBACMgmtInterface::RoleInfo>::iterator roleIterator;
	CString permXML("<RBACManager><Roles>");
	//Itearate through all the roles
	for(roleIterator = roleInfoAllRoles.begin();roleIterator != roleInfoAllRoles.end(); roleIterator++)
	{
			//Check the permission
			CategoryDisplayNameMap::iterator iterDisObj;
			//Get all npn builtin roles
			if(Utils::IsBuiltinRole(roleIterator->m_RoleId))
				continue;
			SSDKRBACMgmtInterface::RoleInfo tempRoleInfo;
			tempRoleInfo = rbac->GetProperty(myut.ut, SSDKRBACMgmtInterface::ROLE_ALL_PROPERTIES,roleIterator->m_RoleId,ssdkStatus);
			if(OK != ssdkStatus)
			{
				DEBUGL1("Get Unique id failed:%d\n",ssdkStatus);
				return STATUS_AL_SEC_INTERNAL_ERROR;
			}	
			//Put the role Id
			DEBUGL8("Role Unique id:%s\n",tempRoleInfo.m_RoleUniqueID.c_str());
			CString tempRoleId(roleIterator->m_RoleId);
			EbxSSDKUtil::CheckXMLEntityAndReplaceString(tempRoleId);
			permXML +="<Role ID='" + tempRoleInfo.m_RoleUniqueID+"'><Information><name>"+tempRoleId+"</name>";		
			permXML +="<Permissions>";
			for(iterDisObj=CRoleManager::displayObjectMap.begin();iterDisObj!=CRoleManager::displayObjectMap.end();++iterDisObj)
			{
				DEBUGL8("Permission is = %s\n", (iterDisObj->first).c_str());
				Status status = STATUS_OK;
				if(roleObj.CheckPermissionEnable(roleIterator->m_RolePermissions,iterDisObj->first,status))
				{
					permXML += "<permission isEnable='true'>" + iterDisObj->first + "</permission>";
				}
			}
			permXML = permXML + "</Permissions></Information></Role>";
	}		
	permXML += "</Roles></RBACManager>";
	DEBUGL8("Permission XML :: %s\n",permXML.c_str());
	//Create a file
	FilePtr outputFile;
	if(File::Exists(absFilePath))
        {
                if(STATUS_OK != File::DeleteFile(absFilePath))
                {
                        DEBUGL1("Unable to delete the old file\n");
                        return STATUS_AL_SEC_INTERNAL_ERROR;
                }
        }
	outputFile = File::CreateFile(absFilePath,FolderPtr(), "w");
	if(!outputFile)
	{
		DEBUGL1("Export Role Info::File Creation failed\n");
		return STATUS_AL_SEC_INTERNAL_ERROR;
	}
	Status fileStatus = STATUS_OK;
	fileStatus = outputFile->Write(permXML.c_str());
	if(STATUS_OK != fileStatus)
	{
		DEBUGL1("\n ExportRoleInfo::File write failed \n");
		if(STATUS_DISK_FULL == fileStatus)
			return fileStatus;
		outputFile->Close();
		return STATUS_AL_SEC_INTERNAL_ERROR;
	}
	fileStatus = outputFile->Close();
	if(STATUS_OK != fileStatus)
	{
		DEBUGL1("\n ExportRoleInfo::File close failed \n");
		if(STATUS_DISK_FULL == fileStatus)
			return fileStatus;
		return STATUS_AL_SEC_INTERNAL_ERROR;
	}
		
	return STATUS_OK;
}

Status RbacServerSettings::Execute()
{
	//Get the root path for default role, server id, type from DOM
	CString path;
	ElementRef pathNode = NULL;
        NodeRef pathText = NULL;
        pathNode = m_command->GetParamNode("settingDetails");
        if (!(pathNode) || (!(pathText = pathNode->getFirstChild()) || ((path =  pathText->getNodeValue()).size() == 0) ) )
	{
                DEBUGL2("RbacServerSettings::Execute::::Failed To  node in Command XML\n");
		return STATUS_AL_SEC_INTERNAL_ERROR;
	}
	return ExecuteSSDK(path);	
}

Status GetRbacServerSettings::ExecuteSSDK(const CString& path)
{
	MyUserToken myut;
        BOProxyRef boProxy;
        Ref<SSDKLogInterface> LogExternalPtr=NULL;
        Status ret=STATUS_FAILED;
        //Authenticate the user
        ret = Authenticate(myut, boProxy, LogExternalPtr);
	if(ret != STATUS_OK)
        {
                DEBUGL2("Import::Authenticate::Failed \n");
                return ret;
        }
	ssdk::SSDKStatus ssdkStatus;
        Ref<SSDKSecurityManagerInterface> ssdkSecurityManagerInterface = ssdk::GetSecurityManager();
        //Get the Config interface
        Ref<SSDKConfigInterface> configPtr = dynamic_cast<SSDKConfigInterface*> (ssdkSecurityManagerInterface->GetInterface  (SSDKSecurityManagerInterface::CONFIG_INTERFACE,ssdkStatus));
        if(configPtr ==(void*) NULL || ssdkStatus != OK)
        {
                DEBUGL1("\n ssdk SecurityManager::SSDKConfigInterface Failed \n");
                return STATUS_AL_SEC_INTERNAL_ERROR;
        }
	//Get the rbacType
	CString rbacTypeValue = configPtr->GetProperty(myut.ut,SSDKConfigInterface::SA_SERVAUTHEXTERNRBACENABLE,ssdkStatus);
	if(OK != ssdkStatus)
	{
		DEBUGL1("Get value from config failed:%d\n",ssdkStatus);
		ret = SSDK_RBAC_MANAGER_PLUGIN_ERROR_MAP(ssdkStatus);
		return ret;
	}
	//Get the default role
	CString defaultRoleValue = configPtr->GetProperty(myut.ut,SSDKConfigInterface::SA_SERVAUTHRBACDEFAULTROLE,ssdkStatus);
        if(OK != ssdkStatus)
        {
                DEBUGL1("Get value from config failed:%d\n",ssdkStatus);
                ret = SSDK_RBAC_MANAGER_PLUGIN_ERROR_MAP(ssdkStatus);
                return ret;
        }
	//Get the rbac server id from SRAM
	CString rbacServerIdValue;
	uint64 key = IndexedDB::CreateKey(SSDK_SRAM_CODE_TYPE,SSDK_SRAM_RBAC_SERVER_ID_CODE,SSDK_SRAM_SUB_CODE);
	if(!Utils::GetInt32ValueFromSRAM(key,rbacServerIdValue))
	{
		DEBUGL1("Get value from SRAM failed\n");
		return STATUS_AL_SEC_INTERNAL_ERROR;
	}
	//Set all the values to transaction doc
	CString rbacTypePath = path+"/rbacType";
	//Convert the value to true or false
	if(rbacTypeValue == "0")
		rbacTypeValue = "false";
	else
		rbacTypeValue = "true"; 
	if(STATUS_OK != Utils::SetValueToPath(boProxy,rbacTypePath,rbacTypeValue))
	{
		DEBUGL1("Unable to set value to boproxy\n");
		return STATUS_AL_SEC_INTERNAL_ERROR;
	}			
	CString rbacServerID = path+"/rbacServerID";
        if(STATUS_OK != Utils::SetValueToPath(boProxy,rbacServerID,rbacServerIdValue))
        {
                DEBUGL1("Unable to set value to boproxy\n");
                return STATUS_AL_SEC_INTERNAL_ERROR;
        }
	CString defaultRolePath = path+"/defaultRole";
        if(STATUS_OK != Utils::SetValueToPath(boProxy,defaultRolePath,defaultRoleValue))
        {
                DEBUGL1("Unable to set value to boproxy\n");
                return STATUS_AL_SEC_INTERNAL_ERROR;
        }

	//Fetch EWBAccess and USB DirectPrint permission enable flag from config.
	CString ewbAccessPermFromConfig(""),usbPrintPermFromConfig("");

	ewbAccessPermFromConfig = configPtr->GetProperty(myut.ut,SSDKConfigInterface::SA_RBACPERMEWBACCESSENABLE,ssdkStatus);
	if(OK != ssdkStatus)
	{
		DEBUGL1("Get value from config failed:%d\n",ssdkStatus);
		ret = SSDK_RBAC_MANAGER_PLUGIN_ERROR_MAP(ssdkStatus);
		return ret;
	}
	usbPrintPermFromConfig = configPtr->GetProperty(myut.ut,SSDKConfigInterface::SA_RBACPERMUSBPRINTENABLE,ssdkStatus);
	if(OK != ssdkStatus)
	{
		DEBUGL1("Get value from config failed:%d\n",ssdkStatus);
		ret = SSDK_RBAC_MANAGER_PLUGIN_ERROR_MAP(ssdkStatus);
		return ret;
	}
        CString ewbAccessPerm = (ewbAccessPermFromConfig == "1")?"true":"false";
        CString usbPrintPerm = (usbPrintPermFromConfig == "1")?"true":"false";

	//Check if the value of flags in RbacSettings dom is in sync with config.map
	map<CString,CString> fetchMap,setMap;
	fetchMap.insert(pair<CString,CString>(XPATH_RBACSETTINGS_EWB_ACCESS,""));
	fetchMap.insert(pair<CString,CString>(XPATH_RBACSETTINGS_USB_DIRECT_PRINT,""));
	if(STATUS_OK != Utils::GetValuesFromRbacSettingsDOM(fetchMap))
	{
		DEBUGL1("Get value from RbacSettings dom failed\n");
		return STATUS_FAILED;
	}
	if(fetchMap[XPATH_RBACSETTINGS_EWB_ACCESS] != ewbAccessPerm)
	{
		DEBUGL6("Value of EWB Access flag in RbacSettings dom is not in sync with config.map\n");
		setMap[XPATH_RBACSETTINGS_EWB_ACCESS] = ewbAccessPerm;
	}
	if(fetchMap[XPATH_RBACSETTINGS_USB_DIRECT_PRINT] != usbPrintPerm)
	{
		DEBUGL6("Value of USB Direct Print flag in RbacSettings dom is not in sync with config.map\n");
		setMap[XPATH_RBACSETTINGS_USB_DIRECT_PRINT] = usbPrintPerm;
	}
	if((!setMap.empty()) && (STATUS_OK != Utils::SetValuesToRbacSettingsDOM(boProxy,setMap))) // Set the value of flag present in config to RbacSettings dom.
	{
		DEBUGL1("Set value to RbacSettings dom failed\n");
		return STATUS_FAILED;
	}

	//Set values to session dom
	CString ewbAccessPermPath = path+EWB_ACCESS_PATH;
	if(STATUS_OK != Utils::SetValueToPath(boProxy,ewbAccessPermPath,ewbAccessPerm))
	{
		DEBUGL1("Unable to set value to boproxy\n");
		return STATUS_AL_SEC_INTERNAL_ERROR;
	}
	CString usbPrintPermPath = path+USB_DIRECT_PRINT_PATH;
	if(STATUS_OK != Utils::SetValueToPath(boProxy,usbPrintPermPath,usbPrintPerm))
	{
		DEBUGL1("Unable to set value to boproxy\n");
		return STATUS_AL_SEC_INTERNAL_ERROR;
	}
	return STATUS_OK;
}


Status SetRbacServerSettings::ExecuteSSDK(const CString& path)
{
	//Get the values from transaction doc
	MyUserToken myut;
	BOProxyRef boProxy;
	Ref<SSDKLogInterface> LogExternalPtr=NULL;
	Status ret=STATUS_FAILED;
	//Authenticate the user
	ret = Authenticate(myut, boProxy, LogExternalPtr);
	if(ret != STATUS_OK)
	{
		DEBUGL2("Import::Authenticate::Failed \n");
		return ret;
	}
	ssdk::SSDKStatus ssdkStatus;
	Ref<SSDKSecurityManagerInterface> ssdkSecurityManagerInterface = ssdk::GetSecurityManager();
	//Get the Config interface
	Ref<SSDKConfigInterface> configPtr = dynamic_cast<SSDKConfigInterface*> (ssdkSecurityManagerInterface->GetInterface  (SSDKSecurityManagerInterface::CONFIG_INTERFACE,ssdkStatus));
	if(configPtr ==(void*) NULL || ssdkStatus != OK)
	{
		DEBUGL1("\n ssdk SecurityManager::SSDKConfigInterface Failed \n");
		return STATUS_AL_SEC_INTERNAL_ERROR;
	}

	CString isFactoryDefault("");
	ElementRef pathNode = NULL;
	NodeRef pathText = NULL;
	pathNode = m_command->GetParamNode("factoryDefault");
	if (pathNode) 
	{
		if(pathText = pathNode->getFirstChild())
			isFactoryDefault =  pathText->getNodeValue();
	}

	if(isFactoryDefault == "true")
	{
		DEBUGL8("Command recieved to Reset Extended permissions RbacSettings values\n");
		CString rbacTypeValue,rbacServerIdValue,ewbAccessPerm(""),usbPrintPerm("");
	        ci::operatingenvironment::Ref<ci::hierarchicaldb::HierarchicalDB>  pHDB = NULL;
        	pHDB = ci::hierarchicaldb::HierarchicalDB::Acquire(NULL);
	        if (!pHDB)
        	{
                	DEBUGL1("HDB acquire failed");
	                return STATUS_AL_SEC_INTERNAL_ERROR;
        	}
	        NodeRef domNode = NULL;
        	CString xpath = path ;
	        if(boProxy->GetValue(xpath.c_str(),domNode) != STATUS_FAILED)
        	{
			map<CString,CString> setMap;

			NodeRef ewbAccessNode = pHDB -> BindToElement (domNode,EWB_ACCESS_PATH,false);
			if (ewbAccessNode)
			{	
				DEBUGL8("ewbAccessNode Exists\n");
				//Set Default values for Extended permissions in RbacSettings dom and Config
				CString ewbAccessPermToConfig("1"),ewbAccessPerm("true");
				ssdkStatus = configPtr->SetProperty(myut.ut,SSDKConfigInterface::SA_RBACPERMEWBACCESSENABLE,ewbAccessPermToConfig);
				if(OK!=ssdkStatus)
				{
					DEBUGL1("Not able to set Err::%d \n",ssdkStatus);
					ret = SSDK_RBAC_MANAGER_PLUGIN_ERROR_MAP(ssdkStatus);
					return ret;
				}
				setMap.insert(pair<CString,CString>(XPATH_RBACSETTINGS_EWB_ACCESS,ewbAccessPerm));
			}
			NodeRef usbPrintNode = pHDB -> BindToElement (domNode,USB_DIRECT_PRINT_PATH,false);
	                if(usbPrintNode)
			{
				DEBUGL8("usbPrintNode Exists\n");
				CString usbPrintPermToConfig("1"),usbPrintPerm("true");
				ssdkStatus = configPtr->SetProperty(myut.ut,SSDKConfigInterface::SA_RBACPERMUSBPRINTENABLE,usbPrintPermToConfig);
				if(OK!=ssdkStatus)
				{
					DEBUGL1("Not able to set Err::%d \n",ssdkStatus);
					ret = SSDK_RBAC_MANAGER_PLUGIN_ERROR_MAP(ssdkStatus);
					return ret;
				}
				setMap.insert(pair<CString,CString>(XPATH_RBACSETTINGS_USB_DIRECT_PRINT,usbPrintPerm));
			}
			if((!setMap.empty()) && (STATUS_OK != Utils::SetValuesToRbacSettingsDOM(boProxy,setMap)))
			{
				DEBUGL1("Set value to RbacSettings dom failed\n");
				return STATUS_FAILED;
			}
		}
		else
	        {
        	        DEBUGL1("Unable to bind to %s/RbacSettings\n",path.c_str());
                	return STATUS_AL_SEC_INTERNAL_ERROR;
        	}
		return STATUS_OK;
		
	}
	CString rbacTypeValue,rbacServerIdValue,ewbAccessPerm(""),usbPrintPerm("");
	ci::operatingenvironment::Ref<ci::hierarchicaldb::HierarchicalDB>  pHDB = NULL;
        pHDB = ci::hierarchicaldb::HierarchicalDB::Acquire(NULL);
        if (!pHDB)
        {
                DEBUGL1("HDB acquire failed");
		return STATUS_AL_SEC_INTERNAL_ERROR;
        }
        NodeRef domNode = NULL;
	CString xpath = path ;
	if(boProxy->GetValue(xpath.c_str(),domNode) != STATUS_FAILED)
        {
		NodeRef ewbAccessNode = pHDB -> BindToElement (domNode,EWB_ACCESS_PATH,false);
		if(ewbAccessNode)
			ewbAccessPerm = ewbAccessNode->getTextContent();
		NodeRef usbPrintNode = pHDB -> BindToElement (domNode,USB_DIRECT_PRINT_PATH,false);
		if(usbPrintNode)
			usbPrintPerm = usbPrintNode->getTextContent();
		//Get Rbactype	
		CString rbacType("rbacType");
		NodeRef rbacTypeNode = pHDB -> BindToElement( domNode , rbacType , false );	
		//Must present node
		if(rbacTypeNode)
			rbacTypeValue = rbacTypeNode->getTextContent();
		//Get rbacServerID and default Role . Optional Nodes
		CString rbacServerId("rbacServerID");
		NodeRef rbacServerIdNode = pHDB -> BindToElement( domNode , rbacServerId , false );
		if(rbacServerIdNode &&  !rbacTypeNode) // RbacServerID set, but not type
		{
			DEBUGL1("Node rbacType is not found\n");
			return STATUS_AL_SEC_INTERNAL_ERROR;

		}
		if(rbacServerIdNode)
			rbacServerIdValue = rbacServerIdNode->getTextContent();
		/*
		CString defaultRole("defaultRole");
                NodeRef defaultRoleNode = pHDB -> BindToElement( domNode , defaultRole , false );
                if(defaultRoleNode)
                        defaultRoleValue = defaultRoleNode->getTextContent();*/
	}
	else
	{
		DEBUGL1("Unable to bind to %s/RbacSettings\n",path.c_str());
		return STATUS_AL_SEC_INTERNAL_ERROR;
	}
	
	#if 0	
	//check whether id exists in Network Dom
        Ref<Moniker> pOrgDocumentMoniker = 0;
        Ref<Moniker> pDeltaDocumentMoniker = 0;
        DocumentRef pDeltaDoc = NULL;
        CString sOrgDocName = "Network"; //Root node of the document to be opened
        ret = boProxy->getDocumentMonikers(sOrgDocName,pOrgDocumentMoniker,pDeltaDocumentMoniker);
        if((ret!= OK) || !pOrgDocumentMoniker)
        {
                DEBUGL1("Failed to get Original DocumentMonikers for Network Document\n");
                return STATUS_AL_SEC_INTERNAL_ERROR;
        }
        sOrgDocName = pOrgDocumentMoniker->GetGID();
        DEBUGL8("OriginalDocName  == %s \n",sOrgDocName.c_str());
        if(CRoleManager::m_pHDB->OpenDocument(sOrgDocName,pDeltaDoc) == STATUS_OK)
        {
                DocumentGuard domGuard(pDeltaDoc);
                if(STATUS_OK != domGuard.ReadLock(true))
                {
                        DEBUGL1("Failed to Acquire Read Lock on Network DOM\n");
                        return STATUS_AL_SEC_INTERNAL_ERROR;
                }
                ci::operatingenvironment::Ref<dom::Element> directoryElemNode = NULL;
                CString  directoryServiceXpath = XPATH_DIRECTORY_SERVICE_NAME + rbacServerIdValue + "'" +"]";
                directoryElemNode=CRoleManager::m_pHDB->BindToElement(pDeltaDoc,directoryServiceXpath);
                if(!directoryElemNode)
                {
			DEBUGL1("BindToElement() Element failed for the Node %s\n",directoryServiceXpath.c_str());
                        return STATUS_AL_SEC_INVALID_INPUT;
		}
		else
			DEBUGL8("Server Id Present in Network Dom");
		directoryElemNode = NULL;
                domGuard.Release();
	}
	#endif //0
	
        //Set the value to SSDK
	//Convert the value to true or false
	if(rbacTypeValue != "")
	{
		bool bOut;
		if(!EbxSSDKUtil::validateInput<bool>(rbacTypeValue,bOut))
		{
			DEBUGL1("Invalid RbacType Value \n");
			return STATUS_AL_SEC_INVALID_INPUT;
		}
	}
        if(rbacTypeValue == "true")
                rbacTypeValue = "1";
        else
                rbacTypeValue = "0";
        ssdkStatus = configPtr->SetProperty(myut.ut,SSDKConfigInterface::SA_SERVAUTHEXTERNRBACENABLE,rbacTypeValue);
        if(OK!=ssdkStatus)
        {
                DEBUGL1("Not able to set Err::%d \n",ssdkStatus);
                ret = SSDK_RBAC_MANAGER_PLUGIN_ERROR_MAP(ssdkStatus);
                return ret;
        }
	//Set rbacServerId to SRAM
	if(!rbacServerIdValue.empty())
	{
		int iOut;
		if(!EbxSSDKUtil::validateInput<int>(rbacServerIdValue,iOut))
		{
			DEBUGL1("Invalid Rbac ServerId Value \n");
			return STATUS_AL_SEC_INVALID_INPUT;
		}
		uint64 key = IndexedDB::CreateKey(SSDK_SRAM_CODE_TYPE,SSDK_SRAM_RBAC_SERVER_ID_CODE,SSDK_SRAM_SUB_CODE);
		if(!Utils::SetInt32ValueToSRAM(key, rbacServerIdValue))
		{
			DEBUGL1("\n Set value to SRAM failed \n");
			return STATUS_AL_SEC_INTERNAL_ERROR;
		}
	}
	//Check for rbactype and Import role rule mapping file
	CString defaultRoleValue(EXTERNAL_RBAC_DEFAULT_ROLE);
	if(rbacTypeValue == "1" && File::Exists(currentRoleSetting))
	{
		RoleRuleMap roleRuleMapping;
		CString otherUser;
		ret = Utils::ParseRoleMap(currentRoleSetting,roleRuleMapping,otherUser);
		if(ret != STATUS_OK)
		{
			DEBUGL1("Parsing of imported role rule map failed\n");
			return ret;
		}
		//Set SA_SERVAUTHRBACDEFAULTROLE to User when there is no AnyOtherUser tag as info given by MSM
		if((otherUser != "EMPTY") && (!otherUser.empty()))
			defaultRoleValue = otherUser;
		DEBUGL8("\n RbacType = 1 and Setting default role to ::%s",defaultRoleValue.c_str());
		ssdkStatus = configPtr->SetProperty(myut.ut,SSDKConfigInterface::SA_SERVAUTHRBACDEFAULTROLE,defaultRoleValue);
		if(OK!=ssdkStatus)
		{	
			DEBUGL1("Not able to set Err::%d \n",ssdkStatus);
			ret = SSDK_RBAC_MANAGER_PLUGIN_ERROR_MAP(ssdkStatus);
			return ret;
		}
		
	}
	else if(rbacTypeValue == "0" && File::Exists(currentRoleSetting))
	{
		DEBUGL8("\n RbacType = 0 and Setting default role to ::User");
		ssdkStatus = configPtr->SetProperty(myut.ut,SSDKConfigInterface::SA_SERVAUTHRBACDEFAULTROLE,DEFAULT_ROLE_NAME);
                if(OK!=ssdkStatus)
                {
                        DEBUGL1("Not able to set Err::%d \n",ssdkStatus);
                        ret = SSDK_RBAC_MANAGER_PLUGIN_ERROR_MAP(ssdkStatus);
                        return ret;
                }
	}
	ci::logmanager::LogInterface::OperationApplicationType oprApplication;
	CString loggedInUser = myut.ut->GetProperty(SSDKUserTokenInterface::USER_ID_PROPERTY, ssdkStatus);
	if(ssdkStatus != OK)
	{
		DEBUGL1("Failed to Get the user name from user token %d\n", ssdkStatus);
	}
	DEBUGL8("The Logged in User is %s\n", loggedInUser.c_str());		
	CString loggedInUserDomainName = myut.ut->GetProperty(SSDKUserTokenInterface::USER_DOMAIN_PROPERTY, ssdkStatus);
        if(ssdkStatus != OK)
        {
                DEBUGL1("Failed to Get the Domain name from user token %d\n", ssdkStatus);
        }
        DEBUGL8("The Logged in User  Domain Name is %s\n", loggedInUserDomainName.c_str());
	
	map<CString,CString> setMap;
	CString ewbAccessPermToConfig(""),usbPrintPermToConfig("");
	if(!ewbAccessPerm.empty())
	{
		if(ewbAccessPerm == "true")
			ewbAccessPermToConfig = "1";
		else if(ewbAccessPerm == "false")
			ewbAccessPermToConfig = "0";
		else
		{
			DEBUGL1("Invalid value set to ewbAccessPerm ::%s\n",ewbAccessPerm.c_str());
			return STATUS_AL_SEC_INVALID_INPUT;

		}
		ssdkStatus = configPtr->SetProperty(myut.ut,SSDKConfigInterface::SA_RBACPERMEWBACCESSENABLE,ewbAccessPermToConfig);
		if(OK!=ssdkStatus)
		{
			DEBUGL1("Not able to set Err::%d \n",ssdkStatus);
			ret = SSDK_RBAC_MANAGER_PLUGIN_ERROR_MAP(ssdkStatus);
			return ret;
		}
		setMap.insert(pair<CString,CString>(XPATH_RBACSETTINGS_EWB_ACCESS,ewbAccessPerm));
	}
	if(!usbPrintPerm.empty())
	{
		if(usbPrintPerm == "true")
			usbPrintPermToConfig = "1";
		else if(usbPrintPerm == "false")
			usbPrintPermToConfig = "0";
		else
		{
			DEBUGL1("Invalid value set to usbPrintPerm ::%s\n",usbPrintPerm.c_str());
			return STATUS_AL_SEC_INVALID_INPUT;

		}

		ssdkStatus = configPtr->SetProperty(myut.ut,SSDKConfigInterface::SA_RBACPERMUSBPRINTENABLE,usbPrintPermToConfig);
		if(OK!=ssdkStatus)
		{
			DEBUGL1("Not able to set Err::%d \n",ssdkStatus);
			ret = SSDK_RBAC_MANAGER_PLUGIN_ERROR_MAP(ssdkStatus);
			return ret;
		}
		setMap.insert(pair<CString,CString>(XPATH_RBACSETTINGS_USB_DIRECT_PRINT,usbPrintPerm));
	}
	if((!setMap.empty()) && (STATUS_OK != Utils::SetValuesToRbacSettingsDOM(boProxy,setMap)))
	{
		DEBUGL1("Set value to RbacSettings dom failed\n");
		return STATUS_FAILED;
	}
	//ci::logmanager::LogInterface::OperationApplicationType opType;            
	CString ipAddr; 
	boProxy->GetApplicationInformation(oprApplication,ipAddr);
	Utils::WriteLog(NOTIFICATION_RBAC_SETTINGS_SUCCESS_CODE,true,ci::logmanager::LogInterface::opEdit,ci::logmanager::LogInterface::otConfiguration,ci::logmanager::LogInterface::mtMaintenance,oprApplication,loggedInUser ,"",ipAddr,loggedInUserDomainName);	
	return STATUS_OK;
}

Status Clone::Execute()
{
	MyUserToken myut;
        BOProxyRef boProxy;
        Ref<SSDKLogInterface> LogExternalPtr=NULL;
        //Authenticate the user
        Status ret=STATUS_OK;
        ret = Authenticate(myut, boProxy, LogExternalPtr);
        if(ret!= STATUS_OK)
        {
                DEBUGL2("ImportExportRoleRule ::Authenticate::Failed \n");
                return ret;
        }
        //Get the absolute file path
        //Get the file storage path. Previously need to read from fileStorage Path
        CString sFileStoragePath;

        ElementRef pfileStorageNode = NULL;
        NodeRef pfileStorageText = NULL;
        pfileStorageNode = m_command->GetParamNode("filePath");
        if (!(pfileStorageNode) || (!(pfileStorageText = pfileStorageNode->getFirstChild()) || ((sFileStoragePath= pfileStorageText->getNodeValue()).size() == 0) ) )
                DEBUGL2("ImportExportMapping::Execute::::Failed To  node in Command XML\n");
        DEBUGL8("Params fileStoragePath value:: %s\n",sFileStoragePath.c_str());
        if(sFileStoragePath.empty())
        {
                DEBUGL1("\n fileStorage Path is empty\n");
                return STATUS_AL_SEC_COMMAND_SCHEMA;
        }
        //Get the relative path
        ElementRef fileNameNode = NULL;
        NodeRef fileNameText = NULL;
        fileNameNode = m_command->GetParamNode("fileName");
        CString sRelativePath;
        if (!(fileNameNode) || (!(fileNameText = fileNameNode->getFirstChild()) || ((sRelativePath= fileNameText->getNodeValue()).size() == 0) ) )
                DEBUGL2("ImportExportMapping::Execute::::Failed To  node in Command XML\n");
        if(sRelativePath.empty())
        {
                DEBUGL1("\nFile name is empty\n");
                return STATUS_AL_SEC_COMMAND_SCHEMA;
        }
        DEBUGL8("Params relativePath value:: %s\n",sRelativePath.c_str());
	CString absFilePath;
        absFilePath=sFileStoragePath+"/"+sRelativePath;	
	return	ExecuteSSDK(absFilePath);
}

Status GenerateClone::ExecuteSSDK(const CString& absFilePath)
{
	MyUserToken myut;
        BOProxyRef boProxy;
        Ref<SSDKLogInterface> LogExternalPtr=NULL;
        Status ret=STATUS_FAILED;
        //Authenticate the user
        ret = Authenticate(myut, boProxy, LogExternalPtr);
        if(ret != STATUS_OK)
        {
                DEBUGL2("Import::Authenticate::Failed \n");
                return ret;
        }
        ssdk::SSDKStatus ssdkStatus;
        Ref<SSDKSecurityManagerInterface> ssdkSecurityManagerInterface = ssdk::GetSecurityManager();
        //Get the Config interface
        Ref<SSDKConfigInterface> configPtr = dynamic_cast<SSDKConfigInterface*> (ssdkSecurityManagerInterface->GetInterface  (SSDKSecurityManagerInterface::CONFIG_INTERFACE,ssdkStatus));
        if(configPtr ==(void*) NULL || ssdkStatus != OK)
        {
                DEBUGL1("\n ssdk SecurityManager::SSDKConfigInterface Failed \n");
                return STATUS_AL_SEC_INTERNAL_ERROR;
        }
        //Get the rbacType
        CString rbacTypeValue = configPtr->GetProperty(myut.ut,SSDKConfigInterface::SA_SERVAUTHEXTERNRBACENABLE,ssdkStatus);
        if(OK != ssdkStatus)
        {
                DEBUGL1("Get value from config failed:%d\n",ssdkStatus);
                ret = SSDK_RBAC_MANAGER_PLUGIN_ERROR_MAP(ssdkStatus);
                return ret;
        }
        //Get the default role
        CString defaultRoleValue = configPtr->GetProperty(myut.ut,SSDKConfigInterface::SA_SERVAUTHRBACDEFAULTROLE,ssdkStatus);
        if(OK != ssdkStatus)
        {
                DEBUGL1("Get value from config failed:%d\n",ssdkStatus);
                ret = SSDK_RBAC_MANAGER_PLUGIN_ERROR_MAP(ssdkStatus);
                return ret;
        }
        //Get the rbac server id from SRAM
        CString rbacServerIdValue;
        uint64 key = IndexedDB::CreateKey(SSDK_SRAM_CODE_TYPE,SSDK_SRAM_RBAC_SERVER_ID_CODE,SSDK_SRAM_SUB_CODE);
        if(!Utils::GetInt32ValueFromSRAM(key,rbacServerIdValue))
        {
                DEBUGL1("Get value from SRAM failed\n");
                return STATUS_AL_SEC_INTERNAL_ERROR;
        }
	// Cloning EWBAccess/USBDirectPrint permission settings
	CString ewbAccessPermFromConfig = configPtr->GetProperty(myut.ut,SSDKConfigInterface::SA_RBACPERMEWBACCESSENABLE,ssdkStatus);
	if(OK != ssdkStatus)
	{
		DEBUGL1("Get value from config failed:%d\n",ssdkStatus);
		ret = SSDK_RBAC_MANAGER_PLUGIN_ERROR_MAP(ssdkStatus);
		return ret;
	}
	CString usbPrintPermFromConfig = configPtr->GetProperty(myut.ut,SSDKConfigInterface::SA_RBACPERMUSBPRINTENABLE,ssdkStatus);
	if(OK != ssdkStatus)
	{
		DEBUGL1("Get value from config failed:%d\n",ssdkStatus);
		ret = SSDK_RBAC_MANAGER_PLUGIN_ERROR_MAP(ssdkStatus);
		return ret;
	}
	CString rbacType = (rbacTypeValue == "1")?"true":"false";	
	CString ewbAccessPerm = (ewbAccessPermFromConfig == "1")?"true":"false";
	CString usbPrintPerm = (usbPrintPermFromConfig == "1")?"true":"false";
	CString finalXml = "<RBACManager><RbacSettings><rbacType>" + rbacType + "</rbacType><rbacServerID>" + rbacServerIdValue + "</rbacServerID><ExtendedPermissions><ewbAccess>" + ewbAccessPerm + "</ewbAccess><usbDirectPrint>" + usbPrintPerm + "</usbDirectPrint></ExtendedPermissions></RbacSettings></RBACManager>";
	//Create a file
        FilePtr outputFile;
        if(File::Exists(absFilePath))
        {
                if(STATUS_OK != File::DeleteFile(absFilePath))
                {
                        DEBUGL1("Unable to delete the old file\n");
                        return STATUS_AL_SEC_INTERNAL_ERROR;
                }
        }
        outputFile = File::CreateFile(absFilePath,FolderPtr(), "w");
        if(!outputFile)
        {
                DEBUGL1("clone Info::File Creation failed\n");
                return STATUS_AL_SEC_INTERNAL_ERROR;
        }
        Status fileStatus = STATUS_OK;
  	fileStatus = outputFile->Write(finalXml.c_str());
  	if(STATUS_OK != fileStatus)
  	{
  		DEBUGL1("\n ExportRoleInfo::File write failed \n");
  		if(STATUS_DISK_FULL == fileStatus)
  			return fileStatus;
  		outputFile->Close();
  		return STATUS_AL_SEC_INTERNAL_ERROR;
  	}
  	fileStatus = outputFile->Close();
  	if(STATUS_OK != fileStatus)
  	{
  		DEBUGL1("\n ExportRoleInfo::File close failed \n");
  		if(STATUS_DISK_FULL == fileStatus)
  			return fileStatus;
  		return STATUS_AL_SEC_INTERNAL_ERROR;
  	}
	return STATUS_OK;
}

Status ApplyClone::ExecuteSSDK(const CString& absFilePath)
{
	MyUserToken myut;
	BOProxyRef boProxy;
	Ref<SSDKLogInterface> LogExternalPtr=NULL;
	Status ret=STATUS_FAILED;
	//Authenticate the user
	ret = Authenticate(myut, boProxy, LogExternalPtr);
	if(ret != STATUS_OK)
	{
		DEBUGL2("Import::Authenticate::Failed \n");
		return ret;
	}
	//Create a temp document
	if(!File::Exists(absFilePath))
	{
		DEBUGL2("Import::File Not found \n");
		return STATUS_AL_SEC_RBAC_DEFAULT_SETTINGS_FILE_NOT_FOUND;
	}

	dom::DocumentRef tempDoc=NULL;
	ci::operatingenvironment::Ref<ci::hierarchicaldb::HierarchicalDB>  pHDB = NULL;
	pHDB = ci::hierarchicaldb::HierarchicalDB::Acquire(NULL);
	if (!pHDB)
	{
		DEBUGL1("HDB ACQUIRE FAILED\n");
		return STATUS_AL_SEC_INTERNAL_ERROR;
	}
	DEBUGL8("HDB ACQUIRE SUCCESS\n");
	if(STATUS_OK !=(ret = pHDB->CreateTempDocumentFromFile(tempDoc,absFilePath)))
	{
		DEBUGL1("CREATION OF TEMP DOC FAILED\n");
		if(STATUS_HDB_ERROR_PARSING_XML == ret)
			return STATUS_AL_SEC_BAD_REQUEST;
		return STATUS_AL_SEC_INTERNAL_ERROR;
	}
	DEBUGL8("TEMP DOC CREATION SUCCESS\n");
	CString path("RBACManager/RbacSettings");
	NodeRef domNode = pHDB->BindToElement(tempDoc,path);
	if(!domNode)
	{
		DEBUGL1("Unable to bind to %s\n",path.c_str());
		return STATUS_AL_SEC_INTERNAL_ERROR;
	}
	NodeRef rbacTypeNode = pHDB->BindToElement(domNode,"rbacType");
	if(!rbacTypeNode)
	{
		DEBUGL1("Unable to bind to rbac type\n");
		return STATUS_AL_SEC_INTERNAL_ERROR;
	}
	CString rbacTypeValue = rbacTypeNode->getTextContent();	
	NodeRef rbacServerNode = pHDB->BindToElement(domNode,"rbacServerID");
	if(!rbacServerNode)
	{
		DEBUGL1("Unable to bind to rbac server id\n");
		return STATUS_AL_SEC_INTERNAL_ERROR;
	}
	CString rbacServerIdValue = rbacServerNode->getTextContent();
	//Fetch EWBAccess/USBDirectPrint permission settings from Clone file
	CString ewbAccessPerm(""),usbPrintPerm("");
	NodeRef ewbAccessNode = pHDB -> BindToElement (domNode,EWB_ACCESS_PATH,false);
	if(ewbAccessNode) // If Old clone file not containing EWBAcess perm node is applied, error should not be returned.
		ewbAccessPerm = ewbAccessNode->getTextContent();
	NodeRef usbPrintNode = pHDB -> BindToElement (domNode,USB_DIRECT_PRINT_PATH,false);
	if(usbPrintNode) //If Old clone file not containing USBPrint perm node is applied, error should not be returned.
		usbPrintPerm = usbPrintNode->getTextContent();
	//Set rbacType to SSDK config
	ssdk::SSDKStatus ssdkStatus;
	Ref<SSDKSecurityManagerInterface> ssdkSecurityManagerInterface = ssdk::GetSecurityManager();
	//Get the Config interface
	Ref<SSDKConfigInterface> configPtr = dynamic_cast<SSDKConfigInterface*> (ssdkSecurityManagerInterface->GetInterface  (SSDKSecurityManagerInterface::CONFIG_INTERFACE,ssdkStatus));
	if(configPtr ==(void*) NULL || ssdkStatus != OK)
	{
		DEBUGL1("\n ssdk SecurityManager::SSDKConfigInterface Failed \n");
		return STATUS_AL_SEC_INTERNAL_ERROR;
	}
	//Set the value to SSDK
	//Convert the value to true or false
		if(rbacTypeValue != "")
		{	
			bool bOut;
			if(!EbxSSDKUtil::validateInput<bool>(rbacTypeValue,bOut))
		        {
	        		DEBUGL1("Invalid Rbac TypeValue \n");
	                	return STATUS_AL_SEC_INVALID_INPUT;
	        	}
		}
	if(rbacTypeValue == "true")
		rbacTypeValue = "1";
	else
		rbacTypeValue = "0";
	ssdkStatus = configPtr->SetProperty(myut.ut,SSDKConfigInterface::SA_SERVAUTHEXTERNRBACENABLE,rbacTypeValue);
	if(OK!=ssdkStatus)
	{
		DEBUGL1("Not able to set Err::%d \n",ssdkStatus);
		ret = SSDK_RBAC_MANAGER_PLUGIN_ERROR_MAP(ssdkStatus);
		return ret;
	}
	//Set rbacServerId to SRAM
	if(!rbacServerIdValue.empty())
	{
		int iOut;
		if(!EbxSSDKUtil::validateInput<int>(rbacServerIdValue,iOut))
		{
			DEBUGL1("Invalid Rbac ServerId Value \n");
			return STATUS_AL_SEC_INVALID_INPUT;
		}
		uint64 key = IndexedDB::CreateKey(SSDK_SRAM_CODE_TYPE,SSDK_SRAM_RBAC_SERVER_ID_CODE,SSDK_SRAM_SUB_CODE);
		if(!Utils::SetInt32ValueToSRAM(key, rbacServerIdValue))
		{
			DEBUGL1("\n Set value to SRAM failed \n");
			return STATUS_AL_SEC_INTERNAL_ERROR;
		}
	}
		// Applying EWBAccess/USBDirectPrint permission settings to config and RbacSettings dom
		map<CString,CString> setMap;
		CString ewbAccessPermToConfig(""), usbPrintPermToConfig("");
		if(!ewbAccessPerm.empty())
		{
			if(ewbAccessPerm == "true")
				ewbAccessPermToConfig = "1";
			else if(ewbAccessPerm == "false")
				ewbAccessPermToConfig = "0";
			else
			{
				DEBUGL1("Not valid input to ewbAccessPerm:: %s \n",ewbAccessPerm.c_str());
				return STATUS_AL_SEC_INVALID_INPUT;
			}
			ssdkStatus = configPtr->SetProperty(myut.ut,SSDKConfigInterface::SA_RBACPERMEWBACCESSENABLE,ewbAccessPermToConfig);
			if(OK!=ssdkStatus)
			{
				DEBUGL1("Not able to set Err::%d \n",ssdkStatus);
				ret = SSDK_RBAC_MANAGER_PLUGIN_ERROR_MAP(ssdkStatus);
				return ret;
			}
			setMap.insert(pair<CString,CString>(XPATH_RBACSETTINGS_EWB_ACCESS,ewbAccessPerm));
		}
		if(!usbPrintPerm.empty())
		{
			if(usbPrintPerm == "true")
				usbPrintPermToConfig = "1";
			else if (usbPrintPerm == "false")
				usbPrintPermToConfig = "0";
			else
			{
				DEBUGL1("Not valid input to usbPrintPerm:: %s \n",usbPrintPerm.c_str());
				return STATUS_AL_SEC_INVALID_INPUT;
			}

			ssdkStatus = configPtr->SetProperty(myut.ut,SSDKConfigInterface::SA_RBACPERMUSBPRINTENABLE,usbPrintPermToConfig);
			if(OK!=ssdkStatus)
			{
				DEBUGL1("Not able to set Err::%d \n",ssdkStatus);
				ret = SSDK_RBAC_MANAGER_PLUGIN_ERROR_MAP(ssdkStatus);
				return ret;
			}
			setMap.insert(pair<CString,CString>(XPATH_RBACSETTINGS_USB_DIRECT_PRINT,usbPrintPerm));
		}
		// Set the configurations to RbacSettings dom.
		if((!setMap.empty()) && (STATUS_OK != Utils::SetValuesToRbacSettingsDOM(boProxy,setMap)))
		{
			DEBUGL1("Set value to RbacSettings dom failed\n");
			return STATUS_FAILED;
		}
		return STATUS_OK;
}

};
};
};

