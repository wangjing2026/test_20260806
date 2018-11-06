// (c) 2010 TOSHIBA TEC CORPORATION All Rights Reserved.

#ifndef __EBXSSDKUTIL_H_
#define __EBXSSDKUTIL_H_

#include "CI/SI/ssdkusertokeninterface.h"
#include "CI/SI/ssdkcodes.h"
#include "CI/OperatingEnvironment/cstring.h"
#include "CI/SI/ssdkrbacmgmtinterface.h"
#include "CI/OperatingEnvironment/ref.h"
#include "CI/SI/ssdksecuritymanagerinterface.h"
#include "CI/SI/ssdkencryptioninterface.h"
#include "CI/SI/ssdkcertificateinterface.h"
#include "CI/SI/ssdkprogressindicatorinterface.h"
#include "CI/SI/ssdkconfiginterface.h"
#include "CI/SI/rbac_objects.h"
#include <CI/SoftwareDiagnostics/softwarediagnostics.h>
#include "CI/SI/rbac_operations.h"
#include "CI/OperatingEnvironment/file.h"
#include "CI/HierarchicalDB/hierarchicaldb.h"
#include <vector>
#include <sys/stat.h>

class EbxSSDKUtil {
    static void  ExtractToken(ci::operatingenvironment::CString allToken,const char* del,std::vector<ci::operatingenvironment::CString>& objVector)
    {
        if(allToken.size()==0)
            return ;

        char *myAllToken = new char[allToken.size() +1];
        memset(myAllToken,'\0',allToken.size() +1);
        strncpy(myAllToken,allToken.c_str(),allToken.size());
        char *singleToken = NULL;
        singleToken = strtok(myAllToken,del);
        if(singleToken != NULL)
            objVector.push_back(ci::operatingenvironment::CString(singleToken));
		else
			objVector.push_back(ci::operatingenvironment::CString(myAllToken));
        while(singleToken != NULL)
        {
            singleToken = strtok(NULL,del);
            if(singleToken != NULL)
                objVector.push_back(ci::operatingenvironment::CString(singleToken));
        }

        if(myAllToken != NULL)
            delete [] myAllToken;
    }

public:

    /**
    * This function is an eBX integration wrapper for SSDK method:
    * SSDKRBACMgmtInterface::Authorize(). This method assumes that the
    * objectId string is in the following format: "Part1:...:PartN:UUID".
    * By default this method uses a combination of Role and ACL based
    * authorization. If caller calls  EbxSSDKUtil::Authorize()
    * API with authType=SSDKUserTokenInterface::AUTH_ACL_ROLE, the method
    * first tries to Authorize objectId using  ACL. If the SSDK authorization
    * retuns status SSDKStatus::OK or SSDKStatus::AUTH_AUTHORIZATION_DENIED
    * then the method  EbxSSDKUtil::Authorize() returns  SSDKStatus::OK or
    * SSDKStatus::AUTH_AUTHORIZATION_DENIED.If the SSDK authorization returns
    * SSDKStatus::AUTH_BAD_AUTHORIZATION then the EbxSSDKUtil::Authorize() API
    * tries to use role based authorization for each of the parts: Part1,...,
    * PartN in the objectId string. If ssdk::Authorize() returns OK or
    * AUTH_AUTHORIZATION_DENIED on any part then EbxSSDKUtil::Authorize() will
    * return the same. If ssdk::Authorize() returns AUTH_BAD_AUTHORIZATION
    * on any part then EbxSSDKUtil::Authorize() tries to do authorization
    * on the next part present in object id.You can also perform just role based
    * authorization or ACL based authorization by specifying AUTH_ROLE or AUTH_ACL
    * as the authType parameter.
    *
    * @param[in]    obj           object id
    * @param[in]    operation     integer, defined by the ssdk client
    * @param[in]    authType      Authorization type. Default is to authorize
    *                             using both ACL and role.
    * @return       SSDKStatus    OK, or error codes with prefix of AUTH_
    *                             from ssdkcodes.h
    */
    static ssdk::SSDKStatus Authorize(const ssdk::SSDKUserTokenInterface* userToken,
        const std::string& objectId,
        ci::operatingenvironment::CString operationId,
        ssdk::SSDKUserTokenInterface::AuthorizeType authType = ssdk::SSDKUserTokenInterface::AUTH_ACL_ROLE)
    {
    	if(!userToken)
			return ssdk::AUTH_TOKEN_NOT_VALID;
        ssdk::SSDKStatus status = ssdk::AUTH_NOTFOUND;
        ssdk::SSDKUserTokenInterface* userTokenTemp = const_cast<ssdk::SSDKUserTokenInterface*> (userToken);

        if(objectId.empty())
            return ssdk::AUTH_NOTFOUND;

        //Check Authorize() for AUTH_ACL
        if (authType == ssdk::SSDKUserTokenInterface::AUTH_ACL || authType == ssdk::SSDKUserTokenInterface::AUTH_ROLE)
            status =   userTokenTemp->Authorize(objectId,operationId,authType);
        else if(authType == ssdk::SSDKUserTokenInterface::AUTH_ACL_ROLE)
        {
            //Tokenize the objectid on ":"

            std::vector<ci::operatingenvironment::CString> objVec;
            ExtractToken(objectId,":",objVec);
            unsigned int vecSize= objVec.size();
            if(vecSize >0)
            {
                unsigned int vecSize= objVec.size();

                status = userTokenTemp->Authorize(objectId,operationId,ssdk::SSDKUserTokenInterface::AUTH_ACL);
            		if (status == ssdk::OK || status == ssdk::AUTH_AUTHORIZATION_DENIED)
            		{
                		return status;
			   		}
						for(unsigned int _cnt=0;_cnt<vecSize;_cnt++)
                		{
			           			status = userTokenTemp->Authorize(objVec[_cnt],operationId,ssdk::SSDKUserTokenInterface::AUTH_ROLE);
								if ((status == ssdk::OK) || (status = ssdk::AUTH_AUTHORIZATION_DENIED))
                        			break;
								else if(status == ssdk::AUTH_BAD_AUTHORIZATION)
									continue;
                		}

        	}
	}
	return status;
    }
#if 0
    /**
    * This function is an eBX integration wrapper for SSDK method:
    * SSDKRBACMgmtInterface::SetProperty(OBJECT_ACL_PROPERTY). The
    * method performs proper authorization for the operation and then
    * calls the SSDKRBACMgmtInterface::SetProperty(OBJECT_ACL_PROPERTY)
    * method. If the ACL for the objectId does not exist this function
    * authorizes the userToken for "OP_CREATE_ACL" operation on the objectId.
    * If the ACL of the object exists this function authorizes the
    * userToken for "ModifyACL" operation on the objectId.
    *
    * @param[in]  userToken         user token for role-based access
    *                               authorization of this operation
    * @param[in]  propName          Name of the property, in this case
    *                               OBJECT_ACL_PROPERTY.
    * @param[in]  objectId          id of the object
    * @param[in]  objectPermissions Map of user and operations. The ACL
    is removed if the map is empty.
    * @return     status            OK (success),or error code with prefix
    *                               AUTH_XXX(see ssdkerrorcodes.h)
    */
    static ssdk::SSDKStatus SetProperty(const ssdk::SSDKUserTokenInterface* userToken,
        ssdk::SSDKRBACMgmtInterface::ObjectPropertyName propName,
        const std::string& objectId,
        const std::map<std::string, std::vector <ssdk::SSDKRBACMgmtInterface::OperationInfo> >& userOperations)
    {
    	if(!userToken)
			return ssdk::AUTH_TOKEN_NOT_VALID;
        ssdk::SSDKStatus status;
        ci::operatingenvironment::Ref<ssdk::SSDKSecurityManagerInterface> ssdkSecurityManagerInterface =ssdk::GetSecurityManager();
        ci::operatingenvironment::Ref<ssdk::SSDKRBACMgmtInterface> rbac = dynamic_cast<ssdk::SSDKRBACMgmtInterface*> (ssdkSecurityManagerInterface->GetInterface  (ssdk::SSDKSecurityManagerInterface	     ::RBAC_MGMT_INTERFACE,status));
        if(!rbac || status!=ssdk::OK)
        {
            DEBUGL8("Failed to get rbac interface\n");
            return status;
        }

        std::map<std::string,std::vector<ssdk::SSDKRBACMgmtInterface::OperationInfo> > ObjectPermissionsRead;
        ObjectPermissionsRead.clear();
        status=rbac->GetProperty(userToken,ssdk::SSDKRBACMgmtInterface::OBJECT_ACL_PROPERTY,objectId,ObjectPermissionsRead);
        if(ssdk::OK!= status)
        {
            status=Authorize(userToken,objectId,ssdk::OP_CREATE_ACL);
        }
        else
        {
            status=Authorize(userToken,objectId,ssdk::OP_MODIFY_ACL);
        }
        if(ssdk::OK != status)
            return status;

        status=rbac->SetProperty(userToken,propName,objectId,userOperations);
        return status;

    }

	 /**
	  * This function is an eBX integration wrapper for SSDK method:
	  * SSDKRBACMgmtInterface::GetProperty(OBJECT_ACL_PROPERTY).
	  *
	  * @param[in]	userToken		  user token for role-based access
	  * 							  authorization of this operation
	  * @param[in]	propName		  Name of the property, in this case
	  * 							  OBJECT_ACL_PROPERTY.
	  * @param[in]	objectId		  id of the object
	  * @param[in]	objectPermissions Map of user and operations. The ACL
						   is removed if the map is empty.
	  * @return 	status			  OK (success),or error code with prefix
	  * 							  AUTH_XXX(see ssdkerrorcodes.h)
	  */

    static ssdk::SSDKStatus GetProperty(const ssdk::SSDKUserTokenInterface* userToken,
        ssdk::SSDKRBACMgmtInterface::ObjectPropertyName propName,
        const std::string& objectId,
        std::map<std::string, std::vector <ssdk::SSDKRBACMgmtInterface::OperationInfo> >& objectPermissions)
    {
 		if(!userToken)
			return ssdk::AUTH_TOKEN_NOT_VALID;
        ssdk::SSDKStatus status;
        ci::operatingenvironment::Ref<ssdk::SSDKSecurityManagerInterface> ssdkSecurityManagerInterface =ssdk::GetSecurityManager();
        ci::operatingenvironment::Ref<ssdk::SSDKRBACMgmtInterface> rbac = dynamic_cast<ssdk::SSDKRBACMgmtInterface*>(ssdkSecurityManagerInterface->GetInterface(ssdk::SSDKSecurityManagerInterface::RBAC_MGMT_INTERFACE,status));
        if(!rbac || status!=ssdk::OK)
        {
            DEBUGL8("Failed to set the  rbac interface\n");
            return status;
        }
        status=rbac->GetProperty(userToken,ssdk::SSDKRBACMgmtInterface::OBJECT_ACL_PROPERTY,objectId,objectPermissions);
        return status;
    }
#endif
/*	This function will resolve the XML entity reference for a string.
 *	If the input string has any one of these XML entity references "&","<",">","`","\"",
 *	then this function will replace that character with the corresponding string like "&amp;",
 *	"&lt;","&gt;","&apos;", "&quot;"
 */
static void CheckXMLEntityAndReplaceString(ci::operatingenvironment::CString  & actualStr)
{
	if(actualStr.empty())
		return ;
	ci::operatingenvironment::CString tempStr("");
	Status retStatus = ci::hierarchicaldb::chelper::Escape(actualStr, tempStr);
	DEBUGL8("Escape() API return status:%d\n", retStatus);
	DEBUGL8("Input String::%s\n", actualStr.c_str());
	actualStr = tempStr;
	DEBUGL8("Output String::%s\n", actualStr.c_str());
	return ;
#if 0
        std::vector<ci::operatingenvironment::CString> findStrVec;
        findStrVec.push_back("&");
        findStrVec.push_back("<");
        findStrVec.push_back(">");
	findStrVec.push_back("`");
        findStrVec.push_back("\"");

        std::vector<ci::operatingenvironment::CString> replaceVec;
        replaceVec.push_back("&amp;");
        replaceVec.push_back("&lt;");
        replaceVec.push_back("&gt;");
	replaceVec.push_back("&apos;");
        replaceVec.push_back("&quot;");
        int fndLen = 1;

        for(unsigned int _cnt=0;_cnt<findStrVec.size();_cnt++)
        {
                int startPos = 0;
                int endPos   = 0;
                while(endPos >= 0)
                {
                        endPos = actualStr.find(findStrVec[_cnt],startPos);
                        if( endPos >= 0)
                        {
                                ci::operatingenvironment::CString replaceStr(replaceVec[_cnt]);
                                actualStr.replace(endPos,fndLen,replaceStr,0,replaceStr.length());
                        }
                        startPos = endPos + fndLen;
                }
        }
        return ;
#endif
}
	/**
	* Get all or specified per level configuration settings from SSDK Configuration Manager.  If mapPropertyValue is empty, then all
	* per level settings will be returned.
	*
	* @param[in]      userToken         user token for role-based access
	*                                   authorization of this operation
	* @param[in]      securityLevel     the level of security for which the settings are requested.
	*					   				 If level is set to 4(custom) and if the current level is not custom, an error code is returned

	* @param[in/out]  mapPropertyValue  Map of configuration properties and value.  If the
	*                                   configuration properties requested are specified
	*                                   in the map by the caller, then only those will be provided.
	*					    			Otherwise all per level settings are included. The method fills in the
	*                                   values of configuration property using SSDK Configuration
	*                                   manager component.
	* @return     status                OK on success, error code on error
	*/
	static ssdk::SSDKStatus GetSecuritySettings(const ssdk::SSDKUserTokenInterface* userToken,
					   ssdk::SSDKConfigInterface::MapPropertyEnumName securityLevel,
                       std::map<ssdk::SSDKConfigInterface::StringPropertyName,std::string>& mapPropertyValue)
	{
		ssdk::SSDKStatus status = ssdk::OK;
		std::string getConfigLevel;
		if(!userToken)
			return ssdk::AUTH_TOKEN_NOT_VALID;

		//Get Config Interface Object
		ci::operatingenvironment::Ref<ssdk::SSDKSecurityManagerInterface> ssdkSecurityManagerInterface = ssdk::GetSecurityManager();
		ci::operatingenvironment::Ref<ssdk::SSDKConfigInterface>configPtr = dynamic_cast<ssdk::SSDKConfigInterface*> (ssdkSecurityManagerInterface->GetInterface(ssdk::SSDKSecurityManagerInterface::CONFIG_INTERFACE, status));
		if((status !=ssdk::OK)||(configPtr == (void*)NULL))
		{
			DEBUGL1("Creation of config Object Failed\n");
			return status;
		}

		/*ssdk::SSDKConfigInterface::StringPropertyName configAttributeName;
                configAttributeName=SSDKConfigInterface::SA_SERVSECURITYLEVEL;
  		//Getting security level from config
  		getConfigLevel=configPtr->GetProperty(userToken,configAttributeName,status);
                if(ssdk::OK!=status)
                {
                	DEBUGL1("GET VALUE FROM CONFIG FAILED %d \n",status);
                	return status;
                }
                if(getConfigLevel==securityLevel)
		{
		}*/
                                                                                                                                                             		     if(mapPropertyValue.empty())
		{
			status = configPtr->GetProperty(userToken, securityLevel, mapPropertyValue);
			if(status !=STATUS_OK)
			{
				DEBUGL1("Get Property failed %d\n", status);
				return status;
			}
		}
		else
		{
                	std::map<ssdk::SSDKConfigInterface::StringPropertyName,std::string>::iterator iter;
			for( iter = mapPropertyValue.begin(); iter != mapPropertyValue.end(); ++iter )
			{
                		getConfigLevel=configPtr->GetProperty(userToken,iter->first,status);
                        	if(ssdk::OK!=status)
                        	{
                        		DEBUGL1("GET VALUE FROM CONFIG FAILED %d \n",status);
                                	return status;
                        	}
				iter->second=getConfigLevel;

			}
		}


		return status;
	}
	/**
	 * Get multiple configuration properties from SSDK Configuration Manager.
	 *
	 * @param[in]      userToken         user token for role-based access
	 *                                   authorization of this operation
	 * @param[in/out]  mapPropertyValue  Map of configuration properties and value. The
	 *                                   configuration properties requested are specified
	 *                                   in the map by the caller. The method fills in the
	 *                                   values of configuration property using SSDK Configuration
	 *                                   manager component.
	 * @return     status                OK on success, error code on error
	 */
	static ssdk::SSDKStatus GetSecuritySettings(const ssdk::SSDKUserTokenInterface* userToken,
									std::map<ssdk::SSDKConfigInterface::StringPropertyName,std::string>& mapPropertyValue)
	{
		ssdk::SSDKStatus status =ssdk::OK;
                std::string getConfigLevel;
                if(!userToken)
                        return ssdk::AUTH_TOKEN_NOT_VALID;

                //Get Config Interface Object
                ci::operatingenvironment::Ref<ssdk::SSDKSecurityManagerInterface> ssdkSecurityManagerInterface = ssdk::GetSecurityManager();
                ci::operatingenvironment::Ref<ssdk::SSDKConfigInterface> configPtr = dynamic_cast<ssdk::SSDKConfigInterface*> (ssdkSecurityManagerInterface->GetInterface(ssdk::SSDKSecurityManagerInterface::CONFIG_INTERFACE, status));
                if((status !=ssdk::OK)||(configPtr == (void*)NULL))
                {
                	DEBUGL1("Creation of config Object Failed\n");
                        return status;
                }

		ssdk::SSDKConfigInterface::StringPropertyName configAttributeName;
                configAttributeName=ssdk::SSDKConfigInterface::SA_SERVSECURITYLEVEL;
                //Getting security level from config
                getConfigLevel=configPtr->GetProperty(userToken,configAttributeName,status);
                if(ssdk::OK!=status)
                {
                	DEBUGL1("GET VALUE FROM CONFIG FAILED %d \n",status);
                        return status;
                }

		if("4"==getConfigLevel)
			return ssdk::INVALID_ARGUMENT;
		//if the map passed is empty
		if(mapPropertyValue.empty())
                {
                    	status = configPtr->GetProperty(userToken,(ssdk::SSDKConfigInterface::MapPropertyEnumName)atoi(getConfigLevel.c_str()),mapPropertyValue);
                        if(status !=STATUS_OK)
                        {
                                DEBUGL1("Get Property failed %d\n", status);
                                return status;
                        }
                }
                else
                {
                        std::map<ssdk::SSDKConfigInterface::StringPropertyName,std::string>::iterator iter;
                        for( iter = mapPropertyValue.begin(); iter != mapPropertyValue.end(); ++iter )
                        {
                                getConfigLevel=configPtr->GetProperty(userToken,iter->first,status);
                                if(ssdk::OK!=status)
                                {
                                        DEBUGL1("GET VALUE FROM CONFIG FAILED %d \n",status);
                                        return status;
                                }
                                iter->second=getConfigLevel;

                        }
                }

		return status;
	}
	/**
	 * Set multiple configuration properties in SSDK Configuration Manager.
	 *
	 *
	 * @param[in]      userToken         user token for role-based access
	 *                                   authorization of this operation
	 * @param[in]      mapPropertyValue  Map of configuration properties and value. The
	 *                                   configuration properties and values that need to
	 *                                   updated are specified in the map by the caller. The
	 *                                   method sets the values of the properties in SSDK
	 *                                   Configuration Manager.
	 * @return     status                OK on success, error code on error
	 */
     static ssdk::SSDKStatus SetSecuritySettings(const ssdk::SSDKUserTokenInterface* userToken,
                                   std::map<ssdk::SSDKConfigInterface::StringPropertyName,std::string>& mapPropertyValue)
	{
		ssdk::SSDKStatus status =ssdk::OK;
		std::string getConfigLevel;
                if(!userToken)
                        return ssdk::AUTH_TOKEN_NOT_VALID;

                //Get Config Interface Object
                 ci::operatingenvironment::Ref<ssdk::SSDKSecurityManagerInterface> ssdkSecurityManagerInterface = ssdk::GetSecurityManager();
                 ci::operatingenvironment::Ref<ssdk::SSDKConfigInterface> configPtr = dynamic_cast<ssdk::SSDKConfigInterface*> (ssdkSecurityManagerInterface->GetInterface(ssdk::SSDKSecurityManagerInterface::CONFIG_INTERFACE, status));
                if((status !=ssdk::OK)||(configPtr == (void*)NULL))
                {
                        DEBUGL1("Creation of config Object Failed\n");
                        return status;
                }

                if(mapPropertyValue.empty())
			return ssdk::INVALID_ARGUMENT;
		std::map<ssdk::SSDKConfigInterface::StringPropertyName,std::string>::iterator iter;
                for( iter = mapPropertyValue.begin(); iter != mapPropertyValue.end(); ++iter )
		{
	        	status = configPtr->SetProperty(userToken,iter->first,iter->second);
                        if(ssdk::OK != status)
                        {
                        	DEBUGL1("GET VALUE FROM CONFIG FAILED. Status : %d \n",status);
                                return status;
                        }

		}
		return status;
	}
	/**
	 * Write Data To File
	 * @param[in]      file         file to be written
	 * @param[in]      data  	string  to be written in file
	 * @return           int          o on success, 1 on failure
	 */
	static int WriteDataToFile(const std::string &file, std::string &data)
	{

		FILE *filePtr;
		filePtr = fopen(file.c_str(), "w+");
		if(filePtr==0)
		{
			DEBUGL1("Failed to open file  :: %s \n",file.c_str());
			perror(" ");
			return 1;
		}
		else
		{
			int ret1=fseek(filePtr, 0, SEEK_SET);
			if(ret1 != 0)
				{
					DEBUGL1("fseek failed for file : %s \n",file.c_str());
					perror(" ");
					fclose(filePtr);
					return 1;
				}
			fwrite(data.c_str(), sizeof(char), data.size(), filePtr);
			fclose(filePtr);
		}

		return 0;
	}
	/**
	 * Get Data from a  File
	 * @param[in]      file         File to be read
	 * @param[in]      filedata   Output of the file in string format
	 * @return           int         o on success, 1 on failure
	 */
	static int  GetFileData(const std::string &file, std::string & filedata)
	{

		FILE *filePtr;
		filePtr = fopen(file.c_str(), "r");
		if (filePtr==0)
		{
			DEBUGL1("Failed to open file  : %s \n",file.c_str());
			perror(" ");
			return 1;
		}
		else
		{
			int ret= fseek(filePtr, 0, SEEK_END);
			if (ret != 0)
			{
				DEBUGL1("fseek failed for file : %s \n",file.c_str());
				perror(" ");
				fclose(filePtr);
				return 1;
			}
			long noofbytes = ftell(filePtr);
			if(noofbytes <= 0)
			{
				DEBUGL1("ftell failed for file : %s \n",file.c_str());				
				fclose(filePtr);
				return 1;
			}
			char *buf = new char[noofbytes];
			if(buf==NULL)
			{
				DEBUGL1("Unable to allocate memory \n");
				fclose(filePtr);
				if(buf)delete [] buf;
				return 1;
			}
			memset(buf, '\0', sizeof(buf));
			int ret1 = fseek(filePtr, 0, SEEK_SET);
			if(ret1 != 0)
			{
				DEBUGL1("fseek failed for file : %s \n",file.c_str());
				perror(" ");
				fclose(filePtr);
                                if(buf)delete [] buf; 
				return 1;
			}
			unsigned long numread =fread(buf, sizeof(char), noofbytes, filePtr);
			if (numread != noofbytes)
			{
				DEBUGL1("could not read entire file = %s,file size =%l, bytes read = %l ",file.c_str(),noofbytes,numread);
				fclose(filePtr);
                                if(buf)delete [] buf;
				return 1;
			}

			fclose(filePtr);
			filedata.assign(buf, noofbytes);
                        if(buf)delete [] buf;			
		}
	 return 0;
	}
	/**
	 * Creates a signature of the Input file using this certificate's
	 * private key (if available).
	 *
	 * @param[in]		userToken		An existing token
	 * @param[in]		inputFileName		Input file to be signed.
	 * @param[in,out]	signature			Base 64 encoded signature data
	 * @return		status			OK (success), or error codes with
	 *									prefix of CRYPTO_XXX(see ssdkcodes.h)
	 */
	static ssdk::SSDKStatus SignFile( const ssdk::SSDKUserTokenInterface *  userToken,const std::string &inputFileName,std::string &  signature )
	{

		ssdk::SSDKStatus status =ssdk::OK;
		if(!userToken)
				return ssdk::AUTH_TOKEN_NOT_VALID;
               if(inputFileName.empty())
				return ssdk::INVALID_ARGUMENT;
		   std::string inputTempStr;
		   if(1==GetFileData(inputFileName,inputTempStr))
			   {
				   DEBUGL1("GetFileData  FAILED. Status :");
					   return ssdk::INVALID_ARGUMENT;
			   }
		   DEBUGL8("inputFileName  : : %s \n",inputFileName.c_str());

		//Get Certificate Interface Object
		 ci::operatingenvironment::Ref<ssdk::SSDKSecurityManagerInterface> ssdkSecurityManagerInterface = ssdk::GetSecurityManager();
		 ci::operatingenvironment::Ref<ssdk::SSDKCertificateInterface> CertificatePtr = dynamic_cast<ssdk::SSDKCertificateInterface*> (ssdkSecurityManagerInterface->GetInterface(ssdk::SSDKSecurityManagerInterface::CERTIFICATE_INTERFACE, status));
		 if((status !=ssdk::OK)||(CertificatePtr == (void*)NULL))
		 {
				 DEBUGL1("Creation of Certificate Object Failed\n");
				 return status;
		 }
			 //Signing a file.
		 status = CertificatePtr->Sign(userToken, inputTempStr, signature);
		 if(ssdk::OK != status)
		 {
				 DEBUGL1("Signing a file  FAILED. Status : %d \n",status);
				 return status;
		 }
		return status;
	}

	/**
	 * Verifies the signature data of a file.
	 *
	 * @param[in]		userToken		An existing token
	 * @param[in]		filename			input file to be verified.
	 * @param[in,out]	signatureData		Base 64 encoded signature data
	 * @return			status			OK (success), or error codes with
	 *									prefix of CRYPTO_XXX(see ssdkcodes.h)
	 */
	static ssdk::SSDKStatus VerifyFile( const ssdk::SSDKUserTokenInterface *  userToken, const std::string &filename, const std::string &  signatureData)
	{
		ssdk::SSDKStatus status =ssdk::OK;
		if(!userToken)
				return ssdk::AUTH_TOKEN_NOT_VALID;
		if(filename.empty()||signatureData.empty())
				return ssdk::INVALID_ARGUMENT;
		//Get Certificate Interface Object
		 ci::operatingenvironment::Ref<ssdk::SSDKSecurityManagerInterface> ssdkSecurityManagerInterface = ssdk::GetSecurityManager();
		 ci::operatingenvironment::Ref<ssdk::SSDKCertificateInterface> CertificatePtr = dynamic_cast<ssdk::SSDKCertificateInterface*> (ssdkSecurityManagerInterface->GetInterface(ssdk::SSDKSecurityManagerInterface::CERTIFICATE_INTERFACE, status));
		 if((status !=ssdk::OK)||(CertificatePtr == (void*)NULL))
		 {
				 DEBUGL1("Creation of Certificate Object Failed\n");
				 return status;
		 }

		 std::string inputdataStr;
		 if(1==GetFileData(filename,inputdataStr))
			 {
				 DEBUGL1("GetFileData  FAILED. Status :");
					 return ssdk::INVALID_ARGUMENT;
			 }
		 DEBUGL8("inputFileName  : : %s \n",filename.c_str());

		 //Verifying a file.
		 status= CertificatePtr->Verify(userToken, inputdataStr, signatureData);
 		 if(ssdk::OK != status)
		 {
				 DEBUGL1("Verifying  file  FAILED. Status : %d \n",status);
				 return status;
		 }
		return status;

	}
	/**
	 ** LengthOfUTFString
 	 ** Function: LengthOfUTFString finds out char length of a string even if it contains UTF8 char
	 **/

	template <class OutputIterator>
	static OutputIterator  LengthOfUTFString(const unsigned char* it, const unsigned char* end, OutputIterator out)
	{

		while (it != end)
		{
			if (*it < 192)
			{
				DEBUGL8("convert multibye not set**************\n");
				*out++ = *it++; // single byte character 
			}
			else if (*it < 224 && it + 1 < end && *(it+1) > 127) {
				// double byte character
				*out++ = ((*it & 0x1F) << 6) | (*(it+1) & 0x3F);
				it += 2;
				DEBUGL8("convert multibye 2 set**************\n");

			}
			else if (*it < 240 && it + 2 < end && *(it+1) > 127 && *(it+2) > 127) {
				// triple byte character
				*out++ = ((*it & 0x0F) << 12) | ((*(it+1) & 0x3F) << 6) | (*(it+2) & 0x3F);
				it += 3;
				DEBUGL8("convert multibye 3 set**************\n");

			}
			else if (*it < 248 && it + 3 < end && *(it+1) > 127 && *(it+2) > 127 && *(it+3) > 127) {
				// 4-byte character
				*out++ = ((*it & 0x07) << 18) | ((*(it+1) & 0x3F) << 12) |
					((*(it+2) & 0x3F) << 6) | (*(it+3) & 0x3F);
				it += 4;
				DEBUGL8("convert multibye 4 set**************\n");
			}
			else
			{
				DEBUGL8("convert here*********************\n");
				++it; // Invalid byte sequence (throw an exception here if you want)
			}
		}

	return out;
	}

	/**
	 * Encrypts / Decrypts file data.  The encryption mechanism is based
	 * on the type of key management technique or algorithm.
	 *
	 * Memory must be allocated and freed by the caller function
	 * In case, memory is not sufficient, an error code is returned
	 * The first call to the function returns the memory required
	 * in the encDataSize parameter, if the buffer size is inadequate.
	 * This size should be used for actual allocation of encData buffer
	 * mentioned below.
	 *
	 * @param[in]		userToken		An existing token
	 * @param[in]		otype			Type of operation to perform
	 * @param[in]		etype			type of encryption which is dependent
	 *									on the key management technique
	 * @param[in]		inputFileName			input file name
	 * @param[in,out]	outputFileName		output file name.
	 * @param[in]		password		Password string used for encryption.
	 *									This parameter is required only when
	 *									ET_MAC or ET_PASSWORD_WITH_MAC encType is
	 *									specified.
	 * @param[in,out]	prog			Optional parameter. progress
	 *									indicator object that can
	 *									be used by the calling module to
	 *									stop, or continue. Pause is not
	 *									supported.This can be set to NULL if
	 *									tracking of progress is not needed.
	 * @return			status			OK (success),or error codes with
	 *									prefix of CRYPTO_XXX(see ssdkcodes.h)
	 */

	static ssdk::SSDKStatus EncryptionOperation( const ssdk::SSDKUserTokenInterface *  userToken,
													ssdk::SSDKEncryptionInterface::EncType  etype,
													ssdk::SSDKEncryptionInterface::OperationType  otype,
													const std::string & inputFileName,
													const std::string & outputFileName,
													const std::string& password = "",
													ssdk::SSDKProgressIndicatorInterface *	prog = 0)
	{

		ssdk::SSDKStatus status =ssdk::OK;
		if(!userToken)
				return ssdk::AUTH_TOKEN_NOT_VALID;
		ci::operatingenvironment::Ref<ssdk::SSDKSecurityManagerInterface> ssdkSecurityManagerInterface = ssdk::GetSecurityManager();
		ci::operatingenvironment::Ref<ssdk::SSDKEncryptionInterface> EncPtr = dynamic_cast<ssdk::SSDKEncryptionInterface*> (ssdkSecurityManagerInterface->GetInterface(ssdk::SSDKSecurityManagerInterface::ENCRYPTION_INTERFACE, status));
		if((status !=ssdk::OK)||(EncPtr == (void*)NULL))
		{
				DEBUGL1("Creation of Encryption Object Failed\n");
				return status;
		}
		size_t outputDataSize=0;
		std::string inputTempStr;
		if(1==GetFileData(inputFileName,inputTempStr))
		 	{
				DEBUGL1("GetFileData  FAILED. Status :");
				 	return ssdk::INVALID_ARGUMENT;
		 	}
		DEBUGL8("inputFileName  : : %s \n",inputFileName.c_str());
		DEBUGL8("outputFileName  : : %s \n",outputFileName.c_str());

		const unsigned char* inputData=reinterpret_cast<const unsigned char*>(inputTempStr.c_str());
		unsigned char* outputData = NULL;
		size_t inputDataLength=inputTempStr.size();

		status = EncPtr->EncryptionOperation( userToken,otype,etype,inputData,	inputDataLength,
										reinterpret_cast<const unsigned char*>(outputData),	outputDataSize,
										password ,prog );

		if(status==ssdk::CRYPTO_BAD_DATALENGTH)
			{
			outputData = reinterpret_cast< unsigned char*>( malloc(outputDataSize * sizeof( unsigned char) + 1));
			memset(outputData,' ',outputDataSize);

			status = EncPtr->EncryptionOperation( userToken,otype,etype,inputData,inputDataLength,
											reinterpret_cast<const unsigned char*>(outputData),
											outputDataSize,password ,	prog );
				if(ssdk::OK != status)
				{
					DEBUGL1("EncryptionOperation  FAILED. Status : %d \n",status);
					return status;
				}
			}

		outputData[outputDataSize] = 0;
		DEBUGL8("outputData  :: %s \n,  OutputDataSize::%d\n",outputData,outputDataSize);

		//std::string outputTempStr=reinterpret_cast<const char*>(outputData); NG for binary data
		std::string outputTempStr(reinterpret_cast<const char*>(outputData),outputDataSize);

		if(1==WriteDataToFile(outputFileName,outputTempStr))
		 	{
				DEBUGL1("WriteDataToFile  FAILED. Status :");
				 return ssdk::INVALID_ARGUMENT;
		 	}

		return status;

	}
       /**
        * Encrypts / Decrypts file.  The encryption mechanism is based
        * on the type algorithm and mode.
        *
        * Memory must be allocated and freed by the caller function
        * In case, memory is not sufficient, an error code is returned
        * The first call to the function returns the memory required
        * in the encDataSize parameter, if the buffer size is inadequate.
        * This size should be used for actual allocation of encData buffer
        * mentioned below.
        *
        * \verbatim
        * For ENCRYPT_OPERATION or DECRYPT_OPERATION the following
        * combinations of EncAlgorithm and EncMode are
        * valid:
        *
        * EncAlgorithm        EncMode
        * -----------	      -------
        * ENC_ALG_AES         ENC_MODE_CBC/ECB/OFB/CFB
        * ENC_ALG_DES3        ENC_MODE_CBC/ECB/OFB/CFB
        * ENC_ALG_RC4         ENC_MODE_NONE
        * ENC_ALG_RSA         ENC_MODE_NONE
        * ENC_ALG_ECC         ENC_MODE_NONE
        * ENC_ALG_NONE        ENC_MODE_NONE
        * ENC_ALG_NONE        ENC_MODE_NONE
        * ENC_ALG_NONE        ENC_MODE_NONE
        *
        * For DIGEST_OPERATION the following combination of
        * EncType, EncAlgorithm and EncMode are valid
        *
        * EncType       EncAlgorithm       EncMode
        * -------       ------------       -------
        * ET_NONE       ENC_ALG_MD5        ENC_MODE_NONE
        *
        * \endverbatim
        *
        * @param[in]       userToken       An existing token
        * @param[in]       otype           Type of operation to perform
        * @param[in]       alg		       algorithm to be used. Mutually
        *                                  exclusive with encType.
        * @param[in]       mode		       encryption mode to be used. Mutually
        *								   exclusive with encType.
        * @param[in]       inputFileName       	input filename to be encrypted
        * @param[in,out]   outputFileName      output file name .This is allocated
        *                                			  by the caller function
        * @param[in,out]   key             BASE64 encoded key.If no key is passed,
        *                                  a key will be generated based upon the
        *                                  will be the public key. Encryption is
        *                                  ALLOWED ONLY using the algorithm. In
        *                                  case of asymmetric algorithms, this key
        *                                  parameter which is either a symmetric key
        *                                  or the public part of an asymmetric key.
        *								   In case of 'ET_PASSWORD_WITH_MAC', this is the the
        *                                  password.
        * @param[in,out]   inputKeyLength  length of the input key buffer above
        * @param[in]	   iv 		   The initialization vector to be passed
        * @param[in]	   inputIVLength   The length of the initialization vector
        * @param[in,out]   privKey         BASE64 encoded key.If no key is passed
        *                                  in the 'key' parameter above,a private
        *                                  key will be generated based upon the algorithm.
        *                                  This is applicable only for RSA and ECC. This
        *                                  is not populated for DES3, RC4 and AES
        * @param[in,out]   privKeyLength   length of the privKey key buffer
        * @param[in]       keySize         key size to be used as supported by the
        *								   algorithm.
        *                                  For RSA 1024, 2048 are supported
        *								   DES3, ECC use default length;
        *                                  AES: 192,256;
        *                                  RC4: variable
        * @param[in]       base64Encode    encode the output using BASE64 encoding.
        *                                  Applicable only when EncAlgorithm is used
        *                                  and EncType is ET_NONE. Default is false.
        * @param[in,out]   prog            progress indicator object that can
        *                                  be used by the calling module to
        *                                  stop, or continue. Pause is not
        *                                  supported.This can be set to NULL if
        *                                  tracking of progress is not needed.
        * @return          status          OK (success),or error codes with
        *                                  prefix of CRYPTO_XXX(see ssdkcodes.h)
        *
        */
	static ssdk::SSDKStatus EncryptionOperation  	( const ssdk::SSDKUserTokenInterface *  userToken,
												ssdk::SSDKEncryptionInterface::OperationType  otype,
												ssdk::SSDKEncryptionInterface::EncAlgorithm  alg,
												ssdk::SSDKEncryptionInterface::EncMode   mode,
												const std::string & inputFileName,
												const std::string & outputFileName,
												unsigned char *&  key,
												size_t &  inputKeyLength,
												unsigned char *&  InitVector,
												size_t &  inputIVLength,
												unsigned char *&  privKey,
												size_t &  privKeyLength,
												unsigned int  keySize,
												bool  base64Encode,
												ssdk::SSDKProgressIndicatorInterface *  prog = 0)
	{

		 ssdk::SSDKStatus status =ssdk::OK;
		 if(!userToken)
				 return ssdk::AUTH_TOKEN_NOT_VALID;
		 ci::operatingenvironment::Ref<ssdk::SSDKSecurityManagerInterface> ssdkSecurityManagerInterface = ssdk::GetSecurityManager();
		 ci::operatingenvironment::Ref<ssdk::SSDKEncryptionInterface> EncPtr = dynamic_cast<ssdk::SSDKEncryptionInterface*> (ssdkSecurityManagerInterface->GetInterface(ssdk::SSDKSecurityManagerInterface::ENCRYPTION_INTERFACE, status));
		 if((status !=ssdk::OK)||(EncPtr == (void*)NULL))
		 {
				 DEBUGL1("Creation of Encryption Object Failed\n");
				 return status;
		 }
		 std::string inputTempStr;
		 if(1==GetFileData(inputFileName,inputTempStr))
		 	{
				DEBUGL1("GetFileData  FAILED. Status :");
				 return ssdk::INVALID_ARGUMENT;
		 	}
		 DEBUGL8("inputFileName  : : %s \n",inputFileName.c_str());
		 DEBUGL8("outputFileName  : : %s \n",outputFileName.c_str());
		 const unsigned char* inputData=reinterpret_cast<const unsigned char*>(inputTempStr.c_str());
		 unsigned char* outputData = NULL;
		 size_t inputDataLength=inputTempStr.size();
		 size_t outputDataSize=0;
		 status = EncPtr->EncryptionOperation(userToken,otype,alg,mode,inputData,inputDataLength,reinterpret_cast<const unsigned char*>(outputData),
											outputDataSize,key,inputKeyLength,InitVector,inputIVLength,
											privKey,	privKeyLength,keySize,base64Encode,	prog);

		  if(status==ssdk::CRYPTO_BAD_DATALENGTH)
			  {
			  outputData = reinterpret_cast< unsigned char*>( malloc(outputDataSize * sizeof( unsigned char) + 1));
			  memset(outputData,' ',outputDataSize);

			  status = EncPtr->EncryptionOperation(userToken,otype,alg,mode,inputData,inputDataLength,reinterpret_cast<const unsigned char*>(outputData),
											outputDataSize,key,inputKeyLength,InitVector,inputIVLength,
											privKey,	privKeyLength,keySize,base64Encode,prog);
				  if(ssdk::OK != status)
				  {
					  DEBUGL1("EncryptionOperation	FAILED. Status : %d \n",status);
					  return status;
				  }
			  }

		  outputData[outputDataSize] = 0;
		  DEBUGL8("outputData  :: %s \n,  OutputDataSize::%d\n",outputData,outputDataSize);

		  //std::string outputTempStr=reinterpret_cast<const char*>(outputData); NG for binary data
		  std::string outputTempStr(reinterpret_cast<const char*>(outputData),outputDataSize);

		  if(1==WriteDataToFile(outputFileName,outputTempStr))
		  	{
			  DEBUGL1("WriteDataToFile	FAILED. Status :");
		  			  return ssdk::INVALID_ARGUMENT;
		  	}
		return status;

	}
	template<typename T>
	static bool validateInput(const std::string &input, T &output)
	{
		bool bRet = false;

		std::stringstream ss , out;
		ss<<input;
		T temp = -1;
		std::string str;
		if((ss>>temp) && !(ss>>str))
		{
			out<<temp;
			out >> output;
			bRet = true;
		}	

		return bRet;
	}
	//Template Specialization Used for handling bool datatype
	template<typename>
	static bool validateInput(const std::string &input , bool &output)
	{
		std::string str = input;

		std::transform(str.begin(), str.end(), str.begin(), ::tolower);
		if(str == "true" || str == "1") 
		{
			output = true;
			return true;
		} 
		else if(str == "false" || str == "0")
		{
			output = false;
			return true;
		}
		return false;
	}


};
#endif
