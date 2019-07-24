/*  (C)Copyright TOSHIBA TEC Corporation 1999,2000. All rights Reserved */
/*
使用インクルード宣言
*/
#include  <stdio.h>
#include  <string.h>
#include  <time.h>
#if 0 /*Paresh commented: vxWorks file*/
#include  "timers.h"

/*#include  "msgQSmLib.h"*/ /*Tornade2.2対応*/
#include  "smMemLib.h"
#include  "smObjLib.h"
#endif
#include "device_info_def.h"	/* maru 070823	*/
#if defined(LOIRE2_ENG) || defined(AL2_ENG) || defined(WEISS_ENG) || defined(ECO_LOIRE2)	/* EBX_DCR_28692 */
#include"IndexedDB/indexeddb.h" /*JEC-01518_CRS_L4.6x_silent_reboot -- roopesh*/
#include "CI/SystemInformation/systeminformation.h"
using namespace ci::systeminformation;
using namespace ci::indexeddb;
#endif
using namespace dl;

extern "C" {

#if 0
#include  "msgQLib.h"
#endif

#include  "comMain.h"                            /* DPPC共通ヘッダ */
#include  "comIErr.h"

/* 03/02/22 追加 */
#include  "cmnUtyEx.h"

/* 2002/03/14 追加 -> 2002/06/11 削除 */
/* #include  "sysMNbuf.h" */

/* V00R26で追加 */
#include  "mcnTBLPar.h"                          /* マシン管理ＬＩＢヘッダ */
#include  "mcnTBLStr.h"

#include  "mcnMNPar.h"                           /* マシン管理タスクdefineヘッダ */
#include  "mcnMNStr.h"                           /* マシン管理タスク構造体ヘッダ */
#include  "mcnMNEngdef.h"                        /* マシン管理タスクエンジン情報defineヘッダ */
#include  "mcnMNEvent.h"                         /* マシン管理タスクイベントdefineヘッダ */
/* 2002/06/11 削除 */
/*#include  "mcnMNMsg.h"*/                           /* マシン管理タスクメッセージＩＤヘッダ */
#include  "f220mcnMNErr.h"                       /* マシン管理タスクエラーコードヘッダ */
#include  "mcnMNEx.h"                            /* マシン管理タスク外部宣言ヘッダ */
/* 2002/06/11 追加 */
#include  "mcnMNCom.h"

/* 05/07/22 add */
#include  "digUIPar.h"                           /* 自己診断ＵＩヘッダ */
#include  "parMNStr.h"                           /* パラメータテーブルヘッダ */
#include  "parMNPar.h"
#include  "parMNEx.h"
#include  "f220parMNErr.h"

#include  "comOErr.h"

/*
プロトタイプ宣言
*/
I32 mcnActAdfGo( I32 );
/* V00R07で追加 */
I32 mcnActAdfStop( I32 );
I32 mcnActScnGo( I32 );
I32 mcnActScnGoWupCal( I32 );
I32 mcnActPrnGo( I32 );
/* V00R07で追加 */
I32 mcnActPrnStop( I32 );
I32 mcnActPrnGoWupAduOut( I32 );
I32 mcnActEtcGo( I32 );
I32 mcnActEtcGoWupPol( I32 );

I32 mcnActEtcReject( I32 );
I32 mcnActScnDuplex( I32 );
I32 mcnActRscNotRun( I32 );
I32 mcnActResourceErr( I32 );
I32 mcnActParResource( I32 );
I32 mcnActParRMode( I32 );
I32 mcnActParMMode( I32 );
I32 mcnActParOnOff( I32 );
I32 mcnActMcpuPwrOff( I32 );
I32 mcnActRscESavOn( I32 );
I32 mcnActMcVersion( I32 );
/* V00R35で追加 */
I32 mcnActIDCountGet( I32 ) ;
/* V00R57で追加 */
I32 mcnActRecoverRscErr( I32 );
/* 000919 siramura add for thames */
I32 mcnActScnWaitCal( I32 );
/* 03/02/19 msei add for McKinley */
I32 mcnActSearchHomePosition( I32 ) ;
#if defined(LOIRE2_ENG) || defined(AL2_ENG) || defined(WEISS_ENG) || defined(ECO_LOIRE2)	/* EBX_DCR_28692 */
void mcnMNSetSilentRebootFlagFalse(); /*JEC-01518_CRS_L4.6x_silent_reboot -- roopesh*/
void mcnMNRebootMFP();
STATUS mcnMNExecuteSilentReboot();
void mcnMNCheckReboot();
void mcnMNCheckSilentRebootStatus(); /*EBX_DTFR_19655*/
#endif
extern int msgSndDIM_ScnStatus(int status);
extern int msgSndDIM_PrnStatus(int status);

/*
(C)Copyright TOSHIBA Corporation 1996. All rights Reserved
  NO:
        
  NAME:
        mcnActAdfGo
  TITLE:
        ＡＤＦ動作要求メッセージ作成処理
  MODULE:
        I32 mcnActAdfGo( I32 iEventData )
  INCLUDE:
        msgQLib.h / msgQSmLib.h
        comMain.h
        mcnMNPar.h / mcnMNStr.h / mcnMNEngdef.h / mcnMNEvent.h / 
        mcnMNMsg.h / f220mcnMNErr.h / mcnMNEx.h
  PARAMETER:
        IN,I32,iEventData,4B,内部イベント
  RETURNS:  
        I32,4B,OK:正常
  OUTLINE:  
        内部イベントに応じて、ＡＤＦに関する動作要求をするメッセージを作成する。
        メッセージ送信先は共通動作タスク。
        ＡＤＦ動作中フラグのセットを行い、動作のＡＣＫ・ＴＥＲＭを要求タスクへ
        返信する為に返信先を保存する。
  NOTES:    
        対象イベント：原稿カウント
  HISTORY:  
        0.00 ,K.Iwasaki ,96/09/30 ,Original
        1.00 ,Sshiramura,000529   ,原稿カウントはTHAMESでは未使用
*/
I32 mcnActAdfGo( I32 iEventData )
{
    I32 iSts ;

/* 02/04/25 未使用のため削除 */
/*    I32 iRet ;*/
/*    I32 iCommand ;*/                              /* 送信メッセージコマンド */
/*    MSG_Q_ID  msgQId ;*/                          /* 送信先メッセージＩＤ */
/*    union MCN_MSGPARAM  sPar ;*/                  /* 送信メッセージパラメータ */


/* 
  ＡＤＦ動作の登録

　iEventData別に、sgiEXに送信するメッセージを登録する
　sgiEXからのACK,TERMを指示元に返すため、
　今のmsgQIDをADF用、SCN用、PRT用別々に持つ
  送信メッセージ作成とともに、usEngExecFlagADFを操作
*/

    iSts = OK ;
#if 0
    memset( &sPar.aParam, MCN_CLEAR, sizeof( union MCN_MSGPARAM ) ) ;
                                         /* パラメータエリアクリア */
    memcpy( &msgQId, &msgQIdSgiEX, sizeof( MSG_Q_ID ) ) ;
                                         /* 送信メッセージキューＩＤセット */
/*
    共通動作タスクへのメッセージを作成
    usEngExecFlagADFを操作
*/

    switch( iEventData )
    {
        case MCN_EVENT_GO_ORGCOUNT:              /* 原稿カウント */
            iCommand = SYS_MSG_CPCT ;
            iRet = mcnMotionMsgMake( &msgQId, iCommand, sPar.iParam ) ;
            usEngExecFlagADF = MCN_SGI_ORG_CNT ;
            usSgiStatusADF = usSgiStatusADF | MCN_STS_BSY ;
            break ;
        default :
            break;
    }

/*
    msgSendQIdの保存
*/
    memcpy(&MsgQIdADF, &msgSendQId, sizeof(MSG_Q_ID));

#endif
    return iSts;
}
/*
(C)Copyright TOSHIBA Corporation 1996. All rights Reserved
  NO:
        
  NAME:
        mcnActAdfStop
  TITLE:
        ＡＤＦ動作中止メッセージ作成処理
  MODULE:
        I32 mcnActAdfStop( I32 iEventData )
  INCLUDE:
        msgQLib.h / msgQSmLib.h
        comMain.h
        mcnMNPar.h / mcnMNStr.h / mcnMNEngdef.h / mcnMNEvent.h / 
        mcnMNMsg.h / f220mcnMNErr.h / mcnMNEx.h
  PARAMETER:
        IN,I32,iEventData,4B,内部イベント
  RETURNS:  
        I32,4B,OK:正常
  OUTLINE:  
        内部イベントに応じて、ＡＤＦに関する動作中止要求をするメッセージを
        作成する。
        メッセージ送信先は共通動作タスク。
  NOTES:    
        対象イベント：原稿カウント中止
  HISTORY:  
        0.00 ,K.Iwasaki ,96/12/19 ,(V00R07)Original
        1.00 ,Sshiramura,000529   ,原稿カウントはTHAMESでは未使用
*/
I32 mcnActAdfStop( I32 iEventData )
{
    I32 iSts ;
#if 0
    I32 iRet ;
    I32 iCommand ;                               /* 送信メッセージコマンド */
    MSG_Q_ID  msgQId ;                           /* 送信先メッセージＩＤ */
    union MCN_MSGPARAM  sPar ;                   /* 送信メッセージパラメータ */
#endif


/* 
  ＡＤＦ動作の中止指示メッセージを作成する。
  （共通動作タスクへのメッセージを作成）
*/

    iSts = OK ;
#if 0
    memset( &sPar.aParam, MCN_CLEAR, sizeof( union MCN_MSGPARAM ) ) ;
                                         /* パラメータエリアクリア */
    memcpy( &msgQId, &msgQIdSgiEX, sizeof( MSG_Q_ID ) ) ;
                                         /* 送信メッセージキューＩＤセット */

    switch( iEventData )
    {
        case MCN_EVENT_STOP_ORGCOUNT:              /* 原稿カウント中止 */
            iCommand = SYS_MSG_CCPC ;
            iRet = mcnMotionMsgMake( &msgQId, iCommand, sPar.iParam ) ;
            break ;
        default :
            break;
    }
#endif
    return iSts;
}
/*
(C)Copyright TOSHIBA Corporation 1996,1997. All rights Reserved
  NO:
        
  NAME:
        mcnActScnGo
  TITLE:
        スキャナ動作要求メッセージ作成処理
  MODULE:
        I32 mcnActScnGo( I32 iEventData )
  INCLUDE:
        msgQLib.h / msgQSmLib.h
        comMain.h
        mcnMNPar.h / mcnMNStr.h / mcnMNEngdef.h / mcnMNEvent.h / 
        mcnMNMsg.h / f220mcnMNErr.h / mcnMNEx.h
  PARAMETER:
        IN,I32,iEventData,4B,内部イベント
  RETURNS:  
        I32,4B,OK:正常
  OUTLINE:  
        内部イベントに応じて、スキャナに関する動作要求をするメッセージを作成する。
        メッセージ送信先は共通動作タスク。
        スキャナ動作中フラグのセットを行い、動作のＡＣＫ・ＴＥＲＭを要求タスクへ
        返信する為に返信先を保存する。
        キャリッジ移動要求の場合は、キャリッジ移動処理管理テーブルへの登録を行う。
  NOTES:    
        対象イベント：キャリッジ移動、スキャナウォーミングアップ、スキャナ予熱
  HISTORY:  
        0.00 ,K.Iwasaki ,96/09/30 ,Original
        0.01 ,K.Iwasaki ,96/11/12 ,予熱処理追加
        0.02 ,K.Iwasaki ,97/04/14 ,(V00R31)キャリッジ移動管理方法変更
        0.03 ,K.Iwasaki ,97/11/11 ,(V00R75)リカバリ時キャリッジ移動中、単独移動の受付
        0.04 ,K.Iwasaki ,97/12/08 ,(V00R79)予熱開始中のエラー発生は通知しない様にする
       ----McKinley----
        1.00 ,msei      ,03/05/19 ,キャリッジ移動 I/F変更
        1.01 ,msei      ,03/05/22 ,送信するパラメータがずれていた不具合修正
        1.02 ,msei      ,03/06/26 ,グローバル変数を命名規約に準拠させる
        1.03 ,msei      ,03/10/11 ,リソース動作状況ログ（SCN動作系）対応
       ----RioGrande----
        2.00 ,K.Fujita  ,04/09/15 ,予熱種別追加（Amazonより復活）
       ----Mash/BP----
		MB059, M.Kinoshita ,08/07/01 ,予熱開始時のSC表示対応
*/
I32 mcnActScnGo( I32 iEventData )
{
    I32 iSts ;
    I32 iRet ;
    I32 iCommand ;                               /* 送信メッセージコマンド */
    MSG_Q_ID  msgQId ;                           /* 送信先メッセージＩＤ */
    union MCN_MSGPARAM  sPar ;                   /* 送信メッセージパラメータ */


/* 
  スキャナ動作の登録

　iEventData別に、sgiEXに送信するメッセージを登録する
　sgiEXからのACK,TERMを指示元に返すため、
　今のmsgQIDをADF用、SCN用、PRT用別々に持つ
  送信メッセージ作成とともに、usEngExecFlagScnを操作
*/

    iSts = OK ;
    memset( &sPar.aParam, MCN_CLEAR, sizeof( union MCN_MSGPARAM ) ) ;
                                         /* パラメータエリアクリア */
    memcpy( &msgQId, &msgQIdSgiEX, sizeof( MSG_Q_ID ) ) ;
                                         /* 送信メッセージキューＩＤセット */
/*
    共通動作タスクへのメッセージを作成
    usEngExecFlagScnを操作
*/

    switch( iEventData )
    {
        case MCN_EVENT_GO_CALMOVE:               /* キャリッジ移動 */
            iCommand = SYS_MSG_CCMO ;
/*            sPar.iParam[0] = iRecvData[0] ;      *//* ＪＯＢ ＩＤ *//*1.00*/
            sPar.iParam[0] = iMcnRecvData[2] ;      /* キャリッジ移動 Ｘ座標   *//*1.00*//*1.01*/
            sPar.iParam[1] = iMcnRecvData[3] ;      /* キャリッジ移動 Ｙ座標   *//*1.00*//*1.01*/
            sPar.iParam[2] = iMcnRecvData[4] ;      /* インジケータ移動 Ｘ座標 *//*1.00*//*1.01*/
            sPar.iParam[3] = iMcnRecvData[5] ;      /* インジケータ移動 Ｙ座標 *//*1.00*//*1.01*/
            cmnPrintf_mcnMN(1,"[mcnMN]: sPar.iParam[0] = %d\n",sPar.iParam[0]);
            cmnPrintf_mcnMN(1,"[mcnMN]: sPar.iParam[1] = %d\n",sPar.iParam[1]);
            cmnPrintf_mcnMN(1,"[mcnMN]: sPar.iParam[2] = %d\n",sPar.iParam[2]);
            cmnPrintf_mcnMN(1,"[mcnMN]: sPar.iParam[3] = %d\n",sPar.iParam[3]);

            /* キャリッジ移動処理管理テーブルに登録 */
/* V00R31で修正 */
            iRet = mcnCalTblSet( &msgMcnSendQId ) ;
            iRet = mcnMotionMsgMake( &msgQId, iCommand, sPar.iParam ) ;
/* V00R75で修正 */
            usMcnEngExecFlagScn = usMcnEngExecFlagScn | MCN_SGI_CAL_MOV ;
            usMcnSgiStatusScn = usMcnSgiStatusScn | MCN_STS_BSY ;
            mcnSetEngExecFlagTime(usMcnEngExecFlagScn);/*1.03*/
            break ;
        case MCN_EVENT_GO_WUPSCN:                /* スキャナＷｕｐ */
            iCommand = SYS_MSG_CWUP ;
            sPar.iParam[0] = MCN_RCV_RSC_SCN ;   /* スキャナ指定 */
            iRet = mcnMotionMsgMake( &msgQId, iCommand, sPar.iParam ) ;
            usMcnEngExecFlagScn = MCN_SGI_SCN_WUP ;
            usMcnSgiStatusScn = usMcnSgiStatusScn | MCN_STS_BSY ;
	        msgSndDIM_ScnStatus(1);

            break ;
        case MCN_EVENT_GO_ESAVSCN:               /* スキャナ予熱 */
            iCommand = SYS_MSG_CSPH ;
            sPar.iParam[0] = iMcnRecvData[0] ;   /* ＯＮ／ＯＦＦ */
            sPar.iParam[1] = MCN_RCV_RSC_SCN ;   /* スキャナ指定 */
            /* 2.00 予熱開始の場合は予熱種別をセット */
            if(sPar.iParam[0] == FALSE)
            {
                sPar.iParam[2] = iMcnRecvData[2] ;
            }else{
                sPar.iParam[2] = MCN_CLEAR;
            }
            iRet = mcnMotionMsgMake( &msgQId, iCommand, sPar.iParam ) ;
            usMcnEngExecFlagScn = MCN_SGI_SCN_ESAV ;
            usMcnSgiStatusScn = usMcnSgiStatusScn | MCN_STS_BSY ;

/* V00R79で追加 */
/*
  予熱開始の時は、予熱中フラグをONにする。
*/
            if( iMcnRecvData[0] == FALSE )
            {
                hMcnScnESavFlg = ON ;

                /* 2.00 スーパー予熱ならフラグをONにする */
                if( iMcnRecvData[2] == MCN_ESAV_KIND_SPR )
                {
                    hMcnScnSESavFlg = ON;       /* スキャナスーパー予熱中フラグ */
                }
                /* 2.00 普通の予熱時はスーパー予熱フラグをOFFにする */
                else
                {
                    hMcnScnSESavFlg = OFF;       /* スキャナスーパー予熱中フラグ */
                }

/* 06/02/14 K.Fujita add for K2*/
#if 0	/* MB059 Delete */
D                hMcnScnESavTermFlg = ON ;      /*予熱中フラグ(Term受信)をON*/
#endif

            }

            break ;
        default :
            break;
    }


/*
    msgSendQIdの保存
*/
    memcpy(&MsgQIdMcnScn, &msgMcnSendQId, sizeof(MSG_Q_ID));


    return iSts;
}
/*
(C)Copyright TOSHIBA TEC Corporation 2000. All rights Reserved
  NO:
        
  NAME:
        mcnActScnWaitCal
  TITLE:
        W-up中のキャリッジ移動保留およびメッセージ作成処理
  MODULE:
        I32 mcnActScnWaitCal( I32 iEventData )
  INCLUDE:
        msgQLib.h / msgQSmLib.h
        comMain.h
        mcnMNPar.h / mcnMNStr.h / mcnMNEngdef.h / mcnMNEvent.h / 
        mcnMNMsg.h / f220mcnMNErr.h / mcnMNEx.h
  PARAMETER:
        IN,I32,iEventData,4B,内部イベント
  RETURNS:  
        I32,4B,OK:正常
  OUTLINE:  
        内部イベントに応じて、スキャナに関する動作要求をするメッセージを作成する。
        メッセージ送信先は共通動作タスク。
        スキャナ動作中フラグのセットを行い、動作のＡＣＫ・ＴＥＲＭを要求タスクへ
        返信する為に返信先を保存する。
        キャリッジ移動要求の場合は、キャリッジ移動処理管理テーブルへの登録を行う。
  NOTES:    
        対象イベント：キャリッジ移動
  HISTORY:  
        0.00 ,S.shiramura ,00/09/19 ,Original
       ----McKinley----
        1.00 ,msei      ,03/06/26 ,グローバル変数を命名規約に準拠させる
        1.01 ,msei      ,03/10/14 ,キャリッジ移動 I/F変更
        1.02 ,msei      ,03/10/14 ,リソース動作状況ログ（SCN動作系）対応
        1.03 ,msei      ,03/11/07 ,スキャナウォーミングアップ中に２回以上キャリッジ移動指示を受けると、
                                   スキャナステータスがキャリッジ移動中のままになる不具合修正
*/
I32 mcnActScnWaitCal( I32 iEventData )
{
    I32 iSts ;
    I32 iRet ;
    I32 iCommand ;                               /* 送信メッセージコマンド */
    MSG_Q_ID  msgQId ;                           /* 送信先メッセージＩＤ */
    union MCN_MSGPARAM  sPar ;                   /* 送信メッセージパラメータ */


/* 
  スキャナ動作の登録

　iEventData別に、sgiEXに送信するメッセージを登録する
　sgiEXからのACK,TERMを指示元に返すため、
　今のmsgQIDをADF用、SCN用、PRT用別々に持つ
  送信メッセージ作成とともに、usEngExecFlagScnを操作
*/

    iSts = OK ;
    memset( &sPar.aParam, MCN_CLEAR, sizeof( union MCN_MSGPARAM ) ) ;
                                         /* パラメータエリアクリア */
    memcpy( &msgQId, &msgQIdSgiEX, sizeof( MSG_Q_ID ) ) ;
                                         /* 送信メッセージキューＩＤセット */
/*
    共通動作タスクへのメッセージを作成
    usEngExecFlagScnを操作
*/

    switch( iEventData )
    {
        case MCN_EVENT_WAIT_CALMOVE:             /* キャリッジ移動保留 */
            /*iMcnWaitCalMoveID = iMcnRecvData[0] ;*/      /* JOB ID保存 *//*1.01*/
            iMcnCarriageX  = iMcnRecvData[2];/*1.01*/
            iMcnCarriageY  = iMcnRecvData[3];/*1.01*/
            iMcnIndicatorX = iMcnRecvData[4];/*1.01*/
            iMcnIndicatorY = iMcnRecvData[5];/*1.01*/
            if( iMcnCalMoveRecvFlg != ON )/*1.03*/
            {
                /* キャリッジ移動処理管理テーブルに登録 */
                iRet = mcnCalTblSet( &msgMcnSendQId ) ;
                iMcnCalMoveRecvFlg = ON ;
            }
            /* msgSendQIdの保存 */
            memcpy(&MsgQIdMcnScn, &msgMcnSendQId, sizeof(MSG_Q_ID));
            break;
            
        case MCN_EVENT_GO_WAITCALMOVE:               /* キャリッジ移動 */
            iCommand = SYS_MSG_CCMO ;
            /*sPar.iParam[0] = iMcnWaitCalMoveID ;*/      /* ＪＯＢ ＩＤ *//*1.01*/
            sPar.iParam[0] = iMcnCarriageX  ;/*1.01*/
            sPar.iParam[1] = iMcnCarriageY  ;/*1.01*/
            sPar.iParam[2] = iMcnIndicatorX ;/*1.01*/
            sPar.iParam[3] = iMcnIndicatorY ;/*1.01*/
#if 0  /* ここでは登録しない */
            /* キャリッジ移動処理管理テーブルに登録 */
            iRet = mcnCalTblSet( &msgSendQId ) ;
#endif
            iRet = mcnMotionMsgMake( &msgQId, iCommand, sPar.iParam ) ;
            usMcnEngExecFlagScn = usMcnEngExecFlagScn | MCN_SGI_CAL_MOV ;
            usMcnSgiStatusScn = usMcnSgiStatusScn | MCN_STS_BSY ;
            iMcnCalMoveRecvFlg = OFF ;
            iMcnCarriageX  = (I32)NULL;/*1.01*/
            iMcnCarriageY  = (I32)NULL;/*1.01*/
            iMcnIndicatorX = (I32)NULL;/*1.01*/
            iMcnIndicatorY = (I32)NULL;/*1.01*/
            mcnSetEngExecFlagTime(usMcnEngExecFlagScn);/*1.02*/
            break ;
        default :
            break;
    }

    return iSts;
}
/*
(C)Copyright TOSHIBA Corporation 1996,1997. All rights Reserved
  NO:
        
  NAME:
        mcnActScnGoWupCal
  TITLE:
        リカバリのキャリッジ移動要求メッセージ作成処理
  MODULE:
        I32 mcnActScnGoWupCal( I32 iEventData )
  INCLUDE:
        msgQLib.h / msgQSmLib.h
        comMain.h
        mcnMNPar.h / mcnMNStr.h / mcnMNEngdef.h / mcnMNEvent.h / 
        mcnMNMsg.h / f220mcnMNErr.h / mcnMNEx.h
  PARAMETER:
        IN,I32,iEventData,4B,内部イベント
  RETURNS:  
        I32,4B,OK:正常
  OUTLINE:  
        リカバリによるキャリッジ移動の動作要求をするメッセージを作成する。
        メッセージ送信先は共通動作タスク。
        スキャナ動作中フラグのセットを行う。
        スキャナウォーミングアップを行っていない場合は、動作のＡＣＫ・ＴＥＲＭを
        要求タスクへ返信する為に返信先を保存する。
        キャリッジ移動動作管理テーブルに項目の設定を行う。
  NOTES:    
        
  HISTORY:  
        0.00 ,K.Iwasaki ,96/09/30 ,Original
        0.01 ,K.Iwasaki ,97/04/14 ,(V00R31)キャリッジ移動管理方法変更
        0.02 ,K.Iwasaki ,97/07/24 ,(V00R57)動作中のリカバリ指示TRMにNOT READYをセット
        0.03 ,K.Iwasaki ,97/11/11 ,(V00R75)リカバリ時キャリッジ移動中、単独移動の受付
        1.00 ,Sshiramura,000531   ,リカバリ時キャリッジ移動廃止につき本関数は未使用
*/
#if 0
I32 mcnActScnGoWupCal( I32 iEventData )
{
    I32 iSts ;
    I32 iRet ;
    I32 iCommand ;                               /* 送信メッセージコマンド */
    MSG_Q_ID  msgQId ;                           /* 送信先メッセージＩＤ */
    union MCN_MSGPARAM  sPar ;                   /* 送信メッセージパラメータ */


/* 
  スキャナ動作の登録

　iEventData別に、sgiEXに送信するメッセージを登録する
　スキャナＷｕｐの要求がされていな（返信先msgQIDが登録されていない）場合は、
　sgiEXからのACK,TERMを指示元に返すため、
　今のmsgQIDをSCN用に登録する。
  送信メッセージ作成とともに、usEngExecFlagScnを操作
*/

    iSts = OK ;
    memset( &sPar.aParam, MCN_CLEAR, sizeof( union MCN_MSGPARAM ) ) ;
                                         /* パラメータエリアクリア */
    memcpy( &msgQId, &msgQIdSgiEX, sizeof( MSG_Q_ID ) ) ;
                                         /* 送信メッセージキューＩＤセット */
/*
    共通動作タスクへのメッセージを作成
    usEngExecFlagScnを操作
*/
/* V00R75で修正 */
    iCommand = SYS_MSG_CRCM ;
    sPar.iParam[0] = iWupCalJobId ;      /* ＪＯＢ ＩＤ */
    iRet = mcnMotionMsgMake( &msgQId, iCommand, sPar.iParam ) ;
/* V00R57で修正 */
    usEngExecFlagScn = MCN_SGI_WUPCAL_MOV ;

/*
    msgSendQIdの保存
*/
    if( (usMcnJobScn & MCN_SGI_SCN_WUP) == FALSE )
    {
        memcpy(&MsgQIdScn, &msgSendQId, sizeof(MSG_Q_ID));
    }

    /* キャリッジ移動処理管理テーブルに登録 */
/* V00R31で修正 */
/* V00R75で削除 */
/*  iRet = mcnCalTblSet( &MsgQIdScn ) ;  */

    usSgiStatusScn = usSgiStatusScn | MCN_STS_BSY ;

    return iSts;
}
#endif
#if defined(LOIRE2_ENG) || defined(AL2_ENG) || defined(WEISS_ENG) || defined(ECO_LOIRE2)	/* EBX_DCR_28692 */
void mcnMNRebootMFP()
{
	cmnPrintf_mcnMN(0,"[mcnMN]Rebooting the MFP as part of silent reboot\n");
	SystemInformationRef sysInfo = SystemInformation::Acquire();
	if (!sysInfo) {
		cmnPrintf_mcnMN(8,"SystemInformation get failed.\n");
	return;
	}
	int ret;
	sysInfo->RunCmd("reboot", ret);
}
void mcnMNSetSilentRebootFlagFalse()
{
	ci::operatingenvironment::Ref<IndexedDB> m_pIDB = IndexedDB::Acquire();
        if(m_pIDB)
	{
		char value = 0;
		uint64 key = IndexedDB::CreateKey(5000, 4, 0);
	        m_pIDB->SetValue(key, &value, 1); // clear silent reboot flag
		cmnPrintf_mcnMN(7, "[mcnMN:mcnMNSetSilentRebootFlagFalse() - key value is set to FALSE \n");
	}
	else
	{
		cmnPrintf_mcnMN(8, "[mcnMN:mcnMNSetSilentRebootFlagFalse() - IndexedDB object not found!! \n");
	}
}
STATUS mcnMNExecuteSilentReboot( )
{
	ci::operatingenvironment::Ref<IndexedDB> m_pIDB = IndexedDB::Acquire();
	int m_bSilentReboot;
	if(m_pIDB)
	{
		uint64 key = IndexedDB::CreateKey(5000, 4, 0);
		int size = 1;
		void* ptr = NULL;
		STATUS ret = m_pIDB->GetValue(key, size, ptr);
		if(ret != OK) {
			cmnPrintf_mcnMN(1,"[mcnMN]: Failed to get silent reboot flag\n");
		} else {
			if(ptr)
			{
				m_bSilentReboot = (*(char*)(ptr) == 1);
				if(m_bSilentReboot == TRUE)
				return TRUE;
			}
		}
	}
	else
	{
		cmnPrintf_mcnMN(8, "[mcnMN:mcnMNExecuteSilentReboot() - IndexedDB object not found!! \n");
	}
 return FALSE;
}
void mcnMNCheckReboot()
{
	if(mcnMNExecuteSilentReboot() == TRUE){
		mcnMNSetSilentRebootFlagFalse();
		mcnMNRebootMFP();
	}
}

/*EBX_DTFR_19655*/
void mcnMNCheckSilentRebootStatus()
{
	if(hMcnFstSleepFlg == FALSE && hMcnPrnRebootFlg == TRUE)
	{			
		mcnMNCheckReboot();	
	}
}
#endif
/*
(C)Copyright TOSHIBA Corporation 1996. All rights Reserved
  NO:
        
  NAME:
        mcnActPrnGo
  TITLE:
        プリンタ動作要求メッセージ作成処理
  MODULE:
        I32 mcnActPrnGo( I32 iEventData )
  INCLUDE:
        msgQLib.h / msgQSmLib.h
        comMain.h
        mcnMNPar.h / mcnMNStr.h / mcnMNEngdef.h / mcnMNEvent.h / 
        mcnMNMsg.h / f220mcnMNErr.h / mcnMNEx.h
  PARAMETER:
        IN,I32,iEventData,4B,内部イベント
  RETURNS:  
        I32,4B,OK:正常
  OUTLINE:  
        内部イベントに応じて、プリンタに関する動作要求をするメッセージを作成する。
        メッセージ送信先は共通動作タスク。
        プリンタ動作中フラグのセットを行い、動作のＡＣＫ・ＴＥＲＭを要求タスクへ
        返信する為に返信先を保存する。
  NOTES:    
        対象イベント：ＡＤＵ内用紙排出、プリンタウォーミングアップ、プリンタ予熱
  HISTORY:  
        0.00 ,K.Iwasaki ,96/09/30 ,Original
        0.01 ,K.Iwasaki ,96/11/12 ,予熱処理追加
        0.02 ,K.Iwasaki ,97/12/08 ,(V00R79)予熱開始中のエラー発生は通知しない様にする
        1.00 ,Sshiramura ,991215 ,Thames トナー補給指示削除
       ----McKinley----
        2.00 ,msei       ,03/02/19 ,ホーム位置検出指示、リボルバー回転指示追加
        2.01 ,msei       ,03/02/28 ,CWUPのパラメータにトナー補給ＯＮ／ＯＦＦを追加
        2.02 ,msei       ,03/03/13 ,CRTNのパラメータ不具合修正
                                    ( hCshpAfterEtef->hTnrUncertain )
        2.03 ,msei       ,03/06/26 ,グローバル変数を命名規約に準拠させる
        2.04 ,K.Fujita   ,04/07/06 ,Kトナーニアエンプティリセット対応
       ----RioGrande----
        3.00 ,K.Fujita  ,04/09/15 ,予熱種別追加（Amazonより復活）
       ----Mash/BP----
		MB059, M.Kinoshita ,08/07/01 ,予熱開始時のSC表示対応
*/
I32 mcnActPrnGo( I32 iEventData )
{
    I32 iSts ;
    I32 iRet ;
    I32 iCommand ;                               /* 送信メッセージコマンド */
    MSG_Q_ID  msgQId ;                           /* 送信先メッセージＩＤ */
    union MCN_MSGPARAM  sPar ;                   /* 送信メッセージパラメータ */

    I32 iIdx ;  /*2.04*/

/* 
  プリンタ動作の登録

　iEventData別に、sgiEXに送信するメッセージを登録する
　sgiEXからのACK,TERMを指示元に返すため、
　今のmsgQIDをADF用、SCN用、PRT用別々に持つ
  送信メッセージ作成とともに、usEngExecFlagPrnを操作
*/

    iSts = OK ;
    memset( &sPar.aParam, MCN_CLEAR, sizeof( union MCN_MSGPARAM ) ) ;
                                         /* パラメータエリアクリア */
#if defined(LOIRE2_ENG) || defined(AL2_ENG) || defined(WEISS_ENG) || defined(ECO_LOIRE2)	/* EBX_DCR_28692 */								 
	if((mcnMNExecuteSilentReboot() == TRUE)&&
		(hMcnPrnRebootFlg == FALSE) &&
		(MCN_EVENT_GO_WUPPRN == iEventData)){
		cmnPrintf_mcnMN(7,"sending dummy msg for dummy sgi\n");
		 memcpy( &msgQId, &msgQIdSgiEXDummy, sizeof( MSG_Q_ID ) ) ; 									  		
	}else
#endif	
    memcpy( &msgQId, &msgQIdSgiEX, sizeof( MSG_Q_ID ) ) ;
                                         /* 送信メッセージキューＩＤセット */
/*
    共通動作タスクへのメッセージを作成
    usEngExecFlagPrnを操作
*/

    switch( iEventData )
    {
        case MCN_EVENT_GO_ADUOUT:                /* ＡＤＦ白紙排出 */
            iCommand = SYS_MSG_CAWP ;
            sPar.iParam[0] = iMcnRecvData[0] ;      /* ＪＯＢ ＩＤ */
            iRet = mcnMotionMsgMake( &msgQId, iCommand, sPar.iParam ) ;
            usMcnEngExecFlagPrn = MCN_SGI_ADU_OUT ;
            usMcnSgiStatusPrn = usMcnSgiStatusPrn | MCN_STS_BSY ;
            break ;
        case MCN_EVENT_GO_WUPPRN:                /* プリンタＷｕｐ */
            iCommand = SYS_MSG_CWUP ;
            sPar.iParam[0] = MCN_RCV_RSC_PRT ;      /* プリンタ指定 */
            sPar.iParam[1] = (I32)hMcnWupAddToner;  /* トナー補給ＯＮ／ＯＦＦ 03/02/28 */
            iRet = mcnMotionMsgMake( &msgQId, iCommand, sPar.iParam ) ;
            usMcnEngExecFlagPrn = MCN_SGI_PRN_WUP ;
            usMcnSgiStatusPrn = usMcnSgiStatusPrn | MCN_STS_BSY ;
			msgSndDIM_PrnStatus(1);
            break ;
#ifdef MISSI_TONER
/* 991215 Sshiramura thames トナー補給指示削除 */
/* 98/10/27 追加 --------------------------------------------------*/
        case MCN_EVENT_GO_TNRADD:                /* トナー補給指示 */
            iCommand = SYS_MSG_CTOC ;
            sPar.iParam[0] = MCN_RCV_RSC_PRT ;   /* プリンタ指定？ */
            iRet = mcnMotionMsgMake( &msgQId, iCommand, sPar.iParam ) ;
            usEngExecFlagPrn = MCN_SGI_TNR_ADD ;
            usSgiStatusPrn = usSgiStatusPrn | MCN_STS_BSY ;
            break ;
/*-----------------------------------------------------------------*/
#endif
        case MCN_EVENT_GO_ESAVPRN:               /* プリンタ予熱 */
            iCommand = SYS_MSG_CSPH ;
            sPar.iParam[0] = iMcnRecvData[0] ;      /* ＯＮ／ＯＦＦ */
            sPar.iParam[1] = MCN_RCV_RSC_PRT ;   /* プリンタ指定 */
            /* 3.00 予熱開始の場合は予熱種別をセット */
            if(sPar.iParam[0] == FALSE)
            {
                sPar.iParam[2] = iMcnRecvData[2] ;
            }else{
                sPar.iParam[2] = MCN_CLEAR;
		  msgSndDIM_PrnStatus(1);
            }
            iRet = mcnMotionMsgMake( &msgQId, iCommand, sPar.iParam ) ;
            usMcnEngExecFlagPrn = MCN_SGI_PRN_ESAV ;
            usMcnSgiStatusPrn = usMcnSgiStatusPrn | MCN_STS_BSY ;

/* V00R79で追加 */
/*
  予熱開始の時は、予熱中フラグをONにする。
*/
            if( iMcnRecvData[0] == FALSE )
            {
                hMcnPrnESavFlg = ON ;

                /* 3.00 スーパー予熱ならフラグをONにする */
                if( iMcnRecvData[2] == MCN_ESAV_KIND_SPR )
                {
                    hMcnPrnSESavFlg = ON;       /* プリンタスーパー予熱中フラグ */
                }
                /* 3.00 普通の予熱時はスーパー予熱フラグをOFFにする */
                else
                {
                    hMcnPrnSESavFlg = OFF;       /* プリンタスーパー予熱中フラグ */
                }

/* 06/02/14 K.Fujita add for K2*/
#if 0	/* MB059 Delete */
D                hMcnPrnESavTermFlg = ON ;          /*予熱中フラグ(Term受信)をON*/
#endif

            }

            break ;
/* 03/02/19 ホーム位置検出指示 */
        case MCN_EVENT_GO_HOMEPOS:               /* ホーム位置検出指示 */
            iCommand = SYS_MSG_CSHP ;
            sPar.iParam[0] = iMcnRecvData[0] ;      /* リボルバー回転位置 */
            sPar.iParam[1] = OFF ;               /* プリンタ指定       */
            iRet = mcnMotionMsgMake( &msgQId, iCommand, sPar.iParam ) ;
            usMcnEngExecFlagPrn = MCN_SGI_HOME_POS ;
            usMcnSgiStatusPrn = usMcnSgiStatusPrn | MCN_STS_BSY ;
            break ;

        case MCN_EVENT_GO_REVOTRN:               /* リボルバー回転指示 */

/* 04/07/06 K.Fujita add for Mckinley/Rainbow */ /*2.04*/
/*
リボルバー回転位置がＫトナーであれば、sgiEXには何もメッセージを送らずそのまま
UI側にTERMを送信する。
*/
            if (iMcnRecvData[0] == COM_FO_ORDER_K)
            {
                hmcnKTonerChange = ON;
                printf("[mcnMN]: hmcnKTonerChange = ON(%d) \n", ON );
                iIdx = iMcnEventCount ;
                iMcnEvent[iIdx] = MCN_EVENT_TERM_REVOTRN;
                iMcnEventCount++ ;

            }
            else {
                iCommand = SYS_MSG_CRTN ;
                sPar.iParam[0] = iMcnRecvData[0] ;            /* リボルバー回転位置 */
                sPar.iParam[1] = (I32)hMcnTnrUncertain ;   /* 初期化ＯＮ/ＯＦＦ  *//*2.02*/
                iRet = mcnMotionMsgMake( &msgQId, iCommand, sPar.iParam ) ;
                usMcnEngExecFlagPrn = MCN_SGI_REVOLVER_TURN ;
                usMcnSgiStatusPrn = usMcnSgiStatusPrn | MCN_STS_BSY ;

/* 05/08/24 */
#if 1
				if (hmcnYTonerChange == MCN_REVOTRN_STOP) 
					hmcnYTonerChange = OFF;
				else if (hmcnMTonerChange == MCN_REVOTRN_STOP) 
					hmcnMTonerChange = OFF;
				else if (hmcnCTonerChange == MCN_REVOTRN_STOP) 
					hmcnCTonerChange = OFF;
				switch (iMcnRecvData[0])
				{
					case COM_FO_ORDER_Y:
						if ((hmcnYTonerChange != ON) && (mcnPrnWngTblChk(COM_MCW_PRC_YTNR_NEMPTY) == OK))
							hmcnYTonerChange = MCN_REVOTRN_START;
						break;
					case COM_FO_ORDER_M:
						if ((hmcnMTonerChange != ON) && (mcnPrnWngTblChk(COM_MCW_PRC_MTNR_NEMPTY) == OK))
							hmcnMTonerChange = MCN_REVOTRN_START;
						break;
					case COM_FO_ORDER_C:
						if ((hmcnCTonerChange != ON) && (mcnPrnWngTblChk(COM_MCW_PRC_CTNR_NEMPTY) == OK))
							hmcnCTonerChange = MCN_REVOTRN_START;
						break;
					default:
						break;
				}
#endif

            }

            break ;

        default :
            break;
    }


/*
    msgSendQIdの保存
*/
    memcpy(&MsgQIdMcnPrn, &msgMcnSendQId, sizeof(MSG_Q_ID));


    return iSts;
}
/*
(C)Copyright TOSHIBA Corporation 1996,1997. All rights Reserved
  NO:
        
  NAME:
        mcnActPrnGoWupAduOut
  TITLE:
        リカバリのＡＤＵ内用紙排出要求メッセージ作成処理
  MODULE:
        I32 mcnActPrnGoWupAduOut( I32 iEventData )
  INCLUDE:
        msgQLib.h / msgQSmLib.h
        comMain.h
        mcnMNPar.h / mcnMNStr.h / mcnMNEngdef.h / mcnMNEvent.h / 
        mcnMNMsg.h / f220mcnMNErr.h / mcnMNEx.h
  PARAMETER:
        IN,I32,iEventData,4B,内部イベント
  RETURNS:  
        I32,4B,OK:正常
  OUTLINE:  
        リカバリによるＡＤＵ内用紙排出の動作要求をするメッセージを作成する。
        メッセージ送信先は共通動作タスク。
        プリンタ動作中フラグのセットを行う。
        プリンタウォーミングアップを行っていない場合は、動作のＡＣＫ・ＴＥＲＭを
        要求タスクへ返信する為に返信先を保存する。
  NOTES:    
        
  HISTORY:  
        0.00 ,K.Iwasaki ,96/09/30 ,Original
        0.01 ,K.Iwasaki ,97/07/24 ,(V00R57)動作中のリカバリ指示TRMにNOT READYをセット
       ----McKinley----
        1.00 ,msei      ,03/06/26 ,グローバル変数を命名規約に準拠させる
*/
I32 mcnActPrnGoWupAduOut( I32 iEventData )
{
    I32 iSts ;
    I32 iRet ;
    I32 iCommand ;                               /* 送信メッセージコマンド */
    MSG_Q_ID  msgQId ;                           /* 送信先メッセージＩＤ */
    union MCN_MSGPARAM  sPar ;                   /* 送信メッセージパラメータ */


/* 
  プリンタ動作の登録

　iEventData別に、sgiEXに送信するメッセージを登録する
　プリンタＷｕｐの要求がされていな（返信先msgQIDが登録されていない）場合は、
　sgiEXからのACK,TERMを指示元に返すため、
　今のmsgQIDをPRN用に登録する。
  送信メッセージ作成とともに、usEngExecFlagPrnを操作
*/

    iSts = OK ;
    memset( &sPar.aParam, MCN_CLEAR, sizeof( union MCN_MSGPARAM ) ) ;
                                         /* パラメータエリアクリア */
    memcpy( &msgQId, &msgQIdSgiEX, sizeof( MSG_Q_ID ) ) ;
                                         /* 送信メッセージキューＩＤセット */
/*
    共通動作タスクへのメッセージを作成
    usEngExecFlagPrnを操作
*/

    iCommand = SYS_MSG_CAWP ;
    sPar.iParam[0] = iMcnWupAduOutJobId ;      /* ＪＯＢ ＩＤ */
    iRet = mcnMotionMsgMake( &msgQId, iCommand, sPar.iParam ) ;
/* V00R57で修正 */
    usMcnEngExecFlagPrn = MCN_SGI_WUPADU_OUT ;

/*
    msgSendQIdの保存
*/
    if( (usMcnJobPrn & MCN_SGI_PRN_WUP) == FALSE )
    {
        memcpy(&MsgQIdMcnPrn, &msgMcnSendQId, sizeof(MSG_Q_ID));
    }

    usMcnSgiStatusPrn = usMcnSgiStatusPrn | MCN_STS_BSY ;


    return iSts;
}
/*
(C)Copyright TOSHIBA Corporation 1996. All rights Reserved
  NO:
        
  NAME:
        mcnActPrnStop
  TITLE:
        プリンタ動作中止メッセージ作成処理
  MODULE:
        I32 mcnActPrnStop( I32 iEventData )
  INCLUDE:
        msgQLib.h / msgQSmLib.h
        comMain.h
        mcnMNPar.h / mcnMNStr.h / mcnMNEngdef.h / mcnMNEvent.h / 
        mcnMNMsg.h / f220mcnMNErr.h / mcnMNEx.h
  PARAMETER:
        IN,I32,iEventData,4B,内部イベント
  RETURNS:  
        I32,4B,OK:正常
  OUTLINE:  
        内部イベントに応じて、プリンタに関する動作中止要求をするメッセージを
        作成する。
        メッセージ送信先は共通動作タスク。
  NOTES:    
        対象イベント：ＡＤＵ内用紙排出中止,IH停止
  HISTORY:  
        0.00 ,K.Iwasaki ,96/12/19 ,(V00R07)Original
        1.00 ,Sshiramrua,000525   ,IH停止指示処理追加 
        1.01 ,Sshiramura,000529   ,ADUアウト削除
       ----McKinley----
        2.00 ,msei      ,03/06/26 ,グローバル変数を命名規約に準拠させる
*/
I32 mcnActPrnStop( I32 iEventData )
{
    I32 iSts ;
    I32 iRet ;
    I32 iCommand ;                               /* 送信メッセージコマンド */
    MSG_Q_ID  msgQId ;                           /* 送信先メッセージＩＤ */
    union MCN_MSGPARAM  sPar ;                   /* 送信メッセージパラメータ */


/* 
  プリンタ動作の中止指示メッセージを作成する。
  （共通動作タスクへのメッセージを作成）
*/

    iSts = OK ;
    memset( &sPar.aParam, MCN_CLEAR, sizeof( union MCN_MSGPARAM ) ) ;
                                         /* パラメータエリアクリア */
    memcpy( &msgQId, &msgQIdSgiEX, sizeof( MSG_Q_ID ) ) ;
                                         /* 送信メッセージキューＩＤセット */

    switch( iEventData )
    {
#if 0 /* 000529 ssiramura */
        case MCN_EVENT_STOP_ADUOUT:                /* ＡＤＵ排出中止 */
            iCommand = SYS_MSG_CCWP ;
            iRet = mcnMotionMsgMake( &msgQId, iCommand, sPar.iParam ) ;
            break ;
#endif
        case MCN_EVENT_STOP_IH_HEATER:             /* IH停止 */
            iCommand = SYS_MSG_CCIH ;
        /* IH停止コマンド送信　*/
            iRet = mcnMotionMsgMake( &msgQId, iCommand, sPar.iParam ) ;
        /* 動作要求元にエラーのため実行不可であるメッセージを返す　*/
            if(iRet == OK)
            {
                iSts = mcnActRscNotRun( iMcnMessageCom ) ;
            } 
            break ;
        default :
            break;
    }

    return iSts;
}

/*
(C)Copyright TOSHIBA Corporation 1996,1997. All rights Reserved
  NO:
        
  NAME:
        mcnActEtcGo
  TITLE:
        複数リソース動作要求メッセージ作成処理
  MODULE:
        I32 mcnActEtcGo( I32 iEventData )
  INCLUDE:
        msgQLib.h / msgQSmLib.h
        comMain.h
        mcnMNPar.h / mcnMNStr.h / mcnMNEngdef.h / mcnMNEvent.h / 
        mcnMNMsg.h / f220mcnMNErr.h / mcnMNEx.h
  PARAMETER:
        IN,I32,iEventData,4B,内部イベント
  RETURNS:  
        I32,4B,OK:正常
  OUTLINE:  
        内部イベントに応じて、スキャナとプリンタに関する動作、リソースに
        関係のない動作等の要求を行うメッセージを作成する。
        メッセージ送信先は共通動作タスク。
        動作の対象となるリソースの動作中フラグのセットを行い、
        動作のＡＣＫ・ＴＥＲＭを要求タスクへ返信する為に返信先を保存する。
        但し、ポリゴンモータ回転とフィニッシャビン移動はＡＣＫ・ＴＥＲＭが
        不要の為、メッセージの作成のみ。
  NOTES:    
        対象イベント：動作モード通知、スキャナ＆プリンタウォーミングアップ、
                      スキャナ＆プリンタ予熱、タイマ（ＭＣＰＵ電源変化）、
                      ポリゴンモータ回転、ＩＤカウント残値通知、
                      フィニッシャビン移動
  HISTORY:  
        0.00 ,K.Iwasaki ,96/09/30 ,Original
        0.01 ,K.Iwasaki ,96/11/12 ,予熱・タイマ処理追加
        0.02 ,K.Iwasaki ,97/05/10 ,(V00R35)ＩＤコードカウント対応
        0.03 ,K.Iwasaki ,97/11/25 ,(V00R77)フィニッシャビン移動対応
        0.04 ,K.Iwasaki ,97/12/08 ,(V00R79)予熱開始中のエラー発生は通知しない様にする
        1.00 ,Sshiramura,000224   ,自己診断モード通知は全モード通知するようにする
        1.01 ,Sshiramura,000711   ,ポリゴン制御変更によるイベント追加
       ----McKinley----
        2.00 ,msei      ,03/02/28 ,CWUPのパラメータにトナー補給ＯＮ／ＯＦＦを追加
        2.01 ,msei      ,03/06/26 ,グローバル変数を命名規約に準拠させる
       ----RioGrande----
        3.00 ,K.Fujita  ,04/09/15 ,予熱種別追加（Amazonより復活）
        3.01 ,K.Fujita  ,04/09/30 ,マニュアルステイプル動作状態追加
       -----K2 RC-----
		KK303, S.Tanaka, 06/07/06, timer_createによるメモリリーク対応
		KK319, S.Tanaka, 07/01/22, ポリゴン停止タイマ不具合対策
		----- Mash/BP -----
		MB017, E.Sonoda, 07/10/31, BoxToPrintポリゴン先投げ対応
		MB059, M.Kinoshita ,08/07/01 ,予熱開始時のSC表示対応

*/
I32 mcnActEtcGo( I32 iEventData )
{
    I32 iSts ;
    I32 iRet ;
    I32 iCommand ;                               /* 送信メッセージコマンド */
    MSG_Q_ID  msgQId ;                           /* 送信先メッセージＩＤ */
    union MCN_MSGPARAM  sPar ;                   /* 送信メッセージパラメータ */

	HI	hMode;/*05/07/22*/

/* 
  動作の登録

　iEventData別に、sgiEXに送信するメッセージを登録する
　sgiEXからのACK,TERMを指示元に返すため、
　今のmsgQIDをADF用、SCN用、PRT用､ETC用別々に持つ
  送信メッセージ作成とともに、usEngExecFlagPrn等を操作
  但し、REQUESTメッセージの場合はメッセージ登録のみ
*/

    iSts = OK ;
    memset( &sPar.aParam, MCN_CLEAR, sizeof( union MCN_MSGPARAM ) ) ;
                                         /* パラメータエリアクリア */
    memcpy( &msgQId, &msgQIdSgiEX, sizeof( MSG_Q_ID ) ) ;
                                         /* 送信メッセージキューＩＤセット */
/*
    共通動作タスクへのメッセージを作成
    usEngExecFlagADF/usEngExecFlagScn/usEngExecFlagPrn/usEngExecFlagEtcを操作
    msgSendQIdの保存
*/

    switch( iEventData )
    {
        case MCN_EVENT_GO_MMODE:                 /* 動作モード */
            iCommand = SYS_MSG_CMMO ;
#if 0   /* 000224 自己診断通知は全モードを通知する */
            sPar.iParam[0] = iMCMode ;           /* モード */
#endif
            sPar.iParam[0] = iMCModeReq ;        /* モード */
            iRet = mcnMotionMsgMake( &msgQId, iCommand, sPar.iParam ) ;
            usMcnEngExecFlagEtc = MCN_SGI_MMODE ;
            memcpy(&MsgQIdMcnEtc, &msgMcnSendQId, sizeof(MSG_Q_ID));
            break ;
        case MCN_EVENT_GO_WUPALL:                /* Ｗｕｐ */
            iCommand = SYS_MSG_CWUP ;
            sPar.iParam[0] = MCN_RCV_RSC_ALL ;   /* スキャナ&プリンタ指定 */
            sPar.iParam[1] = (I32)hMcnWupAddToner;  /* トナー補給ＯＮ／ＯＦＦ 03/02/28 */
            iRet = mcnMotionMsgMake( &msgQId, iCommand, sPar.iParam ) ;
            usMcnEngExecFlagScn = MCN_SGI_SCN_WUP ;
            usMcnEngExecFlagPrn = MCN_SGI_PRN_WUP ;
            memcpy(&MsgQIdMcnScn, &msgMcnSendQId, sizeof(MSG_Q_ID));
            memcpy(&MsgQIdMcnPrn, &msgMcnSendQId, sizeof(MSG_Q_ID));
            usMcnSgiStatusPrn = usMcnSgiStatusPrn | MCN_STS_BSY ;
            usMcnSgiStatusScn = usMcnSgiStatusScn | MCN_STS_BSY ;
			msgSndDIM_PrnStatus(1);
			msgSndDIM_ScnStatus(1);
            hMcnAduClose = OFF;  /*ADU紙残りエラー暫定対策 99/03/10 */
            break ;
        case MCN_EVENT_GO_ESAVALL:               /* 予熱 */
            iCommand = SYS_MSG_CSPH ;
            sPar.iParam[0] = iMcnRecvData[0] ;      /* ＯＮ／ＯＦＦ */
            sPar.iParam[1] = MCN_RCV_RSC_ALL ;   /* スキャナ&プリンタ指定 */
            /* 3.00 予熱開始の場合は予熱種別をセット */
            if(sPar.iParam[0] == FALSE)
            {
                sPar.iParam[2] = iMcnRecvData[2] ;
            }else{
                sPar.iParam[2] = MCN_CLEAR;
		  msgSndDIM_PrnStatus(1);
            }
            iRet = mcnMotionMsgMake( &msgQId, iCommand, sPar.iParam ) ;
            usMcnEngExecFlagScn = MCN_SGI_SCN_ESAV ;
            usMcnEngExecFlagPrn = MCN_SGI_PRN_ESAV ;
            memcpy(&MsgQIdMcnScn, &msgMcnSendQId, sizeof(MSG_Q_ID));
            memcpy(&MsgQIdMcnPrn, &msgMcnSendQId, sizeof(MSG_Q_ID));
            usMcnSgiStatusPrn = usMcnSgiStatusPrn | MCN_STS_BSY ;
            usMcnSgiStatusScn = usMcnSgiStatusScn | MCN_STS_BSY ;

/* V00R79で追加 */
/*
  予熱開始の時は、予熱中フラグをONにする。
*/
            if( iMcnRecvData[0] == FALSE )
            {
                hMcnPrnESavFlg = ON ;
                hMcnScnESavFlg = ON ;

                /* 3.00 スーパー予熱ならフラグをONにする */
                if( iMcnRecvData[2] == MCN_ESAV_KIND_SPR )
                {
                    hMcnPrnSESavFlg = ON;       /* プリンタスーパー予熱中フラグ */
                    hMcnScnSESavFlg = ON;       /* スキャナスーパー予熱中フラグ */
                    hMcnScnSESavFlgWup = ON;    /* スキャナスーパー予熱開始フラグ */
                }
                /* 3.00 普通の予熱時はスーパー予熱フラグをOFFにする */
                else
                {
                    hMcnPrnSESavFlg = OFF;       /* プリンタスーパー予熱中フラグ */
                    hMcnScnSESavFlg = OFF;       /* スキャナスーパー予熱中フラグ */
                }

/* 06/02/14 K.Fujita add for K2*/
#if 0	/* MB059 Delete */
D                hMcnPrnESavTermFlg = ON ;       /* プリンタ予熱中フラグ(Term受信) */
D                hMcnScnESavTermFlg = ON ;       /* スキャナ予熱中フラグ(Term受信) */
#endif

            }

            break ;
        case MCN_EVENT_GO_MCPU   :               /* ＭＣＰＵ電源 */
            iCommand = SYS_MSG_CMPW ;
            sPar.iParam[0] = iMcnRecvData[0] ;      /* ＯＮ／ＯＦＦ */
            iRet = mcnMotionMsgMake( &msgQId, iCommand, sPar.iParam ) ;
            usMcnEngExecFlagScn = MCN_SGI_MCPU_ONOF ;
            usMcnEngExecFlagPrn = MCN_SGI_MCPU_ONOF ;
            usMcnEngExecFlagADF = MCN_SGI_MCPU_ONOF ;
            memcpy(&MsgQIdMcnScn, &msgMcnSendQId, sizeof(MSG_Q_ID));
            memcpy(&MsgQIdMcnPrn, &msgMcnSendQId, sizeof(MSG_Q_ID));
            memcpy(&MsgQIdMcnADF, &msgMcnSendQId, sizeof(MSG_Q_ID));
            usMcnSgiStatusPrn = usMcnSgiStatusPrn | MCN_STS_BSY ;
            usMcnSgiStatusScn = usMcnSgiStatusScn | MCN_STS_BSY ;
            usMcnSgiStatusADF = usMcnSgiStatusADF | MCN_STS_BSY ;
            break ;

        case MCN_EVENT_GO_POLYGON:
            iCommand = SYS_MSG_RPOS ;            /* ポリゴンモータ回転 */
            sPar.iParam[0] = iMcnRecvData[0] ;      /* ＯＮ／ＯＦＦ */
			sPar.iParam[1] = iMcnRecvData[1] ;		/* 回転数 *//* MB017 */

            iRet = mcnMotionMsgMake( &msgQId, iCommand, sPar.iParam ) ;

/* 05/07/22*/
            if (sPar.iParam[0] == TRUE) { /*ポリゴン回転の場合*/
                if (hMcnPlgnStartFlg_Prn == ON
#ifdef BP_ENG/*chnage is to make sure that mcnPolygonCancelTimer should not be called only when mcnPolygonStartTimer is called when authentication  is enabled.*/
				&&(1 == iMcnPolygonTimerCount) 
#endif
				) { /*プリンタ先投げが行われている場合は、タイマーを取り消してフラグをOFFにする*/
                    mcnPolygonCancelTimer();
                }
                hMcnPlgnStartFlg = ON;      /* フラグ設定 */
                hMcnPlgnStartFlg_Prn = OFF; /* フラグ設定 */
                hMcnPlgnStopFlg_Prn = OFF;  /* フラグ設定 */
            }
            else {
                hMcnPlgnStartFlg = OFF; /* フラグ設定 */
                hMcnPlgnStopFlg = OFF;  /* フラグ設定 */
            }

            break ;

#if defined (LOIRE2_ENG)
        case MCN_EVENT_GO_FUSER_HEAT:    /* Rinulin - EBX_DCR_54404 - F/B of EBX_DCR_32711 - Fuser Pre-heat up CRS */
            iCommand = SYS_MSG_RHES ;           
            sPar.iParam[0] = iMcnRecvData[0] ;      /* TRUE/FALSE */
			sPar.iParam[1] = iMcnRecvData[1] ;		/* Color Mode */
            iRet = mcnMotionMsgMake( &msgQId, iCommand, sPar.iParam ) ;
            break ;			
#endif
/* 05/07/22 add */
		case MCN_EVENT_GO_POLYGON_PRN:
			iCommand = SYS_MSG_RPOS ;				/* ポリゴンモータ回転 */
			sPar.iParam[0] = iMcnRecvData[0] ;		/* ＯＮ／ＯＦＦ */
			sPar.iParam[1] = iMcnRecvData[1] ;		/* 回転数 *//* MB017 */
#ifdef BP_ENG /*new RPOS msg added for BP machine -- Roopesh*/
			sPar.iParam[2] = iMcnRecvData[2] ;
#endif
			if (sPar.iParam[0] == TRUE)
			{ /*ポリゴン回転の場合*/
				if (parMNNV08ParamI(D4_POLYGON_MOTOR_STOP_TIME, NVM_NO_SCODE, (void*)&hMode) == ERROR)
				{
					cmnPrintf_mcnMN(1,"[mcnMN]*** parMNNV08ParamI(08-486) ERROR \n" );
					return(ERROR);
				}
				hMode = 15+(hMode)*5; /*(hMode + 1) * 15;*/ /* 11/10/10 - changed based on MSM request - ACR: JEC-01245_DeviceConfigurationList_08.xls*/
#ifdef BP_ENG /*JEC-01518_CRS_FPOT_Improvement_for_EX-BP.doc -- roopesh*/
				if(FALSE == iMcnRecvData[2]){
				if (mcnPolygonStartTimer(hMode) == OK){
#else
				if (mcnPolygonStartTimer(hMode) == OK){
#endif
					iRet = mcnMotionMsgMake( &msgQId, iCommand, sPar.iParam ) ;
					hMcnPlgnStartFlg_Prn = ON;	/* フラグ設定 */
					hMcnPlgnStartFlg = OFF ;	/* フラグ解除 */
					hMcnPlgnStopFlg = OFF ; 	/* フラグ解除 */
				}
#ifdef BP_ENG				
				}else{
					iRet = mcnMotionMsgMake( &msgQId, iCommand, sPar.iParam ) ;
					hMcnPlgnStartFlg_Prn = ON;	/* フラグ設定 */
					hMcnPlgnStartFlg = OFF ;	/* フラグ解除 */
					hMcnPlgnStopFlg = OFF ; 	/* フラグ解除 */
				}
#endif				
			}
			else
			{	/*ポリゴン停止の場合*/
				/* KK319 Start */
				if (iMcnPolygonTimerCount == 0)		
				{
					iRet = mcnMotionMsgMake( &msgQId, iCommand, sPar.iParam ) ;
					hMcnPlgnStartFlg_Prn = OFF; /* フラグ設定 */
					hMcnPlgnStopFlg_Prn = OFF;	/* フラグ設定 */
				}
				else
				{
					cmnPrintf_mcnMN(1, "[mcnMN]:Polygon Started Can't send RPOS\n");
				}
				/* KK319 End */
			}

			break ;

/* 000711 shiramura add for thames ポリゴン制御変更 */
        case MCN_EVENT_STOP_POLYGON:
            iCommand = SYS_MSG_RPOS ;            /* ポリゴンモータ回転 */
            sPar.iParam[0] = FALSE ;             /* ＯＦＦ */
            iRet = mcnMotionMsgMake( &msgQId, iCommand, sPar.iParam ) ;

/* 05/07/22 */
            hMcnPlgnStartFlg = OFF;     /* フラグ設定 */
            hMcnPlgnStopFlg = OFF;      /* フラグ設定 */
            hMcnPlgnStartFlg_Prn = OFF; /* フラグ設定 */
            hMcnPlgnStopFlg_Prn = OFF;  /* フラグ設定 */
            break ;

#if defined (LOIRE2_ENG) 	
		case MCN_EVENT_STOP_FUSERHEAT :		/* Rinulin - EBX_DCR_54404 - F/B of EBX_DCR_32711 - Fuser Pre-heat up CRS - Start */
			iCommand = SYS_MSG_RHES ;            
            sPar.iParam[0] = FALSE ;            
			sPar.iParam[1] = iMcnRecvData[1] ;		/* Color Mode */		
            iRet = mcnMotionMsgMake( &msgQId, iCommand, sPar.iParam ) ;
            break ;
#endif
/* V00R35で追加 */
        case MCN_EVENT_GO_IDCOUNT:              /* ＩＤカウント残値通知 */
            iCommand = SYS_MSG_CIDC ;
            sPar.iParam[0] = iMcnIDCountMax - iMcnIDCount ;      /* 残値 */
            iRet = mcnMotionMsgMake( &msgQId, iCommand, sPar.iParam ) ;
            usMcnEngExecFlagIdCount = MCN_SGI_IDCOUNT ;
            memcpy(&MsgQIdMcnIdCount, &msgMcnSendQId, sizeof(MSG_Q_ID));
            break ;
/* V00R77で追加 */
        case MCN_EVENT_GO_FINBINMOV:            /* フィニッシャビン移動 */
            iCommand = SYS_MSG_RFSM ;
            iRet = mcnMotionMsgMake( &msgQId, iCommand, sPar.iParam ) ;
            break ;

/* 04/09/30 K.Fujita add for RioGrande */
        case MCN_EVENT_GO_MANSTAPLE: /*マニュアルステイプル*//*3.01*/
            iCommand = SYS_MSG_CKSD ;

/* 05/09/01 */
            sPar.iParam[0] = iMcnRecvData[0] ;      /* ＯＫ／ＥＲＲＯＲ */
			if (iMcnRecvData[0] == ERROR) {
				hMcnManualSts = MCN_MANUAL_WAIT_TERM;
			}

            iRet = mcnMotionMsgMake( &msgQId, iCommand, sPar.iParam ) ;
            break ;

        default :
            break;
    }


    return iSts;
}
/*
(C)Copyright TOSHIBA Corporation 1996. All rights Reserved
  NO:
        
  NAME:
        mcnActEtcGoWupPol
  TITLE:
        リカバリのポリゴンモータ回転要求メッセージ作成処理
  MODULE:
        I32 mcnActEtcGoWupPol( I32 iEventData )
  INCLUDE:
        msgQLib.h / msgQSmLib.h
        comMain.h
        mcnMNPar.h / mcnMNStr.h / mcnMNEngdef.h / mcnMNEvent.h / 
        mcnMNMsg.h / f220mcnMNErr.h / mcnMNEx.h
  PARAMETER:
        IN,I32,iEventData,4B,内部イベント
  RETURNS:  
        I32,4B,OK:正常
  OUTLINE:  
        リカバリによるポリゴンモータ回転の動作要求をするメッセージを作成する。
        メッセージ送信先は共通動作タスク。
        ポリゴンモータ回転はＡＣＫ・ＴＥＲＭが不要の為、ポリゴンモータ回転の
        終了をセットする。
  NOTES:    
        
  HISTORY:  
        0.00 ,K.Iwasaki ,96/09/30 ,Original
*/
I32 mcnActEtcGoWupPol( I32 iEventData )
{
    I32 iSts ;
    I32 iRet ;
    I32 iCommand ;                               /* 送信メッセージコマンド */
    MSG_Q_ID  msgQId ;                           /* 送信先メッセージＩＤ */
    union MCN_MSGPARAM  sPar ;                   /* 送信メッセージパラメータ */


/* 
  動作の登録

　iEventData別に、sgiEXに送信するメッセージを登録する
*/

    iSts = OK ;
    memset( &sPar.aParam, MCN_CLEAR, sizeof( union MCN_MSGPARAM ) ) ;
                                         /* パラメータエリアクリア */
    memcpy( &msgQId, &msgQIdSgiEX, sizeof( MSG_Q_ID ) ) ;
                                         /* 送信メッセージキューＩＤセット */
/*
    共通動作タスクへのメッセージを作成
*/

    iCommand = SYS_MSG_RPOS ;            /* ポリゴンモータ回転 */
    sPar.iParam[0] = TRUE ;              /* ＯＮ／ＯＦＦ */
    sPar.iParam[1] = MCN_POLYGON_KIND_COPY; /* 回転数 */ /* 05/07/22 add */
    iRet = mcnMotionMsgMake( &msgQId, iCommand, sPar.iParam ) ;

/* ポリゴンモータ回転終了セット */
    usMcnTermPrn = usMcnTermPrn | MCN_SGI_POLYGON ;

    return iSts;
}

/*
(C)Copyright TOSHIBA Corporation 1996,1997. All rights Reserved
  NO:
        
  NAME:
        mcnActEtcReject
  TITLE:
        動作要求不可ＴＥＲＭメッセージ作成処理
  MODULE:
        I32 mcnActEtcReject( I32 iEventData )
  INCLUDE:
        msgQLib.h / msgQSmLib.h
        comMain.h
        mcnMNPar.h / mcnMNStr.h / mcnMNEngdef.h / mcnMNEvent.h / 
        mcnMNMsg.h / f220mcnMNErr.h / mcnMNEx.h
  PARAMETER:
        IN,I32,iEventData,4B,内部イベント
  RETURNS:  
        I32,4B,OK:正常
  OUTLINE:  
        内部イベントに応じて、要求動作が実行不可である意のメッセージを作成する為、
        各処理に分岐する。
        メッセージ送信先は動作要求のメッセージに設定されている返信先タスク。
  NOTES:    
        
  HISTORY:  
        0.00 ,K.Iwasaki ,96/09/30 ,Original
        0.01 ,K.Iwasaki ,96/11/12 ,予熱・タイマ処理追加
        0.02 ,K.Iwasaki ,97/05/10 ,(V00R35)ＩＤコードカウント対応
        0.03 ,K.Iwasaki ,97/07/24 ,(V00R57)動作中のリカバリ指示TRMにNOT READYをセット
       ----McKinley----
        1.00 ,msei      ,03/06/26 ,グローバル変数を命名規約に準拠させる
*/
I32 mcnActEtcReject( I32 iEventData )
{
    I32 iSts ;

    iSts = OK ;
    switch( iEventData )
    {
        case MCN_EVENT_SJOB_DUPLEX:
                iSts = mcnActScnDuplex( iMcnMessageCom ) ;
                break ;
        case MCN_EVENT_AJOB_NOTRUN:
        case MCN_EVENT_SJOB_NOTRUN:
        case MCN_EVENT_PJOB_NOTRUN:
        case MCN_EVENT_ALLJOB_NOTRUN:
                iSts = mcnActRscNotRun( iMcnMessageCom ) ;
                break ;
        case MCN_EVENT_AJOB_RESOURCE:
        case MCN_EVENT_SJOB_RESOURCE:
        case MCN_EVENT_PJOB_RESOURCE:
        case MCN_EVENT_ALLJOB_RESOURCE:
        case MCN_EVENT_ETCJOB_RESOURCE:
                iSts = mcnActResourceErr( iMcnMessageCom ) ;
                break ;
        case MCN_EVENT_PAR_RESOURCE:
                iSts = mcnActParResource( iMcnMessageCom ) ;
                break ;
        case MCN_EVENT_PAR_RMODE:
                iSts = mcnActParRMode( iMcnMessageCom ) ;
                break ;
        case MCN_EVENT_PAR_MMODE:
                iSts = mcnActParMMode( iMcnMessageCom ) ;
                break ;
        case MCN_EVENT_PAR_ONOFF:
                iSts = mcnActParOnOff( iMcnMessageCom ) ;
                break ;
        case MCN_EVENT_MCPU_PWROFF:
                iSts = mcnActMcpuPwrOff( iMcnMessageCom ) ;
                break ;
        case MCN_EVENT_RSC_ESAVON:
                iSts = mcnActRscESavOn( iMcnMessageCom ) ;
                break ;
        case MCN_EVENT_MC_VERSION:
                iSts = mcnActMcVersion( iMcnMessageCom ) ;
                break ;
        case MCN_EVENT_IDCOUNT_GET :
                iSts = mcnActIDCountGet( iMcnMessageCom ) ;
                break ;
/* V00R57で追加 */
        case MCN_EVENT_SJOB_RECOVERRSC:
        case MCN_EVENT_PJOB_RECOVERRSC:
        case MCN_EVENT_ALLJOB_RECOVERRSC:
                iSts = mcnActRecoverRscErr( iMcnMessageCom ) ;
                break ;
        default :  
                break;
    }

    return iSts ;
}
/*
(C)Copyright TOSHIBA Corporation 1996. All rights Reserved
  NO:
        
  NAME:
        mcnActScnDuplex
  TITLE:
        要求重複動作不可メッセージ作成処理
  MODULE:
        I32 mcnActScnDuplex( I32 iMsgCom )
  INCLUDE:
        msgQLib.h / msgQSmLib.h
        comMain.h / comIErr.h
        mcnMNPar.h / mcnMNStr.h / mcnMNEngdef.h / mcnMNEvent.h / 
        mcnMNMsg.h / f220mcnMNErr.h / mcnMNEx.h
  PARAMETER:
        IN,I32,iMsgCom,4B,要求メッセージのメッセージＩＤ
  RETURNS:  
        I32,4B,OK:正常
  OUTLINE:  
        動作要求のメッセージＩＤに応じて、既に別タスクのキャリッジ移動動作が
        実行されている為に実行不可である意のメッセージを作成する。
        メッセージ送信先は動作要求のメッセージに設定されている返信先タスク。
  NOTES:    
        対象メッセージＩＤ：キャリッジ移動
  HISTORY:  
        0.00 ,K.Iwasaki ,96/09/30 ,Original
       ----McKinley----
        1.00 ,msei      ,03/06/26 ,グローバル変数を命名規約に準拠させる
*/
I32 mcnActScnDuplex( I32 iMsgCom )
{
    I32 iSts ;
    I32 iRet ;
    I32 iCommand ;                               /* 送信メッセージコマンド */
    MSG_Q_ID  msgQId ;                           /* 送信先メッセージＩＤ */
    union MCN_MSGPARAM  sPar ;                   /* 送信メッセージパラメータ */


/* 
　iMsgCom別に、msgSendQIdに送信するメッセージを登録する
*/

    iSts = OK ;
    memset( &sPar.aParam, MCN_CLEAR, sizeof( union MCN_MSGPARAM ) ) ;
                                         /* パラメータエリアクリア */
    memcpy( &msgQId, &msgMcnSendQId, sizeof( MSG_Q_ID ) ) ;
                                         /* 送信メッセージキューＩＤセット */

    switch( iMsgCom )
    {
        case SYS_MSG_CCMO   :                    /* キャリッジ移動 */
            iCommand = SYS_MSG_TCMO ;
            sPar.iParam[0] = ERROR ;
            sPar.iParam[1] = S_f220McnMNErr_DUPLEX_JOB;
            iRet = mcnMotionMsgMake( &msgQId, iCommand, sPar.iParam ) ;
            break ;
        default :
            break;
    }


    return iSts;
}
/*
(C)Copyright TOSHIBA Corporation 1996,1997. All rights Reserved
  NO:
        
  NAME:
        mcnActRscNotRun
  TITLE:
        リソースエラー中動作不可メッセージ作成処理
  MODULE:
        I32 mcnActRscNotRun( I32 iMsgCom )
  INCLUDE:
        msgQLib.h / msgQSmLib.h
        comMain.h / comIErr.h
        mcnMNPar.h / mcnMNStr.h / mcnMNEngdef.h / mcnMNEvent.h / 
        mcnMNMsg.h / f220mcnMNErr.h / mcnMNEx.h
  PARAMETER:
        IN,I32,iMsgCom,4B,要求メッセージのメッセージＩＤ
  RETURNS:  
        I32,4B,OK:正常
  OUTLINE:  
        動作要求のメッセージＩＤに応じて、リソースがエラー以下の状態（エラー・
        ダウン・NOT READY等）である為に実行不可である意のメッセージを作成する。
        但し、ポリゴンモータ回転とフィニッシャビン移動はＡＣＫ・ＴＥＲＭが
        不要の為、処理は行わない。
        メッセージ送信先は動作要求のメッセージに設定されている返信先タスク。
  NOTES:    
        対象メッセージＩＤ：キャリッジ移動、原稿カウント、ＡＤＵ内用紙排出、
                            リカバリ、予熱、タイマ（ＭＣＰＵ電源変化）、
                            ポリゴンモータ回転、フィニッシャビン移動
                            リボルバー回転
  HISTORY:  
        0.00 ,K.Iwasaki ,96/09/30 ,Original
        0.01 ,K.Iwasaki ,96/11/12 ,予熱・タイマ処理追加
        0.02 ,K.Iwasaki ,97/06/09 ,(V00R44)リカバリTRMにNOT READYリソース追加
        0.03 ,K.Iwasaki ,97/11/25 ,(V00R77)フィニッシャビン移動対応
        1.00 ,Sshiramura, 000615  ,NOT_RUNエラーで自己診断がキー入力不可になる問題対応 
       ----McKinley----
        2.00 ,msei      ,03/02/19 ,リボルバー回転指示対応
        2.01 ,msei      ,03/06/26 ,グローバル変数を命名規約に準拠させる
       ----RioGrande----
        3.00 ,K.Fujita  ,04/09/15 ,予熱種別追加（Amazonより復活）
*/
I32 mcnActRscNotRun( I32 iMsgCom )
{
    I32 iSts ;
    I32 iRet ;
    I32 iCommand ;                               /* 送信メッセージコマンド */
    MSG_Q_ID  msgQId ;                           /* 送信先メッセージＩＤ */
    union MCN_MSGPARAM  sPar ;                   /* 送信メッセージパラメータ */


/* 
　iMsgCom別に、msgSendQIdに送信するメッセージを登録する
*/

    iSts = OK ;
    memset( &sPar.aParam, MCN_CLEAR, sizeof( union MCN_MSGPARAM ) ) ;
                                         /* パラメータエリアクリア */
    memcpy( &msgQId, &msgMcnSendQId, sizeof( MSG_Q_ID ) ) ;
                                         /* 送信メッセージキューＩＤセット */

    switch( iMsgCom )
    {
        case SYS_MSG_CCMO   :                    /* キャリッジ移動 */
            iCommand = SYS_MSG_TCMO ;
            sPar.iParam[0] = ERROR ;
            sPar.iParam[1] = S_f220McnMNErr_NOT_RUN;
            iRet = mcnMotionMsgMake( &msgQId, iCommand, sPar.iParam ) ;
            break ;
        case SYS_MSG_CPCT   :                    /* 原稿カウント指示 */
            iCommand = SYS_MSG_TPCT ;
            sPar.iParam[1] = S_f220McnMNErr_NOT_RUN;
            iRet = mcnMotionMsgMake( &msgQId, iCommand, sPar.iParam ) ;
            break ;
        case SYS_MSG_CAOP   :                    /* ＡＤＵ排出 */
            iCommand = SYS_MSG_TAOP ;
            sPar.iParam[0] = S_f220McnMNErr_NOT_RUN;
            iRet = mcnMotionMsgMake( &msgQId, iCommand, sPar.iParam ) ;
            break ;
        case SYS_MSG_CMRC   :                    /* Ｗｕｐ */
        case SYS_MSG_C05R   :                    /* Test Copy Test Print */
            iCommand = SYS_MSG_TMRC ;
/* 000615 siramura NOT_RUNエラーで自己診断がキー入力不可になる問題対応 */
/*                 01,03,08,09の時はNOT_RUN_DIGMODE */
            if( (iMCMode == COM_MODE_01PNLCHECK)||
                (iMCMode == COM_MODE_03IOCHECK) ||
                (iMCMode == COM_MODE_08SETTING) ||
                (iMCMode == COM_MODE_09PACKING)  ) 
            {
#ifdef DEBUG_MCNRCV
    printf("[mcnMN]: TF02-CHK1 [mcnActRscNotRun] \n");
    printf("[mcnMN]: [0] = S_f220McnMNErr_NOT_RUN_DIGMODE \n");
#endif
                sPar.iParam[0] = S_f220McnMNErr_NOT_RUN_DIGMODE;
            }
            else
            {
#ifdef DEBUG_MCNRCV
    printf("[mcnMN]: TF02-CHK1 [mcnActRscNotRun] \n");
    printf("[mcnMN]: [0] = S_f220McnMNErr_NOT_RUN \n");
#endif
                sPar.iParam[0] = S_f220McnMNErr_NOT_RUN;
            }
            sPar.hParam[2] = iMcnRecvData[0];
            sPar.hParam[3] = iMcnRecvData[1];
            sPar.hParam[4] = iMcnRecvData[2];
/* V00R44で追加 */
            sPar.hParam[5] = MCN_RCV_RSC_NON;
            iRet = mcnMotionMsgMake( &msgQId, iCommand, sPar.iParam ) ;
            break ;
        case SYS_MSG_CONF   :                    /* MCPU電源指示 */
            iCommand = SYS_MSG_TONF ;
            sPar.iParam[0] = iMcnRecvData[0];
/* 000525 siramura IH制御用メッセージパラメータ追加 */
            sPar.iParam[1] = iMcnRecvData[1];
            sPar.iParam[2] = S_f220McnMNErr_NOT_RUN; /* 00525 [1] -> [2] */
            iRet = mcnMotionMsgMake( &msgQId, iCommand, sPar.iParam ) ;
            break ;
        case SYS_MSG_CAPS   :                    /* 予熱動作 */
            iCommand = SYS_MSG_TAPS ;
            sPar.iParam[0] = iMcnRecvData[0];
            sPar.iParam[1] = S_f220McnMNErr_NOT_RUN;
            sPar.iParam[2] = iMcnRecvData[1];
            /* 3.00 予熱開始の場合は予熱種別をセット */
            if(sPar.iParam[0] == FALSE)
            {
                sPar.iParam[3] = iMcnRecvData[2];
            }else{
                sPar.iParam[3] = MCN_CLEAR;
            }
            iRet = mcnMotionMsgMake( &msgQId, iCommand, sPar.iParam ) ;
            break ;
        case SYS_MSG_RPOS   :                    /* ポリゴンモータ回転 */
        case SYS_MSG_EPLG   :
            break ;
/* V00R77で追加 */
        case SYS_MSG_RFSM   :                    /* フィニッシャビン移動 */
#ifdef DEBUG
    printf( "[mcnMN]:mcnActRscNotRun FinisherBin Move \n" ) ;
#endif
/* 03/02/19 追加 */
        case SYS_MSG_CRTN   :                    /* リボルバー回転 */
            cmnPrintf_mcnMN(2,"[mcnMN]: TRTN-CHK2 [mcnActRscNotRun]\n");
            cmnPrintf_mcnMN(2,"[mcnMN]: [0] = S_f220McnMNErr_NOT_RUN \n");
            iCommand = SYS_MSG_TRTN ;
            sPar.iParam[0] = S_f220McnMNErr_NOT_RUN;
            iRet = mcnMotionMsgMake( &msgQId, iCommand, sPar.iParam ) ;
            break ;

        default :
            break;
    }


    return iSts;
}
/*
(C)Copyright TOSHIBA Corporation 1996,1997. All rights Reserved
  NO:
        
  NAME:
        mcnActResourceErr
  TITLE:
        リソース動作中動作不可メッセージ作成処理
  MODULE:
        I32 mcnActResourceErr( I32 iMsgCom )
  INCLUDE:
        msgQLib.h / msgQSmLib.h
        comMain.h / comIErr.h
        mcnMNPar.h / mcnMNStr.h / mcnMNEngdef.h / mcnMNEvent.h / 
        mcnMNMsg.h / f220mcnMNErr.h / mcnMNEx.h
  PARAMETER:
        IN,I32,iMsgCom,4B,要求メッセージのメッセージＩＤ
  RETURNS:  
        I32,4B,OK:正常
  OUTLINE:  
        動作要求のメッセージＩＤに応じて、リソースが現在動作中である為に
        実行不可である意のメッセージを作成する。
        但し、ポリゴンモータ回転とフィニッシャビン移動はＡＣＫ・ＴＥＲＭが
        不要の為、処理は行わない。
        メッセージ送信先は動作要求のメッセージに設定されている返信先タスク。
  NOTES:    
        対象メッセージＩＤ：キャリッジ移動、原稿カウント、ＡＤＵ内用紙排出、
                            リカバリ、動作モード通知、予熱、
                            タイマ（ＭＣＰＵ電源変化）、ポリゴンモータ回転
                            ＩＤコード通知、フィニッシャビン移動
  HISTORY:  
        0.00 ,K.Iwasaki ,96/09/30 ,Original
        0.01 ,K.Iwasaki ,96/11/12 ,予熱・タイマ処理追加
        0.02 ,K.Iwasaki ,97/05/10 ,(V00R35)ＩＤコードカウント対応
        0.03 ,K.Iwasaki ,97/06/09 ,(V00R44)リカバリTRMにNOT READYリソース追加
        0.04 ,K.Iwasaki ,97/11/25 ,(V00R77)フィニッシャビン移動対応
        -----McKinley-------
        1.00 ,msei  ,2002/03/15, SYS_MSG_CTIN -> SYS_CTIN
                                 SYS_MSG_TTIN -> SYS_TTINに変更
        1.01,msei   ,2002/06/11, SYS_CTIN -> SYS_MSG_CTIN
                                 SYS_TTIN -> SYS_MSG_TTINに戻す
        1.02 ,msei      ,03/02/19 ,リボルバー回転対応
        1.03 ,msei      ,03/06/26 ,グローバル変数を命名規約に準拠させる
       ----RioGrande----
        2.00 ,K.Fujita  ,04/09/15 ,予熱種別追加（Amazonより復活）
*/
I32 mcnActResourceErr( I32 iMsgCom )
{
    I32 iSts ;
    I32 iRet ;
    I32 iCommand ;                               /* 送信メッセージコマンド */
    MSG_Q_ID  msgQId ;                           /* 送信先メッセージＩＤ */
    union MCN_MSGPARAM  sPar ;                   /* 送信メッセージパラメータ */


/* 
　iMsgCom別に、msgSendQIdに送信するメッセージを登録する
*/

    iSts = OK ;
    memset( &sPar.aParam, MCN_CLEAR, sizeof( union MCN_MSGPARAM ) ) ;
                                         /* パラメータエリアクリア */
    memcpy( &msgQId, &msgMcnSendQId, sizeof( MSG_Q_ID ) ) ;
                                         /* 送信メッセージキューＩＤセット */

    switch( iMsgCom )
    {
        case SYS_MSG_CCMO   :                    /* キャリッジ移動 */
            iCommand = SYS_MSG_TCMO ;
            sPar.iParam[0] = ERROR ;
            sPar.iParam[1] = S_f220McnMNErr_RESOURCE_ERROR;
            iRet = mcnMotionMsgMake( &msgQId, iCommand, sPar.iParam ) ;
            break ;
        case SYS_MSG_CPCT   :                    /* 原稿カウント指示 */
            iCommand = SYS_MSG_TPCT ;
            sPar.iParam[1] = S_f220McnMNErr_RESOURCE_ERROR;
            iRet = mcnMotionMsgMake( &msgQId, iCommand, sPar.iParam ) ;
            break ;
        case SYS_MSG_CAOP   :                    /* ＡＤＵ排出 */
            iCommand = SYS_MSG_TAOP ;
            sPar.iParam[0] = S_f220McnMNErr_RESOURCE_ERROR;
            iRet = mcnMotionMsgMake( &msgQId, iCommand, sPar.iParam ) ;
            break ;
/* 2002/03/14 変更 -> 2002/06/11 戻す */
        case SYS_MSG_CTIN   :                   /* 動作モード */ 
            iCommand = SYS_MSG_TTIN ;
            sPar.iParam[0] = S_f220McnMNErr_RESOURCE_ERROR;
            sPar.hParam[2] = iMcnRecvData[1] ;      /* モード */
            iRet = mcnMotionMsgMake( &msgQId, iCommand, sPar.iParam ) ;
/*            printf("##### mcnMN:TTIN f220Mcn ResErr #####\n"); *atsushi*/ 
            break ;
        case SYS_MSG_CMRC   :                    /* Ｗｕｐ */
        case SYS_MSG_C05R   :                    /* Test Copy Test Print */
#ifdef DEBUG_MCNRCV
    printf("[mcnMN]: TF02-CHK2 [mcnActResourceErr]\n");
    printf("[mcnMN]: [0] = S_f220McnMNErr_RESOURCE_ERROR \n");
#endif
            iCommand = SYS_MSG_TMRC ;
            sPar.iParam[0] = S_f220McnMNErr_RESOURCE_ERROR;
            sPar.hParam[2] = iMcnRecvData[0];
            sPar.hParam[3] = iMcnRecvData[1];
            sPar.hParam[4] = iMcnRecvData[2];
/* V00R44で追加 */
            sPar.hParam[5] = MCN_RCV_RSC_NON;
            iRet = mcnMotionMsgMake( &msgQId, iCommand, sPar.iParam ) ;
            break ;
        case SYS_MSG_CONF   :                    /* MCPU電源 */
            iCommand = SYS_MSG_TONF ;
            sPar.iParam[0] = iMcnRecvData[0];
/* 000525 siramura IH制御用メッセージパラメータ追加 */
            sPar.iParam[1] = iMcnRecvData[1];
            sPar.iParam[2] = S_f220McnMNErr_RESOURCE_ERROR; /* 00525 [1] -> [2] */
            iRet = mcnMotionMsgMake( &msgQId, iCommand, sPar.iParam ) ;
            break ;
        case SYS_MSG_CAPS   :                    /* 予熱動作 */
            iCommand = SYS_MSG_TAPS ;
            sPar.iParam[0] = iMcnRecvData[0];
            sPar.iParam[1] = S_f220McnMNErr_RESOURCE_ERROR;
            sPar.iParam[2] = iMcnRecvData[1];
            /* 2.00 予熱開始の場合は予熱種別をセット */
            if(sPar.iParam[0] == FALSE)
            {
                sPar.iParam[3] = iMcnRecvData[2];
            }else{
                sPar.iParam[3] = MCN_CLEAR;
            }
            iRet = mcnMotionMsgMake( &msgQId, iCommand, sPar.iParam ) ;
            break ;
        case SYS_MSG_RPOS   :                    /* ポリゴンモータ回転 */
        case SYS_MSG_EPLG   :
            break ;
        case SYS_MSG_CIDN   :                    /* ＩＤコード通知 */
            iCommand = SYS_MSG_TIDN ;
            sPar.iParam[0] = S_f220McnMNErr_RESOURCE_ERROR;
            iRet = mcnMotionMsgMake( &msgQId, iCommand, sPar.iParam ) ;
            break ;
/* V00R77で追加 */
        case SYS_MSG_RFSM   :                    /* フィニッシャビン移動 */
#ifdef DEBUG
    printf( "[mcnMN]:mcnActResourceErr FinisherBin Move \n" ) ;
#endif
            break ;
/*  03/02/19 追加 */
        case SYS_MSG_CRTN   :                    /* リボルバー回転指示 */
            cmnPrintf_mcnMN(2,"[mcnMN]: TRTN-CHK [mcnActResourceErr]\n");
            cmnPrintf_mcnMN(2,"[mcnMN]: [0] = S_f220McnMNErr_RESOURCE_ERROR \n");
            iCommand = SYS_MSG_TRTN ;
            sPar.iParam[0] = S_f220McnMNErr_RESOURCE_ERROR;
            iRet = mcnMotionMsgMake( &msgQId, iCommand, sPar.iParam ) ;
            break ;
        default :
            break;
    }


    return iSts;
}
/*
(C)Copyright TOSHIBA Corporation 1996. All rights Reserved
  NO:
        
  NAME:
        mcnActParResource
  TITLE:
        要求リソースエラーメッセージ作成処理
  MODULE:
        I32 mcnActParResource( I32 iMsgCom )
  INCLUDE:
        msgQLib.h / msgQSmLib.h
        comMain.h / comIErr.h
        mcnMNPar.h / mcnMNStr.h / mcnMNEngdef.h / mcnMNEvent.h / 
        mcnMNMsg.h / f220mcnMNErr.h / mcnMNEx.h
  PARAMETER:
        IN,I32,iMsgCom,4B,要求メッセージのメッセージＩＤ
  RETURNS:  
        I32,4B,OK:正常
  OUTLINE:  
        動作要求のメッセージＩＤに応じて、要求メッセージのパラメータの
        リソース指定がエラーである意のメッセージを作成する。
        メッセージ送信先は動作要求のメッセージに設定されている返信先タスク。
  NOTES:    
        対象メッセージＩＤ：リカバリ、予熱、
  HISTORY:  
        0.00 ,K.Iwasaki ,96/09/30 ,Original
        0.01 ,K.Iwasaki ,96/11/12 ,予熱処理追加
       ----McKinley----
        1.00 ,msei      ,03/06/26 ,グローバル変数を命名規約に準拠させる
*/
I32 mcnActParResource( I32 iMsgCom )
{
    I32 iSts ;
    I32 iRet ;
    /*  I32 iCommand ;   送信メッセージコマンド */
    MSG_Q_ID  msgQId ;                           /* 送信先メッセージＩＤ */
    union MCN_MSGPARAM  sPar ;                   /* 送信メッセージパラメータ */


/* 
　iMsgCom別に、msgSendQIdに送信するメッセージを登録する
*/

    iSts = OK ;
    memset( &sPar.aParam, MCN_CLEAR, sizeof( union MCN_MSGPARAM ) ) ;
                                         /* パラメータエリアクリア */
    memcpy( &msgQId, &msgMcnSendQId, sizeof( MSG_Q_ID ) ) ;
                                         /* 送信メッセージキューＩＤセット */

    switch( iMsgCom )
    {
        case SYS_MSG_CMRC   :                    /* Ｗｕｐ */
        case SYS_MSG_C05R   :                    /* Test Copy Test Print */
            iRet = mcnComErrMsgMake( &msgQId, iMsgCom,
                                     (I32)S_f220McnMNErr_PARAMETER_ERR ) ;
            break ;
        case SYS_MSG_CAPS   :                    /* 予熱動作 */
            iRet = mcnComErrMsgMake( &msgQId, iMsgCom,
                                     (I32)S_f220McnMNErr_PARAMETER_ERR ) ;
            break ;
        default :
            break;
    }


    return iSts;
}
/*
(C)Copyright TOSHIBA Corporation 1996. All rights Reserved
  NO:
        
  NAME:
        mcnActParRMode
  TITLE:
        要求リカバリモードエラーメッセージ作成処理
  MODULE:
        I32 mcnActParRMode( I32 iMsgCom )
  INCLUDE:
        msgQLib.h / msgQSmLib.h
        comMain.h / comIErr.h
        mcnMNPar.h / mcnMNStr.h / mcnMNEngdef.h / mcnMNEvent.h / 
        mcnMNMsg.h / f220mcnMNErr.h / mcnMNEx.h
  PARAMETER:
        IN,I32,iMsgCom,4B,要求メッセージのメッセージＩＤ
  RETURNS:  
        I32,4B,OK:正常
  OUTLINE:  
        動作要求のメッセージＩＤに応じて、要求メッセージのパラメータの
        リカバリモード指定がエラーである意のメッセージを作成する。
        メッセージ送信先は動作要求のメッセージに設定されている返信先タスク。
  NOTES:    
        対象メッセージＩＤ：リカバリ
  HISTORY:  
        0.00 ,K.Iwasaki ,96/09/30 ,Original
       ----McKinley----
        1.00 ,msei      ,03/06/26 ,グローバル変数を命名規約に準拠させる
*/
I32 mcnActParRMode( I32 iMsgCom )
{
    I32 iSts ;
    I32 iRet ;
    /* I32 iCommand ;     送信メッセージコマンド */
    MSG_Q_ID  msgQId ;                           /* 送信先メッセージＩＤ */
    union MCN_MSGPARAM  sPar ;                   /* 送信メッセージパラメータ */


/* 
　iMsgCom別に、msgSendQIdに送信するメッセージを登録する
*/

    iSts = OK ;
    memset( &sPar.aParam, MCN_CLEAR, sizeof( union MCN_MSGPARAM ) ) ;
                                         /* パラメータエリアクリア */
    memcpy( &msgQId, &msgMcnSendQId, sizeof( MSG_Q_ID ) ) ;
                                         /* 送信メッセージキューＩＤセット */

    switch( iMsgCom )
    {
        case SYS_MSG_CMRC   :                    /* Ｗｕｐ */
        case SYS_MSG_C05R   :
            iRet = mcnComErrMsgMake( &msgQId, iMsgCom,
                                     (I32)S_f220McnMNErr_PARAMETER_ERR ) ;
            break ;
        default :
            break;
    }


    return iSts;
}
/*
(C)Copyright TOSHIBA Corporation 1996. All rights Reserved
  NO:
        
  NAME:
        mcnActParMMode
  TITLE:
        要求動作モードエラーメッセージ作成処理
  MODULE:
        I32 mcnActParMMode( I32 iMsgCom )
  INCLUDE:
        msgQLib.h / msgQSmLib.h
        comMain.h / comIErr.h
        mcnMNPar.h / mcnMNStr.h / mcnMNEngdef.h / mcnMNEvent.h / 
        mcnMNMsg.h / f220mcnMNErr.h / mcnMNEx.h
  PARAMETER:
        IN,I32,iMsgCom,4B,要求メッセージのメッセージＩＤ
  RETURNS:  
        I32,4B,OK:正常
  OUTLINE:  
        動作要求のメッセージＩＤに応じて、要求メッセージのパラメータの
        動作モード指定がエラーである意のメッセージを作成する。
        メッセージ送信先は動作要求のメッセージに設定されている返信先タスク。
  NOTES:    
        対象メッセージＩＤ：動作モード通知
  HISTORY:  
        0.00 ,K.Iwasaki ,96/09/30 ,Original
		1.00 ,msei	,2002/03/15, SYS_MSG_CTIN -> SYS_CTINに変更
        1.01 ,msei  ,2002/06/11, SYS_CTIN -> SYS_MSG_CTIN に戻す
        1.02 ,msei      ,03/06/26 ,グローバル変数を命名規約に準拠させる

*/
I32 mcnActParMMode( I32 iMsgCom )
{
    I32 iSts ;
    I32 iRet ;
    /* I32 iCommand ;    送信メッセージコマンド */
    I32 iCommand ;	
    MSG_Q_ID  msgQId ;                           /* 送信先メッセージＩＤ */
    union MCN_MSGPARAM  sPar ;                   /* 送信メッセージパラメータ */


/* 
　iMsgCom別に、msgSendQIdに送信するメッセージを登録する
*/

    iSts = OK ;
    memset( &sPar.aParam, MCN_CLEAR, sizeof( union MCN_MSGPARAM ) ) ;
                                         /* パラメータエリアクリア */
    memcpy( &msgQId, &msgMcnSendQId, sizeof( MSG_Q_ID ) ) ;
                                         /* 送信メッセージキューＩＤセット */

    switch( iMsgCom )
    {
/* 2002/03/14 追加 */
        case SYS_MSG_CTIN   :                    /* 初期化 */
            iRet = mcnComErrMsgMake( &msgQId, iMsgCom,
                                     (I32)S_f220McnMNErr_PARAMETER_ERR ) ;
	/* sending TTIN as it is needed in boot mode change case */
	     iCommand = SYS_MSG_TTIN ;
            sPar.iParam[0] = S_f220McnMNErr_PARAMETER_ERR;
            sPar.hParam[2] = iMcnRecvData[1] ;      /* モード */
            iRet = mcnMotionMsgMake( &msgQId, iCommand, sPar.iParam ) ;
		
            break ;
        default :
            break;
    }


    return iSts;
}
/*
(C)Copyright TOSHIBA Corporation 1996. All rights Reserved
  NO:
        
  NAME:
        mcnActParOnOff
  TITLE:
        要求ＯＮ／ＯＦＦエラーメッセージ作成処理
  MODULE:
        I32 mcnActParOnOff( I32 iMsgCom )
  INCLUDE:
        msgQLib.h / msgQSmLib.h
        comMain.h / comIErr.h
        mcnMNPar.h / mcnMNStr.h / mcnMNEngdef.h / mcnMNEvent.h / 
        mcnMNMsg.h / f220mcnMNErr.h / mcnMNEx.h
  PARAMETER:
        IN,I32,iMsgCom,4B,要求メッセージのメッセージＩＤ
  RETURNS:  
        I32,4B,OK:正常
  OUTLINE:  
        動作要求のメッセージＩＤに応じて、要求メッセージのパラメータの
        ＯＮ／ＯＦＦに関する指定がエラーである意のメッセージを作成する。
        メッセージ送信先は動作要求のメッセージに設定されている返信先タスク。
  NOTES:    
        対象メッセージＩＤ：リカバリ、予熱、タイマ（ＭＣＰＵ電源変化）、
                            ポリゴンモータ回転
  HISTORY:  
        0.00 ,K.Iwasaki ,96/09/30 ,Original
        0.01 ,K.Iwasaki ,96/11/12 ,予熱・タイマ処理追加
       ----McKinley----
        1.00 ,msei      ,03/06/26 ,グローバル変数を命名規約に準拠させる
*/
I32 mcnActParOnOff( I32 iMsgCom )
{
    I32 iSts ;
    I32 iRet ;
    MSG_Q_ID  msgQId ;                           /* 送信先メッセージＩＤ */
    union MCN_MSGPARAM  sPar ;                   /* 送信メッセージパラメータ */


/* 
　iMsgCom別に、msgSendQIdに送信するメッセージを登録する
*/

    iSts = OK ;
    memset( &sPar.aParam, MCN_CLEAR, sizeof( union MCN_MSGPARAM ) ) ;
                                         /* パラメータエリアクリア */
    memcpy( &msgQId, &msgMcnSendQId, sizeof( MSG_Q_ID ) ) ;
                                         /* 送信メッセージキューＩＤセット */

    switch( iMsgCom )
    {
        case SYS_MSG_CMRC   :                    /* Ｗｕｐ */
        case SYS_MSG_C05R   :                    /* Test Copy Test Print */
            iRet = mcnComErrMsgMake( &msgQId, iMsgCom,
                                     (I32)S_f220McnMNErr_PARAMETER_ERR ) ;
            break ;
        case SYS_MSG_CONF   :                    /* タイマ動作 */
            iRet = mcnComErrMsgMake( &msgQId, iMsgCom,
                                     (I32)S_f220McnMNErr_PARAMETER_ERR ) ;
            break;
        case SYS_MSG_CAPS   :                    /* 予熱動作 */
            iRet = mcnComErrMsgMake( &msgQId, iMsgCom,
                                     (I32)S_f220McnMNErr_PARAMETER_ERR ) ;
            break;
        case SYS_MSG_RPOS   :                    /* ポリゴンモータ回転要求 */
        case SYS_MSG_EPLG   :
            iRet = mcnComErrMsgMake( &msgQId, iMsgCom,
                                     (I32)S_f220McnMNErr_PARAMETER_ERR ) ;
            break;
        default :
            break;
    }


    return iSts;
}
/*
(C)Copyright TOSHIBA Corporation 1996,1997. All rights Reserved
  NO:
        
  NAME:
        mcnActMcpuPwrOff
  TITLE:
        ＭＣＰＵ電源ＯＦＦ中動作不可メッセージ作成処理
  MODULE:
        I32 mcnActMcpuPwrOff( I32 iMsgCom )
  INCLUDE:
        msgQLib.h / msgQSmLib.h
        comMain.h / comIErr.h
        mcnMNPar.h / mcnMNStr.h / mcnMNEngdef.h / mcnMNEvent.h / 
        mcnMNMsg.h / f220mcnMNErr.h / mcnMNEx.h
  PARAMETER:
        IN,I32,iMsgCom,4B,要求メッセージのメッセージＩＤ
  RETURNS:  
        I32,4B,OK:正常
  OUTLINE:  
        動作要求のメッセージＩＤに応じて、ＭＣＰＵの電源がＯＦＦ中である為に
        実行不可である意のメッセージを作成する。
        但し、ポリゴンモータ回転とフィニッシャビン移動はＡＣＫ・ＴＥＲＭが
        不要の為、処理は行わない。
        メッセージ送信先は動作要求のメッセージに設定されている返信先タスク。
  NOTES:    
        対象メッセージＩＤ：キャリッジ移動、原稿カウント、ＡＤＵ内用紙排出、
                            リカバリ、動作モード通知、予熱、ポリゴンモータ回転
                            ＩＤコード通知、フィニッシャビン移動
  HISTORY:  
        0.00 ,K.Iwasaki ,96/11/12 ,Original
        0.01 ,K.Iwasaki ,97/05/10 ,(V00R35)ＩＤコードカウント対応
        0.02 ,K.Iwasaki ,97/06/09 ,(V00R44)リカバリTRMにNOT READYリソース追加
        0.03 ,K.Iwasaki ,97/11/25 ,(V00R77)フィニッシャビン移動対応
        -----McKinley-------
        1.00 ,msei  ,2002/03/14,   SYS_MSG_CTIN -> SYS_CTIN
                                   SYS_MSG_TTIN -> SYS_TTINに変更

        1.01 ,msei  ,2002/06/11,   SYS_CTIN -> SYS_MSG_CTIN
                                   SYS_TTIN -> SYS_MSG_TTINに戻す
        1.02 ,msei      ,03/02/19 ,リボルバー回転対応
        1.03 ,msei      ,03/06/26 ,グローバル変数を命名規約に準拠させる
       ----RioGrande----
        2.00 ,K.Fujita  ,04/09/15 ,予熱種別追加（Amazonより復活）
*/
I32 mcnActMcpuPwrOff( I32 iMsgCom )
{
    I32 iSts ;
    I32 iRet ;
    I32 iCommand ;                               /* 送信メッセージコマンド */
    MSG_Q_ID  msgQId ;                           /* 送信先メッセージＩＤ */
    union MCN_MSGPARAM  sPar ;                   /* 送信メッセージパラメータ */


/* 
　iMsgCom別に、msgSendQIdに送信するメッセージを登録する
*/

    iSts = OK ;
    memset( &sPar.aParam, MCN_CLEAR, sizeof( union MCN_MSGPARAM ) ) ;
                                         /* パラメータエリアクリア */
    memcpy( &msgQId, &msgMcnSendQId, sizeof( MSG_Q_ID ) ) ;
                                         /* 送信メッセージキューＩＤセット */

    switch( iMsgCom )
    {
/* 2002/03/14 変更 -> 2002/06/11 戻す*/
        case SYS_MSG_CTIN   :                    /* 初期化指示 */
            iCommand = SYS_MSG_TTIN ;
            sPar.iParam[0] = S_f220McnMNErr_MCPU_OFF;
            sPar.hParam[2] = iMcnRecvData[1];
            iRet = mcnMotionMsgMake( &msgQId, iCommand, sPar.iParam ) ;
/*            printf("##### mcnMN:TTIN f220Mcn McpuErr #####\n"); *atsushi*/ 
            break ;
        case SYS_MSG_CMRC   :                    /* Ｗｕｐ */
        case SYS_MSG_C05R   :
#ifdef DEBUG_MCNRCV
    printf("[mcnMN]: TF02-CHK3 [mcnActMcpuPwrOff]\n");
    printf("[mcnMN]: [0] = S_f220McnMNErr_MCPU_OFF \n");
#endif
            iCommand = SYS_MSG_TMRC ;
            sPar.iParam[0] = S_f220McnMNErr_MCPU_OFF;
            sPar.hParam[2] = iMcnRecvData[0];
            sPar.hParam[3] = iMcnRecvData[1];
            sPar.hParam[4] = iMcnRecvData[2];
/* V00R44で追加 */
            sPar.hParam[5] = MCN_RCV_RSC_NON;
            iRet = mcnMotionMsgMake( &msgQId, iCommand, sPar.iParam ) ;
            break ;
        case SYS_MSG_CAPS   :                    /* 予熱動作 */
            iCommand = SYS_MSG_TAPS ;
            sPar.iParam[0] = iMcnRecvData[0];
            sPar.iParam[1] = S_f220McnMNErr_MCPU_OFF;
            sPar.iParam[2] = iMcnRecvData[1];
            /* 2.00 予熱開始の場合は予熱種別をセット */
            if(sPar.iParam[0] == FALSE)
            {
                sPar.iParam[3] = iMcnRecvData[2];
            }else{
                sPar.iParam[3] = MCN_CLEAR;
            }
           iRet = mcnMotionMsgMake( &msgQId, iCommand, sPar.iParam ) ;
            break ;
        case SYS_MSG_CCMO   :                    /* キャリッジ移動 */
            iCommand = SYS_MSG_TCMO ;
            sPar.iParam[0] = ERROR ;
            sPar.iParam[1] = S_f220McnMNErr_MCPU_OFF;
            iRet = mcnMotionMsgMake( &msgQId, iCommand, sPar.iParam ) ;
            break ;
        case SYS_MSG_CPCT   :                    /* 原稿カウント指示 */
            iCommand = SYS_MSG_TPCT ;
            sPar.iParam[1] = S_f220McnMNErr_MCPU_OFF;
            iRet = mcnMotionMsgMake( &msgQId, iCommand, sPar.iParam ) ;
            break ;
        case SYS_MSG_CAOP   :                    /* ＡＤＵ排出 */
            iCommand = SYS_MSG_TAOP ;
            sPar.iParam[0] = S_f220McnMNErr_MCPU_OFF;
            iRet = mcnMotionMsgMake( &msgQId, iCommand, sPar.iParam ) ;
            break ;
        case SYS_MSG_RPOS   :                    /* ポリゴンモータ回転要求 */
        case SYS_MSG_EPLG   :
            break;
        case SYS_MSG_CIDN   :                    /* ＩＤコード通知 */
            iCommand = SYS_MSG_TIDN ;
            sPar.iParam[0] = S_f220McnMNErr_MCPU_OFF;
            iRet = mcnMotionMsgMake( &msgQId, iCommand, sPar.iParam ) ;
            break ;
/* V00R77で追加 */
        case SYS_MSG_RFSM   :                    /* フィニッシャビン移動 */
#ifdef DEBUG
    printf( "[mcnMN]:mcnActMcpuPwrOff FinisherBin Move \n" ) ;
#endif
            break ;
/* 03/02/19 追加 */
        case SYS_MSG_CRTN   :                    /* リボルバー回転 */
            cmnPrintf_mcnMN(2,"[mcnMN]: TRTN-CHK3 [mcnActMcpuPwrOff]\n");
            cmnPrintf_mcnMN(2,"[mcnMN]: [0] = S_f220McnMNErr_MCPU_OFF \n");
            iCommand = SYS_MSG_TRTN ;
            sPar.iParam[0] = S_f220McnMNErr_MCPU_OFF;
            iRet = mcnMotionMsgMake( &msgQId, iCommand, sPar.iParam ) ;
            break ;

        default :
            break;
    }


    return iSts;
}
/*
(C)Copyright TOSHIBA Corporation 1996,1997. All rights Reserved
  NO:
        
  NAME:
        mcnActRscESavOn
  TITLE:
        リソース予熱中動作不可メッセージ作成処理
  MODULE:
        I32 mcnActRscESavOn( I32 iMsgCom )
  INCLUDE:
        msgQLib.h / msgQSmLib.h
        comMain.h / comIErr.h
        mcnMNPar.h / mcnMNStr.h / mcnMNEngdef.h / mcnMNEvent.h / 
        mcnMNMsg.h / f220mcnMNErr.h / mcnMNEx.h
  PARAMETER:
        IN,I32,iMsgCom,4B,要求メッセージのメッセージＩＤ
  RETURNS:  
        I32,4B,OK:正常
  OUTLINE:  
        動作要求のメッセージＩＤに応じて、リソースが現在予熱中である為に
        実行不可である意のメッセージを作成する。
        但し、ポリゴンモータ回転とフィニッシャビン移動はＡＣＫ・ＴＥＲＭが
        不要の為、処理は行わない。
        メッセージ送信先は動作要求のメッセージに設定されている返信先タスク。
  NOTES:    
        対象メッセージＩＤ：キャリッジ移動、原稿カウント、ＡＤＵ内用紙排出、
                            リカバリ、動作モード通知、ポリゴンモータ回転
                            ＩＤコード通知、フィニッシャビン移動
                            リボルバー回転
  HISTORY:  
        0.00 ,K.Iwasaki ,96/11/12 ,Original
        0.01 ,K.Iwasaki ,97/05/10 ,(V00R35)ＩＤコードカウント対応
        0.02 ,K.Iwasaki ,97/06/09 ,(V00R44)リカバリTRMにNOT READYリソース追加
        0.03 ,K.Iwasaki ,97/11/25 ,(V00R77)フィニッシャビン移動対応
        -----McKinley-------
        1.00 ,msei    ,2002/03/12, SYS_MSG_CTIN -> SYS_CTIN
                                 SYS_MSG_TTIN -> SYS_TTINに変更

        1.01 ,msei    ,2002/06/11, SYS_CTIN -> SYS_MSG_CTIN
                                 SYS_TTIN -> SYS_MSG_TTINにもどす
        1.02 ,msei      ,03/02/19 ,リボルバー回転対応
        1.03 ,msei      ,03/06/26 ,グローバル変数を命名規約に準拠させる
*/
I32 mcnActRscESavOn( I32 iMsgCom )
{
    I32 iSts ;
    I32 iRet ;
    I32 iCommand ;                               /* 送信メッセージコマンド */
    MSG_Q_ID  msgQId ;                           /* 送信先メッセージＩＤ */
    union MCN_MSGPARAM  sPar ;                   /* 送信メッセージパラメータ */


/* 
　iMsgCom別に、msgSendQIdに送信するメッセージを登録する
*/

    iSts = OK ;
    memset( &sPar.aParam, MCN_CLEAR, sizeof( union MCN_MSGPARAM ) ) ;
                                         /* パラメータエリアクリア */
    memcpy( &msgQId, &msgMcnSendQId, sizeof( MSG_Q_ID ) ) ;
                                         /* 送信メッセージキューＩＤセット */

    switch( iMsgCom )
    {
/* 2002/03/14 変更 -> 2002/06/11 戻す*/
        case SYS_MSG_CTIN   :                    /* 初期化指示 */
            iCommand = SYS_MSG_TTIN ;
            sPar.iParam[0] = S_f220McnMNErr_RESOURCE_ESAV;
            sPar.hParam[2] = iMcnRecvData[1];
            iRet = mcnMotionMsgMake( &msgQId, iCommand, sPar.iParam ) ;
/*            printf("##### mcnMN:TTIN f220Mcn EsavErr #####\n");*atsushi*/ 
            break ;
        case SYS_MSG_CMRC   :                    /* Ｗｕｐ */
        case SYS_MSG_C05R   :                    /* Test Copy Test Print */
#ifdef DEBUG_MCNRCV
    printf("[mcnMN]: TF02-CHK4 [mcnActRscESavOn]\n");
    printf("[mcnMN]: [0] = S_f220McnMNErr_RESOURCE_ESAV \n");
#endif
            iCommand = SYS_MSG_TMRC ;
            sPar.iParam[0] = S_f220McnMNErr_RESOURCE_ESAV;
            sPar.hParam[2] = iMcnRecvData[0];
            sPar.hParam[3] = iMcnRecvData[1];
            sPar.hParam[4] = iMcnRecvData[2];
/* V00R44で追加 */
            sPar.hParam[5] = MCN_RCV_RSC_NON;
            iRet = mcnMotionMsgMake( &msgQId, iCommand, sPar.iParam ) ;
            break ;
        case SYS_MSG_CCMO   :                    /* キャリッジ移動 */
            iCommand = SYS_MSG_TCMO ;
            sPar.iParam[0] = ERROR ;
            sPar.iParam[1] = S_f220McnMNErr_RESOURCE_ESAV;
            iRet = mcnMotionMsgMake( &msgQId, iCommand, sPar.iParam ) ;
            break ;
        case SYS_MSG_CPCT   :                    /* 原稿カウント指示 */
            iCommand = SYS_MSG_TPCT ;
            sPar.iParam[1] = S_f220McnMNErr_RESOURCE_ESAV;
            iRet = mcnMotionMsgMake( &msgQId, iCommand, sPar.iParam ) ;
            break ;
        case SYS_MSG_CAOP   :                    /* ＡＤＵ排出 */
            iCommand = SYS_MSG_TAOP ;
            sPar.iParam[0] = S_f220McnMNErr_RESOURCE_ESAV;
            iRet = mcnMotionMsgMake( &msgQId, iCommand, sPar.iParam ) ;
            break ;
        case SYS_MSG_RPOS   :                    /* ポリゴンモータ回転要求 */
        case SYS_MSG_EPLG   :
            break;
        case SYS_MSG_CIDN   :                    /* ＩＤコード通知 */
            iCommand = SYS_MSG_TIDN ;
            sPar.iParam[0] = S_f220McnMNErr_RESOURCE_ESAV;
            iRet = mcnMotionMsgMake( &msgQId, iCommand, sPar.iParam ) ;
            break ;
/* V00R77で追加 */
        case SYS_MSG_RFSM   :                    /* フィニッシャビン移動 */
#ifdef DEBUG
    printf( "[mcnMN]:mcnActRscESavOn FinisherBin Move \n" ) ;
#endif
            break ;
        case SYS_MSG_CRTN   :                    /* リボルバー回転 */
            cmnPrintf_mcnMN(2,"[mcnMN]: TRTN-CHK4 [mcnActRscESavOn]\n");
            cmnPrintf_mcnMN(2,"[mcnMN]: [0] = S_f220McnMNErr_RESOURCE_ESAV \n");
            iCommand = SYS_MSG_TRTN ;
            sPar.iParam[0] = S_f220McnMNErr_RESOURCE_ESAV;
            iRet = mcnMotionMsgMake( &msgQId, iCommand, sPar.iParam ) ;
            break ;
        default :
            break;
    }


    return iSts;
}
/*
(C)Copyright TOSHIBA Corporation 1996. All rights Reserved
  NO:
        
  NAME:
        mcnActMcVersion
  TITLE:
        仕向取得失敗動作不可メッセージ作成処理
  MODULE:
        I32 mcnActMcVersion( I32 iMsgCom )
  INCLUDE:
        msgQLib.h / msgQSmLib.h
        comMain.h / comIErr.h
        mcnMNPar.h / mcnMNStr.h / mcnMNEngdef.h / mcnMNEvent.h / 
        mcnMNMsg.h / f220mcnMNErr.h / mcnMNEx.h
  PARAMETER:
        IN,I32,iMsgCom,4B,要求メッセージのメッセージＩＤ
  RETURNS:  
        I32,4B,OK:正常
  OUTLINE:  
        動作要求のメッセージＩＤに応じて、仕向の取得に失敗した為に
        実行不可である意のメッセージを作成する。
        メッセージ送信先は動作要求のメッセージに設定されている返信先タスク。
  NOTES:    
        対象メッセージＩＤ：動作モード通知
  HISTORY:  
        0.00 ,K.Iwasaki ,96/09/30 ,Original
        -----McKinley-------
        1.00 ,msei  ,2002/03/12, SYS_MSG_CTIN -> SYS_CTIN
                                 SYS_MSG_TTIN -> SYS_TTINに変更
        1.01 ,msei  ,2002/06/11, SYS_CTIN -> SYS_MSG_CTIN
                                 SYS_TTIN -> SYS_MSG_TTIN に戻す
        1.02 ,msei      ,03/06/26 ,グローバル変数を命名規約に準拠させる
*/
I32 mcnActMcVersion( I32 iMsgCom )
{
    I32 iSts ;
    I32 iRet ;
    I32 iCommand ;                               /* 送信メッセージコマンド */
    MSG_Q_ID  msgQId ;                           /* 送信先メッセージＩＤ */
    union MCN_MSGPARAM  sPar ;                   /* 送信メッセージパラメータ */


/* 
　iMsgCom別に、msgSendQIdに送信するメッセージを登録する
*/

    iSts = OK ;
    memset( &sPar.aParam, MCN_CLEAR, sizeof( union MCN_MSGPARAM ) ) ;
                                         /* パラメータエリアクリア */
    memcpy( &msgQId, &msgMcnSendQId, sizeof( MSG_Q_ID ) ) ;
                                         /* 送信メッセージキューＩＤセット */

    switch( iMsgCom )
    {
/* 2002/03/14 追加 */
        case SYS_MSG_CTIN   :                    /* 初期化指示 */
            iCommand = SYS_MSG_TTIN ;
            sPar.iParam[0] = S_f220McnMNErr_MCVERSION_ERR;
            sPar.hParam[2] = iMcnRecvData[1];
            iRet = mcnMotionMsgMake( &msgQId, iCommand, sPar.iParam ) ;
/*            printf("##### mcnMN:TTIN f220Mcn VerErr #####\n"); *atsushi*/ 
            break ;
        default :
            break;
    }


    return iSts;
}
/*
(C)Copyright TOSHIBA Corporation 1997. All rights Reserved
  NO:
        
  NAME:
        mcnActIDCountGet
  TITLE:
        ＩＤカウント取得失敗動作不可メッセージ作成処理
  MODULE:
        I32 mcnActIDCountGet( I32 iMsgCom )
  INCLUDE:
        msgQLib.h / msgQSmLib.h
        comMain.h / comIErr.h
        mcnMNPar.h / mcnMNStr.h / mcnMNEngdef.h / mcnMNEvent.h / 
        mcnMNMsg.h / f220mcnMNErr.h / mcnMNEx.h
  PARAMETER:
        IN,I32,iMsgCom,4B,要求メッセージのメッセージＩＤ
  RETURNS:  
        I32,4B,OK:正常
  OUTLINE:  
        動作要求のメッセージＩＤに応じて、ＩＤカウントの取得に失敗した為に
        実行不可である意のメッセージを作成する。
        メッセージ送信先は動作要求のメッセージに設定されている返信先タスク。
  NOTES:    
        対象メッセージＩＤ：ＩＤコード通知
  HISTORY:  
        0.00 ,K.Iwasaki ,97/05/10 ,(V00R35)Original
       ----McKinley----
        1.00 ,msei      ,03/06/26 ,グローバル変数を命名規約に準拠させる
*/
I32 mcnActIDCountGet( I32 iMsgCom )
{
    I32 iSts ;
    I32 iRet ;
    I32 iCommand ;                               /* 送信メッセージコマンド */
    MSG_Q_ID  msgQId ;                           /* 送信先メッセージＩＤ */
    union MCN_MSGPARAM  sPar ;                   /* 送信メッセージパラメータ */


/* 
　iMsgCom別に、msgSendQIdに送信するメッセージを登録する
*/

    iSts = OK ;
    memset( &sPar.aParam, MCN_CLEAR, sizeof( union MCN_MSGPARAM ) ) ;
                                         /* パラメータエリアクリア */
    memcpy( &msgQId, &msgMcnSendQId, sizeof( MSG_Q_ID ) ) ;
                                         /* 送信メッセージキューＩＤセット */

    switch( iMsgCom )
    {
        case SYS_MSG_CIDN   :                    /* ＩＤコード通知 */
            iCommand = SYS_MSG_TIDN ;
            sPar.iParam[0] = S_f220McnMNErr_IDCOUNT_ERR;
            iRet = mcnMotionMsgMake( &msgQId, iCommand, sPar.iParam ) ;
            break ;
        default :
            break;
    }


    return iSts;
}
/*
(C)Copyright TOSHIBA Corporation 1997. All rights Reserved
  NO:
        
  NAME:
        mcnActRecoverRscErr
  TITLE:
        リソース動作中リカバリ動作不可メッセージ作成処理
  MODULE:
        I32 mcnActRecoverRscErr( I32 iMsgCom )
  INCLUDE:
        msgQLib.h / msgQSmLib.h
        comMain.h / comIErr.h
        mcnMNPar.h / mcnMNStr.h / mcnMNEngdef.h / mcnMNEvent.h / 
        mcnMNMsg.h / f220mcnMNErr.h / mcnMNEx.h
  PARAMETER:
        IN,I32,iMsgCom,4B,要求メッセージのメッセージＩＤ
  RETURNS:  
        I32,4B,OK:正常
  OUTLINE:  
        動作要求のメッセージＩＤに応じて、リソースが現在動作中である為に
        実行不可である意のメッセージを作成する。
        リカバリ要求の場合は、NOT READYリソースに要求されたリソースを
        セットしてTRMメッセージを作成する。
        メッセージ送信先は動作要求のメッセージに設定されている返信先タスク。
  NOTES:    
        対象メッセージＩＤ：リカバリ
  HISTORY:  
        0.00 ,K.Iwasaki ,97/07/24 ,(V00R57)Original
       ----McKinley----
        1.00 ,msei      ,03/06/26 ,グローバル変数を命名規約に準拠させる
*/
I32 mcnActRecoverRscErr( I32 iMsgCom )
{
    I32 iSts ;
    I32 iRet ;
    I32 iCommand ;                               /* 送信メッセージコマンド */
    MSG_Q_ID  msgQId ;                           /* 送信先メッセージＩＤ */
    union MCN_MSGPARAM  sPar ;                   /* 送信メッセージパラメータ */


/* 
　iMsgCom別に、msgMcnSendQIdに送信するメッセージを登録する
*/

    iSts = OK ;
    memset( &sPar.aParam, MCN_CLEAR, sizeof( union MCN_MSGPARAM ) ) ;
                                         /* パラメータエリアクリア */
    memcpy( &msgQId, &msgMcnSendQId, sizeof( MSG_Q_ID ) ) ;
                                         /* 送信メッセージキューＩＤセット */

    switch( iMsgCom )
    {
        case SYS_MSG_CMRC   :                    /* Ｗｕｐ */
        case SYS_MSG_C05R   :                    /* Test Copy Test Print */
#ifdef DEBUG_MCNRCV
    printf("[mcnMN]: TF02-CHK5 [mcnActRecoverRscErr]\n");
    printf("[mcnMN]: [0] = S_f220McnMNErr_RESOURCE_ERROR \n");
    printf("[mcnMN]: [5] = %08x \n",iRecvData[2] );
#endif
            iCommand = SYS_MSG_TMRC ;
            sPar.iParam[0] = S_f220McnMNErr_RESOURCE_ERROR;
            sPar.hParam[2] = iMcnRecvData[0];       /* JobID  */
            sPar.hParam[3] = iMcnRecvData[1];       /* モード */
            sPar.hParam[4] = iMcnRecvData[2];       /* 要求リソース */
            sPar.hParam[5] = iMcnRecvData[2];       /* NOT READYリソース */
            iRet = mcnMotionMsgMake( &msgQId, iCommand, sPar.iParam ) ;
            break ;
        default :
            break;
    }


    return iSts;
}
};
