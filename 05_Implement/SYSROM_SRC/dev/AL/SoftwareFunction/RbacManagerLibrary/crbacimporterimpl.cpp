/******************************************************************************
 * @Copyright (C) 2010 Toshiba TEC Corp
 * @Workfile:  crbacimporterimpl.cpp$
 * @Revision:  1 $
 * @Date:      23-Dec-2010$
 * @Author:    ThiyaguR 
 * @Notes:
 *******************************************************************************/

#include "crbacimporterimpl.h"
#include "chelper.h"
#include "AL/UIController/boproxy.h"
#include "csessioncontext.h"
#include <algorithm>

namespace al{
	namespace softwarefunction{
		namespace rbacmanagerlibrary{
		
			RBACImporterImpl::RBACImporterImpl(SessionContextRef & sessionContext):m_sessionContext(sessionContext)
			{

			}
			RBACImporterImpl::~RBACImporterImpl()
			{

			}
			EIStatus RBACImporterImpl::Restore(Ref<DataTableExportImport> target)
			{
				Ref<SSDKUserTokenInterface> sessionUserToken = m_sessionContext->GetSessionUserToken();
				if(!sessionUserToken)
				{
					DEBUGL1("sessionUserToken is NULL\n");
					return al::softwarefunction::exportimport::EI_FAILED;
				}
				SSDKStatus ssdkStatus = OK;
				SSDKUserTokenInterface *userToken = sessionUserToken.operator->();
				if(!userToken)
				{
					DEBUGL1("User Token is NULL\n");
					return al::softwarefunction::exportimport::EI_FAILED;
				}
				//Check for user Authorization
				if(OK != (ssdkStatus = userToken->Authorize(ssdk::OBJ_USERMANAGEMENT, ssdk::OP_EXECUTE)))
				{
					DEBUGL1("User is not Authorized for this operation, status ::%d\n",ssdkStatus);
					return al::softwarefunction::exportimport::EI_PERMISSION_DENY;
				}
				//Delete All Roles from SSDK DB
				if (STATUS_OK != Helper::DeleteAllRoles(sessionUserToken))
				{
					DEBUGL1("FAILED TO DELETE ALL RBACS\n");
					return al::softwarefunction::exportimport::EI_FAILED;
				}
				if(target->size() == 0)
				{
					DEBUGL4("No Records to Import\n");
					return al::softwarefunction::exportimport::EI_SUCCESS;
				}
				DEBUGL6("TOTAL RECORDS PRESENT IN ROLE INFO VECTOR :: %d\n",target->size());
				bool partialImportFlag = false;
				Ref<DataTableExportImport> validatedDataTable = new DataTableExportImport;
				Ref<DataTableExportImport> allowBuiltInRoleDataTable = new DataTableExportImport;
				partialImportFlag = ValidateDataTable(target,validatedDataTable,allowBuiltInRoleDataTable);

				DEBUGL6("After Datavalidation partialImportFlag is :: %d\n",partialImportFlag);
				if(validatedDataTable->size() == 0 && allowBuiltInRoleDataTable->size() == 0)
				{
					DEBUGL2("Validation failed for all records\n");
					return al::softwarefunction::exportimport::EI_FAILED;
				}
				RbacPermissionImpl* rbacAdd = dynamic_cast<RbacPermissionImpl*>(RbacPermissionImpl::GetInstance());
				if(!rbacAdd)
				{
					DEBUGL1("rbacAdd is NULL\n");
					return al::softwarefunction::exportimport::EI_FAILED;
				}
				if(STATUS_OK != rbacAdd->AddRoles(sessionUserToken,validatedDataTable,partialImportFlag))
				{
					DEBUGL1("Failed to Add RBACs\n");
					// API level failure then delete all the rbacs
					if (STATUS_OK != Helper::DeleteAllRoles(sessionUserToken))
						DEBUGL1("FAILED TO DELETE ALL RBACS\n");
					else
						DEBUGL8("Deleted All RBACs\n");
					return al::softwarefunction::exportimport::EI_FAILED;
				}
				DEBUGL6("After AddRoles partialImportFlag is :: %d\n",partialImportFlag);
				bool partialImportFlagForUpdateRole = false;
				if(allowBuiltInRoleDataTable->size()>0)
				{
					if(STATUS_OK != rbacAdd->UpdateRoles(sessionUserToken,allowBuiltInRoleDataTable,partialImportFlagForUpdateRole))                               		     {
						DEBUGL1("Failed to Update Roles\n");
					}

				}
				DEBUGL6("After UpdateRoles partialImportFlagForUpdateRole is :: %d\n",partialImportFlagForUpdateRole);
				if(File::Exists(currentRoleSetting))
				{
					BOProxyRef boProxy;
					m_sessionContext->GetBoProxy(boProxy);
					CString commandString,response;
					commandString = "<Command><ImportMappingXML><commandNode>RBACManager/Objects</commandNode><Params>";
					commandString += "<filePath>" + importFilePath  + "</filePath>";
					commandString += "<fileName>" + importFileName + "</fileName>";
					commandString += "</Params></ImportMappingXML></Command>";
					Status ret = boProxy->ExecuteCommand(commandString.c_str(),response);
					if(STATUS_OK != ret)
						DEBUGL1("Failed in BoProxy::ExecuteCommand\n");
				}
				if(partialImportFlag || partialImportFlagForUpdateRole)
					return al::softwarefunction::exportimport::EI_PART_SUCCESS;

				return al::softwarefunction::exportimport::EI_SUCCESS;

			}

			EIStatus RBACImporterImpl::Import(Ref<DataTableExportImport> target)
			{
				return al::softwarefunction::exportimport::EI_SUCCESS;
			}

			bool RBACImporterImpl:: ValidateDataTable(Ref<DataTableExportImport> target,Ref<DataTableExportImport> validatedDataTable,Ref<DataTableExportImport> allowBuiltInRoleDataTable)
			{
				DataTableExportImport::iterator iter;
				bool partialImportFlag = false;
				map<CString,al::softwarefunction::exportimport::UoDataRow> updatedDataTableMap;
				map<CString,al::softwarefunction::exportimport::UoDataRow> allowBuiltInRoleDataTableMap;
				DataTableExportImport emptyRoleIdDataTable;
				unsigned int cnt = 1;
				//unsigned int iTotalNonBuiltInRoles =0;
				//Helper::PopulateBuiltInRoles(m_sessionContext->GetSessionUserToken(),totalBuiltInRole);
				unsigned int max_role_for_import = static_cast<unsigned int>(MAX_ROLE_COUNT);
				DEBUGL6("MAXIMUM RECORD CAN BE PARSED FROM DATA TABLE :: %d\n",max_role_for_import);
				bool isGuestInRolePresent = false;
				//To check if Exported from Old ROM which does not support EWBAccess and USB Direct Print
				bool isUpdateFromNewXml = false;
				for(iter = target->begin();iter!=target->end();++iter)
				{
					CString roleUniqueId = (*iter)[ROLE_ID_MAP_KEY];
                                        CString roleName = (*iter)[ROLE_NAME_MAP_KEY];
                                        if(roleUniqueId == "0")
                                        {
                                                CString tempRoleName = roleName;
                                                if(tempRoleName.find("DatabaseVersion_") != string::npos)
                                                {
							isUpdateFromNewXml =true;
                        	                        break;
                                                }
                                                else
                                                {
                                                        DEBUGL1("Role Name Unknown When Role ID=0\n");
							return false;
                                                }
					}	
				}
				
				if(isUpdateFromNewXml)
				target->erase(iter);  
				
				for(iter = target->begin();iter!=target->end();++iter)
				{
					//validate role unique id
					CString roleUniqueId = (*iter)[ROLE_ID_MAP_KEY];
                                        CString roleName = (*iter)[ROLE_NAME_MAP_KEY];
					//Counting records
					if(!isUpdateFromNewXml)
					{
						if(!((*iter)[ROLE_PERM_MAP_KEY].empty()))
							(*iter)[ROLE_PERM_MAP_KEY] += ",";
                                                (*iter)[ROLE_PERM_MAP_KEY] +="EWBAccess";
						if((*iter)[ROLE_PERM_MAP_KEY].find("PrintJob") != string::npos)
                                                 (*iter)[ROLE_PERM_MAP_KEY] +=",USBDirectPrint";
                                        }
					//CHeck for device remote maintenance permission
					CString tempPerm = (*iter)[ROLE_PERM_MAP_KEY];
					std::transform(tempPerm.begin(),tempPerm.end(),tempPerm.begin(),(int(*)(int))tolower);
					if(tempPerm.find("deviceremotemaintenance") != string::npos)
					{
						DEBUGL3("Invalid device permission present");
						partialImportFlag = true;
						continue;
					}
					//Counting Records
					//cnt++;
					DEBUGL8("Role Unique ID in DataTable :: %s\n",roleUniqueId.c_str());
					if(roleUniqueId == GUEST_ROLE_UNIQUE_ID)
					{
						CString tempRoleName = roleName;
						std::transform(tempRoleName.begin(),tempRoleName.end(),tempRoleName.begin(), (int(*)(int)) tolower);
						if(tempRoleName== GUEST_ROLE_NAME)
						{
							if(!isGuestInRolePresent)
							{
								max_role_for_import += 1;
								DEBUGL6("NEW MAXIMUM RECORD CAN BE PARSED FROM DATA TABLE :: %d\n",max_role_for_import);
								isGuestInRolePresent = true; 
							}
						}
						else
						{
							partialImportFlag = true;
							DEBUGL6("partialImportFlag is true as for role unique ID Guest role name is not present\n");
							continue;
						}
					}
						
					DEBUGL6("RECORDS INDEX COUNT :: %d\n",cnt);
					DEBUGL6("MAX RECORDS POSSIBLE FOR IMPORT :: %d\n",max_role_for_import);		
					if(cnt > max_role_for_import)
                                        {
                                                partialImportFlag = true;
						DEBUGL6("partialImportFlag is true as role count reached maximum limits\n");
                                                break;
                                        }

					int validateUniqueId = ValidateRoleUniqueId(roleUniqueId);
					if(validateUniqueId == -1)
					{
						partialImportFlag = true;
						DEBUGL6("partialImportFlag is true as role unique id failed againts rule\n");
						continue;
					}
					else if(validateUniqueId == 2)
					{
						partialImportFlag = true;
						DEBUGL6("partialImportFlag is true as role unique id is duplicate\n");
					}
					//CString roleName = (*iter)[ROLE_NAME_MAP_KEY];
					//validate role name
					bool isAllowBuiltInRole = false;
					if(!ValidateRoleName(roleName,isAllowBuiltInRole))
					{
						partialImportFlag = true;
						DEBUGL6("partialImportFlag is true as role unique name failed againts rule\n");
                                                continue;
					}
					if(isAllowBuiltInRole && (roleUniqueId !=GUEST_ROLE_UNIQUE_ID))
					{
						DEBUGL6("Guest Role does not have Role Unique ID :: %s\n",roleUniqueId.c_str());
						partialImportFlag = true;
						isAllowBuiltInRole = false;
						DEBUGL6("partialImportFlag is true as allowed built-in role does not have proper role unique id in role info vector\n");
						continue;
					}
					if(isAllowBuiltInRole)
						allowBuiltInRoleDataTableMap[roleUniqueId] = *iter;
					else if(roleUniqueId.empty())
						emptyRoleIdDataTable.push_back(*iter);
					else
						updatedDataTableMap[roleUniqueId] = *iter;
					 //Counting Records
                        cnt++;
				}	
				map<CString,al::softwarefunction::exportimport::UoDataRow>::iterator updatedIt;
				DEBUGL6("Updated map size:%d\n",updatedDataTableMap.size());
				DEBUGL6("Empty id size:%d\n",emptyRoleIdDataTable.size());
				DEBUGL6("AllowBuiltInRole Data Table Size:: %d\n",allowBuiltInRoleDataTableMap.size());
				for(updatedIt = updatedDataTableMap.begin(); updatedIt != updatedDataTableMap.end(); updatedIt++)
					validatedDataTable->push_back(updatedIt->second);
				DEBUGL5("Before adding empty role id records validated data table vector size:%d\n",validatedDataTable->size());
				validatedDataTable->insert(validatedDataTable->end(),emptyRoleIdDataTable.begin(),emptyRoleIdDataTable.end());
				DEBUGL5("after adding empty role id records validated data table vector size:%d\n",validatedDataTable->size());

				DEBUGL6("Before adding allow built in role records allowBuiltInRoleDataTable vector size ::%d\n",allowBuiltInRoleDataTable->size());
				for(updatedIt = allowBuiltInRoleDataTableMap.begin(); updatedIt != allowBuiltInRoleDataTableMap.end(); updatedIt++)
					allowBuiltInRoleDataTable->push_back(updatedIt->second);
				DEBUGL6("Before adding allow built in role records allowBuiltInRoleDataTable vector size ::%d\n",allowBuiltInRoleDataTable->size());		
					
				return partialImportFlag;
			}
			bool RBACImporterImpl::ValidateRoleName(const CString& roleName,bool & isAllowBuiltInRole)
			{
				isAllowBuiltInRole = false;
				// Check for rbac name validation
				if(!Helper::CheckForNameValidity(roleName))
				{
					DEBUGL4("RBAC Name ::%s has prohibitted characters\n",roleName.c_str());
					return false;
				}
				CString tempName(roleName);
                                std::transform(tempName.begin(),tempName.end(),tempName.begin(), (int(*)(int)) tolower);
				//Check for dupplicate rbac name
				pair<set<CString>::iterator,bool> ret;
				ret = m_roleNameSet.insert(tempName);
				if(ret.second == false)
				{
					DEBUGL4("Duplicate Role Name :: %s\n",roleName.c_str());
					return false;
				}
				//Check for built in role
				bool isBuiltRole =Helper::IsBuiltInRole(tempName,isAllowBuiltInRole);
				if(isBuiltRole)
				{
					if(isAllowBuiltInRole)
						return true;
					return false;
				}
				return true;	
			}
			/* ValidateRoleUniqueId
 			 * Validates the unique id of the given role
 			 * param[in] const CString& roleUniqueIdStr
 			 * return int 
 			 * 	-1 - Validation failed
 			 * 	0  - Success
 			 * 	1  - Empty id
 			 * 	2  - Duplicate id
 			 */
			int RBACImporterImpl::ValidateRoleUniqueId(const CString & roleUniqueIdStr)
			{
				istringstream tempRoleId;
				tempRoleId.str(roleUniqueIdStr);
				int roleUniqueId = -1;
				if(roleUniqueIdStr.empty())
				{
					DEBUGL3("Role Unique ID is Empty :: %s\n",roleUniqueIdStr.c_str());
					return 1;
				}	
				tempRoleId>>roleUniqueId;
				if(string::npos != roleUniqueIdStr.find("."))
				{
					DEBUGL1("Not a valid integer\n");
					return -1;
				}
				if((roleUniqueId <= MIN_ROLE_ID_VAL) || ((roleUniqueId > MAX_ROLE_ID_VAL) && (roleUniqueId != GUEST_ROLE_UNIQUE_ID_INT)))
				{
					DEBUGL4("RBAC UNIQUE ID::%d does not fall in range %d and %d\n",roleUniqueId,MIN_ROLE_ID_VAL,MAX_ROLE_ID_VAL);
					return -1;
				}
				//Check for dupplicate role unique ID
				pair<set<int>::iterator,bool> ret;
				ret = m_roleUniqueIdSet.insert(roleUniqueId);
				if(ret.second == false)
				{
					DEBUGL4("Duplicate RBAC Unique ID :: %d\n",roleUniqueId);
					return 2;
				}
				return 0;			
			}
		};
	};
}; 
