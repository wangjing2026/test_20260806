/* parasoft suppress item PB-33 */
/* parasoft suppress item METRICS-01_duplicated_2 */
/**
 *Copyright 2008 TOSHIBA TEC CORPORATION All rights reserved
 *@Workfile: utils.h$
 *@Revision: 1 $
 *@Date: 10/25/2007$ 
 *@Author: Manoj Saha 
 *@Description:
 *@Notes:
*/
#ifndef AL_SOFTWARE_FUNCTIONS_ROLEMANAGER_UTILS_H
#define AL_SOFTWARE_FUNCTIONS_ROLEMANAGER_UTILS_H

#include "CI/HierarchicalDB/DOM/node.h"
#include "CI/HierarchicalDB/DOM/document.h"
#include "CI/HierarchicalDB/DOM/element.h"
#include "CI/HierarchicalDB/hierarchicaldb.h"
#include "CI/MessagingSystem/msg.h"
#include <CI/MessagingSystem/msgport.h>
#include <CI/SystemResourceManager/systemresourcemanager.h>
#include <CI/ServiceStartupManager/client.h>
#include "AL/UIController/boproxy.h"
#include <map>
#include<iomanip>
#include<sstream>
#include<algorithm> 

// namespace

using namespace std;
using namespace dom;
using namespace ci::operatingenvironment;
using namespace ci::messagingsystem;
using namespace al::uicontroller;
namespace al
{
       	namespace softwarefunction
        {
                namespace rbacmanager
                {

		class Utils
		{		
			public:
			static CString getNodeText(Ref<Node> node);
			static Ref<Node> GetChildByName(Ref<Node> parent, CString childName);
			static Status setNodeText(Ref<Node> node, CString text);
			static Status RemoveChild(ci::operatingenvironment::Ref<dom::Element> pRemoveElem);
			static void StatusOfCommandOperation(dom::NodeRef pCommandNode,CString status);
			static Status ParseRoleMap(const CString& xmlName,map<CString,vector<CString> >& roleNameAttrVector,CString& otherUser,const CString& roleNameToMapping="");
			static bool IsBuiltinRole(const CString& role,bool searchByName = true);//updated to check for Built-In role based on ID 
			static bool SetValueToSRAM(const uint64 & key, const CString & value);
			static bool GetValueFromSRAM(const uint64& key,CString& value);
			static bool SetInt32ValueToSRAM(const uint64 & key, const CString&  value);
			static bool GetInt32ValueFromSRAM(const uint64 & key, CString & value);		
			static Status SetValueToPath(BOProxyRef boProxy,const CString& path,const CString& pathValue);
			static Status CheckForCorruption(SSDKStatus status);
			static Status WriteLog(const CString& errorCode,bool info,ci::logmanager::LogInterface::MsgOperationType oprType,ci::logmanager::LogInterface::MsgOperationTarget oprTarget,ci::logmanager::LogInterface::MessageType msgType,ci::logmanager::LogInterface::OperationApplicationType oprApplication,const CString& userName, const CString& operatedName, const CString& ipAddr,const CString& userDomainName);
			static bool SortAscending(SSDKRBACMgmtInterface::RoleInfo const &a, SSDKRBACMgmtInterface::RoleInfo const &b );
			static bool SortDescending(SSDKRBACMgmtInterface::RoleInfo const &a, SSDKRBACMgmtInterface::RoleInfo const &b );
			static bool CheckForNameValidity(const CString& name);
			static Status GetValuesFromRbacSettingsDOM(map<CString,CString> & inputMap);
			static Status SetValuesToRbacSettingsDOM(BOProxyRef boProxy,map<CString,CString> & inputMap);
			static bool IsMaxLengthExceeded(const CString& name,int maxlength);
			private:
			static set<CString>& BuiltinRoles(bool searchByName = true ); //updated to search Built in roles by ID
			static set<CString> m_builtinRoles; //updated to DeleteRoles by ID.
			static set<CString> m_builtinRolesOrg;
		};
		class EncodeDecode
		{
		public :
		Status Encode(vector<string> inputVector,CString &encodedString );
		Status Decode(const string in,vector<string> &outvector);
		};
		}
	}
}
// namespaces

#endif //AL_SOFTWARE_FUNCTIONS_ROLEMANAGER_UTILS_H
