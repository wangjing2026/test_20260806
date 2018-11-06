/* parasoft suppress item PB-33 */
/* parasoft suppress item METRICS-01_duplicated_2 */
/**************************************************************************************************************************
 * @Copyright 2008 TOSHIBA TEC CORPORATION All rights reserved
 * $Workfile: ccommand.h$
 * $Revision: 1 $
 * $Date: 07/10/2008$
 * Author: Manoj Saha/Ghaayathri
 * Description:
 *****************************************************************************************************************************/
#ifndef __AL_SOFTWAREFUNCTION_ROLE_CCOMMAND_H__
#define __AL_SOFTWAREFUNCTION_ROLE_CCOMMAND_H__

#include "CI/OperatingEnvironment/cexception.h"
#include "CI/OperatingEnvironment/interface.h"
#include "CI/OperatingEnvironment/cstring.h"
#include "CI/HierarchicalDB/DOM/node.h"
#include "CI/HierarchicalDB/DOM/document.h"
#include "CI/HierarchicalDB/hierarchicaldb.h"
#include "CI/IndexedDB/indexeddb.h"

#include "CI/MessagingSystem/msg.h"
#include "AL/UIController/bocontracts.h"
#include "AL/UIController/boproxy.h"

#include "utils.h"
#include "croleexception.h"
#include "CI/SI/ssdkusertokeninterface.h"
#include "CI/SI/ssdkrbacmgmtinterface.h"
#include "CI/SI/ssdkloginterface.h"
#include "CI/SI/ssdksecuritymanagerinterface.h"
#include "CI/SI/rbac_objects.h"
#include "CI/SI/rbac_operations.h"

#include "securitycommonresource.h"
//using namespace SI;
using namespace ssdk;
// namespaces
namespace al
{
	namespace softwarefunction
	{
		namespace rbacmanager
		{
						
		using namespace std;
		using namespace dom;
		using namespace ci::operatingenvironment;
		using namespace ci::messagingsystem;
		using namespace al::uicontroller;
		using namespace ci::indexeddb;
			class CCommand
			{
			private:
				dom::DocumentRef m_pCommandTempDocument;
				ci::hierarchicaldb::HierarchicalDBRef m_pHDB;
				dom::NodeRef m_pCommandNode;
				dom::NodeRef m_pCommandXPathNode;
				ci::operatingenvironment::CString m_sessionID;
			public:
				CCommand(dom::DocumentRef & pDocument,ci::hierarchicaldb::HierarchicalDBRef pHDB);
				const char * GetCommandName();
				dom::NodeRef GetCommandRoot();
				dom::NodeRef GetCommandNode();
				dom::NodeRef GetParamNode(const char *pParamName);
				ci::operatingenvironment::CString GetSessionID() { return m_sessionID; }
			};

			class DocumentGuard{
				DocumentRef m_pDeltaDoc;
				bool m_isWriteLock;
				bool m_isReadLock;
				public:
				Status ReadLock(bool block = false);
				Status WriteLock(bool block = false);
				Status Release();
				DocumentGuard(DocumentRef docRef):m_pDeltaDoc(docRef),m_isWriteLock(false),m_isReadLock(false){}
				~DocumentGuard();
			};


class CCommandHandler
{
protected:
    ci::operatingenvironment::Ref<CCommand> m_command;
public:
    virtual ~CCommandHandler(){}
    explicit CCommandHandler(ci::operatingenvironment::Ref<CCommand> command);
    virtual Status Execute() = 0;
    void SetUserToken(ci::operatingenvironment::Ref<SSDKUserTokenInterface> & userToken)
        {
                m_userToken = userToken;
        }
protected:
    BOProxyRef m_boProxy;
    ci::operatingenvironment::Ref<SSDKUserTokenInterface> m_userToken;
};

class MyUserToken{
 public:
	SSDKUserTokenInterface* ut;
	MyUserToken():ut(NULL){
	}
	explicit MyUserToken(SSDKUserTokenInterface* userToken):ut(userToken){}
	~MyUserToken()
	{
	}
};
class CRoleCommandHandler: public CCommandHandler
{
private:
public:
    virtual ~CRoleCommandHandler(){}
    explicit CRoleCommandHandler(ci::operatingenvironment::Ref<al::softwarefunction::rbacmanager::CCommand>& command) : CCommandHandler(command)
{}
    //CString GetValueFromPath(BOProxyRef boProxy, CString path, CString fieldName, CString statusXpath, CString cmdStatus, ERROR_STATUS errorCode,bool isReqd=true);
    void ExtractToken(CString& allToken,const char* del,vector<CString>& roleVector);
    //Status GetSystemUserToken(MyUserToken &myut,BOProxyRef &boProxy);
    virtual Status Authenticate(MyUserToken &myut, BOProxyRef &boProxy, Ref<SSDKLogInterface> &LogExternalPtr );
    Status ProcessTransDoc(CString cmdName,CString & sUserDataXPath, ElementRef pParamNode,DocumentRef & pNewUserDoc); 
    Status AddTheNewNode(CString& xml,  NodeRef pNewUserDoc, ci::operatingenvironment::Ref<dom::Element>& pNewUserEle);
    void MakeTokenizeString(std::vector<string> users,CString & allUsers);
    CString CreateXMLNode(std::vector<string> strList,CString rootNodeName ,CString childNodeName);
    Status GetAllObjectOperationSet(MyUserToken &myut,const Ref<SSDKRBACMgmtInterface> & rbac,const CString &objectName,const CString &sObjectType,set< pair <CString,CString> > & objectPermSet);
    Status ReadPermissions(NodeRef pNewRoleData, CString sDataXPath,map<CString,bool> & permissionEnableDisableMap , bool checkAttribute, bool checkPermNodeReqd = true);
    Status SSDK_RBAC_MANAGER_PLUGIN_ERROR_MAP(SSDKStatus ssdkError);
    Status SetRoleRule(const SSDKUserTokenInterface* userToken,const map<CString,vector<CString> >& roleNameAttrMap); 
    bool CheckPermission();
    void UnassignPermission(map<CString,bool> & permissionEnableDisableMap);
    Status UpdateRoleRule(const CString& roleNameToMapping, SSDKRBACMgmtInterface::RoleInfo& SSDKRoleInfo);
};

//Sreekumar
			
			// namespaces
		}
	}
}

#endif //end of #ifndef __AL_SOFTWAREFUNCTION_ROLE_CCOMMAND_H__
