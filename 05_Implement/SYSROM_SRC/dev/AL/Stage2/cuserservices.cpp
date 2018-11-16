// (c) 2010 TOSHIBA TEC CORPORATION All Rights Reserved.

/* parasoft suppress item PB-33 */
/* parasoft suppress item PB-23 */

/********************************************************************************
*
* $Workfile: cuserservices.cpp$
* $Revision: 1 $
* $Date: 11/14/2008$
* Author: Sunil Sadanadappa
*
* Description: This file defines the Stage2 CUserServices class.
* Notes:
*
*********************************************************************************/

#include <cstdlib>
#include "soapH.h"
#include "CI/OperatingEnvironment/cuuid.h"
#include "cuserservices.h"
#include "cdevicerecord.h"
#include "csessionmgr.h"
#include "cdeviceutility.h"
#include "ccommonfunctions.h"
#include "cuicontrolleradapter.h"
#include "cdeviceconfiguration.h"
#include "cRDBInterface.h"
#include "AL/status.h"

#include "CI/SI/ssdksecuritymanagerinterface.h"
#include "CI/SI/ssdkrbacmgmtinterface.h"
#include "CI/SI/ssdkusertokeninterface.h"
#include "CI/OperatingEnvironment/mutex.h"

#include "AL/SoftwareFunction/AccountingLibrary/accountingdefines.h"
#include "AL/SoftwareFunction/AuthenticationLibrary/authenticationlibraryinterface.h"
#define FORMAT_STYLE			"%03d"
#define TYPE_URL				"url"
#define	TYPE_TEMPLATE			"template"
#define	TYPE_TEMPLATE_GROUP		"templategroup"


using namespace ci::operatingenvironment;
using namespace al::uicontroller;
using namespace ci::hierarchicaldb;
using namespace std;
using namespace ssdk;
using namespace al::softwarefunction::accountinglibrary;
using namespace al::softwarefunction::authenticationlibrary;

extern const char *soap_wsa_anonymousURI;
extern MutexPtr gMDSLoginOperationMutex;
extern MutexPtr gMDSLogoutOperationMutex;

namespace al
{
	namespace stage2
	{
	  #include "EventServiceSoapBinding.nsmap"

		
		#ifdef STAGE2_UNSUPPORTED
		
		/**
		* Function: To get all the defined group lists in the MFP.
		*/
		/*Status UserServices::GetTemplatesGroupList(BOProxyRef& pBOProxy, vector<CString>& vGroupList)
		{
			DEBUGL4("UserServices::GetTemplatesGroupList Entry\n");
			
			commandParam stParam;		
			vector<commandParam> vCommandParams;			

			CString strViewXPath = "JobTemplates/View_"+ CUUID().toString() + "/GroupList" ;
			stParam.strParamName = "viewXpath";
			stParam.strParamValue = strViewXPath;
			vCommandParams.push_back(stParam);

			stParam.strParamName = "definedGroups";
			stParam.strParamValue = "true";
			vCommandParams.push_back(stParam);

			CString sResponse = "";
			if(STATUS_OK != CUIControllerAdapter::SendCommandParamAsElement("GetGroupList", "JobTemplates/GroupList", vCommandParams, sResponse, pBOProxy))
			{
				throw CStg2Exception(eReceiverError, "ERR_EBS_SERVER_INTERNAL_ERROR", SOAP_FAULT, "");
			}
			
			if(STATUS_OK != CUIControllerAdapter::GetOperationStatus(sResponse))
			{
				if(STATUS_OK != CUIControllerAdapter::AnalyzeResponse(sResponse))
				{
					throw CStg2Exception(eReceiverError, "ERR_EBS_SERVER_INTERNAL_ERROR", SOAP_FAULT, "");
				}
			}
			
			//get the group list from given xpath.
			NodeRef pGroupList =  0;			
			if (STATUS_OK != pBOProxy->GetValue(strViewXPath.c_str(), pGroupList) || !pGroupList) 
			{
				throw CStg2Exception(eReceiverError, "ERR_EBS_SERVER_INTERNAL_ERROR", SOAP_FAULT, "");
			}

			NodeListRef pGroups = pGroupList->getChildNodes();
			for (unsigned long i = 0; i < pGroups->getLength(); ++i) 
			{
				Ref<Element> pGroup = pGroups->item(i);
				if (!pGroup) 
				{
					continue; 
				}
				
				CString sGroupID = pGroup->getAttribute("gid");
				if(sGroupID.empty()) 
				{
					continue;
				}
				vGroupList.push_back(sGroupID);				
			}

			DEBUGL1("UserServices::GetTemplatesGroupList Exit\n");
			return STATUS_OK;
		}*/

		user__ownershipInfoType* UserServices::getOwnershipInfo(struct soap* soap, BOProxyRef& pBOProxy, CString& sOwnerID)
		{
			DEBUGL8("UserServices::getOwnerShipInfo Entry\n");
			DEBUGL1("getOwnershipInfo is not supported in eBX 1st.\n");
						
			user__ownershipInfoType* pOwnershipInfo = NULL;
			/*common__stringListType* pBoxList =  getBoxList(soap, pBOProxy, sOwnerID);
			common__stringListType* pMailBoxList = getMailBoxList(soap, pBOProxy, sOwnerID ); 
			user__templateOwnershipType* pTemplate = getTemplateOwnership(soap, pBOProxy, sOwnerID);
			
			if(pBoxList || pMailBoxList || pTemplate)
			{
				pOwnershipInfo = soap_new_user__ownershipInfoType(soap, -1);
				pOwnershipInfo->BoxList = pBoxList;
				pOwnershipInfo->MailBoxList = pMailBoxList;
				pOwnershipInfo->Template = pTemplate;
			}*/
						
			DEBUGL8("UserServices::getOwnerShipInfo Exit\n");

			return pOwnershipInfo;
		}

		Status UserServices::setOwnershipInfo(BOProxyRef& pBOProxy, CString& sOwnerID, user__ownershipInfoType*& OwnershipInfo, OffendingParamsList& vOffendingValue)
		{
			DEBUGL8("UserServices::setOwnerShipInfo Entry\n");
			
			DEBUGL1("setOwnershipInfo is not supported in eBX 1st.\n");
			/*
			//eFiling Box List
			if(OwnershipInfo->BoxList)
			{
				setBoxList(pBOProxy, sOwnerID, OwnershipInfo->BoxList, vOffendingValue);
			}
			
			//MailBox List
			if(OwnershipInfo->MailBoxList)
			{
				setMailBoxList(pBOProxy, sOwnerID, OwnershipInfo->MailBoxList, vOffendingValue);
			}
					
			//Template
			if(OwnershipInfo->Template)
			{
				setTemplateOwnership(pBOProxy, sOwnerID, OwnershipInfo->Template, vOffendingValue);
			}
			*/
			
			DEBUGL8("UserServices::setOwnerShipInfo Exit\n");

			return STATUS_OK;;
		}
		
		
		/*user__templateOwnershipType* UserServices::getTemplateOwnership(soap* soap, BOProxyRef& pBOProxy, CString& sReqOwnerID)
		{
			user__templateOwnershipType* pTemplateOwnership = NULL;
		
			return pTemplateOwnership;
		}*/
		
		
		/*user__templateIdListType* UserServices::getTemplateList(struct soap* soap, BOProxyRef& pBOProxy, CString& sReqOwnerID)
		{
			DEBUGL8("UserServices::getTemplateList Entry\n");
			
			vector<CString> vGroupList;
			vGroupList.clear();
			
			if (STATUS_OK != GetTemplatesGroupList(pBOProxy, vGroupList)) 
			{
				throw CStg2Exception(eReceiverError, "ERR_EBS_SERVER_INTERNAL_ERROR", SOAP_FAULT, "");
			}

			user__templateIdListType *TemplateList = soap_new_user__templateIdListType(soap, -1);	
			
			CString sCmd = CString ("GetTemplateList");
			CString sCmdNode = CString("JobTemplates/GroupList/Group/TemplateList");
			CString sResponseXPath = CString ("JobTemplates/View/TemplateList");
			
			for (unsigned int i = 0; i < vGroupList.size(); ++i)
			{
				commandParam stParam;		
				vector<commandParam> vCommandParams;
				
				stParam.strParamName = "selectedGroup";
				stParam.strParamValue = vGroupList.at(i);
				vCommandParams.push_back(stParam);

				CString strViewXPath = "JobTemplates/View_"+ CUUID().toString() + "/TemplateList" ;
				stParam.strParamName = "viewXpath";
				stParam.strParamValue = strViewXPath;
				vCommandParams.push_back(stParam);

				stParam.strParamName = "definedTemplates";
				stParam.strParamValue = "true";
				vCommandParams.push_back(stParam);

				CString sResponse = "";
				if(STATUS_OK != CUIControllerAdapter::SendCommandParamAsElement("GetTemplateList", "JobTemplates/GroupList/Group/TemplateList", vCommandParams, sResponse, pBOProxy))
				{
					throw CStg2Exception(eReceiverError, "ERR_EBS_SERVER_INTERNAL_ERROR", SOAP_FAULT, "");
				}
				
				if(STATUS_OK != CUIControllerAdapter::GetOperationStatus(sResponse))
				{
					if(STATUS_OK != CUIControllerAdapter::AnalyzeResponse(sResponse))
					{
						throw CStg2Exception(eReceiverError, "ERR_EBS_SERVER_INTERNAL_ERROR", SOAP_FAULT, "");
					}		
				}
				
				//get the Template list from given groupid.
				NodeRef pTemplateList;
				if (STATUS_OK != pBOProxy->GetValue(strViewXPath.c_str(), pTemplateList)|| !pTemplateList) 
				{
					throw CStg2Exception(eReceiverError, "ERR_EBS_SERVER_INTERNAL_ERROR", SOAP_FAULT, "");
				}

				Ref<NodeList> pTemplates = (static_cast<ElementRef>(pTemplateList))->getElementsByTagName("Template");

				for (unsigned long j = 0; j < pTemplates->getLength(); ++j)
				{
					user__templateIdInfoType* pTemplateType = soap_new_user__templateIdInfoType(soap, -1);
					
					ElementRef pTemplateElement = pTemplates->item(j);					
					CString sOwnerID = chelper::GetPathValue(pTemplateElement, "MetaData/ownerName");
					if (sReqOwnerID != sOwnerID ) 
					{
						DEBUGL8("The selected Templates OwnerID: %s does not match the requested id: %s\n", sOwnerID.c_str(), sReqOwnerID.c_str());
						continue;
					}
					
					
					pTemplateType->TemplateNumber = atoi(pTemplateElement->getAttribute("tid").c_str());
					pTemplateType->GroupNumber = static_cast<int*>(soap_malloc(soap,sizeof(int)));
					*(pTemplateType->GroupNumber) = atoi((vGroupList[i]).c_str());
					pTemplateType->TemplateCategory = common__templateCategoryNameType__Standard; //has to be revised
					TemplateList->Template.push_back(pTemplateType);
				}
			}
			
			if(TemplateList->Template.empty())
			{
				TemplateList = NULL;
			}
			DEBUGL8("UserServices::getTemplateList Exit\n");

			return TemplateList;
			
		}*/
		
		/*common__stringListType* UserServices::getBoxList(struct soap* soap, BOProxyRef& pBOProxy, CString& sReqOwnerID)
		{
			DEBUGL8("UserServices::getBoxList Entry \n");
			
			CString sCmd = CString("GetEFilingBoxes");
			CString sCmdNode = CString("eFiling/BoxList");
			CString sResponseXPath = CString("eFiling/View/BoxList");

			struct commandParam stParamValuePair;
			vector<commandParam> vParamValuePair;

			stParamValuePair.strParamName = CString("responseXpath");
			stParamValuePair.strParamValue = sResponseXPath;
			stParamValuePair.mapAttributes["contentType"] = CString("Value");

			vParamValuePair.push_back(stParamValuePair);

			CString sResponse = "";			
			if (STATUS_OK != CUIControllerAdapter::SendCommand(sCmd, sCmdNode, vParamValuePair, sResponse, pBOProxy))
			{
				throw CStg2Exception(eReceiverError, "ERR_EBS_SERVER_INTERNAL_ERROR", SOAP_FAULT, "");				
			} 
			
			if(STATUS_OK != CUIControllerAdapter::GetOperationStatus(sResponse))
			{
				if(STATUS_OK != CUIControllerAdapter::AnalyzeResponse(sResponse))
				{
					throw CStg2Exception(eReceiverError, "ERR_EBS_SERVER_INTERNAL_ERROR", SOAP_FAULT, "");
				}
			}
			
			
			NodeRef pBoxList = NULL;
			if (STATUS_OK != pBOProxy->GetValue(sResponseXPath.c_str(), pBoxList) || !pBoxList) 
			{
				throw CStg2Exception(eReceiverError, "ERR_EBS_SERVER_INTERNAL_ERROR", SOAP_FAULT, "");				
			}

			common__stringListType* pIdList = soap_new_common__stringListType(soap, -1);
			Ref<NodeList> pBoxes = (static_cast<ElementRef>(pBoxList))->getElementsByTagName("Box");
			for(unsigned long i = 0 ; i < pBoxes->getLength(); ++i)
			{
				Ref<Element> pBox = pBoxes->item(i);
				if(!pBox || (sReqOwnerID != chelper::GetPathValue(pBox, "ownerid")))
				{
					continue;							
				}
				pIdList->Item.push_back(pBox->getAttribute("boxNo"));
			}
			
			if(pIdList->Item.empty())
			{
				pIdList = NULL;
			}

			DEBUGL8("UserServices::getBoxList Exit \n");

			return pIdList;
			
			
		}*/
		
		/*
		common__stringListType* UserServices::getMailBoxList(struct soap* soap, BOProxyRef& pBOProxy, CString& sReqOwnerID)
		{
			DEBUGL8("UserServices::getMailBoxList Entry \n");
						
			vector<CString> xPaths;
			vector<CString> values;
			
			xPaths.push_back("MailBoxes/PropertyFilter/MailBox/ownerId");
			values.push_back("");
			
			CUIControllerAdapter::SetXPathValues(xPaths, values,  pBOProxy);
				

			CString sCmd = CString("GetMailBoxes");
			CString sCmdNode = CString("MailBoxes/MailBox");
			CString sResponseXPath = CString("MailBoxes/List");

			struct commandParam stParamValuePair;
			vector<commandParam> vParamValuePair;

			stParamValuePair.strParamName = CString("responseXPath");
			stParamValuePair.strParamValue = sResponseXPath;
			stParamValuePair.mapAttributes["contentType"] = CString("Value");

			vParamValuePair.push_back(stParamValuePair);
			
			stParamValuePair.strParamName = CString("propertyFilter");
			stParamValuePair.strParamValue = "MailBoxes/PropertyFilter/MailBox";
			stParamValuePair.mapAttributes["contentType"] = CString("Value");
			
			vParamValuePair.push_back(stParamValuePair);
			
			CString sResponse = "";
			if (STATUS_OK != CUIControllerAdapter::SendCommand(sCmd, sCmdNode, vParamValuePair, sResponse, pBOProxy)) 
			{
				
				throw CStg2Exception(eReceiverError, "ERR_EBS_SERVER_INTERNAL_ERROR", SOAP_FAULT, "");
			}
			
			if(STATUS_OK != CUIControllerAdapter::GetOperationStatus(sResponse))
			{
				if(STATUS_OK != CUIControllerAdapter::AnalyzeResponse(sResponse))
				{
					throw CStg2Exception(eReceiverError, "ERR_EBS_SERVER_INTERNAL_ERROR", SOAP_FAULT, "");
				}
			}
			
			NodeRef pMailBoxList = NULL;
			if (STATUS_OK != pBOProxy->GetValue(sResponseXPath.c_str(), pMailBoxList) || !pMailBoxList)
			{
				DEBUGL1("BOProxy Failed to get the MailBox list element on XPath: %s \n", sResponseXPath.c_str());
				throw CStg2Exception(eReceiverError, "ERR_EBS_SERVER_INTERNAL_ERROR", SOAP_FAULT, "");
			} 

			common__stringListType* pIdList = soap_new_common__stringListType(soap, -1);	
			
			Ref<NodeList> pMailBoxes = (static_cast<ElementRef>(pMailBoxList))->getElementsByTagName("MailBox");
			for(unsigned long i = 0 ; i < pMailBoxes->getLength(); ++i)
			{
				Ref<Element> pMailBox = pMailBoxes->item(i);
				if(!pMailBox || (sReqOwnerID != chelper::GetPathValue(pMailBox, "ownerId")))
				{
					continue;							
				}
				pIdList->Item.push_back(pMailBox->getAttribute("number"));
			}
			
			if(pIdList->Item.empty())
			{
				pIdList = NULL;
			}

			DEBUGL8("UserServices::getMailBoxList Exit \n");

			return pIdList;
		}*/
		
		/*Status UserServices::setTemplateOwnership(BOProxyRef& pBOProxy, CString& sOwnerID, user__templateOwnershipType*& pTemplateOwnership, OffendingParamsList& vOffendingValue)
		{
			return setTemplateList(pBOProxy, sOwnerID, pTemplateOwnership->TemplateList, vOffendingValue);
		}*/
		
		
		/*Status UserServices::setTemplateList(BOProxyRef& pBOProxy, CString& sOwnerID, user__templateIdListType*& pTemplateList, OffendingParamsList& vOffendingValue)
		{
			DEBUGL8("UserServices::setTemplateList Entry\n");
			

			CString sCmd = CString("UpdateTemplateMetadata");
			CString sCmdNode = CString("JobTemplates/GroupList/Group/TemplateList/Template");
			CString sXPath = CString("JobTemplates/View/Template/New/MetaData");

			vector<CString> vXPaths;
			vector<CString> vValues;

			vXPaths.push_back("JobTemplates/View/Template/New/MetaData/ownerName");
			vValues.push_back(sOwnerID);

			CUIControllerAdapter::SetXPathValues(vXPaths, vValues, pBOProxy);

			for ( unsigned int i = 0; i < pTemplateList->Template.size(); ++i) 
			{
				struct commandParam stParamValuePair[3];
				vector<commandParam> vParamValuePair;

				if (!pTemplateList->Template[i]->GroupNumber) 
				{
					continue;
				}
				stParamValuePair[0].strParamName = CString("selectedGroup");				
				stParamValuePair[0].strParamValue = ci::operatingenvironment::format(CString(FORMAT_STYLE),*(pTemplateList->Template[i]->GroupNumber));
				
				
				stParamValuePair[1].strParamName = CString("selectedTemplate");
				stParamValuePair[1].strParamValue = ci::operatingenvironment::format(CString(FORMAT_STYLE),pTemplateList->Template[i]->TemplateNumber);
				
								
				stParamValuePair[2].strParamName = CString("newMetadata");
				stParamValuePair[2].strParamValue = sXPath; 

				vParamValuePair.push_back(stParamValuePair[0]);
				vParamValuePair.push_back(stParamValuePair[1]);
				vParamValuePair.push_back(stParamValuePair[2]);
				
				CString sResponse = "";
				if (STATUS_OK != CUIControllerAdapter::SendCommandParamAsElement(sCmd, sCmdNode, vParamValuePair, sResponse, pBOProxy) )
				{
					throw CStg2Exception(eReceiverError, "ERR_EBS_SERVER_INTERNAL_ERROR",SOAP_FAULT, "Error in Sending Command");
				} 
				
				if (STATUS_OK != CUIControllerAdapter::GetOperationStatus(sResponse)) 
				{
				
					CString sGroupID = string_cast(*(pTemplateList->Template[i]->GroupNumber));
					CString sTemplateID = string_cast(pTemplateList->Template[i]->TemplateNumber);
					CString sValue = "GroupNumber:" + sGroupID + "/" + "TemplateNumber:" + sTemplateID;
					
					OffendingParamsStruct offendingParam;
					offendingParam.Name = "Invalid Data";
					offendingParam.Value = sValue;
					vOffendingValue.push_back(offendingParam);
									
				}
			}	
			DEBUGL8("UserServices::setTemplateList Exit\n");
			return STATUS_OK;
		}*/
		
		/*
		Status UserServices::setBoxList(BOProxyRef& pBOProxy, CString& sOwnerID, common__stringListType*& OwnedEntityIdList, OffendingParamsList& vOffendingValue)
		{
			DEBUGL8("UserServices::setBoxList Entry\n");

			CString sCmd = CString("EditEFilingResourceProperties");
			CString sCmdNode = CString("eFiling/BoxList/Box");
			CString sXPath = CString("eFiling/View/Edit/Box");
			
			for (unsigned int i = 0; i < OwnedEntityIdList->Item.size(); ++i)
			{
				vector<CString> vXPaths;
				vector<CString> vValues;

				vXPaths.push_back("eFiling/View/Edit/Box/ownerid");
				vValues.push_back(sOwnerID);
				
				CUIControllerAdapter::SetXPathValues(vXPaths, vValues, pBOProxy);


				struct commandParam stParamValuePair[2];
				vector<commandParam> vParamValuePair;

				stParamValuePair[0].strParamName = CString("moniker");
				stParamValuePair[0].strParamValue = OwnedEntityIdList->Item.at(i) + CString("/");

				stParamValuePair[1].strParamName = CString("properties");
				stParamValuePair[1].strParamValue = sXPath;
				stParamValuePair[1].mapAttributes["contentType"] = CString("XPath");

				vParamValuePair.push_back(stParamValuePair[0]);
				vParamValuePair.push_back(stParamValuePair[1]);

				CString sResponse = "";

				if (STATUS_OK != CUIControllerAdapter::SendCommand(sCmd, sCmdNode, vParamValuePair, sResponse, pBOProxy))
				{
					throw CStg2Exception(eReceiverError, "ERR_EBS_SERVER_INTERNAL_ERROR",SOAP_FAULT, "Error in Sending Command");
				}
				
				if(STATUS_OK != CUIControllerAdapter::GetOperationStatus(sResponse))
				{
					
					OffendingParamsStruct offendingParam;
					offendingParam.Name = "Invalid Data";
					offendingParam.Value = OwnedEntityIdList->Item.at(i);
					vOffendingValue.push_back(offendingParam);
					continue;
				}
				
			}	

			DEBUGL8("UserServices::setBoxList Exit\n");
			
			return STATUS_OK;	
		}*/

		/*
		Status UserServices::setMailBoxList( BOProxyRef& pBOProxy, CString& sOwnerID, common__stringListType*& OwnedEntityIdList, OffendingParamsList& vOffendingValue)
		{
			DEBUGL8("UserServices::setMailBoxList Entry\n");

			CString sCmd = CString("EditMailBox");
			CString sCmdNode = CString("MailBoxes/MailBox");

			for (unsigned int i = 0; i < OwnedEntityIdList->Item.size(); ++i)
			{
				vector<CString> vXPaths;
				vector<CString> vValues;

				vXPaths.push_back("MailBoxes/New/MailBox/@number");
				vValues.push_back(OwnedEntityIdList->Item.at(i));

				vXPaths.push_back("MailBoxes/New/MailBox/ownerId");
				vValues.push_back(sOwnerID);

				
				CUIControllerAdapter::SetXPathValues(vXPaths, vValues, pBOProxy);

				struct commandParam stParamValuePair[2];
				vector<commandParam> vParamValuePair;

				stParamValuePair[0].strParamName = CString("mailBoxDetails");
				stParamValuePair[0].strParamValue = CString("MailBoxes/New/MailBox");
				stParamValuePair[0].mapAttributes["contentType"] = CString("XPath");

				stParamValuePair[1].strParamName = CString("boxNumber");
				stParamValuePair[1].strParamValue = OwnedEntityIdList->Item.at(i); 
				stParamValuePair[1].mapAttributes["contentType"] = CString("Value");

				vParamValuePair.push_back(stParamValuePair[0]);
				vParamValuePair.push_back(stParamValuePair[1]);


				CString sResponse = "";
				if (STATUS_OK != CUIControllerAdapter::SendCommand(sCmd, sCmdNode, vParamValuePair, sResponse, pBOProxy))
				{
					throw CStg2Exception(eReceiverError, "ERR_EBS_SERVER_INTERNAL_ERROR",SOAP_FAULT, "Error in Sending Command");
				} 
				if(STATUS_OK != CUIControllerAdapter::GetOperationStatus(sResponse))
				{
					OffendingParamsStruct offendingParam;
					offendingParam.Name = "Invalid Data";
					offendingParam.Value = OwnedEntityIdList->Item.at(i);
					vOffendingValue.push_back(offendingParam);
					continue;
				}
			}

			
			return STATUS_OK;
		}*/
		#endif
		
		Status UserServices::GetSecuritySettings(const BOProxyRef& pBOProxy, bool & bUserAuthEnabled, bool & bDeptAuthEnabled, 
													CString& sUserName)
		{
			DEBUGL5("UserServices::GetSecuritySettings Enter\n");

			CString strResponse;
			
			if(!strncasecmp(sUserName.c_str(),"Admin" , CString("Admin").size()+1))
			{
				Ref<AuthenticationLibraryInterface> pAuthLibIF = AuthenticationLibraryInterface::create();
				CString sSessionID = pBOProxy->getSessionID();
				if(pAuthLibIF)
				{
					if(STATUS_OK != pAuthLibIF->LoginRequiredFromStage2(sSessionID))
					{
						DEBUGL1("UserServices::GetSecuritySettings : LoginRequiredFromStage2 failed \n");
						CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");
					}	
				}
				else
					DEBUGL5("UserServices::GetSecuritySettings pAuthLibIF is NULL\n");
					
			}
			else
			{
				strResponse.clear();
				
				commandParam stParam;		
				vector<commandParam> vCommandParams;	

				if(STATUS_OK != CUIControllerAdapter::SendCommand("LoginRequired","SecurityConfiguration/SecuritySettings", vCommandParams, strResponse, pBOProxy))
				{
					DEBUGL1("UserServices::GetSecuritySettings : SendCommand failed \n");
					CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");
				}
				
				CString sOperationStatus = CUIControllerAdapter::GetOperationStatusString(strResponse);
				if(STG2_SUCCESS != sOperationStatus)
				{
					DEBUGL1("UserServices::GetSecuritySettings Operation failed.\n");
					CErrorCodeMapper::GenerateException(sOperationStatus, "SecurityConfiguration");
				}
			}
			CString isLoginReqd = CUIControllerAdapter::GetTextNodeValue("SecurityConfiguration/SecuritySettings/isLoginReqd", pBOProxy);
			CString isDeptLoginReqd = CUIControllerAdapter::GetTextNodeValue("SecurityConfiguration/SecuritySettings/isDeptLoginReqd", pBOProxy);
			
			DEBUGL5("UserServices::GetSecuritySettings Exit. User Auth = %s, Depth Auth = %s\n", isLoginReqd.c_str(), isDeptLoginReqd.c_str());
			
			bUserAuthEnabled = ("TRUE" == isLoginReqd) ? true:false;
			bDeptAuthEnabled = ("TRUE" == isDeptLoginReqd) ? true:false;
						
			return STATUS_OK;
		}
		
		Status UserServices::ExecutePasswordResetLogin(soap* soap, BOProxyRef& pBOProxy, userauth__userPasswordResetAuthenticationType* pUserResetAuthType, CString& strSession )
		{
			DEBUGL5("UserServices::ExecutePasswordResetLogin Enter\n");
			
			if(!pUserResetAuthType)
			{
				CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");
			}

			CString sLoginName = pUserResetAuthType->LoginName;
			CString sCurrentPassword;
			CString sNewPassword;
			
			if(pUserResetAuthType->Password) 
			{
				sCurrentPassword = *(pUserResetAuthType->Password);
			}
			if(pUserResetAuthType->NewPassword)
			{
				sNewPassword = *(pUserResetAuthType->NewPassword);
			}
						
			DEBUGL8("UserServices::ExecutePasswordResetLogin User Name : %s Password %s\n", sLoginName.c_str(), sCurrentPassword.c_str());
			
			//	First login using current password. 
			//If we get following status then We can proceed for password change. Refer to SP 869			
			CString sOperationStatus = UserServices::ExecuteLoginUserCommand(sLoginName,sCurrentPassword,pBOProxy);
			if(STG2_SUCCESS != sOperationStatus &&  "STATUS_AUTH_PASSWORD_RESET" != sOperationStatus 
				&& "STATUS_PASSWORD_EXPIRED" != sOperationStatus && "STATUS_PASSWORD_POLICY_EXPIRED" != sOperationStatus)
			{
				DEBUGL1("Login failed\n");
				CErrorCodeMapper::GenerateException(sOperationStatus, AUTH_MGR);
			}
										
			
			//Change password			
			/*if(sCurrentPassword == sNewPassword)
			{
				CErrorCodeMapper::GenerateException("STG2_REPEATED_PASSWORD");
			}*/
			if(STATUS_OK != UserServices::ExecuteChangeUserPasswordCommand(sLoginName, sCurrentPassword, sNewPassword, "Change", pBOProxy))
			{
				DEBUGL1("UserServices::ExecutePasswordResetLogin : Failed to execute change of user password\n");
				CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");
			}
				
			//Now we need to logoff and then login using new password
			CString sResponse;
						
			if(STATUS_OK != CUIControllerAdapter::SendCommand("Logoff", "Authentication/UserCredential", sResponse, pBOProxy))
			{
				DEBUGL1("UserServices::ExecutePasswordResetLogin : Sendcommand failed\n");
				CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");
			}

			// Get the result
			sOperationStatus = CUIControllerAdapter::GetOperationStatusString(sResponse);
			if(STG2_SUCCESS != sOperationStatus)
			{
				DEBUGL1("UserServices::ExecutePasswordResetLogin : Logoff failed \n");
				CErrorCodeMapper::GenerateException(sOperationStatus, AUTH_MGR);
			}
			
			//Now that we have changed password, login again
									
			SessionMgrRef sessionMgrRef = CSessionMgr::Acquire();
			sessionMgrRef->RemoveSession(strSession);   // Remove current session and destroy it as it is no longer needed.
			
			/* Commented Destroy becuase it degrades performance.
			
			//pBOProxy->Destroy();
			
			*/
			
			strSession = sessionMgrRef->CreateNewSession(soap);
			pBOProxy = sessionMgrRef->getBOProxyRef(strSession);
			
			sOperationStatus = UserServices::ExecuteLoginUserCommand(sLoginName,sNewPassword,pBOProxy);
			
			if(STG2_SUCCESS != sOperationStatus)
			{
				DEBUGL1("UserServices::ExecutePasswordResetLogin : Relogin failed when resetting\n");
				CErrorCodeMapper::GenerateException(sOperationStatus, AUTH_MGR);
			}
			return STATUS_OK;
		}
		
		Status UserServices::ExecuteDepartmentLogin(const BOProxyRef& pBOProxy, userauth__departmentAuthenticationType* pDeptAuth)
		{
			DEBUGL5("UserServices::ExecuteDepartmentLogin Enter\n");
			if(!pDeptAuth)
			{
				CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");
			}
			
			CString sDepartmentCode = pDeptAuth->DepartmentCode;
			CString sOperationStatus = ValidateDepartmentCode(pBOProxy, sDepartmentCode, true);
			if(STG2_SUCCESS != sOperationStatus)
			{
				DEBUGL1("UserServices::ExecuteDepartmentLogin Operation failed\n");
				// Although we are using Accouting plugin, the exception has been generated in the name of Auth plugin as the operation is login and its error code differs.
				//Refer to Stage2ErrorMap.xml for more details of error code.
				CErrorCodeMapper::GenerateException(sOperationStatus, AUTH_MGR);  
			}
			return STATUS_OK;
		}
		
		
		CString UserServices::ExecuteLoginUserCommand(CString strLoginName,CString strPasswd,BOProxyRef &pBOProxy, CString sDomainName)
		{
			DEBUGL8("UserServices::ExecuteLoginUserCommand enter \n");
			if(!pBOProxy) 
			{
				CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");
			}
			
			CUIControllerAdapter::SetXMLValue("", "<Authentication></Authentication>", pBOProxy); // to clear delta document.
			
			CString  strUserCredentialPath   = "Authentication/UserCredential";
			std::vector<CString> xPaths;
			std::vector<CString> values;
	
			values.push_back(strLoginName);
			xPaths.push_back(strUserCredentialPath + "/userName");
			values.push_back(strPasswd);
			xPaths.push_back(strUserCredentialPath + "/passwd");
			if(!sDomainName.empty())
			{
				values.push_back(sDomainName);
				xPaths.push_back(strUserCredentialPath + "/domainName");
			}

			DEBUGL8("ExecuteLoginUserCommand : calling SetXPathValues\n");

			CUIControllerAdapter::SetXPathValues(xPaths, values, pBOProxy, true);

			CString sOperationStatus;
			if(!strncasecmp(strLoginName.c_str(),"Admin" , CString("Admin").size()+1))
			{
				Ref<AuthenticationLibraryInterface> pAuthLibIF = AuthenticationLibraryInterface::create();
				CString sSessionID = pBOProxy->getSessionID();
				Status status = pAuthLibIF->AdminLoginFromStage2(sSessionID);
				if(status == STATUS_OK)
				{
					sOperationStatus = STG2_SUCCESS;
				}
				else if(status == STATUS_AL_SEC_WRONG_USER_CRED)
				{
					DEBUGL1("UserServices::GetSecuritySettings : AdminLoginFromStage2 failed \n");
					throw CStg2Exception(eSenderError, "ERR_EBS_AUTH_INVALID_CREDENTIALS",SOAP_FAULT, "Invalid user name/password combination");
				}
				else
				{
					DEBUGL1("UserServices::GetSecuritySettings : AdminLoginFromStage2 failed \n");
					CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");
				}
			}
			else
			{
				// Execute the command
				CString strResponse;
				strResponse.clear();

				//Set application name
				vector<commandParam> vectParams;
				commandParam cmdParam;
				CString appName         = "SOAPXML";
				cmdParam.strParamValue  = appName;
				cmdParam.strParamName   = "appName";
				cmdParam.mapAttributes.insert(make_pair("contentType","XPath"));
				vectParams.push_back(cmdParam);
		
				if(STATUS_OK != CUIControllerAdapter::SendCommand("Login", "Authentication/UserCredential", vectParams, strResponse, pBOProxy))
				{
					DEBUGL1("UserServices::ExecuteLoginUserCommand : SendCommand failed\n");
					CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");
				}
		
				// Get the result
				sOperationStatus = CUIControllerAdapter::GetOperationStatusString(strResponse);
			}
			DEBUGL8("UserServices::ExecuteLoginUserCommand Exit \n");
			return sOperationStatus;
					
		}
		
		CString UserServices::ValidateDepartmentCode(const BOProxyRef& pBOProxy, const CString& sDepartmentCode , bool bForLogin)
		{
			DEBUGL5("UserServices::ValidateDepartmentCode Enter\n");
			
			vector<commandParam> vCommandParams;	
			
			commandParam cmdParam;		
			cmdParam.strParamName = "billingCode";
			cmdParam.strParamValue = sDepartmentCode;
			cmdParam.mapAttributes.insert(make_pair("contentType","Value"));
			vCommandParams.push_back(cmdParam);
			
			cmdParam.strParamName = "isUsedForLogin";			
			cmdParam.mapAttributes.insert(make_pair("contentType","Value"));
			if (bForLogin) cmdParam.strParamValue = "true";
			else cmdParam.strParamValue = "false";
			vCommandParams.push_back(cmdParam);
						
			CString sResponse;
			if(STATUS_OK != CUIControllerAdapter::SendCommand("ValidateBillingCode","Accounting/BillingCodes", vCommandParams, sResponse, pBOProxy))
			{
				DEBUGL1("UserServices::ValidateDepartmentCode SendCommand failed\n");
				CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");
			}
			
			return CUIControllerAdapter::GetOperationStatusString(sResponse);
			
		}
		
	} //NS Stage2
}	//NF AL

using namespace al;
using namespace stage2;
using namespace al::uicontroller;
using namespace ci::operatingenvironment;
using namespace ci::hierarchicaldb;


SOAP_FMAC5 int SOAP_FMAC6 __user__login(struct soap* soap, _userauth__login *userauth__login, _userauth__loginResponse *userauth__loginResponse)
{
	DEBUGL8("Stage2::__user__login entry\n");
	
	if(!userauth__login)
	{
		CErrorCodeMapper::GenerateException("STG2_MISSING_PARAM");
	}
	
	SessionMgrRef sessionMgrRef = CSessionMgr::Acquire();
	CString strSession = sessionMgrRef->CreateNewSession(soap);
	BOProxyRef pBOProxy = sessionMgrRef->getBOProxyRef(strSession);
		
	try
	{
		// Determine security settings - userAuthentication, departmentAuthentication - enabled/disabled
		bool bUserAuth = false;
		bool bDeptAuth = false;
						
		userauth__departmentAuthenticationType * pDeptAuth = dynamic_cast<userauth__departmentAuthenticationType*>(userauth__login->Authentication);
		userauth__userAuthenticationType* pUserAuthType = dynamic_cast<userauth__userAuthenticationType*>(userauth__login->Authentication);
		userauth__userPasswordResetAuthenticationType* pUserResetAuthType = dynamic_cast<userauth__userPasswordResetAuthenticationType*>(userauth__login->Authentication);		
		
		CString sLoginName;
		CString sPassword;
		CString sDomainName;
			
		if(pUserAuthType)	
		{
			sLoginName = pUserAuthType->LoginName;
			if(pUserAuthType->Password) sPassword = *(pUserAuthType->Password);
			if(pUserAuthType->Domain) sDomainName = *(pUserAuthType->Domain);
		}
		
		if(UserServices::GetSecuritySettings(pBOProxy, bUserAuth, bDeptAuth, sLoginName) != STATUS_OK)
		{
			DEBUGL1("__user__login UserServices::GetSecuritySettings failed\n");
			CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");
		}
		
		userauth__loginResponse->Token = soap_new_common__sessionTokenType(soap, -1); 	
		// return token if following conditions are true
		//(1). Both dept and user auth are disabled.	
		//(2). No Authentication info has been suppiled or Auth object is User Auth type and all credentials are empty.
		if(!bUserAuth && !bDeptAuth 
			&& (!userauth__login->Authentication || (pUserAuthType &&  sPassword.empty() && sLoginName.empty() && sDomainName.empty())))
		{
			userauth__loginResponse->Token->Token = strSession;
			sessionMgrRef->SetLoginType(strSession, eNoLogin);
			sessionMgrRef->SetLoginID(strSession, ""); // LoginID is empty because no credentials have been used.
		}		
		else 
		{
			// Login request contains userAuth object. 
			//Check for credentials irrespective of Authetication Settings on controller.
			if(pUserAuthType) 
			{
				if(pUserAuthType->Domain && pUserAuthType->Domain->empty()) 
				{				
					CErrorCodeMapper::GenerateException("STG2_EMPTY_DOMAIN_NAME");			
				}
				CString sOperationStatus = UserServices::ExecuteLoginUserCommand(sLoginName,sPassword,pBOProxy, sDomainName);
				if(STG2_SUCCESS != sOperationStatus)
				{
					DEBUGL1("Login failed\n");
					CErrorCodeMapper::GenerateException(sOperationStatus, AUTH_MGR);
				}
				userauth__loginResponse->Token->Token = strSession;
				sessionMgrRef->SetLoginType(strSession, eUserLogin);
				CString sLoginID = CUIControllerAdapter::GetTextNodeValue("Authentication/UserCredential/User/@ID", pBOProxy);
				sessionMgrRef->SetLoginID(strSession, sLoginID);
				
							
			}
			else if (pUserResetAuthType && bUserAuth)  // we are changing password - happens when admin has reset password or first login
			{
				if(STATUS_OK != UserServices::ExecutePasswordResetLogin(soap, pBOProxy, pUserResetAuthType, strSession))
				{
					DEBUGL1("UserServices::ExecutePasswordResetLogin  failed\n");
					CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");
				}
				userauth__loginResponse->Token->Token = strSession;
				sessionMgrRef->SetLoginType(strSession, eUserLogin);
				CString sLoginID = CUIControllerAdapter::GetTextNodeValue("Authentication/UserCredential/User/@ID", pBOProxy);
				sessionMgrRef->SetLoginID(strSession, sLoginID);
				
			}
			else if(!bUserAuth && bDeptAuth && pDeptAuth) //only department login is enabled and object is of department type.
			{
				if(STATUS_OK != UserServices::ExecuteDepartmentLogin(pBOProxy, pDeptAuth))
				{			
					DEBUGL1("UserServices::ExecuteDepartmentLogin  failed\n");
					CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");
				}
				userauth__loginResponse->Token->Token = strSession;
				sessionMgrRef->SetLoginType(strSession, eDepartmentLogin);
				sessionMgrRef->SetLoginID(strSession, pDeptAuth->DepartmentCode);
			}
			else
			{
				CErrorCodeMapper::GenerateException("STG2_INVALID_AUTH_TYPE");
			}
		}
	}
	catch(CStg2Exception &ex)
	{
		// Remove session from map and destroy the session.
		sessionMgrRef->RemoveSession(strSession);
		/* Commented Destroy becuase it degrades performance.
			
			//pBOProxy->Destroy();
			
			*/
      
		DEBUGL1("Stage2::__user__login:Stg2Exception:Description = %s\n", ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), ex.getFaultType(), ex.getErrorType()) != STATUS_OK)
			DEBUGL1("__user__login CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}
	catch(CException &ex)
	{
		// Remove session from map and destroy the session.
		sessionMgrRef->RemoveSession(strSession);
		/* Commented Destroy becuase it degrades performance.
			
			//pBOProxy->Destroy();
			
			*/
		DEBUGL1("Stage2::__user__login:CException:Description = %s\n",ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap,ex.C_Str(),"",eReceiverError) != STATUS_OK)
			DEBUGL1("__user__login CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}
	DEBUGL8("Stage2::__user__login exit\n");


	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __stg2__login(struct soap* soap, _userauth__login *userauth__login, _userauth__loginResponse *userauth__loginResponse)
{
	if(!CCommonFunctions::IsOutputManagementEnabled( ))
	{
		stage2ErrorStruct stError;
		CErrorCodeMapper::MapInternalErrCodeToStage2Fault(stError, "STG2_UNSUPPORTED_OPERATION");
		CCommonFunctions::GenerateSoapFault(soap, stError.strDescription, stError.strErrCode, stError.eErrorType);
		return SOAP_FAULT;
		
	}
	return __user__login(soap, userauth__login, userauth__loginResponse);
}

SOAP_FMAC5 int SOAP_FMAC6 __user__logout(struct soap* soap, _userauth__logout *userauth__logout, _userauth__logoutResponse *userauth__logoutResponse)
{
	try
	{
		DEBUGL8("Stage2::__user__logout entry\n");
		BOProxyRef boProxy = CCommonFunctions::getBOProxyRef(soap , true);

		if(!boProxy)
		{
			DEBUGL1("Stage2::__user__logout Invalid Session\n");
			CErrorCodeMapper::GenerateException("STG2_INVALID_SESSION");
		}
		
		if(!userauth__logout)
		{
			CErrorCodeMapper::GenerateException("STG2_MISSING_PARAM");
		}
		
	//	DEBUGL4("Login ID is %s\n", CCommonFunctions::GetLoggedInUserID(soap).c_str());
	
		CString strSession;
		if(soap && soap->header && soap->header->common__sessionHeader)
		{
			strSession = soap->header->common__sessionHeader->Token;
		}
		
		DEBUGL8("Stage2::__user__logout : session name provided in header is ++%s++\n", strSession.c_str());
		
		
		SessionMgrRef sessionMgrRef = CSessionMgr::Acquire();
		if(sessionMgrRef->IsUserLogin(strSession))
		{
			CString sUserName = CCommonFunctions::GetLoggedInUserName(boProxy);
			if(!strncasecmp(sUserName.c_str(),"Admin" , CString("Admin").size()))
			{
				Ref<AuthenticationLibraryInterface> pAuthLibIF = AuthenticationLibraryInterface::create();
				if(STATUS_OK != pAuthLibIF->AdminLogoutFromStage2(strSession))
				{
					DEBUGL1("UserServices::GetSecuritySettings : AdminLogoutFromStage2 failed \n");
					CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");
				}
				
			}
			
			else
			{
				vector<commandParam> vectParams;
				commandParam cmdParam;
				cmdParam.strParamValue = "SOAPXML";
				cmdParam.strParamName = "appName";
				cmdParam.mapAttributes.insert(make_pair("contentType","Value"));
				vectParams.push_back(cmdParam);
			
				CString strResponse;	
				if(STATUS_OK != CUIControllerAdapter::SendCommand("Logoff", "Authentication/UserCredential",vectParams,strResponse, boProxy))
				{
					DEBUGL1("__user__logout Logoff SendCommand failed\n");
					CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");
				}
		
				CString sOperationStatus = CUIControllerAdapter::GetOperationStatusString(strResponse);
				if(STG2_SUCCESS != sOperationStatus)
				{
					CErrorCodeMapper::GenerateException(sOperationStatus, AUTH_MGR);
				}
			}
		}
		else if(sessionMgrRef->IsDepartmentLogin(strSession))
		{
			CString strResponse;	
			if(STATUS_OK != CUIControllerAdapter::SendCommand("NotifyLogout", "Accounting/BillingCodes", strResponse, boProxy))
			{
				DEBUGL1("__user__logout NotifyLogout SendCommand failed\n");
				CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");
			}
	
			CString sOperationStatus = CUIControllerAdapter::GetOperationStatusString(strResponse);
			if(STG2_SUCCESS != sOperationStatus)
			{				
				CErrorCodeMapper::GenerateException(sOperationStatus, ACC_MGR);
			}		
		
		}				
		sessionMgrRef->RemoveSession(strSession,true);
	
		userauth__logoutResponse->Result = common__operationResultType__OperationSucceeded;	
		DEBUGL8("Stage2::__user__logout exit\n");
	}		
	catch(CStg2Exception &ex)
	{
		DEBUGL1("Stage2::__user__logout:Stg2Exception:Description = %s\n", ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), ex.getFaultType(), ex.getErrorType()) != STATUS_OK)
			DEBUGL1("__user__logout CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}
	catch(CException &ex)
	{
      DEBUGL1("Stage2::__user__logout:CException:Description = %s\n",ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap,ex.C_Str(),"",eReceiverError) != STATUS_OK)
			DEBUGL1("__user__logout CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}
	return SOAP_OK;

}

SOAP_FMAC5 int SOAP_FMAC6 __stg2__logout(struct soap* soap, _userauth__logout *userauth__logout, _userauth__logoutResponse *userauth__logoutResponse)
{
	if(!CCommonFunctions::IsOutputManagementEnabled( ))
	{
		stage2ErrorStruct stError;
		CErrorCodeMapper::MapInternalErrCodeToStage2Fault(stError, "STG2_UNSUPPORTED_OPERATION");
		CCommonFunctions::GenerateSoapFault(soap, stError.strDescription, stError.strErrCode, stError.eErrorType);
		return SOAP_FAULT;
	}
	return __user__logout(soap, userauth__logout, userauth__logoutResponse);
}

user__basicUserInfoListType* UserServices::getUserList(soap* soap, const BOProxyRef& pBOProxy)
{
	DEBUGL8("UserServices::getUserList Enter\n");
	if(STATUS_OK != ExecuteGetUserListCommand(pBOProxy,false))
	{
		DEBUGL1("UserServices::getUserList ExecuteGetUserListCommand failed\n");
		CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");	
	}
	
	NodeRef pNode = NULL;
	if(STATUS_OK != pBOProxy->GetValue(USERS_XPATH, pNode) || !pNode)
	{
		DEBUGL1("Failed to GetValue\n");
		CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");
	}
	
	NodeListRef pChildren = pNode->getChildNodes();	
	user__basicUserInfoListType* pUserList =  soap_new_user__basicUserInfoListType(soap, -1);
		
	for ( unsigned long i = 0; i < pChildren->getLength(); i++)
	{
		NodeRef pUserNode = pChildren->item(i);
		if(!pUserNode) continue;
		user__basicUserInfoType * User = soap_new_user__basicUserInfoType(soap,-1);
		User->Name = CCommonFunctions::GetValue(pUserNode, "Information/name");
		User->UserID = (ElementRef(pUserNode))->getAttribute("ID");
		CString sDomain = chelper::GetPathValue(pUserNode, "Information/domainName");
		CString sStatus = chelper::GetPathValue(pUserNode, "Information/userStatus");
		CString sExternalUser = chelper::GetPathValue(pUserNode,"Information/isExternalUser");
		if(sDomain.empty() == false)
		{
			
			User->Domain = soap_new_std__string(soap,-1);
			*(User->Domain) = sDomain;
		}
		if(sStatus.empty() == false)
		{
			
			User->Status = soap_new_std__string(soap,-1);
			*(User->Status) = sStatus;
		}
		if(sExternalUser.empty() == false)
		{
			User->ExternalUser = static_cast<bool *>(soap_malloc(soap,sizeof(bool)));
			*(User->ExternalUser) = sExternalUser == "true" ? true : false;
		}
		/*		CString sUserName = CCommonFunctions::GetValue(pUserNode, "Information/name");
		if(sUserName == NODE_DOES_NOT_EXIST || sUserName.empty()) continue;*/
		
		pUserList->User.push_back(User);
		//DEBUGL8("UserName is ++%s++\n", sUserName.c_str());
		
	}		
	DEBUGL8("UserServices::getUserList Exit\n");
	return pUserList;
}

user__basicUserInfoType* UserServices::getBasicUserInfo(soap* soap, const NodeRef &pUserNode )
{
	DEBUGL8("UserServices::getBasicUserInfo Enter\n");
	user__basicUserInfoType * User = soap_new_user__basicUserInfoType(soap,-1);
	if(!pUserNode)
	{
		DEBUGL1("pUserNode doesnot exist\n");
		CErrorCodeMapper::GenerateException("STATUS_FAILED");
	}
	User->UserID =  (ElementRef(pUserNode))->getAttribute("ID");
	User->Name = CCommonFunctions::GetValue(pUserNode, "Information/name");
	CString sDomain = CCommonFunctions::GetValue(pUserNode, "Information/domainName");
	CString sStatus = CCommonFunctions::GetValue(pUserNode, "Information/userStatus");
	CString sExternalUser = CCommonFunctions::GetValue(pUserNode,"Information/isExternalUser");
	if(sDomain.empty() == false  && sDomain != NODE_DOES_NOT_EXIST)
	{
		
		User->Domain = soap_new_std__string(soap,-1);
		*(User->Domain) = sDomain;
	}
	if(sStatus.empty() == false && sStatus != NODE_DOES_NOT_EXIST)
	{
		
		User->Status = soap_new_std__string(soap,-1);
		*(User->Status) = sStatus;
	}
	if(sExternalUser.empty() == false && sExternalUser != NODE_DOES_NOT_EXIST)
	{
		User->ExternalUser = static_cast<bool *>(soap_malloc(soap,sizeof(bool)));
		*(User->ExternalUser) = sExternalUser == "true" ? true : false;
	}
	DEBUGL8("UserServices::getUserList Exit\n");
	return User;
}

SOAP_FMAC5 int SOAP_FMAC6 __stg2__getUserList(struct soap* soap, _user__getUserList *user__getUserList, _user__getUserListResponse *user__getUserListResponse)
{
	DEBUGL8("Stage2::__stg2__getUserList entry\n");
	if(!CCommonFunctions::IsOutputManagementEnabled( ))
	{
		stage2ErrorStruct stError;
		CErrorCodeMapper::MapInternalErrCodeToStage2Fault(stError, "STG2_UNSUPPORTED_OPERATION");
		CCommonFunctions::GenerateSoapFault(soap, stError.strDescription, stError.strErrCode, stError.eErrorType);
		return SOAP_FAULT;
	}
	return __user__getUserList(soap,user__getUserList,user__getUserListResponse);
}
SOAP_FMAC5 int SOAP_FMAC6 __user__getUserList(struct soap* soap, _user__getUserList *user__getUserList, _user__getUserListResponse *user__getUserListResponse)
{
	DEBUGL8("Stage2::__user__getUserList entry\n");
	CString        operationName     = "getUserList";
	try
	{
		 //Check if we have a conflicting operation in progress
		if(CLongRunningOpHelper::IsAnotherConflictingOperationInProgress(soap, operationName))
		{
			// Don't use GenerateException since it will update Running operation status to "false" which we don't want.
			stage2ErrorStruct stError;
			CErrorCodeMapper::MapInternalErrCodeToStage2Fault(stError, "STG2_CONF_OP_INPROGRESS");
			CCommonFunctions::GenerateSoapFault(soap, stError.strDescription, stError.strErrCode, stError.eErrorType);
			return SOAP_FAULT;	
		}
		else
		{
			//Add this operation to the current running op list
			CLongRunningOpHelper::UpdateRunningOpStatus(soap, operationName, true);
		}
		
		BOProxyRef pBOProxy = CCommonFunctions::getBOProxyRef(soap);
		if(!pBOProxy)
		{
			DEBUGL1("__user__getUserList Invalid Session\n");
			CErrorCodeMapper::GenerateException("STG2_INVALID_SESSION");		
		}
		
		if(!user__getUserList)
		{
			CErrorCodeMapper::GenerateException("STG2_MISSING_PARAM");
		}
				
		user__getUserListResponse->UserList = UserServices::getUserList(soap, pBOProxy);
		
		//Remove this operation from the current running op list
		CLongRunningOpHelper::UpdateRunningOpStatus(soap, operationName, false);	
		
	}
	catch(CStg2Exception &ex)
	{
		DEBUGL1("Stage2::__user__getUserList caught Stg2Exception Exception. Description = %s\n", ex.C_Str());
		CLongRunningOpHelper::UpdateRunningOpStatus(soap, operationName, false);
		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), ex.getFaultType(), ex.getErrorType()) != STATUS_OK)
		DEBUGL4("__user__getUserList CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}
	catch(CException &ex)
	{
		DEBUGL1("Stage2::__user_getUserList caught CException exception. Description = %s\n",ex.C_Str());
		CLongRunningOpHelper::UpdateRunningOpStatus(soap, operationName, false);
		if(CCommonFunctions::GenerateSoapFault(soap,ex.C_Str(),"",eReceiverError) != STATUS_OK)
		DEBUGL4("__user__getUserList CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}

	DEBUGL8("Stage2::__user__getUserList exit\n");
	return SOAP_OK;
}
SOAP_FMAC5 int SOAP_FMAC6 __stg2__getDepartmentList(struct soap* soap, _user__getDepartmentList *user__getDepartmentList, _user__getDepartmentListResponse *user__getDepartmentListResponse)
{
	if(!CCommonFunctions::IsOutputManagementEnabled( ))
	{
		stage2ErrorStruct stError;
		CErrorCodeMapper::MapInternalErrCodeToStage2Fault(stError, "STG2_UNSUPPORTED_OPERATION");
		CCommonFunctions::GenerateSoapFault(soap, stError.strDescription, stError.strErrCode, stError.eErrorType);
		return SOAP_FAULT;
	}
	return __user__getDepartmentList(soap, user__getDepartmentList, user__getDepartmentListResponse);
}
SOAP_FMAC5 int SOAP_FMAC6 __stg2__getDepartmentUserList(struct soap* soap, _user__getDepartmentUserList *user__getDepartmentUserList, _user__getDepartmentUserListResponse *user__getDepartmentUserListResponse)
{
	if(!CCommonFunctions::IsOutputManagementEnabled( ))
	{
		stage2ErrorStruct stError;
		CErrorCodeMapper::MapInternalErrCodeToStage2Fault(stError, "STG2_UNSUPPORTED_OPERATION");
		CCommonFunctions::GenerateSoapFault(soap, stError.strDescription, stError.strErrCode, stError.eErrorType);
		return SOAP_FAULT;
	}
	return __user__getDepartmentUserList(soap, user__getDepartmentUserList, user__getDepartmentUserListResponse);
}
SOAP_FMAC5 int SOAP_FMAC6 __stg2__getGroupList(struct soap* soap, _user__getGroupList *user__getGroupList, _user__getGroupListResponse *user__getGroupListResponse)
{
	DEBUGL8("Stage2::__stg2__getGroupList entry\n");
	if(!CCommonFunctions::IsOutputManagementEnabled( ))
	{
		stage2ErrorStruct stError;
		CErrorCodeMapper::MapInternalErrCodeToStage2Fault(stError, "STG2_UNSUPPORTED_OPERATION");
		CCommonFunctions::GenerateSoapFault(soap, stError.strDescription, stError.strErrCode, stError.eErrorType);
		return SOAP_FAULT;
	}
	
	return	__user__getGroupList(soap,user__getGroupList,user__getGroupListResponse);
}



common__stringListType* UserServices::GetGroupList(soap* soap, const BOProxyRef& pBOProxy)
{
	DEBUGL8("UserServices::GetGroupList Enter\n");
	
	if(STATUS_OK != ExecuteGetGroupListCommand(pBOProxy))
	{
		DEBUGL1("UserServices::GetGroupList ExecuteGetGroupListCommand failed\n");
		CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");
	}
	
	NodeRef pNode = NULL;
	if(STATUS_OK != pBOProxy->GetValue(GROUPS_XPATH, pNode) || !pNode)
	{
		DEBUGL1("UserServices::GetGroupList GetValue failed\n");
		CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");
	}
	
	common__stringListType * pGroupList =  soap_new_common__stringListType(soap,-1);
		
	Ref<NodeList> pGroups  = pNode->getChildNodes();
	for (unsigned long i = 0; i < pGroups->getLength(); i++)
	{
		Ref<Node> pGroup = pGroups->item(i);
		if(!pGroup) continue;
		
		CString sGroupName = CCommonFunctions::GetValue(pGroup, "Information/name");
		if(NODE_DOES_NOT_EXIST == sGroupName || sGroupName.empty()) continue;
		
		pGroupList->Item.push_back(sGroupName);
		DEBUGL8("%d\t name : %s\n",i,sGroupName.c_str());
	}	
	DEBUGL8("UserServices::GetGroupList Exit\n");
	return 	pGroupList;


}



SOAP_FMAC5 int SOAP_FMAC6 __user__getGroupList(struct soap* soap, _user__getGroupList *user__getGroupList, _user__getGroupListResponse *user__getGroupListResponse)
{
	DEBUGL8("__user__getGroupList entry\n");
	try
	{
		BOProxyRef pBOProxy = CCommonFunctions::getBOProxyRef(soap);
		if(!pBOProxy)
		{
			DEBUGL1("__user__getGroupList Invalid Session\n");
			CErrorCodeMapper::GenerateException("STG2_INVALID_SESSION");
		}
				
		if(!user__getGroupList) 
		{
			CErrorCodeMapper::GenerateException("STG2_MISSING_PARAM");
		}

		user__getGroupListResponse->GroupList = UserServices::GetGroupList(soap, pBOProxy);
		
	}
	catch(CStg2Exception & ex)
	{
		DEBUGL1("Stage2::__user__getGroupList caught Stg2Exception Exception. Description = %s\n", ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), ex.getFaultType(), ex.getErrorType()) != STATUS_OK)
		DEBUGL4("__user__getGroupList CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}
	catch(CException & ex)
	{
		DEBUGL1("Stage2::__user__getGroupList caught CException Exception. Description = %s\n", ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), "", eReceiverError) != STATUS_OK)
		DEBUGL4("__user__getGroupList CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}


	DEBUGL8("Stage2::__user__getGroupList exit\n");
	return SOAP_OK;
}
SOAP_FMAC5 int SOAP_FMAC6 __stg2__getGroupUserList(struct soap* soap, _user__getGroupUserList *user__getGroupUserList, _user__getGroupUserListResponse *user__getGroupUserListResponse)
{
	DEBUGL8("Stage2::__stg2__getGroupUserList entry\n");
	if(!CCommonFunctions::IsOutputManagementEnabled( ))
	{
		stage2ErrorStruct stError;
		CErrorCodeMapper::MapInternalErrCodeToStage2Fault(stError, "STG2_UNSUPPORTED_OPERATION");
		CCommonFunctions::GenerateSoapFault(soap, stError.strDescription, stError.strErrCode, stError.eErrorType);
		return SOAP_FAULT;
	}
	return __user__getGroupUserList(soap,user__getGroupUserList,user__getGroupUserListResponse);
}

user__basicUserInfoListType* UserServices::getGroupUserList(soap* soap, const BOProxyRef& pBOProxy, const CString& sGroupName)
{
	DEBUGL8("UserServices::getGroupUserList Enter\n");
	if(STATUS_OK != ExecuteGetGroupInfoCommand(sGroupName,pBOProxy))
	{
		DEBUGL1("ExecuteGetGroupInfoCommand failed\n");
		CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");
	}
	CUIControllerAdapter::SetXMLValue("", "<UserManager></UserManager>" , pBOProxy); // to clear delta document.
	
	// Set an empty UserManager/Users DOM.
	vector<CString> xPaths;
	vector<CString> values;
	xPaths.push_back("UserManager/Users");
	values.push_back("");
	CUIControllerAdapter::SetXPathValues(xPaths, values, pBOProxy);
		
	commandParam p1, p2;
	// searchDetails Param
	p1.strParamName = "searchDetails";
	p1.strParamValue = "UserManager";
	p1.mapAttributes.insert(make_pair("contentType","XPath"));
	p1.mapAttributes.insert(make_pair("searchType","GroupName"));
	// searchInfo Param
	p2.strParamName = "searchInfo";
	p2.strParamValue = "";
	p2.mapAttributes.insert(make_pair("value1",sGroupName));
	p2.mapAttributes.insert(make_pair("value2",""));
	vector<commandParam> vectParams;
	vectParams.push_back(p1);
	vectParams.push_back(p2);

	CString sResponse;
	if(STATUS_OK != CUIControllerAdapter::SendCommand("SearchUsers", "UserManager/Users", vectParams, sResponse, pBOProxy))
	{
		DEBUGL1("Failed to SendCommand\n");
		CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");
	
	}
	CString sOperationStatus = CUIControllerAdapter::GetOperationStatusString(sResponse);
	if(STG2_SUCCESS != sOperationStatus  )
	{
		if("STATUS_PERMISSION_CHECK_ERROR" ==sOperationStatus)
		{
			DEBUGL1("Received STATUS_PERMISSION_CHECK_ERROR for Search use by name\n");
			CErrorCodeMapper::GenerateException(sOperationStatus);
		}
	}
	user__basicUserInfoListType* pUserList =  soap_new_user__basicUserInfoListType(soap, -1);
	NodeRef pNode;
	CString parentXpath = "UserManager/Users";
	pBOProxy->GetValue(parentXpath.c_str(),pNode);
	if(!pNode)
	return pUserList;

	NodeListRef pChildren = pNode->getChildNodes();	
	for ( ulong i=0; i < pChildren->getLength(); i++)
	{
		NodeRef	pChildNode = pChildren->item(i);
		user__basicUserInfoType * User = soap_new_user__basicUserInfoType(soap,-1);

		User = UserServices::getBasicUserInfo(soap, pChildNode);
		if(User)
			pUserList->User.push_back(User);

	}
	DEBUGL8("UserServices::getGroupUserList Exit\n");
	return pUserList;
}

SOAP_FMAC5 int SOAP_FMAC6 __user__getGroupUserList(struct soap* soap, _user__getGroupUserList *user__getGroupUserList, _user__getGroupUserListResponse *user__getGroupUserListResponse)
{
	DEBUGL8("__user__getGroupUserList entry\n");
	CString        operationName     = "getGroupUserList";
	try
	{
		 //Check if we have a conflicting operation in progress
		if(CLongRunningOpHelper::IsAnotherConflictingOperationInProgress(soap, operationName))
		{
			// Don't use GenerateException since it will update Running operation status to "false" which we don't want.
			stage2ErrorStruct stError;
			CErrorCodeMapper::MapInternalErrCodeToStage2Fault(stError, "STG2_CONF_OP_INPROGRESS");
			CCommonFunctions::GenerateSoapFault(soap, stError.strDescription, stError.strErrCode, stError.eErrorType);
			return SOAP_FAULT;	
		}
		else
		{
			//Add this operation to the current running op list
			CLongRunningOpHelper::UpdateRunningOpStatus(soap, operationName, true);
		}
	  
		BOProxyRef pBOProxy = CCommonFunctions::getBOProxyRef(soap);
		if(!pBOProxy)
		{
			DEBUGL1("__user__getGroupUserList Invalid Session\n");
			CErrorCodeMapper::GenerateException("STG2_INVALID_SESSION");
		}
		
		if(!user__getGroupUserList)
		{
			CErrorCodeMapper::GenerateException("STG2_MISSING_PARAM");
		}

		CString sGroupName = user__getGroupUserList->GroupName;
		if(user__getGroupUserList->GroupName.empty())
		{
			DEBUGL1("Group Name is empty\n");
			CErrorCodeMapper::GenerateException("STG2_EMPTY_GROUP_NAME");
		}

		user__getGroupUserListResponse->UserList = UserServices::getGroupUserList(soap,pBOProxy,sGroupName);
		 //Remove this operation from the current running op list
		CLongRunningOpHelper::UpdateRunningOpStatus(soap, operationName, false);
				
	}
	catch(CStg2Exception & ex)
	{
		DEBUGL1("Stage2::__user__getGroupList caught Stg2Exception Exception. Description = %s\n", ex.C_Str());
		CLongRunningOpHelper::UpdateRunningOpStatus(soap, operationName, false);
		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), ex.getFaultType(), ex.getErrorType()) != STATUS_OK)
		DEBUGL4("__user__getGroupList CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}
	catch(CException & ex)
	{
		DEBUGL1("Stage2::__user__getGroupList caught CException Exception. Description = %s\n", ex.C_Str());
		CLongRunningOpHelper::UpdateRunningOpStatus(soap, operationName, false);
		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), "", eReceiverError) != STATUS_OK)
		DEBUGL4("__user__getGroupList CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}

	DEBUGL8("Stage2::user__getGroupUserList exit\n");
	return SOAP_OK;
}

/* CString UserServices::ExecuteUpdateUserInfo(Ref<BOProxy>& pBOProxy)
{
	vector<commandParam> vCommandParam;
	commandParam cmdParam;
	cmdParam.strParamValue = USERMANAGER_NEW_USER;
	cmdParam.strParamName = "userDetails";
	cmdParam.mapAttributes.insert(make_pair("contentType","XPath"));
	vCommandParam.push_back(cmdParam);
	
	cmdParam.strParamValue = "ID";
	cmdParam.strParamName = "searchBy";
	cmdParam.mapAttributes.insert(make_pair("contentType","Value"));
	vCommandParam.push_back(cmdParam);
	
	
	CString sResponse;
	if(STATUS_OK != CUIControllerAdapter::SendCommand("UpdateUserInfo", USERS_XPATH, vCommandParam,sResponse,pBOProxy))
	{
		DEBUGL1("Failed to SendCommand\n");
		CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");
	}
	
	CString sOperationStatus = CUIControllerAdapter::GetOperationStatusString(sResponse);
	DEBUGL8("UserServices::CreateOrEditUser exit \n");
	return sOperationStatus;

} */


/* CString UserServices::setUserInfoDelta(Ref<BOProxy>& pBOProxy, common__userInfoType* pUserInfo)
{
	SetUserInfo(pUserInfo,USERMANAGER_NEW_USER,pBOProxy,"UpdateUserInfo");	
	
	return STG2_SUCCESS;

} */


/* CString UserServices::setUserInfo(Ref<BOProxy>& pBOProxy, common__userInfoType* pUserInfo) 
{
	DEBUGL8("UserServices::setUserInfo entry\n");
				
	CString sOperationStatus = setUserInfoDelta(pBOProxy, pUserInfo);
	if(STG2_SUCCESS != sOperationStatus)
	{
		return sOperationStatus;
	}
		
	sOperationStatus = ExecuteUpdateUserInfo(pBOProxy);
	
	DEBUGL8("UserServices::setUserInfo exit\n");
	return sOperationStatus;
} */

Status UserServices::setUserInfoList(soap* soap, Ref<BOProxy>& pBOProxy, user__userInfoListType* pUserInfoList) 
{
	DEBUGL8("UserServices::setUserInfoList enter\n");
	
	vector<common__userInfoType* >::iterator iter  = pUserInfoList->UserInfo.begin();
	vector<common__userInfoType* >::iterator iterEnd = pUserInfoList->UserInfo.end();
	
	for(;iter != iterEnd; ++iter) 
	{
		CUIControllerAdapter::SetXMLValue("", "<UserManager></UserManager>", pBOProxy); // to clear delta document.
		CString sOperationStatus = ExecuteCreateOrEditUserCommand(*iter,"UpdateUserInfo",pBOProxy);
		if(STG2_SUCCESS != sOperationStatus)
		{
			//STATUS_PERMISSION_CHECK_ERROR could mean that a normal user was trying to set its own info.
			// In this case we should allow a normal user to change its language and key board layout by calling UpdateOwnUserInfo
			if("STATUS_PERMISSION_CHECK_ERROR" == sOperationStatus || "STATUS_USER_UNAUTHORIZED" == sOperationStatus)
			{
				CString sUserID =  *((*iter)->UserID);
				// Check if logged in user and  user to be modified are same.
				if(CCommonFunctions::GetLoggedInUserID(soap) == sUserID) 
				{
					CUIControllerAdapter::SetXMLValue("", "<UserManager></UserManager>", pBOProxy); // to clear delta document.
					CString sStatus = ExecuteCreateOrEditUserCommand(*iter,"UpdateOwnUserInfo",pBOProxy);
					if(STG2_SUCCESS != sStatus)
					{
						// if UpdateOwnUserInfo fails then return its error by changing sOperationStatus to sStatus.
						sOperationStatus = sStatus;
						break;
					}
					else
					{	// UpdateOwnUserInfo succeeds then continue like UpdateUserInfo passed.
						continue;
					}				
				}		
			}
			
			
			if(pUserInfoList->UserInfo.begin() == iter)
			{
				DEBUGL1("UserServices::setUserInfo failed\n");
				CErrorCodeMapper::GenerateException(sOperationStatus,USR_MGR);
			}
			else
			{
				OffendingParamsList vOffendingParam;
				OffendingParamsStruct stOffendingParam;
				stOffendingParam.Name    = "UserName";
				stOffendingParam.Value   = (*iter)->Name;
				vOffendingParam.push_back(stOffendingParam);
				
				DEBUGL1("setUserInfoList is partial success\n");
				CErrorCodeMapper::GenerateException("STG2_USER_PARTIAL_SUCCESS", vOffendingParam);
			}
		}		
	}
	DEBUGL8("UserServices::setUserInfoList exit\n");
	return STATUS_OK;
}

Status UserServices::ValidateUserInfoList(user__userInfoListType* pUserInfoList) 
{
	DEBUGL8("UserServices::ValidateUserInfoList Enter\n");
	vector<common__userInfoType* >::iterator iter = pUserInfoList->UserInfo.begin();
	vector<common__userInfoType* >::iterator iterEnd = pUserInfoList->UserInfo.end();
		
	for(;iter != iterEnd; ++iter) 
	{	
		if(!(*iter)) continue;		
		
		common__userInfoType* pUserInfo = *iter;
		
		// check for ID
		if(!pUserInfo->UserID || pUserInfo->UserID->empty())
		{
			if(pUserInfoList->UserInfo.size() == 1)
			{
				CErrorCodeMapper::GenerateException("STG2_MISSING_USER_ID");	
			}
			else
			{
				CErrorCodeMapper::GenerateException("STG2_MULTIPLE_MISSING_USER_ID");	
			}			
		}
		
		// check for user name
		if(pUserInfo->Name.empty())
		{
			DEBUGL1("UserName  is empty\n");
			if(pUserInfoList->UserInfo.size() == 1)
			{
				CErrorCodeMapper::GenerateException("STG2_EMPTY_USER_NAME");	
			}
			else
			{
				CErrorCodeMapper::GenerateException("STG2_MULTIPLE_EMPTY_USER_NAME");
			}		
		}
				
		// check for domain name
		if(pUserInfo->Domain && pUserInfo->Domain->empty())
		{
			DEBUGL1("Domain is empty\n");
			CErrorCodeMapper::GenerateException("STG2_INVALID_DOMAIN");	
		}
		
		// Check for empty groups
		if(pUserInfo->GroupList && false == pUserInfo->GroupList->Item.empty() && CCommonFunctions::IsAnyItemEmptyInList(pUserInfo->GroupList->Item))
		{
			DEBUGL1("setUserInfoDelta: One or more of the groups are empty\n");
			CErrorCodeMapper::GenerateException("STG2_EMPTY_GROUPS");
		}
		// Check for empty departments
		if(pUserInfo->Department && pUserInfo->Department->DepartmentCodeList 
			&& false == pUserInfo->Department->DepartmentCodeList->Item.empty()  && CCommonFunctions::IsAnyItemEmptyInList(pUserInfo->Department->DepartmentCodeList->Item))
		{
			DEBUGL1("setUserInfoDelta: One or more of the departments are empty\n");
			CErrorCodeMapper::GenerateException("STG2_EMPTY_DEPT_CODE");
		}
		// Check for empty Roles
		if(pUserInfo->RoleList && false == pUserInfo->RoleList->Item.empty() && CCommonFunctions::IsAnyItemEmptyInList(pUserInfo->RoleList->Item))
		{
			DEBUGL1("setUserInfoDelta: One or more of the roles are empty\n");
			CErrorCodeMapper::GenerateException("STG2_EMPTY_ROLES");
		}			
	}

	DEBUGL8("UserServices::ValidateUserInfoList Exit\n");
	return STATUS_OK;

}



SOAP_FMAC5 int SOAP_FMAC6 __stg2__setUserInfo(struct soap* soap, _user__setUserInfo *user__setUserInfo, _user__setUserInfoResponse *user__setUserInfoResponse)
{
	DEBUGL8("__stg2__setUserInfo entry\n");
	if(!CCommonFunctions::IsOutputManagementEnabled( ))
	{
		stage2ErrorStruct stError;
		CErrorCodeMapper::MapInternalErrCodeToStage2Fault(stError, "STG2_UNSUPPORTED_OPERATION");
		CCommonFunctions::GenerateSoapFault(soap, stError.strDescription, stError.strErrCode, stError.eErrorType);
		return SOAP_FAULT;
	}
	return	__user__setUserInfo(soap,user__setUserInfo,user__setUserInfoResponse);
}
SOAP_FMAC5 int SOAP_FMAC6 __user__setUserInfo(struct soap* soap, _user__setUserInfo *user__setUserInfo, _user__setUserInfoResponse *user__setUserInfoResponse)
{
	DEBUGL8("__user__setUserInfo entry\n");
	CString     operationName     = "setUserInfo";
	
	try
	{		
		 //Check if we have a conflicting operation in progress
		if(CLongRunningOpHelper::IsAnotherConflictingOperationInProgress(soap, operationName))
		{
			// Don't use GenerateException since it will update Running operation status to "false" which we don't want.
			stage2ErrorStruct stError;
			CErrorCodeMapper::MapInternalErrCodeToStage2Fault(stError, "STG2_CONF_OP_INPROGRESS");
			CCommonFunctions::GenerateSoapFault(soap, stError.strDescription, stError.strErrCode, stError.eErrorType);
			return SOAP_FAULT;			
		}
		else
		{
			//Add this operation to the current running op list
			CLongRunningOpHelper::UpdateRunningOpStatus(soap, operationName, true);
		}
		
		BOProxyRef pBOProxy = CCommonFunctions::getBOProxyRef(soap);
		if(!pBOProxy)
		{
			DEBUGL1("__user__setUserInfo Invalid Session\n");
			CErrorCodeMapper::GenerateException("STG2_INVALID_SESSION");
		}
		
		if(!user__setUserInfo || !user__setUserInfo->UserInfoList || user__setUserInfo->UserInfoList->UserInfo.empty())
		{
			CErrorCodeMapper::GenerateException("STG2_MISSING_PARAM");
		}
		
		if(STATUS_OK != UserServices::ValidateUserInfoList(user__setUserInfo->UserInfoList))
		{
			CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");
		}
				
		CUIControllerAdapter::SetXMLValue("", "<UserManager></UserManager>", pBOProxy); // to clear delta document.
			
		user__setUserInfoResponse->Result = common__operationResultType__OperationSucceeded; 
			
		UserServices::setUserInfoList(soap, pBOProxy, user__setUserInfo->UserInfoList);
		 //Remove this operation from the current running op list
		CLongRunningOpHelper::UpdateRunningOpStatus(soap, operationName, false);	
								
	}
	catch (CStg2Exception &ex)
	{
		DEBUGL1("Stage2::__user__setUserInfo caught CStg2Exception Exception.Description %s\n",ex.C_Str());
		CLongRunningOpHelper::UpdateRunningOpStatus(soap, operationName, false);	
		if(CCommonFunctions::GenerateSoapFault(soap,ex.C_Str(),ex.getFaultType(),ex.getErrorType(), ex.getOffendingList()) != STATUS_OK)
		DEBUGL4("Stage2::__user__setUserInfo CCommonFunctions::GenerateSoapFault Failed \n");
		return SOAP_FAULT;
	}
	catch(CException &ex)
	{
		DEBUGL1("Stage2::__user__setUserInfo caught CException Exception.Description %s\n",ex.C_Str());
		CLongRunningOpHelper::UpdateRunningOpStatus(soap, operationName, false);	
		if(CCommonFunctions::GenerateSoapFault(soap,ex.C_Str(),"",eReceiverError)  != STATUS_OK)
		DEBUGL4("Stage2::__user_setUserInfo CCommonFunctions::GenerateSoapFault Failed \n");
		return SOAP_FAULT;
	}
	DEBUGL8("Stage2::__user__setUserInfo exit\n");
	return SOAP_OK;
}
SOAP_FMAC5 int SOAP_FMAC6 __stg2__getJobQuota(struct soap* soap, _user__getJobQuota *user__getJobQuota, _user__getJobQuotaResponse *user__getJobQuotaResponse)
{
		
	DEBUGL8("Stage2::__stg2__getJobQuota entry\n");
	if(!CCommonFunctions::IsOutputManagementEnabled( ))
	{
		stage2ErrorStruct stError;
		CErrorCodeMapper::MapInternalErrCodeToStage2Fault(stError, "STG2_UNSUPPORTED_OPERATION");
		CCommonFunctions::GenerateSoapFault(soap, stError.strDescription, stError.strErrCode, stError.eErrorType);
		return SOAP_FAULT;
	}
	return __user__getJobQuota(soap,user__getJobQuota,user__getJobQuotaResponse);
	
}
SOAP_FMAC5 int SOAP_FMAC6 __user__getJobQuota(struct soap* soap, _user__getJobQuota *user__getJobQuota, _user__getJobQuotaResponse *user__getJobQuotaResponse)
{
	
	DEBUGL8("Stage2::__user__getJobQuota entry\n");
	try
	{
		BOProxyRef pBOProxy = CCommonFunctions::getBOProxyRef(soap);
		if(!pBOProxy)
		{
			DEBUGL1("__user__getJobQuota Invalid Session\n");
			CErrorCodeMapper::GenerateException("STG2_INVALID_SESSION");
		}	
		
		if(!user__getJobQuota || !user__getJobQuota->Owner)
		{
			CErrorCodeMapper::GenerateException("STG2_MISSING_PARAM");
		}
		
		user__ownerDepartmentType* pDeptType = dynamic_cast <user__ownerDepartmentType *> (user__getJobQuota->Owner);
		user__ownerUserType* pUserType = dynamic_cast<user__ownerUserType *> (user__getJobQuota->Owner);
		user__ownerMDSUserType* pMDSType = dynamic_cast<user__ownerMDSUserType *> (user__getJobQuota->Owner);
		
		// return error if
		// 1. ID node is present but is empty.
		// 2. ID node is not present and Login is not user login type.
		if( pDeptType || pUserType )
		{		
			if((user__getJobQuota->Owner->ID && user__getJobQuota->Owner->ID->empty()) || 
				(!user__getJobQuota->Owner->ID && false == CCommonFunctions::IsUserLogin(soap))) 
			{
				DEBUGL1("ID is emtpy\n");
				CErrorCodeMapper::GenerateException("STG2_EMPTY_ID");			
			}
					
			
			if(pDeptType)
			{
				if(!user__getJobQuota->Owner->ID)
				{
					CErrorCodeMapper::GenerateException("STG2_EMPTY_ID");
				}
				
				CString	sDepartmentCode = *(user__getJobQuota->Owner->ID);
				
				if(STATUS_OK != DeviceRecord::ExecuteGetDepartmentInfoCommand(sDepartmentCode,pBOProxy))
				{
					DEBUGL1("DeviceRecord::ExecuteGetDepartmentInfoCommand\n");
					CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");			
				}	
				
				CString sXPath = CString(DEPARTMENT_BILLINGCODE_PATH) + "[code=" + sDepartmentCode + "]/Quota";
				user__getJobQuotaResponse->JobQuota = soap_new_common__jobQuotaCategoryType(soap, -1);
				user__getJobQuotaResponse->JobQuota->Print  = UserServices::MapUserQuotaInfo(sXPath,pBOProxy,soap);
			}
			else if (pUserType)
			{
			
				CString sUserID;
				if(!user__getJobQuota->Owner->ID)
				{
					sUserID = CCommonFunctions::GetLoggedInUserID(soap);	
				}
				else
				{
					sUserID = *(user__getJobQuota->Owner->ID);
				}
					
				CString sOperationStatus = UserServices::ExecuteGetUserCommand(sUserID,pBOProxy);
				if(STG2_SUCCESS != sOperationStatus)
				{
					CErrorCodeMapper::GenerateException(sOperationStatus,USR_MGR);
				}
							
				CString sXPath = CString(USERS_XPATH) +"/User/Information/AccountingProperty/Quota";
				user__getJobQuotaResponse->JobQuota = soap_new_common__jobQuotaCategoryType(soap,-1);
				user__getJobQuotaResponse->JobQuota->Print  = UserServices::MapUserQuotaInfo(sXPath,pBOProxy,soap);
			}
		}
		else if (pMDSType)
		{
			DEBUGL1("Owner Type is not valid\n");
			CErrorCodeMapper::GenerateException("STG2_INVALID_OWNER_TYPE");
		}
		else
		{
			DEBUGL1("Owner Type is not valid\n");
			CErrorCodeMapper::GenerateException("STG2_INVALID_OWNER_TYPE");
		}
	
	}
	catch(CStg2Exception & ex)
	{
		DEBUGL1("Stage2::__user__getJobQuota caught Stg2Exception Exception. Description = %s\n", ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), ex.getFaultType(), ex.getErrorType()) != STATUS_OK)
		DEBUGL4("__user__getJobQuota CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}
	catch(CException & ex)
	{
		DEBUGL1("Stage2::__user__getJobQuota caught CException Exception. Description = %s\n", ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), "", eReceiverError) != STATUS_OK)
		DEBUGL4("__user__getJobQuota CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}


	DEBUGL8("Stage2::__user__getJobQuota exit\n");
	return SOAP_OK;
}
SOAP_FMAC5 int SOAP_FMAC6 __stg2__setJobQuota(struct soap* soap, _user__setJobQuota *user__setJobQuota, _user__setJobQuotaResponse *user__setJobQuotaResponse)
{
	DEBUGL8("Stage2::__stg2__setJobQuota entry\n");
	if(!CCommonFunctions::IsOutputManagementEnabled( ))
	{
		stage2ErrorStruct stError;
		CErrorCodeMapper::MapInternalErrCodeToStage2Fault(stError, "STG2_UNSUPPORTED_OPERATION");
		CCommonFunctions::GenerateSoapFault(soap, stError.strDescription, stError.strErrCode, stError.eErrorType);
		return SOAP_FAULT;
	}
	return __user__setJobQuota(soap,user__setJobQuota,user__setJobQuotaResponse);
}
SOAP_FMAC5 int SOAP_FMAC6 __user__setJobQuota(struct soap* soap, _user__setJobQuota *user__setJobQuota, _user__setJobQuotaResponse *user__setJobQuotaResponse)
{
	
	DEBUGL8("Stage2::__user__setJobQuota entry\n");
	try
	{
		BOProxyRef boProxy = CCommonFunctions::getBOProxyRef(soap);
		if(!boProxy)
		{
			DEBUGL1("__user__setJobQuota Invalid Session\n");
			CErrorCodeMapper::GenerateException("STG2_INVALID_SESSION");			
		}
		
		if(user__setJobQuota->JobQuota->Print->__union_printQuotaCategoryType == SOAP_UNION__common__union_printQuotaCategoryType_PrintJobQuota)
		{
		
			DEBUGL1("__user__setJobQuota Invalid Quota Type SOAP_UNION__common__union_printQuotaCategoryType_PrintJobQuota\n");
			CErrorCodeMapper::GenerateException("STATUS_INVALID_INPUT");	
		}
		
		/*if(user__setJobQuota->JobQuota->Print->__union_printQuotaCategoryType == SOAP_UNION__common__union_printQuotaCategoryType_PrintJobQuota 
			&& (!user__setJobQuota->JobQuota->Print->union_printQuotaCategoryType.PrintJobQuota ))
		{
			DEBUGL1("PrintJob Quota is missing\n");
			CErrorCodeMapper::GenerateException("STG2_MISSING_PARAM");
		}*/
			
		/*if(user__setJobQuota->JobQuota->Print->__union_printQuotaCategoryType == SOAP_UNION__common__union_printQuotaCategoryType_ColorJobQuota 
			&& (!user__setJobQuota->JobQuota->Print->union_printQuotaCategoryType.ColorJobQuota  
				|| !user__setJobQuota->JobQuota->Print->union_printQuotaCategoryType.ColorJobQuota->ColorQuota 
					|| !user__setJobQuota->JobQuota->Print->union_printQuotaCategoryType.ColorJobQuota->BlackQuota))
		{
			DEBUGL1("ColorJob Quota is missing\n");
			CErrorCodeMapper::GenerateException("STG2_MISSING_PARAM");
		}*/
			
		
		
		CString ownerType = "";
		Status execCommandStatus = STATUS_OK;

		user__ownerDepartmentType *deptType = dynamic_cast<user__ownerDepartmentType *>(user__setJobQuota->Owner);


		user__ownerUserType *userType = dynamic_cast<user__ownerUserType *>(user__setJobQuota->Owner);
		
		user__ownerMDSUserType *mdsuserType = dynamic_cast<user__ownerMDSUserType*>(user__setJobQuota->Owner);
		
		CString ID = "",sUserName = "" ;
		CString sDeptName = "",sDeptCode="";
		if(userType || deptType )
		{
			
			if(!user__setJobQuota->Owner->ID || user__setJobQuota->Owner->ID->empty())
			{
				DEBUGL1("ID is empty\n");
				CErrorCodeMapper::GenerateException("STG2_EMPTY_ID");
			}
			
			if(userType)
			{
				ownerType = "User";
				sUserName =  CCommonFunctions::GetUserNameUsingUserID(*(user__setJobQuota->Owner->ID),boProxy);
			}
			else if(deptType)
			{
				ownerType = "Department";	
				CString sOperationStatus = UserServices::ExecuteSearchDepartmentCommand(*(user__setJobQuota->Owner->ID),boProxy);
				if(STG2_SUCCESS != sOperationStatus)
				{
					DEBUGL1("UserServices::ExecuteSearchDepartmentCommand\n");
					CErrorCodeMapper::GenerateException(sOperationStatus, ACC_MGR);
				}
				ID = CUIControllerAdapter::GetTextNodeValue("Accounting/BillingCodes/BillingCode/@ID",boProxy);			
				sDeptName = CUIControllerAdapter::GetTextNodeValue("Accounting/BillingCodes/BillingCode/name",boProxy);			
				sDeptCode = CUIControllerAdapter::GetTextNodeValue("Accounting/BillingCodes/BillingCode/code",boProxy);			
			}
		}
		else if(mdsuserType)
		{
			DEBUGL1("Owner Type is not valid\n");
			CErrorCodeMapper::GenerateException("STG2_INVALID_OWNER_TYPE");
		}
		else
		{
			DEBUGL1("Owner Type is not valid\n");
			CErrorCodeMapper::GenerateException("STG2_INVALID_OWNER_TYPE");
		}

		DEBUGL8("__user_setJobQuota :: ownerType is %s \n",ownerType.c_str());
		if(user__setJobQuota->JobQuota)
		{
			if(user__setJobQuota->JobQuota->Print)
			{
				/*NodeRef pQuotaType = NULL;
				boProxy->GetValue("Controller/Settings/QuotaSetting/Print/Type",pQuotaType);
				if(!pQuotaType)
				{
					DEBUGL1("__user__setJobQuota operation failed\n");
					CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");
				}

				CString quotaType = pQuotaType->getTextContent();
				
				if(quotaType == "BlackOrColorQuota" && user__setJobQuota->JobQuota->Print->__union_printQuotaCategoryType == SOAP_UNION__common__union_printQuotaCategoryType_PrintJobQuota)
				{
					DEBUGL1("__user__setJobQuota operation failed\n");
					CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");
				}
				if(quotaType == "PrintJobQuota" && user__setJobQuota->JobQuota->Print->__union_printQuotaCategoryType == SOAP_UNION__common__union_printQuotaCategoryType_ColorJobQuota)
				{
					DEBUGL1("__user__setJobQuota operation failed\n");
					CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");
				}*/
				//if(user__setJobQuota->JobQuota->Print->__union_printQuotaCategoryType == SOAP_UNION__common__union_printQuotaCategoryType_ColorJobQuota)
					execCommandStatus = UserServices::SetColourQuotaInfo(user__setJobQuota->JobQuota->Print->union_printQuotaCategoryType.ColorJobQuota,boProxy,ownerType);
				//else
				//	execCommandStatus = UserServices::SetPrintQuotaInfo(user__setJobQuota->JobQuota->Print->union_printQuotaCategoryType.PrintJobQuota,boProxy,ownerType);
				
				execCommandStatus = ownerType == "User" ?  UserServices::ExecuteModifyUserQuotaInfo(*(user__setJobQuota->Owner->ID),boProxy) :  UserServices::ExecuteModifyDeptQuotaInfo(ID,sDeptName,sDeptCode,boProxy);
			}
		}
		user__setJobQuotaResponse->Result = common__operationResultType__OperationSucceeded;
		
	}
	catch(CStg2Exception & ex)
	{
		DEBUGL1("Stage2::__user__setJobQuota caught Stg2Exception Exception. Description = %s\n", ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), ex.getFaultType(), ex.getErrorType()) != STATUS_OK)
		DEBUGL4("__user__setJobQuota CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}
	catch(CException & ex)
	{
		DEBUGL1("Stage2::__user__setJobQuota caught CException Exception. Description = %s\n", ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), "", eReceiverError) != STATUS_OK)
		DEBUGL4("__user__setJobQuota CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}
	
	DEBUGL8("Stage2::__user__setJobQuota exit\n");
	return SOAP_OK;
}
SOAP_FMAC5 int SOAP_FMAC6 __stg2__getDepartmentInfo(struct soap* soap, _user__getDepartmentInfo *user__getDepartmentInfo, _user__getDepartmentInfoResponse *user__getDepartmentInfoResponse)
{
	if(!CCommonFunctions::IsOutputManagementEnabled( ))
	{
		stage2ErrorStruct stError;
		CErrorCodeMapper::MapInternalErrCodeToStage2Fault(stError, "STG2_UNSUPPORTED_OPERATION");
		CCommonFunctions::GenerateSoapFault(soap, stError.strDescription, stError.strErrCode, stError.eErrorType);
		return SOAP_FAULT;
	}
	return __user__getDepartmentInfo(soap, user__getDepartmentInfo, user__getDepartmentInfoResponse);
}
SOAP_FMAC5 int SOAP_FMAC6 __stg2__setDepartmentInfo(struct soap* soap, _user__setDepartmentInfo *user__setDepartmentInfo, _user__setDepartmentInfoResponse *user__setDepartmentInfoResponse)
{
	if(!CCommonFunctions::IsOutputManagementEnabled( ))
	{
		stage2ErrorStruct stError;
		CErrorCodeMapper::MapInternalErrCodeToStage2Fault(stError, "STG2_UNSUPPORTED_OPERATION");
		CCommonFunctions::GenerateSoapFault(soap, stError.strDescription, stError.strErrCode, stError.eErrorType);
		return SOAP_FAULT;
	}
	return __user__setDepartmentInfo(soap, user__setDepartmentInfo, user__setDepartmentInfoResponse);
}
#ifdef STAGE2_UNSUPPORTED
SOAP_FMAC5 int SOAP_FMAC6 __user__getAllUserManagementInfo(struct soap* soap, _user__getAllUserManagementInfo *user__getAllUserManagementInfo, _user__getAllUserManagementInfoResponse *user__getAllUserManagementInfoResponse)
{
	DEBUGL8("Stage2::__user__getAllUserManagementInfo enter\n");
	CString errMsg,errType;
	try
	{
		BOProxyRef boProxy = CCommonFunctions::getBOProxyRef(soap);
		if(!boProxy)
			CErrorCodeMapper::GenerateException("STG2_INVALID_SESSION");
		/*
		if(CCommonFunctions::IsAdminUser(boProxy) == false)
			CErrorCodeMapper::GenerateException("STATUS_PERMISSION_CHECK_ERROR");
		*/	
		if(!user__getAllUserManagementInfoResponse)
			CErrorCodeMapper::GenerateException("STG2_MISSING_PARAM");

		user__getAllUserManagementInfoResponse->UserManagement = soap_new_user__userManagementType(soap,-1);
		user__getAllUserManagementInfoResponse->UserManagement->UserList = UserServices::GetAllUser(boProxy,soap);

		if(UserServices::ExecuteGetGroupListCommand(boProxy) != STATUS_OK)
			CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");

		user__getAllUserManagementInfoResponse->UserManagement->GroupList = UserServices::MapGroupListInfo(GROUPS_XPATH,boProxy,soap);

		user__getAllUserManagementInfoResponse->UserManagement->DepartmentList = soap_new_user__departmentListType(soap, -1);
		
		UserServices::getDepartments(soap, user__getAllUserManagementInfoResponse->UserManagement->DepartmentList->Department);
		return SOAP_OK;
	}
	catch(CStg2Exception & ex)
	{
		DEBUGL1("Stage2::__user__getAllUserManagementInfo Department caught Stg2Exception Exception. Description = %s\n", ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), ex.getFaultType(), ex.getErrorType()) != STATUS_OK)
		DEBUGL4("__user__getAllUserManagementInfo Department CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}
	catch(CException & ex)
	{
		DEBUGL1("Stage2::__user__getAllUserManagementInfo Department caught CException Exception. Description = %s\n", ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), "", eReceiverError) != STATUS_OK)
		DEBUGL4("__user__getAllUserManagementInfo Department CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}
	return SOAP_OK;
}

void UserServices::ValidateUserList(vector<user__userType * >User)
{
		std::vector<user__userType * >::iterator userIter = User.begin();		

		for(;userIter < User.end(); userIter++)
		{
			if((*userIter)->UserInfo->Name == "")
			{
				CErrorCodeMapper::GenerateException("STG2_MULTIPLE_EMPTY_USER_NAME");	
			}
			
			// Check for empty Department Code
			if((*userIter)->UserInfo->Department && (*userIter)->UserInfo->Department->DepartmentCodeList)
			{
				vector<string>::iterator iter =   (*userIter)->UserInfo->Department->DepartmentCodeList->Item.begin();
				vector<string>::iterator iterEnd =   (*userIter)->UserInfo->Department->DepartmentCodeList->Item.end();
				for (; iter != iterEnd; ++iter)
				{
					if(iter->empty()) 
					{
						CErrorCodeMapper::GenerateException("STG2_MULTIPLE_EMPTY_DEPT_CODE");
					}
				}		
			}			
			
			// Check for empty Groups
			if((*userIter)->UserInfo->GroupList && false == (*userIter)->UserInfo->GroupList->Item.empty())
			{
				vector<string>::iterator iter =   (*userIter)->UserInfo->GroupList->Item.begin();
				vector<string>::iterator iterEnd =  (*userIter)->UserInfo->GroupList->Item.end();
				for (; iter != iterEnd; ++iter)
				{
					if(iter->empty()) 
					{
						CErrorCodeMapper::GenerateException("STG2_EMPTY_GROUPS");					
					}
				}	
			}
		
			// Check for empty Roles
			if((*userIter)->UserInfo->RoleList && false == (*userIter)->UserInfo->RoleList->Item.empty())
			{
				vector<string>::iterator iter =   (*userIter)->UserInfo->RoleList->Item.begin();
				vector<string>::iterator iterEnd =  (*userIter)->UserInfo->RoleList->Item.end();
				for (; iter != iterEnd; ++iter)
				{
					if(iter->empty()) 
					{
						CErrorCodeMapper::GenerateException("STG2_EMPTY_ROLES");					
					}
				}	
			}				
			
			// Check for domain 
			if((*userIter)->UserInfo->Domain && (*userIter)->UserInfo->Domain->empty())
			{
				CErrorCodeMapper::GenerateException("STG2_MULTIPLE_INVALID_DOMAIN");
			}		
		
			if((*userIter)->FavouritesInfo)
			{
				user__favouritesInfoType* pUserFavInfo = (*userIter)->FavouritesInfo;
				UserServices::ValidateFavoriteInfo(pUserFavInfo, true);
			}			
		}

   return;
}

SOAP_FMAC5 int SOAP_FMAC6 __user__setAllUserManagementInfo(struct soap* soap, _user__setAllUserManagementInfo *user__setAllUserManagementInfo, _user__setAllUserManagementInfoResponse *user__setAllUserManagementInfoResponse)
{
	DEBUGL8("__user__setAllUserManagementInfo enter\n");
	CString errMsg;
	try
	{
		BOProxyRef boProxy = CCommonFunctions::getBOProxyRef(soap);
		if(!boProxy)
		{
			CErrorCodeMapper::GenerateException("STG2_INVALID_SESSION");
		}

		if(!user__setAllUserManagementInfoResponse || !user__setAllUserManagementInfo->UserManagement 
			|| !user__setAllUserManagementInfo->UserManagement->UserList || !user__setAllUserManagementInfo->UserManagement->GroupList 
				|| !user__setAllUserManagementInfo->UserManagement->DepartmentList)
		{
			CErrorCodeMapper::GenerateException("STG2_MISSING_PARAM");
		}
		
		UserServices::ValidateUserList(user__setAllUserManagementInfo->UserManagement->UserList->User);

/*
		std::vector<user__userType * >::iterator userIter = user__setAllUserManagementInfo->UserManagement->UserList->User.begin();		

		for(;userIter < user__setAllUserManagementInfo->UserManagement->UserList->User.end(); userIter++)
		{
			if((*userIter)->UserInfo->Name == "")
			{
				CErrorCodeMapper::GenerateException("STG2_MULTIPLE_EMPTY_USER_NAME");	
			}
			
			// Check for empty Department Code
			if((*userIter)->UserInfo->Department && (*userIter)->UserInfo->Department->DepartmentCodeList)
			{
				vector<string>::iterator iter =   (*userIter)->UserInfo->Department->DepartmentCodeList->Item.begin();
				vector<string>::iterator iterEnd =   (*userIter)->UserInfo->Department->DepartmentCodeList->Item.end();
				for (; iter != iterEnd; ++iter)
				{
					if(iter->empty()) 
					{
						CErrorCodeMapper::GenerateException("STG2_MULTIPLE_EMPTY_DEPT_CODE");
					}
				}		
			}			
			
			// Check for empty Groups
			if((*userIter)->UserInfo->GroupList && false == (*userIter)->UserInfo->GroupList->Item.empty())
			{
				vector<string>::iterator iter =   (*userIter)->UserInfo->GroupList->Item.begin();
				vector<string>::iterator iterEnd =  (*userIter)->UserInfo->GroupList->Item.end();
				for (; iter != iterEnd; ++iter)
				{
					if(iter->empty()) 
					{
						CErrorCodeMapper::GenerateException("STG2_EMPTY_GROUPS");					
					}
				}	
			}
		
			// Check for empty Roles
			if((*userIter)->UserInfo->RoleList && false == (*userIter)->UserInfo->RoleList->Item.empty())
			{
				vector<string>::iterator iter =   (*userIter)->UserInfo->RoleList->Item.begin();
				vector<string>::iterator iterEnd =  (*userIter)->UserInfo->RoleList->Item.end();
				for (; iter != iterEnd; ++iter)
				{
					if(iter->empty()) 
					{
						CErrorCodeMapper::GenerateException("STG2_EMPTY_ROLES");					
					}
				}	
			}				
			
			// Check for domain 
			if((*userIter)->UserInfo->Domain && (*userIter)->UserInfo->Domain->empty())
			{
				CErrorCodeMapper::GenerateException("STG2_MULTIPLE_INVALID_DOMAIN");
			}		
		
			if((*userIter)->FavouritesInfo)
			{
				user__favouritesInfoType* pUserFavInfo = (*userIter)->FavouritesInfo;
				UserServices::ValidateFavoriteInfo(pUserFavInfo, true);
			}			
		}
*/
		
		vector<user__departmentType * > departmentTypes = user__setAllUserManagementInfo->UserManagement->DepartmentList->Department;		

		vector<user__departmentType * >::const_iterator itDeptType = departmentTypes.begin();
		vector<user__departmentType * >::const_iterator itDeptTypeEnd = departmentTypes.end();
	
		for(; itDeptType != itDeptTypeEnd; ++itDeptType)
		{
			user__departmentType *Department = (*itDeptType);
			if(!Department || !Department->DepartmentInfo || Department->DepartmentInfo->Code.empty() || Department->DepartmentInfo->Name.empty())
			{
				CErrorCodeMapper::GenerateException("STG2_MISSING_PARAM");
			}
			if(Department->ConfigurationInfo  && Department->ConfigurationInfo->Quota && Department->ConfigurationInfo->Quota->Job)
			{
				if(!Department->ConfigurationInfo->Quota->Job->Print)
					CErrorCodeMapper::GenerateException("STG2_MISSING_PARAM");
				if(Department->ConfigurationInfo->Quota->Job->Print->__union_printQuotaCategoryType == SOAP_UNION__common__union_printQuotaCategoryType_ColorJobQuota)
				{
					if(!Department->ConfigurationInfo->Quota->Job->Print->union_printQuotaCategoryType.ColorJobQuota || !Department->ConfigurationInfo->Quota->Job->Print->union_printQuotaCategoryType.ColorJobQuota->ColorQuota || !Department->ConfigurationInfo->Quota->Job->Print->union_printQuotaCategoryType.ColorJobQuota->BlackQuota)
						CErrorCodeMapper::GenerateException("STG2_MISSING_PARAM");
				}
			}
		}

		vector<class common__groupType * >::iterator iter= user__setAllUserManagementInfo->UserManagement->GroupList->Group.begin();
		if(!user__setAllUserManagementInfo->UserManagement->GroupList->Group.empty())
		{
			for(;iter < user__setAllUserManagementInfo->UserManagement->GroupList->Group.end();iter++)	
			{
				if(!(*iter)->GroupID || (*iter)->GroupID->empty())
				{
					CErrorCodeMapper::GenerateException("STG2_MULTIPLE_MISSING_GROUP_ID");
				}				
				
				if((*iter)->Name.empty())
				{
					CErrorCodeMapper::GenerateException("STG2_EMPTY_GROUPS");	
				}						
				
				if((*iter)->RoleList && false == (*iter)->RoleList->Item.empty())  
				{
					if(CCommonFunctions::IsAnyItemEmptyInList((*iter)->RoleList->Item))
					{
						CErrorCodeMapper::GenerateException("STG2_EMPTY_ROLES");
					}
				}
			}		
		}

		user__setAllUserManagementInfoResponse->Result = common__operationResultType__OperationFailed;//initialisation

		OffendingParamsStruct offendingParamInfo;
		OffendingParamsList   vOffendingParamsList;

		std::vector<user__userType * >::iterator uIt;
		for(uIt = user__setAllUserManagementInfo->UserManagement->UserList->User.begin() ; uIt != user__setAllUserManagementInfo->UserManagement->UserList->User.end(); uIt++)
		{
			CString sOperationStatus = UserServices::ModifyUserInfo(*uIt,boProxy,"UpdateUserInfo");
			if(STG2_SUCCESS != sOperationStatus )
			{
				if (uIt == user__setAllUserManagementInfo->UserManagement->UserList->User.begin())
				{
					errMsg = "invalid userList data";
					CErrorCodeMapper::GenerateException(sOperationStatus,USR_MGR);		
				}
				else
				{
					offendingParamInfo.Name    = (*uIt)->UserInfo->Name;
					offendingParamInfo.Value = "invalid input data";
					vOffendingParamsList.push_back(offendingParamInfo);
					CErrorCodeMapper::GenerateException("STG2_USER_PARTIAL_SUCCESS",vOffendingParamsList);
				}
			}
		}

		std::vector<class common__groupType * >::iterator gIt;
		for(gIt=user__setAllUserManagementInfo->UserManagement->GroupList->Group.begin();gIt!=user__setAllUserManagementInfo->UserManagement->GroupList->Group.end();gIt++)
		{
			CString sOperationStatus = UserServices::ExecuteCreateOrEditGroupCommand(*gIt,"UpdateGroup",boProxy,soap);
			if(STG2_SUCCESS != sOperationStatus )
			{
				if (gIt == user__setAllUserManagementInfo->UserManagement->GroupList->Group.begin())
				{
					errMsg = "invalid groupList data";
					CErrorCodeMapper::GenerateException(sOperationStatus,GRP_MGR);				
				}
				else
				{
					offendingParamInfo.Name    = (*gIt)->Name;
					offendingParamInfo.Value = "invalid input data";
					vOffendingParamsList.push_back(offendingParamInfo);
					CErrorCodeMapper::GenerateException("STG2_GROUP_PARTIAL_SUCCESS",vOffendingParamsList);
				}
			}
		}
		if(UserServices::setDepartments(soap, user__setAllUserManagementInfo->UserManagement->DepartmentList->Department, "ModifyBillingCode",vOffendingParamsList) != STATUS_OK)
			CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");


		user__setAllUserManagementInfoResponse->Result = common__operationResultType__OperationSucceeded;
		return SOAP_OK;
	}

	catch(CStg2Exception & ex)
	{
		DEBUGL1("Stage2::user__setAllUserManagementInfo caught Stg2Exception Exception. Description = %s\n", ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), ex.getFaultType(), ex.getErrorType()) != STATUS_OK)
		DEBUGL4("user__setAllUserManagementInfo CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}
	catch(CException & ex)
	{
		DEBUGL1("Stage2::user__setAllUserManagementInfo caught CException Exception. Description = %s\n", ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), "", eReceiverError) != STATUS_OK)
		DEBUGL4("user__setAllUserManagementInfo CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}

}
#endif

CString UserServices::ExecuteGetBillingCodes(const Ref<BOProxy>& pBOProxy,const CString& strDeptCode)
{
	DEBUGL8("UserServices::ExecuteGetBillingCodes Enter\n");
	const CString strBillingCodeXPath = "Accounting/BillingCodes";
	vector<CString> xPaths;
	vector<CString> values;
	//xPaths.push_back("Accounting/BillingCodes");
	//values.push_back("");
	if(strDeptCode.empty())
	{
		// Just set an empty Accouting DOM
		values.push_back("");
		xPaths.push_back(strBillingCodeXPath);
	}
	else
	{
		// Set the required dept's dept code as filter 
		values.push_back(strDeptCode);
		xPaths.push_back(CString(DEPARTMENT_BILLINGCODE_INFO_PATH)+"/code");
	}
	CUIControllerAdapter::SetXPathValues(xPaths, values, pBOProxy);
	vector<commandParam> vCommandParam;
	if(false == strDeptCode.empty())
	{
		commandParam cmdParam;
		cmdParam.strParamValue = CString(DEPARTMENT_BILLINGCODE_INFO_PATH);
		cmdParam.strParamName = "billingCodesDetails";
		cmdParam.mapAttributes.insert(make_pair("contentType","XPath"));
		vCommandParam.push_back(cmdParam);
	}
	
	// Execute the command
	CString strResponse;
	if(STATUS_OK != CUIControllerAdapter::SendCommand("GetBillingCodes", DEPARTMENT_COMMANDNODE_XPATH,vCommandParam,strResponse,pBOProxy))
	{
		DEBUGL1("Failed to SendCommand\n");
		CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");
	}
	
	return CUIControllerAdapter::GetOperationStatusString(strResponse);


}

common__stringListType* UserServices::getDepartmentList(soap* soap, const Ref<BOProxy>& pBOProxy)
{
	DEBUGL8("UserServices::getDepartmentList Enter\n");
   	DepartmentFinderRef finder = DepartmentFinder::Create(pBOProxy->getSessionID());
	DepartmentListRef departments;
	Status retStatus = finder->GetAllDepartments(CounterQuotaSearchOptions_None, departments);
	if (retStatus == STATUS_AL_SEC_PERMISSION_CHECK_ERROR) 
	{
		DEBUGL2( "UserServices::getDepartmentList ** Permission check error.\n" );
	  	CErrorCodeMapper::GenerateException("STATUS_PERMISSION_CHECK_ERROR");
	}
	if (retStatus != STATUS_OK)
	{
		DEBUGL2( "UserServices::getDepartmentList ** Operation failed.\n" );
  		CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");
	}
	common__stringListType *pDepartmentList = soap_new_common__stringListType(soap, -1);
	for (DepartmentList::const_iterator it = departments->begin(); it != departments->end(); it++)
	{
		DepartmentRef department = *it;
		CString sDepartmentName = department->GetCode();
		pDepartmentList->Item.push_back(sDepartmentName);
	}
	DEBUGL8("UserServices::getDepartmentList Exit\n");
	return pDepartmentList;
}



SOAP_FMAC5 int SOAP_FMAC6 __user__getDepartmentList(struct soap* soap, _user__getDepartmentList *user__getDepartmentList, _user__getDepartmentListResponse *user__getDepartmentListResponse)
{
	try
	{
		DEBUGL1("__user__getDepartmentList entry\n");
		
		BOProxyRef pBOProxy = CCommonFunctions::getBOProxyRef(soap);
		if(!pBOProxy)
		{
			DEBUGL1("__user__getDepartmentList Invalid Session\n");
			CErrorCodeMapper::GenerateException("STG2_INVALID_SESSION");
		}
		
		if(!user__getDepartmentList)
		{			
			CErrorCodeMapper::GenerateException("STG2_MISSING_PARAM");
		}
				
		user__getDepartmentListResponse->DepartmentList =  UserServices::getDepartmentList(soap, pBOProxy);
	}
	catch(CStg2Exception &ex)
	{
		DEBUGL1("Stage2::__user__getDepartmentList caught Stg2Exception Exception. Description = %s\n", ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), ex.getFaultType(), ex.getErrorType()) != STATUS_OK)

		DEBUGL4("__user__getDepartmentList CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}
	catch(CException &ex)
	{
		DEBUGL1("Stage2::__user__getDepartmentList caught CException exception. Description = %s\n",ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap,ex.C_Str(),"",eReceiverError) != STATUS_OK)
		DEBUGL4("__user__getDepartmentList CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}
	DEBUGL1("__user__getDepartmentList exit\n");
	return SOAP_OK;
	
}

user__basicUserInfoListType* UserServices::getDepartmentUserList(soap* soap, Ref<BOProxy>& pBOProxy, CString& sCode)
{
	DEBUGL8("UserServices::getDepartmentUserList entry\n");
	
	CString sOperationStatus = ValidateDepartmentCode(pBOProxy, sCode);
	if(STG2_SUCCESS != sOperationStatus)
	{
		DEBUGL1("ValidateDepartmentCode operation failed\n");
		CErrorCodeMapper::GenerateException(sOperationStatus,ACC_MGR);
	}
		
	//code below has been commented for because ExecuteSearchUserByDepartmentCode didn't work in plugin side.
	//Hence, to fix the issue ExecuteGetUserListCommand has been used.
	/*sOperationStatus = ExecuteSearchUserByDepartmentCode(sCode, pBOProxy);	
	if(STG2_SUCCESS != sOperationStatus)
	{
		DEBUGL1("ExecuteSearchUserByDepartmentCode operation failed\n");
		CErrorCodeMapper::GenerateException(sOperationStatus,USR_MGR);
	}*/
	if(STATUS_OK != ExecuteGetUserListCommand(pBOProxy))
	{
		DEBUGL1("ExecuteGetUserListCommand Failed \n");
		CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");
	
	}
	
	
	Ref<Node> pUsersNode =  NULL;
	if(STATUS_OK != pBOProxy->GetValue("UserManager/Users",pUsersNode) || !pUsersNode)
	{
		DEBUGL1("GetValue failed\n");
		CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");
	}
	
	
	user__basicUserInfoListType* pUserList =  soap_new_user__basicUserInfoListType(soap, -1);
	
	NodeListRef pUserNodeList = pUsersNode->getChildNodes();
	for(unsigned long i = 0; i < pUserNodeList->getLength(); i++)
	{
		Ref<Node> pUserNode = pUserNodeList->item(i);
		if(!pUserNode) continue;
				
		Ref<Node> pDepartmentsNode = chelper::NavigatePath("Information/Departments", pUserNode);
		Ref<NodeList> pDepartmentList = pDepartmentsNode->getChildNodes();
		
		for(unsigned long j = 0 ; j < pDepartmentList->getLength(); ++j)
		{
			Ref<Node> pDepartmentNode = pDepartmentList->item(j);
			if(!pDepartmentNode) continue;
			
			CString sDepartmentID =  chelper::GetPathValue(pDepartmentNode, "departmentCode");
			
			if(sCode == sDepartmentID)
			{	
				user__basicUserInfoType * User = soap_new_user__basicUserInfoType(soap,-1);
				User->Name = CCommonFunctions::GetValue(pUserNode, "Information/name");
				User->UserID = (ElementRef(pUserNode))->getAttribute("ID");
				CString sDomain = CCommonFunctions::GetValue(pUserNode, "Information/domainName");
				CString sStatus = CCommonFunctions::GetValue(pUserNode, "Information/userStatus");
				CString sExternalUser = CCommonFunctions::GetValue(pUserNode,"Information/isExternalUser");
				if(sDomain.empty() == false  && sDomain != NODE_DOES_NOT_EXIST)
				{
					
					User->Domain = soap_new_std__string(soap,-1);
					*(User->Domain) = sDomain;
				}
				if(sStatus.empty() == false && sStatus != NODE_DOES_NOT_EXIST)
				{
					
					User->Status = soap_new_std__string(soap,-1);
					*(User->Status) = sStatus;
				}
				if(sExternalUser.empty() == false && sExternalUser != NODE_DOES_NOT_EXIST)
				{
					User->ExternalUser = static_cast<bool *>(soap_malloc(soap,sizeof(bool)));
					*(User->ExternalUser) = sExternalUser == "true" ? true : false;
				}
				pUserList->User.push_back(User);
				break;
			}
		}		
				
	}
	
	DEBUGL8("UserServices::getDepartmentUserList exit\n");
	return pUserList;
}


SOAP_FMAC5 int SOAP_FMAC6 __user__getDepartmentUserList(struct soap* soap, _user__getDepartmentUserList *user__getDepartmentUserList, _user__getDepartmentUserListResponse *user__getDepartmentUserListResponse)
{
	 CString        operationName     = "getDepartmentUserList";
	try
	{
		 //Check if we have a conflicting operation in progress
		if(CLongRunningOpHelper::IsAnotherConflictingOperationInProgress(soap, operationName))
		{
			// Don't use GenerateException since it will update Running operation status to "false" which we don't want.
			stage2ErrorStruct stError;
			CErrorCodeMapper::MapInternalErrCodeToStage2Fault(stError, "STG2_CONF_OP_INPROGRESS");
			CCommonFunctions::GenerateSoapFault(soap, stError.strDescription, stError.strErrCode, stError.eErrorType);
			return SOAP_FAULT;	
		}
		else
		{
			//Add this operation to the current running op list
			CLongRunningOpHelper::UpdateRunningOpStatus(soap, operationName, true);
		}
		
		DEBUGL8("__user__getDepartmentUserList entry\n");
		BOProxyRef pBOProxy = CCommonFunctions::getBOProxyRef(soap);
		if(!pBOProxy)
		{
			DEBUGL1("__user__getDepartmentUserList Invalid Session\n");
			CErrorCodeMapper::GenerateException("STG2_INVALID_SESSION");
		}
				
		
		if(!user__getDepartmentUserList)
		{
			CErrorCodeMapper::GenerateException("STG2_MISSING_PARAM");
		}
		
		CString sDepartmentCode = user__getDepartmentUserList->Code;
		
		if(sDepartmentCode.empty())
		{
			CErrorCodeMapper::GenerateException("STG2_EMPTY_DEPT_CODE");
		}
			
		user__getDepartmentUserListResponse->UserList = UserServices::getDepartmentUserList(soap, pBOProxy, sDepartmentCode);
		//Remove this operation from the current running op list
		CLongRunningOpHelper::UpdateRunningOpStatus(soap, operationName, false);
						
	}
	catch(CStg2Exception &ex)
	{
		DEBUGL1("Stage2::__user__getDepartmentUserList caught Stg2Exception Exception. Description = %s\n", ex.C_Str());
		CLongRunningOpHelper::UpdateRunningOpStatus(soap, operationName, false);
		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), ex.getFaultType(), ex.getErrorType()) != STATUS_OK)
			DEBUGL4("__user__getDepartmentUserList CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}
	catch(CException &ex)
	{
		DEBUGL1("Stage2::__user__getDepartmentUserList caught CException exception. Description = %s\n",ex.C_Str());
		CLongRunningOpHelper::UpdateRunningOpStatus(soap, operationName, false);
		if(CCommonFunctions::GenerateSoapFault(soap,ex.C_Str(),"",eReceiverError) != STATUS_OK)
			DEBUGL4("__user__getDepartmentUserList CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}
	DEBUGL8("Stage2::__user__getDepartmentUserList exit\n");
	return SOAP_OK;
}
#ifdef STAGE2_UNSUPPORTED
SOAP_FMAC5 int SOAP_FMAC6 __user__getAllUser(struct soap* soap, _user__getAllUser *user__getAllUser, _user__getAllUserResponse *user__getAllUserResponse)
{
	DEBUGL8("Stage2::__user__getAllUser entry\n");
	try
	{
		BOProxyRef pBOProxy = CCommonFunctions::getBOProxyRef(soap);
		if(!pBOProxy)
		{
			CErrorCodeMapper::GenerateException("STG2_INVALID_SESSION");		
		}

		if(!user__getAllUser || !user__getAllUserResponse)
		{
			CErrorCodeMapper::GenerateException("STG2_MISSING_PARAM");
		}

		user__getAllUserResponse->UserList = UserServices::GetAllUser(pBOProxy,soap);
	}
	catch(CStg2Exception & ex)
	{
		DEBUGL1("Stage2::__user__getAllUser caught Stg2Exception Exception. Description = %s\n", ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), ex.getFaultType(), ex.getErrorType()) != STATUS_OK)
		DEBUGL4("__user__getAllUser CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}
	catch(CException & ex)
	{
		DEBUGL1("Stage2::__user__getAllUser caught CException Exception. Description = %s\n", ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), "", eReceiverError) != STATUS_OK)
		DEBUGL4("__user__getAllUser CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}
	DEBUGL8("Stage2::__user__getAllUser exit\n");

	return SOAP_OK;
}
void UserServices::getExistingAndDeletableUsers(BOProxyRef pBOProxy,vector<CString>& vExistingUserIds,map<CString,CString> &deletableUserIds)
{
	DEBUGL4("Stage2::getExistingAndDeletableUsers enter\n");
	if(STATUS_OK != UserServices::ExecuteGetUserListCommand(pBOProxy,false))
	{
		DEBUGL1("getExistingAndDeletableUsers::getUserList ExecuteGetUserListCommand failed\n");
		CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");	
	}
	
	NodeRef pNode = NULL;
	if(STATUS_OK != pBOProxy->GetValue(USERS_XPATH, pNode) || !pNode)
	{
		DEBUGL1("Failed to GetValue\n");
		CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");
	}

	NodeListRef pChildren = pNode->getChildNodes();	
	
	for ( ulong i=0; i < pChildren->getLength(); i++)
	{
		CString userXpath = CString(USERS_XPATH) + "/User["+ string_cast(i+1) +"]/@ID";
		NodeRef pID = NULL;
		if (STATUS_OK != pBOProxy->GetValue(userXpath.c_str(),pID))
		{
			DEBUGL1("UserServices::getExistingAndDeletableUsers Failed in GetValue!!!\n");
			CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");
		}
		CString userID = pID->getTextContent();
		DEBUGL8("getExistingAndDeletableUsers :: Fetching User info from xpath %s userID - %s\n",userXpath.c_str(),userID.c_str());
		vExistingUserIds.push_back(userID);
		userXpath = CString(USERS_XPATH) + "/User["+string_cast(i+1)+"]/Information/isFactoryDefaultUser";
		if( STATUS_OK != pBOProxy->GetValue(userXpath.c_str(),pID))
		{
			DEBUGL1("UserServices::getExistingAndDeletableUsers Failed in GetValue!!!\n");
			CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");
		}
		if(userID != "10001" && pID->getTextContent() != "YES" )
		{
			userXpath = CString(USERS_XPATH) + "/User["+string_cast(i+1)+"]/Information/name";
			if(	STATUS_OK != pBOProxy->GetValue(userXpath.c_str(),pID))
			{
				DEBUGL1("UserServices::getExistingAndDeletableUsers Failed in GetValue!!!\n");
				CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");				
			}
			deletableUserIds.insert(pair<CString,CString>(userID,pID->getTextContent()));
			DEBUGL8("getExistingAndDeletableUsers :: Pushing deletable  User info user name  %s userID - %s\n",pID->getTextContent().c_str(),userID.c_str());
		}
	}
	
	DEBUGL8("Stage2::getExistingAndDeletableUsers size of vExistingUserIds  = %d \n",int(vExistingUserIds.size()));
	DEBUGL4("Stage2::getExistingAndDeletableUsers exit\n");
	return;
}
void UserServices::SortOutUserList(vector<user__userType * > User,std::vector<user__userType * > &usersToCreate,std::vector<user__userType * > &usersToEdit,map<CString,CString> &deletableUserIds,vector<CString> &vExistingUserIds)
{
	DEBUGL4("UserServices::SortOutUserList enter\n");
	std::vector<user__userType * > usersListSupplied; // vector of users supplied by client
	std::vector<user__userType * >::iterator userIter = User.begin();
	//remove invalid entries such as duplicated user name and user IDs.		
	for(;userIter < User.end(); userIter++)
	{
		if((*userIter)->UserInfo->UserID) // check if user id is not supplied we dont need to verify if its duplicated
		{
			if(*((*userIter)->UserInfo->UserID) != "") // if user id is empty we dont need to verify if its duplicated
			{
				map<CString,CString>::iterator itFoundDeletableID = deletableUserIds.find(*((*userIter)->UserInfo->UserID));
				if(deletableUserIds.end() != itFoundDeletableID)
				{
					//remove the id's which will be edited from the deletable id's vector so that it retains only the deltable ones
					deletableUserIds.erase(itFoundDeletableID);
					DEBUGL8("Stage2::__user__setAllUser the user id that will be NOT be DELETED :: %s size of vDeletableUserIds : %d \n",itFoundDeletableID->second.c_str(),int(deletableUserIds.size()));
					
				}
			}
 		}
		usersListSupplied.push_back(*userIter);
	}
	
	userIter = usersListSupplied.begin();
	for(;userIter < usersListSupplied.end(); userIter++)
	{
		if((*userIter)->UserInfo->UserID) // check if user id is not supplied we need to create it
		{
			if(*((*userIter)->UserInfo->UserID) != "") // if user id is empty we need to create it
			{
				vector<CString>::iterator itBeginExistingID = vExistingUserIds.begin();
				vector<CString>::iterator itEndExistingID = vExistingUserIds.end();
				if(itEndExistingID == find(itBeginExistingID,itEndExistingID,*((*userIter)->UserInfo->UserID)))
				{
					DEBUGL8("Stage2::__user__setAllUser the user id that will be CREATED :: %s \n",(*((*userIter)->UserInfo->UserID)).c_str());
					usersToCreate.push_back(*userIter); // if user id is not found in the existing id list it needs to be created
				}
				else
				{
					DEBUGL8("Stage2::__user__setAllUser the user id that will be EDITED :: %s \n",(*((*userIter)->UserInfo->UserID)).c_str());
					usersToEdit.push_back(*userIter); // since user id mentioned exists it will be edited

				}
			}
			else 
				usersToCreate.push_back(*userIter);
		}
		else 
			usersToCreate.push_back(*userIter);
	}

	DEBUGL4("UserServices::SortOutUserList exit\n");
	return;
}
void UserServices::ValidateUsersDomainInfo(vector<user__userType * > usersToCreate)
{
	DEBUGL4("UserServices::ValidateUsersDomainInfo enter\n");
	std::vector<user__userType * >::iterator userIter = usersToCreate.begin();
	for(;userIter < usersToCreate.end(); userIter++)
	{
		// Check for domain 
		if((*userIter)->UserInfo->Domain && ((*userIter)->UserInfo->Domain->empty() || ((*userIter)->UserInfo->ExternalUser && false == *((*userIter)->UserInfo->ExternalUser))))
		{
			CErrorCodeMapper::GenerateException("STG2_MULTIPLE_INVALID_DOMAIN");
		}		
	}		
	DEBUGL4("UserServices::ValidateUsersDomainInfo exit\n");
	return;
}
void UserServices::UpdateExistingUsers(vector<user__userType * >& usersToEdit,BOProxyRef& pBOProxy,OffendingParamsList &vOffendingParamsList,bool &createUpdateOrDelete )
{
	DEBUGL4("UserServices::UpdateExistingUsers enter\n");
	OffendingParamsStruct offendingParamInfo;
	std::vector<user__userType * >::iterator userIter = usersToEdit.begin();
	for(;userIter < usersToEdit.end(); userIter++)
	{
		CString sOperationStatus = UserServices::ModifyUserInfo(*userIter,pBOProxy,"UpdateUserInfo");
		if(STG2_SUCCESS != sOperationStatus)
		{
			DEBUGL1("modifications of following user failed %s \n ",((*userIter)->UserInfo->Name).c_str());
			if(createUpdateOrDelete == false)
				CErrorCodeMapper::GenerateException(sOperationStatus,USR_MGR);		
			else		
			{
				offendingParamInfo.Name    = (*userIter)->UserInfo->Name;
				offendingParamInfo.Value = "invalid input data";
				vOffendingParamsList.push_back(offendingParamInfo);
				CErrorCodeMapper::GenerateException("STG2_USER_PARTIAL_SUCCESS",vOffendingParamsList);
			}		
		}
		createUpdateOrDelete = true;
	}	
	DEBUGL4("UserServices::UpdateExistingUsers exit\n");
	return;


}
void UserServices::CreateNonExistingUsers(vector<user__userType * >& usersToCreate,BOProxyRef& pBOProxy,OffendingParamsList &vOffendingParamsList,bool &createUpdateOrDelete )
{
	DEBUGL4("UserServices::CreateNonExistingUsers enter\n");
	OffendingParamsStruct offendingParamInfo;
	std::vector<user__userType * >::iterator userIter = usersToCreate.begin();
	for(;userIter < usersToCreate.end(); userIter++)
	{
		CString sOperationStatus = UserServices::ModifyUserInfo(*userIter,pBOProxy,"AddUser");
		if(STG2_SUCCESS != sOperationStatus)
		{
			DEBUGL1("Creation of following user failed  %s\n",((*userIter)->UserInfo->Name).c_str());
			if(createUpdateOrDelete == false)
				CErrorCodeMapper::GenerateException(sOperationStatus,USR_MGR);		
			else		
			{
				offendingParamInfo.Name    = (*userIter)->UserInfo->Name;
				offendingParamInfo.Value = "invalid input data";
				vOffendingParamsList.push_back(offendingParamInfo);
				CErrorCodeMapper::GenerateException("STG2_USER_PARTIAL_SUCCESS",vOffendingParamsList);
			}	
		}
		createUpdateOrDelete = true;
	}
	DEBUGL4("UserServices::CreateNonExistingUsers exit\n");
	return;
}
void UserServices::DeleteUnsuppliedUsers(map<CString,CString> &deletableUserIds,BOProxyRef& pBOProxy,OffendingParamsList & vOffendingParamsList,bool& createUpdateOrDelete)
{
	DEBUGL4("UserServices::DeleteUnsuppliedUsers enter");
	OffendingParamsStruct offendingParamInfo;
	map<CString,CString>::iterator itr = deletableUserIds.begin();
	for( ; itr != deletableUserIds.end();itr++)
	{
		CString sOperationStatus = UserServices::ExecuteDeleteUserCommand(itr->first,pBOProxy);
		if (STG2_SUCCESS !=  sOperationStatus) 
		{
			DEBUGL1("Deletion  of following user failed \n %s",(itr->second).c_str());
			if(createUpdateOrDelete == false)
				CErrorCodeMapper::GenerateException(sOperationStatus, USR_MGR);
			else
			{
				offendingParamInfo.Name    = "failed to delete";
				offendingParamInfo.Value = itr->second;
				vOffendingParamsList.push_back(offendingParamInfo);
				CErrorCodeMapper::GenerateException("STG2_USER_PARTIAL_SUCCESS",vOffendingParamsList);
			}
		}	
		createUpdateOrDelete = true;
	}
	DEBUGL4("UserServices::DeleteUnsuppliedUsers exit");
	return;

}
SOAP_FMAC5 int SOAP_FMAC6 __user__setAllUser(struct soap* soap, _user__setAllUser *user__setAllUser, _user__setAllUserResponse *user__setAllUserResponse)
{
	DEBUGL8("Stage2::__user__setAllUser entry\n");
	OffendingParamsList   vOffendingParamsList;
	try
	{
		BOProxyRef pBOProxy = CCommonFunctions::getBOProxyRef(soap);
		if(!pBOProxy)
		{
			CErrorCodeMapper::GenerateException("STG2_INVALID_SESSION");
		}
	
		if(!user__setAllUser || !user__setAllUserResponse || !user__setAllUser->UserList)
		{
			CErrorCodeMapper::GenerateException("STG2_MISSING_PARAM");			
		}		
		UserServices::ValidateUserList(user__setAllUser->UserList->User);//validate input user list for invalid user names,empty roles,empty groups,empty departments and invalid domain names

		vector<CString> vExistingUserIds;
		map<CString,CString> deletableUserIds;		

		UserServices::getExistingAndDeletableUsers(pBOProxy,vExistingUserIds,deletableUserIds);//get the list of existing users and the users created by admin

		std::vector<user__userType * > usersToCreate;//vector of users that needs to be created
		std::vector<user__userType * > usersToEdit;//vector of users that needs to be modified
		UserServices::SortOutUserList(user__setAllUser->UserList->User,usersToCreate,usersToEdit,deletableUserIds,vExistingUserIds);//sort out the input user list to the ones that needs to be created,edtied and also the delted users

		if(usersToCreate.empty() == false) 
			UserServices::ValidateUsersDomainInfo(usersToCreate);//Valdate the domain info of the users to be created

		bool createUpdateOrDelete = false;
		if(usersToEdit.empty() == false) // modify the existing users
			UserServices::UpdateExistingUsers(usersToEdit,pBOProxy,vOffendingParamsList,createUpdateOrDelete);

		if(usersToCreate.empty() == false) // create the un-existing USERs
			UserServices::CreateNonExistingUsers(usersToCreate,pBOProxy,vOffendingParamsList,createUpdateOrDelete);

		if(deletableUserIds.empty() == false) //delete the unsupplied USERs
			UserServices::DeleteUnsuppliedUsers(deletableUserIds,pBOProxy,vOffendingParamsList,createUpdateOrDelete);

		user__setAllUserResponse->Result = common__operationResultType__OperationSucceeded ;
	}
	catch(CStg2Exception & ex)
	{
		DEBUGL1("Stage2::__user__setAllUser caught Stg2Exception Exception. Description = %s\n", ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), ex.getFaultType(), ex.getErrorType(),vOffendingParamsList) != STATUS_OK)
			DEBUGL4("__user__setAllUser CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}
	catch(CException & ex)
	{
		DEBUGL1("Stage2::__user__setAllUser caught CException Exception. Description = %s\n", ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), "", eReceiverError,vOffendingParamsList) != STATUS_OK)
			DEBUGL4("__user__setAllUser CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}
	DEBUGL8("Stage2::__user__setAllUser exit\n");
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __user__getUser(struct soap* soap, _user__getUser *user__getUser, _user__getUserResponse *user__getUserResponse)
{
	DEBUGL8("Stage2::__user__getUser entry\n");
	try 
	{
		BOProxyRef pBOProxy = CCommonFunctions::getBOProxyRef(soap);
		if(!pBOProxy)
		{
			CErrorCodeMapper::GenerateException("STG2_INVALID_SESSION");
		}
		
		if(!user__getUser || !user__getUserResponse)
		{
			CErrorCodeMapper::GenerateException("STG2_MISSING_PARAM");
		}		
		
		if(user__getUser->Name.empty())
		{
			CErrorCodeMapper::GenerateException("STG2_EMPTY_USER_NAME");
		}

		CString sOperationStatus = UserServices::ExecuteGetUserCommand(user__getUser->Name,pBOProxy, false);
		if(STG2_SUCCESS != sOperationStatus)
		{
			CErrorCodeMapper::GenerateException(sOperationStatus,USR_MGR);
		}
	
		CString ID = CUIControllerAdapter::GetTextNodeValue("UserManager/Users/User/@ID",pBOProxy);
		if(ID == "")
		{
			// ID is emtpy that indicates User does not exist.
			// Hence, We are sending plugin's error code.					
			CErrorCodeMapper::GenerateException("STATUS_USER_NOT_FOUND", USR_MGR) ;
		}	

		user__getUserResponse->User = UserServices::GetUser("UserManager/Users/User",pBOProxy,soap);
	}
	catch(CStg2Exception & ex)
	{
		DEBUGL1("Stage2::__user__getUser caught Stg2Exception Exception. Description = %s\n", ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), ex.getFaultType(), ex.getErrorType()) != STATUS_OK)
		DEBUGL4("__user__getUser CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}
	catch(CException & ex)
	{
		DEBUGL1("Stage2::__user__getUser caught CException Exception. Description = %s\n", ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), "", eReceiverError) != STATUS_OK)
		DEBUGL4("__user__getUser CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}


	DEBUGL8("Stage2::__user__getUser exit\n");

	return SOAP_OK;
}

void UserServices::ValidateFavoriteInfo(user__favouritesInfoType* pUserFavInfo, bool isSetAll)
{
	DEBUGL4("UserServices::ValidateFavoriteInfo Enter\n");
	if(pUserFavInfo->URLList)
	{
		std::vector<user__favouriteURLType * >::iterator iter = pUserFavInfo->URLList->URL.begin();
		std::vector<user__favouriteURLType * >::iterator iterEnd = pUserFavInfo->URLList->URL.end();
		for(; iter!= iterEnd; ++iter)
		{
			if(!(*iter))
			{
				CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");
			}		
			
			user__favouriteURLType* pURL = *iter;
			
			if(pURL->FavouriteName.empty())
			{
				if(pUserFavInfo->URLList->URL.size() == 1 && isSetAll == false)
				{
					CErrorCodeMapper::GenerateException("STG2_EMPTY_FAVOURITE_NAME");	
				}
				else
				{
					CErrorCodeMapper::GenerateException("STG2_MULTIPLE_EMPTY_FAVOURITE_NAME");	
				}			
			}		
		}
	}
	
	if(pUserFavInfo->BoxList)
	{
		// TBD Not Supported in Plugin
	}
	
	if(pUserFavInfo->MailBoxList)
	{
		// TBD Not Supported in Plugin
	}
	
	if(pUserFavInfo->Template)
	{
		if(pUserFavInfo->Template->TemplateList)
		{
			std::vector<user__favouriteTemplateType * >::iterator iter = pUserFavInfo->Template->TemplateList->Template.begin();
			std::vector<user__favouriteTemplateType * >::iterator iterEnd = pUserFavInfo->Template->TemplateList->Template.end();
			for(; iter!= iterEnd; ++iter)
			{
				if(!(*iter))
				{
					CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");
				}		
				
				user__favouriteTemplateType* pTemplate = *iter;

				if(pTemplate->FavouriteName.empty())
				{
					if(pUserFavInfo->Template->TemplateList->Template.size() == 1 && isSetAll == false)
					{
						CErrorCodeMapper::GenerateException("STG2_EMPTY_FAVOURITE_NAME");	
					}
					else
					{
						CErrorCodeMapper::GenerateException("STG2_MULTIPLE_EMPTY_FAVOURITE_NAME");	
					}			
				}		
				
				if(common__templateCategoryNameType__Standard  != pTemplate->TemplateCategory && common__templateCategoryNameType__Extension != pTemplate->TemplateCategory)
				{		
					if(pUserFavInfo->Template->TemplateList->Template.size() == 1 && isSetAll == false)
						CErrorCodeMapper::GenerateException("STG2_INVALID_TEMPLATE_CATEGORY");	
					else	
						CErrorCodeMapper::GenerateException("STG2_MULTIPLE_INVALID_TEMPLATE_CATEGORY");	
				}
				
				if(!pTemplate->GroupNumber)
				{		
					if(pUserFavInfo->Template->TemplateList->Template.size() == 1 && isSetAll == false)
						CErrorCodeMapper::GenerateException("STG2_MISSING_GROUP_NUMBER");	
					else	
						CErrorCodeMapper::GenerateException("STG2_MULTIPLE_MISSING_GROUP_NUMBER");	
				}
				
				if(common__templateCategoryNameType__Extension == pTemplate->TemplateCategory)
				{		
					if(!pTemplate->ExtensionName || pTemplate->ExtensionName->empty())
					{
						if(pUserFavInfo->Template->TemplateList->Template.size() == 1 && isSetAll == false)
							CErrorCodeMapper::GenerateException("STG2_MISSING_EXTENSION_NAME");	
						else
							CErrorCodeMapper::GenerateException("STG2_MULTIPLE_MISSING_EXTENSION_NAME");	
					}				
				}				
			}
		}
		if(pUserFavInfo->Template->TemplateGroupList)
		{
			std::vector<user__favouriteTemplateGroupType * >::iterator iter = pUserFavInfo->Template->TemplateGroupList->TemplateGroup.begin();
			std::vector<user__favouriteTemplateGroupType * >::iterator iterEnd = pUserFavInfo->Template->TemplateGroupList->TemplateGroup.end();
			for(; iter!= iterEnd; ++iter)
			{
				if(!(*iter))
				{
					CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");
				}		
				
				user__favouriteTemplateGroupType* pGroup = *iter;
				
				if(pGroup->FavouriteName.empty())
				{
					if(pUserFavInfo->Template->TemplateGroupList->TemplateGroup.size() == 1 && isSetAll == false)
					{
						CErrorCodeMapper::GenerateException("STG2_EMPTY_FAVOURITE_NAME");	
					}
					else
					{
						CErrorCodeMapper::GenerateException("STG2_MULTIPLE_EMPTY_FAVOURITE_NAME");	
					}			
				}					
				
				if(common__templateCategoryNameType__Standard  != pGroup->TemplateCategory && common__templateCategoryNameType__Extension != pGroup->TemplateCategory)
				{		
					if(pUserFavInfo->Template->TemplateGroupList->TemplateGroup.size() == 1 && isSetAll == false)
						CErrorCodeMapper::GenerateException("STG2_INVALID_TEMPLATE_CATEGORY");	
					else
						CErrorCodeMapper::GenerateException("STG2_MULTIPLE_INVALID_TEMPLATE_CATEGORY");	
				}

				if(common__templateCategoryNameType__Extension == pGroup->TemplateCategory)
				{		
					if(!pGroup->ExtensionName || pGroup->ExtensionName->empty())
					{
						if(pUserFavInfo->Template->TemplateGroupList->TemplateGroup.size() == 1 && isSetAll == false)
							CErrorCodeMapper::GenerateException("STG2_MISSING_EXTENSION_NAME");	
						else	
							CErrorCodeMapper::GenerateException("STG2_MULTIPLE_MISSING_EXTENSION_NAME");	
					}				
				}				
			}			
		}
	}
	
	DEBUGL4("UserServices::ValidateFavoriteInfo Exit\n");
}

SOAP_FMAC5 int SOAP_FMAC6 __user__setUser(struct soap* soap, _user__setUser *user__setUser, _user__setUserResponse *user__setUserResponse)
{
	DEBUGL8("Stage2::__user__setUser entry\n");
	try
	{
		BOProxyRef pBOProxy = CCommonFunctions::getBOProxyRef(soap);
		if(!pBOProxy)
		{
			CErrorCodeMapper::GenerateException("STG2_INVALID_SESSION");	
		}

		if(!user__setUser || !user__setUserResponse || !user__setUser->User || !user__setUser->User->UserInfo)
		{
			CErrorCodeMapper::GenerateException("STG2_MISSING_PARAM");
		}
		
		if(!user__setUser->User->UserInfo->UserID || user__setUser->User->UserInfo->UserID->empty())
		{
			CErrorCodeMapper::GenerateException("STG2_MISSING_USER_ID");	
		}
		
		if(user__setUser->User->UserInfo->Name.empty())
		{
			CErrorCodeMapper::GenerateException("STG2_EMPTY_USER_NAME");	
		}		
		
		// Check for empty Department Code
		if(user__setUser->User->UserInfo->Department && user__setUser->User->UserInfo->Department->DepartmentCodeList)
		{
			vector<string>::iterator iter =   user__setUser->User->UserInfo->Department->DepartmentCodeList->Item.begin();
			vector<string>::iterator iterEnd =   user__setUser->User->UserInfo->Department->DepartmentCodeList->Item.end();
			for (; iter != iterEnd; ++iter)
			{
				if(iter->empty()) 
				{
					if(iter == user__setUser->User->UserInfo->Department->DepartmentCodeList->Item.begin())
						CErrorCodeMapper::GenerateException("STG2_EMPTY_DEPT_CODE");
					else	
						CErrorCodeMapper::GenerateException("STG2_MULTIPLE_EMPTY_DEPT_CODE");
				}
			}		
		}

		// Check for empty Groups
		if(user__setUser->User->UserInfo->GroupList && false == user__setUser->User->UserInfo->GroupList->Item.empty())
		{
			vector<string>::iterator iter =   user__setUser->User->UserInfo->GroupList->Item.begin();
			vector<string>::iterator iterEnd =  user__setUser->User->UserInfo->GroupList->Item.end();
			for (; iter != iterEnd; ++iter)
			{
				if(iter->empty()) 
				{
					if(iter == user__setUser->User->UserInfo->GroupList->Item.begin())
						CErrorCodeMapper::GenerateException("STG2_EMPTY_GROUP_NAME");
					else	
						CErrorCodeMapper::GenerateException("STG2_EMPTY_GROUPS");					
				}
			}	
		}
		
		// Check for empty Roles
		if(user__setUser->User->UserInfo->RoleList && false == user__setUser->User->UserInfo->RoleList->Item.empty())
		{
			vector<string>::iterator iter =   user__setUser->User->UserInfo->RoleList->Item.begin();
			vector<string>::iterator iterEnd =  user__setUser->User->UserInfo->RoleList->Item.end();
			for (; iter != iterEnd; ++iter)
			{
				if(iter->empty()) 
				{
					if(iter == user__setUser->User->UserInfo->RoleList->Item.begin())
						CErrorCodeMapper::GenerateException("STG2_EMPTY_ROLE_NAME");
					else	
						CErrorCodeMapper::GenerateException("STG2_EMPTY_ROLES");					
				}
			}	
		}		
		
		// Check for domain 
		if(user__setUser->User->UserInfo->Domain && user__setUser->User->UserInfo->Domain->empty())
		{
			CErrorCodeMapper::GenerateException("STG2_INVALID_DOMAIN");
		}		
		
		if(user__setUser->User->FavouritesInfo)
		{
			user__favouritesInfoType* pUserFavInfo = user__setUser->User->FavouritesInfo;
			
			UserServices::ValidateFavoriteInfo(pUserFavInfo, false);
		}
		
		if(user__setUser->User->OwnershipInfo)
		{		
			// TBD Not Supported in Plugin
		}

		CString sOperationStatus = UserServices::ModifyUserInfo(user__setUser->User,pBOProxy,"UpdateUserInfo");
		if(STG2_SUCCESS != sOperationStatus)
		{
			CErrorCodeMapper::GenerateException(sOperationStatus,USR_MGR);		
		}
		
		user__setUserResponse->Result = common__operationResultType__OperationSucceeded;
	}
	catch(CStg2Exception & ex)
	{
		DEBUGL1("Stage2::__user__setUser caught Stg2Exception Exception. Description = %s\n", ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), ex.getFaultType(), ex.getErrorType()) != STATUS_OK)
		DEBUGL4("__user__setUser CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}
	catch(CException & ex)
	{
		DEBUGL1("Stage2::__user__setUser caught CException Exception. Description = %s\n", ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), "", eReceiverError) != STATUS_OK)
		DEBUGL4("__user__setUser CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}
	DEBUGL8("Stage2::__user__setUser exit\n");

	return SOAP_OK;
}
#endif
SOAP_FMAC5 int SOAP_FMAC6 __stg2__createUser(struct soap* soap, _user__createUser *user__createUser, _user__createUserResponse *user__createUserResponse)
{
	DEBUGL8("Stage2::__stg2__createUser entry\n");
	if(!CCommonFunctions::IsOutputManagementEnabled( ))
	{
		stage2ErrorStruct stError;
		CErrorCodeMapper::MapInternalErrCodeToStage2Fault(stError, "STG2_UNSUPPORTED_OPERATION");
		CCommonFunctions::GenerateSoapFault(soap, stError.strDescription, stError.strErrCode, stError.eErrorType);
		return SOAP_FAULT;
	}
	return __user__createUser(soap,user__createUser,user__createUserResponse);

}
SOAP_FMAC5 int SOAP_FMAC6 __user__createUser(struct soap* soap, _user__createUser *user__createUser, _user__createUserResponse *user__createUserResponse)
{
	try
	{
		DEBUGL8("__user__createUser entry\n");
		
		BOProxyRef pBOProxy = CCommonFunctions::getBOProxyRef(soap);
		if(!pBOProxy)
		{
			DEBUGL1("__user__createUser Invalid Session\n");
			CErrorCodeMapper::GenerateException("STG2_INVALID_SESSION");
		}

		if(!user__createUser || !user__createUser->UserInfo) 	
		{
			CErrorCodeMapper::GenerateException("STG2_MISSING_PARAM");
		}
		
		if(user__createUser->UserInfo->Name.empty())
		{
			DEBUGL1("__user__createUser user name is empty\n");
			CErrorCodeMapper::GenerateException("STG2_EMPTY_USER_NAME");
		}
		//Check for PIN data
		if(user__createUser->UserInfo->PIN)
		{
			CString sPIN = *(user__createUser->UserInfo->PIN);
			if(sPIN.empty())
			{
				DEBUGL1("__user__createUser pin is empty\n");
				CErrorCodeMapper::GenerateException("STG2_EMPTY_PIN");
			}
		}	
		// Check for empty Department Code
		if(user__createUser->UserInfo->Department && user__createUser->UserInfo->Department->DepartmentCodeList 
			&&  false == user__createUser->UserInfo->Department->DepartmentCodeList->Item.empty() 
				&& (CCommonFunctions::IsAnyItemEmptyInList(user__createUser->UserInfo->Department->DepartmentCodeList->Item)))
		{
			DEBUGL1("__user__createUser department code is empty\n");
			CErrorCodeMapper::GenerateException("STG2_EMPTY_DEPT_CODE");
		}
		// Check for empty Role 
		if(user__createUser->UserInfo->RoleList && false == user__createUser->UserInfo->RoleList->Item.empty()
			&& CCommonFunctions::IsAnyItemEmptyInList(user__createUser->UserInfo->RoleList->Item))
		{
			DEBUGL1("__user__createUser Role is empty\n");
			CErrorCodeMapper::GenerateException("STG2_EMPTY_ROLES");					
		}
		// Check for empty Groups
		if(user__createUser->UserInfo->GroupList && false == user__createUser->UserInfo->GroupList->Item.empty()
			&& CCommonFunctions::IsAnyItemEmptyInList(user__createUser->UserInfo->GroupList->Item))
		{
			DEBUGL1("__user__createUser Group is empty\n");
			CErrorCodeMapper::GenerateException("STG2_EMPTY_GROUPS");					
		}
		
		if( user__createUser->UserInfo->Domain && user__createUser->UserInfo->Domain->empty())
		{
			DEBUGL1("__user__createUser domain is invalid \n");
			CErrorCodeMapper::GenerateException("STG2_INVALID_DOMAIN");
		} 
		
		CUIControllerAdapter::SetXMLValue("", "<UserManager></UserManager>" , pBOProxy); // to clear delta document.
		
		user__createUserResponse->Result = common__operationResultType__OperationSucceeded;
		CString sOperationStatus = UserServices::ExecuteCreateOrEditUserCommand(user__createUser->UserInfo,"AddUser",pBOProxy);
		if(STG2_SUCCESS != sOperationStatus)
		{
			DEBUGL1("ExecuteCreateOrEditUserCommand failed\n");
			CErrorCodeMapper::GenerateException(sOperationStatus,USR_MGR);
		}
				
	}
	catch(CStg2Exception & ex)
	{
		DEBUGL1("Stage2::__user__createUser caught Stg2Exception Exception. Description = %s\n", ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), ex.getFaultType(), ex.getErrorType()) != STATUS_OK)
		DEBUGL4("__user__createUser CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}
	catch(CException & ex)
	{
		DEBUGL1("Stage2::__user__createUser caught CException Exception. Description = %s\n", ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), "", eReceiverError) != STATUS_OK)
		DEBUGL4("__user__createUser CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}

	DEBUGL8("Stage2::__user__createUser exit\n");
	return SOAP_OK;
}
SOAP_FMAC5 int SOAP_FMAC6 __stg2__deleteUser(struct soap* soap, _user__deleteUser *user__deleteUser, _user__deleteUserResponse *user__deleteUserResponse)
{
	DEBUGL8("Stage2::__stg2__deleteUser entry\n");
	if(!CCommonFunctions::IsOutputManagementEnabled( ))
	{
		stage2ErrorStruct stError;
		CErrorCodeMapper::MapInternalErrCodeToStage2Fault(stError, "STG2_UNSUPPORTED_OPERATION");
		CCommonFunctions::GenerateSoapFault(soap, stError.strDescription, stError.strErrCode, stError.eErrorType);
		return SOAP_FAULT;
	}
	return __user__deleteUser(soap,user__deleteUser,user__deleteUserResponse);

}
SOAP_FMAC5 int SOAP_FMAC6 __user__deleteUser(struct soap* soap, _user__deleteUser *user__deleteUser, _user__deleteUserResponse *user__deleteUserResponse)
{
	DEBUGL8("Stage2::__user__deleteUser entry\n");
	try
	{
		BOProxyRef pBOProxy = CCommonFunctions::getBOProxyRef(soap);
		if(!pBOProxy)
		{
			DEBUGL1("__user__deleteUser pBOProxy is NULL\n");
			CErrorCodeMapper::GenerateException("STG2_INVALID_SESSION");
		}
		
		if(!user__deleteUser)  
		{
			CErrorCodeMapper::GenerateException("STG2_MISSING_PARAM");
		}
		
		CString sUserID = user__deleteUser->UserID;
		
		if(sUserID.empty())
		{
			DEBUGL1("UserName is empty\n");
			CErrorCodeMapper::GenerateException("STG2_EMPTY_USER_ID");
		}
		
		user__deleteUserResponse->Result = common__operationResultType__OperationSucceeded;	
		CString sOperationStatus = UserServices::ExecuteDeleteUserCommand(sUserID, pBOProxy);
		if (STG2_SUCCESS !=  sOperationStatus) 
		{
			DEBUGL1("UserServices::ExecuteDeleteUserCommand Operation failed\n");
			CErrorCodeMapper::GenerateException(sOperationStatus, USR_MGR);
		}
				
	}
	catch(CStg2Exception & ex)
	{
		DEBUGL1("Stage2::__user__deleteUser caught Stg2Exception Exception. Description = %s\n", ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), ex.getFaultType(), ex.getErrorType()) != STATUS_OK)
		DEBUGL4("__user__deleteUser CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}
	catch(CException & ex)
	{
		DEBUGL1("Stage2::__user__deleteUser caught CException Exception. Description = %s\n", ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), "", eReceiverError) != STATUS_OK)
		DEBUGL4("__user__deleteUser CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}
	DEBUGL8("Stage2::__user__deleteUser exit\n");
	return SOAP_OK;
}

#ifdef STAGE2_UNSUPPORTED
SOAP_FMAC5 int SOAP_FMAC6 __user__deleteAllUser(struct soap* soap, _user__deleteAllUser *user__deleteAllUser, _user__deleteAllUserResponse *user__deleteAllUserResponse)
{
	DEBUGL8("Stage2::__user__deleteAllUser entry\n");
	try
	{	
		BOProxyRef boProxy = CCommonFunctions::getBOProxyRef(soap);
		if(!boProxy)
		{
			DEBUGL1("__user__deleteAllGroup Invalid Session\n");
			CErrorCodeMapper::GenerateException("STG2_INVALID_SESSION");
		}		
		
		if(!user__deleteAllUser || !user__deleteAllUserResponse) 
		{
			DEBUGL1("Request or Response pointer is NULL\n");
			CErrorCodeMapper::GenerateException("STG2_MISSING_PARAM");
		}		

		//vector<CString> failedUsers;
		Status execCommandStatus = UserServices::ExecuteDeleteAllUsersCommand(boProxy);//,failedUsers);
/* 		if(!failedUsers.empty())
		{
			CString temp = "Following Users were not deleted ";
			vector<CString>::iterator it;
			for(it=failedUsers.begin();it!=failedUsers.end();it++)
			{	
				CString user = (*it) + "\t";
				temp += user;
			}
			temp = CString("deletion of following users failed ")+temp;
			throw CStg2Exception(eReceiverError, "ERR_EBS_USER_OPERATION_DENIED",SOAP_FAULT,temp.c_str() );
		} */
		if(execCommandStatus == STATUS_OK)
			user__deleteAllUserResponse->Result = common__operationResultType__OperationSucceeded;
		else
			user__deleteAllUserResponse->Result = common__operationResultType__OperationFailed;
	}
	catch(CStg2Exception & ex)
	{
		DEBUGL1("Stage2::__user__deleteAllUser caught Stg2Exception Exception. Description = %s\n", ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), ex.getFaultType(), ex.getErrorType()) != STATUS_OK)
		DEBUGL4("__user__deleteAllUser CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}
	catch(CException & ex)
	{
		DEBUGL1("Stage2::__user__deleteAllUser caught CException Exception. Description = %s\n", ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), "", eReceiverError) != STATUS_OK)
		DEBUGL4("__user__deleteAllUser CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}
	DEBUGL8("Stage2::__user__deleteAllUser exit\n");
	return SOAP_OK;
}

#endif



user__userInfoListType* UserServices::GetUsersInfo(vector<string >& vName, soap* soap, BOProxyRef &pBOProxy)
{
	user__userInfoListType* pUserInfoList = soap_new_user__userInfoListType(soap,-1);
	vector<string>::iterator iter = vName.begin();
	vector<string>::iterator iterEnd = vName.end();
		
	for(; iter != iterEnd ; ++iter)
	{
		if(iter->empty())
		{
			if(vName.size() == 1)
			{
				CErrorCodeMapper::GenerateException("STG2_EMPTY_USER_ID");	
			}
			else
			{
				CErrorCodeMapper::GenerateException("STG2_MULTIPLE_EMPTY_USER_ID");	
			}	
		}
		
		CUIControllerAdapter::SetXMLValue("", "<UserManager></UserManager>", pBOProxy); // to clear delta document.
		
		CString sOperationStatus = ExecuteGetUserCommand(*iter,pBOProxy);
		if("STATUS_USER_NOT_FOUND" == sOperationStatus && vName.size() > 1)
		{
			CErrorCodeMapper::GenerateException("STG2_MULTI_USER_DOESNT_EXIST");
		}
		
		if(STG2_SUCCESS != sOperationStatus)
		{
			CErrorCodeMapper::GenerateException(sOperationStatus,USR_MGR);
		}
		pUserInfoList->UserInfo.push_back(UserServices::MapUserInfo("UserManager/Users/User",pBOProxy,soap));
	}
	
	return pUserInfoList;
}

user__userInfoListType* UserServices::GetAllUsersInfo(soap* soap,BOProxyRef &pBOProxy)
{
	DEBUGL8("UserServices::GetAllUserInfo enter \n");

	CUIControllerAdapter::SetXMLValue("", "<UserManager></UserManager>" , pBOProxy); // to clear delta document.
	std::vector<CString> xPaths;
	std::vector<CString> values;
	
	values.push_back("");
	xPaths.push_back(USERS_XPATH);
	

	CUIControllerAdapter::SetXPathValues(xPaths, values, pBOProxy);

	vector<commandParam> vectParams;
	commandParam cmdParam;
	cmdParam.strParamValue = "UserManager";
	cmdParam.strParamName = "userDetails";
	cmdParam.mapAttributes.insert(make_pair("contentType","XPath"));
	vectParams.push_back(cmdParam);
	// Execute the command
	CString strResponse;
	if(STATUS_OK != CUIControllerAdapter::SendCommand("GetUsers",USERS_XPATH,vectParams,strResponse,pBOProxy))
	{
		DEBUGL1("Failed to SendCommand\n");
		CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");
	
	}
	CString sOperationStatus  = CUIControllerAdapter::GetOperationStatusString(strResponse);
	if(STG2_SUCCESS != sOperationStatus)
	{
		if("STATUS_USER_UNAUTHORIZED" == sOperationStatus || "STATUS_PERMISSION_CHECK_ERROR" == sOperationStatus)
		{
			if(CCommonFunctions::IsUserLogin(soap)) // Get User ID if it is UserLogin otherwise return error.
			{
				CString userID = CCommonFunctions::GetLoggedInUserID(soap);
				CString sOperationStatus = ExecuteGetUserCommand(userID,pBOProxy);
				if(STG2_SUCCESS != sOperationStatus)
				{
					CErrorCodeMapper::GenerateException(sOperationStatus,USR_MGR);
				}
				
			}
			else
			{
				CErrorCodeMapper::GenerateException(sOperationStatus,USR_MGR);
			}

		}
		else
		{
			CErrorCodeMapper::GenerateException(sOperationStatus,USR_MGR);
		}
	}
	NodeRef pUsersNode = NULL;
	if(STATUS_OK != pBOProxy->GetValue(USERS_XPATH,pUsersNode) || !pUsersNode)
	{
		DEBUGL1("UserServices::GetAllUsersInfo GetValue failed\n");
		CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");	
	}
	
	NodeListRef pUserNodeList =  pUsersNode->getChildNodes();	
	
	user__userInfoListType* pUserInfoList = soap_new_user__userInfoListType(soap,-1);
	for(unsigned long i = 0; i < pUserNodeList->getLength(); ++i)
	{
		ElementRef pNode = pUserNodeList->item(i);
		if(!pNode) continue;

		CString sID = pNode->getAttribute("ID");
		CString sXPath = "UserManager/Users/User[@ID = '" + sID + "']";
		DEBUGL8("XPath is ++%s++\n", sXPath.c_str());
		pUserInfoList->UserInfo.push_back(MapUserInfo(sXPath , pBOProxy , soap));
	}
	DEBUGL8("UserServices::GetAllUserInfo Exit \n");
	return pUserInfoList;
}

SOAP_FMAC5 int SOAP_FMAC6 __stg2__getUserInfo(struct soap* soap, _user__getUserInfo *user__getUserInfo, _user__getUserInfoResponse *user__getUserInfoResponse)
{
	DEBUGL8("Stage2::__stg2__getUserInfo entry\n");
	if(!CCommonFunctions::IsOutputManagementEnabled( ))
	{
		stage2ErrorStruct stError;
		CErrorCodeMapper::MapInternalErrCodeToStage2Fault(stError, "STG2_UNSUPPORTED_OPERATION");
		CCommonFunctions::GenerateSoapFault(soap, stError.strDescription, stError.strErrCode, stError.eErrorType);
		return SOAP_FAULT;
	}
	return __user__getUserInfo(soap,user__getUserInfo,user__getUserInfoResponse);
}
SOAP_FMAC5 int SOAP_FMAC6 __user__getUserInfo(struct soap* soap, _user__getUserInfo *user__getUserInfo, _user__getUserInfoResponse *user__getUserInfoResponse)
{
	DEBUGL8("__user__getUserInfo entry\n");
	CString        operationName     = "getUserInfo";
	try
	{
		 //Check if we have a conflicting operation in progress
		if(CLongRunningOpHelper::IsAnotherConflictingOperationInProgress(soap, operationName))
		{
			// Don't use GenerateException since it will update Running operation status to "false" which we don't want.
			stage2ErrorStruct stError;
			CErrorCodeMapper::MapInternalErrCodeToStage2Fault(stError, "STG2_CONF_OP_INPROGRESS");
			CCommonFunctions::GenerateSoapFault(soap, stError.strDescription, stError.strErrCode, stError.eErrorType);
			return SOAP_FAULT;	
		}
		else
		{
			//Add this operation to the current running op list
			CLongRunningOpHelper::UpdateRunningOpStatus(soap, operationName, true);
		}
		
		BOProxyRef pBOProxy = CCommonFunctions::getBOProxyRef(soap);
		if(!pBOProxy)
		{
			DEBUGL1("__user__getUserInfo Invalid Session\n");
			CErrorCodeMapper::GenerateException("STG2_INVALID_SESSION");
		}
    
		if(!user__getUserInfo)
		{
			CErrorCodeMapper::GenerateException("STG2_MISSING_PARAM");
		}
		
		if(user__getUserInfo->UserID.empty())
		{
			/* if(CCommonFunctions::IsUserLogin(soap) == false)
			{
				DEBUGL1("UserServicese::GetAllUsersInfo not a user log in\n");
				CErrorCodeMapper::GenerateException("STG2_EMPTY_USER_ID");	
			} */
			user__getUserInfoResponse->UserInfoList = UserServices::GetAllUsersInfo(soap, pBOProxy);

		}
		else
		{
			user__getUserInfoResponse->UserInfoList = UserServices::GetUsersInfo(user__getUserInfo->UserID, soap, pBOProxy);
		}
		//Remove this operation from the current running op list
		CLongRunningOpHelper::UpdateRunningOpStatus(soap, operationName, false);	
				
	}
	catch(CStg2Exception &ex)
	{
		DEBUGL1("Stage2::__user__getUserInfo caught CStg2Exception exception .Description = %s\n",ex.C_Str());
		CLongRunningOpHelper::UpdateRunningOpStatus(soap, operationName, false);
		if(CCommonFunctions::GenerateSoapFault(soap,ex.C_Str(),ex.getFaultType(),ex.getErrorType())!=STATUS_OK)
		DEBUGL4("Stage2::__user_getUserInfo CCommonFunctions::GenerateSoapFault failed\n");
		return SOAP_FAULT;
	}
	catch(CException &ex)
	{
		DEBUGL1("Stage2::__user__getUserInfo caught CException exception.Description = %s\n",ex.C_Str());
		CLongRunningOpHelper::UpdateRunningOpStatus(soap, operationName, false);
		if(CCommonFunctions::GenerateSoapFault(soap,ex.C_Str(),"",eReceiverError) != STATUS_OK)
		DEBUGL4("Stage2 :: __user__getUserInfo CCommonFunctions::GenerateSoapFault failed\n");
		return SOAP_FAULT;
	}
	
	DEBUGL8("Stage2::__user__getUserInfo exit\n");
	return SOAP_OK;
}
#ifdef STAGE2_UNSUPPORTED
SOAP_FMAC5 int SOAP_FMAC6 __user__getOwnershipInfo(struct soap* soap, _user__getOwnershipInfo *user__getOwnershipInfo, _user__getOwnershipInfoResponse *user__getOwnershipInfoResponse)
{

	CCommonFunctions::GenerateSoapFault(soap, "Operation not supported", "ERR_EBS_SERVER_OPERATION_UNSUPPORTED", eReceiverError);
	return SOAP_FAULT;
	/*
	
	try
	{
		DEBUGL1("Stage2:: __user__getOwnershipInfo Entry\n");
		BOProxyRef	pBOProxy  = CCommonFunctions::getBOProxyRef(soap);

		if(!pBOProxy)
		{
			throw CStg2Exception(eSenderError, "ERR_EBS_INVALID_TOKEN", SOAP_FAULT, "Invalid Session");
		}
		
		if (!user__getOwnershipInfoResponse)
		{
			throw CStg2Exception(eSenderError, "ERR_EBS_COMMON_PAR_INVALID_VALUE", SOAP_FAULT, "Required Parameter is missing");
		}

		if (!user__getOwnershipInfo || !user__getOwnershipInfo->Owner)
		{
			throw CStg2Exception(eSenderError, "ERR_EBS_COMMON_PAR_INVALID_VALUE", SOAP_FAULT, "Required Parameter is missing");
		}
		
		user__getOwnershipInfoResponse->OwnershipInfo = UserServices::getOwnershipInfo(soap, pBOProxy, user__getOwnershipInfo->Owner->ID);
		if(!user__getOwnershipInfoResponse->OwnershipInfo)
		{
			user__getOwnershipInfoResponse->OwnershipInfo = soap_new_user__ownershipInfoType(soap, -1);
		}
		
		
		DEBUGL1("Stage2:: __user__getOwnershipInfo Exit\n");
	}
	catch(CStg2Exception &ex)
	{
		DEBUGL1("Stage2::__user__getOwnershipInfo caught CStg2Exception exception .Description = %s\n",ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap,ex.C_Str(),ex.getFaultType(),ex.getErrorType())!=STATUS_OK)
		DEBUGL4("Stage2::__user__getOwnershipInfo CCommonFunctions::GenerateSoapFault failed\n");
		return SOAP_FAULT;
	}
	catch(CException &ex)
	{
		DEBUGL1("Stage2::__user__getOwnershipInfo caught CException exception.Description = %s\n",ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap,ex.C_Str(),"",eReceiverError) != STATUS_OK)
		DEBUGL4("Stage2 :: __user__getOwnershipInfo CCommonFunctions::GenerateSoapFault failed\n");
		return SOAP_FAULT;
	}
	return SOAP_OK;*/
}
SOAP_FMAC5 int SOAP_FMAC6 __user__setOwnershipInfo(struct soap* soap, _user__setOwnershipInfo *user__setOwnershipInfo, _user__setOwnershipInfoResponse *user__setOwnershipInfoResponse)
{
	
	CCommonFunctions::GenerateSoapFault(soap, "Operation not supported", "ERR_EBS_SERVER_OPERATION_UNSUPPORTED", eReceiverError);
	return SOAP_FAULT;
	
	/*try
	{
		DEBUGL1("Stage2:: __user__setOwnershipInfo Entry\n");
		BOProxyRef	pBOProxy  = CCommonFunctions::getBOProxyRef(soap);

		if(!pBOProxy)
		{
			DEBUGL1("Stage2 : BOProxy Acquire Failed to session\n");
			throw CStg2Exception(eSenderError, "ERR_EBS_INVALID_TOKEN", SOAP_FAULT, "Invalid Session");
		}

		if (!user__setOwnershipInfo || !user__setOwnershipInfoResponse 
				|| !user__setOwnershipInfo->Owner  || !user__setOwnershipInfo->OwnershipInfo)
		{
			throw CStg2Exception(eSenderError, "ERR_EBS_COMMON_PAR_INVALID_VALUE", SOAP_FAULT, "One or more Required Paramenters are NULL");
		}

		OffendingParamsList vOffendingValue;
		vOffendingValue.clear();
		if (STATUS_OK != UserServices::setOwnershipInfo(pBOProxy, user__setOwnershipInfo->Owner->ID, user__setOwnershipInfo->OwnershipInfo, vOffendingValue)) 
		{
			
		}
		
		if(vOffendingValue.empty() == false)
		{
			throw CStg2Exception(eSenderError, "ERR_EBS_COMMON_PARTIAL_SUCCESS", SOAP_FAULT, vOffendingValue, "");	
		}


		user__setOwnershipInfoResponse->Result = common__operationResultType__OperationSucceeded;

		DEBUGL1("Stage2:: __user__setOwnershipInfo Exit\n");
	}
	catch(CStg2Exception &ex)
	{
		DEBUGL1("Stage2::__user__setOwnershipInfo caught CStg2Exception exception .Description = %s\n",ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap,ex.C_Str(),ex.getFaultType(),ex.getErrorType())!=STATUS_OK)
		DEBUGL4("Stage2::__user__setOwnershipInfo CCommonFunctions::GenerateSoapFault failed\n");
		return SOAP_FAULT;
	}
	catch(CException &ex)
	{
		DEBUGL1("Stage2::__user__setOwnershipInfo caught CException exception.Description = %s\n",ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap,ex.C_Str(),"",eReceiverError) != STATUS_OK)
		DEBUGL4("Stage2 :: __user__setOwnershipInfo CCommonFunctions::GenerateSoapFault failed\n");
		return SOAP_FAULT;
	}
	return SOAP_OK;*/
}
SOAP_FMAC5 int SOAP_FMAC6 __user__getBoxList(struct soap* soap, _user__getBoxList *user__getBoxList, _user__getBoxListResponse *user__getBoxListResponse)
{

	CCommonFunctions::GenerateSoapFault(soap, "Operation not supported", "ERR_EBS_SERVER_OPERATION_UNSUPPORTED", eReceiverError);
	return SOAP_FAULT;
	/*try
	{
		DEBUGL1("Stage2:: __user__getBoxList Entry\n");
		BOProxyRef	pBOProxy  = CCommonFunctions::getBOProxyRef(soap);
		if(!pBOProxy)
		{
			DEBUGL1(" Stage2 : BOProxy Acquire Failed to session\n");
			throw CStg2Exception(eSenderError, "ERR_EBS_INVALID_TOKEN", SOAP_FAULT, "Invalid Session");
		}

		if (!user__getBoxListResponse || !user__getBoxList || !user__getBoxList->Owner) 
		{
			throw CStg2Exception(eSenderError, "ERR_EBS_COMMON_PAR_INVALID_VALUE", SOAP_FAULT, "One or more Required Paramenters are NULL");
		}

		user__getBoxListResponse->IdList = UserServices::getBoxList(soap, pBOProxy, user__getBoxList->Owner->ID);
		if(!user__getBoxListResponse->IdList)
		{
			user__getBoxListResponse->IdList = soap_new_common__stringListType(soap, -1);
		}
		
		DEBUGL1("Stage2:: __user__getBoxList Exit\n");
	}
	catch(CStg2Exception &ex)
	{
		DEBUGL1("Stage2::__user__getBoxList caught CStg2Exception exception .Description = %s\n",ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap,ex.C_Str(),ex.getFaultType(),ex.getErrorType())!=STATUS_OK)
		DEBUGL4("Stage2::__user__getBoxList CCommonFunctions::GenerateSoapFault failed\n");
		return SOAP_FAULT;
	}
	catch(CException &ex)
	{
		DEBUGL1("Stage2::__user__getBoxList caught CException exception.Description = %s\n",ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap,ex.C_Str(),"",eReceiverError) != STATUS_OK)
		DEBUGL4("Stage2 :: __user__getBoxList CCommonFunctions::GenerateSoapFault failed\n");
		return SOAP_FAULT;
	}

	return SOAP_OK;*/
}
SOAP_FMAC5 int SOAP_FMAC6 __user__setBoxList(struct soap* soap, _user__setBoxList *user__setBoxList, _user__setBoxListResponse *user__setBoxListResponse)
{

	CCommonFunctions::GenerateSoapFault(soap, "Operation not supported", "ERR_EBS_SERVER_OPERATION_UNSUPPORTED", eReceiverError);
	return SOAP_FAULT;
	/*try
	{
		DEBUGL1("Stage2:: __user__setBoxList Entry\n");
				
		BOProxyRef	pBOProxy  = CCommonFunctions::getBOProxyRef(soap);
		if(!pBOProxy)
		{
			DEBUGL1(" Stage2 : BOProxy Acquire Failed to session\n");
			throw CStg2Exception(eSenderError, "ERR_EBS_INVALID_TOKEN", SOAP_FAULT, "Invalid Session");
			
		}
		if (!user__setBoxList || !user__setBoxList->Owner  
				|| !user__setBoxList->OwnedEntityIdList || !user__setBoxListResponse)
		{
			throw CStg2Exception(eSenderError, "ERR_EBS_COMMON_PAR_INVALID_VALUE", SOAP_FAULT, "One or more Required Paramenters are NULL");
		}

		OffendingParamsList vOffendingValue;
		vOffendingValue.clear();
		if (STATUS_OK != UserServices::setBoxList(pBOProxy, user__setBoxList->Owner->ID, user__setBoxList->OwnedEntityIdList, vOffendingValue))
		{
						
		}

		if(vOffendingValue.empty() == false)
		{
			throw CStg2Exception(eSenderError, "ERR_EBS_COMMON_PARTIAL_SUCCESS", SOAP_FAULT, vOffendingValue, "");	
		}

		user__setBoxListResponse->Result = common__operationResultType__OperationSucceeded;
		
		DEBUGL1("Stage2:: __user__setBoxList Exit\n");
		
	}
	catch(CStg2Exception &ex)
	{
		DEBUGL1("Stage2::__user__setBoxList caught CStg2Exception exception .Description = %s\n",ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), ex.getFaultType(), ex.getErrorType(), ex.getOffendingList()) != STATUS_OK)
		DEBUGL4("Stage2::__user__setBoxList CCommonFunctions::GenerateSoapFault failed\n");
		return SOAP_FAULT;
	}
	catch(CException &ex)
	{
		DEBUGL1("Stage2::__user__setBoxList caught CException exception.Description = %s\n",ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap,ex.C_Str(),"",eReceiverError) != STATUS_OK)
		DEBUGL4("Stage2 :: __user__setBoxList CCommonFunctions::GenerateSoapFault failed\n");
		return SOAP_FAULT;
	}
	return SOAP_OK;*/
}
SOAP_FMAC5 int SOAP_FMAC6 __user__getMailBoxList(struct soap* soap, _user__getMailBoxList *user__getMailBoxList, _user__getMailBoxListResponse *user__getMailBoxListResponse)
{
	
	CCommonFunctions::GenerateSoapFault(soap, "Operation not supported", "ERR_EBS_SERVER_OPERATION_UNSUPPORTED", eReceiverError);
	return SOAP_FAULT;
	/*try
	{
		DEBUGL1("Stage2:: __user__getMailBoxList Entry\n");

		BOProxyRef	pBOProxy  = CCommonFunctions::getBOProxyRef(soap);
		if(!pBOProxy)
		{
			DEBUGL1(" Stage2 : BOProxy Acquire Failed to session\n");
			throw CStg2Exception(eSenderError, "ERR_EBS_INVALID_TOKEN", SOAP_FAULT, "Invalid Session");
		}

		if(!user__getMailBoxList || !user__getMailBoxList->Owner || !user__getMailBoxListResponse)
		{
			throw CStg2Exception(eSenderError, "ERR_EBS_COMMON_PAR_INVALID_VALUE", SOAP_FAULT, "One or more Required Paramenters are NULL");
		}		
		
		user__getMailBoxListResponse->IdList = UserServices::getMailBoxList(soap, pBOProxy, user__getMailBoxList->Owner->ID);
		if(!user__getMailBoxListResponse->IdList)
		{
			user__getMailBoxListResponse->IdList = soap_new_common__stringListType(soap, -1);
			
		}
		
		DEBUGL1("Stage2:: __user__getMailBoxList Exit\n");
	}
	catch(CStg2Exception &ex)
	{
		DEBUGL1("Stage2::__user__getMailBoxList caught CStg2Exception exception .Description = %s\n",ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap,ex.C_Str(),ex.getFaultType(),ex.getErrorType())!=STATUS_OK)
		DEBUGL4("Stage2::__user__getMailBoxList CCommonFunctions::GenerateSoapFault failed\n");
		return SOAP_FAULT;
	}
	catch(CException &ex)
	{
		DEBUGL1("Stage2::__user__getMailBoxList caught CException exception.Description = %s\n",ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap,ex.C_Str(),"",eReceiverError) != STATUS_OK)
		DEBUGL4("Stage2 :: __user__getMailBoxList CCommonFunctions::GenerateSoapFault failed\n");
		return SOAP_FAULT;
	}
	return SOAP_OK;*/
}
SOAP_FMAC5 int SOAP_FMAC6 __user__setMailBoxList(struct soap* soap, _user__setMailBoxList *user__setMailBoxList, _user__setMailBoxListResponse *user__setMailBoxListResponse)
{

	CCommonFunctions::GenerateSoapFault(soap, "Operation not supported", "ERR_EBS_SERVER_OPERATION_UNSUPPORTED", eReceiverError);
	return SOAP_FAULT;
	/*
	try
	{
		DEBUGL1("Stage2:: __user__setMailBoxList Entry\n");
		BOProxyRef	pBOProxy = CCommonFunctions::getBOProxyRef(soap);
		if(!pBOProxy)
		{
			DEBUGL1(" Stage2 : BOProxy Acquire Failed to session\n");
			throw CStg2Exception(eSenderError, "ERR_EBS_INVALID_TOKEN", SOAP_FAULT, "Invalid Session");
		}

		if (!user__setMailBoxList || !user__setMailBoxList->Owner  
				|| !user__setMailBoxList->OwnedEntityIdList || !user__setMailBoxListResponse)
		{
			throw CStg2Exception(eSenderError, "ERR_EBS_COMMON_PAR_INVALID_VALUE", SOAP_FAULT, "One or more Required Paramenters are NULL");
		}

		OffendingParamsList vOffendingValue;	
		vOffendingValue.clear();
		if (STATUS_OK != UserServices::setMailBoxList(pBOProxy, user__setMailBoxList->Owner->ID, user__setMailBoxList->OwnedEntityIdList, vOffendingValue))
		{
			
		}
				
		if(vOffendingValue.empty() == false)
		{
			throw CStg2Exception(eSenderError, "ERR_EBS_COMMON_PARTIAL_SUCCESS", SOAP_FAULT, vOffendingValue, "");	
		}
		
		user__setMailBoxListResponse->Result = common__operationResultType__OperationSucceeded;
		
		DEBUGL1("Stage2:: __user__setMailBoxList Exit\n");
	}
	catch(CStg2Exception &ex)
	{
		DEBUGL1("Stage2::__user__setMailBoxList caught CStg2Exception exception .Description = %s\n",ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap,ex.C_Str(),ex.getFaultType(),ex.getErrorType())!=STATUS_OK)
		DEBUGL4("Stage2::__user__setMailBoxList CCommonFunctions::GenerateSoapFault failed\n");
		return SOAP_FAULT;
	}
	catch(CException &ex)
	{
		DEBUGL1("Stage2::__user__setMailBoxList caught CException exception.Description = %s\n",ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap,ex.C_Str(),"",eReceiverError) != STATUS_OK)
		DEBUGL4("Stage2 :: __user__setMailBoxList CCommonFunctions::GenerateSoapFault failed\n");
		return SOAP_FAULT;
	}
	return SOAP_OK;*/
}
SOAP_FMAC5 int SOAP_FMAC6 __user__getAllTemplateList(struct soap* soap, _user__getAllTemplateList *user__getAllTemplateList, _user__getAllTemplateListResponse *user__getAllTemplateListResponse)
{

	CCommonFunctions::GenerateSoapFault(soap, "Operation not supported", "ERR_EBS_SERVER_OPERATION_UNSUPPORTED", eReceiverError);
	return SOAP_FAULT;
	
	/*try
	{
		BOProxyRef	pBOProxy = CCommonFunctions::getBOProxyRef(soap);
		if(!pBOProxy)
		{
			DEBUGL1(" Stage2 : BOProxy Acquire Failed to session\n");
			throw CStg2Exception(eSenderError, "ERR_EBS_INVALID_TOKEN", SOAP_FAULT, "Invalid Session");
		}
				
		if(!user__getAllTemplateList || !user__getAllTemplateList->Owner)
		{
			throw CStg2Exception(eSenderError, "ERR_EBS_COMMON_PAR_INVALID_VALUE", SOAP_FAULT, "One or more Required Paramenters are NULL");
		}		
		if(!user__getAllTemplateListResponse)
		{
			throw CStg2Exception(eSenderError, "ERR_EBS_COMMON_PAR_INVALID_VALUE", SOAP_FAULT, "user__getMailBoxListResponse is NULL");
		}

		user__getAllTemplateListResponse->Template = UserServices::getTemplateOwnership(soap, pBOProxy, user__getAllTemplateList->Owner->ID);
		if(!user__getAllTemplateListResponse->Template)
		{
			user__getAllTemplateListResponse->Template = soap_new_user__templateOwnershipType(soap, -1);
		}
		
	}
	catch(CStg2Exception &ex)
	{
		DEBUGL1("Stage2::__user__getMailBoxList caught CStg2Exception exception .Description = %s\n",ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap,ex.C_Str(),ex.getFaultType(),ex.getErrorType())!=STATUS_OK)
		DEBUGL4("Stage2::__user__getMailBoxList CCommonFunctions::GenerateSoapFault failed\n");
		return SOAP_FAULT;
	}
	catch(CException &ex)
	{
		DEBUGL1("Stage2::__user__getMailBoxList caught CException exception.Description = %s\n",ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap,ex.C_Str(),"",eReceiverError) != STATUS_OK)
		DEBUGL4("Stage2 :: __user__getMailBoxList CCommonFunctions::GenerateSoapFault failed\n");
		return SOAP_FAULT;
	}
	
	//CCommonFunctions::GenerateSoapFault(pSOAP, "Operation not supported", "ERR_EBS_SERVER_OPERATION_UNSUPPORTED", eReceiverError);
	return SOAP_OK;*/

}
SOAP_FMAC5 int SOAP_FMAC6 __user__setAllTemplateList(struct soap* soap, _user__setAllTemplateList *user__setAllTemplateList, _user__setAllTemplateListResponse *user__setAllTemplateListResponse)
{
	CCommonFunctions::GenerateSoapFault(soap, "Operation not supported", "ERR_EBS_SERVER_OPERATION_UNSUPPORTED", eReceiverError);
	return SOAP_FAULT;
	
	/*try
	{	
		DEBUGL4("__user__setAllTemplateList entry\n");
		BOProxyRef pBOProxy = CCommonFunctions::getBOProxyRef(soap);
		if(!pBOProxy)
		throw CStg2Exception(eSenderError,"ERR_EBS_INVALID_TOKEN", SOAP_FAULT, "Invalid Session");
		
		if(!(user__setAllTemplateList) || !(user__setAllTemplateListResponse) || !(user__setAllTemplateList->Owner) ||  !(user__setAllTemplateList->Template))
		{
			throw CStg2Exception(eSenderError, "ERR_EBS_COMMON_PAR_INVALID_VALUE", SOAP_FAULT, "One or more required parameter is not available");
		}
				
		OffendingParamsList vOffendingValue;
		if(STATUS_OK != UserServices::setTemplateOwnership(pBOProxy, user__setAllTemplateList->Owner->ID , user__setAllTemplateList->Template, vOffendingValue))
		{
				
		}
	
		if(vOffendingValue.empty() == false)
		{
			throw CStg2Exception(eSenderError, "ERR_EBS_COMMON_PARTIAL_SUCCESS", SOAP_FAULT, vOffendingValue, "");	
		}
		
		user__setAllTemplateListResponse->Result = common__operationResultType__OperationSucceeded;
	
	}
	catch(CStg2Exception & ex)
	{
		DEBUGL1("Stage2::__user__setAllTemplateList caught Stg2Exception Exception. Description = %s\n", ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), ex.getFaultType(), ex.getErrorType()) != STATUS_OK)
		DEBUGL4("__user__setAllTemplateList CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}
	catch(CException & ex)
	{
		DEBUGL1("Stage2::__user__setAllTemplateList caught CException Exception. Description = %s\n", ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), "", eReceiverError) != STATUS_OK)
		DEBUGL4("__user__setAllTemplateList CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}

	DEBUGL8("Stage2::__user__setAllTemplateList exit\n");
	return SOAP_OK;
	*/	

}

CString UserServices::setFavouriteTemplateGroupType(const BOProxyRef& pBOProxy, user__favouriteTemplateGroupType* pGroup, const CString& sUserName)
{
	commandParam stParam;		
	vector<commandParam> vCommandParams;	
	
	stParam.strParamName = "shortcutId";
	stParam.strParamValue = format(CString(FORMAT_STYLE), pGroup->FavouriteID);
	vCommandParams.push_back(stParam);
	
	stParam.strParamName = "shortcutName";
	stParam.strParamValue = pGroup->FavouriteName ;
	vCommandParams.push_back(stParam);
	
	stParam.strParamName = "shortcutType";
	stParam.strParamValue = "templategroup";
	vCommandParams.push_back(stParam);
	
	stParam.strParamName = "groupId";
	stParam.strParamValue = format(CString(FORMAT_STYLE), pGroup->GroupNumber);;
	vCommandParams.push_back(stParam);
		
	CString strCommandResponse;
	strCommandResponse.clear();
	
	if(STATUS_OK != CUIControllerAdapter::SendCommandParamAsElement("RegisterPrivateShortcut", "JobTemplates", vCommandParams, strCommandResponse, pBOProxy))
	{
		CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");
	}
	
	CString sOperationStatus = CUIControllerAdapter::GetOperationStatusString(strCommandResponse);
	return sOperationStatus;
}


CString UserServices::setFavouriteTemplateType(const BOProxyRef& pBOProxy, user__favouriteTemplateType* pTemplate, const CString& sUserName)
{
	commandParam stParam;		
	vector<commandParam> vCommandParams;	
	
	stParam.strParamName = "shortcutId";
	stParam.strParamValue = format(CString(FORMAT_STYLE), pTemplate->FavouriteID);
	vCommandParams.push_back(stParam);
	
	stParam.strParamName = "shortcutName";
	stParam.strParamValue = pTemplate->FavouriteName ;
	vCommandParams.push_back(stParam);
	
	stParam.strParamName = "shortcutType";
	stParam.strParamValue = "template";
	vCommandParams.push_back(stParam);
		
	stParam.strParamName = "templateId";
	stParam.strParamValue = format(CString(FORMAT_STYLE), pTemplate->TemplateNumber);
	vCommandParams.push_back(stParam);	
	
	if(common__templateCategoryNameType__Standard  == pTemplate->TemplateCategory || common__templateCategoryNameType__Extension == pTemplate->TemplateCategory)
	{
		if(pTemplate->GroupNumber)
		{
			stParam.strParamName = "groupId";
			stParam.strParamValue = format(CString(FORMAT_STYLE), *(pTemplate->GroupNumber));
			vCommandParams.push_back(stParam);
		}
		else
		{
			CErrorCodeMapper::GenerateException("STG2_MISSING_GROUP_NUMBER");	
		}
	}
		
	CString strCommandResponse;
	strCommandResponse.clear();
	
	if(STATUS_OK != CUIControllerAdapter::SendCommandParamAsElement("RegisterPrivateShortcut", "JobTemplates", vCommandParams, strCommandResponse, pBOProxy))
	{
		CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");
	}
	
	CString sOperationStatus = CUIControllerAdapter::GetOperationStatusString(strCommandResponse);
	return sOperationStatus;
}


CString UserServices::setFavouriteURLInfo(const BOProxyRef& pBOProxy, user__favouriteURLType* pURL, const CString& sUserName)
{
	commandParam stParam;		
	vector<commandParam> vCommandParams;	
	
	stParam.strParamName = "shortcutId";
	stParam.strParamValue = format(CString(FORMAT_STYLE), pURL->FavouriteID);
	vCommandParams.push_back(stParam);
	
	stParam.strParamName = "shortcutName";
	stParam.strParamValue = pURL->FavouriteName ;
	vCommandParams.push_back(stParam);
	
	stParam.strParamName = "shortcutType";
	stParam.strParamValue = "url";
	vCommandParams.push_back(stParam);
	
	stParam.strParamName = "urlId";
	stParam.strParamValue = string_cast(pURL->URLID );
	vCommandParams.push_back(stParam);
		
	CString strCommandResponse;
	strCommandResponse.clear();
	
	if(STATUS_OK != CUIControllerAdapter::SendCommandParamAsElement("RegisterPrivateShortcut", "JobTemplates", vCommandParams, strCommandResponse, pBOProxy))
	{
		CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");
	}
	
	CString sOperationStatus = CUIControllerAdapter::GetOperationStatusString(strCommandResponse);
	return sOperationStatus;
}

Status UserServices::SetFavouritesInfo(const BOProxyRef& pBOProxy, user__favouritesInfoType* pUserFavInfo, const CString& sUserName)
{
	DEBUGL4("UserServices::SetFavouritesInfo Enter\n");
	if(!pUserFavInfo)
	{
		return STATUS_FAILED;
	}

	ValidateFavoriteInfo(pUserFavInfo, false);
	
	if(pUserFavInfo->URLList)
	{
		std::vector<user__favouriteURLType * >::iterator iter = pUserFavInfo->URLList->URL.begin();
		std::vector<user__favouriteURLType * >::iterator iterEnd = pUserFavInfo->URLList->URL.end();		
		for(; iter!= iterEnd; ++iter)
		{
			CString sOperationStatus = setFavouriteURLInfo(pBOProxy, *iter, sUserName);
			if(STG2_SUCCESS != sOperationStatus)
			{
				if(iter == pUserFavInfo->URLList->URL.begin())  // if first entry in the list then throw exception
				{
					DEBUGL1("ExecuteModifyBillingCode operation failed\n");
					CErrorCodeMapper::GenerateException(sOperationStatus,TMPLT_MGR);
				}
				else			
				{
					user__favouriteURLType* pURL = *iter;				
					OffendingParamsStruct stOffendingParam;
					stOffendingParam.Name    = "URL";
					stOffendingParam.Value   = pURL->FavouriteName;
					OffendingParamsList   vOffendingParamsList;	
					vOffendingParamsList.push_back(stOffendingParam);
		            CErrorCodeMapper::GenerateException("STG2_FAVOURITE_PARTIAL_SUCCESS",vOffendingParamsList);
				}
			}
		}
	}	
	
	if(pUserFavInfo->Template)
	{
		if(pUserFavInfo->Template->TemplateList)
		{
			std::vector<user__favouriteTemplateType * >::iterator iter = pUserFavInfo->Template->TemplateList->Template.begin();
			std::vector<user__favouriteTemplateType * >::iterator iterEnd = pUserFavInfo->Template->TemplateList->Template.end();
			for(; iter!= iterEnd; ++iter)
			{
				CString sOperationStatus = setFavouriteTemplateType(pBOProxy, *iter, sUserName);
				if(STG2_SUCCESS != sOperationStatus)
				{
					if(iter == pUserFavInfo->Template->TemplateList->Template.begin())
					{
						CErrorCodeMapper::GenerateException(sOperationStatus,TMPLT_MGR);
					}
					else
					{
						OffendingParamsStruct stOffendingParam;
						stOffendingParam.Name    = "Template";
						stOffendingParam.Value   = (*iter)->FavouriteName;
						OffendingParamsList   vOffendingParamsList;	
						vOffendingParamsList.push_back(stOffendingParam);
						CErrorCodeMapper::GenerateException("STG2_FAVOURITE_PARTIAL_SUCCESS",vOffendingParamsList);
					}
				}
			}		
		}
		if(pUserFavInfo->Template->TemplateGroupList)
		{
			std::vector<user__favouriteTemplateGroupType * >::iterator iter = pUserFavInfo->Template->TemplateGroupList->TemplateGroup.begin();
			std::vector<user__favouriteTemplateGroupType * >::iterator iterEnd = pUserFavInfo->Template->TemplateGroupList->TemplateGroup.end();
			for(; iter!= iterEnd; ++iter)
			{
				CString sOperationStatus = setFavouriteTemplateGroupType(pBOProxy, *iter, sUserName);
				if(STG2_SUCCESS != sOperationStatus)
				{
					if(iter == pUserFavInfo->Template->TemplateGroupList->TemplateGroup.begin())
					{
						CErrorCodeMapper::GenerateException(sOperationStatus,TMPLT_MGR);
					}
					else
					{
						OffendingParamsStruct stOffendingParam;
						stOffendingParam.Name    = "TemplateGroup";
						stOffendingParam.Value   = (*iter)->FavouriteName;
						OffendingParamsList   vOffendingParamsList;	
						vOffendingParamsList.push_back(stOffendingParam);
						CErrorCodeMapper::GenerateException("STG2_FAVOURITE_PARTIAL_SUCCESS",vOffendingParamsList);
					}
				}				
			}			
		}
	}	
	
	DEBUGL4("UserServices::SetFavouritesInfo Exit\n");
	return STATUS_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __user__setFavouritesInfo(soap* soap, _user__setFavouritesInfo* user__setFavouritesInfo, _user__setFavouritesInfoResponse* user__setFavouritesInfoResponse)
{
	
	try
	{
		DEBUGL8("Stage2::__user__setFavouritesInfo entry\n");
		BOProxyRef pBOProxy = CCommonFunctions::getBOProxyRef(soap);
		if(!pBOProxy)
		{
			CErrorCodeMapper::GenerateException("STG2_INVALID_SESSION");
		}
		
		if(!user__setFavouritesInfo || !user__setFavouritesInfo->FavouritesInfo || !user__setFavouritesInfo->Owner  || !user__setFavouritesInfoResponse)
		{
			CErrorCodeMapper::GenerateException("STG2_MISSING_PARAM");
		}
		
		if(!user__setFavouritesInfo->Owner->ID)
		{
			CErrorCodeMapper::GenerateException("STG2_EMPTY_ID");		
		}
		
		if(user__setFavouritesInfo->Owner->ID->empty())
		{
			CErrorCodeMapper::GenerateException("STG2_EMPTY_ID");		
		}
		
		CString sUserName = *(user__setFavouritesInfo->Owner->ID);	
		CString sLoggedInUser = CCommonFunctions::GetLoggedInUserName(pBOProxy);

		if(strncasecmp(sUserName.c_str(),sLoggedInUser.c_str() , sUserName.size()))
		{
			CErrorCodeMapper::GenerateException("STG2_USERNAME_NOT_MATCH");
		}
		
		user__setFavouritesInfoResponse->Result = common__operationResultType__OperationSucceeded;
		
		if(STATUS_OK != UserServices::SetFavouritesInfo(pBOProxy, user__setFavouritesInfo->FavouritesInfo, sUserName))
		{
			user__setFavouritesInfoResponse->Result = common__operationResultType__OperationFailed;
		}		
		
		DEBUGL8("Stage2::__user__setFavouritesInfo exit\n");		
	}
	catch(CStg2Exception & ex)
	{
		DEBUGL1("Stage2::__user__setFavouritesInfo caught Stg2Exception Exception. Description = %s\n", ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), ex.getFaultType(), ex.getErrorType()) != STATUS_OK)
		DEBUGL4("__user__setFavouritesInfo CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}
	catch(CException & ex)
	{
		DEBUGL1("Stage2::__user__setFavouritesInfo caught CException Exception. Description = %s\n", ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), "", eReceiverError) != STATUS_OK)
		DEBUGL4("__user__setFavouritesInfo CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}
	return SOAP_OK;
	
}

Ref<Node> UserServices::ExecuteGetPrivateMenuList(const BOProxyRef &pBOProxy)
{
	commandParam stParam;		
	vector<commandParam> vCommandParams;			

	stParam.strParamName = "definedShortCuts";
	stParam.strParamValue = "true";
	vCommandParams.push_back(stParam);
	
	CString strViewXPath = "JobTemplates/View_"+ CUUID().toString()+ "/MenuTemplateList" ;	
	stParam.strParamName = "viewXpath";
	stParam.strParamValue = strViewXPath;
	vCommandParams.push_back(stParam);

	CString strCommandResponse;
	strCommandResponse.clear();
	
	if(STATUS_OK != CUIControllerAdapter::SendCommandParamAsElement("GetPrivateShortcutList", "JobTemplates", vCommandParams, strCommandResponse, pBOProxy))
	{
		CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");
	}
	
	CString sOperationStatus = CUIControllerAdapter::GetOperationStatusString(strCommandResponse);
	if(STG2_SUCCESS != sOperationStatus)
	{
		CErrorCodeMapper::GenerateException(sOperationStatus,TMPLT_MGR);
	}
	
	Ref<Node> pNode =  NULL;
	if(STATUS_OK != pBOProxy->GetValue(strViewXPath.c_str(), pNode) || !pNode)
	{
		CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");
	}
	return pNode;

}


user__favouriteTemplateType* UserServices::getFavouriteTemplateInfo(soap* soap, const Ref<Node>& pNode)
{
	user__favouriteTemplateType* pTemplate = soap_new_user__favouriteTemplateType(soap, -1);
	CString sID = CCommonFunctions::GetValue(pNode, "@mid");
	if(NODE_DOES_NOT_EXIST != sID)
	{
		pTemplate->FavouriteID = atoi(sID.c_str());
	}
	CString sName = CCommonFunctions::GetValue(pNode, "@name");
	if(NODE_DOES_NOT_EXIST != sID)
	{
		pTemplate->FavouriteName = sName;
	}
	CString sGroupID = CCommonFunctions::GetValue(pNode, "Template/gid");
	if(NODE_DOES_NOT_EXIST != sGroupID)
	{	
		pTemplate->GroupNumber = static_cast<int*>(soap_malloc(soap, sizeof(int)));
		*(pTemplate->GroupNumber) = atoi(sGroupID.c_str());
	}	
	CString sTemplateID = CCommonFunctions::GetValue(pNode, "Template/tid");
	if(NODE_DOES_NOT_EXIST != sTemplateID)
	{
		pTemplate->TemplateNumber = atoi(sTemplateID.c_str());
	}	
	
	//    enum common__templateCategoryNameType TemplateCategory; /* required element of type common:templateCategoryNameType */
    //    std::string *ExtensionName;     /* optional element of type xsd:string */
    return pTemplate;
       

}

user__favouriteTemplateGroupType* UserServices::getFavouriteTemplateGroupInfo(soap* soap, const Ref<Node>& pNode)
{
	user__favouriteTemplateGroupType* pTemplateGroup = soap_new_user__favouriteTemplateGroupType(soap, -1);
	
	CString sID = CCommonFunctions::GetValue(pNode, "@mid");
	if(NODE_DOES_NOT_EXIST != sID)
	{
		pTemplateGroup->FavouriteID = atoi(sID.c_str());
	}
	CString sName = CCommonFunctions::GetValue(pNode, "@name");
	if(NODE_DOES_NOT_EXIST != sID)
	{
		pTemplateGroup->FavouriteName = sName;
	}			
	
	//int GroupNumber;        /* required element of type xsd:int */
	CString sGroupID = CCommonFunctions::GetValue(pNode, "Template/gid");
	if(NODE_DOES_NOT_EXIST != sGroupID)
	{	
		pTemplateGroup->GroupNumber =  atoi(sGroupID.c_str());
	}	
	
	//enum common__templateCategoryNameType TemplateCategory; /* required element of type common:templateCategoryNameType */
    //std::string *ExtensionName;     /* optional element of type xsd:string */
	
	return pTemplateGroup;
}


user__favouriteURLType* UserServices::getFavouriteURLInfo(soap* soap, const Ref<Node>& pNode)
{
	user__favouriteURLType* pURLInfo = soap_new_user__favouriteURLType(soap, -1);
	
	CString sID = CCommonFunctions::GetValue(pNode, "@mid");
	if(NODE_DOES_NOT_EXIST != sID)
	{
		pURLInfo->FavouriteID = atoi(sID.c_str());
	}
	CString sName = CCommonFunctions::GetValue(pNode, "@name");
	if(NODE_DOES_NOT_EXIST != sID)
	{
		pURLInfo->FavouriteName = sName;
	}
	CString sURLID = CCommonFunctions::GetValue(pNode, "URLid");
	if(NODE_DOES_NOT_EXIST != sURLID)
	{
		pURLInfo->URLID = atoi(sURLID.c_str());
	}	
	return pURLInfo;
}

user__favouritesInfoType* UserServices::GetFavouritesInfo(soap* soap, const Ref<BOProxy>& pBOProxy, const CString& sUserName)
{
	user__favouritesInfoType* pFavourites  = NULL;	
	Ref<Node> pMenuTemplateList = ExecuteGetPrivateMenuList(pBOProxy);
	Ref<HierarchicalDB> pHDB = HierarchicalDB::Acquire(NULL);
		
	//CString sBindPath = "User[@name='" + sUserName + "']/ShortCutMenu";
	CString sBindPath = "User/ShortCutMenu";
	DEBUGL4("BindPath is ++%s++\n", sBindPath.c_str());
		
	Ref<NodeList> pSCList = pHDB->FindNodes(pMenuTemplateList, sBindPath);
	user__favouriteURLListType* pURLList = soap_new_user__favouriteURLListType(soap, -1);
    user__favouriteRepositoryListType* pBoxList = soap_new_user__favouriteRepositoryListType(soap, -1);
    user__favouriteRepositoryListType* pMailBoxList = soap_new_user__favouriteRepositoryListType(soap, -1); 
    
	user__favouriteTemplateListType* pTemplateList = soap_new_user__favouriteTemplateListType(soap, -1);
	user__favouriteTemplateGroupListType* pTemplateGroupList = soap_new_user__favouriteTemplateGroupListType(soap, -1);
	
	for(unsigned long i = 0; i < pSCList->getLength(); ++i)
	{
		Ref<Element> pSC = pSCList->item(i);
		if(!pSC)
		{
			continue;
		}		
		CString sType = pSC->getAttribute("type");
		if(TYPE_URL == sType)
		{
			pURLList->URL.push_back(getFavouriteURLInfo(soap,pSC));
		}
		if(TYPE_TEMPLATE == sType )
		{
			pTemplateList->Template.push_back(getFavouriteTemplateInfo(soap, pSC));
		}	
		if(TYPE_TEMPLATE_GROUP == sType)
		{
			pTemplateGroupList->TemplateGroup.push_back(getFavouriteTemplateGroupInfo(soap, pSC));
		}
	}

	if(false == pURLList->URL.empty() || false == pBoxList->Repository.empty() || false == pMailBoxList->Repository.empty() 
		|| false == pTemplateList->Template.empty()  || false == pTemplateGroupList->TemplateGroup.empty())
	{
		pFavourites = soap_new_user__favouritesInfoType(soap, -1);
		if(false == pURLList->URL.empty())
		{
			pFavourites->URLList = pURLList;
		}
		if(false == pBoxList->Repository.empty())
		{
			pFavourites->BoxList = pBoxList;
		}
		if(false == pMailBoxList->Repository.empty())
		{
			pFavourites->MailBoxList = pMailBoxList;
		}
		if(false == pTemplateList->Template.empty()  || false == pTemplateGroupList->TemplateGroup.empty())
		{
			pFavourites->Template = soap_new_user__favouriteTemplateInfoType(soap, -1);
			if(false == pTemplateList->Template.empty())
			{
				pFavourites->Template->TemplateList = pTemplateList;
			}
			if(false == pTemplateGroupList->TemplateGroup.empty())
			{
				pFavourites->Template->TemplateGroupList = pTemplateGroupList;
			}
		}
	}
	return pFavourites;
}


SOAP_FMAC5 int SOAP_FMAC6 __user__getFavouritesInfo(soap* soap, _user__getFavouritesInfo* user__getFavouritesInfo, _user__getFavouritesInfoResponse* user__getFavouritesInfoResponse)
{
	try
	{
		DEBUGL8("Stage2::__user__getFavouritesInfo entry\n");
		BOProxyRef pBOProxy = CCommonFunctions::getBOProxyRef(soap);
		
		if(!pBOProxy)
		{
			CErrorCodeMapper::GenerateException("STG2_INVALID_SESSION");		
		}
		
		if(!user__getFavouritesInfo || !user__getFavouritesInfo->Owner  || !user__getFavouritesInfoResponse)
		{
			CErrorCodeMapper::GenerateException("STG2_MISSING_PARAM");
		}
		if(!user__getFavouritesInfo->Owner->ID)
		{
			CErrorCodeMapper::GenerateException("STG2_EMPTY_ID");		
		}

		if(user__getFavouritesInfo->Owner->ID->empty())
		{
			CErrorCodeMapper::GenerateException("STG2_EMPTY_ID");		
		}
		
		CString sUserName = *(user__getFavouritesInfo->Owner->ID);	
		CString sLoggedInUser = CCommonFunctions::GetLoggedInUserName(pBOProxy);
		if(strncasecmp(sUserName.c_str(),sLoggedInUser.c_str() , sUserName.size()))
		{
			CErrorCodeMapper::GenerateException("STG2_USERNAME_NOT_MATCH");
		}
		
		user__getFavouritesInfoResponse->FavouritesInfo = UserServices::GetFavouritesInfo(soap, pBOProxy, sUserName);
		
		DEBUGL8("Stage2::__user__getFavouritesInfo exit\n");		
	}
	catch(CStg2Exception & ex)
	{
		DEBUGL1("Stage2::__user__getFavouritesInfo caught Stg2Exception Exception. Description = %s\n", ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), ex.getFaultType(), ex.getErrorType()) != STATUS_OK)
		DEBUGL4("__user__getFavouritesInfo CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}
	catch(CException & ex)
	{
		DEBUGL1("Stage2::__user__getFavouritesInfo caught CException Exception. Description = %s\n", ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), "", eReceiverError) != STATUS_OK)
		DEBUGL4("__user__getFavouritesInfo CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}
	return SOAP_OK;
}


SOAP_FMAC5 int SOAP_FMAC6 __user__getAllConfigurationInfo(struct soap* pSOAP, _user__getAllConfigurationInfo *user__getAllConfigurationInfo, _user__getAllConfigurationInfoResponse *user__getAllConfigurationInfoResponse)
{
	CCommonFunctions::GenerateSoapFault(pSOAP, "Operation not supported", "ERR_EBS_SERVER_OPERATION_UNSUPPORTED", eReceiverError);
	return SOAP_FAULT;

}


SOAP_FMAC5 int SOAP_FMAC6 __user__setAllConfigurationInfo(struct soap* pSOAP, _user__setAllConfigurationInfo *user__setAllConfigurationInfo, _user__setAllConfigurationInfoResponse *user__setAllConfigurationInfoResponse)
{
	CCommonFunctions::GenerateSoapFault(pSOAP, "Operation not supported", "ERR_EBS_SERVER_OPERATION_UNSUPPORTED", eReceiverError);
	return SOAP_FAULT;
}


SOAP_FMAC5 int SOAP_FMAC6 __user__getAllNotification(struct soap* pSOAP, _user__getAllNotification *user__getAllNotification, _user__getAllNotificationResponse *user__getAllNotificationResponse)
{
	CCommonFunctions::GenerateSoapFault(pSOAP, "Operation not supported", "ERR_EBS_SERVER_OPERATION_UNSUPPORTED", eReceiverError);
	return SOAP_FAULT;
}


SOAP_FMAC5 int SOAP_FMAC6 __user__setAllNotification(struct soap* pSOAP, _user__setAllNotification *user__setAllNotification, _user__setAllNotificationResponse *user__setAllNotificationResponse)
{
	CCommonFunctions::GenerateSoapFault(pSOAP, "Operation not supported", "ERR_EBS_SERVER_OPERATION_UNSUPPORTED", eReceiverError);
	return SOAP_FAULT;
}


SOAP_FMAC5 int SOAP_FMAC6 __user__getNotification(struct soap* pSOAP, _user__getNotification *user__getNotification, _user__getNotificationResponse *user__getNotificationResponse)
{
	CCommonFunctions::GenerateSoapFault(pSOAP, "Operation not supported", "ERR_EBS_SERVER_OPERATION_UNSUPPORTED", eReceiverError);
	return SOAP_FAULT;
}


SOAP_FMAC5 int SOAP_FMAC6 __user__setNotification(struct soap* pSOAP, _user__setNotification *user__setNotification, _user__setNotificationResponse *user__setNotificationResponse)
{
	CCommonFunctions::GenerateSoapFault(pSOAP, "Operation not supported", "ERR_EBS_SERVER_OPERATION_UNSUPPORTED", eReceiverError);
	return SOAP_FAULT;
}


SOAP_FMAC5 int SOAP_FMAC6 __user__getNotificationDestinationList(struct soap* pSOAP, _user__getNotificationDestinationList *user__getNotificationDestinationList, _user__getNotificationDestinationListResponse *user__getNotificationDestinationListResponse)
{
	CCommonFunctions::GenerateSoapFault(pSOAP, "Operation not supported", "ERR_EBS_SERVER_OPERATION_UNSUPPORTED", eReceiverError);
	return SOAP_FAULT;
}


SOAP_FMAC5 int SOAP_FMAC6 __user__setNotificationDestinationList(struct soap* pSOAP, _user__setNotificationDestinationList *user__setNotificationDestinationList, _user__setNotificationDestinationListResponse *user__setNotificationDestinationListResponse)
{
	CCommonFunctions::GenerateSoapFault(pSOAP, "Operation not supported", "ERR_EBS_SERVER_OPERATION_UNSUPPORTED", eReceiverError);
	return SOAP_FAULT;
}
#endif
SOAP_FMAC5 int SOAP_FMAC6 __stg2__getAllQuota(struct soap* soap, _user__getAllQuota *user__getAllQuota, _user__getAllQuotaResponse *user__getAllQuotaResponse)
{
	DEBUGL8("Stage2::__stg2__getAllQuota entry\n");
	if(!CCommonFunctions::IsOutputManagementEnabled( ))
	{
		stage2ErrorStruct stError;
		CErrorCodeMapper::MapInternalErrCodeToStage2Fault(stError, "STG2_UNSUPPORTED_OPERATION");
		CCommonFunctions::GenerateSoapFault(soap, stError.strDescription, stError.strErrCode, stError.eErrorType);
		return SOAP_FAULT;
	}
	return __user__getAllQuota(soap,user__getAllQuota,user__getAllQuotaResponse);

}


SOAP_FMAC5 int SOAP_FMAC6 __user__getAllQuota(struct soap* soap, _user__getAllQuota *user__getAllQuota, _user__getAllQuotaResponse *user__getAllQuotaResponse)
{
	DEBUGL8("Stage2::__user__getAllQuota entry\n");
	try
	{
		BOProxyRef pBOProxy = CCommonFunctions::getBOProxyRef(soap);
		if(!pBOProxy)
			CErrorCodeMapper::GenerateException("STG2_INVALID_SESSION");

		user__ownerDepartmentType* pDeptType = dynamic_cast <user__ownerDepartmentType *> (user__getAllQuota->Owner);
		user__ownerUserType* pUserType = dynamic_cast<user__ownerUserType *> (user__getAllQuota->Owner);
		user__ownerMDSUserType* pMDSType = dynamic_cast<user__ownerMDSUserType *> (user__getAllQuota->Owner);
			
		
		if( pDeptType || pUserType )
		{
			// return error if
			// 1. ID node is present but is empty.
			// 2. ID node is not present and Login is not user login type.
					
			if((user__getAllQuota->Owner->ID && user__getAllQuota->Owner->ID->empty()) || 
				(!user__getAllQuota->Owner->ID && false == CCommonFunctions::IsUserLogin(soap))) 
			{
				DEBUGL1("ID is emtpy\n");
				CErrorCodeMapper::GenerateException("STG2_EMPTY_ID");			
			}
					
			user__getAllQuotaResponse->Quota = soap_new_common__quotaCategoryType(soap,-1);
			
			if(pDeptType)
			{
				if(!user__getAllQuota->Owner->ID)
				{
					CErrorCodeMapper::GenerateException("STG2_EMPTY_ID");
				}
				
				CString	sDepartmentCode = *(user__getAllQuota->Owner->ID);
				
				if(STATUS_OK != DeviceRecord::ExecuteGetDepartmentInfoCommand(sDepartmentCode,pBOProxy))
				{
					DEBUGL1("DeviceRecord::ExecuteGetDepartmentInfoCommand\n");
					CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");			
				}	
				
				CString sXPath = CString(DEPARTMENT_BILLINGCODE_PATH) + "[code=" + sDepartmentCode + "]/Quota";
				user__getAllQuotaResponse->Quota->Total = soap_new_common__quotaType(soap,-1);
				user__getAllQuotaResponse->Quota->Total = UserServices::MapTotalQuotaInfo(sXPath,pBOProxy,soap);
				
				user__getAllQuotaResponse->Quota->Job = soap_new_common__jobQuotaCategoryType(soap, -1);
				user__getAllQuotaResponse->Quota->Job->Print  = UserServices::MapUserQuotaInfo(sXPath,pBOProxy,soap);
				
				
			}
			else if (pUserType)
			{
			
				CString sUserID;
				if(!user__getAllQuota->Owner->ID)
				{
					sUserID = CCommonFunctions::GetLoggedInUserID(soap);	
				}
				else
				{
					sUserID = *(user__getAllQuota->Owner->ID);
				}
					
				CString sOperationStatus = UserServices::ExecuteGetUserCommand(sUserID,pBOProxy);
				if(STG2_SUCCESS != sOperationStatus)
				{
					CErrorCodeMapper::GenerateException(sOperationStatus,USR_MGR);
				}
							
				CString sXPath = CString(USERS_XPATH) +"/User/Information/AccountingProperty/Quota";
				user__getAllQuotaResponse->Quota->Total = soap_new_common__quotaType(soap,-1);
				user__getAllQuotaResponse->Quota->Total = UserServices::MapTotalQuotaInfo(sXPath,pBOProxy,soap) ;
				
				user__getAllQuotaResponse->Quota->Job = soap_new_common__jobQuotaCategoryType(soap,-1);
				user__getAllQuotaResponse->Quota->Job->Print  = UserServices::MapUserQuotaInfo(sXPath,pBOProxy,soap);
			}
		}
		else if(pMDSType)
		{
			DEBUGL1("Owner Type is MDS not valid\n");
			CErrorCodeMapper::GenerateException("STG2_INVALID_OWNER_TYPE");
		}
		else
		{
			DEBUGL1("Owner Type is not valid\n");
			CErrorCodeMapper::GenerateException("STG2_INVALID_OWNER_TYPE");
		}

		//TBD:: no support for connector quota so far.
/*		user__getAllQuotaResponse->Quota->Job = soap_new_common__jobQuotaCategoryType(soap,-1);
		user__getAllQuotaResponse->Quota->Job->Print  = soap_new_common__printQuotaCategoryType(soap,-1);
		user__getAllQuotaResponse->Quota->Job->Print = ownerType == "User" ? UserServices::MapUserQuotaInfo(CString(USERS_XPATH)+CString("/User/Information/AccountingProperty/Quota"),boProxy,soap) : UserServices::MapUserQuotaInfo(CString(DEPARTMENT_BILLINGCODE_PATH)+"[code="+*(user__getAllQuota->Owner->ID)+"]/Quota",boProxy,soap);*/
		DEBUGL8("Stage2::__user_getAllQuota Connector type not supported\n");
	}
	catch(CStg2Exception & ex)
	{
		DEBUGL1("Stage2::__user__getAllQuota caught Stg2Exception Exception. Description = %s\n", ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), ex.getFaultType(), ex.getErrorType()) != STATUS_OK)
		DEBUGL4("__user__getAllQuota CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}
	catch(CException & ex)
	{
		DEBUGL1("Stage2::__user__getAllQuota caught CException Exception. Description = %s\n", ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), "", eReceiverError) != STATUS_OK)
		DEBUGL4("__user__getAllQuota CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}

	DEBUGL8("Stage2::__user__getAllQuota exit\n");
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __stg2__setAllQuota(struct soap* soap, _user__setAllQuota *user__setAllQuota, _user__setAllQuotaResponse *user__setAllQuotaResponse)
{
	DEBUGL8("Stage2::__stg2__setAllQuota entry\n");
	if(!CCommonFunctions::IsOutputManagementEnabled( ))
	{
		stage2ErrorStruct stError;
		CErrorCodeMapper::MapInternalErrCodeToStage2Fault(stError, "STG2_UNSUPPORTED_OPERATION");
		CCommonFunctions::GenerateSoapFault(soap, stError.strDescription, stError.strErrCode, stError.eErrorType);
		return SOAP_FAULT;
	}
	return __user__setAllQuota(soap,user__setAllQuota,user__setAllQuotaResponse);

}	
SOAP_FMAC5 int SOAP_FMAC6 __user__setAllQuota(struct soap* soap, _user__setAllQuota *user__setAllQuota, _user__setAllQuotaResponse *user__setAllQuotaResponse)
{
	DEBUGL8("Stage2::__user__setAllQuota entry\n");
	try
	{
		BOProxyRef boProxy = CCommonFunctions::getBOProxyRef(soap);
		if(!boProxy)
		{
			DEBUGL1("__user__setAllQuota Invalid Session\n");
			CErrorCodeMapper::GenerateException("STG2_INVALID_SESSION");			
		}

		if(!user__setAllQuota || !user__setAllQuota->Owner ||!user__setAllQuota->Quota)
		{
			DEBUGL2("__user__setAllQuota invalid input\n");
			CErrorCodeMapper::GenerateException("STG2_MISSING_PARAM");
		}
		
			
		user__ownerDepartmentType *deptType = dynamic_cast<user__ownerDepartmentType *>(user__setAllQuota->Owner);
		
		user__ownerUserType *userType = dynamic_cast<user__ownerUserType *>(user__setAllQuota->Owner);
		
		user__ownerMDSUserType *mdsuserType = dynamic_cast<user__ownerMDSUserType *>(user__setAllQuota->Owner);
		CString ownerType = "";
		if(userType)
		{
			ownerType = "User";
		}
		else if(deptType)
		{
			ownerType = "Department";	
		}
		else if( mdsuserType)
		{
			ownerType = "MDSUser";	
		}else
		{
			DEBUGL1("Owner Type is not valid\n");
			CErrorCodeMapper::GenerateException("STG2_INVALID_OWNER_TYPE");
		}
		DEBUGL8("_user__setAllQuota :: ownerType is %s \n",ownerType.c_str());
		
		if( ownerType == "User" || ownerType == "Department")
		{
			if(!user__setAllQuota->Quota->Job && !user__setAllQuota->Quota->Total)
			{
				DEBUGL2("__user__setAllQuota both job and total quota are not provided\n");
				CErrorCodeMapper::GenerateException("STG2_MISSING_PARAM");
			}
			
			if(user__setAllQuota->Quota->Job && user__setAllQuota->Quota->Job->Print->__union_printQuotaCategoryType == SOAP_UNION__common__union_printQuotaCategoryType_PrintJobQuota)
			{
				DEBUGL1("user__setAllQuota Invalid Quota Type SOAP_UNION__common__union_printQuotaCategoryType_PrintJobQuota\n");
				CErrorCodeMapper::GenerateException("STATUS_INVALID_INPUT");	
			}
		
			if(!user__setAllQuota->Owner->ID || user__setAllQuota->Owner->ID->empty())
			{
				DEBUGL1("ID is empty\n");
				CErrorCodeMapper::GenerateException("STG2_EMPTY_ID");
			}
			CString ID = "";
			CString sDeptName = "",sDeptCode="";
			if( ownerType == "Department" )
			{
				CString sOperationStatus = UserServices::ExecuteSearchDepartmentCommand(*(user__setAllQuota->Owner->ID),boProxy);
				if(STG2_SUCCESS != sOperationStatus)
				{
					DEBUGL1("UserServices::ExecuteSearchDepartmentCommand\n");
					CErrorCodeMapper::GenerateException(sOperationStatus, ACC_MGR);
				}
				ID = CUIControllerAdapter::GetTextNodeValue("Accounting/BillingCodes/BillingCode/@ID",boProxy);			
				sDeptName = CUIControllerAdapter::GetTextNodeValue("Accounting/BillingCodes/BillingCode/name",boProxy);			
				sDeptCode = CUIControllerAdapter::GetTextNodeValue("Accounting/BillingCodes/BillingCode/code",boProxy);			
			}
			Status execCommandStatus = STATUS_OK;
			if(user__setAllQuota->Quota	&& user__setAllQuota->Quota->Total)
			{
			
				if(STATUS_OK != UserServices::SetTotalQuotaInfo(user__setAllQuota->Quota->Total,boProxy,ownerType))
				{
					DEBUGL1("_user__setAllQuota SetTotalQuotaInfo failed\n");
					CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");			
				}	
				
				execCommandStatus = ownerType == "User" ?  UserServices::ExecuteModifyUserQuotaInfo(*(user__setAllQuota->Owner->ID),boProxy) :  UserServices::ExecuteModifyDeptQuotaInfo(ID,sDeptName,sDeptCode,boProxy);
				if(STATUS_OK != execCommandStatus)
				{
					DEBUGL1("_user__setAllQuota Updating Quota info  failed\n");
					CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");			
				}
				
			}	
			
			if( user__setAllQuota->Quota	&& user__setAllQuota->Quota->Job )
			{
				if(STATUS_OK != UserServices::SetColourQuotaInfo(user__setAllQuota->Quota->Job->Print->union_printQuotaCategoryType.ColorJobQuota,boProxy,ownerType))
				{
					DEBUGL1("_user__setAllQuota SetColourQuotaInfo failed\n");
					CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");			
				}
				
				execCommandStatus = ownerType == "User" ?  UserServices::ExecuteModifyUserQuotaInfo(*(user__setAllQuota->Owner->ID),boProxy) :  UserServices::ExecuteModifyDeptQuotaInfo(ID,sDeptName,sDeptCode,boProxy);
				if(STATUS_OK != execCommandStatus)
				{
					DEBUGL1("_user__setAllQuota Updating Quota info  failed\n");
					CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");			
				}
			}
		}
		else if(ownerType == "MDSUser")
		{
					
			CString sMDSMode = "";
			if(boProxy->GetValue("Controller/Settings/Functions/MDS/Authentication",sMDSMode) != STATUS_OK)
			{
				DEBUGL1("Failed in getvalue of controller dom!\n");
				CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");
			}
			if(sMDSMode != "true")
			{
				DEBUGL1("MDS Mode is been disabled!!!\n");
				CErrorCodeMapper::GenerateException("STG2_UNSUPPORTED_OPERATION");
			}
		
			if(!user__setAllQuota->Quota->Job && !user__setAllQuota->Quota->Total)
		   {
			DEBUGL2("__user__setAllQuota both job and total quota are not provided\n");
			CErrorCodeMapper::GenerateException("STG2_MISSING_PARAM");
		   }

		   if(user__setAllQuota->Quota->Job && user__setAllQuota->Quota->Job->Print->__union_printQuotaCategoryType == SOAP_UNION__common__union_printQuotaCategoryType_PrintJobQuota)
		   {
			DEBUGL1("user__setAllQuota Invalid Quota Type SOAP_UNION__common__union_printQuotaCategoryType_PrintJobQuota\n");
			CErrorCodeMapper::GenerateException("STATUS_INVALID_INPUT");
		   }

			CString sUserName = "";
			if(mdsuserType->UserName)
			{
				sUserName = CCommonFunctions::EscapedXMLString(*(mdsuserType->UserName));
			}	
			if(sUserName.empty())
			{
				DEBUGL1("__user__setAllQuotaMDS:: User name is empty!\n");
				throw CStg2Exception(eSenderError, "ERR_EBS_COMMON_PAR_INVALID_VALUE",SOAP_FAULT, "UserName is empty");
			}
			/*if(CCommonFunctions::GetLoggedInUserName(boProxy) != sUserName)
			{
				DEBUGL1("__user__setAllQuotaMDS:: Logged in user does not exist!\n");
				throw CStg2Exception(eSenderError, "ERR_EBS_AUTH_INVALID_CREDENTIALS",SOAP_FAULT, "Logged in user does not exist");
			}*/
			CString sDomainName = "";
			if(mdsuserType->Domain)
			{
				sDomainName = CCommonFunctions::EscapedXMLString(*(mdsuserType->Domain));
			}
			vector<commandParam> vecParams;
			commandParam stParam;
			stParam.strParamName = "userName";
			stParam.strParamValue = sUserName;
			vecParams.push_back(stParam);
			stParam.strParamName = "domainName";
			stParam.strParamValue = sDomainName;
			vecParams.push_back(stParam);	
			if(user__setAllQuota->Quota)
			{
				if(user__setAllQuota->Quota->Total)
				{
				stParam.strParamName = "jobQuotaEnabled";
				(user__setAllQuota->Quota->Total->Enable == true)?stParam.strParamValue = "true":stParam.strParamValue = "false";
				vecParams.push_back(stParam);
				if(user__setAllQuota->Quota->Total->Balance)
				{
					stParam.strParamName = "jobRemainingQuantity";
					stParam.strParamValue = *(user__setAllQuota->Quota->Total->Balance);
					vecParams.push_back(stParam);
				}
				}
			
				if(user__setAllQuota->Quota->Job && user__setAllQuota->Quota->Job->Print)
				{
					if( user__setAllQuota->Quota->Job->Print->__union_printQuotaCategoryType == SOAP_UNION__common__union_printQuotaCategoryType_ColorJobQuota)
					{
						common__colorJobQuotaType *pColorJobQuota = user__setAllQuota->Quota->Job->Print->union_printQuotaCategoryType.ColorJobQuota;
						if(pColorJobQuota)
						{
							CString sQuotaEnable = "";
							CString sBalance = "";
							if(pColorJobQuota->ColorQuota)
							{
								sQuotaEnable = ((pColorJobQuota->ColorQuota->Enable) == true) ? "true":"false";
								stParam.strParamName = "fullColorQuotaEnabled";											
								stParam.strParamValue = sQuotaEnable;
								vecParams.push_back(stParam);
								if(pColorJobQuota->ColorQuota->Balance)
								{
									sBalance = *(pColorJobQuota->ColorQuota->Balance);
									stParam.strParamName = "fullColorRemainingQuantity";											
									stParam.strParamValue = sBalance;
									vecParams.push_back(stParam);
								}
							}
							if(pColorJobQuota->BlackQuota)
							{
								sQuotaEnable = ((pColorJobQuota->BlackQuota->Enable) == true) ? "true":"false";
								stParam.strParamName = "blackQuotaEnabled";											
								stParam.strParamValue = sQuotaEnable;
								vecParams.push_back(stParam);
								if(pColorJobQuota->BlackQuota->Balance)
								{
									sBalance = *(pColorJobQuota->BlackQuota->Balance);
									stParam.strParamName = "blackRemainingQuantity";											
									stParam.strParamValue = sBalance;
									vecParams.push_back(stParam);
								}
							}
						}
					}	
				}
			}
			CString strResponse;
			if(STATUS_OK != CUIControllerAdapter::SendCommand("ModifyMDSUserQuota",ACCOUNTING_COMMANDNODE_XPATH,vecParams,strResponse,boProxy))
			{
				DEBUGL1("Failed to SendCommand\n");
				CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");
			}
			CString sOperationStatus  = CUIControllerAdapter::GetOperationStatusString(strResponse);
			if(sOperationStatus == "STATUS_UNKNOWN_RESOURCE" )
			{
				DEBUGL1("Failed:: Logged in user does not exist! \n");
				throw CStg2Exception(eSenderError, "ERR_EBS_AUTH_INVALID_CREDENTIALS",SOAP_FAULT, "Logged in user does not exist");
			}
			if( sOperationStatus != STG2_SUCCESS )
			{
				DEBUGL1("Failed response for PrepareMDSUser! \n");
				CErrorCodeMapper::GenerateException(sOperationStatus,ACC_MGR);
			}
		}
		
		user__setAllQuotaResponse->Result = common__operationResultType__OperationSucceeded;
		
		DEBUGL8("Stage2 :: __user_setAllQuota : connector quota info not supported\n");
	}
	catch(CStg2Exception & ex)
	{
		DEBUGL1("Stage2::__user__setAllQuota caught Stg2Exception Exception. Description = %s\n", ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), ex.getFaultType(), ex.getErrorType()) != STATUS_OK)
		DEBUGL4("__user__setAllQuota CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}
	catch(CException & ex)
	{
		DEBUGL1("Stage2::__user__setAllQuota caught CException Exception. Description = %s\n", ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), "", eReceiverError) != STATUS_OK)
		DEBUGL4("__user__setAllQuota CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}
	DEBUGL8("Stage2::__user__setAllQuota exit\n");
	return SOAP_OK;
}





#ifdef STAGE2_UNSUPPORTED
SOAP_FMAC5 int SOAP_FMAC6 __user__getConnectorQuota(struct soap* pSOAP, _user__getConnectorQuota *user__getConnectorQuota, _user__getConnectorQuotaResponse *user__getConnectorQuotaResponse)
{
	CCommonFunctions::GenerateSoapFault(pSOAP, "Operation not supported", "ERR_EBS_SERVER_OPERATION_UNSUPPORTED", eReceiverError);
	return SOAP_FAULT;
}


SOAP_FMAC5 int SOAP_FMAC6 __user__setConnectorQuota(struct soap* pSOAP, _user__setConnectorQuota *user__setConnectorQuota, _user__setConnectorQuotaResponse *user__setConnectorQuotaResponse)
{
	CCommonFunctions::GenerateSoapFault(pSOAP, "Operation not supported", "ERR_EBS_SERVER_OPERATION_UNSUPPORTED", eReceiverError);
	return SOAP_FAULT;
}


SOAP_FMAC5 int SOAP_FMAC6 __user__getAllDepartment(struct soap* soap, _user__getAllDepartment *user__getAllDepartment, _user__getAllDepartmentResponse *user__getAllDepartmentResponse)
{
	DEBUGL6("Stage2 : __user__getAllDepartment Entry\n");
	try
	{
		BOProxyRef boProxy = CCommonFunctions::getBOProxyRef(soap);
		if(!boProxy)
			CErrorCodeMapper::GenerateException("STG2_INVALID_SESSION");	
		
		if(!user__getAllDepartmentResponse)
			CErrorCodeMapper::GenerateException("STG2_MISSING_PARAM");
		
		user__getAllDepartmentResponse->DepartmentList = soap_new_user__departmentListType(soap, -1);
		
		UserServices::getDepartments(soap, user__getAllDepartmentResponse->DepartmentList->Department);
	}
	catch(CStg2Exception & ex)
	{
		DEBUGL1("Stage2::__user__getAllDepartment caught Stg2Exception Exception. Description = %s\n", ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), ex.getFaultType(), ex.getErrorType()) != STATUS_OK)
		DEBUGL4("__user__getAllDepartment CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}
	catch(CException & ex)
	{
		DEBUGL1("Stage2::__user__getAllDepartment caught CException Exception. Description = %s\n", ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), "", eReceiverError) != STATUS_OK)
		DEBUGL4("__user__getAllDepartment CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}
	
	DEBUGL6("Stage2 : __user__getAllDepartment Exit\n");
	return SOAP_OK;
}


SOAP_FMAC5 int SOAP_FMAC6 __user__setAllDepartment(struct soap* soap, _user__setAllDepartment *user__setAllDepartment, _user__setAllDepartmentResponse *user__setAllDepartmentResponse)
{
	DEBUGL6("Stage2 : __user__setAllDepartment Entry\n");
	OffendingParamsList   vOffendingParamsList;
	try
	{
		BOProxyRef boProxy = CCommonFunctions::getBOProxyRef(soap);
		if(!boProxy)
			CErrorCodeMapper::GenerateException("STG2_INVALID_SESSION");
		
		if(!user__setAllDepartment || !user__setAllDepartmentResponse || !user__setAllDepartment->DepartmentList)
			CErrorCodeMapper::GenerateException("STG2_MISSING_PARAM");
		
		vector<user__departmentType * > departmentTypes = user__setAllDepartment->DepartmentList->Department;		
		
		vector<user__departmentType * >::const_iterator itDeptType = departmentTypes.begin();
		vector<user__departmentType * >::const_iterator itDeptTypeEnd = departmentTypes.end();
	
		for(; itDeptType != itDeptTypeEnd; ++itDeptType)
		{
			user__departmentType *Department = (*itDeptType);
			if(!Department || !Department->DepartmentInfo || Department->DepartmentInfo->Code.empty() || Department->DepartmentInfo->Name.empty())
			{
				CErrorCodeMapper::GenerateException("STG2_MISSING_PARAM");
			}
			if(Department->ConfigurationInfo  && Department->ConfigurationInfo->Quota && Department->ConfigurationInfo->Quota->Job)
			{
				if(!Department->ConfigurationInfo->Quota->Job->Print)
					CErrorCodeMapper::GenerateException("STG2_MISSING_PARAM");
				if(Department->ConfigurationInfo->Quota->Job->Print->__union_printQuotaCategoryType == SOAP_UNION__common__union_printQuotaCategoryType_ColorJobQuota)
				{
					if(!Department->ConfigurationInfo->Quota->Job->Print->union_printQuotaCategoryType.ColorJobQuota || !Department->ConfigurationInfo->Quota->Job->Print->union_printQuotaCategoryType.ColorJobQuota->ColorQuota || !Department->ConfigurationInfo->Quota->Job->Print->union_printQuotaCategoryType.ColorJobQuota->BlackQuota)
						CErrorCodeMapper::GenerateException("STG2_MISSING_PARAM");
				}
			}
		}



		//get the existing department id's
		vector<CString> vExistingDeptIds;
		map<CString,CString> vExistingDeptMap;
		UserServices::getDepartmentKey(boProxy,vExistingDeptIds,vExistingDeptMap);
		
		vector<CString> vDeletableDeptIds = vExistingDeptIds;
		DEBUGL8("Stage2::__user__setAllDepartment size of vExistingDeptIds  = %d ,vDeletableDeptIds  = %d \n",int(vExistingDeptIds.size()),int(vDeletableDeptIds.size()));
		std::vector<user__departmentType * > deptsToCreate;
		std::vector<user__departmentType * > deptsToEdit;
		
		std::vector<user__departmentType * >::iterator ipDeptIter = user__setAllDepartment->DepartmentList->Department.begin();
		for(;ipDeptIter < user__setAllDepartment->DepartmentList->Department.end(); ipDeptIter++)
		{
			if((*ipDeptIter)->DepartmentInfo->DepartmentID) // check if dept id is supplied if not we need to create it
			{
				if(*((*ipDeptIter)->DepartmentInfo->DepartmentID) != "") // if dept id is empty we need to create it
				{
					vector<CString>::iterator itBeginExistingID = vExistingDeptIds.begin();
					vector<CString>::iterator itEndExistingID = vExistingDeptIds.end();
					if(itEndExistingID == find(itBeginExistingID,itEndExistingID, *((*ipDeptIter)->DepartmentInfo->DepartmentID)))
					{
						DEBUGL8("Stage2::__user__setAllDepartment the dept id that will be CREATED :: %s \n",(*((*ipDeptIter)->DepartmentInfo->DepartmentID)).c_str());
						deptsToCreate.push_back(*ipDeptIter); // if dept id is not found in the existing id list it needs to be created
					}
					else
					{
						DEBUGL8("Stage2::__user__setAllDepartment the dept id that will be EDITED :: %s \n",(*((*ipDeptIter)->DepartmentInfo->DepartmentID)).c_str());
						deptsToEdit.push_back(*ipDeptIter); // since dept id mentioned exists it will be edited
						vector<CString>::iterator itBeginDeletableID = vDeletableDeptIds.begin();
						vector<CString>::iterator itEndDeletableID  = vDeletableDeptIds.end();
						vector<CString>::iterator itFoundDeletableID = find(itBeginDeletableID,itEndDeletableID, *((*ipDeptIter)->DepartmentInfo->DepartmentID));
						if(itEndDeletableID != itFoundDeletableID)
						{
							//remove the id's which will be edited from the deletable id's vector so that it retains only the deltable ones
							vDeletableDeptIds.erase(itFoundDeletableID);
							DEBUGL8("Stage2::__user__setAllDepartment the dept id that will be NOT be DELETED :: %s size of vDeletableDeptIds : %d \n",(*itFoundDeletableID).c_str(),int(vDeletableDeptIds.size()));
							
						}
					}
				}
				else 
					deptsToCreate.push_back(*ipDeptIter);
			}
			else 
				deptsToCreate.push_back(*ipDeptIter);
		}
		
		if(deptsToEdit.empty() == false) // modify the existing depts
		{	
			DEBUGL8("Stage2::__user__setAllDepartmentediting departments enter\n");
			if(UserServices::setDepartments(soap,deptsToEdit, "ModifyBillingCode",vOffendingParamsList,"ALL") != STATUS_OK)
				DEBUGL1("Stage2::__user__setAllDepartment could not modify all departments \n");
		}
		if(deptsToCreate.empty() == false) // create the un-existing depts
		{	
			DEBUGL8("Create departments enter\n");
			if(UserServices::setDepartments(soap,deptsToCreate, "RegisterBillingCode",vOffendingParamsList,"ALL") != STATUS_OK)
				DEBUGL1("Stage2::__user__setAllDepartment could not create all departments \n");
		}
		
		if(vDeletableDeptIds.empty() == false) //delete the unsupplied dept ids
		{
			OffendingParamsStruct offendingParamInfo;
			for(vector<CString>::iterator itr = vDeletableDeptIds.begin() ; itr< vDeletableDeptIds.end();itr++)
			{
				map<CString,CString>::iterator mItr = vExistingDeptMap.find(*itr);
				if(mItr == vExistingDeptMap.end())
					continue;
				CString sResult = UserServices::ExecuteDeleteDepartment(mItr->second,boProxy);
				if (STG2_SUCCESS !=  sResult) 
				{
					if(itr == vDeletableDeptIds.begin())
						CErrorCodeMapper::GenerateException(sResult, ACC_MGR);
					else
					{
						offendingParamInfo.Name    = "failed to delete dept";
						offendingParamInfo.Value = mItr->second;
						vOffendingParamsList.push_back(offendingParamInfo);
						CErrorCodeMapper::GenerateException("STG2_DEPT_PARTIAL_SUCCESS",vOffendingParamsList);
					}
				}	
			}
			
		}

		user__setAllDepartmentResponse->Result = common__operationResultType__OperationSucceeded ;
	}
	catch(CStg2Exception & ex)
	{
		DEBUGL1("Stage2::__user__setAllDepartment caught Stg2Exception Exception. Description = %s\n", ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), ex.getFaultType(), ex.getErrorType(),vOffendingParamsList) != STATUS_OK)
		DEBUGL4("__user__setAllDepartment CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}
	catch(CException & ex)
	{
		DEBUGL1("Stage2::__user__setAllDepartment caught CException Exception. Description = %s\n", ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), "", eReceiverError) != STATUS_OK)
		DEBUGL4("__user__setAllDepartment CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}
	
	DEBUGL6("Stage2 : __user__setAllDepartment Exit\n");
	return SOAP_OK;
}
#endif
CString UserServices::ExecuteDeleteDepartment(CString sDepartmentCode,BOProxyRef &boProxy)
{
	DEBUGL8("UserServices::ExecuteDeleteDepartmentById enter DEPTID DELTETED ++%s++ \n",sDepartmentCode.c_str());
		
	std::vector<CString> xPaths;
	std::vector<CString> values;

	values.push_back(sDepartmentCode);
	xPaths.push_back("Accounting/DeleteBillingCode/BillingCodes/BillingCode/code");

	DEBUGL8("ExecuteDeleteDepartmentById : calling SetXPathValues\n");
	CUIControllerAdapter::SetXPathValues(xPaths, values, boProxy);

	commandParam p;
	p.strParamName = "billingCodeList";
	p.strParamValue = "Accounting/DeleteBillingCode/BillingCodes/BillingCode";
	p.mapAttributes.insert(make_pair("contentType","XPath"));

	vector<commandParam> vectParams;
	vectParams.push_back(p);
	CString strResponse;	
	
	if(STATUS_OK != CUIControllerAdapter::SendCommand("DeleteBillingCode", "Accounting/BillingCodes", vectParams, strResponse, boProxy))
	{
		DEBUGL1("UserServices::ExecuteDeleteDepartment SendCommand failed\n");
		CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");	
	}


	return CUIControllerAdapter::GetOperationStatusString(strResponse);
	
}


#ifdef STAGE2_UNSUPPORTED
SOAP_FMAC5 int SOAP_FMAC6 __user__getAllResourceLimit(struct soap* pSOAP, _user__getAllResourceLimit *user__getAllResourceLimit, _user__getAllResourceLimitResponse *user__getAllResourceLimitResponse)
{
	CCommonFunctions::GenerateSoapFault(pSOAP, "Operation not supported", "ERR_EBS_SERVER_OPERATION_UNSUPPORTED", eReceiverError);
	return SOAP_FAULT;
}


SOAP_FMAC5 int SOAP_FMAC6 __user__setAllResourceLimit(struct soap* pSOAP, _user__setAllResourceLimit *user__setAllResourceLimit, _user__setAllResourceLimitResponse *user__setAllResourceLimitResponse)
{
	CCommonFunctions::GenerateSoapFault(pSOAP, "Operation not supported", "ERR_EBS_SERVER_OPERATION_UNSUPPORTED", eReceiverError);
	return SOAP_FAULT;
}


SOAP_FMAC5 int SOAP_FMAC6 __user__getResourceLimit(struct soap* pSOAP, _user__getResourceLimit *user__getResourceLimit, _user__getResourceLimitResponse *user__getResourceLimitResponse)
{
	CCommonFunctions::GenerateSoapFault(pSOAP, "Operation not supported", "ERR_EBS_SERVER_OPERATION_UNSUPPORTED", eReceiverError);
	return SOAP_FAULT;
}


SOAP_FMAC5 int SOAP_FMAC6 __user__setResourceLimit(struct soap* pSOAP, _user__setResourceLimit *user__setResourceLimit, _user__setResourceLimitResponse *user__setResourceLimitResponse)
{
	CCommonFunctions::GenerateSoapFault(pSOAP, "Operation not supported", "ERR_EBS_SERVER_OPERATION_UNSUPPORTED", eReceiverError);
	return SOAP_FAULT;
}

SOAP_FMAC5 int SOAP_FMAC6 __user__getDepartment(struct soap* soap, _user__getDepartment *user__getDepartment, _user__getDepartmentResponse *user__getDepartmentResponse)
{
	DEBUGL8("Stage2::__user__getDepartment entry\n");
	try
	{
		BOProxyRef boProxy = CCommonFunctions::getBOProxyRef(soap);
		if(!boProxy)
			CErrorCodeMapper::GenerateException("STG2_INVALID_SESSION");
		
		if(!user__getDepartment || !user__getDepartmentResponse)
			CErrorCodeMapper::GenerateException("STG2_MISSING_PARAM");

		if(user__getDepartment->Code.empty())
			CErrorCodeMapper::GenerateException("STG2_EMPTY_DEPT_CODE");

		vector<user__departmentType * > departmentTypes;		
		if(UserServices::getDepartments(soap, departmentTypes, user__getDepartment->Code) != STATUS_OK)
			CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");

		user__getDepartmentResponse->Department = static_cast<user__departmentType*>(departmentTypes.front());

	}
	catch(CStg2Exception & ex)
	{
		DEBUGL1("Stage2::__user__getDepartment caught Stg2Exception Exception. Description = %s\n", ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), ex.getFaultType(), ex.getErrorType()) != STATUS_OK)
		DEBUGL4("__user__getDepartment CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}
	catch(CException & ex)
	{
		DEBUGL1("Stage2::__user__getDepartment caught CException Exception. Description = %s\n", ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), "", eReceiverError) != STATUS_OK)
		DEBUGL4("__user__getDepartment CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}
	DEBUGL8("Stage2::__user__getDepartment exit\n");
	return SOAP_OK;

}


SOAP_FMAC5 int SOAP_FMAC6 __user__setDepartment(struct soap* soap, _user__setDepartment *user__setDepartment, _user__setDepartmentResponse *user__setDepartmentResponse)
{
	DEBUGL8("Stage2::__user__setDepartment entry\n");

	try
	{
		BOProxyRef boProxy = CCommonFunctions::getBOProxyRef(soap);
		if(!boProxy)
			CErrorCodeMapper::GenerateException("STG2_INVALID_SESSION");
		
		if(!user__setDepartment || !user__setDepartmentResponse || !user__setDepartment->Department)
			CErrorCodeMapper::GenerateException("STG2_MISSING_PARAM");

		vector<user__departmentType * > departmentTypes;		
		departmentTypes.push_back(user__setDepartment->Department);

		vector<user__departmentType * >::const_iterator itDeptType = departmentTypes.begin();
		vector<user__departmentType * >::const_iterator itDeptTypeEnd = departmentTypes.end();
	
		for(; itDeptType != itDeptTypeEnd; ++itDeptType)
		{
			user__departmentType *Department = (*itDeptType);
			if(!Department || !Department->DepartmentInfo || Department->DepartmentInfo->Code.empty() || Department->DepartmentInfo->Name.empty())
			{
				CErrorCodeMapper::GenerateException("STG2_MISSING_PARAM");
			}
			if(Department->ConfigurationInfo  && Department->ConfigurationInfo->Quota && Department->ConfigurationInfo->Quota->Job)
			{
				if(!Department->ConfigurationInfo->Quota->Job->Print)
					CErrorCodeMapper::GenerateException("STG2_MISSING_PARAM");
				if(Department->ConfigurationInfo->Quota->Job->Print->__union_printQuotaCategoryType == SOAP_UNION__common__union_printQuotaCategoryType_ColorJobQuota)
				{
					if(!Department->ConfigurationInfo->Quota->Job->Print->union_printQuotaCategoryType.ColorJobQuota || !Department->ConfigurationInfo->Quota->Job->Print->union_printQuotaCategoryType.ColorJobQuota->ColorQuota || !Department->ConfigurationInfo->Quota->Job->Print->union_printQuotaCategoryType.ColorJobQuota->BlackQuota)
						CErrorCodeMapper::GenerateException("STG2_MISSING_PARAM");
				}
			}
		}


		OffendingParamsList   vOffendingParamsList;
		if(UserServices::setDepartments(soap, departmentTypes, "ModifyBillingCode",vOffendingParamsList) != STATUS_OK)
			CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");

		user__setDepartmentResponse->Result = common__operationResultType__OperationSucceeded;

	}
	catch(CStg2Exception & ex)
	{
		DEBUGL1("Stage2::__user__setDepartment caught Stg2Exception Exception. Description = %s\n", ex.C_Str());
		user__setDepartmentResponse->Result = common__operationResultType__OperationFailed;
		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), ex.getFaultType(), ex.getErrorType()) != STATUS_OK)
		DEBUGL4("__user__setDepartment CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}
	catch(CException & ex)
	{
		DEBUGL1("Stage2::__user__setDepartment caught CException Exception. Description = %s\n", ex.C_Str());
		user__setDepartmentResponse->Result = common__operationResultType__OperationFailed;
		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), "", eReceiverError) != STATUS_OK)
		DEBUGL4("__user__setDepartment CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}
	DEBUGL8("Stage2::__user__setDepartment exit\n");
	return SOAP_OK;
}
#endif
SOAP_FMAC5 int SOAP_FMAC6 __user__keepSessionAlive(struct soap* soap, _userauth__keepSessionAlive *userauth__keepSessionAlive, _userauth__keepSessionAliveResponse *userauth__keepSessionAliveResponse)
{
	try
	{
		DEBUGL8("Stage2::__user__keepSessionAlive entry\n");
		
		BOProxyRef pBOProxy = CCommonFunctions::getBOProxyRef(soap);
		if(!pBOProxy)
		{
			DEBUGL1("Stage2::__user__keepSessionAlive Invalid Session\n");
			CErrorCodeMapper::GenerateException("STG2_INVALID_SESSION");
		}
		
		if(!userauth__keepSessionAlive)
		{
			CErrorCodeMapper::GenerateException("STG2_MISSING_PARAM");
		}
		// Although getBOProxyRef internally calls 	UpdateLastActivity, We are explicitly doing it below.
		// This doesn't need to be done but doing so is harmless.	
		
		CString strSession = soap->header->common__sessionHeader->Token;
		DEBUGL8("Stage2::__user__keepSessionAlive : session name provided in header is %s\n", strSession.c_str());

		userauth__keepSessionAliveResponse->Result = common__operationResultType__OperationSucceeded;		

		SessionMgrRef sessionMgrRef = CSessionMgr::Acquire();
		if(STATUS_OK != sessionMgrRef->UpdateLastActivity(strSession))
		{
			DEBUGL2("Stage2::__user__keepSessionAlive updateLastActivity failed\n");
			CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");
		}
	
		DEBUGL8("Stage2::__user__keepSessionAlive exit\n");
	}
	catch(CStg2Exception &ex)
	{
		DEBUGL1("Stage2::__user__keepSessionAlive:Stg2Exception:Description = %s\n", ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), ex.getFaultType(), ex.getErrorType()) != STATUS_OK)
			DEBUGL1("__user__keepSessionAlive CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}
	catch(CException &ex)
	{
		DEBUGL1("Stage2::__user__keepSessionAlive:CException:Description = %s\n",ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap,ex.C_Str(),"",eReceiverError) != STATUS_OK)
			DEBUGL1("__user__keepSessionAlive CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}	
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __stg2__keepSessionAlive(struct soap* soap, _userauth__keepSessionAlive *userauth__keepSessionAlive, _userauth__keepSessionAliveResponse *userauth__keepSessionAliveResponse)
{
	if(!CCommonFunctions::IsOutputManagementEnabled( ))
	{
		stage2ErrorStruct stError;
		CErrorCodeMapper::MapInternalErrCodeToStage2Fault(stError, "STG2_UNSUPPORTED_OPERATION");
		CCommonFunctions::GenerateSoapFault(soap, stError.strDescription, stError.strErrCode, stError.eErrorType);
		return SOAP_FAULT;
	}
	return __user__keepSessionAlive(soap, userauth__keepSessionAlive, userauth__keepSessionAliveResponse);
}
SOAP_FMAC5 int SOAP_FMAC6 __stg2__createDepartment(struct soap* soap, _user__createDepartment *user__createDepartment, _user__createDepartmentResponse *user__createDepartmentResponse)
{
	DEBUGL8("Stage2::__stg2__createDepartment entry\n");
	if(!CCommonFunctions::IsOutputManagementEnabled( ))
	{
		stage2ErrorStruct stError;
		CErrorCodeMapper::MapInternalErrCodeToStage2Fault(stError, "STG2_UNSUPPORTED_OPERATION");
		CCommonFunctions::GenerateSoapFault(soap, stError.strDescription, stError.strErrCode, stError.eErrorType);
		return SOAP_FAULT;
	}
	return __user__createDepartment(soap,user__createDepartment,user__createDepartmentResponse);

}


CString UserServices::CreateDepartment(const Ref<BOProxy>& pBOProxy, common__departmentInfoType* pDepartmentInfo)
{
	DEBUGL8("UserServices::CreateDepartment Enter\n");
	
	CUIControllerAdapter::SetXMLValue("", "<Accounting></Accounting>" , pBOProxy); // to clear delta document.
	const CString strBillingCodeXPath = "Accounting/BillingCodes/BillingCode";
	
	vector<CString> vXPath;
	vector<CString> vValue;
	
	CString sDepartmentCode = pDepartmentInfo->Code;
	vXPath.push_back(strBillingCodeXPath + "/code");
	vValue.push_back(sDepartmentCode);
	
	CString sDepartmentName = pDepartmentInfo->Name;
	vValue.push_back(sDepartmentName);
	vXPath.push_back(strBillingCodeXPath + "/name");
	
	CUIControllerAdapter::SetXPathValues(vXPath, vValue, pBOProxy);
	
	// Send the command
	commandParam param;
	param.strParamName = "billingCodeDetails";
	param.strParamValue = strBillingCodeXPath;
	param.mapAttributes.insert(make_pair("contentType","XPath"));
	vector<commandParam> vectParams;
	vectParams.push_back(param);
	CString sResponse;	

	if(STATUS_OK != CUIControllerAdapter::SendCommand("RegisterBillingCode", "Accounting/BillingCodes", vectParams, sResponse, pBOProxy))
	{
		DEBUGL1("Failed to SendCommand\n");
		CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");
	}

	CString sOperationStatus = CUIControllerAdapter::GetOperationStatusString(sResponse);
	DEBUGL8("UserServices::CreateDepartment Exit\n");
	return sOperationStatus;
}

SOAP_FMAC5 int SOAP_FMAC6 __user__createDepartment(struct soap* soap, _user__createDepartment *user__createDepartment, _user__createDepartmentResponse *user__createDepartmentResponse)
{
	try
	{
		DEBUGL8("__user__createDepartment entry\n");
		BOProxyRef pBOProxy = CCommonFunctions::getBOProxyRef(soap);
		if(!pBOProxy)
		{
			DEBUGL1("__user__createDepartment Invalid Session\n");
			CErrorCodeMapper::GenerateException("STG2_INVALID_SESSION");
		}
		
		if(!user__createDepartment || !user__createDepartment->DepartmentInfo)
		{
			CErrorCodeMapper::GenerateException("STG2_MISSING_PARAM");
		}
				
		if(user__createDepartment->DepartmentInfo->Code.empty())
		{
			DEBUGL1("user__createDepartment->DepartmentInfo->Code is empty\n");
			CErrorCodeMapper::GenerateException("STG2_EMPTY_DEPT_CODE");
		}
		
		if(user__createDepartment->DepartmentInfo->Name.empty())
		{
			DEBUGL1("user__createDepartment->DepartmentInfo->Name is empty\n");
			CErrorCodeMapper::GenerateException("STG2_EMPTY_DEPT_NAME");
		}
		
		user__createDepartmentResponse->Result = common__operationResultType__OperationSucceeded;
		
		CString sOperationStatus = UserServices::CreateDepartment(pBOProxy, user__createDepartment->DepartmentInfo);
		if(STG2_SUCCESS != sOperationStatus)
		{
			DEBUGL1("UserServices::CreateDepartment failed\n");
			CErrorCodeMapper::GenerateException(sOperationStatus,ACC_MGR);
		
		}
	}
	catch(CStg2Exception & ex)
	{
      DEBUGL1("Stage2::____user__createDepartment:Stg2Exception: Description = %s\n", ex.C_Str());
		user__createDepartmentResponse->Result = common__operationResultType__OperationFailed;
		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), ex.getFaultType(), ex.getErrorType()) != STATUS_OK)
		DEBUGL4("____user__createDepartment CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}
	catch(CException & ex)
	{
      DEBUGL1("Stage2::____user__createDepartmen:CException:Description = %s\n", ex.C_Str());
		user__createDepartmentResponse->Result = common__operationResultType__OperationFailed;
		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), "", eReceiverError) != STATUS_OK)
		DEBUGL4("____user__createDepartment CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}
	DEBUGL8("Stage2::__user__createDepartment exit\n");
	return SOAP_OK;
}


SOAP_FMAC5 int SOAP_FMAC6 __stg2__deleteDepartment(struct soap* soap, _user__deleteDepartment *user__deleteDepartment, _user__deleteDepartmentResponse *user__deleteDepartmentResponse)
{
	DEBUGL8("Stage2::__stg2__deleteDepartment entry\n");
	if(!CCommonFunctions::IsOutputManagementEnabled( ))
	{
		stage2ErrorStruct stError;
		CErrorCodeMapper::MapInternalErrCodeToStage2Fault(stError, "STG2_UNSUPPORTED_OPERATION");
		CCommonFunctions::GenerateSoapFault(soap, stError.strDescription, stError.strErrCode, stError.eErrorType);
		return SOAP_FAULT;
	}
	return __user__deleteDepartment(soap,user__deleteDepartment,user__deleteDepartmentResponse);

}


SOAP_FMAC5 int SOAP_FMAC6 __user__deleteDepartment(struct soap* soap, _user__deleteDepartment *user__deleteDepartment, _user__deleteDepartmentResponse *user__deleteDepartmentResponse)
{
	try
	{
		DEBUGL8("Stage2::__user__deleteDepartment entry\n");
		BOProxyRef boProxy = CCommonFunctions::getBOProxyRef(soap);
		if(!boProxy)
		{
			DEBUGL1("__user__deleteDepartment Invalid Session\n");
			CErrorCodeMapper::GenerateException("STG2_INVALID_SESSION");
		}
		if(!user__deleteDepartment)
		{
			CErrorCodeMapper::GenerateException("STG2_MISSING_PARAM");
		}
		
		CString sDepartmentCode = user__deleteDepartment->Code;
		if(sDepartmentCode.empty())
		{
			DEBUGL1("DepartmentCode is empty\n");
			CErrorCodeMapper::GenerateException("STG2_EMPTY_DEPT_CODE");
		}
				
		user__deleteDepartmentResponse->Result = common__operationResultType__OperationSucceeded;
		
		CString sOperationStatus = UserServices::ExecuteDeleteDepartment( sDepartmentCode , boProxy);
		if(STG2_SUCCESS != sOperationStatus)
		{
			DEBUGL1("UserServices::ExecuteDeleteDepartment Operation failed\n");
			CErrorCodeMapper::GenerateException(sOperationStatus, ACC_MGR);
		}		
	}
	
	catch(CStg2Exception & ex)
	{
		DEBUGL1("Stage2::__user__deleteDepartment caught Stg2Exception Exception. Description = %s\n", ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), ex.getFaultType(), ex.getErrorType()) != STATUS_OK)
		DEBUGL4("__user__deleteDepartment CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}
	catch(CException & ex)
	{
		DEBUGL1("Stage2::__user__deleteDepartment caught CException Exception. Description = %s\n", ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), "", eReceiverError) != STATUS_OK)
		DEBUGL4("__user__deleteDepartment CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}
	
	DEBUGL8("Stage2::__user__deleteDepartment exit\n");
	return SOAP_OK;
}

#ifdef STAGE2_UNSUPPORTED
SOAP_FMAC5 int SOAP_FMAC6 __user__deleteAllDepartment(struct soap* soap, _user__deleteAllDepartment *user__deleteAllDepartment, _user__deleteAllDepartmentResponse *user__deleteAllDepartmentResponse)
{

	try
	{
		DEBUGL8("Stage2::__user__deleteAllDepartment entry\n");
		
		BOProxyRef boProxy = CCommonFunctions::getBOProxyRef(soap);
		if(!boProxy)
		{
			CErrorCodeMapper::GenerateException("STG2_INVALID_SESSION");
		}

		if(!user__deleteAllDepartment || !user__deleteAllDepartmentResponse)
		{
			CErrorCodeMapper::GenerateException("STG2_MISSING_PARAM");
		}

		
		vector<commandParam> vectParams;
		CString strResponse;	
		
		Status status = CUIControllerAdapter::SendCommand("DeleteBillingCode", "Accounting/BillingCodes", vectParams, strResponse, boProxy);

		// Get the result
		CString execCommandStatus = CUIControllerAdapter::GetOperationStatusString(strResponse);
		DEBUGL8("__user__deleteAllDepartment : SendCommand status=%d response=%s response status of operation=%d\n", status, strResponse.c_str(), execCommandStatus.c_str());

		if(status == STATUS_OK && execCommandStatus == STG2_SUCCESS)
			user__deleteAllDepartmentResponse->Result = common__operationResultType__OperationSucceeded;
		else
		{
			user__deleteAllDepartmentResponse->Result = common__operationResultType__OperationFailed;
			CErrorCodeMapper::GenerateException(execCommandStatus, ACC_MGR);
		}
	}
	catch(CStg2Exception & ex)
	{
		DEBUGL1("Stage2::__user__deleteAllDepartment caught Stg2Exception Exception. Description = %s\n", ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), ex.getFaultType(), ex.getErrorType()) != STATUS_OK)
		DEBUGL4("__user__deleteAllDepartment CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}
	catch(CException & ex)
	{
		DEBUGL1("Stage2::__user__deleteAllDepartment caught CException Exception. Description = %s\n", ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), "", eReceiverError) != STATUS_OK)
		DEBUGL4("__user__deleteAllDepartment CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}

	return SOAP_OK;
}
#endif

user__departmentInfoListType* UserServices::getDepartmentInfo(soap* soap, const Ref<BOProxy>& pBOProxy, vector<string >& vCode)
{
	DepartmentFinderRef finder = DepartmentFinder::Create(pBOProxy->getSessionID());
	user__departmentInfoListType* pDepartmentInfoList = soap_new_user__departmentInfoListType(soap, -1);
	if(vCode.empty() == false)
	{
		for(vector<string>::iterator iter = vCode.begin(); iter != vCode.end(); ++iter)
		{	
			if(iter->empty())
			{
				CString errorCode = vCode.size() == 1 ? "STG2_EMPTY_DEPT_CODE" : "STG2_MULTIPLE_EMPTY_DEPT_CODE";
				CErrorCodeMapper::GenerateException(errorCode);
			}
			DepartmentRef department;
			Status retStatus = finder->GetDepartmentByCode(*iter, CounterQuotaSearchOptions_None, department);
			if (retStatus == STATUS_AL_SEC_PERMISSION_CHECK_ERROR) // #include "AL/status.h"
			{
				DEBUGL2( "UserServices::getDepartmentInfo ** Permission check error.\n" );
			  	CErrorCodeMapper::GenerateException("STATUS_PERMISSION_CHECK_ERROR");
			}
 			if (retStatus != STATUS_OK || !department)
			{
			   	DEBUGL2("UserServices::getDepartmentInfo The code %s is invalid\n",(*iter).c_str() );
				CString errorCode = vCode.size() == 1 ? "STG2_DEPT_DOESNT_EXIST" : "STG2_MULTI_DEPT_DOESNT_EXIST";
				CErrorCodeMapper::GenerateException(errorCode);
			}
			common__departmentInfoType * pDepartmentInfo = soap_new_common__departmentInfoType(soap, -1);
			pDepartmentInfo->Code =  department->GetCode();
			pDepartmentInfo->Name = department->GetName();
			pDepartmentInfo->DepartmentID = soap_new_std__string(soap, -1);
			*(pDepartmentInfo->DepartmentID) = string_cast(department->GetID());
			pDepartmentInfoList->DepartmentInfo.push_back(pDepartmentInfo);
		}
	}
	else
	{
		DepartmentListRef departments;
		Status retStatus = finder->GetAllDepartments(CounterQuotaSearchOptions_None, departments);
		if (retStatus == STATUS_AL_SEC_PERMISSION_CHECK_ERROR) 
		{
			DEBUGL2( "UserServices::getDepartmentInfo ** Permission check error.\n" );
		  	CErrorCodeMapper::GenerateException("STATUS_PERMISSION_CHECK_ERROR");
		}
		if (retStatus != STATUS_OK)
		{
			DEBUGL2( "UserServices::getDepartmentInfo ** Operation failed.\n" );
	  		CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");
		}
		for (DepartmentList::const_iterator it = departments->begin(); it != departments->end(); it++)
		{
			common__departmentInfoType * pDepartmentInfo = soap_new_common__departmentInfoType(soap, -1);
			DepartmentRef department = *it;
			pDepartmentInfo->Code =  department->GetCode();
			pDepartmentInfo->Name = department->GetName();
			pDepartmentInfo->DepartmentID = soap_new_std__string(soap, -1);
			*(pDepartmentInfo->DepartmentID) = string_cast(department->GetID());
			pDepartmentInfoList->DepartmentInfo.push_back(pDepartmentInfo);
		}
	}
 	return pDepartmentInfoList;
}

SOAP_FMAC5 int SOAP_FMAC6 __user__getDepartmentInfo(struct soap* soap, _user__getDepartmentInfo *user__getDepartmentInfo, _user__getDepartmentInfoResponse *user__getDepartmentInfoResponse)
{
	DEBUGL1("__user__getDepartmentInfo entry\n");
	CString        operationName     = "getDepartmentInfo";
	try
	{
		
		 //Check if we have a conflicting operation in progress
		if(CLongRunningOpHelper::IsAnotherConflictingOperationInProgress(soap, operationName))
		{
			// Don't use GenerateException since it will update Running operation status to "false" which we don't want.
			stage2ErrorStruct stError;
			CErrorCodeMapper::MapInternalErrCodeToStage2Fault(stError, "STG2_CONF_OP_INPROGRESS");
			CCommonFunctions::GenerateSoapFault(soap, stError.strDescription, stError.strErrCode, stError.eErrorType);
			return SOAP_FAULT;	
		}
		else
		{
			//Add this operation to the current running op list
			CLongRunningOpHelper::UpdateRunningOpStatus(soap, operationName, true);
		}
		
		BOProxyRef pBOProxy = CCommonFunctions::getBOProxyRef(soap);
		if(!pBOProxy)
		{
			DEBUGL1("__user__getDepartmentInfo Invalid Session\n");
			CErrorCodeMapper::GenerateException("STG2_INVALID_SESSION");
		}
		
		if(!user__getDepartmentInfo)
		{
			CErrorCodeMapper::GenerateException("STG2_MISSING_PARAM");
		}

		user__getDepartmentInfoResponse->DepartmentInfoList = UserServices::getDepartmentInfo(soap, pBOProxy, user__getDepartmentInfo->Code);
			
		//Remove this operation from the current running op list
		CLongRunningOpHelper::UpdateRunningOpStatus(soap, operationName, false);	
		
	}
	catch(CStg2Exception & ex)
	{
		DEBUGL1("Stage2::__user__getDepartmentInfo caught Stg2Exception Exception. Description = %s\n", ex.C_Str());
		CLongRunningOpHelper::UpdateRunningOpStatus(soap, operationName, false);
		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), ex.getFaultType(), ex.getErrorType()) != STATUS_OK)
		DEBUGL4("__user__getDepartment CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}
	catch(CException & ex)
	{
		DEBUGL1("Stage2::__user__getDepartmentInfo caught CException Exception. Description = %s\n", ex.C_Str());
		CLongRunningOpHelper::UpdateRunningOpStatus(soap, operationName, false);
		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), "", eReceiverError) != STATUS_OK)
		DEBUGL4("__user__getDepartment CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}

	DEBUGL1("Stage2::__user__getDepartmentInfo exit\n");
	return SOAP_OK;
}

CString UserServices::ExecuteModifyBillingCode(const Ref<BOProxy>& pBOProxy, CString sBillingCodeXPath)
{
	DEBUGL8("UserServices::ExecuteModifyBillingCode enter\n");
	commandParam p;
	p.strParamName = "billingCodeDetails";
	p.strParamValue = sBillingCodeXPath;
	p.mapAttributes.insert(make_pair("contentType","XPath"));
	vector<commandParam> vParam;
	vParam.push_back(p);
	
	CString sResponse;
	if(STATUS_OK != CUIControllerAdapter::SendCommand("ModifyBillingCode", "Accounting/BillingCodes", vParam, sResponse, pBOProxy))
	{
		DEBUGL1("Failed to SendCommand\n");
		CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");
	}
	
	CString sOperationStatus =  CUIControllerAdapter::GetOperationStatusString(sResponse);
	DEBUGL8("UserServices::ExecuteModifyBillingCode exit\n");
	return sOperationStatus;
}


/* CString UserServices::setDepartmentInfoDeltaDoc(const Ref<BOProxy>& pBOProxy, common__departmentInfoType* pDepartmentInfo, CString sXPath)
{

	if(pDepartmentInfo->Code.empty())
	{
		DEBUGL1("Code is empty\n");
		return ("STG2_EMPTY_DEPT_CODE");
	}
	
	if(pDepartmentInfo->Name.empty())
	{
		DEBUGL1("Name is empty\n");
		return ("STG2_EMPTY_DEPT_NAME");
	}
	
	
	CUIControllerAdapter::SetXMLValue("", "<Accounting></Accounting>" , pBOProxy); // to clear delta document.
	vector<CString> vXPath;
	vector<CString> vValue;		
		
	if(pDepartmentInfo->DepartmentID)
	{
		CString sDepartmentID = *(pDepartmentInfo->DepartmentID);
		vXPath.push_back(sXPath + "/@ID");
		vValue.push_back(sDepartmentID);
	}
				
	CString sDepartmentCode = pDepartmentInfo->Code;
	vXPath.push_back(sXPath + "/code");
	vValue.push_back(sDepartmentCode);
	
	CString sDepartmentName = pDepartmentInfo->Name;
	vXPath.push_back(sXPath + "/name");
	vValue.push_back(sDepartmentName);
		
	CUIControllerAdapter::SetXPathValues(vXPath, vValue, pBOProxy);
	
	return STG2_SUCCESS;

} */

CString UserServices::setDepartmentInfo(const Ref<BOProxy>& pBOProxy, common__departmentInfoType* pDepartmentInfo)
{
	DEBUGL8("UserServices::setDepartmentInfo enter\n");
	
	CString strBillingCodeXPath = "Accounting/BillingCodes/BillingCode";
	
	vector<CString> vXPath;
	vector<CString> vValue;		
		
	if(pDepartmentInfo->DepartmentID)
	{
		CString sDepartmentID = *(pDepartmentInfo->DepartmentID);
		vXPath.push_back(strBillingCodeXPath + "/@ID");
		vValue.push_back(sDepartmentID);
	}
				
	CString sDepartmentCode = pDepartmentInfo->Code;
	vXPath.push_back(strBillingCodeXPath + "/code");
	vValue.push_back(sDepartmentCode);
	
	CString sDepartmentName = pDepartmentInfo->Name;
	vXPath.push_back(strBillingCodeXPath + "/name");
	vValue.push_back(sDepartmentName);
		
	CUIControllerAdapter::SetXPathValues(vXPath, vValue, pBOProxy);
		
	CString sOperationStatus = ExecuteModifyBillingCode(pBOProxy,strBillingCodeXPath);
	DEBUGL8("UserServices::setDepartmentInfo exit\n");
	return sOperationStatus;
}

Status UserServices::ValidateDepartmentInfoList(user__departmentInfoListType* pDepartmentInfoList , bool bValidateID)
{
	DEBUGL8("UserServices::ValidateDepartmentInfoList enter\n");
	vector<common__departmentInfoType* >::iterator iter = pDepartmentInfoList->DepartmentInfo.begin();
	vector<common__departmentInfoType* >::iterator iterEnd = pDepartmentInfoList->DepartmentInfo.end();
	
	for (; iter != iterEnd; iter++)
	{
		common__departmentInfoType* pDeptInfo = *iter;
	
		if(bValidateID)
		{
			if(!pDeptInfo->DepartmentID || pDeptInfo->DepartmentID->empty())
			{
				if(pDepartmentInfoList->DepartmentInfo.size() == 1)
				{
					CErrorCodeMapper::GenerateException("STG2_MISSING_DEPT_ID");
				}
				else
				{
					CErrorCodeMapper::GenerateException("STG2_MULTIPLE_MISSING_DEPT_ID");
				}
			}
		}
	
		if(pDeptInfo->Name.empty())
		{
			if(pDepartmentInfoList->DepartmentInfo.size() == 1) 
			{
				CErrorCodeMapper::GenerateException("STG2_EMPTY_DEPT_NAME");
			}
			else
			{
				CErrorCodeMapper::GenerateException("STG2_MULTIPLE_EMPTY_DEPT_NAME");
			}
		}

		if(pDeptInfo->Code.empty())
		{
			if(pDepartmentInfoList->DepartmentInfo.size() == 1)
			{
				CErrorCodeMapper::GenerateException("STG2_EMPTY_DEPT_CODE");
			}
			else
			{
				CErrorCodeMapper::GenerateException("STG2_MULTIPLE_EMPTY_DEPT_CODE");
			}
        }
		
	}
	
	DEBUGL8("UserServices::ValidateDepartmentInfoList exit.\n");

   return STATUS_OK;
}


Status UserServices::setDepartmentInfoList(const Ref<BOProxy>& pBOProxy, user__departmentInfoListType* pDepartmentInfoList)
{
	DEBUGL8("UserServices::setDepartmentInfoList enter\n");
	if(pDepartmentInfoList)
	{
		vector<common__departmentInfoType* >::iterator iter = pDepartmentInfoList->DepartmentInfo.begin();
		vector<common__departmentInfoType* >::iterator iterEnd = pDepartmentInfoList->DepartmentInfo.end();
		
		for (; iter != iterEnd ; ++iter)
		{
			common__departmentInfoType* pDeptInfo = *iter;	
			CUIControllerAdapter::SetXMLValue("", "<Accounting></Accounting>" , pBOProxy); // to clear delta document.
			CString sOperationStatus = setDepartmentInfo(pBOProxy, pDeptInfo);
			
			if(STG2_SUCCESS != sOperationStatus)
			{
				if(pDepartmentInfoList->DepartmentInfo.begin() == iter)
				{
					DEBUGL1("ExecuteModifyBillingCode operation failed\n");
					CErrorCodeMapper::GenerateException(sOperationStatus,ACC_MGR);
				}
				else
				{
					OffendingParamsList vOffendingParam;
					OffendingParamsStruct stOffendingParam;
					stOffendingParam.Name    = "DepartmentCode";
					stOffendingParam.Value   = pDeptInfo->Code;
					vOffendingParam.push_back(stOffendingParam);
					CErrorCodeMapper::GenerateException("STG2_DEPT_PARTIAL_SUCCESS",vOffendingParam);
				}
			}				
		}
	}

	DEBUGL8("UserServices::setDepartmentInfoList exit\n");
	return STATUS_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __user__setDepartmentInfo(struct soap* soap, _user__setDepartmentInfo *user__setDepartmentInfo, _user__setDepartmentInfoResponse *user__setDepartmentInfoResponse)
{
	DEBUGL8("__user__setDepartmentInfo entry\n");	
	CString        operationName     = "setDepartmentInfo";
	try
	{
		//Check if we have a conflicting operation in progress
		if(CLongRunningOpHelper::IsAnotherConflictingOperationInProgress(soap, operationName))
		{
			// Don't use GenerateException since it will update Running operation status to "false" which we don't want.
			stage2ErrorStruct stError;
			CErrorCodeMapper::MapInternalErrCodeToStage2Fault(stError, "STG2_CONF_OP_INPROGRESS");
			CCommonFunctions::GenerateSoapFault(soap, stError.strDescription, stError.strErrCode, stError.eErrorType);
			return SOAP_FAULT;	
		}
		else
		{
			//Add this operation to the current running op list
			CLongRunningOpHelper::UpdateRunningOpStatus(soap, operationName, true);
		}
			
		BOProxyRef pBOProxy = CCommonFunctions::getBOProxyRef(soap);
		if(!pBOProxy)
		{
			DEBUGL1("__user__setDepartmentInfo Invalid Session\n");
			CErrorCodeMapper::GenerateException("STG2_INVALID_SESSION");
		}

		if(!user__setDepartmentInfo || !user__setDepartmentInfo->DepartmentInfoList 
			|| user__setDepartmentInfo->DepartmentInfoList->DepartmentInfo.empty())
		{
			CErrorCodeMapper::GenerateException("STG2_MISSING_PARAM");
		}

		user__setDepartmentInfoResponse->Result = common__operationResultType__OperationSucceeded;
		if(STATUS_OK != UserServices::ValidateDepartmentInfoList(user__setDepartmentInfo->DepartmentInfoList, true))
		{
			CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");	
		}
		
		if(STATUS_OK != UserServices::setDepartmentInfoList(pBOProxy, user__setDepartmentInfo->DepartmentInfoList))
		{
			CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");	
		}
		
		 //Remove this operation from the current running op list
		CLongRunningOpHelper::UpdateRunningOpStatus(soap, operationName, false);	
		
	}
	catch(CStg2Exception & ex)
	{
		DEBUGL1("Stage2::__user__setDepartmentInfo caught Stg2Exception Exception. Description = %s\n", ex.C_Str());
		CLongRunningOpHelper::UpdateRunningOpStatus(soap, operationName, false);	
		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), ex.getFaultType(), ex.getErrorType(), ex.getOffendingList()) != STATUS_OK)
		DEBUGL4("__user__setDepartmentInfo CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}
	catch(CException & ex)
	{
		DEBUGL1("Stage2::__user__setDepartmentInfo caught CException Exception. Description = %s\n", ex.C_Str());
		CLongRunningOpHelper::UpdateRunningOpStatus(soap, operationName, false);	
		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), "", eReceiverError) != STATUS_OK)
		DEBUGL4("__user__setDepartmentInfo CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}

	DEBUGL8("Stage2::__user__setDepartmentInfo exit\n");
	return SOAP_OK;

}

#ifdef STAGE2_UNSUPPORTED
SOAP_FMAC5 int SOAP_FMAC6 __user__getAllGroup(struct soap* soap, _user__getAllGroup *user__getAllGroup, _user__getAllGroupResponse *user__getAllGroupResponse)
{
	DEBUGL8("Stage2::__user__getAllGroup entry\n");
	try
	{
		BOProxyRef pBOProxy = CCommonFunctions::getBOProxyRef(soap);
		if(!pBOProxy)
		{
			CErrorCodeMapper::GenerateException("STG2_INVALID_SESSION");		
		}	
		if(!user__getAllGroup || !user__getAllGroupResponse) 
		{
			CErrorCodeMapper::GenerateException("STG2_MISSING_PARAM");
		}		

/* 		if(CCommonFunctions::IsAdminUser(pBOProxy) == false)
		{
			// Indicates User does not have the rights to execute this operation.
			// Hence, We are sending plugin's error code.					
			CErrorCodeMapper::GenerateException("STATUS_PERMISSION_CHECK_ERROR", USR_MGR) ;
		}	 */
		if(UserServices::ExecuteGetGroupListCommand(pBOProxy) != STATUS_OK)
		{
			CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");	
		}
		user__getAllGroupResponse->GroupList = UserServices::MapGroupListInfo(GROUPS_XPATH,pBOProxy,soap);
	}
	catch(CStg2Exception & ex)
	{
		DEBUGL1("Stage2::__user__getAllGroup caught Stg2Exception Exception. Description = %s\n", ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), ex.getFaultType(), ex.getErrorType()) != STATUS_OK)
		DEBUGL4("__user__getAllGroup CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}
	catch(CException & ex)
	{
		DEBUGL1("Stage2::__user__getAllGroup caught CException Exception. Description = %s\n", ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), "", eReceiverError) != STATUS_OK)
		DEBUGL4("__user__getAllGroup CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}

	DEBUGL8("Stage2::__user__getAllGroup exit\n");
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __user__setAllGroup(struct soap* soap, _user__setAllGroup *user__setAllGroup, _user__setAllGroupResponse *user__setAllGroupResponse)
{
	DEBUGL8("Stage2::__user__setAllGroup entry\n");
	OffendingParamsList   vOffendingParamsList;
	try
	{
		OffendingParamsStruct offendingParamInfo;
		BOProxyRef boProxy = CCommonFunctions::getBOProxyRef(soap);
		if(!boProxy)
		{
			CErrorCodeMapper::GenerateException("STG2_INVALID_SESSION");
		}	
		
		if(!user__setAllGroup || !user__setAllGroupResponse || !user__setAllGroup->GroupList)
		{
			CErrorCodeMapper::GenerateException("STG2_MISSING_PARAM");
		}	

/* 		if(CCommonFunctions::IsAdminUser(boProxy) == false)
		{
			// Indicates User does not have the rights to execute this operation.
			// Hence, We are sending plugin's error code.					
			CErrorCodeMapper::GenerateException("STATUS_PERMISSION_CHECK_ERROR", USR_MGR) ;
		} */
		
		vector<class common__groupType * >::iterator i= user__setAllGroup->GroupList->Group.begin();
		if(!user__setAllGroup->GroupList->Group.empty())
		{
			for(;i < user__setAllGroup->GroupList->Group.end();i++)	
			{
				if(!(*i)->GroupID || (*i)->GroupID->empty())
				{
					CErrorCodeMapper::GenerateException("STG2_MULTIPLE_MISSING_GROUP_ID");
				}				
				
				if((*i)->Name.empty())
				{
					CErrorCodeMapper::GenerateException("STG2_EMPTY_GROUPS");	
				}						
				
				if((*i)->RoleList && false == (*i)->RoleList->Item.empty())  
				{
					if(CCommonFunctions::IsAnyItemEmptyInList((*i)->RoleList->Item))
					{
						CErrorCodeMapper::GenerateException("STG2_EMPTY_ROLES");
					}
				}
			}		
		}
		
		if(UserServices::ExecuteGetGroupListCommand(boProxy) != STATUS_OK)
		{
			CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");
		}
		
		vector<CString> existingGrpNames ;
		NodeRef pNode = NULL;
		CString xpath= GROUPS_XPATH;
		boProxy->GetValue(xpath.c_str(),pNode);
		NodeListRef pChildren = NULL;
		if(pNode)
		{
			pChildren = pNode->getChildNodes();	
			for ( ulong ii=0; ii < pChildren->getLength(); ii++)
			{
				NodeRef tempNode = pChildren->item(ii);
				tempNode = CUIControllerAdapter::GetChildByName(tempNode,"Information");
				existingGrpNames.push_back(CCommonFunctions::GetNodeValueByRef(tempNode,"name"));
			}		
		}
		vector<CString> deletableGrpNames = existingGrpNames;
		bool createUpdateOrDelete = false;
		if(!user__setAllGroup->GroupList->Group.empty())
		{
			i= user__setAllGroup->GroupList->Group.begin();
			for(;i < user__setAllGroup->GroupList->Group.end();i++)	
			{			
				//remove invalid entries such as duplicated user name and user IDs.
				//vector<class common__groupType * >::iterator tmpIter = user__setAllGroup->GroupList->Group.begin();
				//int count = 0;
				vector<CString>::iterator itStartDelID = deletableGrpNames.begin();
				vector<CString>::iterator itEndDelID = deletableGrpNames.end();
				vector<CString>::iterator itFoundDeletableID = find(itStartDelID,itEndDelID,(*i)->Name);
				if(itEndDelID != itFoundDeletableID)
				{
					// if group name is found in the existing id list it needs to be removed from the deletable list
					DEBUGL8("Stage2::__user__setAllGroup the group that will not be DELETD:: %s \n",((*i)->Name).c_str());
					deletableGrpNames.erase(itFoundDeletableID); 
				}
/* 				for(;tmpIter <user__setAllGroup->GroupList->Group.end(); tmpIter++)
				{
						if((*i)->Name == (*tmpIter)->Name)
							count++;
				} 
				if(count >1)
				{
					offendingParamInfo.Name    = (*i)->Name ;
					offendingParamInfo.Value = "duplicative user id";
					vOffendingParamsList.push_back(offendingParamInfo);
					DEBUGL2("user__setAllGroup :: duplicative ID %s \n",(offendingParamInfo.Name).c_str());
					continue;
				} */
				vector<CString>::iterator itBeginExistingID = existingGrpNames.begin();
				vector<CString>::iterator itEndExistingID = existingGrpNames.end();
				if(itEndExistingID == find(itBeginExistingID,itEndExistingID,(*i)->Name))
				{
					DEBUGL8("Stage2::__user__setAllGroup the group name that will be CREATED :: %s \n",(*i)->Name.c_str());
					 // if Group name is not found in the existing id list it needs to be CREATED
					CString sOperationStatus = UserServices::ExecuteCreateOrEditGroupCommand(*i,"CreateGroup",boProxy,soap);
					if(STG2_SUCCESS != sOperationStatus)
					{
						DEBUGL1("Creation of following group failed %s \n ",(*i)->Name.c_str());
						if(createUpdateOrDelete == false)
							CErrorCodeMapper::GenerateException(sOperationStatus,GRP_MGR);		
						else
						{
							offendingParamInfo.Name    = (*i)->Name;
							offendingParamInfo.Value = "invalid input data";
							vOffendingParamsList.push_back(offendingParamInfo);
							CErrorCodeMapper::GenerateException("STG2_GROUP_PARTIAL_SUCCESS",vOffendingParamsList);						
						}	
					}
					createUpdateOrDelete = true;				
				}
				else
				{
					DEBUGL8("Stage2::__user__setAllGroup the group name that will be MODIFIED :: %s \n",((*i)->Name).c_str());
					 // if Group name is not found in the existing id list it needs to be MODIFIED
					CString sOperationStatus = UserServices::ExecuteCreateOrEditGroupCommand(*i,"UpdateGroup",boProxy,soap);
					if(STG2_SUCCESS != sOperationStatus)
					{
						DEBUGL1("Modification of following group failed %s \n ",((*i)->Name).c_str());
						if(createUpdateOrDelete == false)
							CErrorCodeMapper::GenerateException(sOperationStatus,GRP_MGR);		
						else
						{
							offendingParamInfo.Name    = (*i)->Name;
							offendingParamInfo.Value = "invalid input data";
							vOffendingParamsList.push_back(offendingParamInfo);
							CErrorCodeMapper::GenerateException("STG2_GROUP_PARTIAL_SUCCESS",vOffendingParamsList);
						}	
					}
					createUpdateOrDelete = true;				
				}
			}
		}
		
		if(deletableGrpNames.empty() == false) //delete the unsupplied GROUPs
		{
			vector<CString>::iterator itr = deletableGrpNames.begin();
			for( ; itr != deletableGrpNames.end();itr++)
			{
				CString sOperationStatus = UserServices::ExecuteDeleteGroupCommand(*itr,boProxy);
				if(STG2_SUCCESS != sOperationStatus)
				{
					DEBUGL1("Deletion  of following group failed \n %s",(*itr).c_str());
					if(createUpdateOrDelete == false)
						CErrorCodeMapper::GenerateException(sOperationStatus,GRP_MGR);		
					else
					{
						offendingParamInfo.Name    = "failed to delete";
						offendingParamInfo.Value = *itr;
						vOffendingParamsList.push_back(offendingParamInfo);
						CErrorCodeMapper::GenerateException("STG2_GROUP_PARTIAL_SUCCESS",vOffendingParamsList);
					}	
				}
				createUpdateOrDelete = true;
			}
		}

		user__setAllGroupResponse->Result = common__operationResultType__OperationSucceeded;
	}
	catch(CStg2Exception & ex)
	{
		DEBUGL1("Stage2::__user__setAllGroup caught Stg2Exception Exception. Description = %s\n", ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), ex.getFaultType(), ex.getErrorType(),vOffendingParamsList) != STATUS_OK)
		DEBUGL4("__user__setAllGroup CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}
	catch(CException & ex)
	{
		DEBUGL1("Stage2::__user__setAllGroup caught CException Exception. Description = %s\n", ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), "", eReceiverError,vOffendingParamsList) != STATUS_OK)
		DEBUGL4("__user__setAllGroup CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}


	DEBUGL8("Stage2::__user__setAllGroup exit\n");
	return SOAP_OK;
}
#endif

SOAP_FMAC5 int SOAP_FMAC6 __stg2__getGroup(struct soap* soap, _user__getGroup *user__getGroup, _user__getGroupResponse *user__getGroupResponse)
{
	DEBUGL8("Stage2::__stg2__getGroup entry\n");
	if(!CCommonFunctions::IsOutputManagementEnabled( ))
	{
		stage2ErrorStruct stError;
		CErrorCodeMapper::MapInternalErrCodeToStage2Fault(stError, "STG2_UNSUPPORTED_OPERATION");
		CCommonFunctions::GenerateSoapFault(soap, stError.strDescription, stError.strErrCode, stError.eErrorType);
		return SOAP_FAULT;
	}
	return __user__getGroup(soap,user__getGroup,user__getGroupResponse);
}


common__groupType* UserServices::getGroup(soap* soap, const Ref<BOProxy>& pBOProxy, const CString& sGroupName)
{
	DEBUGL4("UserServices::getGroup Enter\n");

	CUIControllerAdapter::SetXMLValue("", "<GroupManager></GroupManager>" , pBOProxy); // to clear delta document.
	
	if(STATUS_OK != ExecuteGetGroupInfoCommand(sGroupName,pBOProxy))
	{
		DEBUGL1("ExecuteGetGroupInfoCommand failed\n");
		CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");
	}
	
	common__groupType* pGroup = MapGroupInfo("GroupManager/Group",pBOProxy,soap);
	DEBUGL4("UserServices::getGroup Exit\n");
	return pGroup;

}



SOAP_FMAC5 int SOAP_FMAC6 __user__getGroup(struct soap* soap, _user__getGroup *user__getGroup, _user__getGroupResponse *user__getGroupResponse)
{
	try
	{
		DEBUGL8("__user__getGroup entry\n");
		BOProxyRef pBOProxy = CCommonFunctions::getBOProxyRef(soap);
		if(!pBOProxy)
		{
			DEBUGL1("__user__getGroup Invalid Session\n");
			CErrorCodeMapper::GenerateException("STG2_INVALID_SESSION");
		}
		
		if(!user__getGroup)
		{
			CErrorCodeMapper::GenerateException("STG2_MISSING_PARAM");
		}
		
		CString sGroupName = user__getGroup->Name;
		if(sGroupName.empty())
		{
			DEBUGL1("Group Name is empty\n");
			CErrorCodeMapper::GenerateException("STG2_EMPTY_GROUP_NAME");
		}
		
		user__getGroupResponse->Group = UserServices::getGroup(soap, pBOProxy, sGroupName);
	}
	catch(CStg2Exception &ex)
	{
		DEBUGL1("Stage2::__user_getGroup Caught CStg2Exception Exception. Description = %s\n",ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap,ex.C_Str(),ex.getFaultType(),ex.getErrorType()) != STATUS_OK)
		DEBUGL4("Stage2::__user__getGroup CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}
	catch(CException &ex)
	{
		DEBUGL1("Stage2::__user_getGroup Caught CException Exception. Description = %s\n",ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap,ex.C_Str(),"",eReceiverError) != STATUS_OK)
		DEBUGL4("Stage2::__user__getGroup CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}

	DEBUGL8("Stage2::__user__getGroup exit\n");
	return SOAP_OK;

}


SOAP_FMAC5 int SOAP_FMAC6 __stg2__setGroup(struct soap* soap, _user__setGroup *user__setGroup, _user__setGroupResponse *user__setGroupResponse)
{
	DEBUGL8("Stage2::__stg2__setGroup entry\n");
	if(!CCommonFunctions::IsOutputManagementEnabled( ))
	{
		stage2ErrorStruct stError;
		CErrorCodeMapper::MapInternalErrCodeToStage2Fault(stError, "STG2_UNSUPPORTED_OPERATION");
		CCommonFunctions::GenerateSoapFault(soap, stError.strDescription, stError.strErrCode, stError.eErrorType);
		return SOAP_FAULT;
	}
	return __user__setGroup(soap,user__setGroup,user__setGroupResponse);
}
SOAP_FMAC5 int SOAP_FMAC6 __user__setGroup(struct soap* soap, _user__setGroup *user__setGroup, _user__setGroupResponse *user__setGroupResponse)
{
	
	try
	{
		DEBUGL8("__user__setGroup entry\n");
		BOProxyRef pBOProxy = CCommonFunctions::getBOProxyRef(soap);
		if(!pBOProxy)
		{
			CErrorCodeMapper::GenerateException("STG2_INVALID_SESSION");
		}
		
		if(!user__setGroup || !user__setGroup->Group)
		{
			CErrorCodeMapper::GenerateException("STG2_MISSING_PARAM");
		}
		
		if(!user__setGroup->Group->GroupID || user__setGroup->Group->GroupID->empty())
		{
			CErrorCodeMapper::GenerateException("STG2_MISSING_GROUP_ID");
		}
				
		CUIControllerAdapter::SetXMLValue("", "<GroupManager></GroupManager>" , pBOProxy); // to clear delta document.
		
		user__setGroupResponse->Result = common__operationResultType__OperationSucceeded;
		
		CString sOperationStatus = UserServices::ExecuteCreateOrEditGroupCommand(user__setGroup->Group,"UpdateGroup",pBOProxy,soap);
		
		if(STG2_SUCCESS != sOperationStatus)
		{
			CErrorCodeMapper::GenerateException(sOperationStatus, GRP_MGR);
		}
		
	}
	catch(CStg2Exception &ex)
	{
		DEBUGL1("Stage2:__user_setGroup caught CStg2Exception Exception. Description = %s \n",ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap,ex.C_Str(),ex.getFaultType(),ex.getErrorType()) != STATUS_OK)
		DEBUGL4("Stage2::__user_setGroup CCommonFunctions::GenerateSoapFault() Failed \n");
		return SOAP_FAULT;
	}
	catch(CException &ex)
	{
		DEBUGL1("Stage2::__user_setGroup caught CException Exception. Description = %s \n",ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap,ex.C_Str(),"",eReceiverError) != STATUS_OK)
		DEBUGL4("Stage2::__user__setGroup CCommonFunctions::GenerateSoapFAult() Failed \n");
		return SOAP_FAULT;
	}
	
	DEBUGL8("Stage2::__user__setGroup exit\n");
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __stg2__createGroup(struct soap* soap, _user__createGroup *user__createGroup, _user__createGroupResponse *user__createGroupResponse)
{
	DEBUGL8("Stage2::__stg2__createGroup entry\n");
	if(!CCommonFunctions::IsOutputManagementEnabled( ))
	{
		stage2ErrorStruct stError;
		CErrorCodeMapper::MapInternalErrCodeToStage2Fault(stError, "STG2_UNSUPPORTED_OPERATION");
		CCommonFunctions::GenerateSoapFault(soap, stError.strDescription, stError.strErrCode, stError.eErrorType);
		return SOAP_FAULT;
	}
	return __user__createGroup(soap,user__createGroup,user__createGroupResponse);

}

SOAP_FMAC5 int SOAP_FMAC6 __user__createGroup(struct soap* soap, _user__createGroup *user__createGroup, _user__createGroupResponse *user__createGroupResponse)
{
	DEBUGL8("Stage2::__user__createGroup entry\n");
	try
	{
		BOProxyRef pBOProxy = CCommonFunctions::getBOProxyRef(soap);
		if(!pBOProxy)
		{
			DEBUGL1("__user__createGroup Invalid Session\n");
			CErrorCodeMapper::GenerateException("STG2_INVALID_SESSION");
		}

		if(!user__createGroup || !user__createGroup->Group)
		{
			CErrorCodeMapper::GenerateException("STG2_MISSING_PARAM");
		}
						
		user__createGroupResponse->Result = common__operationResultType__OperationSucceeded;
		
		CUIControllerAdapter::SetXMLValue("", "<GroupManager></GroupManager>" , pBOProxy); // to clear delta document.
		
		CString sOperationStatus = UserServices::ExecuteCreateOrEditGroupCommand(user__createGroup->Group, "CreateGroup", pBOProxy, soap);
		
		if(STG2_SUCCESS != sOperationStatus)
		{
			CErrorCodeMapper::GenerateException(sOperationStatus, GRP_MGR);
		}
		
	}
	catch(CStg2Exception & ex)
	{
		DEBUGL1("Stage2::__user__createGroup caught Stg2Exception Exception. Description = %s\n", ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), ex.getFaultType(), ex.getErrorType()) != STATUS_OK)
		DEBUGL4("__user__createGroup CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}
	catch(CException & ex)
	{
		DEBUGL1("Stage2::__user__createGroup caught CException Exception. Description = %s\n", ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), "", eReceiverError) != STATUS_OK)
		DEBUGL4("__user__createGroup CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}

	DEBUGL8("Stage2::__user__createGroup exit\n");

	return SOAP_OK;
}
SOAP_FMAC5 int SOAP_FMAC6 __stg2__deleteGroup(struct soap* soap, _user__deleteGroup *user__deleteGroup, _user__deleteGroupResponse *user__deleteGroupResponse)
{
	DEBUGL8("Stage2::__stg2__deleteGroup entry\n");
	if(!CCommonFunctions::IsOutputManagementEnabled( ))
	{
		stage2ErrorStruct stError;
		CErrorCodeMapper::MapInternalErrCodeToStage2Fault(stError, "STG2_UNSUPPORTED_OPERATION");
		CCommonFunctions::GenerateSoapFault(soap, stError.strDescription, stError.strErrCode, stError.eErrorType);
		return SOAP_FAULT;
	}
	return __user__deleteGroup(soap,user__deleteGroup,user__deleteGroupResponse);
}
SOAP_FMAC5 int SOAP_FMAC6 __user__deleteGroup(struct soap* soap, _user__deleteGroup *user__deleteGroup, _user__deleteGroupResponse *user__deleteGroupResponse)
{
	DEBUGL8("__user__deleteGroup entry\n");
	try
	{
		BOProxyRef pBOProxy = CCommonFunctions::getBOProxyRef(soap);
		if(!pBOProxy)
		{
			DEBUGL1("__user__deleteGroup Invalid Session\n");
			CErrorCodeMapper::GenerateException("STG2_INVALID_SESSION");
		}
		
		if(!user__deleteGroup)
		{
			CErrorCodeMapper::GenerateException("STG2_MISSING_PARAM");
		}

		CString sGroupName = user__deleteGroup->Name;
		
		if(sGroupName.empty()) 
		{
			DEBUGL1("GroupName is empty\n");
			CErrorCodeMapper::GenerateException("STG2_EMPTY_GROUP_NAME");
		}

		user__deleteGroupResponse->Result = common__operationResultType__OperationSucceeded;
		CString sOperationStatus = UserServices::ExecuteDeleteGroupCommand(sGroupName, pBOProxy);
		if(STG2_SUCCESS != sOperationStatus)
		{
			DEBUGL1("UserServices::ExecuteDeleteGroupCommand operation failed\n");
			CErrorCodeMapper::GenerateException(sOperationStatus,GRP_MGR);
		}
				
	}
	catch(CStg2Exception & ex)
	{
		DEBUGL1("Stage2::__user__deleteGroup caught Stg2Exception Exception. Description = %s\n", ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), ex.getFaultType(), ex.getErrorType()) != STATUS_OK)
		DEBUGL4("__user__deleteGroup CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}
	catch(CException & ex)
	{
		DEBUGL1("Stage2::__user__deleteGroup caught CException Exception. Description = %s\n", ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), "", eReceiverError) != STATUS_OK)
		DEBUGL4("__user__deleteGroup CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}
	DEBUGL8("Stage2::__user__deleteGroup exit\n");

	return SOAP_OK;
}

#ifdef STAGE2_UNSUPPORTED
SOAP_FMAC5 int SOAP_FMAC6 __user__deleteAllGroup(struct soap* soap, _user__deleteAllGroup *user__deleteAllGroup, _user__deleteAllGroupResponse *user__deleteAllGroupResponse)
{
	DEBUGL8("Stage2::__user__deleteAllGroup entry\n");
	try
	{
		BOProxyRef boProxy = CCommonFunctions::getBOProxyRef(soap);
		if(!boProxy)
		{
			CErrorCodeMapper::GenerateException("STG2_INVALID_SESSION");
		}		
		
		if(!user__deleteAllGroup || !user__deleteAllGroupResponse)
		{
			CErrorCodeMapper::GenerateException("STG2_MISSING_PARAM");
		}
		
		UserServices::ExecuteDeleteAllGroupsCommand(boProxy);
		user__deleteAllGroupResponse->Result = common__operationResultType__OperationSucceeded;

	}
	catch(CStg2Exception & ex)
	{
		DEBUGL1("Stage2::__user__deleteAllGroup caught Stg2Exception Exception. Description = %s\n", ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), ex.getFaultType(), ex.getErrorType()) != STATUS_OK)
		DEBUGL4("__user__deleteAllGroup CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}
	catch(CException & ex)
	{
		DEBUGL1("Stage2::__user__deleteAllGroup caught CException Exception. Description = %s\n", ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), "", eReceiverError) != STATUS_OK)
		DEBUGL4("__user__deleteAllGroup CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}

	DEBUGL8("Stage2::__user__deleteAllGroup exit\n");
	return SOAP_OK;
}






/*SOAP_FMAC5 int SOAP_FMAC6 __user__backupUserManagement( soap* soap, _user__backupUserManagement *backupUserManagement, _user__backupUserManagementResponse *backupUserManagementResponse)
{
	DEBUGL8("cdeviceconfiguration::__user__backupUserManagement Entry");
	try
	{
		BOProxyRef pBOProxy = CCommonFunctions::getBOProxyRef(soap);
		if (!pBOProxy) throw CStg2Exception(eSenderError,"ERR_EBS_INVALID_TOKEN",SOAP_FAULT,"Invalid Session");

		if (!backupUserManagement) throw CStg2Exception(eSenderError, "ERR_EBS_COMMON_PAR_INVALID_VALUE", SOAP_FAULT, "__user__backupUserManagement failed");

		if(!backupUserManagementResponse)
		throw CStg2Exception(eSenderError, "ERR_EBS_COMMON_PAR_INVALID_VALUE",SOAP_FAULT, "__user__backupUserManagementResponse is NULL");
	}
	catch(CStg2Exception & ex)
	{
		DEBUGL1("Stage2::__user__backupUserManagement caught Stg2Exception Exception. Description = %s\n", ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), ex.getFaultType(), ex.getErrorType()) != STATUS_OK)
		DEBUGL4("__user__backupUserManagement CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}
	catch(CException & ex)
	{
		DEBUGL1("Stage2::__user__backupUserManagement caught CException Exception. Description = %s\n", ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), "backupUserManagement Failed", eReceiverError) != STATUS_OK)
		DEBUGL4("__user__backupUserManagement CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}

	DEBUGL8("__user__backupUserManagement Exit");
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __user__restoreUserManagement(soap* soap, _user__restoreUserManagement *restoreUserManagement, _user__restoreUserManagementResponse *restoreUserManagementResponse)
{
	DEBUGL8("cdeviceconfiguration::__user__restoreUserManagement Entry");
	try
	{
		BOProxyRef pBOProxy = CCommonFunctions::getBOProxyRef(soap);
		if (!pBOProxy) throw CStg2Exception(eSenderError,"ERR_EBS_INVALID_TOKEN",SOAP_FAULT,"Invalid Session");

		if (!restoreUserManagement) throw CStg2Exception(eSenderError, "ERR_EBS_COMMON_PAR_INVALID_VALUE", SOAP_FAULT, "__user__restoreUserManagement failed");

		if(!restoreUserManagementResponse)
		throw CStg2Exception(eSenderError, "ERR_EBS_COMMON_PAR_INVALID_VALUE",SOAP_FAULT, "__user__restoreUserManagementResponse is NULL");
	}
	catch(CStg2Exception & ex)
	{
		DEBUGL1("Stage2::__user__restoreUserManagement caught Stg2Exception Exception. Description = %s\n", ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), ex.getFaultType(), ex.getErrorType()) != STATUS_OK)
		DEBUGL4("__user__restoreUserManagement CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}
	catch(CException & ex)
	{
		DEBUGL1("Stage2::__user__restoreUserManagement caught CException Exception. Description = %s\n", ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), "restoreUserManagement Failed", eReceiverError) != STATUS_OK)
		DEBUGL4("__user__restoreUserManagement CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}

	DEBUGL8("__user__restoreUserManagement Exit");
	return SOAP_OK;

}*/
#endif

SOAP_FMAC5 int SOAP_FMAC6 __user__resetAllQuota(struct soap* soap, _user__resetAllQuota *user__resetAllQuota, _user__resetAllQuotaResponse *user__resetAllQuotaResponse)
{

	DEBUGL8("Stage2::__user__resetAllQuota entry\n");
	CString        operationName     = "resetAllQuota";
	try
	{
		 //Check if we have a conflicting operation in progress
		if(CLongRunningOpHelper::IsAnotherConflictingOperationInProgress(soap, operationName))
		{
			// Don't use GenerateException since it will update Running operation status to "false" which we don't want.
			stage2ErrorStruct stError;
			CErrorCodeMapper::MapInternalErrCodeToStage2Fault(stError, "STG2_CONF_OP_INPROGRESS");
			CCommonFunctions::GenerateSoapFault(soap, stError.strDescription, stError.strErrCode, stError.eErrorType);
			return SOAP_FAULT;	
		}
		else
		{
			//Add this operation to the current running op list
			CLongRunningOpHelper::UpdateRunningOpStatus(soap, operationName, true);
		}
		
		BOProxyRef boProxy = CCommonFunctions::getBOProxyRef(soap);
		if(!boProxy)
		{
			DEBUGL1("__user__resetAllQuota pBOProxy is NULL\n");
			CErrorCodeMapper::GenerateException("STG2_INVALID_SESSION");
		}
		
		if(!user__resetAllQuota)
		{
			CErrorCodeMapper::GenerateException("STG2_MISSING_PARAM");
		}
				
		if(!user__resetAllQuota->AllUsers && !user__resetAllQuota->AllDepartments)
		{
			user__resetAllQuotaResponse->Result = common__operationResultType__OperationSucceeded ;
			return SOAP_OK;	
		}
		Status resetUser = STATUS_OK;
		Status resetDept = STATUS_OK;
		if(user__resetAllQuota->AllUsers)
			resetUser = UserServices::ExecuteResetUserQuotaCommand(boProxy);
		if(user__resetAllQuota->AllDepartments)
			resetDept  = UserServices::ExecuteResetBillingCodeQuotaCommand(boProxy);
		if(resetUser != STATUS_OK || resetDept != STATUS_OK)
		{	
			DEBUGL2("Stage2::__user__resetAllQuota failed\n");	
			user__resetAllQuotaResponse->Result = common__operationResultType__OperationFailed;
		}
		else
		{
			user__resetAllQuotaResponse->Result = common__operationResultType__OperationSucceeded ;
		}
		
		//Remove this operation from the current running op list
       CLongRunningOpHelper::UpdateRunningOpStatus(soap, operationName, false);	
			
	}
	catch(CStg2Exception & ex)
	{
		DEBUGL1("Stage2::__user__resetAllQuota caught Stg2Exception Exception. Description = %s\n", ex.C_Str());
		CLongRunningOpHelper::UpdateRunningOpStatus(soap, operationName, false);	
		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), ex.getFaultType(), ex.getErrorType()) != STATUS_OK)
		DEBUGL4("__user__resetAllQuota CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}
	catch(CException & ex)
	{
		DEBUGL1("Stage2::__user__resetAllQuota caught CException Exception. Description = %s\n", ex.C_Str());
		CLongRunningOpHelper::UpdateRunningOpStatus(soap, operationName, false);	
		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), "", eReceiverError) != STATUS_OK)
		DEBUGL4("__user__resetAllQuota CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}

	DEBUGL8("Stage2::__user__resetAllQuota exit\n");
	return SOAP_OK;	
}
SOAP_FMAC5 int SOAP_FMAC6 __stg2__resetAllQuota(struct soap* soap, _user__resetAllQuota *user__resetAllQuota, _user__resetAllQuotaResponse *user__resetAllQuotaResponse)
{
	if(!CCommonFunctions::IsOutputManagementEnabled( ))
	{
		stage2ErrorStruct stError;
		CErrorCodeMapper::MapInternalErrCodeToStage2Fault(stError, "STG2_UNSUPPORTED_OPERATION");
		CCommonFunctions::GenerateSoapFault(soap, stError.strDescription, stError.strErrCode, stError.eErrorType);
		return SOAP_FAULT;
	}
	return __user__resetAllQuota(soap, user__resetAllQuota, user__resetAllQuotaResponse);
}

Status UserServices::ExecuteResetUserQuotaCommand(BOProxyRef &pBOProxy)
{
	DEBUGL8("Stage2::ExecuteResetUserQuotaCommand enter\n");
	if(!pBOProxy) return STATUS_FAILED; 
	
	CUIControllerAdapter::SetXMLValue("", "<Accounting><Users></Users></Accounting>" , pBOProxy); 

	// Construct the command
	CString strCommandMsg = "<Command><ResetUserQuota>	<commandNode>Accounting/Users</commandNode><Params/></ResetUserQuota></Command>";
	DEBUGL8("\ExecuteResetUserQuotaCommand::SendCommandParamElement: message:\n%s\n", strCommandMsg.c_str());

	// Execute command
	CString strResponse;
	if(STATUS_OK != pBOProxy->ExecuteCommand(strCommandMsg.c_str(), strResponse))
	{
		CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");
	}

	CString sOperationStatus = CUIControllerAdapter::GetOperationStatusString(strResponse);
	if(STG2_SUCCESS != sOperationStatus)
	{
		DEBUGL1("UserServices::ExecuteResetUserQuotaCommand : Failed to ExecuteCommand\n");
		CErrorCodeMapper::GenerateException(sOperationStatus, ACC_MGR);
	}

	DEBUGL8("Stage2::ExecuteResetUserQuotaCommand exit\n");
	return STATUS_OK;
}
Status UserServices::ExecuteResetBillingCodeQuotaCommand(BOProxyRef &pBOProxy)
{
	DEBUGL8("Stage2::ExecuteResetBillingCodeQuotaCommand enter\n");
	if(!pBOProxy) return STATUS_FAILED; 
	
	CUIControllerAdapter::SetXMLValue("", "<Accounting><BillingCodes></BillingCodes></Accounting>" , pBOProxy); 

	// Construct the command
	CString strCommandMsg = "<Command><ResetBillingCodeQuota><commandNode>Accounting/BillingCodes</commandNode><Params/></ResetBillingCodeQuota></Command>";
	DEBUGL8("\ExecuteResetBillingCodeQuotaCommand::SendCommandParamElement: message:\n%s\n", strCommandMsg.c_str());

	// Execute command
	CString strResponse;
	if(STATUS_OK != pBOProxy->ExecuteCommand(strCommandMsg.c_str(), strResponse))
	{
		CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");
	}

	CString sOperationStatus = CUIControllerAdapter::GetOperationStatusString(strResponse);
	if(STG2_SUCCESS != sOperationStatus)
	{
		DEBUGL1("UserServices::ExecuteResetBillingCodeQuotaCommand : Failed to ExecuteCommand\n");
		CErrorCodeMapper::GenerateException(sOperationStatus, ACC_MGR);
	}

	DEBUGL8("Stage2::ExecuteResetBillingCodeQuotaCommand exit\n");
	return STATUS_OK;
}


common__stringListType* UserServices::GetSimilarChildNodes(const CString parentXpath,const BOProxyRef& boProxy,soap* soap)
{
	DEBUGL8("start of UserServices::GetSimilarChildNodes, parentXpath => %s\n",parentXpath.c_str());
	common__stringListType *stringList;
	stringList = soap_new_common__stringListType(soap,-1);

	NodeListRef pChildren = NULL;
	NodeRef pNode = NULL;
	boProxy->GetValue(parentXpath.c_str(),pNode);
	if(!pNode)
	return stringList;

	pChildren = pNode->getChildNodes();	

	for ( ulong i=0; i < pChildren->getLength(); i++)
	{
		NodeRef	pChildNode = pChildren->item(i);
		stringList->Item.push_back(pChildNode->getTextContent());
		DEBUGL8("child %d text content : %s\n",i+1,(pChildNode->getTextContent()).c_str());
	}		
	DEBUGL8("end of UserServices::GetSimilarChildNodes\n");
	return stringList;
}
common__departmentDetailType* UserServices::GetUserRelatedDepartments(CString parentXpath,BOProxyRef &boProxy,soap* soap)
{
	common__departmentDetailType* department = soap_new_common__departmentDetailType(soap,-1);
	
	DEBUGL8("start of UserServices::GetUserRelatedDepartments, parentXpath => %s\n",parentXpath.c_str());
	department->DepartmentCodeList = soap_new_common__stringListType(soap,-1);

	NodeListRef pChildren = NULL;
	NodeRef pNode = NULL;
	CString temp = parentXpath + "/Departments";
	boProxy->GetValue(temp.c_str(),pNode);
	if(!pNode)
		return department;

	pChildren = pNode->getChildNodes();	
	for ( ulong i=0; i < pChildren->getLength(); i++)
	{
		NodeRef	pChildNode = pChildren->item(i);
		CString deptCode = CCommonFunctions::GetNodeValueByRef(pChildNode, "departmentCode");
		department->DepartmentCodeList->Item.push_back(deptCode);
		DEBUGL8("child %d text content : %s\n",i+1,deptCode.c_str());
	}		

	DEBUGL8("end of UserServices::GetUserRelatedDepartments\n");
	return department;
}
CString UserServices::ExecuteSearchUserByUserName(CString sUserName,const BOProxyRef &pBOProxy)
{
	DEBUGL8("UserServices::ExecuteSearchUserByUserName enter\n");
	
	CUIControllerAdapter::SetXMLValue("", "<UserManager></UserManager>" , pBOProxy); // to clear delta document.
	
	// Set an empty UserManager/Users DOM.
	vector<CString> xPaths;
	vector<CString> values;
	xPaths.push_back("UserManager/Users");
	values.push_back("");
	CUIControllerAdapter::SetXPathValues(xPaths, values, pBOProxy);
		
	commandParam p1, p2;
	// searchDetails Param
	p1.strParamName = "searchDetails";
	p1.strParamValue = "UserManager";
	p1.mapAttributes.insert(make_pair("contentType","XPath"));
	p1.mapAttributes.insert(make_pair("searchType","User"));

	// searchInfo Param
	p2.strParamName = "searchInfo";
	p2.strParamValue = "";
	p2.mapAttributes.insert(make_pair("value1",sUserName));
	p2.mapAttributes.insert(make_pair("value2",""));

	vector<commandParam> vectParams;
	vectParams.push_back(p1);
	vectParams.push_back(p2);

	CString sResponse;
	if(STATUS_OK != CUIControllerAdapter::SendCommand("SearchUsers", "UserManager/Users", vectParams, sResponse, pBOProxy))
	{
		DEBUGL1("Failed to SendCommand\n");
		CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");
	
	}
			
	return CUIControllerAdapter::GetOperationStatusString(sResponse);
}


/* CString UserServices::ExecuteSearchUserByDepartmentCode(CString sDepartmentCode,BOProxyRef &pBOProxy)
{
	DEBUGL8("UserServices::ExecuteSearchUserByDepartmentCode enter\n");
	
	// Set an empty UserManager/Users DOM.
	vector<CString> xPaths;
	vector<CString> values;
	xPaths.push_back("UserManager/Users");
	values.push_back("");
	CUIControllerAdapter::SetXPathValues(xPaths, values, pBOProxy);
		
	commandParam p1, p2;
	// searchDetails Param
	p1.strParamName = "searchDetails";
	p1.strParamValue = "UserManager";
	p1.mapAttributes.insert(make_pair("contentType","XPath"));
	p1.mapAttributes.insert(make_pair("searchType","Department"));

	// searchInfo Param
	p2.strParamName = "searchInfo";
	p2.strParamValue = "";
	p2.mapAttributes.insert(make_pair("value1",sDepartmentCode));
	p2.mapAttributes.insert(make_pair("value2",""));

	vector<commandParam> vectParams;
	vectParams.push_back(p1);
	vectParams.push_back(p2);

	CString sResponse;
	if(STATUS_OK != CUIControllerAdapter::SendCommand("SearchUsers", "UserManager/Users", vectParams, sResponse, pBOProxy))
	{
		DEBUGL1("Failed to SendCommand\n");
		CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");
	
	}
			
	return CUIControllerAdapter::GetOperationStatusString(sResponse);
}
*/
CString UserServices::ExecuteSearchDepartmentCommand(CString sDepartmentCode, BOProxyRef& pBOProxy)
{
	DEBUGL8("UserServices::ExecuteSearchDepartmentCommand Enter\n");
	
	return ExecuteGetBillingCodes(pBOProxy, sDepartmentCode);
	
	
	/* 
	if(!pBOProxy) 
	{
		DEBUGL1("BOProxy is NULL\n");
		CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");
	}

	CString strXML = "<SetValue><Payload XMLPayLoad=\"true\" overrideDelta=\"true\"><path/><value>";
	strXML += "<Accounting><BillingCodes><BillingCodeInfo><code>";
	strXML += sDepartmentCode;
	strXML += "</code></BillingCodeInfo></BillingCodes></Accounting>";
	strXML += "</value></Payload></SetValue>";
	if(STATUS_OK != pBOProxy->SetValue(strXML.c_str()))
	{
		DEBUGL1("SetValue Failed \n");
		CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");
	}

	// Construct the command
	CString strCommandMsg = "<Command><GetBillingCodes><commandNode>Accounting/BillingCodes</commandNode>";
	DEBUGL8("\nCUIControllerAdapter::SendCommandParamElement: message:\n%s\n", strCommandMsg.c_str());

	// Include the parameters for the command
	strCommandMsg += "<Params>";
	strCommandMsg += "<billingCodesDetails contentType=\"XPath\" >Accounting/BillingCodes/BillingCodeInfo</billingCodesDetails>";
	strCommandMsg += "</Params></GetBillingCodes></Command>";
	DEBUGL8("\nCUIControllerAdapter::SendCommandParamElement: message:\n%s\n", strCommandMsg.c_str());

	// Execute command
	CString strResponse;
	if(STATUS_OK != pBOProxy->ExecuteCommand(strCommandMsg.c_str(), strResponse))
	{
		DEBUGL1("Failed to SendCommand\n");
		CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");
	}
	
	return CUIControllerAdapter::GetOperationStatusString(strResponse); */

	
} 
Status UserServices::ExecuteChangeUserPasswordCommand(CString sUserName,CString sCurrentPassword,CString sNewPassword, CString sChnageType, BOProxyRef &pBOProxy)
{
	DEBUGL8("ExecuteChangeUserPasswordCommand::Enter \n");
	CString  strUserCredentialPath   = "Authentication/UserCredential";
	std::vector<CString> xPaths;
	std::vector<CString> values;
	
	DEBUGL4("sUserName is ++%s++, sCurrentPassword is ++%s++, newPassword is ++%s++\n", sUserName.c_str(), sCurrentPassword.c_str(), sNewPassword.c_str());
	
	strUserCredentialPath = "UserManager/View/User/Information";
	values.push_back(sUserName);
	xPaths.push_back(strUserCredentialPath + "/name");
	if(!sCurrentPassword.empty())
	{
		values.push_back(sCurrentPassword);
		xPaths.push_back(strUserCredentialPath + "/passwd");
	}
	values.push_back(sNewPassword);
	xPaths.push_back(strUserCredentialPath + "/newPasswd");
	values.push_back(sNewPassword);
	xPaths.push_back(strUserCredentialPath + "/repeatedPasswd");

	DEBUGL8("ExecuteChangeUserPasswordCommand : calling SetXPathValues\n");

	CUIControllerAdapter::SetXPathValues(xPaths, values, pBOProxy);
	vector<commandParam> vectParams;
	commandParam cmdParam;
	
	cmdParam.strParamName = "userDetails";
	cmdParam.strParamValue = "UserManager/View/User";
	cmdParam.mapAttributes.insert(make_pair("contentType","XPath"));
	vectParams.push_back(cmdParam);
	
	/* cmdParam.strParamValue = "Reset"; // If Old Password is provided then password set command type to Change.
	if(!sCurrentPassword.empty())
	{
		cmdParam.strParamValue = "Change";
	} */
	
	cmdParam.strParamValue = sChnageType;
	cmdParam.strParamName = "cmdDetails";	
	cmdParam.mapAttributes.insert(make_pair("contentType","Value"));
	
	vectParams.push_back(cmdParam);

	// Execute the command
	CString strResponse;
	strResponse.clear();
	if(STATUS_OK != CUIControllerAdapter::SendCommand("ChangePassword","UserManager/Users",vectParams,strResponse,pBOProxy))
	{
		CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");
	}
	
	CString sOperationStatus = CUIControllerAdapter::GetOperationStatusString(strResponse);
	if(STG2_SUCCESS != sOperationStatus)
	{
		CErrorCodeMapper::GenerateException(sOperationStatus, USR_MGR);
	}
	return STATUS_OK;
}

CString UserServices::ExecuteGetUserCommand(CString sUserName,BOProxyRef &pBOProxy,bool byUserID)
{
	DEBUGL8("UserServices::ExecuteGetUserCommand Enter\n");
	
	CUIControllerAdapter::SetXMLValue("", "<UserManager></UserManager>" , pBOProxy); // to clear delta document.
	
	std::vector<CString> xPaths;
	std::vector<CString> values;
	
	values.push_back(sUserName);
	if(byUserID)
		xPaths.push_back(CString(USERS_XPATH)+"/User/@ID");
	else
		xPaths.push_back(CString(USERS_XPATH)+"/User/Information/name");

	CUIControllerAdapter::SetXPathValues(xPaths, values,pBOProxy);

	vector<commandParam> vectParams;
	commandParam cmdParam,searchParam;
	cmdParam.strParamValue = CString(USERS_XPATH)+"/User";
	cmdParam.strParamName = "userDetails";
	cmdParam.mapAttributes.insert(make_pair("contentType","XPath"));
	vectParams.push_back(cmdParam);
	if(byUserID)
	{
		searchParam.strParamValue = "ID";
		searchParam.strParamName = "searchBy";
		searchParam.mapAttributes.insert(make_pair("contentType","Value"));
		vectParams.push_back(searchParam);
	}
	CString strResponse ;
	if(STATUS_OK != CUIControllerAdapter::SendCommand("GetUsers", USERS_XPATH,vectParams,strResponse,pBOProxy))
	{
		DEBUGL1("Failed to SendCommand\n");
		CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");
	}
	
	CString sOperationStatus = CUIControllerAdapter::GetOperationStatusString(strResponse);
	DEBUGL8("UserServices::ExecuteGetUserCommand Exit\n");
	return sOperationStatus;
}
common__quotaType* UserServices::MapTotalQuotaInfo(CString quotaPath,BOProxyRef &boProxy,soap *soap)
{
	DEBUGL8("start of UserServices::MapTotalQuotaInfo\n");
	NodeRef pNode = NULL;
	boProxy->GetValue(quotaPath.c_str(),pNode);
	common__quotaType* jobQuota = soap_new_common__quotaType(soap,-1);
	//WSS 1017 issue fix
	//jobQuota->Name = soap_new_std__string(soap,-1);
	//*(jobQuota->Name) = "job";

	if(CCommonFunctions::GetNodeValueByRef(pNode, "jobQuotaEnabled") == "1"||CCommonFunctions::GetNodeValueByRef(pNode, "jobQuotaEnabled") == "true")
	{
		jobQuota->Enable = true;
	}
	else
	{
		jobQuota->Enable = false;
	}
	//WSS 1017 issue fix to return quota even if the value is false
	jobQuota->Balance = soap_new_std__string(soap,-1);
	*(jobQuota->Balance) = CCommonFunctions::GetNodeValueByRef(pNode, "jobRemainingQuantity" );
	jobQuota->Quota = soap_new_std__string(soap,-1);
	*(jobQuota->Quota) = CCommonFunctions::GetNodeValueByRef(pNode, "jobDefaultQuantity" );
	DEBUGL8("end of UserServices::MapTotalQuotaInfo\n");
	return jobQuota;
}

common__printQuotaCategoryType* UserServices::MapUserQuotaInfo(CString quotaPath,BOProxyRef &boProxy,soap *soap)
{
	DEBUGL8("start of UserServices::MapUserQuotaInfo\n");
	NodeRef pNode = NULL;
	boProxy->GetValue(quotaPath.c_str(),pNode);
	DEBUGL8("end of UserServices::MapUserQuotaInfo\n");
	return UserServices::MapUserQuotaInfo(pNode,boProxy,soap);
}
common__printQuotaCategoryType* UserServices::MapUserQuotaInfo(NodeRef pNode,BOProxyRef &boProxy,soap *soap)
{
	DEBUGL8("start of UserServices::MapUserQuotaInfo\n");
	
	common__printQuotaCategoryType* printQuota = soap_new_common__printQuotaCategoryType(soap,-1);
	
	/*NodeRef pQuotaType = NULL;
	boProxy->GetValue("Controller/Settings/QuotaSetting/Print/Type",pQuotaType);
	if(!pQuotaType)
	{
		DEBUGL1("UserServices::MapUserQuotaInfo could not bind to xpath Controller/Settings/QuotaSetting/Print/Type \n");
		CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");
	}

	CString quotaType = pQuotaType->getTextContent();
	
	
	if(quotaType == "BlackOrColorQuota")
	{*/
		printQuota->__union_printQuotaCategoryType = SOAP_UNION__common__union_printQuotaCategoryType_ColorJobQuota;
		printQuota->union_printQuotaCategoryType.ColorJobQuota  = soap_new_common__colorJobQuotaType(soap,-1);
	/*}
	else
	{
		printQuota->__union_printQuotaCategoryType = SOAP_UNION__common__union_printQuotaCategoryType_PrintJobQuota;
		printQuota->union_printQuotaCategoryType.PrintJobQuota  = soap_new_common__quotaType(soap,-1);
	}*/
		
	
	if(!pNode)
	{
		DEBUGL1("UserServices::MapUserQuotaInfo pnode passed is empty \n");
		return printQuota;
	}
	//if(quotaType == "BlackOrColorQuota")
	//{
		common__colorJobQuotaType *quotaInfo= soap_new_common__colorJobQuotaType(soap,-1);
		quotaInfo->ColorQuota = soap_new_common__quotaType(soap,-1);
		quotaInfo->ColorQuota->Name = soap_new_std__string(soap,-1);
		*(quotaInfo->ColorQuota->Name) = "fullColor";
		quotaInfo->ColorQuota->Enable = false;
		if(CCommonFunctions::GetNodeValueByRef(pNode, "fullColorQuotaEnabled") == "1"||CCommonFunctions::GetNodeValueByRef(pNode, "fullColorQuotaEnabled") == "true")
		{
			quotaInfo->ColorQuota->Enable = true;
		}
		
		CString sColorBalance = CCommonFunctions::GetNodeValueByRef(pNode, "fullColorRemainingQuantity" ) ;
		if(!sColorBalance.empty())
		{
			quotaInfo->ColorQuota->Balance = soap_new_std__string(soap,-1);
			*(quotaInfo->ColorQuota->Balance) =  sColorBalance;
		}
		CString sColorQuota = CCommonFunctions::GetNodeValueByRef(pNode, "fullColorDefaultQuantity" );
		if(!sColorQuota.empty())
		{
			quotaInfo->ColorQuota->Quota = soap_new_std__string(soap,-1);
			*(quotaInfo->ColorQuota->Quota) = sColorQuota;
		}		

		quotaInfo->BlackQuota = soap_new_common__quotaType(soap,-1);
		quotaInfo->BlackQuota->Name = soap_new_std__string(soap,-1);
		*(quotaInfo->BlackQuota->Name) = "black";
		quotaInfo->BlackQuota->Enable = false;
		
		if(CCommonFunctions::GetNodeValueByRef(pNode, "blackQuotaEnabled") == "1"||CCommonFunctions::GetNodeValueByRef(pNode, "blackQuotaEnabled") == "true")
		{
			quotaInfo->BlackQuota->Enable = true;
		}
		
		CString sBlackBalance = CCommonFunctions::GetNodeValueByRef(pNode, "blackRemainingQuantity" );
		if(!sBlackBalance.empty())
		{
			quotaInfo->BlackQuota->Balance = soap_new_std__string(soap,-1);
			*(quotaInfo->BlackQuota->Balance) = sBlackBalance; 
		}
		
		CString sBlackQuota = CCommonFunctions::GetNodeValueByRef(pNode, "blackDefaultQuantity" );
		if(!sBlackQuota.empty())
		{
			quotaInfo->BlackQuota->Quota = soap_new_std__string(soap,-1);
			*(quotaInfo->BlackQuota->Quota) = sBlackQuota;
		}
		
		printQuota->union_printQuotaCategoryType.ColorJobQuota  = quotaInfo;
	/*}
	else
	{
		common__quotaType *printQuotaInfo = soap_new_common__quotaType(soap,-1);	
		printQuotaInfo->Name = soap_new_std__string(soap,-1);
		*(printQuotaInfo->Name) = "print";
		printQuotaInfo->Enable = false;
		if(CCommonFunctions::GetNodeValueByRef(pNode, "printJobQuotaEnabled") == "1"||CCommonFunctions::GetNodeValueByRef(pNode, "printJobQuotaEnabled") == "true")
		{
			printQuotaInfo->Enable = true;
			printQuotaInfo->Balance = soap_new_std__string(soap,-1);
			*(printQuotaInfo->Balance) = CCommonFunctions::GetNodeValueByRef(pNode, "printJobRemainingQuantity " );
			printQuotaInfo->Quota = soap_new_std__string(soap,-1);
			*(printQuotaInfo->Quota) = CCommonFunctions::GetNodeValueByRef(pNode, "printJobDefaultQuantity " );
		}		
		printQuota->union_printQuotaCategoryType.PrintJobQuota  = printQuotaInfo;
	}*/

	DEBUGL8("end of UserServices::MapUserQuotaInfo\n");
	return printQuota;
}

common__userInfoType* UserServices::MapUserInfo(CString xpath,BOProxyRef &boProxy,soap* soap)
{
	DEBUGL8("start of UserServices::MapUserInfo\n");
	common__userInfoType *userInfo;
	userInfo = soap_new_common__userInfoType(soap,-1);
	NodeRef pNode = NULL;
	NodeRef pChildNode = NULL;
	boProxy->GetValue(xpath.c_str(),pNode);
	if(pNode)
	{
		userInfo->UserID = soap_new_std__string(soap,-1);
		*(userInfo->UserID) = ((ElementRef)pNode)->getAttribute("ID");
		
		pNode = CUIControllerAdapter::GetChildByName(pNode, "Information" );
		userInfo->Name = CCommonFunctions::GetNodeValueByRef(pNode, "name" );

		CString domain = CCommonFunctions::GetNodeValueByRef(pNode, "domainName" );
		if(false == domain.empty())
		{
			userInfo->Domain = soap_new_std__string(soap, -1);
			*(userInfo->Domain) = domain;
		}
		//Get PIN data
		CString sPIN = CCommonFunctions::GetNodeValueByRef(pNode, "PIN" );	
		if(false == sPIN.empty())
		{
			userInfo->PIN = soap_new_std__string(soap, -1);
			*(userInfo->PIN) = sPIN;
		}
		//WSS 1097 - added on 27-may-2013 
		//Get  oneTimeUser
		CString sEnableOneTimeUser = CCommonFunctions::GetNodeValueByRef(pNode, "oneTimeUser" );
		if(false == sEnableOneTimeUser.empty())
		{
			userInfo->EnableOneTimeUser = static_cast<bool*>(soap_malloc(soap,sizeof(bool)));
			*(userInfo->EnableOneTimeUser) = ("true" == sEnableOneTimeUser) ? true:false;
		}
		//Get CardID
		CString sCardID = CCommonFunctions::GetNodeValueByRef(pNode, "cardID" );
		if(false == sCardID.empty())
		{
			userInfo->CardID = soap_new_std__string(soap, -1);
			*(userInfo->CardID) = sCardID;
		}		
		CString sExternalUser = CCommonFunctions::GetNodeValueByRef(pNode, "isExternalUser" );
		if(false == sExternalUser.empty())
		{
			userInfo->ExternalUser = static_cast<bool*>(soap_malloc(soap,sizeof(bool)));
			*(userInfo->ExternalUser) = ("true" == sExternalUser || "1" == sExternalUser) ? true:false;
		}
		CString sServerType = CCommonFunctions::GetNodeValueByRef(pNode, "serverType" );
		if(sServerType.empty() == false)
		{
			userInfo->ServerType = static_cast<enum common__serverTypeNameType*>(soap_malloc(soap,sizeof(enum common__serverTypeNameType)));
			if(sServerType == "LOCAL")
				*(userInfo->ServerType) =common__serverTypeNameType__LOCAL;
			else if(sServerType == "WINDOWS")
				*(userInfo->ServerType) =common__serverTypeNameType__WINDOWS;
			else if(sServerType == "LDAP")
				*(userInfo->ServerType) =common__serverTypeNameType__LDAP;
			else
				DEBUGL2("UserServices::MapUserInfo invalid server type \n");
		}
		CString password = CCommonFunctions::GetNodeValueByRef(pNode, "passwd" );
		if(false == password.empty())
		{
			userInfo->Password = soap_new_std__string(soap, -1);
			*(userInfo->Password) = password;
		}
		
		CString firstName = CCommonFunctions::GetNodeValueByRef(pNode, "firstName" );
		if(false == firstName.empty())
		{
			userInfo->FirstName = soap_new_std__string(soap, -1);
			*(userInfo->FirstName) = firstName;
		}
		
		CString lastName = CCommonFunctions::GetNodeValueByRef(pNode, "lastName" );
		if(false == lastName.empty())
		{
			userInfo->LastName = soap_new_std__string(soap, -1);
			*(userInfo->LastName) = lastName;
		}
		
		std::vector<common__securityQAType*> vSecQA;
		NodeListRef pChildren = pNode->getChildNodes();
		for ( ulong i=0; i < pChildren->getLength(); i++)
		{
			NodeRef pChildNode= pChildren->item(i);
			if(pChildNode->getNodeName() != "Security")
				continue;
			common__securityQAType* secQA = soap_new_common__securityQAType(soap, -1);
			secQA->Question = CCommonFunctions::GetNodeValueByRef(pChildNode, "question" );
			secQA->Answer = CCommonFunctions::GetNodeValueByRef(pChildNode, "answer" );
	      		vSecQA.push_back(secQA);
		}		
		if(false == vSecQA.empty())
		      	userInfo->Security = vSecQA;
		
		CString emailID = CCommonFunctions::GetNodeValueByRef(pNode, "emailId" );
		if(false == emailID.empty())
		{
			userInfo->EMailId = soap_new_std__string(soap, -1);
			*(userInfo->EMailId) = emailID;
		}
		
		CString status = CCommonFunctions::GetNodeValueByRef(pNode, "userStatus" );
		if(false == status.empty())
		{
			userInfo->Status = soap_new_std__string(soap, -1);
			*(userInfo->Status) = status;
		}
				
		CString sKeyLayout = CCommonFunctions::GetNodeValueByRef(pNode, "userKeyBoard" );
		CString sPanelLanguage = CCommonFunctions::GetNodeValueByRef(pNode, "copierLanguage" );
		if(false == sPanelLanguage.empty() || false == sKeyLayout.empty())
		{
			userInfo->Preference = soap_new_common__preferenceType(soap, -1);
			if(false == sPanelLanguage.empty())
			{
				userInfo->Preference->PanelLanguage = soap_new_std__string(soap, -1);
				*(userInfo->Preference->PanelLanguage) = sPanelLanguage;
			}
			if(false == sKeyLayout.empty())
			{
				userInfo->Preference->PanelKeyboardLayout = soap_new_std__string(soap, -1);
				*(userInfo->Preference->PanelKeyboardLayout) = sKeyLayout;
			}
		}
				
		DEBUGL8("DepartmentCodeList\n");
		userInfo->Department  = soap_new_common__departmentDetailType(soap, -1);
		userInfo->Department  = UserServices::GetUserRelatedDepartments(xpath+"/Information",boProxy,soap);	
		DEBUGL8("RoleList\n");
		userInfo->RoleList = UserServices::GetSimilarChildNodes(xpath+"/Information/Roles",boProxy,soap);
		DEBUGL8("GroupList\n");
		userInfo->GroupList = UserServices::GetSimilarChildNodes(xpath+"/Information/Groups",boProxy,soap);
	
	}
	DEBUGL8("end of UserServices::MapUserInfo\n");
	return userInfo;
}

common__groupType* UserServices::MapGroupInfo(CString sXPath,const BOProxyRef& pBOProxy,soap* soap)
{
	DEBUGL8("start of UserServices::MapGroupInfo\n");
	common__groupType *pGroup = soap_new_common__groupType(soap,-1);
	NodeRef pNode = NULL;

	if(STATUS_OK != pBOProxy->GetValue(sXPath.c_str(),pNode) || !pNode)
	{
		DEBUGL1("GetValue failed\n");
		CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");
	}
	
	CString sID = ((ElementRef)pNode)->getAttribute("ID");
	if(false == sID.empty())
	{
		pGroup->GroupID = soap_new_std__string(soap,-1);
		*(pGroup->GroupID) = sID;
	}
	
	pGroup->Name = chelper::GetPathValue(pNode, "Information/name");
	
	DEBUGL8("RoleList\n");
	pGroup->RoleList = UserServices::GetSimilarChildNodes(sXPath + "/Information/Roles/CurrentRole", pBOProxy, soap);

	DEBUGL8("GroupList\n");
	pGroup->SubGroupList = UserServices::GetSubGroupList(sXPath + "/Information/Groups", pBOProxy, soap);
		
	DEBUGL8("end of UserServices::MapGroupInfo\n");
	return pGroup;
}

common__groupListType* UserServices::MapGroupListInfo(CString xpath,BOProxyRef &boProxy,soap* soap)
{
	DEBUGL8("start of UserServices::MapGroupListInfo\n");
	common__groupListType *groupList;
	groupList = soap_new_common__groupListType(soap,-1);
	NodeRef pNode = NULL;
	boProxy->GetValue(xpath.c_str(),pNode);
	NodeListRef pChildren = NULL;

	if(pNode)
	pChildren = pNode->getChildNodes();	
	vector<std::string>groups;
	for ( ulong i=0; i < pChildren->getLength(); i++)
	{
		NodeRef tempNode = pChildren->item(i);
		tempNode = CUIControllerAdapter::GetChildByName(tempNode,"Information");
		groups.push_back(CCommonFunctions::GetNodeValueByRef(tempNode,"name"));
	}		
	vector<std::string>::iterator it;
	for(it = groups.begin() ; it != groups.end(); it++)
	{
		DEBUGL8("UserServices::MapGroupListInfo :: retreiving info of group : %s\n",(*it).c_str());
		if(UserServices::ExecuteGetGroupInfoCommand(*it,boProxy) != STATUS_OK)
		continue;
		groupList->Group.push_back(UserServices::MapGroupInfo("GroupManager/Group",boProxy,soap));
	}
	DEBUGL8("end of UserServices::MapGroupListInfo\n");
	return groupList;
}
Status UserServices::SetSubGroupList(CString xPath,common__groupListType *subGroupList,BOProxyRef &boProxy)
{
	DEBUGL8("start of UserServices::SetSubGroupList\n");
	if(!boProxy) return STATUS_FAILED; 
	std::vector<CString> xPaths;
	std::vector<CString> values;


	std::vector<class common__groupType * >::iterator i;
	int j=1;
	for(i=subGroupList->Group.begin();i!=subGroupList->Group.end();i++,j++)
	{
		DEBUGL8("%s\n",((*i)->Name).c_str());
		values.push_back((*i)->Name);
		
		CString temp = xPath + "[@id=" + chelper::Itoa(j) + "]";
		xPaths.push_back(temp);


	}
	if(!values.empty())
	CUIControllerAdapter::SetXPathValues(xPaths, values, boProxy);
	
	DEBUGL8("end of UserServices::SetSubGroupList\n");
	return STATUS_OK;
}

common__groupListType* UserServices::GetSubGroupList(CString xPath,const BOProxyRef &boProxy,soap* soap)
{
	DEBUGL8("start of UserServices::GetSubGroupList\n");
	common__groupListType *subGroupList;
	subGroupList = soap_new_common__groupListType(soap,-1);
	if(!boProxy) return subGroupList; 

	common__stringListType* groupMemberList = UserServices::GetSimilarChildNodes(xPath,boProxy,soap);
	
	vector<string >::iterator iter = groupMemberList->Item.begin();
	vector<string >::iterator iterEnd = groupMemberList->Item.end();
	
	for(;iter != iterEnd ; iter++)
	{
		DEBUGL8("%s\n",iter->c_str());
		Status retStatus = UserServices::ExecuteGetGroupInfoCommand(*iter,boProxy);
		if(retStatus != STATUS_OK)
		break;
		subGroupList->Group.push_back(UserServices::MapGroupInfo("GroupManager/Group",boProxy,soap));
	}
	DEBUGL8("end of UserServices::GetSubGroupList\n");
	return subGroupList;
}

Status UserServices::ExecuteGetGroupInfoCommand(const CString& sGroupName,const BOProxyRef& pBOProxy,CString sGetType)
{
	DEBUGL8("UserServices::ExecuteGetGroupInfoCommand Enter\n");	

	vector<CString> vXPath;
	vector<CString> vValue;
	
	vValue.push_back(sGroupName);
	if(sGetType == "NAME")
		vXPath.push_back("GroupManager/Group/Information/name");
	else
		vXPath.push_back("GroupManager/Group/@ID");
	CUIControllerAdapter::SetXPathValues(vXPath, vValue, pBOProxy);

	vector<commandParam> vectParams;
	commandParam cmdParam;
	cmdParam.strParamValue = "GroupManager/Group";
	cmdParam.strParamName = "groupDetails";
	cmdParam.mapAttributes.insert(make_pair("contentType","XPath"));
	vectParams.push_back(cmdParam);
	cmdParam.strParamValue = (sGetType == "NAME") ? "NAME" : "ID";
	cmdParam.strParamName = "searchBy";
	cmdParam.mapAttributes.insert(make_pair("contentType","Value"));
	vectParams.push_back(cmdParam);


	// Execute the command
	CString strResponse;
	if(STATUS_OK != CUIControllerAdapter::SendCommand("GetGroupInfo",GROUPS_XPATH,vectParams,strResponse,pBOProxy))
	{
		DEBUGL1("Failed to SendCommand\n");
		CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");
	}
	
	// Get the result
	CString sOperationStatus = CUIControllerAdapter::GetOperationStatusString(strResponse);
	if(STG2_SUCCESS != sOperationStatus)
	{
		DEBUGL1("GetGroupInfo operation failed\n");
		CErrorCodeMapper::GenerateException(sOperationStatus,GRP_MGR);
	}
	
	DEBUGL8("UserServices::ExecuteGetGroupInfoCommand Exit\n");
	return STATUS_OK;
}

Status UserServices::SetUserInfo(common__userInfoType* pUserInfo,CString xpath,BOProxyRef &pBOProxy,const CString &cmdName)
{
	DEBUGL8("start of UserServices::SetUserInfo\n");
	//Get Authentication Data...
	CDeviceConfig::ExecuteGetSettings(pBOProxy);
	CString eMode = "";
	CString sOpenAccess = CCommonFunctions::GetValue("Authentication/AuthenticationSettings/openAccess",pBOProxy);
	if("0" == sOpenAccess || "false" == sOpenAccess)
	{
		eMode = "Disable";
	}
	else
	{
		CString sAuthMode = CCommonFunctions::GetValue("Authentication/AuthenticationSettings/AuthenticationType/authenticationName",pBOProxy);
		if("Local" == sAuthMode)
		{
			eMode = "Local";
		}
		else if("Ldap" == sAuthMode)
		{
			eMode = "Ldap";
		}
		else if("WindowsDomain" == sAuthMode)
		{
			eMode = "WindowsDomain";
		}
	}	
	vector<CString> xPathsVector;
	vector<CString> valuesVector;
	CString sUserInfoXPath = xpath + "/Information" ;
	//user id
	CString sID = (pUserInfo->UserID) ? *(pUserInfo->UserID): "";
	valuesVector.push_back(sID);
	xPathsVector.push_back(xpath+"/@ID");
	userDataStruct userData;	
	if( sID.empty() != true)
	{
		userData.sID = sID;
		GetUserData(pBOProxy,userData);
	}
	
	if(userData.IsBuiltInUser == false)
	{
	
		valuesVector.push_back(pUserInfo->Name);
		xPathsVector.push_back(sUserInfoXPath +"/name");
		CString sExternalUserEnable = "";	
		// Is External user should be set only during user creation
		if("AddUser" == cmdName && pUserInfo->ExternalUser)
		{
			sExternalUserEnable = (true == *(pUserInfo->ExternalUser)) ? "true":"false";
			valuesVector.push_back(sExternalUserEnable);
			xPathsVector.push_back(sUserInfoXPath +"/isExternalUser");
		}
		CString sServerType = "";
		// Is External user should be set only during user creation
		if("AddUser" == cmdName && pUserInfo->ServerType)
		{
			switch(*(pUserInfo->ServerType))
			{
				case common__serverTypeNameType__LOCAL:
					sServerType = "LOCAL";
					break;
				case common__serverTypeNameType__WINDOWS:
					sServerType = "WINDOWS";
					break;
				case common__serverTypeNameType__LDAP:
					sServerType = "LDAP";
					break;
				default:
					sServerType = "LOCAL";
					break;

			}
			valuesVector.push_back(sServerType);
			xPathsVector.push_back(sUserInfoXPath +"/serverType");
		}

		//domain
		(pUserInfo->Domain) ? valuesVector.push_back(*(pUserInfo->Domain)) : valuesVector.push_back("");
		xPathsVector.push_back(sUserInfoXPath + "/domainName");
		
		//firstName
		(pUserInfo->FirstName) ? valuesVector.push_back(*(pUserInfo->FirstName)) : valuesVector.push_back("");
		xPathsVector.push_back(sUserInfoXPath + "/firstName");
		
		//lastName
		(pUserInfo->LastName) ? valuesVector.push_back(*(pUserInfo->LastName)) : valuesVector.push_back("");
		xPathsVector.push_back(sUserInfoXPath + "/lastName");
		
		//emailId
		(pUserInfo->EMailId) ? valuesVector.push_back(*(pUserInfo->EMailId)) : valuesVector.push_back("");
		xPathsVector.push_back(sUserInfoXPath + "/emailId");
		
		
			
		// Role List
		if(pUserInfo->RoleList && false == pUserInfo->RoleList->Item.empty())
		{
			UserServices::SetSimilarChildNodes(pUserInfo->RoleList, sUserInfoXPath + "/Roles/role",pBOProxy);
		}
		else
		{
			xPathsVector.push_back(sUserInfoXPath + "/Roles");
			valuesVector.push_back("");		
		}
		
		// GroupList
		if(pUserInfo->GroupList && false == pUserInfo->GroupList->Item.empty())
		{
			DEBUGL1("Group list exist\n");
			UserServices::SetSimilarChildNodes(pUserInfo->GroupList, sUserInfoXPath + "/Groups/group",pBOProxy);
		}
		else
		{
			xPathsVector.push_back(sUserInfoXPath + "/Groups");
			valuesVector.push_back("");	
		}
			
		// Not supported hence commented.
		/* for(unsigned int i =0; i < pUserInfo->Security.size();i++)
		{
			CString temp = sUserInfoXPath +"Security[@number=" + string_cast(i) + "]";
			valuesVector.push_back(pUserInfo->Security[0]->Question);
			xPathsVector.push_back(temp+"/question");

			valuesVector.push_back(pUserInfo->Security[0]->Answer);
			xPathsVector.push_back(temp+"/answer");
		} */
		//For External User PIN Settings not allowed.
		
		//if(pUserInfo->PIN && (("AddUser" == cmdName && (sServerType == "LOCAL" || (sServerType.empty()== true && sExternalUserEnable == "false")))||("AddUser" != cmdName && userData.IsExternalUser == false)))
		
		//Fixed done for Pin setting issue
		bool sSavePINSetting = false;
		if(sServerType == "LOCAL" ||(sServerType.empty() && sExternalUserEnable == "false"))
		{
			sSavePINSetting= true;
		}
		if(sExternalUserEnable.empty() && sServerType.empty())
		{
			if(eMode == "Local"|| eMode == "Disable")
				sSavePINSetting=true;
			else
				sSavePINSetting =false;
		}	
		if(pUserInfo->PIN && (("AddUser" == cmdName && sSavePINSetting)|| ("AddUser" != cmdName && userData.IsExternalUser == false ) ))
		{
			DEBUGL4(" Setting Pin info....\n");
			CString sPIN = *(pUserInfo->PIN);
			valuesVector.push_back(sPIN);
			xPathsVector.push_back(sUserInfoXPath + "/PIN");
		}
		
		//"EnableOneTimeUser"  is available only while creating user for a local user
		if(pUserInfo->EnableOneTimeUser && "AddUser" == cmdName)
		{	
			CString sEnableOneTimeUser = *(pUserInfo->EnableOneTimeUser) ? "true" : "false";
			DEBUGL8("UserServices::SetUserInfo EnableOneTimeUser = %s \n ",sEnableOneTimeUser.c_str());
			if(sServerType == "LOCAL" || (sServerType.empty()== true && sExternalUserEnable == "false"))//Check if creating local user
			{
				valuesVector.push_back(sEnableOneTimeUser);
				xPathsVector.push_back(sUserInfoXPath + "/oneTimeUser");
			}
			else//remote server user
			{
				if(sEnableOneTimeUser == "true")
				{
					DEBUGL1("UserServices::SetUserInfo EnableOneTimeUser is sent for external user\n");
					CErrorCodeMapper::GenerateException("STATUS_INVALID_INPUT");	
				}
				else
				{
					valuesVector.push_back(sEnableOneTimeUser);
					xPathsVector.push_back(sUserInfoXPath + "/oneTimeUser");
				}
			}
		}
		//"CardID"  is available only while creating user for a local user
		if(pUserInfo->CardID)
		{	
			CString sCardID = *(pUserInfo->CardID);
			DEBUGL8("UserServices::SetUserInfo sCardID = %s  , sServerType = %s , sExternalUserEnable = %s\n ",sCardID.c_str(),sServerType.c_str(),sExternalUserEnable.c_str());
			if(	sCardID.empty() && "AddUser" == cmdName)
			{
				DEBUGL1("UserServices::SetUserInfo CardID is empty  while creating user\n");
				CErrorCodeMapper::GenerateException("STG2_INVALID_CARD_ID");
			}

			if(sServerType == "LOCAL" || (sServerType.empty()== true && sExternalUserEnable == "false") || (("AddUser" != cmdName && userData.IsExternalUser == false)))//Check if creating/editing local user 
			{
				valuesVector.push_back(sCardID);
				xPathsVector.push_back(sUserInfoXPath + "/cardID");
			}
		}
	}
	// Departments
	if(pUserInfo->Department && false == pUserInfo->Department->DepartmentCodeList->Item.empty())
	{
		SetUserRelatedDepartments(pUserInfo->Department, sUserInfoXPath ,pBOProxy);		
	}
	else
	{
		xPathsVector.push_back(sUserInfoXPath + "/Departments");
		valuesVector.push_back("");
	}

	// Password
	if(pUserInfo->Password)
	{
		valuesVector.push_back(*(pUserInfo->Password));
		xPathsVector.push_back(sUserInfoXPath + "/passwd");
	}


	//copier language
	xPathsVector.push_back(sUserInfoXPath + "/copierLanguage");
	(pUserInfo->Preference) && (pUserInfo->Preference->PanelLanguage) ? valuesVector.push_back(*(pUserInfo->Preference->PanelLanguage)) : valuesVector.push_back("");
		
	//userKeyBoard
	xPathsVector.push_back(sUserInfoXPath + "/userKeyBoard");
	(pUserInfo->Preference) && (pUserInfo->Preference->PanelKeyboardLayout) ? valuesVector.push_back(*(pUserInfo->Preference->PanelKeyboardLayout)) : valuesVector.push_back("");
	DEBUGL8("UserServices::SetUserInfo : calling SetXPathValues\n");

	CUIControllerAdapter::SetXPathValues(xPathsVector, valuesVector, pBOProxy);

	DEBUGL8("end of UserServices::SetUserInfo\n");
	return STATUS_OK;
}

Status	UserServices::SetSimilarChildNodes(common__stringListType* stringList,CString xpath,BOProxyRef &boProxy)
{
	DEBUGL8("UserServices::SetSimilarChildNodes enter\n");
	if(!boProxy || !stringList) return STATUS_FAILED; 

	std::vector<CString> vXPath;
	std::vector<CString> vValue;

	vector<string>::iterator iter = stringList->Item.begin();
	vector<string>::iterator iterEnd = stringList->Item.end();
	int j = 1;
	for(; iter != iterEnd; j++, ++iter)
	{
		vValue.push_back(*iter);
		
		CString temp = xpath + "[@id=" + string_cast(j) + "]";
		vXPath.push_back(temp);
	}
	if(!vValue.empty())
	CUIControllerAdapter::SetXPathValues(vXPath, vValue, boProxy);
	DEBUGL8("UserServices::SetSimilarChildNodes exit\n");
	return STATUS_OK;

}
CString	UserServices::SetUserRelatedDepartments(common__departmentDetailType *pDepartment,CString sXPath ,BOProxyRef& pBOProxy)
{
	DEBUGL8("UserServices::SetUserRelatedDepartments enter\n");
	
	vector<CString> vXPath;
	vector<CString> vValue;
	if(pDepartment->DepartmentCodeList)
	{
		// If the list cotains duplicate department code then remove the duplicate department code using sort and unique.
		//Here, unique returns iterator to the last item of the vector containing non-duplicate department codes. 
		sort(pDepartment->DepartmentCodeList->Item.begin(), pDepartment->DepartmentCodeList->Item.end());
		vector<string>::iterator lastItem = unique(pDepartment->DepartmentCodeList->Item.begin(), pDepartment->DepartmentCodeList->Item.end());
		
		
		vector<string >::iterator iter = pDepartment->DepartmentCodeList->Item.begin();
		int j = 1;		
		for(;iter != lastItem ; j++, iter++)
		{
			CString sOperationStatus = ExecuteSearchDepartmentCommand(*iter ,pBOProxy);
			if(STG2_SUCCESS != sOperationStatus)
			{
				 // pass billing code to plugin. Since we could not get department ID and departmentName, set them to dummy.
				vXPath.push_back(sXPath + "/Departments/Department[@priority=" + chelper::Itoa(j) + "]/departmentCode");
				vValue.push_back(*iter);								
				vXPath.push_back(sXPath + "/Departments/Department[@priority=" + chelper::Itoa(j) + "]/departmentID");
				vValue.push_back("dummy");			
				vXPath.push_back(sXPath + "/Departments/Department[@priority=" + chelper::Itoa(j) + "]/departmentName");
				vValue.push_back("dummy");
				
			} 
			else
			{
				vXPath.push_back(sXPath + "/Departments/Department[@priority=" + chelper::Itoa(j) + "]/departmentCode");
				vValue.push_back(*iter);															
				vXPath.push_back(sXPath + "/Departments/Department[@priority=" + chelper::Itoa(j) + "]/departmentID");
				vValue.push_back(CUIControllerAdapter::GetTextNodeValue("Accounting/BillingCodes/BillingCode/@ID",pBOProxy));			
				vXPath.push_back(sXPath + "/Departments/Department[@priority=" + chelper::Itoa(j) + "]/departmentName");
				vValue.push_back(CUIControllerAdapter::GetTextNodeValue("Accounting/BillingCodes/BillingCode/name",pBOProxy));	
			
				
				
			}
		}
	}
	else
	{
		vValue.push_back("");
		vXPath.push_back(sXPath + "/Departments");
	}

	if(!vValue.empty())
		CUIControllerAdapter::SetXPathValues(vXPath, vValue, pBOProxy);
	DEBUGL8("UserServices::SetUserRelatedDepartments exit\n");
	return STG2_SUCCESS;

}


/* Status UserServices::ExecuteGetUserAccountingInfo(ci::operatingenvironment::CString sID,al::uicontroller::BOProxyRef &boProxy)
{
	DEBUGL8("UserServices::getUserIdUsingName : User ID : %s \n", sID.c_str());
	if(sID == "" || sID.empty())
	{
		DEBUGL1("ID empty returning STATUS_FAILED\n");
		return STATUS_FAILED;
	}
	std::vector<CString> xPaths;
	std::vector<CString> values;

	values.push_back(sID);
	xPaths.push_back(CString(ACCOUNTING_USER_ID_PATH)+"/@ID");

	CUIControllerAdapter::SetXPathValues(xPaths, values,boProxy);

	vector<commandParam> vectParams;
	commandParam cmdParam;
	cmdParam.strParamValue = ACCOUNTING_USER_ID_PATH;
	cmdParam.strParamName = "userDetails";
	cmdParam.mapAttributes.insert(make_pair("contentType","XPath"));
	vectParams.push_back(cmdParam);

	// Execute the command
	CString strResponse = "";
	CUIControllerAdapter::SendCommand("GetUserAccountingInfo", ACCOUNTING_COMMANDNODE_XPATH,vectParams,strResponse,boProxy);
	DEBUGL8("UserServices::getUserIdUsingName SendCommand returned %s\n",strResponse.c_str());
	// Get the result
	Status execCommandStatus = CUIControllerAdapter::GetOperationStatus(strResponse);

	DEBUGL8("UserServices::getUserIdUsingName exit\n");

	return execCommandStatus;

} */
Status UserServices::SetColourQuotaInfo( common__colorJobQuotaType *colorJobQuota,BOProxyRef &boProxy,CString ownerType)
{
	DEBUGL8("UserServices::SetColourQuotaInfo enter \n");
	if(!boProxy || !colorJobQuota)
	{
		return STATUS_FAILED;
	}
	Status ret =  STATUS_OK;
	CString strXML ; 
	strXML += ownerType == "User" ? "<UserManager><View><NewUser><Information><AccountingProperty><Quota>" : "<Accounting><BillingCodes><BillingCode><Quota>";
	if(colorJobQuota->ColorQuota)
	{
		if(colorJobQuota->ColorQuota->Quota)
		strXML +="<fullColorDefaultQuantity>"+ CCommonFunctions::EscapedXMLString(*(colorJobQuota->ColorQuota->Quota)) +"</fullColorDefaultQuantity>";
	  if(colorJobQuota->ColorQuota->Balance)
		strXML +="<fullColorRemainingQuantity>"+ CCommonFunctions::EscapedXMLString(*(colorJobQuota->ColorQuota->Balance)) +"</fullColorRemainingQuantity>";
		strXML +="<fullColorQuotaEnabled>";
		strXML += (colorJobQuota->ColorQuota->Enable) ? "1":"0";
		strXML +="</fullColorQuotaEnabled>";
	}
	if(colorJobQuota->BlackQuota)
	{
	  if(colorJobQuota->BlackQuota->Quota)
		strXML +="<blackDefaultQuantity>"+ CCommonFunctions::EscapedXMLString(*(colorJobQuota->BlackQuota->Quota)) +"</blackDefaultQuantity>";
	  if(colorJobQuota->BlackQuota->Balance)
		strXML +="<blackRemainingQuantity>"+ CCommonFunctions::EscapedXMLString(*(colorJobQuota->BlackQuota->Balance)) +"</blackRemainingQuantity>";
		strXML +="<blackQuotaEnabled>";
		strXML += (colorJobQuota->BlackQuota->Enable) ? "1":"0";
		strXML +="</blackQuotaEnabled>";
	}
	strXML += ownerType == "User" ? "</Quota></AccountingProperty></Information></NewUser></View></UserManager>" : "</Quota></BillingCode></BillingCodes></Accounting>";
	
	CUIControllerAdapter::SetXMLValue("", strXML, boProxy);
	
	DEBUGL8("UserServices::SetColourQuotaInfo exit \n");
	return ret;

}
Status UserServices::SetTotalQuotaInfo( common__quotaType *jobQuota,BOProxyRef &boProxy,CString ownerType)
{
	DEBUGL8("UserServices::SetTotalQuotaInfo enter \n");
	if(!boProxy)
	return STATUS_FAILED;
	Status ret =  STATUS_OK;
	CString strXML;
	strXML+= ownerType == "User" ? "<UserManager><View><NewUser><Information><AccountingProperty><Quota>" : "<Accounting><BillingCodes><BillingCode><Quota>";
	if(jobQuota)
	{
	  if(jobQuota->Quota)
		strXML +="<jobDefaultQuantity>"+ CCommonFunctions::EscapedXMLString(*(jobQuota->Quota)) +"</jobDefaultQuantity>";
	  if(jobQuota->Balance)
		strXML +="<jobRemainingQuantity>"+ CCommonFunctions::EscapedXMLString (*(jobQuota->Balance)) +"</jobRemainingQuantity>";
	  strXML +="<jobQuotaEnabled>";
	  strXML += (jobQuota->Enable) ? "1":"0";
	   strXML +="</jobQuotaEnabled>";
	}
	strXML += ownerType == "User" ? "</Quota></AccountingProperty></Information></NewUser></View></UserManager>" : "</Quota></BillingCode></BillingCodes></Accounting>";
	
	CUIControllerAdapter::SetXMLValue("", strXML, boProxy);
	
	DEBUGL8("UserServices::SetTotalQuotaInfo exit \n");
	return ret;

}
Status UserServices::SetPrintQuotaInfo( common__quotaType *printJobQuota,BOProxyRef &boProxy,CString ownerType)
{
	DEBUGL8("UserServices::SetPrintQuotaInfo enter \n");
	if(!boProxy)
	return STATUS_FAILED;
	Status ret =  STATUS_OK;
	CString strXML;
	strXML += ownerType == "User" ? "<UserManager><View><NewUser><Information><AccountingProperty><Quota>" : "<Accounting><BillingCodes><BillingCode><Quota>";
	if(printJobQuota)
	{
		if(printJobQuota->Quota)
			strXML +="<printJobDefaultQuantity>"+ CCommonFunctions::EscapedXMLString(*(printJobQuota->Quota)) +"</printJobDefaultQuantity>";
	  	if(printJobQuota->Balance)
			strXML +="<printJobRemainingQuantity>"+ CCommonFunctions::EscapedXMLString(*(printJobQuota->Balance)) +"</printJobRemainingQuantity>";
		strXML +="<printJobQuotaEnabled>";
		strXML += (printJobQuota->Enable) ? "1":"0";
		strXML +="</printJobQuotaEnabled>";
	}
	strXML += ownerType == "User" ? "</Quota></AccountingProperty></Information></NewUser></View></UserManager>" : "</Quota></BillingCode></BillingCodes></Accounting>";
	
	CUIControllerAdapter::SetXMLValue("", strXML, boProxy);
	
	DEBUGL8("UserServices::SetPrintQuotaInfo exit \n");
	return ret;

}
Status UserServices::ExecuteModifyDeptQuotaInfo(CString &sID,CString &sDeptName,CString &sDeptCode,BOProxyRef &boProxy)
{
	DEBUGL8("UserServices::ExecuteModifyDeptQuotaInfo : User ID : %s \n", sID.c_str());

	std::vector<CString> xPaths;
	std::vector<CString> values;

	values.push_back(sID);
	xPaths.push_back(CString(DEPARTMENT_BILLINGCODE_PATH)+"/@ID");
	values.push_back(sDeptName);
	xPaths.push_back(CString(DEPARTMENT_BILLINGCODE_PATH)+"/name");
	values.push_back(sDeptCode);
	xPaths.push_back(CString(DEPARTMENT_BILLINGCODE_PATH)+"/code");

	CUIControllerAdapter::SetXPathValues(xPaths, values,boProxy);
	
	vector<commandParam> vectParams;
	commandParam cmdParam;
	cmdParam.strParamValue = DEPARTMENT_BILLINGCODE_PATH;
	cmdParam.strParamName = "billingCodeDetails";
	cmdParam.mapAttributes.insert(make_pair("contentType","XPath"));
	vectParams.push_back(cmdParam);

	// Execute the command
	CString strResponse = "";
	CUIControllerAdapter::SendCommand("ModifyBillingCode", DEPARTMENT_COMMANDNODE_XPATH,vectParams,strResponse,boProxy);
	DEBUGL8("UserServices::ExecuteModifyDeptQuotaInfo SendCommand returned %s\n",strResponse.c_str());
	// Get the result
	CString sOperationStatus = CUIControllerAdapter::GetOperationStatusString(strResponse);
	if(STG2_SUCCESS != sOperationStatus)
	{
		DEBUGL1("UserServices::ExecuteModifyDeptQuotaInfo : Failed to SendCommand\n");
		CErrorCodeMapper::GenerateException(sOperationStatus, ACC_MGR);
	}		
	DEBUGL8("UserServices::ExecuteModifyDeptQuotaInfo exit \n");
	return STATUS_OK;
}
Status UserServices::ExecuteModifyUserQuotaInfo(const CString &sID,BOProxyRef &boProxy)
{
	DEBUGL8("UserServices::ExecuteModifyUserQuotaInfo : User ID : %s\n", sID.c_str());


	std::vector<CString> xPaths;
	std::vector<CString> values;

	values.push_back(sID);
	xPaths.push_back(CString(USERMANAGER_NEW_USER)+"/@ID");
	CUIControllerAdapter::SetXPathValues(xPaths, values,boProxy);
	
	vector<commandParam> vectParams;
	commandParam cmdParam;
	cmdParam.strParamValue = USERMANAGER_NEW_USER;
	cmdParam.strParamName = "userDetails";
	cmdParam.mapAttributes.insert(make_pair("contentType","XPath"));
	vectParams.push_back(cmdParam);
	
	
	cmdParam.strParamValue = "ID";
	cmdParam.strParamName = "searchBy";
	cmdParam.mapAttributes.insert(make_pair("contentType","Value"));
	vectParams.push_back(cmdParam);
	
	// Execute the command
	CString strResponse = "";
	CUIControllerAdapter::SendCommand("UpdateUserInfo",USERS_XPATH,vectParams,strResponse,boProxy);
	CString sOperationStatus = CUIControllerAdapter::GetOperationStatusString(strResponse);
	if(STG2_SUCCESS != sOperationStatus)
	{
		DEBUGL1("UserServices::ExecuteModifyUserQuotaInfo : Failed to SendCommand\n");
		CErrorCodeMapper::GenerateException(sOperationStatus, USR_MGR);
	}		
	DEBUGL8("UserServices::ExecuteModifyUserQuotaInfo exit \n");
	return STATUS_OK;
}
/* Status UserServices::ExecuteRegisterUserAccountingInfoCommand(CString sID,BOProxyRef &boProxy)
{
	DEBUGL8("UserServices::ExecuteRegisterUserAccountingInfoCommand : User ID : %s \n", sID.c_str());

	std::vector<CString> xPaths;
	std::vector<CString> values;


	values.push_back("0");
	xPaths.push_back(CString(ACCOUNTING_USER_QUOTA_PATH)+"[@ID ='" + sID +"']/fullColorDefaultQuantity");
	values.push_back("0");
	xPaths.push_back(CString(ACCOUNTING_USER_QUOTA_PATH)+"[@ID ='" + sID +"']/fullColorRemainingQuantity");
	values.push_back("0");
	xPaths.push_back(CString(ACCOUNTING_USER_QUOTA_PATH)+"[@ID ='" + sID +"']/fullColorQuotaEnabled");
	values.push_back("0");
	xPaths.push_back(CString(ACCOUNTING_USER_QUOTA_PATH)+"[@ID ='" + sID +"']/blackDefaultQuantity");
	values.push_back("0");
	xPaths.push_back(CString(ACCOUNTING_USER_QUOTA_PATH)+"[@ID ='" + sID +"']/blackRemainingQuantity");
	values.push_back("0");
	xPaths.push_back(CString(ACCOUNTING_USER_QUOTA_PATH)+"[@ID ='" + sID +"']/blackQuotaEnabled");

	values.push_back(sID);
	xPaths.push_back(CString(ACCOUNTING_USER_ID_PATH)+"/@ID");
	CUIControllerAdapter::SetXPathValues(xPaths, values,boProxy);
	
	vector<commandParam> vectParams;
	commandParam cmdParam;
	cmdParam.strParamValue = ACCOUNTING_USER_ID_PATH;
	cmdParam.strParamName = "userQuotaDetails";
	cmdParam.mapAttributes.insert(make_pair("contentType","XPath"));
	vectParams.push_back(cmdParam);

	// Execute the command
	CString strResponse = "";
	CUIControllerAdapter::SendCommand("RegisterUserAccountingInfo", ACCOUNTING_COMMANDNODE_XPATH,vectParams,strResponse,boProxy);
	DEBUGL8("UserServices::ExecuteRegisterUserAccountingInfoCommand SendCommand returned %s\n",strResponse.c_str());
	// Get the result
	Status execCommandStatus = CUIControllerAdapter::GetOperationStatus(strResponse);

	DEBUGL8("UserServices::ExecuteRegisterUserAccountingInfoCommand exit \n");
	return execCommandStatus;
} */

Status UserServices::ExecuteGetGroupListCommand(const BOProxyRef &boProxy)
{
	DEBUGL8("UserServices::ExecutGetGroupListCommand enter\n");
	
	std::vector<CString> xPaths;
	std::vector<CString> values;

	values.push_back("");
	xPaths.push_back(GROUPS_XPATH);

	CUIControllerAdapter::SetXPathValues(xPaths, values, boProxy);

	vector<commandParam> vectParams;
	commandParam cmdParam;
	cmdParam.strParamValue = GROUPS_XPATH;
	cmdParam.strParamName = "groupDetails";
	cmdParam.mapAttributes.insert(make_pair("contentType","XPath"));
	vectParams.push_back(cmdParam);

	// Execute the command
	CString strResponse = "";
	if(STATUS_OK != CUIControllerAdapter::SendCommand("GetGroups",GROUPS_XPATH,vectParams,strResponse,boProxy))
	{
		DEBUGL1("Failed to SendCommand\n");
		CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");
	}
	
	CString sOperationStatus = CUIControllerAdapter::GetOperationStatusString(strResponse);
	if (STG2_SUCCESS != sOperationStatus) 
	{
		DEBUGL1("GetGroups command failed\n");
		CErrorCodeMapper::GenerateException(sOperationStatus, GRP_MGR);
	}
	DEBUGL8("UserServices::ExecutGetGroupListCommand exit \n");
	return STATUS_OK;
}
Status UserServices::ExecuteGetUserListCommand(const BOProxyRef &boProxy,	bool bAllData)
{
	DEBUGL8("UserServices::ExecuteGetUserListCommand enter\n");
	
	std::vector<CString> xPaths;
	std::vector<CString> values;
	
	values.push_back("");
	xPaths.push_back(USERS_XPATH);
	

	CUIControllerAdapter::SetXPathValues(xPaths, values, boProxy);

	vector<commandParam> vectParams;
	commandParam cmdParam;
	cmdParam.strParamValue = "UserManager";
	cmdParam.strParamName = "userDetails";
	cmdParam.mapAttributes.insert(make_pair("contentType","XPath"));
	vectParams.push_back(cmdParam);
	if(!bAllData)
	{
		commandParam cmdDetails;
		cmdDetails.strParamValue = "FEW";
		cmdDetails.strParamName = "cmdDetails";
		cmdDetails.mapAttributes.insert(make_pair("contentType","Value"));
		vectParams.push_back(cmdDetails);
	}
	// Execute the command
	CString strResponse;
	if(STATUS_OK != CUIControllerAdapter::SendCommand("GetUsers",USERS_XPATH,vectParams,strResponse,boProxy))
	{
		DEBUGL1("Failed to SendCommand\n");
		CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");
	
	}
	CString sOperationStatus  = CUIControllerAdapter::GetOperationStatusString(strResponse);
	if(STG2_SUCCESS != sOperationStatus)
	{
		CErrorCodeMapper::GenerateException(sOperationStatus,USR_MGR);
	}
	
	DEBUGL8("UserServices::ExecuteGetUserListCommand exit \n");
	return STATUS_OK;
}

#ifdef STAGE2_UNSUPPORTED
user__userListType* UserServices::GetAllUser(BOProxyRef &boProxy,soap *soap)
{
	DEBUGL8("UserServices::GetAllUser enter\n");
	user__userListType *userList = soap_new_user__userListType(soap,-1);

	if(STATUS_OK != ExecuteGetUserListCommand(boProxy,false))
	{
		CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");	
	}
	
	NodeRef pNode = NULL;
	if(STATUS_OK != boProxy->GetValue(USERS_XPATH, pNode) || !pNode)
	{
		CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");
	}

	NodeListRef pChildren = pNode->getChildNodes();		
	
	vector<CString> usersList;
	for ( ulong i=0; i < pChildren->getLength(); i++)
	{
		CString userXpath = CString(USERS_XPATH) + "/User[" + string_cast(i+1) + "]/Information/name";
		NodeRef pName = NULL;
		if(STATUS_OK != boProxy->GetValue(userXpath.c_str(),pName))
		{	
			DEBUGL1("UserServices::GetAllUser Failed in GetValue!!!\n");
			CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");
		}
		CString userName = pName->getTextContent();
		DEBUGL8("GetAllUser :: Fetching User info from xpath %s UserName - %s\n",userXpath.c_str(),userName.c_str());
		usersList.push_back(userName);
	}		
	for(ulong j= 0;j<usersList.size();j++)
	{
		if(usersList[j] == "Undefined")
		{
			if(STG2_SUCCESS != UserServices::ExecuteSearchUserByUserName(usersList[j],boProxy))
			{
				CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");
			}		
		}
		else
		{
			CString sOperationStatus = ExecuteGetUserCommand(usersList[j],boProxy, false);
			if(STG2_SUCCESS != sOperationStatus)
			{
				CErrorCodeMapper::GenerateException(sOperationStatus,USR_MGR);
			}				
		}
		userList->User.push_back(UserServices::GetUser("UserManager/Users/User",boProxy,soap));
	}
	DEBUGL8("UserServices::GetAllUser exit\n");
	return userList;
}
user__userType* UserServices::GetUser(CString userPath,BOProxyRef &boProxy,soap *soap)
{
	DEBUGL8("UserServices::GetUser enter\n");
	user__userType *user = soap_new_user__userType(soap,-1);

	user->UserInfo = UserServices::MapUserInfo(userPath,boProxy,soap);
	// handling of ConfigurationInfo :: gets quota info alone
	user->ConfigurationInfo = soap_new_user__configurationInfoType(soap,-1);
	user->ConfigurationInfo->Quota = soap_new_common__quotaCategoryType(soap,-1);
	user->ConfigurationInfo->Quota->Job = soap_new_common__jobQuotaCategoryType(soap,-1);
	user->ConfigurationInfo->Quota->Job->Print  = soap_new_common__printQuotaCategoryType(soap,-1);
	user->ConfigurationInfo->Quota->Job->Print  = UserServices::MapUserQuotaInfo(userPath+CString("/Information/AccountingProperty/Quota"),boProxy,soap);

	//handling of OwnershipInfo 
	user->OwnershipInfo = getOwnershipInfo(soap ,boProxy, user->UserInfo->Name);
	DEBUGL8("UserServices::GetUser exit \n");
	return user;
}


CString UserServices::ModifyUserInfo(user__userType *User,BOProxyRef &boProxy,const CString cmdName)
{
	DEBUGL8("UserServices::ModifyUserInfo enter\n");
	
	CString sResetXML = "<ResetValue>";
	sResetXML += "<path>";
	sResetXML += USERMANAGER_NEW_USER;
	sResetXML += "</path>";
	sResetXML += "</ResetValue>";
	boProxy->ResetValue(sResetXML.c_str(),eDefaultValue);
	Status execCommandStatus = STATUS_OK;
	if(User->ConfigurationInfo)
	if(User->ConfigurationInfo->Quota)
	if(User->ConfigurationInfo->Quota->Job)
	if(User->ConfigurationInfo->Quota->Job->Print)
	{
		NodeRef pQuotaType = NULL;
		boProxy->GetValue("Controller/Settings/QuotaSetting/Print/Type",pQuotaType);
		if(!pQuotaType)
		{
			CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");
		}
		
		CString quotaType = pQuotaType->getTextContent();
		
		if(quotaType == "BlackOrColorQuota" && User->ConfigurationInfo->Quota->Job->Print->__union_printQuotaCategoryType == SOAP_UNION__common__union_printQuotaCategoryType_PrintJobQuota)
		{
			CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");
		}	
		if(quotaType == "PrintJobQuota" && User->ConfigurationInfo->Quota->Job->Print->__union_printQuotaCategoryType == SOAP_UNION__common__union_printQuotaCategoryType_ColorJobQuota)
		{	
			CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");
		}	
		if(User->ConfigurationInfo->Quota->Job->Print->__union_printQuotaCategoryType == SOAP_UNION__common__union_printQuotaCategoryType_ColorJobQuota)
			execCommandStatus = UserServices::SetColourQuotaInfo(User->ConfigurationInfo->Quota->Job->Print->union_printQuotaCategoryType.ColorJobQuota,boProxy,"User");
		else
			execCommandStatus = UserServices::SetPrintQuotaInfo(User->ConfigurationInfo->Quota->Job->Print->union_printQuotaCategoryType.PrintJobQuota,boProxy,"User");
		if(STATUS_OK != execCommandStatus)		
		{
			CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");
		}	
	}
	
	return UserServices::ExecuteCreateOrEditUserCommand(User->UserInfo,cmdName,boProxy);
}
#endif

CString UserServices::ExecuteCreateOrEditUserCommand(common__userInfoType *pUserInfo, CString sCommandName, BOProxyRef& pBOProxy)
{
	DEBUGL8("UserServices::CreateOrEditUser enter\n");
	
	CString sXPath = USERMANAGER_NEW_USER; 
	
	SetUserInfo(pUserInfo, sXPath , pBOProxy,sCommandName);
	
	vector<commandParam> vectParams;
	commandParam cmdParam;
	cmdParam.strParamValue = sXPath;
	cmdParam.strParamName = "userDetails";
	cmdParam.mapAttributes.insert(make_pair("contentType","XPath"));
	vectParams.push_back(cmdParam);

	if("UpdateUserInfo" == sCommandName)
	{
		cmdParam.strParamValue = "ID";
		cmdParam.strParamName = "searchBy";
		cmdParam.mapAttributes.insert(make_pair("contentType","Value"));
		vectParams.push_back(cmdParam);
	}
	
	// Execute the command
	CString strResponse;
	if(STATUS_OK != CUIControllerAdapter::SendCommand(sCommandName,"UserManager/Users",vectParams,strResponse,pBOProxy))
	{
		DEBUGL1("Failed to SendCommand\n");
		CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");
	}
	
	CString sOperationStatus = CUIControllerAdapter::GetOperationStatusString(strResponse);
	DEBUGL8("UserServices::CreateOrEditUser exit \n");
	return sOperationStatus;
}

Status	UserServices::CompareAndSetSimilarNodes(common__stringListType* setStringList,common__stringListType* refStringList,CString xpath,BOProxyRef &boProxy)
{

	DEBUGL8("UserServices::CompareAndSetSimilarNodes enter\n");
	if(!boProxy || !setStringList) return STATUS_FAILED; 

	std::vector<CString> vXPath;
	std::vector<CString> vValue;

	vector<string>::iterator iter = setStringList->Item.begin();
	vector<string>::iterator iterEnd = setStringList->Item.end();
	vector<string>::iterator refIterEnd = refStringList->Item.end();
	
	int j = 1;
	for(; iter != iterEnd; ++iter)
	{
		vector<string>::iterator refIter = refStringList->Item.begin();
		for(;refIter!=refIterEnd;refIter++)
		{
			if(*refIter == *iter)
			{
				DEBUGL5("UserServices::CompareAndSetSimilarNodes found %s present in prev list \n",(*iter).c_str());
				break;
			}
		}
		if(refIter != refIterEnd)
			continue;
	
		vValue.push_back(*iter);
		
		CString temp = xpath + "[@id=" + string_cast(j) + "]";
		vXPath.push_back(temp);
		j++;
	}
	if(!vValue.empty())
	CUIControllerAdapter::SetXPathValues(vXPath, vValue, boProxy);
	DEBUGL8("UserServices::CompareAndSetSimilarNodes exit\n");
	return STATUS_OK;

}
CString UserServices::ExecuteCreateOrEditGroupCommand(common__groupType *pGroup, CString cmdName, BOProxyRef& pBOProxy,soap *soap)
{
	DEBUGL8("UserServices::ExecuteCreateOrEditGroupCommand enter\n");
		
	std::vector<CString> xPaths;
	std::vector<CString> values;

   if(pGroup->Name.empty())
	{
		DEBUGL1("Group name is empty \n");
		CErrorCodeMapper::GenerateException("STG2_EMPTY_GROUP_NAME");
	}
		

	values.push_back(pGroup->Name);
	CString sNewGroupXPath = CString(GROUPS_XPATH) + "/NewGroup";
	xPaths.push_back(sNewGroupXPath + "/Information/name");

    common__stringListType *RoleList = NULL;
	if("UpdateGroup" == cmdName)
	{
		if(pGroup->GroupID)
		{
			values.push_back(*(pGroup->GroupID));
			xPaths.push_back(sNewGroupXPath + "/@ID");
			//Get Current Roles in order to remove them
			if(STATUS_OK != ExecuteGetGroupInfoCommand(*(pGroup->GroupID),pBOProxy,"ID"))
			{
				DEBUGL1("ExecuteGetGroupInfoCommand failed\n");
				CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");
			}
			
			RoleList = UserServices::GetSimilarChildNodes( "GroupManager/Group/Information/Roles/CurrentRole", pBOProxy, soap);
			if(pGroup->RoleList)
			{
				UserServices::CompareAndSetSimilarNodes(RoleList,pGroup->RoleList,sNewGroupXPath + "/Information/Roles/RemoveRole/role",pBOProxy);
			}
			else
			{
				UserServices::SetSimilarChildNodes(RoleList,sNewGroupXPath + "/Information/Roles/RemoveRole/role",pBOProxy);
			}			
		}
	}

	CUIControllerAdapter::SetXPathValues(xPaths, values, pBOProxy);

	if(pGroup->RoleList && (false == pGroup->RoleList->Item.empty()))  
	{
		if(CCommonFunctions::IsAnyItemEmptyInList(pGroup->RoleList->Item))
		{
			DEBUGL1("UserServices::ExecuteCreateOrEditGroupCommand: One or more of the roles are empty\n");
		   CErrorCodeMapper::GenerateException("STG2_EMPTY_ROLES");
		}
		 if(("UpdateGroup" == cmdName) && RoleList)
		{
			UserServices::CompareAndSetSimilarNodes(pGroup->RoleList,RoleList,sNewGroupXPath + "/Information/Roles/AddRole/role",pBOProxy);
		}
		else
		{
			UserServices::SetSimilarChildNodes(pGroup->RoleList,sNewGroupXPath + "/Information/Roles/AddRole/role",pBOProxy);
		}
	}
	
	if(pGroup->SubGroupList && (false == pGroup->SubGroupList->Group.empty()))
	{	
		UserServices::SetSubGroupList(sNewGroupXPath + "/Information/Groups/group",pGroup->SubGroupList,pBOProxy);
	}

	vector<commandParam> vParam;
	commandParam cmdParam;
	cmdParam.strParamValue = sNewGroupXPath;
	cmdParam.strParamName = "groupDetails";
	cmdParam.mapAttributes.insert(make_pair("contentType","XPath"));
	vParam.push_back(cmdParam);
	
	if("UpdateGroup" == cmdName)
	{
		cmdParam.strParamValue = "ID";
		cmdParam.strParamName = "searchBy";
		cmdParam.mapAttributes.insert(make_pair("contentType","Value"));
		vParam.push_back(cmdParam);
	}
	
	// Execute the command
	CString strResponse;
	if(STATUS_OK != CUIControllerAdapter::SendCommand(cmdName,GROUPS_XPATH,vParam,strResponse,pBOProxy))
	{
		DEBUGL1("Failed to SendCommand\n");
		CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");
	}
	
	return CUIControllerAdapter::GetOperationStatusString(strResponse);
}

CString UserServices::ExecuteDeleteGroupCommand(const CString& groupName, const BOProxyRef& boProxy)
{
	DEBUGL8("UserServices::ExecuteDeleteGroupCommand enter\n");
	
	std::vector<CString> xPaths;
	std::vector<CString> values;

	values.push_back(groupName);
	xPaths.push_back(CString(GROUPS_GROUP_PATH)+"/Information/name");

	CUIControllerAdapter::SetXPathValues(xPaths, values, boProxy);

	vector<commandParam> vectParams;
	commandParam cmdParam;
	cmdParam.strParamValue = GROUPS_GROUP_PATH;
	cmdParam.strParamName = "groupDetails";
	cmdParam.mapAttributes.insert(make_pair("contentType","XPath"));
	vectParams.push_back(cmdParam);

	// Execute the command
	CString strResponse;
	if(STATUS_OK != CUIControllerAdapter::SendCommand("DeleteGroup",GROUPS_XPATH,vectParams,strResponse,boProxy))
	{
		DEBUGL1("UserServices::ExecuteDeleteGroupCommand SendCommand failed\n");
		CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");	
	}
	
	return CUIControllerAdapter::GetOperationStatusString(strResponse);
	
}

CString UserServices::ExecuteDeleteUserCommand(const CString& sUserID, const BOProxyRef &boProxy)
{
	DEBUGL8("UserServices::ExecuteDeleteUserCommand enter\n");
	std::vector<CString> xPaths;
	std::vector<CString> values;

	values.push_back(sUserID);
	xPaths.push_back(CString(USERMANAGER_NEW_USER)+"/@ID");

	CUIControllerAdapter::SetXPathValues(xPaths, values, boProxy);

	vector<commandParam> vectParams;
	commandParam cmdParam;
	
	cmdParam.strParamValue = USERMANAGER_NEW_USER;
	cmdParam.strParamName = "userList";
	cmdParam.mapAttributes.insert(make_pair("contentType","XPath"));
	vectParams.push_back(cmdParam);
	
	cmdParam.strParamValue = "ID";
	cmdParam.strParamName = "searchBy";
	cmdParam.mapAttributes.insert(make_pair("contentType","Value"));
	vectParams.push_back(cmdParam);

	// Execute the command
	CString strResponse;
	if(STATUS_OK != CUIControllerAdapter::SendCommand("DeleteUser",USERS_XPATH,vectParams,strResponse,boProxy))
	{
		DEBUGL1("UserServices::ExecuteDeleteUserCommand Failed to SendCommand\n");
		CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");		
	}
	
	return CUIControllerAdapter::GetOperationStatusString(strResponse);
		
}

/* Status UserServices::ExecuteDeleteUserAccountingInfoCommand(CString sID,BOProxyRef &boProxy)
{
	DEBUGL8("UserServices::ExecuteDeleteUserAccountingInfoCommand enter\n");
	
	if(!boProxy)
	{
		DEBUGL1("ExecuteDeleteUserAccountingInfoCommand:: bo proxy ref passed is NULL\n");
		return STATUS_FAILED;
	}

	std::vector<CString> xPaths;
	std::vector<CString> values;

	values.push_back(sID);
	xPaths.push_back(CString(ACCOUNTING_USER_ID_PATH)+"/@ID");

	CUIControllerAdapter::SetXPathValues(xPaths, values, boProxy);

	vector<commandParam> vectParams;
	commandParam cmdParam;
	cmdParam.strParamValue = ACCOUNTING_COMMANDNODE_XPATH;
	cmdParam.strParamName = "userList";
	cmdParam.mapAttributes.insert(make_pair("contentType","XPath"));
	vectParams.push_back(cmdParam);

	// Execute the command
	CString strResponse = "";
	CUIControllerAdapter::SendCommand("DeleteUserAccountingInfo",ACCOUNTING_COMMANDNODE_XPATH,vectParams,strResponse,boProxy);
	Status execCommandStatus = CUIControllerAdapter::GetOperationStatus(strResponse);
	DEBUGL8("UserServices::ExecuteDeleteUserAccountingInfoCommand exit \n");
	return execCommandStatus;
} */

Status UserServices::ExecuteDeleteAllGroupsCommand(BOProxyRef &boProxy)
{
	DEBUGL8("start of UserServices::ExecuteDeleteAllGroupsCommand entry \n");
	if(!boProxy)
	{
		DEBUGL1("ExecuteDeleteAllGroupsCommand:: bo proxy ref passed is NULL\n");
		return STATUS_FAILED;
	}

	std::vector<CString> xPaths;
	std::vector<CString> values;

	values.push_back("");
	xPaths.push_back(GROUPS_GROUP_PATH);

	CUIControllerAdapter::SetXPathValues(xPaths, values, boProxy);

	vector<commandParam> vectParams;
	// Execute the command
	CString strResponse = "";
	CUIControllerAdapter::SendCommand("DeleteGroup",GROUPS_XPATH,vectParams,strResponse,boProxy);
	
	CString sOperationStatus = CUIControllerAdapter::GetOperationStatusString(strResponse);
	if(STG2_SUCCESS != sOperationStatus)
	{
		CErrorCodeMapper::GenerateException(sOperationStatus, USR_MGR);
	}	

	DEBUGL8("end of UserServices::ExecuteDeleteAllGroupsCommand exit\n");
	return STATUS_OK;
}
Status UserServices::ExecuteDeleteAllUsersCommand(BOProxyRef &boProxy) //,vector<CString> &failedUsers)
{
	DEBUGL8("start of UserServices::ExecuteDeleteAllUsersCommand entry \n");
/* 	if(!boProxy)
	{
		DEBUGL1("UserServices::ExecuteDeleteAllUsersCommand boproxy received is null");
		return STATUS_FAILED;
	} */
	std::vector<CString> xPaths;
	std::vector<CString> values;

	values.push_back("");
	xPaths.push_back(CString(USERMANAGER_NEW_USER));

	CUIControllerAdapter::SetXPathValues(xPaths, values, boProxy);

	vector<commandParam> vectParams;

	// Execute the command
	CString strResponse = "";
	CUIControllerAdapter::SendCommand("DeleteUser",USERS_XPATH,vectParams,strResponse,boProxy);
	CString  sOperationStatus = CUIControllerAdapter::GetOperationStatusString(strResponse);
	if(STG2_SUCCESS != sOperationStatus)
	{
		CErrorCodeMapper::GenerateException(sOperationStatus,USR_MGR);
	}	
/* 	if ( execCmdStatus != "STATUS_OK") 
	{
		DEBUGL1("Stage2: UserManager Failed to execute the Command: delete all user failed with status : %s \n",execCmdStatus.c_str());
		if(execCmdStatus == "STATUS_USER_UNAUTHORIZED" || execCmdStatus == "STATUS_INVALID_USER_TOKEN" || execCmdStatus == "STATUS_USER_TOKEN_NOT_FOUND"|| execCmdStatus == "STATUS_FAILED")
			throw CStg2Exception(eSenderError, "ERR_EBS_USER_OPERATION_DENIED", SOAP_FAULT,"User does not have rights to execute this operation");
		else
			throw CStg2Exception(eSenderError, "ERR_EBS_SERVER_INTERNAL_ERROR", SOAP_FAULT,"Internal server error");
	} */
	DEBUGL8("UserServices::ExecuteDeleteAllUsersCommand exit \n");
	return STATUS_OK;

}

CString UserServices::GetUserIdUsingName(CString name,BOProxyRef &boProxy)
{
	CString ID = "";
	if(!boProxy)
	return ID;
	DEBUGL8("UserServices::GetUserIdUsingName Enter\n");
	CString sOperationStatus =  UserServices::ExecuteSearchUserByUserName(name,boProxy);
	if(STG2_SUCCESS != sOperationStatus  )
	{
		if("STATUS_PERMISSION_CHECK_ERROR" ==sOperationStatus)
		{
			DEBUGL1("Received STATUS_PERMISSION_CHECK_ERROR for Search use by name\n");
			CErrorCodeMapper::GenerateException(sOperationStatus);
		}
		DEBUGL1("user %s doesnot exist\n",name.c_str());
		return ID;
	}
	NodeRef pNode = NULL;
	boProxy->GetValue((CString(USERS_XPATH)+"/User/Information").c_str(),pNode);
	if(!pNode)
	{
		DEBUGL1("user %s doesnot exist\n",name.c_str());
		return ID;
	}
	ID = CUIControllerAdapter::GetTextNodeValue(CString(USERS_XPATH)+"/User/@ID",boProxy);
	DEBUGL8("UserServices::GetUserIdUsingName Exit\n");
	return ID;
}

#ifdef STAGE2_UNSUPPORTED
SOAP_FMAC5 int SOAP_FMAC6 __user__getApplicationQuota(struct soap* pSOAP, _user__getApplicationQuota *user__getApplicationQuota, _user__getApplicationQuotaResponse *user__getApplicationQuotaResponse)
{
	CCommonFunctions::GenerateSoapFault(pSOAP, "Operation not supported", "ERR_EBS_SERVER_OPERATION_UNSUPPORTED", eReceiverError);
	return SOAP_FAULT;

}

SOAP_FMAC5 int SOAP_FMAC6 __user__setApplicationQuota(struct soap* pSOAP, _user__setApplicationQuota *user__setApplicationQuota, _user__setApplicationQuotaResponse *user__setApplicationQuotaResponse)
{
	CCommonFunctions::GenerateSoapFault(pSOAP, "Operation not supported", "ERR_EBS_SERVER_OPERATION_UNSUPPORTED", eReceiverError);
	return SOAP_FAULT;

}
SOAP_FMAC5 int SOAP_FMAC6 __user__changePassword(struct soap* soap, _user__changePassword *user__changePassword, _user__changePasswordResponse *user__changePasswordResponse)
{
	DEBUGL8("Stage2:__user__changePassword Enter\n");

	try
	{
		BOProxyRef boProxy = CCommonFunctions::getBOProxyRef(soap);
		if(!boProxy)
		{
			CErrorCodeMapper::GenerateException("STG2_INVALID_SESSION");
		}
		
		// Validate the parameters
		if(!user__changePassword || !user__changePasswordResponse)
		{
			CErrorCodeMapper::GenerateException("STG2_MISSING_PARAM");
		}		

		// Get the user name
		CString sUserName;
		if(user__changePassword->UserName)
		{
			sUserName = *(user__changePassword->UserName);
		}
		
				
		if(sUserName.empty())
		{
			if(false == CCommonFunctions::IsUserLogin(soap))
			{
				DEBUGL1("User name is empty\n");
				CErrorCodeMapper::GenerateException("STG2_EMPTY_USER_NAME"); 
			}
			else
			{
				sUserName = CCommonFunctions::GetLoggedInUserName(boProxy);
			}
		}		
		
		DEBUGL6("User name is %s\n", sUserName.c_str());
		
		CString sCurrentPassword;
		if(user__changePassword->Password)
		{
			sCurrentPassword = *(user__changePassword->Password);
			/* if(sCurrentPassword.empty())
			{
				DEBUGL1("Password is emtpy\n");
				CErrorCodeMapper::GenerateException("STG2_EMPTY_PASSWORD");
			}	 */	
		}
		
		CString sNewPassword;
		if(user__changePassword->NewPassword)
		{
			sNewPassword = *(user__changePassword->NewPassword);
		}
		/* if(sNewPassword.empty())
		{
			DEBUGL1("New Password is emtpy\n");
			CErrorCodeMapper::GenerateException("STG2_EMPTY_NEWPASSWORD");
		} */		
		
		if(sCurrentPassword == sNewPassword)
		{
			CErrorCodeMapper::GenerateException("STG2_PASSWORD_REPEATED");
		}
		
		CString sChangeType = "Reset";		
		if(!sCurrentPassword.empty()) // If CurrentPassword is provided then password set command type to Change.
		{
			sChangeType = "Change";
		}
		
		if(STATUS_OK != UserServices::ExecuteChangeUserPasswordCommand(sUserName,sCurrentPassword,sNewPassword, sChangeType , boProxy))
		{
			user__changePasswordResponse->Result = common__operationResultType__OperationFailed;
		}
		else
		{
			user__changePasswordResponse->Result = common__operationResultType__OperationSucceeded;
		}		
	}
	catch(CStg2Exception & ex)
	{
		DEBUGL1("Stage2::__user__changePassword caught Stg2Exception Exception. Description = %s\n", ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), ex.getFaultType(), ex.getErrorType()) != STATUS_OK)
		DEBUGL2("__user__changePassword CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}
	catch(CException & ex)
	{
		DEBUGL1("Stage2::__user__changePassword caught CException Exception. Description = %s\n", ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), "", eReceiverError) != STATUS_OK)
		DEBUGL2("__user__changePassword CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}
	DEBUGL8("Stage2:__user__changePassword Exit\n");	

	return SOAP_OK;
}


/**
* Calls Accounting Plug-in's GetBillingCodes command to retrieve billing code information corresponding 
* to the department code given in strDeptCode and populates newly allocated user__departmentType for 
* each billing code. Pointers to newly allocated user__departmentType pushed into vector<user__departmentType *> . 
*/
Status UserServices::getDepartments(soap * soap, vector<user__departmentType * > & departmentTypes, const CString & strDeptCode)
{
	DEBUGL8("Stage2::getDepartments entry\n");
	BOProxyRef boProxy = CCommonFunctions::getBOProxyRef(soap);
	if(!boProxy)
	{
		CErrorCodeMapper::GenerateException("STG2_INVALID_SESSION");
	}

	const CString strBillingCodeXPath = "Accounting/BillingCodes";

	std::vector<CString> xPaths;
	std::vector<CString> values;
	
	if(strDeptCode.empty())
	{
		// Just set an empty Accouting DOM
		values.push_back("");
		xPaths.push_back(strBillingCodeXPath);
	}
	else
	{
		// Set the required dept's dept code as filter 
		values.push_back(strDeptCode);
		xPaths.push_back(strBillingCodeXPath + "/BillingCode/code");
	}

	DEBUGL8("getDepartments : calling SetXPathValues\n");
	CUIControllerAdapter::SetXPathValues(xPaths, values, boProxy);

	// Send the command
	vector<commandParam> vectParams;
	// If a dept Code is specified, then get the corresponding BillingCode
	// Else pass empty vectParams (no billingCodeDetails element), Empty vectParams means get all BillingCodes
	if(!strDeptCode.empty())
	{
		commandParam p;
		p.strParamName = "billingCodesDetails";
		p.strParamValue = strBillingCodeXPath + "/BillingCode";
		p.mapAttributes.insert(make_pair("contentType","XPath"));
		vectParams.push_back(p);
	}
	CString strResponse;	

	Status status = CUIControllerAdapter::SendCommand("GetBillingCodes", "Accounting/BillingCodes", vectParams, strResponse, boProxy);

	// Get the result
	
	HierarchicalDBRef  pHDB = HierarchicalDB::Acquire(NULL);
	CString sResult = CUIControllerAdapter::GetOperationStatusString(strResponse);
	if(STG2_SUCCESS != sResult)
	{
		CErrorCodeMapper::GenerateException(sResult, ACC_MGR);
	}
	
	
	Status execCommandStatus = CUIControllerAdapter::GetOperationStatus(strResponse);
	DEBUGL8("getDepartments : SendCommand status=%d response=%s response status of operation=%d\n", status, strResponse.c_str(), execCommandStatus);

	if(status != STATUS_OK || execCommandStatus != STATUS_OK)
		CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");

	// Read the BillingCodes
	NodeRef refBillingCodes = NULL;			
	if(boProxy->GetValue("Accounting/BillingCodes/", refBillingCodes) != STATUS_OK)
		CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");
	
	Ref<NodeList> refBillingCodeList = refBillingCodes->getChildNodes();
	unsigned long numBillingCodes = refBillingCodeList->getLength();
	user__departmentType * pDepartmentTypes = soap_new_user__departmentType(soap, static_cast<int>(numBillingCodes));
	for(unsigned long i = 0; i < numBillingCodes; ++i)
	{
		NodeRef refBillingCode = refBillingCodeList->item(i);
		if(!refBillingCode)
		{
			DEBUGL1("getDepartments refBillingCode[%d] is NULL\n", static_cast<int>(i));
			continue;
		}
		if(refBillingCode->getNodeName() != "BillingCode")
		continue;

		// Populate the DepartmentInfo struct
		pDepartmentTypes[i].DepartmentInfo = soap_new_common__departmentInfoType(soap, -1);
		pDepartmentTypes[i].DepartmentInfo->Code = chelper::GetNodeValue(refBillingCode, "code");
		pDepartmentTypes[i].DepartmentInfo->Name = chelper::GetNodeValue(refBillingCode, "name");
		pDepartmentTypes[i].DepartmentInfo->DepartmentID = soap_new_std__string(soap, -1);
		*(pDepartmentTypes[i].DepartmentInfo->DepartmentID) = chelper::GetAttributeValue(refBillingCode, "ID");

		// Populate the ConfigurationInfo/Quota Struct

		NodeRef pQuotaRootNode = NULL;
		chelper::GetNode(refBillingCode, "Quota", pQuotaRootNode);
		pDepartmentTypes[i].ConfigurationInfo = soap_new_user__configurationInfoType(soap, -1);
		pDepartmentTypes[i].ConfigurationInfo->Quota = soap_new_common__quotaCategoryType(soap, -1);
		pDepartmentTypes[i].ConfigurationInfo->Quota->Job = soap_new_common__jobQuotaCategoryType(soap, -1);
		pDepartmentTypes[i].ConfigurationInfo->Quota->Job->Print = soap_new_common__printQuotaCategoryType(soap, -1);
		pDepartmentTypes[i].ConfigurationInfo->Quota->Job->Print = UserServices::MapUserQuotaInfo(pQuotaRootNode,boProxy,soap);
		departmentTypes.push_back(&pDepartmentTypes[i]);
	}

	DEBUGL8("Stage2::getDepartments exit\n");
	return STATUS_OK;
}

/**
* Calls Accounting Plug-in's ModifyBillingCodes command to modify the billing codes with the values in 
* user__departmentType structs, pointers to which are passed in vector departmentTypes. The billing codes 
* to be modified are identified by the department ID.
* in case input argument "mode" is other then default value "None" it will throw exceptions in case the command fails
* for mode ALL it will run thru all the departments input in the vector even if the command fails
*/
Status UserServices::setDepartments(soap * soap, const vector<user__departmentType * > & departmentTypes, const CString & strCommand,OffendingParamsList  & vOffendingParamsList,const CString& mode)
{
	DEBUGL6("UserServices::setDepartments Enter\n");

	BOProxyRef boProxy = CCommonFunctions::getBOProxyRef(soap);
	OffendingParamsStruct offendingParamInfo;

	if(!boProxy)
	{
		CErrorCodeMapper::GenerateException("STG2_INVALID_SESSION");
	}
	
	
	//In case when we are modifying existing Billing codes,
	//we have to make sure that Client has not suppiled invalid Department ID.
	//To check that	Get All Billing codes By calling GetBillingCodes command of Accounting Plugin.
	// Check if all Department IDs suppiled in the request are valid.
	// The following block is added to fix an issue in Build_12 Test Report.
	if(strCommand == "ModifyBillingCode" )
	{
		
		vector<CString> vExistingID;
		map<CString,CString> vExistingDeptMap; // We don't need this parameter but getDepartmentKey function uses it.
		getDepartmentKey(boProxy, vExistingID, vExistingDeptMap);
		
		//Now we have two vectors. One contains Existing Department codes in vExistingDeptCodes. 
		//Other contains Department Codes from the request.
		// We have to make sure that none of the Department Codes from the Request is invalid.
		
		vector<user__departmentType * >::const_iterator itDeptType = departmentTypes.begin();
		vector<user__departmentType * >::const_iterator itDeptTypeEnd = departmentTypes.end();
		
		for(; itDeptType != itDeptTypeEnd; ++itDeptType)
		{
			user__departmentType *Department = (*itDeptType);
			// If department id is not provided. The condition is handled later in the function.
			if(!Department->DepartmentInfo->DepartmentID)
			{
				continue;			
			}
			const CString strDeptID = *(Department->DepartmentInfo->DepartmentID);
			
			vector<CString>::iterator it = vExistingID.begin();
			vector<CString>::iterator itEnd = vExistingID.end();
			
			vector<CString>::iterator result = find( it, itEnd, strDeptID);
			if(result == itEnd)
			{
				DEBUGL4("Department code %s is Invalid Department ID\n", strDeptID.c_str());
				CString sErrorMessage = "Department does not exist" ;
				CErrorCodeMapper::GenerateException("STG2_DEPT_DOESNT_EXIST");
			}
		}
	}
	// Validation Block ends here
	
	
	// Set the values in the Accounting/BillingCodes DOM
	const CString strBillingCodeXPath = "Accounting/" + strCommand + "/BillingCodes/BillingCode";
	

	vector<user__departmentType * >::const_iterator itDeptType = departmentTypes.begin();
	vector<user__departmentType * >::const_iterator itDeptTypeEnd = departmentTypes.end();
	

	for(; itDeptType != itDeptTypeEnd; ++itDeptType)
	{
		user__departmentType *Department = (*itDeptType);
		if(!Department)
		{
			DEBUGL2("UserServices::setDepartments NULL user__departmentType *\n");
			continue;
		}

		std::vector<CString> xPaths;
		std::vector<CString> values;
		CString strDeptCode = "";
		// Set the DepartmentInfo values
		if(Department->DepartmentInfo)
		{
			strDeptCode = Department->DepartmentInfo->Code;
			const CString strDeptName = Department->DepartmentInfo->Name;
			// Department ID needs to be specified only for ModifyBillingCode
			// For RegisterBillingCode, we do not specify the dept ID. (Infact we dont have one)
			if(strCommand == "ModifyBillingCode")
			{
				if(!Department->DepartmentInfo->DepartmentID)
				{
					DEBUGL4("Department ID is not specified\n");
					CErrorCodeMapper::GenerateException("STG2_MISSING_DEPT_ID");	
				}
				const CString strDeptID = *(Department->DepartmentInfo->DepartmentID);
				values.push_back(strDeptID);
				xPaths.push_back(strBillingCodeXPath + "/@ID");
			}
			values.push_back(strDeptCode);
			xPaths.push_back(strBillingCodeXPath + "/code");
			values.push_back(strDeptName);
			xPaths.push_back(strBillingCodeXPath + "/name");
		}
		
		// TBD - Set the OwnershipInfo values
		if(Department->OwnershipInfo)
		{
		}

		// Set the ConfigurationInfo values
		if(Department->ConfigurationInfo)
		{
			// TBD - Set Notification
			if(Department->ConfigurationInfo->Notification)
			{
			}

			// Set Quota
			common__quotaCategoryType * Quota = Department->ConfigurationInfo->Quota;
			if(Quota && Quota->Job)
			if(Quota->Job->Print)
			if(Quota->Job->Print->__union_printQuotaCategoryType == SOAP_UNION__common__union_printQuotaCategoryType_ColorJobQuota)
			{
				common__colorJobQuotaType *colorJobQuota = Quota->Job->Print->union_printQuotaCategoryType.ColorJobQuota;
				CString val = "";
				if(colorJobQuota->ColorQuota)
				{
					if(colorJobQuota->ColorQuota->Quota)
					{
						xPaths.push_back(strBillingCodeXPath + "/Quota/fullColorDefaultQuantity") ;
						values.push_back(*(colorJobQuota->ColorQuota->Quota));
					}
					if(colorJobQuota->ColorQuota->Balance)
					{
						xPaths.push_back(strBillingCodeXPath + "/Quota/fullColorRemainingQuantity") ;
						values.push_back(*(colorJobQuota->ColorQuota->Balance));
					}
					val = colorJobQuota->ColorQuota->Enable ? "1":"0";
					xPaths.push_back(strBillingCodeXPath + "/Quota/fullColorQuotaEnabled" );
					values.push_back(val);					
				}
				if(colorJobQuota->BlackQuota)
				{
					if(colorJobQuota->BlackQuota->Quota)
					{
						xPaths.push_back(strBillingCodeXPath + "/Quota/blackDefaultQuantity") ;
						values.push_back(*(colorJobQuota->BlackQuota->Quota));
					}
					if(colorJobQuota->BlackQuota->Balance)
					{
						xPaths.push_back(strBillingCodeXPath + "/Quota/blackRemainingQuantity") ;
						values.push_back(*(colorJobQuota->BlackQuota->Balance));
					}
					val = colorJobQuota->BlackQuota->Enable ? "1":"0";
					xPaths.push_back(strBillingCodeXPath + "/Quota/blackQuotaEnabled" );
					values.push_back(val);					
				}

			}

			// TDB - Set Resource Limit
			if(Department->ConfigurationInfo->ResourceLimit)
			{
			}
		}

		DEBUGL8("__user__createDepartment : calling SetXPathValues\n");
		CUIControllerAdapter::SetXPathValues(xPaths, values, boProxy);

		// Send the command
		commandParam p;
		p.strParamName = "billingCodeDetails";
		p.strParamValue = strBillingCodeXPath;
		p.mapAttributes.insert(make_pair("contentType","XPath"));
		vector<commandParam> vectParams;
		vectParams.push_back(p);
		CString strResponse;	

		Status status = CUIControllerAdapter::SendCommand(strCommand, "Accounting/BillingCodes", vectParams, strResponse, boProxy);
		if (status != STATUS_OK) 
		{
			DEBUGL1("Stage2: Failed to send command to ACCManager ! \n");
			CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");
		}

		// Get the result
		CString strResponseStatus =  CUIControllerAdapter::GetOperationStatusString(strResponse);
		DEBUGL4("strResponseStatus is ++%s++\n",strResponseStatus.c_str()); 
		if(strResponseStatus != STG2_SUCCESS)
		{
			if(itDeptType == departmentTypes.begin())
				CErrorCodeMapper::GenerateException(strResponseStatus, ACC_MGR);
			else
			{
				offendingParamInfo.Name    = "Department modify/register failed";
				offendingParamInfo.Value = "Department code  = "+ strDeptCode;;
				vOffendingParamsList.push_back(offendingParamInfo);
				CErrorCodeMapper::GenerateException("STG2_DEPT_PARTIAL_SUCCESS",vOffendingParamsList);
			}
		}
		else
			DEBUGL8("alls well that ends well :) \n");
		//Status execCommandStatus = CUIControllerAdapter::GetOperationStatus(strResponse);
		//DEBUGL8("UserServices::setDepartments : SendCommand status=%d response=%s response status of operation=%d\n", status, strResponse.c_str(), execCommandStatus);

		
		
		//if((status != STATUS_OK || execCommandStatus != STATUS_OK )&& mode != "None")
		//	CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");
	}

	DEBUGL6("UserServices::setDepartments Exit\n");
	return STATUS_OK;
}
#endif


// gets the list of existing department codes 
/* Status UserServices::getDepartmentCodes(BOProxyRef& boProxy, vector<CString>& vExistingDeptCodes)
{

    const CString strBillingCodeXPath = "Accounting/BillingCodes";

    std::vector<CString> xPaths;
    std::vector<CString> values;

    // Just set an empty Accouting DOM
    values.push_back("");
    xPaths.push_back(strBillingCodeXPath);

    DEBUGL8("getDepartmentCodes : calling SetXPathValues\n");
    CUIControllerAdapter::SetXPathValues(xPaths, values, boProxy);
    // Send command to Get All Billing codes.

    vector<commandParam> vectParams;
    vectParams.clear();
    CString strResponse;
    Status status = CUIControllerAdapter::SendCommand("GetBillingCodes", "Accounting/BillingCodes", vectParams, strResponse, boProxy);


    // Get the result
    Status execCommandStatus = CUIControllerAdapter::GetOperationStatus(strResponse);
    DEBUGL8("getDepartmentCodes : SendCommand status=%d response=%s response status of operation=%d\n", status, strResponse.c_str(), execCommandStatus);

    if(status != STATUS_OK || execCommandStatus != STATUS_OK)
    {
        throw CStg2Exception(eSenderError, "ERR_EBS_SERVER_INTERNAL_ERROR", SOAP_FAULT, "CUIControllerAdapter::SendCommand or CUIControllerAdapter::GetOperationStatus Failed");
    }
    NodeRef refBillingCodes = NULL;
    if(boProxy->GetValue("Accounting/BillingCodes/", refBillingCodes) != STATUS_OK || !refBillingCodes)
    {
        throw CStg2Exception(eReceiverError, "ERR_EBS_SERVER_INTERNAL_ERROR", SOAP_FAULT, "boProxy::GetValue Accounting/BillingCodes Failed");
    }

    Ref<NodeList> refBillingCodeList = refBillingCodes->getChildNodes();
    unsigned long numBillingCodes = refBillingCodeList->getLength();

    //Create Vector of Existing Department codes.

    vExistingDeptCodes.clear();

    for(unsigned long i = 0; i < numBillingCodes; ++i)
    {
        NodeRef refBillingCode = refBillingCodeList->item(i);
        if(!refBillingCode)
        {
            DEBUGL1("getDepartments refBillingCode[%u] is NULL\n", i);
            continue;
        }
        if(refBillingCode->getNodeName() != "BillingCode")
        continue;

        CString sCode = chelper::GetNodeValue(refBillingCode, "code");
        vExistingDeptCodes.push_back(sCode);
    }

    // Clearing data from Accounting DOM as required data are stored in vExistingDeptCodes vector.
    CString sResetXML = "<ResetValue>";
    sResetXML += "<path>";
    sResetXML += strBillingCodeXPath;
    sResetXML += "</path>";
    sResetXML += "</ResetValue>";
    boProxy->ResetValue(sResetXML.c_str(),eDefaultValue);

    return STATUS_OK;

}


 */
// gets the list of existing department codes or ids
Status UserServices::getDepartmentKey(BOProxyRef& boProxy, vector<CString>& vExistingDeptKeys,map<CString,CString>& vExistingDeptMap)
{

	const CString strBillingCodeXPath = "Accounting/BillingCodes";

	std::vector<CString> xPaths;
	std::vector<CString> values;
	
	// Just set an empty Accouting DOM
	values.push_back("");
	xPaths.push_back(strBillingCodeXPath);
	
	DEBUGL8("getDepartmentKey : calling SetXPathValues\n");
	CUIControllerAdapter::SetXPathValues(xPaths, values, boProxy);
	// Send command to Get All Billing codes.	
	
	vector<commandParam> vectParams;
	vectParams.clear();
	CString strResponse;	
	Status status = CUIControllerAdapter::SendCommand("GetBillingCodes", "Accounting/BillingCodes", vectParams, strResponse, boProxy);

	
	// Get the result
	CString execCommandStatus = CUIControllerAdapter::GetOperationStatusString(strResponse);
	DEBUGL8("getDepartmentKey : SendCommand status=%d response=%s response status of operation=%d\n", status, strResponse.c_str(), execCommandStatus.c_str());

	if(status != STATUS_OK)
	{
		CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");
	}
	if(execCommandStatus != STG2_SUCCESS)
	{
		CErrorCodeMapper::GenerateException(execCommandStatus, ACC_MGR);
	}
	NodeRef refBillingCodes = NULL;			
	if(boProxy->GetValue("Accounting/BillingCodes/", refBillingCodes) != STATUS_OK || !refBillingCodes)
	{
		CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");
	}
	
	Ref<NodeList> refBillingCodeList = refBillingCodes->getChildNodes();
	unsigned long numBillingCodes = refBillingCodeList->getLength();
	
	//Create Vector of Existing Department codes.
	
	vExistingDeptKeys.clear();
	
	for(unsigned long i = 0; i < numBillingCodes; ++i)
	{
		Ref<Element> refBillingCode = refBillingCodeList->item(i);
		if(!refBillingCode)
		{
			DEBUGL1("getDepartments refBillingCode[%u] is NULL\n", i);
			continue;
		}
		if(refBillingCode->getNodeName() != "BillingCode")
		continue;
		
		CString sCode= chelper::GetNodeValue(refBillingCode, "code") ;
		CString sID = refBillingCode->getAttribute("ID");
		vExistingDeptKeys.push_back(sID);
		vExistingDeptMap.insert(make_pair(sID,sCode));
		//DEBUGL9("Pushing Code %s and ID %s into Vector\n", sCode.c_str(),sID.c_str());
	}
	
	// Clearing data from Accounting DOM as required data are stored in vExistingDeptCodes vector.
	CString sResetXML = "<ResetValue>";
	sResetXML += "<path>";
	sResetXML += strBillingCodeXPath;
	sResetXML += "</path>";
	sResetXML += "</ResetValue>";
	boProxy->ResetValue(sResetXML.c_str(),eDefaultValue);
	
	return STATUS_OK;

}


SOAP_FMAC5 int SOAP_FMAC6 __user__importDepartmentInfo(struct soap* soap, 
                                                       _user__importDepartmentInfo *user__importDepartmentInfo, 
                                                       _user__importDepartmentInfoResponse *user__importDepartmentInfoResponse) 
{
	DEBUGL8("user__importDepartmentInfo:Enter\n");
   bool     isValidSession = true;

	try
	{
		BOProxyRef  pBOProxy = CCommonFunctions::getBOProxyRef(soap);
		if(!pBOProxy)
		{
         DEBUGL1("user__importDepartmentInfo:Invalid Session\n");
         isValidSession = false;
		}

      if(!user__importDepartmentInfo || !user__importDepartmentInfo->Parameter)
      {
         DEBUGL1("__user__importDepartmentInfo:Missing mandatory params\n");
		   CErrorCodeMapper::GenerateException("STG2_MISSING_PARAM");
      }

      //If we are kicking off the command
      if(user__importDepartmentInfo->Parameter->__union_importDepartmentInfoParameterType == SOAP_UNION__user__union_importDepartmentInfoParameterType_Detail)
      {
         UserServices::ImportDepartmentInfo( soap,
                                             pBOProxy,
                                             user__importDepartmentInfo->Parameter->union_importDepartmentInfoParameterType.Detail, 
                                             user__importDepartmentInfoResponse, 
                                             isValidSession);
      }
      else //we are checking status
      {
         if(!isValidSession)
            CErrorCodeMapper::GenerateException("STG2_INVALID_SESSION");

         CString asyncOpID = "";
         if(user__importDepartmentInfo->Parameter->union_importDepartmentInfoParameterType.OperationStatusID)
            asyncOpID = *(user__importDepartmentInfo->Parameter->union_importDepartmentInfoParameterType.OperationStatusID);

         if(asyncOpID.empty())
         {
            DEBUGL1("__user__importDepartmentInfo:Empty operation id\n");  
		      CErrorCodeMapper::GenerateException("STG2_EMPTY_OP_ID");
         }

         user__importDepartmentInfoResponse->Response = CLongRunningOpHelper::ProcessAsyncImportOperationStatusQuery(soap, asyncOpID);

      }
	}					
	catch(CStg2Exception & ex)
	{
      DEBUGL1("Stage2::__user__importDepartmentInfo:Stg2Exception:Description = %s\n", ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), ex.getFaultType(), ex.getErrorType()) != STATUS_OK)
			DEBUGL1("__user__importDepartmentInfo CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}
	catch(CException & ex)
	{
      DEBUGL1("Stage2::__user__importDepartmentInfo:CException:Description = %s\n", ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), "", eReceiverError) != STATUS_OK)
			DEBUGL1("__user__importDepartmentInfo CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}

	return SOAP_OK;
	
}

SOAP_FMAC5 int SOAP_FMAC6 __user__exportDepartmentInfo(struct soap* soap, 
                                                       _user__exportDepartmentInfo *user__exportDepartmentInfo, 
                                                       _user__exportDepartmentInfoResponse *user__exportDepartmentInfoResponse) 
{
	try
	{
      DEBUGL8("__user__exportDepartmentInfo:Enter\n");
		BOProxyRef pBOProxy = CCommonFunctions::getBOProxyRef(soap);
		if(!pBOProxy)
		{
			CErrorCodeMapper::GenerateException("STG2_INVALID_SESSION");
		}

      if(!user__exportDepartmentInfo || !user__exportDepartmentInfo->Parameter)
      {
         DEBUGL1("__user__exportDepartmentInfo:Missing mandatory params\n");
		   CErrorCodeMapper::GenerateException("STG2_MISSING_PARAM");
      }

      //If we are kicking off the command
      if(user__exportDepartmentInfo->Parameter->__union_exportDepartmentInfoParameterType == SOAP_UNION__user__union_exportDepartmentInfoParameterType_Detail)
      {
         UserServices::ExportDeptInfo( soap,
                                       pBOProxy,
                                       user__exportDepartmentInfo->Parameter->union_exportDepartmentInfoParameterType.Detail, 
                                       user__exportDepartmentInfoResponse);
      }
      else //we are checking status
      {
         CString asyncOpID = "";
         if(user__exportDepartmentInfo->Parameter->union_exportDepartmentInfoParameterType.OperationStatusID)
            asyncOpID = *(user__exportDepartmentInfo->Parameter->union_exportDepartmentInfoParameterType.OperationStatusID);

         if(asyncOpID.empty())
         {
            DEBUGL1("__user__exportDepartmentInfo:Empty operation id\n");  
		      CErrorCodeMapper::GenerateException("STG2_EMPTY_OP_ID");
         }

         user__exportDepartmentInfoResponse->Response = CLongRunningOpHelper::ProcessAsyncExportOperationStatusQuery(soap, asyncOpID);

      }
      DEBUGL8("__user__exportDepartmentInfo:Exit\n");
	}


	catch(CStg2Exception & ex)
	{
      DEBUGL1("Stage2::__user__exportDepartmentInfo:Stg2Exception:Description = %s\n", ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), ex.getFaultType(), ex.getErrorType()) != STATUS_OK)
			DEBUGL1("__user__exportDepartmentInfo CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}
	catch(CException & ex)
	{
      DEBUGL1("Stage2::__user__exportDepartmentInfo:CException :Description = %s\n", ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), "", eReceiverError) != STATUS_OK)
			DEBUGL1("__user__exportDepartmentInfo CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}
		
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __user__importUserInfo(struct soap* soap, 
                                                 _user__importUserInfo *user__importUserInfo, 
                                                 _user__importUserInfoResponse *user__importUserInfoResponse) 
{
	DEBUGL8("__user__importUserInfo:Enter\n");
   bool     isValidSession    = true;
   CString  downloadFolder    = "";

	try
	{
		BOProxyRef  pBOProxy = CCommonFunctions::getBOProxyRef(soap);
		if(!pBOProxy)
		{
         DEBUGL1("__user__importUserInfo:Invalid Session\n");
         isValidSession = false;
		}

      if(!user__importUserInfo || !user__importUserInfo->Parameter)
      {
         DEBUGL1("__user__importUserInfo:Missing mandatory params\n");
		   CErrorCodeMapper::GenerateException("STG2_MISSING_PARAM");
      }
	  
      //If we are kicking off the command
      if(user__importUserInfo->Parameter->__union_importUserInfoParameterType == SOAP_UNION__user__union_importUserInfoParameterType_Detail)
      {
         UserServices::ImportUserInfo(soap,
                                      pBOProxy,
                                      user__importUserInfo->Parameter->union_importUserInfoParameterType.Detail, 
                                      user__importUserInfoResponse, isValidSession);
      }
      else //we are checking status
      {
         if(!isValidSession)
            CErrorCodeMapper::GenerateException("STG2_INVALID_SESSION");

         CString asyncOpID = "";
         if(user__importUserInfo->Parameter->union_importUserInfoParameterType.OperationStatusID)
            asyncOpID = *(user__importUserInfo->Parameter->union_importUserInfoParameterType.OperationStatusID);

         if(asyncOpID.empty())
         {
            DEBUGL1("__user__importUserInfo:Empty operation id\n");  
		      CErrorCodeMapper::GenerateException("STG2_EMPTY_OP_ID");
         }

         user__importUserInfoResponse->Response = CLongRunningOpHelper::ProcessAsyncImportOperationStatusQuery(soap, asyncOpID);

      }
	}	
	catch(CStg2Exception & ex)
	{
      DEBUGL1("Stage2::__user__importUserInfo:Stg2Exception:Description = %s\n", ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), ex.getFaultType(), ex.getErrorType()) != STATUS_OK)
			DEBUGL1("__user__importUserInfo CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}
	catch(CException & ex)
	{
      DEBUGL1("Stage2::__user__importUserInfo :CException:Description = %s\n", ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), "", eReceiverError) != STATUS_OK)
			DEBUGL1("__user__importUserInfo CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}

	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __user__exportUserInfo(struct soap* soap, 
                                                 _user__exportUserInfo *user__exportUserInfo, 
                                                 _user__exportUserInfoResponse *user__exportUserInfoResponse) 
{
	try
	{
      DEBUGL8("__user__exportUserInfo:Enter\n");
		BOProxyRef pBOProxy = CCommonFunctions::getBOProxyRef(soap);
		if(!pBOProxy)
		{
         DEBUGL1("__user__exportUserInfo:Invalid Session\n");
			CErrorCodeMapper::GenerateException("STG2_INVALID_SESSION");
		}

      if(!user__exportUserInfo || !user__exportUserInfo->Parameter)
      {
         DEBUGL1("__user__exportUserInfo:Missing mandatory params\n");
		   CErrorCodeMapper::GenerateException("STG2_MISSING_PARAM");
      }

      //If we are kicking off the command
      if(user__exportUserInfo->Parameter->__union_exportUserInfoParameterType == SOAP_UNION__user__union_exportUserInfoParameterType_Detail)
      {
         UserServices::ExportUserInfo(soap,
                                      pBOProxy,
                                      user__exportUserInfo->Parameter->union_exportUserInfoParameterType.Detail, 
                                      user__exportUserInfoResponse);
      }
      else //we are checking status
      {

         CString asyncOpID = "";
         if(user__exportUserInfo->Parameter->union_exportUserInfoParameterType.OperationStatusID)
            asyncOpID = *(user__exportUserInfo->Parameter->union_exportUserInfoParameterType.OperationStatusID);

         if(asyncOpID.empty())
         {
            DEBUGL1("__user__exportUserInfo:Empty operation id\n");  
		      CErrorCodeMapper::GenerateException("STG2_EMPTY_OP_ID");
         }

         user__exportUserInfoResponse->Response = CLongRunningOpHelper::ProcessAsyncExportOperationStatusQuery(soap, asyncOpID);
      }
	}	
	catch(CStg2Exception & ex)
	{
      DEBUGL1("Stage2::__user__exportUserInfo:Stg2Exception:Description = %s\n", ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), ex.getFaultType(), ex.getErrorType()) != STATUS_OK)
			DEBUGL1("__user__exportUserInfo CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}
	catch(CException & ex)
	{
      DEBUGL1("Stage2::__user__exportUserInfo:CException:Description = %s\n", ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), "", eReceiverError) != STATUS_OK)
			DEBUGL1("__user__exportUserInfo CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}
		
	return SOAP_OK;
}

#ifdef STAGE2_UNSUPPORTED
SOAP_FMAC5 int SOAP_FMAC6 __user__getCount(struct soap* soap, 
                                           _user__getCount *user__getCount, 
                                           _user__getCountResponse *user__getCountResponse)
{
   bool bIncludeAll     = false;
   bool bIncludeUser    = false;
   bool bIncludeRole    = false;
   bool bIncludeGroup   = false;


	try
	{
      DEBUGL8("__user__getCount:Enter\n");
		
		BOProxyRef pBOProxy = CCommonFunctions::getBOProxyRef(soap);
		if(!pBOProxy)
		{
			CErrorCodeMapper::GenerateException("STG2_INVALID_SESSION");
		}

		if(!user__getCount)
		{
			CErrorCodeMapper::GenerateException("STG2_MISSING_PARAM");
		}

      user__getCountResponse->Count = soap_new_user__countResponseType(soap, -1);

      RBACCountStruct rbacCounts;
      if(!user__getCount->Count)
      {
         rbacCounts = UserServices::GetRBACCounts(pBOProxy, NULL);
         bIncludeAll = true;
      }
      else
      {
         switch (*(user__getCount->Count))
         {
            case user__countEntityType__User:
			   {
               rbacCounts     = UserServices::GetRBACCounts(pBOProxy, user__getCount->Count);
               bIncludeUser   = true;
				   break;
			   }
            case user__countEntityType__Role:
			   {
               rbacCounts     = UserServices::GetRBACCounts(pBOProxy, user__getCount->Count);
               bIncludeRole   = true;
				   break;
			   }
            case user__countEntityType__Group:
			   {
               rbacCounts     = UserServices::GetRBACCounts(pBOProxy, user__getCount->Count);
               bIncludeGroup  = true;
				   break;
			   }
            case user__countEntityType__Department:
            {
			      CErrorCodeMapper::GenerateException("STG2_UNSUPPORTED_ENTITY_TYPE");
            }         
            default:
               DEBUGL8("__user__getCount:Invalid entity type\n");

         }
      }
      if(bIncludeAll || bIncludeUser)
      {
         if(user__getCountResponse->Count)
         {
            user__getCountResponse->Count->User       = static_cast<int*>(soap_malloc(soap, sizeof(int)));
            *(user__getCountResponse->Count->User)    = rbacCounts.userCount;
         }
      }
      if(bIncludeAll || bIncludeRole)
      {
         if(user__getCountResponse->Count)
         {
            user__getCountResponse->Count->Role       = static_cast<int*>(soap_malloc(soap, sizeof(int)));
            *(user__getCountResponse->Count->Role)    = rbacCounts.roleCount;
         }
      }

      if(bIncludeAll || bIncludeGroup)
      {
         if(user__getCountResponse->Count)
         {
            user__getCountResponse->Count->Group       = static_cast<int*>(soap_malloc(soap, sizeof(int)));
            *(user__getCountResponse->Count->Group)    = rbacCounts.groupCount;
         }
      }
		
	}
	catch(CStg2Exception & ex)
	{
      DEBUGL1("Stage2::__user__getCount:Stg2Exception:Description = %s\n", ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), ex.getFaultType(), ex.getErrorType()) != STATUS_OK)
		DEBUGL4("__user__getCount CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}
	catch(CException & ex)
	{
      DEBUGL1("Stage2::__user__getCount:CException:Description = %s\n", ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), "", eReceiverError) != STATUS_OK)
		DEBUGL4("__user__getCount CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}
   DEBUGL8("__user__getCount:Exit\n");

	return SOAP_OK;

}
#endif

/**
 * ImportUserInfo
 */
void UserServices::ImportUserInfo(soap* soap,
                                  BOProxyRef &pBOProxy,
                                  user__importUserInfoType *importInfo, 
                                  _user__importUserInfoResponse *user__importUserInfoResponse,
                                  bool isValidSession)
{
   DEBUGL8("ImportUserInfo:Enter\n");
   CString sFilePath;
   ImportInfoStruct   importInput;
   importInput.strFileName     = importInfo->FileName;
   if(importInput.strFileName.empty())
   {
		sFilePath = CString (STAGE2_BIT_BUCKET_FILE);
	
	}
	else
	{
		importInput.strFolderName  = CCommonFunctions::GetTemporaryStoragePath(pBOProxy);
		sFilePath          = importInput.strFolderName + "/" + importInput.strFileName;		
	}
   DEBUGL8("Download File path is ++%s++\n", sFilePath.c_str());

   //Download the package
   CCommonFunctions::DownloadBase64Data(importInfo->Data, soap, sFilePath);

   //We are waiting until the file download is complete to send any faults.
   //The client may not receive the fault if it is sent out when the file
   //is still being downloaded
   if(!isValidSession)
      CErrorCodeMapper::GenerateException("STG2_INVALID_SESSION");
	  
	if(importInput.strFileName.empty())
      CErrorCodeMapper::GenerateException("STG2_EMPTY_FILENAME");

   //Check if another long running operation initiated thro Stage2,
   //is in progress
   CLongRunningOpHelper::IsAnotherLongRunningOpInProgress();

   if(common__importMethodNameType__Addition == importInfo->MethodType || common__importMethodNameType__AdditionWithCounterReset == importInfo->MethodType)
   {
	   DEBUGL2("ImportUserInfo: Invalid method type: Addition\n");
	   CErrorCodeMapper::GenerateException("STG2_UNSUPPORTED_METHOD");
   }
   else
      importInput.method = importInfo->MethodType;

   importInput.strImportType = "UserInfoImport";
   if(true == importInfo->RoleGroup)
   {
	   importInput.strImportType = "RoleImport";
   }

   DEBUGL8("ImportUserInfo: Import type is: %s \n", importInput.strImportType.c_str());

   importInput.strOperationName = "importUserInfo";

   importInput.timeOut = 0;
   if(importInfo->TimeOut)
   {
      importInput.timeOut = atoi((*(importInfo->TimeOut)).c_str());
   }

   if(importInput.timeOut <= ASYNC_OP_THRESHOLD) //synchronous - business as usual
   {
      DEBUGL8("ImportUserInfo:Synchronous\n");
      //importInput.strOperationId              = CUUID().toString(); //we give a uuid ourselves in this case
      user__importUserInfoResponse->Response  = CLongRunningOpHelper::ProcessStage2SynchronousImport(soap, pBOProxy, importInput);
   }
   else //asynchronous
   {
      DEBUGL8("ImportUserInfo:Asynchronous\n");
      user__importUserInfoResponse->Response = CLongRunningOpHelper::ProcessStage2AsynchronousImport(soap, pBOProxy, importInput);
   }
	DEBUGL8("ImportUserInfo:Exit\n");
	
}

/**
 * ProcessExportUserInfoInput - Process export user info input params
 */
void UserServices::ProcessExportUserInfoInput( user__exportUserInfoType *exportUserInfo, 
                                              ExportInfoStruct& exportInput) 
{
   DEBUGL8("ProcessExportUserInfoInput:Enter\n");
   //Check if user specified export type
	if(exportUserInfo->ExtendedInfo) //we support csv for all except combined
	{
		switch (*(exportUserInfo->ExtendedInfo))
		{	case user__exportUserExtendedInfoNameType__SizeCounter:
			{
				exportInput.strExportType = "UserSLCounterExport";
            if(exportUserInfo->ExportFileFormat == common__exportFileFormatNameType__XML_)
               CErrorCodeMapper::GenerateException("STG2_UNSUPPORTED_FILE_FORMAT");

				break;
			}
			case user__exportUserExtendedInfoNameType__AllCounter:
			{
				exportInput.strExportType = "UserAllCounterExport";
            if(exportUserInfo->ExportFileFormat == common__exportFileFormatNameType__XML_)
               CErrorCodeMapper::GenerateException("STG2_UNSUPPORTED_FILE_FORMAT");

				break;
			}	
         case user__exportUserExtendedInfoNameType__RoleGroup:
			{
            if(exportUserInfo->ExportFileFormat == common__exportFileFormatNameType__CSV)
               CErrorCodeMapper::GenerateException("STG2_UNSUPPORTED_FILE_FORMAT");

				exportInput.strExportType = "RoleInfoExport";
				break;
			}
			case user__exportUserExtendedInfoNameType__Combined:
			{
            if(exportUserInfo->ExportFileFormat == common__exportFileFormatNameType__CSV)
               CErrorCodeMapper::GenerateException("STG2_UNSUPPORTED_FILE_FORMAT");

				exportInput.strExportType = "RoleAllExport";
				break;
			}
		}
	}
   else //if extendedInfo is not provided, we support only csv
   {
      //Check for valid file format
      if(exportUserInfo->ExportFileFormat == common__exportFileFormatNameType__XML_)
         CErrorCodeMapper::GenerateException("STG2_UNSUPPORTED_FILE_FORMAT");
   }

   //Set file extn based on file format
   switch(exportUserInfo->ExportFileFormat)
   {
	   case common__exportFileFormatNameType__XML_:
	   {
	      exportInput.strFileName += ".xml";
         break;
	   }
	   case common__exportFileFormatNameType__CSV:
	   {
		   exportInput.strFileName += ".csv";	
		   break;	
	   }
   }

   DEBUGL8("ProcessExportUserInfoInput:Exit\n");

}

/**
 * ExportUserInfo - Exports user info
 */
void UserServices::ExportUserInfo(soap* soap,
                                    BOProxyRef &pBOProxy,
                                    user__exportUserInfoType *exportUserInfo, 
                                    _user__exportUserInfoResponse *user__exportInfoResponse)
{
   DEBUGL8("ExportUserInfo:Enter\n");
   ExportInfoStruct   exportInput;

	exportInput.strFolderName  = CCommonFunctions::GetTemporaryStoragePath(pBOProxy) ; //pBOProxy->getSessionDataStoragePath();
	exportInput.strFileName    = USER_INFO_EXPORT_FILE;

	exportInput.strExportType  = "UserInfoExport";

   //Check if another long running operation initiated thro Stage2,
   //is in progress
   CLongRunningOpHelper::IsAnotherLongRunningOpInProgress();
   exportInput.exportFormat = exportUserInfo->ExportFileFormat;

   //Validate the input
   ProcessExportUserInfoInput(exportUserInfo, exportInput);

   DEBUGL8("ExportUserInfo:FileName is: %s\n", exportInput.strFileName.c_str());
   DEBUGL8("ExportUserInfo:Export type is: %s\n", exportInput.strExportType.c_str());

   exportInput.strOperationName = "exportUserInfo";

   //Check for timeout
   exportInput.timeOut     = 0;
   if(exportUserInfo->TimeOut)
   {
      exportInput.timeOut = atoi((*(exportUserInfo->TimeOut)).c_str());
   }

   if(exportInput.timeOut <= ASYNC_OP_THRESHOLD) //synchronous - business as usual
   {
      user__exportInfoResponse->Response = CLongRunningOpHelper::ProcessStage2SynchronousExport(soap,pBOProxy, exportInput);
   }
   else //asynchronous
   {
      DEBUGL8("ExportUserInfo:Asynchronous\n");
      user__exportInfoResponse->Response = CLongRunningOpHelper::ProcessStage2AsynchronousExport(soap,pBOProxy, exportInput);
   }

	DEBUGL8("ExportUserInfo:Exit\n");
	
}

/**
 * ImportDepartmentInfo
 */
void UserServices::ImportDepartmentInfo(soap* soap,
                                       BOProxyRef &pBOProxy,
                                       user__importDepartmentInfoType *importInfo, 
                                       _user__importDepartmentInfoResponse *user__importInfoResponse,
                                       bool isValidSession)
{
   DEBUGL8("ImportDepartmentInfo:Enter\n");
	CString sFilePath ;
   ImportInfoStruct   importInput;
   importInput.strFileName     = importInfo->FileName;
   if(importInput.strFileName.empty())
    {
		sFilePath = CString (STAGE2_BIT_BUCKET_FILE);		
	}
	else
	{
		importInput.strFolderName  = CCommonFunctions::GetTemporaryStoragePath(pBOProxy);
		sFilePath          = importInput.strFolderName + "/" + importInput.strFileName;		
	}
   DEBUGL8("ImportDepartmentInfo:Download File path is ++%s++\n", sFilePath.c_str());

   //Download the package
   CCommonFunctions::DownloadBase64Data(importInfo->Data, soap, sFilePath);

   //We are waiting until the file download is complete to send any faults.
   //The client may not receive the fault if it is sent out when the file
   //is still being downloaded
   if(!isValidSession)
      CErrorCodeMapper::GenerateException("STG2_INVALID_SESSION");
	  
	if(importInput.strFileName.empty())
      CErrorCodeMapper::GenerateException("STG2_EMPTY_FILENAME");	

   //Check if another long running operation initiated thro Stage2,
   //is in progress
   CLongRunningOpHelper::IsAnotherLongRunningOpInProgress();

   importInput.method = importInfo->MethodType;

   importInput.strImportType     = "DeptInfoImport";

   DEBUGL8("ImportDepartmentInfo: Import type is: %s \n", importInput.strImportType.c_str());

   importInput.strOperationName  = "importDepartmentInfo";

   importInput.timeOut = 0;
   if(importInfo->TimeOut)
   {
      importInput.timeOut = atoi((*(importInfo->TimeOut)).c_str());
   }  

   if(importInput.timeOut <= ASYNC_OP_THRESHOLD) //synchronous - business as usual
   {
      DEBUGL8("ImportDepartmentInfo:Synchronous\n");
      user__importInfoResponse->Response      = CLongRunningOpHelper::ProcessStage2SynchronousImport(soap, pBOProxy, importInput);
   }
   else //asynchronous
   {
      DEBUGL8("ImportDepartmentInfo:Asynchronous\n");
      user__importInfoResponse->Response = CLongRunningOpHelper::ProcessStage2AsynchronousImport(soap, pBOProxy, importInput);

   } //end of asynchronous
	DEBUGL8("ImportDepartmentInfo:Exit\n");
	
}

/**
 * ProcessExportDeptInfoInput
 */
void UserServices::ProcessExportDeptInfoInput( user__exportDepartmentInfoType *exportInfo,
                                                ExportInfoStruct& exportInput)
{
   DEBUGL8("ProcessExportDeptInfoInput:Enter\n");
	switch(exportInfo->ExportFileFormat)
	{
		case common__exportFileFormatNameType__XML_:
		{
		   CErrorCodeMapper::GenerateException("STG2_UNSUPPORTED_FILE_FORMAT");
		}
		case common__exportFileFormatNameType__CSV:
		{
			exportInput.strFileName += ".csv";	
			break;	
		}
	}	
		
   //Check if user specified export type
	if(exportInfo->ExtendedInfo)
	{
		switch (*(exportInfo->ExtendedInfo))
		{
			case user__exportDepartmentExtendedInfoNameType__SizeCounter:
			{
				exportInput.strExportType = "DeptSLCounterExport";
				break;
			}
			case user__exportDepartmentExtendedInfoNameType__AllCounter:
			{
				exportInput.strExportType = "DeptAllCounterExport";
				break;
			}			
		}				
	}
   DEBUGL8("ProcessExportDeptInfoInput:Exit\n");

}

/**
 * ExportDeptInfo
 */
void UserServices::ExportDeptInfo(soap* soap,
                                  BOProxyRef &pBOProxy,
                                  user__exportDepartmentInfoType *exportInfo, 
                                  _user__exportDepartmentInfoResponse *exportInfoResponse)
{
   DEBUGL8("ExportDeptInfo:Enter\n");

   ExportInfoStruct   exportInput;

	exportInput.strFolderName  = CCommonFunctions::GetTemporaryStoragePath(pBOProxy) ; //pBOProxy->getSessionDataStoragePath();
	exportInput.strFileName    = DEPT_INFO_EXPORT_FILE;
   exportInput.strExportType  = "DeptInfoExport";

   //Check if another long running operation initiated thro Stage2,
   //is in progress
   CLongRunningOpHelper::IsAnotherLongRunningOpInProgress();

   //Validate the input
   ProcessExportDeptInfoInput(exportInfo, exportInput);

   DEBUGL8("ExportDeptInfo:FileName is: %s\n", exportInput.strFileName.c_str());

   exportInput.strOperationName  = "exportDepartmentInfo";
   exportInput.exportFormat      = exportInfo->ExportFileFormat;

   exportInput.timeOut     = 0;
   if(exportInfo->TimeOut)
   {
      exportInput.timeOut = atoi((*(exportInfo->TimeOut)).c_str());
   }

   if(exportInput.timeOut <= ASYNC_OP_THRESHOLD) //synchronous - business as usual
   {
      DEBUGL8("ExportDeptInfo:Synchronous\n");
      exportInfoResponse->Response = CLongRunningOpHelper::ProcessStage2SynchronousExport(soap,pBOProxy, exportInput);
   }
   else //asynchronous
   {
      DEBUGL8("ExportDeptInfo:Asynchronous\n");
      exportInfoResponse->Response = CLongRunningOpHelper::ProcessStage2AsynchronousExport(soap,pBOProxy, exportInput);
   } //end of asynchronous

	DEBUGL8("ExportDeptInfo:Exit\n");
	
}

#ifdef STAGE2_UNSUPPORTED
/**
 * GetRBACCounts
 */
RBACCountStruct UserServices::GetRBACCounts(BOProxyRef& pBOProxy, 
                                            user__countEntityType* entityType)
{
	DEBUGL8("GetRBACCounts:Enter\n");

   ssdk::SSDKStatus status;
   Ref<SSDKSecurityManagerInterface>  ssdkSecurityManagerInterface  = ssdk::GetSecurityManager();
   Ref<SSDKUserTokenInterface>        userInterface                 = dynamic_cast<SSDKUserTokenInterface*> 								         
	(ssdkSecurityManagerInterface->GetInterface(SSDKSecurityManagerInterface::USER_TOKEN_INTERFACE,status));
   Ref<SSDKRBACMgmtInterface> rbacInterface                         = dynamic_cast<SSDKRBACMgmtInterface*> 		
	(ssdkSecurityManagerInterface->GetInterface(SSDKSecurityManagerInterface::RBAC_MGMT_INTERFACE, status));

   //Get the autoprocessing token
   //SSDKUserTokenInterface *adminToken = NULL;
   //status   =  userInterface->Authenticate(SSDKUserTokenInterface::TOKEN_CONTEXT_AUTOPROCESSING,adminToken);
   Ref<SSDKUserTokenInterface> userTokenRef = NULL;
	if(STATUS_OK != pBOProxy->GetUserToken(userTokenRef))
	{
		DEBUGL2("GetRBACCounts::Not able to get token from boserver.\n");
		CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");
	}

   SSDKUserTokenInterface* pUserToken     = userTokenRef.operator->();
   string                  query          = "";
   bool                    bGetAll        = false;
   bool                    bGetUser       = false;
   bool                    bGetRole       = false;
   bool                    bGetGroup      = false;


   RBACCountStruct         rbacCounts;
   rbacCounts.userCount   = 0;
   rbacCounts.groupCount  = 0;
   rbacCounts.roleCount   = 0;

   if(!entityType)
   {
      bGetAll = true;
      DEBUGL8("GetRBACCounts:Getting all counts\n");
   }

   if(entityType)
   {
      if(*entityType == user__countEntityType__User)
      {
         bGetUser = true;
      }
      else if(*entityType == user__countEntityType__Role)
      {
         bGetRole = true;
      }
      else if(*entityType == user__countEntityType__Group)
      {
         bGetGroup = true;
      }
   }

   if(bGetAll || bGetUser)
   {
      vector<SSDKRBACMgmtInterface::UserInfo> users;
      status = rbacInterface->GetProperty(pUserToken,
                                          SSDKRBACMgmtInterface::USER_ID_PROPERTY,
                                          SSDKRBACMgmtInterface::USER_ID_PROPERTY,
                                          query,
                                          users);      
      if(OK == status)
      {
          rbacCounts.userCount = users.size();
      }
      else
      {
          DEBUGL8("\nStatus::%d\n",status);
          if(status == AUTH_BAD_AUTHORIZATION)	
             CErrorCodeMapper::GenerateException("STATUS_USER_UNAUTHORIZED");
      }
      DEBUGL8("GetRBACCounts:UserCount:%d\n", rbacCounts.userCount);
   }

   if(bGetAll || bGetRole)
   {
      vector<SSDKRBACMgmtInterface::RoleInfo> roles;
      query    = "";
      status   = rbacInterface->GetProperty(pUserToken,
                                          SSDKRBACMgmtInterface::ROLE_ID_PROPERTY,
                                          SSDKRBACMgmtInterface::ROLE_ID_PROPERTY,
                                          query,
                                          roles);
      if(OK == status)
      {
          rbacCounts.roleCount = roles.size();
      }
      else
      {
          DEBUGL8("\nStatus::%d\n",status);
          if(status == AUTH_BAD_AUTHORIZATION)	
             CErrorCodeMapper::GenerateException("STATUS_USER_UNAUTHORIZED");
      }

      DEBUGL8("GetRBACCounts:RoleCount:%d\n", rbacCounts.roleCount);
   }

   if(bGetAll || bGetGroup)
   {
      vector<SSDKRBACMgmtInterface::GroupInfo> groups;
      query    = "";
      status   = rbacInterface->GetProperty(pUserToken,
                                             SSDKRBACMgmtInterface::GROUP_ID_PROPERTY,
                                             SSDKRBACMgmtInterface::GROUP_ID_PROPERTY,
                                             query,
                                             groups);
      if(OK == status)
      {
          rbacCounts.groupCount = groups.size();
      }
      else
      {
         DEBUGL8("GetRBACCounts:Status::%d\n",status);
         if(status == AUTH_BAD_AUTHORIZATION)	
            CErrorCodeMapper::GenerateException("STATUS_USER_UNAUTHORIZED");
      }

      DEBUGL8("GetRBACCounts:GroupCount:%d\n", rbacCounts.groupCount);
   }
   
	DEBUGL8("GetRBACCounts:Exit\n");

   return rbacCounts;

}
#endif
SOAP_FMAC5 int SOAP_FMAC6 __stg2__deleteAllEWBHistory(struct soap* soap, _user__deleteAllEWBHistory *user__deleteAllEWBHistory, _user__deleteAllEWBHistoryResponse *user__deleteAllEWBHistoryResponse)
{
 DEBUGL8("Stage2::__stg2__deleteAllEWBHistory entry\n");
 if(!CCommonFunctions::IsOutputManagementEnabled( ))
 {
  stage2ErrorStruct stError;
  CErrorCodeMapper::MapInternalErrCodeToStage2Fault(stError, "STG2_UNSUPPORTED_OPERATION");
  CCommonFunctions::GenerateSoapFault(soap, stError.strDescription, stError.strErrCode, stError.eErrorType);
  return SOAP_FAULT;
 }
 return __user__deleteAllEWBHistory(soap,user__deleteAllEWBHistory,user__deleteAllEWBHistoryResponse);
}
SOAP_FMAC5 int SOAP_FMAC6 __user__deleteAllEWBHistory(struct soap* soap, _user__deleteAllEWBHistory *user__deleteAllEWBHistory, _user__deleteAllEWBHistoryResponse *user__deleteAllEWBHistoryResponse)
{
	DEBUGL8("__user__deleteAllEWBHistory entry\n");
	try
	{
		BOProxyRef pBOProxy = CCommonFunctions::getBOProxyRef(soap);
		if(!pBOProxy)
		{
			DEBUGL1("__user__deleteAllEWBHistory Invalid Session\n");
			CErrorCodeMapper::GenerateException("STG2_INVALID_SESSION");
		}
		
		if(!user__deleteAllEWBHistory)
		{
			CErrorCodeMapper::GenerateException("STG2_MISSING_PARAM");
		}

		user__deleteAllEWBHistoryResponse->Result = common__operationResultType__OperationSucceeded;
		CString sOperationStatus = UserServices::ExecuteDeleteAllEWBHistoryCommand(pBOProxy);
		if(STG2_SUCCESS != sOperationStatus)
		{
			DEBUGL1("UserServices::ExecuteDeleteAllEWBHistoryCommand operation failed\n");
			CErrorCodeMapper::GenerateException(sOperationStatus,USR_MGR);
		}
				
	}
	catch(CStg2Exception & ex)
	{
		DEBUGL1("Stage2::__user__deleteAllEWBHistory caught Stg2Exception Exception. Description = %s\n", ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), ex.getFaultType(), ex.getErrorType()) != STATUS_OK)
		DEBUGL4("__user__deleteAllEWBHistory CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}
	catch(CException & ex)
	{
		DEBUGL1("Stage2::__user__deleteAllEWBHistory caught CException Exception. Description = %s\n", ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), "", eReceiverError) != STATUS_OK)
		DEBUGL4("__user__deleteAllEWBHistory CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}
	DEBUGL8("Stage2::__user__deleteAllEWBHistory exit\n");

	return SOAP_OK;
}

CString UserServices::ExecuteDeleteAllEWBHistoryCommand(const BOProxyRef &boProxy)
{
	DEBUGL8("UserServices::ExecuteDeleteAllEWBHistoryCommand enter\n");
	CString strResponse;
	if(STATUS_OK != CUIControllerAdapter::SendCommand("DeleteEWBHistory",USERS_XPATH,strResponse,boProxy))
	{
		DEBUGL1("UserServices::ExecuteDeleteAllEWBHistoryCommand Failed to SendCommand\n");
		CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");		
	}

	return CUIControllerAdapter::GetOperationStatusString(strResponse);
		
}
void UserServices::GetUserData(Ref<BOProxy>& pBOProxy,struct userDataStruct& userData)
{
	DEBUGL8("CCommonFunctions::GetUserData Enter\n");
	CString sOperationStatus = ExecuteGetUserCommand(userData.sID,pBOProxy);
	if(STG2_SUCCESS != sOperationStatus)
	{
		CErrorCodeMapper::GenerateException(sOperationStatus,USR_MGR);
	}
	
	CString sXPath = "UserManager/Users/User[@ID='" + userData.sID +"']/Information/";
	CString sFactoryDefaultUser = CCommonFunctions::GetValue(sXPath+ "isFactoryDefaultUser", pBOProxy);
	userData.IsBuiltInUser = sFactoryDefaultUser == "YES" ?true:false;
	CString sExternalUser = CCommonFunctions::GetValue(sXPath+ "isExternalUser", pBOProxy);
	userData.IsExternalUser =  sExternalUser == "true" ? true:false;
	DEBUGL8("CCommonFunctions:::GetUserData sBuiltInUser=[%s] ExternalUser[%s] Exit\n",sFactoryDefaultUser.c_str(),sExternalUser.c_str());
}
//WSS 1097 - Added on 27-may-2013
SOAP_FMAC5 int SOAP_FMAC6 __user__deleteAllUserCache(struct soap* soap, _user__deleteAllUserCache *user__deleteAllUserCache, _user__deleteAllUserCacheResponse *user__deleteAllUserCacheResponse)
{
	DEBUGL8("__user__deleteAllUserCache entry\n");
	try
	{
		BOProxyRef pBOProxy = CCommonFunctions::getBOProxyRef(soap);
		if(!pBOProxy)
		{
			DEBUGL1("__user__deleteAllUserCache Invalid Session\n");
			CErrorCodeMapper::GenerateException("STG2_INVALID_SESSION");
		}
		
		if(!user__deleteAllUserCache)
		{
			CErrorCodeMapper::GenerateException("STG2_MISSING_PARAM");
		}

		user__deleteAllUserCacheResponse->Result = common__operationResultType__OperationSucceeded;
		CString sOperationStatus = UserServices::ExecuteDeleteAllUserCache(pBOProxy);
		if(STG2_SUCCESS != sOperationStatus)
		{
			DEBUGL1("UserServices::ExecuteDeleteAllUserCache Command operation failed\n");
			CErrorCodeMapper::GenerateException(sOperationStatus,USR_MGR);
		}
				
	}
	catch(CStg2Exception & ex)
	{
		DEBUGL1("Stage2::__user__deleteAllUserCache caught Stg2Exception Exception. Description = %s\n", ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), ex.getFaultType(), ex.getErrorType()) != STATUS_OK)
		DEBUGL4("__user__deleteAllUserCache CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}
	catch(CException & ex)
	{
		DEBUGL1("Stage2::__user__deleteAllUserCache caught CException Exception. Description = %s\n", ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), "", eReceiverError) != STATUS_OK)
		DEBUGL4("__user__deleteAllUserCache CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}
	DEBUGL8("Stage2::__user__deleteAllUserCache exit\n");

	return SOAP_OK;
}
//WSS 1097 - Added on 27-may-2013
CString UserServices::ExecuteDeleteAllUserCache(const BOProxyRef &boProxy)
{
	DEBUGL8("UserServices::ExecuteDeleteAllUserCache enter\n");
	CString strResponse;
	if(STATUS_OK != CUIControllerAdapter::SendCommand("DeleteCache",USERS_XPATH,strResponse,boProxy))
	{
		DEBUGL1("UserServices::ExecuteDeleteAllUserCache Failed to SendCommand\n");
		CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");		
	}
	return CUIControllerAdapter::GetOperationStatusString(strResponse);
}
//WSS 1097 - Added on 27-may-2013
SOAP_FMAC5 int SOAP_FMAC6 __stg2__deleteAllUserCache(struct soap* soap, _user__deleteAllUserCache *user__deleteAllUserCache, _user__deleteAllUserCacheResponse *user__deleteAllUserCacheResponse)
{
	 DEBUGL8("Stage2::__stg2__deleteAllUserCache entry\n");
	 if(!CCommonFunctions::IsOutputManagementEnabled( ))
	 {
	  stage2ErrorStruct stError;
	  CErrorCodeMapper::MapInternalErrCodeToStage2Fault(stError, "STG2_UNSUPPORTED_OPERATION");
	  CCommonFunctions::GenerateSoapFault(soap, stError.strDescription, stError.strErrCode, stError.eErrorType);
	  return SOAP_FAULT;
	 }
	 return __user__deleteAllUserCache(soap,user__deleteAllUserCache,user__deleteAllUserCacheResponse);
}


SOAP_FMAC5 int SOAP_FMAC6 __user__loginMDS(struct soap* soap,_userauth__loginMDS* userauth__loginMDS,_userauth__loginMDSResponse* userauth__loginMDSResponse)
{
	DEBUGL8("Stage2::_user__logintMDS entry\n");
	try{
		if(!userauth__loginMDS->MDSUserInfo || !userauth__loginMDSResponse)
		{
			CErrorCodeMapper::GenerateException("STG2_MISSING_PARAM");
		}
		//CString sSession = "";
		CString sSession = "PANEL_RENDERER_SESSION";
		BOProxyRef pBOProxy = BOProxy::Acquire(sSession);
		if(!pBOProxy)
		{
			DEBUGL1("__user__loginMDS:Failed to acquire BOProxy\n");
			CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");
		}
		CString sMDSMode = "";
		if(pBOProxy->GetValue("Controller/Settings/Functions/MDS/Authentication",sMDSMode) != STATUS_OK)
		{
			DEBUGL1("Failed in getvalue of controller dom!\n");
			CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");
		}
		if(sMDSMode != "true")
		{
			DEBUGL1("MDS Mode is been disabled!!!\n");
			CErrorCodeMapper::GenerateException("STG2_UNSUPPORTED_OPERATION");
		}
		
		CString sUserName = userauth__loginMDS->MDSUserInfo->Name;
		if(sUserName.empty())
		{
			DEBUGL1("Error result obtain from panel!\n");
			throw CStg2Exception(eSenderError, "ERR_EBS_COMMON_PAR_INVALID_VALUE", SOAP_FAULT, "Invalid user name");
		}
		CString sSetValues = "<UserCredential>";		
		sSetValues += "<userName>"+CCommonFunctions::EscapedXMLString(sUserName)+"</userName>";
		//CString sExternalUser = "";
		//if(userauth__loginMDS->MDSUserInfo->ExternalUser)
		//	sExternalUser = *(userauth__loginMDS->MDSUserInfo->ExternalUser) == true ?true:false;
		if(userauth__loginMDS->MDSUserInfo->Domain)
		{
				sSetValues += "<domainName>"+CCommonFunctions::EscapedXMLString(*(userauth__loginMDS->MDSUserInfo->Domain))+"</domainName>";
		}
		CString sPasswd = "";
		if(userauth__loginMDS->MDSUserInfo->Password)
		{
			sPasswd = *(userauth__loginMDS->MDSUserInfo->Password);
			sSetValues += "<passwd>"+ CCommonFunctions::EscapedXMLString(sPasswd) +"</passwd>";	
		}
		//credentialType
		if(sPasswd.empty())
			sSetValues +="<credentialType>ID_GATE</credentialType>";
		else	
			sSetValues +="<credentialType>NORMAL</credentialType>";
		//Mds user	
		sSetValues += "<isMdsUser>true</isMdsUser>";
		
		if(userauth__loginMDS->MDSUserInfo->EMailId)
		{
			sSetValues += "<emailAddress>"+CCommonFunctions::EscapedXMLString(*(userauth__loginMDS->MDSUserInfo->EMailId))+"</emailAddress>";	
		}
		//CString sDepartment = "";
		//if(userauth__loginMDS->MDSUserInfo->Department)
		//	sDepartment = *(userauth__loginMDS->MDSUserInfo->Department);	
		if(userauth__loginMDS->MDSUserInfo->Preference)
		{
			if(userauth__loginMDS->MDSUserInfo->Preference->PanelLanguage)
			{	
				sSetValues += "<copierLanguage>"+ CCommonFunctions::EscapedXMLString(*(userauth__loginMDS->MDSUserInfo->Preference->PanelLanguage))+"</copierLanguage>";
			}
			
			if(userauth__loginMDS->MDSUserInfo->Preference->PanelKeyboardLayout)
			{
				sSetValues += "<userKeyBoard>"+CCommonFunctions::EscapedXMLString(*(userauth__loginMDS->MDSUserInfo->Preference->PanelKeyboardLayout))+"</userKeyBoard>";	
			}
		}
		if(userauth__loginMDS->MDSUserInfo->PrivateHomeDirectory)
		{
			sSetValues += "<homeDirectory>"+CCommonFunctions::EscapedXMLString(*(userauth__loginMDS->MDSUserInfo->PrivateHomeDirectory))+"</homeDirectory>";	
		}
		CString sQuotaEnable = "";
		if(userauth__loginMDS->MDSUserInfo->Quota)
		{
			CString sQuotaSetValue = "<AccountingProperty><Quota>";
			if(userauth__loginMDS->MDSUserInfo->Quota->Total)
			{
				sQuotaEnable = ((userauth__loginMDS->MDSUserInfo->Quota->Total->Enable) == true) ? "true":"false";
				sQuotaSetValue += "<jobQuotaEnabled>"+sQuotaEnable+"</jobQuotaEnabled>";
				if(userauth__loginMDS->MDSUserInfo->Quota->Total->Balance)
				{	
					sQuotaSetValue += "<jobRemainingQuantity>"+CCommonFunctions::EscapedXMLString(*(userauth__loginMDS->MDSUserInfo->Quota->Total->Balance))+"</jobRemainingQuantity>";
				}
				else if(sQuotaEnable == "true")
				{
					sQuotaSetValue += "<jobRemainingQuantity>0</jobRemainingQuantity>";
				}
			}
			if(userauth__loginMDS->MDSUserInfo->Quota->Job)
		   {
				if(userauth__loginMDS->MDSUserInfo->Quota->Job->Print)
				{
					common__printQuotaCategoryType *pPrint = userauth__loginMDS->MDSUserInfo->Quota->Job->Print;
					if(pPrint->__union_printQuotaCategoryType == SOAP_UNION__common__union_printQuotaCategoryType_ColorJobQuota)
					{
						//common__colorJobQuotaType *pColorJobQuota = soap_new_common__colorJobQuotaType(soap,-1);
						common__colorJobQuotaType *pColorJobQuota = pPrint->union_printQuotaCategoryType.ColorJobQuota;
						if(pColorJobQuota)
						{
							if(pColorJobQuota->ColorQuota)
							{
								sQuotaEnable = ((pColorJobQuota->ColorQuota->Enable) == true) ? "true":"false";
								sQuotaSetValue += "<fullColorQuotaEnabled>"+sQuotaEnable+"</fullColorQuotaEnabled>";
								if(pColorJobQuota->ColorQuota->Balance)
									sQuotaSetValue += "<fullColorRemainingQuantity>"+CCommonFunctions::EscapedXMLString(*(pColorJobQuota->ColorQuota->Balance))+"</fullColorRemainingQuantity>";
								else if(sQuotaEnable == "true")
									sQuotaSetValue += "<fullColorRemainingQuantity>0</fullColorRemainingQuantity>";
							}
							if(pColorJobQuota->BlackQuota)
							{
								sQuotaEnable = ((pColorJobQuota->BlackQuota->Enable) == true) ? "true":"false";
								sQuotaSetValue += "<blackQuotaEnabled>"+sQuotaEnable+"</blackQuotaEnabled>";
								if(pColorJobQuota->BlackQuota->Balance)
									sQuotaSetValue += "<blackRemainingQuantity>"+CCommonFunctions::EscapedXMLString(*(pColorJobQuota->BlackQuota->Balance))+"</blackRemainingQuantity>";
								else if(sQuotaEnable == "true")
									sQuotaSetValue += "<blackRemainingQuantity>0</blackRemainingQuantity>";
							}
						}
					}
				}
			}
			sQuotaSetValue+= "</Quota></AccountingProperty>";
			sSetValues += sQuotaSetValue;
		}
		//Permission..
		CString sPermissions = "";
		if(userauth__loginMDS->MDSUserInfo->PermissionList)
		{
			if(userauth__loginMDS->MDSUserInfo->PermissionList->Item.size() != 0)
			{
				vector<CString> sPermissionList = userauth__loginMDS->MDSUserInfo->PermissionList->Item;
				vector<CString> ::iterator it = sPermissionList.begin();
				sPermissions = "<Permissions>";
				for(;it!= sPermissionList.end();it++)
				{
					sPermissions += "<permission>"+ CCommonFunctions::EscapedXMLString(*it) +"</permission>";
				}
				sPermissions += "</Permissions>";
			}	
		}
		sSetValues += sPermissions;
		sSetValues += "</UserCredential>";
		CString sXPATH = "Panel/MDS/Login/UserCredential";
		
		CString sResultStatus = "";
		Status retStatus = STATUS_OK;
		retStatus = gMDSLoginOperationMutex->Acquire();
		if(retStatus != STATUS_OK)
		{
			DEBUGL1("Failed in acquire Mutex result[%d]!!\n",retStatus);
			CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");
		}
		try
		{
			CUIControllerAdapter::SetXMLValue("Panel/MDS/Login",sSetValues,pBOProxy);
			CUIControllerAdapter::GenerateNotification(CString("Panel/MDS/Login"),pBOProxy);
			CString sResutStatusXpath = "Panel/MDS/Login/Response/statusOfOperation"; 
			DEBUGL2("Getting the result status!!\n");
			int i = 0;
			for(; i < 150 && sResultStatus.empty(); ++i){
				pBOProxy->GetValue(sResutStatusXpath.c_str(), sResultStatus);
				usleep(30000);
			}

			if(i == 150 && sResultStatus.empty()){
				DEBUGL2("Timeout is occured!!\n");
				CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");
			}
			DEBUGL2("MDS Login sResultStatus[%s]\n",sResultStatus.c_str())	;
			CString sResetXPATH = "Panel/MDS/Login/UserCredential";
			//clear user Information		
			CUIControllerAdapter::ResetValue(sResetXPATH,pBOProxy);
			CUIControllerAdapter::ResetValue(sResutStatusXpath,pBOProxy);
		}
		catch(...)
		{
			gMDSLoginOperationMutex->Release();
			DEBUGL1("Failed in sending user crendial data to panel!\n");
			CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");
		}

		gMDSLoginOperationMutex->Release();
		
		if(sResultStatus == "STATUS_IN_USER_LOGIN" || sResultStatus == "STATUS_IN_MFP_USE" || sResultStatus == "STATUS_INVALID_INPUT" || sResultStatus == "STATUS_WRONG_USER_CRED")
		{
			DEBUGL1("Error result obtain from panel!\n");
			CErrorCodeMapper::GenerateException(sResultStatus, MDS);
		}
		
		userauth__loginMDSResponse->Response = soap_new_userauth__loginMDSResponseType(soap,-1);
		if(sResultStatus != "STATUS_OK")
		{
			if(sResultStatus == "STATUS_QUOTA_UPDATE_FAILED")
			{
				userauth__loginMDSResponse->Response->Extension = soap_new_common__entityListType(soap,-1);
				common__entityType* pEntity = soap_new_common__entityType(soap, -1);
				pEntity->__union_entityType = SOAP_UNION__common__union_entityType_Value;
				pEntity->Name = "ChangeQuota";
				pEntity->union_entityType.Value = soap_new_common__stringListType(soap, -1);
				pEntity->union_entityType.Value->Item.push_back("false");
				userauth__loginMDSResponse->Response->Extension->Entity.push_back(pEntity);
			}
			else// This includes "STATUS_FATAL_ERROR" also
			{	
				DEBUGL1("Error Status result obtained from Panel!\n");
				CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");
			}
		}
		userauth__loginMDSResponse->Response->Result = common__operationResultType__OperationSucceeded;
	}
	catch(CStg2Exception &ex)
	{
		DEBUGL1("Stage2::__user__loginMDS:Stg2Exception:Description = %s\n", ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), ex.getFaultType(), ex.getErrorType()) != STATUS_OK)
			DEBUGL1("__user__loginMDS CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}
	catch(CException &ex)
	{
		DEBUGL1("Stage2::__user__loginMDS:CException:Description = %s\n",ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap,ex.C_Str(),"",eReceiverError) != STATUS_OK)
			DEBUGL1("__user__loginMDS CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}
	DEBUGL8("Stage2::_user__logintMDS exit\n");
	return SOAP_OK;
}
SOAP_FMAC5 int SOAP_FMAC6 __stg2__loginMDS(soap* pSOAP, _userauth__loginMDS* userauth__loginMDS, _userauth__loginMDSResponse* userauth__loginMDSResponse)
{
  DEBUGL8("Stage2::__stg2__logintMDS entry\n");
  if(!CCommonFunctions::IsOutputManagementEnabled( ))
  {
   stage2ErrorStruct stError;
   CErrorCodeMapper::MapInternalErrCodeToStage2Fault(stError, "STG2_UNSUPPORTED_OPERATION");
   CCommonFunctions::GenerateSoapFault(pSOAP, stError.strDescription, stError.strErrCode, stError.eErrorType);
   return SOAP_FAULT;
  }
  return __user__loginMDS(pSOAP,userauth__loginMDS,userauth__loginMDSResponse);

}

void UserServices::SetLogoutExecuteValue(CString sXPath, CString sXML, BOProxyRef& BOProxy)
	{
		Status retStatus = STATUS_OK;
		CString strXML    = "<SetValue>";

		strXML += "<Payload XMLPayLoad = \"true\" overrideDelta = \"true\" sendNotification=\"true\"><path>" + sXPath + "</path>";
		strXML += "<value>" + sXML + "</value></Payload>";
		strXML += "</SetValue>";
		
		DEBUGL4("XML is ++%s++\n", strXML.c_str());

		//CCommonFunctions::FindReplace(strXML, "&" , "&amp;"); // Escape & from xml string.
		
		retStatus = BOProxy->SetValue(strXML.c_str());
		if (retStatus != STATUS_OK)
		{
			DEBUGL8("CUIControllerAdapter:SetLogoutExecuteValue: SetValue failed\n");
			CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");
		}
	}

Status UserServices::SetLogoutResponse(CString& sXPATH)
{
	DEBUGL8("UserServices::SetLogoutResponse entry\n");
	ci::hierarchicaldb::HierarchicalDBRef m_pHDB = HierarchicalDB::Acquire(NULL);
	if (!m_pHDB)
	{
		DEBUGL1("UserServices::SetLogoutResponse  Failed to Acquire HDB\n");
 	 	return false;
	}
	DocumentRef pPanelDom = NULL;
	if(m_pHDB->OpenDocument("Panel",pPanelDom,"/work/al/etc/dom/") != STATUS_OK)
	{
	  	DEBUGL1("UserServices::SetLogoutResponse  Failed to Open Panel DOM \n");
 	  	return false;
	}
	bool bStatus = false;
	m_pHDB->BeginTransaction(pPanelDom,eWRITE,true);

	NodeRef sOperationStatus = m_pHDB->BindToElement(pPanelDom,sXPATH);
	if(!sOperationStatus)
	{
		DEBUGL1("UserServices::SetLogoutResponse BindToElement for sOperationStatus path **%s**  failed\n",sXPATH.c_str());
		m_pHDB->EndTransaction(pPanelDom,eWRITE);
		return false;
	}


	sOperationStatus->setTextContent("0");
	m_pHDB->EndTransaction(pPanelDom,eWRITE);
	CString sStatus = sOperationStatus->getTextContent();
	DEBUGL4("UserServices::SetLogoutResponse  sStatus = %s\n",sStatus.c_str());

	
	DEBUGL8("UserServices::SetLogoutResponse exit\n");
	return STATUS_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __user__logoutMDS(struct soap* soap, _userauth__logoutMDS* userauth__logoutMDS, _userauth__logoutMDSResponse* userauth__logoutMDSResponse)
{
	DEBUGL8("Stage2::__user__logoutMDS entry\n");
	try
	{
		if(!userauth__logoutMDS || !userauth__logoutMDSResponse)
		{
			CErrorCodeMapper::GenerateException("STG2_MISSING_PARAM");
		}
		
		CString sSession = "PANEL_RENDERER_SESSION";
		BOProxyRef pBOProxy = BOProxy::Acquire(sSession);
		if(!pBOProxy)
		{
			DEBUGL1("__user__logoutMDS:Failed to acquire BOProxy\n");
			CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");
		}
		CString sMDSMode = "";
		if(pBOProxy->GetValue("Controller/Settings/Functions/MDS/Authentication",sMDSMode) != STATUS_OK)
		{
			DEBUGL1("Failed in getvalue of controller dom!\n");
			CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");
		}
		if(sMDSMode != "true")
		{
			DEBUGL1("MDS Mode is been disabled!!!\n");
			CErrorCodeMapper::GenerateException("STG2_UNSUPPORTED_OPERATION");
		}
		CString sXMLValue = "<Execute>true</Execute>"	;
		CString sXPATH = "Panel/MDS/Logout";
		CString sResultStatus = "0";
		Status retStatus = STATUS_OK;
		retStatus= gMDSLogoutOperationMutex->Acquire();
		if(retStatus != STATUS_OK)
		{
			DEBUGL1("Failed in acquire Mutex result[%d]!!\n",retStatus);
			CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");	
		}
		try
		{
			CString sResultStatusXpath = "Panel/MDS/Logout/Response/statusOfOperation";
			UserServices::SetLogoutResponse(sResultStatusXpath);
			UserServices::SetLogoutExecuteValue(sXPATH.c_str(),sXMLValue,pBOProxy);
			CUIControllerAdapter::GenerateNotification(sXPATH.c_str(),pBOProxy);
			DEBUGL2("Getting the result status!!\n");
			int i = 0;
			for(; i < 150 && sResultStatus == "0"; ++i){
				sResultStatus = CUIControllerAdapter::GetTextNodeValue(sResultStatusXpath.c_str());
				usleep(30000);
			}

			if(i == 150 && sResultStatus == "0"){
				DEBUGL2("Timeout is occured!!\n");
				CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");
			}

			DEBUGL2("MDS LogOut::sResultStatus[%s]\n",sResultStatus.c_str());
			UserServices::SetLogoutResponse(sResultStatusXpath);
			//CUIControllerAdapter::ResetValue(sXPATH,pBOProxy);
			
		}
		catch(...)
		{
			gMDSLogoutOperationMutex->Release();
			DEBUGL1("Failed in sending logout data to panel!\n");
			CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");
		}

		gMDSLogoutOperationMutex->Release();
		
		if(sResultStatus == "STATUS_NOT_USER_LOGIN" || sResultStatus == "STATUS_IN_MFP_USE" )
		{
			DEBUGL1("Error result obtain from panel!\n");
			CErrorCodeMapper::GenerateException(sResultStatus, MDS);
		}

		if(sResultStatus != "STATUS_OK")
		//if(sResultStatus == "STATUS_FATAL_ERROR" || sResultStatus == "STATUS_INVALID_INPUT" || sResultStatus == "STATUS_DB_CORRUPTED" )
		{
			DEBUGL1("Error Status result obtained from Panel!\n");
			CErrorCodeMapper::GenerateException("STG2_INTERNAL_SERVER_ERROR");
		}
		
		userauth__logoutMDSResponse->Result = common__operationResultType__OperationSucceeded;
		return SOAP_OK;
	}
	catch(CStg2Exception &ex)
	{
		DEBUGL1("Stage2::__user__logoutMDS:Stg2Exception:Description = %s\n", ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), ex.getFaultType(), ex.getErrorType()) != STATUS_OK)
			DEBUGL1("__user__logoutMDS CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}
	catch(CException &ex)
	{
		DEBUGL1("Stage2::__user__logoutMDS:CException:Description = %s\n",ex.C_Str());
		if(CCommonFunctions::GenerateSoapFault(soap,ex.C_Str(),"",eReceiverError) != STATUS_OK)
			DEBUGL1("__user__logoutMDS CCommonFunctions::GenerateSoapFault Failed\n");
		return SOAP_FAULT;
	}	
	DEBUGL8("Stage2::__user__logoutMDS exit\n");
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __stg2__logoutMDS(soap* pSOAP, _userauth__logoutMDS* userauth__logoutMDS, _userauth__logoutMDSResponse* userauth__logoutMDSResponse)
{
	 DEBUGL8("Stage2::__stg2__logoutMDS entry\n");
	 if(!CCommonFunctions::IsOutputManagementEnabled( ))
	 {
	  stage2ErrorStruct stError;
	  CErrorCodeMapper::MapInternalErrCodeToStage2Fault(stError, "STG2_UNSUPPORTED_OPERATION");
	  CCommonFunctions::GenerateSoapFault(pSOAP, stError.strDescription, stError.strErrCode, stError.eErrorType);
	  return SOAP_FAULT;
	 }
	 return __user__logoutMDS(pSOAP,userauth__logoutMDS,userauth__logoutMDSResponse);
	
} 


//Please do not remove - We may need this if MSM agrees to adding this new operation
//SOAP_FMAC5 int SOAP_FMAC6 __user__getRecentLongRunningOperations(struct soap* soap, _user__getRecentLongRunningOperations *user__getRecentLongRunningOperations, _user__getRecentLongRunningOperationsResponse *user__getRecentLongRunningOperationsResponse)
//{
//	DEBUGL8("__user__getRecentLongRunningOperations:Enter\n");
//
//	try
//	{
//		BOProxyRef  pBOProxy = CCommonFunctions::getBOProxyRef(soap);
//		if(!pBOProxy)
//		{
//         DEBUGL1("__user__getRecentLongRunningOperations:Invalid Session\n");
//			CErrorCodeMapper::GenerateException("STG2_INVALID_SESSION");		
//		}
//
//      CString whereClause = CString(STAGE2_OP_NAME) + "='" + user__getRecentLongRunningOperations->OperationName + "'";
//         
//      LongRunningOpStruct opResult;
//
//      CRDBInterface rdb(STAGE2_DB, STAGE2_OPRESULTS_TABLE);
//
//      std::vector<LongRunningOpStruct> vAsyncOpResults = rdb.SelectLongRunningOperation(whereClause);
//      if(vAsyncOpResults.size() == 0)
//      {
//			DEBUGL1("__user__getRecentLongRunningOperations:No entries for operation name\n");
//			//CErrorCodeMapper::GenerateException("STG2_INVALID_AYSNC_OP_ID");
//      }
//      else
//      {
//         DEBUGL8("__user__getRecentLongRunningOperations:NumResults %d: \n", vAsyncOpResults.size());
//
//         for (uint ii=0; ii<vAsyncOpResults.size(); ii++)
//         {
//            user__asyncOperationInfoType *asyncInfo   = soap_new_user__asyncOperationInfoType(soap,-1);
//            asyncInfo->OperationName                  = vAsyncOpResults[ii].strCommandName;
//            asyncInfo->AsyncOpId                      = vAsyncOpResults[ii].strAsyncOperationId;
//            asyncInfo->Status                         = vAsyncOpResults[ii].strStatus;
//            asyncInfo->ClientIP                       = vAsyncOpResults[ii].strClientIP;
//            asyncInfo->RecordingTime                  = vAsyncOpResults[ii].recordingTime;
//
//            user__getRecentLongRunningOperationsResponse->Operations.Operation.push_back(asyncInfo);
//         }
//      }
//				
//	}	
//	catch(CStg2Exception & ex)
//	{
//      DEBUGL1("Stage2::__user__getRecentLongRunningOperations:Stg2Exception:Description = %s\n", ex.C_Str());
//		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), ex.getFaultType(), ex.getErrorType()) != STATUS_OK)
//			DEBUGL1("__user__getRecentLongRunningOperations CCommonFunctions::GenerateSoapFault Failed\n");
//		return SOAP_FAULT;
//	}
//	catch(CException & ex)
//	{
//      DEBUGL1("Stage2::__user__getRecentLongRunningOperations:CException:Description = %s\n", ex.C_Str());
//		if(CCommonFunctions::GenerateSoapFault(soap, ex.C_Str(), "", eReceiverError) != STATUS_OK)
//			DEBUGL1("__user__getRecentLongRunningOperations CCommonFunctions::GenerateSoapFault Failed\n");
//		return SOAP_FAULT;
//	}
//   return STATUS_OK;
//}
