/******************************************************************************
 * @Copyright (C) 2010 Toshiba TEC Corp
 * @Workfile:  csessioncontext.h$
 * @Revision:  1 $
 * @Date:      20-Dec-2010$
 * @Author:    Manoj Kumar Saha
 * @Notes:
 *******************************************************************************/

#ifndef AL_SOFTWARE_FUNCTIONS_GROUPMANAGER_LIBRARY_SESSION_CONTEXT_H
#define AL_SOFTWARE_FUNCTIONS_GROUPMANAGER_LIBRARY_SESSION_CONTEXT_H 

#include <string>
#include "CI/OperatingEnvironment/ref.h"
#include "CI/SI/ssdkusertokeninterface.h"
#include "CI/SI/ssdkinterface.h"
#include "AL/status.h"
#include "AL/SoftwareFunction/RbacManagerLibrary/rbacimporterfactory.h"
#include "AL/UIController/boproxy.h"
//#include "AL/SoftwareFunction/GroupManagerLibrary/userexporterfactory.h"
using namespace al::uicontroller;
using namespace ci::operatingenvironment;
using namespace ssdk;
namespace al
{
namespace softwarefunction
{
namespace rbacmanagerlibrary 
{
    class SessionContext
    {
	const std::string m_sessionName;
    public:
        SessionContext(const std::string & sessionName);
        //SessionContext(const std::string & sessionName,ExportType exportType);
        ci::operatingenvironment::Ref<ssdk::SSDKUserTokenInterface> GetSessionUserToken() const;
        ci::operatingenvironment::Ref<ssdk::SSDKUserTokenInterface> GetAutoProcessingUserToken() const;
        Status Authorize(std::string rbac_object, std::string rbac_operation) const;
	void GetBoProxy(BOProxyRef & boProxy);
    };
    typedef ci::operatingenvironment::Ref<SessionContext> SessionContextRef;
}
}
}
#endif //AL_SOFTWARE_FUNCTIONS_GROUPMANAGER_LIBRARY_SESSION_CONTEXT_H

