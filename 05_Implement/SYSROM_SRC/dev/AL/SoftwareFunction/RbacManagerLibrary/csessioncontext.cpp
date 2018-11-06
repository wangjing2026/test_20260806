/******************************************************************************
 * @Copyright (C) 2010 Toshiba TEC Corp
 * @Workfile:  csessioncontext.cpp$
 * @Revision:  1 $
 * @Date:      20-Dec-2010$
 * @Author:    Manoj Kumar Saha
 * @Notes:
 *******************************************************************************/

#include "csessioncontext.h"
#include "CI/SI/ssdkinterface.h"
#include "CI/SI/ssdksecuritymanagerinterface.h"
#include "CI/SI/ssdkusertokeninterface.h"
#include "CI/SI/rbac_objects.h"
#include "CI/SI/rbac_operations.h"

using namespace al::uicontroller;
using namespace ssdk;
namespace al
{
	namespace softwarefunction
	{
		namespace rbacmanagerlibrary
		{
			SessionContext::SessionContext(const std::string & sessionName):m_sessionName(sessionName)
			{

			}
			Ref<SSDKUserTokenInterface> SessionContext::GetSessionUserToken() const
			{	
				Status retStatus;
				std::string functionName = "SessionContext::GetSessionUserToken";
				std::string sessionName = m_sessionName;
				BOProxyRef boProxy = BOProxy::Acquire(sessionName);
				//Check For NULL
				if(!boProxy)
				{
					DEBUGL2("Failed to Acquire BoProxy\n");
					return static_cast<Ref<SSDKUserTokenInterface> >(NULL);
				}
				Ref<SSDKUserTokenInterface> userToken = NULL;
				retStatus = boProxy->GetUserToken(userToken);
				if (retStatus != STATUS_OK)
				{
					DEBUGL1("%s : Failed. retStatus=%d\n", functionName.c_str(), retStatus);
					return static_cast<Ref<SSDKUserTokenInterface> >(NULL);
				}
				if (!userToken)
				{
					DEBUGL1("%s : Failed. The userToken is null.\n", functionName.c_str());
					return static_cast<Ref<SSDKUserTokenInterface> >(NULL);
				}

				return userToken;
			}

			Ref<SSDKUserTokenInterface> SessionContext::GetAutoProcessingUserToken() const
			{
				std::string functionName = "SessionContext::GetAutoProcessingUserToken";
				SSDKStatus retStatus = ssdk::OK;

				Ref<SSDKSecurityManagerInterface> ssdkSecurityManagerInterface = GetSecurityManager();
				if (!ssdkSecurityManagerInterface)
				{
					DEBUGL1("%s : Failed. The ssdkSecurityManagerInterface is null.\n", functionName.c_str());
					return static_cast<Ref<SSDKUserTokenInterface> >(NULL);
				}

				Ref<SSDKUserTokenInterface> usertokenInterface = dynamic_cast<SSDKUserTokenInterface*>(ssdkSecurityManagerInterface->GetInterface(SSDKSecurityManagerInterface::USER_TOKEN_INTERFACE, retStatus));
				if (!usertokenInterface )
				{
					DEBUGL1("%s : Failed. The usertokenInterface is null.\n", functionName.c_str());
					return NULL;
				}
				if (retStatus != ssdk::OK )
				{
					DEBUGL1("%s : Failed. retStatus=%d\n", functionName.c_str(), retStatus);
					return static_cast<Ref<SSDKUserTokenInterface> >(NULL);
				}
				SSDKUserTokenInterface* userToken = NULL;

				//Get Autoprocessing token
				retStatus = usertokenInterface->Authenticate(SSDKUserTokenInterface::TOKEN_CONTEXT_AUTOPROCESSING, userToken);
				if ((retStatus != ssdk::OK) && (retStatus != ssdk::AUTH_GOOD_AUTHENTICATION_NEEDS_PASSWORD_CHANGE))
				{
					DEBUGL1("%s : Failed. retStatus=%d\n", functionName.c_str(), retStatus);
					return static_cast<Ref<SSDKUserTokenInterface> >(NULL);
				}
				if(!userToken)
				{
					DEBUGL1("%s : Failed. The userToken is null.\n", functionName.c_str());
					return static_cast<Ref<SSDKUserTokenInterface> >(NULL);
				}

				return userToken;
			}
			void SessionContext::GetBoProxy(BOProxyRef & boProxy)
			{
				Status retStatus;
				std::string functionName = "SessionContext::GetSessionUserToken";
				std::string sessionName = m_sessionName;
				boProxy = BOProxy::Acquire(sessionName);
				//Check For NULL
				if(!boProxy)
					DEBUGL2("Failed to Acquire BoProxy\n");
			}

		}
	}
}
