/******************************************************************************
 * @Copyright (C) 2010 Toshiba TEC Corp
 * @Workfile:  rbaccommonresource.h$
 * @Revision:  1 $
 * @Date:      24-June-2010$
 * @Author:    Manoj Kumar Saha
 * @Notes:
 *******************************************************************************/

#ifndef RBACMANAGER_LIBRARY_COMMON_RESOURCE_H
#define RBACMANAGER_LIBRARY_COMMON_RESOURCE_H
#include <map>
#include <vector>
#include <string>
#include <set>
#include <AL/status.h>
#include "CI/OperatingEnvironment/cstring.h"
#include "CI/SI/rbac_objects.h"

using namespace std;
using namespace ci::operatingenvironment;

namespace al{
namespace softwarefunction{
namespace rbacmanagerlibrary{

typedef map<CString,set<CString> > stringMap2Set;
typedef std::set<std::pair<std::string,std::string> > SetOfPairOfString;
typedef set<CString> SetString;
//All Ebx defined permission Name
const CString OPR_COPY_JOB("CopyJob");
const CString OPR_PRINT_JOB("PrintJob");
const CString OPR_PRINT_MGNT("PrintManagement");
const CString OPR_REMOTESCAN("RemoteScan");
const CString OPR_INTERNET_FAX_TRANS("InternetFaxTransmission");
const CString OPR_FAX_TRANS("FaxTransmission");
const CString OPR_FAX_RECVD_PRINT("FaxReceivedPrint");
const CString OPR_COLOR_PRINT("ColorPrint");
const CString OPR_VARIOUS_PRINT_SETTING("VariousPrintSetting");
const CString OPR_LOCAL_STORAGE("StoreToLocalStorage");
const CString OPR_USB_STORAGE("StoreToUSBDevice");
const CString OPR_SEND_EMAIL("SendEmail");
const CString OPR_REMOTE_STORE("StoreToRemoteServer");
const CString OPR_DISTRIBUTED_SCAN("DistrubutedScan");
const CString OPR_WSSCAN("WSScanPush");
const CString OPR_EFILING_ACCESS("eFilingAccess");
const CString OPR_DEVICE_SETTING("DeviceSetting");
const CString OPR_USER_DEPT_MGNT("UserDepartmentManagement");
const CString OPR_JOB_OPERATION("JobOperation");
const CString OPR_READ("Read");
const CString OPR_LOG_READ("LogRead");
const CString OPR_EDIT("Edit");
const CString OPR_LOG_EXPORT("LogExport");
const CString OPR_CLONE("Cloning");
const CString OPR_ADDRESSBOOK_READ_EDIT("AddressBookReadEdit");
const CString OPR_JOBTEMPLATE_READ_EDIT("JobTemplateReadEdit");
const CString OPR_ACCESS_DIAGONOSTICMODE("AccessDiagonosticMode");
const CString OPR_EWB_ACCESS("EWBAccess");
const CString OPR_USBDIRECTPRINT("USBDirectPrint");
const CString OPR_DEVICE_REMOTE_MAINTENANCE("DeviceRemoteMaintenance");
const CString ROLE_ID_MAP_KEY("roleID");
const int MIN_ROLE_ID_VAL = 0;
const int MAX_ROLE_ID_VAL = 100;
const int MAX_ROLE_COUNT = 30;
const int GUEST_ROLE_UNIQUE_ID_INT = 114;
const int ALLOW_BUILT_IN_ROLES = 1;
const CString ROLE_NAME_MAP_KEY("roleName");
const CString IS_ADMIN_ROLE("IsAdminRole");
const CString ROLE_PERM_MAP_KEY("permission");
const CString PROHIBITED_CHARS_FOR_NAME("\"'()*+,/:;<=>?[\\]`{|}~");

const CString GUEST_ROLE_UNIQUE_ID("114");
const CString GUEST_ROLE_NAME("guest");
const CString currentRoleSetting("/work/al/bin/ImportedRoleMapping.xml");
const CString importFilePath("/work/al/bin/");
const CString importFileName("ImportedRoleMapping.xml");
};
};
};
#endif
