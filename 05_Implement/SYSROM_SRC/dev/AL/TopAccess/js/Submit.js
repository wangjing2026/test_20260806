    /** <pre>
     *-------------------------------------------------------------------------------

     *

     *-------------------------------------------------------------------------------
     * File: TopAccessUtil.js
     *-------------------------------------------------------------------------------
     * DESCRIPTION:
     * This JS file is for enter key support in TopAccess.
       This JS file can be used for mapping all the page in TopAccess/E-Filing
       for enter Key suport.Press is a common functon using form events and page
       number as inputs for mapping the enter key support
     *
     * AUTHOR:        Sangeetha
     * PROJECT:		[TOPACCESS]
     *
     * Date Of Creation	: 17/OCT/2007
     * Version No.		: 1.0
     *-------------------------------------------------------------------------------
     * DEPENDENCIES
     *
     *-------------------------------------------------------------------------------
     * </pre>
     */


function enter(no)
{
    if((no>=1 && no<=17)|| no==19 || no==20)//for pages in network list
    {
        switch (no)
                {
            case "11":
                if(getValue("STRSMTPCLPASS") != getValue("HID_INI_STRSMTPCLPASS"))
                    document.forms[0].STRSMTPCLPASS.onchange();
                fnnSaveChanges();
                //fnnSetEnableSSL()// for SMTPCLIENT page
                break;
            case "8":
                if(getValue("STRPASSWORD") != getValue("HID_INI_STRPASSWORD"))
                    document.forms[0].STRPASSWORD.onchange();
                if(getValue("passwordGuestLogon") != getValue("HID_INI_passwordGuestLogon"))
                    document.forms[0].passwordGuestLogon.onchange();
                fnnSaveChanges();
                //fnnSetSMBLogon()// for SMB SESSION page
                break;
            case "13":
                if(getValue("STRPOPPASS") != getValue("HID_INI_STRPOPPASS"))
                    document.forms[0].STRPOPPASS.onchange();
                fnnSaveChanges();
                break;
            case "7":
                if(getValue("STRDDNSPRIPASS") != getValue("HID_INI_STRDDNSPRIPASS"))
                    document.forms[0].STRDDNSPRIPASS.onchange();
                if(getValue("STRDDNSSECPASS") != getValue("HID_INI_STRDDNSSECPASS"))
                    document.forms[0].STRDDNSSECPASS.onchange();
                fnnSaveChanges();
                break;
            case "17":
                if(getValue("STRSNMPV3TRAPAUTHPWD") != getValue("HID_INI_STRSNMPV3TRAPAUTHPWD"))
                    document.forms[0].STRSNMPV3TRAPAUTHPWD.onchange();
                if(getValue("STRSNMPV3TRAPPRVPWD") != getValue("HID_INI_STRSNMPV3TRAPPRVPWD"))
                    document.forms[0].STRSNMPV3TRAPPRVPWD.onchange();
                fnnSaveChanges();
                break;
            case "20":
                fnnSaveChanges();
                //fnnSetIPv6()// for SMB SESSION page
                break;
            default:                
                fnnSaveChanges();
        }
    }
    else
    {
        switch (no)
                {
            case "0":
                fnnSearchBtnClk();// for address book search
                break;
            case "18":
                fnnSave();// for create page in Admin-network-security
                break;
        }
    }
}

function Press(e,no,btcheck) { //e is event object passed from function invocation
/*Added for solving enter key problem in all network settings checking for browser and os */
	var browser=navigator.appName
	if(btcheck==2 && fnnOperatingSystem()!="Mac" && browser!="Netscape")
	{
			window.close();
	}
//btcheck=1;
	if((btcheck==1 && e.keyCode==32) || btcheck==0){
var characterCode;// literal character code will be stored in this variable
if(e && e.which){ //if which property of event object is supported (NN4)
//e = e;
characterCode = e.which;//character code is contained in NN4's which property
}
else{
//e = event;
characterCode = e.keyCode;//character code is contained in IE's keyCode property
}

	if(characterCode == 13 || characterCode == 3 || (btcheck==1 && e.keyCode==32))//if generated character code is equal to ascii 13 (if enter key)
		{
			if(fnnOperatingSystem()=="Mac")
			{
			return;
			}
			else{
				enter(no); //submit the form
				return false;
				}
		}
	else{
		return true
		}
	}
}   



// This function will find the Operating system which we are using.
var detect = navigator.userAgent.toLowerCase();
function checkIt(string)
{
	place = detect.indexOf(string) + 1;
	return place;
}

function fnnOperatingSystem()
{
			var OS;
			if (!OS)
			{
				if (checkIt('linux')) OS = "Linux";
				else if (checkIt('x11')) OS = "Unix";
				else if (checkIt('mac')) OS = "Mac"
				else if (checkIt('win')) OS = "Windows"
				else OS = "an unknown operating system";
			}
	return OS;
}