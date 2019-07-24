/*  (C)Copyright TOSHIBA TEC Corporation 1999,2000,2001. All rights Reserved  */
/* (C) Copyright TOSHIBA Corporation 1996,1997. All Rights Reserved
　TITLE:
    JobioMN.c　---　DPPC 入出力JOB管理タスク  JOB管理メッセージ処理ルーチン
  OUTLINE:
    F220システムＦ／Ｗ  入出力ＪＯＢ管理タスク
　HISTORY:
    1.1 Y.Matsuda   96/09/13    original

    2.1 yMatsuda    97/10/22    GDI印刷JOB開始受信時にデータ転送中フラグ解除していたのを削除
    3.1 E.Saka      97/10/24    jobUI表示更新対応
    4.1 E.Saka      97/10/28    GDI印刷ジョブ中断時、PMを一旦解放し、
                                再スタート時に再確保するように修正
    5.1 E.Saka      97/11/02    EXFAX,DSS印刷ジョブ中断時、PMを一旦解放し、
                                再スタート時に再確保するように修正
                                (#define IOMN_EXFAXMFPが必要)
    M1.1 (TJ)H.Takahashi 98/07/07 手置き逐次対応の為関数追加
    DM45/35用修正(based on MISSI V007.20)
    D1.1 T.Fujii    99/11/15    DM45/35 後追い逐次・並行動作・先行入力対応
    D1.2 T.Fujii    99/12/24    DM45/35 (後追い)逐次印刷対応
    D1.4 Y.Shimada  00/ 1/25    DM45/35 フォームストレージ変更：ＩｐＬｉｂ／逐次制御の組み込み
                                D1.4D parMN <-> HDD
    D1.6 T.Fujii    00/02/09    DM45/35 prnUI表示対応
    D1.2.1 T.Fujii  00/02/23    DM45/35 並行動作制御修正、サブジョブ中断処理修正、
                                        フィニッシャ初期化修正、逐次印刷修正
    D1.4.1 T.Fujii  00/02/29    DM45/35 デバッグ用parMNコピーを無効化
    D1.7.1 J.Ootani 00/03/01    DM45/35 SEINE FAX F-CODE対応(マージ)
    D1.8 Y.Shimada  00/03/09    DM45/35 印刷Ｑｕｅ１本化
    D1.10 Y.Shimada 00/04/05    DM45/35 CDJBのI/F変更
    D1.6.7 T.Fujii  00/04/10    DM45/35 ジョブ属性変更通知送信処理修正
    D1.6.8 T.Fujii  00/04/10    DM45/35 出力開始チェック仕様変更(入力枚数0に対応)
    D1.2.4 T.Fujii  00/04/10    DM45/35 サブジョブ中断・再開処理修正
    D1.2.5  T.Fujii 00/04/10    DM45/35 中止処理修正
    D1.2.4.1 T.Fujii 00/04/15   DM45/35 印刷中断・再開処理修正
    D1.7.9  J.Ootani 00/04/08   DM45/35 FAXリストレポート対応
    D1.7.10 J.Ootani 00/04/08   DM45/35 FAXダイレクト送信対応
    D1.7.12 J.Ootani 00/04/19   DM45/35 FAXリスト印刷不具合対応
    D1.13   T.Fujii  00/05/17   DM45/35 AJSTパラメータ拡張対応、逐次コピー中断要因変更、
                                        サブジョブ中止処理修正
    D1.9.3  J.Ootani 00/05/17   DM45/35 PrinterのみW-UP中の逐次開始対応
    D1.13.1 T.Fujii  00/05/20   DM45/35 サブジョブ中止処理修正/ソースコード整理
    D1.11   Y.Shimada 00/05/08  DM45/35 割り込み複写対応
    D1.14   T.Fujii   00/05/25  DM45/35 DSSスキャン対応
    D2.0    M.Fukuda  00/05/29  DM45/35 NOP_DEL  関数削除 iomnJobSuspendByMemoryFull2
    D2.0    M.Fukuda  00/05/29  DM45/35 NOP_DEL  関数削除 iomnFcodePrnStart
    D2.0    M.Fukuda  00/05/29  DM45/35 NOP_DEL  関数削除 iomnDssPrnJobStart
    D2.0    M.Fukuda  00/05/29  DM45/35 NOP_DEL  関数削除 iomnDirectFaxStartAck
    D2.0    M.Fukuda  00/05/29  DM45/35 NOP_DEL  関数削除 iomnDigJobStartAck
    D2.0    M.Fukuda  00/05/29  DM45/35 NOP_DEL  関数削除 iomnDssPrnJobStartAck
    D2.0    M.Fukuda  00/05/29  DM45/35 NOP_DEL  関数削除 iomnDssScnTJED
    D2.0    M.Fukuda  00/05/29  DM45/35 NOP_DEL  関数削除 iomnBPPCJobFinSts
    D2.0    M.Fukuda  00/05/29  DM45/35 NOP_DEL  関数削除 iomnDssScnJobFinSts
    D2.0    M.Fukuda  00/05/29  DM45/35 NOP_DEL  関数削除 iomnJobSuspendByMemoryFull
    D2.0    M.Fukuda  00/05/29  DM45/35 NOP_DEL  関数削除 iomnFaxMemFull
    D2.0    M.Fukuda  00/05/29  DM45/35 NOP_DEL  関数削除 iomnGetMsgQevent
    D2.0    M.Fukuda  00/05/29  DM45/35 NOP_DEL  関数削除 iomnItoA
    D2.0    M.Fukuda  00/05/29  DM45/35 NOP_DEL  関数削除 iomnJobUICjsu
    D2.0    M.Fukuda  00/05/29  DM45/35 NOP_DEL  関数削除 iomnDssPrivateAjsu
    D2.0    M.Fukuda  00/05/29  DM45/35 NOP_DEL  関数削除 iomnDssScnTjsu
    D2.0    M.Fukuda  00/05/29  DM45/35 NOP_DEL  関数削除 iomnDummyParMNorHdd
    D2.0    M.Fukuda  00/05/29  DM45/35 NOP_DEL  関数削除 iomnGetSubJobtype
    D2.0    M.Fukuda  00/05/29  DM45/35 NOP_DEL  関数削除 iomnGdiDataTransConfirm
                              (iomnRecieveCJDTの処理全面削除)
    D2.0    M.Fukuda  00/05/29  DM45/35 NOP_DEL  関数削除 iomnDssAcceptConfirm
    D2.0    M.Fukuda  00/05/29  DM45/35 NOP_DEL  関数削除 iomnSendPrintStartEvent
    D2.0    M.Fukuda  00/05/29  DM45/35 NOP_DEL  関数削除 iomnSendFaxScnJobAck
    D2.0    M.Fukuda  00/05/29  DM45/35 NOP_DEL  関数削除 iomnSendCjrcJob
    D2.0    M.Fukuda  00/05/29  DM45/35 NOP_DEL  関数削除 iomnGetMessageQIdToUI
    D2.0    M.Fukuda  00/05/29  DM45/35 NOP_DEL  関数削除 iomnGetRunningPrnJob
    D2.0    M.Fukuda  00/05/29  DM45/35 NOP_DEL  関数削除 iomnSendEPWN
    D2.0    M.Fukuda  00/05/29  DM45/35 NOP_DEL  関数削除 iomnGetPriUI
    D2.0    M.Fukuda  00/05/29  DM45/35 NOP_DEL  関数削除 iomnGetMsgQId
    D2.0    M.Fukuda  00/05/29  DM45/35 NOP_DEL  関数削除 iomnFcodePrnStartConfirm
    D2.0    M.Fukuda  00/05/29  DM45/35 NOP_DEL  関数削除 iomnStandbyPrnJob
    D2.0    M.Fukuda  00/05/29  DM45/35 NOP_DEL  関数削除 iomnSendTfpm
                              (iomnRecieveCFPMの処理削除)
    D2.1    T.Fujii   00/06/06  DM45/35 V004.60不具合対応
                                        parMN再設定タイミング変更
                                        フォーム印刷でHDD設定値の書き換えを削除
                                        TJSU受信時にEJED送信を追加
                                        原稿入力開始前のparMN原稿枚数クリア処理を追加
                                        ページ確定メッセージ送信対応
                                        D1.13.1によるサブジョブ中止処理を再修正
    D2.3    T.Fujii   00/06/06  DM45/35 PC-FAX対応
    D2.0.1  T.Fujii   00/06/06  DM45/35 NOP_DEL(ソースコード整理)
                                        if文,case文でのCOM_JB_BPPC,COM_JB_EXPPC1を削除
                                        puJobDelete()関数の呼び出しを削除
    D1.9.7  J.Ootani  00/06/06  DM45/35 逐次中断時ALLリカバリ対応
    D2.4    T.Fujii   00/06/15  DM45/35 prnEX遅延中断対応
    D2.4.1  T.Fujii   00/06/17  DM45/35 V004.70不具合対応
                                        AJSTパラメータ拡張対応修正
    D2.4.2  M.Fukuda  00/06/19  DM45/35 JOBENDイベント送信処理対応
    D2.4.3  M.Fukuda  00/06/19  DM45/35 V004.70不具合対応
                                        FAXリストレポート不具合修正
    D2.4.4  T.Fujii   00/06/20  DM45/35 V004.70不具合対応
                                        ジョブ終了後のJobIDクリア処理対応
    D2.4.8  T.Fujii   00/06/24  DM45/35 5Jobフルでフロント中断中になる不具合修正
    D2.5    Y.Shimada 00/06/15  DM45/35 リスト印刷(フロントジョブ)対応(フォーム印刷と共通化)
    D1.11.1 Y.Shimada 00/06/29  DM45/35 割り込み複写対応
    D2.8    T.Fujii   00/07/01  DM45/35 印刷系イベント送信処理修正
    D2.9    M.Fukuda  00/07/04  DM45/35 FAXリストレポート対応
    D2.10   M.Fukuda  00/07/04  DM45/35 JOBENDイベント送信処理対応
    D2.11   T.Fujii   00/07/04  DM45/35 用紙無し時のマシンリカバリ省略対応
    D2.0.2  T.Fujii   00/07/04  DM45/35 NOP_DEL(ソースコード整理)
                                        if文,case文でのCOM_JB_EXPPC1を削除
    D2.12   T.Fujii   00/07/05  DM45/35 メッセージパラメータ保存処理再修正
    D2.10.1 T.Fujii   00/07/08  DM45/35 JOBEND送信処理修正
    D2.5.2  Y.Shimada 00/07/10  DM45/35 リスト印刷(フロントジョブ)の複数ジョブ対応
    D2.10.2 T.Fujii   00/07/10  DM45/35 JOBEND送信処理修正
    D2.11.1 T.Fujii   00/07/11  DM45/35 マシンリカバリを省略するときの中断要因を追加
    D2.9.1  M.Fukuda  00/07/12  DM45/35 中止処理開始後のリストレポート印刷開始対応
    D2.9.2  M.Fukuda  00/07/12  DM45/35 FAXリストレポート対nclude    <usrLib.h>応
    D2.10.3 M.Fukuda  00/07/14  DM45/35 JOBEND送信処理修正（DFジャム発生対応）
    D2.9.3  M.Fukuda  00/07/19  DM45/35 FAXリストレポート対応（逐次制御テーブルクリア追加）
    D3.0              00/07/28  DM45/35 ソースコード整理
    D3.3    T.Fujii   00/08/08  DM45/35 MISSI V7.3以降の修正をDMにも反映
                                        ・原稿入力中止時に無条件でフォーム登録のファイル削除を
                                          行なう不具合修正(M8.1)
    D3.5    T.Fujii   00/08/08  DM45/35 再開CJSTに対するAck送信先修正
    D3.0.1  T.Fujii   00/08/09  DM45/35 ソースコード整理
                                        ・iomnOutputJobRestart()関数を無効化
    D3.7    T.Fujii   00/08/10  DM45/35 原稿枚数オーバー時の印刷中止処理対応
    D3.9.4  J.Oootani 00/08/18  DM45/35 LAN-P/DSS Sleep解除タイミング修正対応
    D3.10   T.Fujii   00/08/24  DM45/35 FAXリスト印刷処理修正
    D3.12   T.Fujii   00/09/02  DM45/35 ジョブ中断処理修正
    D3.13   T.Fujii   00/09/02  DM45/35 コーディングミス等修正
    D3.14   Y.Shimada 00/09/04  DM45/35 Log強化対応
    D3.12.1 T.Fujii   00/09/04  DM45/35 BG印刷ジョブ中断処理修正
    D3.14.1 T.Fujii   00/09/04  DM45/35 Log強化対応
    D3.11.2 Y.Shimada 00/09/07  DM45/35 中断中の割り込み全面見直し
    D3.12.2 T.Fujii   00/09/07  DM45/35 BG印刷ジョブ中断処理再修正、ソースコード整理
    D3.13.2 T.Fujii   00/09/07  DM45/35 TDJB送信処理抜け修正
    D3.16   M.Fukuda  00/09/12  DM45/35 JOB削除開始フラグ設定処理修正
    D3.19   T.Fujii   00/09/12  DM45/35 コーディングミス修正
    D3.20   T.Fujii   00/09/12  DM45/35 ECDN送信処理修正
    D3.21.1 T.Fujii   00/09/18  DM45/35 後追い逐次での部数更新処理修正
    D3.0.2  T.Fujii   00/09/18  DM45/35 NOP_DEL(ソースコード整理)
    D3.19.2 T.Fujii   00/09/18  DM45/35 AJSU送信処理修正
    D3.19.3 T.Fujii   00/09/18  DM45/35 ADDS送信処理修正
    D3.22   T.Fujii   00/09/19  DM45/35 従来逐次の後にhIHOFFフラグを壊す不具合修正
    D3.12.4 T.Fujii   00/09/25  DM45/35 BG印刷ジョブ中断処理修正
    D3.19.4 T.Fujii   00/09/25  DM45/35 AJSU送信処理再修正
    D3.21.3 T.Fujii   00/09/27  DM45/35 parMNコピー処理再修正
    D3.24   Y.Shimada 00/09/11  DM45/35 ＲＤＣ(FAX)対応
    D3.25   T.Fujii   00/09/27  DM45/35 データ転送中フラグ設定修正
    D3.27   T.Fujii   00/09/30  DM45/35 割り込みコピー中のJOB中止でJOBEND送信する不具合修正
    D3.24.2 Y.Shimada 00/10/02  DM45/35 ＲＤＣ(FAX)対応(非実行中のCFED/CRED受信対応)
    D3.28.2 J.Ootani  00/10/07  DM45/35 Sleep移行チェックはfb関数を使用するためhFaxConnectionを使用しない対応
    D3.31   T.Fujii   00/10/10  DM45/35 マシンリカバリ処理修正
    D3.33   T.Fujii   00/10/10  DM45/35 不要処理削除
    D3.40   T.Fujii   00/10/18  DM45/35 フロントUI取得関数を追加
    D3.41   M.Fukuda  00/10/19  DM45/35 ダイレクト送信中止処理不具合対応
    D3.41.1 T.Fujii   00/10/19  DM45/35 FAXダイレクト送信中止後、mcnUIへの画面変更処理追加
    D3.28.7 J.Ootani  00/10/19  DM45/35 電源ON処理を印刷Queチェック時に統一対応
    D3.31.3 T.Fujii   00/10/24  DM45/35 逐次コピー時もmcnUIを表示させる
    D3.31.4 T.Fujii   00/10/25  DM45/35 原稿入力中断/終了でmcnUIが表示されない不具合修正
    D3.40.1 T.Fujii   00/10/25  DM45/35 iomnLibGetFrontUIのエラー復帰先UI取得方法変更
    D4.1    T.Fujii   00/11/01  DM45/35 ジョブ削除でEC22送信しない不具合修正
    D4.2    T.Fujii   00/11/01  DM45/35 印刷ジョブCS中断で中断失敗を送信している不具合修正
    D4.4    T.Fujii   00/11/07  DM45/35 原稿入力が０ページの場合は印刷ジョブを削除する
    D4.7    T.Fujii   00/11/18  DM45/35 前ジョブでエラー終了するとジャム解除で印刷開始しなくなる
                                        不具合修正
    D4.4.1  T.Fujii   00/11/20  DM45/35 後追い逐次コピー入力終了時の印刷ジョブ削除処理修正
    D4.7.1  T.Fujii   00/11/20  DM45/35 プリンタW-UP中の2つ目の入力ジョブでCopyingロックになる
                                        不具合修正
    D4.7.2  T.Fujii   00/11/21  DM45/35 プリンタW-UP中の2つ目のFAX/DSS入力ジョブでロックする
                                        不具合修正
    D4.8    T.Fujii   00/11/21  DM45/35 05テストコピーでcpyUIにメッセージ送信する不具合修正
    D5.0    T.Fujii   00/12/12  DM45/35 CDJB多重動作対応
    D5.3    T.Fujii   01/03/30  DM45/35 割り込み複写ジョブ開始不可能時にEB46も送信するよう修正
    D5.3.1  T.Fujii   01/05/08  DM45/35 割り込み複写ジョブ開始不可能時のジョブ情報クリア処理削除
    D6.0    T.Fujii   01/10/20  DM45/35 TDJBを送信しない不具合を修正

    MCK1.1  M.Karasawa  02/06/11    McKinley    新メッセージ対応
    MCK1.5  M.Karasawa  02/07/25    McKinley    後追い逐次複写対応
    MCK1.5  M.Karasawa  02/07/26    McKinley    iomnChangeToMcnUI()関数使用箇所削除
                                                NOP_DEL iomnScnChangeUI()

    Visual Source Safe登録＞＞＞＞＞＞＞＞＞＞＞＞＞＞＞＞＞＞＞＞＞＞＞＞＞＞＞＞＞＞＞＞＞＞＞＞＞
    (Vxxx.yy_zz :   Vxxx.yy > 修正元ソースRev.      zz > 修正番号)
    V007.00_01  M.Karasawa  02/09/02    McKinley    入力中断時、出力の中断は上位からの指示待ち
    V007.00_02  M.Karasawa  02/09/02    McKinley    入力ジョブ中断指示受信時、出力の中断は上位からの指示待ち
    V007.00_03  M.Karasawa  02/09/02    McKinley    AJSU送信処理修正
    V007.00_04  M.Karasawa  02/09/02    McKinley    TJSU受信処理修正
    V008.t1_01  M.Karasawa  02/09/30    McKinley    印刷ジョブ登録時、入力が終わっている時の対策
    V008.00_03  M.Karasawa  02/10/23    McKinley    printf -> cmnPrintf_ioMN 置き換え修正
    V009.00_03  M.Karasawa  02/11/26    Mckinley    imEx.h削除
    V012.00_01  M.Karasawa  03/01/24    McKinley    V11暫定対応を正式に対応
    V012.00_07  M.Karasawa  03/01/24    McKinley    includeファイル追加(jcLib_Str.h、jcLib_Ext.h)
    V022.00_01  M.Karasawa  03/07/08    McKinley    グローバル変数名変更
                                                    msgQIdforCpyJob         ->> msgQIdioMNforCpyJob
    eB3-01      M.Taki      07/07/25    BP/MASH     eB3コンパイルオプション追加(BP_ENG,MASH_ENG)
    eB3-05      M.Taki      08/02/20    BP/MASH     画質維持中エラー中断処理対応
    eB3-07      N.Sato      09/05/18    LOIRE/AL    手差しガイド幅チェックエラー対応

    Dimensions登録＞＞＞＞＞＞＞＞＞＞＞＞＞＞＞＞＞＞＞＞＞＞＞＞＞＞＞＞＞＞＞＞＞＞＞＞＞＞＞＞＞
   【St.Helens/Mosel】
    (Vxxx.yy_zz :   Vxxx.yy > 修正元ソースRev.           zz > 修正番号)
    S2-ENG      K.Ishiakwa  13/10/08    St.Helens/Mosel  共通化対応											：S2		：COM#100.2
    S2-001      K.Ishikawa  12/10/22    St.Helens/Mosel  ODC_ERR_MOVE_CHG_STOP処理対応						：S2		：COM#100.2
    S2-002      K.Ishikawa  13/01/14    St.Helens/Mosel  EPEH中断での上位側へ中断が通知されない問題対応		：S2		：COM#100.2
    SM-001      K.Ishikawa  13/10/16    St.Helens/Mosel  warning削除の為、#include追加						：S2		：COM#100.2

  NOTES:
*/
#include    <errnoLib.h>

#if 0
//#include    <vxWorks.h>  /* commented vxWorks header file -- gangadhara - 080813 */
//#include    <semLib.h>
//#include    <msgQLib.h>
//#include    <sysLib.h>  /* commented vxWorks header file -- gangadhara - 080813 */
//#include    <taskLib.h>
#endif

#include    <time.h>

#include    <string.h>
#include    <stdlib.h>
#include    <stdio.h>
#include "v2linux_wrapper.h"
/*#include    <usrLib.h> */ /* commented vxWorks header file -- gangadhara - 080813 */
/*#include    <objLib.h> */ /* commented vxWorks header file -- gangadhara - 080813 */
#if 0   /* MCK1.1 新メッセージ対応  */
#include    <comCom.h>
#include    <comLib.h>
#include    <comMsg.h>
#include    <comPar.h>
#include    <comSize.h>
#include    <comValEx.h>
#else
#include    <comMain.h>
#endif
/*#include    <usrLib.h> */ /* commented vxWorks header file -- gangadhara - 080813 */
#include    <comIErr.h>
#include    <system.h>
#include    <cmnPar.h>
#include    <cmnStr.h>
#include    <cmnEx.h>
#if 0   /* MCK1.1 新メッセージ対応  */
#include    <mcnMNMsg.h>
#else
#include    <mcnMNCom.h>
#endif
#include    <f220engErr.h>
#include    <f220cpyExErr.h>
#if 0   /* ROM6 */
#include    <f220scnEXErr.h>
#else
#include    <scnEXErr.h>
#endif
#include    <f220prnEXErr.h>
#if 0   /* MCK1.1 新メッセージ対応  */
#include    "ioMNbuf.h"
#endif
#include    "ioMNpar.h"
#if 0   /* MCK1.1 新メッセージ対応  */
#include    "ioMNmsg.h"
#else
#include    "ioMNLocalmsg.h"
#endif
#include    "ioMNstr.h"
#include    "ioMNmfp.h"
#include    "f220ioMNErr.h"
#include    <parMNPar.h>
#include    <parMNStr.h>
#include    <parMNEx.h>
#include    <quePar.h>
#include    <queStr.h>
#include    <queEx.h>
#include    <jobPar.h>
#include    <jobStr.h>
#include    <jobEx.h>
#include    <f220jobErr.h>
#include    <fbPar.h>
#include    <fuPar.h>
#include    <pmPar.h>
#include    <flPar.h>
#include    <f220fuErr.h>
#include    <f220fbErr.h>
#include    <f220pmErr.h>
#include    <f220flErr.h>
#include    <fbStr.h>
#include    <fuStr.h>
#include    <pmStr.h>
#include    <flStr.h>
#include    <pmEx.h>
#include    <f220puErr.h>
#include    <puPar.h>
#include    <puStr.h>
#include    <f220spErr.h>
#include    <spEx.h>
#include    <spPar.h>
#include    <spStr.h>
#include    <fbEx.h>
#include    <lfxMNPar.h>
#include    <sysTBLEx.h>
/*#include    <imEx.h>*//* V009.00_03   */
#include    <puEx.h>
#include    <flEx.h>
#if 0   /* MCK1.1 新メッセージ対応  */
#include    <iomsgPar.h>
#include    <jobmsgPar.h>   /* MCK  */
#endif
#include    <comApl.h>      /* MCK1.1 新メッセージ対応  */
#include    <jcLib_Par.h>   /* MCK1.5   *//* --- 後追い逐次 --- */
#include    <jcLib_Str.h>   /* V012.00_07   *//* ADD    */
#include    <jcLib_Ext.h>   /* V012.00_07   *//* ADD    */
#include    <cmnUtyEx.h>    /* V008.00_03 ADD   */
#if 1 /* RioGrande マニュアル対応 */
#include    <sysMNLib.h>
#endif
/* HM新フィニッシャ対応 *//* START */
#include    <mcnTBLPar.h>
#include    <mcnTBLStr.h>
#include    <mcnTBLEx.h>
#include    <f220mcnTBLErr.h>
#include    <commchn.h>
#include    <ioExMNPar.h>/* 画質維持中エラー対応 *//* eB3-05 */
/* HM新フィニッシャ対応 *//* END */
#include    "ioMNPrnJoint.h"    /* PRINT_JOINT */
#include		"digUIPar.h"

#if defined(LOIRE2_ENG) || defined(AL2_ENG) || defined(WEISS_ENG) || defined(ECO_LOIRE2) /*EBX_DTFR_19655*/
	extern void mcnMNCheckSilentRebootStatus();
#endif
/*  JOB管理処理関数プロトタイプ宣言 */
STATUS  iomnDoJobManagement();
STATUS  iomnRecJobStart();                  /*  JOB開始指示受信処理 */
#if 0   /* MCK1.1 新メッセージ対応  引数追加    */
STATUS  iomnPPCJobStart(HI, HI, HI, HI);    /*  複写JOB開始処理 */
#else
STATUS  iomnPPCJobStart(HI, HI, HI, HI, HI);    /*  複写JOB開始処理 */
#endif
STATUS  iomnPrepareForPPC(HI, HI, HI, HI);  /*  複写開始準備処理    */
#if 0   /* MCK1.1 新メッセージ対応  引数追加    */
STATUS  iomnFaxScnJobStart(HI, HI, HI, HI); /*  FAXScnJOB開始処理   */
#else
STATUS  iomnFaxScnJobStart(HI, HI, HI, HI, HI); /*  FAXScnJOB開始処理   */
#endif
STATUS  iomnPrepareForFax(HI, HI, HI, HI);  /*  FAXJOB開始準備処理  */
#if 0   /* MCK1.1 新メッセージ対応  引数追加    */
STATUS  iomnLstFaxStart(HI, HI, HI, HI);    /*  Faxリスト／レポートJOB開始処理  */
#else
STATUS  iomnLstFaxStart(HI, HI, HI, HI, HI);    /*  Faxリスト／レポートJOB開始処理  */
#endif
STATUS  iomnRecieveEC44();                  /*  FAXﾀﾞｲﾚｸﾄ送信接続完了ｲﾍﾞﾝﾄ */
STATUS  iomnRecieveEvent(I32);              /*  FAXﾀﾞｲﾚｸﾄ送信ｲﾍﾞﾝﾄ */
#if 0   /* MCK1.1 新メッセージ対応  引数追加    */
STATUS  iomnListMakeJobFinSts(HI, HI, I32, I32, HI, HI);  /* リスト作成完了 */
STATUS  iomnPrnJobStart(HI, HI, HI, HI, HI);/*  GDIJOB開始メッセージ受信処理        */
#else
STATUS  iomnListMakeJobFinSts(HI, HI, I32, I32, HI, HI, HI);  /* リスト作成完了 */
STATUS  iomnPrnJobStart(HI, HI, HI, HI, HI, HI);/*  GDIJOB開始メッセージ受信処理        */
#endif
STATUS  iomnPrnJobCheckEntry(HI , HI);      /*  印刷ジョブＱｕｅ登録時のチェック処理*/
#if 0   /* MCK1.1 新メッセージ対応  引数追加    */
STATUS  iomnDssScnJobStart(HI, HI, HI, HI); /*  DSS入力JOB開始メッセージ受信処理    */
#else
STATUS  iomnDssScnJobStart(HI, HI, HI, HI, HI); /*  DSS入力JOB開始メッセージ受信処理    */
#endif
STATUS  iomnPrepareForIVSave(HI, HI, HI, HI);   /*  Form登録JOB開始準備処理 */
STATUS  iomnPrepareForIVPrn(HI, HI, HI, HI);    /*  Form登録JOB開始準備処理 */
STATUS  iomnPrepareUniqueProc(HI, HI, HI, HI, B *); /* リスト印刷／フォーム印刷の固有準備処理 */
STATUS  iomnMakeListFileName(HI, HI,B *f);  /* リストファイル名の取得 */
STATUS  iomnPrepareForSendDSS(HI, HI, HI, HI);  /*  Dss入力JOB開始準備処理  */
#if 0   /* MCK1.1 新メッセージ対応  引数追加    */
STATUS  iomnDirectFaxStart(HI, HI, HI, HI); /*  Faxダイレクト送信開始メッセージ受信処理 */
STATUS  iomnIVSaveJobStart(HI, HI, HI, HI); /*  フォーム登録JOB開始メッセージ受信処理   */
STATUS  iomnIVPrnJobStart(HI, HI, HI, HI);  /*  フォーム確認JOB開始メッセージ受信処理   */
#else
STATUS  iomnDirectFaxStart(HI, HI, HI, HI, HI); /*  Faxダイレクト送信開始メッセージ受信処理 */
STATUS  iomnIVSaveJobStart(HI, HI, HI, HI, HI); /*  フォーム登録JOB開始メッセージ受信処理   */
STATUS  iomnIVPrnJobStart(HI, HI, HI, HI, HI);  /*  フォーム確認JOB開始メッセージ受信処理   */
#endif
STATUS  iomnRecJobStartAck();               /*  JOB開始受け付け受信処理     */
#if 0   /* MCK1.1 新メッセージ対応  引数追加    */
STATUS  iomnPPCJobStartAck(HI, HI, HI, HI, I32, I32, I32);     /*  複写JOB開始受付処理         */
STATUS  iomnDssScnJobStartAck(HI, HI, HI, I32);  /*  DSS入力JOB開始Ack受け付け処理       */
STATUS  iomnFaxScnJobStartAck(HI, HI, HI, I32);  /*  FAX原稿入力JOB受付け許可ACK */
#else
STATUS  iomnPPCJobStartAck(HI, HI, HI, HI, I32, I32, I32, HI);     /*  複写JOB開始受付処理         */
STATUS  iomnDssScnJobStartAck(HI, HI, HI, I32, HI);  /*  DSS入力JOB開始Ack受け付け処理       */
STATUS  iomnFaxScnJobStartAck(HI, HI, HI, I32, HI);  /*  FAX原稿入力JOB受付け許可ACK */
#endif
STATUS  iomnPrnJobStartAck(HI, HI, HI);     /*  印刷JOB受付けACK受信処理    */
#if 0   /* MCK1.1 新メッセージ対応  引数追加    */
STATUS  iomnIVSaveJobStartAck(HI, HI, HI, I32);  /*  フォーム登録JOBAck受信処理  */
STATUS  iomnIVPrnJobStartAck(HI, HI, HI, I32);   /*  フォーム確認JOBAck受信処理  */
#else
STATUS  iomnIVSaveJobStartAck(HI, HI, HI, I32, HI);  /*  フォーム登録JOBAck受信処理  */
STATUS  iomnIVPrnJobStartAck(HI, HI, HI, I32, HI);   /*  フォーム確認JOBAck受信処理  */
#endif
STATUS  iomnSusCanReservation(HI);          /*  中断中止予約時処理  */
STATUS  iomnRecieveCJSU();                  /*  ＪＯＢ中断受信処理          */
STATUS  iomnRecieveAJSU();                  /*  JOB中断Ack受信処理          */
STATUS  iomnRecieveTJSU();                  /*  JOB中断終了Trm受信処理              */
STATUS  iomnRecieveCCCN();                  /*  JOB遅延中断取り消し指示             */
STATUS  iomnRecieveTCCN();                  /*  JOB遅延中断取り消しステータス       */
STATUS  iomnRecieveCDJB();                  /*  JOB削除指示                         */
STATUS  iomnRecieveRLST();                  /*  リスト／レポート作成要求受信処理    */
STATUS  iomnRecieveTJED();                  /*  JOB終了ステータス受信処理   */
STATUS  iomnPPCTJED(HI,HI,I32,I32,HI,HI);   /*  PPCJOB終了受信処理          */
STATUS  iomnScnTJED(HI,HI,I32,I32,HI,HI);   /*  Fax入力JOB終了受信処理      */
STATUS  iomnPrnTJED(HI,HI,I32,I32,HI,HI);   /*  GDIJOB終了／自発中断処理    */
STATUS  iomnJobFinCommand();                /*  JOB完了指示受信処理         */
STATUS  iomnJobFinSts();                    /*  JOB完了ステータス受信処理   */
STATUS  iomnPrnJobFinSts(HI,HI,I32,I32,HI,HI);          /*  印刷JOB完了ステータス       */
#if 0   /* MCK1.1 新メッセージ対応  引数追加    */
STATUS  iomnFrontPrnJobFinSts(HI,HI,I32,I32,HI,HI);     /*  印刷JOB完了ステータス       */
STATUS  iomnExPPCJobFinSts(HI,HI,I32,I32,HI,HI) ;       /*  拡張複写JOB完了ステータス処理   */
STATUS  iomnScnJobFinSts(HI,HI,I32,I32,HI,HI);          /*  Fax入力JOB完了ステータス    */
STATUS  iomnDssPrnJobFinSts(HI,HI,I32,I32,HI,HI);       /*  DSS印刷JOB完了ステータス    */
#else
STATUS  iomnFrontPrnJobFinSts(HI,HI,I32,I32,HI,HI,HI);     /*  印刷JOB完了ステータス       */
STATUS  iomnExPPCJobFinSts(HI,HI,I32,I32,HI,HI,HI) ;       /*  拡張複写JOB完了ステータス処理   */
STATUS  iomnScnJobFinSts(HI,HI,I32,I32,HI,HI,HI);          /*  Fax入力JOB完了ステータス    */
STATUS  iomnDssPrnJobFinSts(HI,HI,I32,I32,HI,HI,HI);       /*  DSS印刷JOB完了ステータス    */
#endif
STATUS  iomnRecieveCJDT();                  /*  データ転送指示受信処理  */
STATUS  iomnDataTransport(HI, HI, I32, I32);    /*  データ転送ステータス送信準備処理    */
STATUS  iomnRecoverAck();                   /*  リカバリ受け付けＡｃｋメッセージ受信処理    */
STATUS  iomnRecoverStatus();                /*  リカバリ終了メッセージ受信処理  */
STATUS  iomnJobCancel();                    /*  ＪＯＢ中止受信処理  */
STATUS  iomnDirectFaxJobCancel(HI, HI);     /*  FAXダイレクト送信時のJOB中止    */
#if 0   /* MCK1.2   *//* 関数削除   */
/**/STATUS  iomnJobAcceptable();                /*  JOB受け付け許可通知受信処理 */
#endif
#if 0   /* V017.00_02 OLD */
/**/STATUS  iomnJobCancelByUser(HI,HI);         /*  csキーによるJOB中止処理 */
#else   /* V017.00_02 NEW */
STATUS  iomnJobCancelByUser(HI,HI,HI);         /*  csキーによるJOB中止処理 */
#endif
STATUS  iomnPaperInAdd();                   /*  ADD用紙残受信処理   */
STATUS  iomnChangeJobAtr();                 /*  JOB属性変更通知受信 */
STATUS  iomnChangeJobCustomAtr();		/*EBX_DCR_41129*/
STATUS  iomnChangeJobAtrPrn();              /*  JOB属性変更通知受信(印刷用) */
STATUS  iomnCountDown();                    /*  カウントダウン受信  */
STATUS  iomnFinishPagePrn();                /*  ページ印刷終了受信  */
STATUS  iomnRecMemoryEvent();               /*  メモリ残量通知受信  */
STATUS  iomnRecieveEFPR();                  /*  メモリＦＵＬＬ通知受信処理  */
STATUS  iomnRecieveEFCL();                  /*  メモリＦＵＬＬ解除通知受信処理  */
STATUS  iomnFaxMemFull2(HI);                 /*  FaxメモリFULL時のJOB制御    */
STATUS  iomnChangeMemfullJobtype(HI);       /*  メモリFULL発生時の印刷JOB種別変更処理   */
STATUS  iomnRecieveESPR();                  /*  印刷開始通知受信処理    */
STATUS  iomnFrontCjsu(HI,HI, HI);           /*  複写JOB中断指示受信処理 */
STATUS  iomnFrontDssCjsu(HI,HI, HI);           /*  Dss入力JOB中断指示受信処理 */
STATUS  iomnDirectCjsu(HI,HI, HI);           /*  DirectFaxJOB中断指示受信処理 */
STATUS  iomnPrnCjsu(HI,HI, HI);             /*  GDI印刷JOB中断指示受信処理  */
STATUS  iomnDssPrnCjsu(HI,HI, HI);          /*  Dss印刷JOB中断指示受信処理  */
STATUS  iomnPPCAjsu(HI, HI, HI);          /*  複写JOB中断Ack受信処理  */
STATUS  iomnFrontAjsu(HI, HI, HI);          /*  FrontJOB中断Ack受信処理  */
STATUS  iomnPrnAjsu(HI, HI, HI);            /*  印刷JOB中断Ack受信処理  */
STATUS  iomnJobUIAjsu(HI, HI, HI);          /*  JOB操作UIJOB中断Ack受信処理 */
STATUS  iomnPPCTjsu(HI, HI, I32, I32, HI, HI);      /*  複写JOB中断ステータス受信処理   */
STATUS  iomnScnTjsu(HI, HI, I32, I32, HI, HI);      /*  入力JOB中断ステータス受信処理   */
STATUS  iomnPrnTjsu(HI, HI, I32, I32, HI, HI);      /*  印刷JOB中断ステータス受信処理   */
STATUS  iomnFrontPrnTjsu(HI, HI, I32, I32, HI, HI); /*  Front印刷JOB中断ステータス受信処理  */
STATUS  iomnDssPrnTjsu(HI, HI,I32, I32, HI, HI);    /*  DSS印刷JOB中断ステータス受信処理    */
STATUS  iomnGetMcnRecmode(HI , HI , HI *);          /*  マシンリカバリタイプ取得処理    */
STATUS  iomnPrnChangeUI(HI, HI, HI);                /*  印刷終了時の優先度変更処理  */
STATUS  iomnGetEntryIdFromJobtype(HI, HI *);        /*  JOB種別ごとのEntryIDを取得する  */
STATUS  iomnSetStartQue(IOMN_START_QUE *, HI, HI, HI, HI, HI);            /*  スタートQUEテーブルパラメータセット */
STATUS  iomnGetStartQue(IOMN_START_QUE *, HI *, HI *, HI *, HI *, HI *);  /*  スタートQUEテーブルパラメータ取得   */
STATUS  iomnCleanStartQue(IOMN_START_QUE *);        /*  スタートQUEテーブル初期化   */
STATUS  iomnRecieveCFPM();                      /*  ＰＭ開放指示受信処理    */
STATUS  iomnRecieveEERX();                      /*  ＥＲＲＯＲ発生通知受信処理  */
STATUS  iomnRecieveEFSC();                      /*  入力完了メッセージ受信処理  */
STATUS  iomnEndOfMemoryFull(HI, HI);            /*  メモリFULL終了処理  */
STATUS  iomnGetPrnqueInSuspendProcess(HI *);    /*  中断処理中の印刷Queidを取得する */
STATUS  iomnRecieveEFST();                      /*  ステイプル解除通知受信処理  */
STATUS  iomnRecieveRDDS();                      /*  DSSデータ転送開始要求受信処理   */
STATUS  iomnRecieveEDCN();                      /*  データ転送キャンセル通知受信処理    */
STATUS  iomnPrepareForPrint(HI ,HI);            /*  印刷JOB開始前の準備処理 */
STATUS  iomnRecieveCCJB();                      /* 印刷ジョブ入れ替え処理 */
extern  STATUS iomnCheckSCS(HI);
void    iomnFAXLineBusy();                  /* FAX回線管理 */
void    iomnFAXLineOff();                   /* FAX回線管理 */

static  STATUS  iomnCalcJobStatus( HI, HI, I32, I32, HI * ); /* TJED受信時JOB状態算出処理 */
STATUS  iomnRecieveEDFL();
STATUS  iomnRecieveELST();
STATUS  iomnRecieveEFED();
STATUS  iomnRecieveCFST();
STATUS  iomnRecieveCFED();
STATUS  iomnRecieveEC20();
STATUS  iomnRecieveTFST();
STATUS  iomnRecieveTFED();
STATUS  iomnGetFrontUIMsgQId( MSG_Q_ID * );
STATUS  iomnFsmsInitStartMngTbl( HI );
/* D3.24 START Y.Shimada ADD */
STATUS  iomnRdcInitStartMngTbl( HI );
/* D3.24 END */
STATUS  iomnFsmsInitEndMngTbl();
STATUS  iomnFsmsCFEDProc( HI );
STATUS  iomnGetFrontUIInfo( MSG_Q_ID *, I32 * );
STATUS  iomnGetOptMNInfo( MSG_Q_ID *, I32 * );
STATUS  iomnFsmsSetSeqMngTbl( MSG_Q_ID, I32, I32, HI, IOMN_FSMS_SEQ_MNG *); 

void    iomnStoreCmdSender( HI hJobID );   /* コマンド送信元の保存 */
void    iomnStoreCjstParam( HI hJobID );   /* CJSTパラメータの保存 */
void    iomnFreeCopyJobInfo( HI hJobID );  /* コピー情報の開放 */
/* D3.12.1 START T.Fujii 印刷禁止処理修正 */
#if 0   /* OLD */
STATUS  iomnReleaseSyncLink( HI hJobID );    /* 逐次制御テーブルの開放 */
#else   /* D3.12.1 MID  NEW */
STATUS  iomnReleaseSyncLink( HI hIpTblID );
#endif
/* D3.12.1 END */
STATUS  iomnSubJobCancel( HI hJobID, HI hJobType, HI hCanFactor );   /* 入出力サブジョブの中止 */
STATUS  iomnInputJobCancel( HI hInputJobID, HI hCanFactor );         /* 入力ジョブの中止 */
STATUS  iomnOutputJobCancel( HI hOutJobID, HI hCanFactor );          /* 出力ジョブの中止 */
STATUS  iomnReceiveEPOT();               /* ページ出力終了通知受信処理 */
STATUS  iomnPPCPrnStart( HI hJobID );    /* コピーの場合の印刷開始処理 */
STATUS  iomnPPCPrnEnd( HI hJobID );      /* コピーの場合の印刷終了処理 */
STATUS  iomnClearSyncLink( HI hJobID, HI hSubJob ); /* JobIDクリア処理 */

/* D3.40 START T.Fujii フロントUI取得関数を追加 */
HI      iomnLibGetFrontUI( void );       /* フロントUIを取得する */
/* D3.40 END */

/* ローカル関数 */
LOCAL STATUS iomnPrepareForPPC2(HI hJobID);     /* 複写開始準備処理2 */
/* LOCAL STATUS iomnSetCpyInputMode( HI hJobID );  *//* V028.00_00 *//* 原稿入力ジョブのモードチェック＆設定 */

void    iomnStoreRddsParam( HI hJobID );           /* RDDSパラメータの保存 */
STATUS  iomnDataReceiveStart( HI hJobID );       /* データ受信開始処理 */
STATUS  iomnDataRcvJobStartAck( HI hJobID );     /* データ受信ジョブ開始受け付け処理 */
STATUS  iomnDataRcvJobFinSts( HI hJobID, HI hJobFinSts, I32 iErrApl, I32 iErrEngine,
                             HI hTjfnststype );         /* データ受信完了処理 */
STATUS  iomnSetInputMode( HI hJobID, HI hIOMode );       /* 動作モード、サブジョブ種別設定 */
STATUS  iomnInputJobFree( HI hJobID, HI hFlgCopyParMN ); /* 入力ジョブの解放 */

/* ローカル関数 */
LOCAL   STATUS iomnPrepareForDataReceive( HI hJobID );    /* データ受信開始準備 */

STATUS  iomnStart_IP_SIS_Cls( HI );              /* ＩＰテーブル／逐次制御テーブルの解放 */
STATUS  iomnStart_IP_Cls( HI );                  /* ＩＰテーブルの解放 */
STATUS  iomnStart_SIS_Cls( HI );                 /*  逐次制御テーブルの解放 */
STATUS  iomnHddFileToParMN(HI ,PAR_TBL_COM_FNC * ,B * );     /* ＨＤＤの保存値をｐａｒＭＮへ取り出し */
STATUS  iomnFormFileNameChg(HI ,HI ,B * );        /* ファイル名変換（ＨＤＤ） */
STATUS  iomnPrepareForNewJob( HI hJobID, HI hJobType, HI hContinuity );  /* 新規ジョブの準備 */
STATUS  iomnOutputJobSuspend( HI hInputJobID, HI hSusFactor );   /* 出力ジョブの中断 */
/* D3.0.1 START T.Fujii ソースコード整理 */
#if 0   /* OLD */
STATUS  iomnOutputJobRestart( HI hOutputJobID );     /* 出力ジョブの再開 */
#endif
/* D3.0.1 END */
STATUS  iomnWaitingOutputJobCancel( HI hOutputJobID, HI hJobStatus );  /* 未実行出力ジョブの中止 */
/* D3.12 START T.Fujii ジョブ中断処理修正 */
STATUS iomnInputJobSuspend( HI hInputJobID, HI hSusFactor, HI hJobType );
/* D3.12 END */
STATUS  iomnJobCancelTestCopy( HI hJobid, HI hJobtype, HI hCanFacter);  /* V017.00_01   */
STATUS  iomnJobPause(HI hJobid, HI hCanFactor, HI hJcbID, HI hTNo, HI hProcessType, HI hAppliType); /* V019.00_03   */
#if 1 /* RioGrande マニュアル対応 */
STATUS iomnRecieveRIST(void);
STATUS iomnRecieveAIST(void);
STATUS iomnRecieveTIFN(void);
extern STATUS iomnInsmanualConfirm(void);
#endif

extern STATUS iomnRecJobReg();																							/* SM-001 */
extern STATUS iomnRecCancelJobReg();																					/* SM-001 */


/* D3.14 START Y.Shimada ADD */
extern VOID iomnDebugTrace( HI, HI, HI);
/* D3.14 END */

/*  各種確認処理    */
extern  STATUS  iomnConfirmAboutCancel(HI);         /*  JOB中止確認処理 */
extern  STATUS  iomnPPCStartConfirm(HI);            /*  複写ＪＯＢ実行確認  */
extern  STATUS  iomnConfirmJobEnd(HI, PAR_TBL_RESOLUTION);
                                                    /*  JOBEND開始可不確認処理 */
/*  message管理関数 */
void    iomnSetJobManagementMsg(IOMNMSG_STORE *);   /*  メッセージ格納処理  */
void    iomnCleanJobManagementMsg(HI);              /*  メッセージ格納テーブル初期化    */

/*  メッセージ送信関数プロトタイプ宣言  */
extern  STATUS  iomnSendJobStart(HI, HI, HI, HI);               /*  JOB開始メッセージ送信   */
#if 0   /* MCK1.1 新メッセージ対応  引数追加    */
extern  STATUS  iomnSendJobStartAck(MSG_Q_ID,HI,HI,HI, HI, I32, I32, I32); /*  複写JOB開始受け付けメッセージ送信   */
extern  STATUS  iomnSendJobFinishSts(MSG_Q_ID, I32, HI, HI,I32, I32);/* JOB完了ステータスメッセージ送信 */
#else
extern  STATUS  iomnSendJobStartAck(MSG_Q_ID,HI,HI,HI, HI, I32, I32, I32, HI); /*  複写JOB開始受け付けメッセージ送信   */
extern  STATUS  iomnSendJobFinishSts(MSG_Q_ID, I32, HI, HI,I32, I32, HI);/* JOB完了ステータスメッセージ送信 */
#endif
extern  STATUS  iomnSendJobFinishCmd(MSG_Q_ID, HI);             /*  JOB完了メッセージ送信       */
extern  STATUS  iomnSendJobCancel(HI, MSG_Q_ID, HI);            /*  JOB中止指示送信             */
extern  STATUS  iomnSendJobSuspend(HI, MSG_Q_ID, HI, HI);       /*  中断メッセージ送信          */
extern  STATUS  iomnSendJobCancelFail(HI, MSG_Q_ID, I32);       /*  JOB中断／中止不成功         */
extern  STATUS  iomnSendJobAcceptEvent(HI, MSG_Q_ID, I32);      /*  JOB受け付け許可通知         */
extern  STATUS  iomnSendJobAtrEvent(MSG_Q_ID, I32);             /*  JOB属性変更通知送信         */
extern  STATUS  iomnSendCountDownEvent(MSG_Q_ID, I32);          /*  カウントダウンイベント送信  */
extern  STATUS  iomnSendEsrc(HI);                               /*  リカバリ開始メッセージ送信  */
extern  STATUS  iomnSendEfrc(HI, HI, I32, HI);                  /*  リカバリ終了メッセージ送信  */
extern  STATUS  iomnSetChangeUIQue(IOMNUI_QUE *, HI, HI, HI);   /*  UI_QUEへのパラメータセット  */
extern  STATUS  iomnGetChangeUIQue(IOMNUI_QUE *, HI *, HI *, HI *); /*  UI_QUEのパラメータ取得  */
extern  void    iomnCleanChangeUIQue(IOMNUI_QUE *);             /*  UI_QUEのパラメータ初期化    */
extern  STATUS  iomnSendTJDT(HI, HI, I32, I32);                 /*  データ転送ステータスを送信する  */
extern  STATUS  iomnSendPageEndEvent(MSG_Q_ID, I32);            /*  ページ終了通知イベント送信  */  
extern  STATUS  iomnSendMemoryResidue(MSG_Q_ID, I32);           /*  メッセージ残量通知イベント送信  */
extern  STATUS  iomnSendTjsu(I32, HI, HI, I32, I32, HI);        /*  中断終了ステータス送信          */
extern  STATUS  iomnSendAjsu(HI, HI);                           /*  中断受け付けメッセージ送信      */
extern  STATUS  iomnJobDeleteConfirm(HI, HI *, HI *, I32 *);    /*  JOB削除指示実行可否確認 */
extern  STATUS  iomnSuspendConfirm(HI, HI, HI);                 /*  JOB中断実行可否確認         */
extern  STATUS  iomnSendEFSC(HI, MSG_Q_ID, I32, HI, I32, I32);  /*  入力完了メッセージ送信処理  */
extern  STATUS  iomnSendEFPR(MSG_Q_ID, I32, HI);                /*  メモリFULLイベント送信処理  */
extern  STATUS  iomnSendRDUP(HI);                               /*  並行印刷要求送信処理    */
extern  STATUS  iomnSendTimer(I32, HI);             /*  タイマ指示送信処理  */
extern  STATUS  iomnIVStartConfirm(HI);             /*  フォーム確認JOB開始確認 */
extern  STATUS  iomnSendEFST(HI, MSG_Q_ID, I32, HI);/*  ステイプル解除通知送信処理  */
extern  STATUS  iomnSendTJED(MSG_Q_ID, I32, HI, HI, I32, I32, HI);/*    ＪＯＢ終了ステータスを送信する  */
extern  STATUS  iomnSendCCCN(HI, MSG_Q_ID, I32);    /*  遅延中断中止指示送信処理    */
extern  STATUS  iomnSendTCCN(HI, MSG_Q_ID, I32);    /*  遅延中断中止ステータス送信処理  */
extern  STATUS  iomnMemFullAcceptConfirm(HI);       /*  メモリFULL受付許可確認処理  */
extern  STATUS  iomnSendADJB(HI,HI);                     /*  JOB削除受付Ack送信              */
extern  STATUS  iomnSendEFCL(MSG_Q_ID, I32, HI);         /*  メモリFULL解除通知送信処理  */
extern  STATUS  iomnSendTDJB(HI, HI, HI);                /*  JOB削除ステータス送信           */
/* D5.3 START T.Fujii EB46送信対応 */
extern STATUS iomnSendECFN( HI hCpyJobID );
/* D5.3 END */
extern  STATUS  iomnDssScnStartConfirm(HI);              /*  Dss入力JOB実行可否確認      */
extern  STATUS  iomnSendCini(MSG_Q_ID, I32, HI, HI);     /*  初期化指示送信  */
extern  STATUS  iomnFaxLstStartConfirm(HI);              /*  リスト印刷実行可否確認  */
extern  STATUS  iomnFaxScnStartConfirm(HI);              /*  Fax入力JOB実行可否確認      */

/*  共通モジュール関数プロトタイプ宣言  */
extern  STATUS  iomnSendMessage(MSG_Q_ID, I32, I32,union ioMNparam *);
extern  STATUS  iomnSendErrMessage(MSG_Q_ID, I32);
#if 0
extern  void    iomnDebugMessage(B *, I32);                     /*  Debug Message の表示    */
#endif
extern  void    iomnDelay(I32);                                 /*  タスクDelayの設定   */
extern  STATUS  iomnMcnRecover(HI, HI, HI, HI, HI);                 /*  マシンリカバリ指示送信  */
extern  STATUS  iomnChangeUIPriority(HI, HI, HI, HI, HI, HI);   /*  UI優先度変更    */
extern  STATUS  iomnSendEDFL( MSG_Q_ID, I32 );
extern  STATUS  iomnSendELST( MSG_Q_ID, I32 );
extern  STATUS  iomnSendEFED( MSG_Q_ID, I32 );
extern  STATUS  iomnIVFormFileDelete(HI);               /*  フォーム登録ファイル削除処理  */
extern  STATUS  iomnSendACJB(HI);                         /* 印刷ｼﾞｮﾌﾞ入れ替え受付送信 */
extern  STATUS  iomnSendTCJB( HI, STATUS, I32);           /* 印刷ｼﾞｮﾌﾞ入れ替え終了送信 */
extern  HI      iomnFsmsStartConfirm( HI, HI );
/* D3.24 START Y.Shimada ADD */
extern  HI      iomnRdcStartConfirm( HI, HI );
/* D3.24 END */

extern  STATUS  iomnSendCFST( MSG_Q_ID, I32, HI );
extern  STATUS  iomnSendTFST( HI, STATUS );
extern  STATUS  iomnSendTFED( HI, STATUS );
extern  STATUS  iomnSendCFED( MSG_Q_ID , I32, HI );
extern  STATUS  iomnSendEC20( MSG_Q_ID );
extern  STATUS  iomnSendEC22();
extern  STATUS  iomnAddErrLog( B *, I32 );
extern  STATUS  iomnSendEpds(HI, HI, HI, HI);           /* 遅延中断イベント送信 *//* V016.00_02 ADD */

#if 0   /* V012.00_01   */
/**/extern  STATUS  iomnCpyMNSetPrnJobID(HI);   /* V011.00_05   *//* 暫定 ヘッダーに移すこと！！    */
#endif
extern  STATUS  iomnCheckPrnTime( HI ); /*  印刷実行時刻チェック    *//* V028.00_00 */
extern  STATUS iomnPrnJobSendStart( HI hJobID, HI hContinuity, HI hPrtType, HI hStartTriggerVal); /* EBX_STFR_17335 *//*EBX_DCR_53040_feedback_L4.91_EBX_DCR_57425*/

#include <ipLibPar.h>
#include <ipLibStr.h>
#include <ipLibEx.h>

#include "iomnSyncTblEx.h"  /* 逐次制御用ライブラリ */
#include "iomnCpyMNEx.h"    /* コピージョブ制御用ライブラリ */
#include "iomnPrnQueEx.h"   /* 印刷キュー操作ライブラリ */

#ifdef IOMN_DM_DEBUG_991115  /* D1.1 T.Fujii 99/11/15 */
extern void iomnSyncTblCheck();
extern void iomnCpyMNCheck();
#endif

extern  STATUS  iomnSendTjfn( HI hJobID, HI hSts, I32 iErrSys, I32 iErrEng );     /* TJFN送信 */
extern  STATUS  iomnSendESPR( HI hJobID, HI hPrnID );     /* 印刷開始通知 */
extern  STATUS  iomnSendEPFN( HI hJobID );                /* 印刷終了通知 */
/* 印刷イベントの送信 */
extern  STATUS  iomnSendPrintEventMsg(HI hJobID,HI hPrnID,I32 iMsgIDtoUI,I32 iMsgIDtoJOB,union ioMNparam *pSendPrm);
extern  STATUS  iomnSendAdds( HI hJobID, I32 iSts );      /* ADDS送信 */
extern  STATUS  iomnSendTdds( HI hJobID, HI hSts, I32 iErrSys, I32 iErrEng );     /* TDDS送信 */
extern  STATUS  iomnFanControl( B sw );       /* ファン制御関数 */
extern  BOOL    iomnSuscauseForInForce( I32 iCause, HI hJobtype );   /* 強制スタート再開用中断要因チェック */
extern  STATUS  iomnPrintJobStart(HI, HI, HI, HI);  /* 印刷ジョブ開始 */
extern  STATUS  iomnSendJobEvent(MSG_Q_ID, I32);    /* イベントメッセージ送信 */
extern  STATUS  iomnSendEJED( HI hJobID, union ioMNparam *pSendPrm );    /* 印刷中断通知 */
extern  void    iomnReserveSuspend(HI , HI, HI);    /*  JOB中断予約テーブルセット処理   *//* V036.00_02 */
extern  STATUS  iomnIntPPCCheck(); /* PRINT_JOINT *//* 連結割り込み */
extern  STATUS  iomnReleasePrn( HI );         /* VTR11 */
/* Feedback EBX_STFR_17062 --Start*/
extern STATUS iomnEPSTReceivedCheck(HI hjobID);
extern HI 	hioMNEPST_received_status;  
/* Feedback EBX_STFR_17062 --End*/
extern  I32         iioMNTaskId;            /*送信元タスクＩＤ格納用    */
extern  I32         iioMNMsgId;             /*メッセージＩＤ格納用      */
extern  MSG_Q_ID    ioMNMsgQId;             /*メッセージ送信元QueID格納用   */
extern  size_t      iioMNparamSize;         /*パラメータサイズ  */
extern  HI          hioMNFrontJobID;        /*フロントJOBID     */
extern  HI          hioMNIntJobID;          /*  割り込み複写JOBID   */
HI          hioMNDataRecvJobID;     /* データ受信用JOBID */
#if defined (WEISS_ENG) || defined (S2_ENG)
HI		hioMNDelayCJST = FALSE;
struct timeval tPreTime;
#endif
IOMN_FSMS_STATUS    ioMNFsmsStatus;

IOMN_FSMS_SEQ_MNG   ioMNFsmsStartSeqMng[IOMN_FSMS_MAX_SEQ_MNG];
                                            /* 開始シーケンス管理テーブル */
IOMN_FSMS_SEQ_MNG   ioMNFsmsEndSeqMng[IOMN_FSMS_MAX_SEQ_MNG];
                                            /* 解除シーケンス管理テーブル */

extern  struct  ioMNhw_TBL      ioMNhw;     /*  ハードウェア管理テーブル    */
extern  struct  ioMNjob_status  ioMNstatus; /*  入出力ＪＯＢ管理における状態管理テーブル    */
extern  struct  ioMNtimeTBL     ioMNtime;   /*  時間監視テーブル    */
extern  union   ioMNparam       ioMNsmyparam;   /*  入出力ＪＯＢ管理タスクにおける送信用パラメータ共用体*/
extern  union   ioMNparam       ioMNrmyparam;   /*  入出力ＪＯＢ管理タスクにおける受信用パラメータ共用体*/

extern  IOMNUI_QUE      ioMNUiBuffer;       /*  UI優先度変更事象発生    */
extern  IOMNUI_QUE      ioMNUiIntBuf;       /*  復帰ＵＩ情報テーブル    */
extern  IOMNUI_QUE      ioMNUiPrintBuf;     /*  復帰ＵＩ情報テーブル    */
extern  IOMNUI_QUE      ioMNUiErrClrBuf;    /*  ERROR復帰後のＵＩ情報   */
extern  IOMNUI_QUE      ioMNUiReserveBuf;   /*  指定するタイミングまでUI変更を留保するためのバッファ    */
extern  IOMNUI_QUE      ioMNUiJobBuf;       /*  JOB操作UI復帰先情報 */

extern  IOMNMSG_STORE   ioMNTjfn[IOMN_MAXJOBID],ioMNCjst[IOMN_MAXJOBID];
extern  IOMNMSG_STORE   ioMNRjre[IOMN_MAXJOBID], ioMNRjcn[IOMN_MAXJOBID] /* 10911 fix*/;
extern  IOMNMSG_STORE   ioMNAjst[IOMN_MAXJOBID],ioMNTjed[IOMN_MAXJOBID];
extern  IOMNMSG_STORE   ioMNCjfn[IOMN_MAXJOBID],ioMNCjsu[IOMN_MAXJOBID];
extern  IOMNMSG_STORE   ioMNAjsu[IOMN_MAXJOBID],ioMNTjsu[IOMN_MAXJOBID];
extern  IOMNMSG_STORE   ioMNCjcn[IOMN_MAXJOBID];
extern  IOMNMSG_STORE   ioMNEmemFront,ioMNEmemBack;
extern  IOMNMSG_STORE   ioMNCdof;
/* D5.0 START T.Fujii CDJB多重動作対応 */
#if 0   /* OLD */
extern  IOMNMSG_STORE   ioMNCdjb;               /*  JOB削除メッセージ格納用     */
#else   /* D5.0 NEW */
extern  IOMNMSG_STORE   ioMNCdjb[IOMN_MAXJOBID];    /*  JOB削除メッセージ格納用     */
#endif
/* D5.0 END */

extern  IOMNMSG_STORE   ioMNCfst;
extern  IOMNMSG_STORE   ioMNCfed;
extern  IOMNMSG_STORE   ioMNTfst;
extern  IOMNMSG_STORE   ioMNTfed;
extern  IOMNMSG_STORE   ioMNEc20;

extern  IOMN_DATATRANS  ioMNTjdt;               /*  Data転送ステータス格納用    */
extern  IOMN_EPWN       ioMNEpwn;               /*  電源オン通知パラメータ格納用    */

extern  IOMN_FILEID     ioMNfileId;
extern  IOMN_RECOVERY   ioMNRcvBuffer;          /*  マシンリカバリ処理Queingバッファ    */
extern  IOMN_START_QUE  ioMNPrnQue;             /*  印刷開始Queingテーブル  */
extern  IOMN_START_QUE  ioMNFrontQue;           /*  FrontJOB開始Queingテーブル  */
#if 0   /* MCK1.1 新メッセージ対応  */
extern  IOMSG_STR_CJRC_CTL  ioMNCjrcQue;        /*  JOBリカバリQUEing処理テーブル   */
#endif
extern  IOMNMSG_STORE   ioMNCccn;                       /*  遅延中断中止メッセージ格納用    */
extern  IOMN_SUSPEND_QUE    ioMNFrontSusQue;    /*  フロントJOB中断Queingテーブル   */
extern  IOMN_SUSPEND_QUE    ioMNBackSusQue;     /*  バックグランドJOB中断Queingテーブル */
extern  IOMN_SUSPEND_QUE    ioMNFrontDssSusQue; /*  DSS入力処理時の中断Queingテーブル   */
extern  IOMN_SUSPEND_QUE    ioMNBackDssSusQue;      /*  DSS印刷処理時の中断Queingテーブル   */

/* D4.8 START T.Fujii メッセージ送信先の選択 */
extern  MSG_Q_ID        msgQIdioMNforCpyJob;        /* コピージョブに関するメッセージ送信先 */
/* D4.8 END */
extern  JOB_STR_CJCN  ioMNCjcnQue;        /*  JOB中止Queingテーブル   *//* V024.00_01 */

/*
    iomnPrnQueJobDelete()のI/F変更に伴い、下記のグローバル変数を使用する。
    ジョブ完了(TJFN)、ジョブ中止(CJCN)時は、-1を設定 → ジョブIDからエントリIDを求めて使用する
    ジョブ削除(CDJB)は、メッセージのパラメータを設定 → エントリIDとして使用する
*/
HI hIoMNPrnQueEntryID = -1;

IOMN_JOBINFO iomnJobInfo[IOMN_MAXJOBID];

/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         7
TITLE:      JOB管理処理Routine
MODULE：    STATUS  iomnDoJobManagement();
INCLUDE:    ioMNPar.h
PARAMETER:
            I32     imsgid;
RETURNS:

OUTLINE:
            JOB管理系メッセージ受信時の処理
NOTES:

HISTORY:
        1.1     Y.Matsuda   96/9/18
        M3.30(TJ)H.Takahashi    98/11/30
                ADUフル発生通知対応
        M4.11(TJ)H.Takahashi    99/01/22
                FSMS対応
        M7.20(TJ)H.Takahashi    99/09/02
                ファイル化完了イベント対応
        D1.1     T.Fujii     99/11/15    DM45/35 後追い逐次・並行動作・先行入力対応
        D1.6.7   T.Fujii     00/04/10    DM45/35 ジョブ属性変更通知送信処理修正
        D1.7.10  J.Ootani    00/04/08    DM45/35 FAXダイレクト送信対応
        D2.3     T.Fujii     00/06/06    DM45/35 PC-FAX対応

        MCK1.1  M.Karasawa  02/06/11    新メッセージ対応
        MCK1.4  M.Karasawa  02/07/18    IOMSG_EFSC_EX   →  JOB_MSG_ESCP変更

    V010.00_04  M.Karasawa  02/12/17    McKinley    メッセージ変更 JOB_MSG_RFSR -> JOB_MSG_EFSR
    V016.00_02  M.Karasawa  03/03/26    McKinley    メッセージ置換                  構造体置換
                                                    JOB_MSG_CCIC -> JOB_MSG_CPSW    JOB_STR_CCIC -> JOB_STR_CPSW
                                                    JOB_MSG_TCIC -> JOB_MSG_TPSW    JOB_STR_TCIC -> JOB_STR_TPSW
                                                    新規メッセージ
                                                    JOB_MSG_EPDS                    JOB_STR_ECIC -> JOB_STR_EPDS
RIOGRANDE:
    041207Daily M.Karasawa  04/12/07    RioGrande   ジョブ連結動作改善
*/

STATUS  iomnDoJobManagement()
{
    STATUS  Status; /*  Message受信処理結果 */

    hIoMNPrnQueEntryID = -1;

    switch(iioMNMsgId){
	/* 10911 fix*/
        case    JOB_MSG_RJRE:   
                    if(ERROR == (Status = iomnRecJobReg())){
                        iomnDebugMessage("error:iomnRecJobReg()\n",0);
                        return(ERROR);
                    }
                    iomnDebugMessage("OK:iomnRecJobReg()\n",0);
                    break;
		case	JOB_MSG_RJCN:	
					if(ERROR == (Status = iomnRecCancelJobReg())){
						iomnDebugMessage("error:iomnRecCancelJobReg()\n",0);
						return(ERROR);
					}
					iomnDebugMessage("OK:iomnRecCancelJobReg()\n",0);
					break;
#if 0   /* MCK1.1 新メッセージ対応  */
/**/        case    IOMSG_CJST_UI:  /*JOB開始指示*/
/**/        case    IOMSG_CJST_DIG: /*JOB開始指示*/
/**/        case    IOMSG_CJST_JOB: /*JOB開始指示*/
#else
        case    JOB_MSG_CJST:   /* JOB開始指示          */
#endif
                    if(ERROR == (Status = iomnRecJobStart())){
                        iomnDebugMessage("error:iomnRecJobStart()\n",0);
                        return(ERROR);
                    }
                    iomnDebugMessage("OK:iomnRecJobStart()\n",0);
                    break;
#if 0   /* MCK1.1 新メッセージ対応  */
/**/        case    IOMSG_AJST_EX:  /*JOB開始受けつけ*/
#else
        case    JOB_MSG_AJST:   /* JOB開始受けつけ      */
#endif


                    if(ERROR == (Status = iomnRecJobStartAck())){
                        iomnDebugMessage("error:iomnRecJobStartAck()\n",0);
                        return(ERROR);
                    }
                    iomnDebugMessage(">>>OK:iomnRecJobStartAck()\n",0);
                    break;
#if 0   /* MCK1.1 新メッセージ対応  */
/**/        case    IOMSG_TJED_EX:  /*JOB終了ステータス*/
#else
        case    JOB_MSG_TJSX:   /* JOB終了ステータス    */
#endif
                    if(ERROR == (Status = iomnRecieveTJED())){
                        iomnDebugMessage(">>>Error:iomnRecieveTJED()\n",0);
                        return(ERROR);
                    }
                    iomnDebugMessage(">>>OK:iomnRecieveTJED()\n",0);
                    break;
#if 0   /* MCK1.1 新メッセージ対応  *//* 要検討 */
/**/        case    IOMSG_AJRC_EX:  /*リカバリ受け付けAck*/
/**/                    if(ERROR == (Status = iomnRecoverAck())){
/**/                        iomnDebugMessage("error:iomnRecoverAck() \n",0);
/**/                        return(ERROR);
/**/                    }
/**/                    iomnDebugMessage(">>>OK:iomnRecoverAck()\n",0);
/**/                    break;
#endif
#if 0   /* MCK1.1 新メッセージ対応  *//* 要検討 */
/**/        case    IOMSG_TJRC_EX:  /*リカバリ終了ステータス*/
/**/                    if(ERROR == (Status = iomnRecoverStatus())){
/**/                        iomnDebugMessage("error:iomnRecoverStatus()\n",0);
/**/                        return(ERROR);
/**/                    }
/**/                    iomnDebugMessage(">>>OK:iomnRecoverStatus()\n",0);
/**/                    break;
#endif
#if 0   /* MCK1.1 新メッセージ対応  */
/**/        case    IOMSG_CJFN_UI:  /*JOB完了指示*/
/**/        case    IOMSG_CJFN_JOB: /*JOB完了指示*/
#else
        case    JOB_MSG_CJFN:   /* JOB完了指示                  */
#endif
                    if(ERROR == (Status = iomnJobFinCommand())){
                        iomnDebugMessage("error:iomnJobFinCommand()\n",0);
                        return(ERROR);
                    }
                    iomnDebugMessage("OK:iomnJobFinCommand()\n",0);
                    break;
#if 0   /* MCK1.1 新メッセージ対応  */
/**/        case    IOMSG_TJFN_EX:  /*JOB完了ステータス*/
#else
        case    JOB_MSG_TJFN:   /* JOB完了ステータス            */
#endif
                    if(ERROR == (Status = iomnJobFinSts())){
                        iomnDebugMessage("error:iomnJobFinSts()\n",0);
                        return(ERROR);
                    }
                    iomnDebugMessage("OK:iomnJobFinSts()\n",0);
                    break;
#if 0   /* MCK1.1 新メッセージ対応  */
/**/        case    IOMSG_CJSU_UI:  /*JOB中断指示*/
/**/        case    IOMSG_CJSU_JOB: /*JOB中断指示*/
#else
        case    JOB_MSG_CJSU:   /* JOB中断指示                  */
#endif
                    if(ERROR == (Status = iomnRecieveCJSU())){
                        iomnDebugMessage(">>>Error:iomnRecieveCJSU()\n",0);
                        return(ERROR);
                    }
                    iomnDebugMessage("OK:iomnRecieveCJSU()\n",0);
                    break;
#if 0   /* MCK1.1 新メッセージ対応  */
/**/        case    IOMSG_AJSU_EX:  /*JOB中断受け付け*/
#else
        case    JOB_MSG_AJSU:   /* JOB中断受け付け              */
#endif
                    if(ERROR == (Status = iomnRecieveAJSU())){
                        iomnDebugMessage(">>>Error:iomnRecieveAJSU()\n",0);
                        return(ERROR);
                    }
                    iomnDebugMessage("OK:iomnRecieveAJSU()\n",0);
                    break;
#if 0   /* MCK1.1 新メッセージ対応  */
/**/        case    IOMSG_TJSU_EX:  /*JOB中断終了ステータス*/
#else
        case    JOB_MSG_TJSU:   /* JOB中断終了ステータス        */
#endif
                    if(ERROR == (Status = iomnRecieveTJSU())){
                        iomnDebugMessage(">>>Error:iomnRecieveTJSU()\n",0);
                        return(ERROR);
                    }
                    iomnDebugMessage("OK:iomnRecieveTJSU()\n",0);
                    break;
#if 0   /* MCK1.1 新メッセージ対応  */
/**/        case    IOMSG_CCCN_UI:  /*遅延中断取り消し指示*/
/**/        case    IOMSG_CCCN_JOB: /*遅延中断取り消し指示*/
#else
#if 0   /* V016.00_02   */
/**/        case    JOB_MSG_CCIC:   /* 遅延中断取り消し指示         */
#else   /* V016.00_02   */
        case    JOB_MSG_CPSW:   /* 遅延中断取り消し指示         */
#endif
#endif
                    if(ERROR == (Status = iomnRecieveCCCN())){
                        iomnDebugMessage(">>>Error:iomnRecieveCCCN()\n",0);
                        return(ERROR);
                    }
                    iomnDebugMessage("OK:iomnRecieveCCCN()\n",0);
                    break;
#if 0   /* MCK1.1 新メッセージ対応  */
/**/        case    IOMSG_TCCN_EX:  /*遅延中断取り消しステータス*/
#else
#if 0   /* V016.00_02   */
/**/        case    JOB_MSG_TCIC:   /* 遅延中断取り消しステータス   */
#else
        case    JOB_MSG_TPSW:   /* 遅延中断取り消しステータス   */
#endif  /* V016.00_02   */
#endif
                    if(ERROR == (Status = iomnRecieveTCCN())){
                        iomnDebugMessage(">>>Error:iomnRecieveTCCN()\n",0);
                        return(ERROR);
                    }
                    iomnDebugMessage("OK:iomnRecieveTCCN()\n",0);
                    break;
#if 0   /* MCK1.1 新メッセージ対応  *//* 要検討 */
/**/        case    IOMSG_CJDT_JOB: /*データ転送指示*/
/**/                    if(ERROR == (Status = iomnRecieveCJDT())){
/**/                        iomnDebugMessage(">>>Error:iomnRecieveCJDT()\n",0);
/**/                        return(ERROR);
/**/                    }
/**/                    iomnDebugMessage("OK:iomnJovCancel()\n",0);
/**/                    break;
#endif
#if 0   /* MCK1.1 新メッセージ対応  */
/**/        case    IOMSG_CJCN_UI:  /*JOB中止指示*/
/**/        case    IOMSG_CJCN_JOB: /*JOB中止指示*/
#else
        case    JOB_MSG_CJCN:   /* JOB中止指示      */
#endif
                    if(ERROR == (Status = iomnJobCancel())){
                        iomnDebugMessage(">>>Error:iomnJovCancel()\n",0);
                        return(ERROR);
                    }
                    iomnDebugMessage("OK:iomnJobCancel()\n",0);
                    break;
#if 0   /* MCK1.1 新メッセージ対応  *//* 要検討 */
/**/        case    IOMSG_CDJB_UI:  /*JOB削除指示*/
/**/                    if(ERROR == (Status = iomnRecieveCDJB())){
/**/                        iomnDebugMessage(">>>Error:iomnRecieveCDJB()\n",0);
/**/                        return(ERROR);
/**/                    }
/**/                    iomnDebugMessage("OK:iomnRecieveCDJB()\n",0);
/**/                    break;
/**/        case    IOMSG_CCJB_UI:  /*  印刷ジョブ入れ替え指示  */
/**/        case    IOMSG_CCJB_JOB: /*  印刷ジョブ入れ替え指示  */
#else
        case    JOB_MSG_CJCH:   /* 印刷ジョブ入れ替え指示   */
#endif
                    if(ERROR == (Status = iomnRecieveCCJB())){
                        iomnDebugMessage(">>>Error:iomnRecieveCCJB()\n",0);
                        return(ERROR);
                    }
                    iomnDebugMessage(">>>OK:iomnRecieveCCJB()\n",0);
                    break;
#if 0   /* MCK1.1 新メッセージ対応  *//* 要検討 */
/**/        case    IOMSG_ENJE_EX:  /*  次JOB受け付け許可*/
/**/                    if(ERROR == (Status = iomnJobAcceptable())){
/**/                        iomnDebugMessage(">>>Error:iomnJobAcceptable()\n",0);
/**/                        return(ERROR);
/**/                    }
/**/                    iomnDebugMessage("OK:iomnJobAcceptable()\n",0);
/**/                    break;
#endif
#if 0   /* MCK1.1 新メッセージ対応  *//* 要検討 */
/**/        case    IOMSG_EADD_EX:  /*#ADD用紙残*/
/**/                    if(ERROR == (Status = iomnPaperInAdd())){
/**/                        iomnDebugMessage("error:iomnPaperInAdd()\n",0);
/**/                        return(ERROR);
/**/                    }
/**/                    iomnDebugMessage("OK:iomnPaperInAdd()\n",0);
/**/                    break;
#endif
#if 0   /* MCK1.1 新メッセージ対応  *//* 要検討 */
/**/        case    IOMSG_EATS_EX:  /*入力JOB属性変更通知*/
#else
        case    JOB_MSG_EJAB:   /* JOB属性変更通知  */
#endif
                    if(ERROR == (Status = iomnChangeJobAtr())){
                        iomnDebugMessage(">> ioMN >> ERROR:iomnChangeJobAtr()\n",0);
                        return(ERROR);
                    }
                    iomnDebugMessage("OK:iomnChangeJobAtr()\n",0);
                    break;
#if 0   /* MCK1.1 新メッセージ対応  *//* 入力JOB属性変更通知と統合  */
/**/        case    IOMSG_EATR_EX:  /*#JOB属性変更通知*/
/**/                    if(ERROR == (Status = iomnChangeJobAtrPrn())){
/**/                        iomnDebugMessage("error:iomnChangeJobAtr()\n",0);
/**/                        return(ERROR);
/**/                    }
/**/                    iomnDebugMessage("OK:iomnChangeJobAtr()\n",0);
/**/                    break;
#endif
/*EBX_DCR_41129 start*/
		case	JOB_MSG_EJCA: 
					if(ERROR == (Status = iomnChangeJobCustomAtr())){
                        iomnDebugMessage(">> ioMN >> ERROR:iomnChangeJobCustomAtr()\n",0);
                        return(ERROR);
                    }
                    iomnDebugMessage("OK:iomnChangeJobCustomAtr()\n",0);
					break;
/*EBX_DCR_41129 end*/
#if 0   /* MCK1.1 新メッセージ対応  *//* 要検討 */
/**/        case    IOMSG_ECDN_EX:  /*#カウントダウン */
#else
        case    JOB_MSG_EPCD:   /* カウントダウン   */
#endif
                    if(ERROR == (Status = iomnCountDown())){
                        iomnDebugMessage("error:iomnCountDown()\n",0);
                        return(ERROR);
                    }
                    iomnDebugMessage("OK:iomnCountDown()\n",0);
                    break;
	/*Manoj-A DCR_28119: L4.8 Counters: sheet counter and toner erase counter*/
		case	JOB_MSG_EPSC:
					memcpy(&ioMNsmyparam, &ioMNrmyparam, sizeof(union ioMNparam));
					if (ioMNCjst[ioMNrmyparam.sEpscCtl.hJobid].OldmsgQId != NULL)/*Validity check for msgqid*/
					{
						if(ERROR == (Status = iomnSendMessage(ioMNCjst[ioMNrmyparam.sEpscCtl.hJobid].OldmsgQId, JOB_MSG_EPSC, 0, &ioMNsmyparam))){
        				iomnDebugMessage(">>>error:iomnSendMessage()EPSC in iomnDoJobManagement()\n",0);
       					return(ERROR);
							}
					}
					break;
	/*End*/
#if 0   /* MCK1.1 新メッセージ対応  *//* 要検討 */
/**/        case    IOMSG_EPED_EX:  /*#頁印刷終了通知*/
/**/                    if(ERROR == (Status = iomnFinishPagePrn())){
/**/                        iomnDebugMessage(">>>Error:iomnFinishPagePrn()\n",0);
/**/                        return(ERROR);
/**/                    }
/**/                    iomnDebugMessage(">>>OK:iomnFinishPagePrn()\n",0);
/**/                    break;
#endif
#if 0   /* MCK1.1 新メッセージ対応  *//* 要検討 */
/**/        case    IOMSG_EPOT_EX:  /* 頁出力終了イベント */
#else
        case    JOB_MSG_EPOP:   /* 頁出力終了イベント   */
#endif
                    if(ERROR == (Status = iomnReceiveEPOT())){
                        iomnDebugMessage("error:iomnCountDown()\n",0);
                        return(ERROR);
                    }
                    iomnDebugMessage("OK:iomnCountDown()\n",0);
                    break;
#if 0   /* MCK1.1 新メッセージ対応  *//* 要検討 */
/**/        case    IOMSG_EMEM_EX:  /*#メモリ残量通知*/
#else
        case    JOB_MSG_EJME:   /* メモリ残量通知       */
#endif
                    if(ERROR == (Status = iomnRecMemoryEvent())){
                        iomnDebugMessage(">>>Error:iomnRecMemoryEvent()\n",0);
                        return(ERROR);
                    }
                    iomnDebugMessage(">>>OK:iomnRecMemoryEvent()\n",0);
                    break;
#if 0   /* MCK1.1 新メッセージ対応  */
/**/        case    IOMSG_EFPR_UI:  /*  メモリfull通知  */
/**/        case    IOMSG_EFPR_JOB: /*  メモリfull通知  */
/**/        case    IOMSG_EFPR_EX:  /*  メモリfull通知  */
#else
        case    SYS_MSG_EFPR:   /* メモリfull通知       */
#endif
                    if(ERROR == (Status = iomnRecieveEFPR())){
                        iomnDebugMessage(">>>Error:iomnRecieveEFPR()\n",0);
                        return(ERROR);
                    }
                    iomnDebugMessage(">>>OK:iomnRecieveEFPR()\n",0);
                    break;
#if 0   /* MCK1.1 新メッセージ対応  */
/**/        case    IOMSG_EFCL_JOB: /*  メモリfull解除通知  */
/**/        case    IOMSG_EFCL_EX:  /*  メモリfull解除通知  */
#else
        case    SYS_MSG_EFCL:   /* メモリFULL解除通知   */
#endif
                    if(ERROR == (Status = iomnRecieveEFCL())){
                        iomnDebugMessage(">>>Error:iomnRecieveEFCL()\n",0);
                        return(ERROR);
                    }
                    iomnDebugMessage(">>>OK:iomnRecieveEFCL()\n",0);
                    break;
#if 0   /* MCK1.1 新メッセージ対応  *//* 要検討 */
/**/        case    IOMSG_RLST_DIG: /*  リスト/レポート作成要求  */
/**/                    if(ERROR == (Status = iomnRecieveRLST())){
/**/                        iomnDebugMessage(">>>Error:iomnRecieveRLST()\n",0);
/**/                        return(ERROR);
/**/                    }
/**/                    iomnDebugMessage(">>>OK:iomnRecieveRLST()\n",0);
/**/                    break;
#endif
#if 0   /* MCK1.1 新メッセージ対応  */
/**/        case    IOMSG_ESPR_EX:  /*  印刷開始通知    */
#else
        case    JOB_MSG_EPST:   /* 印刷開始通知     */
#endif
                    if(ERROR == (Status = iomnRecieveESPR())){
                        iomnDebugMessage(">>>Error:iomnRecieveESPR()\n",0);
                        return(ERROR);
                    }
                    iomnDebugMessage(">>>OK:iomnRecieveRLST()\n",0);
                    break;
	/* Anusha added for DTFR 9599, EJFN from scnEX is now sent through ioExMN */
	case	JOB_MSG_EJFN:
			{
	                    iomnDebugMessage(">>>OK:iomnRecieveEJFN()\n",0);
			    if(ERROR == (Status = iomnSendMessage(ioMNCjst[ ioMNrmyparam.sEjfnCtl.hJobId].OldmsgQId, JOB_MSG_EJFN, 0, &ioMNrmyparam))){
       			 iomnDebugMessage(">>>error:iomnSendMessage() in iomnSendMemoryResidue()\n",0);
			        return(ERROR);
    				}
			}


		break;
#if 0   /* MCK1.1 新メッセージ対応  */
/**/        case    IOMSG_EERX_EX:  /*  ERROR発生通知   */
/**/        case    IOMSG_EERX_UI:  /*  ERROR発生通知   */
#else
        case    SYS_MSG_EERX:   /* ERROR発生通知    */
#endif
                    if(ERROR == (Status = iomnRecieveEERX())){
                        iomnDebugMessage(">>>Error:iomnRecieveEERX()\n",0);
                        return(ERROR);
                    }
                    iomnDebugMessage(">>>OK:iomnRecieveEERX()\n",0);
                    break;
#if 0   /* MCK1.1 新メッセージ対応  *//* 要検討 */
/**/        case    IOMSG_CFPM_JOB: /*  PM開放指示  */
/**/                    if(ERROR == (Status = iomnRecieveCFPM())){
/**/                        iomnDebugMessage(">>>Error:iomnRecieveCFPM()\n",0);
/**/                        return(ERROR);
/**/                    }
/**/                    iomnDebugMessage(">>>OK:iomnRecieveCFPM()\n",0);
/**/                    break;
#endif
#if 0   /* MCK1.1 新メッセージ対応  *//* 要検討 */
/**/            case    IOMSG_EFSC_EX:  /*  入力完了メッセージ受信処理  */
#else
            case    JOB_MSG_ESCP:  /*  入力完了メッセージ受信処理  *//* MCK1.4  */
#endif
                        if(ERROR == (Status = iomnRecieveEFSC())){
                            iomnDebugMessage(">>>Error:iomnRecieveEFSC()\n",0);
                            return(ERROR);
                        }
                        iomnDebugMessage(">>>OK:iomnRecieveEFSC()\n",0);
                        break;
#if 0   /* MCK1.1 新メッセージ対応  */
/**/        case    IOMSG_EFST_EX:  /*  ステイプル解除通知  */
#else
        case    JOB_MSG_EMSC:   /* ステイプル解除通知   */
#endif
                    if(ERROR == (Status = iomnRecieveEFST())){
                        iomnDebugMessage(">>>Error:iomnRecieveEFST()\n",0);
                        return(ERROR);
                    }
                    break;
#if 0   /* MCK1.1 新メッセージ対応  */
/**/        case    IOMSG_EDFL_EX:  /* ADUフル発生通知 */
#else
        case    SYS_MSG_EDFL:   /* ADUフル発生通知  */
#endif
                    if ( ERROR == (Status=iomnRecieveEDFL()) ){
                        iomnDebugMessage(">>>Error:iomnRecieveEDFL()\n",0);
                        return ERROR;
                    }
                    break;
#if 0   /* MCK1.1 新メッセージ対応  */
/**/        case    IOMSG_ELST_EX:  /* 最終原稿通知 */
#else
        case    JOB_MSG_ESLP:   /* 最終原稿通知     */
#endif
                    if ( ERROR == (Status=iomnRecieveELST()) ){
                        iomnDebugMessage(">>>Error:iomnRecieveELST()\n",0);
                        return ERROR;
                    }
                    break;
#if 0   /* MCK1.1 新メッセージ対応  *//* 要検討 */
/**/        case    IOMSG_EFED_EX:  /* ファイル化完了通知 */
#else
        case    JOB_MSG_ESNC:   /* ファイル化完了   */
#endif
                    if ( ERROR == (Status=iomnRecieveEFED()) ){
                        iomnDebugMessage(">>>Error:iomnRecieveEFED()\n",0);
                        return ERROR;
                    }
                    break;
#if 0   /* MCK1.1 新メッセージ対応  *//* 要検討 */
/**/        case    IOMSG_EDCN_JOB:  /*  データ転送キャンセル通知  */
/**/                    if(ERROR == (Status = iomnRecieveEDCN())){
/**/                        cmnPrintf_ioMN(2,">>>Error:iomnRecieveEDCN()\n");
/**/                        return(ERROR);
/**/                    }
/**/                    break;
/**/        case    IOMSG_RDDS_JOB:  /*  DSSデータ転送開始通知 */
/**/                    if(ERROR == (Status = iomnRecieveRDDS())){
/**/                        cmnPrintf_ioMN(2,">>>Error:iomnRecieveRDDS()\n");
/**/                        return(ERROR);
/**/                    }
/**/                    break;
#endif
#if 0   /* MCK1.1 新メッセージ対応  */
/**/        case    IOMSG_CFST_UI   :   /* FSMS開始指示 */
/**/        case    IOMSG_CFST_JOB  :   /* FSMS開始指示 */
#else
        case    SYS_MSG_CFST:   /* FSMS開始指示     */
#endif
#if 0   /* MCK1.1 新メッセージ対応  */
/**/        /* D3.24 START Y.Shimada ADD */
/**/        case    IOMSG_CRST_UI   :   /* RDC開始指示 */
/**/        case    IOMSG_CRST_JOB  :   /* RDC開始指示 */
/**/        /* D3.24 END */
#endif
                    if(ERROR == (Status = iomnRecieveCFST())){
                        cmnPrintf_ioMN(2,">>>Error:iomnRecieveCFST()\n");
                        return(ERROR);
                    }
                    break;
#if 0   /* MCK1.1 新メッセージ対応  */
/**/        case    IOMSG_CFED_UI   :   /* FSMS解除指示 */
/**/        case    IOMSG_CFED_JOB  :   /* FSMS解除指示 */
#else
        case    SYS_MSG_CFED:   /* FSMS解除指示     */
#endif
#if 0   /* MCK1.1 新メッセージ対応  */
/**/        /* D3.24 START Y.Shimada ADD */
/**/        case    IOMSG_CRED_UI   :   /* RDC解除指示 */
/**/        case    IOMSG_CRED_JOB  :   /* RDC解除指示 */
/**/        /* D3.24 END */
#endif
                    if(ERROR == (Status = iomnRecieveCFED())){
                        cmnPrintf_ioMN(2,">>>Error:iomnRecieveCFED()\n");
                        return(ERROR);
                    }
                    break;
#if 0   /* MCK1.1 新メッセージ対応  */
/**/        case    IOMSG_TFST_UI   :   /* FSMS開始結果 */
/**/        case    IOMSG_TFST_JOB  :   /* FSMS開始結果 */
#else
        case    SYS_MSG_TFST:   /* FSMS開始結果     */
#endif
#if 0   /* MCK1.1 新メッセージ対応  */
/**/        /* D3.24 START Y.Shimada ADD */
/**/        case    IOMSG_TRST_UI   :   /* RDC開始結果 */
/**/        case    IOMSG_TRST_JOB  :   /* RDC開始結果 */
/**/        /* D3.24 END */
#endif
                    if(ERROR == (Status = iomnRecieveTFST())){
                        cmnPrintf_ioMN(2,">>>Error:iomnRecieveTFST()\n");
                        return(ERROR);
                    }
                    break;
#if 0   /* MCK1.1 新メッセージ対応  */
/**/        case    IOMSG_TFED_UI   :   /* FSMS解除結果 */
/**/        case    IOMSG_TFED_JOB  :   /* FSMS解除結果 */
#else
        case    SYS_MSG_TFED:   /* FSMS解除結果     */
#endif
#if 0   /* MCK1.1 新メッセージ対応  */
/**/        /* D3.24 START Y.Shimada ADD */
/**/        case    IOMSG_TRED_UI   :   /* RDC解除結果 */
/**/        case    IOMSG_TRED_JOB  :   /* RDC解除結果 */
/**/        /* D3.24 END */
#endif
                    if(ERROR == (Status = iomnRecieveTFED())){
                        cmnPrintf_ioMN(2,">>>Error:iomnRecieveTFED()\n");
                        return(ERROR);
                    }
                    break;
#if 0   /* MCK1.1 新メッセージ対応  */
/**/        case    IOMSG_EC20_JOB  :   /* 再起動イベント */
#else
        case    SYS_MSG_EC20:   /* 再起動イベント   */
#endif
                    if(ERROR == (Status = iomnRecieveEC20())){
                        cmnPrintf_ioMN(2,">>>Error:iomnRecieveEC20()\n");
                        return(ERROR);
                    }
                    break;
#if 0   /* MCK1.1 新メッセージ対応  *//* 要検討 */
/**/        case    COM_MSG_EC44  :   /* ダイレクト送信接続完了イベント */
/**/                    if(ERROR == (Status = iomnRecieveEC44())){
/**/                        cmnPrintf_ioMN(2,">>>Error:iomnRecieveEC44()\n");
/**/                        return(ERROR);
/**/                    }
/**/                    break;
#endif
#if 0   /* MCK1.1 新メッセージ対応  */
/**/        case    IOMSG_EPSE_EX   :   /* １ページ入力終了イベント */
/**/        case    IOMSG_ENXT_EX   :   /* 時原稿有無イベント */
/**/        case    IOMSG_EPNS_JOB  :   /* 次ページ入力イベント */
#else
        case    JOB_MSG_ESOS:   /* １ページ入力終了イベント */
        case    JOB_MSG_ESNT:   /* 時原稿有無イベント       */
        case    JOB_MSG_EFSR:   /* 次ページ入力イベント     *//* V010.00_04 */
/*        case    JOB_MSG_RFSR:   *//* 次ページ入力イベント     */
#endif
                    if(ERROR == (Status = iomnRecieveEvent(iioMNMsgId))){
                        cmnPrintf_ioMN(2,">>>Error:iomnRecieveEvent()\n");
                        return(ERROR);
                    }
                    break;
#if 0   /* MCK1.1 新メッセージ対応  *//* 要検討 */
/**/        case    IOMSG_EIST_JOB:  /* iFax転送開始 */
/**/                    iomnFAXLineBusy();
/**/                    break;
/**/
/**/        case    IOMSG_EIED_JOB:  /* iFax転送終了 */
/**/                    iomnFAXLineOff();
/**/                    break;
#endif
#if 1 /* RioGrande マニュアル対応 */
        case JOB_MSG_RIST:      /* マニュアル動作通知 */
            if(ERROR == (Status = iomnRecieveRIST())){
                iomnDebugMessage("Error:iomnRecieveRIST()\n",0);
                return(ERROR);
            }
            iomnDebugMessage("OK:iomnRecieveRIST()\n",0);
            break;
        case JOB_MSG_AIST:      /* マニュアル動作開始応答 */
            if(ERROR == (Status = iomnRecieveAIST())){
                iomnDebugMessage("Error:iomnRecieveAIST()\n",0);
                return(ERROR);
            }
            iomnDebugMessage("OK:iomnRecieveAIST()\n",0);
            break;
        case JOB_MSG_TIFN:      /* マニュアル動作開始結果 */
            if(ERROR == (Status = iomnRecieveTIFN())){
                iomnDebugMessage("Error:iomnRecieveTIFN()\n",0);
                return(ERROR);
            }
            iomnDebugMessage("OK:iomnRecieveTIFN()\n",0);
            break;
#endif
        default :
                    iomnDebugMessage(">>>Undefined Message ID!!!\n",0);
                    break;
    }
/* 041207Daily *//* START */
    switch(iioMNMsgId){
        case    JOB_MSG_EPST:   /* 印刷開始通知     */
                    /*  印刷開始時刻チェック    */
                    if(ERROR == (Status = iomnCheckPrnTime( IOMN_OFF ))){
                        return(ERROR);
                    }
                    break;
	 case	JOB_MSG_EQNE: /* L4.9 Feedback of MDS requirement EBX_DCR_33145 */
	 		if(ERROR == (Status = iomnReceiveEQNE())){
                        cmnPrintf_ioMN(2,">>>Error: iomnRecieveEQNE ()\n");
                        return(ERROR);
                    }
                    break;
        default :
                    break;
    }
/* 041207Daily *//* END   */
    return(OK);
}
/* 10911 fix*/
STATUS iomnRecJobReg()
{

	HI hJobID;
	HI hJobType;
	HI hProcessType;
/*EBX_DTFR_19655*/
#if defined(LOIRE2_ENG) || defined(AL2_ENG) || defined(WEISS_ENG) || defined(ECO_LOIRE2)
	static HI hIsFirstJob = TRUE; 
#endif
	STATUS Status;
	
    hJobID          = ioMNrmyparam.sRjreCtl.hJobID;
	hProcessType	= ioMNrmyparam.sRjreCtl.hProcessType;


    ioMNRjre[hJobID].iOldTaskId = iioMNTaskId;  
    ioMNRjre[hJobID].iOldMsgId  = iioMNMsgId;   
    ioMNRjre[hJobID].OldmsgQId  = ioMNMsgQId;

	ioMNRjre[hJobID].msgparam.sRjreCtl.hJobID = hJobID;
	ioMNRjre[hJobID].msgparam.sRjreCtl.hProcessType = hProcessType;

	cmnPrintf_ioMN(7,"### iomnRecJobReg(): JOBID->%d, hProcessType: %d \n", (I32)hJobID, hProcessType);

	if(ERROR == (Status = jobJobtype(hJobID, &hJobType))){
		cmnPrintf_ioMN(2,">>>Error:jobJobtype() in iomnPrnJobStart()\n");
		return(ERROR);
	}
	iomnDebugMessage("### JOB TYPE->%x\n",(I32)hJobType);

	if((iomnCheckSCS(hJobID) != TRUE ) && ( hJobType == COM_JB_IPPC || hJobType == COM_JB_EXPPC2 )){
		if( iomnCpyMNStorePrnJob(hJobID) == ERROR ){
			iomnDebugMessage(">> ioMN >> ERROR : Entry Copy Print!!\n",0);
#if 0			
			if(ERROR == (Status = iomnSendTjfn(hJobID, IOMSG_FIN_ERROR, IOMSG_JB_ENTRYERR, IOMSG_NOT_USED))){
				iomnDebugMessage(">>>Error:iomnSendJobFinishSts()\n",0);
				return(ERROR);
			}
#else
			jobSetStatus(hJobID, COM_JS_SUSPEND, IOMSG_QUE_ENTRY_ERROR);
			cmnPrintf_ioMN(8,"iomnRecJobReg(): que entry error; hence jobstatus is set to COM_JS_SUSPEND and cause IOMSG_QUE_ENTRY_ERROR\n");
#endif
#if 0 /* this is called on CJST, if suscause is IOMSG_QUE_ENTRY_ERROR*/
			iomnCleanJobManagementMsg(hJobID); 
#endif
			return(ERROR);
		}
	}else{
#if 0
		if( iomnPrepareForNewJob(hJobID, hJobType, hContinuity) == ERROR ) {
			cmnPrintf_ioMN(2,  ">> ioMN >> ERROR:iomnPrepareForNewJob() in iomnPrnJobStart()\n");
			return( ERROR );
		}
#endif
		if( iomnPrnJobCheckEntry(hJobID, hJobType ) == ERROR ){
#if 0
			if( hJobType == COM_JB_LIST_RESULT_SEND ){
				iomnDebugMessage(">>>Error:iomnPrnJobCheckEntry()\n",0);

				if(ERROR == (Status = iomnSendJobFinishSts(ioMNCjst[hJobID].OldmsgQId, imsgid,
												hJobID, IOMSG_FIN_ERROR, IOMSG_ERR_JOB_START, IOMSG_NOT_USED, hProcessNo))){

					iomnDebugMessage(">>>Error:iomnSendJobFinishSts()\n",0);
					return(ERROR);
					}

				iomnCleanJobManagementMsg(hJobID);
				iomnDebugMessage(">>>OK:iomnCleanJobManagementMsg()\n",0);
				return(OK);
			}
			else
		 {
				cmnPrintf_ioMN(2, ">> ioMN >> ERROR:iomnPrnQueJobEntry() in iomnPrnJobStart()\n");
				if( ioMNstatus.hDssDataTrans == IOMN_ON ) {
					if((siIdx_no = jcLibJCB_tnoidx(ioMNCjst[hioMNDataRecvJobID].msgparam.sCjstCtl.hTNo)) == JCLIB_NOEXIST){
						cmnPrintf_ioMN(2, ">> ioMN >> ERROR:jcLibJCB_tnoidx() in iomnSyncTblLinkOutputJob()\n" );
					}else{
						if((sJobTbl = (JOB_EXECDATA_TBL*) jcLibTBL_getp(JCLIB_EXC_TBL, siIdx_no)) == JCLIB_NOEXIST){
							cmnPrintf_ioMN(2, ">> ioMN >> ERROR:jcLibTBL_getp() in iomnSyncTblLinkOutputJob()\n" );
						}else{
							if(sJobTbl->sCommon.hOutJobId != hJobID){
								/* 処理不要 */
							}else{
								iomnSyncTblClearJobID(iomnJobInfo[hioMNDataRecvJobID].hIpTblID, COM_JB_SUB_OUTPUT);
							}
							jcLibUTY_memfree((void *) sJobTbl);
						}
					}
				}
				/*	ＪＯＢ完了メッセージ送信	*/
				if(ERROR == (Status = iomnSendTjfn(hJobID, IOMSG_FIN_ERROR, IOMSG_JB_ENTRYERR, IOMSG_NOT_USED))){
					iomnDebugMessage(">>>Error:iomnSendJobFinishSts()\n",0);
					return(ERROR);
				}
				iomnCleanJobManagementMsg(hJobID);
				return(OK);
			}
#endif
			/* DCR_46831 starts */
			int hMode = sysmnGetHMode();
			MCNTBL_PRN_STSTBL  sMcnPrnSts ;  
		    STATUS st = mcnTBLPRNStsTbl( &sMcnPrnSts ) ; 
			
			if(OK == st){  //Implies Job Registration Failed bcoz of SYSMN_RSLOCK
        		if(((hMode == IOMSG_STAT_SYSRESET) || (hMode == IOMSG_STAT_SHUTDOWN)) && (sMcnPrnSts.hPrnSCCnt >0)){
			       jobSetStatus(hJobID, COM_JS_SUSPEND, IOMSG_ERR_REG_RSLOCK);
        	       cmnPrintf_ioMN(8,"iomnRecJobReg: que entry error; hence jobstatus is set to COM_JS_SUSPEND and cause IOMSG_ERR_REG_RSLOCK\n");
			       return(ERROR);
			    }
			}
			/* DCR_46831 ends */
			jobSetStatus(hJobID, COM_JS_SUSPEND, IOMSG_QUE_ENTRY_ERROR);
			cmnPrintf_ioMN(8,"iomnRecJobReg(): que entry error; hence jobstatus is set to COM_JS_SUSPEND and cause IOMSG_QUE_ENTRY_ERROR\n");
#if 0 /* this is called on CJST, if suscause is IOMSG_QUE_ENTRY_ERROR*/
			iomnCleanJobManagementMsg(hJobID);
#endif
			return(ERROR);
		}
	}
#if 0
	if(ioMNstatus.memfull_sts == IOMN_ON && hJobtype == COM_JB_DRC_PRT_FAX){

		Status = iomnMemFullAcceptConfirm(hJobID);

		if(Status != OK ){

			if(ERROR == (Status = iomnSendTJED(ioMNCjst[hJobID].OldmsgQId,
									JOB_MSG_TJSX, hJobID, IOMSG_SUS_ERROR,
									Status, IOMSG_NOT_USED, 0)))
			{
				iomnDebugMessage(">>>Error:iomnSendTJED()\n",0);
				return(ERROR);
			}
			iomnDebugMessage(">>>Send TJED MESSAGE\n",0);
		}
		else{
			iomnDebugMessage(">>>OK:Fax Memory Full Print Acceptable\n",0);
		}
	}
	if(ERROR == (Status = iomnCheckPrnTime( IOMN_OFF ))){
		return(ERROR);
	}
#endif
	
	jobSetStatus(hJobID, COM_JS_SUSPEND, IOMSG_SUS_REG);

/*EBX_DTFR_19655*/
#if defined(LOIRE2_ENG) || defined(AL2_ENG) || defined(WEISS_ENG) || defined(ECO_LOIRE2)
	if( hIsFirstJob == TRUE )
	{
		hIsFirstJob = FALSE;
		mcnMNCheckSilentRebootStatus();
	}
#endif
	iomnDebugMessage(">>> iomnRecJobReg() End\n",0);


	return OK;
	


}
/* 10911 fix*/

STATUS iomnRecCancelJobReg()
{

	HI hJobID;
	HI hProcessType;
	
    hJobID          = ioMNrmyparam.sRjcnCtl.hJobID;
	hProcessType	= ioMNrmyparam.sRjcnCtl.hProcessType;

    ioMNRjcn[hJobID].iOldTaskId = iioMNTaskId;  
    ioMNRjcn[hJobID].iOldMsgId  = iioMNMsgId;   
    ioMNRjcn[hJobID].OldmsgQId  = ioMNMsgQId;

	ioMNRjcn[hJobID].msgparam.sRjcnCtl.hJobID = hJobID;
	ioMNRjcn[hJobID].msgparam.sRjcnCtl.hProcessType = hProcessType;

	cmnPrintf_ioMN(7,"### iomnRecCancelJobReg(): JOBID->%d, hProcessType: %d\n", (I32)hJobID, hProcessType);

    if(ERROR == iomnPrnQueJobDelete( hJobID , hIoMNPrnQueEntryID ))
   	{
		cmnPrintf_ioMN(8,"iomnRecCancelJobReg(): error in que delete \n");
		return ERROR;
   	}

	iomnCleanJobManagementMsg(hJobID);
	
	iomnDebugMessage(">>> iomnRecCancelJobReg() End\n",0);
	return OK; /* L4.5 Prevent Tool changes f/b to L3.7*/
}

/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         7.1
TITLE:      JOB開始指示受信処理
MODULE：    STATUS  iomnRecJobStart()

INCLUDE:    ioMNPar.h
PARAMETER:
RETURNS:    I32 4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            JOB開始指示メッセージ受信時の処理。
NOTES:
HISTORY:
        1.1     Y.Matsuda   96/9/18
        M1.1(TJ)H.Takahashi 98/09/25
                DSS逐次対応
        D1.1    T.Fujii    99/11/15 DM45/35後追い逐次対応
        D1.4    Y.Shimada  00/ 1/25 DM45/35 フォームストレージ変更：ＩｐＬｉｂ／逐次制御の組み込み
        D1.7.1  J.Ootani   00/03/01 DM45/35 SEINE FAX F-CODE対応(マージ)
        D1.7.12 J.Ootani   00/04/19 DM45/35 リスト印刷不具合対応
        D2.0.1  T.Fujii    00/06/06 DM45/35 NOP_DEL(ソースコード整理)
                                            case文でのCOM_JB_BPPC,COM_JB_EXPPC1を削除
        D2.9    M.Fukuda   00/07/04 DM45/35 FAXリストレポート対応
        D2.12   T.Fujii    00/07/05 DM45/35 メッセージパラメータ保存処理再修正
        D4.7.2  T.Fujii    00/11/21 DM45/35 プリンタW-UP中の2つ目のFAX/DSS入力ジョブでロックする
                                            不具合修正

        MCK1.1  M.Karasawa  02/06/11    McKinley    新メッセージ対応
    V018.00_01  M.Karasawa  03/04/09    McKinley    メッセージ変更対応
*/
STATUS  iomnRecJobStart()
{

    HI      hJobID;         /*  JOBID   */
    HI      hStartTrigger;  /*  開始要因    */
    HI      hContinuity;    /*  継続or新規  */
    HI      hJobType;       /*  JOB種類 */
    HI      hPrtType=0;     /*  GDIJOB印刷時の継続印刷指定  */    /* D1.1 とりあえず0を設定 */

    /* MCK1.1 新メッセージ メンバー対応 START   */
    HI      hJcbID;
    HI      hTNo;
    HI      hIndiType;
    HI      hProcessNo;
    /* MCK1.1 END   */

    STATUS  Status;         /*  ＪＯＢ状態変更結果  */

    /*  Get Recieve Message */
    hJobID          = ioMNrmyparam.sCjstCtl.hJobid;
    hStartTrigger   = ioMNrmyparam.sCjstCtl.hTrigger;
    hContinuity     = ioMNrmyparam.sCjstCtl.hContinue;
    /* この位置にhPrtTypeがある     */
    /* MCK1.1 新メッセージ メンバー対応 START   */
    hJcbID          = ioMNrmyparam.sCjstCtl.hJcbID;
    hTNo            = ioMNrmyparam.sCjstCtl.hTNo;
    hIndiType       = ioMNrmyparam.sCjstCtl.hIndiType;
    hProcessNo      = ioMNrmyparam.sCjstCtl.hProcessType;
    /* MCK1.1 END   */
    /* この位置にhfaxSndQueIdがある */
	cmnPrintf_ioMN(7,"### JOBID->%x, StartTrigger->%x, Continuity->%x, Print Type->%x, JCB ID->%x, TNo->%x, \
						  IndiType->%x, ProcessNo->%x\n", (I32)hJobID, (I32)hStartTrigger, (I32)hContinuity, \
						  (I32)hPrtType, (I32)hJcbID, (I32)hTNo, (I32)hIndiType, (I32)hProcessNo);
	cmnPrintf_ioMN(7,"### FaxSndQueId->%x, AppliType->%x, Cassette->%x\n", (I32)ioMNrmyparam.sCjstCtl.hfaxSndQueId, \
						  (I32)ioMNrmyparam.sCjstCtl.hAppliType, (I32)ioMNrmyparam.sCjstCtl.hCassette);

    /*  JOB種類取得 */
    if(ERROR == (Status = jobJobtype(hJobID, &hJobType))){
        cmnPrintf_ioMN(2,">>>Error:jobJobtype() in iomnRecJobStart())\n");
        printErrno(errno);
        return(ERROR);
    }

    /*  JOB開始指示パラメータの格納 */
    if( hContinuity==IOMSG_JB_NEW || hContinuity==IOMSG_JB_NEW_TRK ) {
        iomnStoreCmdSender( hJobID );   /* コマンド送信元の保存 */
        /* D4.7.2 START T.Fujii メッセージ受信情報クリア処理追加 */
        /* メッセージ受信情報の一部をクリアする */
        memset( &ioMNTjfn[hJobID], 0x00, sizeof(struct msgstore) );
        /* D4.7.2 END */
    }
    iomnStoreCjstParam( hJobID );   /* CJSTパラメータの保存 */

    /*  各種JOB開始処理 */
    switch(hJobType){
        case    COM_JB_IPPC:
        case    COM_JB_EXPPC2:
                if( hJobID <= IOMN_INPUTJOB_ID_MAX ) {  /* jobIDが入力用ID以内ならコピー入力ジョブとみなす */
#if 0   /* MCK1.1 新メッセージ対応  引数追加    */
/**/                    if(ERROR == (Status = iomnPPCJobStart(hJobID, hStartTrigger, hContinuity, hJobType))){
#else
                    if(ERROR == (Status = iomnPPCJobStart(hJobID, hStartTrigger, hContinuity, hJobType, hProcessNo))){
#endif
                        iomnDebugMessage(">> ioMN >> Error:iomnPPCJobStart()\n",0);
                        return(ERROR);
                    }
                }
                else {  /* jobIDが入力用以外なら印刷ジョブとみなす */
#if 0   /* MCK1.1 新メッセージ対応  引数追加    */
/**/                    if(ERROR == (Status = iomnPrnJobStart  (hJobID, hStartTrigger, hContinuity, hPrtType, hJobType))){
#else
                    if(ERROR == (Status = iomnPrnJobStart  (hJobID, hStartTrigger, hContinuity, hPrtType, hJobType, hProcessNo))){
#endif
                        iomnDebugMessage(">> ioMN >> Error:iomnPrnJobStart()\n",0);
                        return(ERROR);
                    }
                }
                break;
        case    COM_JB_SCN_FAX1:
        case    COM_JB_SCN_FAX2:
        case    COM_JB_CON_REG_FAX2:
        case    COM_JB_BOD_REG_FAX2:
        case    COM_JB_POLL_SEND_FAX1:
        case    COM_JB_POLL_SEND_FAX2:
#if 0   /* MCK1.1 新メッセージ対応  引数追加    */
/**/                if(ERROR == (Status = iomnFaxScnJobStart(hJobID, hStartTrigger, hContinuity, hJobType))){
#else
                if(ERROR == (Status = iomnFaxScnJobStart(hJobID, hStartTrigger, hContinuity, hJobType, hProcessNo))){
#endif
                    iomnDebugMessage(">>>Error:iomnFaxPrnJobStart()\n",0);
                    return(ERROR);
                }
                break;
        case    COM_JB_DRC_SCN_FAX:
#if 0   /* MCK1.1 新メッセージ対応  引数追加    */
/**/                if(ERROR == (Status = iomnDirectFaxStart(hJobID, hStartTrigger, hContinuity, hJobType))){
#else
                if(ERROR == (Status = iomnDirectFaxStart(hJobID, hStartTrigger, hContinuity, hJobType, hProcessNo))){
#endif
                    iomnDebugMessage(">>>Error:iomnDirectFaxStart()\n",0);
                    return(ERROR);
                }
                break;
        case    COM_JB_LIST_PRT_FAX:
                /*  フロント指示によるリスト／レポート印刷  */
        case    COM_JB_LIST_RESULT_SEND:
                /*  バックグランド時のリスト／レポート送信  */
                if(hJobID == hioMNFrontJobID){
#if 0   /* MCK1.1 新メッセージ対応  引数追加    */
/**/                    if(ERROR == (Status = iomnIVPrnJobStart(hJobID, hStartTrigger, hContinuity, hJobType))){
#else
                    if(ERROR == (Status = iomnIVPrnJobStart(hJobID, hStartTrigger, hContinuity, hJobType, hProcessNo))){
#endif
                        iomnDebugMessage(">>>Error:iomnIVPrnJobStart()\n",0);
                        return(ERROR);
                    }
                    iomnDebugMessage(">>>OK:iomnIVPrnJobStart()\n",0);
                }else{
                    /*  バックグランド時のリスト／レポート印刷  */
                    if( parMNSetIOMode(hJobID,COM_OT_TRK_FIN) == ERROR){
                        cmnPrintf_ioMN(2,">>>error:parMNSetIOMode() hJobID=%d\n",hJobID);
                    }
#if 0   /* MCK1.1 新メッセージ対応  引数追加    */
/**/                    if(ERROR == (Status = iomnPrnJobStart(hJobID, hStartTrigger, hContinuity, hPrtType, hJobType))){
#else
                    if(ERROR == (Status = iomnPrnJobStart(hJobID, hStartTrigger, hContinuity, hPrtType, hJobType, hProcessNo))){
#endif
                        cmnPrintf_ioMN(2,">>>Error:iomnPrnJobStart()\n");
                        return(ERROR);
                    }
                }
                break;
        case    COM_JB_CON_GET_FAX2:
        case    COM_JB_BOD_GET_FAX2:
        case    COM_JB_DRC_PRT_FAX:
        case    COM_JB_PRT_FAX1:
        case    COM_JB_PRT_FAX2:
        case    COM_JB_POLL_RECV_FAX1:
        case    COM_JB_POLL_RECV_FAX2:
        case    COM_JB_PRT_GDI1:
        case    COM_JB_PRT_GDI2:
        case    COM_JB_DRC_PRT_GDI:
#if 0   /* MCK1.1 新メッセージ対応  引数追加    */
/**/                if(ERROR == (Status = iomnPrnJobStart(hJobID, hStartTrigger, hContinuity, hPrtType, hJobType))){
#else
                if(ERROR == (Status = iomnPrnJobStart(hJobID, hStartTrigger, hContinuity, hPrtType, hJobType, hProcessNo))){
#endif
                    iomnDebugMessage(">>>Error:iomnPrnJobStart()\n",0);
                    return(ERROR);
                }
                break;
        case    COM_JB_IVSAVE_PPC:
        case    COM_JB_IVSAVE_FAX:
#if 0   /* MCK1.1 新メッセージ対応  引数追加    */
/**/                if(ERROR == (Status = iomnIVSaveJobStart(hJobID, hStartTrigger, hContinuity, hJobType))){
#else
                if(ERROR == (Status = iomnIVSaveJobStart(hJobID, hStartTrigger, hContinuity, hJobType, hProcessNo))){
#endif
                    iomnDebugMessage(">>>Error:iomnIVSaveJobStart()\n",0);
                    return(ERROR);
                }
                break;
        case    COM_JB_IVPRM_PPC:
        case    COM_JB_IVPRM_FAX:
                if( hContinuity == IOMSG_JB_NEW ) {
                    /* 新規 */
#if 0   /* MCK1.1 新メッセージ対応  引数追加    */
/**/                    if(ERROR == (Status = iomnIVPrnJobStart(hJobID, hStartTrigger, hContinuity, hJobType))){
#else
                    if(ERROR == (Status = iomnIVPrnJobStart(hJobID, hStartTrigger, hContinuity, hJobType, hProcessNo))){
#endif
                        iomnDebugMessage(">>>Error:iomnIVPrnJobStart()\n",0);
                        return(ERROR);
                    }
                }else{
                    /* 中断／再開 */
#if 0   /* MCK1.1 新メッセージ対応  引数追加    */
/**/                    if(ERROR == (Status = iomnPrnJobStart(hJobID, hStartTrigger, hContinuity, hPrtType, hJobType))){
#else
                    if(ERROR == (Status = iomnPrnJobStart(hJobID, hStartTrigger, hContinuity, hPrtType, hJobType, hProcessNo))){
#endif
                        iomnDebugMessage(">> ioMN >> Error:iomnPrnJobStart()\n",0);
                        return(ERROR);
                    }
                }
                break;
        case    COM_JB_SEND_DSI:
#if 0   /* MCK1.1 新メッセージ対応  引数追加    */
/**/                if(ERROR == (Status = iomnDssScnJobStart(hJobID, hStartTrigger, hContinuity, hJobType))){
#else
                if(ERROR == (Status = iomnDssScnJobStart(hJobID, hStartTrigger, hContinuity, hJobType, hProcessNo))){
#endif
                    iomnDebugMessage(">>>Error:iomnDssScnJobStart()\n",0);
                    return(ERROR);
                }
                break;
        case    COM_JB_RECV_DSI:
        case    COM_JB_PRIVATE_DSI:
        case    COM_JB_RECV_DSI_SYNC:
        case    COM_JB_PRIVATE_DSI_SYNC:
                /* MCK1.1 メッセージ統合により、入出力の判断はプロセスで行なう  */
                if(hProcessNo == JP_RIP){
                    if(ERROR == (Status = iomnRecieveRDDS())){
                        cmnPrintf_ioMN(2,">>>Error:iomnRecieveRDDS()\n");
                        return(ERROR);
                    }
                }else{  /* MCK1.1   */
#if 0   /* MCK1.1 新メッセージ対応  引数追加    */
/**/                    if(ERROR == (Status = iomnPrnJobStart(hJobID, hStartTrigger, hContinuity, hPrtType, hJobType))){
#else
                    if(ERROR == (Status = iomnPrnJobStart(hJobID, hStartTrigger, hContinuity, hPrtType, hJobType, hProcessNo))){
#endif
                        iomnDebugMessage(">>>Error:iomnPrnJobStart()\n",0);
                        return(ERROR);
                    }
                }       /* MCK1.1   */
                break;
        case    COM_JB_LIST_SEND_FAX:
        case    COM_JB_JOB_UI:
        case    COM_JB_UNDEF:
        case    COM_JB_RECV_FAX:
        case    COM_JB_DIG:
        default:
                iomnDebugMessage(">>>Error:Undefined Job_Type -->%x\n", (I32)hJobType);
                break;
    }
    return(OK);
}


/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         7.1.1
TITLE:      複写JOB開始処理
MODULE：    STATUS  iomnPPCJobStart(HI hJobID, HI hStartTrigger, HI hContinuity, HI hJobtype)

INCLUDE:    ioMNPar.h
            ioMNstr.h
            ioMNbuf.h
PARAMETER:
            HI      hJobID;             JOBID   
            HI      hStartTrigger;      開始要因
            HI      hContinuity;        継続or新規  
            HI      hJobtype;           JOB種別
RETURNS:    I32 4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            JOB開始指示メッセージ受信時の処理。
NOTES:
HISTORY:
        1.1     Y.Matsuda   96/9/18
        2.1     y.Matsuda   97/11/15
                メッセージすれ違いにより異なる非優先ＵＩからＪＯＢ開始を
                指示された場合の対策追加。バックグランド印刷開始時に複写開始を
                指示された時に、ＪＯＢ開始後複写ＵＩキーロックされない不具合対応
                case    S_ioMN_NOT_PRIMARY_UI:
        2.2     yMatsuda    
                PM12MB時対応追加    97/11/20
                case    S_ioMN_12MB_GDI_RECIEVE:
                case    S_ioMN_12MB_GDI_EXIST:
        3.1     yMatsuda    98/02/26
                原稿戻し枚数パラメータにNULLではなく、IOMSG_NOT_USEDを設定する
        M1.1    (TJ)H.Takahashi 98/7/17
                (追加)手置き逐次型複写対応のため
        M1.2    (TJ)H.Takahashi 98/11/04
                割り込み複写対応
        D1.1    T.Fujii 99/11/15 DM45/35 後追い逐次・並行動作対応
        D2.4.1  T.Fujii 00/06/17 DM45/35 V004.70不具合対応
                                         AJSTパラメータ拡張対応修正
        D2.4.8  T.Fujii                  5Jobフルでフロント中断中になる不具合対応
        D3.19   T.Fujii 00/09/12 DM45/35 コーディングミス修正
        D5.3    T.Fujii 01/03/30 DM45/35 割り込み複写ジョブ開始不可能時にEB46も送信するよう修正
        D5.3.1  T.Fujii 01/05/08 DM45/35 割り込み複写ジョブ開始不可能時のジョブ情報クリア処理削除

        MCK1.1  M.Karasawa  02/06/11    新メッセージ対応
    V018.00_01  M.Karasawa  03/04/09    McKinley    メッセージ変更対応
    V019.00_02  M.Karasawa  03/05/20    McKinley    メッセージメンバ追加対応（TJSX追加、EPSTとEJABは対応不要）
    V023.00_01  M.Karasawa  03/07/16    McKinley    エラー終了時TJFNに通し番号が入らない不具合の修正
*/
#if 0   /* MCK1.1 新メッセージ対応  引数追加    */
/**/STATUS  iomnPPCJobStart(HI hJobID, HI hStartTrigger, HI hContinuity, HI hJobtype)
#else
STATUS  iomnPPCJobStart(HI hJobID, HI hStartTrigger, HI hContinuity, HI hJobtype, HI hProcessNo)
#endif
{
    STATUS  Status;         /*  各種処理結果    */
#if 0   /* MCK1.1 新メッセージ対応  */
/**/    I32     imsgid=IOMSG_CJST_UI;       /* JOB完了メッセージID */   /* D1.1ダミー定義追加 */
/**/    I32     imsgidTjed=IOMSG_TJED_UI;   /* JOB終了メッセージID */   /* D1.1ダミー定義追加 */
#else
    I32     imsgid     = JOB_MSG_TJFN;
    I32     imsgidTjed = JOB_MSG_TJSX;
#endif
    HI      hJobstatus;                 /* JOB状態 */
    I32     iSuscause;                  /* 中断要因 */
    STATUS  sStatus;

    /*  messageID取得   */
#if 0   /* MCK1.1 新メッセージ対応  */
/**/    if(ioMNCjst[hJobID].iOldMsgId == IOMSG_CJST_UI){
/**/        imsgid = IOMSG_TJFN_UI;
/**/        imsgidTjed = IOMSG_TJED_UI;
/**/    }
/**/    if(ioMNCjst[hJobID].iOldMsgId == IOMSG_CJST_JOB){
/**/        imsgid = IOMSG_TJFN_JOB;
/**/        imsgidTjed = IOMSG_TJED_JOB;
/**/    }
/**/    if(ioMNCjst[hJobID].iOldMsgId == IOMSG_CJST_DIG){
/**/        imsgid = IOMSG_TJFN_DIG;
/**/    }
#endif

    /*  JOB状態確認 */
    if(ERROR == (Status = jobStatus(hJobID, &hJobstatus, &iSuscause))){
        iomnDebugMessage(">>>Error:jobStatus()\n",0);
        return(ERROR);
    }
    iomnDebugMessage("###JOB STATUS->%x\n",(I32)hJobstatus);

    /*  ＰＰＣＪＯＢ実行可否確認    */
    Status = iomnPPCStartConfirm(hJobID);
    switch(Status){
        case    IOMN_ON:
                /*  ＪＯＢ開始予約フラグが設定されている場合は中断終了まで開始保留  */
                if(ioMNstatus.hJobStart == IOMN_ON){
                    iomnDebugMessage(">>>Wait... Back Job Suspending\n",0);

                    /*  JOB中断後の再開情報格納 */
                    iomnSetStartQue(&ioMNFrontQue, hJobID, hStartTrigger, hContinuity, 0, hJobtype);
                    iomnDebugMessage(">>>OK:iomnSetStartQue()\n",0);
                    ioMNstatus.hJobStart = IOMN_OFF;    
                    return(OK);
                }
                else{
                    iomnDebugMessage(">>>PPC JOB Start\n",0);
                }
                break;
        case    S_ioMN_MACHINE_RECOVERY:  /*  マシンリカバリ中の場合      */
        case    S_ioMN_ENGINE_ERROR:      /*  Error発生中の場合           */
        case    S_ioMN_TEST_STAPLE:       /*  テストステープル中の場合    */
                iomnDebugMessage(">>>Impossible to Job Start:%x\n",Status);
                /*  新規ＪＯＢ開始または基本複写の場合  */
                if(hContinuity == IOMSG_JB_NEW ){
                    /*  TJFN Messageの登録  */
                    ioMNTjfn[hJobID].iOldTaskId = iTaskIdIoExMN;
#if 0   /* MCK1.1 新メッセージ対応  */
/**/                    ioMNTjfn[hJobID].iOldMsgId = IOMSG_TJFN_EX;
#else
                    ioMNTjfn[hJobID].iOldMsgId = JOB_MSG_TJFN;
#endif
                    ioMNTjfn[hJobID].OldmsgQId = msgQIdIoExMN;
                    ioMNTjfn[hJobID].msgparam.sTjfnCtl.hJobid = hJobID;
                    ioMNTjfn[hJobID].msgparam.sTjfnCtl.hSts = IOMSG_FIN_ERROR;
                    ioMNTjfn[hJobID].msgparam.sTjfnCtl.iErrSys = IOMSG_NOT_USED;
                    ioMNTjfn[hJobID].msgparam.sTjfnCtl.iErrEng = Status;
                    /* MCK1.1 新メッセージ対応  */
                    ioMNTjfn[hJobID].msgparam.sTjfnCtl.hProcessType = hProcessNo;
                    /* V018.00_01 START */
                    ioMNTjfn[hJobID].msgparam.sTjfnCtl.hJcbID     = ioMNCjst[hJobID].msgparam.sCjstCtl.hJcbID;
                    ioMNTjfn[hJobID].msgparam.sTjfnCtl.hTNodel    = IOMSG_NOT_USED;
                    ioMNTjfn[hJobID].msgparam.sTjfnCtl.hUkedel    = IOMSG_NOT_USED;
                    ioMNTjfn[hJobID].msgparam.sTjfnCtl.hAppliType = ioMNCjst[hJobID].msgparam.sCjstCtl.hAppliType;
                    /* V018.00_01 END   */
                    /* V023.00_01 START */
                    ioMNTjfn[hJobID].msgparam.sTjfnCtl.hTNo       = ioMNCjst[hJobID].msgparam.sCjstCtl.hTNo;
                    /* V023.00_01 END   */
                    iomnDebugMessage(">>>Set TJFN Message\n",0);

                    iomnReleasePrn(hJobID);/* VTR11 */

                    /*  ＪＯＢ完了メッセージ送信    */
                    if(ERROR == (Status = iomnSendJobFinishSts(ioMNCjst[hJobID].OldmsgQId, imsgid,
#if 0   /* MCK1.1 新メッセージ対応  引数追加    */
                                                hJobID, IOMSG_FIN_ERROR, IOMSG_NOT_USED, Status))){
#else
                                                hJobID, IOMSG_FIN_ERROR, IOMSG_NOT_USED, Status, hProcessNo))){
#endif
                        iomnDebugMessage(">>>Error:iomnSendJobFinishSts()\n",0);
                        return(ERROR);
                    }
                    iomnDebugMessage(">>>Send JOB Finish Message\n",0);
                }
                /*  ＪＯＢ再開の場合    */
                else{
                    /*  Ack Message登録 */
                    ioMNAjst[hJobID].iOldTaskId = iTaskIdIoExMN;
#if 0   /* MCK1.1 新メッセージ対応  */
/**/                    ioMNAjst[hJobID].iOldMsgId = IOMSG_AJST_EX;
#else
                    ioMNAjst[hJobID].iOldMsgId = JOB_MSG_AJST;
#endif
                    ioMNAjst[hJobID].OldmsgQId = msgQIdIoExMN;

                    ioMNAjst[hJobID].msgparam.sAjstCtl.hJobid       = hJobID;
                    ioMNAjst[hJobID].msgparam.sAjstCtl.hInUpDown    = IOMSG_NOT_USED;
                    ioMNAjst[hJobID].msgparam.sAjstCtl.hOutTiming   = IOMSG_NOT_USED;
                    ioMNAjst[hJobID].msgparam.sAjstCtl.iErrStatus   = OK;
                    /* V018.00_01 START */
                    ioMNAjst[hJobID].msgparam.sAjstCtl.hWarSts      = IOMSG_NOT_USED;
                    ioMNAjst[hJobID].msgparam.sAjstCtl.iWarErrSys   = IOMSG_NOT_USED;
                    ioMNAjst[hJobID].msgparam.sAjstCtl.iWarErrEng   = IOMSG_NOT_USED;
                    ioMNAjst[hJobID].msgparam.sAjstCtl.hTNo         = ioMNCjst[hJobID].msgparam.sCjstCtl.hTNo;
                    ioMNAjst[hJobID].msgparam.sAjstCtl.hProcessType = hProcessNo;
                    ioMNAjst[hJobID].msgparam.sAjstCtl.hJcbID       = ioMNCjst[hJobID].msgparam.sCjstCtl.hJcbID;
                    ioMNAjst[hJobID].msgparam.sAjstCtl.hAppliType   = ioMNCjst[hJobID].msgparam.sCjstCtl.hAppliType;
                    /* V018.00_01 END   */
                    iomnDebugMessage(">>>Set AJST Message\n",0);

                    /*  TJED Messageの登録  */
                    ioMNTjed[hJobID].iOldTaskId = iTaskIdIoExMN;
#if 0   /* MCK1.1 新メッセージ対応  */
/**/                    ioMNTjed[hJobID].iOldMsgId = IOMSG_TJED_EX;
#else
                    ioMNTjed[hJobID].iOldMsgId = JOB_MSG_TJSX;
#endif
                    ioMNTjed[hJobID].OldmsgQId = msgQIdIoExMN;
                    ioMNTjed[hJobID].msgparam.sTjedCtl.hJobid       = hJobID;
                    ioMNTjed[hJobID].msgparam.sTjedCtl.hSts         = IOMSG_SUS_ERROR;
                    ioMNTjed[hJobID].msgparam.sTjedCtl.iErrSys      = IOMSG_NOT_USED;
                    ioMNTjed[hJobID].msgparam.sTjedCtl.iErrEng      = Status;
                    ioMNTjed[hJobID].msgparam.sTjedCtl.hOrgBack     = IOMSG_NOT_USED;
                    /* V018.00_01 START */
                    ioMNTjed[hJobID].msgparam.sTjedCtl.hTNo         = ioMNCjst[hJobID].msgparam.sCjstCtl.hTNo;
                    ioMNTjed[hJobID].msgparam.sTjedCtl.hProcessType = hProcessNo;
                    ioMNTjed[hJobID].msgparam.sTjedCtl.hJcbID       = ioMNCjst[hJobID].msgparam.sCjstCtl.hJcbID;
                    ioMNTjed[hJobID].msgparam.sTjedCtl.hAppliType   = ioMNCjst[hJobID].msgparam.sCjstCtl.hAppliType;
                    /* V018.00_01 END   */
                    /* V019.00_02 START */
                    ioMNTjed[hJobID].msgparam.sTjedCtl.hPaperSize   = IOMSG_NOT_USED;
                    ioMNTjed[hJobID].msgparam.sTjedCtl.hPaperType   = IOMSG_NOT_USED;
                    ioMNTjed[hJobID].msgparam.sTjedCtl.hPaperSource = IOMSG_NOT_USED;
                    /* V019.00_02 END   */
                    iomnDebugMessage(">>>Set TJED Message\n",0);
                    
                    sStatus = jobSetStatus( hJobID, COM_JS_SUSPEND, Status );
                    if ( sStatus == ERROR ){
                        cmnPrintf_ioMN(2,"<<<ERROR iomnPPCJobStart():jobSetStatus\n");
                        return ERROR;
                    }
                    /*  ＪＯＢ終了メッセージ送信    */
                    if(ERROR == (Status = iomnSendTJED(ioMNCjst[hJobID].OldmsgQId, imsgidTjed,
                                            hJobID, IOMSG_SUS_ERROR, IOMSG_NOT_USED, Status, IOMSG_NOT_USED))){
                        iomnDebugMessage(">>>Error:iomnSendTJED()\n",0);
                        return(ERROR);
                    }
                    iomnDebugMessage(">>>Send TJED Message\n",0);
                }
                return(OK);
        case    S_ioMN_12MB_GDI_RECIEVE: /*  PM12MB時GDIデータ受信中の場合   */
        case    S_ioMN_12MB_GDI_EXIST:   /*  PM12MB時GDIデータ有りの場合 */
        case    S_ioMN_NOT_PRIMARY_UI:   /*  非優先ＵＩからのJOB開始指示を受信した場合   */
        case    S_ioMN_DUAL_FUNCTION:    /*  並行動作の制限上実行できない場合            */
        case    S_ioMN_MEMORY_FULL:      /*  FAXメモリFULLが発生している場合             */
                iomnDebugMessage(">>>Impossible to Job Start:%x\n",Status);
                /*  新規ＪＯＢ開始または基本複写の場合  */
                if(hContinuity == IOMSG_JB_NEW ){
                    /*  TJFN Messageの登録  */
                    ioMNTjfn[hJobID].iOldTaskId = iTaskIdIoExMN;
#if 0   /* MCK1.1 新メッセージ対応  */
/**/                    ioMNTjfn[hJobID].iOldMsgId = IOMSG_TJFN_EX;
#else
                    ioMNTjfn[hJobID].iOldMsgId = JOB_MSG_TJFN;
#endif
                    ioMNTjfn[hJobID].OldmsgQId = msgQIdIoExMN;
                    ioMNTjfn[hJobID].msgparam.sTjfnCtl.hJobid = hJobID;
                    ioMNTjfn[hJobID].msgparam.sTjfnCtl.hSts = IOMSG_FIN_ERROR;
                    ioMNTjfn[hJobID].msgparam.sTjfnCtl.iErrSys = Status;
                    ioMNTjfn[hJobID].msgparam.sTjfnCtl.iErrEng = IOMSG_NOT_USED;
                    /* MCK1.1 新メッセージ対応  */
                    ioMNTjfn[hJobID].msgparam.sTjfnCtl.hProcessType = hProcessNo;
                    /* V018.00_01 START */
                    ioMNTjfn[hJobID].msgparam.sTjfnCtl.hJcbID     = ioMNCjst[hJobID].msgparam.sCjstCtl.hJcbID;
                    ioMNTjfn[hJobID].msgparam.sTjfnCtl.hTNodel    = IOMSG_NOT_USED;
                    ioMNTjfn[hJobID].msgparam.sTjfnCtl.hUkedel    = IOMSG_NOT_USED;
                    ioMNTjfn[hJobID].msgparam.sTjfnCtl.hAppliType = ioMNCjst[hJobID].msgparam.sCjstCtl.hAppliType;
                    /* V018.00_01 END   */
                    /* V023.00_01 START */
                    ioMNTjfn[hJobID].msgparam.sTjfnCtl.hTNo       = ioMNCjst[hJobID].msgparam.sCjstCtl.hTNo;
                    /* V023.00_01 END   */
                    iomnDebugMessage(">>>Set TJFN Message\n",0);

                    /*  ＪＯＢ完了メッセージ送信    */
                    if(ERROR == (Status = iomnSendJobFinishSts(ioMNCjst[hJobID].OldmsgQId, imsgid,
#if 0   /* MCK1.1 新メッセージ対応  引数追加    */
                                            hJobID, IOMSG_FIN_ERROR, Status, IOMSG_NOT_USED))){
#else
                                            hJobID, IOMSG_FIN_ERROR, Status, IOMSG_NOT_USED, hProcessNo))){
#endif
                        iomnDebugMessage(">>>Error:iomnSendJobFinishSts()\n",0);
                        return(ERROR);
                        }
                    }
                /*  ＪＯＢ再開の場合    */
                else{
                    /*  Ack Message登録 */
                    ioMNAjst[hJobID].iOldTaskId = iTaskIdIoExMN;
#if 0   /* MCK1.1 新メッセージ対応  */
/**/                    ioMNAjst[hJobID].iOldMsgId = IOMSG_AJST_EX;
#else
                    ioMNAjst[hJobID].iOldMsgId = JOB_MSG_AJST;
#endif
                    ioMNAjst[hJobID].OldmsgQId = msgQIdIoExMN;
                    ioMNAjst[hJobID].msgparam.sAjstCtl.hJobid       = hJobID;
                    ioMNAjst[hJobID].msgparam.sAjstCtl.hInUpDown    = IOMSG_NOT_USED;
                    ioMNAjst[hJobID].msgparam.sAjstCtl.hOutTiming   = IOMSG_NOT_USED;
                    ioMNAjst[hJobID].msgparam.sAjstCtl.iErrStatus   = OK;
                    /* V018.00_01 START */
                    ioMNAjst[hJobID].msgparam.sAjstCtl.hWarSts      = IOMSG_NOT_USED;
                    ioMNAjst[hJobID].msgparam.sAjstCtl.iWarErrSys   = IOMSG_NOT_USED;
                    ioMNAjst[hJobID].msgparam.sAjstCtl.iWarErrEng   = IOMSG_NOT_USED;
                    ioMNAjst[hJobID].msgparam.sAjstCtl.hTNo         = ioMNCjst[hJobID].msgparam.sCjstCtl.hTNo;
                    ioMNAjst[hJobID].msgparam.sAjstCtl.hProcessType = hProcessNo;
                    ioMNAjst[hJobID].msgparam.sAjstCtl.hJcbID       = ioMNCjst[hJobID].msgparam.sCjstCtl.hJcbID;
                    ioMNAjst[hJobID].msgparam.sAjstCtl.hAppliType   = ioMNCjst[hJobID].msgparam.sCjstCtl.hAppliType;
                    /* V018.00_01 END   */
                    iomnDebugMessage(">>>Set AJST Message\n",0);

                    /*  TJED Messageの登録  */
                    ioMNTjed[hJobID].iOldTaskId = iTaskIdIoExMN;
#if 0   /* MCK1.1 新メッセージ対応  */
/**/                    ioMNTjed[hJobID].iOldMsgId = IOMSG_TJED_EX;
#else
                    ioMNTjed[hJobID].iOldMsgId = JOB_MSG_TJSX;
#endif
                    ioMNTjed[hJobID].OldmsgQId = msgQIdIoExMN;
                    ioMNTjed[hJobID].msgparam.sTjedCtl.hJobid = hJobID;
                    ioMNTjed[hJobID].msgparam.sTjedCtl.hSts = IOMSG_SUS_ERROR;
                    ioMNTjed[hJobID].msgparam.sTjedCtl.iErrSys = IOMSG_NOT_USED;
                    ioMNTjed[hJobID].msgparam.sTjedCtl.iErrEng = Status;
                    ioMNTjed[hJobID].msgparam.sTjedCtl.hOrgBack = IOMSG_NOT_USED;
                    /* V018.00_01 START */
                    ioMNTjed[hJobID].msgparam.sTjedCtl.hTNo         = ioMNCjst[hJobID].msgparam.sCjstCtl.hTNo;
                    ioMNTjed[hJobID].msgparam.sTjedCtl.hProcessType = hProcessNo;
                    ioMNTjed[hJobID].msgparam.sTjedCtl.hJcbID       = ioMNCjst[hJobID].msgparam.sCjstCtl.hJcbID;
                    ioMNTjed[hJobID].msgparam.sTjedCtl.hAppliType   = ioMNCjst[hJobID].msgparam.sCjstCtl.hAppliType;
                    /* V018.00_01 END   */
                    /* V019.00_02 START */
                    ioMNTjed[hJobID].msgparam.sTjedCtl.hPaperSize   = IOMSG_NOT_USED;
                    ioMNTjed[hJobID].msgparam.sTjedCtl.hPaperType   = IOMSG_NOT_USED;
                    ioMNTjed[hJobID].msgparam.sTjedCtl.hPaperSource = IOMSG_NOT_USED;
                    /* V019.00_02 END   */
                    iomnDebugMessage(">>>Set TJED Message\n",0);
                    
                    sStatus = jobSetStatus( hJobID, COM_JS_SUSPEND, Status );
                    if ( sStatus == ERROR ){
                        cmnPrintf_ioMN(2,"<<<ERROR iomnPPCJobStart():jobSetStatus\n");
                        return ERROR;
                    }
                    /*  ＪＯＢ終了メッセージ送信    */
                    if(ERROR == (Status = iomnSendTJED(ioMNCjst[hJobID].OldmsgQId, imsgidTjed,
                                            hJobID, IOMSG_SUS_ERROR, IOMSG_NOT_USED, Status, IOMSG_NOT_USED))){
                        iomnDebugMessage(">>>Error:iomnSendTJED()\n",0);
                        return(ERROR);
                    }
                    iomnDebugMessage(">>>Send TJED Message\n",0);
                }
                return(OK);

        case    ERROR:
                iomnDebugMessage(">>>Error:iomnPPCStartCofirmation()\n",0);

                /*  TJFN Messageの登録  */
                ioMNTjfn[hJobID].iOldTaskId = iTaskIdIoExMN;
#if 0   /* MCK1.1 新メッセージ対応  */
/**/                ioMNTjfn[hJobID].iOldMsgId = IOMSG_TJFN_EX;
#else
                ioMNTjfn[hJobID].iOldMsgId = JOB_MSG_TJFN;
#endif
                ioMNTjfn[hJobID].OldmsgQId = msgQIdIoExMN;
                ioMNTjfn[hJobID].msgparam.sTjfnCtl.hJobid = hJobID;
                ioMNTjfn[hJobID].msgparam.sTjfnCtl.hSts = IOMSG_FIN_ERROR;
                ioMNTjfn[hJobID].msgparam.sTjfnCtl.iErrSys = errno;
                ioMNTjfn[hJobID].msgparam.sTjfnCtl.iErrEng = IOMSG_NOT_USED;
                /* V018.00_01 START */
                ioMNTjfn[hJobID].msgparam.sTjfnCtl.hJcbID     = ioMNCjst[hJobID].msgparam.sCjstCtl.hJcbID;
                ioMNTjfn[hJobID].msgparam.sTjfnCtl.hTNodel    = IOMSG_NOT_USED;
                ioMNTjfn[hJobID].msgparam.sTjfnCtl.hUkedel    = IOMSG_NOT_USED;
                ioMNTjfn[hJobID].msgparam.sTjfnCtl.hAppliType = ioMNCjst[hJobID].msgparam.sCjstCtl.hAppliType;
                /* V018.00_01 END   */
                /* V023.00_01 START */
                ioMNTjfn[hJobID].msgparam.sTjfnCtl.hTNo       = ioMNCjst[hJobID].msgparam.sCjstCtl.hTNo;
                ioMNTjfn[hJobID].msgparam.sTjfnCtl.hProcessType = hProcessNo;
                /* V023.00_01 END   */
                iomnDebugMessage(">>>Set TJFN Message\n",0);

                /*  ＪＯＢ完了メッセージ送信    */
                if(ERROR == (Status = iomnSendJobFinishSts(ioMNCjst[hJobID].OldmsgQId, imsgid,
#if 0   /* MCK1.1 新メッセージ対応  引数追加    */
                                                hJobID, IOMSG_FIN_ERROR, errno, IOMSG_NOT_USED))){
#else
                                                hJobID, IOMSG_FIN_ERROR, errno, IOMSG_NOT_USED, hProcessNo))){
#endif
                    iomnDebugMessage(">>>Error:iomnSendJobFinishSts()\n",0);
                    return(ERROR);
                }
                return(OK);
        default:
                iomnDebugMessage(">>>ERROR:PPC Confirmation Status->%x\n",Status);
                break;
    }

    /* JOB開始準備 */
    if( iomnPrepareForPPC(hJobID, hStartTrigger, hContinuity, hJobtype) != ERROR ) {
        iomnDebugMessage(">>>OK:Enable to Start PPC\n",0);
        /* コピー原稿入力開始の準備ができた場合 */
        /*  JOB開始メッセージ送信   */
        if( iomnSendJobStart(hJobID, hStartTrigger, hContinuity, 0) != ERROR ) {
            return(OK);     /* コピー開始(成功) */
        }
        else {
            iomnDebugMessage(">>>error:iomnSendJobStart()\n",0);
            if( hContinuity == IOMSG_JB_NEW ) {
                /* 新規ジョブ開始の場合は後片付けが必要 */
                iomnFreeCopyJobInfo( hJobID );  /* 終了する前にコピージョブ情報を開放 */
            }
            return( ERROR );
        }
    }
    else {
        iomnDebugMessage(">>>Error:iomnPrepareForPPC()\n",0);
    }

    /* 新規ジョブ開始の場合はTJFNを送信する */
    if( hContinuity == IOMSG_JB_NEW ) {
        /* TJFNの送信 */
/* D3.19 START T.Fujii コーディングミス修正 */
#if 0   /* OLD */
        iomnSendTjfn( hJobID, (HI)IOMSG_FIN_ERROR, (I32)IOMSG_JB_FAILURE, (I32)IOMSG_NOT_USED );
#else   /* D3.19 MID    NEW */
        iomnSendTjfn( hJobID, (HI)IOMSG_FIN_ERROR, 
                      (I32)(M_f220IoMNErr|IOMSG_JB_FAILURE), (I32)IOMSG_NOT_USED );
#endif
/* D3.19 END */
        /* D5.3 START T.Fujii EB46送信対応 */
        if( hJobID == hioMNIntJobID ) { /* 割り込みコピーの場合 */
/* D5.3.1 START T.Fujii 異常処理時のジョブ情報削除処理削除 */
#if 0   /* OLD */
/**/            if( iomnCpyMNGetCpyJobStatus( 0 ) ) {   /* 万が一、ジョブ情報が残っている場合 */
/**/                iomnCpyMNDeletePPCFile( 0 );    /* コピー用ファイルの削除と再作成 */
/**/                iomnCpyMNFreeCpyJobID( 0 );     /* コピージョブ情報テーブルの開放 */
/**/            }
#endif
/* D5.3.1 END */
            iomnSendECFN( 0 );  /* EB46送信 */
        }
        /* D5.3 END */
        /*  メッセージテーブルの初期化  */
        iomnCleanJobManagementMsg( hJobID );
    }
    return( ERROR );
}


/*
(C) Copyright TOSHIBA TEC Corporation 1999,2000. ALL Right Reserved
NO.         100.3
TITLE:      
MODULE：    LOCAL STATUS iomnSetCpyInputMode(HI hJobID)
INCLUDE:
PARAMETER:
            HI      hJobid;             JOBID
RETURNS:    STATUS  4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            原稿入力ジョブのモードチェック＆設定。
NOTES:
HISTORY:
        D1.1     T.Fujii   99/11/15 DM45/35 original
        D1.9.3   J.Ootani  00/05/17 DM45/35 PrinterのみW-UP中の逐次開始対応
*/
#if 0   /* V028.00_00 */
LOCAL STATUS iomnSetCpyInputMode( HI hJobID )
{
    HI pValue;
    HI pValueWup;

    #ifdef IOMN_DM_DEBUG_991115  /* D1.1 T.Fujii 99/11/15 */
        cmnPrintf_ioMN(0, ">> ioMN >> START:iomnSetCpyInputMode\n" );
    #endif

    /* 入出力モードをチェック */
    if( parMNIOMode(hJobID,(HI *)&pValue) == ERROR ) {
        #ifdef IOMN_DM_DEBUG_991115  /* D1.1 T.Fujii 99/11/15 */
            cmnPrintf_ioMN(0, ">> ioMN >> ERROR:parMNIOMode() in iomnSetCpyInputMode\n" );
        #endif
        return( ERROR );
    }
    #ifdef IOMN_DM_DEBUG_991115  /* D1.1 T.Fujii 99/11/15 */
        cmnPrintf_ioMN(0, ">> ioMN >> GET:parMNIOMode() = %x\n", pValue );
    #endif

    /* 入出力モードの妥当性チェック／修正 */
    if(    pValue==COM_OT_SCS     || pValue==COM_OT_SCS_MNL
        || pValue==COM_OT_SCS_ONE || pValue==COM_OT_SCS_BOTH ) {
        /* (従来型)逐次の場合 */

        /* 逐次可能かチェックし、必要なら後追い逐次に置き換える */
        if( iomnPrnQueCheckEntry() == ERROR /* 印刷ジョブが存在する場合 */
            || ioMNstatus.hStartup_mode == COM_MODE_05ADJUSTMENT    /* 自己診断05の場合 */
            || ioMNstatus.hStartup_mode == COM_MODE_05ADJUSTMENT_RET )
        {
            pValue = COM_OT_TRK_SCS;    /* 後追い逐次を設定 */
        }

        /* Printerの状態取得 */
        if( parMNRscPrinterStatus(&pValueWup) == ERROR )
        {
            cmnPrintf_ioMN(2, ">> ioMN >> ERROR:parMNRscPrinterStatus() in iomnSetCpyInputMode\n" );
        }
        else {
            if( pValueWup != COM_MST_READY)
            { /* PrinterがCOM_MST_READY状態でない場合 */
                pValue = COM_OT_TRK_SCS;    /* 後追い逐次を設定 */
            }
        }
    }
    else if( pValue==COM_OT_FIN ) {
        /* (従来型)完了の場合 */
        pValue = COM_OT_TRK_FIN;    /* 後追い逐次完了型を設定する */
    }
    else {
        /* 上記以外(通常の場合) */
        pValue = -1;    /* 入出力モードの変更は無し */
    }

    /* ジョブモード(動作モード、サブジョブ種別)設定 */
    if( iomnSetInputMode(hJobID, pValue) == ERROR ) {
        cmnPrintf_ioMN(2, ">> ioMN >> ERROR:iomnSetInputMode() in iomnSetCpyInputMode\n" );
        return( ERROR );
    }

    #ifdef IOMN_DM_DEBUG_991115  /* D1.1 T.Fujii 99/11/15 */
        if( pValue != -1 ) {
            cmnPrintf_ioMN(0, ">> ioMN >> CHANGED:parMNIOMode() = %x\n", pValue );
        }
    #endif

    #ifdef IOMN_DM_DEBUG_991115  /* D1.1 T.Fujii 99/11/15 */
        cmnPrintf_ioMN(0, ">> ioMN >> END OK:iomnSetCpyInputMode\n" );
    #endif
    return( OK );
}
#endif

/*
(C) Copyright TOSHIBA TEC Corporation 1999,2000. ALL Right Reserved
NO.         100.x
TITLE:      
MODULE：    LOCAL void iomnFreeCopyJobInfo(HI hJobID)
INCLUDE:
PARAMETER:
            HI      hJobid;             JOBID
RETURNS:    STATUS  4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            コピージョブのために保存していた情報をすべて開放する
NOTES:
HISTORY:
        D1.1    T.Fujii 99/11/15    DM45/35 original
        D3.12.1 T.Fujii 00/09/04    DM45/35 BG印刷ジョブ中断処理修正

    V013.00_04  M.Karasawa  03/02/07    McKinley    ジョブ削除(jobDelete())はioMNでは行わず、上位で行う
    V016.00_04  M.Karasawa  03/03/31    McKinley    複写ジョブ管理エリアに関する処理を前面削除
*/
void iomnFreeCopyJobInfo( HI hJobid )
{
    HI hIpTblID;    /* 逐次制御テーブルID */
    HI hPrintJobID; /* 印刷用ジョブID */

    /* 逐次制御テーブルIDを取得 */
    hIpTblID = iomnJobInfo[hJobid].hIpTblID;

    /* 印刷用ジョブIDの取得 */
    hPrintJobID = iomnSyncTblGetJobID( hIpTblID, COM_JB_SUB_OUTPUT );

    /* 逐次制御テーブルの開放およびIPテーブルの開放 */
    iomnReleaseSyncLink( hJobid);
    /* D3.12.1 START T.Fujii BG印刷ジョブ中断処理修正 */
    /* IPテーブルID情報のクリア */
    iomnJobInfo[hJobid].hIpTblID = IPLIB_INIT;
    /* D3.12.1 END */

#if 0   /* V013.00_04   */
/**/    /* 印刷用ジョブID解放 */
/**/    jobDelete( hPrintJobID );
#endif
#if 0   /* V016.00_04   START   */
/**/    /* コピージョブ情報テーブルを開放 */
/**/    iomnCpyMNFreeCpyJobID( iomnCpyMNGetCpyJobID() );
#endif  /* V016.00_04   END     */
}


/*
(C) Copyright TOSHIBA TEC Corporation 1999,2000. ALL Right Reserved
NO.         100.x
TITLE:      
MODULE：    STATUS iomnPrepareForPPC2(HI hJobid)
INCLUDE:    ioMNpar.h
            ioMNstr.h
PARAMETER:
            HI      hJobid;             JOBID   
            HI      hStartTrigger;      開始要因
            HI      hContinuity;        継続or新規  
            HI      hJobtype;           JOB種別
RETURNS:    STATUS 4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            JOB開始指示メッセージ受信時の開始準備処理。
NOTES:
HISTORY:
        D1.1    T.Fujii    99/11/15    DM45/35 後追い逐次・並行動作対応
        D1.4    Y.Shimada  00/ 1/25    DM45/35 フォームストレージ変更：ＩｐＬｉｂ／逐次制御の組み込み
        D1.2.4  T.Fujii    00/04/10    DM45/35 サブジョブ中断・再開処理修正
        D4.7    T.Fujii    00/11/18    DM45/35 前ジョブがエラー終了するとジャム解除で印刷開始
                                               しなくなる不具合修正

        MCK1.5  M.Karasawa  02/07/25    McKinley    後追い逐次複写対応
    V011.00_02  M.Karasawa  03/01/14    McKinley    jcLibのSRAM対応伴う修正
    V011.00_06  M.Karasawa  03/01/14    McKinley    関数名ミス（jcLibTBL_memfree -> jcLibUTY_memfree）
*/
LOCAL STATUS iomnPrepareForPPC2( HI hJobid )
{
    HI hIpTblID = 0;        /* IPテーブルID */
    HI hPrintJobID = -1;         /* 出力用ジョブID */
    HI hJobType;            /* ジョブ種別 */

/* MCK1.5   *//* --- 後追い逐次 --- */
/*  H16                 siIdx_no;   *//* インデックス番号     */														/* SM-001 */
/*  JOB_EXECDATA_TBL    *sJobTbl;   *//* 実行データテーブル   */														/* SM-001 */
	HI hCopyProtection;
/* MCK1.5 END   */

    if( ERROR == jobJobtype(hJobid, &hJobType) ){
        cmnPrintf_ioMN(2,">>>Error:jobJobtype() in iomnPrepareForPPC2())\n");
        printErrno(errno);
        return(ERROR);
    }

    /* ipテーブルの取得とリンク情報の確立 */
    hIpTblID = ipLibEntryJob( hJobid );

    if( hIpTblID != ERROR ) {
        /* ipテーブルの確保ができた場合 */

        /* ジョブIDごとのジョブ情報エリアにipテーブルIDを保存 */
        iomnJobInfo[hJobid].hIpTblID = hIpTblID;

        /* 逐次制御テーブルに入力ジョブを登録 */
        if( iomnSyncTblEntryJob( hIpTblID, COM_JB_SUB_INPUT, hJobid ) != ERROR ) {

            /* 逐次制御テーブルをIPテーブル使用中に更新 */
            iomnSyncTblSetStatus( hIpTblID, COM_JB_SUB_INPUT, IOMN_IPTBL_USING );
/*Ashwin Changed For synchronising Print Job with IpTable*/
#if 1   /* MCK1.5   *//* --- 後追い逐次 --- */
/**/            /* 印刷用のジョブIDを取得 */
#if 0
/**/            hPrintJobID = jobCreate( hJobType , COM_JB_BACKGROND, COM_JS_READY);
#endif

		{
			JOB_EXECDATA_TBL* psJobTable;
			psJobTable = (JOB_EXECDATA_TBL*) ioMNCjst[hJobid].msgparam.sCjstCtl.pUiWorkPointer;
			parMNCopyProtection(hJobid, &hCopyProtection);
			hPrintJobID = psJobTable->sCommon.hOutJobId;
			printf("\nPrintJobId = %d", hPrintJobID);
		}
#else
/* 印刷ジョブの生成は、JCでの動作モード確定時、後追い動作になった時点で行なう。 */
/* ioMNは、実行データテーブルからジョブIDを取得する。        
*/
            if((siIdx_no = jcLibJCB_tnoidx(ioMNCjst[hJobid].msgparam.sCjstCtl.hTNo)) != JCLIB_NOEXIST){
                if((sJobTbl = (JOB_EXECDATA_TBL*) jcLibTBL_getp(JCLIB_EXC_TBL, siIdx_no)) != JCLIB_NOEXIST){
                    hPrintJobID = sJobTbl->sCommon.hOutJobId;
                    jcLibUTY_memfree((void *)sJobTbl);          /* V011.00_02   *//* ADD    *//* V011.00_06 */
                }else{
                    hPrintJobID = ERROR;
                }
            }else{
                hPrintJobID = ERROR;
            }
#endif

            if( hPrintJobID != ERROR ) {
                /* 印刷用のジョブIDが取得できた場合 */
                iomnDebugMessage(">>OK:jobCreate()JobID=%d\n",(I32)hPrintJobID);
                /* ジョブ情報のクリア */
                iomnJobInfo[hPrintJobID].hIpTblID = IPLIB_INIT;

                /* D4.7 START T.Fujii メッセージ受信情報クリア処理追加 */
                /* メッセージ受信情報の一部をクリアする */
                memset( &ioMNCjst[hPrintJobID], 0x00, sizeof(struct msgstore) );
                memset( &ioMNAjst[hPrintJobID], 0x00, sizeof(struct msgstore) );
                memset( &ioMNTjfn[hPrintJobID], 0x00, sizeof(struct msgstore) );
                memset( &ioMNTjed[hPrintJobID], 0x00, sizeof(struct msgstore) );
                memset( &ioMNTjsu[hPrintJobID], 0x00, sizeof(struct msgstore) );
                /* D4.7 END */
		if(hCopyProtection == FALSE) /*To Support Copy Protection*/
		{
                /* 逐次制御テーブルに出力ジョブを登録して関連付ける */
                if( iomnSyncTblEntryJob(hIpTblID, COM_JB_SUB_OUTPUT, hPrintJobID) == OK ) {
                    iomnDebugMessage(">>OK:iomnPrepareForPPC2()\n",0);
                    return( OK );   /* 全て成功で終了(コピー開始可能) */
                }
		}
		else
		{
			cmnPrintf_ioMN(6, "CopyProtection is ON So Output Job is not registered to SyncTable\n" );
			return OK;
                }
#if 0   /* MCK1.5   *//* --- 後追い逐次 --- */
/* ジョブ生成はJCで行なうので、削除処理不要 */
/**/                /* ここまで来たときはエラー */
/**/                jobDelete( hPrintJobID );   /* 印刷用ジョブを削除する */
#endif
            }
            else {
                /* ジョブIDが取得できなかった場合 */
                cmnPrintf_ioMN(2, ">> ioMN >> ERROR:jobCreate()\n" );
            }

            /* 結局エラーになってしまった場合、後片付けが必要 */
            /* 逐次制御テーブルからジョブ削除 */
            iomnSyncTblDeleteJob( hIpTblID );
        }
        else {
            /* 逐次制御テーブルへのジョブ登録失敗時 */
            iomnDebugMessage(">>ERROR:iomnSyncTblEntryInputJob()\n",0);
        }

        /* 結局エラーになってしまった場合、後片付けが必要 */
        /* ipテーブル解放 */
        ipLibDeleteJob( hIpTblID );
    }
    else {
        /* ipテーブルの確保に失敗 */
        iomnDebugMessage(">>ERROR:ipLibEntryJob()\n",0);
    }

    return( ERROR );
}


/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         7.1.1.2
TITLE:      
MODULE：    STATUS iomnPrepareForPPC(HI hJobid, HI hStartTrigger, HI hContinuity, HI hJobtype)
INCLUDE:    ioMNpar.h
            ioMNstr.h
PARAMETER:
            HI      hJobid;             JOBID   
            <不要>HI      hStartTrigger;      開始要因
            <不要>HI      hContinuity;        継続or新規    
            <不要>HI      hJobtype;           JOB種別
RETURNS:    I32 4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            JOB開始指示メッセージ受信時の開始準備処理。
NOTES:
HISTORY:
        1.1     Y.Matsuda   97/7/30
        M1.0    (TJ)H.Takahashi 98/10/29
                        無駄処理が多いため,全面改訂
        M4.10   (TJ)H.Takahashi 99/01/13
                1st.コピー高速化対応
        D1.1    T.Fujii 99/11/15 DM45/35 後追い逐次・先行入力対応
        D1.2    T.Fujii 99/12/24 DM45/35 並行動作対応(フィニッシャ初期化処理を印刷開始時に移動)
        D1.3    T.Fujii 00/01/17 DM45/35 従来逐次対応
        D2.1    T.Fujii 00/06/06 DM45/35 V004.60不具合対応(parMNの原稿枚数をクリア)
        D3.21.1 T.Fujii 00/09/18 DM45/35 後追い逐次での部数更新処理修正
        D3.12.4 T.Fujii 00/09/25 DM45/35 BG印刷ジョブ中断処理修正
        D4.7.1  T.Fujii 00/11/20 DM45/35 プリンタW-UP中の２つ目の入力ジョブでCopyingロックになる
                                         不具合修正
        D4.7.2  T.Fujii 00/11/21 DM45/35 プリンタW-UP中の2つ目のFAX/DSS入力ジョブでロックになる
                                         不具合対応

    MCK1.4      M.Karasawa  02/07/18    McKinley    V005.02 逐次コピー動作用修正
    MCK1.4      M.Karasawa  02/07/18    McKinley    parMN関連はcpyJCに移行
    V016.00_04  M.Karasawa  03/03/31    McKinley    複写ジョブ管理エリアに関する処理を前面削除
*/
STATUS  iomnPrepareForPPC(HI hJobid, HI hStartTrigger, HI hContinuity, HI hJobtype)
{
    STATUS  Status;
#if 0   /* MCK1.4   *//* --- CPY逐次 --- */
/**/    B       aFileNamebuf[FILENAME];         /*  ファイル名  */
#endif
#if 0   /* V016.00_04   START   */
/**/    HI      hCpyJobID;
#endif  /* V016.00_04   END     */
    HI      hIOMode;     /* 入出力動作モード */
    /* D3.21.1 START T.Fujii 後追い逐次での部数更新処理修正 */
    HI      hOutputJobID;       /* 出力ジョブのJob ID */
    HI      hOutputJobType;     /* 出力ジョブのJob種別 */
    HI      hOutputJobStatus;   /* 出力ジョブのジョブ状態 */
    I32     iOutputSuscause;    /* 出力ジョブの中断要因 */
    /* D3.21.1 END */

    iomnDebugMessage(">>>>Top of iomnPrepareForPPC()<<<<\n",0);

    /* 新規ジョブの場合 */
    if(hContinuity == IOMSG_JB_NEW){
        /*  Initialize  */
#if 0   /* MCK1.4   */
/**/        memset( aFileNamebuf,0x00,sizeof(aFileNamebuf) );
#endif
/* D4.7.2 START T.Fujii FAX/DSS入力も同じ現象になるため共通処理に変更 */
#if 0   /* OLD */
        /* D4.7.1 START T.Fujii メッセージ受信情報クリア処理追加 */
        /* メッセージ受信情報の一部をクリアする */
        memset( &ioMNTjfn[hJobid], 0x00, sizeof(struct msgstore) );
        /* D4.7.1 END */
#endif
/* D4.7.2 END */
#if 0   /* V016.00_04   START   */
/**/        /* コピージョブ情報テーブルの確保およびJOB種別によるファイル名の設定 */
/**/        switch( hJobtype ){
/**/            case COM_JB_IPPC :
/**/                /* 割り込み用コピージョブ情報テーブルの確保 */
/**/                hCpyJobID = iomnCpyMNShiftToInt();
/**/                break;
/**/
/**/            case COM_JB_EXPPC2 :
/**/                /* コピージョブ情報保存用テーブルの確保 */
/**/                hCpyJobID = iomnCpyMNGetNextCpyJobID();
/**/                break;
/**/
/**/            default :
/**/                cmnPrintf_ioMN(2,"Error JobType! iomnPrepareForPPC()\n");
/**/                return ERROR;
/**/        }
/**/
/**/        if( hCpyJobID < 0 ) {
/**/            /* コピージョブIDが取得できない場合はエラーで終了 */
/**/            return( ERROR );
/**/        }
#endif  /* V016.00_04   END     */
#if 0   /* MCK1.4   */
/**/        /* ファイル名の設定 */
/**/        strcpy( aFileNamebuf, iomnCpyMNGetCpyFileName(hCpyJobID) );
/**/
/**/        if( parMNSetPages(hJobid, 0) == ERROR ) {
/**/            cmnPrintf_ioMN(2,"[ioMN]ERROR:parMNSetPages(JobID=%d)\n",hJobid);
/**/        }
#endif
#if 0   /* MCK1.4 動作モードはioMNより上位の判断に依存  *//* --- CPY逐次 --- */
/**/        /* 入力モードの設定をこの位置に移動 */
/**/        /* ジョブモード(入出力動作モードの置き換え、サブジョブ種別)設定 */
/**/        if( iomnSetCpyInputMode(hJobid) == ERROR ) {
/**/            iomnDebugMessage(">> ioMN >> Error:iomnSetCpyInputMode()\n",0);
/**/            /* 終了する前にコピージョブ情報テーブルを開放 */
/**/            iomnCpyMNFreeCpyJobID( hCpyJobID );
/**/            return( ERROR );
/**/        }
#endif

        /* 入出力動作モードを取りなおす */
        if( parMNIOMode(hJobid, &hIOMode) == ERROR ) {
            /* 入出力動作モードが取得できなかった場合 */
            cmnPrintf_ioMN(2, ">> ioMN >> ERROR: parMNIOMode(hJobid=%d) in iomnPrepareForPPC()\n", hJobid );
            cmnPrintf_ioMN(2, ">> ioMN >> Error in iomnSetCpyInputMode() ??\n" );
#if 0   /* V016.00_04   START   */
/**/            /* 終了する前にコピージョブ情報テーブルを開放 */
/**/            iomnCpyMNFreeCpyJobID( hCpyJobID );
#endif  /* V016.00_04   END     */
            return( ERROR );
        }

        /* 後追い逐次の場合、IPテーブルの確保などを行う */
        if( (hIOMode == COM_OT_TRK_SCS) || (hIOMode == COM_OT_TRK_FIN) ) {
            /* IPテーブルの確保など、コピージョブ開始の準備 */
            if( iomnPrepareForPPC2(hJobid) == ERROR ) {
#if 0   /* V016.00_04   START   */
/**/                /* エラーの場合コピージョブ情報テーブルを開放して終了 */
/**/                iomnCpyMNFreeCpyJobID( hCpyJobID );
#endif  /* V016.00_04   END     */
                return( ERROR );
            }
        }

#if 0   /* MCK1.4   */
/**/        #ifdef  DEBUG
/**/        cmnPrintf_ioMN(0,"### File Name-->%s\n",aFileNamebuf);
/**/        #endif
#endif

        /*  バス切り替え    */
        if(ERROR == (Status = cmnChgBus(IMGBUS_PM))){
            iomnDebugMessage(">>>Error:cmnChgBus()\n",0);
#if 0   /* V016.00_04   START   */
/**/            iomnFreeCopyJobInfo( hJobid );  /* 終了する前にコピージョブ情報を開放 */
#endif  /* V016.00_04   END     */
            return(ERROR);
        }

#if 0   /* MCK1.4   */
/**/        /*  ページパラメータテーブルへのファイル名設定  */
/**/        if(ERROR == (Status = jobSetPagePrmFile(hJobid, aFileNamebuf))){
/**/            iomnDebugMessage(">>>Error:jobSetPagePrmFile()\n",0);
/**/            iomnFreeCopyJobInfo( hJobid );  /* 終了する前にコピージョブ情報を開放 */
/**/            return(ERROR);
/**/        }
/**/
/**/        /*  開始パラメータ設定  */
/**/        /*  原稿入力は昇順とする */
/**/        if(ERROR == (Status = parMNSetPrnOrder(hJobid, COM_IO_UP))){
/**/            iomnDebugMessage(">>>Error:parMNSetPageorder()\n",0);
/**/            iomnFreeCopyJobInfo( hJobid );  /* 終了する前にコピージョブ情報を開放 */
/**/            return(ERROR);
/**/        }
#endif
#if 0   /* V016.00_04   START   */
/**/        /* コピージョブ動作履歴保存 */
/**/        if( iomnCpyMNStoreCopyLog(hCpyJobID, IOMN_CPYJOB_SCN_START) 
/**/            == ERROR ) {
/**/            iomnFreeCopyJobInfo( hJobid );  /* 終了する前にコピージョブ情報を開放 */
/**/            return( ERROR );
/**/        }
#endif  /* V016.00_04   END     */
    }
    /* D3.21.1 START T.Fujii 後追い逐次での部数更新処理修正 */
    else {
        /* 再開の場合、部数が更新されていればparMNをコピーする */
        if( iomnSyncTblCpyParMN(iomnJobInfo[hJobid].hIpTblID,IOMN_CPYPARMN_COPIES) == OK ) {
            /* もし後追い逐次で印刷動作中だったら中断指示を出す */
            /* 出力ジョブのジョブIDを取得する */
            hOutputJobID = iomnSyncTblGetJobID(iomnJobInfo[hJobid].hIpTblID,COM_JB_SUB_OUTPUT);
            if( hOutputJobID > IOMN_INPUTJOB_ID_MAX ) {
                /* 出力ジョブのジョブ状態を取得 */
                if( jobStatus( hOutputJobID, &hOutputJobStatus, &iOutputSuscause) != ERROR ) {
                    /* 後追い逐次で動作中だった場合は中断処理を行なう */
                    if( hOutputJobStatus == COM_JS_RUNNING ) {
                        /* 出力ジョブのジョブ種別を取得 */
                        jobJobtype( hOutputJobID, &hOutputJobType );
                        /* 出力ジョブの中断 */
/* D3.12.4 START T.Fujii BG印刷ジョブ中断処理修正 */
#if 0   /* OLD */
                        iomnPrnCjsu( hOutputJobID, IOMN_SUS_FRONT, hOutputJobType );
#else   /* D3.12.4 MID  NEW */
                        iomnPrnCjsu( hOutputJobID, IOMSG_SUS_CS, hOutputJobType );
#endif
/* D3.12.4 END */
                    }
                }
            }
        }
    }
    /* D3.21.1 END */

    iomnDebugMessage(">>>>Bottom of iomnPrepareForPPC()<<<<\n",0);
    return(OK);
}


/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         7.1.2.1
TITLE:      FAX入力JOB開始処理
MODULE：    STATUS  iomnFaxScnJobStart(HI hJobID, HI hStartTrigger, HI hContinuity, HI hJobtype)

INCLUDE:    ioMNPar.h
            ioMNbuf.h
PARAMETER:
            HI      hJobID;             JOBID   
            HI      hStartTrigger;      開始要因
            HI      hContinuity;        継続or新規  
            HI      hJobtype;           JOB種別
RETURNS:    I32 4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            Ｆａｘ原稿入力ＪＯＢについてＪＯＢ開始メッセージを受信した場合の処理で、
            ＪＯＢ実行可否を確認した後、ＪＯＢ状態を変更し、ＪＯＢ開始メッセージを送信する。
NOTES:
HISTORY:
        1.1     Y.Matsuda   96/9/18
        D1.7.8 J.Ootani 00/03/11    DM45/35 FAXジョブ開始終了対応
        D2.4.1 T.Fujii  00/06/17    DM45/35 V004.70不具合対応
                                            AJSTパラメータ拡張対応修正

        MCK1.1  M.Karasawa  02/06/11    McKinley    新メッセージ対応
        MCK1.3  M.Karasawa  02/07/03    McKinley    Mckinley用に処理見なおし（FAX送信）
*/
#if 0   /* MCK1.1 新メッセージ対応  引数追加    */
/**/STATUS  iomnFaxScnJobStart(HI hJobID, HI hStartTrigger, HI hContinuity, HI hJobtype)
#else
STATUS  iomnFaxScnJobStart(HI hJobID, HI hStartTrigger, HI hContinuity, HI hJobtype, HI hProcessNo)
#endif
{
    STATUS  Status;     /*  処理結果    */
#if 0   /* MCK1.3 FAX送信対応  */
/**/    I32     imsgidTjed=IOMSG_TJED_UI;     /*  ＪＯＢ終了メッセージＩＤ    */    /* D1.1 T.Fujii */
/**/    I32     imsgid=IOMSG_TJFN_UI;   /* D1.1 T.Fujii */
#else
    I32     imsgid=JOB_MSG_TJFN;
#endif
    iomnDebugMessage(">>>>Top of iomnFaxScnJobStart()<<<<\n",0);

#if 0   /* MCK1.1 新メッセージ対応  */
/**/    /*  messageID取得   */
/**/    if(ioMNCjst[hJobID].iOldMsgId == IOMSG_CJST_UI){
/**/        imsgid = IOMSG_TJFN_UI;
/**/        imsgidTjed = IOMSG_TJED_UI;
/**/    }
/**/    if(ioMNCjst[hJobID].iOldMsgId == IOMSG_CJST_JOB){
/**/        imsgid = IOMSG_TJFN_JOB;
/**/        imsgidTjed = IOMSG_TJED_JOB;
/**/    }
/**/    if(ioMNCjst[hJobID].iOldMsgId == IOMSG_CJST_DIG){
/**/        imsgid = IOMSG_TJFN_DIG;
/**/    }
#endif
#if 0   /* MCK1.3 START FAX送信実行確認についてはJC層で行なうので削除   */
/**/    Status = iomnFaxScnStartConfirm(hJobID);
/**/    /*  JOB実行可否確認結果による処理   */
/**/    switch(Status){
/**/      case    IOMN_ON:
/**/        /*  ＪＯＢ開始予約フラグが設定されている場合は中断終了まで開始保留  */
/**/        if(ioMNstatus.hJobStart == IOMN_ON){
/**/            iomnDebugMessage(">>>Wait... Back Job Suspending\n",0);
/**/
/**/            /*  JOB中断後の再開情報格納 */
/**/            iomnSetStartQue(&ioMNFrontQue, hJobID, hStartTrigger,
/**/                                         hContinuity, 0, hJobtype);
/**/            iomnDebugMessage(">>>OK:iomnSetStartQue()\n",0);
/**/            ioMNstatus.hJobStart = IOMN_OFF;    
/**/            return(OK);
/**/        }
/**/        else{
/**/            iomnDebugMessage(">>>Fax Scan JOB Start\n",0);
/**/        }
/**/        break;
/**/
/**/      case    S_ioMN_ENGINE_ERROR:
/**/      case    S_ioMN_MACHINE_RECOVERY:
/**/      case    S_ioMN_TEST_STAPLE:
/**/        iomnDebugMessage(">>>Impossible to Job Start:%x\n",Status);
/**/
/**/        /*  新規ＪＯＢ開始または基本複写の場合  */
/**/        if(hContinuity == IOMSG_JB_NEW){
/**/            /*  TJFN Messageの登録  */
/**/            ioMNTjfn[hJobID].iOldTaskId = iTaskIdIoExMN;
/**/            ioMNTjfn[hJobID].iOldMsgId = IOMSG_TJFN_EX;
/**/            ioMNTjfn[hJobID].OldmsgQId = msgQIdIoExMN;
/**/            ioMNTjfn[hJobID].msgparam.sTjfnCtl.hJobid = hJobID;
/**/            ioMNTjfn[hJobID].msgparam.sTjfnCtl.hSts = IOMSG_FIN_ERROR;
/**/            ioMNTjfn[hJobID].msgparam.sTjfnCtl.iErrSys = IOMSG_NOT_USED;
/**/            ioMNTjfn[hJobID].msgparam.sTjfnCtl.iErrEng = Status;
/**/            iomnDebugMessage(">>>Set TJFN Message\n",0);
/**/
/**/            /*  ＪＯＢ完了メッセージ送信    */
/**/            if(ERROR == (Status = iomnSendJobFinishSts(
/**/                                     ioMNCjst[hJobID].OldmsgQId, imsgid,
/**/                                     hJobID, IOMSG_FIN_ERROR,
/**/                                     IOMSG_NOT_USED, Status)))
/**/            {
/**/                iomnDebugMessage(">>>Error:iomnSendJobFinishSts()\n",0);
/**/                return(ERROR);
/**/            }
/**/        }
/**/        /*  ＪＯＢ再開の場合    */
/**/        else{
/**/            /*  Ack Message登録 */
/**/            ioMNAjst[hJobID].iOldTaskId = iTaskIdIoExMN;
/**/            ioMNAjst[hJobID].iOldMsgId = IOMSG_AJST_EX;
/**/            ioMNAjst[hJobID].OldmsgQId = msgQIdIoExMN;
/**/            ioMNAjst[hJobID].msgparam.sAjstCtl.hJobid = hJobID;
/**/            ioMNAjst[hJobID].msgparam.sAjstCtl.hInUpDown = IOMSG_NOT_USED;
/**/            ioMNAjst[hJobID].msgparam.sAjstCtl.hOutTiming = IOMSG_NOT_USED;
/**/            ioMNAjst[hJobID].msgparam.sAjstCtl.iErrStatus = OK;
/**/            iomnDebugMessage(">>>Set AJST Message\n",0);
/**/
/**/            /*  TJED Messageの登録  */
/**/            ioMNTjed[hJobID].iOldTaskId = iTaskIdIoExMN;
/**/            ioMNTjed[hJobID].iOldMsgId = IOMSG_TJED_EX;
/**/            ioMNTjed[hJobID].OldmsgQId = msgQIdIoExMN;
/**/            ioMNTjed[hJobID].msgparam.sTjedCtl.hJobid = hJobID;
/**/            ioMNTjed[hJobID].msgparam.sTjedCtl.hSts = IOMSG_SUS_ERROR;
/**/            ioMNTjed[hJobID].msgparam.sTjedCtl.iErrSys = IOMSG_NOT_USED;
/**/            ioMNTjed[hJobID].msgparam.sTjedCtl.iErrEng = Status;
/**/            ioMNTjed[hJobID].msgparam.sTjedCtl.hOrgBack = 0;
/**/            iomnDebugMessage(">>>Set TJED Message\n",0);
/**/                    
/**/            /*  ＪＯＢ終了メッセージ送信    */
/**/            if(ERROR == (Status = iomnSendTJED(ioMNCjst[hJobID].OldmsgQId,
/**/                                    imsgidTjed, hJobID, IOMSG_SUS_ERROR,
/**/                                    IOMSG_NOT_USED, Status, 0)))
/**/            {
/**/                iomnDebugMessage(">>>Error:iomnSendTJED()\n",0);
/**/                return(ERROR);
/**/            }
/**/            iomnDebugMessage(">>>Send TJED Message\n",0);
/**/        }
/**/        return(OK);
/**/
/**/      case    S_ioMN_DUAL_FUNCTION:
/**/      case    S_ioMN_MEMORY_FULL:
/**/        iomnDebugMessage(">>>Impossible to Job Start:%x\n",Status);
/**/        /*  新規ＪＯＢ開始または基本複写の場合  */
/**/        if(hContinuity == IOMSG_JB_NEW){
/**/            /*  TJFN Messageの登録  */
/**/            ioMNTjfn[hJobID].iOldTaskId = iTaskIdIoExMN;
/**/            ioMNTjfn[hJobID].iOldMsgId = IOMSG_TJFN_EX;
/**/            ioMNTjfn[hJobID].OldmsgQId = msgQIdIoExMN;
/**/            ioMNTjfn[hJobID].msgparam.sTjfnCtl.hJobid = hJobID;
/**/            ioMNTjfn[hJobID].msgparam.sTjfnCtl.hSts = IOMSG_FIN_ERROR;
/**/            ioMNTjfn[hJobID].msgparam.sTjfnCtl.iErrSys = Status;
/**/            ioMNTjfn[hJobID].msgparam.sTjfnCtl.iErrEng = IOMSG_NOT_USED;
/**/            iomnDebugMessage(">>>Set TJFN Message\n",0);
/**/
/**/            /*  ＪＯＢ完了メッセージ送信    */
/**/            if(ERROR == (Status = iomnSendJobFinishSts(
/**/                                    ioMNCjst[hJobID].OldmsgQId, imsgid,
/**/                                    hJobID, IOMSG_FIN_ERROR, Status,
/**/                                    IOMSG_NOT_USED)))
/**/            {
/**/                iomnDebugMessage(">>>Error:iomnSendJobFinishSts()\n",0);
/**/                return(ERROR);
/**/            }
/**/        }
/**/        /*  ＪＯＢ再開の場合    */
/**/        else{
/**/            /*  Ack Message登録 */
/**/            ioMNAjst[hJobID].iOldTaskId = iTaskIdIoExMN;
/**/            ioMNAjst[hJobID].iOldMsgId = IOMSG_AJST_EX;
/**/            ioMNAjst[hJobID].OldmsgQId = msgQIdIoExMN;
/**/            ioMNAjst[hJobID].msgparam.sAjstCtl.hJobid = hJobID;
/**/            ioMNAjst[hJobID].msgparam.sAjstCtl.hInUpDown = IOMSG_NOT_USED;
/**/            ioMNAjst[hJobID].msgparam.sAjstCtl.hOutTiming = IOMSG_NOT_USED;
/**/            ioMNAjst[hJobID].msgparam.sAjstCtl.iErrStatus = OK;
/**/            iomnDebugMessage(">>>Set AJST Message\n",0);
/**/
/**/            /*  TJED Messageの登録  */
/**/            ioMNTjed[hJobID].iOldTaskId = iTaskIdIoExMN;
/**/            ioMNTjed[hJobID].iOldMsgId = IOMSG_TJED_EX;
/**/            ioMNTjed[hJobID].OldmsgQId = msgQIdIoExMN;
/**/            ioMNTjed[hJobID].msgparam.sTjedCtl.hJobid = hJobID;
/**/            ioMNTjed[hJobID].msgparam.sTjedCtl.hSts = IOMSG_SUS_ERROR;
/**/            ioMNTjed[hJobID].msgparam.sTjedCtl.iErrSys = IOMSG_NOT_USED;
/**/            ioMNTjed[hJobID].msgparam.sTjedCtl.iErrEng = Status;
/**/            ioMNTjed[hJobID].msgparam.sTjedCtl.hOrgBack = 0;
/**/            iomnDebugMessage(">>>Set TJED Message\n",0);
/**/
/**/            /*  ＪＯＢ終了メッセージ送信    */
/**/            if(ERROR == (Status = iomnSendTJED(ioMNCjst[hJobID].OldmsgQId,
/**/                                    imsgidTjed, hJobID, IOMSG_SUS_ERROR,
/**/                                    IOMSG_NOT_USED, Status, 0)))
/**/            {
/**/                iomnDebugMessage(">>>Error:iomnSendTJED()\n",0);
/**/                return(ERROR);
/**/            }
/**/            iomnDebugMessage(">>>Send TJED Message\n",0);
/**/        }
/**/        return(OK);
/**/
/**/      case    ERROR:
/**/        iomnDebugMessage(">>>Error:iomnFaxScnStartConfirm()\n",0);
/**/
/**/        /*  TJFN Messageの登録  */
/**/        ioMNTjfn[hJobID].iOldTaskId = iTaskIdIoExMN;
/**/        ioMNTjfn[hJobID].iOldMsgId = IOMSG_TJFN_EX;
/**/        ioMNTjfn[hJobID].OldmsgQId = msgQIdIoExMN;
/**/        ioMNTjfn[hJobID].msgparam.sTjfnCtl.hJobid = hJobID;
/**/        ioMNTjfn[hJobID].msgparam.sTjfnCtl.hSts = IOMSG_FIN_ERROR;
/**/        ioMNTjfn[hJobID].msgparam.sTjfnCtl.iErrSys = errno;
/**/        ioMNTjfn[hJobID].msgparam.sTjfnCtl.iErrEng = IOMSG_NOT_USED;
/**/        iomnDebugMessage(">>>Set TJFN Message\n",0);
/**/
/**/        /*  ＪＯＢ完了メッセージ送信    */
/**/        if(ERROR == (Status = iomnSendJobFinishSts(ioMNCjst[hJobID].OldmsgQId,
/**/                                imsgid, hJobID, IOMSG_FIN_ERROR, errno,
/**/                                IOMSG_NOT_USED)))
/**/        {
/**/            iomnDebugMessage(">>>Error:iomnSendJobFinishSts()\n",0);
/**/            return(ERROR);
/**/        }
/**/        return(OK);
/**/      default:
/**/        iomnDebugMessage(">>>ERROR:FaxScanJOB Confirmation Status->%x\n",Status);
/**/        break;
/**/    }
#endif

    /*  JOB開始準備処理 */
    if(ERROR == (Status = iomnPrepareForFax(hJobID, hStartTrigger,
                            hContinuity, hJobtype)))
    {
        iomnDebugMessage(">>>Error:iomnPrepareForFax()\n",0);
        /* ＪＯＢ完了メッセージ送信 */
        if(ERROR == (Status = iomnSendJobFinishSts(ioMNCjst[hJobID].OldmsgQId,
                                imsgid, hJobID, IOMSG_FIN_ERROR, errno,
#if 0   /* MCK1.1 新メッセージ対応  引数追加    */
                                IOMSG_NOT_USED)))
#else
                                IOMSG_NOT_USED, hProcessNo)))
#endif
        {
            cmnPrintf_ioMN(2,">>>Error:iomnSendJobFinishSts()\n");
            return(ERROR);
        }
        return(OK);
    }

    /* JOB開始メッセージ送信 */
    if(ERROR == (Status = iomnSendJobStart(hJobID, hStartTrigger,
                            hContinuity, 0)))
    {
        iomnDebugMessage(">>>Error:iomnSendJobStart()\n",0);
        /* ＩＰテーブルと逐次制御テーブルの解放 */
        iomnStart_IP_SIS_Cls( hJobID );
        return(ERROR);
    }
    iomnDebugMessage(">>>OK:iomnSendJobStart()\n",0);

    iomnDebugMessage(">>>>Bottom of iomnFaxScnJobStart()<<<<\n",0);
    return(OK);
}


/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         7.1.2.1.1
TITLE:      
MODULE：    STATUS iomnPrepareForFax(HI hJobid, HI hStartTrigger, HI hContinuity, HI hJobtype)
INCLUDE:    ioMNpar.h
            ioMNstr.h
PARAMETER:
            HI      hJobid;             JOBID   
            HI      hStartTrigger;      開始要因
            HI      hContinuity;        継続or新規  
            HI      hJobtype;           JOB種別
RETURNS:    I32 4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            拡張FAX開始指示メッセージ受信時の開始準備処理。
NOTES:
HISTORY:
        1.1     Y.Matsuda   97/7/30
        D1.7.1 J.Ootani 00/03/01    DM45/35 SEINE FAX F-CODE対応(マージ)
        D1.7.8 J.Ootani 00/03/11    DM45/35 FAXジョブ開始終了対応
        D2.1   T.Fujii  00/06/06    DM45/35 V004.60不具合対応(parMNの原稿枚数をクリア)
*/
STATUS  iomnPrepareForFax(HI hJobid, HI hStartTrigger, HI hContinuity, HI hJobtype)
{
    STATUS  Status;
    HI      hIpTblID;                       /*  ＩＰテーブルＩＤ */

    iomnDebugMessage(">>>>Top of iomnPrepareForFax()<<<<\n",0);

    /*  CHANGE Bus From Pm to Fax   */
    if(hJobtype ==  COM_JB_SCN_FAX1
       || hJobtype == COM_JB_POLL_SEND_FAX1){
        if(ERROR == (Status = cmnChgBus(IMGBUS_FAX))){
            iomnDebugMessage(">>>Error:cmnChgBus()\n",0);
            return(ERROR);
        }
        iomnDebugMessage(">>>OK:cmnChgBus()\n",0);
    }

    /* 新規の場合 */
    if(hContinuity == IOMSG_JB_NEW){
        /* parMNの原稿枚数をクリア */
        if( parMNSetPages(hJobid, 0) == ERROR ) {
            cmnPrintf_ioMN(2,"[ioMN]ERROR:parMNSetPages(JobID=%d)\n",hJobid);
        }

        /* ipテーブルの取得とリンク情報の確立 */
        hIpTblID = ipLibEntryJob( hJobid );

        if( hIpTblID != ERROR ) {
            /* ipテーブルの確保ができた場合 */
            /* ジョブIDごとのジョブ情報エリアにipテーブルIDを保存 */
            iomnJobInfo[hJobid].hIpTblID = hIpTblID;

            /* 逐次制御テーブルに入力ジョブを登録 */
            if(iomnSyncTblEntryJob(hIpTblID,COM_JB_SUB_INPUT,hJobid) != ERROR)
            {
                /* 逐次制御テーブルをIPテーブル使用中に更新 */
                iomnSyncTblSetStatus(hIpTblID,COM_JB_SUB_INPUT,
                                                  IOMN_IPTBL_USING);
                /* サブジョブ種別(INPUT)設定 */
                if(parMNSetSubJobmode(hJobid,COM_JB_SUB_INPUT) == ERROR){
                    iomnDebugMessage(">>ERROR:parMNSetSubJobmode()\n",0);
                    /* 上位に返る前に、ＩＰテーブル／逐次制御テーブル */
                    /* の解放を行う */
                    iomnStart_IP_SIS_Cls( hJobid );
                   return(ERROR);
                }
                /*入出力動作モード（後追い完了）の設定 */
                if( parMNSetIOMode(hJobid,COM_OT_TRK_FIN) == ERROR){
                    iomnDebugMessage( ">>>Error:parMNSetIOMode()\n", 0 );
                    /* 上位に返る前に、ＩＰテーブル／逐次制御テーブルの解放 */
                    /* を行う */
                    iomnStart_IP_SIS_Cls( hJobid );
                    return( ERROR );
                }
            }
            else
            {   /* エラー時は、ｉｐテーブル解放 */
                iomnDebugMessage(">>ERROR:iomnSyncTblEntryJob()\n",0);
                ipLibDeleteJob( hIpTblID );
                return(ERROR);
            }
        }
        else
        {   /* ipテーブルの確保に失敗 */
            iomnDebugMessage(">>ERROR:ipLibEntryJob()\n",0);
            return(ERROR);
        }
    }

    iomnDebugMessage(">>>>Bottom of iomnPrepareForFax()<<<<\n",0);
    return(OK);
}


/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         7.1.2.2
TITLE:      FAXリスト／レポート印刷JOB開始処理
MODULE：    STATUS  iomnLstFaxStart(HI hJobID, HI hStartTrigger, HI hContinuity, HI hJobtype)

INCLUDE:    ioMNPar.h
            ioMNbuf.h
PARAMETER:
            HI      hJobID;             JOBID   
            HI      hStartTrigger;      開始要因
            HI      hContinuity;        継続or新規  
            HI      hJobtype;           JOB種別

RETURNS:    I32 4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            フロントJOBにてＦａｘリスト印刷ＪＯＢ開始指示を受信し、JOB開始指示を送信する。
NOTES:
HISTORY:
        1.1     Y.Matsuda   97/04/12
        D1.2.1  T.Fujii     00/02/23  DM45/35 フィニッシャ初期化修正
        D2.4.1  T.Fujii     00/06/17  DM45/35 V004.70不具合対応
                                              AJSTパラメータ拡張対応修正

        MCK1.1  M.Karasawa 02/06/11 McKinley    新メッセージ対応
    V018.00_01  M.Karasawa  03/04/09    McKinley    メッセージ変更対応
    V019.00_02  M.Karasawa  03/05/20    McKinley    メッセージメンバ追加対応（TJSX追加、EPSTとEJABは対応不要）
    V023.00_01  M.Karasawa  03/07/16    McKinley    エラー終了時TJFNに通し番号が入らない不具合の修正
*/
STATUS  iomnLstFaxStart(HI hJobID, HI hStartTrigger, HI hContinuity, HI hJobtype, HI hProcessNo)
{
    STATUS  Status = ERROR; /*  処理結果    */  /* D1.1 T.Fujii ダミー定義追加 */
    #ifdef DEBUG    /* D1.1 T.Fujii 99/11/15 マクロ指定追加 */
    HI      hJobstatus; /*  JOB状態     */
    I32     iSuscause;  /*  中断要因    */
    #endif  /* D1.1 DEBUG */
    STATUS  iConfirm;   /*  確認結果    */
#if 0   /* MCK1.1 新メッセージ対応  */
/**/    I32     imsgid= IOMSG_TJFN_UI;      /*  メッセージＩＤ  */  /* D1.1 T.Fujii ダミー定義追加 */
/**/    I32     imsgidTjed = IOMSG_TJED_UI; /*  ＪＯＢ終了メッセージＩＤ    */  /* D1.1 T.Fujii ダミー定義追加 */
#else
    I32     imsgid= JOB_MSG_TJFN;
    I32     imsgidTjed = JOB_MSG_TJSX;
#endif
    iomnDebugMessage(">>>>Top of iomnLstFaxStart()<<<<\n",0);
    iomnDebugMessage("### JOBID --> %x\n",(I32)hJobID);
    iomnDebugMessage("### hStartTrigger --> %x\n",(I32)hStartTrigger);
    iomnDebugMessage("### hContinuity --> %x\n",(I32)hContinuity);

#if 0   /* MCK1.1 新メッセージ対応  */
/**/    /*  messageID取得   */
/**/    if(ioMNCjst[hJobID].iOldMsgId == IOMSG_CJST_UI){
/**/    }
/**/    if(ioMNCjst[hJobID].iOldMsgId == IOMSG_CJST_JOB){
/**/        imsgid = IOMSG_TJFN_JOB;
/**/        imsgidTjed = IOMSG_TJED_JOB;
/**/    }
/**/    if(ioMNCjst[hJobID].iOldMsgId == IOMSG_CJST_DIG){
/**/        imsgid = IOMSG_TJFN_DIG;
#endif

    /*  実行要求ＪＯＢの実行可否確認処理（job種別、ＪＯＢ状態、Ｈ／Ｗ構成）*/
    iConfirm = iomnFaxLstStartConfirm(hJobID);
    switch(iConfirm){
        case    IOMN_ON:
        case    S_ioMN_FAX_RECIEVING:
                if(ioMNstatus.hJobStart == IOMN_ON){
                    iomnDebugMessage(">>>Wait... Back Job Suspending\n",0);

                    /*  JOB中断後の再開情報格納 */
                    iomnSetStartQue(&ioMNFrontQue, hJobID, hStartTrigger, hContinuity, 0, hJobtype);
                    iomnDebugMessage(">>>OK:iomnSetStartQue()\n",0);
                    ioMNstatus.hJobStart = IOMN_OFF;    
                    return(OK);
                }
                else{
                    /*  JOB開始メッセージ送信   */
                    if(ERROR == (Status = iomnSendJobStart(hJobID, hStartTrigger, hContinuity, 0))){
                        iomnDebugMessage(">>>Error:iomnSendJobStart()\n",0);
                        return(ERROR);
                    }
                    iomnDebugMessage(">>>OK:iomnSendJobStart()\n",0);
                }
                break;

        case    S_ioMN_ENGINE_ERROR:
        case    S_ioMN_MACHINE_RECOVERY:
        case    S_ioMN_TEST_STAPLE:
                iomnDebugMessage(">>>Impossible to Job Start:%x\n",Status);
                /*  新規ＪＯＢ開始または基本複写の場合  */
                if(hContinuity == IOMSG_JB_NEW){
                    /*  TJFN Messageの登録  */
                    ioMNTjfn[hJobID].iOldTaskId = iTaskIdIoExMN;
#if 0   /* MCK1.1 新メッセージ対応  */
/**/                    ioMNTjfn[hJobID].iOldMsgId = IOMSG_TJFN_EX;
#else
                    ioMNTjfn[hJobID].iOldMsgId = JOB_MSG_TJFN;
#endif
                    ioMNTjfn[hJobID].OldmsgQId = msgQIdIoExMN;
                    ioMNTjfn[hJobID].msgparam.sTjfnCtl.hJobid = hJobID;
                    ioMNTjfn[hJobID].msgparam.sTjfnCtl.hSts = IOMSG_FIN_ERROR;
                    ioMNTjfn[hJobID].msgparam.sTjfnCtl.iErrSys = IOMSG_NOT_USED;
                    ioMNTjfn[hJobID].msgparam.sTjfnCtl.iErrEng = Status;
                    /* MCK1.1 新メッセージ対応  */
                    ioMNTjfn[hJobID].msgparam.sTjfnCtl.hProcessType = hProcessNo;
                    /* V018.00_01 START */
                    ioMNTjfn[hJobID].msgparam.sTjfnCtl.hJcbID     = ioMNCjst[hJobID].msgparam.sCjstCtl.hJcbID;
                    ioMNTjfn[hJobID].msgparam.sTjfnCtl.hTNodel    = IOMSG_NOT_USED;
                    ioMNTjfn[hJobID].msgparam.sTjfnCtl.hUkedel    = IOMSG_NOT_USED;
                    ioMNTjfn[hJobID].msgparam.sTjfnCtl.hAppliType = ioMNCjst[hJobID].msgparam.sCjstCtl.hAppliType;
                    /* V018.00_01 END   */
                    /* V023.00_01 START */
                    ioMNTjfn[hJobID].msgparam.sTjfnCtl.hTNo       = ioMNCjst[hJobID].msgparam.sCjstCtl.hTNo;
                    /* V023.00_01 END   */
                    iomnDebugMessage(">>>Set TJFN Message\n",0);

                    /*  ＪＯＢ完了メッセージ送信    */
                   if(ERROR == (Status = iomnSendJobFinishSts(ioMNCjst[hJobID].OldmsgQId, imsgid,
#if 0   /* MCK1.1 新メッセージ対応  引数追加    */
                                                hJobID, IOMSG_FIN_ERROR, IOMSG_NOT_USED, Status))){
#else
                                                hJobID, IOMSG_FIN_ERROR, IOMSG_NOT_USED, Status, hProcessNo))){
#endif
                        iomnDebugMessage(">>>Error:iomnSendJobFinishSts()\n",0);
                        return(ERROR);
                    }
                }
                /*  ＪＯＢ再開の場合    */
                else{
                    /*  Ack Message登録 */
                    ioMNAjst[hJobID].iOldTaskId = iTaskIdIoExMN;
#if 0   /* MCK1.1 新メッセージ対応  */
/**/                    ioMNAjst[hJobID].iOldMsgId = IOMSG_AJST_EX;
#else
                    ioMNAjst[hJobID].iOldMsgId = JOB_MSG_AJST;
#endif
                    ioMNAjst[hJobID].OldmsgQId = msgQIdIoExMN;
                    ioMNAjst[hJobID].msgparam.sAjstCtl.hJobid = hJobID;
                    ioMNAjst[hJobID].msgparam.sAjstCtl.hInUpDown = IOMSG_NOT_USED;
                    ioMNAjst[hJobID].msgparam.sAjstCtl.hOutTiming = IOMSG_NOT_USED;
                    ioMNAjst[hJobID].msgparam.sAjstCtl.iErrStatus = OK;
                    /* V018.00_01 START */
                    ioMNAjst[hJobID].msgparam.sAjstCtl.hWarSts      = IOMSG_NOT_USED;
                    ioMNAjst[hJobID].msgparam.sAjstCtl.iWarErrSys   = IOMSG_NOT_USED;
                    ioMNAjst[hJobID].msgparam.sAjstCtl.iWarErrEng   = IOMSG_NOT_USED;
                    ioMNAjst[hJobID].msgparam.sAjstCtl.hTNo         = ioMNCjst[hJobID].msgparam.sCjstCtl.hTNo;
                    ioMNAjst[hJobID].msgparam.sAjstCtl.hProcessType = hProcessNo;
                    ioMNAjst[hJobID].msgparam.sAjstCtl.hJcbID       = ioMNCjst[hJobID].msgparam.sCjstCtl.hJcbID;
                    ioMNAjst[hJobID].msgparam.sAjstCtl.hAppliType   = ioMNCjst[hJobID].msgparam.sCjstCtl.hAppliType;
                    /* V018.00_01 END   */
                    iomnDebugMessage(">>>Set AJST Message\n",0);

                    /*  TJED Messageの登録  */
                    ioMNTjed[hJobID].iOldTaskId = iTaskIdIoExMN;
#if 0   /* MCK1.1 新メッセージ対応  */
/**/                    ioMNTjed[hJobID].iOldMsgId = IOMSG_TJED_EX;
#else
                    ioMNTjed[hJobID].iOldMsgId = JOB_MSG_TJSX;
#endif
                    ioMNTjed[hJobID].OldmsgQId = msgQIdIoExMN;
                    ioMNTjed[hJobID].msgparam.sTjedCtl.hJobid       = hJobID;
                    ioMNTjed[hJobID].msgparam.sTjedCtl.hSts         = IOMSG_SUS_ERROR;
                    ioMNTjed[hJobID].msgparam.sTjedCtl.iErrSys      = IOMSG_NOT_USED;
                    ioMNTjed[hJobID].msgparam.sTjedCtl.iErrEng      = Status;
                    ioMNTjed[hJobID].msgparam.sTjedCtl.hOrgBack     = 0;
                    /* V018.00_01 START */
                    ioMNTjed[hJobID].msgparam.sTjedCtl.hTNo         = ioMNCjst[hJobID].msgparam.sCjstCtl.hTNo;
                    ioMNTjed[hJobID].msgparam.sTjedCtl.hProcessType = hProcessNo;
                    ioMNTjed[hJobID].msgparam.sTjedCtl.hJcbID       = ioMNCjst[hJobID].msgparam.sCjstCtl.hJcbID;
                    ioMNTjed[hJobID].msgparam.sTjedCtl.hAppliType   = ioMNCjst[hJobID].msgparam.sCjstCtl.hAppliType;
                    /* V018.00_01 END   */
                    /* V019.00_02 START */
                    ioMNTjed[hJobID].msgparam.sTjedCtl.hPaperSize   = IOMSG_NOT_USED;
                    ioMNTjed[hJobID].msgparam.sTjedCtl.hPaperType   = IOMSG_NOT_USED;
                    ioMNTjed[hJobID].msgparam.sTjedCtl.hPaperSource = IOMSG_NOT_USED;
                    /* V019.00_02 END   */
                    iomnDebugMessage(">>>Set TJED Message\n",0);

                    /*  ＪＯＢ終了メッセージ送信    */
                    if(ERROR == (Status = iomnSendTJED(ioMNCjst[hJobID].OldmsgQId, imsgidTjed,
                                            hJobID, IOMSG_SUS_ERROR, IOMSG_NOT_USED, Status, 0))){
                        iomnDebugMessage(">>>Error:iomnSendTJED()\n",0);
                        return(ERROR);
                    }
                    iomnDebugMessage(">>>Send TJED Message\n",0);
                }
                return(OK);
                
        case    S_ioMN_MEMORY_FULL:
        case    S_ioMN_DUAL_FUNCTION:
                iomnDebugMessage(">>>Impossible to Job Start:%x\n",Status);

                /*  TJFN Messageの登録  */
                ioMNTjfn[hJobID].iOldTaskId = iTaskIdIoExMN;
#if 0   /* MCK1.1 新メッセージ対応  */
/**/                ioMNTjfn[hJobID].iOldMsgId = IOMSG_TJFN_EX;
#else
                ioMNTjfn[hJobID].iOldMsgId = JOB_MSG_TJFN;
#endif
                ioMNTjfn[hJobID].OldmsgQId = msgQIdIoExMN;
                ioMNTjfn[hJobID].msgparam.sTjfnCtl.hJobid = hJobID;
                ioMNTjfn[hJobID].msgparam.sTjfnCtl.hSts = IOMSG_FIN_ERROR;
                ioMNTjfn[hJobID].msgparam.sTjfnCtl.iErrSys = Status;
                ioMNTjfn[hJobID].msgparam.sTjfnCtl.iErrEng = IOMSG_NOT_USED;
                /* MCK1.1 新メッセージ対応  */
                ioMNTjfn[hJobID].msgparam.sTjfnCtl.hProcessType = hProcessNo;
                /* V018.00_01 START */
                ioMNTjfn[hJobID].msgparam.sTjfnCtl.hJcbID     = ioMNCjst[hJobID].msgparam.sCjstCtl.hJcbID;
                ioMNTjfn[hJobID].msgparam.sTjfnCtl.hTNodel    = IOMSG_NOT_USED;
                ioMNTjfn[hJobID].msgparam.sTjfnCtl.hUkedel    = IOMSG_NOT_USED;
                ioMNTjfn[hJobID].msgparam.sTjfnCtl.hAppliType = ioMNCjst[hJobID].msgparam.sCjstCtl.hAppliType;
                /* V018.00_01 END   */
                /* V023.00_01 START */
                ioMNTjfn[hJobID].msgparam.sTjfnCtl.hTNo       = ioMNCjst[hJobID].msgparam.sCjstCtl.hTNo;
                /* V023.00_01 END   */
                iomnDebugMessage(">>>Set TJFN Message\n",0);

                /*  ＪＯＢ完了メッセージ送信    */
                if(ERROR == (Status = iomnSendJobFinishSts(ioMNCjst[hJobID].OldmsgQId, imsgid,
#if 0   /* MCK1.1 新メッセージ対応  引数追加    */
                                            hJobID, IOMSG_FIN_ERROR, Status, IOMSG_NOT_USED))){
#else
                                            hJobID, IOMSG_FIN_ERROR, Status, IOMSG_NOT_USED, hProcessNo))){
#endif
                    iomnDebugMessage(">>>Error:iomnSendJobFinishSts()\n",0);
                    return(ERROR);
                }
                return(OK);

        case    OK:
                /*  JOB開始処理のキューイング   */
                iomnSetStartQue(&ioMNFrontQue, hJobID, hStartTrigger, hContinuity, 0, hJobtype);
                iomnDebugMessage(">>>OK:iomnSetStartQue()\n",0);
                return(OK);

        case    ERROR:
                iomnDebugMessage(">>>Error:iomnFaxScnStartConfirm()\n",0);

                /*  TJFN Messageの登録  */
                ioMNTjfn[hJobID].iOldTaskId = iTaskIdIoExMN;
#if 0   /* MCK1.1 新メッセージ対応  */
/**/                ioMNTjfn[hJobID].iOldMsgId = IOMSG_TJFN_EX;
#else
                ioMNTjfn[hJobID].iOldMsgId = JOB_MSG_TJFN;
#endif
                ioMNTjfn[hJobID].OldmsgQId = msgQIdIoExMN;
                ioMNTjfn[hJobID].msgparam.sTjfnCtl.hJobid = hJobID;
                ioMNTjfn[hJobID].msgparam.sTjfnCtl.hSts = IOMSG_FIN_ERROR;
                ioMNTjfn[hJobID].msgparam.sTjfnCtl.iErrSys = errno;
                ioMNTjfn[hJobID].msgparam.sTjfnCtl.iErrEng = IOMSG_NOT_USED;
                /* MCK1.1 新メッセージ対応  */
                ioMNTjfn[hJobID].msgparam.sTjfnCtl.hProcessType = hProcessNo;
                /* V018.00_01 START */
                ioMNTjfn[hJobID].msgparam.sTjfnCtl.hJcbID     = ioMNCjst[hJobID].msgparam.sCjstCtl.hJcbID;
                ioMNTjfn[hJobID].msgparam.sTjfnCtl.hTNodel    = IOMSG_NOT_USED;
                ioMNTjfn[hJobID].msgparam.sTjfnCtl.hUkedel    = IOMSG_NOT_USED;
                ioMNTjfn[hJobID].msgparam.sTjfnCtl.hAppliType = ioMNCjst[hJobID].msgparam.sCjstCtl.hAppliType;
                /* V018.00_01 END   */
                /* V023.00_01 START */
                ioMNTjfn[hJobID].msgparam.sTjfnCtl.hTNo       = ioMNCjst[hJobID].msgparam.sCjstCtl.hTNo;
                /* V023.00_01 END   */
                iomnDebugMessage(">>>Set TJFN Message\n",0);

                /*  JOB実行失敗をメッセージ送信元に通知 */
                if(ERROR == (Status = iomnSendJobFinishSts(ioMNCjst[hJobID].OldmsgQId, imsgid, hJobID,
#if 0   /* MCK1.1 新メッセージ対応  引数追加    */
                                                             IOMSG_FIN_ERROR,errno, IOMSG_NOT_USED))){
#else
                                                             IOMSG_FIN_ERROR,errno, IOMSG_NOT_USED, hProcessNo))){
#endif
                    iomnDebugMessage(">>>Error:iomnSendJobFinishSts()\n",0);
                    return(ERROR);
                }
                iomnDebugMessage(">>>OK:Send JOB FINISH Status Message\n",0);

        default:
                /*  TJFN Messageの登録  */
                ioMNTjfn[hJobID].iOldTaskId = iTaskIdIoExMN;
#if 0   /* MCK1.1 新メッセージ対応  */
/**/                ioMNTjfn[hJobID].iOldMsgId = IOMSG_TJFN_EX;
#else
                ioMNTjfn[hJobID].iOldMsgId = JOB_MSG_TJFN;
#endif
                ioMNTjfn[hJobID].OldmsgQId = msgQIdIoExMN;
                ioMNTjfn[hJobID].msgparam.sTjfnCtl.hJobid = hJobID;
                ioMNTjfn[hJobID].msgparam.sTjfnCtl.hSts = IOMSG_FIN_ERROR;
                ioMNTjfn[hJobID].msgparam.sTjfnCtl.iErrSys = errno;
                ioMNTjfn[hJobID].msgparam.sTjfnCtl.iErrEng = IOMSG_NOT_USED;
                /* MCK1.1 新メッセージ対応  */
                ioMNTjfn[hJobID].msgparam.sTjfnCtl.hProcessType = hProcessNo;
                /* V018.00_01 START */
                ioMNTjfn[hJobID].msgparam.sTjfnCtl.hJcbID     = ioMNCjst[hJobID].msgparam.sCjstCtl.hJcbID;
                ioMNTjfn[hJobID].msgparam.sTjfnCtl.hTNodel    = IOMSG_NOT_USED;
                ioMNTjfn[hJobID].msgparam.sTjfnCtl.hUkedel    = IOMSG_NOT_USED;
                ioMNTjfn[hJobID].msgparam.sTjfnCtl.hAppliType = ioMNCjst[hJobID].msgparam.sCjstCtl.hAppliType;
                /* V018.00_01 END   */
                /* V023.00_01 START */
                ioMNTjfn[hJobID].msgparam.sTjfnCtl.hTNo       = ioMNCjst[hJobID].msgparam.sCjstCtl.hTNo;
                /* V023.00_01 END   */
                iomnDebugMessage(">>>Set TJFN Message\n",0);

                /*  JOB実行失敗をメッセージ送信元に通知 */
                if(ERROR == (Status = iomnSendJobFinishSts(ioMNCjst[hJobID].OldmsgQId, imsgid, hJobID,
#if 0   /* MCK1.1 新メッセージ対応  引数追加    */
                                                             IOMSG_FIN_ERROR,errno, IOMSG_NOT_USED))){
#else
                                                             IOMSG_FIN_ERROR,errno, IOMSG_NOT_USED, hProcessNo))){
#endif
                    iomnDebugMessage(">>>Error:iomnSendJobFinishSts()\n",0);
                    return(ERROR);
                }
                return(OK);
    }

    #ifdef  DEBUG
        jobStatus(hJobID, &hJobstatus, &iSuscause);
        iomnDebugMessage("*** JOB STATUS -->%x\n",(I32)hJobstatus);
    #endif

    iomnDebugMessage(">>>>Bottom of iomnLstFaxStart()<<<<\n",0);
    return(OK);
}

/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         7.1.3
TITLE:      PrintJOB開始処理
MODULE：    STATUS  iomnPrnJobStart(HI hJobID, HI hStartTrigger, HI hContinuity, HI hPrtType, HI hJobtype)
INCLUDE:    ioMNPar.h
            ioMNbuf.h
PARAMETER:
            HI      hJobID;             JOBID   
            HI      hStartTrigger;      開始要因
            HI      hContinuity;        継続or新規  
            HI      hPrtType;           印刷JOBTYPE
            HI      hJobtype;           JOB種別
RETURNS:    I32 4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            印刷Queに印刷JOBを登録する
NOTES:
HISTORY:
        1.1     Y.Matsuda   96/9/18
        1.2     Y.Matsuda   97/11/25
                        印刷JOB開始要因の変更
                            IOMSG_IN_BYPAS時
                            IOMSG_IN_FORCE時
        2.1     Y.Matsuda   97/12/10    Ver 2.1
                        印刷JOB開始前にPMJOBIDを再確保する処理を導入
        V2.7    J.Ootani    97/12/08
            ＤＳＳ対応
        V3.7    J.Ootani    97/12/10
            手差し印刷開始指示に対するＡＣＫ送信処理追加
            紙サイズ不一致で中断している場合の印刷開始指示に対するＡＣＫ送信処理追加
        V4.3    J.Ootani    98/1/28
            開始指示に対するNO START TC02送信処理追加
        Ver3.0DSS yMatsuda  98/02/19
            ＤＳＳ通信中フラグ解除追加
        2.2     E.Saka      98/01/22
                        FAX夜間印刷OFF設定時はリストレポートJOB印刷
                        開始時も電源ONしない
        3.1     Y.Matsuda   98/02/26
                    NVRAM異常対応
        V5.1D   yMatsuda    98/06/02
            Repairing for Trouble that while walkup user is in operating the UI,
            IOMN return incorrect status to DSSPRN.
    M1.1(TJ)H.Takahashi 98/08/10
        手置き逐次複写対応のため、iomnSendTJED()でJOB状態を変更しない為
    M1.1a(TJ)H.Takahashi 98/08/10
        NILE最新版マージ時におけるM1.1対応のため
    M1.2(TJ)H.Takahashi 98/08/12
        DSS印刷時のPMID設定対応のため
    M1.3(TJ)H.Takahashi 98/09/25
            DSS逐次対応
    M6.21(TJ)H.Takahashi    99/06/01
            CS中断→再開→CS中断対応
    D1.2    T.Fujii     99/12/24    DM45/35 (後追い)逐次印刷対応
    D1.2.1  T.Fujii     00/02/23    DM45/35 逐次印刷修正
    D1.2.4  T.Fujii     00/04/10    DM45/35 サブジョブ中断・再開処理修正
    D1.13   T.Fujii     00/05/17    DM45/35 AJSTパラメータ拡張対応
    D2.9    M.Fukuda    00/07/04    DM45/35 FAXリストレポート対応
    D3.5    T.Fujii     00/08/08    DM45/35 再開CJSTに対するAck送信先修正
    D3.28.2 J.Ootani    00/10/07    DM45/35 Sleep移行チェックはfb関数を使用するためhFaxConnectionを使用しない対応
    D3.28.7 J.Ootani    00/10/19    DM45/35 電源ON処理を印刷Queチェック時に統一対応

        MCK1.1  M.Karasawa  02/06/11    McKinley    新メッセージ対応
        MCK1.5  M.Karasawa  02/07/25    McKinley    後追い逐次複写対応
    V008.t1_01  M.Karasawa  02/09/30    McKinley    印刷ジョブ登録時、入力が終わっている時の対策
    V010.00_02  M.Karasawa  02/12/05    McKinley    印刷のAJST送信タイミング変更
    V011.00_01  M.Karasawa  03/01/14    McKinley    中断処理対応
    V012.00_02  M.Karasawa  03/01/24    McKinley    印刷ジョブの再開処理対応
    V015.00_08  M.Karasawa  03/03/05    McKinley    メイン処理の軽減対策
    V031.00_01  M.Karasawa  03/10/31    McKinley    印刷資源取得失敗時にエラー通知を行うようにする(McK4743)
    V041.00_01  M.Karasawa  04/04/13    McKinley    タブ紙確認中断対応
RioGrande
    Beta4   M.Karasawa 04/11/09 RioGrande   インサータ確認中断対応

K2
    VTK10.100   M.Karasawa  06/02/20    K2  Pre7641対策
*/
#if 0   /* MCK1.1 新メッセージ対応  引数追加    */
/**/STATUS  iomnPrnJobStart(HI hJobID, HI hStartTrigger, HI hContinuity, HI hPrtType, HI hJobtype)
#else
STATUS  iomnPrnJobStart(HI hJobID, HI hStartTrigger, HI hContinuity, HI hPrtType, HI hJobtype, HI hProcessNo)
#endif
{
    STATUS  Status;     /*  Status of Function  */
/* 10911 fix*/
    HI      hJobstatus, hJobStatus_Local; /*  JOB Status  */
    I32     iSuscause, iSusCause_Local;  /*  Cause for Suspend   */
#if defined (WEISS_ENG) || defined (S2_ENG)
	HI hDelayTime;
	HI hNoPrnJob = FALSE;
	HI hPrintJobid, hPrintJobtype, hPrintJobstatus;
	HI hRestBusu,hRestTani;
	HI hJobSkip = FALSE;
#endif
    /* D3.28.7 Start J.Ootani 電源ON処理を印刷Queチェック時に統一対応 */
    #if 0
/**/    HI      hfaxprt ;
    #endif
    /* D3.28.7 End */
#if 0   /* MCK1.1 新メッセージID対応*/
/**/    I32     imsgid=IOMSG_TJFN_UI;       /*  MessageID   */
#else
/*      I32     imsgid=JOB_MSG_TJFN;      *//*  MessageID   */															/* SM-001 */
#endif
#if 0   /* PRINT_JOINT *//* 中断中断 */
/* PRINT_JOINT */    HI      hEntryId;
#endif
    HI      hJobType2=0;    /* JOB種別  *//* MCK1.5   *//* --- 後追い逐次 --- */										/* SM-001 */

    /* V031.00_01 *//* ADD START    */
/*  H16                 siIdx_no;   *//* インデックス番号     */														/* SM-001 */
/*  JOB_EXECDATA_TBL    *sJobTbl;   *//* 実行データテーブル   */														/* SM-001 */
    /* V031.00_01 *//* ADD END  */

    iomnDebugMessage("### JOBID --> %x\n",(I32)hJobID);
    iomnDebugMessage("### hStartTrigger --> %x\n",(I32)hStartTrigger);
    iomnDebugMessage("### hContinuity --> %x\n",(I32)hContinuity);
    iomnDebugMessage("### Print Type -> %x\n",(I32)hPrtType);
    iomnDebugMessage("### JOB TYPE -> %x\n",(I32)hJobtype);
/* 10911 fix*/
	jobStatus(hJobID, &hJobStatus_Local, &iSusCause_Local);
	if(hJobStatus_Local == COM_JS_SUSPEND && iSusCause_Local == IOMSG_SUS_REG)
	{
		jobSetStatus(hJobID, COM_JS_READY, 0);
		cmnPrintf_ioMN(7, "iomnPrnJobStart(): jobstatus: %x, SusCause: %x, hence setting the jobstatus as COM_JS_READY (1)\n", hJobStatus_Local, iSusCause_Local);
	}
    
	if(hJobStatus_Local == COM_JS_SUSPEND && iSusCause_Local == IOMSG_ERR_REG_RSLOCK)	//DCR_46831
    {
        cmnPrintf_ioMN(7, "DCR_46831 Job Registration FAILED because of SYSMN_LOCK, hence sending TJFN with FIN_ERROR\n");
        if(ERROR == (Status = iomnSendTjfn(hJobID, IOMSG_FIN_ERROR, IOMSG_ERR_REG_RSLOCK, IOMSG_NOT_USED))){
            iomnDebugMessage(">>>Error:iomnSendJobFinishSts() DCR_46831\n",0);
            return(ERROR);
        }
        iomnCleanJobManagementMsg(hJobID);
        return(OK);
    }
	
	if(hJobStatus_Local == COM_JS_SUSPEND && iSusCause_Local == IOMSG_QUE_ENTRY_ERROR)
	{
		cmnPrintf_ioMN(7, "Que entry error occurred during RJRE(job registration); hence sending TJFN with FIN_ERROR\n");
		if(ERROR == (Status = iomnSendTjfn(hJobID, IOMSG_FIN_ERROR, IOMSG_JB_ENTRYERR, IOMSG_NOT_USED))){
			iomnDebugMessage(">>>Error:iomnSendJobFinishSts()\n",0);
			return(ERROR);
		}
		iomnCleanJobManagementMsg(hJobID);
		return(OK);
	}


#if defined (WEISS_ENG) || defined (S2_ENG)
	if(parMNNV08ParamI(FPOT_DELAY_TIME,NVM_NO_SCODE,&hDelayTime) == ERROR)
	{
		cmnPrintf_ioMN(2,"iomnPrnJobStart(): FPOT_DELAY_TIME  returned ERROR\n");
		return ERROR;
	}
	hDelayTime = hDelayTime*100;/*converting to mSec*/
	if((hDelayTime > 0) && (hJobtype == COM_JB_RECV_DSI)){
             if(ERROR == (Status = iomnGetPrintJob(&hPrintJobid, &hPrintJobtype, &hPrintJobstatus)))
             {
                  iomnDebugMessage(">>>Error:iomnGetPrintJob()\n",0);
                  return(ERROR);
             }
		if(hPrintJobid == 0 && hPrintJobtype == 0  && hPrintJobstatus == 0)
	       {
			hNoPrnJob = TRUE;	       
                     iomnDebugMessage("***iomnSetPrnTime()hNoPrnJob\n",0);
			 if( parMNRestNumberOfCopies(hJobID,&hRestBusu) == ERROR)
		        {
				cmnPrintf_ioMN(2,"iomnPrnJobStart(): parMNRestNumberOfCopies func returned ERROR\n");
				return ERROR;
		        }
		        if( parMNRestUnit(hJobID,&hRestTani) == ERROR)
		        {
				cmnPrintf_ioMN(2,"iomnPrnJobStart(): parMNRestUnit func returned ERROR\n");
				return ERROR;
		        }
			if( hRestBusu > 0 || hRestTani > 0 )
			{
				hJobSkip = TRUE;
			}					 
		}
		if((hNoPrnJob == TRUE)&&(hJobSkip != TRUE))
		{
			if(hDelayTime <= 1000) /* comparing with 1000 msec or 1 sec*/
			{
	            cmnPrintf_ioMN(7,"Delaying CJST to ioExMN by %d mSec[Roopesh]\n",hDelayTime*100);
				taskDelay(hDelayTime/10);
			}
			else
			{
				cmnPrintf_ioMN(7,"Delaying CJST to ioExMN by  %d sec [Roopesh]\n",hDelayTime/1000);
				gettimeofday(&tPreTime, NULL);
				tPreTime.tv_sec = tPreTime.tv_sec +hDelayTime/1000;
				tPreTime.tv_usec = tPreTime.tv_usec +((hDelayTime%1000)*1000);
				hioMNDelayCJST= TRUE;
			}
		}
	}
#endif		
    /*  JOB状態取得 */
    if(ERROR == (Status = jobStatus(hJobID, &hJobstatus, &iSuscause))){
        cmnPrintf_ioMN(2,">>>Error:jobStatus()\n");
        printErrno(errno);
        return(ERROR);
    }
    cmnPrintf_ioMN(7, "###JOB Status->%x, cause = %x\n",hJobstatus, iSuscause);

    /*  手差し給紙の場合は印刷開始コマンドを投げる  */
	if(IOMSG_SUS_RETURN == iSuscause)
	{
		/*Here Just change the job status to PRT_Waiting this is equivalent to RETURN Status in TJFN */
	    if(ERROR == (Status = jobSetStatus(hJobID, COM_JS_PRT_WAITING, 0))){
			cmnPrintf_ioMN(7,">>>Error:jobsetstatus()\n",0);
			printErrno(errno);
			return(ERROR);
		}
	}
	else
    if(hStartTrigger == IOMSG_IN_BYPAS
        || hStartTrigger == IOMSG_IN_TAB    /* V041.00_01 */
        || hStartTrigger == IOMSG_IN_INS    /* Beta4 */
        || (hJobstatus == COM_JS_SUSPEND 
            && (iSuscause & 0x0000ffff) == (I32)IOMSG_SUS_BYPASS))
    {
        /*  印刷準備処理    */
        if(ERROR == (Status = iomnPrepareForPrint(hJobID, hJobtype))){
            cmnPrintf_ioMN(0,">>>Error:iomnPrepareForPrint() in iomnPrnJobStart()\n");
            return(OK);
        }

        /* JOB開始指示ACK送信 */
#if 0   /* V010.00_02   */
/**//* D3.5 START T.Fujii 再開CJSTに対するAck送信先修正 */
/**/#if 0   /* OLD */
/**/        if(ERROR == (Status = iomnSendJobStartAck(ioMNCjst[hJobID].OldmsgQId,
/**/#else   /* D3.5 MID     NEW */
/**/        if(ERROR == (Status = iomnSendJobStartAck(ioMNMsgQId,
/**/#endif
/**//* D3.5 END */
/**/#if 0   /* MCK1.1 新メッセージ対応  引数追加    */
/**/                      hJobID, hStartTrigger, hContinuity, 0, 0, 0, OK)))
/**/#else
/**/                      hJobID, hStartTrigger, hContinuity, 0, 0, 0, OK, hProcessNo)))
/**/#endif
/**/        {
/**/            cmnPrintf_ioMN(2,">>>Error:iomnSendJobStartAck()\n");
/**/            return(ERROR);
/**/        }
/**/        iomnDebugMessage(">>>OK:Send Message Job Start ACK\n",0);
#endif  /* V010.00_02   */
        /*  JOB開始送信 */
        if(ERROR == (Status = iomnSendJobStart(hJobID, hStartTrigger,
                                hContinuity, hPrtType)))
        {
            cmnPrintf_ioMN(2,">>>Error:iomnSendJobStart() in iomnPrnJobStart()\n");
            return(ERROR);
        }
        iomnDebugMessage(">>>OK:Restart Printing->%x\n",iSuscause);
    }
    else if(hStartTrigger == IOMSG_IN_FORCE)
    { /*  強制スタートの場合  */
        /*  ＪＯＢ実行中の場合  */
        if(hJobstatus == COM_JS_RUNNING || hJobstatus == COM_JS_PRINTING){
            /*  印刷開始ＪＯＢのQUEING  */
            iomnSetStartQue(&ioMNPrnQue, hJobID, hStartTrigger,
                                     hContinuity, hPrtType, hJobtype);
            iomnDebugMessage(">>>OK:iomnSetStartQue()\n",0);
            return(OK);
        }
        /*  JOB実行中でない場合 */
        else{
            /*  紙サイズ不一致で中断している場合    */
            if(hJobstatus == COM_JS_SUSPEND
                 && iomnSuscauseForInForce(iSuscause, hJobtype) == TRUE );
            {
                /*  印刷準備処理    */
                if(ERROR == (Status = iomnPrepareForPrint(hJobID, hJobtype))){
                    cmnPrintf_ioMN(2,
                      ">>>Error:iomnPrepareForPrint() in iomnPrnJobStart()\n");
                    return(OK);
                }
#if 0   /* V010.00_02   */
/**/                /* JOB開始指示ACK送信 */
/**/                if(ERROR == (Status = iomnSendJobStartAck(
/**//* D3.5 START T.Fujii 再開CJSTに対するAck送信先修正 */
/**/#if 0   /* OLD */
/**/                                        ioMNCjst[hJobID].OldmsgQId,
/**/#else   /* D3.5 MID     NEW */
/**/                                        ioMNMsgQId,
/**/#endif
/**//* D3.5 END */
/**/                                        hJobID, hStartTrigger, hContinuity,
/**/#if 0   /* MCK1.1 新メッセージ対応  引数追加    */
/**/                                        0, 0, 0, OK)))
/**/#else
/**/                                        0, 0, 0, OK, hProcessNo)))
/**/#endif
/**/                {
/**/                    cmnPrintf_ioMN(2,">>>Error:iomnSendJobStartAck()\n");
/**/                    return(ERROR);
/**/                }
/**/                iomnDebugMessage(">>>OK:Send Message Job Start ACK\n",0);
#endif  /* V010.00_02   */

                /*  JOB開始指示送信 */
                if(ERROR == (Status = iomnSendJobStart(hJobID, hStartTrigger,
                                        hContinuity, hPrtType)))
                {
                    iomnDebugMessage(
                     ">>>Error:iomnSendJobStart() in iomnPrnJobStart()\n",0);
                    return(ERROR);
                }
                iomnDebugMessage(">>>OK:Restart Printing->%x\n",iSuscause);
            }
        }
    }
#if 0   /* V012.00_02   *//* OLD    */
/**/    else if(hStartTrigger == IOMSG_IN_DSS_RESTART)
#else   /* V012.00_02   *///* NEW   */
    /* 印刷ジョブに対する再開処理について対応   */
    else if(hStartTrigger == IOMSG_IN_DSS_RESTART ||
    (hStartTrigger == IOMSG_IN_MANUAL && hContinuity == IOMSG_JB_CONTINUE))
#endif
    { /*  ＣＳ中断の再開の場合  */
        iomnDebugMessage(
          ">>>>iomnPrnJobStart() Trigger = IOPMSG_IN_DSS_RESTART\n",0);
        /*  Ack Message Return  */
#if 0   /* V010.00_02   */
/**//* D3.5 START T.Fujii 再開CJSTに対するAck送信先修正 */
/**/#if 0   /* OLD */
/**/        if(ERROR == (Status = iomnSendJobStartAck(ioMNCjst[hJobID].OldmsgQId,
/**/#else   /* D3.5 MID     NEW */
/**/        if(ERROR == (Status = iomnSendJobStartAck(ioMNMsgQId,
/**/#endif
/**//* D3.5 END */
/**/                                hJobID, hStartTrigger, hContinuity,
/**/#if 0   /* MCK1.1 新メッセージ対応  引数追加    */
/**/                                0, 0, 0, OK)))
/**/#else
/**/                                0, 0, 0, OK, hProcessNo)))
/**/#endif
/**/        {
/**/            cmnPrintf_ioMN(2,">>>Error:iomnSendJobStartAck()\n");
/**/            return(ERROR);
/**/        }
#endif  /* V010.00_02   */
        iomnDebugMessage(">>>OK:Restart Printing->%x\n",iSuscause);
        /*  ＪＯＢ実行中の場合  */
        iomnDebugMessage(">>>>iomnPrnJobStart() hJobstatus = %d\n",hJobstatus);
        if(hJobstatus == COM_JS_SUSPEND
           || hJobstatus == COM_JS_RUNNING)
        {
            /*  印刷開始ＪＯＢの中断要因変更  */
            if(ERROR == (Status = jobSetStatus(hJobID,
                                    COM_JS_SUSPEND,IOMSG_SUS_ETC)))
            {
                cmnPrintf_ioMN(2,"jobSetStatus() error return\n");
                return(ERROR);
            }
            cmnPrintf_ioMN(0,">>>OK:jobSetStatus()  IOMSG_SUS_ETC\n");
            /*  印刷開始ＪＯＢのキューイング  */
            iomnSetStartQue(&ioMNPrnQue, hJobID, hStartTrigger,
                                        hContinuity, 0, hJobtype);
            iomnDebugMessage(">>>OK:iomnSetStartQue()\n",0);

            iomnDebugMessage("Restart Clear Table\n", 0 );
            iomnDebugMessage("Restart Jobid[%x]\n", hJobID );
#if 0   /* PRINT_JOINT */
/* PRINT_JOINT */            iomnDebugMessage("BGJobid[%x]\n", ioMNstatus.hBackJobId );
/* PRINT_JOINT */            /* 動作中印刷JOBの場合 */
/* PRINT_JOINT */            if ( ioMNstatus.hBackJobId == hJobID ){
#else
            if( ioMNPJ_BackIDCheck(hJobID) == OK ){
#endif
#if 0   /* PRINT_JOINT *//* 中断中断 */
/* PRINT_JOINT */                /* EntryIdの取得 */
/* PRINT_JOINT */                Status = iomnGetEntryIdFromJobtype( hJobtype, &hEntryId );
/* PRINT_JOINT */                if ( Status == ERROR ){
/* PRINT_JOINT */                    iomnDebugMessage(">>>Error:iomnGetEntryIdFromJobtype()\n",
/* PRINT_JOINT */                                                                         0);
/* PRINT_JOINT */                    return(ERROR);
/* PRINT_JOINT */                }
/* PRINT_JOINT */                /* バックグラウンドサスペンドテーブル初期化 */
/* PRINT_JOINT */                ioMNstatus.BackSuspend[hEntryId].hSusTask = 0;
/* PRINT_JOINT */                ioMNstatus.BackSuspend[hEntryId].hSusType = 0;
/* PRINT_JOINT */                iomnDebugMessage("hEntryId[%x]\n", hEntryId);
#else
		/*If this is job is running in Interrupt Mode then do not reset suspend Info*/
		/*Get the Previous Suspend Info*/
		{
			HI hSuspTsk, hSuspType;
			ioMNPJ_GetSusInfo(hJobID, &hSuspTsk, &hSuspType);
			#if 0 /*since interrupt mode is not known to DL ioMNstatus.hPPCIntReserve  falg is not required*/
			if( ioMNstatus.hPPCIntReserve != IOMN_ON || ioMNstatus.hSusDelay != IOMN_ON || hSuspType != IOMSG_SUS_INT){  /* VTK10.100 */
			#else
			if(hSuspType != IOMSG_SUS_INT || ioMNstatus.hSusDelay != IOMN_ON ){
				cmnPrintf_ioMN(0,"ioMNPJ_SetSusInfo is Called, SusTpye(%d), Delay(%d)\n", hSuspType, ioMNstatus.hSusDelay);
			#endif
			    ioMNPJ_SetSusInfo(hJobID, IOMN_OFF, IOMN_OFF);
			    /* ERRORは見ない */
			}                                                                               /* VTK10.100 */
                }                                                                               /* VTK10.100 */
#endif
            }
            /* V011.00_01   *//* START  */
            /* 未実行ジョブに対する再開は、ジョブ状態をREADYに戻す  */
            else{
                if(ERROR == (Status = jobSetStatus( hJobID, COM_JS_READY, 0 )))
                {
                    cmnPrintf_ioMN(2,"jobSetStatus() error return\n");
                    return(ERROR);
                }
                cmnPrintf_ioMN(0,">>>OK:jobSetStatus()  READY\n");
            }
            /* V011.00_01   *//* END    */
            return(OK);
        }
    }
    else{ /*  通常の印刷ＪＯＢ依頼    */
#if 0 /* 10911 fix - moved to RJRE handling*/
        /* 新規の場合のみ */
        /* 新規ジョブのときの準備処理 */

        /* 後追い逐次複写時、登録処理はここで実施       */
        /* 処理は複写ジョブ登録用の処理を行なう         */
        /* MCK1.5   *//* --- 後追い逐次 --- */
        if(ERROR == (Status = jobJobtype(hJobID, &hJobType2))){															/* SM-001 */
            cmnPrintf_ioMN(2,">>>Error:jobJobtype() in iomnPrnJobStart()\n");
            return(ERROR);
        }
        iomnDebugMessage("### JOB TYPE->%x\n",(I32)hJobType2);															/* SM-001 */

        /* 逐次以外の複写ジョブの印刷キュー登録はここで行なう   */
        /* V007.00_temp1    *//* 修正   */
        if((iomnCheckSCS(hJobID) != TRUE ) && ( hJobType2 == COM_JB_IPPC || hJobType2 == COM_JB_EXPPC2 )){				/* SM-001 */
#if 0   /* V008.t1_01   */
/**/#if 0   /* 暫定処理         *//* V006.03 */
/**/            if( iomnCpyMNEntryPrnJob( iomnSyncTblGetJobID( iomnJobInfo[hJobID].hIpTblID, COM_JB_SUB_INPUT )) == ERROR ){
/**/#else
/**/            if( iomnCpyMNEntryPrnJob( iomnSyncTblGetJobID( iomnJobInfo[1].hIpTblID, COM_JB_SUB_INPUT )) == ERROR ){
/**/#endif
#else
            /* Mckinleyの場合、印刷ジョブ登録はかならずしも、入力中とは限らない             */
            /* 印刷の開始はJCがとるので、ioMNは開始指示に従い、印刷ジョブの登録のみ行なう   */
            if( iomnCpyMNStorePrnJob(hJobID) == ERROR ){
#endif
                iomnDebugMessage(">> ioMN >> ERROR : Entry Copy Print!!\n",0);
/* V031.00_01 START */
                /*  ＪＯＢ完了メッセージ送信    */
                if(ERROR == (Status = iomnSendTjfn(hJobID, IOMSG_FIN_ERROR, IOMSG_JB_ENTRYERR, IOMSG_NOT_USED))){
                    iomnDebugMessage(">>>Error:iomnSendJobFinishSts()\n",0);
                    return(ERROR);
                }
                iomnCleanJobManagementMsg(hJobID);
                return(OK);
/* V031.00_01 END   */
            }
        }else{
        /* MCK1.5 END   */
            if( iomnPrepareForNewJob(hJobID, hJobtype, hContinuity) == ERROR ) {
                cmnPrintf_ioMN(2,  ">> ioMN >> ERROR:iomnPrepareForNewJob() in iomnPrnJobStart()\n");
                return( ERROR );
            }

            /* Dss PRIVATE PRINTの場合印刷の優先順位をあげる */
            if( iomnPrnJobCheckEntry(hJobID, hJobtype ) == ERROR ){
                if( hJobtype == COM_JB_LIST_RESULT_SEND ){
                    /* 送信Queの登録失敗時、ＪＯＢ完了メッセージ送信 */
                    /* 下記処理は送信JOBのみ                         */
                    iomnDebugMessage(">>>Error:iomnPrnJobCheckEntry()\n",0);

#if 0   /* MCK1.1 新メッセージ対応  */
/**/                    /*  messageID取得   */
/**/                    if(ioMNCjst[hJobID].iOldMsgId == IOMSG_CJST_UI){
/**/                        imsgid = IOMSG_TJFN_UI;
/**/                    }
/**/                    if(ioMNCjst[hJobID].iOldMsgId == IOMSG_CJST_JOB){
/**/                        imsgid = IOMSG_TJFN_JOB;
/**/                    }
/**/                    if(ioMNCjst[hJobID].iOldMsgId == IOMSG_CJST_DIG){
/**/                        imsgid = IOMSG_TJFN_DIG;
/**/                     }
#endif
                    /*  ＪＯＢ完了メッセージ送信    */
                    if(ERROR == (Status = iomnSendJobFinishSts(ioMNCjst[hJobID].OldmsgQId, imsgid,
#if 0   /* MCK1.1 新メッセージ対応  引数追加    */
                                                    hJobID, IOMSG_FIN_ERROR, IOMSG_ERR_JOB_START, IOMSG_NOT_USED))){
#else
                                                    hJobID, IOMSG_FIN_ERROR, IOMSG_ERR_JOB_START, IOMSG_NOT_USED, hProcessNo))){
#endif
                        iomnDebugMessage(">>>Error:iomnSendJobFinishSts()\n",0);
                        return(ERROR);
                        }
                    /*  ＪＯＢ管理メッセージバッファクリア  */
                    iomnCleanJobManagementMsg(hJobID);
                    iomnDebugMessage(">>>OK:iomnCleanJobManagementMsg()\n",0);
                    return(OK);
                }
                else{
                    cmnPrintf_ioMN(2, ">> ioMN >> ERROR:iomnPrnQueJobEntry() in iomnPrnJobStart()\n");
#if 0   /* V031.00_01 */
                    return( ERROR );
#else   /* V031.00_01 */
                    if( ioMNstatus.hDssDataTrans == IOMN_ON ) {
                        if((siIdx_no = jcLibJCB_tnoidx(ioMNCjst[hioMNDataRecvJobID].msgparam.sCjstCtl.hTNo)) == JCLIB_NOEXIST){
                            cmnPrintf_ioMN(2, ">> ioMN >> ERROR:jcLibJCB_tnoidx() in iomnSyncTblLinkOutputJob()\n" );
                        }else{
                            if((sJobTbl = (JOB_EXECDATA_TBL*) jcLibTBL_getp(JCLIB_EXC_TBL, siIdx_no)) == JCLIB_NOEXIST){
                                cmnPrintf_ioMN(2, ">> ioMN >> ERROR:jcLibTBL_getp() in iomnSyncTblLinkOutputJob()\n" );
                            }else{
                                if(sJobTbl->sCommon.hOutJobId != hJobID){
                                    /* 処理不要 */
                                }else{
								/*STFR_12963 FR changes: function defination changed*/
                                    iomnSyncTblClearJobID(iomnJobInfo[hioMNDataRecvJobID].hIpTblID, COM_JB_SUB_OUTPUT, hioMNDataRecvJobID);
                                }
                                jcLibUTY_memfree((void *) sJobTbl);
                            }
                        }
                    }
                    /*  ＪＯＢ完了メッセージ送信    */
                    if(ERROR == (Status = iomnSendTjfn(hJobID, IOMSG_FIN_ERROR, IOMSG_JB_ENTRYERR, IOMSG_NOT_USED))){
                        iomnDebugMessage(">>>Error:iomnSendJobFinishSts()\n",0);
                        return(ERROR);
                    }
                    iomnCleanJobManagementMsg(hJobID);
                    return(OK);
#endif  /* V031.00_01 */
                }
            }
        /* MCK1.5   *//* --- 後追い逐次 --- */
        }
        /* MCK1.5 END   */

#if 0   /* V010.00_02   */
/**/        /*  Ack Message Return  */
/**/#if 0   /* MCK1.1 新メッセージ対応  引数追加    */
/**//**/#if 0   /* MCK  *//* 暫定   */
/**//**/        if(ERROR == (Status = iomnSendJobStartAck(ioMNCjst[hJobID].OldmsgQId,
/**//**/                        hJobID, hStartTrigger, hContinuity, 0, 0, 0,OK)))
/**//**/        {
/**//**/            iomnDebugMessage(">>>Error:iomnSendJobStartAck()\n",0);
/**//**/            return(ERROR);
/**//**/        }
/**//**/#else
/**//**/        memset(&ioMNsmyparam, 0x00, sizeof(union ioMNparam));
/**//**/        ioMNsmyparam.sAjstCtl.hTNo       = hJobID;
/**//**/        ioMNsmyparam.sAjstCtl.hInUpDown  = hStartTrigger;
/**//**/        ioMNsmyparam.sAjstCtl.hOutTiming = hContinuity;
/**//**/        ioMNsmyparam.sAjstCtl.hWarSts    = 0;
/**//**/        ioMNsmyparam.sAjstCtl.iWarErrSys = 0;
/**//**/        ioMNsmyparam.sAjstCtl.iWarErrEng = 0;
/**//**/        ioMNsmyparam.sAjstCtl.iErrStatus = OK;
/**//**/        ioMNsmyparam.sAjstCtl.hProcessNo = ioMNCjst[hJobID].msgparam.sCjstCtl.hProcessNo;
/**//**/
/**//**/        if(ERROR == iomnSendMessage(ioMNCjst[hJobID].OldmsgQId,JOB_AJST,0,&ioMNsmyparam)){
/**//**/            iomnDebugMessage(">>>error:iomnSendMessage() in iomnPrnJobStart()\n",0);
/**//**/            return(ERROR);
/**//**/        }
/**//**/#endif
/**/#else
/**/        if(ERROR == (Status = iomnSendJobStartAck(ioMNCjst[hJobID].OldmsgQId,
/**/                        hJobID, hStartTrigger, hContinuity, 0, 0, 0, OK, hProcessNo)))
/**/        {
/**/            iomnDebugMessage(">>>Error:iomnSendJobStartAck()\n",0);
/**/            return(ERROR);
/**/        }
/**/#endif
/**/        iomnDebugMessage(">>>OK:iomnSendJobStartAck()\n",0);
#endif  /* V010.00_02   */

        /* D3.28.7 Start J.Ootani 電源ON処理を印刷Queチェック時に統一対応 */
        #if 0
/**/        /*  リスト夜間印刷の場合,電源ON処理実施 */
/**/        if(hJobtype == COM_JB_LIST_PRT_FAX){
/**/            if (ERROR == parMNNV13Param(931, &hfaxprt)){
/**/                cmnPrintf_ioMN(2,">>>Error:parMNNV13Param()\n");
/**/                printErrno(errno);
/**/                /*  初期値設定  */
/**/                hfaxprt = 0;
/**/            }
/**/            iomnDebugMessage("***** 13-931 = %d\n", hfaxprt) ;
/**/            /* 夜間印刷オフの場合はONしない */
/**/            if (1 == hfaxprt &&
/**/                IOMN_ON  == ioMNstatus.hWeeklytimer &&
/**/                IOMN_OFF == ioMNstatus.hWeeklymode){
/**/                iomnDebugMessage("*** 13-931 NO POWER ON\n", 0) ;
/**/            }
/**/            else
/**/            {
/**/                if(ioMNstatus.hWeeklysts == IOMN_OFF &&
/**/                   ioMNstatus.hTimerProccess == IOMN_UNDEF)
/**/                {
/**/                    /*  タイマ指示送信  */
/**/                    if(ERROR == (Status = iomnSendTimer(TRUE,
/**/                                            IOMN_PWON_BY_PRINT)))
/**/                    {
/**/                        iomnDebugMessage(">>>Error:iomnSendTimer()\n",0);
/**/                        return(ERROR);
/**/                    }
/**/                    iomnDebugMessage(">>>OK:iomnSendTimer()\n",0);
/**/                }
/**/                /*  OFF処理中の場合 */
/**/                else if(ioMNstatus.hWeeklysts == IOMN_ON &&
/**/                    ioMNstatus.hTimerProccess == IOMN_OFF){
/**/                    /*  タイマＯＮ処理予約フラグセット  */
/**/                    ioMNstatus.hReserveTimerON = IOMN_ON;
/**/                    ioMNstatus.hReserveTimerONtype = IOMN_PWON_BY_PRINT;
/**/                    iomnDebugMessage(">>>OK:Reserve Timer ON\n",0);
/**/                }
/**/            }
/**/        }
        #endif
        /* D3.28.7 End */

        /* D3.28.2 Start J.Ootani Sleep移行チェックはfb関数を使用するためhFaxConnectionを使用しない対応 */
        #if 0
        /*  FAXの場合   */
        if(hJobtype == COM_JB_PRT_FAX1
            || hJobtype == COM_JB_PRT_FAX2
            || hJobtype == COM_JB_POLL_RECV_FAX1
            || hJobtype == COM_JB_POLL_RECV_FAX2
            || hJobtype == COM_JB_DRC_PRT_FAX)
        {
            /*  受信中フラグが立っていればフラグ解除    */
            if(ioMNstatus.hFaxConnection == IOMN_ON){
                ioMNstatus.hFaxConnection = IOMN_OFF;
                iomnDebugMessage(">>>Clear Fax Connection Flag\n",0);
            }
        }
        #endif
        /* D3.28.2 End */

        /*  メモリFULL送信の場合    */
        if(ioMNstatus.memfull_sts == IOMN_ON && hJobtype == COM_JB_DRC_PRT_FAX){
            /*  Memory Full Print 受付可否確認  */
            Status = iomnMemFullAcceptConfirm(hJobID);

            /*  受付不可能な場合    */
            if(Status != OK ){
                /*  FAXJOB管理に対しJOB終了ステータス送信   */
                if(ERROR == (Status = iomnSendTJED(ioMNCjst[hJobID].OldmsgQId,
#if 0   /* MCK1.1 新メッセージ対応  */
/**/                                        IOMSG_TJED_JOB, hJobID, IOMSG_SUS_ERROR,
#else
                                        JOB_MSG_TJSX, hJobID, IOMSG_SUS_ERROR,
#endif
                                        Status, IOMSG_NOT_USED, 0)))
                {
                    iomnDebugMessage(">>>Error:iomnSendTJED()\n",0);
                    return(ERROR);
                }
                iomnDebugMessage(">>>Send TJED MESSAGE\n",0);
            }
            else{
                iomnDebugMessage(">>>OK:Fax Memory Full Print Acceptable\n",0);
            }
        }
#endif
        /* V015.00_08   *//* START  */
        /*  時間チェックなしで印刷ジョブの取得を行う        */
        if((iomnCheckSCS(hJobID) != TRUE ) && ( hJobType2 == COM_JB_IPPC || hJobType2 == COM_JB_EXPPC2 )){				/* SM-001 */
#if 0
            if( iomnCpyMNStorePrnJob(hJobID) == ERROR ){
                iomnDebugMessage(">> ioMN >> ERROR : Entry Copy Print!!\n",0);
                if(ERROR == (Status = iomnSendTjfn(hJobID, IOMSG_FIN_ERROR, IOMSG_JB_ENTRYERR, IOMSG_NOT_USED))){
                    iomnDebugMessage(">>>Error:iomnSendJobFinishSts()\n",0);
                    return(ERROR);
                }
                iomnCleanJobManagementMsg(hJobID);
                return(OK);
            }
#endif
        }else{
            if( iomnPrepareForNewJob(hJobID, hJobtype, hContinuity) == ERROR ) {
                cmnPrintf_ioMN(2,  ">> ioMN >> ERROR:iomnPrepareForNewJob() in iomnPrnJobStart()\n");
                return( ERROR );
            }
       	}
        if(ERROR == (Status = iomnCheckPrnTime( IOMN_OFF ))){
            return(ERROR);
        }
        iomnDebugMessage(">>> iomnCheckPrnTime() in iomnPrnJobStart()\n",0);
        /* V015.00_08   *//* END    */
    }
    return(OK);
}


/*
(C) Copyright TOSHIBA TEC Corporation 2000,2000. ALL Right Reserved
NO.
TITLE:      印刷ジョブＱｕｅ登録時のチェック処理
MODULE：    STATUS  iomnPrnJobCheckEntry(HI hJobid, HI hJobtype)
INCLUDE:
PARAMETER:
            HI      hJobid;             JOBID
            HI      hJobtype;           JOB種別
RETURNS:    I32 4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            印刷Ｑｕｅへ印刷ジョブを登録する時のチェックを行う。
NOTES:
HISTORY:
        D1.8    Y.Shimada       '00/ 3/ 9       DM45/35 Ｑｕｅ１本化
*/
STATUS iomnPrnJobCheckEntry(HI hJobid, HI hJobtype)
{
    HI hPriority;
    HI hEntryMode;

    /* 初期値は、優先度＝低、末尾に登録 */
    hPriority  = PRNQUE_PRI_LOW;
    hEntryMode = IOMN_PRNQUE_ENTRY_LAST;

    /* 割り込み複写、ﾌﾟﾗｲﾍﾞｰﾄDSS、ﾌﾟﾗｲﾍﾞｰﾄDSS(逐次) */
    if(( hJobtype == COM_JB_IPPC )||
       ( hJobtype == COM_JB_PRIVATE_DSI )||
       ( hJobtype == COM_JB_PRIVATE_DSI_SYNC )){
        /* 優先度＝高、先頭に登録 */
        hPriority  = PRNQUE_PRI_HIGH;
        /* 割り込みジョブのみ先頭、それ以外は末尾 */
        hEntryMode = ( hJobtype == COM_JB_IPPC )? IOMN_PRNQUE_ENTRY_FIRST : IOMN_PRNQUE_ENTRY_LAST ;
    }
    /* 印刷Ｑｕｅへの登録 */
    return( iomnPrnQueJobEntry( hJobid, hJobtype, hPriority, hEntryMode ));
}


/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         7.1.3.1
TITLE:      印刷開始条件Queing処理
MODULE：    STATUS  iomnPrepareForPrint(HI hJobid, HI hJobtype)
INCLUDE:    ioMNPar.h
            ioMNbuf.h
PARAMETER:
            HI      hJobid;             JOBID
            HI      hJobtype;           JOB種別
RETURNS:    I32 4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            印刷JOB開始前の準備を行う
NOTES:
            手差し印刷など印刷ＵＩから指示されるものについてのみ使用する。
HISTORY:
        1.1     Y.Matsuda   97/12/16
                for Ver 2.1 対応
    M1.1(TJ)H.Takahashi 98/08/12
        puJobEntry()をcallしないように修正
    D1.1    T.Fujii     99/11/15    DM45/35 後追い逐次・並行動作対応
    D2.1    T.Fujii     00/06/06    DM45/35 V004.60不具合修正(parMN再設定タイミング変更)
*/
STATUS  iomnPrepareForPrint(HI hJobid, HI hJobtype)
{

    iomnDebugMessage(">>>>Top of iomnPrepareForPrint()<<<<\n",0);

    return(OK);
}


/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         7.1.3.2
TITLE:      印刷開始条件Queing処理
MODULE：    STATUS  iomnSetStartQue(IOMN_START_QUE *pPrnQue, HI hJobID, HI hStartTrigger, HI hContinuity, 
                                                                                HI hPrtType, HI hJobtype)
INCLUDE:    ioMNPar.h
            ioMNbuf.h
PARAMETER:
            IOMN_START_QUE *pPrnQue;    格納するテーブルへのポインタ
            HI      hJobID;             JOBID   
            HI      hStartTrigger;      開始要因
            HI      hContinuity;        継続or新規  
            HI      hJobtype;           JOB種別
RETURNS:    I32 4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            印刷開始指示をQueingテーブルに登録する
NOTES:
HISTORY:
        1.1     Y.Matsuda   97/04/17
*/
STATUS  iomnSetStartQue(IOMN_START_QUE *pPrnQue, HI hJobID, HI hStartTrigger, HI hContinuity, HI hPrtType, HI hJobtype)
{
    /*  テーブル初期化  */
    memset(pPrnQue, 0x00, sizeof(struct start_que));

    /*  パラメータセット    */
    pPrnQue->hJobid = hJobID;
    pPrnQue->hTrigger = hStartTrigger;
    pPrnQue->hContinue = hContinuity;
    pPrnQue->hPrtType = hPrtType;
    pPrnQue->hJobtype = hJobtype;

    iomnDebugMessage(">>>Set Print Start Que<<<()\n",0);
    iomnDebugMessage("###hJobID -> %x\n",(I32)pPrnQue->hJobid);
    iomnDebugMessage("###hStartTrigger -> %x\n",(I32)pPrnQue->hTrigger);
    iomnDebugMessage("###hContinuity -> %x\n",(I32)pPrnQue->hContinue);
    iomnDebugMessage("###Print Type -> %x\n",(I32)pPrnQue->hPrtType);
    iomnDebugMessage("###hJobtype -> %x\n",(I32)pPrnQue->hJobtype);
    return(OK);
}


/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         7.1.3.2
TITLE:      印刷開始条件Queingテーブルから条件取得
MODULE：    STATUS  iomnGetStartQue(IOMN_START_QUE *pPrnQue, HI *pJobID, HI *pStartTrigger, HI *pContinuity, 
                                    HI &pPrtType, HI *pJobtype)
INCLUDE:    ioMNPar.h
            ioMNbuf.h
PARAMETER:
            IOMN_START_QUE *pPrnQue;    格納するテーブルへのポインタ
            HI      *pJobID;            JOBID   
            HI      *pStartTrigger;     開始要因
            HI      *pContinuity;       継続or新規  
            HI      *pPrtType;          印刷継続性  
            HI      *pJobtype;          JOB種別
RETURNS:    I32 4B
            OK:     正常
            ERROR:  テーブルに有効値がセットされていない
OUTLINE:
            印刷開始指示のをQueingテーブルから取得する
NOTES:
HISTORY:
        1.1     Y.Matsuda   97/04/17
*/
STATUS  iomnGetStartQue(IOMN_START_QUE *pPrnQue, HI *pJobID, HI *pStartTrigger, HI *pContinuity, 
                            HI *pPrtType, HI *pJobtype)
{
    /*  テーブルに値がセットされていない場合ERRORを戻す */
    if(pPrnQue->hJobid == 0
        && pPrnQue->hTrigger == 0
        && pPrnQue->hContinue == 0
        && pPrnQue->hPrtType == 0
        && pPrnQue->hJobtype == 0){
        iomnDebugMessage(">>>Print Start was not Quing\n",0);
        return(ERROR);
    }

    /*  パラメータセット    */
    *pJobID = pPrnQue->hJobid;
    *pStartTrigger = pPrnQue->hTrigger;
    *pContinuity = pPrnQue->hContinue;
    *pPrtType = pPrnQue->hPrtType;
    *pJobtype = pPrnQue->hJobtype;

    iomnDebugMessage(">>>Get Print Start Que<<<()\n",0);
    iomnDebugMessage("###hJobID -> %x\n",(I32)pPrnQue->hJobid);
    iomnDebugMessage("###hStartTrigger -> %x\n",(I32)pPrnQue->hTrigger);
    iomnDebugMessage("###hContinuity -> %x\n",(I32)pPrnQue->hContinue);
    iomnDebugMessage("###Print Type -> %x\n",(I32)pPrnQue->hPrtType);
    iomnDebugMessage("###hJobtype -> %x\n",(I32)pPrnQue->hJobtype);
    return(OK);
}


/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         7.1.3.3
TITLE:      印刷開始条件Queingテーブル初期化
MODULE：    STATUS  iomnCleanStartQue(IOMN_START_QUE *pPrnQue)
INCLUDE:    ioMNPar.h
            ioMNbuf.h
PARAMETER:
            IOMN_START_QUE *pPrnQue;    初期化するテーブルへのポインタ
RETURNS:    I32 4B
            OK:     正常
            ERROR:  テーブルに有効値がセットされていない
OUTLINE:
            印刷開始指示のをQueingテーブルから取得する
NOTES:
HISTORY:
        1.1     Y.Matsuda   97/04/17
*/
STATUS  iomnCleanStartQue(IOMN_START_QUE *pPrnQue)
{
    /*  テーブル初期化  */
    memset(pPrnQue, 0x00, sizeof(struct start_que));

    iomnDebugMessage(">>>Clear Print Start Que<<<()\n",0);
    iomnDebugMessage("###hJobID -> %x\n",(I32)pPrnQue->hJobid);
    iomnDebugMessage("###hStartTrigger -> %x\n",(I32)pPrnQue->hTrigger);
    iomnDebugMessage("###hContinuity -> %x\n",(I32)pPrnQue->hContinue);
    iomnDebugMessage("###hJobtype -> %x\n",(I32)pPrnQue->hJobtype);
    return(OK);
}


/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         7.1.5
TITLE:      Faxダイレクト送信 JOB開始処理
MODULE：    STATUS  iomnDirectFaxStart(HI hJobID, HI hStartTrigger, HI hContinuity, HI hJobtype)
INCLUDE:    ioMNPar.h
            ioMNbuf.h
PARAMETER:
            HI      hJobID;             JOBID   
            HI      hStartTrigger;      開始要因
            HI      hContinuity;        継続or新規  
            HI      hJobtype;           JOB種別
RETURNS:    I32 4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            FaxJOB管理タスクよりJOB開始指示を受けると、JOB状態をRUNNING状態に変更し、
            FaxJOB管理タスクに対してJOB開始受付Ackを送信する。
NOTES:
HISTORY:
        1.1     Y.Matsuda   97/04/24
        D1.7.13  J.Ootani    00/04/26    DM45/35 FAXダイレクト送信不具合対応対応
        D1.13    T.Fujii    00/05/17     DM45/35 AJSTパラメータ拡張対応
        D2.4.1   T.Fujii    00/06/17     DM45/35 V004.70不具合対応
                                                 AJSTパラメータ拡張対応修正
        D3.41    M.Fukuda   00/10/19     DM45/35 ダイレクト送信中止処理不具合対応

        MCK1.1  M.Karasawa  02/06/11    McKinley    新メッセージ対応
        MCK1.3  M.Karasawa  02/07/03    McKinley    Mckinley用に処理見なおし（ダイレクト送信）
*/
#if 0   /* MCK1.1 新メッセージ対応  引数追加    */
/**/STATUS  iomnDirectFaxStart(HI hJobID, HI hStartTrigger, HI hContinuity, HI hJobtype)
#else
STATUS  iomnDirectFaxStart(HI hJobID, HI hStartTrigger, HI hContinuity, HI hJobtype, HI hProcessNo)
#endif
{
    STATUS  Status;
#if 0   /* MCK1.3 ipテーブル確保できない場合等、ioMNにて受付エラーになる・・・  */
/**/    I32     imsgid=IOMSG_TJFN_UI;       /*  MessageID   */  /* D1.1 T.Fujii ダミー定義追加 */
/**/    I32     imsgidTjed=IOMSG_TJED_UI;   /*  ＪＯＢ終了メッセージＩＤ    */  /* D1.1 T.Fujii ダミー定義追加 */
#else
    I32     imsgid=JOB_MSG_TJFN;       /*  MessageID   */
#endif

    iomnDebugMessage(">>>Top of iomnDirectFaxStart()<<<\n",0);

#if 0   /* MCK1.3 ipテーブル確保できない場合等、ioMNにて受付エラーになる・・・  */
/**/    /*  messageID取得   */
/**/    if(ioMNCjst[hJobID].iOldMsgId == IOMSG_CJST_UI){
/**/        imsgid = IOMSG_TJFN_UI;
/**/        imsgidTjed = IOMSG_TJED_UI;
/**/    }
/**/    if(ioMNCjst[hJobID].iOldMsgId == IOMSG_CJST_JOB){
/**/        imsgid = IOMSG_TJFN_JOB;
/**/        imsgidTjed = IOMSG_TJED_JOB;
/**/    }
/**/    if(ioMNCjst[hJobID].iOldMsgId == IOMSG_CJST_DIG){
/**/        imsgid = IOMSG_TJFN_DIG;
/**/    }
#endif
#if 0   /* MCK1.3 START ダイレクト送信実行確認についてはJC層で行なうので削除    */
/**/    /*  ダイレクト送信開始可否確認処理  */
/**/    Status = iomnFaxScnStartConfirm(hJobID);
/**/
/**/    /*  JOB実行可否確認結果による処理   */
/**/    switch(Status){
/**/        case    IOMN_ON:
/**/                /*  ＪＯＢ開始予約フラグが設定されている場合は中断終了まで開始保留  */
/**/                if(ioMNstatus.hJobStart == IOMN_ON){
/**/                    iomnDebugMessage(">>>Wait... Back Job Suspending\n",0);
/**/
/**/                    /*  JOB中断後の再開情報格納 */
/**/                    iomnSetStartQue(&ioMNFrontQue, hJobID, hStartTrigger, hContinuity, 0, hJobtype);
/**/                    iomnDebugMessage(">>>OK:iomnSetStartQue()\n",0);
/**/                    ioMNstatus.hJobStart = IOMN_OFF;    
/**/                    return(OK);
/**/                }
/**/                else{
#endif

/* D3.41 START M.Fukuda ダイレクト送信中止処理不具合対応 */
    /*  JOB種別変更 */
    if(ERROR == (Status = jobFuncSelect(hJobID, hJobtype,
                                    COM_JB_FOREGROND, COM_JS_RUNNING)))
    {
        cmnPrintf_ioMN(2,">>>Error :jobFuncSelect() in iomnDirectFaxStart()\n");
        return(ERROR);
    }
/* D3.41 END */

#if 0   /* MCK1.3 START ダイレクト送信実行確認についてはJC層で行なうので削除    */
/**/                    iomnDebugMessage(">>>Fax Direct JOB Start\n",0);
/**/                }
/**/                break;
/**/
/**/        case    S_ioMN_ENGINE_ERROR:
/**/        case    S_ioMN_MACHINE_RECOVERY:
/**/        case    S_ioMN_TEST_STAPLE:
/**/                iomnDebugMessage(">>>Impossible to Job Start:%x\n",Status);
/**/                /*  新規開始の場合  */
/**/                if(hContinuity == IOMSG_JB_NEW){
/**/                    /*  TJFN Messageの登録  */
/**/                    ioMNTjfn[hJobID].iOldTaskId = iTaskIdFaxMN;
/**/                    ioMNTjfn[hJobID].iOldMsgId = IOMSG_TJFN_EX;
/**/                    ioMNTjfn[hJobID].OldmsgQId = msgQIdFaxMN;
/**/                    ioMNTjfn[hJobID].msgparam.sTjfnCtl.hJobid = hJobID;
/**/                    ioMNTjfn[hJobID].msgparam.sTjfnCtl.hSts = IOMSG_FIN_ERROR;
/**/                    ioMNTjfn[hJobID].msgparam.sTjfnCtl.iErrSys = IOMSG_NOT_USED;
/**/                    ioMNTjfn[hJobID].msgparam.sTjfnCtl.iErrEng = Status;
/**/                    iomnDebugMessage(">>>Set TJFN Message\n",0);
/**/
/**/                    /*  ＪＯＢ完了メッセージ送信    */
/**/                    if(ERROR == (Status = iomnSendJobFinishSts(ioMNCjst[hJobID].OldmsgQId, imsgid,
/**/                                                    hJobID, IOMSG_FIN_ERROR, IOMSG_NOT_USED, Status))){
/**/                        iomnDebugMessage(">>>Error:iomnSendJobFinishSts()\n",0);
/**/                        return(ERROR);
/**/                    }
/**/                    /*  ＪＯＢ管理メッセージバッファクリア  */
/**/                    iomnCleanJobManagementMsg(hJobID);
/**/                    iomnDebugMessage(">>>OK:iomnCleanJobManagementMsg()\n",0);
/**/                }
/**/                /*  再開の場合  */
/**/                else{
/**/                    /*  ＪＯＢ受付メッセージ情報格納    */
/**/                    ioMNAjst[hJobID].iOldTaskId = iTaskIdFaxMN;
/**/                    ioMNAjst[hJobID].iOldMsgId = IOMSG_AJST_JOB;
/**/                    ioMNAjst[hJobID].OldmsgQId = msgQIdFaxMN;
/**/                    ioMNAjst[hJobID].msgparam.sAjstCtl.hJobid = hJobID;
/**/                    ioMNAjst[hJobID].msgparam.sAjstCtl.hInUpDown = IOMSG_NOT_USED;
/**/                    ioMNAjst[hJobID].msgparam.sAjstCtl.hOutTiming = IOMSG_NOT_USED;
/**/                    ioMNAjst[hJobID].msgparam.sAjstCtl.iErrStatus = OK;
/**/                    iomnDebugMessage(">>>Set AJST Message\n",0);
/**/                    
/**/                    /*  FAXJOB管理に対してJOB受け付けメッセージを送信   */
/**/                    if(ERROR == (Status = iomnSendJobStartAck(ioMNCjst[hJobID].OldmsgQId, hJobID, 
/**/                                                   IOMSG_NOT_USED, IOMSG_NOT_USED, 0, 0, 0, OK))){
/**/                        iomnDebugMessage(">>>Error;iomnSendJobStartAck()\n",0);
/**/                        return(ERROR);
/**/                    }
/**/                    iomnDebugMessage(">>>OK:iomnSendJobStartAck()\n",0);
/**/
/**/                    /*  TJED Messageの登録  */
/**/                    ioMNTjed[hJobID].iOldTaskId = iTaskIdFaxMN;
/**/                    ioMNTjed[hJobID].iOldMsgId = IOMSG_TJED_EX;
/**/                    ioMNTjed[hJobID].OldmsgQId = msgQIdFaxMN;
/**/                    ioMNTjed[hJobID].msgparam.sTjedCtl.hJobid = hJobID;
/**/                    ioMNTjed[hJobID].msgparam.sTjedCtl.hSts = IOMSG_SUS_ERROR;
/**/                    ioMNTjed[hJobID].msgparam.sTjedCtl.iErrSys = IOMSG_NOT_USED;
/**/                    ioMNTjed[hJobID].msgparam.sTjedCtl.iErrEng = Status;
/**/                    ioMNTjed[hJobID].msgparam.sTjedCtl.hOrgBack = 0;
/**/                    iomnDebugMessage(">>>Set TJED Message\n",0);
/**/
/**/                    /*  ＪＯＢ終了メッセージ送信    */
/**/                    if(ERROR == (Status = iomnSendTJED(ioMNCjst[hJobID].OldmsgQId, imsgidTjed,
/**/                                            hJobID, IOMSG_SUS_ERROR, IOMSG_NOT_USED, Status, 0))){
/**/                        iomnDebugMessage(">>>Error:iomnSendTJED()\n",0);
/**/                        return(ERROR);
/**/                    }
/**/                    iomnDebugMessage(">>>Send TJED Message\n",0);
/**/                    
/**/                }
/**/                return(OK);
/**/
/**/        case    S_ioMN_DUAL_FUNCTION:
/**/        case    S_ioMN_MEMORY_FULL:
/**/                iomnDebugMessage(">>>Impossible to Job Start:%x\n",Status);
/**/
/**/                /*  新規開始の場合  */
/**/                if(hContinuity == IOMSG_JB_NEW){
/**/                    /*  TJFN Messageの登録  */
/**/                    ioMNTjfn[hJobID].iOldTaskId = iTaskIdFaxMN;
/**/                    ioMNTjfn[hJobID].iOldMsgId = IOMSG_TJFN_EX;
/**/                    ioMNTjfn[hJobID].OldmsgQId = msgQIdFaxMN;
/**/                    ioMNTjfn[hJobID].msgparam.sTjfnCtl.hJobid = hJobID;
/**/                    ioMNTjfn[hJobID].msgparam.sTjfnCtl.hSts = IOMSG_FIN_ERROR;
/**/                    ioMNTjfn[hJobID].msgparam.sTjfnCtl.iErrSys = Status;
/**/                    ioMNTjfn[hJobID].msgparam.sTjfnCtl.iErrEng = IOMSG_NOT_USED;
/**/                    iomnDebugMessage(">>>Set TJFN Message\n",0);
/**/
/**/                    /*  ＪＯＢ完了メッセージ送信    */
/**/                    if(ERROR == (Status = iomnSendJobFinishSts(ioMNCjst[hJobID].OldmsgQId, imsgid,
/**/                                            hJobID, IOMSG_FIN_ERROR, Status, IOMSG_NOT_USED))){
/**/                        iomnDebugMessage(">>>Error:iomnSendJobFinishSts()\n",0);
/**/                        return(ERROR);
/**/                    }
/**/                    /*  ＪＯＢ管理メッセージバッファクリア  */
/**/                    iomnCleanJobManagementMsg(hJobID);
/**/                    iomnDebugMessage(">>>OK:iomnCleanJobManagementMsg()\n",0);
/**/                }
/**/
/**/                /*  JOB再開の場合   */
/**/                else{
/**/                    /*  Ack Message登録 */
/**/                    ioMNAjst[hJobID].iOldTaskId = iTaskIdFaxMN;
/**/                    ioMNAjst[hJobID].iOldMsgId = IOMSG_AJST_JOB;
/**/                    ioMNAjst[hJobID].OldmsgQId = msgQIdFaxMN;
/**/                    ioMNAjst[hJobID].msgparam.sAjstCtl.hJobid = hJobID;
/**/                    ioMNAjst[hJobID].msgparam.sAjstCtl.hInUpDown = IOMSG_NOT_USED;
/**/                    ioMNAjst[hJobID].msgparam.sAjstCtl.hOutTiming = IOMSG_NOT_USED;
/**/                    ioMNAjst[hJobID].msgparam.sAjstCtl.iErrStatus = OK;
/**/                    iomnDebugMessage(">>>Set AJST Message\n",0);
/**/
/**/                    /*  TJED Messageの登録  */
/**/                    ioMNTjed[hJobID].iOldTaskId = iTaskIdFaxMN;
/**/                    ioMNTjed[hJobID].iOldMsgId = IOMSG_TJED_EX;
/**/                    ioMNTjed[hJobID].OldmsgQId = msgQIdFaxMN;
/**/                    ioMNTjed[hJobID].msgparam.sTjedCtl.hJobid = hJobID;
/**/                    ioMNTjed[hJobID].msgparam.sTjedCtl.hSts = IOMSG_SUS_ERROR;
/**/                    ioMNTjed[hJobID].msgparam.sTjedCtl.iErrSys = IOMSG_NOT_USED;
/**/                    ioMNTjed[hJobID].msgparam.sTjedCtl.iErrEng = Status;
/**/                    ioMNTjed[hJobID].msgparam.sTjedCtl.hOrgBack = 0;
/**/                    iomnDebugMessage(">>>Set TJED Message\n",0);
/**/
/**/                    /*  ＪＯＢ終了メッセージ送信    */
/**/                    if(ERROR == (Status = iomnSendTJED(ioMNCjst[hJobID].OldmsgQId, imsgidTjed,
/**/                                            hJobID, IOMSG_SUS_ERROR, Status, IOMSG_NOT_USED, 0))){
/**/                        iomnDebugMessage(">>>Error:iomnSendTJED()\n",0);
/**/                        return(ERROR);
/**/                    }
/**/                    iomnDebugMessage(">>>Send TJED Message\n",0);
/**/                }
/**/                return(OK);
/**/
/**/        case    ERROR:
/**/                iomnDebugMessage(">>>Error:iomnFaxScnStartConfirm()\n",0);
/**/
/**/                /*  TJFN Messageの登録  */
/**/                ioMNTjfn[hJobID].iOldTaskId = iTaskIdFaxMN;
/**/                ioMNTjfn[hJobID].iOldMsgId = JOB_MSG_TJFN;
/**/                ioMNTjfn[hJobID].OldmsgQId = msgQIdFaxMN;
/**/                ioMNTjfn[hJobID].msgparam.sTjfnCtl.hJobid = hJobID;
/**/                ioMNTjfn[hJobID].msgparam.sTjfnCtl.hSts = IOMSG_FIN_ERROR;
/**/                ioMNTjfn[hJobID].msgparam.sTjfnCtl.iErrSys = errno;
/**/                ioMNTjfn[hJobID].msgparam.sTjfnCtl.iErrEng = IOMSG_NOT_USED;
/**/                iomnDebugMessage(">>>Set TJFN Message\n",0);
/**/
/**/                /*  ＪＯＢ完了メッセージ送信    */
/**/                if(ERROR == (Status = iomnSendJobFinishSts(ioMNCjst[hJobID].OldmsgQId, imsgid,
/**/                                                hJobID, IOMSG_FIN_ERROR, errno, IOMSG_NOT_USED))){
/**/                    iomnDebugMessage(">>>Error:iomnSendJobFinishSts()\n",0);
/**/                    return(ERROR);
/**/                }
/**/                /*  ＪＯＢ管理メッセージバッファクリア  */
/**/                iomnCleanJobManagementMsg(hJobID);
/**/                iomnDebugMessage(">>>OK:iomnCleanJobManagementMsg()\n",0);
/**/                return(OK);
/**/        default:
/**/                cmnPrintf_ioMN(2,">>>ERROR:FaxScanJOB Confirmation Status->%x\n",Status);
/**/                break;
/**/    }
#endif  /* MCK1.3 END   */

    /*  CHANGE Bus From Pm to Fax   */
    if(ERROR == (Status = cmnChgBus(IMGBUS_FAX))){
        iomnDebugMessage(">>>Error:cmnChgBus()\n",0);
        return(ERROR);
    }
    iomnDebugMessage(">>>OK:cmnChgBus()\n",0);

#if 0   /* MCK1.3 START ダイレクト送信開始準備を行ない、開始指示を実行層に送信する  */
/**/    /*  ＪＯＢ受付メッセージ情報格納    */
/**/    ioMNAjst[hJobID].iOldTaskId = iTaskIdFaxMN;
/**/    ioMNAjst[hJobID].iOldMsgId = IOMSG_AJST_JOB;
/**/    ioMNAjst[hJobID].OldmsgQId = msgQIdFaxMN;
/**/    ioMNAjst[hJobID].msgparam.sAjstCtl.hJobid = hJobID;
/**/    ioMNAjst[hJobID].msgparam.sAjstCtl.hInUpDown = IOMSG_NOT_USED;
/**/    ioMNAjst[hJobID].msgparam.sAjstCtl.hOutTiming = IOMSG_NOT_USED;
/**/    ioMNAjst[hJobID].msgparam.sAjstCtl.iErrStatus = OK;
/**/    iomnDebugMessage(">>>Set AJST Message\n",0);
/**/
/**/    /*  FAXJOB管理に対してJOB受け付けメッセージを送信   */
/**/    if(ERROR == (Status = iomnSendJobStartAck(ioMNCjst[hJobID].OldmsgQId, hJobID, 
/**/                                               IOMSG_NOT_USED, IOMSG_NOT_USED, 0, 0, 0, OK))){
/**/        iomnDebugMessage(">>>Error;iomnSendJobStartAck()\n",0);
/**/        return(ERROR);
/**/    }
#else   /* MCK1.3 NEW iomnRecieveEC44()の処理を移植 */
    /* サブジョブ種別の設定  */
    if( parMNSetSubJobmode(hJobID, COM_JB_SUB_INPUT) == ERROR ) {
        cmnPrintf_ioMN(2, ">> ioMN >> ERROR: parMNSetSubJobmode()\n");
        return( ERROR );
    }
    iomnDebugMessage( ">>>OK:SubJobmode = SUB_INPUT(JobID=%d)\n", hJobID);

    /* ＩＯモードの設定  */
    if( parMNSetIOMode( hJobID , COM_OT_TRK_FIN ) == ERROR )
    {
        cmnPrintf_ioMN(2, ">> ioMN >> ERROR: parMNSetIOMode()\n");
        return( ERROR );
    }
    iomnDebugMessage( ">>>OK:IOMode = COM_OT_TRK_FIN(JobID=%d)\n", hJobID);

    /*  JOB開始準備処理 */
    if(ERROR == (Status = iomnPrepareForFax(hJobID, hStartTrigger,
                            hContinuity, hJobtype)))
    {
        iomnDebugMessage(">>>Error:iomnPrepareForFax()\n",0);
        /* ＪＯＢ完了メッセージ送信 */
        if(ERROR == (Status = iomnSendJobFinishSts(ioMNCjst[hJobID].OldmsgQId,
                                imsgid, hJobID, IOMSG_FIN_ERROR, errno,
                                IOMSG_NOT_USED,ioMNCjst[hJobID].msgparam.sCjstCtl.hProcessType)))
        {
            cmnPrintf_ioMN(2,">>>Error:iomnSendJobFinishSts()\n");
            return(ERROR);
        }
        return(OK);
    }

    /* JOB開始メッセージ送信 */
    if(ERROR == (Status = iomnSendJobStart(hJobID,hStartTrigger, 
                            hContinuity, 0)))
    {
        iomnDebugMessage(">>>Error:iomnSendJobStart()\n",0);
        /* FAXジョブ開始終了対応 */
        /* ＩＰテーブルと逐次制御テーブルの解放 */
        iomnStart_IP_SIS_Cls( hJobID );
        return(ERROR);
    }
#endif  /* MCK1.3 END   */
    iomnDebugMessage(">>>OK:iomnSendJobStartAck()\n",0);
    return(OK);
}


/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         7.1.6
TITLE:      DSS Scanner JOB開始処理
MODULE：    STATUS  iomnDssScnJobStart(HI hJobID, HI hStartTrigger, HI hContinuity, HI hJobtype)
INCLUDE:    ioMNPar.h
            ioMNbuf.h
PARAMETER:
            HI      hJobID;             JOBID   
            HI      hStartTrigger;      開始要因
            HI      hContinuity;        継続or新規  
            HI      hJobtype;           JOB種別
RETURNS:    I32 4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            DSS入力JOB開始指示を受け、JOB状態確認、JOB状態変更を行い、JOB開始メッセージを送信する。
NOTES:
HISTORY:
        1.1     Y.Matsuda   97/02/15

        MCK1.1  M.Karasawa 02/06/11 McKinley    新メッセージ対応
*/
#if 0   /* MCK1.1 新メッセージ対応  引数追加    */
/**/STATUS  iomnDssScnJobStart(HI hJobID, HI hStartTrigger, HI hContinuity, HI hJobtype)
#else
STATUS  iomnDssScnJobStart(HI hJobID, HI hStartTrigger, HI hContinuity, HI hJobtype, HI hProcessNo)
#endif
{
    STATUS  Status;         /*  各種関数戻り値  */

#if 0   /* MCK1.1 McKinley用メッセージID対応    */
/**/    I32     imsgid=IOMSG_TJFN_UI;   /*  JOB完了メッセージID */  /* D1.1 T.Fujii */
#endif
    iomnDebugMessage(">>>>Top of iomnDssScnJobStart()<<<<\n",0);

#if 0   /* MCK1.1 McKinley用メッセージID対応    */
/**/    /*  messageID取得   */
/**/    if(ioMNCjst[hJobID].iOldMsgId == IOMSG_CJST_UI) imsgid = IOMSG_TJFN_UI;
/**/    if(ioMNCjst[hJobID].iOldMsgId == IOMSG_CJST_JOB) imsgid = IOMSG_TJFN_JOB;
/**/    if(ioMNCjst[hJobID].iOldMsgId == IOMSG_CJST_DIG) imsgid = IOMSG_TJFN_DIG;
#endif

    iomnDebugMessage("### JOBID         --> %x\n",(I32)hJobID);
    iomnDebugMessage("### hStartTrigger --> %x\n",(I32)hStartTrigger);
    iomnDebugMessage("### hContinuity   --> %x\n",(I32)hContinuity);
    iomnDebugMessage("### hJobtype      --> %x\n",(I32)hJobtype);
    iomnDebugMessage("### hProcessType  --> %x\n",(I32)hProcessNo); /* MCK1.1   */

#if 0   /* MCK1.1 新メッセージ対応  *//* MckinleyではioMNでスキャン開始時の制限は不用 コード整理対象    */
/**/    /*  JOB実行可否確認 */
/**/    Status = iomnDssScnStartConfirm(hJobID);
/**/    /*  JOB実行可否確認結果による処理   */
/**/    switch(Status){
/**/        case    IOMN_ON:
/**/                /*  ＪＯＢ開始予約フラグが設定されている場合は中断終了まで開始保留  */
/**/                if(ioMNstatus.hJobStart == IOMN_ON){
/**/                    iomnDebugMessage(">>>Wait... Back Job Suspending\n",0);
/**/
/**/                    /*  JOB中断後の再開情報格納 */
/**/                    iomnSetStartQue(&ioMNFrontQue, hJobID, hStartTrigger, hContinuity, 0, hJobtype);
/**/                    iomnDebugMessage(">>>OK:iomnSetStartQue()\n",0);
/**/                    ioMNstatus.hJobStart = IOMN_OFF;    
/**/                    return(OK);
/**/                }
/**/                else{
/**/                    iomnDebugMessage(">>>Fax Scan JOB Start\n",0);
/**/                }
/**/                break;
/**/
/**/        case    S_ioMN_ENGINE_ERROR:
/**/        case    S_ioMN_MACHINE_RECOVERY:
/**/        case    S_ioMN_TEST_STAPLE:
/**/                iomnDebugMessage(">>>Impossible to Job Start:%x\n",Status);
/**/
/**/                /*  TJFN Messageの登録  */
/**/                ioMNTjfn[hJobID].iOldTaskId = iTaskIdIoExMN;
/**/                ioMNTjfn[hJobID].iOldMsgId = IOMSG_TJFN_EX;
/**/                ioMNTjfn[hJobID].OldmsgQId = msgQIdIoExMN;
/**/                ioMNTjfn[hJobID].msgparam.sTjfnCtl.hJobid = hJobID;
/**/                ioMNTjfn[hJobID].msgparam.sTjfnCtl.hSts = IOMSG_FIN_ERROR;
/**/                ioMNTjfn[hJobID].msgparam.sTjfnCtl.iErrSys = IOMSG_NOT_USED;
/**/                ioMNTjfn[hJobID].msgparam.sTjfnCtl.iErrEng = Status;
/**/                ioMNTjfn[hJobID].msgparam.sTjfnCtl.hProcessNo = hProcessNo;
/**/                iomnDebugMessage(">>>Set TJFN Message\n",0);
/**/
/**/                /*  ＪＯＢ完了メッセージ送信    */
/**/                if(ERROR == (Status = iomnSendJobFinishSts(ioMNCjst[hJobID].OldmsgQId, imsgid,
/**/                                                hJobID, IOMSG_FIN_ERROR, IOMSG_NOT_USED, Status))){
/**/                    iomnDebugMessage(">>>Error:iomnSendJobFinishSts()\n",0);
/**/                    return(ERROR);
/**/                }
/**/                return(OK);
/**/
/**/        case    S_ioMN_DUAL_FUNCTION:
/**/        case    S_ioMN_MEMORY_FULL:
/**/                iomnDebugMessage(">>>Impossible to Job Start:%x\n",Status);
/**/
/**/                /*  TJFN Messageの登録  */
/**/                ioMNTjfn[hJobID].iOldTaskId = iTaskIdIoExMN;
/**/                ioMNTjfn[hJobID].iOldMsgId = IOMSG_TJFN_EX;
/**/                ioMNTjfn[hJobID].OldmsgQId = msgQIdIoExMN;
/**/                ioMNTjfn[hJobID].msgparam.sTjfnCtl.hJobid = hJobID;
/**/                ioMNTjfn[hJobID].msgparam.sTjfnCtl.hSts = IOMSG_FIN_ERROR;
/**/                ioMNTjfn[hJobID].msgparam.sTjfnCtl.iErrSys = Status;
/**/                ioMNTjfn[hJobID].msgparam.sTjfnCtl.iErrEng = IOMSG_NOT_USED;
/**/                ioMNTjfn[hJobID].msgparam.sTjfnCtl.hProcessNo = hProcessNo;
/**/                iomnDebugMessage(">>>Set TJFN Message\n",0);
/**/
/**/                /*  ＪＯＢ完了メッセージ送信    */
/**/                if(ERROR == (Status = iomnSendJobFinishSts(ioMNCjst[hJobID].OldmsgQId, imsgid,
/**/                                            hJobID, IOMSG_FIN_ERROR, Status, IOMSG_NOT_USED))){
/**/                    iomnDebugMessage(">>>Error:iomnSendJobFinishSts()\n",0);
/**/                    return(ERROR);
/**/                }
/**/                return(OK);
/**/
/**/        case    ERROR:
/**/                iomnDebugMessage(">>>Error:iomnPPCStartCofirmation()\n",0);
/**/
/**/                /*  TJFN Messageの登録  */
/**/                ioMNTjfn[hJobID].iOldTaskId = iTaskIdIoExMN;
/**/                ioMNTjfn[hJobID].iOldMsgId = IOMSG_TJFN_EX;
/**/                ioMNTjfn[hJobID].OldmsgQId = msgQIdIoExMN;
/**/                ioMNTjfn[hJobID].msgparam.sTjfnCtl.hJobid = hJobID;
/**/                ioMNTjfn[hJobID].msgparam.sTjfnCtl.hSts = IOMSG_FIN_ERROR;
/**/                ioMNTjfn[hJobID].msgparam.sTjfnCtl.iErrSys = errno;
/**/                ioMNTjfn[hJobID].msgparam.sTjfnCtl.iErrEng = IOMSG_NOT_USED;
/**/                ioMNTjfn[hJobID].msgparam.sTjfnCtl.hProcessNo = hProcessNo;
/**/                iomnDebugMessage(">>>Set TJFN Message\n",0);
/**/
/**/                /*  ＪＯＢ完了メッセージ送信    */
/**/                if(ERROR == (Status = iomnSendJobFinishSts(ioMNCjst[hJobID].OldmsgQId, imsgid,
/**/                                                hJobID, IOMSG_FIN_ERROR, errno, IOMSG_NOT_USED))){
/**/                    iomnDebugMessage(">>>Error:iomnSendJobFinishSts()\n",0);
/**/                    return(ERROR);
/**/                }
/**/                return(OK);
/**/        default:
/**/                iomnDebugMessage(">>>ERROR:FaxScanJOB Confirmation Status->%x\n",Status);
/**/                break;
/**/    }
#endif
    /*  DssJOB開始準備処理  */
    if(ERROR == (Status = iomnPrepareForSendDSS(hJobID, hStartTrigger, hContinuity, hJobtype))){
        iomnDebugMessage(">>>Error:iomnPrepareForSendDSS()\n",0);
        return(ERROR);
    }

    /*  JOB開始メッセージ送信   */
    if(ERROR == (Status = iomnSendJobStart(hJobID, hStartTrigger, hContinuity, 0))){
        iomnDebugMessage(">>>Error:iomnSendJobStart()\n",0);
        return(ERROR);
    }

    iomnDebugMessage(">>>>Bottom of iomnDssScnJobStart()<<<<\n",0);
    return(OK);
}


/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         7.1.6.1
TITLE:      
MODULE：    STATUS iomnPrepareForSendDSS(HI hJobid, HI hStartTrigger, HI hContinuity, HI hJobtype)
INCLUDE:    ioMNpar.h
            ioMNstr.h
PARAMETER:
            HI      hJobid;             JOBID   
            HI      hStartTrigger;      開始要因
            HI      hContinuity;        継続or新規  
            HI      hJobtype;           JOB種別
RETURNS:    I32 4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            拡張FAX開始指示メッセージ受信時の開始準備処理。
NOTES:
HISTORY:
        1.1     Y.Matsuda   97/7/30
    M1.1(TJ)H.Takahashi 98/08/12
        DSSスキャン開始時にpuJobEntry()をcallしないように修正
    D1.14   T.Fujii     00/05/25 DM45/35 DSSスキャン対応(並行動作のためのサブジョブ化対応)
    D2.1    T.Fujii     00/06/06 DM45/35 V004.60不具合対応(parMNの原稿枚数をクリア)
*/
STATUS  iomnPrepareForSendDSS(HI hJobid, HI hStartTrigger, HI hContinuity, HI hJobtype)
{
    STATUS  Status;         /*  各処理ステータス    */

    iomnDebugMessage(">>>Top of iomnPrepareForSendDSS()<<<\n",0);

    if(ERROR == (Status = cmnChgBus(IMGBUS_PM))){
        cmnPrintf_ioMN(2,">>>Error:cmnChgBus()\n");
        printErrno(errno);
        return(ERROR);
    }
    iomnDebugMessage(">>>OK:cmnChgBus()\n",0);

    /* 新規の場合 */
    if( hContinuity == IOMSG_JB_NEW ) {
        /* サブジョブ種別(INPUT)設定 */
        if( parMNSetSubJobmode(hJobid,COM_JB_SUB_INPUT) == ERROR ) {
            iomnDebugMessage(">> ioMN >> ERROR:parMNSetSubJobmode()\n",0);
            iomnDebugMessage(">> ioMN >> BOTTOM:iomnPrepareForSendDSS() return ERROR\n",0);
            return(ERROR);
        }
        /* parMNの原稿枚数をクリア */
        if( parMNSetPages(hJobid, 0) == ERROR ) {
            cmnPrintf_ioMN(2,"[ioMN]ERROR:parMNSetPages(JobID=%d)\n",hJobid);
        }
        iomnDebugMessage(">> ioMN >> CHECK:Set subjob for DSS-Scan;COM_JB_SUB_INPUT\n",0);
    }

    iomnDebugMessage(">>>Bottom of iomnPrepareForSendDSS()<<<\n",0);
    return(OK);
}


/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         7.1.7
TITLE:      フォーム登録JOB開始処理
MODULE：    STATUS  iomnIVSaveJobStart(HI hJobID, HI hStartTrigger, HI hContinuity, HI hJobtype)
INCLUDE:    ioMNPar.h
            ioMNbuf.h
PARAMETER:
            HI      hJobID;             JOBID   
            HI      hStartTrigger;      開始要因
            HI      hContinuity;        継続or新規  
            HI      hJobtype;           JOB種別
RETURNS:    I32 4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            Fax及び複写の登録をＵＩより指示され、
NOTES:
            Fax、複写ともにUI側でファイル名を共通テーブルに設定する。
HISTORY:
        1.1     Y.Matsuda   97/02/15
        2.5     Y.Matsuda   98/01/10    for Ver 2.5
            印刷中断してフォーム登録開始予約処理追加
        D1.4    Y.Shimada   00/ 1/25
                フォームストレージ変更：ＩｐＬｉｂ／逐次制御の組み込み

        MCK1.1  M.Karasawa 02/06/11 McKinley    新メッセージ対応
    V018.00_01  M.Karasawa  03/04/09    McKinley    メッセージ変更対応
    V023.00_01  M.Karasawa  03/07/16    McKinley    エラー終了時TJFNに通し番号が入らない不具合の修正
*/
#if 0   /* MCK1.1 新メッセージ対応  引数追加    */
/**/STATUS  iomnIVSaveJobStart(HI hJobID, HI hStartTrigger, HI hContinuity, HI hJobtype)
#else
STATUS  iomnIVSaveJobStart(HI hJobID, HI hStartTrigger, HI hContinuity, HI hJobtype, HI hProcessNo)
#endif
{
    STATUS  Status;                 /*  各種処理結果    */
#if 0   /* MCK1.1 新メッセージ対応  */
/**/    I32     imsgid=IOMSG_TJFN_UI;   /* D1.1 T.Fujii ダミー定義追加 */
#else
    I32     imsgid=JOB_MSG_TJFN;
#endif

    iomnDebugMessage(">>>Top of iomnIVSaveJobStart()<<<\n",0);

#if 0   /* MCK1.1 新メッセージ対応  */
/**/    /*  messageID取得   */
/**/    if(ioMNCjst[hJobID].iOldMsgId == IOMSG_CJST_UI) imsgid = IOMSG_TJFN_UI;
/**/    if(ioMNCjst[hJobID].iOldMsgId == IOMSG_CJST_JOB) imsgid = IOMSG_TJFN_JOB;
/**/    if(ioMNCjst[hJobID].iOldMsgId == IOMSG_CJST_DIG) imsgid = IOMSG_TJFN_DIG;
#endif

    /*  JOB実行可否確認 */
    Status = iomnIVStartConfirm(hJobID);

    /*  JOB実行可否確認結果による処理   */
    switch(Status){
        case    IOMN_ON:
                if(ioMNstatus.hJobStart == IOMN_ON){
                    iomnDebugMessage(">>>Wait... Back Job Suspending\n",0);

                    /*  JOB中断後の再開情報格納 */
                    iomnSetStartQue(&ioMNFrontQue, hJobID, hStartTrigger, hContinuity, 0, hJobtype);
                    iomnDebugMessage(">>>OK:iomnSetStartQue()\n",0);
                    ioMNstatus.hJobStart = IOMN_OFF;    
                    return(OK);
                }else{
                    iomnDebugMessage(">>>Form JOB Start\n",0);
                }
                break;

        case    S_ioMN_ENGINE_ERROR:
        case    S_ioMN_MACHINE_RECOVERY:
        case    S_ioMN_TEST_STAPLE:
                iomnDebugMessage(">>>Impossible to Job Start:%x\n",Status);

                /*  TJFN Messageの登録  */
                ioMNTjfn[hJobID].iOldTaskId = iTaskIdIoExMN;
#if 0   /* MCK1.1 新メッセージ対応  */
/**/                ioMNTjfn[hJobID].iOldMsgId = IOMSG_TJFN_EX;
#else
                ioMNTjfn[hJobID].iOldMsgId = JOB_MSG_TJFN;
#endif
                ioMNTjfn[hJobID].OldmsgQId = msgQIdIoExMN;
                ioMNTjfn[hJobID].msgparam.sTjfnCtl.hJobid = hJobID;
                ioMNTjfn[hJobID].msgparam.sTjfnCtl.hSts = IOMSG_FIN_ERROR;
                ioMNTjfn[hJobID].msgparam.sTjfnCtl.iErrSys = IOMSG_NOT_USED;
                ioMNTjfn[hJobID].msgparam.sTjfnCtl.iErrEng = Status;
                /* MCK1.1 新メッセージ対応  */
                ioMNTjfn[hJobID].msgparam.sTjfnCtl.hProcessType = hProcessNo;
                /* V018.00_01 START */
                ioMNTjfn[hJobID].msgparam.sTjfnCtl.hJcbID     = ioMNCjst[hJobID].msgparam.sCjstCtl.hJcbID;
                ioMNTjfn[hJobID].msgparam.sTjfnCtl.hTNodel    = IOMSG_NOT_USED;
                ioMNTjfn[hJobID].msgparam.sTjfnCtl.hUkedel    = IOMSG_NOT_USED;
                ioMNTjfn[hJobID].msgparam.sTjfnCtl.hAppliType = ioMNCjst[hJobID].msgparam.sCjstCtl.hAppliType;
                /* V018.00_01 END   */
                /* V023.00_01 START */
                ioMNTjfn[hJobID].msgparam.sTjfnCtl.hTNo       = ioMNCjst[hJobID].msgparam.sCjstCtl.hTNo;
                /* V023.00_01 END   */
                iomnDebugMessage(">>>Set TJFN Message\n",0);
                /*  ＪＯＢ完了メッセージ送信    */
                if(ERROR == (Status = iomnSendJobFinishSts(ioMNCjst[hJobID].OldmsgQId, imsgid,
#if 0   /* MCK1.1 新メッセージ対応  引数追加    */
                                                hJobID, IOMSG_FIN_ERROR, IOMSG_NOT_USED, Status))){
#else
                                                hJobID, IOMSG_FIN_ERROR, IOMSG_NOT_USED, Status, hProcessNo))){
#endif
                    iomnDebugMessage(">>>Error:iomnSendJobFinishSts()\n",0);
                    return(ERROR);
                }
                return(OK);

        case    S_ioMN_DUAL_FUNCTION:
        case    S_ioMN_MEMORY_FULL:
                iomnDebugMessage(">>>Impossible to Job Start:%x\n",Status);

                /*  TJFN Messageの登録  */
                ioMNTjfn[hJobID].iOldTaskId = iTaskIdIoExMN;
#if 0   /* MCK1.1 新メッセージ対応  */
/**/                ioMNTjfn[hJobID].iOldMsgId = IOMSG_TJFN_EX;
#else
                ioMNTjfn[hJobID].iOldMsgId = JOB_MSG_TJFN;
#endif
                ioMNTjfn[hJobID].OldmsgQId = msgQIdIoExMN;
                ioMNTjfn[hJobID].msgparam.sTjfnCtl.hJobid = hJobID;
                ioMNTjfn[hJobID].msgparam.sTjfnCtl.hSts = IOMSG_FIN_ERROR;
                ioMNTjfn[hJobID].msgparam.sTjfnCtl.iErrSys = Status;
                ioMNTjfn[hJobID].msgparam.sTjfnCtl.iErrEng = IOMSG_NOT_USED;
                /* MCK1.1 新メッセージ対応  */
                ioMNTjfn[hJobID].msgparam.sTjfnCtl.hProcessType = hProcessNo;
                /* V018.00_01 START */
                ioMNTjfn[hJobID].msgparam.sTjfnCtl.hJcbID     = ioMNCjst[hJobID].msgparam.sCjstCtl.hJcbID;
                ioMNTjfn[hJobID].msgparam.sTjfnCtl.hTNodel    = IOMSG_NOT_USED;
                ioMNTjfn[hJobID].msgparam.sTjfnCtl.hUkedel    = IOMSG_NOT_USED;
                ioMNTjfn[hJobID].msgparam.sTjfnCtl.hAppliType = ioMNCjst[hJobID].msgparam.sCjstCtl.hAppliType;
                /* V018.00_01 END   */
                /* V023.00_01 START */
                ioMNTjfn[hJobID].msgparam.sTjfnCtl.hTNo       = ioMNCjst[hJobID].msgparam.sCjstCtl.hTNo;
                /* V023.00_01 END   */
                iomnDebugMessage(">>>Set TJFN Message\n",0);

                /*  ＪＯＢ完了メッセージ送信    */
                if(ERROR == (Status = iomnSendJobFinishSts(ioMNCjst[hJobID].OldmsgQId, imsgid,
#if 0   /* MCK1.1 新メッセージ対応  引数追加    */
                                            hJobID, IOMSG_FIN_ERROR, Status, IOMSG_NOT_USED))){
#else
                                            hJobID, IOMSG_FIN_ERROR, Status, IOMSG_NOT_USED, hProcessNo))){
#endif
                    iomnDebugMessage(">>>Error:iomnSendJobFinishSts()\n",0);
                    return(ERROR);
                }
                return(OK);

        case    ERROR:
                iomnDebugMessage(">>>Error:iomnPPCStartCofirmation()\n",0);

                /*  TJFN Messageの登録  */
                ioMNTjfn[hJobID].iOldTaskId = iTaskIdIoExMN;
#if 0   /* MCK1.1 新メッセージ対応  */
/**/                ioMNTjfn[hJobID].iOldMsgId = IOMSG_TJFN_EX;
#else
                ioMNTjfn[hJobID].iOldMsgId = JOB_MSG_TJFN;
#endif
                ioMNTjfn[hJobID].OldmsgQId = msgQIdIoExMN;
                ioMNTjfn[hJobID].msgparam.sTjfnCtl.hJobid = hJobID;
                ioMNTjfn[hJobID].msgparam.sTjfnCtl.hSts = IOMSG_FIN_ERROR;
                ioMNTjfn[hJobID].msgparam.sTjfnCtl.iErrSys = errno;
                ioMNTjfn[hJobID].msgparam.sTjfnCtl.iErrEng = IOMSG_NOT_USED;
                /* MCK1.1 新メッセージ対応  */
                ioMNTjfn[hJobID].msgparam.sTjfnCtl.hProcessType = hProcessNo;
                /* V018.00_01 START */
                ioMNTjfn[hJobID].msgparam.sTjfnCtl.hJcbID     = ioMNCjst[hJobID].msgparam.sCjstCtl.hJcbID;
                ioMNTjfn[hJobID].msgparam.sTjfnCtl.hTNodel    = IOMSG_NOT_USED;
                ioMNTjfn[hJobID].msgparam.sTjfnCtl.hUkedel    = IOMSG_NOT_USED;
                ioMNTjfn[hJobID].msgparam.sTjfnCtl.hAppliType = ioMNCjst[hJobID].msgparam.sCjstCtl.hAppliType;
                /* V018.00_01 END   */
                /* V023.00_01 START */
                ioMNTjfn[hJobID].msgparam.sTjfnCtl.hTNo       = ioMNCjst[hJobID].msgparam.sCjstCtl.hTNo;
                /* V023.00_01 END   */
                iomnDebugMessage(">>>Set TJFN Message\n",0);

                /*  ＪＯＢ完了メッセージ送信    */
                if(ERROR == (Status = iomnSendJobFinishSts(ioMNCjst[hJobID].OldmsgQId, imsgid,
#if 0   /* MCK1.1 新メッセージ対応  引数追加    */
                                                hJobID, IOMSG_FIN_ERROR, errno, IOMSG_NOT_USED))){
#else
                                                hJobID, IOMSG_FIN_ERROR, errno, IOMSG_NOT_USED, hProcessNo))){
#endif
                    iomnDebugMessage(">>>Error:iomnSendJobFinishSts()\n",0);
                    return(ERROR);
                }
                return(OK);

        default:
                iomnDebugMessage(">>>ERROR:FaxScanJOB Confirmation Status->%x\n",Status);
                break;
    }

    /*  フォーム登録JOB開始準備処理 */
    if(ERROR == (Status = iomnPrepareForIVSave(hJobID, hStartTrigger, hContinuity, hJobtype))){
        iomnDebugMessage(">>>Error:iomnPrepareForIVSave()\n",0);
        return(ERROR);
    }

    /*  JOB開始メッセージ送信   */
    if(ERROR == (Status = iomnSendJobStart(hJobID, hStartTrigger, hContinuity, 0))){
        iomnDebugMessage(">>>Error:iomnSendJobStart()\n",0);
        /* ＩＰテーブルと逐次制御テーブルの解放 */
        iomnStart_IP_SIS_Cls( hJobID );
        return(ERROR);
    }

    iomnDebugMessage(">>>OK:iomnSendJobStart()\n",0);
    iomnDebugMessage(">>>Bottom of iomnIVSaveJobStart()<<<\n",0);
    return(OK);
}


/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         7.1.7.1
TITLE:      
MODULE：    STATUS iomnPrepareForIVSave(HI hJobid, HI hStartTrigger, HI hContinuity, HI hJobtype)
INCLUDE:    ioMNpar.h
            ioMNstr.h
PARAMETER:
            HI      hJobid;             JOBID   
            HI      hStartTrigger;      開始要因
            HI      hContinuity;        継続or新規  
            HI      hJobtype;           JOB種別
RETURNS:    I32 4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            フォーム登録JOB開始指示メッセージ受信時の開始準備処理。
NOTES:
HISTORY:
        1.1     Y.Matsuda   97/7/30
        M1.1    (TJ)H.Takahashi 98/8/10
                初期化時にpuJobEntry()をcall
        D1.4    Y.Shimada  '00/ 1/25
                フォームストレージ変更：ＩｐＬｉｂ／逐次制御の組み込み
        D2.1    T.Fujii     00/06/06 DM45/35 V004.60不具合対応(parMNの原稿枚数をクリア)
*/
STATUS  iomnPrepareForIVSave(HI hJobid, HI hStartTrigger, HI hContinuity, HI hJobtype)
{
    STATUS  Status;
    B       aFileNamebuf[FILENAME];         /*  ファイル名  */
    HI      hImageID;                       /*  合成画像ID  */
    HI      hIpTblID;                       /*  ＩＰテーブルＩＤ */

    iomnDebugMessage(">>>Top of iomnPrepareForIVSave()<<<\n",0);

    /*  バッファ初期化  */
    memset(aFileNamebuf,0x00,FILENAME);

    /*  合成画像ID取得  */
    if(ERROR == (Status = parMNImageID(hJobid, &hImageID))){
        iomnDebugMessage(">>>Error:parMNImageID()\n",0);
        printErrno(errno);
        return(ERROR);
    }

    /*  ファイル名取得  */
    if(( Status = iomnFormFileNameChg( hJobtype , hImageID , aFileNamebuf)) == ERROR ){
        printErrno(errno);
        return(ERROR);
    }

    /*  新規JOB開始の場合はPMのJOB登録実施  */
    if(hContinuity == IOMSG_JB_NEW){
        /*  ページパラメータテーブルへのファイル名設定  */
        if(ERROR == (Status = jobSetPagePrmFile(hJobid, aFileNamebuf))){
            iomnDebugMessage(">>>Error:jobSetPagePrmFile()\n",0);
            return(ERROR);
        }
        iomnDebugMessage(">>>OK:jobSetPagePrmFile()\n",0);

        /* parMNの原稿枚数をクリア */
        if( parMNSetPages(hJobid, 0) == ERROR ) {
            cmnPrintf_ioMN(2,"[ioMN]ERROR:parMNSetPages(JobID=%d)\n",hJobid);
        }

        /* ipテーブルの取得とリンク情報の確立 */
        hIpTblID = ipLibEntryJob( hJobid );

        if( hIpTblID != ERROR ) {
            /* ipテーブルの確保ができた場合 */

            /* ジョブIDごとのジョブ情報エリアにipテーブルIDを保存 */
            iomnJobInfo[hJobid].hIpTblID = hIpTblID;

            /* 逐次制御テーブルに入力ジョブを登録 */
            if( iomnSyncTblEntryJob( hIpTblID, COM_JB_SUB_INPUT, hJobid ) != ERROR ) {

                /* 逐次制御テーブルをIPテーブル使用中に更新 */
                iomnSyncTblSetStatus( hIpTblID, COM_JB_SUB_INPUT, IOMN_IPTBL_USING );

                /* サブジョブ種別(INPUT)設定 */
                if( parMNSetSubJobmode( hJobid , COM_JB_SUB_INPUT ) == ERROR ){
                   iomnDebugMessage(">>ERROR:parMNSetSubJobmode()\n",0);
                   /* 上位に返る前に、ＩＰテーブル／逐次制御テーブルの解放を行う */
                   iomnStart_IP_SIS_Cls( hJobid );
                   return(ERROR);
                }

                /*入出力動作モード（後追い完了）の設定 */
               if( parMNSetIOMode( hJobid , COM_OT_TRK_FIN ) == ERROR ){
                   iomnDebugMessage( ">>>Error:parMNSetIOMode() in iomnPrepareForIVSave()\n", 0 );
                   /* 上位に返る前に、ＩＰテーブル／逐次制御テーブルの解放を行う */
                   iomnStart_IP_SIS_Cls( hJobid );
                   return( ERROR );
               }

            }else{
            /* エラー時は、ｉｐテーブル解放 */
                iomnDebugMessage(">>ERROR:iomnSyncTblEntryJob()\n",0);
                ipLibDeleteJob( hIpTblID );
                return(ERROR);
            }
        }else{
            /* ipテーブルの確保に失敗 */
            iomnDebugMessage(">>ERROR:ipLibEntryJob()\n",0);
            return(ERROR);
        }
    }

    iomnDebugMessage(">>>Bottom of iomnPrepareForIVSave()<<<\n",0);
    return(OK);
}


/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         7.1.8
TITLE:      フォーム確認JOB開始処理
MODULE：    STATUS  iomnIVPrnJobStart(HI hJobID, HI hStartTrigger, HI hContinuity, HI hJobtype)
INCLUDE:    ioMNPar.h
            ioMNbuf.h
PARAMETER:
            HI      hJobID;             JOBID   
            HI      hStartTrigger;      開始要因
            HI      hContinuity;        継続or新規  
            HI      hJobtype;           JOB種別
RETURNS:    I32 4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            UIよりフォーム確認JOB開始指示を受け、Faxまたは拡張複写の登録フォームの印刷を行う
NOTES:
            Fax、複写ともにUI側でファイル名を共通テーブルに設定する。
HISTORY:
        1.1     Y.Matsuda   97/02/15
        2.5     Y.Matsuda   98/01/10    for Ver 2.5
            印刷中断してフォーム登録開始予約処理追加
        D1.4    Y.Shimada  '00/ 1/25
                フォームストレージ変更：ＩｐＬｉｂ／逐次制御の組み込み
        D1.13   T.Fujii     00/05/17 DM45/35 AJSTパラメータ拡張対応
        D2.5    Y.Shimada   00/06/15 DM45/35 リスト印刷(フロントジョブ)対応(フォーム印刷と共通化)

        MCK1.1  M.Karasawa 02/06/11 McKinley    新メッセージ対応
    V018.00_01  M.Karasawa  03/04/09    McKinley    メッセージ変更対応
    V023.00_01  M.Karasawa  03/07/16    McKinley    エラー終了時TJFNに通し番号が入らない不具合の修正
*/
#if 0   /* MCK1.1 新メッセージ対応  引数追加    */
/**/STATUS  iomnIVPrnJobStart(HI hJobID, HI hStartTrigger, HI hContinuity, HI hJobtype)
#else
STATUS  iomnIVPrnJobStart(HI hJobID, HI hStartTrigger, HI hContinuity, HI hJobtype, HI hProcessNo)
#endif
{
    STATUS  Status;                 /*  各種処理結果    */
#if 0   /* MCK1.1 新メッセージ対応  */
/**/    I32     imsgid=IOMSG_TJFN_UI;   /*  MessageID   */  /* D1.1 T.Fujii ダミー定義追加 */
#else
    I32     imsgid=JOB_MSG_TJFN;
#endif
    iomnDebugMessage(">>>Top of iomnIVPrnJobStart()<<<\n",0);

#if 0   /* MCK1.1 新メッセージ対応  */
/**/    /*  messageID取得   */
/**/    if(ioMNCjst[hJobID].iOldMsgId == IOMSG_CJST_UI) imsgid = IOMSG_TJFN_UI;
/**/    if(ioMNCjst[hJobID].iOldMsgId == IOMSG_CJST_JOB) imsgid = IOMSG_TJFN_JOB;
/**/    if(ioMNCjst[hJobID].iOldMsgId == IOMSG_CJST_DIG) imsgid = IOMSG_TJFN_DIG;
#endif

    /*  フォーム確認JOB開始準備処理 */
    if(ERROR == (Status = iomnPrepareForIVPrn(hJobID, hStartTrigger, hContinuity, hJobtype))){
        iomnDebugMessage(">>>Error:iomnPrepareForIVPrn()\n",0);
        return(ERROR);
    }

    /* ジョブ開始受付(AJST)をｃｐｙＵＩに送信 */
    if(ERROR == (Status = iomnSendJobStartAck(ioMNCjst[hJobID].OldmsgQId, hJobID, hStartTrigger,
#if 0   /* MCK1.1 新メッセージ対応  引数追加    */
                                             hContinuity, 0, 0, 0,OK))){
#else
                                             hContinuity, 0, 0, 0,OK, hProcessNo))){
#endif
        iomnDebugMessage(">>>Error:iomnSendJobStartAck()\n",0);
        /* 上位に返る前に、ＩＰテーブル／逐次制御テーブルの解放を行う */
        iomnStart_IP_SIS_Cls( hJobID );
        return(ERROR);
    }

    /* 逐次制御テーブルとＩＰテーブルの解放 */
    iomnStart_IP_SIS_Cls( hJobID );

    /* ジョブ完了ステータス(TJFN)をＵＩに送信 */
    /*  TJFN Messageの登録  */
    ioMNTjfn[hJobID].iOldTaskId = iTaskIdIoExMN;
#if 0   /* MCK1.1 新メッセージ対応  */
/**/    ioMNTjfn[hJobID].iOldMsgId = IOMSG_TJFN_EX;
#else
    ioMNTjfn[hJobID].iOldMsgId = JOB_MSG_TJFN;
#endif
    ioMNTjfn[hJobID].OldmsgQId = msgQIdIoExMN;
    ioMNTjfn[hJobID].msgparam.sTjfnCtl.hJobid = hJobID;
    ioMNTjfn[hJobID].msgparam.sTjfnCtl.hSts = IOMSG_FIN_OK;
    ioMNTjfn[hJobID].msgparam.sTjfnCtl.iErrSys = IOMSG_NOT_USED;
    ioMNTjfn[hJobID].msgparam.sTjfnCtl.iErrEng = IOMSG_NOT_USED;
    /* MCK1.1 新メッセージ対応  */
    ioMNTjfn[hJobID].msgparam.sTjfnCtl.hProcessType = hProcessNo;
    /* V018.00_01 START */
    ioMNTjfn[hJobID].msgparam.sTjfnCtl.hJcbID     = ioMNCjst[hJobID].msgparam.sCjstCtl.hJcbID;
    ioMNTjfn[hJobID].msgparam.sTjfnCtl.hTNodel    = IOMSG_NOT_USED;
    ioMNTjfn[hJobID].msgparam.sTjfnCtl.hUkedel    = IOMSG_NOT_USED;
    ioMNTjfn[hJobID].msgparam.sTjfnCtl.hAppliType = ioMNCjst[hJobID].msgparam.sCjstCtl.hAppliType;
    /* V018.00_01 END   */
    /* V023.00_01 START */
    ioMNTjfn[hJobID].msgparam.sTjfnCtl.hTNo       = ioMNCjst[hJobID].msgparam.sCjstCtl.hTNo;
    /* V023.00_01 END   */
    iomnDebugMessage(">>>Set TJFN Message\n",0);

    /*  ＪＯＢ完了メッセージ送信    */
    if(ERROR == (Status = iomnSendJobFinishSts(ioMNCjst[hJobID].OldmsgQId, imsgid,
#if 0   /* MCK1.1 新メッセージ対応  引数追加    */
                          hJobID, IOMSG_FIN_OK, IOMSG_NOT_USED, IOMSG_NOT_USED))){
#else
                          hJobID, IOMSG_FIN_OK, IOMSG_NOT_USED, IOMSG_NOT_USED, hProcessNo))){
#endif
        iomnDebugMessage(">>>Error:iomnSendJobFinishSts()\n",0);
        return(ERROR);
    }

    iomnDebugMessage(">>>Bottom of iomnIVPrnJobStart()<<<\n",0);
    return(OK);
}


/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         7.1.8.1
TITLE:      
MODULE：    STATUS iomnPrepareForIVPrn(HI hJobid, HI hStartTrigger, HI hContinuity, HI hJobtype)
INCLUDE:    ioMNpar.h
            ioMNstr.h
PARAMETER:
            HI      hJobid;             JOBID   
            HI      hStartTrigger;      開始要因
            HI      hContinuity;        継続or新規  
            HI      hJobtype;           JOB種別
RETURNS:    I32 4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            フォーム確認JOB開始指示メッセージ受信時の開始準備処理。
NOTES:
HISTORY:
        1.1     Y.Matsuda   97/7/30
        M1.1    (TJ)H.Takahashi 98/8/10
                初期化時にpuJobEntry()をcall
        M1.2(TJ)H.Takahashi 98/11/24
                無駄処理削除
        D1.4    Y.Shimada   00/ 1/25
                フォームストレージ変更：ＩｐＬｉｂ／逐次制御の組み込み
        D1.2.1  T.Fujii     00/02/23    DM45/35 フィニッシャ初期化修正
        D2.1    T.Fujii     00/06/06    DM45/35 V004.60不具合対応
                                        フォーム印刷でHDD設定値の書き換えを削除
        D2.5    Y.Shimada   00/06/15    DM45/35 リスト印刷(フロントジョブ)対応(フォーム印刷と共通化)
        D2.5.2  Y.Shimada   00/07/10    DM45/35 リスト印刷(フロントジョブ)の複数ジョブ対応
*/
STATUS  iomnPrepareForIVPrn(HI hJobid, HI hStartTrigger, HI hContinuity, HI hJobtype)
{
    STATUS  Status;
    HI      hScanJobID;             /* 入力ジョブＩＤ       */
    HI      hPrintJobID;            /* 出力ジョブＩＤ       */
    B       aFileNamebuf[FILENAME]; /* フォームファイル名   */

    iomnDebugMessage(">>>Top of iomnPrepareForIVPrn()<<<\n",0);

    /*  新規JOB開始の場合はPMのJOB登録実施  */
    if(hContinuity == IOMSG_JB_NEW){
        /* ＩＰテーブル確保、逐次制御テーブル登録 */
        if(ERROR == (Status = iomnPrepareForPPC2( hJobid ))){
            cmnPrintf_ioMN(2,">>>Error :iomnPrepareForPPC2()\n");
            printErrno(errno);
            return(ERROR);
        }
    }

    /*  開始パラメータ設定  */
    if(ERROR == (Status = parMNSetPrnOrder(hJobid, COM_IO_DOWN))){
        iomnDebugMessage(">>>Error:parMNSetPageorder()\n",0);
        /* 上位に返る前に、ＩＰテーブル／逐次制御テーブルの解放を行う */
        iomnStart_IP_SIS_Cls( hJobid );
        return(ERROR);
    }
    iomnDebugMessage(">>>OK:parMNSetPrnOrder()\n",0);

    hScanJobID  = iomnSyncTblGetJobID( iomnJobInfo[hJobid].hIpTblID, COM_JB_SUB_INPUT  );
    hPrintJobID = iomnSyncTblGetJobID( iomnJobInfo[hJobid].hIpTblID, COM_JB_SUB_OUTPUT );

    /* コマンド送信元とCJSTパラメータの保存 */
    iomnStoreCmdSender( hPrintJobID );  /* コマンド送信元の保存 */
    iomnStoreCjstParam( hPrintJobID );  /* CJSTパラメータの保存 */

    /* リスト印刷、フォーム印刷の個別処理を下記の関数内にて分岐して処理させる */
    if( iomnPrepareUniqueProc( hScanJobID, hPrintJobID, hJobtype, hContinuity, aFileNamebuf) == ERROR ){
        iomnDebugMessage( "JobioMN:ERROR iomnPrepareUniqueProc() in iomnPrepareForIVPrn()\n", 0 );
        iomnStart_IP_SIS_Cls( hJobid );
        return( ERROR );
    }

    /*  新規JOB開始の場合はファイル名登録 */
    if(hContinuity == IOMSG_JB_NEW){
        /*  ページパラメータテーブルへのファイル名設定  */
        if(ERROR == (Status = jobSetPagePrmFile( hPrintJobID , aFileNamebuf))){
            iomnDebugMessage(">>>Error:jobSetPagePrmFile()\n",0);
            /* 上位に返る前に、ＩＰテーブル／逐次制御テーブルの解放を行う */
            iomnStart_IP_SIS_Cls( hJobid );
            return(ERROR);
        }
        iomnDebugMessage(">>>OK:jobSetPagePrmFile()\n",0);
        /* ジョブ開始指示(CJST)のパラメータ保存 */
        memcpy(&ioMNCjst[hPrintJobID], &ioMNCjst[hScanJobID], sizeof(IOMNMSG_STORE));
    }

    /*入出力動作モード（後追い完了）の設定 */
    if( parMNSetIOMode( hPrintJobID , COM_OT_TRK_FIN ) == ERROR ){
        iomnDebugMessage( ">>>Error:parMNSetIOMode() in iomnPrepareForIVPrn()\n", 0 );
        /* 上位に返る前に、ＩＰテーブル／逐次制御テーブルの解放を行う */
        iomnStart_IP_SIS_Cls( hJobid );
        return( ERROR );
    }

    /*  新規JOB開始の場合はparMNをHDDにセーブし、印刷Ｑｕｅ登録を行う */
    if(hContinuity == IOMSG_JB_NEW){
        /* 印刷Ｑｕｅへの登録 */
        if( iomnPrnJobCheckEntry(hPrintJobID, hJobtype ) == ERROR ){
            iomnDebugMessage( ">>>Error:iomnPrnQueJobEntry() in iomnPrepareForIVPrn()\n", 0 );
            /* 上位に返る前に、ＩＰテーブル／逐次制御テーブルの解放を行う */
            iomnStart_IP_SIS_Cls( hJobid );
            return( ERROR );
        }
    }

    iomnDebugMessage(">>>Bottom of iomnPrepareForIVPrn()<<<\n",0);
    return(OK);
}


/*
(C) Copyright TOSHIBA TEC Corporation 2000,2000. ALL Right Reserved
NO.         
TITLE:      
MODULE：    STATUS iomnPrepareUniqueProc(HI hScanJobID,HI hPrintJobID,HI hJobtype,HI hContinuity,B *aFileNamebuf)
INCLUDE:    ioMNpar.h
            ioMNstr.h
PARAMETER:
            HI      hScanJobID          フロントジョブＩＤ(I)
            HI      hPrintJobID         バックグランドジョブＩＤ(I)
            HI      hJobtype            ジョブ種別(I)
            HI      hContinuity         開始モード(新規／再開等)
            B       *aFileNamebuf       ファイル名(O)
RETURNS:    I32 4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            リスト印刷／フォーム印刷の固有準備処理を行う
            ：フォーム印刷
              ・parMN値(ﾌﾛﾝﾄｼﾞｮﾌﾞID)を出力用のparMN(ﾊﾞｯｸｸﾞﾗﾝﾄﾞｼﾞｮﾌﾞID)へ複写
              ・合成画像ID取得
              ・ファイル名取得
              ・サブジョブ種別の設定(OUTPUT)
            ：リスト印刷
              ・parMN値(ﾌﾛﾝﾄｼﾞｮﾌﾞID)を出力用のparMN(ﾊﾞｯｸｸﾞﾗﾝﾄﾞｼﾞｮﾌﾞID)へ複写
              ・ファイル名取得
              ・ファイルの作成
              ・ファイル属性の設定
              ・サブジョブ種別の設定(INPUT)
NOTES:
HISTORY:
        D2.5    Y.Shimada   00/06/15    DM45/35 リスト印刷(フロントジョブ)対応(フォーム印刷と共通化)
*/
STATUS iomnPrepareUniqueProc(HI hScanJobID,HI hPrintJobID,HI hJobtype,HI hContinuity,B *aFileNamebuf)
{
    HI hImageID;
    FL_FILE_COM_ATR sCom_Atr;
    FL_FILE_IN_ATR sIn_Atr;
    FL_FILE_OUT_ATR sOut_Atr;

    switch(hJobtype){
        case    COM_JB_IVPRM_PPC:               /* 複写フォーム印刷 */
            /*  新規JOB開始の場合は、入力ジョブのparMN設定値を、出力ジョブのparMNへ複写 */
            if(hContinuity == IOMSG_JB_NEW){
                if( parMNCopyTbl(hScanJobID, hPrintJobID) == ERROR ) {
                    iomnDebugMessage( "JobioMN:ERROR parMNCopyTbl() in iomnPrepareUniqueProc()\n", 0 );
                    printErrno(errno);
                    return( ERROR );
                }
            }
            /*  合成画像ID取得  */
            if( parMNImageID(hScanJobID, &hImageID) == ERROR ){
                iomnDebugMessage( "JobioMN:ERROR parMNImageID() in iomnPrepareUniqueProc()\n", 0 );
                printErrno(errno);
                return(ERROR);
            }
            /*  ファイル名取得  */
            if( iomnFormFileNameChg( hJobtype , hImageID , aFileNamebuf) == ERROR ){
                iomnDebugMessage( "JobioMN:ERROR iomnFormFileNameChg() in iomnPrepareUniqueProc()\n", 0 );
                printErrno(errno);
                return(ERROR);
            }
            /* 印刷用ジョブIDに対するサブジョブ種別を出力ジョブに設定 */
            if( parMNSetSubJobmode(hPrintJobID, COM_JB_SUB_OUTPUT) == ERROR ){
                iomnDebugMessage( "JobioMN:ERROR parMNSetSubJobmode() in iomnPrepareUniqueProc()\n", 0 );
                printErrno(errno);
                return( ERROR );
            }
            break;

        case    COM_JB_LIST_PRT_FAX:            /* リスト印刷(フロント指示) */
            /*  新規JOB開始の場合は、入力ジョブのparMN設定値を、出力ジョブのparMNへ複写 */
            if(hContinuity == IOMSG_JB_NEW){
                if( parMNSetLstPrnPar(hPrintJobID, hScanJobID) == ERROR ) {
                    iomnDebugMessage( "JobioMN:ERROR parMNSetLstPrnPar() in iomnPrepareUniqueProc()\n", 0 );
                    printErrno(errno);
                    return( ERROR );
                }
            }
            /* リストファイル名の取得 */
            if( iomnMakeListFileName(hPrintJobID, hJobtype, aFileNamebuf) == ERROR ){
                iomnDebugMessage( "JobioMN:ERROR iomnListFileNameMakee() in iomnPrepareUniqueProc()\n", 0 );
                return( ERROR );
            }
            /* リストファイルの作成 */
            if( flFileCreate( iTaskIdIoMN , 0 , aFileNamebuf ) == ERROR ){
                iomnDebugMessage("JobioMN:ERROR flFileCreate() in iomnPrepareUniqueProc()\n",0); 
                printErrno(errno);
                return(ERROR);
            }
            /* リストファイルの属性設定(初期値) */
            memset( &sCom_Atr, 0x00, sizeof(sCom_Atr) );
            memset( &sIn_Atr , 0x00, sizeof(sIn_Atr)  );
            memset( &sOut_Atr, 0x00, sizeof(sOut_Atr) );
            if( flFileAtrPut( iTaskIdIoMN , -1 , aFileNamebuf, &sCom_Atr, &sIn_Atr, &sOut_Atr ) == ERROR ){
                iomnDebugMessage("JobioMN:ERROR flFileAtrPut() in iomnPrepareUniqueProc()\n",0); 
                printErrno(errno);
                return(ERROR);
            }
            /* リスト作成ジョブに対するサブジョブ種別を入力ジョブに設定 */
            if( parMNSetSubJobmode(hPrintJobID, COM_JB_SUB_INPUT) == ERROR ){
                iomnDebugMessage( "JobioMN:ERROR parMNSetSubJobmode() in iomnPrepareUniqueProc()\n", 0 );
                printErrno(errno);
                return( ERROR );
            }
            break;

        default:
            cmnPrintf_ioMN(2,"JobioMN:ERROR JobType=%04xH in iomnPrepareUniqueProc()\n",hJobtype);
            return(ERROR);
    }

    return(OK);
}


/*
(C) Copyright TOSHIBA TEC Corporation 2000,2000. ALL Right Reserved
NO.         
TITLE:      
MODULE：    STATUS iomnMakeListFileName(HI hJobID,HI hJobType,B *aFileNamebuf)
INCLUDE:    flpar.h

PARAMETER:
            HI      hJobID              フロントジョブＩＤ(I)
            HI      hJobType            ジョブ種別(I)
            B       *aFileNamebuf       ファイル名(O)
RETURNS:    I32 4B
            OK:     正常
OUTLINE:
            リスト作成用のファイル名を作成する
NOTES:
HISTORY:
        D2.5    Y.Shimada   00/06/15    DM45/35
                                        リスト印刷(フロントジョブ)対応(フォーム印刷と共通化)
*/
STATUS iomnMakeListFileName(HI hJobID,HI hJobType,B *aFileNamebuf)
{
    I32 iPoz;
    B   aVolID;

    /* ボリューム名の組立 */
    iPoz = strlen(FL_ID_VOLUME_FLP) ;
    memcpy( aFileNamebuf, FL_ID_VOLUME_FLP, iPoz);
    *(aFileNamebuf+iPoz) = '/';
    iPoz++;

    /* ファイル名の先頭１文字の選定 */
    switch( hJobType ){
        case COM_JB_LIST_PRT_FAX:               /* リスト作成（印刷） */
        case COM_JB_LIST_RESULT_SEND:           /* リスト作成（送信） */
            aVolID = 'L';
            break;
        default:                                /* 上記以外 */
            iomnDebugMessage( "JobioMN:ERROR JobType=%04xH in iomnMakeListFileName()\n", (I32)hJobType );
            aVolID = '0';
            break;
    }

    /* ファイル名の組立 */
    sprintf(0, aFileNamebuf+iPoz , "%c%07d" , aVolID, hJobID );
    *(aFileNamebuf+iPoz+8) = 0x00;
    iomnDebugMessage( "JobioMN:OK iomnMakeListFileName(%s)\n", (I32)aFileNamebuf );

    return( OK );
}


/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         7.2
TITLE:      JOB開始受け付けAck受信処理
MODULE：    STATUS  iomnRecJobStartAck()

INCLUDE:    ioMNPar.h
PARAMETER:
RETURNS:    I32 4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            JOB開始指示に対する受け付けメッセージ受信時の処理。
NOTES:
HISTORY:
        1.1     Y.Matsuda   96/9/18
        M1.1(TJ)H.Takahashi 98/09/25
                DSS逐次対応
        D1.1    T.Fujii     99/11/15 DM45/35 後追い逐次機能組み込み
                                     サブジョブ種別により処理を分岐
        D1.2    T.Fujii     99/12/24 DM45/35 (後追い)逐次印刷対応
        D1.4    Y.Shimada   00/ 1/25 DM45/35 フォームストレージ変更：ＩｐＬｉｂ／逐次制御の組み込み
        D1.7.1  J.Ootani    00/03/01 DM45/35 SEINE FAX F-CODE対応(マージ)
        D1.7.13 J.Ootani    00/04/26 DM45/35 FAXダイレクト送信不具合対応対応
        D1.13   T.Fujii     00/05/17 DM45/35 AJSTパラメータ拡張対応
        D2.0.1  T.Fujii     00/06/06 DM45/35 NOP_DEL(ソースコード整理)
                                             case文でのCOM_JB_BPPC,COM_JB_EXPPC1を削除
        D2.4.1  T.Fujii     00/06/17 DM45/35 V004.70不具合対応
                                             AJSTパラメータ拡張対応修正
        D2.9    M.Fukuda    00/07/04 DM45/35 FAXリストレポート対応
        D3.19.3 T.Fujii     00/09/18 DM45/35 ADDS送信処理修正

        MCK1.1  M.Karasawa  02/06/11    McKinley    新メッセージ対応
        MCK1.3  M.Karasawa  02/07/03    McKinley    Mckinley用に処理見なおし（ダイレクト送信）
*/
STATUS  iomnRecJobStartAck()
{
    HI      hJobID;         /*  JOBID   */
    HI      hUpdown;        /*  入力順序    */
    HI      hOutTiming;     /*  印刷タイミング  */
    HI      hJobType;       /*  JOB種別 */
    STATUS  Status;         /*  各種処理結果    */
    HI      hWarSts;        /*  ワーニングステータス    */
    I32     iWarErrSys;     /*  アプリ系ワーニングERROR要因 */
    I32     iWarErrEng;     /*  エンジン系ワーニングError要因   */
    HI      hSubJobmode;
    I32     iErrStatus;  /* エラーステータス */

    HI          hTNo;       /* MCK1.1   */
    HI          hProcessNo; /* MCK1.1   */

#if 0   /* MCK1.1 新メッセージ対応  */
/**/    /*  受信パラメータ取得  */
/**/    hJobID = ioMNrmyparam.sAjstCtl.hJobid;
/**/    hUpdown = ioMNrmyparam.sAjstCtl.hInUpDown;
/**/    hOutTiming = ioMNrmyparam.sAjstCtl.hOutTiming;
/**/    hWarSts = ioMNrmyparam.sAjstCtl.hWarSts;
/**/    iWarErrSys = ioMNrmyparam.sAjstCtl.iWarErrSys;
/**/    iWarErrEng = ioMNrmyparam.sAjstCtl.iWarErrEng;
/**/    iErrStatus = ioMNrmyparam.sAjstCtl.iErrStatus;
/**/
/**/    iomnDebugMessage("### hJobID -->%x\n",(I32)hJobID);
/**/    iomnDebugMessage("### Start Type -->%x\n",(I32)hUpdown);
/**/    iomnDebugMessage("### OUT TIMING -->%x\n",(I32)hOutTiming);
/**/    iomnDebugMessage("### Warning Status -->%x\n",(I32)hWarSts);
/**/    iomnDebugMessage("### System Warning Type -->%x\n",(I32)iWarErrSys);
/**/    iomnDebugMessage("### Engine Warning Type -->%x\n",(I32)iWarErrEng);
/**/    iomnDebugMessage("### Error Status -->%x\n",(I32)iErrStatus);
#else
    hJobID      = ioMNrmyparam.sAjstCtl.hJobid;
    hUpdown     = ioMNrmyparam.sAjstCtl.hInUpDown;
    hOutTiming  = ioMNrmyparam.sAjstCtl.hOutTiming;
    hWarSts     = ioMNrmyparam.sAjstCtl.hWarSts;
    iWarErrSys  = ioMNrmyparam.sAjstCtl.iWarErrSys;
    iWarErrEng  = ioMNrmyparam.sAjstCtl.iWarErrEng;
    iErrStatus  = ioMNrmyparam.sAjstCtl.iErrStatus;
    hTNo        = ioMNrmyparam.sAjstCtl.hTNo;
    hProcessNo  = ioMNrmyparam.sAjstCtl.hProcessType;

	cmnPrintf_ioMN(7,"### hJobID->%x, InUpDown->%x, OutTiming->%x, WarSts->%x, WarErrSys->%x, WarErrEng->%x, ErrStatus->%x, \
						  TNo->%x, ProcessNo->%x", (I32)hJobID, (I32)hUpdown, (I32)hOutTiming,(I32)hWarSts, \
						  (I32)iWarErrSys, (I32)iWarErrEng, (I32)iErrStatus, (I32)hTNo, (I32)hProcessNo);
#endif
    /*  パラメータ格納  */
    iomnSetJobManagementMsg(&ioMNAjst[hJobID]);

    /*  JOBIDよりJOB種別取得    */
    if(ERROR == (Status = jobJobtype(hJobID, &hJobType))){
        cmnPrintf_ioMN(2,">>>Error:jobJobtype() in iomnRecJobStartAck()\n");
        return(ERROR);
    }
    iomnDebugMessage("### JOB TYPE->%x\n",(I32)hJobType);

    /* サブジョブ種別をここで取得 */
    if( parMNSubJobmode(hJobID, &hSubJobmode) == ERROR ) {
        iomnDebugMessage(">> ioMN >> Error:iomnRecJobStartAck()\n",0);
        return( ERROR );
    }

    /*  JOB種別によりメッセージ処理を変更   */
    switch(hJobType){
        case    COM_JB_IPPC:
        case    COM_JB_EXPPC2:
                /* サブジョブ種別をチェックして処理を分岐 */
                if( hSubJobmode == COM_JB_SUB_INPUT ) { /* サブジョブ種別が入力のとき */
                    if(ERROR == (Status = iomnPPCJobStartAck(hJobID, hUpdown, hOutTiming, 
#if 0   /* MCK1.1 新メッセージ対応  引数追加    */
                                                                hWarSts, iWarErrSys, iWarErrEng, iErrStatus))){
#else
                                                                hWarSts, iWarErrSys, iWarErrEng, iErrStatus, hProcessNo))){
#endif
                        iomnDebugMessage(">>>Error:iomnPPCJobStartAck()\n",0);
                        return(ERROR);
                    }
                }
                else {  /* サブジョブ種別が出力のとき */
                    if(ERROR == (Status = iomnPrnJobStartAck(hJobID, hUpdown, hOutTiming))){
                        iomnDebugMessage(">>>Error:iomnPrnJobStartAck()\n",0);
                        return(ERROR);
                    }
                }
                break;
        case    COM_JB_SCN_FAX1:
        case    COM_JB_SCN_FAX2:
        case    COM_JB_POLL_SEND_FAX1:
        case    COM_JB_POLL_SEND_FAX2:
        case    COM_JB_CON_REG_FAX2:
        case    COM_JB_BOD_REG_FAX2:
#if 0   /* MCK1.1 新メッセージ対応  引数追加    */
                if(ERROR == (Status = iomnFaxScnJobStartAck(hJobID, hUpdown, hOutTiming, iErrStatus))){
#else
                if(ERROR == (Status = iomnFaxScnJobStartAck(hJobID, hUpdown, hOutTiming, iErrStatus, hProcessNo))){
#endif
                    iomnDebugMessage(">>>Error:iomnFaxScnJobStartAck()\n",0);
                    return(ERROR);
                }
                break;
        case    COM_JB_DRC_SCN_FAX:
                /* MCK1.3 START ダイレクト送信対応          */
                /* JCに対してAJSTを送信する。               */
                /* ioMNとしては処理ないのでそのまま転送     */
                memcpy(&ioMNsmyparam, &ioMNrmyparam, sizeof(union ioMNparam));
                if(ERROR == (Status = iomnSendMessage(ioMNCjst[hJobID].OldmsgQId, JOB_MSG_AJST, 0, &ioMNsmyparam))){
                    iomnDebugMessage(">>>error:iomnSendMessage() in iomnSendMemoryResidue()\n",0);
                    return(ERROR);
                }
                /* MCK1.3 END                               */
                break;
/* D3.19.3 START T.Fujii ADDS送信処理修正 */
#if 0 /* OLD */
        case    COM_JB_PRT_FAX1:
        case    COM_JB_PRT_FAX2:
        case    COM_JB_POLL_RECV_FAX1:
        case    COM_JB_POLL_RECV_FAX2:
        case    COM_JB_DRC_PRT_FAX:
#endif
/* D3.19.3 END */
        case    COM_JB_PRIVATE_DSI:
        case    COM_JB_RECV_DSI:
        case    COM_JB_PRIVATE_DSI_SYNC:
        case    COM_JB_RECV_DSI_SYNC:
                /* サブジョブ種別をチェックして処理を分岐 */
                if( hSubJobmode == COM_JB_SUB_INPUT ) {     /* サブジョブ種別が入力のとき */
                    if( iomnDataRcvJobStartAck(hJobID) == ERROR ) {
                        iomnDebugMessage(">> ioMN >> Error:iomnDataRcvJobStartAck()\n",0);
                        return(ERROR);
                    }
                    /* D3.19.3 START T.Fujii ADDS送信処理修正 */
                    break;
                    /* D3.19.3 END */
                }
/* D3.19.3 START T.Fujii ADDS送信処理修正 */
#if 1   /* NEW */
                /* サブジョブ種別が出力のときは、そのまま下に */
        case    COM_JB_PRT_FAX1:
        case    COM_JB_PRT_FAX2:
        case    COM_JB_POLL_RECV_FAX1:
        case    COM_JB_POLL_RECV_FAX2:
        case    COM_JB_DRC_PRT_FAX:
        case    COM_JB_PRT_GDI1:
        case    COM_JB_PRT_GDI2:
        case    COM_JB_DRC_PRT_GDI:
                /* 出力ジョブの場合 */
                if( iomnPrnJobStartAck(hJobID, hUpdown, hOutTiming) == ERROR ) {
                    iomnDebugMessage(">> ioMN >> Error:iomnPrnJobStartAck()\n",0);
                    return(ERROR);
                }
                break;
#else   /* D3.19.3 MID  OLD */
                else {  /* サブジョブ種別が出力のとき */
                    if( iomnPrnJobStartAck(hJobID, hUpdown, hOutTiming) == ERROR ) {
                        iomnDebugMessage(">> ioMN >> Error:iomnPrnJobStartAck()\n",0);
                        return(ERROR);
                    }
                }
                break;
#endif
/* D3.19.3 END */
        case    COM_JB_LIST_SEND_FAX:
                    break;
        case    COM_JB_LIST_RESULT_SEND:
        case    COM_JB_LIST_PRT_FAX:
        case    COM_JB_CON_GET_FAX2:
        case    COM_JB_BOD_GET_FAX2:
                /*  フロント操作によるリスト印刷    */
                if(hJobID == hioMNFrontJobID){
#if 0   /* MCK1.1 新メッセージ対応  引数追加    */
                    if(ERROR == (Status = iomnIVPrnJobStartAck(hJobID, hUpdown, hOutTiming, iErrStatus))){
#else
                    if(ERROR == (Status = iomnIVPrnJobStartAck(hJobID, hUpdown, hOutTiming, iErrStatus, hProcessNo))){
#endif
                        iomnDebugMessage(">>>Error:iomnIVPrnJobStart()\n",0);
                        return(ERROR);
                    }
                }else{
                /*  バックグランドJOBによるリスト印刷   */
                    if(ERROR == (Status = iomnPrnJobStartAck(hJobID, hUpdown, hOutTiming))){
                        iomnDebugMessage(">>>Error:iomnPrnJobStartAck()\n",0);
                        return(ERROR);
                    }
                }
                break;
        case    COM_JB_IVSAVE_PPC:
        case    COM_JB_IVSAVE_FAX:
#if 0   /* MCK1.1 新メッセージ対応  引数追加    */
                if(ERROR == (Status = iomnIVSaveJobStartAck(hJobID, hUpdown, hOutTiming, iErrStatus))){
#else
                if(ERROR == (Status = iomnIVSaveJobStartAck(hJobID, hUpdown, hOutTiming, iErrStatus, hProcessNo))){
#endif
                    iomnDebugMessage(">>>Error:iomnIVSaveJobStart()\n",0);
                    return(ERROR);
                }
                break;
        case    COM_JB_IVPRM_PPC:
                if(ERROR == (Status = iomnPrnJobStartAck(hJobID, hUpdown, hOutTiming))){
                    iomnDebugMessage(">>>Error:iomnPrnJobStartAck()\n",0);
                    return(ERROR);
                }
                break;
        case    COM_JB_IVPRM_FAX:
#if 0   /* MCK1.1 新メッセージ対応  引数追加    */
                if(ERROR == (Status = iomnIVPrnJobStartAck(hJobID, hUpdown, hOutTiming, iErrStatus))){
#else
                if(ERROR == (Status = iomnIVPrnJobStartAck(hJobID, hUpdown, hOutTiming, iErrStatus, hProcessNo))){
#endif
                    iomnDebugMessage(">>>Error:iomnIVPrnJobStart()\n",0);
                    return(ERROR);
                }
                break;
        case    COM_JB_SEND_DSI:
#if 0   /* MCK1.1 新メッセージ対応  引数追加    */
                if(ERROR == (Status = iomnDssScnJobStartAck(hJobID, hUpdown, hOutTiming, iErrStatus))){
#else
                if(ERROR == (Status = iomnDssScnJobStartAck(hJobID, hUpdown, hOutTiming, iErrStatus, hProcessNo))){
#endif
                    iomnDebugMessage(">>>Error:iomnDSSJobStartAck()\n",0);
                    return(ERROR);
                }
                break;
        case    COM_JB_UNDEF:
        case    COM_JB_JOB_UI:
        case    COM_JB_RECV_FAX:
        case    COM_JB_DIG:
        default:
                iomnDebugMessage(">>>Error:Undefined JOB TYPE-->%x\n",(I32)hJobType);
                break;
    }

    /*  JOB中断中止予約時処理   */
    if(ERROR == (Status = iomnSusCanReservation(hJobID))){
        iomnDebugMessage(">>>Error:iomnSusCanReservation()\n",0);
        return(ERROR);
    }
    return(OK);
}


/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         7.2.1
TITLE:      複写JOB開始受け付け処理
MODULE：    STATUS  iomnPPCJobStartAck(HI hJobID, HI hUpdown, HI hOutTiming, HI hWarSts, I32 iWarErrSys, I32 iWarErrEng  I32 iErrStatus)
INCLUDE:    ioMNPar.h
            ioMNmsg.h
PARAMETER:  HI  hJobID      JOBID
            HI  hUpdown     印刷順
            HI  hOutTiming  印刷タイミング（逐次、完了）
            HI  hWarSts;    警告ステータス
            I32 iWarErrSys; システム系ワーニング
            I32 iWarErrEng; エンジン系ワーニング
            I32 iErrStatus; エラーステータス
RETURNS:    I32 4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            複写JOB開始受け付け処理。
NOTES:
HISTORY:
        1.1     Y.Matsuda   96/9/18
        D1.1    T.Fujii     99/11/15 DM45/35 後追い逐次対応
        D1.3    T.Fujii     00/01/17 DM45/35 従来逐次対応
        D1.2.1  T.Fujii     00/02/23 DM45/35 サブジョブ中断処理修正
        D1.2.4  T.Fujii     00/04/03 DM45/35 サブジョブ中断・再開処理修正
        D1.13   T.Fujii     00/05/17 DM45/35 AJSTパラメータ拡張対応
        D3.12   T.Fujii     00/09/02 DM45/35 ジョブ中断処理修正
        D3.12.1 T.Fujii     00/09/04 DM45/35 BG印刷ジョブ中断処理修正

        MCK1.1  M.Karasawa  02/06/11    McKinley    新メッセージ対応
        MCK1.5  M.Karasawa  02/07/25    McKinley    後追い逐次複写対応
    V011.00_05  M.Karasawa  03/01/14    McKinley    複写印刷登録処理修正（完了型対応）
    V016.00_04  M.Karasawa  03/03/31    McKinley    複写ジョブ管理エリアに関する処理を前面削除
*/
#if 0   /* MCK1.1 新メッセージ対応  引数追加    */
/**/STATUS  iomnPPCJobStartAck(HI hJobID, HI hUpdown, HI hOutTiming, HI hWarSts, I32 iWarErrSys, I32 iWarErrEng, I32 iErrStatus)
#else
STATUS  iomnPPCJobStartAck(HI hJobID, HI hUpdown, HI hOutTiming, HI hWarSts, I32 iWarErrSys, I32 iWarErrEng, I32 iErrStatus, HI hProcessNo)
#endif
{
    STATUS  Status;     /*  各種処理結果    */
    #ifdef DEBUG
    HI      hJobstatus;
    I32     iSuscause;
    #endif

    /* D3.12 START T.Fujii ジョブ中断処理修正 */
/* D3.12.1 START T.Fujii BG印刷ジョブ中断処理修正 */
#if 0   /* OLD */
/**/    if( ioMNstatus.hBGPrnSuspend == IOMN_ON ) {
/**/        ioMNstatus.hBGPrnSuspend = IOMN_OFF;
/**/    }
#else   /* D3.12.1 MID  NEW */
    /* ジョブ中断解除 */
    iomnSyncTblSetJobSuspend( iomnJobInfo[hJobID].hIpTblID, IOMN_OFF );
#endif
/* D3.12.1 END */
    /* D3.12 END */

    /* 印刷ジョブの登録 */
    /* MCK1.5   *//* --- 後追い逐次 --- */
    /* 逐次のみここでの登録を行なう。               */
    /* 後追い逐次は印刷開始CJST受信時に登録する     */
    if(iomnCheckSCS(hJobID) == TRUE
    /* V016.00_04   START   */
    /* ジョブ登録は新規のときのみ   */
    && (ioMNCjst[hJobID].msgparam.sCjstCtl.hContinue == IOMSG_JB_NEW 
    || ioMNCjst[hJobID].msgparam.sCjstCtl.hContinue == IOMSG_JB_NEW_TRK) /* ありえないけど一応・・・    */
    /* V016.00_04    END     */
    ){
    /* MCK1.5 END   */
        if( iomnCpyMNEntryPrnJob(hJobID) == ERROR ) {
            iomnDebugMessage(">>>Fail to entry print job for PPC\n",0);
            /* 登録に失敗した場合、入力処理をキャンセルする */
            /* 入出力実行管理に対してJOB中止メッセージ送信 */
            if( iomnSendJobCancel(hJobID, msgQIdIoExMN, IOMSG_CAN_SYSERR) == ERROR ) {
                iomnDebugMessage(">> ioMN >> ERROR:iomnSendJobCancel() in iomnPPCJobStartAck()\n",0);
                return(ERROR);
            }
            iomnDebugMessage(">> ioMN >> OK:Send CJCN ---> tioExMN",0);
            return( ERROR );
        }
    /* MCK1.5   *//* --- 後追い逐次 --- */
    }
    /* MCK1.5 END   */
#if 0   /* V016.00_04   START   */
/**//* V011.00_05   *//* START  */
/**/    else{
/**/        iomnCpyMNSetPrnJobID(iomnSyncTblGetJobID( iomnJobInfo[hJobID].hIpTblID, COM_JB_SUB_OUTPUT ));
/**/    }
/**//* V011.00_05   *//* END    */
#endif  /* V016.00_04   END     */
    /* UIに対してJOB受け付けメッセージを送信   */
    if(ERROR == (Status = iomnSendJobStartAck(ioMNCjst[hJobID].OldmsgQId, hJobID, hUpdown, hOutTiming, 
#if 0   /* MCK1.1 新メッセージ対応  引数追加    */
                                                                hWarSts, iWarErrSys, iWarErrEng, iErrStatus))){
#else
                                                                hWarSts, iWarErrSys, iWarErrEng, iErrStatus, hProcessNo))){
#endif
        iomnDebugMessage(">>>Error;iomnSendJobStartAck()\n",0);
        return(ERROR);
    }
    iomnDebugMessage(">>>Send AJST Message\n",0);

    #ifdef  DEBUG
        jobStatus(hJobID, &hJobstatus, &iSuscause);
        iomnDebugMessage("*** JOB STATUS -->%x\n",(I32)hJobstatus);
    #endif

    return(OK);
}

/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         7.2.4
TITLE:      FAX入力JOB開始受け付け受信処理
MODULE：    STATUS  iomnFaxScnJobStartAck(HI hJobID, HI hUpdown, HI hOutTiming, I32 iErrStatus)

INCLUDE:    ioMNPar.h
            ioMNstr.H
PARAMETER:
            HI  hJoibID;
            HI  hUpdown;
            HI  hOutTiming
            I32 iErrStatus
RETURNS:    I32 4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            FaxJOB開始受け付けAckを受信し、JOB開始指示送信元タスクに対してAck
            メッセージを送信する。
NOTES:
HISTORY:
        1.1     Y.Matsuda   96/9/18
        D1.13   T.Fujii     00/05/17 DM45/35 AJSTパラメータ拡張対応
        D2.4.1  T.Fujii     00/06/17 DM45/35 V004.70不具合対応
                                             AJSTパラメータ拡張対応修正

        MCK1.1  M.Karasawa 02/06/11 McKinley    新メッセージ対応
*/
#if 0   /* MCK1.1 新メッセージ対応  引数追加    */
/**/STATUS  iomnFaxScnJobStartAck(HI hJobID, HI hUpdown, HI hOutTiming, I32 iErrStatus)
#else
STATUS  iomnFaxScnJobStartAck(HI hJobID, HI hUpdown, HI hOutTiming, I32 iErrStatus, HI hProcessNo)
#endif
{
    STATUS  Status;
    #ifdef DEBUG
    HI      hJobstatus;
    I32     iSuscause;
    #endif

    /*  FAXJOB管理に対してJOB受け付けメッセージを送信   */
    if(ERROR == (Status = iomnSendJobStartAck(ioMNCjst[hJobID].OldmsgQId,
#if 0   /* MCK1.1 新メッセージ対応  引数追加    */
                                               hJobID, hUpdown, hOutTiming, 0, 0, 0, iErrStatus))){
#else
                                               hJobID, hUpdown, hOutTiming, 0, 0, 0, iErrStatus, hProcessNo))){
#endif
        iomnDebugMessage(">>>Error;iomnSendJobStartAck()\n",0);
        return(ERROR);
    }
    iomnDebugMessage(">>>OK:iomnSendJobStartAck()\n",0);
    #ifdef  DEBUG
        jobStatus(hJobID, &hJobstatus, &iSuscause);
        iomnDebugMessage("*** JOB STATUS -->%x\n",(I32)hJobstatus);
    #endif
    return(OK);
}


/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         7.2.5
TITLE:      印刷JOB開始受け付け受信処理
MODULE：    STATUS  iomnPrnJobStartAck(HI hJobID, HI hUpdown, HI hOutTiming)
INCLUDE:    ioMNPar.h
            ioMNstr.H
PARAMETER:
            HI  hJoibID;
            HI  hUpdown;
            HI  hOutTiming
RETURNS:    I32 4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            印刷JOB開始受け付けAckを受信し、JOB開始指示送信元タスクに対して印刷開始イベント
            メッセージを送信する。
NOTES:
HISTORY:
        1.1     Y.Matsuda   96/9/18
        1.2     Y.Matsuda   97/11/22
                FC371,373対応   機能切り替えキー押下に伴う優先度変更中は
                印刷ＵＩに切り替えない、復帰させないよう修正
                →割り込みからの復帰でＦＡＸＵＩに戻ってしまう不具合対応
        2.3     Y.Matsuda   97/12/18
                Error発生に伴いマシンｕｉに変更中に受信してしまった場合に
                印刷ＵＩ表示状態で維持されてしまう不具合修正
        M4.1?   (TJ)H.Takahashi 99/01/26
                CDJB変更対応
        M6.1    (TJ)H.Takahashi 99/05/07
                印刷JOB開始タイミング高速化対応
        D1.1    T.Fujii     99/11/15 DM45/35 後追い逐次機能組み込み
        D1.7.1 J.Ootani 00/03/01    DM45/35 SEINE FAX F-CODE対応(マージ)
        D2.0.1 T.Fujii 00/06/06     DM45/35 NOP_DEL(ソースコード整理)
                                            if文でのCOM_JB_BPPC,COM_JB_EXPPC1を削除
        D5.0   T.Fujii 00/12/12     DM45/35 CDJB多重動作対応

        MCK1.1  M.Karasawa  02/06/11    McKinley    新メッセージ対応
    V010.00_02  M.Karasawa  02/12/05    印刷のAJST送信タイミング変更
*/
STATUS  iomnPrnJobStartAck(HI hJobID, HI hUpdown, HI hOutTiming)
{
    STATUS  Status;             /*  各種処理結果    */
    HI      hDelFacter; /*  JOB削除要因 */

#if 0   /* PRINT_JOINT *//* EPSTにて制御するように変更 */
/* PRINT_JOINT */    if(ERROR == ioMNPJ_EPSTClear(hJobID)){                                                 /* PRINT_JOINT */
/* PRINT_JOINT */        iomnDebugMessage("[ioMN] ERROR : ioMNPJ_EPSTClear() in iomnPrnJobStartAck()\n",0); /* PRINT_JOINT */
/* PRINT_JOINT */    }                                                                                      /* PRINT_JOINT */
#endif
    /* V010.00_02   *//* ADD START  */
    memcpy(&ioMNsmyparam, &ioMNrmyparam, sizeof(union ioMNparam));
    if(ERROR == (Status = iomnSendMessage(ioMNCjst[hJobID].OldmsgQId, JOB_MSG_AJST, 0, &ioMNsmyparam))){
        iomnDebugMessage(">>>error:iomnSendMessage() in iomnSendMemoryResidue()\n",0);
        return(ERROR);
    }
    /* V010.00_02   *//* ADD END    */

    /*  JOB削除が予約されている場合、JOB中止処理を行う  */
    iomnDebugMessage("###JOB Delete Flug->%x\n",(I32)ioMNstatus.hJobDelete);
    if(ioMNstatus.hJobDelete == IOMN_ON){

        /* JOB削除要因取得 */
#if 0   /* MCK1.1 新メッセージ対応  *//* Member *//* 削除要因NULLいれたけど大丈夫？ */
/**//* D5.0 START T.Fujii CDJB多重動作対応 */
/**/#if 0   /* OLD */
/**/        hDelFacter = ioMNCdjb.msgparam.sCdjbCtl.hDelFacter;
/**/#else   /* D5.0 NEW */
/**/        hDelFacter = ioMNCdjb[hJobID].msgparam.sCdjbCtl.hDelFacter;
/**/#endif
/**//* D5.0 END */
#else
        hDelFacter = 0;
#endif
        /*  JOB中止指示送信 */
        if(ERROR == (Status = iomnSendJobCancel(hJobID, msgQIdIoExMN, 
                                                        hDelFacter))){
            iomnDebugMessage(">>>Error:iomnSendJobCancel()\n",0);
            return(ERROR);
        }
        iomnDebugMessage(">>>OK:iomnSendJobCancel()\n",0);
        /*  JOB削除予約フラグ解除   */
        ioMNstatus.hJobDelete = IOMN_OFF;
    }

    iomnDebugMessage(">>>>Bottom of iomnPrnJobStartAck()<<<<\n",0);
    return(OK);
}


/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         7.2.6
TITLE:      Dss入力JOB開始受け付け受信処理
MODULE：    STATUS  iomnDssScnJobStartAck(HI hJobID, HI hUpdown, HI hOutTiming, I32 iErrStatus)

INCLUDE:    ioMNPar.h
            ioMNstr.H
            
PARAMETER:
            HI  hJoibID;
            HI  hUpdown;
            HI  hOutTiming
            I32 iErrStatus
RETURNS:    I32 4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            Dss入力JOB開始受け付けAckを受信し、JOB開始指示送信元タスクに対してJOB開始ACK
            メッセージを送信する。
NOTES:
HISTORY:
        1.1     Y.Matsuda   96/9/18
        D1.13   T.Fujii     00/05/17 DM45/35 AJSTパラメータ拡張対応
        D2.4.1  T.Fujii     00/06/17 DM45/35 V004.70不具合対応
                                             AJSTパラメータ拡張対応修正

        MCK1.1  M.Karasawa 02/06/11 McKinley    新メッセージ対応
*/
#if 0   /* MCK1.1 新メッセージ対応  引数追加    */
/**/STATUS  iomnDssScnJobStartAck(HI hJobID, HI hUpdown, HI hOutTiming, I32 iErrStatus)
#else
STATUS  iomnDssScnJobStartAck(HI hJobID, HI hUpdown, HI hOutTiming, I32 iErrStatus, HI hProcessNo)
#endif
{
    STATUS  Status;
    #ifdef DEBUG
    HI      hJobstatus;
    I32     iSuscause;
    #endif

    /*  DSSJOB管理に対してJOB受け付けメッセージを送信   */
    if(ERROR == (Status = iomnSendJobStartAck(ioMNCjst[hJobID].OldmsgQId,
#if 0   /* MCK1.1 新メッセージ対応  引数追加    */
                                               hJobID, hUpdown, hOutTiming, 0, 0, 0, iErrStatus))){
#else
                                               hJobID, hUpdown, hOutTiming, 0, 0, 0, iErrStatus, hProcessNo))){
#endif
        iomnDebugMessage(">>>Error;iomnSendJobStartAck()\n",0);
        return(ERROR);
    }
    #ifdef  DEBUG
        jobStatus(hJobID, &hJobstatus, &iSuscause);
        iomnDebugMessage("*** JOB STATUS -->%x\n",(I32)hJobstatus);
    #endif

    return(OK);
}


/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         7.2.8
TITLE:      フォーム確認JOB開始受け付け処理
MODULE：    STATUS  iomnIVPrnJobStartAck(HI hJobID, HI hUpdown, HI hOutTiming, I32 iErrStatus)
INCLUDE:    ioMNPar.h
PARAMETER:
            HI  hJobID;
            HI  hUpdown;
            HI  hOutTiming;
            I32 iErrStatus
RETURNS:    I32 4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            フォーム確認印刷ＪＯＢ開始Ａｃｋメッセージを受信し、上位タスクに対し
            ＪＯＢ開始受付を通知する。
NOTES:
HISTORY:
        1.1     Y.Matsuda   96/9/18
        D1.13   T.Fujii     00/05/17 DM45/35 AJSTパラメータ拡張対応
        D2.4.1  T.Fujii     00/06/17 DM45/35 V004.70不具合対応
                                             AJSTパラメータ拡張対応修正
        MCK1.1  M.Karasawa 02/06/11 McKinley    新メッセージ対応
*/
#if 0   /* MCK1.1 新メッセージ対応  引数追加    */
/**/STATUS  iomnIVPrnJobStartAck(HI hJobID, HI hUpdown, HI hOutTiming, I32 iErrStatus )
#else
STATUS  iomnIVPrnJobStartAck(HI hJobID, HI hUpdown, HI hOutTiming, I32 iErrStatus, HI hProcessNo)
#endif
{
    STATUS  Status;
    HI      hJobstatus; /*  JOB状態         */
    I32     iSuscause;  /*  中断要因        */

    iomnDebugMessage(">>>>Top of iomnIVPrnJobStartAck()<<<<\n",0);

    /*  JOB開始指示もとに対してJOB受け付けメッセージを送信  */
    if(ERROR == (Status = iomnSendJobStartAck(ioMNCjst[hJobID].OldmsgQId,
#if 0   /* MCK1.1 新メッセージ対応  引数追加    */
                                               hJobID, hUpdown, hOutTiming, 0, 0, 0, iErrStatus))){
#else
                                               hJobID, hUpdown, hOutTiming, 0, 0, 0, iErrStatus, hProcessNo))){
#endif
        iomnDebugMessage(">>>Error;iomnSendJobStartAck()\n",0);
        return(ERROR);
    }
    iomnDebugMessage(">>>OK:iomnSendJobStartAck()\n",0);

    /*  フロントJOB状態取得 */
    if(ERROR == (Status = jobStatus(hioMNFrontJobID, &hJobstatus, &iSuscause))){
        iomnDebugMessage(">>>Error:jobStatus()\n",0);
        return(ERROR);
    }
    iomnDebugMessage("###Front JOB STATUS->%x\n",(I32)hJobstatus);

    iomnDebugMessage(">>>>Bottom of iomnIVPrnJobStartAck()<<<<\n",0);
    return(OK);
}


/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         7.2.9
TITLE:      フォーム登録JOB開始受け付け受信処理
MODULE：    STATUS  iomnIVSaveJobStartAck(HI hJobID, HI hUpdown, HI hOutTiming, I32 iErrStatus)
INCLUDE:    ioMNPar.h
            ioMNstr.H
PARAMETER:
            HI  hJoibID;
            HI  hUpdown;
            HI  hOutTiming
            I32 iErrStatus
RETURNS:    I32 4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            フォーム登録ＪＯＢの開始受付Ａｃｋを受信し、上位タスクに対し
            Ａｃｋを通知する
NOTES:
HISTORY:
        1.1     Y.Matsuda   96/9/18
        D1.13   T.Fujii     00/05/17 DM45/35 AJSTパラメータ拡張対応
        D2.4.1  T.Fujii     00/06/17 DM45/35 V004.70不具合対応
                                             AJSTパラメータ拡張対応修正

        MCK1.1  M.Karasawa 02/06/11 McKinley    新メッセージ対応
*/
#if 0   /* MCK1.1 新メッセージ対応  引数追加    */
/**/STATUS  iomnIVSaveJobStartAck(HI hJobID, HI hUpdown, HI hOutTiming, I32 iErrStatus)
#else
STATUS  iomnIVSaveJobStartAck(HI hJobID, HI hUpdown, HI hOutTiming, I32 iErrStatus, HI hProcessNo)
#endif
{
    STATUS  Status;
    #ifdef DEBUG
    HI      hJobstatus;
    I32     iSuscause;
    #endif

    iomnDebugMessage(">>>>Top of iomnIVSaveJobStartAck()<<<<\n",0);

    /*  JOB開始指示もとに対してJOB受け付けメッセージを送信  */
    if(ERROR == (Status = iomnSendJobStartAck(ioMNCjst[hJobID].OldmsgQId,
#if 0   /* MCK1.1 新メッセージ対応  引数追加    */
                                               hJobID, hUpdown, hOutTiming, 0, 0, 0, iErrStatus ))){
#else
                                               hJobID, hUpdown, hOutTiming, 0, 0, 0, iErrStatus, hProcessNo ))){
#endif
        iomnDebugMessage(">>>Error;iomnSendJobStartAck()\n",0);
        return(ERROR);
    }
    iomnDebugMessage(">>>OK:iomnSendJobStartAck()\n",0);

    #ifdef  DEBUG
        jobStatus(hJobID, &hJobstatus, &iSuscause);
        iomnDebugMessage("###JOB STATUS -->%x\n",(I32)hJobstatus);
    #endif
    iomnDebugMessage(">>>>Bottom of iomnIVSaveJobStartAck()<<<<\n",0);
    return(OK);
}


/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         7.2.10
TITLE:      JOB中断中止予約確認処理
MODULE：    STATUS  iomnSusCanReservation(HI hJobid)
INCLUDE:    ioMNpar.h
            ioMNstr.H
PARAMETER:
            HI  hJobid;
RETURNS:    I32 4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            JOB受付Ackを受け取った時点で、すでにJOB中断または中止を受け取っていた場合
            それらの処理を実行する
NOTES:
HISTORY:
        1.1     Y.Matsuda   96/9/18

        MCK1.1  M.Karasawa  02/06/11    McKinley    新メッセージ対応
    V024.00_01  M.Karasawa  03/07/16    McKinley    AJST待ちの予約中止処理の不具合対応
*/
STATUS  iomnSusCanReservation(HI hJobid)
{
    STATUS  Status;

    iomnDebugMessage(">>>Top of iomnSusCanReservation()<<<\n",0);

    /*  フロントJOBの中断が予約されている場合   */
    if(ioMNstatus.hReserveFrontSuspend == IOMN_ON){
        iomnDebugMessage("###Suspend JOBID->%x\n",(I32)ioMNFrontSusQue.hJobid);
        iomnDebugMessage("###Suspend SusFacter->%x\n",(I32)ioMNFrontSusQue.hSusFacter);
        iomnDebugMessage("###Suspend EntryID->%x\n",(I32)ioMNFrontSusQue.hEntryId);
        /*  中断指示送信    */
        if(ERROR == (Status = iomnSendJobSuspend(ioMNFrontSusQue.hJobid, msgQIdIoExMN, 
                                        ioMNFrontSusQue.hSusFacter, ioMNFrontSusQue.hEntryId))){
            iomnDebugMessage(">>>Error:iomnSendJobSuspend()\n",0);
            return(ERROR);
        }

        /*  予約フラグ解除  */
        ioMNstatus.hReserveFrontSuspend = IOMN_OFF;
        iomnDebugMessage(">>OK:Front JOB Suspend Reserve flag clear\n",0);
    }

    /*  印刷JOB中断が予約されている場合 */
    else if(ioMNstatus.hReserveBackSuspend == IOMN_ON){
        iomnDebugMessage("###Suspend JOBID->%x\n",(I32)ioMNBackSusQue.hJobid);
        iomnDebugMessage("###Suspend SusFacter->%x\n",(I32)ioMNBackSusQue.hSusFacter);
        iomnDebugMessage("###Suspend EntryID->%x\n",(I32)ioMNBackSusQue.hEntryId);
        /*  中断指示送信    */
        if(ERROR == (Status = iomnSendJobSuspend(ioMNBackSusQue.hJobid, msgQIdIoExMN, 
                                        ioMNBackSusQue.hSusFacter, ioMNBackSusQue.hEntryId))){
            iomnDebugMessage(">>>Error:iomnSendJobSuspend()\n",0);
            return(ERROR);
        }

        /*  予約フラグ解除  */
        ioMNstatus.hReserveBackSuspend = IOMN_OFF;
        iomnDebugMessage(">>OK:Back JOB Suspend Reserve flag clear\n",0);
    }

    /*  基本複写JOB中止が予約されている場合 */
    else if(ioMNstatus.hReserveJobCancel == IOMN_ON){
#if 0
/**/        iomnDebugMessage("###Cancel JOBID->%x\n",(I32)ioMNCccn.msgparam.sCjcnCtl.hJobid);
/**/        iomnDebugMessage("###Cancel Facter->%x\n",(I32)ioMNCccn.msgparam.sCjcnCtl.hCanFacter);
/**/        /*  中止指示送信    */
/**/        if(ERROR == (Status = iomnSendJobCancel(ioMNCccn.msgparam.sCjcnCtl.hJobid, 
/**/                                        msgQIdIoExMN, ioMNCccn.msgparam.sCjcnCtl.hCanFacter))){
/**/            iomnDebugMessage(">>>Error:iomnSendJobCancel()\n",0);
/**/            return(ERROR);
/**/        }
/**/        /*  予約フラグ解除  */
/**/        ioMNstatus.hReserveJobCancel = IOMN_OFF;
/**/        iomnDebugMessage(">>OK:JOB Cancel Reserve flag clear\n",0);
#else
        /* キャンセル指示を受けているジョブかどうかの判断。*/
        if( ioMNCjcnQue.hJobid == hJobid){
            iomnDebugMessage("###Cancel JOBID->%x\n",(I32)ioMNCjcnQue.hJobid);
            iomnDebugMessage("###Cancel Facter->%x\n",(I32)ioMNCjcnQue.hCanFactor);
            /*  中止指示送信    */
            if(ERROR == (Status = iomnSendJobCancel(ioMNCjcnQue.hJobid, 
                                            msgQIdIoExMN, ioMNCjcnQue.hCanFactor))){
                iomnDebugMessage(">>>Error:iomnSendJobCancel()\n",0);
                return(ERROR);
            }
            ioMNstatus.hReserveJobCancel = IOMN_OFF;
            /* 動作上問題ないのでクリアしない。ログに情報残しておきたいので。
            ioMNCjcnQue.hJobid = 0;
            ioMNCjcnQue.hCanFactor = 0;
            */
            iomnDebugMessage(">>OK:JOB Cancel Reserve flag clear\n",0);
        }
#endif
    }
    iomnDebugMessage(">>>Bottom of iomnSusCanReservation()<<<\n",0);
    return(OK);
}


/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         7.3
TITLE:      JOB終了ステータス受信処理
MODULE：    STATUS  iomnRecieveTJED()           JOB終了ステータス受信処理
INCLUDE:    ioMNPar.h
PARAMETER:  N/A
RETURNS:    I32 4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            JOB終了メッセージ受信時の処理。
NOTES:
HISTORY:
        1.1     Y.Matsuda   96/9/18
        M1.1    (TJ)H.Takahashi 98/06/11
                手置き逐次複写対応のため
        M1.2(TJ)H.Takahashi 98/09/25
                DSS逐次対応
        M1.3(TJ)H.Takahashi 98/11/04
                割り込み複写対応
        D1.7.1 J.Ootani 00/03/01    DM45/35 SEINE FAX F-CODE対応(マージ)
        D2.0.2  T.Fujii     00/07/04 DM45/35 NOP_DEL(ソースコード整理)
                                             if文,case文でのCOM_JB_EXPPC1を削除
*/
STATUS  iomnRecieveTJED()
{
    STATUS  Status;     /*  各種処理結果    */
    HI      hjobid;     /*  ＪＯＢＩＤ      */
    HI      hSts;       /*  終了ステータス  */
    I32     iErrEng;    /*  エンジン系エラー要因    */
    I32     iErrSys;    /*  アプリ系エラー要因  */
    HI      hOrgBack;   /*  原稿戻し枚数    */
    HI      hJobtype;   /*  JOB種類 */
    HI      hSubJobmode;    /* サブジョブ種別 */

    /*  パラメータ取得  */
    hjobid = ioMNrmyparam.sTjedCtl.hJobid;
    hSts = ioMNrmyparam.sTjedCtl.hSts;
    iErrSys = ioMNrmyparam.sTjedCtl.iErrSys;
    iErrEng = ioMNrmyparam.sTjedCtl.iErrEng;
    hOrgBack = ioMNrmyparam.sTjedCtl.hOrgBack;
#if 0
    iomnDebugMessage("### JOBID -->%x\n",(I32)hjobid);
    iomnDebugMessage("### hSts -->%x\n",(I32)hSts);
    iomnDebugMessage("### iErrSys -->%x\n",iErrSys);
    iomnDebugMessage("### iErrEng -->%x\n",iErrEng);
    iomnDebugMessage("### hOrgBack -->%x\n",(I32)hOrgBack);
#endif
	cmnPrintf_ioMN(7,"### JOBID -->%x, hSts -->%x, iErrSys -->%x, iErrEng -->%x, hOrgBack -->%x \n", (I32)hjobid, (I32)hSts, iErrSys, iErrEng, (I32)hOrgBack);
    iomnDelay(100);

	/* Feedback EBX_STFR_17062 --Start */
			/* Setting of EPST received status */
	if(hjobid >= 4)
	{
		cmnPrintf_ioMN(7,"Setting EPST status calling iomnEPSTReceivedCheck\n");
		iomnEPSTReceivedCheck(hjobid);
	}		
	/* Feedback EBX_STFR_17062 --End */
    if(ERROR == ioMNPJ_EPSTClear(hjobid)){                                                  /* PRINT_JOINT */
        iomnDebugMessage("[ioMN] ERROR : ioMNPJ_EPSTClear() in iomnRecieveESPR()\n",0);     /* PRINT_JOINT */
    }                                                                                       /* PRINT_JOINT */

    /*  JOB終了メッセージ格納   */
    iomnSetJobManagementMsg(&ioMNTjed[hjobid]);

    /*  JOB種別取得 */
    if(ERROR == (Status = jobJobtype(hjobid, &hJobtype))){
        iomnDebugMessage(">>>Error:jobJobtype() in iomnRecieveTJED()\n",0);
        return(ERROR);
    }
    iomnDebugMessage("###JOB TYPE -> %x\n",(I32)hJobtype);

    /*  JOB種別ごとにJOB中断処理    */
    switch(hJobtype){
        case    COM_JB_IPPC:
        case    COM_JB_EXPPC2:
                /* サブジョブ種別をチェックして処理を分岐 */
                if( parMNSubJobmode(hjobid, &hSubJobmode) == ERROR ) {
                    return( ERROR );
                }
                if( hSubJobmode == COM_JB_SUB_INPUT ) { /* サブジョブ種別が入力のとき */
                    if(ERROR == (Status = iomnPPCTJED(hjobid, hSts, iErrSys, iErrEng, hOrgBack, hJobtype))){
                        iomnDebugMessage(">>>Error:iomnPPCTJED()\n",0);
                        return(ERROR);
                    }
                }
                else {  /* サブジョブ種別が出力のとき */
                    if(ERROR == (Status = iomnPrnTJED(hjobid, hSts, iErrSys, iErrEng, hOrgBack, hJobtype))){
                        iomnDebugMessage(">>>Error:iomnPrnTJED()\n",0);
                        return(ERROR);
                    }
                }
                break;
        case    COM_JB_SCN_FAX1:
        case    COM_JB_SCN_FAX2:
        case    COM_JB_POLL_SEND_FAX1:
        case    COM_JB_POLL_SEND_FAX2:
        case    COM_JB_IVSAVE_FAX:
        case    COM_JB_IVSAVE_PPC:
        case    COM_JB_DRC_SCN_FAX:
        case    COM_JB_SEND_DSI:
        case    COM_JB_CON_REG_FAX2:
        case    COM_JB_BOD_REG_FAX2:
                if(ERROR == (Status = iomnScnTJED(hjobid, hSts, iErrSys, iErrEng, hOrgBack, hJobtype))){
                    iomnDebugMessage(">>>Error:iomnScnTJED()\n",0);
                    return(ERROR);
                }
                break;
        case    COM_JB_PRT_FAX1:
        case    COM_JB_PRT_FAX2:
        case    COM_JB_LIST_PRT_FAX:
        case    COM_JB_PRT_GDI1:
        case    COM_JB_PRT_GDI2:
        case    COM_JB_DRC_PRT_GDI:
        case    COM_JB_DRC_PRT_FAX:
        case    COM_JB_IVPRM_PPC:
        case    COM_JB_IVPRM_FAX:
        case    COM_JB_PRIVATE_DSI:
        case    COM_JB_RECV_DSI:
        case    COM_JB_PRIVATE_DSI_SYNC:
        case    COM_JB_RECV_DSI_SYNC:
        case    COM_JB_CON_GET_FAX2:
        case    COM_JB_BOD_GET_FAX2:
                if(ERROR == (Status = iomnPrnTJED(hjobid, hSts, iErrSys, iErrEng, hOrgBack, hJobtype))){
                    iomnDebugMessage(">>>Error:iomnPrnTJED()\n",0);
                    return(ERROR);
                }
                break;
        case    COM_JB_UNDEF:
        case    COM_JB_JOB_UI:
        case    COM_JB_DIG:
        default:
            iomnDebugMessage(">>>Error:Undefined JOB Type!-->%x\n",(I32)hJobtype);
            return(ERROR);
    }
    return(OK);
}


/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         7.3.1
TITLE:      PPCJOB終了ステータス受信処理
MODULE：    STATUS  iomnPPCTJED(HI hjobid, HI hSts, I32 iErrSys, I32 iErrEng, HI hOrgBack, HI hJobtype)
INCLUDE:    ioMNPar.h
            ioMNstr.h
PARAMETER:
            HI  hjobid;     JOBID
            HI  hSts        終了結果
            I32 iErrSys     アプリ系エラー要因
            I32 iErrEng     エンジン系エラー要因
            HI  hOrgBack    原稿戻し枚数
RETURNS:    I32 4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            JOB終了メッセージ受信時の処理。
NOTES:
HISTORY:
        1.1     Y.Matsuda   97/01/25
        M1.1    (TJ)H.Takahashi 98/07/07
                手置き逐次複写対応のため
        M1.2    (TJ)H.Takahashi 98/08/28
                手置き逐次マシンリカバリ対応のため
        M1.2a   手置きリリース延期のため
        D1.1    T.Fujii  99/11/15 DM45/35 後追い逐次・並行動作対応
        D1.2.1  T.Fujii  00/02/23 DM45/35 サブジョブ中断処理修正
        D1.13   T.Fujii  00/05/17 DM45/35 中断処理修正
        D1.9.7  J.Ootani 00/06/06 DM45/35 逐次中断時ALLリカバリ対応
        D2.4    T.Fujii  00/06/15 DM45/35 prnEX遅延中断対応
        D3.31   T.Fujii  00/10/10 DM45/35 マシンリカバリ処理修正
        D3.31.3 T.Fujii  00/10/24 DM45/35 逐次コピー時もmcnUIを表示させる

        MCK1.1  M.Karasawa  02/06/11    McKinley    新メッセージ対応
        MCK1.5  M.Karasawa  02/07/26    McKinley    DELETE iomnChangeToMcnUI
    V040.00_01  M.Karasawa  04/03/23    McKinley    TJSXを送信していない(Pre3736に関連)
    V042.00_01  M.Karasawa  04/04/29    McKinley    逐次動作時の紙粉対応
    eB3-05      M.Taki      08/02/20    BP/MASH     画質維持中エラー中断処理対応
*/
STATUS  iomnPPCTJED(HI hjobid, HI hSts, I32 iErrSys, I32 iErrEng, HI hOrgBack, HI hJobtype)
{
    STATUS  Status;
    HI      hJobStat=0;																									/* SM-001 */
    STATUS  Status2;
    I32     iSusCause;
	HI hCode;

#if defined (S2_ENG)																									/* S2_ENG */
	if( iErrEng == (M_f220EngErr|(UH16)ODC_ERR_MOVE_CHG_STOP)){															/* S2-001 */
		parMNSetOutputChgFlg(hjobid, TRUE);																				/* S2-001 */
	}																													/* S2-001 */
#endif																													/* S2_ENG */

    /* V042.00_01 START */
    if( iErrEng == (M_f220EngErr|(UH16)ENG_ERR_PRINT_OVER)    ||
#if defined (S2_ENG)																									/* S2_ENG */
        iErrEng == (M_f220EngErr|(UH16)ODC_ERR_MOVE_CHG_STOP) ||                                                        /* S2-001 */
#endif																													/* S2_ENG */
        iErrEng == IOEXMN_ERR_PTN_RETRY){   /* 画質維持中エラーの場合 *//* eB3-05 */
        /*
            AJST受信時に印刷キューへの登録を避けるため、擬似的に再開したようにしておく必要あり。
            通常CJSTのContinueで判断するので書き換えておく。
        */
        ioMNCjst[hjobid].msgparam.sCjstCtl.hContinue = IOMSG_JB_CONTINUE;
        ioMNCjst[hjobid].msgparam.sCjstCtl.hIndiType = IOMSG_JB_CONTINUE;
        if( iomnSendJobStart(hjobid, IOMSG_IN_MANUAL, IOMSG_JB_CONTINUE, 0) != ERROR ) {
            return(OK);     /* コピー開始(成功) */
        }else{
            return(ERROR);
        }
    }
	printf("Staple Alarm iErrEng = %X  (0x0000FFFF & iErrEng) = %X  ENG_ERR_FIN_STA_ALARM = %d\n", iErrEng, (0x0000FFFF & iErrEng), ENG_ERR_FIN_STA_ALARM);
	if(ENG_ERR_FIN_STA_ALARM == (0x0000FFFF & iErrEng))
	{
		if(hJobtype == COM_JB_IPPC || hJobtype == COM_JB_EXPPC2)
		{
			parMNNV08ParamI(STAPLE_NEEDLE_CHECK_INTERRUPT, 0, &hCode);
		}
		else
		{
			parMNNV08ParamI(STAPLE_NEEDLE_CHECK_INTERRUPT, 1, &hCode);
		}

		if(hCode == 0)
		{
			return OK;
		}
	}
    /* V042.00_01 END   */

    /* JOB状態算出処理 */
    Status = iomnCalcJobStatus( hjobid, hSts, iErrSys, iErrEng, &hJobStat );
    switch( Status ){
        case    IOMN_ON :
                /* 中断要因取得 */
                if(iErrSys != IOMSG_NOT_USED){
                        iSusCause = iErrSys;
                }
                else if(iErrEng != IOMSG_NOT_USED){
                        iSusCause = iErrEng;
                }
                else iSusCause = 0;

                /* JOB状態変更あり */
                if ( ERROR == (Status2=jobSetStatus(hjobid,hJobStat,iSusCause))){
                        cmnPrintf_ioMN(2,"iomnPPCTJED()-IOMN_ON-jobSetStatus() ERROR\n");
                        return ERROR;
                }
                iomnDebugMessage(">>>iomnPPCTJED()Change JOB STATUS[%x]\n",
                                hJobStat);
                break;
        case    IOMN_OFF:
                /* JOB状態変更無し */
                iomnDebugMessage(">>>iomnPPCTJED() No Change JOB STATUS\n",0);
                break;
        default :
                cmnPrintf_ioMN(2,"iomnPPCTJED()-default ERROR\n");
                return ERROR;
    }

    /*  Error発生時 */
#if 0  /* V040.00_01 START */
/**//* D3.31 START T.Fujii リソースごとの処理に修正 */
/**/#if 0   /* OLD */
/**//**/    if(ioMNstatus.hError_sts != 0){
/**/#else   /* D3.31 MID    NEW */
/**/    if( (ioMNstatus.hError_sts & (MCN_RCV_RSC_SCN|MCN_RCV_RSC_ADF)) ||
/**/        (iomnCheckSCS(hjobid) && ioMNstatus.hError_sts != 0) ) {
/**/#endif
/**//* D3.31 END */
#else
    if( (ioMNstatus.hError_sts & (MCN_RCV_RSC_SCN|MCN_RCV_RSC_ADF)) ||
        (iomnCheckSCS(hjobid) && ioMNstatus.hError_sts != 0)     ||
        iErrSys == S_scnEX_COUNT_OVER                               ||  /* 原稿枚数オーバー         */
        iErrSys == S_scnEX_PAGEOVER_NAKAORI                         ||  /* 中折綴じで枚数オーバー   */
        iErrSys == S_scnEX_PAGEOVER_PAGEADD                         ||  /* ページ付加で枚数オーバー */
        iErrSys == S_scnEX_PAGEOVER_COVER                           ||  /* 表紙で枚数オーバー       */
        iErrSys == S_scnEX_PAGEOVER_SHEET                           ||  /* シートで枚数オーバー     */
        iErrSys == S_FL_VOLUME_AREAFULL                             ||  /* ボリュームの空き領域不足 */
        iErrSys == S_scnEX_MEMFUL_NAKAORI                           ||  /* 中折綴じで枚数メモリフル */
        iErrSys == S_scnEX_MEMFUL_PAGEADD                           ||  /* ページ付加でメモリフル   */
        iErrSys == S_scnEX_MEMFUL_COVER                             ||  /* 表紙メモリフル           */
        iErrSys == S_scnEX_MEMFUL_SHEET                             ){  /* シートメモリフル         */
#endif /* V040.00_01 END   */
        /*  JOB終了メッセージ送信   */
#if 0   /* MCK1.1 新メッセージ対応  */
/**/        if(ERROR == (Status = iomnSendTJED(ioMNCjst[hjobid].OldmsgQId, IOMSG_TJED_UI, hjobid,
#else
        if(ERROR == (Status = iomnSendTJED(ioMNCjst[hjobid].OldmsgQId, JOB_MSG_TJSX, hjobid,
#endif
                                                                    hSts, iErrSys, iErrEng, hOrgBack))){
            cmnPrintf_ioMN(2,">>>Error:iomnSendTJED()\n");
            return(ERROR);
        }
        iomnDebugMessage(">>>Send TJED Message for Error Status\n",0);
    }
    /*  Error非発生時   */
    else{
        /*  リカバリコマンド送信    */
        /* 逐次/完了情報を取得 */
        if(iomnCheckSCS(hjobid) == TRUE)
        {
            if(ERROR == (Status = iomnMcnRecover(hjobid, MCN_RCV_MODE_PPC,
                                                             MCN_RCV_RSC_ALL, IOMN_MCN_REC_TJED, 0))){
                cmnPrintf_ioMN(2,">> ioMN >> Error:iomnMcnRecover() in iomnPPCTJED()\n");
                return(ERROR);
            }
            iomnDebugMessage(">> ioMN >> CHECK:Mcn Recover for TJED Message on Resource Printer\n",0);
        }
        else
        {
            if(ERROR == (Status = iomnMcnRecover(hjobid, MCN_RCV_MODE_PPC,
                                          MCN_RCV_RSC_SCN, IOMN_MCN_REC_TJED, 0))){
                cmnPrintf_ioMN(2,">>>Error:iomnMcnRecover() in iomnPPCTJED()\n");
                return(ERROR);
            }
            iomnDebugMessage(">>>Mcn Recover for TJED Message on Resource Scanner\n",0);
        }
    }
    return(OK);
}


/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         7.3.2
TITLE:      原稿入力JOB終了ステータス受信処理
MODULE：    STATUS  iomnScnTJED(HI hjobid, HI hSts, I32 iErrSys, I32 iErrEng, HI hOrgBack, HI hJobtype)
INCLUDE:    
            ioMNPar.h
            ioMNstr.h
PARAMETER:
            HI  hjobid;     JOBID
            HI  hSts        終了結果
            I32 iErrSys     アプリ系エラー要因
            I32 iErrEng     エンジン系エラー要因
            HI  hOrgBack    原稿戻し枚数
RETURNS:    
            I32 4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            JOB終了メッセージ受信時の処理。
NOTES:
HISTORY:
        1.1     Y.Matsuda   97/01/25
        M1.1    (TJ)H.Takahashi 98/08/10
                手置き逐次型複写対応のため、JOB状態設定を本関数で実行
        D1.2.1  T.Fujii 00/02/23 DM45/35 サブジョブ中断処理修正
        D3.31   T.Fujii 00/10/10 DM45/35 マシンリカバリ処理修正

        MCK1.1  M.Karasawa  02/06/11    McKinley    新メッセージ対応
        MCK1.2  M.Karasawa  02/06/21    McKinley    UI変更処理コメントアウト
        MCK1.5  M.Karasawa  02/07/26    McKinley    DELETE iomnChangeToMcnUI

    V007.00_01  M.Karasawa  02/09/02    McKinley    入力中断時、出力の中断は上位からの指示待ち
    V040.00_01  M.Karasawa  04/03/23    McKinley    TJSXを送信していない(Pre3736に関連)
*/
STATUS  iomnScnTJED(HI hjobid, HI hSts, I32 iErrSys, I32 iErrEng, HI hOrgBack, HI hJobtype)
{
    STATUS  Status;
    HI      hMode;      /*  リカバリモード  */
    I32     imsgid;     /*  MessageID   */
    I32     iSuscause;

    iomnDebugMessage(">>>Top of iomnScnTJED()<<<\n",0);
    iomnDebugMessage("###JOB TYPE->%x\n",(I32)hJobtype);

    /*  マシンリカバリモード取得    */  /* D1.1 T.Fujii 99/11/15 */
    if(ERROR == (Status = iomnGetMcnRecmode(hjobid, hJobtype, &hMode))){
        iomnDebugMessage(">>>Error:iomnGetMcnRecmode()\n",0);
        return(ERROR);
    }
    iomnDebugMessage("###Recovery Mode->%x\n",(I32)hMode);

    /* 中断要因の判定 */
    if(iErrSys != IOMSG_NOT_USED){
       iSuscause = iErrSys;
    }
    else if(iErrEng != IOMSG_NOT_USED){
        iSuscause = iErrEng;
    }
    else iSuscause = 0;

    /* JOB状態の設定 */
    if ( ERROR == (Status=jobSetStatus(hjobid,COM_JS_SUSPEND,iSuscause)) ){
        cmnPrintf_ioMN(2,">>>Error:jobSetStatus()\n");
        return(ERROR);
    }

#if 0   /* V007.00_01   */
/**/    if( hSts == IOMSG_SUS_ERROR ) { /* エラーによる中断の場合 */
/**/        if( iSuscause != S_scnEX_NO_NEXT_ORIGINAL ) {   /* 次原稿無しの場合はなにもしない */
/**/            iomnDebugMessage(">> ioMN >> CHECK:TJED is IOMSG_SUS_ERROR in iomnScnTJED()\n",(I32)0);
/**/            /* 出力ジョブが存在すれば中断処理を行う */
/**/            iomnOutputJobSuspend( hjobid, IOMN_SUS_FRONT );
/**/        }
/**/    }
#endif

    /*  Error発生時 */
#if 0  /* V040.00_01 START */
/**//* D3.31 START T.Fujii リソースごとの処理に修正 */
/**/#if 0   /* OLD */
/**//**/    if(ioMNstatus.hError_sts != 0){
/**/#else   /* D3.31 MID    NEW */
/**/    if( ioMNstatus.hError_sts & (MCN_RCV_RSC_SCN|MCN_RCV_RSC_ADF) ) {
/**/#endif
/**//* D3.31 END */
#else
    if((ioMNstatus.hError_sts & (MCN_RCV_RSC_SCN|MCN_RCV_RSC_ADF))||
        iErrSys == S_scnEX_COUNT_OVER                             ||  /* 原稿枚数オーバー         */
        iErrSys == S_scnEX_PAGEOVER_NAKAORI                       ||  /* 中折綴じで枚数オーバー   */
        iErrSys == S_scnEX_PAGEOVER_PAGEADD                       ||  /* ページ付加で枚数オーバー */
        iErrSys == S_scnEX_PAGEOVER_COVER                         ||  /* 表紙で枚数オーバー       */
        iErrSys == S_scnEX_PAGEOVER_SHEET                         ||  /* シートで枚数オーバー     */
        iErrSys == S_FL_VOLUME_AREAFULL                           ||  /* ボリュームの空き領域不足 */
        iErrSys == S_scnEX_MEMFUL_NAKAORI                         ||  /* 中折綴じで枚数メモリフル */
        iErrSys == S_scnEX_MEMFUL_PAGEADD                         ||  /* ページ付加でメモリフル   */
        iErrSys == S_scnEX_MEMFUL_COVER                           ||  /* 表紙メモリフル           */
        iErrSys == S_scnEX_MEMFUL_SHEET                           ){  /* シートメモリフル         */
#endif /* V040.00_01 END   */
        /*  MessageID取得   */
#if 0   /* MCK1.1 新メッセージ対応  */
/**/        if(ioMNCjst[hjobid].iOldMsgId == IOMSG_CJST_UI){
/**/            imsgid = IOMSG_TJED_UI;
/**/        }
/**/        else{
/**/            imsgid = IOMSG_TJED_JOB;
/**/        }
#else
        imsgid = JOB_MSG_TJSX;
#endif
        /*  JOB終了メッセージ送信   */
        if(ERROR == (Status = iomnSendTJED(ioMNCjst[hjobid].OldmsgQId, imsgid, hjobid,
                                                                hSts, iErrSys, iErrEng, hOrgBack))){
            iomnDebugMessage(">>>Error:iomnSendTJED()\n",0);
            return(ERROR);
        }
        iomnDebugMessage(">>>OK:Send TJED Message\n",0);
    }
    /*  Error非発生時   */
    else{
        /*  リカバリコマンド送信    */
        if(ERROR == (Status = iomnMcnRecover(hjobid, hMode, MCN_RCV_RSC_SCN, IOMN_MCN_REC_TJED, 0))){
            iomnDebugMessage(">>>Error:iomnMcnRecover() in iomnPPCTJED()\n",0);
            return(ERROR);
        }
        iomnDebugMessage(">>>Mcn Recover for TJED Message on Resource Scanner\n",0);
    }
    iomnDebugMessage(">>>Bottom of iomnScnTJED()<<<\n",0);
    return(OK);
}


/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         7.3.4
TITLE:      GDI印刷JOB終了ステータス受信処理
MODULE：    STATUS  iomnPrnTJED(HI hjobid, HI hSts, I32 iErrSys, I32 iErrEng, HI hOrgBack, HI hJobtype)
INCLUDE:    
            ioMNPar.h
            ioMNstr.h
PARAMETER:
            HI  hjobid;     JOBID
            HI  hSts        終了結果
            I32 iErrSys     アプリ系エラー要因
            I32 iErrEng     エンジン系エラー要因
            HI  hOrgBack    原稿戻し枚数
RETURNS:    
            I32 4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            JOB終了、自発中断メッセージ受信時の処理。ステータスによって中断終了メッセージ送信等
            を行なう。
NOTES:
HISTORY:
        1.1     Y.Matsuda   97/01/25
        M1.1    (TJ)H.Takahashi 98/08/10
                手置き逐次型複写対応のため、JOB状態設定を本関数で実行
    M1.2(TJ)H.Takahashi 98/08/12
        自発中断時にpuJobDelete()をcallしないように修正
    D2.0.1  T.Fujii     00/06/06 DM45/35 NOP_DEL(ソースコード整理)
                                         puJobDelete()関数の呼び出しを削除
    D2.11   T.Fujii     00/07/04 DM45/35 用紙無しなどの時、マシンリカバリ省略対応
    D2.11.1 T.Fujii     00/07/11 DM45/35 マシンリカバリを省略するときの中断要因を追加

    MCK1.1  M.Karasawa 02/06/11 McKinley    新メッセージ対応
    MCK1.2  M.Karasawa 02/06/21 McKinley    UI変更処理コメントアウト
V034.04_01  M.Karasawa 03/12/19 McKinley    サドルフルでコンパネロック対策(McK5235)
V036.00_01  M.Karasawa 04/01/29 McKinley    モノクロ印刷連続中の自動中断対応

RioGrande
    Beta3   M.Karasawa 04/10/26 RioGrande   稼動中トナー補給中断対応

K2
    VTK10.000   M.Karasawa  06/02/08    K2  Pre7641,7871対策
BP/Mash
    eB3-05      M.Taki      08/02/20    BP/MASH     画質維持中エラー中断処理対応
    eB3-07      N.Sato      09/05/18    LOIRE/AL    手差しガイド幅チェックエラー対応
*/
STATUS  iomnPrnTJED(HI hjobid, HI hSts, I32 iErrSys, I32 iErrEng, HI hOrgBack, HI hJobtype)
{
    STATUS  Status;     /*      */
    HI      hRecmode;   /*  マシンリカバリモード    */
    I32     imsgid;     /*  MessageId   */
    I32     iSuscause;
	HI hCode;
/* 20120917 - L4.5 Print Prohibition ITC - SLDL177 fix*/
	HI hRetry = FALSE;
	HI hRetrySaddleExit = FALSE; /*EBX_DCR_57038, JEC-01518_CRS_FinExit(Saddle)_when_the_job_restarts.docx */

	HI hUsrConfirmSts;
	MCN_TBL_PAPERINFO sPaperInf;
	mcnTBLPaperInfo(&sPaperInf);

    /*  マシンリカバリモード取得    */   /*　D1.1 T.Fujii 99/11/15 */
    if(ERROR == (Status = iomnGetMcnRecmode(hjobid, hJobtype, &hRecmode))){
        iomnDebugMessage(">>>Error:iomnGetMcnRecmode()\n",0);
        return(ERROR);
    }
    iomnDebugMessage("###Machine Recovery Type->%x\n",(I32)hRecmode);

    /* D1.1 印刷中断メッセージ送信処理を変更 */
    /* iomnSendTJED()の中で以下のメッセージ送信をするように変更 */
    /* １．フロントUIにEJED_UIを送信 */
    /* ２．印刷種別に応じて管理タスク/prnUIにEJEDを送信 */

    /* 中断要因の判定 */
    if(iErrSys != IOMSG_NOT_USED){
        iSuscause = iErrSys;
    }
    else if(iErrEng != IOMSG_NOT_USED){
        iSuscause = iErrEng;
    }
    else iSuscause = 0;
    iomnDebugMessage(">>>Suspended for Error Type : %x\n",iSuscause);

    /* JOB状態の設定 */
    if ( ERROR == (Status=jobSetStatus(hjobid,COM_JS_SUSPEND,iSuscause)) ){
       cmnPrintf_ioMN(2,">>>Error:jobSetStatus()\n");
       return(ERROR);
    }

#if defined (S2_ENG)																									/* S2_ENG */
	if( iErrEng == (M_f220EngErr|(UH16)ODC_ERR_MOVE_CHG_STOP)){															/* S2-001 */
		parMNSetOutputChgFlg(hjobid, TRUE);																				/* S2-001 */
	}																													/* S2-001 */
#endif																													/* S2_ENG */
	if(ERROR == (Status = parMNGetSaddleExitRetryPossible(hjobid, &hRetrySaddleExit))) /*EBX_DCR_57038, JEC-01518_CRS_FinExit(Saddle)_when_the_job_restarts.docx*/
	{
	  iomnDebugMessage(">>>Error:parMNGetSaddleExitRetryPossible()\n",0);
	  return(ERROR);
	}

/* V036.00_01 START */
    /*
        大量のモノクロ印刷中に08の枚数設定により、紙カスを落とすため自発中断する
        （上位への通知、マシンリカバリは抑制。再開処理は自動再開。）
    */
/*	Saravana added if (iErrEng == (M_f220PrnEXErr | IOMSG_ERR_LONGPAPER_SIZE)) condn to 
	block TJSX being sent to SL, as SL sends C_cancel for next job		*/
/* 20120917 - L4.5 Print Prohibition ITC - SLDL177 fix*/
	parMNRetryPossible(hjobid, &hRetry);
	if( iErrEng == (M_f220EngErr|(UH16)ENG_ERR_PRINT_OVER)        ||
#if defined (S2_ENG)																									/* S2_ENG */
		iErrEng == (M_f220EngErr|(UH16)ODC_ERR_MOVE_CHG_STOP)     ||													/* S2-001 */
#endif																													/* S2_ENG */
		iErrEng == IOEXMN_ERR_PTN_RETRY                           ||
		iErrSys == (M_f220PrnEXErr | IOMSG_ERR_LONGPAPER_SIZE)    ||
		hRetrySaddleExit == TRUE                                  || /*EBX_DCR_57038, JEC-01518_CRS_FinExit(Saddle)_when_the_job_restarts.docx*/
		/* 20110209 - prnEX sends IOMSG_ERR_JOB_START with TJSX when engPrintOpen returns ERROR*/
		(iSuscause == (M_f220PrnEXErr | IOMSG_ERR_JOB_START))     ||
#if !defined (S2_ENG)																									/* S2_ENG */
		(iSuscause == (M_f220PrnEXErr | IOMSG_ERR_PRINT_ERRSTP))  ||													/* S2-002 */
#endif																													/* S2_ENG */
#if defined (WEISS_ENG) || defined (S2_ENG)
		(iSuscause == (M_f220EngErr|(UH16)ENG_ERR_MCN_TEMP_FALL)) ||
#endif
		(iSuscause == (M_f220EngErr|(UH16)ENG_ERR_HEATER_TEMP))   ||				/* added as per Sei-san request -- roopesh */
		(iSuscause == (M_f220EngErr|(UH16)ENG_ERR_TONER_SUPPLY))  ||				/* Fixed for STFR_14622 */
		/* If Retry Is TRUE and Paper out suspension Then do not send Suspension to SL, Retry the job from ioMN */
		(hRetry == TRUE && (M_f220EngErr|(UH16)ENG_ERR_PAPER_EMPTY) == iSuscause)){	/* 画質維持中エラーの場合 *//* eB3-05 */
		cmnPrintf_ioMN(8,"iomnPrnTJED: not sending TJSX to wrap iSuscause = %X; CJST is tried in a loop by ioMN \n",iSuscause);
		/* --Prashant changed for EBX_STFR_17335 Not blocking CJST for any of the above errors, 
		Retrying sending of CJST immediately without any error check for retryable errors othr than below two errors*/ 	
/* EBX_DTFR_19548 starts */
#if 0
		if(!(iErrEng == IOEXMN_ERR_PTN_RETRY || iSuscause == (M_f220PrnEXErr | IOMSG_ERR_JOB_START)) && (ioMNCjcn[hjobid].iOldTaskId == 0) && (ioMNCjsu[hjobid].iOldTaskId == 0)) {  /* EBX_DTFR_17113 FIX*/
				taskDelay(50);	// Adding a delay of 500ms before retrying - EBX_DCR_49844 - Jose
/*Rajasekhar: ioMN Force trigger issue, In case of Retryable errors from ioMN directly we are calling iomnPrnJobSendStart() function, hence changes are required here for retryable errors
EBX_DCR_53040_feedback_L4.91_EBX_DCR_57425*/
			HI hForceTrigger = 0;	
			HI hStartTrigger = -1;
			if( parMNForceFlag(hjobid, &hForceTrigger) != ERROR )	
			{			
				if( hForceTrigger == TRUE )
					hStartTrigger = IOMSG_IN_FORCE;		
			}
/*changes end*/		
			iomnPrnJobSendStart(hjobid,IOMSG_JB_CONTINUE,ioMNCjst[hjobid].msgparam.sCjstCtl.hPrtType, hStartTrigger);
		}
#endif
		ioMNstatus.hUnblockCJST_JobId = hjobid;
/* EBX_DTFR_19548 ends */
		return(OK);
	}
	printf("Staple Alarm iErrEng = %X  (0x0000FFFF & iErrEng) = %X  ENG_ERR_FIN_STA_ALARM = %X\n", iErrEng, (0x0000FFFF & iErrEng), ENG_ERR_FIN_STA_ALARM);
	if(ENG_ERR_FIN_STA_ALARM == (0x0000FFFF & iErrEng))
	{
		if(hJobtype == COM_JB_IPPC || hJobtype == COM_JB_EXPPC2)
		{
			parMNNV08ParamI(STAPLE_NEEDLE_CHECK_INTERRUPT, 0, &hCode);
		}
		else
		{
			parMNNV08ParamI(STAPLE_NEEDLE_CHECK_INTERRUPT, 1, &hCode);
		}

		if(hCode == 0)
		{
			return OK;
		}
	}

	/******new job skip and interrupt requirement********/
	if(iErrSys == (M_f220PrnEXErr | IOMSG_ERR_BYPASS))
	{
		parMNJSByPassConfirmStatus(hjobid, &hUsrConfirmSts);
		if(hUsrConfirmSts == TRUE)
		{
/* L4.0 --> L3.3 - FR11574 - SLDL365 - 20120503 */
			/*FR11574: New updated Request from MSM.
			If no paper on bypass tray then Suspend for bypass confirmation
			otherwise resume the job*/
			if(sPaperInf.hSFBPaperVolume == PAPER_NORMAL)
			{
				cmnPrintf_ioMN(8,"Usr Confirmation has already been done, Restarting the job. %X\n", iErrSys);
				return OK;
			}
			else
			{
				parMNSetJSByPassConfirmStatus(hjobid, FALSE);
			}
		}
	}
	else if(iErrSys == (M_f220PrnEXErr | IOMSG_ERR_TAB))
	{
		parMNJSTabConfirmStatus(hjobid, &hUsrConfirmSts);
		if(hUsrConfirmSts == TRUE)
		{
			short int hMedia, hPaperType = COM_PT_THICK1;
			cmnPrintf_ioMN(8,"Usr Confirmation has already been done, Restarting the job, %X\n", iErrSys);
			
			parMNNV08ParamI(TABPAPER_ATTR,NVM_NO_SCODE,(VOID *)&hMedia);
			switch(hMedia)
			{
#ifdef	BP_ENG
				case 4: 			
					hPaperType = COM_PT_THIN1;
					cmnPrintf_ioMN(8, "ioMN SetMediaType = Plain1(Tab)\n");
					break;
				case 5: 			
					hPaperType  = COM_PT_THIN2;
					cmnPrintf_ioMN(8, "ioMN SetMediaType = Plain2(Tab)\n");
					break;
#else
				case 0: 			  
					hPaperType  = COM_PT_THIN;
					cmnPrintf_ioMN(8, "ioMN SetMediaType = Plain(Tab)\n");
					break;
#endif
				case 1: 			
					hPaperType  = COM_PT_NORMAL;
					cmnPrintf_ioMN(8, "ioMN SetMediaType = Normal(Tab)\n");
					break;
				case 2: 			
					hPaperType  = COM_PT_THICK1;
					cmnPrintf_ioMN(8, "ioMN SetMediaType = Thick 1(Tab)\n");
					break;
				case 3: 			 
					hPaperType  = COM_PT_THICK2;
					cmnPrintf_ioMN(8, "ioMN SetMediaType = Thick 2(Tab)\n");
					break;
				default:
					hPaperType  = COM_PT_THICK1;
					cmnPrintf_ioMN(8, "ioMN SetMediaType = Thick 1(Tab)\n");
					break;
			}
			mcnTBLSetPaperType(COM_PS_SFB,hPaperType);
			return OK;
		}
	}
	else if(iErrSys == (M_f220PrnEXErr | IOMSG_ERR_INS))
	{
		parMNJSInsConfirmStatus(hjobid, &hUsrConfirmSts);
		if(hUsrConfirmSts == TRUE)
		{
#if defined(WEISS_ENG)||defined (AL_ENG)||defined (LOIRE_ENG) || defined (S2_ENG)
			/*STFR_11661: If no paper on inserter tray then no need to auto resume
			  Send the E_spntaneousSuspend*/
			if(sPaperInf.hINSPaperVolume == PAPER_NORMAL)
			{
				cmnPrintf_ioMN(8,"Usr Confirmation has already been done, Restarting the job, %X\n", iErrSys);
				return OK;
			}
			else
			{
				parMNSetJSInsConfirmStatus(hjobid,FALSE);
			}
#else
			cmnPrintf_ioMN(8,"Usr Confirmation has already been done, Restarting the job, %X\n", iErrSys);
			return OK;

#endif			
		}
	}
/* V036.00_01 END   */
    /*  メッセージＩＤ取得  */
#if 0   /* MCK1.1 新メッセージ対応  */
/**/    if(ioMNCjst[hjobid].iOldMsgId == IOMSG_CJST_UI){
/**/        imsgid = IOMSG_TJED_UI;
/**/    }
/**/    else{
/**/        imsgid = IOMSG_TJED_JOB;
/**/    }
#else
    imsgid = JOB_MSG_TJSX;
#endif

    /*  ステータスによって処理を変更    */
    switch(hSts){
        case    IOMSG_SUS_OK:
                /*  リカバリコマンド送信    */
                if(ERROR == (Status = iomnMcnRecover(hjobid, hRecmode, MCN_RCV_RSC_PRT, IOMN_MCN_REC_TJED, 0))){
                    iomnDebugMessage(">>>Error:iomnMcnRecover() in iomnPrnTJED()\n",0);
                    return(ERROR);
                }
                break;
        case    IOMSG_SUS_ERROR:
                /*  エンジン系エラー発生時  */
                if(iErrEng != IOMSG_NOT_USED 
                    && (ioMNstatus.hError_sts & IOMN_MREC_PRT) != 0){
                    /*  JOB終了メッセージ送信   */
                    if(ERROR == (Status = iomnSendTJED(ioMNCjst[hjobid].OldmsgQId, imsgid, 
                                                        hjobid, hSts, iErrSys, iErrEng, hOrgBack))){
                        iomnDebugMessage(">>>Error:iomnSendTJED()\n",0);
                        return(ERROR);
                    }
#if 0   /* MCK1.2 UI変更処理は不要  */
/**/                    /*  印刷ＵＩ再表示予約フラグセット  */
/**/                    ioMNstatus.hPrintUI = IOMN_ON;
#endif
                }
                /*  エンジン系エラーが発生していない場合    */
                else if(iErrEng != IOMSG_NOT_USED
                    && (ioMNstatus.hError_sts & IOMN_MREC_PRT) == 0){
                    /*  マシンリカバリ処理  */
                    switch( iSuscause ) {
                        case (M_f220EngErr|(UH16)ENG_ERR_PAPER_EMPTY):      /* 用紙無し */
                        case (M_f220PrnEXErr|(UH16)IOMSG_ERR_BYPASS_EMPTY): /* 手差し用紙無し */
                        case (M_f220EngErr|(UH16)ENG_ERR_INNER_FULL):       /* 胴内フル */
                        case (M_f220EngErr|(UH16)ENG_ERR_FINISHER_TRAY):    /* フィニッシャフル */
			   case (M_f220EngErr|(UH16)ENG_ERR_FINISHER_TRAY2):							
                        case (M_f220EngErr|(UH16)ENG_ERR_TONER_SUPPLY):     /* トナー補給中 */
                        /* V034.04_01 START *//* UIロック回避のため、リカバリ指示を行わない要因を追加 */
                        case (M_f220EngErr|(UH16)ENG_ERR_STAPLE):               /* ステイプル針無し */
                        case (M_f220EngErr|(UH16)ENG_ERR_SDL_STPL_EMPTY):       /* サドル針無し */
                        case (M_f220EngErr|(UH16)ENG_ERR_SADDLE_FULL):          /* サドルフル */
                        case (M_f220EngErr|(UH16)ENG_ERR_PUNCH_FULL):           /* パンチフル */
                        case (M_f220EngErr|(UH16)ENG_ERR_TONER_EMPTY):          /* トナーエンプティ */
                        case (M_f220EngErr|(UH16)ENG_ERR_HEATER_TEMP):          /* IH温度制御中 */
                        case (M_f220EngErr|(UH16)ENG_ERR_PAPER_CAS):            /* カセット無し */
                        case (M_f220EngErr|(UH16)ENG_ERR_COLORTONER_EMPTY):     /* カラートナーエンプティ */
                        case (M_f220EngErr|(UH16)ENG_ERR_MTONER_EMPTY):     /* Ashwin Changed for New TNR Empty Error Code*/
                        case (M_f220EngErr|(UH16)ENG_ERR_YTONER_EMPTY):     /* Ashwin Changed for New TNR Empty Error Code*/
                        case (M_f220EngErr|(UH16)ENG_ERR_DIFFERENT_PAPER_SIZE): /* 手差しガイド幅エラー中断 *//* eB3-07 */
                        /* V034.04_01 START *//* UIロック回避のため、リカバリ指示を行わない要因を追加 */
                            /* 用紙無しなどの場合はマシンリカバリを行なわずTJEDを送信する */
                            if( iomnSendTJED(ioMNCjst[hjobid].OldmsgQId,imsgid,hjobid,
                                             hSts,iErrSys,iErrEng,hOrgBack) == ERROR ) {
                                #ifdef DEBUG
                                cmnPrintf_ioMN(2, "[ioMN]Error:iomnSendTJED() in iomnPrnTJED()\n" );
                                #endif
                                return(ERROR);
                            }
                            break;

                        default:
                            /* 上記以外のエラー要因の場合はマシンリカバリを行なう */
                            if( iomnMcnRecover(hjobid,hRecmode,MCN_RCV_RSC_PRT,IOMN_MCN_REC_TJED,0)
                                == ERROR ) {
                                #ifdef DEBUG
                                cmnPrintf_ioMN(2, "[ioMN]Error:iomnMcnRecover() in iomnPrnTJED()\n" );
                                #endif
                                return(ERROR);
                            }
                            break;
                    }
                }
                else if(iErrEng == IOMSG_NOT_USED 
                        && iErrSys != IOMSG_NOT_USED){
                    /* V034.04_01 START *//* UIロック回避のため、リカバリ指示を行わない要因を追加 */
                    if(iErrSys == (M_f220PrnEXErr|(UH16)IOMSG_ERR_PAPER_UNMACH) ||      /* 用紙サイズとカセットの不一致 */
                       iErrSys == (M_f220PrnEXErr|(UH16)IOMSG_ERR_NO_DIFDIR_CST)||      /* 縦横交互印刷時、交互方向カセット無し */
                       iErrSys == (M_f220PrnEXErr|(UH16)IOMSG_ERR_PAPERTYPE_UNMATCH)||  /* 用紙タイプとカセットの不一致 */
                       iErrSys == (M_f220PrnEXErr|(UH16)IOMSG_ERR_INEXECUTABLE_SIZE)||  /* 実行不可サイズ */
                       iErrSys == (M_f220PrnEXErr|(UH16)IOMSG_ERR_INEXECUTABLE_TYPE)||  /* 実行不可タイプ */
                       iErrSys == (M_f220PrnEXErr|(UH16)IOMSG_ERR_BYPASS)||             /* 手差し確認中断 */
                       iErrSys == (M_f220PrnEXErr|(UH16)IOMSG_ERR_BYPASS_EMPTY)||       /* 手差し用紙無し */
                       iErrSys == (M_f220PrnEXErr|(UH16)IOMSG_ERR_TONERCOVER_OPEN)||    /* Beta3 *//* ADD *//* VTK10.000 Change */
                       (iErrSys & 0x0000ffff) == IOMSG_ERR_JAM_MANUALSTART ){                               /* VTK10.000 ADD    */
                        if( iomnSendTJED(ioMNCjst[hjobid].OldmsgQId,imsgid,hjobid,
                                         hSts,iErrSys,iErrEng,hOrgBack) == ERROR ) {
                            #ifdef DEBUG
                            cmnPrintf_ioMN(2, "[ioMN]Error:iomnSendTJED() in iomnPrnTJED()\n" );
                            #endif
                            return(ERROR);
                        }
                    }else{
                    /* V034.04_01 END   *//* UIロック回避のため、リカバリ指示を行わない要因を追加 */
                        /*  Printer Recover 処理*/
                        if(ERROR == (Status = iomnMcnRecover(hjobid, hRecmode,
                                                             MCN_RCV_RSC_PRT, IOMN_MCN_REC_TJED, 0))){
                            iomnDebugMessage(">>>Error:McnRecover()in iomnSendTJED()\n",0);
                            return(ERROR);
                        }
                    /* V034.04_01 START *//* UIロック回避のため、リカバリ指示を行わない要因を追加 */
                    }
                    /* V034.04_01 END   *//* UIロック回避のため、リカバリ指示を行わない要因を追加 */
                }
                else{
                    /*  Printer Recover 処理*/
                    if(ERROR == (Status = iomnMcnRecover(hjobid, hRecmode,
                                                         MCN_RCV_RSC_PRT, IOMN_MCN_REC_TJED, 0))){
                        iomnDebugMessage(">>>Error:McnRecover()in iomnSendTJED()\n",0);
                        return(ERROR);
                    }
                }
                break;

        case    IOMSG_SUS_FAX_MEMFUL:
                /*  GDIの場合はERROR    */
                iomnDebugMessage(">>>Error:iomnPrnTJED()\n",0);
                break;
        default:
            iomnDebugMessage(">>>Undefined Status!\n",0);
            return(ERROR);
    }

    return(OK);
}


/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         7.6
TITLE:      JOB完了指示受信処理
MODULE：    STATUS  iomnJobFinCommand()

INCLUDE:    ioMNPar.h
            ioMNstr.h
PARAMETER:
RETURNS:    I32 4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            JOB完了メッセージを受信し、メッセージ内容を保存し、JOB完了メッセージを
            送信する。
NOTES:
HISTORY:
        1.1     Y.Matsuda   96/9/18
        D1.2.4  T.Fujii     00/04/03 DM45/35 サブジョブ中断・再開処理修正
*/
STATUS  iomnJobFinCommand()
{
    STATUS  Status;     /*  各種処理結果    */
    HI      hJobstatus; /*  JOB状態 */
    I32     iSuscause;  /*  中断要因    */
    HI      hJobid;
    iomnDebugMessage(">>>Top of iomnJobFinCommand()<<<\n",0);
    
    /*  パラメータ取得  */
    hJobid = ioMNrmyparam.sCjfnCtl.hJobid;
    iomnDebugMessage("### JOBID        -->%x\n",(I32)hJobid);
    /* V018.00_01 START */
    iomnDebugMessage("### hJcbID       -->%x\n",(I32)ioMNrmyparam.sCjfnCtl.hJcbID);
    iomnDebugMessage("### hTNo         -->%x\n",(I32)ioMNrmyparam.sCjfnCtl.hTNo);
    iomnDebugMessage("### hFinMode     -->%x\n",(I32)ioMNrmyparam.sCjfnCtl.hFinMode);
    iomnDebugMessage("### hAppliType   -->%x\n",(I32)ioMNrmyparam.sCjfnCtl.hAppliType);
    iomnDebugMessage("### hProcessType -->%x\n",(I32)ioMNrmyparam.sCjfnCtl.hProcessType);
    /* V018.00_01 END   */

    /*  メッセージ格納  */
    iomnSetJobManagementMsg(&ioMNCjfn[hJobid]);

    /*  JOB状態取得 */
    if(ERROR == (Status = jobStatus(hJobid, &hJobstatus, &iSuscause))){
        iomnDebugMessage(">>>Error:jobStatus()\n",0);
        return(ERROR);
    }

    /*  JOB完了済みか否かを確認 */
    /*  既にJOB完了ステータス送信済みの場合 */
    if(ioMNCjst[hJobid].OldmsgQId == 0) return(OK);

    /*  メッセージ送信  */
    if(ERROR == (Status = iomnSendJobFinishCmd(msgQIdIoExMN, hJobid))){
        iomnDebugMessage(">>>Error:iomnSendJobFinishCmd()\n",0);
        return(ERROR);
    }
    /*  JOB状態変更 */
    if(ERROR == (Status = jobSetStatus(hJobid, COM_JS_RUNNING, 0))){
        iomnDebugMessage(">>>Error:jobSetStatus()\n",0);
        return(ERROR);
    }
    iomnDebugMessage(">>>JOB STATUS -> COM_JS_RUNNING\n",0);
    iomnDebugMessage(">>>Bottom of iomnJobFinCommand()<<<\n",0);
    return(OK);
}


/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         7.7
TITLE:      JOB完了ステータス受信処理
MODULE：    STATUS  iomnJobFinSts()
INCLUDE:    ioMNPar.h
PARAMETER:
            N/A
RETURNS:    I32 4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            JOB完了ステータス受信時の処理。
NOTES:
HISTORY:
        1.1     Y.Matsuda   96/9/18
        M1.1(TJ)H.Takahashi 98/09/28
                DSS逐次対応
        M1.2(TJ)H.Takahashi 98/11/04
                割り込み複写対応
        M4.2(TJ)H.Takahashi 99/02/05
                cmnFanControll()対応
        D1.1    T.Fujii     99/11/15    DM45/35 後追い逐次対応
        D1.2    T.Fujii     99/12/24    DM45/35 (後追い)逐次印刷・並行動作対応
        D1.4    Y.Shimada   00/ 1/25    DM45/35 フォームストレージ変更：ＩｐＬｉｂ／逐次制御の組み込み
        D1.7.1    J.Ootani  00/03/01    DM45/35 SEINE FAX F-CODE対応(マージ)
        D1.7.8.1  J.Ootani  00/03/31    DM45/35 終了処理不具合対応
        D2.4.3  M.Fukuda    00/06/19    DM45/35 V004.70不具合対応
                                                FAXリストレポート不具合対応
        D2.9      M.Fukuda  00/07/04    DM45/35 FAXリストレポート対応
        D2.0.2    T.Fujii   00/07/04    DM45/35 NOP_DEL(ソースコード整理)
                                                if文,case文でのCOM_JB_EXPPC1を削除
        D3.14     Y.Shimada 00/09/04    DM45/35 Log強化対応

        MCK1.1      M.Karasawa 02/06/11 McKinley    新メッセージ対応
        VTR18.000_1 M.Karasawa 05/09/01 RioGrande   分析くん8658「割り込みﾎﾞﾀﾝを押すと点滅のままﾓｰﾀーが回りっぱなしになる」対応
*/
STATUS  iomnJobFinSts()
{
    HI      hJobID;             /*  JOBID   */
    HI      hJobFinSts;         /*  JOB完了処理の結果   */
    HI      hJobtype;           /*  JOB種別 */
    HI      hTjfnststype;       /*  完了タイプ  */
    I32     iErrApl;            /*  アプリ系終了要因    */
    I32     iErrEngine;         /*  エンジン系終了要因  */
    STATUS  Status;             /*  JOBタイプ取得結果   */
    HI      hSubJobmode;

    HI      hTNo;       /* MCK  */
    HI      hProcessNo; /* MCK  */

    HI      hChangeUIType;  /*  優先度変更要因      *//* PRINT_JOINT *//* 連結中断 */
    HI      hChangeUImode;  /*  優先度変更タイプ    *//* PRINT_JOINT *//* 連結中断 */
    HI      hReturnUI;      /*  復帰先UI            *//* PRINT_JOINT *//* 連結中断 */
    PAR_TBL_RESOLUTION  hPrnResolution; /* 前印刷JOB解像度        *//* PIRNT_JOINT *//* JOBEND */
    STATUS  iConfirm;                   /* JOBEND開始可不確認結果 *//* PIRNT_JOINT *//* JOBEND */

    /* D1.1 T.Fujii 991201 */
    #ifdef IOMN_DM_DEBUG_991201
    cmnPrintf_ioMN(0, ">> ioMN >> START:iomnJobFinSts()\n" );
    #endif /* D1.1 991201 */

    /*  受信パラメータ取得  */
#if 0   /* MCK1.1 新メッセージ対応  */
/**/    hJobID = ioMNrmyparam.sTjfnCtl.hJobid;
/**/    hJobFinSts = ioMNrmyparam.sTjfnCtl.hSts;
/**/    iErrApl = ioMNrmyparam.sTjfnCtl.iErrSys;
/**/    iErrEngine = ioMNrmyparam.sTjfnCtl.iErrEng;
/**/
/**/    iomnDebugMessage("*** JOBID  ---> %x ***\n",(I32)hJobID);
/**/    iomnDebugMessage("*** Status ---> %x ***\n",(I32)hJobFinSts);
/**/    iomnDebugMessage("*** Error(SYS)---> %x ***\n",iErrApl);
/**/    iomnDebugMessage("*** Error(ENG)---> %x ***\n",iErrEngine);
#else
    hJobID      = ioMNrmyparam.sTjfnCtl.hJobid;
    hJobFinSts  = ioMNrmyparam.sTjfnCtl.hSts;
    iErrApl     = ioMNrmyparam.sTjfnCtl.iErrSys;
    iErrEngine  = ioMNrmyparam.sTjfnCtl.iErrEng;
    /* hJcbID がはいる  */
    hTNo        = ioMNrmyparam.sTjfnCtl.hTNo;
    hProcessNo  = ioMNrmyparam.sTjfnCtl.hProcessType;
    /* hTNodel がはいる */
    /* hUkedel がはいる */

	cmnPrintf_ioMN(7,"###JOBID-> %x, Status-> %x, ErrSys-> %x, ErrEng-> %x, TNo-> %x, ProcessNo-> %x", (I32)hJobID, \
						 (I32)hJobFinSts, (I32)iErrApl, (I32)iErrEngine, (I32)hTNo, (I32)hProcessNo);
#endif
/* EBX_DTFR_19548 starts */
    if(ioMNstatus.hUnblockCJST_JobId == hJobID)
    {
        cmnPrintf_ioMN(7, "iomnJobFinSts(): resetting hUnblockCJST_JobId for JobId %d\n", hJobID);
        ioMNstatus.hUnblockCJST_JobId = IOMN_OFF;
    }
/* EBX_DTFR_19548 ends */
    /* PRINT_JOINT *//* START */
    if((hJobFinSts == IOMSG_FIN_RETURN) || (hJobFinSts == IOMSG_FIN_SUS)){
#if 0 /* VTR18.000_1 */
/**//* PRINT_JOINT *//* 連結割り込み *//* ↓↓↓↓↓ */
/**/        if(ioMNstatus.hPPCIntReserve == IOMN_ON){
/**/            ioMNPJ_IntSet(hJobID, IOMN_PJ_OFF);
/**/            if(ioMNPJ_IntReserveCheck() == ERROR ){
/**/                if( ioMNstatus.hError_sts == 0 ) {
/**/                    /*  割り込み複写ＵＩ変更処理    */
/**/                    if(ERROR == (Status = iomnIntPPCCheck(hJobID, hJobtype))){
/**/                        iomnCleanJobManagementMsg(hJobID);  /* V015.00_02   */
/**/                        puts( "[ioMN]Error:iomnIntPPCCheck()" );
/**/                        return(ERROR);
/**/                    }
/**/                }
/**/                else {
/**/                    /* 復帰先ＵＩ取得変更 */
/**/                    if( iomnGetChangeUIQue(&ioMNUiErrClrBuf,&hChangeUIType,&hReturnUI,&hChangeUImode)
/**/                        != ERROR ) {
/**/                        iomnSetChangeUIQue(&ioMNUiErrClrBuf,IOMN_CPRI_INT_COPY,UI_INTPPC,IOMN_CPRI_INT_COPY);
/**/                        cmnPrintf_ioMN(0,"[ioMN]CHANGE:Return UI %x -> UI_INTPPC\n",hReturnUI);
/**/                   }
/**/                }
/**/            }
/**/        }
/**//* PRINT_JOINT *//* 連結割り込み *//* ↑↑↑↑↑ */
#endif /* VTR18.000_1 */
/* PIRNT_JOINT *//* JOBEND *//* START */
        /*  JOBEND開始可不確認処理 */
        /* 処理完了したJOBの解像度取得 */
		memset(&ioMNsmyparam, 0, sizeof(ioMNsmyparam));
		ioMNsmyparam.sEjfrCtl.hJobId = hJobID;
        if(ERROR == (Status = iomnSendMessage(ioMNCjst[hJobID].OldmsgQId,
                                  JOB_MSG_EJFR, 0, &ioMNsmyparam))){
            iomnDebugMessage(">> ioMN >> Error:iomnSendMessage() in iomnJobFinSts() - TJFN with IOMSG_FIN_RETURN \n",0);
            return(ERROR);
        }
		memset(&ioMNsmyparam, 0, sizeof(ioMNsmyparam));
        iomnDebugMessage(">>>OK:iomnSendMessage()\n",0);
        if(ERROR == (Status = parMNPrnResolution(hJobID, &hPrnResolution)))
        {
             iomnDebugMessage(">>>Error:parMNPrnResolution()\n",0);
             return(ERROR);
        }
        if(ERROR == (Status = jobJobtype(hJobID, &hJobtype))){
            cmnPrintf_ioMN(2,">>>Error:jobJobtype() in JobFinSts()\n");
            /* 処理は継続 */
        }
        /* 処理完了したJOB種別、解像度の退避 */
        ioMNstatus.hEC22SendInfo.hJobmode = hJobtype;
        memcpy(&ioMNstatus.hEC22SendInfo.hprnResolution,&hPrnResolution ,sizeof(PAR_TBL_RESOLUTION));
        if ((iConfirm = iomnConfirmJobEnd(hJobtype, hPrnResolution)) == ERROR ){
            iomnDebugMessage(">>>ERROR:iomnConfirmJobEnd() in iomnDssPrnJobFinSts()\n",0);
            return(ERROR);
        }
        if ( iConfirm == IOMN_ON ){
                /*  JOBEND開始可の時 */
#if 0   /* PRINT_JOINT *//* JOBEND */
/* PRINT_JOINT */            if( (ioMNPJ_BackIDExist() == ERROR ) ||
/* PRINT_JOINT */                (ioMNPJ_BackIDCheck(hJobID) == OK) ){
#else
            if( iomnPJ_JobendCheck(hJobID) == OK){
#endif
                /* BG印刷ジョブIDが未登録またはID一致の場合は処理する */
                /* EC22をmcnMNへ送信し、JOBEND処理を行ってもらう */
                Status = iomnSendEC22();
                if ( Status == ERROR ){
                    cmnPrintf_ioMN(2,">>>Error:iomnSendEC22()-iomnDssPrnJobFinSts()\n");
                    iomnAddErrLog( "iomnSendEC22()-iomnDssPrnJobFinSts()", errno );
                    return ERROR;
                }
            }
            /* TJFN受信時のBG印刷JOBキューイング有無フラグ OFF */
            ioMNstatus.hEC22SendInfo.hNextBgJobFlg = IOMN_OFF;
        }
        else if ( iConfirm == IOMN_OFF ){
            /*  JOBEND開始不可の時 */
            /* TJFN受信時のBG印刷JOBキューイング有無フラグ ON */
#if 0   /* PRINT_JOINT *//* JOBEND */
/* PRINT_JOINT */            if(ioMNPJ_BackIDExist() == ERROR){
#else
        if( iomnPJ_JobendCheck(hJobID) == OK){
#endif
                ioMNstatus.hEC22SendInfo.hNextBgJobFlg = IOMN_ON;
            }
            iomnDebugMessage("****hNextBgJobFlg ON!!!\n", 0);
        }
/* PIRNT_JOINT *//* JOBEND *//* END   */
#if 0 /* VTR18.000_1 */
/**/        return(ioMNPJ_TJFNError(hJobID));
#else /* VTR18.000_1 */
        if( ioMNPJ_TJFNError(hJobID) == ERROR ){
            return( ERROR );
        }
/* L3.3: 12446 FR - 20120517 */
		if((hJobFinSts == IOMSG_FIN_SUS) && (ioMNCjsu[hJobID].iOldTaskId != 0))
		{
/* commented for observation made during testing STFR_10886 -- 20111208 */
#if 0
			jobSetStatus( hJobID, COM_JS_SUSPEND, IOMSG_SUS_RETURN);
#endif
#if 0 /*changes needed for WEISS also -- roopesh*/
			if(ERROR == (Status = iomnSendTjsu(JOB_MSG_TJSU, hJobID, IOMSG_SUS_OK, IOMSG_NOT_USED, IOMSG_NOT_USED, 0)))
#else			
			if(ERROR == (Status = iomnSendTjsu(JOB_MSG_TJSU, hJobID, IOMSG_SUS_ERROR, (I32)(M_f220IoMNErr | IOMSG_JB_NOT_RUN), IOMSG_NOT_USED, IOMSG_NOT_USED)))
#endif			
			{
				cmnPrintf_ioMN(2,">>>Error:iomnSendTjsu()\n");
				return(ERROR);
			}
		}
		else
		{
			cmnPrintf_ioMN(7,"prnEX has sent False Status, Correcting the status\n");
			hJobFinSts = IOMSG_FIN_RETURN;
		}
        if(ioMNstatus.hPPCIntReserve == IOMN_ON){
            /* 直前でクリアしているが一応クリアを行なう */
            ioMNPJ_IntSet(hJobID, IOMN_PJ_OFF);
            if(ioMNPJ_IntReserveCheck() == ERROR ){
                if( ioMNstatus.hError_sts == 0 ) {
                    /*  割り込み複写ＵＩ変更処理    */
                    if(ERROR == (Status = iomnIntPPCCheck(hJobID, hJobtype))){
                        iomnCleanJobManagementMsg(hJobID);  /* V015.00_02   */
                        puts( "[ioMN]Error:iomnIntPPCCheck()" );
                        return( ERROR );
                    }
                }
                else {
                    /* 復帰先ＵＩ取得変更 */
                    if( iomnGetChangeUIQue(&ioMNUiErrClrBuf,&hChangeUIType,&hReturnUI,&hChangeUImode) != ERROR ) {
                        iomnSetChangeUIQue(&ioMNUiErrClrBuf,IOMN_CPRI_INT_COPY,UI_INTPPC,IOMN_CPRI_INT_COPY);
                        cmnPrintf_ioMN(0,"[ioMN]CHANGE:Return UI %x -> UI_INTPPC\n",hReturnUI);
                   }
                }
            }
        }
        return( OK );
#endif /* VTR18.000_1 */
    }
    if(ERROR == ioMNPJ_EPSTClear(hJobID)){                                              /* PRINT_JOINT */
        iomnDebugMessage("[ioMN] ERROR : ioMNPJ_EPSTClear() in iomnJobFinSts()\n",0);   /* PRINT_JOINT */
    }                                                                                   /* PRINT_JOINT */

    /* PRINT_JOINT *//* END   */
    /*  JOB完了タイプ取得   */
    hTjfnststype = hJobFinSts;
    if(iErrApl != IOMSG_NOT_USED)       hTjfnststype |= IOMN_APLERR;
    if(iErrEngine != IOMSG_NOT_USED)    hTjfnststype |= IOMN_ENGERR;

    iomnDebugMessage("*** JobFinishType ---> %x ***\n",(I32)hTjfnststype);

    /*  JOB完了パラメータの格納 */
    iomnSetJobManagementMsg(&ioMNTjfn[hJobID]);

    /*  JOB種別取得 */
    if(ERROR == (Status = jobJobtype(hJobID, &hJobtype))){
        cmnPrintf_ioMN(2,">>>Error:jobJobtype() in JobFinSts()\n");
        printErrno(errno);
        return(ERROR);
    }
    iomnDebugMessage("###JOB Type ---> %x\n",(I32)hJobtype);

    /* D3.14 START Y.Shimada ADD */
    iomnDebugTrace( IOMN_OFF, hJobID, hJobtype);
    /* D3.14 END */

    /* サブジョブ種別をここで取得する */
    if( parMNSubJobmode(hJobID, &hSubJobmode) == ERROR ) {
        cmnPrintf_ioMN(2,">> ioMN >> Error:parMNSubJobmode() in JobFinSts()\n");
        return( ERROR );
    }
    #ifdef IOMN_DM_DEBUG_991201     /* D1.1 T.Fujii 991201 */
    cmnPrintf_ioMN(0, ">> ioMN >> GET:parMNSubJobmode(hJobID=%x)=%x\n", hJobID, hSubJobmode );
    #endif  /* D1.1 991201 */

    /*  JOB中断処理中である場合、中断処理テーブルクリア */
    /*  JOB種別毎　処理 */
    switch(hJobtype){
        case    COM_JB_IPPC:
        case    COM_JB_EXPPC2:
                /* サブジョブ種別をチェックして処理を分岐 */
                if( hSubJobmode == COM_JB_SUB_INPUT ) { /* サブジョブ種別が入力のとき */
                    if(ERROR == (Status = iomnExPPCJobFinSts(hJobID, hJobFinSts, iErrApl, 
#if 0   /* MCK1.1 新メッセージ対応  引数追加    */
                                                            iErrEngine, hTjfnststype, hJobtype))){
#else
                                                            iErrEngine, hTjfnststype, hJobtype, hProcessNo))){
#endif
                        iomnDebugMessage(">>>Error:iomnExPPCJobFinSts()\n",0);
                        return(ERROR);
                    }
/*Clearing the CJSU flag when it has received before AJST and AJST has not been received.*/
#if 0 /* EBX_STFR_18005 - Shalini - L6.011: Setting the hReserveFrontSuspend to OFF in case of INPUT agent */
					 if(iomnCheckSCS(hJobID)==TRUE) /* 20111010 - STFR_11030: for L3.0 as jobid for sync job can be 0 in shadow wf case */
				 	{
						 ioMNstatus.hReserveBackSuspend = IOMN_OFF; /* 20111010 - STFR_11030 */
						 cmnPrintf_ioMN(7,"iomnJobFinSts(): IPPC or EXPPC2 scan part, ioMNstatus.hReserveBackSuspend is cleared\n");
				 	}
					else
					{
#endif
					ioMNstatus.hReserveFrontSuspend = IOMN_OFF; /* 20111010 - STFR_11030 */
					cmnPrintf_ioMN(7,"iomnJobFinSts(): IPPC or EXPPC2 scan part, ioMNstatus.hReserveFrontSuspend is cleared\n");
 /*               } */ /* EBX_STFR_18005 */
                }
                else {  /* サブジョブ種別が出力のとき */
                    iomnPPCPrnEnd( hJobID );    /* コピージョブ履歴の更新 */
                    if(ERROR == (Status = iomnDssPrnJobFinSts(hJobID, hJobFinSts, 
#if 0   /* MCK1.1 新メッセージ対応  引数追加    */
                                                    iErrApl, iErrEngine, hTjfnststype, hJobtype))){
#else
                                                    iErrApl, iErrEngine, hTjfnststype, hJobtype, hProcessNo))){
#endif
                        iomnDebugMessage(">>>Error:iomnPrnJobFinSts()\n",0);
                        return(ERROR);
                    }
					ioMNstatus.hReserveBackSuspend = IOMN_OFF; /* 20111010 - STFR_11030 */
					cmnPrintf_ioMN(7,"iomnJobFinSts(): IPPC or EXPPC2 print part, ioMNstatus.hReserveBackSuspend is cleared\n");
                }
                break;
        case    COM_JB_SCN_FAX1:
        case    COM_JB_SCN_FAX2:
        case    COM_JB_POLL_SEND_FAX1:
        case    COM_JB_POLL_SEND_FAX2:
        case    COM_JB_IVSAVE_PPC:
        case    COM_JB_IVSAVE_FAX:
        case    COM_JB_DRC_SCN_FAX:
        case    COM_JB_SEND_DSI:
        case    COM_JB_CON_REG_FAX2:
        case    COM_JB_BOD_REG_FAX2:
                if(ERROR == (Status = iomnScnJobFinSts(hJobID, hJobFinSts, 
#if 0   /* MCK1.1 新メッセージ対応  引数追加    */
                                                    iErrApl, iErrEngine, hTjfnststype, hJobtype))){
#else
                                                    iErrApl, iErrEngine, hTjfnststype, hJobtype, hProcessNo))){
#endif
                    iomnDebugMessage(">>>Error:iomnScnJobFinSts()\n",0);
                    return(ERROR);
                }
				ioMNstatus.hReserveFrontSuspend = IOMN_OFF; /* 20111010 - STFR_11030 */
				cmnPrintf_ioMN(7,"iomnJobFinSts(): COM_JB_SEND_DSI etc, ioMNstatus.hReserveFrontSuspend is cleared\n");

                break;
        case    COM_JB_DRC_PRT_FAX:
        case    COM_JB_POLL_RECV_FAX1:
        case    COM_JB_POLL_RECV_FAX2:
        case    COM_JB_PRT_FAX1:
        case    COM_JB_PRT_FAX2:
        case    COM_JB_PRT_GDI1:
        case    COM_JB_PRT_GDI2:
        case    COM_JB_DRC_PRT_GDI:
                if(ERROR == (Status = iomnDssPrnJobFinSts(hJobID, hJobFinSts,
#if 0   /* MCK1.1 新メッセージ対応  引数追加    */
                              iErrApl, iErrEngine, hTjfnststype, hJobtype))){
#else
                              iErrApl, iErrEngine, hTjfnststype, hJobtype, hProcessNo))){
#endif
                    cmnPrintf_ioMN(2,">>>Error:iomnPrnJobFinSts()\n");
                    return(ERROR);
                }
				ioMNstatus.hReserveBackSuspend = IOMN_OFF; /* 20111010 - STFR_11030 */
				cmnPrintf_ioMN(7,"iomnJobFinSts(): COM_JB_PRT_FAX1 or COM_JB_PRT_FAX2 etc, ioMNstatus.hReserveBackSuspend is cleared\n");
                break;
        case    COM_JB_RECV_DSI:
        case    COM_JB_PRIVATE_DSI:
        case    COM_JB_RECV_DSI_SYNC:
        case    COM_JB_PRIVATE_DSI_SYNC:
                /* サブジョブ種別をチェックして処理を分岐 */
                if( hSubJobmode == COM_JB_SUB_INPUT ) { /* サブジョブ種別が入力のとき */
                    if( iomnDataRcvJobFinSts(hJobID, hJobFinSts, iErrApl, iErrEngine, hTjfnststype)
                        == ERROR ) {
                        iomnDebugMessage(">> ioMN >> Error:iomnDataRcvJobFinSts()\n",0);
                        return(ERROR);
                    }
                }
                else {  /* サブジョブ種別が出力のとき */
#if 0 /*Multiple job execution*/
			/*Paresh: Added to Reset the DssDataTrans Flag for next Job use*/
		    ioMNstatus.hDssDataTrans = IOMN_OFF;
#endif
                    if(ERROR == (Status = iomnDssPrnJobFinSts(hJobID, hJobFinSts, 
#if 0   /* MCK1.1 新メッセージ対応  引数追加    */
                                                     iErrApl, iErrEngine, hTjfnststype, hJobtype))){
#else
                                                     iErrApl, iErrEngine, hTjfnststype, hJobtype, hProcessNo))){
#endif
                        iomnDebugMessage(">>>Error:iomnDssPrnJobFinSts()\n",0);
                        return(ERROR);
                    }
				ioMNstatus.hReserveBackSuspend = IOMN_OFF; /* 20111010 - STFR_11030 */
				cmnPrintf_ioMN(7,"iomnJobFinSts(): COM_JB_RECV_DSI etc, ioMNstatus.hReserveBackSuspend is cleared\n");
                }
                break;
        case    COM_JB_LIST_PRT_FAX:
        case    COM_JB_CON_GET_FAX2:
        case    COM_JB_BOD_GET_FAX2:
                /* サブジョブ種別をチェックして処理を分岐 */
                if( hSubJobmode == COM_JB_SUB_INPUT )
                { /* サブジョブ種別が入力のとき */
                    /* FAXリストレポート作成終了 */
                    ioMNstatus.hFaxLstmakeJobStart = IOMN_OFF;
                    if( iomnListMakeJobFinSts(hJobID, hJobFinSts,
                                              iErrApl, iErrEngine,
#if 0   /* MCK1.1 新メッセージ対応  引数追加    */
                                              hTjfnststype, hJobtype) == ERROR )
#else
                                              hTjfnststype, hJobtype, hProcessNo) == ERROR )
#endif
                    {
                        cmnPrintf_ioMN(2,">> ioMN >> Error:iomnListMakeJobFinSts()\n");
                        return(ERROR);
                    }
                }
                else
                {
                    /*  フロント操作によるリスト印刷の場合  */
                    if(hJobID == hioMNFrontJobID){
                        if(ERROR == (Status = iomnFrontPrnJobFinSts(hJobID,
                                                 hJobFinSts, iErrApl,
                                                 iErrEngine, hTjfnststype,
#if 0   /* MCK1.1 新メッセージ対応  引数追加    */
                                                 hJobtype)))
#else
                                                 hJobtype, hProcessNo)))
#endif
                        {
                            cmnPrintf_ioMN(2,">>>Error:iomnFrontPrnJobFinSts()\n");
                            return(ERROR);
                        }
                    }
                    else
                    {
                    /*  バックグランド印刷によるリスト印刷の場合    */
                        if(ERROR == (Status = iomnDssPrnJobFinSts(hJobID,
                                               hJobFinSts,iErrApl,iErrEngine,
#if 0   /* MCK1.1 新メッセージ対応  引数追加    */
                                               hTjfnststype, hJobtype)))
#else
                                               hTjfnststype, hJobtype, hProcessNo)))
#endif
                        {
                            cmnPrintf_ioMN(2,">>>Error:iomnDssPrnJobFinSts()\n");
                            return(ERROR);
                        }
                    }
                }
                break;
        case    COM_JB_LIST_RESULT_SEND:
                /* FAXリストレポート作成終了 */
                ioMNstatus.hFaxLstmakeJobStart = IOMN_OFF;
                if( iomnListMakeJobFinSts(hJobID, hJobFinSts,
                                          iErrApl, iErrEngine,
#if 0   /* MCK1.1 新メッセージ対応  引数追加    */
                                          hTjfnststype, hJobtype) == ERROR )
#else
                                          hTjfnststype, hJobtype, hProcessNo) == ERROR )
#endif
                {
                    cmnPrintf_ioMN(2,">> ioMN >> Error:iomnListMakeJobFinSts()\n");
                    return(ERROR);
                }
                break;
        case    COM_JB_IVPRM_PPC:
                if(ERROR == (Status = iomnDssPrnJobFinSts(hJobID, hJobFinSts, 
#if 0   /* MCK1.1 新メッセージ対応  引数追加    */
                                                    iErrApl, iErrEngine, hTjfnststype, hJobtype))){
#else
                                                    iErrApl, iErrEngine, hTjfnststype, hJobtype, hProcessNo))){
#endif
                    iomnDebugMessage(">>>Error:iomnDssPrnJobFinSts()\n",0);
                    return(ERROR);
                }
                break;
        case    COM_JB_IVPRM_FAX:
                if(ERROR == (Status = iomnFrontPrnJobFinSts(hJobID, hJobFinSts, 
#if 0   /* MCK1.1 新メッセージ対応  引数追加    */
                                                    iErrApl, iErrEngine, hTjfnststype, hJobtype))){
#else
                                                    iErrApl, iErrEngine, hTjfnststype, hJobtype, hProcessNo))){
#endif
                    iomnDebugMessage(">>>Error:iomnFrontPrnJobFinSts()\n",0);
                    return(ERROR);
                }
                break;
        case    COM_JB_LIST_SEND_FAX:
        case    COM_JB_RECV_FAX:
        case    COM_JB_JOB_UI:
        case    COM_JB_DIG:
        case    COM_JB_UNDEF:
        default:
                iomnDebugMessage(">>>Error:Undefined JOB Type!-->%x\n",(I32)hJobtype);
                return(ERROR);
    }

    return(OK);
}


/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         7.7.2
TITLE:      ExPPCJOB完了ステータス処理
MODULE：    STATUS  iomnExPPCJobFinSts(HI hJobID, HI hJobFinSts, I32 iERRApl, I32 iErrEngine, HI hTjfnststype, HI hJobtype)
INCLUDE:    ioMNpar.h
            ioMNstr.h
PARAMETER:
RETURNS:    I32 4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            拡張複写JOB時、JOB完了ステータスを受信しステータス、およびエラー内容に応じて
            マシンリカバリ処理、JOB完了処理を行う。
NOTES:
HISTORY:
        1.1     Y.Matsuda       97/02/06
        M1.1    (TJ)H.Takahashi 98/6/23
                8/E対応 JOB毎のpubJobEntry(),puJobDelete()を削除
        M1.2    (TJ)H.Takahashi 98/11/04
                割り込み複写対応
        M4.10   (TJ)H.Takahashi 99/01/13
                1st.コピー高速化対応
        D1.1    T.Fujii     99/11/15
                DM45/35 後追い逐次対応。入力処理の終了にのみ対応するよう変更
        D1.2.4.1 T.Fujii    00/04/15 DM45/35 印刷中断・再開処理修正
        D2.0.1   T.Fujii    00/06/06 DM45/35 NOP_DEL(ソースコード整理)
                                             puJobDelete()関数の呼び出しを完全に削除
        D2.1     T.Fujii    00/06/06 DM45/35 V004.60不具合対応
                                             parMN再設定処理修正
                                             ページ確定メッセージ送信対応
        D2.4    T.Fujii     00/06/15 DM45/35 prnEX自発中断対応
        D2.4.2  M.Fukuda    00/06/19 DM45/35 JOBENDイベント送信処理対応
        D2.10   M.Fukuda    00/07/04 DM45/35 JOBENDイベント送信処理対応
        D2.10.3 M.Fukuda    00/07/14 DM45/35 JOBEND送信処理修正（DFジャム発生対応）
        D3.7    T.Fujii     00/08/10 DM45/35 原稿枚数オーバー時の印刷中止処理対応
        D3.12   T.Fujii     00/09/02 DM45/35 ジョブ中断処理修正
        D3.12.1 T.Fujii     00/09/04 DM45/35 BG印刷ジョブ中断処理修正
        D3.12.2 T.Fujii     00/09/07 DM45/35 BG印刷ジョブ中断処理再修正、ソースコード整理
        D3.13.2 T.Fujii     00/09/07 DM45/35 TDJB送信処理抜け修正
        D3.0.2  T.Fujii     00/09/18 DM45/35 NOP_DEL(ソースコード整理)
                                             ・この関数は必ずEXPPC2/IPPCのためif文不要
        D3.22   T.Fujii     00/09/19 DM45/35 従来逐次の後にhIHOFFフラグを壊す不具合修正
        D3.21.3 T.Fujii     00/09/27 DM45/35 parMNコピー処理再修正
        D3.31   T.Fujii     00/10/10 DM45/35 マシンリカバリ処理修正
        D3.31.3 T.Fujii     00/10/24 DM45/35 逐次コピー時もmcnUIを表示させる
        D3.31.4 T.Fujii     00/10/25 DM45/35 原稿入力中断/終了でmcnUIが表示されない不具合修正
        D4.4    T.Fujii     00/11/07 DM45/35 原稿入力が０ページの場合は印刷ジョブを削除する
        D4.4.1  T.Fujii     00/11/20 DM45/35 印刷ジョブ削除処理修正
        D4.8    T.Fujii     00/11/21 DM45/35 05テストコピーでcpyUIにメッセージ送信する不具合修正
        D5.0    T.Fujii     00/12/12 DM45/35 CDJB多重動作対応

        MCK1.1  M.Karasawa  02/06/11    McKinley    新メッセージ対応
        MCK1.5  M.Karasawa  02/07/26    McKinley    DELETE iomnChangeToMcnUI
    V013.00_04  M.Karasawa  03/02/07    McKinley    ジョブ削除(jobDelete())はioMNでは行わず、上位で行う
    V013.00_05  M.Karasawa  03/02/07    McKinley    ioMNで行う必要のないキャンセル処理を削除
    V016.00_04  M.Karasawa  03/03/31    McKinley    複写ジョブ管理エリアに関する処理を前面削除
*/
#if 0   /* MCK1.1 新メッセージ対応  引数追加    */
/**/STATUS  iomnExPPCJobFinSts(HI hJobID, HI hJobFinSts, I32 iERRApl, I32 iErrEngine, HI hTjfnststype, HI hJobtype)
#else
STATUS  iomnExPPCJobFinSts(HI hJobID, HI hJobFinSts, I32 iERRApl, I32 iErrEngine, HI hTjfnststype, HI hJobtype, HI hProcessNo)
#endif
{
    STATUS      Status;     /*  マシンリカバリ結果  */
    I32         imsgid;     /*  メッセージＩＤ  */
#if 0   /* MCK1.1   */
    HI          hPmJobID = -1;  /* D1.1 T.Fujii 99/11/15 ダミー定義追加 */
    FL_VOLUME_SPACE lVolumespace;   /*  Volumeサイズ構造体  */
#endif
    STATUS      iConfirm;              /*  JOBEND開始可不確認結果 */
    PAR_TBL_RESOLUTION  hPrnResolution;/*  前印刷JOB解像度        */
    I32         iErrEng;    /*  マシン管理より通知されるERROR type  */
    HI          hIOMode     /* 入出力動作モード */;
    /* D4.4 START T.Fujii 原稿入力０ページ対応 */
    HI          hInputPages = 0;
    /* D4.4 END */
    /* D4.4.1 START T.Fujii 原稿入力０ページ対応修正 */
/*    HI          hOutputJobID = -1;    *//* V028.00_00 */
    HI          hSyncOutJobID = -1;
    /* D4.4.1 END */

    /* D1.1 T.Fujii 991201 */
    #ifdef IOMN_DM_DEBUG_991201
    cmnPrintf_ioMN(0, ">> ioMN >> START:iomnExPPCJobFinSts()\n" );
    #endif /* D1.1 */

    /* D3.12.2 START T.Fujii BG印刷ジョブ中断処理再修正、ソースコード整理 */
    /* 入出力動作モードを取得 */
    if( parMNIOMode(hJobID, &hIOMode) == ERROR ) {
        /* 入出力動作モードが取得できなかった場合 */
        cmnPrintf_ioMN(2, "[ioMN]ERROR: parMNIOMode(hJobID=%d) in iomnExPPCJobFinSts()\n", hJobID );
        hIOMode = -1;   /* 無効値を入れておいて処理は継続 */
    }
    /* D3.12.2 END */

    /* D3.12.2 START T.Fujii BG印刷ジョブ中断処理再修正、ソースコード整理 */
    /* 逐次コピーの場合の処理 */
    if( hIOMode != COM_OT_TRK_SCS && hIOMode != COM_OT_TRK_FIN ) {
    /* D3.12.2 END */
        /* 処理完了したJOBの解像度取得 */
        if(ERROR == (Status = parMNPrnResolution(hJobID, &hPrnResolution)))
        {
             iomnDebugMessage(">>>Error:parMNPrnResolution()\n",0);
             return(ERROR);
        }
        iomnDebugMessage("*** hPrnResolution(hMain) ->%hd\n",hPrnResolution.hMain);
        iomnDebugMessage("*** hPrnResolution(hSub) ->%hd\n",hPrnResolution.hSub);
        /* 処理完了したJOB種別、解像度の退避 */
        ioMNstatus.hEC22SendInfo.hJobmode = hJobtype;
/* D3.22 START T.Fujii hIHOFFフラグを壊す不具合修正 */
#if 0   /* OLD */
        memcpy(&ioMNstatus.hEC22SendInfo.hprnResolution,&hPrnResolution ,sizeof(EC22INFO_TBL));
#else   /* D3.22 MID    NEW */
        memcpy(&ioMNstatus.hEC22SendInfo.hprnResolution,&hPrnResolution,sizeof(PAR_TBL_RESOLUTION));
#endif
/* D3.22 END */
    }

/* D3.0.2 START T.Fujii ソースコード整理(この関数は必ずEXPPC2/IPPC) */
#if 0   /* OLD */
    if( hJobtype==COM_JB_EXPPC2 || hJobtype==COM_JB_IPPC ) {
#endif
/* D3.0.2 END */
#if 0   /* MCK1.1 新メッセージ対応  *//* 要検討 */
/**/        /*  HDD空きサイズ取得   */
/**/        if(ERROR == (Status = flVolSpaceGet(iTaskIdIoMN, hPmJobID, "CPC", &lVolumespace))){
/**/            cmnPrintf_ioMN(2,">>>Error:flVolSpaceGet()\n");
/**/            printErrno(errno);
/**/        }
/**/        /*  Memory残量通知パラメータセット  */
/**/        ioMNsmyparam.sEmemCtl.iRemainCapacity = lVolumespace.liVolSpace;
/**/        ioMNsmyparam.sEmemCtl.iFileArea = lVolumespace.liVolSize;
/**/        #ifdef DEBUG
/**/            cmnPrintf_ioMN(0,"### Vacancy Size -> %ld\n",lVolumespace.liVolSpace);
/**/            cmnPrintf_ioMN(0,"### All Memory -> %ld\n",lVolumespace.liVolSize);
/**/        #endif
/**/
/**/        /*  パラメータセット    */
/**/        ioMNsmyparam.sEmemCtl.hJobid = hJobID;
/**/        ioMNsmyparam.sEmemCtl.hOrgNum = IOMSG_NOT_USED;
/**/
/**/        /*  上位タスクにメモリ残量通知実施  */
/**//* D4.8 START T.Fujii メッセージ送信先修正 */
/**/#if 0   /* OLD */
/**/        if(ERROR == (Status = iomnSendMessage(msgQIdCpyUI, IOMSG_EMEM_UI, 0, &ioMNsmyparam))){
/**/            iomnDebugMessage(">>>error:iomnSendMessage() in iomnSendMemoryResidue()\n",0);
/**/            return(ERROR);
/**/        }
/**/#else   /* D4.8 MID     NEW */
/**/        if( msgQIdioMNforCpyJob == msgQIdCpyUI ) {
/**/            if(ERROR == (Status = iomnSendMessage(msgQIdioMNforCpyJob, IOMSG_EMEM_UI, 0, &ioMNsmyparam))){
/**/                iomnDebugMessage(">>>error:iomnSendMessage() in iomnSendMemoryResidue()\n",0);
/**/                return(ERROR);
/**/            }
/**/#endif
/**/        }
#endif
/* D4.8 END */
#if 0   /* V016.00_04   START   */
/**/        /* コピージョブ履歴更新(ジョブ状態を入力完了に) */
/**/        if( iomnCpyMNStoreCopyLog(iomnCpyMNGetCpyJobID(), IOMN_CPYJOB_SCN_END) == ERROR ) {
/**/            iomnDebugMessage(">> ioMN >> Error:iomnCpyMNStoreCopyLog() in iomnExPPCJobFinSts()\n",0);
/**/            /* return( ERROR );     処理は継続させる */
/**/        }
#endif  /* V016.00_04   END     */
        /* 入出力モードによる処理の分岐 */
        if( (hIOMode == COM_OT_TRK_SCS) || (hIOMode == COM_OT_TRK_FIN) ) {
            /* D3.12.2 START T.Fujii BG印刷ジョブ中断処理再修正 */
            /* ジョブ中断解除 */
            iomnSyncTblSetJobSuspend( iomnJobInfo[hJobID].hIpTblID, IOMN_OFF );
            /* D3.12.2 END */

            /* D4.4.1 START T.Fujii 原稿入力０ページ対応修正 */
            hSyncOutJobID = iomnSyncTblGetJobID( iomnJobInfo[hJobID].hIpTblID, COM_JB_SUB_OUTPUT );
            /* D4.4.1 END */

            /* 後追い逐次の場合、入力ジョブの解放 */
/* D3.21.3 START T.Fujii parMNコピー処理再修正 */
#if 0   /* OLD */
            if( iomnInputJobFree(hJobID, IOMN_CPYPARMN_ALL) == ERROR ) {
#else   /* D3.21.3 MID  NEW */
            if( iomnInputJobFree(hJobID, (IOMN_CPYPARMN_PAGES | IOMN_CPYPARMN_COPIES)) == ERROR ) {
#endif
/* D3.21.3 END */
                iomnDebugMessage(">> ioMN >> Error:iomnInputJobFree() in iomnExPPCJobFinSts()\n",0);
                /* return( ERROR );     処理は継続させる */
            }
        }
        else {
#if 0   /* V016.00_04   START   */
/**/            /* 後追い逐次以外の場合 */
/**/            /* 出力完了も設定 */
/**/            if( iomnCpyMNStoreCopyLog(iomnCpyMNGetCpyJobID(), IOMN_CPYJOB_PRN_END) == ERROR ) {
/**/                cmnPrintf_ioMN(2, ">> ioMN >> Error:iomnCpyMNStoreCopyLog() in iomnExPPCJobFinSts()\n" );
/**/                /* return( ERROR );     処理は継続させる */
/**/            }
#endif  /* V016.00_04   END     */
            /* フロントUI,prnUI/管理タスクに印刷終了(EPFN)を送信 */
            if( iomnSendEPFN(hJobID) == ERROR ) {
                iomnDebugMessage(">> ioMN >> Error:iomnSendEPFN(%d) in iomnExPPCJobFinSts()\n",(I32)hJobID);
                /* return( ERROR );     処理は継続させる */
            }

            iomnPrnQueJobDelete( hJobID , hIoMNPrnQueEntryID );
        }
/* D3.0.2 START T.Fujii ソースコード整理(この関数は必ずEXPPC2/IPPC) */
#if 0   /* OLD */
    }
#endif
/* D3.0.2 END */

    /* D3.12.2 START T.Fujii BG印刷ジョブ中断処理再修正、ソースコード整理 */
    /* 逐次コピーの場合の処理 */
    if( hIOMode != COM_OT_TRK_SCS && hIOMode != COM_OT_TRK_FIN ) {
    /* D3.12.2 END */
        /*  JOBEND開始可不確認処理 */
        if ((iConfirm = iomnConfirmJobEnd(hJobtype, hPrnResolution)) == ERROR ){
            iomnDebugMessage(">>>ERROR:iomnConfirmJobEnd() in iomnExPPCJobFinSts()\n",0);
            return(ERROR);
        }
        if ( iConfirm == IOMN_ON ){
            /*  JOBEND開始可の時 */
            /* EC22をmcnMNへ送信し、JOBEND処理を行ってもらう */
            Status = iomnSendEC22();
            if ( Status == ERROR ){
                cmnPrintf_ioMN(2,">>>Error:iomnSendEC22()-iomnExPPCJobFinSts()\n");
                iomnAddErrLog( "iomnSendEC22()-iomnExPPCJobFinSts()", errno );
                return ERROR;
            }
            /* TJFN受信時のBG印刷JOBキューイング有無フラグ OFF */
           ioMNstatus.hEC22SendInfo.hNextBgJobFlg = IOMN_OFF;
        }
        else if ( iConfirm == IOMN_OFF ){
            /*  JOBEND開始不可の時 */
            /* TJFN受信時のBG印刷JOBキューイング有無フラグ ON */
            ioMNstatus.hEC22SendInfo.hNextBgJobFlg = IOMN_ON;
            iomnDebugMessage("****hNextBgJobFlg ON!!!\n", 0);
        }
    }

    /* D4.4 START T.Fujii 原稿入力０ページ対応 */
    if( parMNPages(hJobID, &hInputPages) == ERROR ) {
        cmnPrintf_ioMN(2, "[ioMN]ERROR:parMNPages(JobID=%d)\n", hJobID );
    }
    #ifdef DEBUG
    cmnPrintf_ioMN(0, "[ioMN]CHECK:parMNPages(JobID=%d)=%d\n", hJobID, hInputPages );
    #endif
    /* D4.4 END */

#if 0   /* V016.00_04   START   */
/**/    /* D3.7 START T.Fujii 原稿枚数オーバー時の印刷中止処理追加 */
/**/    if(   iERRApl==(M_f220ScnEXErr|IOMSG_ERR_MGZSRT_CNTOVR)
/**/       || iERRApl==(M_f220ScnEXErr|IOMSG_ERR_COVER_PAGEOVER)
/**/       || iERRApl==(M_f220ScnEXErr|IOMSG_ERR_SHEET_PAGEOVER)
/**/       || iERRApl==(M_f220ScnEXErr|IOMSG_ERR_MGZSRT_MEMFUL)
/**/       || iERRApl==(M_f220ScnEXErr|IOMSG_ERR_COVER_MEMFUL)
/**//* D4.4 START T.Fujii 原稿入力０ページ対応 */
/**/#if 0   /* OLD */
/**//**/       || iERRApl==(M_f220ScnEXErr|IOMSG_ERR_SHEET_MEMFUL) ) {
/**//**/        /* 中折り綴じ/表紙・シート挿入で原稿枚数オーバーの場合 */
/**/#else   /* D4.4 MID NEW */
/**/       || iERRApl==(M_f220ScnEXErr|IOMSG_ERR_SHEET_MEMFUL)
/**/       || hInputPages == 0 ) {
/**/        /* 中折り綴じ/表紙・シート挿入で原稿枚数オーバーの場合 */
/**/        /* および原稿入力が０ページだった場合 */
/**/#endif
/**//* D4.4 END */
/**/        if( (hIOMode==COM_OT_TRK_SCS) || (hIOMode==COM_OT_TRK_FIN) ) {
/**/            /* 後追い逐次の印刷ジョブを中止する */
/**//* D4.4.1 START T.Fujii 原稿入力０ページ対応修正 */
/**/#if 0   /* OLD */
/**//**/            if( iomnOutputJobCancel( iomnCpyMNGetPrnJobID(iomnCpyMNGetCpyJobID()),
/**//**/                                     IOMSG_CAN_SYSERR ) == ERROR ) {
/**//**/                puts( "[ioMN]ERROR:iomnOutputJobCancel() in iomnExPPCJobFinSts()" );
/**//**/                cmnPrintf_ioMN(0, "[ioMN]ERROR:BG Print Job (ID=%d) will be continued\n",
/**//**/                        iomnCpyMNGetPrnJobID(iomnCpyMNGetCpyJobID()) );
/**//**/            }
/**/#else   /* D4.4.1 MID   NEW */
/**/            if( (hOutputJobID=iomnCpyMNGetPrnJobID(iomnCpyMNGetCpyJobID())) > IOMN_INPUTJOB_ID_MAX ) {
/**/#if 0   /* V013.00_05   */
/**//**/                if( iomnOutputJobCancel( hOutputJobID, IOMSG_CAN_SYSERR ) == ERROR ) {
/**//**/                    cmnPrintf_ioMN(2, "[ioMN]ERROR:iomnOutputJobCancel(%d) in iomnExPPCJobFinSts()", hOutputJobID );
/**//**/                }
/**/#endif
/**/            }
/**/            else {
/**/                if( hSyncOutJobID>IOMN_INPUTJOB_ID_MAX && ioMNCjst[hSyncOutJobID].OldmsgQId==0 ) {
/**/                    iomnJobInfo[hSyncOutJobID].hIpTblID = IPLIB_INIT;
/**/#if 0   /* V013.00_04   */
/**//**/                    jobDelete( hSyncOutJobID );  /* 印刷用ジョブID解放 */
/**/#endif
/**/                }
/**/                else {
/**/                    puts( "[ioMN]ERROR:Cannot Cancel Output Job." );
/**/                }
/**/            }
/**/#endif
/**//* D4.4.1 END */
/**/        }
/**/    }
/**/    /* D3.7 END */
#endif  /* V016.00_04   END     */

    /*  JOB完了タイプごと処理   */
    switch(hTjfnststype){
        case    IOMN_CAN_NORMAL:
        case    IOMN_CAN_ERR_APL:
                /*  JOB中止フラグ解除   */
                if(hJobID == hioMNFrontJobID || hJobID == hioMNIntJobID){
                    ioMNstatus.FrontCancel.hCanTask = ioMNstatus.FrontCancel.hCanType = IOMN_OFF;
                }
                else{
                    ioMNstatus.BackCancel.hCanTask = ioMNstatus.BackCancel.hCanType = IOMN_OFF;
                }

                /*  Scanner Recover 処理    */
                if(ERROR == (Status = iomnMcnRecover(hJobID, MCN_RCV_MODE_PPC, 
                                                MCN_RCV_RSC_SCN, IOMN_MCN_REC_TJFN, 0))){
                    iomnDebugMessage(">>>Error:iomnMcnRecover()in iomnExPPCJobFinSts()\n",0);
                    return(ERROR);
                }
                if( (hIOMode != COM_OT_TRK_SCS) && (hIOMode != COM_OT_TRK_FIN) ) {
                    /* 後追い逐次でなければ印刷処理もリカバリーを行う */
                    /*  Printer Recover 処理*/
                    if(ERROR == (Status = iomnMcnRecover(hJobID, MCN_RCV_MODE_PPC, 
                                                MCN_RCV_RSC_PRT, IOMN_MCN_REC_TJFN, 0))){
                        iomnDebugMessage(">>>Error:iomnMcnRecover() in iomnExPPCJobFinSts()\n",0);
                        return(ERROR);
                    }
                }
                break;

        case    IOMN_FIN_NORMAL:
        case    IOMN_FIN_ERR_APL:
                /*  ERROR発生中か否かをチェック */
/* D3.31 START T.Fujii リソースごとの処理に修正 */
#if 0   /* OLD */
                if(ioMNstatus.hError_sts != IOMN_OFF){
#else   /* D3.31 MID    NEW */
                if( (ioMNstatus.hError_sts & (MCN_RCV_RSC_SCN|MCN_RCV_RSC_ADF)) ||
                    ( (ioMNstatus.hError_sts & MCN_RCV_RSC_PRT)
                      && (hIOMode!=COM_OT_TRK_SCS && hIOMode!=COM_OT_TRK_FIN) ) )
                {
                    /* SCN/ADFエラー中、逐次コピーでPRNエラー中の何れかの場合 */
#endif
/* D3.31 END */
                    /*  MessageIDの取得 */
#if 0   /* MCK1.1 新メッセージ対応  */
/**/                    if(ioMNCjst[hJobID].iOldMsgId == IOMSG_CJST_UI) imsgid = IOMSG_TJFN_UI;
/**/                    else if(ioMNCjst[hJobID].iOldMsgId == IOMSG_CJST_DIG)   imsgid = IOMSG_TJFN_DIG;
/**/                    else    imsgid = IOMSG_TJFN_JOB;
#else
                    imsgid = JOB_MSG_TJFN;
#endif
                    /*  Errorコード初期化   */
                    iErrEng = IOMSG_NOT_USED;

                    /*  Errorコード設定 */
                    if(MCN_RCV_RSC_ADF == (ioMNstatus.hError_sts & MCN_RCV_RSC_ADF)){
                        iErrEng = ioMNstatus.iAdfErrorCode;
                    }
                    if(MCN_RCV_RSC_SCN == (ioMNstatus.hError_sts & MCN_RCV_RSC_SCN)){
                        iErrEng = ioMNstatus.iScnErrorCode;
                    }
                    if(MCN_RCV_RSC_PRT == (ioMNstatus.hError_sts & MCN_RCV_RSC_PRT)){
                        iErrEng = ioMNstatus.iPrtErrorCode;
                    }

                    /*  JOB完了メッセージ送信   */
                    if(ERROR == (Status = iomnSendJobFinishSts(ioMNCjst[hJobID].OldmsgQId, imsgid,
#if 0   /* MCK1.1 新メッセージ対応  引数追加    */
                                                     hJobID, hJobFinSts, iERRApl, iErrEng))){
#else
                                                     hJobID, hJobFinSts, iERRApl, iErrEng, hProcessNo))){
#endif
                        iomnDebugMessage(">>>Error:iomnSendJobFinishSts()in iomnExPPCJobFinSts()\n",0);
                        return(ERROR);
                    }
                }
                else{
                    /*  Scanner Recover 処理    */
                    if(ERROR == (Status = iomnMcnRecover(hJobID, MCN_RCV_MODE_PPC, 
                                                            MCN_RCV_RSC_SCN, IOMN_MCN_REC_TJFN, 0))){
                        iomnDebugMessage(">>>Error:iomnMcnRecover()in iomnExPPCJobFinSts()\n",0);
                        return(ERROR);
                    }
                    if( (hIOMode != COM_OT_TRK_SCS) && (hIOMode != COM_OT_TRK_FIN) ) {
                        /* 後追い逐次でなければ印刷処理もリカバリーを行う */
                        /*  Printer Recover 処理*/
                        if(ERROR == (Status = iomnMcnRecover(hJobID, MCN_RCV_MODE_PPC, 
                                                            MCN_RCV_RSC_PRT, IOMN_MCN_REC_TJFN, 0))){    
                            iomnDebugMessage(">>>Error:iomnMcnRecover() in iomnExPPCJobFinSts()\n",0);
                            return(ERROR);
                        }
                    }
                }
                break;

        case    IOMN_FIN_ERR_ENG:
        case    IOMN_CAN_ERR_ENG:
        case    IOMN_FIN_ERR_APL_ENG:
        case    IOMN_CAN_ERR_APL_ENG:
                /*  JOB状態変更 　実行中→　中断中　*/
                if(ERROR == (Status = jobSetStatus(hJobID, COM_JS_SUSPEND, iErrEngine))){
                    iomnDebugMessage(">>>Error:jobSetStatus() in iomnExPPCJobFinSts() \n",0);
                    return(ERROR);
                }
                iomnDebugMessage("*** JobStatus --> COM_JS_SUSPEND ***\n",0);

                /*  ERROR発生中か否かをチェック */
/* D3.31 START T.Fujii リソースごとの処理に修正 */
#if 0   /* OLD */
                if(ioMNstatus.hError_sts != IOMN_OFF){
#else   /* D3.31 MID    NEW */
                if( (ioMNstatus.hError_sts & (MCN_RCV_RSC_SCN|MCN_RCV_RSC_ADF)) ||
                    ( (ioMNstatus.hError_sts & MCN_RCV_RSC_PRT)
                      && (hIOMode!=COM_OT_TRK_SCS && hIOMode!=COM_OT_TRK_FIN) ) )
                {
                    /* SCN/ADFエラー中、逐次コピーでPRNエラー中の何れかの場合 */
#endif
/* D3.31 END */
                    /*  MessageIDの取得 */
#if 0   /* MCK1.1 新メッセージ対応  */
/**/                    if(ioMNCjst[hJobID].iOldMsgId == IOMSG_CJST_UI) imsgid = IOMSG_TJFN_UI;
/**/                    else if(ioMNCjst[hJobID].iOldMsgId == IOMSG_CJST_DIG)   imsgid = IOMSG_TJFN_DIG;
/**/                    else    imsgid = IOMSG_TJFN_JOB;
#else
                    imsgid = JOB_MSG_TJFN;
#endif
                    /*  JOB完了メッセージ送信   */
                    if(ERROR == (Status = iomnSendJobFinishSts(ioMNCjst[hJobID].OldmsgQId, imsgid,
#if 0   /* MCK1.1 新メッセージ対応  引数追加    */
                                                     hJobID, hJobFinSts, iERRApl, iErrEngine))){
#else
                                                     hJobID, hJobFinSts, iERRApl, iErrEngine, hProcessNo))){
#endif
                        iomnDebugMessage(">>>Error:iomnSendJobFinishSts()in iomnExPPCJobFinSts()\n",0);
                        return(ERROR);
                    }
                }
                else{
                    /*  Scanner Recover 処理    */
                    if(ERROR == (Status = iomnMcnRecover(hJobID, MCN_RCV_MODE_PPC, 
                                                                MCN_RCV_RSC_SCN, IOMN_MCN_REC_TJFN, 0))){
                        iomnDebugMessage(">>>Error:iomnMcnRecover()in iomnExPPCJobFinSts()\n",0);
                        return(ERROR);
                    }
                    if( (hIOMode != COM_OT_TRK_SCS) && (hIOMode != COM_OT_TRK_FIN) ) {
                        /* 後追い逐次でなければ印刷処理もリカバリーを行う */
                        /*  Printer Recover 処理*/
                        if(ERROR == (Status = iomnMcnRecover(hJobID, MCN_RCV_MODE_PPC, 
                                                                MCN_RCV_RSC_PRT, IOMN_MCN_REC_TJFN, 0))){
                            iomnDebugMessage(">>>Error:McnRecover()in iomnExPPCJobFinSts()\n",0);
                            return(ERROR);
                        }
                    }
                }
                break;

        default:
                iomnDebugMessage(">>>TJFN JOB TYPE ERROR\n",0);
                /*  Scanner Recover 処理    */
                if(ERROR == (Status = iomnMcnRecover(hJobID, MCN_RCV_MODE_PPC, 
                                                    MCN_RCV_RSC_SCN, IOMN_MCN_REC_TJFN, 0))){
                    iomnDebugMessage(">>>Error:iomnMcnRecover()in iomnExPPCJobFinSts()\n",0);
                    return(ERROR);
                }
                if( (hIOMode != COM_OT_TRK_SCS) && (hIOMode != COM_OT_TRK_FIN) ) {
                    /* 後追い逐次でなければ印刷処理もリカバリーを行う */
                     /*  Printer Recover 処理*/
                    if(ERROR == (Status = iomnMcnRecover(hJobID, MCN_RCV_MODE_PPC, 
                                                    MCN_RCV_RSC_PRT, IOMN_MCN_REC_TJFN, 0))){
                        iomnDebugMessage(">>>Error:iomnMcnRecover() in iomnExPPCJobFinSts()\n",0);
                        return(ERROR);
                    }
                }
                break;
    }

    /*  遅延中断中の場合    */
    if(ioMNstatus.hSusDelay == IOMN_ON){
        ioMNstatus.hSusDelay = IOMN_OFF;
        iomnDebugMessage(">>>Clear Delaid Suspend Flag\n",0);
    }
    if(ioMNCccn.iOldTaskId != 0){
        /*  JOB管理テーブルクリア   */
        memset(&ioMNCccn, 0x00, sizeof(struct msgstore));
        iomnDebugMessage(">>>OK:CCCN Table Clear\n",0);
    }

    /* D3.13.2 START T.Fujii TDJB送信処理抜け修正 */
    /* 逐次コピーの場合の処理 */
    if( hIOMode != COM_OT_TRK_SCS && hIOMode != COM_OT_TRK_FIN ) {
        /*  JOB削除指示受信に伴うJOB完了の場合  */
/* D5.0 START T.Fujii CDJB多重動作対応 */
#if 0   /* OLD */
        if(ioMNCdjb.OldmsgQId != NULL && ioMNCdjb.msgparam.sCdjbCtl.hJobid == hJobID){
#else   /* D5.0 NEW */
        if(ioMNCdjb[hJobID].OldmsgQId != NULL){
#endif
/* D5.0 END */
            /*  JOB削除ステータス送信   */
            if(iomnSendTDJB(hJobID, TRUE, IOMSG_NOT_USED) == ERROR ) {
                iomnDebugMessage("ERROR:iomnSendTDJB()\n",0);
                return(ERROR);
            }
            iomnDebugMessage(">>>OK:iomnSendTDJB()\n",0);
        }
    }
    /* D3.13.2 END */

    iomnDebugMessage(">>>Bottom of iomnExPPCJobFinSts()\n",0);
    return(OK);
}

/* D3.33 START T.Fujii 不要処理削除 */
#if 0   /* OLD */
/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         7.7.3
TITLE:      PrintJOB完了ステータス処理
MODULE：    STATUS  iomnPrnJobFinSts(HI hJobID, HI hJobFinSts, I32 iErrApl, I32 iErrEngine, HI hTjfnststype, HI hJobtype)
INCLUDE:    ioMNPar.h
PARAMETER:
            HI  hJobID;         JOBID
            HI  hJobFinSts;     JOB結果
            I32 iErrApl;        アプリ系エラー
            I32 iErrEngine      エンジン系エラー
            HI  hTjfnststype;   JOB完了タイプ
RETURNS:    I32 4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            印刷JOB完了ステータス受信時に完了タイプにより、Queの削除、プリンタリカバリなどを
            行い、JOB要求元に対してJOB完了Trmを通知する。
NOTES:
HISTORY:
        1.1     Y.Matsuda   96/9/18
        1.2     E.Saka      97/11/06
                GDIにおけるPM確保／解放方法修正に伴う変更
        1.3     E.Saka      97/11/26
                上記修正をDSS,GDIに適用
        2.1     yMatsuda    97/12/09    
                メモリＦＵＬＬ時の印刷ＪＯＢ検索処理で{}の関係を修正
        3.0     y.Matsuda   98/02.18
                複数のＦＡＸ印刷JOB蓄積状態で、ＦＡＸメモリFULL発生すると、
                次回からＦＡＸメモリFULL処理しなくなる不具合修正
        D1.1    T.Fujii     99/11/15
                DM45/35 後追い逐次機能組み込み
       D2.0.1   T.Fujii     00/06/06 DM45/35 NOP_DEL(ソースコード整理)
                                             puJobDelete()関数の呼び出しを削除

        MCK1.1  M.Karasawa 02/06/11 McKinley    新メッセージ対応
*/
STATUS  iomnPrnJobFinSts(HI hJobID, HI hJobFinSts, I32 iErrApl, I32 iErrEngine, HI hTjfnststype, HI hJobtype)
{
    STATUS      Status;     /*  マシンリカバリ結果  */
    I32         imsgid;     /*  メッセージＩＤ  */
    HI          hNextEntryId;   /*  次の印刷JOBのEntryID    */
    HI          hNextJobid; /*  次の印刷JOBID   */
    HI          hNextJobtype = 0;   /*  次の印刷JOB種別 */
    HI          hRecmode;       /*  マシンリカバリモード    */

    iomnDebugMessage(">>>>TOP of iomnPrnJobFinSts()<<<<\n",0);

    /* 印刷キューからジョブを削除 */
    if( iomnPrnQueJobDelete( hJobID , hIoMNPrnQueEntryID ) == ERROR ) {
        cmnPrintf_ioMN(2,">>>Error:iomnPrnQueJobDelete() in iomnPrnJobFinSts()\n");
        printErrno(errno);
    }

    /* この関数は実行されるのか？？ */
    /* T.B.D */

    /* ipテーブルの開放 */
        /* T.B.D */

    /* 印刷完了を通知 */
        /* T.B.D */
        /* iomnSendEPFN( hJobID ); */

    /* TJFN送信 */
        /* T.B.D */

    /* コピーの場合HDDのお掃除 */
        /* T.B.D */

    /* 印刷中フラグをクリア */
        /* T.B.D */

    /*  マシンリカバリタイプ、モード取得    */
    if(ERROR == (Status = iomnGetMcnRecmode(hJobID, hJobtype, &hRecmode))){
        iomnDebugMessage(">>>Error:iomnGetMcnRectype()\n",0);
        return(ERROR);
    }
    iomnDebugMessage("###Recovery Mode->%x\n",(I32)hRecmode);

    /*  JOB完了タイプごと処理   */
    switch(hTjfnststype){
        case    IOMN_CAN_NORMAL:
        case    IOMN_CAN_ERR_APL:
                /*  JOB中止フラグ解除   */
                ioMNstatus.BackCancel.hCanTask = ioMNstatus.BackCancel.hCanType = IOMN_OFF;

                /*  Printer Recover 処理*/
                if(ERROR == (Status = iomnMcnRecover(hJobID, hRecmode, MCN_RCV_RSC_PRT, IOMN_MCN_REC_TJFN, 0))){
                    iomnDebugMessage(">>>Error:iomnMcnRecover() in iomnPrnJobFinSts()\n",0);
                    return(ERROR);
                }
                break;

        case    IOMN_FIN_NORMAL:
        case    IOMN_FIN_ERR_APL:
                /*  プリンタリカバリ    */
                if(ERROR == (Status = iomnMcnRecover(hJobID, hRecmode, MCN_RCV_RSC_PRT, IOMN_MCN_REC_TJFN, 0))){
                    iomnDebugMessage(">>>Error:iomnMcnRecover() in iomnPrnJobFinSts()\n",0);
                    return(ERROR);
                }
                break;

        case    IOMN_FIN_ERR_ENG:
        case    IOMN_CAN_ERR_ENG:
        case    IOMN_FIN_ERR_APL_ENG:
        case    IOMN_CAN_ERR_APL_ENG:
                /*  ERROR発生中か否かをチェック */
                if(ioMNstatus.hError_sts != IOMN_OFF){
                    /*  MessageIDの取得 */
#if 0   /* MCK1.1 新メッセージ対応  */
/**/                    if(ioMNCjst[hJobID].iOldMsgId == IOMSG_CJST_UI) imsgid = IOMSG_TJFN_UI;
/**/                    else if(ioMNCjst[hJobID].iOldMsgId == IOMSG_CJST_DIG)   imsgid = IOMSG_TJFN_DIG;
/**/                    else    imsgid = IOMSG_TJFN_JOB;
#else
                imsgid = JOB_MSG_TJFN;
#endif

                    /*  JOB完了メッセージ送信   */
                    if(ERROR == (Status = iomnSendJobFinishSts(ioMNCjst[hJobID].OldmsgQId, imsgid,
                                                     hJobID, hJobFinSts, iErrApl, iErrEngine))){
                        iomnDebugMessage(">>>Error:iomnSendJobFinishSts()in iomnPrnJobFinSts()\n",0);
                        return(ERROR);
                    }
                }
                else{
                    /*  Printer Recover 処理*/
                    if(ERROR == (Status = iomnMcnRecover(hJobID, hRecmode,
                                                                 MCN_RCV_RSC_PRT, IOMN_MCN_REC_TJFN, 0))){
                        iomnDebugMessage(">>>Error:McnRecover()in iomnPrnJobFinSts()\n",0);
                        return(ERROR);
                    }
                }
                break;

        default:
                iomnDebugMessage(">>>TJFN Undefined JOB Type!\n",0);
                /*  Printer Recover 処理*/
                if(ERROR == (Status = iomnMcnRecover(hJobID, hRecmode, MCN_RCV_RSC_PRT, IOMN_MCN_REC_TJFN, 0))){
                    iomnDebugMessage(">>>Error:iomnMcnRecover() in iomnPrnJobFinSts()\n",0);
                    return(ERROR);
                }
                break;
    }

    /*  メモリFULL中で、フロントJOBが中断中でない場合   */
    if(ioMNstatus.memfull_sts == IOMN_ON
        && ioMNstatus.FrontSuspend.hSusTask == IOMN_OFF){
        iomnDebugMessage(">>>End of Memory Full Printing!\n",0);
        /*  Fax関連印刷の場合   */
        if(hJobtype == COM_JB_POLL_RECV_FAX1
            || hJobtype == COM_JB_POLL_RECV_FAX2
            || hJobtype == COM_JB_PRT_FAX1
            || hJobtype == COM_JB_PRT_FAX2
            || hJobtype == COM_JB_DRC_PRT_FAX){
            /*  並行印刷中の場合    */
            if(ioMNstatus.hRDUPstatus == IOMN_ON){
                /*  メモリFULL印刷JOB有無取得   */
                hNextJobid = (HI)ERROR;
                iomnPrnQueSeqEntry( PRNQUE_FAX, 1,&hNextEntryId, &hNextJobid, &hNextJobtype);
                iomnDebugMessage("###Next EntryID ==>%x\n",(I32)hNextEntryId);
                iomnDebugMessage("###Next JobID ==>%x\n",(I32)hNextJobid);
                iomnDebugMessage("###Next JOB TYPE->%d\n",(I32)hNextJobtype);
                /*  次JOB無しまたは並行印刷でない場合   */
                if(hNextEntryId == ERROR
                    || hNextJobtype != COM_JB_DRC_PRT_FAX){
                    /*  並行印刷中フラグクリア  */
                    ioMNstatus.hRDUPstatus = IOMN_OFF;
                    iomnDebugMessage(">>>Clear RDUP flag\n",0);

                    /*  メモリFULL終了処理  */
                    if(ERROR == (Status = iomnEndOfMemoryFull(hJobID, hJobtype))){
                        iomnDebugMessage(">>>ERROR:iomnEndOfMemoryFull()\n",0);
                        return(ERROR);
                    }
                    iomnDebugMessage(">>>OK:iomnEndOfMemoryFull()\n",0);
                }
            }
        }
        /*  Fax関連印刷以外の場合   */
        else{
        }
    }

    return(OK);
}
#endif
/* D3.33 END */

/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         7.7.3.1
TITLE:      リカバリモード取得処理
MODULE：    STATUS  iomnGetMcnRecmode(HI hJobid, HI hJobtype, HI *pRecmode)
INCLUDE:    ioMNPar.h
PARAMETER:
            HI  hJobtype;       JOB種別
            HI  *pRecmode;      リカバリタイプ
RETURNS:    I32 4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            JOB種別よりマシンリカバリモードを取得する
NOTES:
HISTORY:
        1.1     Y.Matsuda   96/9/18
        M1.1(TJ)H.Takahashi 98/09/28
                DSS逐次対応
        D1.7.1 J.Ootani 00/03/01    DM45/35 SEINE FAX F-CODE対応(マージ)
        D2.0.1 T.Fujii  00/06/06    DM45/35 NOP_DEL(ソースコード整理)
                                            case文でのCOM_JB_BPPC,COM_JB_EXPPC1を削除
        D2.9   M.Fukuda 00/07/04    DM45/35 FAXリストレポート対応
*/
STATUS  iomnGetMcnRecmode(HI hJobID, HI hJobtype, HI *pRecmode)
{
    switch(hJobtype){
        case    COM_JB_IPPC:
        case    COM_JB_EXPPC2:
        case    COM_JB_IVSAVE_PPC:
        case    COM_JB_IVPRM_PPC:
                if(ioMNhw.hPriUi == UI_DIG){
                    *pRecmode = MCN_RCV_MODE_DIG;
                }
                else{
                    *pRecmode = MCN_RCV_MODE_PPC;
                }
                break;

        case    COM_JB_POLL_RECV_FAX1:
        case    COM_JB_POLL_RECV_FAX2:
        case    COM_JB_PRT_FAX1:
        case    COM_JB_PRT_FAX2:
        case    COM_JB_LIST_PRT_FAX:
        case    COM_JB_LIST_RESULT_SEND:
        case    COM_JB_SCN_FAX1:
        case    COM_JB_SCN_FAX2:
        case    COM_JB_POLL_SEND_FAX1:
        case    COM_JB_POLL_SEND_FAX2:
        case    COM_JB_DRC_SCN_FAX:
        case    COM_JB_IVSAVE_FAX:
        case    COM_JB_IVPRM_FAX:
        case    COM_JB_SEND_DSI:
        case    COM_JB_DRC_PRT_FAX:
        case    COM_JB_CON_REG_FAX2:
        case    COM_JB_BOD_REG_FAX2:
        case    COM_JB_CON_GET_FAX2:
        case    COM_JB_BOD_GET_FAX2:
                *pRecmode = MCN_RCV_MODE_FAX;
                break;
        case    COM_JB_PRT_GDI1:
        case    COM_JB_PRT_GDI2:
        case    COM_JB_DRC_PRT_GDI:
        case    COM_JB_RECV_DSI:
        case    COM_JB_PRIVATE_DSI:
        case    COM_JB_RECV_DSI_SYNC:
        case    COM_JB_PRIVATE_DSI_SYNC:
                *pRecmode = MCN_RCV_MODE_GDI;
                break;
                
        default:
            iomnDebugMessage(">>>Unreasonable JOB TYPE\n",0);
            return(ERROR);
    }
    return(OK);
}


/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         7.7.3.2
TITLE:      印刷終了時の優先度変更処理
MODULE：    STATUS  iomnPrnChangeUI(HI hJobid, HI hFrontJobtype, HI hJobtype)
INCLUDE:    ioMNPar.h
            ioMNstr.h
            ioMNmsg.h
PARAMETER:
            HI  hJobid;         バックグランドＪＯＢＩＤ
            HI  hFrontJobtype;  フロントJOB種別
            HI  hJobtype;       バックグランドJOB種別
RETURNS:    I32 4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            印刷JOB終了時にフロントJOB種別、フロントＵＩ等の情報を元に
            優先度変更を行う。
NOTES:
HISTORY:
        1.1     Y.Matsuda   96/9/18
       V2.6     J.Ootani    97/12/08
            ＤＳＳＪＯＢ削除処理対応
        2.9     Y.Matsuda   98/02/05
                印刷完了時、すでに印刷復帰先情報が初期化されていた場合に、印刷復帰予約テーブルも
                初期化するよう修正（投入待ち）
       V4.5D    yMatsuda    98/02/25    
                JOB削除処理修正対応
        M6.1    (TJ)H.Takahashi 99/04/26
                LANI/FのDSSI/F統一対応
        D1.2    T.Fujii     99/12/24    逐次印刷・並行動作対応
*/
STATUS  iomnPrnChangeUI(HI hJobid, HI hFrontJobtype, HI hJobtype)
{
    STATUS      Status;
    HI          hFrontJobstatus;
    I32         iFrontSuscause;
    HI          hUI;            /*  優先ＵＩ            */
    HI          hUiMode;        /*  UI優先度変更モード  */
    HI          hChangetype;    /*  UI優先度変更タイプ  */
    HI          hErrRetUI;      /*  優先ＵＩ            */
    HI          hErrRetMode;    /*  UI優先度変更モード  */
    HI          hErrRettype;    /*  UI優先度変更タイプ  */
    HI          hReserveUI;     /*  変更予約中UI    */
    HI          hReserveMode;   /*  変更予約中変更モード    */
    HI          hReserveType;   /*  予約中変更タイプ    */

    iomnDebugMessage(">>>Top of iomnPrnChangeUI()\n\n",0);
    /*  フロントJOB状態取得 */
    if(ERROR == (Status = jobStatus(hioMNFrontJobID, &hFrontJobstatus, &iFrontSuscause))){
        iomnDebugMessage(">>>Error:jobStatus()\n",0);
        return(ERROR);
    }
    iomnDebugMessage("###FrontJob Status->%x\n",(I32)hFrontJobstatus);

    /*  ＪＯＢ操作ＵＩがフロントの場合  */
    if(hFrontJobtype == COM_JB_JOB_UI){
        if( 0 ) {   /* ダミー */
        }
        /*  JOB削除指示を受け取っていない場合、初期化メッセージ送信 */
        else{

        }
    }
    
    /*  フロントＪＯＢ動作中または中断中の優先度変更処理    */
    if((hFrontJobstatus == COM_JS_RUNNING 
            || hFrontJobstatus == COM_JS_SUSPEND)
        && (ioMNhw.hPriUi != UI_PRINT
            && ioMNhw.hPriUi != UI_JOBMN
            && ioMNhw.hPriUi != UI_MACHINE)){
    }

    /*  印刷、フロント非並行時の優先度変更処理  */
    else{
        /*  ＵＩ復帰先取得  */
        if(ERROR != (Status = iomnGetChangeUIQue(&ioMNUiPrintBuf, &hChangetype, &hUI, &hUiMode))){

            /*  ERROR発生中の場合はERROR復帰先情報テーブルに印刷終了時復帰先UI情報をコピーする  */
            if(ERROR != iomnGetChangeUIQue(&ioMNUiErrClrBuf, &hErrRettype, &hErrRetUI, &hErrRetMode)){
                /*  Error解除時の復帰先UIに印刷終了時復帰先UI情報をコピーする   */
                iomnSetChangeUIQue(&ioMNUiErrClrBuf, IOMN_CPRI_ERROR_CLEAR, hUI, IOMN_CPRI_ERROR_CLEAR);
            }
            /*  ERROR発生中でない場合は優先度変更処理を行う */
            else{
                iomnDebugMessage("###ioMNstatus.hChangeUIType->%x\n",(I32)ioMNstatus.hChangeUIType);
                if(ioMNstatus.hChangeUIType == IOMN_CPRI_NONE && ioMNstatus.hError_sts == IOMN_OFF){
                    /*  UI優先度変更処理    */
                    if(ERROR == (Status = iomnChangeUIPriority(hioMNFrontJobID, hUI, hUI, 
                                            IOMN_ON, IOMN_CPRI_PRINT_OFF, IOMN_CPRI_PRINT_OFF))){
                        iomnDebugMessage(">>>Error:iomnChangeUIPriority() in iomnPrnChangeUI()\n",0);
                        return(ERROR);
                    }
                    iomnDebugMessage(">>>OK:Change UI To FrontJOB UI\n",0);
                }
                else if(ioMNstatus.hChangeUIType != IOMN_CPRI_ERROR && ioMNstatus.hError_sts == IOMN_OFF){
                    /*  UI優先度変更処理のQueing    */
                    iomnSetChangeUIQue(&ioMNUiBuffer, IOMN_CPRI_PRINT_OFF, hUI, IOMN_CPRI_PRINT_OFF);
                }
            }
        }
        else{
            iomnDebugMessage(">>>Print Return Buffer had been already initialized\n",0);
        }
    }
    /*  印刷終了時復帰先ＵＩ情報テーブル初期化  */
    iomnCleanChangeUIQue(&ioMNUiPrintBuf);
    iomnDebugMessage(">>>Clear Print Return Table\n",0);

    /*  印刷ＵＩ予約中の場合、予約バッファクリア    */
    if(ERROR != (Status = iomnGetChangeUIQue(&ioMNUiReserveBuf, &hReserveType, &hReserveUI, &hReserveMode))){
        if(hReserveUI == UI_PRINT){
            /* 印刷ＵＩ予約テーブルクリア   */
            iomnCleanChangeUIQue(&ioMNUiReserveBuf);
            iomnDebugMessage(">>>Clear UI_PRINT Resavation Table\n",0);
        }
    }   
    iomnDebugMessage(">>>Bottom of iomnPrnChangeUI()\n\n",0);
    return(OK);
}


/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         7.7.3.3
TITLE:      メモリFULLFAX印刷終了処理
MODULE：    STATUS  iomnEndOfMemoryFull(HI hJobid, HI hJobtype)
INCLUDE:    ioMNPar.h
            ioMNstr.h
            ioMNmsg.h
PARAMETER:
            HI  hJobid;         ＪＯＢＩＤ
            HI  hJobtype;       JOB種別
RETURNS:    I32 4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            Fax印刷JOB終了時に、メモリFULL印刷の場合、メモリ残量を取得し
            フラグの解除、メモリFULL状態の解除、或いは並行印刷を実施する。
NOTES:
HISTORY:
        1.1     Y.Matsuda   96/9/18

        MCK1.1  M.Karasawa 02/06/11 McKinley    新メッセージ対応
*/
STATUS  iomnEndOfMemoryFull(HI hJobid, HI hJobtype)
{
    STATUS      Status;
    FB_SPACE    iomnFbmemory;       /*  Faxメモリ残量   */

    iomnDebugMessage(">>>Top of iomnEndOfMemoryFull()<<<\n",0);
    /*  MEMORY残量取得  暫定的に送信領域をコール    */
    if(ERROR == (Status = fbFileSpace(iTaskIdIoMN, NULL, NULL, &iomnFbmemory))){
        iomnDebugMessage(">>>Error:fbFileSpace",0);
        printErrno(errno);
        return(ERROR);
    }
    cmnPrintf_ioMN(0,"###All Memory Size-> %ld\n",iomnFbmemory.liAlloc);
    cmnPrintf_ioMN(0,"###Vacant Memory Size-> %ld\n",iomnFbmemory.liSpace);

    /*  メモリFULL解除通知送信  */
#if 0   /* MCK1.1 新メッセージ対応  */
/**/    if(ERROR == (Status = iomnSendEFCL(msgQIdFaxMN, IOMSG_EFCL_JOB, hJobid))){
#else
    if(ERROR == (Status = iomnSendEFCL(msgQIdFaxMN, SYS_MSG_EFCL, hJobid))){
#endif
        cmnPrintf_ioMN(2,">>>Error:iomnSendEFCL()\n");
        return(ERROR);
    }
    iomnDebugMessage(">>>Send EFCL Message\n",0);

    iomnDebugMessage(">>>Bottom of iomnEndOfMemoryFull()<<<\n",0);
    return(OK);
}


/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         7.7.4
TITLE:      Front操作によるPrintJOB完了ステータス処理
MODULE：    STATUS  iomnFrontPrnJobFinSts(HI hJobID, HI hJobFinSts, I32 iErrApl, I32 iErrEngine, HI hTjfnststype, HI hJobtype)
INCLUDE:    ioMNPar.h
PARAMETER:
            HI  hJobID;         JOBID
            HI  hJobFinSts;     JOB結果
            I32 iErrApl;        アプリ系エラー
            I32 iErrEngine      エンジン系エラー
            HI  hTjfnststype;   JOB完了タイプ
            HI  hJottype;       JOB種別
RETURNS:    I32 4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            FRONT操作による印刷JOB完了ステータス受信時に完了タイプにより、プリンタリカバリを
            行い、JOB要求元に対してJOB完了Trmを通知する。。
NOTES:
HISTORY:
        1.1     Y.Matsuda   96/9/18
        M1.1    (TJ)H.Takahashi 98/8/10
                        初期化時にpuJobEntry()をcall
        D1.7.1 J.Ootani 00/03/01    DM45/35 SEINE FAX F-CODE対応(マージ)
        D2.0.1  T.Fujii 00/06/06    DM45/35 NOP_DEL(ソースコード整理)
                                            puJobDelete()関数の呼び出しを削除
        D3.31   T.Fujii 00/10/10    DM45/35 マシンリカバリ処理修正

        MCK1.1  M.Karasawa 02/06/11 McKinley    新メッセージ対応
*/
#if 0   /* MCK1.1 新メッセージ対応  引数追加    */
/**/STATUS  iomnFrontPrnJobFinSts(HI hJobID, HI hJobFinSts, I32 iErrApl, I32 iErrEngine, HI hTjfnststype, HI hJobtype)
#else
STATUS  iomnFrontPrnJobFinSts(HI hJobID, HI hJobFinSts, I32 iErrApl, I32 iErrEngine, HI hTjfnststype, HI hJobtype, HI hProcessNo)
#endif
{
    STATUS      Status;     /*  マシンリカバリ結果  */
    I32         imsgid;     /*  メッセージＩＤ      */
    HI          hRecmode;   /*  マシンリカバリモード*/

    /*  マシンリカバリタイプ、モード取得    */
    if(ERROR == (Status = iomnGetMcnRecmode(hJobID, hJobtype, &hRecmode))){
        iomnDebugMessage(">>>Error:iomnGetMcnRectype()\n",0);
        return(ERROR);
    }
    iomnDebugMessage("###Recovery Mode->%x\n",(I32)hRecmode);

    /*  JOB完了タイプごと処理   */
    switch(hTjfnststype){
        case    IOMN_CAN_NORMAL:
        case    IOMN_CAN_ERR_APL:
                /*  JOB中止フラグ解除   */
                ioMNstatus.FrontCancel.hCanTask = ioMNstatus.FrontCancel.hCanType = IOMN_OFF;

                /*  Printer Recover 処理*/
                if(ERROR == (Status = iomnMcnRecover(hJobID, hRecmode, MCN_RCV_RSC_PRT, IOMN_MCN_REC_TJFN, 0))){
                    iomnDebugMessage(">>>Error:iomnMcnRecover() in iomnPrnJobFinSts()\n",0);
                    return(ERROR);
                }
                break;

        case    IOMN_FIN_NORMAL:
        case    IOMN_FIN_ERR_APL:
                /*  プリンタリカバリ    */
                if(ERROR == (Status = iomnMcnRecover(hJobID, hRecmode, MCN_RCV_RSC_PRT, IOMN_MCN_REC_TJFN, 0))){
                    iomnDebugMessage(">>>Error:iomnMcnRecover() in iomnPrnJobFinSts()\n",0);
                    return(ERROR);
                }
                break;

        case    IOMN_FIN_ERR_ENG:
        case    IOMN_CAN_ERR_ENG:
        case    IOMN_FIN_ERR_APL_ENG:
        case    IOMN_CAN_ERR_APL_ENG:
                /*  ERROR発生中か否かをチェック */
/* D3.31 START T.Fujii リソースごとの処理に修正 */
#if 0   /* OLD */
                if(ioMNstatus.hError_sts != IOMN_OFF){
#else   /* D3.31 MID    NEW */
                if( ioMNstatus.hError_sts & MCN_RCV_RSC_PRT ) {
                    /* PRNエラー中の場合 */
#endif
/* D3.31 END */
                    /*  MessageIDの取得 */
#if 0   /* MCK1.1 新メッセージ対応  */
/**/                    if(ioMNCjst[hJobID].iOldMsgId == IOMSG_CJST_UI) imsgid = IOMSG_TJFN_UI;
/**/                    else if(ioMNCjst[hJobID].iOldMsgId == IOMSG_CJST_DIG)   imsgid = IOMSG_TJFN_DIG;
/**/                    else    imsgid = IOMSG_TJFN_JOB;
#else
                    imsgid = JOB_MSG_TJFN;
#endif
                    /*  JOB完了メッセージ送信   */
                    if(ERROR == (Status = iomnSendJobFinishSts(ioMNCjst[hJobID].OldmsgQId, imsgid,
#if 0   /* MCK1.1 新メッセージ対応  引数追加    */
                                                     hJobID, hJobFinSts, iErrApl, iErrEngine))){
#else
                                                     hJobID, hJobFinSts, iErrApl, iErrEngine, hProcessNo))){
#endif
                        iomnDebugMessage(">>>Error:iomnSendJobFinishSts()in iomnPrnJobFinSts()\n",0);
                        return(ERROR);
                    }
                }
                else{
                    /*  Printer Recover 処理*/
                    if(ERROR == (Status = iomnMcnRecover(hJobID, hRecmode,
                                                                 MCN_RCV_RSC_PRT, IOMN_MCN_REC_TJFN, 0))){
                        iomnDebugMessage(">>>Error:McnRecover()in iomnPrnJobFinSts()\n",0);
                        return(ERROR);
                    }
                }
                break;

        default:
                iomnDebugMessage(">>>TJFN Undefined JOB Type!\n",0);
                /*  プリンタリカバリ    */
                if(ERROR == (Status = iomnMcnRecover(hJobID, hRecmode, MCN_RCV_RSC_PRT, IOMN_MCN_REC_TJFN, 0))){
                    iomnDebugMessage(">>>Error:iomnMcnRecover() in iomnPrnJobFinSts()\n",0);
                    return(ERROR);
                }
                break;
    }

    /*  メモリFULL中の場合  */
    if(ioMNstatus.memfull_sts == IOMN_ON){
    }
    return(OK);
}


/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         7.7.5
TITLE:      入力JOB完了ステータス処理
MODULE：    STATUS  iomnScnJobFinSts(HI hJobID, HI hJobFinSts,I32 iErrApl,I32 iErrEngine, HI hTjfnststype, HI hJobtype)
INCLUDE:    ioMNPar.h
PARAMETER:
            HI  hJobID;         JOBID
            HI  hJobFinSts;     JOB結果
            I32 iErrApl;        アプリ系エラー
            I32 iErrEngine      エンジン系エラー
            HI  hTjfnststype;   JOB完了タイプ
RETURNS:    I32 4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            入力JOB完了ステータスを受信し、JOB種別及び完了タイプにより、Queの削除、プリンタリカバリなどを
            行い、JOB要求元に対してJOB完了Trmを通知する。
NOTES:
HISTORY:
        1.1     Y.Matsuda   97/01/24
        2.4     Y.Matsuda   97/12/25    for A310061
                拡張FAX入力時終了時に印刷ＵＩ表示しないよう修正
        2.9     Y.Matsuda   98/02/09    
                入力実行中に印刷系Errorが発生した場合の対応追加
        M1.1    (TJ)H.Takahashi 98/8/10
                                初期化時にpuJobEntry()をcall
        M003.40 (TJ)H.Takahashi 98/12/03
                Form登録中の中断→中止対応  
        D1.4    Y.Shimada       00/ 1/25
                フォームストレージ変更：ＩｐＬｉｂ／逐次制御の組み込み
        D1.7.8  J.Ootani        00/03/11
                DM45/35 FAXジョブ開始終了対応
        D1.2.4.1 T.Fujii        00/04/15 DM45/35 印刷中断・再開処理修正
        D2.4    T.Fujii         00/06/15 DM45/35 prnEX自発中断対応
        D3.3    T.Fujii         00/08/08 DM45/35 MISSI V7.3以降の修正をDMにも反映
                                         ・原稿入力中止時に無条件でフォーム登録のファイル削除を
                                           行なう不具合修正(M8.1)
        D3.12   T.Fujii         00/09/02 DM45/35 ジョブ中断処理修正
        D3.12.1 T.Fujii         00/09/04 DM45/35 BG印刷ジョブ中断処理修正
        D3.12.2 T.Fujii         00/09/07 DM45/35 BG印刷ジョブ中断処理再修正、ソースコード整理
        D3.31   T.Fujii         00/10/10 DM45/35 マシンリカバリ処理修正


        MCK1.1  M.Karasawa 02/06/11 McKinley    新メッセージ対応
*/
#if 0   /* MCK1.1 新メッセージ対応  引数追加    */
/**/STATUS  iomnScnJobFinSts(HI hJobID, HI hJobFinSts, I32 iErrApl, I32 iErrEngine, HI hTjfnststype, HI hJobtype)
#else
STATUS  iomnScnJobFinSts(HI hJobID, HI hJobFinSts, I32 iErrApl, I32 iErrEngine, HI hTjfnststype, HI hJobtype, HI hProcessNo)
#endif
{
    STATUS      Status;     /*  マシンリカバリ結果  */
    I32         imsgid;     /*  メッセージＩＤ  */
    HI          hJobstatus; /*  JOB状態 */
    HI          hRecmode;
    HI          hSuspendQue;/*  中断中印刷ＱＵＥ    */

    iomnDebugMessage(">>>>Top of iomnScnJobFinSts()<<<<\n",0);

    /*
        ジョブ種別を見て、ＩｐＬｉｂ／逐次制御の解放を行う
        他のジョブ種別で「ＩｐＬｉｂ／逐次制御テーブルの確保処理」追加された時
        本処理も有効とする（case ジョブ種別:を追加）
    */
    switch( hJobtype ){
        case COM_JB_SCN_FAX1:
        case COM_JB_SCN_FAX2:
        case COM_JB_POLL_SEND_FAX1:
        case COM_JB_POLL_SEND_FAX2:
        case COM_JB_DRC_SCN_FAX:
        case COM_JB_CON_REG_FAX2:
        case COM_JB_BOD_REG_FAX2:
        case COM_JB_IVSAVE_PPC:
            /* ＩＰテーブルと逐次制御テーブルの解放 */
            iomnStart_IP_SIS_Cls( hJobID );
            break;
    }

    /*  マシンリカバリタイプ、モード取得    */
    if(ERROR == (Status = iomnGetMcnRecmode(hJobID, hJobtype, &hRecmode))){
        iomnDebugMessage(">>>Error:iomnGetMcnRectype()\n",0);
        return(ERROR);
    }
    iomnDebugMessage("###Recovery Mode->%x\n",(I32)hRecmode);

    /*  JOB完了タイプごと処理   */
    switch(hTjfnststype){
        case    IOMN_CAN_NORMAL:
        case    IOMN_CAN_ERR_APL:
/* D3.3 START T.Fujii PPCフォーム登録でなくてもファイル削除する不具合修正 */
#if 0   /* OLD */
            if(hTjfnststype == IOMN_CAN_NORMAL) {
#else   /* D3.3 MID     NEW */
            if(hTjfnststype == IOMN_CAN_NORMAL && hJobtype == COM_JB_IVSAVE_PPC) {
#endif
/* D3.3 END */
                /* メモリクリアー中止時ファイル削除処理 */
                if(ERROR == (Status = iomnIVFormFileDelete(hJobID))) {
                    cmnPrintf_ioMN(2,">>>Error:iomnIVFormFileDelete()\n");
                }else{
                    iomnDebugMessage(">>>OK:iomnIVFormFileDelete()\n",0);
                }
            }
            /*  JOB中止フラグ解除   */
            if(hJobID == hioMNFrontJobID || hJobID == hioMNIntJobID){
                ioMNstatus.FrontCancel.hCanTask = ioMNstatus.FrontCancel.hCanType = IOMN_OFF;
            }else{
                ioMNstatus.BackCancel.hCanTask = ioMNstatus.BackCancel.hCanType = IOMN_OFF;
            }
            iomnDebugMessage(">>>OK:Clear Job Cancel flug\n",0);

            /*  Error発生中でないとき   */
/* D3.31 START T.Fujii リソースごとの処理に修正 */
#if 0   /* OLD */
            if(ioMNstatus.hError_sts == 0){
#else   /* D3.31 MID    NEW */
            if( !(ioMNstatus.hError_sts & (MCN_RCV_RSC_SCN|MCN_RCV_RSC_ADF)) ) {
                /* SCN/ADFエラーが発生していない場合 */
#endif
/* D3.31 END */
                /*  Scanner Recover 処理*/
                if(ERROR == (Status = iomnMcnRecover(hJobID, hRecmode, MCN_RCV_RSC_SCN, IOMN_MCN_REC_TJFN, 0))){
                    cmnPrintf_ioMN(2,">>>Error:iomnMcnRecover() in iomnScnJobFinSts()\n");
                    return(ERROR);
                }
                iomnDebugMessage(">>>OK:iomnMcnRecover\n",0);
            }else{
            /*  Error発生中の場合   */
                /*  MessageIDの取得 */
#if 0   /* MCK1.1 新メッセージ対応  */
/**/                if(ioMNCjst[hJobID].iOldMsgId == IOMSG_CJST_UI) imsgid = IOMSG_TJFN_UI;
/**/                else if(ioMNCjst[hJobID].iOldMsgId == IOMSG_CJST_DIG)   imsgid = IOMSG_TJFN_DIG;
/**/                else    imsgid = IOMSG_TJFN_JOB;
#else
                imsgid = JOB_MSG_TJFN;
#endif
                /*  JOB完了メッセージ送信   */
                if(ERROR == (Status = iomnSendJobFinishSts(ioMNCjst[hJobID].OldmsgQId, imsgid,
#if 0   /* MCK1.1 新メッセージ対応  引数追加    */
                                                 hJobID, hJobFinSts, iErrApl, iErrEngine))){
#else
                                                 hJobID, hJobFinSts, iErrApl, iErrEngine, hProcessNo))){
#endif
                    iomnDebugMessage(">>>Error:iomnSendJobFinishSts()in iomnScnJobFinSts() *\n",0);
                    return(ERROR);
                }
                iomnDebugMessage(">>>OK:iomnSendJobFinishSts()\n",0);
            }
            break;

        case    IOMN_FIN_NORMAL:
        case    IOMN_FIN_ERR_APL:
        case    IOMSG_CAN_ERROR:
            if(hTjfnststype == IOMN_FIN_ERR_APL
                   && (iErrApl & 0xffff) == (I32)IOMSG_ERR_IVSAVE_MEMFUL) {
               /* メモリフル中止時ファイル削除処理 */
               if(ERROR == (Status = iomnIVFormFileDelete(hJobID))) {
                   cmnPrintf_ioMN(2,">>>Error:iomnIVFormFileDelete()\n");
               }else{
                   iomnDebugMessage(">>>OK:iomnIVFormFileDelete()\n",0);
               }
            }
/* D3.31 START T.Fujii リソースごとの処理に修正 */
#if 0   /* OLD */
            if(ioMNstatus.hError_sts == 0){
#else   /* D3.31 MID    NEW */
            if( !(ioMNstatus.hError_sts & (MCN_RCV_RSC_SCN|MCN_RCV_RSC_ADF)) ) {
                /* SCN/ADFエラーが発生していない場合 */
#endif
/* D3.31 END */
                /*  Scanner Recover 処理*/
                if(ERROR == (Status = iomnMcnRecover(hJobID, hRecmode, MCN_RCV_RSC_SCN, IOMN_MCN_REC_TJFN, 0))){
                    cmnPrintf_ioMN(2,">>>Error:iomnMcnRecover() in iomnScnJobFinSts()\n");
                    return(ERROR);
                }
                iomnDebugMessage(">>>Machine Recover :Scanner\n",0);
            }else{
                /*  MessageIDの取得 */

#if 0   /* MCK1.1 新メッセージ対応  */
/**/                if(ioMNCjst[hJobID].iOldMsgId == IOMSG_CJST_UI) imsgid = IOMSG_TJFN_UI;
/**/                else if(ioMNCjst[hJobID].iOldMsgId == IOMSG_CJST_DIG)   imsgid = IOMSG_TJFN_DIG;
/**/                else    imsgid = IOMSG_TJFN_JOB;
#else
                imsgid = JOB_MSG_TJFN;
#endif
                /*  JOB完了メッセージ送信   */
                if(ERROR == (Status = iomnSendJobFinishSts(ioMNCjst[hJobID].OldmsgQId, imsgid,
#if 0   /* MCK1.1 新メッセージ対応  引数追加    */
                                                 hJobID, hJobFinSts, iErrApl, iErrEngine))){
#else
                                                 hJobID, hJobFinSts, iErrApl, iErrEngine, hProcessNo))){
#endif
                    iomnDebugMessage(">>>Error:iomnSendJobFinishSts()in iomnScnJobFinSts() *\n",0);
                    return(ERROR);
                }
                iomnDebugMessage(">>>OK:iomnSendJobFinishSts()\n",0);
            }
            break;

        case    IOMN_FIN_ERR_ENG:
        case    IOMN_CAN_ERR_ENG:
        case    IOMN_FIN_ERR_APL_ENG:
        case    IOMN_CAN_ERR_APL_ENG:
            /*  JOB状態の判定   */
/* D3.31 START T.Fujii リソースごとの処理に修正 */
#if 0   /* OLD */
            hJobstatus = ( ioMNstatus.hError_sts != IOMN_OFF)? COM_JS_SUSPEND : COM_JS_RUNNING ;
#else   /* D3.31 MID    NEW */
            hJobstatus = (ioMNstatus.hError_sts & (MCN_RCV_RSC_SCN|MCN_RCV_RSC_ADF))? COM_JS_SUSPEND : COM_JS_RUNNING;
#endif
/* D3.31 END */
            iomnDebugMessage("###JOB Status->%x\n",(I32)hJobstatus);

            /*  JOB状態変更 */
            if(ERROR == (Status = jobSetStatus(hJobID, hJobstatus, iErrEngine))){
                cmnPrintf_ioMN(2,">>>Error:jobSetStatus() in iomnScnJobFinSts() \n");
                printErrno(errno);
                return(ERROR);
            }

            /*  ERROR発生中か否かをチェック */
/* D3.31 START T.Fujii リソースごとの処理に修正 */
#if 0   /* OLD */
            if(ioMNstatus.hError_sts != IOMN_OFF){
#else   /* D3.31 MID    NEW */
            if( ioMNstatus.hError_sts & (MCN_RCV_RSC_SCN|MCN_RCV_RSC_ADF) ) {
                /* SCN/ADFエラー中の場合 */
#endif
/* D3.31 END */
                /*  MessageIDの取得 */
#if 0   /* MCK1.1 新メッセージ対応  */
/**/                if(ioMNCjst[hJobID].iOldMsgId == IOMSG_CJST_UI) imsgid = IOMSG_TJFN_UI;
/**/                else if(ioMNCjst[hJobID].iOldMsgId == IOMSG_CJST_DIG)   imsgid = IOMSG_TJFN_DIG;
/**/                else    imsgid = IOMSG_TJFN_JOB;
#else
                imsgid = JOB_MSG_TJFN;
#endif
                /*  JOB完了メッセージ送信   */
                if(ERROR == (Status = iomnSendJobFinishSts(ioMNCjst[hJobID].OldmsgQId, imsgid,
#if 0   /* MCK1.1 新メッセージ対応  引数追加    */
                                                 hJobID, hJobFinSts, iErrApl, iErrEngine))){
#else
                                                 hJobID, hJobFinSts, iErrApl, iErrEngine, hProcessNo))){
#endif
                    iomnDebugMessage(">>>Error:iomnSendJobFinishSts()in iomnScnJobFinSts() *\n",0);
                    return(ERROR);
                }
                iomnDebugMessage(">>>OK:iomnSendJobFinishSts()\n",0);
            }else{
                /*  Scanner Recover 処理*/
                if(ERROR == (Status = iomnMcnRecover(hJobID, hRecmode,
                                                         MCN_RCV_RSC_SCN, IOMN_MCN_REC_TJFN, 0))){
                    iomnDebugMessage(">>>Error:iomnMcnRecover() in iomnScnJobFinSts()\n",0);
                    return(ERROR);
                }
                iomnDebugMessage(">>>OK:iomnMcnRecover\n",0);
            }
            break;

        default:
            iomnDebugMessage(">>>TJFN JOB TYPE ERROR\n",0);
            /*  Scanner Recover 処理*/
            if(ERROR == (Status = iomnMcnRecover(hJobID, hRecmode, MCN_RCV_RSC_SCN, IOMN_MCN_REC_TJFN, 0))){
                cmnPrintf_ioMN(2,">>>Error:iomnMcnRecover() in iomnScnJobFinSts()\n");
                return(ERROR);
            }
            break;
    }

    /*  メモリFULL中の場合  */
    if(ioMNstatus.memfull_sts == IOMN_ON){
        /*  中断処理中印刷Que取得   */
        if(ERROR == (Status = iomnGetPrnqueInSuspendProcess(&hSuspendQue))){
            iomnDebugMessage(">>>Error:iomnGetPrnqueInSuspendProcess()\n",0);
            return(ERROR);
        }
        iomnDebugMessage("###PrintQue in Suspending->%x\n",(I32)hSuspendQue);
    }
    iomnDebugMessage(">>>>Bottom of iomnScnJobFinSts()<<<<\n",0);
    return(OK);
}

/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         7.7.6
TITLE:      DssPrintJOB完了ステータス処理
MODULE：    STATUS  iomnDssPrnJobFinSts(HI hJobID, HI hJobFinSts, I32 iErrApl, 
                                        I32 iErrEngine, HI hTjfnststype, HI hJobtype)
INCLUDE:    ioMNPar.h
PARAMETER:
            HI  hJobID;         JOBID
            HI  hJobFinSts;     JOB結果
            I32 iErrApl;        アプリ系エラー
            I32 iErrEngine      エンジン系エラー
            HI  hTjfnststype;   JOB完了タイプ
RETURNS:    I32 4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            印刷JOB完了ステータス受信時に完了タイプにより、Queの削除、プリンタリカバリなどを
            行い、JOB要求元に対してJOB完了Trmを通知する。
NOTES:
HISTORY:
        1.1     Y.Matsuda   96/9/18
        1.2     E.Saka      97/11/06
                GDIにおけるPM確保／解放方法修正に伴う変更
        1.3     E.Saka      97/11/26
                上記修正をDSS,GDIに適用
        2.1     yMatsuda    97/12/09    
                メモリＦＵＬＬ時の印刷ＪＯＢ検索処理で{}の関係を修正
        3.0     y.Matsuda   98/02.18
                複数のＦＡＸ印刷JOB蓄積状態で、ＦＡＸメモリFULL発生すると、
                次回からＦＡＸメモリFULL処理しなくなる不具合修正
    M1.1(TJ)H.Takahashi 98/08/12
        JOB完了時にpuJobDelete()をcallしないように修正
    M1.2(TJ)H.Takahashi 98/09/28
                DSS逐次対応
    M6.1(TJ)H.Takahashi 99/05/07
                印刷JOB開始タイミング高速化対応
    D1.1    T.Fujii     99/11/15 DM45/35 後追い逐次・並行処理対応
    D2.0.1  T.Fujii     00/06/06 DM45/35 NOP_DEL(ソースコード整理)
                                         puJobDelete()関数の呼び出しを削除
    D2.4.2  M.Fukuda    00/06/19 DM45/35 JOBENDイベント送信処理対応
    D2.4.4  T.Fujii     00/06/20 DM45/35 V004.70不具合対応
                                         ジョブ終了後のJobIDクリア処理対応
    D2.10   M.Fukuda    00/07/04 DM45/35 JOBENDイベント送信処理対応
    D2.10.3 M.Fukuda    00/07/14 DM45/35 JOBEND送信処理修正（DFジャム発生対応）
    D3.27   T.Fujii     00/09/30 DM45/35 割り込みコピー中のJOB中止でJOBEND送信する不具合修正
    D3.31   T.Fujii     00/10/10 DM45/35 マシンリカバリ処理修正
    D4.1    T.Fujii     00/11/01 DM45/35 ジョブ削除でEC22送信しない不具合修正
    D5.0    T.Fujii     00/12/12 DM45/35 CDJB多重動作対応

    MCK1.1  M.Karasawa 02/06/11 McKinley    新メッセージ対応
*/
#if 0   /* MCK1.1 新メッセージ対応  引数追加    */
/**/STATUS  iomnDssPrnJobFinSts(HI hJobID, HI hJobFinSts, I32 iErrApl, I32 iErrEngine, HI hTjfnststype, HI hJobtype)
#else
STATUS  iomnDssPrnJobFinSts(HI hJobID, HI hJobFinSts, I32 iErrApl, I32 iErrEngine, HI hTjfnststype, HI hJobtype, HI hProcessNo)
#endif
{
    STATUS      Status;     /*  マシンリカバリ結果  */
    I32         imsgid;     /*  メッセージＩＤ  */
    STATUS      iConfirm;              /*  JOBEND開始可不確認結果 */
    PAR_TBL_RESOLUTION  hPrnResolution;/*  前印刷JOB解像度        */
    HI          hRecmode;       /*  マシンリカバリモード    */
    /* D3.27 START T.Fujii JOBEND送信処理修正 */
    HI          hJobStatus;     /* ジョブ状態 */
    I32         iSuscause;      /* 中断要因 */
    /* D3.27 END */

    iomnDebugMessage(">>>>TOP of iomnDssPrnJobFinSts()<<<<\n",0);

    /* 処理完了したJOBの解像度取得 */
    if(ERROR == (Status = parMNPrnResolution(hJobID, &hPrnResolution)))
    {
         iomnDebugMessage(">>>Error:parMNPrnResolution()\n",0);
         return(ERROR);
    }
    iomnDebugMessage("*** hPrnResolution(hMain) ->%hd\n",hPrnResolution.hMain);
    iomnDebugMessage("*** hPrnResolution(hSub) ->%hd\n",hPrnResolution.hSub);

    /* 処理完了したJOB種別、解像度の退避 */
    ioMNstatus.hEC22SendInfo.hJobmode = hJobtype;
    memcpy(&ioMNstatus.hEC22SendInfo.hprnResolution,&hPrnResolution ,sizeof(PAR_TBL_RESOLUTION));

    /* D3.27 START T.Fujii JOBEND送信処理修正 */
    if( jobStatus(hJobID, &hJobStatus, &iSuscause) == ERROR ){
        puts( "[ioMN]:ERROR jobStatus in iomnDssPrnJobFinSts()" );
        /* ジョブ状態が取得できなくても処理は続ける */
        hJobStatus = -1;
    }
    /* D3.27 END */

    /* 印刷中状態をクリア */
    if( iomnSyncTblSetStatus(iomnJobInfo[hJobID].hIpTblID, COM_JB_SUB_OUTPUT, (HI)IOMN_IPTBL_NOTUSED)
        == ERROR ) {
        iomnDebugMessage(">>>Error:iomnSyncTblSetStatus() in iomnDssPrnJobFinSts()\n",0);
        /* return( ERROR );     処理は継続させる */
    }

    /* フロントUI,prnUI/管理タスクに印刷終了(EPFN)を送信 */
    if( iomnSendEPFN(hJobID) == ERROR ) {
        iomnDebugMessage(">> ioMN >> Error:iomnSendEPFN(%d) in iomnDssPrnJobFinSts()\n",(I32)hJobID);
        /* return( ERROR );     処理は継続させる */
    }

    iomnPrnQueJobDelete( hJobID , hIoMNPrnQueEntryID );  /* 印刷ジョブの削除 */

    /* ipテーブルの解放、原稿入力と印刷ジョブのリンクを解除 */
    /* 逐次制御テーブルをクリアする */
    if( iomnClearSyncLink(hJobID, COM_JB_SUB_OUTPUT) == ERROR ) {
        iomnDebugMessage("[ioMN]ERROR:iomnClearSyncLink(%d) in iomnDssPrnJobFinSts()\n",(I32)hJobID);
        /* return( ERROR );     処理は継続させる */
    }

    /*  JOBEND開始可不確認処理 */
    if ((iConfirm = iomnConfirmJobEnd(hJobtype, hPrnResolution)) == ERROR ){
        iomnDebugMessage(">>>ERROR:iomnConfirmJobEnd() in iomnDssPrnJobFinSts()\n",0);
        return(ERROR);
    }
    if ( iConfirm == IOMN_ON ){
        /*  JOBEND開始可の時 */
/* D3.27 START T.Fujii JOBEND送信処理修正 */
#if 0   /* OLD */
        /* EC22をmcnMNへ送信し、JOBEND処理を行ってもらう */
        Status = iomnSendEC22();
        if ( Status == ERROR ){
            cmnPrintf_ioMN(2,">>>Error:iomnSendEC22()-iomnDssPrnJobFinSts()\n");
            iomnAddErrLog( "iomnSendEC22()-iomnDssPrnJobFinSts()", errno );
            return ERROR;
        }
#else   /* D3.27 MID    NEW */
/* D4.1 START T.Fujii ジョブ削除でEC22送信しない不具合修正 */
#if 0   /* OLD */
        if( hJobStatus != COM_JS_SUSPEND ) {
            /* ジョブ中断中でなければ処理する */
#else   /* D4.1 MID     NEW */
#if 0   /* PRINT_JOINT */
/* PRINT_JOINT */        if( hJobStatus != COM_JS_SUSPEND ||
/* PRINT_JOINT */            ioMNstatus.hBackJobId == IOMN_UNDEF ||
/* PRINT_JOINT */            ioMNstatus.hBackJobId == hJobID ) {
#else
        if(iomnPJ_JobendCheck(hJobID) == OK ){
#endif
            /* ジョブ中断中でないか、BG印刷ジョブIDが未登録またはID一致の場合は処理する */
#endif
/* D4.1 END */
            /* EC22をmcnMNへ送信し、JOBEND処理を行ってもらう */
            Status = iomnSendEC22();
            if ( Status == ERROR ){
                cmnPrintf_ioMN(2,">>>Error:iomnSendEC22()-iomnDssPrnJobFinSts()\n");
                iomnAddErrLog( "iomnSendEC22()-iomnDssPrnJobFinSts()", errno );
                return ERROR;
            }
        }
#endif
/* D3.27 END */
        /* TJFN受信時のBG印刷JOBキューイング有無フラグ OFF */
        ioMNstatus.hEC22SendInfo.hNextBgJobFlg = IOMN_OFF;
    }
    else if ( iConfirm == IOMN_OFF ){
        /*  JOBEND開始不可の時 */
        /* TJFN受信時のBG印刷JOBキューイング有無フラグ ON */
#if 0   /* PRINT_JOINT *//* 連結割り込み */
/* PRINT_JOINT */        ioMNstatus.hEC22SendInfo.hNextBgJobFlg = IOMN_ON;
#else
#if 0   /* PRINT_JOINT *//* JOBEND */
/* PRINT_JOINT */       if(ioMNPJ_BackIDExist() == ERROR){
#else
        if( iomnPJ_JobendCheck(hJobID) == OK){
#endif
            ioMNstatus.hEC22SendInfo.hNextBgJobFlg = IOMN_ON;
        }
#endif
        iomnDebugMessage("****hNextBgJobFlg ON!!!\n", 0);
    }

    /*  マシンリカバリタイプ、モード取得    */
    if(ERROR == (Status = iomnGetMcnRecmode(hJobID, hJobtype, &hRecmode))){
        iomnDebugMessage(">>>Error:iomnGetMcnRectype()\n",0);
        iomnAddErrLog( "iomnGetMcnRecmode()-iomnDssPrnJobFinSts()", errno );
        return(ERROR);
    }
    iomnDebugMessage("###Recovery Mode->%x\n",(I32)hRecmode);

    /*  JOB完了タイプごと処理   */
    switch(hTjfnststype){
        case    IOMN_CAN_NORMAL:
        case    IOMN_CAN_ERR_APL:
                /*  JOB中止フラグ解除   */
                ioMNstatus.BackCancel.hCanTask = ioMNstatus.BackCancel.hCanType = IOMN_OFF;

                /*  Printer Recover 処理*/
                if(ERROR == (Status = iomnMcnRecover(hJobID, hRecmode, MCN_RCV_RSC_PRT, IOMN_MCN_REC_TJFN, 0))){
                    iomnDebugMessage(">>>Error:iomnMcnRecover() in iomnPrnJobFinSts()\n",0);
                    return(ERROR);
                }
                break;

        case    IOMN_FIN_NORMAL:
        case    IOMN_FIN_ERR_APL:
                /*  プリンタリカバリ    */
                if(ERROR == (Status = iomnMcnRecover(hJobID, hRecmode, MCN_RCV_RSC_PRT, IOMN_MCN_REC_TJFN, 0))){
                    iomnDebugMessage(">>>Error:iomnMcnRecover() in iomnPrnJobFinSts()\n",0);
                    iomnAddErrLog( "iomnMcnRecover()-iomnDssPrnJobFinSts()", errno );
                    return(ERROR);
                }
                break;

        case    IOMN_FIN_ERR_ENG:
        case    IOMN_CAN_ERR_ENG:
        case    IOMN_FIN_ERR_APL_ENG:
        case    IOMN_CAN_ERR_APL_ENG:
                /*  ERROR発生中か否かをチェック */
/* D3.31 START T.Fujii リソースごとの処理に修正 */
#if 0   /* OLD */
                if(ioMNstatus.hError_sts != IOMN_OFF){
#else   /* D3.31 MID    NEW */
                if( ioMNstatus.hError_sts & MCN_RCV_RSC_PRT ) {
                    /* PRNエラー中の場合 */
#endif
/* D3.31 END */
                    /*  MessageIDの取得 */
#if 0   /* MCK1.1 新メッセージ対応  */
/**/                    if(ioMNCjst[hJobID].iOldMsgId == IOMSG_CJST_UI) imsgid = IOMSG_TJFN_UI;
/**/                    else if(ioMNCjst[hJobID].iOldMsgId == IOMSG_CJST_DIG)   imsgid = IOMSG_TJFN_DIG;
/**/                    else    imsgid = IOMSG_TJFN_JOB;
#else
                    imsgid = JOB_MSG_TJFN;
#endif
                    /*  JOB完了メッセージ送信   */
                    if(ERROR == (Status = iomnSendJobFinishSts(ioMNCjst[hJobID].OldmsgQId, imsgid,
#if 0   /* MCK1.1 新メッセージ対応  引数追加    */
                                                     hJobID, hJobFinSts, iErrApl, iErrEngine))){
#else
                                                     hJobID, hJobFinSts, iErrApl, iErrEngine, hProcessNo))){
#endif
                        iomnDebugMessage(">>>Error:iomnSendJobFinishSts()in iomnPrnJobFinSts()\n",0);
                        return(ERROR);
                    }
                }
                else{
                    /*  Printer Recover 処理*/
                    if(ERROR == (Status = iomnMcnRecover(hJobID, hRecmode,
                                                                 MCN_RCV_RSC_PRT, IOMN_MCN_REC_TJFN, 0))){
                        iomnDebugMessage(">>>Error:McnRecover()in iomnPrnJobFinSts()\n",0);
                        return(ERROR);
                    }
                }
                break;

            default:
                iomnDebugMessage(">>>TJFN Undefined JOB Type!\n",0);
                /*  Printer Recover 処理*/
                if(ERROR == (Status = iomnMcnRecover(hJobID, hRecmode, MCN_RCV_RSC_PRT, IOMN_MCN_REC_TJFN, 0))){
                    iomnDebugMessage(">>>Error:iomnMcnRecover() in iomnDssPrnJobFinSts()\n",0);
                    return(ERROR);
                }
                break;
    }

    /*  JOB削除指示受信に伴うJOB完了の場合  */
/* D5.0 START T.Fujii CDJB多重動作対応 */
#if 0   /* OLD */
    if(ioMNCdjb.OldmsgQId != NULL && ioMNCdjb.msgparam.sCdjbCtl.hJobid == hJobID){
#else   /* D5.0 NEW */
    if(ioMNCdjb[hJobID].OldmsgQId != NULL){
#endif
/* D5.0 END */
        /*  JOB削除ステータス送信   */
        if(ERROR == (Status = iomnSendTDJB(hJobID, TRUE, IOMSG_NOT_USED))){
            iomnDebugMessage("ERROR:iomnSendTDJB()\n",0);
            return(ERROR);
        }
        iomnDebugMessage(">>>OK:iomnSendTDJB()\n",0);
    }

    iomnDebugMessage(">>>>Bottom of iomnDssPrnJobFinSts()<<<<\n",0);
    return(OK);
}


/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         7.8
TITLE:      データ転送指示受信処理
MODULE：    STATUS  iomnRecieveCJDT();
INCLUDE:    ioMNPar.h
PARAMETER:
RETURNS:    I32 4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            ＧＤＩＪＯＢ管理タスクからデータ受信許可を要求するコマンド。
            H/W実装状態、エラー発生状態、JOB実行状態によって許可するか否かを判断する。

        -> ソースコード整理に伴い、上記処理前面削除（CJDTは未使用メッセージの為、CJDT受信時、
        ERRORを返す。）
NOTES:
HISTORY:
        1.1     Y.Matsuda   97/1/9
    D2.0    M.Fukuda  00/05/29  DM45/35 NOP_DEL iomnRecieveCJDTの処理削除
*/
STATUS  iomnRecieveCJDT()
{
    return(ERROR);
}


/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         7.8.1
TITLE:      データ転送処理
MODULE：    STATUS  iomnDataTransport(HI hJobid, HI hResource, I32 iSts, I32 iSize)
INCLUDE:    ioMNPar.h
PARAMETER:
RETURNS:    I32 4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            HI      hJobid;             JOBID
            HI      hResource(PM,HDD);  転送先指定
            I32     iSts;               転送許可(TRUE, FALSE)
            I32     iSize;              転送許可サイズ
NOTES:
HISTORY:
        1.1     Y.Matsuda   97/1/9
*/
STATUS  iomnDataTransport(HI hJobid, HI hResource, I32 iSts, I32 iSize)
{
    STATUS  Status;
    HI      hJobtype;               /*  JOB種別         */

    iomnDebugMessage(">>>Top of iomnDataTransport()<<<<\n",0);
    /*  JOB種別取得 */
    if(ERROR == (Status = jobJobtype(hJobid, &hJobtype))){
        cmnPrintf_ioMN(2,">>>Error:jobJobtype()\n");
        printErrno(errno);
        return(ERROR);
    }
    iomnDebugMessage("###JOB TYPE --> %x\n",(I32)hJobtype);

    /*  転送パラメータ表示  */
    iomnDebugMessage("###JOB TYPE->%x\n",(I32)hJobtype);
    iomnDebugMessage("###STATUS  ->%x\n",iSts);
    iomnDebugMessage("###RESOURCE->%x\n",(I32)hResource);
    iomnDebugMessage("###SIZE(KB)->%x\n",iSize);

    /*  DATA受信許可    */
    if(ERROR == (Status = iomnSendTJDT(hJobid, hResource, iSts, iSize))){   
        iomnDebugMessage(">>>Error:iomnSendTJDT()\n",0);
        return(ERROR);
    }
    iomnDebugMessage(">>>OK:Send TJDT Message\n", 0);

    /*  Data転送パラメータ格納テーブル初期化    */
    memset(&ioMNTjdt, 0x00, sizeof(struct data_transport));
    iomnDebugMessage(">>>Clear ioMNTjdt TBL\n",0);

    iomnDebugMessage(">>>Bottom of iomnDataTransport()<<<<\n",0);
    return(OK);
}


/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         7.9
TITLE:      リカバリ受け付けメッセージ受信処理
MODULE：    STATUS  iomnRecoverAck()

INCLUDE:    ioMNPar.h
PARAMETER:
            N/A
RETURNS:    I32 4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            リカバリ受け付けＡｃｋを受信し、複写ＵＩにリカバリ処理開始を通知する。
NOTES:
HISTORY:
        1.1     Y.Matsuda   96/9/18

        MCK1.1  M.Karasawa 02/06/11 McKinley    新メッセージ対応
*/
STATUS  iomnRecoverAck()
{
#if 0   /* MCK1.1 新メッセージ対応  *//* 関数削除可能   */
/**/    HI      hjobid;
/**/    STATUS  SendEsrcsts;
/**/
/**/    hjobid = ioMNrmyparam.sAjrcCtl.hJobid;
/**/    iomnDebugMessage("###JOBID->%x\n",(I32)hjobid);
/**/
/**/    /*  リカバリ開始イベントメッセージ送信処理  */
/**/    if(ERROR == (SendEsrcsts = iomnSendEsrc(hjobid))){
/**/        iomnDebugMessage(">>>Error:iomnSendEsrc()\n",0);
/**/        return(ERROR);
/**/    }
/**/    iomnDebugMessage(">>>OK:iomnSendEsrc()\n",0);
#endif
    return(OK);
}


/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         7.10
TITLE:      リカバリ終了メッセージ受信処理
MODULE：    STATUS  iomnRecoverStatus()

INCLUDE:    ioMNPar.h
PARAMETER:
            N/A

RETURNS:    I32 4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            リカバリ終了メッセージを受信し、複写ＵＩにリカバリ終了を通知する。
NOTES:
HISTORY:
        1.1     Y.Matsuda   96/9/18

        MCK1.1  M.Karasawa 02/06/11 McKinley    新メッセージ対応
*/
STATUS  iomnRecoverStatus()
{
#if 0   /* MCK1.1 新メッセージ対応  */
/**/    STATUS  Status;    /*  リカバリ終了イベント通知処理結果    */
/**/    HI      hJobid;         /*  JOBID   */
/**/    HI      hSts;           /*  リカバリ処理結果    */
/**/    I32     iErr;           /*  エラー内容  */
/**/    HI      hBackCount;     /*  原稿戻し枚数    */
/**/
/**/    /*  パラメータ取得  */
/**/    hJobid = ioMNrmyparam.sTjrcCtl.hJobid;
/**/    hSts = ioMNrmyparam.sTjrcCtl.hSts;
/**/    iErr = ioMNrmyparam.sTjrcCtl.iErr;
/**/    hBackCount = ioMNrmyparam.sTjrcCtl.hOrgBack;
/**/    iomnDebugMessage("###JOBID->%x\n",(I32)hJobid);
/**/    iomnDebugMessage("###STATUS->%x\n",(I32)hSts);
/**/    iomnDebugMessage("###ERROR CODE->%x\n",iErr);
/**/    iomnDebugMessage("###Back Count->%x\n",(I32)hBackCount);
/**/    
/**/    /*      */
/**/    if(hSts != IOMSG_FIN_OK)    {
/**/        /*  Error処理   */
/**/    }
/**/    else{
/**/        /*  JOB状態変更 */
/**/    }
/**/
/**/    /*  リカバリ終了イベント送信    */
/**/    ioMNsmyparam.sEfrcCtl.hJobid = hJobid;
/**/    ioMNsmyparam.sEfrcCtl.hSts = hSts;
/**/    ioMNsmyparam.sEfrcCtl.iErr = iErr;
/**/    ioMNsmyparam.sEfrcCtl.hOrgBack = hBackCount;
/**/    if(ERROR == (Status = iomnSendEfrc(hJobid, hSts, iErr, hBackCount))){
/**/        iomnDebugMessage(">>>ErrorStatus for EFRC Message () **\n", 0);
/**/        return(ERROR);
/**/    }
#endif
    return(OK);
}


/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         7.11
TITLE:      JOB中止メッセージ受信処理
MODULE：    STATUS  iomnJobCancel()
INCLUDE:    ioMNPar.h
            ioMNmsg.h
            ioMNstr.h
PARAMETER:
            N/A
RETURNS:    I32 4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            JOB中止メッセージを受信し、中止要因により処理を行う。
NOTES:
HISTORY:
        1.1     Y.Matsuda   96/9/18
        D1.1    T.Fujii     99/11/15    DM45/35 後追い逐次・並行動作対応
        D2.0.1  T.Fujii     00/06/06    DM45/35 NOP_DEL(ソースコード整理)
                                                if文でのCOM_JB_BPPC,COM_JB_EXPPC1を削除

        MCK1.1  M.Karasawa  02/06/11    McKinley    新メッセージ対応
    V017.00_01  M.Karasawa  03/04/09    McKinley    テストコピー異常系対応
    V017.00_02  M.Karasawa  03/04/09    McKinley    カラートナーエンプティ動作対応
    V018.00_01  M.Karasawa  03/04/09    McKinley    メッセージ変更対応
    V019.00_01  M.Karasawa  03/05/20    McKinley    複写ジョブ以外のJobUIからのDelete対応
    V019.00_03  M.Karasawa  03/05/20    McKinley    ジョブ操作関連の仕様(?)変更対応
    V024.00_03  M.Karasawa  03/07/31    McKinley    キーカウンタ、コインコントローラ仕様対応
    V030.00_01  M.Karasawa  03/10/22    McKinley    中止要因に緊急停止追加
*/
STATUS  iomnJobCancel()
{
    HI      hJobid;         /*  JOBID   */
    HI      hCanFacter;     /*  中断要因    */
    STATUS  Status;         /*  各処理結果  */
    HI      hJobtype;       /*  JOB種別 */
    /* V019.00_03 SATRT */
    HI      hJcbID;
    HI      hTNo;
    HI      hProcessType;
    HI      hAppliType;
    /* V019.00_03 END   */

    /*  Get Parameter   */
    hJobid = ioMNrmyparam.sCjcnCtl.hJobid;
#if 0   /* MCK1.1 新メッセージ対応  */
/**/    hCanFacter = ioMNrmyparam.sCjcnCtl.hCanFacter;
#else
    hCanFacter = ioMNrmyparam.sCjcnCtl.hCanFactor;
#endif
    /* V019.00_03 SATRT */
    hJcbID       = ioMNrmyparam.sCjcnCtl.hJcbID;
    hTNo         = ioMNrmyparam.sCjcnCtl.hTNo;
    hProcessType = ioMNrmyparam.sCjcnCtl.hProcessType;
    hAppliType   = ioMNrmyparam.sCjcnCtl.hAppliType;
    /* V019.00_03 END   */
    iomnDebugMessage("###hJobID        = %x\n",(I32)hJobid);
    iomnDebugMessage("###Cancel Facter = %x\n",(I32)hCanFacter);
    /* V018.00_01 START *//* V019.00_03 パラメータを変更 */
    iomnDebugMessage("###hJcbID        = %x\n",(I32)hJcbID);
    iomnDebugMessage("###hTNo          = %x\n",(I32)hTNo);
    iomnDebugMessage("###hProcessType  = %x\n",(I32)hProcessType);
    iomnDebugMessage("###hAppliType    = %x\n",(I32)hAppliType);
    /* V018.00_01 END   */

    /*  JOB種類取得 */
    if(ERROR == (Status = jobJobtype(hJobid, &hJobtype))){
        cmnPrintf_ioMN(2,">>>Error:jobJobtype() in iomnRecJobStart())\n");
        printErrno(errno);
        return(ERROR);
    }
    iomnDebugMessage("###Cancel JOB TYPE->%x\n",(I32)hJobtype);

    /*  JOBキャンセルメッセージ保存 */
    iomnSetJobManagementMsg(&ioMNCjcn[hJobid]);

    /* V019.00_03 SATRT */
    if( hCanFacter == IOMSG_CAN_REMOVE ){
        /* ジョブ削除処理を行う */
        return(iomnJobPause(hJobid, hCanFacter, hJcbID, hTNo, hProcessType, hAppliType));
    }
    /* V019.00_03 END   */

    /*  ダイレクト送信の場合    */
    if(hJobtype == COM_JB_DRC_SCN_FAX){
        if(ERROR == (Status = iomnDirectFaxJobCancel(hJobid, hCanFacter))){
            iomnDebugMessage(">>>ERROR:iomnDirectFaxJobCancel()\n",0);
            return(ERROR);
        }
        iomnDebugMessage(">>>OK:iomnDirectFaxJobCancel()\n",0);
    }
    /*  複写系JOBの場合    */
    else if(hJobtype == COM_JB_EXPPC2
            || hJobtype == COM_JB_IPPC){
        /*  Change Process by Facter    */
        switch(hCanFacter){
            case    IOMSG_CAN_USER:
            case    IOMSG_CAN_COLORTONER_EMPTY: /* V017.00_02 ADD */
            case    IOMSG_CAN_COMPULSION:       /* V030.00_01 ADD */
#if 0   /* V017.00_02 OLD */
/**/                if(ERROR == (Status = iomnJobCancelByUser(hJobid, hJobtype))){
#else   /* V017.00_02 NEW */
                if(ERROR == (Status = iomnJobCancelByUser(hJobid, hJobtype, hCanFacter))){
#endif
                    iomnDebugMessage(">>>Error:iomnJobCancelByUser()\n",0);
                    return(ERROR);
                }
                return(OK);
            case    IOMSG_CAN_SYSERR:
            case    IOMSG_CAN_ENGERR:
            case    IOMSG_CAN_INT:
            case    IOMSG_CAN_FAX_MEMFUL:
            case    IOMSG_CAN_BYPASS:
            case    IOMSG_CAN_FIN_OVER:
            case    IOMSG_CAN_ADF_PRIFEED:
            case    IOMSG_CAN_NOKEYCOUNTER:     /* V024.00_03 ADD */
            case    IOMSG_CAN_NOCOIN:           /* V024.00_03 ADD */
                if( iomnSubJobCancel(hJobid,hJobtype,hCanFacter) == ERROR ) {
                    iomnDebugMessage(">> ioMN >> ERROR:iomnSubJobCancel() in iomnJobCancel()\n",0);
                    return(ERROR);
                }
                iomnDebugMessage(">>>OK:Send CJCN ---> tioExMN",0);
                break;
            /* V017.00_01   START   */
            /* テストコピー時の異常終了対応 */
            case    IOMSG_CAN_TEST_ERR:
                if(ERROR == (Status = iomnJobCancelTestCopy(hJobid, hJobtype, hCanFacter))){
                    iomnDebugMessage(">>>Error:iomnJobCancelTestCopy()\n",0);
                    return(ERROR);
                }
                break;
            /* V017.00_01   END     */
            default:
                iomnDebugMessage(">>>Error:Undefined Cancel Facter -->%d\n",(I32)hCanFacter);
                return(ERROR);
        }
    }
    /*  その他のJOBの場合   */
    else{
#if 0   /* V019.00_01 OLD */
/**/        /*  入出力実行管理に対してJOB中止メッセージ送信 */
/**/        if(ERROR == (Status = iomnSendJobCancel(hJobid, msgQIdIoExMN, hCanFacter))){
/**/            iomnDebugMessage(">>>ERROR:iomnSendJobCancel() in iomnSendJobCancel()\n",0);
/**/            return(ERROR);
/**/        }
/**/        iomnDebugMessage(">>>OK:Send CJCN ---> tioExMN",0);
#else   /* V019.00_01 NEW */
        if( iomnSubJobCancel(hJobid,hJobtype,hCanFacter) == ERROR ) {
            /* 入出力ジョブの中止処理 */
            iomnDebugMessage(">> ioMN >> ERROR:iomnSubJobCancel() in iomnJobCancel()\n",0);
            return( ERROR );
        }
#endif
    }
    return(OK);
}


/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         7.11.1
TITLE:      JOB Cancel Proccess by User Operation(C/S,A/C etc.)
MODULE：    STATUS  iomnJobCancelByUser(HI hJobid, HI hJobtype, HI hCanFacter)
INCLUDE:    ioMNPar.h
            ioMNmsg.h
            ioMNstr.h
PARAMETER:
            HI      hJobid;
            HI      hJobtype;   JOB種別
            HI      hCanFacter  中止要因
RETURNS:    I32 4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            ユーザ操作による中止処理を行う。
            カラートナーエンプティによる中止処理を行う。（V017.00_02）
NOTES:
HISTORY:
        1.1     Y.Matsuda   96/9/18
        D1.1    T.Fujii     99/11/15 後追い逐次・並行動作対応
        D1.2.5  T.Fujii     00/04/10    DM45/35 中止処理修正

        MCK1.2  M.Karasawa  02/06/21    McKinley    TJIN削除による処理削除
    V017.00_02  M.Karasawa  03/04/09    McKinley    カラートナーエンプティ動作対応
    V030.00_02  M.Karasawa  03/10/23    McKinley    CJCN処理エラー対応（McK4460）
*/
#if 0   /* V017.00_02 OLD */
/**/STATUS  iomnJobCancelByUser(HI hJobid, HI hJobtype)
#else   /* V017.00_02 NEW */
STATUS  iomnJobCancelByUser(HI hJobid, HI hJobtype, HI hCanFacter)
#endif
{
    STATUS      Status;         /*  各処理結果  */
    STATUS      CancelStatus;   /*  CANCEL確認処理  */
    MSG_Q_ID    msgqid;         /*  Message QUEID   */
/* V030.00_02 */
    HI      hJobstatus;
    I32     iSuscause;
/* V030.00_02 */

    iomnDebugMessage("### Top of iomnJobCancelByUser() ###\n",0);
    /*  Confirmation about whether we can Cancel or not */
    CancelStatus = iomnConfirmAboutCancel(hJobid);

    /*  MessageQueid取得    */
    if(hJobtype == COM_JB_DRC_SCN_FAX){                                 /* V011.00_04 有り得ない    */
        /*  ダイレクト送信の場合はFAXJOB管理に対して送信する    */
        msgqid = msgQIdFaxMN;
    }
    else{
        /*  ダイレクト送信以外の場合    */
        msgqid = msgQIdIoExMN;
    }

    /*  中止実行確認処理結果により処理選択  */
    switch(CancelStatus){
        case    IOMN_ON:
                if( msgqid == msgQIdIoExMN ) {
#if 0   /* V017.00_02 OLD */
/**/                    iomnSubJobCancel( hJobid, hJobtype, IOMSG_CAN_USER );
#else   /* V017.00_02 NEW */
                    iomnSubJobCancel( hJobid, hJobtype, hCanFacter );
#endif
                }
                else {
                    /*  入出力実行管理に対してJOB中止メッセージ送信 */
#if 0   /* V017.00_02 OLD */
/**/                    if(ERROR == (Status = iomnSendJobCancel(hJobid, msgqid, IOMSG_CAN_USER))){
#else   /* V017.00_02 NEW */
                    if(ERROR == (Status = iomnSendJobCancel(hJobid, msgqid, hCanFacter))){
#endif
                        iomnDebugMessage(">>>ERROR:iomnSendJobCancel() in iomnJobCancelByUser()\n",0);
                        return(ERROR);
                    }
                }
                iomnDebugMessage("*** Send CJCN ---> tioExMN",0);
                break;

        case    IOMN_OFF:
#if 0   /* MCK1.2 TJINメッセージ削除の為処理を削除  */
/**/                /*  JOB中断／中止不成功メッセージ送信   */
/**/                if(ERROR == (Status = iomnSendJobCancelFail(hJobid,
/**/                                        ioMNCjcn[hJobid].OldmsgQId ,ioMNCjcn[hJobid].iOldTaskId))){
/**/                    iomnDebugMessage(">>>ERROR:iomnSendJobCancelFail() in iomnJobCancelByUser()\n",0);
/**/                    return(ERROR);
/**/                }
#endif
                #ifdef  DEBUG
                    cmnPrintf_ioMN(0,">>>Send TB10 ---> %s\n",taskName(iioMNTaskId));
                #endif
/* V030.00_02 SATART */
                if(ERROR == jobStatus(hJobid, &hJobstatus, &iSuscause)){
                    iomnDebugMessage(">>>Error:jobStatus()\n",0);
                    return(ERROR);
                }
                if( hJobstatus == COM_JS_SUSPEND && iSuscause != IOMSG_SUS_CS){
                    /* 自動再開しないようにCS中断を強制的に設定する */
                    if(ERROR == jobSetStatus(hJobid,COM_JS_SUSPEND,IOMSG_SUS_CS)){
                        cmnPrintf_ioMN(2,">>>Error:jobSetStatus() in iomnJobCancelTestCopy()\n");
                    }
                }

                if( ERROR == iomnSendTjfn(hJobid, (HI)IOMSG_JB_TEST_ERR, (I32)IOMSG_NOT_USED, (I32)IOMSG_NOT_USED) ){
                    iomnDebugMessage(">> ioMN >> Error:iomnSendTjfn()in iomnJobCancelByUser()\n",0);
                    return(ERROR);
                }
/* V030.00_02 END    */
                /*  キャンセルフラグ解除    */
                if(hJobid == hioMNFrontJobID || hJobid == hioMNIntJobID){
                    ioMNstatus.FrontCancel.hCanTask = ioMNstatus.FrontCancel.hCanType = IOMN_OFF;
                }
                else if( hJobid == hioMNDataRecvJobID ) {
                    /* データ転送ジョブのときは何もしない */
                }
                else{
                    ioMNstatus.BackCancel.hCanTask = ioMNstatus.BackCancel.hCanType = IOMN_OFF;
                }
                break;

        case    ERROR:
                iomnDebugMessage(">>>ERROR:iomnConfirmAboutCancel() in iomnJobCancelByUser()\n",0);
                return(ERROR);

        default:
                break;
    }
    return(OK);
}


/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         7.11.2
TITLE:      ダイレクト送信時の中止処理
MODULE：    STATUS  iomnDirectFaxJobCancel(HI hJobid, HI hCanFacter)
INCLUDE:    ioMNPar.h
            ioMNmsg.h
            ioMNstr.h
PARAMETER:
            HI      hJobid;
            HI      中止要因
RETURNS:    I32 4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            ダイレクト送信時、FAXJOB管理より中止コマンドを受信すると、中止可能か否か判断し、可能な
            場合にはFAXJOB管理に対してJOB中止コマンドを送信する。
NOTES:
HISTORY:
        1.1     Y.Matsuda   96/9/18
        D1.7.10 J.Ootani    00/04/10    DM45/35 FAXダイレクト送信対応
                                        全面変更
        D3.41.1 T.Fujii     00/10/19    DM45/35 mcnUIへの画面変更処理追加

        MCK1.1  M.Karasawa  02/06/11    McKinley    新メッセージ対応
        MCK1.5  M.Karasawa  02/07/26    McKinley    DELETE iomnChangeToMcnUI
*/
STATUS  iomnDirectFaxJobCancel(HI hJobid, HI hCanFacter)
{
    STATUS  Status;

    iomnDebugMessage("### Top of iomnDirectFaxJobCancel() ###\n",0);

    /*  ＪＯＢ完了メッセージ送信    */
    if(ERROR == (Status = iomnSendJobFinishSts(ioMNCjst[hJobid].OldmsgQId,
#if 0   /* MCK1.1 新メッセージ対応  */
/**/                               IOMSG_TJFN_JOB, hJobid, IOMSG_NOT_USED,
#else
                               JOB_MSG_TJFN, hJobid, IOMSG_NOT_USED,
#endif
#if 0   /* MCK1.1 新メッセージ対応  引数追加    *//* 暫定   TJFN送信 プロセス番号NOTUSED    */
                               IOMSG_NOT_USED, IOMSG_NOT_USED)))
#else
                               IOMSG_NOT_USED, IOMSG_NOT_USED, IOMSG_NOT_USED)))
#endif
    {
        cmnPrintf_ioMN(2,">>>Error:iomnSendJobFinishSts()\n");
        return(ERROR);
    }
    /*  ＪＯＢ管理メッセージバッファクリア  */
    iomnCleanJobManagementMsg(hJobid);

    iomnDebugMessage("### Bottom of iomnDirectFaxJobCancel() ###\n",0);
    return(OK);
}


/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         7.13
TITLE:      Recieve the JOB Suspend Command Proccess
MODULE：    STATUS  iomnRecieveCJSU()
INCLUDE:    ioMNPar.h
            ioMNmsg.h
            ioMNstr.h
PARAMETER:
            HI      hJobid;
RETURNS:    I32 4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            ＪＯＢ中断コマンド受信処理
NOTES:
HISTORY:
        1.1     Y.Matsuda   97/01/22
       V4.0     J.Ootani    97/12/26
             メッセージのすれちがいによりＪＯＢがすでに存在しない
             場合のＪＯＢ中断要求に対する中断失敗返信の対応
        M1.1(TJ)H.Takahashi 98/09/28
                DSS逐次対応
        D1.1    T.Fujii     99/11/15 DM45/35 後追い逐次・並行動作対応
        D1.4    Y.Shimada   00/ 1/25 DM45/35 フォームストレージ変更：ＩｐＬｉｂ／逐次制御の組み込み
        D1.7.1 J.Ootani 00/03/01    DM45/35 SEINE FAX F-CODE対応(マージ)
        D2.4    T.Fujii     00/06/15 DM45/35 prnEX遅延中断対応
        D2.9    M.Fukuda    00/07/04 DM45/35 FAXリストレポート対応
        D2.0.2  T.Fujii     00/07/04 DM45/35 NOP_DEL(ソースコード整理)
                                             if文,case文でのCOM_JB_EXPPC1を削除
        D3.12   T.Fujii     00/09/02 DM45/35 ジョブ中断処理修正

        MCK1.1  M.Karasawa  02/06/11 McKinley    新メッセージ対応
        MCK1.2  M.Karasawa  02/06/21 McKinley    TJIN削除による処理削除
*/
STATUS  iomnRecieveCJSU()
{
    STATUS  Status;
    HI      hJobtype;
    HI      hJobid;
    HI      hSusFacter;
    HI      hSubJobmode;

    /*  パラメータ取得  */
    hJobid = ioMNrmyparam.sCjsuCtl.hJobid;
#if 0   /* MCK1.1 新メッセージ対応  */
/**/    hSusFacter = ioMNrmyparam.sCjsuCtl.hSusFacter;
#else
    hSusFacter = ioMNrmyparam.sCjsuCtl.hSusFactor;
#endif

    iomnDebugMessage("### JOBID-->%x\n",hJobid);
    iomnDebugMessage("### Facter for Suspend -->%x\n",hSusFacter);

    /*  JOB種別取得 */
    if(ERROR == (Status = jobJobtype(hJobid, &hJobtype))){
        iomnDebugMessage(">>>Error:jobJobtype()\n",0);
#if 0   /* MCK1.2 TJINメッセージ削除の為処理を削除  */
/**/        if(ERROR == (Status = iomnSendJobCancelFail(hJobid,ioMNMsgQId,iioMNTaskId))){
/**/            cmnPrintf_ioMN(2,">>>ERROR:iomnSendJobCancelFail() in iomnRecieveCJSU()\n");
/**/            return(ERROR);
/**/        }
#endif
        return(ERROR);
    }

    /*  中断メッセージ格納  */
    iomnSetJobManagementMsg(&ioMNCjsu[hJobid]);

    /*  JOB種別中断処理 */
    switch(hJobtype){
        case COM_JB_EXPPC2:
        case COM_JB_IPPC:   /* D1.1 T.Fujii 99/11/15 DM45/35 追加 */
            parMNSubJobmode( hJobid, &hSubJobmode );    /* サブジョブ種別取得 */
            if( hSubJobmode == COM_JB_SUB_INPUT ) {
/* D3.12 START T.Fujii ジョブ中断処理修正 */
#if 0   /* OLD */
/**/                /* 入力ジョブの場合 */
/**/                if( iomnFrontCjsu(hJobid, hSusFacter, hJobtype) == ERROR ) {
#else   /* D3.12 MID    NEW */
                if( iomnInputJobSuspend(hJobid, hSusFacter, hJobtype) == ERROR ) {
#endif
/* D3.12 END */
                    iomnDebugMessage("[ioMN]Error:iomnFrontCjsu()\n",0);
                    return(ERROR);
                }
            }
            else {
                /* 出力ジョブの場合 */
                if(ERROR == (Status = iomnPrnCjsu(hJobid, hSusFacter, hJobtype))){
                    iomnDebugMessage(">>>Error:iomnPrnCjsu()\n",0);
                    return(ERROR);
                }
            }
            break;
        case COM_JB_SCN_FAX1:
        case COM_JB_SCN_FAX2:
        case COM_JB_POLL_SEND_FAX1:
        case COM_JB_POLL_SEND_FAX2:
        case COM_JB_IVSAVE_PPC:
        case COM_JB_IVSAVE_FAX:
        case COM_JB_IVPRM_FAX:
        case COM_JB_CON_REG_FAX2:
        case COM_JB_BOD_REG_FAX2:
        case COM_JB_CON_GET_FAX2:
        case COM_JB_BOD_GET_FAX2:
            if(ERROR == (Status = iomnFrontCjsu(hJobid, hSusFacter, hJobtype))){
                iomnDebugMessage(">>>Error:iomnFrontCjsu()\n",0);
                return(ERROR);
            }
            break;
        case COM_JB_SEND_DSI:
            if(ERROR == (Status = iomnFrontDssCjsu(hJobid, hSusFacter, hJobtype))){
                iomnDebugMessage(">>>Error:iomnFrontCjsu()\n",0);
                return(ERROR);
            }
            break;
        case COM_JB_DRC_SCN_FAX:
            if(ERROR == (Status = iomnDirectCjsu(hJobid, hSusFacter, hJobtype))){
                iomnDebugMessage(">>>Error:iomnFrontCjsu()\n",0);
                return(ERROR);
            }
            break;
        case COM_JB_LIST_PRT_FAX:
        case COM_JB_LIST_RESULT_SEND:
            /*  フロント操作によるリスト印刷時  */
            if(hJobid == hioMNFrontJobID){
                if(ERROR == (Status = iomnFrontCjsu(hJobid, hSusFacter, hJobtype))){
                    iomnDebugMessage(">>>Error:iomnFrontCjsu()\n",0);
                    return(ERROR);
                }
            }else{
            /*  バックグランドリスト印刷中断時  */
                if(ERROR == (Status = iomnPrnCjsu(hJobid, hSusFacter, hJobtype))){
                    iomnDebugMessage(">>>Error:iomnPrnCjsu()\n",0);
                    return(ERROR);
                }
            }
            break;
        case COM_JB_PRT_GDI1:
        case COM_JB_PRT_GDI2:
        case COM_JB_PRT_FAX1:
        case COM_JB_PRT_FAX2:
        case COM_JB_POLL_RECV_FAX1:
        case COM_JB_POLL_RECV_FAX2:
        case COM_JB_DRC_PRT_FAX:
        case COM_JB_DRC_PRT_GDI:
        case COM_JB_IVPRM_PPC:
            if(ERROR == (Status = iomnPrnCjsu(hJobid, hSusFacter, hJobtype))){
                iomnDebugMessage(">>>Error:iomnPrnCjsu()\n",0);
                return(ERROR);
            }
            break;
        case COM_JB_RECV_DSI:
        case COM_JB_PRIVATE_DSI:
        case COM_JB_RECV_DSI_SYNC:
        case COM_JB_PRIVATE_DSI_SYNC:
            if(ERROR == (Status = iomnDssPrnCjsu(hJobid, hSusFacter, hJobtype))){
                iomnDebugMessage(">>>Error:iomnDssPrnCjsu()\n",0);
                return(ERROR);
            }
            break;
        case COM_JB_LIST_SEND_FAX:
        case COM_JB_DIG:
        case COM_JB_UNDEF:
        case COM_JB_JOB_UI:
        default:
            iomnDebugMessage(">>>ERROR:Undefined JOB TYPE!!!\n",0);
            return(ERROR);
    }
    return(OK);
}


/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         7.13.1
TITLE:      PPCJOB中断コマンド受信処理
MODULE：    STATUS  iomnFrontCjsu(HI hJobid, HI hSusFacter, HI hJobtype)
INCLUDE:    ioMNPar.h
            ioMNmsg.h
            ioMNstr.h
PARAMETER:
            HI      hJobid;
            HI      hSusFacter;     中断要因
            HI      hJobtype;       JOB種別
RETURNS:    I32 4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            拡張複写JOB中断指示を受け、JOB状態を確認した後、実行管理タスク
            に対してJOB中断コマンドを送信する。
NOTES:
HISTORY:
        1.1     Y.Matsuda   97/01/22

        MCK1.2  M.Karasawa  02/06/21 McKinley    TJIN削除による処理削除
V036.00_02  M.Karasawa 04/01/29 McKinley    AJST未受信時にCJSU読み捨てる不具合の修正(Pre3173)
V037.00_01  M.Karasawa 04/01/29 McKinley    AJST未受信時にCJSU読み捨てる不具合の修正(Pre3173)追加
*/
STATUS  iomnFrontCjsu(HI hJobid, HI hSusFacter, HI hJobtype)
{
    STATUS  Status;     /*  関数戻り値  */
    I32     iConfirm;   /*  中断実行可否確認結果    */
    HI      hJobstatus; /*  JOB状態     */
    I32     iSuscause;  /*  中断要因    */

    /*  JOB状態確認 */
    if(ERROR == (Status = jobStatus(hJobid, &hJobstatus, &iSuscause))){
        iomnDebugMessage(">>>Error:jobStatus()\n",0);
        return(ERROR);
    }
    iomnDebugMessage("###JOB Status->%x\n",hJobstatus);
    
    /*  JOB中断確認処理 */
    iConfirm = iomnSuspendConfirm(hJobid, hJobtype, hJobstatus);
    
    switch(iConfirm){
        case    IOMN_ON:
                /*  JOB中断メッセージ送信   */
                if(ERROR == (Status = iomnSendJobSuspend(hJobid, msgQIdIoExMN, hSusFacter, IOMSG_NOT_USED))){
                    iomnDebugMessage(">>>Error:iomnSendJobSuspend()\n",0);
                    return(ERROR);
                }
                iomnDebugMessage(">>>OK:iomnSendJobSuspend()\n",0);
                break;

        case    IOMN_OFF:
#if 0   /* V036.00_02 */
/**/#if 0   /* MCK1.2 TJINメッセージ削除の為処理を削除  */
/**//**/                /*  JOB中断／中止不成功メッセージ送信   */
/**//**/                if(ERROR == (Status = iomnSendJobCancelFail(hJobid, 
/**//**/                                        ioMNCjsu[hJobid].OldmsgQId ,ioMNCjsu[hJobid].iOldTaskId))){
/**//**/                    iomnDebugMessage(">>>ERROR:iomnSendJobCancelFail() in iomnFrontCjsu()\n",0);
/**//**/                    return(ERROR);
/**//**/                }
/**/#endif
/**/                #ifdef  DEBUG
/**/                    cmnPrintf_ioMN(0,">>>Send TB10 ---> %s\n",taskName(ioMNCjsu[hJobid].iOldTaskId));
/**/                #endif
#else   /* V036.00_02 */
                /* 
                    中断処理できない要因がAJST未受信の場合、
                    中断処理をプールする 。ジョブ終了していないことも条件とする。
                */
#if 0   /* V037.00_01 START */
/**/                if(ioMNAjst[hJobid].iOldTaskId == 0
/**/                || ioMNTjfn[hJobid].iOldTaskId != 0){
#else
                if(ioMNCjst[hJobid].iOldTaskId != 0
                && ioMNAjst[hJobid].iOldTaskId == 0){
#endif  /* V037.00_01 END   */
                    iomnReserveSuspend(hJobid,hSusFacter,IOMSG_NOT_USED);
                }
#endif  /* V036.00_02 */
                break;

        case    ERROR:
                iomnDebugMessage(">>>Error:iomnSuspendConfirm()\n",0);
                return(ERROR);
        default:
                iomnDebugMessage(">>>Status Error of iomnSuspendConfirm()->%x\n",Status);
                iomnDelay(100);
                return(ERROR);
    }
    return(OK);
}


/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         7.13.2
TITLE:      Direct送信JOB中断コマンド受信処理
MODULE：    STATUS  iomnDirectCjsu(HI hJobid, HI hSusFacter, HI hJobtype)
INCLUDE:    ioMNPar.h
            ioMNmsg.h
            ioMNstr.h
PARAMETER:
            HI      hJobid;
            HI      hSusFacter;     中断要因
            HI      hJobtype;       JOB種別
RETURNS:    I32 4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            Direct送信時にJOB中断処理を指示された場合に、JOB中断受付Ackのパラメータを
            テーブルにセットし、受付Ackを送信する。
NOTES:
HISTORY:
        1.1     Y.Matsuda   97/01/22

        MCK1.1  M.Karasawa  02/06/11    McKinley    新メッセージ対応
        MCK1.2  M.Karasawa  02/06/21    McKinley    TJIN削除による処理削除
    V018.00_01  M.Karasawa  03/04/09    McKinley    メッセージ変更対応
*/
STATUS  iomnDirectCjsu(HI hJobid, HI hSusFacter, HI hJobtype)
{
    STATUS  Status;     /*  関数戻り値  */
    I32     iConfirm;   /*  中断実行可否確認結果    */
    HI      hJobstatus; /*  JOB状態     */
    I32     iSuscause;  /*  中断要因    */

    /*  JOB状態確認 */
    if(ERROR == (Status = jobStatus(hJobid, &hJobstatus, &iSuscause))){
        cmnPrintf_ioMN(2,">>>Error:jobStatus()\n");
        printErrno(errno);
        return(ERROR);
    }
    iomnDebugMessage("###JOB Status->%x\n",hJobstatus);
    
    /*  JOB中断確認処理 */
    iConfirm = iomnSuspendConfirm(hJobid, hJobtype, hJobstatus);

    switch(iConfirm){
        case    IOMN_ON:
                /*  ＡＪＳＵ受信パラメータ設定  */
                ioMNAjsu[hJobid].iOldTaskId = iTaskIdFaxMN;
#if 0   /* MCK1.1 新メッセージ対応  */
/**/                ioMNAjsu[hJobid].iOldMsgId = IOMSG_AJSU_JOB;
#else
                ioMNAjsu[hJobid].iOldMsgId = JOB_MSG_AJSU;
#endif
                ioMNAjsu[hJobid].OldmsgQId = msgQIdFaxMN;
                ioMNAjsu[hJobid].msgparam.sAjsuCtl.hJobid       = hJobid;
                ioMNAjsu[hJobid].msgparam.sAjsuCtl.hSusType     = IOMSG_SUS_NORMAL;
                /* V018.00_01 START */
                ioMNAjsu[hJobid].msgparam.sAjsuCtl.hTNo         = ioMNCjst[hJobid].msgparam.sCjstCtl.hTNo;
                ioMNAjsu[hJobid].msgparam.sAjsuCtl.hProcessType = ioMNCjst[hJobid].msgparam.sCjstCtl.hProcessType;
                ioMNAjsu[hJobid].msgparam.sAjsuCtl.hJcbID       = ioMNCjst[hJobid].msgparam.sCjstCtl.hJcbID;
                ioMNAjsu[hJobid].msgparam.sAjsuCtl.hAppliType   = ioMNCjst[hJobid].msgparam.sCjstCtl.hAppliType;
                /* V018.00_01 END   */
                iomnDebugMessage(">>>Set AJSU Message\n",0);

                /*  JOB中断情報格納 */
                ioMNstatus.FrontSuspend.hSusTask = IOMN_ON;
                ioMNstatus.FrontSuspend.hSusType = hSusFacter;
                iomnDebugMessage(">>>SET Job Suspend Table\n",0);
    
                /*  JOB中断受付メッセージ送信   */
                if(ERROR == (Status = iomnSendAjsu(hJobid, IOMSG_SUS_NORMAL))){
                    cmnPrintf_ioMN(2,">>>Error:iomnSendJobSuspend()\n");
                    return(ERROR);
                }
                iomnDebugMessage(">>>OK:iomnSendAjsu()\n",0);
                break;

        case    IOMN_OFF:
#if 0   /* MCK1.2 TJINメッセージ削除の為処理を削除  */
/**/                /*  JOB中断／中止不成功メッセージ送信   */
/**/                if(ERROR == (Status = iomnSendJobCancelFail(hJobid, 
/**/                                        ioMNCjsu[hJobid].OldmsgQId ,ioMNCjsu[hJobid].iOldTaskId))){
/**/                    iomnDebugMessage(">>>ERROR:iomnSendJobCancelFail() in iomnDirectCjsu()\n",0);
/**/                    return(ERROR);
/**/                }
#endif
                #ifdef  DEBUG
                    cmnPrintf_ioMN(0,">>>Send TB10 ---> %s\n",taskName(ioMNCjsu[hJobid].iOldTaskId));
                #endif
                break;

        case    ERROR:
                iomnDebugMessage(">>>Error:iomnSuspendConfirm()\n",0);
                return(ERROR);
        default:
                iomnDebugMessage(">>>Status Error in iomnSuspendConfirm()->%x\n",Status);
                iomnDelay(100);
                return(ERROR);
    }

    return(OK);
}


/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         7.13.3
TITLE:      印刷JOB中断コマンド受信処理
MODULE：    STATUS  iomnPrnCjsu(HI hJobid, HI hSusFacter, HI hJobtype)
INCLUDE:    ioMNPar.h
            ioMNmsg.h
            ioMNstr.h
PARAMETER:
            HI      hJobid;
            HI      hSusFacter;     中断要因
            HI      hJobtype;       JOB種別
RETURNS:    I32 4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            印刷JOB中断指示を受け、JOB中断可否を確認した後、実行管理タスク
            に対してJOB中断コマンドを送信する。
NOTES:
HISTORY:
        1.1     Y.Matsuda   97/01/22
        D2.9    M.Fukuda    00/07/04    DM45/35 FAXリストレポート対応

        MCK1.2  M.Karasawa  02/06/21    McKinley    TJIN削除による処理削除
    V021.00_02  M.Karasawa  03/06/16    McKinley    複写未実行印刷プロセスに対する中断処理タイミング見直し

RIOGRANDE
    Beta5t  M.Karasawa 04/11/24 RioGrande   バックグランド印刷ジョブ取得処理変更(ジョブ連結対応時の処理変更漏れ)

K2
    VTK10.000   M.Karasawa  06/02/08    K2  eB9097対策
*/
STATUS  iomnPrnCjsu(HI hJobid, HI hSusFacter, HI hJobtype)
{
    STATUS  Status;
    I32     iConfirm;
    HI      hJobstatus; /*  JOB状態     */
    I32     iSuscause;  /*  中断要因    */
    HI      hEntryID = 0;   /*  印刷時のQUEID   *//* PRINT_JOINT *//* 中断中断 */
#if 0   /* beta5t */
    HI hrunJobID[IOMN_PJT_NUM];         /* PRINT_JOINT *//* 中断中断 */
    HI hnum;                            /* PRINT_JOINT *//* 中断中断 */
    HI hcjsucount;                      /* PRINT_JOINT *//* 中断中断 */
#endif

    /*  FAXリストレポート作成中の場合  */
    if( hJobtype == COM_JB_LIST_RESULT_SEND ||
        hJobtype == COM_JB_LIST_PRT_FAX ){
        iomnDebugMessage(">>>Error! Background Job(FAX_LIST_PRT_FAX) Still Running!!\n",0);
        return(OK);
    }

    /*  JOB状態確認 */
    if(ERROR == (Status = jobStatus(hJobid, &hJobstatus, &iSuscause))){
        iomnDebugMessage(">>>Error:jobStatus()\n",0);
        return(ERROR);
    }
    iomnDebugMessage("###JOB Status->%x\n",hJobstatus);

    /*  JOB中断確認処理 */
    iConfirm = iomnSuspendConfirm(hJobid, hJobtype, hJobstatus);

    switch(iConfirm){
        case    IOMN_ON:
#if 0   /* PRINT_JOINT *//* 中断中断 */
/* PRINT_JOINT */                /*  EntryID 取得    */
/* PRINT_JOINT */                if(ERROR == (Status = iomnGetEntryIdFromJobtype(hJobtype, &hEntryID))){
/* PRINT_JOINT */                    iomnDebugMessage(">>>Error:iomnGetEntryIdFromJobtype()\n",0);
/* PRINT_JOINT */                    return(ERROR);
/* PRINT_JOINT */                }
/* PRINT_JOINT */                iomnDebugMessage("###ENTRY ID-> %x\n",(I32)hEntryID);
#endif
/* D4.2 START T.Fujii NILEのころから本当は無視していないので処理を削除 */
#if 0   /* OLD */
/**/                /*  DSS印刷以外のＣＳ中断は無視する */
/**/                if(hSusFacter == IOMSG_SUS_CS){
/**/                    iomnDebugMessage(">>>Impossible to Suspend print for CS\n",0);
/**/                    /*  JOB中断／中止不成功メッセージ送信   */
/**/                    if(ERROR == (Status = iomnSendJobCancelFail(hJobid, 
/**/                                        ioMNCjsu[hJobid].OldmsgQId ,ioMNCjsu[hJobid].iOldTaskId))){
/**/                        iomnDebugMessage(">>>ERROR:iomnSendJobCancelFail() in iomnPrnCjsu()\n",0);
/**/                        return(ERROR);
/**/                    }
/**/                    #ifdef  DEBUG
/**/                        cmnPrintf_ioMN(0,">>>Send TB10 ---> %s\n",taskName(ioMNCjsu[hJobid].iOldTaskId));
/**/                    #endif
/**/                }
#endif
/* D4.2 END */
#if 1   /* PRINT_JOINT *//* 中断中断 *//* Beta5t */
                /*  JOB中断メッセージ送信   */
                if(ERROR == (Status = iomnSendJobSuspend(hJobid, msgQIdIoExMN, hSusFacter, hEntryID))){
                    iomnDebugMessage(">>>Error:iomnSendJobSuspend()\n",0);
                    return(ERROR);
                }
                iomnDebugMessage(">>>OK:iomnSendJobSuspend()\n",0);
#else   /* コピー対応時に処理見直してください。入出力中の中断でおかしくなります */
/* PRINT_JOINT */                if(hSusFacter == IOMSG_SUS_CS){
/* PRINT_JOINT */                    if( ioMNPJ_GetRunningJobALL(hrunJobID, &hnum) == ERROR ){
/* PRINT_JOINT */                        iomnDebugMessage("[ioMNPJ] Error:ioMNPJ_GetRunningJobALL() in iomnPrnCjsu()\n",0);
/* PRINT_JOINT */                    }
/* PRINT_JOINT */                    for(hcjsucount = 0; hcjsucount < hnum; hcjsucount++){
/* PRINT_JOINT */                        HI hjobtypel;
/* PRINT_JOINT */                        if(ERROR == (Status = jobJobtype(hrunJobID[hcjsucount], &hjobtypel))){
/* PRINT_JOINT */                            iomnDebugMessage(">>>Error:jobJobtype()\n",0);
/* PRINT_JOINT */                            return(ERROR);
/* PRINT_JOINT */                        }
/* PRINT_JOINT */                        /* ジョブタイプがコピーか割り込み で 中断指示がCS中断時 */
/* PRINT_JOINT */                        if( hjobtypel == COM_JB_EXPPC2 || hjobtypel == COM_JB_IPPC ){
/* PRINT_JOINT */                            if(ERROR == (Status = iomnSendJobSuspend(hrunJobID[hcjsucount], msgQIdIoExMN, hSusFacter, hEntryID))){
/* PRINT_JOINT */                                iomnDebugMessage(">>>Error:iomnSendJobSuspend()\n",0);
/* PRINT_JOINT */                                return(ERROR);
/* PRINT_JOINT */                            }
/* PRINT_JOINT */                            memcpy(&ioMNCjsu[hrunJobID[hcjsucount]], &ioMNCjsu[hJobid], sizeof(IOMNMSG_STORE));
/* PRINT_JOINT */                            /* ジョブIDも書き換える */
/* PRINT_JOINT */                            ioMNCjsu[hrunJobID[hcjsucount]].msgparam.sCjsuCtl.hJobid = hrunJobID[hcjsucount];
/* PRINT_JOINT */                            iomnDebugMessage(">>>OK:iomnSendJobSuspend(%d)\n",hrunJobID[hcjsucount]);
/* PRINT_JOINT */                        }
/* PRINT_JOINT */                    }
/* PRINT_JOINT */                }else{
/* PRINT_JOINT */                    /* CS中断指示で無い場合は、従来どおり該当ジョブの処理を行なう */
/* PRINT_JOINT */                    /*  JOB中断メッセージ送信   */
/* PRINT_JOINT */                    if(ERROR == (Status = iomnSendJobSuspend(hJobid, msgQIdIoExMN, hSusFacter, hEntryID))){
/* PRINT_JOINT */                        iomnDebugMessage(">>>Error:iomnSendJobSuspend()\n",0);
/* PRINT_JOINT */                        return(ERROR);
/* PRINT_JOINT */                    }
/* PRINT_JOINT */                    iomnDebugMessage(">>>OK:iomnSendJobSuspend()\n",0);
/* PRINT_JOINT */                }
#endif
                break;

        case    IOMN_OFF:
#if 0   /* MCK1.2 TJINメッセージ削除の為処理を削除  */
/**/                /*  JOB中断／中止不成功メッセージ送信   */
/**/                if(ERROR == (Status = iomnSendJobCancelFail(hJobid, 
/**/                                        ioMNCjsu[hJobid].OldmsgQId ,ioMNCjsu[hJobid].iOldTaskId))){
/**/                    iomnDebugMessage(">>>ERROR:iomnSendJobCancelFail() in iomnPrnCjsu()\n",0);
/**/                    return(ERROR);
/**/                }
#endif
		{
		        QUE_LIST_PARAM sParam_q;
		        HI             hCount = 0;
		        QUE_LIST_INFO  sInfo_q[IOMN_MAX_PRNJOINT];
		        sParam_q.hNum    = 1;                                   /* .ﾂｽ.ﾃﾃﾃｰﾃｨ.ﾂｷ.ﾂｩ? */
		        sParam_q.hGetCnt = IOMN_MAX_PRNJOINT;   /* .ﾂ｢.ﾂｭ.ﾃﾃｦ<8f>o.ﾂｷ.ﾂｩ?   */
		        if(ERROR == quePrnLstInfo(PRNQUE_LST_FNC_ALL,&sParam_q,&hCount,sInfo_q)){
		                iomnDebugMessage("[ioMN] ERROR : quePrnLstInfo() in iomnPrnJointSearch()\n",0);
                        return(ERROR);
                }
		/* 20110815 -- STFR_10078 - ((hJobstatus == COM_JS_SUSPEND) && (ioMNstatus.eng_recoversts != 0 && ioMNstatus.eng_recoversts != IOMN_UNDEF)) condition 
		    is added for the scenario: SCAN and PRINT are suspended due to jam, after jam is cleared, CJST is received for SCAN and PRINT. CJST-Scan is sent to ioExMN, but CJST-Print is not sent as printer warm-up is going on. Now, if CJSU for PRINT is received; and if the print job status is COM_JS_SUSPEND (spontaneous suspended) and the printer is not warmed up yet, condition is added to send TJSU from ioMN itself */
			if(((hJobstatus == COM_JS_SUSPEND) && (ioMNstatus.eng_recoversts != 0 && ioMNstatus.eng_recoversts != IOMN_UNDEF))
				||( (hJobstatus == COM_JS_PRT_WAITING)&&(1 == hCount) && (COM_JB_EXPPC2 == hJobtype)))
		{
			if(ERROR == (Status = iomnSendTjsu(JOB_MSG_TJSU, hJobid, IOMSG_SUS_OK, IOMSG_NOT_USED, IOMSG_NOT_USED, IOMSG_NOT_USED)))
			{
				cmnPrintf_ioMN(8,">>>Error:iomnSendTjsu() in iomnPrnCjsu()\n");
				return(ERROR);
			}
			iomnDebugMessage(">>>OK:iomnSendTjsu() in iomnPrnCjsu()\n",0);
			if(ERROR == (Status = jobSetStatus(hJobid,COM_JS_SUSPEND,IOMSG_SUS_CS)))
			{
				cmnPrintf_ioMN(8,">>>Error:jobSetStatus() in iomnSendJobSuspend()\n");
			}
			cmnPrintf_ioMN(7,">>>OK:jobSetStatus(%d, %d, %x) in iomnPrnCjsu()\n", hJobid, COM_JS_SUSPEND, IOMSG_SUS_CS);
		}
	               else  if((hJobstatus == COM_JS_READY) || (hJobstatus == COM_JS_PRT_WAITING)){
	                    if(ERROR == (Status = iomnSendTjsu(JOB_MSG_TJSU, hJobid, IOMSG_SUS_ERROR, (I32)(M_f220IoMNErr | IOMSG_JB_NOT_RUN), IOMSG_NOT_USED, IOMSG_NOT_USED))){
	                        cmnPrintf_ioMN(2,">>>Error:iomnSendTjsu() in iomnPrnCjsu()\n");
	                        return(ERROR);
	                    }
	                    iomnDebugMessage(">>>OK:iomnSendTjsu() in iomnPrnCjsu()\n",0);
	                }
                /* V021.00_02 END   */
                memset(&ioMNCjsu[hJobid], 0x00, sizeof(struct msgstore));   /* VTK10.000 Add */
                #ifdef  DEBUG
                    cmnPrintf_ioMN(0,">>>Send TB10 ---> %s\n",taskName(ioMNCjsu[hJobid].iOldTaskId));
                #endif
                break;
	    	}
        case    ERROR:
                iomnDebugMessage(">>>Error:iomnSuspendConfirm()\n",0);
                return(ERROR);
        default:
                iomnDebugMessage(">>>Status Error of iomnSuspendConfirm()->%x\n",Status);
                iomnDelay(100);
                return(ERROR);
    }
    return(OK);
}


/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         7.13.3.1
TITLE:      EntryID取得処理
MODULE：    STATUS  iomnGetEntryIdFromJobtype(HI hJobtype, HI *pEntryId)
INCLUDE:    ioMNPar.h
            ioMNmsg.h
            ioMNstr.h
PARAMETER:
            HI      hJobtype;       JOB種別
            HI  (o)*pEntryId;
RETURNS:    I32 4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            印刷JOBのJOB種別より、中断状態管理テーブルに情報を登録する際必要となる
            EntryIDを取得する
NOTES:
HISTORY:
        1.1     Y.Matsuda   97/04/14
        M1.1(TJ)H.Takahashi 98/09/28
                DSS逐次対応
        D1.1    T.Fujii     99/11/15
                DM45/35 後追い逐次・並行動作対応
        D1.4    Y.Shimada  '00/ 1/25
                DM35/45対応(ＩｐＬｉｂ／逐次制御の組み込み)
        D1.6    T.Fujii     00/02/09 DM45/35 エラーの場合IOMN_UNDEF(=-1)を返すように変更
        D1.7.1 J.Ootani 00/03/01    DM45/35 SEINE FAX F-CODE対応(マージ)
        D2.9   M.Fukuda 00/07/04    DM45/35 FAXリストレポート対応
*/
STATUS  iomnGetEntryIdFromJobtype(HI hJobtype, HI *pEntryId)
{
    iomnDebugMessage(">>>Top of iomnGetEntryIdFromJobtype()<<<\n",0);
    iomnDebugMessage("###JOB TYPE -> %x\n",(I32)hJobtype);
    switch(hJobtype){
        case COM_JB_EXPPC2:
        case COM_JB_IPPC:
        case COM_JB_IVPRM_PPC:
            *pEntryId = PRNQUE_CPY_NORMAL;
            break;
        case COM_JB_PRT_GDI1:
        case COM_JB_PRT_GDI2:
        case COM_JB_DRC_PRT_GDI:
            *pEntryId = PRNQUE_GDI;
            break;
        case COM_JB_PRT_FAX1:
        case COM_JB_PRT_FAX2:
        case COM_JB_POLL_RECV_FAX1:
        case COM_JB_POLL_RECV_FAX2:
        case COM_JB_DRC_PRT_FAX:
            *pEntryId = PRNQUE_FAX;
            break;
        case COM_JB_LIST_PRT_FAX:
            *pEntryId = PRNQUE_LIST;
            break;
        case COM_JB_LIST_RESULT_SEND:
            *pEntryId = IOMN_RESULTSNDQUE_FAX;
            break;
        case    COM_JB_CON_GET_FAX2:
        case    COM_JB_BOD_GET_FAX2:
            *pEntryId = PRNQUE_LIST;
            break;
        case COM_JB_RECV_DSI:
        case COM_JB_RECV_DSI_SYNC:
            *pEntryId = PRNQUE_DSS_NORMAL;
            break;
        case COM_JB_PRIVATE_DSI:
        case COM_JB_PRIVATE_DSI_SYNC:
            *pEntryId = PRNQUE_DSS_PRIVATE;
            break;
        default:
            iomnDebugMessage(">>>Unreasonable Job Type\n",0);
            *pEntryId = IOMN_UNDEF;
            return(ERROR);
    }

    iomnDebugMessage(">>>Bottom of iomnGetEntryIdFromJobtype()<<<\n",0);
    return(OK);
}


/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         7.13.5
TITLE:      Dss印刷JOB中断コマンド受信処理
MODULE：    STATUS  iomnDssPrnCjsu(HI hJobid, HI hSusFacter, HI hJobtype)
INCLUDE:    ioMNPar.h
            ioMNmsg.h
            ioMNstr.h
PARAMETER:
            HI      hJobid;
            HI      hSusFacter;     中断要因
            HI      hJobtype;       JOB種別
RETURNS:    I32 4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            Dss印刷JOB中断指示を受け、JOB状態を確認した後、実行管理タスク
            に対してJOB中断コマンドを送信する。
NOTES:
HISTORY:
        1.1     Y.Matsuda   97/01/22
        V3.0    J.Ootani    97/12/02
             ＤＳＳ印刷ＪＯＢ実行中の機能切り替え中断対応
        V5.0D   yMatsuda    98/04/08
            ＤＳＳ中断処理中の中断指示受信対応追加→Ｑｕｅinｇするよう修正

        MCK1.2  M.Karasawa  02/06/21 McKinley    TJIN削除による処理削除
*/
STATUS  iomnDssPrnCjsu(HI hJobid, HI hSusFacter, HI hJobtype)
{
    STATUS  Status;     /*  関数戻り値  */
    HI      hJobstatus; /*  JOB状態     */
    HI      hEntryID = 0;   /*  印刷時のQUEID   *//* PRINT_JOINT *//* 中断中断 */
    I32     iSuscause;  /*  中断要因    */
    I32     iConfirm;   /*  確認結果    */
/*  HI      hrunJobID[IOMN_PJT_NUM];    *//* PRINT_JOINT *//* 中断中断 */												/* SM-001 */
/*  HI      hnum;                       *//* PRINT_JOINT *//* 中断中断 */												/* SM-001 */
/*  HI      hcjsucount;                 *//* PRINT_JOINT *//* 中断中断 */												/* SM-001 */


    /*  JOB状態確認 */
    if(ERROR == (Status = jobStatus(hJobid, &hJobstatus, &iSuscause))){
        cmnPrintf_ioMN(2,">>>Error:jobStatus()\n");
        printErrno(errno);
    }
    iomnDebugMessage("###JOB Status->%x\n",(I32)hJobstatus);
    iomnDebugMessage("###JOB SUSPEND TYPE->%x\n",iSuscause);

    /*  JOB中断確認処理 */
    /*  ＣＳ中断以外の要因で中断中にＣＳ中断を指示された場合の処理  */
    if(iSuscause != IOMSG_SUS_CS && hSusFacter == IOMSG_SUS_CS) {
        /*  JOB開始指示前またはJOB完了後に中断を指示された場合は中断不可能  */
        if(ioMNTjfn[hJobid].iOldTaskId != 0
           || ioMNCjst[hJobid].iOldTaskId == 0){
            iConfirm = IOMN_OFF;
        }
        /*  JOB実行中にＣＳ中断を指示された場合はＣＳ中断を実行 */
        else {
            iConfirm = IOMN_ON;
        }
    }
    else {
        iConfirm = iomnSuspendConfirm(hJobid, hJobtype, hJobstatus);
    }
    switch(iConfirm){
        case    IOMN_ON:
#if 0   /* PRINT_JOINT *//* 中断中断 */
/* PRINT_JOINT */                /*  EntryID 取得    */
/* PRINT_JOINT */                if(ERROR == (Status = iomnGetEntryIdFromJobtype(hJobtype, &hEntryID))){
/* PRINT_JOINT */                    iomnDebugMessage(">>>Error:iomnGetEntryIdFromJobtype()\n",0);
/* PRINT_JOINT */                    return(ERROR);
/* PRINT_JOINT */                }
/* PRINT_JOINT */                iomnDebugMessage("###ENTRY ID-> %x\n",(I32)hEntryID);
/* PRINT_JOINT */                /*  印刷中断処理中の場合    */
/* PRINT_JOINT */                if(ioMNstatus.BackSuspend[hEntryID].hSusTask != 0){
/* PRINT_JOINT */                    /*  中断処理をQueingする    */
/* PRINT_JOINT */                    ioMNBackDssSusQue.hJobid = hJobid;
/* PRINT_JOINT */                    ioMNBackDssSusQue.hSusFacter = hSusFacter;
/* PRINT_JOINT */                    ioMNBackDssSusQue.hEntryId = hEntryID;
/* PRINT_JOINT */                    cmnPrintf_ioMN(0,"###Queing Suspend JOBID => %x\n",(I32)ioMNBackDssSusQue.hJobid);
/* PRINT_JOINT */                    cmnPrintf_ioMN(0,"###Queing Suspend Facter => %x\n",(I32)ioMNBackDssSusQue.hSusFacter);
/* PRINT_JOINT */                    cmnPrintf_ioMN(0,"###Queing Suspend EntryID => %x\n",(I32)ioMNBackDssSusQue.hEntryId);
/* PRINT_JOINT */                    iomnDebugMessage(">>>Suspend Command was Queing\n",0);
/* PRINT_JOINT */                    return(OK);
/* PRINT_JOINT */                }
#endif
#if 0   /* PRINT_JOINT *//* 中断中断 */
/* PRINT_JOINT */                /*  JOB中断メッセージ送信   */
/* PRINT_JOINT */                if(ERROR == (Status = iomnSendJobSuspend(hJobid, msgQIdIoExMN, hSusFacter, hEntryID))){
/* PRINT_JOINT */                    iomnDebugMessage(">>>Error:iomnSendJobSuspend()\n",0);
/* PRINT_JOINT */                    return(ERROR);
/* PRINT_JOINT */                }
#else

	#if 0
                if( ioMNPJ_GetRunningJobALL(hrunJobID, &hnum) == ERROR ){
                    iomnDebugMessage("[ioMNPJ] Error:ioMNPJ_GetRunningJobALL() in iomnPrnCjsu()\n",0);
                }
                for(hcjsucount = 0; hcjsucount < hnum; hcjsucount++){
                    if(ERROR == (Status = iomnSendJobSuspend(hrunJobID[hcjsucount], msgQIdIoExMN, hSusFacter, hEntryID))){
                        iomnDebugMessage(">>>Error:iomnSendJobSuspend()\n",0);
                        return(ERROR);
                    }
                    memcpy(&ioMNCjsu[hrunJobID[hcjsucount]], &ioMNCjsu[hJobid], sizeof(IOMNMSG_STORE));
                    /* ジョブIDも書き換える */
                    ioMNCjsu[hrunJobID[hcjsucount]].msgparam.sCjsuCtl.hJcbID = hrunJobID[hcjsucount];
                    iomnDebugMessage(">>>OK:iomnSendJobSuspend()\n",0);
                }
	#else
		 if(ERROR == (Status = iomnSendJobSuspend(hJobid, msgQIdIoExMN, hSusFacter, hEntryID))){
                        iomnDebugMessage(">>>Error:iomnSendJobSuspend()\n",0);
                        return(ERROR);
                    }
	#endif
#endif
                break;

        case    IOMN_OFF:
#if 0   /* MCK1.2 TJINメッセージ削除の為処理を削除  */
/**/                /*  JOB中断／中止不成功メッセージ送信   */
/**/                if(ERROR == (Status = iomnSendJobCancelFail(hJobid, 
/**/                                        ioMNCjsu[hJobid].OldmsgQId ,ioMNCjsu[hJobid].iOldTaskId))){
/**/                    iomnDebugMessage(">>>ERROR:iomnSendJobCancelFail() in iomnDssPrnCjsu()\n",0);
/**/                    return(ERROR);
/**/                }
#endif
			/*Changes  done for FCH S5055C MS009639 Lock_occur_by_interruption -- roopesh*/
			if(hJobstatus == COM_JS_PRT_WAITING){
			        if(ERROR == (Status = iomnSendTjsu(JOB_MSG_TJSU, hJobid, IOMSG_SUS_ERROR,
	                                           (I32)(M_f220IoMNErr | IOMSG_JB_NOT_RUN), IOMSG_NOT_USED, IOMSG_NOT_USED))){
	        		    cmnPrintf_ioMN(2,">>>Error:iomnSendTjsu() in iomnSendJobSuspend()\n");
	        		}
			}
                #ifdef  DEBUG
                    cmnPrintf_ioMN(0,">>>OK:Send TB10 ---> %s\n",taskName(ioMNCjsu[hJobid].iOldTaskId));
                #endif
                break;

        case    ERROR:
                iomnDebugMessage(">>>Error:iomnSuspendConfirm()\n",0);
                return(ERROR);
        default:
                iomnDebugMessage(">>>Status Error of iomnSuspendConfirm()->%x\n",Status);
                iomnDelay(100);
                return(ERROR);
    }

    return(OK);
}


/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         7.13.6
TITLE:      Dss入力JOB中断コマンド受信処理
MODULE：    STATUS  iomnFrontDssCjsu(HI hJobid, HI hSusFacter, HI hJobtype)
INCLUDE:    ioMNPar.h
            ioMNmsg.h
            ioMNstr.h
PARAMETER:
            (i)HI      hJobid;
            (i)HI      hSusFacter;     中断要因
            (i)HI      hJobtype;       JOB種別
RETURNS:    I32 4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            ＤＳＳ入力JOBにたいして中断指示を受信した場合の処理
NOTES:
HISTORY:
        1.1     Y.Matsuda   98/04/10

        MCK1.2  M.Karasawa  02/06/21 McKinley    TJIN削除による処理削除
    V037.00_01  M.Karasawa 04/01/29 McKinley    AJST未受信時にCJSU読み捨てる不具合の修正(Pre3173)追加
*/
STATUS  iomnFrontDssCjsu(HI hJobid, HI hSusFacter, HI hJobtype)
{
    STATUS  Status;     /*  関数戻り値  */
    I32     iConfirm;   /*  中断実行可否確認結果    */
    HI      hJobstatus; /*  JOB状態     */
    I32     iSuscause;  /*  中断要因    */

    /*  JOB状態確認 */
    if(ERROR == (Status = jobStatus(hJobid, &hJobstatus, &iSuscause))){
        iomnDebugMessage(">>>Error:jobStatus()\n",0);
        return(ERROR);
    }
    iomnDebugMessage("###JOB Status->%x\n",hJobstatus);
    
    /*  JOB中断確認処理 */
    iConfirm = iomnSuspendConfirm(hJobid, hJobtype, hJobstatus);
    
    switch(iConfirm){
        case    IOMN_ON:
                /*  JOB中断メッセージ送信   */
                if(ERROR == (Status = iomnSendJobSuspend(hJobid, msgQIdIoExMN, hSusFacter, IOMSG_NOT_USED))){
                    iomnDebugMessage(">>>Error:iomnSendJobSuspend()\n",0);
                    return(ERROR);
                }
                iomnDebugMessage(">>>OK:iomnSendJobSuspend()\n",0);
                break;

        case    IOMN_OFF:
#if 0   /* MCK1.2 TJINメッセージ削除の為処理を削除  */
/**/                /*  JOB中断／中止不成功メッセージ送信   */
/**/                if(ERROR == (Status = iomnSendJobCancelFail(hJobid, 
/**/                                        ioMNCjsu[hJobid].OldmsgQId ,ioMNCjsu[hJobid].iOldTaskId))){
/**/                    iomnDebugMessage(">>>ERROR:iomnSendJobCancelFail() in iomnFrontCjsu()\n",0);
/**/                    return(ERROR);
/**/                }
#endif
#if 0   /* V037.00_01 START */
/**/                #ifdef  DEBUG
/**/                    cmnPrintf_ioMN(0,">>>Send TB10 ---> %s\n",taskName(ioMNCjsu[hJobid].iOldTaskId));
/**/                #endif
#else
                if(ioMNCjst[hJobid].iOldTaskId != 0
                && ioMNAjst[hJobid].iOldTaskId == 0){
                    iomnReserveSuspend(hJobid,hSusFacter,IOMSG_NOT_USED);
                }
#endif  /* V037.00_01 END   */
                break;

        case    ERROR:
                iomnDebugMessage(">>>Error:iomnSuspendConfirm()\n",0);
                return(ERROR);
        default:
                iomnDebugMessage(">>>Status Error of iomnSuspendConfirm()->%x\n",Status);
                iomnDelay(100);
                return(ERROR);
    }
    return(OK);
}


/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         7.14
TITLE:      JOB中断受付け受信処理
MODULE：    STATUS  iomnRecieveAJSU()
INCLUDE:    ioMNPar.h
            ioMNmsg.h
            ioMNstr.h
PARAMETER:
            HI      hJobid;
RETURNS:    I32 4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            ＪＯＢ中断処理のACKをioExMNより受け取り、
NOTES:
HISTORY:
        1.1     Y.Matsuda   97/01/22
        M1.1(TJ)H.Takahashi 98/09/28
                DSS逐次対応
        D1.7.1 J.Ootani 00/03/01    DM45/35 SEINE FAX F-CODE対応(マージ)
        D2.9   M.Fukuda 00/07/04    DM45/35 FAXリストレポート対応
        D2.0.2 T.Fujii  00/07/04    DM45/35 NOP_DEL(ソースコード整理)
                                            if文,case文でのCOM_JB_EXPPC1を削除
        D3.19.2 T.Fujii 00/09/18    DM45/35 AJSU送信処理修正
        D3.19.4 T.Fujii 00/09/25    DM45/35 AJSU送信処理再修正
*/
STATUS  iomnRecieveAJSU()
{
    STATUS  Status;         /*  各種処理結果    */
    HI      hJobid;         /*  JOBID   */
    HI      hSustype;       /*  中断タイプ  */
    HI      hJobtype;       /*  JOB種別 */
    /* D3.19.2 START T.Fujii AJSU送信処理修正 */
    HI      hSubJobmode = -1;   /* サブジョブ種別 */
    /* D3.19.2 END */

    /*  パラメータ取得  */
    hJobid = ioMNrmyparam.sAjsuCtl.hJobid;
    hSustype = ioMNrmyparam.sAjsuCtl.hSusType;
    iomnDebugMessage("### JOBID --> %x\n",(I32)hJobid);
    iomnDebugMessage("### Suspend Type --> %x\n",(I32)hSustype);

    /*  JOB種別取得 */
    if(ERROR == (Status = jobJobtype(hJobid, &hJobtype))){
        iomnDebugMessage(">>>Error:jobJobtype()\n",0);
        return(ERROR);
    }
    iomnDebugMessage("###SUSPEND JOB TYPE->%x\n",(I32)hJobtype);
    
    /*  中断メッセージ格納  */
    iomnSetJobManagementMsg(&ioMNAjsu[hJobid]);
    iomnDebugMessage(">>>OK:Get AJSU Message\n",0);

    /*  JOB種別中断処理 */
    switch(hJobtype){
        case COM_JB_IPPC:
        case COM_JB_EXPPC2:
/* D3.19.4 START T.FUjii AJSU送信処理再修正 */
/* D3.19.2 START T.Fujii AJSU送信処理修正 */
#if 1   /* OLD(D3.19.2) -> USE(D3.19.4) */
                if(ERROR == (Status = iomnPPCAjsu(hJobid, hSustype, hJobtype))){
                    iomnDebugMessage(">>>Error:iomnPPCAjsu()\n",0);
                    return(ERROR);
                }
                iomnDebugMessage(">>>OK:iomnPPCAjsu()\n",0);
                break;
#endif
/* D3.19.4 END */
/* D3.19.4 START T.FUjii AJSU送信処理再修正 */
#if 1   /* D3.19.2 NEW */
        case COM_JB_RECV_DSI:
        case COM_JB_PRIVATE_DSI:
        case COM_JB_RECV_DSI_SYNC:
        case COM_JB_PRIVATE_DSI_SYNC:
                /* サブジョブ種別を取得 */
                if( parMNSubJobmode(hJobid, &hSubJobmode) == ERROR ) {
                    puts( "[ioMN]Error:parMNSubJobmode() in iomnRecieveAJSU()" );
                    return( ERROR );
                }

                if( hSubJobmode == COM_JB_SUB_OUTPUT ) {    /* サブジョブ種別が出力のとき */
                    if( iomnPrnAjsu(hJobid, hSustype, hJobtype) == ERROR ) {
                        puts( "[ioMN]Error:iomnPrnAjsu() in iomnRecieveAJSU()" );
                        return(ERROR);
                    }
                    #ifdef DEBUG
                    puts( "[ioMN]OK:iomnPrnAjsu() in iomnRecieveAJSU()" );
                    #endif
                    break;
                }
                /* サブジョブ種別が入力のときは下記の処理を行なう */
                /* (※ 複写ジョブのときもiomnFrontAjsu()を使う） */
#endif
/* D3.19.2 END */
/* D3.19.4 END */
        case COM_JB_SCN_FAX1:
        case COM_JB_SCN_FAX2:
        case COM_JB_DRC_SCN_FAX:
        case COM_JB_POLL_SEND_FAX1:
        case COM_JB_POLL_SEND_FAX2:
        case COM_JB_IVSAVE_PPC:
        case COM_JB_IVSAVE_FAX:
        case COM_JB_SEND_DSI:
        case COM_JB_CON_REG_FAX2:
        case COM_JB_BOD_REG_FAX2:
        case COM_JB_CON_GET_FAX2:
        case COM_JB_BOD_GET_FAX2:
                if(ERROR == (Status = iomnFrontAjsu(hJobid, hSustype, hJobtype))){
                    iomnDebugMessage(">>>Error:iomnFrontAjsu()\n",0);
                    return(ERROR);
                }
                iomnDebugMessage(">>>OK:iomnFrontAjsu()\n",0);
                break;
        case COM_JB_PRT_FAX1:
        case COM_JB_PRT_FAX2:
        case COM_JB_POLL_RECV_FAX1:
        case COM_JB_POLL_RECV_FAX2:
        case COM_JB_PRT_GDI1:
        case COM_JB_PRT_GDI2:
        case COM_JB_IVPRM_PPC:
        case COM_JB_IVPRM_FAX:
        case COM_JB_DRC_PRT_FAX:
        case COM_JB_DRC_PRT_GDI:
/* D3.19.2 START T.Fujii AJSU送信処理修正 */
#if 0   /* OLD */
        case COM_JB_RECV_DSI:
        case COM_JB_PRIVATE_DSI:
        case COM_JB_RECV_DSI_SYNC:
        case COM_JB_PRIVATE_DSI_SYNC:
#endif
/* D3.19.2 END */
                if(ERROR == (Status = iomnPrnAjsu(hJobid, hSustype, hJobtype))){
                    iomnDebugMessage(">>>Error:iomnPrnAjsu()\n",0);
                    return(ERROR);
                }
                iomnDebugMessage(">>>OK:iomnPrnAjsu\n",0);
                break;
        case COM_JB_LIST_PRT_FAX:
                /*  フロント操作によるリスト印刷JOB中断 */
                if(hJobid == hioMNFrontJobID){
                    if(ERROR == (Status = iomnFrontAjsu(hJobid, hSustype, hJobtype))){
                        iomnDebugMessage(">>>Error:iomnFrontAjsu()\n",0);
                        return(ERROR);
                    }
                    iomnDebugMessage(">>>OK:iomnFrontAjsu()\n",0);
                }else{
                /*  バックグランドJOB時のリスト印刷JOB中断  */
                    if(ERROR == (Status = iomnPrnAjsu(hJobid, hSustype, hJobtype))){
                        iomnDebugMessage(">>>Error:iomnPrnAjsu()\n",0);
                        return(ERROR);
                    }
                    iomnDebugMessage(">>>OK:iomnPrnAjsu\n",0);
                }
                break;
        case COM_JB_LIST_RESULT_SEND:
                if(ERROR == (Status = iomnPrnAjsu(hJobid, hSustype, hJobtype))){
                    iomnDebugMessage(">>>Error:iomnPrnAjsu()\n",0);
                    return(ERROR);
                }
                break;
        case COM_JB_LIST_SEND_FAX:
                break;
        case COM_JB_JOB_UI:
                if(ERROR == (Status = iomnJobUIAjsu(hJobid, hSustype, hJobtype))){
                    iomnDebugMessage(">>>Error:iomnJobUIAjsu()\n",0);
                    return(ERROR);
                }
                break;
        case COM_JB_UNDEF:
        case COM_JB_DIG:
        default:
                iomnDebugMessage(">>>ERROR:Unreasonable JOB TYPE!!!\n",0);
                return(ERROR);
    }
    return(OK);
}


/* D3.19.4 START T.Fujii AJSU送信処理再修正(使用することに戻す) */
/* D3.19.2 START T.Fujii AJSU送信処理修正(iomnFrontAjsuを使用するので使わない) */
#if 1   /* OLD(D3.19.2) -> USE(D3.19.4) */
/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         7.13.1
TITLE:      PPCJOB中断ack受信処理
MODULE：    STATUS  iomnPPCAjsu(HI hJobid, HI hSustype, HI hJobtype)
INCLUDE:    ioMNPar.h
            ioMNmsg.h
            ioMNstr.h
PARAMETER:
            HI      hJobid;
RETURNS:    I32 4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            中断受け付けメッセージを受信し、上位層に対して受け付けメッセージを送信する。
NOTES:
HISTORY:
        1.1     Y.Matsuda   97/01/22
    V010.00_01  M.Karasawa  02/12/05    McKinley    割り込み対応(シーケンス変更)
    V016.00_02  M.Karasawa  03/03/26    McKinley    メッセージ置換                  構造体置換
                                                    JOB_MSG_CCIC -> JOB_MSG_CPSW    JOB_STR_CCIC -> JOB_STR_CPSW
                                                    JOB_MSG_TCIC -> JOB_MSG_TPSW    JOB_STR_TCIC -> JOB_STR_TPSW
                                                    新規メッセージ
                                                    JOB_MSG_EPDS                    JOB_STR_ECIC -> JOB_STR_EPDS
*/
STATUS  iomnPPCAjsu(HI hJobid, HI hSustype, HI hJobtype)
{
    STATUS  Status;     /*  各種処理結果    */
    #ifdef  DEBUG
    HI      hJobstatus; /*  JOB状態         */
    I32     iSuscause;  /*  中断要因        */
    #endif
    HI      hIntUIType;  /*  バッファリングされている優先度変更事象のタイプ  */
    HI      hIntUI;      /*  バッファリングされている優先ＵＩ    */
    HI      hIntUIMode;  /*  バッファリングされている優先度変更事象のモード  */
    HI      hErrorUI;   /*  Error復帰先UI   */

    iomnDebugMessage(">>>Top of iomnPPCAjsu()<<<\n",0);

    iomnDebugMessage("###JOBID->%x\n",(I32)hJobid);
    iomnDebugMessage("###Suspend Type->%x\n",(I32)hSustype);
    iomnDebugMessage("###JOB TYPE->%x\n",(I32)hJobtype);

    /* ioMNからの中断処理か確認 */                                  /* V010.00_01   */
    if( ioMNCjsu[hJobid].iOldTaskId != 0 ){                     /* V010.00_01   */
        /*  UIに対してJOB受け付けメッセージを送信   */
        if(ERROR == (Status = iomnSendAjsu(hJobid, hSustype))){
            iomnDebugMessage(">>>Error;iomnSendAjsu()\n",0);
            return(ERROR);
        }
        iomnDebugMessage(">>>OK:iomnSendAjsu()\n",0);
        /* V016.00_02 START */
        if( hSustype == IOMSG_SUS_DELAY ){                          
            if(ERROR == (Status = iomnSendEpds(hJobid, IOMSG_SUSDELAY_STOP, ioMNCjst[hJobid].msgparam.sCjstCtl.hTNo, ioMNCjst[hJobid].msgparam.sCjstCtl.hProcessType))){
                iomnDebugMessage(">>>Error;iomnSendEpds()\n",0);
                return(ERROR);
            }
            iomnDebugMessage(">>>OK:iomnSendEpds()\n",0);
        }
        /* V016.00_02 END   */
    }else{                                                          /* V010.00_01   */
        /* 遅延中断時はCMPanelに対してイベントを送信する    */      /* V010.00_01   */
        if( hSustype == IOMSG_SUS_DELAY ){                          /* V010.00_01   */
            /* V016.00_02 START */
            if(ERROR == (Status = iomnSendEpds(hJobid, IOMSG_SUSDELAY_INT, ioMNCjst[hJobid].msgparam.sCjstCtl.hTNo, ioMNCjst[hJobid].msgparam.sCjstCtl.hProcessType))){
                iomnDebugMessage(">>>Error;iomnSendEpds()\n",0);
                return(ERROR);
            }
            iomnDebugMessage(">>>OK:iomnSendEpds()\n",0);
            /* V016.00_02 END   */
        }                                                           /* V010.00_01   */
    }                                                               /* V010.00_01   */

    /*  遅延中断時には遅延中断中フラグを立てる  */
    if(hSustype == IOMSG_SUS_DELAY){
        ioMNstatus.hSusDelay = IOMN_ON;
        /*  Error発生中の場合   */
        if(ioMNstatus.hError_sts != 0){
            /*  Error復帰先情報に割り込み復帰先情報をコピーする */
            if(ERROR != iomnGetChangeUIQue(&ioMNUiIntBuf, &hIntUIType, &hIntUI, &hIntUIMode)){
                hErrorUI = hIntUI;
                /*  Error復帰先情報セット   */
                iomnSetChangeUIQue(&ioMNUiErrClrBuf, IOMN_CPRI_ERROR_CLEAR, hErrorUI, IOMN_CPRI_ERROR_CLEAR);
                iomnDebugMessage(">>>Set Error Clear Information\n",0);
            }
        }
        iomnDebugMessage(">>>OK:IOMSG_SUS_DELAY\n",0);
    }

    #ifdef  DEBUG
        /*  JOB状態取得 */
        if(ERROR == (Status = jobStatus(hJobid, &hJobstatus, &iSuscause))){
            iomnDebugMessage(">>>Error:jobStatus()\n",0);
            printErrno(errno);
            return(ERROR);
        }
        iomnDebugMessage("###JOB STATUS -->%x\n",(I32)hJobstatus);
    #endif
    iomnDebugMessage(">>>Bottom of iomnPPCAjsu()<<<\n",0);
    return(OK);
}
#endif
/* D3.19.2 END */
/* D3.19.4 END */


/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         7.13.2
TITLE:      FrontJOB中断ack受信処理
MODULE：    STATUS  iomnFrontAjsu(HI hJobid, HI hSustype, HI hJobtype)
INCLUDE:    ioMNPar.h
            ioMNmsg.h
            ioMNstr.h
PARAMETER:
            HI      hJobid;
RETURNS:    I32 4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            中断受け付けメッセージを受信し、上位層に対して受け付けメッセージを送信する。
            但し、自発的に行った中断の場合は送信しない。
NOTES:
HISTORY:
        1.1     Y.Matsuda   97/01/22

    V007.00_03  M.Karasawa  02/09/02    McKinley    AJSU送信処理修正
    V010.00_01  M.Karasawa  02/12/05    McKinley    割り込み対応(シーケンス変更)
*/
STATUS  iomnFrontAjsu(HI hJobid, HI hSustype, HI hJobtype)
{
    STATUS  Status;     /*  各種処理結果    */
    #ifdef  DEBUG
    HI      hJobstatus; /*  JOB状態         */
    I32     iSuscause;  /*  中断要因        */
    #endif
    HI      hIntUIType;  /*  バッファリングされている優先度変更事象のタイプ  */
    HI      hIntUI;      /*  バッファリングされている優先ＵＩ    */
    HI      hIntUIMode;  /*  バッファリングされている優先度変更事象のモード  */
    HI      hErrorUI;   /*  Error復帰先UI   */

    iomnDebugMessage(">>>Top of iomnFrontAjsu()<<<\n",0);

    iomnDebugMessage("###JOBID->%x\n",(I32)hJobid);
    iomnDebugMessage("###Suspend Type->%x\n",(I32)hSustype);
    iomnDebugMessage("###JOB TYPE->%x\n",(I32)hJobtype);
#if 1   /* V007.00_03   *//* AJSUをioMNで止めるシーケンスはない */
        /* V010.00_01   */
    if(ioMNCjsu[hJobid].iOldTaskId != 0){ 
#endif
        /*  UIに対してJOB受け付けメッセージを送信   */
        if(ERROR == (Status = iomnSendAjsu(hJobid, hSustype))){
            iomnDebugMessage(">>>Error;iomnSendAjsu()\n",0);
            return(ERROR);
        }
        iomnDebugMessage(">>>OK:iomnSendAjsu()\n",0);
#if 1   /* V007.00_03   *//* AJSUをioMNで止めるシーケンスはない */
        /* V0009.00_04  */
    }
    else{
        if(hSustype == IOMSG_SUS_DELAY){                    /* V010.00_01   */
        /* イベント送信処理 */                              /* V010.00_01   */
        }                                                   /* V010.00_01   */
        iomnDebugMessage(">>>Not Receive CJSU\n",0);
    }
#endif

    /*  遅延中断時には遅延中断中フラグを立てる  */
    if(hSustype == IOMSG_SUS_DELAY){
        ioMNstatus.hSusDelay = IOMN_ON;
        /*  Error発生中の場合   */
        if(ioMNstatus.hError_sts != 0){
            /*  Error復帰先情報に割り込み復帰先情報をコピーする */
            if(ERROR != iomnGetChangeUIQue(&ioMNUiIntBuf, &hIntUIType, &hIntUI, &hIntUIMode)){
                hErrorUI = hIntUI;
                /*  Error復帰先情報セット   */
                iomnSetChangeUIQue(&ioMNUiErrClrBuf, IOMN_CPRI_ERROR_CLEAR, hErrorUI, IOMN_CPRI_ERROR_CLEAR);
                iomnDebugMessage(">>>Set Error Clear Information\n",0);
            }
        }
        iomnDebugMessage(">>>OK:IOMSG_SUS_DELAY\n",0);
    }

    #ifdef  DEBUG
        /*  JOB状態取得 */
        if(ERROR == (Status = jobStatus(hJobid, &hJobstatus, &iSuscause))){
            iomnDebugMessage(">>>Error:jobStatus()\n",0);
            printErrno(errno);
            return(ERROR);
        }
        iomnDebugMessage("###JOB STATUS -->%x\n",(I32)hJobstatus);
    #endif

    return(OK);
}


/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         7.13.3
TITLE:      印刷JOB中断コマンド受信処理
MODULE：    STATUS  iomnPrnAjsu(HI hJobid, HI hSustype, HI hJobtype)
INCLUDE:    ioMNPar.h
            ioMNmsg.h
            ioMNstr.h
PARAMETER:
            HI      hJobid;
RETURNS:    I32 4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
NOTES:
HISTORY:
        1.1     Y.Matsuda   97/01/22
    V010.00_01  M.Karasawa  02/12/05    McKinley    割り込み対応(シーケンス変更)
    V016.00_02  M.Karasawa  03/03/26    McKinley    メッセージ置換                  構造体置換
                                                    JOB_MSG_CCIC -> JOB_MSG_CPSW    JOB_STR_CCIC -> JOB_STR_CPSW
                                                    JOB_MSG_TCIC -> JOB_MSG_TPSW    JOB_STR_TCIC -> JOB_STR_TPSW
                                                    新規メッセージ
                                                    JOB_MSG_EPDS                    JOB_STR_ECIC -> JOB_STR_EPDS
*/
STATUS  iomnPrnAjsu(HI hJobid, HI hSustype, HI hJobtype)
{
    STATUS  Status;     /*  各種処理結果    */
    #ifdef DEBUG
    HI      hJobstatus; /*  JOB状態         */
    I32     iSuscause;  /*  中断要因        */
    #endif

#if 0   /* V010.00_01   */
/**/    /*  自発的に中断をかけた場合には送信しない  */
/**/    if(ioMNCjsu[hJobid].iOldTaskId != 0 || hJobid == hioMNFrontJobID || hJobid == hioMNIntJobID){
#else
    /* ジョブIDに関わらず、ioMNからの中断時にはAJSUを送らない   */
    if( ioMNCjsu[hJobid].iOldTaskId != 0 ){
#endif
        /*  上位タスクに対してJOB中断受け付けメッセージを送信   */
        if(ERROR == (Status = iomnSendAjsu(hJobid, hSustype))){
            iomnDebugMessage(">>>Error;iomnSendAjsu()\n",0);
            return(ERROR);
        }
        iomnDebugMessage(">>>OK:iomnSendAjsu()\n",0);
        /* V016.00_02 START */
        if( hSustype == IOMSG_SUS_DELAY ){                          
            if(ERROR == (Status = iomnSendEpds(hJobid, IOMSG_SUSDELAY_STOP, ioMNCjst[hJobid].msgparam.sCjstCtl.hTNo, ioMNCjst[hJobid].msgparam.sCjstCtl.hProcessType))){
                iomnDebugMessage(">>>Error;iomnSendEpds()\n",0);
                return(ERROR);
            }
            iomnDebugMessage(">>>OK:iomnSendEpds()\n",0);
        }
        /* V016.00_02 END   */
    }else{
        if(hSustype == IOMSG_SUS_DELAY){                    /* V010.00_01   */
            /* V016.00_02 START */
            if(ERROR == (Status = iomnSendEpds(hJobid, IOMSG_SUSDELAY_INT, ioMNCjst[hJobid].msgparam.sCjstCtl.hTNo, ioMNCjst[hJobid].msgparam.sCjstCtl.hProcessType))){
                iomnDebugMessage(">>>Error;iomnSendEpds()\n",0);
                return(ERROR);
            }
            iomnDebugMessage(">>>OK:iomnSendEpds()\n",0);
            /* V016.00_02 END   */
        }                                                   /* V010.00_01   */
    }

    /*  遅延中断時には遅延中断中フラグを立てる  */
    if(hSustype == IOMSG_SUS_DELAY){
        ioMNstatus.hSusDelay = IOMN_ON;
        iomnDebugMessage(">>>OK:IOMSG_SUS_DELAY\n",0);
    }

    #ifdef  DEBUG
        jobStatus(hJobid, &hJobstatus, &iSuscause);
        iomnDebugMessage("###JOB STATUS -->%x\n",(I32)hJobstatus);
    #endif
    return(OK);
}


/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         7.13.7
TITLE:      JOB操作UIからの中断コマンド受信処理
MODULE：    STATUS  iomnJobUIAjsu(HI hJobid, HI hSustype, HI hJobtype)
INCLUDE:    ioMNPar.h
            ioMNmsg.h
            ioMNstr.h
PARAMETER:
            HI      hJobid;
RETURNS:    I32 4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
NOTES:
HISTORY:
        1.1     Y.Matsuda   97/01/22
*/
STATUS  iomnJobUIAjsu(HI hJobid, HI hSustype, HI hJobtype)
{

    return(OK);
}


/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         7.15
TITLE:      Recieve the JOB Suspend status Proccess
MODULE：    STATUS  iomnRecieveTJSU()
INCLUDE:    ioMNPar.h
            ioMNmsg.h
            ioMNstr.h
PARAMETER:
            HI      hJobid;
RETURNS:    I32 4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            ＪＯＢ中断ステータス受信処理
NOTES:
HISTORY:
        1.1     Y.Matsuda   97/01/22
        V3.0    J.Ootani    97/12/08
             ＤＳＳ印刷ＪＯＢ中断対応
        M1.1(TJ)H.Takahashi 98/09/28
                DSS逐次対応
        D1.4    Y.Shimada  '00/ 1/25
                DM35/45対応(ＩｐＬｉｂ／逐次制御の組み込み)
        D1.2.1  T.Fujii     00/02/23    DM45/35 並行動作制御、サブジョブ中断処理修正
        D1.7.1  J.Ootani 00/03/01    DM45/35 SEINE FAX F-CODE対応(マージ)
        D2.1    T.Fujii     00/06/06    DM45/35 V004.60不具合対応(EJED送信を追加)
        D2.9    M.Fukuda    00/07/04    DM45/35 FAXリストレポート対応
        D2.0.2  T.Fujii     00/07/04    DM45/35 NOP_DEL(ソースコード整理)
                                                if文,case文でのCOM_JB_EXPPC1を削除
        D3.13   T.Fujii     00/09/02    DM45/35 コーディングミス等修正

    V007.00_04  M.Karasawa  02/09/02    McKinley    TJSU受信処理修正
    V018.00_01  M.Karasawa  03/04/09    McKinley    メッセージ変更対応
    V045.00_02  M.Karasawa  04/06/15    Mckinley    遅延中断/遅延中断解除/自発中断/中断指示等により印刷開始できなくなる不具合の修正
*/
STATUS  iomnRecieveTJSU()                   /*  JOB中断終了Trm受信処理  */
{
    STATUS  Status;     /*  関数処理結果    */
    HI      hJobid;     /*  JOBID           */
    HI      hSts;       /*  終了処理結果    */
    I32     iErrSys;    /*  アプリ系エラー要因  */
    I32     iErrEng;    /*  エンジン系エラー要因    */
    HI      hOrgBack;   /*  原稿戻し枚数    */
    HI      hJobtype;   /*  JOB種別 */
    I32     iSuscause;  /*  中断要因    */
#if 0   /* PRINT_JOINT *//* 中断中断 */
/* PRINT_JOINT */    HI      hQuetype;   /*  中断印刷ＱＵＥタイプ    */
#endif
    HI      hSubJobmode;    /* サブジョブ種別 */
    HI      hSusTask;   /* PRINT_JOINT *//* 中断中断 */
    HI      hSusType;   /* PRINT_JOINT *//* 中断中断 */

    iomnDebugMessage(">>>>Top of iomnRecieveTJSU()<<<<\n",0);
    /*  パラメータ取得  */
    hJobid = ioMNrmyparam.sTjsuCtl.hJobid;
    hSts = ioMNrmyparam.sTjsuCtl.hSts;
    iErrSys = ioMNrmyparam.sTjsuCtl.iErrSys;
    iErrEng = ioMNrmyparam.sTjsuCtl.iErrEng;
    hOrgBack = ioMNrmyparam.sTjsuCtl.hOrgBack;

    iomnDebugMessage("### JOBID                  -->%x\n",(I32)hJobid);
    iomnDebugMessage("### hSts                   -->%x\n",(I32)hSts);
    iomnDebugMessage("### iErrSys                -->%x\n",iErrSys);
    iomnDebugMessage("### iErrEng                -->%x\n",iErrEng);
    iomnDebugMessage("### Paper Count to be Back -->%x\n",(I32)hOrgBack);
    /* V018.00_01 START */
    iomnDebugMessage("### hJcbID                 -->%x\n",ioMNrmyparam.sTjsuCtl.hJcbID);
    iomnDebugMessage("### hTNo                   -->%x\n",ioMNrmyparam.sTjsuCtl.hTNo);
    iomnDebugMessage("### hSusFactor             -->%x\n",ioMNrmyparam.sTjsuCtl.hSusFactor);
    iomnDebugMessage("### iSts                   -->%x\n",ioMNrmyparam.sTjsuCtl.iSts);
    iomnDebugMessage("### hProcessType           -->%x\n",ioMNrmyparam.sTjsuCtl.hProcessType);
    iomnDebugMessage("### hAppliType             -->%x\n",ioMNrmyparam.sTjsuCtl.hAppliType);
    /* V018.00_01 END   */

/* EBX_DTFR_19548 starts */
    if(ioMNstatus.hUnblockCJST_JobId == hJobid)
    {
        cmnPrintf_ioMN(7, "iomnRecieveTJSU(): resetting hUnblockCJST_JobId for JobId %d\n", hJobid);
        ioMNstatus.hUnblockCJST_JobId = IOMN_OFF;
    }
/* EBX_DTFR_19548 ends */
    /* PRINT_JOINT *//* 保険。一応クリアを試みる。 */
    if(ERROR == ioMNPJ_EPSTClear(hJobid)){                                                  /* PRINT_JOINT */
        iomnDebugMessage("[ioMN] ERROR : ioMNPJ_EPSTClear() in iomnRecieveTJSU()\n",0);     /* PRINT_JOINT */
    }                                                                                       /* PRINT_JOINT */

    /*  JOB種別取得 */
    if(ERROR == (Status = jobJobtype(hJobid, &hJobtype))){
        iomnDebugMessage(">>>Error:jobJobtype()\n",0);
        return(ERROR);
    }
    iomnDebugMessage("###Suspend JOB Type->%x\n",hJobtype);

    /* サブジョブ種別を取得する */
    if( parMNSubJobmode(hJobid, &hSubJobmode) == ERROR ) {
        cmnPrintf_ioMN(2,">> ioMN >> Error:parMNSubJobmode() in JobFinSts()\n");
        return( ERROR );
    }

    /*  メッセージ格納  */
    iomnSetJobManagementMsg(&ioMNTjsu[hJobid]);

    /* 送信パラメータ設定  */
    ioMNsmyparam.sTjedCtl.hJobid = hJobid;
    ioMNsmyparam.sTjedCtl.hSts = hSts;
    ioMNsmyparam.sTjedCtl.iErrSys = iErrSys;
    ioMNsmyparam.sTjedCtl.iErrEng = iErrEng;
    ioMNsmyparam.sTjedCtl.hOrgBack = hOrgBack;

    /* V007.00_04   *//* 中断イベント送信しない（送信関数内で対応） */
    /* ジョブ中断イベントの送信 */
    if( iomnSendEJED(hJobid,&ioMNsmyparam) == ERROR ) {
        iomnDebugMessage(">> ioMN >> Error:iomnSendEJED(%d)\n",hJobid);
        /* return( ERROR ); 処理は継続させる */
    }
/* V045.00_02 *//* START */
/*
 TJSUのパラメータのパターンによって～.hSustypeをクリアする処理があるため、
 中断要因についてはここで取得しておく必要有り。
*/
    /*  JOB中断要因取得 */
    if(hJobid == hioMNFrontJobID){
        iSuscause = ioMNstatus.FrontSuspend.hSusType;
    }
    else if( hJobid == hioMNDataRecvJobID ) {
        /* データ転送ジョブのときCS中断とする */
        iSuscause = IOMSG_SUS_CS;
    }
    else {
#if 0   /* PRINT_JOINT *//* 中断中断 */
/* PRINT_JOINT */        if(ERROR == (Status = iomnGetEntryIdFromJobtype(hJobtype, &hQuetype))){
/* PRINT_JOINT */            iomnDebugMessage(">>>Error:iomnGetEntryIdFromJobtype()\n",0);
/* PRINT_JOINT */            return(ERROR);
/* PRINT_JOINT */        }
/* PRINT_JOINT */        iomnDebugMessage("###Print Que Type->%x\n",hQuetype);
/* PRINT_JOINT */        iSuscause = ioMNstatus.BackSuspend[hQuetype].hSusType;
#else
        ioMNPJ_GetSusInfo(hJobid, &hSusTask, &hSusType);
        iSuscause = (I32)hSusType;
#endif
    }
/* V045.00_02 *//* END  */

    switch(hJobtype){
        case COM_JB_IPPC:
        case COM_JB_EXPPC2:
                /* サブジョブ種別による分岐 */
                if( hSubJobmode == COM_JB_SUB_INPUT ) {     /* 入力ジョブの場合 */
                    if(ERROR == (Status = iomnPPCTjsu(hJobid, hSts, iErrSys, iErrEng, hOrgBack, hJobtype))){
                        iomnDebugMessage(">> ioMN >> Error:iomnPPCTjsu()\n",0);
                        return(ERROR);
                    }
                }
                else {      /* 出力ジョブの場合 */
                    if( iomnPrnTjsu(hJobid, hSts, iErrSys, iErrEng, hOrgBack, hJobtype) == ERROR ) {
                        iomnDebugMessage(">> ioMN >> Error:iomnPrnTjsu()\n",0);
                        return(ERROR);
                    }
                }
                break;
        case COM_JB_SCN_FAX1:
        case COM_JB_SCN_FAX2:
        case COM_JB_POLL_SEND_FAX1:
        case COM_JB_POLL_SEND_FAX2:
        case COM_JB_DRC_SCN_FAX:
        case COM_JB_IVSAVE_PPC:
        case COM_JB_IVSAVE_FAX:
        case COM_JB_SEND_DSI:
        case COM_JB_CON_REG_FAX2:
        case COM_JB_BOD_REG_FAX2:
                if(ERROR == (Status = iomnScnTjsu(hJobid, hSts, iErrSys, iErrEng, hOrgBack, hJobtype))){
                    iomnDebugMessage(">>>Error:iomnScnTjsu()\n",0);
                    return(ERROR);
                }
                iomnDebugMessage(">>>OK:iomnScnTjsu()\n",0);
                break;
        case COM_JB_PRT_FAX1:
        case COM_JB_PRT_FAX2:
        case COM_JB_PRT_GDI1:
        case COM_JB_PRT_GDI2:
        case COM_JB_DRC_PRT_GDI:
        case COM_JB_POLL_RECV_FAX1:
        case COM_JB_POLL_RECV_FAX2:
        case COM_JB_DRC_PRT_FAX:
                if(ERROR == (Status = iomnPrnTjsu(hJobid, hSts, iErrSys, iErrEng, hOrgBack, hJobtype))){
                    iomnDebugMessage(">>>Error:iomnPrnTjsu()\n",0);
                    return(ERROR);
                }
                break;
        case COM_JB_RECV_DSI:
        case COM_JB_PRIVATE_DSI:
        case COM_JB_RECV_DSI_SYNC:
        case COM_JB_PRIVATE_DSI_SYNC:
                if(ERROR == (Status = iomnDssPrnTjsu(hJobid, hSts, iErrSys, iErrEng, hOrgBack, hJobtype))){
                    iomnDebugMessage(">>>Error:iomnPrnTjsu()\n",0);
                    return(ERROR);
                }
                break;
        case COM_JB_IVPRM_PPC:
                if(ERROR == (Status = iomnPrnTjsu(hJobid, hSts, iErrSys, iErrEng, hOrgBack, hJobtype))){
                    iomnDebugMessage(">>>Error:iomnPrnTjsu()\n",0);
                    return(ERROR);
                }
                break;
        case COM_JB_IVPRM_FAX:
                if(ERROR == (Status = iomnFrontPrnTjsu(hJobid, hSts, iErrSys, iErrEng, hOrgBack, hJobtype))){
                    iomnDebugMessage(">>>Error:iomnFrontPrnTjsu()\n",0);
                    return(ERROR);
                }
                break;
                
        case COM_JB_LIST_PRT_FAX:
        case COM_JB_CON_GET_FAX2:
        case COM_JB_BOD_GET_FAX2:
                /*  フロント操作時の中断処理    */
                if(hJobid == hioMNFrontJobID){
                    if(ERROR == (Status = iomnFrontPrnTjsu(hJobid, hSts, iErrSys, iErrEng, hOrgBack, hJobtype))){
                        iomnDebugMessage(">>>Error:iomnFrontPrnTjsu()\n",0);
                        return(ERROR);
                    }
                }else{
                /*  バックグランド印刷時の中断  */
                    if(ERROR == (Status = iomnPrnTjsu(hJobid, hSts, iErrSys, iErrEng, hOrgBack, hJobtype))){
                        iomnDebugMessage(">>>Error:iomnPrnTjsu()\n",0);
                        return(ERROR);
                    }
                }
                break;
        case    COM_JB_LIST_RESULT_SEND:
                if(ERROR == (Status = iomnPrnTjsu(hJobid, hSts, iErrSys, iErrEng, hOrgBack, hJobtype))){
                    iomnDebugMessage(">>>Error:iomnPrnTjsu()\n",0);
                    return(ERROR);
                }
                break;
        case COM_JB_JOB_UI:
        case COM_JB_UNDEF:
        case COM_JB_LIST_SEND_FAX:
        case COM_JB_DIG:
        default:
                cmnPrintf_ioMN(2,">>>ERROR:Undefined JOB TYPE!!!\n");
                return(ERROR);
    }
/*
 以下の処理は関数前半部で行う
*/
#if 0   /* V045.00_02 *//* DELETE START */
/**/    /*  JOB中断要因取得 */
/**/    if(hJobid == hioMNFrontJobID){
/**/        iSuscause = ioMNstatus.FrontSuspend.hSusType;
/**//* D3.13 START T.Fujii コーディングミス等修正 */
/**/#if 0   /* OLD */
/**/    }else{
/**/#else   /* D3.13 MID    NEW */
/**/    }
/**/    else if( hJobid == hioMNDataRecvJobID ) {
/**/        /* データ転送ジョブのときCS中断とする */
/**/        iSuscause = IOMSG_SUS_CS;
/**/    }
/**/    else {
/**/#endif
/**//* D3.13 END */
/**/        if(ERROR == (Status = iomnGetEntryIdFromJobtype(hJobtype, &hQuetype))){
/**/            iomnDebugMessage(">>>Error:iomnGetEntryIdFromJobtype()\n",0);
/**/            return(ERROR);
/**/        }
/**/        iomnDebugMessage("###Print Que Type->%x\n",hQuetype);
/**/        iSuscause = ioMNstatus.BackSuspend[hQuetype].hSusType;
/**/    }
#endif  /* V045.00_02 *//* DELETE END   */
    iomnDebugMessage("###Cause for JOB Suspend ->%x\n",iSuscause);
    /*  JOB状態変更 */
    if(ERROR == (Status = jobSetStatus(hJobid, COM_JS_SUSPEND, iSuscause))){
        iomnDebugMessage(">>>Error:jobSetStatus()\n",0);
        return(ERROR);
    }

    iomnDebugMessage("###JOB STATUS->COM_JS_SUSPEND iSuscause = %0x\n",iSuscause);
    iomnDebugMessage(">>>>Bottom of iomnRecieveTJSU()<<<<\n",0);

    return(OK);
}


/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         7.15.1
TITLE:      
            複写JOB中断ステータス受信処理
MODULE：    STATUS  iomnPPCTjsu(HI hJobid, HI hSts, I32 iErrSys, I32 iErrEng, HI hOrgBack, HI hJobtype)
INCLUDE:    ioMNPar.h
            ioMNmsg.h
            ioMNstr.h
PARAMETER:
            HI      hJobid;
            HI      hSts;
            I32     iErrSys;
            I32     iErrEng;
            HI      hOrgBack;
            HI      hJobtype;
RETURNS:    I32 4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            フロントJOB中断ステータスを受信し、JOB状態変更、状態格納、UIへの通知を行う。
NOTES:
HISTORY:
        1.1     Y.Matsuda   97/01/22
        D1.2.1  T.Fujii     00/02/23    DM45/35 サブジョブ中断処理修正
        D1.9.7  J.Ootani    00/06/06    DM45/35 逐次中断時ALLリカバリ対応
        D3.31   T.Fujii     00/10/10    DM45/35 マシンリカバリ処理修正
        D3.31.4 T.Fujii     00/10/25    DM45/35 原稿入力中断/終了でmcnUIが表示されない不具合修正

        MCK1.1  M.Karasawa  02/06/11    McKinley    新メッセージ対応
        MCK1.5  M.Karasawa  02/07/26    McKinley    DELETE iomnChangeToMcnUI
V036.00_02  M.Karasawa 04/01/29 McKinley    AJST未受信時にCJSU読み捨てる不具合の修正(Pre3173)
*/
STATUS  iomnPPCTjsu(HI hJobid, HI hSts, I32 iErrSys, I32 iErrEng, HI hOrgBack, HI hJobtype)
{
    STATUS  Status;
    HI      hRectype;

    /*  マシンリカバリタイプ取得    */
    if(ERROR == (Status = iomnGetMcnRecmode(hJobid, hJobtype, &hRectype))){
        iomnDebugMessage(">>>Errir:iomnGetMcnRecmode()\n",0);
        return(ERROR);
    }
    iomnDebugMessage("###Machine Recover Type->%x\n",(I32)hRectype);

    /*  ステータスによって処理を変更    */
    switch(hSts){
        case    IOMSG_SUS_OK:
/* D3.31 START T.Fujii リソースごとの処理に修正 */
#if 0   /* OLD */
            /*  Error発生中でない場合   */
            if(ioMNstatus.hError_sts == 0){
#else   /* D3.31 MID    NEW */
            if( ioMNstatus.hError_sts == 0 ||
                (!iomnCheckSCS(hJobid) && !(ioMNstatus.hError_sts & (MCN_RCV_RSC_SCN|MCN_RCV_RSC_ADF))) )
            {
#endif
/* D3.31 END */
                iomnDebugMessage("### Engine Recovery Status->%x\n",(I32)ioMNstatus.eng_recoversts);
                /*  正常終了でリカバリ中の場合、リカバリ実施しない  */
                if(iomnCheckSCS(hJobid) == TRUE)
                {
                    /*  リカバリコマンド送信    */
                    if(ERROR == (Status = iomnMcnRecover(hJobid, hRectype,
                                         MCN_RCV_RSC_ALL, IOMN_MCN_REC_TJSU, 0))){
                        cmnPrintf_ioMN(2,">>>Error:iomnMcnRecover() in iomnPPCTjsu()\n");
                        return(ERROR);
                    }
                    iomnDebugMessage(">>>Send ALL Recovery Message\n",0);
                }
                else
                {
                    if(IOMN_MREC_SCAN != (ioMNstatus.eng_recoversts & IOMN_MREC_SCAN)){
                        /*  リカバリコマンド送信    */
                        if(ERROR == (Status = iomnMcnRecover(hJobid, hRectype,
                                             MCN_RCV_RSC_SCN, IOMN_MCN_REC_TJSU, 0))){
                            cmnPrintf_ioMN(2,">>>Error:iomnMcnRecover() in iomnPPCTjsu()\n");
                            return(ERROR);
                        }
                        iomnDebugMessage(">>>Send SCN Recovery Message\n",0);
                    }
/* D3.31 START T.Fujii リソースごとの処理に修正 */
/* V036.00_02 START */
                    else{
                        if(ERROR == (Status = iomnSendTjsu(JOB_MSG_TJSU, hJobid,
                                                                        hSts, iErrSys, iErrEng, hOrgBack))){
                            cmnPrintf_ioMN(2,">>>Error:iomnSendTjsu()\n");
                            return(ERROR);
                        }
                        iomnDebugMessage(">>>Send TJSU Message in iomnPPCTjsu()\n",0);
                    }
/* V036.00_02 END   */
#if 0   /* OLD */
                    if(ERROR == (Status = iomnMcnRecover(hJobid, hRectype, 
                                            MCN_RCV_RSC_PRT, IOMN_MCN_REC_TJSU, 0))){    
                        cmnPrintf_ioMN(2,">>>Error:iomnMcnRecover() in iomnPPCTjsu()\n");
                        return(ERROR);
                    }
                    iomnDebugMessage(">>>Send PRN Recovery Message\n",0);
#else   /* D3.31 MID    NEW */
#endif
/* D3.31 END */
                }

            }
            /*  Error発生中の場合   */
            else{
                /*  JOB中断終了メッセージ送信   */
#if 0   /* MCK1.1 新メッセージ対応  */
/**/                if(ERROR == (Status = iomnSendTjsu(IOMSG_TJSU_UI, hJobid,
#else
                if(ERROR == (Status = iomnSendTjsu(JOB_MSG_TJSU, hJobid,
#endif
                                                                hSts, iErrSys, iErrEng, hOrgBack))){
                    cmnPrintf_ioMN(2,">>>Error:iomnSendTjsu()\n");
                    return(ERROR);
                }
                iomnDebugMessage(">>>Send TJSU Message for Error\n",0);
            }
            break;

        case    IOMSG_SUS_ERROR:
            /*  エンジン系エラー発生時  */
            if(iErrEng != IOMSG_NOT_USED){
/* D3.31 START T.Fujii リソースごとの処理に修正 */
#if 0   /* OLD */
                if(ioMNstatus.hError_sts == 0){
#else   /* D3.31 MID    NEW */
                if( ioMNstatus.hError_sts == 0 ||
                    (!iomnCheckSCS(hJobid) && !(ioMNstatus.hError_sts & (MCN_RCV_RSC_SCN|MCN_RCV_RSC_ADF))) )
                {
#endif
/* D3.31 END */
                    /*  リカバリコマンド送信    */
                    if(iomnCheckSCS(hJobid) == TRUE)
                    {
                        /*  リカバリコマンド送信    */
                        if(ERROR == (Status = iomnMcnRecover(hJobid, hRectype,
                                     MCN_RCV_RSC_ALL, IOMN_MCN_REC_TJSU, 0))){
                            cmnPrintf_ioMN(2,">>>Error:iomnMcnRecover() in iomnPPCTjsu()\n");
                            return(ERROR);
                        }
                        iomnDebugMessage(">>>Send ALL Recovery Message\n",0);
                    }
                    else
                    {
                        if(ERROR == (Status = iomnMcnRecover(hJobid, hRectype,
                                         MCN_RCV_RSC_SCN, IOMN_MCN_REC_TJSU, 0))){
                            cmnPrintf_ioMN(2,">>>Error:iomnMcnRecover() in iomnPPCTjsu()\n");
                            return(ERROR);
                        }
/* D3.31 START T.Fujii リソースごとの処理に修正 */
#if 0   /* OLD */
                        if(ERROR == (Status = iomnMcnRecover(hJobid, hRectype, 
                                        MCN_RCV_RSC_PRT, IOMN_MCN_REC_TJSU, 0))){
                            cmnPrintf_ioMN(2,">>>Error:iomnMcnRecover() in iomnPPCTjsu()\n");
                            return(ERROR);
                        }
#endif
/* D3.31 END */
                    }
                }
                else{ 
                    /*  JOB中断終了メッセージ送信   */
#if 0   /* MCK1.1 新メッセージ対応  */
/**/                    if(ERROR == (Status = iomnSendTjsu(IOMSG_TJSU_UI, hJobid,
#else
                    if(ERROR == (Status = iomnSendTjsu(JOB_MSG_TJSU, hJobid,
#endif
                                                                hSts, iErrSys, iErrEng, hOrgBack))){
                        iomnDebugMessage(">>>Error:iomnSendTjsu()\n",0);
                        return(ERROR);
                    }
                }
            }
            /*  エンジン系Error無し時   */
            else{
/* D3.31 START T.Fujii リソースごとの処理に修正 */
#if 0   /* OLD */
                if(ioMNstatus.hError_sts == 0){
#else   /* D3.31 MID    NEW */
                if( ioMNstatus.hError_sts == 0 ||
                    (!iomnCheckSCS(hJobid) && !(ioMNstatus.hError_sts & (MCN_RCV_RSC_SCN|MCN_RCV_RSC_ADF))) )
                {
#endif
/* D3.31 END */
                    /*  リカバリコマンド送信    */
                    if(iomnCheckSCS(hJobid) == TRUE)
                    {
                        /*  リカバリコマンド送信    */
                        if(ERROR == (Status = iomnMcnRecover(hJobid, hRectype,
                                     MCN_RCV_RSC_ALL, IOMN_MCN_REC_TJSU, 0))){
                            cmnPrintf_ioMN(2,">>>Error:iomnMcnRecover() in iomnPPCTjsu()\n");
                            return(ERROR);
                        }
                        iomnDebugMessage(">>>Send ALL Recovery Message\n",0);
                    }
                    else
                    {
                        if(ERROR == (Status = iomnMcnRecover(hJobid, hRectype,
                                         MCN_RCV_RSC_SCN, IOMN_MCN_REC_TJSU, 0))){
                            cmnPrintf_ioMN(2,">>>Error:iomnMcnRecover() in iomnPPCTjsu()\n");
                            return(ERROR);
                        }
/* D3.31 START T.Fujii リソースごとの処理に修正 */
#if 0   /* OLD */
                        if(ERROR == (Status = iomnMcnRecover(hJobid, hRectype,
                                         MCN_RCV_RSC_PRT, IOMN_MCN_REC_TJSU, 0))){
                            cmnPrintf_ioMN(2,">>>Error:iomnMcnRecover() in iomnPPCTjsu()\n");
                            return(ERROR);
                        }
#endif
/* D3.31 END */
                    }
                }
                else{
                    /*  JOB中断終了メッセージ送信   */
#if 0   /* MCK1.1 新メッセージ対応  */
/**/                    if(ERROR == (Status = iomnSendTjsu(IOMSG_TJSU_UI, hJobid,
#else
                    if(ERROR == (Status = iomnSendTjsu(JOB_MSG_TJSU, hJobid,
#endif
                                                                hSts, iErrSys, iErrEng, hOrgBack))){
                        iomnDebugMessage(">>>Error:iomnSendTjsu()\n",0);
                        return(ERROR);
                    }
                }
            }
            break;

        case    IOMSG_SUS_FAX_MEMFUL:
            break;
        default:
            iomnDebugMessage(">>>Undefined Status!\n",0);
            return(ERROR);
    }

    /*  メモリFULL中の場合  */
    if(ioMNstatus.memfull_sts == IOMN_ON){
    }
    /*  遅延中断中の場合    */
    if(ioMNstatus.hSusDelay == IOMN_ON){
        ioMNstatus.hSusDelay = IOMN_OFF;
        iomnDebugMessage(">>>Clear Delaid Suspend Flag\n",0);
    }
    if(ioMNCccn.iOldTaskId != 0){
        /*  JOB管理テーブルクリア   */
        memset(&ioMNCccn, 0x00, sizeof(struct msgstore));
        iomnDebugMessage(">>>OK:CCCN Table Clear\n",0);
    }
    return(OK);
}


/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         7.15.2
TITLE:      
            入力JOB中断ステータス受信処理
MODULE：    STATUS  iomnScnTjsu(HI hJobid, HI hSts, I32 iErrSys, I32 iErrEng, HI hOrgBack, HI hJobtype)
INCLUDE:    ioMNPar.h
            ioMNmsg.h
            ioMNstr.h
PARAMETER:
            HI      hJobid;
            HI      hSts;
            I32     iErrSys;
            I32     iErrEng;
            HI      hOrgBack;
            HI      hJobtype;
RETURNS:    I32 4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            入力JOB中断ステータスを受信し、JOB状態変更、状態格納を行う。
NOTES:
HISTORY:
        1.1     Y.Matsuda   97/01/22
        1.2     Y.Matsuda   97/10/13
                    MessageIDをCJSU受信していないものについてはTJEDに変換して送信
        2.3     Y.Matsuda   97/12/24
                    リカバリ実施時の条件修正（Error発生時に実施しているため修正）
        D1.2.1  T.Fujii     00/02/23    DM45/35 サブジョブ中断処理修正
        D3.12   T.Fujii     00/09/02    DM45/35 ジョブ中断処理修正
        D3.31   T.Fujii     00/10/10    DM45/35 マシンリカバリ処理修正

        MCK1.1  M.Karasawa 02/06/11 McKinley    新メッセージ対応
        MCK1.5  M.Karasawa  02/07/26    McKinley    DELETE iomnChangeToMcnUI
    V042.00_02  M.Karasawa  04/04/29    McKinley    TJSU読み捨て箇所の修正
*/
STATUS  iomnScnTjsu(HI hJobid, HI hSts, I32 iErrSys, I32 iErrEng, HI hOrgBack, HI hJobtype)
{
    STATUS  Status;
    I32     imsgid;     /*  MessageID   */
    HI      hRectype;   /*  マシンリカバリタイプ    */
    HI      hSuspendQue;    /*  中断処理中印刷Queid */

    /*  中断終了メッセージＩＤ取得  */
#if 0   /* MCK1.1 新メッセージ対応  */
/**/    if(ioMNCjst[hJobid].iOldMsgId == IOMSG_CJST_UI){
/**//* D3.12 START T.Fujii ジョブ中断処理修正 */
/**/#if 0   /* OLD */
/**/        if(ioMNCjsu[hJobid].iOldTaskId != 0){ 
/**/#endif
/**//* D3.12 END */
/**/            imsgid = IOMSG_TJSU_UI;
/**//* D3.12 START T.Fujii ジョブ中断処理修正 */
/**/#if 0   /* OLD */
/**/        }
/**/        else{
/**/            imsgid = IOMSG_TJED_UI;
/**/        }
/**/#endif
/**//* D3.12 END */
/**/    }
/**/    else{
/**//* D3.12 START T.Fujii ジョブ中断処理修正 */
/**/#if 0   /* OLD */
/**/        if(ioMNCjsu[hJobid].iOldTaskId != 0){ 
/**/#endif
/**//* D3.12 END */
/**/            imsgid = IOMSG_TJSU_JOB;
/**//* D3.12 START T.Fujii ジョブ中断処理修正 */
/**/#if 0   /* OLD */
/**/        }
/**/        else{
/**/            imsgid = IOMSG_TJED_JOB;
/**/        }
/**/#endif
/**//* D3.12 END */
/**/    }
#else
        imsgid = JOB_MSG_TJSU;
#endif
    /*  マシンリカバリタイプ取得    */
    if(ERROR == (Status = iomnGetMcnRecmode(hJobid, hJobtype, &hRectype))){
        iomnDebugMessage(">>>Errir:iomnGetMcnRecmode()\n",0);
        return(ERROR);
    }
    iomnDebugMessage("###Machine Recover Type->%x\n",(I32)hRectype);

    /*  ステータスによって処理を変更    */
    switch(hSts){
        case    IOMSG_SUS_OK:
/* D3.31 START T.Fujii リソースごとの処理に修正 */
#if 0   /* OLD */
/**/                if(ioMNstatus.hError_sts == 0){
#else   /* D3.31 MID    NEW */
                if( !(ioMNstatus.hError_sts & (MCN_RCV_RSC_SCN|MCN_RCV_RSC_ADF)) ) {
#endif
/* D3.31 END */
                    /*  正常終了でリカバリ中の場合、リカバリ実施しない  */
                    if(IOMN_MREC_SCAN != (ioMNstatus.eng_recoversts & IOMN_MREC_SCAN)){
                        if(ERROR == (Status = iomnMcnRecover(hJobid, hRectype, 
                                                        MCN_RCV_RSC_SCN, IOMN_MCN_REC_TJSU, 0))){    
                            iomnDebugMessage(">>>Error:iomnMcnRecover() in iomnScnTjsu()\n",0);
                            return(ERROR);
                        }
                    }
                    /* V042.00_02 START */
                    else{
                        if(ERROR == (Status = iomnSendTjsu(imsgid, hJobid,
                                                            hSts, iErrSys, iErrEng, hOrgBack))){
                            iomnDebugMessage(">>>Error:iomnSendTjsu()\n",0);
                            return(ERROR);
                        }
                        iomnDebugMessage(">>>OK:iomnSendTjsu()\n",0);
                    }
                    /* V042.00_02 END */
                }
                else{
                    /*  JOB中断終了メッセージ送信   */
                    if(ERROR == (Status = iomnSendTjsu(imsgid, hJobid,
                                                        hSts, iErrSys, iErrEng, hOrgBack))){
                        iomnDebugMessage(">>>Error:iomnSendTjsu()\n",0);
                        return(ERROR);
                    }
                    iomnDebugMessage(">>>OK:iomnSendTjsu()\n",0);
                }
                break;
        case    IOMSG_SUS_ERROR:
                /*  エンジン系エラー非発生時はマシンリカバリ処理に移行する  */
                if(iErrEng != IOMSG_NOT_USED){
                    if((ioMNstatus.hError_sts & IOMN_MREC_SCAN) == 0
                        || (ioMNstatus.hError_sts & IOMN_MREC_ADF) == 0){
                        /*  リカバリコマンド送信    */
                        if(ERROR == (Status = iomnMcnRecover(hJobid, hRectype, 
                                                        MCN_RCV_RSC_SCN, IOMN_MCN_REC_TJSU, 0))){
                            iomnDebugMessage(">>>Error:iomnMcnRecover() in iomnScnTjsu()\n",0);
                            return(ERROR);
                        }
                        iomnDebugMessage(">>>OK:iomnMcnRecover()\n",0);
                    }
                    else{
                        /*  JOB中断終了メッセージ送信   */
                        if(ERROR == (Status = iomnSendTjsu(imsgid, hJobid,
                                                        hSts, iErrSys, iErrEng, hOrgBack))){
                            iomnDebugMessage(">>>Error:iomnSendTjsu()\n",0);
                            return(ERROR);
                        }
                        iomnDebugMessage(">>>OK:iomnSendTjsu()\n",0);
                    }
                }
                else{
                    if((ioMNstatus.hError_sts & IOMN_MREC_SCAN) == 0
                        || (ioMNstatus.hError_sts & IOMN_MREC_ADF) == 0){
                        /*  リカバリコマンド送信    */
                        if(ERROR == (Status = iomnMcnRecover(hJobid, hRectype, 
                                                        MCN_RCV_RSC_SCN, IOMN_MCN_REC_TJSU, 0))){
                            iomnDebugMessage(">>>Error:iomnMcnRecover() in iomnScnTjsu()\n",0);
                            return(ERROR);
                        }
                        iomnDebugMessage(">>>OK:iomnMcnRecover()\n",0);
                    }
                    else{
                        /*  JOB中断終了メッセージ送信   */
                        if(ERROR == (Status = iomnSendTjsu(imsgid, hJobid,
                                                        hSts, iErrSys, iErrEng, hOrgBack))){
                            iomnDebugMessage(">>>Error:iomnSendTjsu()\n",0);
                            return(ERROR);
                        }
                        iomnDebugMessage(">>>OK:iomnSendTjsu()\n",0);
                    }
                }
                break;

        case    IOMSG_SUS_FAX_MEMFUL:
                break;
        default:
            iomnDebugMessage(">>>Undefined Status!\n",0);
            return(ERROR);
    }

    /*  メモリFULL中の場合  */
    if(ioMNstatus.memfull_sts == IOMN_ON){
        /*  中断処理中印刷Que取得   */
        if(ERROR == (Status = iomnGetPrnqueInSuspendProcess(&hSuspendQue))){
            iomnDebugMessage(">>>Error:iomnGetPrnqueInSuspendProcess()\n",0);
            return(ERROR);
        }
        iomnDebugMessage("###PrintQue in Suspending->%x\n",(I32)hSuspendQue);
        
        /*  中断処理中印刷JOBが存在しない場合   */
        if(hSuspendQue == IOMSG_NOT_USED){
        }
    }
    return(OK);
}


/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         7.15.2.1
TITLE:      
            中断処理中の印刷ＱＵＥを取得する
MODULE：    STATUS  iomnGetPrnqueInSuspendProcess(HI *pSuspend)
INCLUDE:    ioMNPar.h
            ioMNmsg.h
            ioMNstr.h
PARAMETER:
            HI      *pSuspend;
RETURNS:    I32 4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            中断処理中の印刷ＱＵＥＩＤを取得する。
NOTES:
HISTORY:
        1.1     Y.Matsuda   97/06/03
*/
STATUS  iomnGetPrnqueInSuspendProcess(HI *pSuspend)
{
    HI          hPrnQue;    /*  印刷QUE */

    iomnDebugMessage(">>>Top of iomnGetPrnqueInSuspendProcess()<<<\n",0);
    /*  Initialize  */
    *pSuspend = IOMSG_NOT_USED;

    /*  中断処理中のQUEIDを取得する */
    for(hPrnQue = 0;hPrnQue < MAX_NUM_PRN_QUE; ++hPrnQue){
        if(ioMNstatus.BackSuspend[hPrnQue].hSusTask == IOMN_ON){
            *pSuspend = hPrnQue;
            iomnDebugMessage("###Suspending Print Que->%x\n",(I32)hPrnQue);
            return(OK);
        }
    }
    iomnDebugMessage(">>>Bottom of iomnGetPrnqueInSuspendProcess()<<<\n",0);
    return(OK);
}


/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         7.15.3
TITLE:      
            印刷JOB中断ステータス受信処理
MODULE：    STATUS  iomnPrnTjsu(HI hJobid, HI hSts, I32 iErrSys, I32 iErrEng, HI hOrgBack, HI hJobtype)
INCLUDE:    ioMNPar.h
            ioMNmsg.h
            ioMNstr.h
PARAMETER:
            HI      hJobid;
            HI      hSts;
            I32     iErrSys;
            I32     iErrEng;
            HI      hOrgBack;
            HI      hJobtype;
RETURNS:    I32 4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            印刷JOB中断ステータスを受信し、JOB状態変更、状態格納を行う。
NOTES:
HISTORY:
        1.1     Y.Matsuda   97/01/22
        1.2     E.Saka      97/11/28 GDI印刷中断時のPM削除とjobUIへの
                                     メッセージ送信を移動
        2.4     y.Matsuda   98/01/09 for FE-311,319,320
                        Error発生時に正常終了した場合、リカバリを行わないよう修正
        D2.0.1  T.Fujii     00/06/06 DM45/35 NOP_DEL(ソースコード整理)
                                             puJobDelete()関数の呼び出しを削除
        D3.11.2 Y.Shimada   00/09/07 DM45/35 中断中の割り込み全面見直し
        D3.31   T.Fujii     00/10/10 DM45/35 マシンリカバリ処理修正

        MCK1.1  M.Karasawa 02/06/11 McKinley    新メッセージ対応

    V007.00_04  M.Karasawa  02/09/02    McKinley    TJSU受信処理修正
*/
STATUS  iomnPrnTjsu(HI hJobid, HI hSts, I32 iErrSys, I32 iErrEng, HI hOrgBack, HI hJobtype)
{
    STATUS  Status;
    I32     imsgid;     /*  MessageID   */
    HI      hRectype;   /*  リカバリタイプ  */
    HI      hEntryID;   /*  印刷QUEID   */
    HI      hFrontJobtype;  /*  フロントJOB種別 */
    HI      hFrontJobstatus;/*  フロントJOB状態 */
    I32     iFrontSuscause; /*  フロントJOB中断要因 */
    HI      pPmJobid ;

    iomnDebugMessage(">>>>Top of iomnPrnTjsu()<<<<\n",0);
    /*  中断終了メッセージＩＤ取得  */
#if 0   /* MCK1.1 新メッセージ対応  */
/**/    if(ioMNCjst[hJobid].iOldMsgId == IOMSG_CJST_UI) imsgid = IOMSG_TJSU_UI;
/**/    else    imsgid = IOMSG_TJSU_JOB;
#else
    imsgid = JOB_MSG_TJSU;
#endif

    /*  マシンリカバリタイプ取得    */
    if(ERROR == (Status = iomnGetMcnRecmode(hJobid, hJobtype, &hRectype))){
        iomnDebugMessage(">>>Errir:iomnGetMcnRecmode()\n",0);
        return(ERROR);
    }
    iomnDebugMessage("###Machine Recover Type->%x\n",(I32)hRectype);

    /* GDI印刷ジョブの中断の場合はjobUIにTJSUを送信 */
    switch(hJobtype){
      case COM_JB_PRT_GDI1:
      case COM_JB_PRT_GDI2:
#if 0 /* RIOモジュール削除 */
/**/        memcpy(&ioMNsmyparam, &ioMNrmyparam, sizeof(ioMNsmyparam)) ;
/**/        if (ERROR == (Status=iomnSendMessage(msgQIdJobUI,
/**/#if 0   /* MCK1.1 新メッセージ対応  */
/**//**/                                             IOMSG_TJSU_UI,
/**/#else
/**/                                             JOB_MSG_TJSU,
/**/#endif
/**/                                             0,
/**/                                             &ioMNsmyparam))){
/**/            iomnDebugMessage(">>error:iomnSendMessage()\n", 0) ;
/**/            return(ERROR) ;
/**/        }       
#endif
        /* GDI中断時にはPM解放（確保済みの場合のみ）*/
        if (IOMN_ON == ioMNstatus.hGdiPmAllocate){
            /* JOBIDからPMJOBID取得 */
            if (ERROR == jobPmJobid(hJobid, &pPmJobid)){
                iomnDebugMessage(">>>***Error:jobPmJobid()\n", 0) ;
                return(ERROR) ;
            } 
            ioMNstatus.hGdiPmAllocate = IOMN_OFF ; /* PM RELEASE FLG SET */
        }
        break ;
    }

    /*  ステータスによって処理を変更    */
    switch(hSts){
        case    IOMSG_SUS_OK:
            /*  Error発生中でなければリカバリ実施   */
/* D3.31 START T.Fujii リソースごとの処理に修正 */
#if 0   /* OLD */
/**/            if(ioMNstatus.hError_sts == 0){
#else   /* D3.31 MID    NEW */
            if( !(ioMNstatus.hError_sts & MCN_RCV_RSC_PRT) ) {
                /* PRNエラー中の場合 */
#endif
/* D3.31 END */
                /*  マシンリカバリ処理実施  */
                if(ERROR == (Status = iomnMcnRecover(hJobid, hRectype, 
                                                    MCN_RCV_RSC_PRT, IOMN_MCN_REC_TJSU, 0))){    
                    iomnDebugMessage(">>>Error:iomnMcnRecover() in iomnPrnTjsu()\n",0);
                    return(ERROR);
                }
                iomnDebugMessage(">>>Send Recovery Message(PRINT)\n",0);
                /* D3.31 START T.Fujii リソースごとの処理に修正 */
                /* エラー復帰先を割込み複写に変えておく */
                if( ioMNstatus.hError_sts ){
#if 0   /* PRINT_JOINT *//* 連結割り込み */
/* PRINT_JOINT */                    if( ioMNstatus.hPPCIntReserve == hJobid ) {
/* PRINT_JOINT */                        iomnSetChangeUIQue(&ioMNUiErrClrBuf, IOMN_CPRI_INT_COPY, UI_INTPPC, IOMN_CPRI_INT_COPY);
/* PRINT_JOINT */                    }
#else
                if( ioMNstatus.hPPCIntReserve == IOMN_ON ) {
                    ioMNPJ_IntSet(hJobid, IOMN_PJ_OFF);
                    if(ioMNPJ_IntReserveCheck() == ERROR ){
                        iomnSetChangeUIQue(&ioMNUiErrClrBuf, IOMN_CPRI_INT_COPY, UI_INTPPC, IOMN_CPRI_INT_COPY);
                    }
                }
#endif
                }
                /* D3.31 END */
            }
            /*  Errorが発生している場合 */
            else{
                /* D3.11.2 START Y.Shimada ADD */
                /* エラー復帰先を割込み複写に変えておく */
#if 0   /* PRINT_JOINT *//* 連結割り込み */
/* PRINT_JOINT */                if( ioMNstatus.hPPCIntReserve == hJobid ) {
/* PRINT_JOINT */                    iomnSetChangeUIQue(&ioMNUiErrClrBuf, IOMN_CPRI_INT_COPY, UI_INTPPC, IOMN_CPRI_INT_COPY);
/* PRINT_JOINT */                }
#else
                if( ioMNstatus.hPPCIntReserve == IOMN_ON ) {
                    ioMNPJ_IntSet(hJobid, IOMN_PJ_OFF);
                    if(ioMNPJ_IntReserveCheck() == ERROR ){
                        iomnSetChangeUIQue(&ioMNUiErrClrBuf, IOMN_CPRI_INT_COPY, UI_INTPPC, IOMN_CPRI_INT_COPY);
                    }
                }
#endif
                /* D3.11.2 END */
                /*  JOB中断終了メッセージ送信   */
                if(ERROR == (Status = iomnSendTjsu(imsgid, hJobid,
                                                        hSts, iErrSys, iErrEng, hOrgBack))){
                    iomnDebugMessage(">>>Error:iomnSendTjsu()\n",0);
                    return(ERROR);
                }
                iomnDebugMessage(">>>Send TJSU Message\n",0);
            }
            break;
        case    IOMSG_SUS_ERROR:
/* D3.31 START T.Fujii リソースごとの処理に修正 */
#if 0   /* OLD */
/**/            /*  エンジン系エラー非発生時はマシンリカバリ処理に移行する  */
/**/            if(iErrEng != IOMSG_NOT_USED && ioMNstatus.hError_sts == 0){
/**/                /*  リカバリコマンド送信    */
/**/                if(ERROR == (Status = iomnMcnRecover(hJobid, hRectype, 
/**/                                                        MCN_RCV_RSC_PRT, IOMN_MCN_REC_TJSU, 0))){
/**/                    iomnDebugMessage(">>>Error:iomnMcnRecover() in iomnPrnTjsu()\n",0);
/**/                    return(ERROR);
/**/                }
/**/            }
/**/            if(iErrEng != IOMSG_NOT_USED && ioMNstatus.hError_sts != 0){ 
/**/                /* D3.11.2 START Y.Shimada ADD */
/**/                /* エラー復帰先を割込み複写に変えておく */
/**/                if( ioMNstatus.hPPCIntReserve == hJobid ) {
/**/                    iomnSetChangeUIQue(&ioMNUiErrClrBuf, IOMN_CPRI_INT_COPY, UI_INTPPC, IOMN_CPRI_INT_COPY);
/**/                }
/**/                /* D3.11.2 END */
/**/                /*  JOB中断終了メッセージ送信   */
/**/                if(ERROR == (Status = iomnSendTjsu(imsgid, hJobid,
/**/                                                        hSts, iErrSys, iErrEng, hOrgBack))){
/**/                    iomnDebugMessage(">>>Error:iomnSendTjsu()\n",0);
/**/                    return(ERROR);
/**/                }
/**/            }
/**/            if((iErrEng == IOMSG_NOT_USED && iErrSys != IOMSG_NOT_USED)
/**/                ||(iErrEng == IOMSG_NOT_USED && iErrSys == IOMSG_NOT_USED)){
/**/                /*  リカバリコマンド送信    */
/**/                if(ERROR == (Status = iomnMcnRecover(hJobid, hRectype,
/**/                                                         MCN_RCV_RSC_PRT, IOMN_MCN_REC_TJSU, 0))){
/**/                    iomnDebugMessage(">>>Error:iomnMcnRecover() in iomnPrnTjsu()\n",0);
/**/                    return(ERROR);
/**/                }
/**/            }
#else   /* D3.31 MID    NEW */
            /*  エンジン系エラー非発生時はマシンリカバリ処理に移行する  */
            if( (iErrEng != IOMSG_NOT_USED) && (ioMNstatus.hError_sts & MCN_RCV_RSC_PRT) ){ 
                /* D3.11.2 START Y.Shimada ADD */
                /* エラー復帰先を割込み複写に変えておく */
#if 0   /* PRINT_JOINT *//* 連結割り込み */
/* PRINT_JOINT */                if( ioMNstatus.hPPCIntReserve == hJobid ) {
/* PRINT_JOINT */                    iomnSetChangeUIQue(&ioMNUiErrClrBuf, IOMN_CPRI_INT_COPY, UI_INTPPC, IOMN_CPRI_INT_COPY);
/* PRINT_JOINT */                }
#else
                if( ioMNstatus.hPPCIntReserve == IOMN_ON ) {
                    ioMNPJ_IntSet(hJobid, IOMN_PJ_OFF);
                    if(ioMNPJ_IntReserveCheck() == ERROR ){
                        iomnSetChangeUIQue(&ioMNUiErrClrBuf, IOMN_CPRI_INT_COPY, UI_INTPPC, IOMN_CPRI_INT_COPY);
                    }
                }
#endif
                /* D3.11.2 END */

                /*  JOB中断終了メッセージ送信   */
                if( iomnSendTjsu(imsgid,hJobid,hSts,iErrSys,iErrEng,hOrgBack) == ERROR ) {
                    puts( "[ioMN]Error:iomnSendTjsu()" );
                    return(ERROR);
                }
            }
            else {
                /*  リカバリコマンド送信    */
                if( iomnMcnRecover(hJobid,hRectype,MCN_RCV_RSC_PRT,IOMN_MCN_REC_TJSU,0) == ERROR ) {
                    puts( "[ioMN]Error:iomnMcnRecover() in iomnPrnTjsu()" );
                    return(ERROR);
                }
                if( ioMNstatus.hError_sts ) { 
                    /* エラー復帰先を割込み複写に変えておく */
#if 0   /* PRINT_JOINT *//* 連結割り込み */
/* PRINT_JOINT */                    if( ioMNstatus.hPPCIntReserve == hJobid ) {
/* PRINT_JOINT */                        iomnSetChangeUIQue(&ioMNUiErrClrBuf, IOMN_CPRI_INT_COPY, UI_INTPPC, IOMN_CPRI_INT_COPY);
/* PRINT_JOINT */                    }
#else
                    if( ioMNstatus.hPPCIntReserve == IOMN_ON ) {
                        ioMNPJ_IntSet(hJobid, IOMN_PJ_OFF);
                        if(ioMNPJ_IntReserveCheck() == ERROR ){
                            iomnSetChangeUIQue(&ioMNUiErrClrBuf, IOMN_CPRI_INT_COPY, UI_INTPPC, IOMN_CPRI_INT_COPY);
                        }
                    }
#endif
                }
            }
#endif
/* D3.31 END */
            break;
        case    IOMSG_SUS_FAX_MEMFUL:
            break;
        default:
            iomnDebugMessage(">>>Undefined Status!\n",0);
            return(ERROR);
    }

    /*  EntryID 取得    */
    if(ERROR == (Status = iomnGetEntryIdFromJobtype(hJobtype, &hEntryID))){
        iomnDebugMessage(">>>Error:iomnGetEntryIdFromJobtype()\n",0);
        return(ERROR);
    }
    iomnDebugMessage("###ENTRY ID-> %x\n",(I32)hEntryID);

    /*  フロントJOB種別取得 */
    if(ERROR == (Status = jobJobtype(hioMNFrontJobID, &hFrontJobtype))){
        iomnDebugMessage(">>>Error:jobJobtype()\n",0);
        return(ERROR);
    }
    iomnDebugMessage("###Front JOB-> %x\n",(I32)hFrontJobtype);

    /*  フロントJOB状態取得 */
    if(ERROR == (Status = jobStatus(hioMNFrontJobID, &hFrontJobstatus, &iFrontSuscause))){
        cmnPrintf_ioMN(2,">>>Error:jobStatus()\n");
        printErrno(errno);
        return(ERROR);
    }
    iomnDebugMessage("###Front JOB Status-> %x\n",(I32)hFrontJobstatus);
    
#if 0   /* V007.00_04   */
/**/    /*  中断要因によって、ＵＩ優先度変更処理実施    */
/**/    if(ioMNstatus.BackSuspend[hEntryID].hSusType == IOMSG_SUS_CS
/**/        || ioMNstatus.BackSuspend[hEntryID].hSusType == IOMSG_SUS_FAX_MEMFUL
/**/        || ioMNstatus.BackSuspend[hEntryID].hSusType == IOMSG_SUS_INT
/**/        || (ioMNhw.hPriUi == UI_PRINT && ioMNstatus.BackSuspend[hEntryID].hSusType == IOMN_SUS_FRONT)
/**/        || (ioMNhw.hPriUi == UI_PRINT && hFrontJobstatus != COM_JS_READY)){
/**/        /*  優先度変更処理  */
/**/        if(ERROR == (Status = iomnPrnChangeUI(hJobid, hFrontJobtype, hJobtype))){
/**/            iomnDebugMessage(">>>Error:iomnPrnChangeUI()\n",0);
/**/            return(ERROR);
/**/        }
/**/    }
#endif
    /*  メモリFULL中で、フロントJOBが中断中でない場合   */
    if(ioMNstatus.memfull_sts == IOMN_ON
        && ioMNstatus.FrontSuspend.hSusTask == IOMN_OFF
        && hJobtype != COM_JB_DRC_PRT_FAX){
    }

    /*  遅延中断中の場合    */
    if(ioMNCccn.iOldTaskId != 0){
        /*  JOB管理テーブルクリア   */
        memset(&ioMNCccn, 0x00, sizeof(struct msgstore));
        iomnDebugMessage(">>>OK:CCCN Table Clear\n",0);
    }

    iomnDebugMessage(">>>>Bottom of iomnPrnTjsu()<<<<\n",0);
    return(OK);
}


/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         7.15.4
TITLE:      
            Front印刷JOB中断ステータス受信処理
MODULE：    STATUS  iomnFrontPrnTjsu(HI hJobid, HI hSts, I32 iErrSys, I32 iErrEng, HI hOrgBack, HI hJobtype)
INCLUDE:    ioMNPar.h
            ioMNmsg.h
            ioMNstr.h
PARAMETER:
            HI      hJobid;
            HI      hSts;
            I32     iErrSys;
            I32     iErrEng;
            HI      hOrgBack;
            HI      hJobtype;
RETURNS:    I32 4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            FrontJOBにおける印刷JOB中断ステータスを受信し、JOB状態変更、状態格納を行う。
NOTES:
HISTORY:
        1.1     Y.Matsuda   97/01/22
        D3.31   T.Fujii     00/10/10 DM45/35 マシンリカバリ処理修正

        MCK1.1  M.Karasawa 02/06/11 McKinley    新メッセージ対応
*/
STATUS  iomnFrontPrnTjsu(HI hJobid, HI hSts, I32 iErrSys, I32 iErrEng, HI hOrgBack, HI hJobtype)
{
    STATUS  Status;
    I32     imsgid;     /*  MessageID   */
    HI      hRectype;   /*  リカバリタイプ  */

    /*  中断終了メッセージＩＤ取得  */
#if 0   /* MCK1.1 新メッセージ対応  */
/**/    if(ioMNCjst[hJobid].iOldMsgId == IOMSG_CJST_UI) imsgid = IOMSG_TJSU_UI;
/**/    else    imsgid = IOMSG_TJSU_JOB;
#else
    imsgid = JOB_MSG_TJSU;
#endif
    
    /*  マシンリカバリタイプ取得    */
    if(ERROR == (Status = iomnGetMcnRecmode(hJobid, hJobtype, &hRectype))){
        iomnDebugMessage(">>>Errir:iomnGetMcnRecmode()\n",0);
        return(ERROR);
    }
    iomnDebugMessage("###Machine Recover Type->%x\n",(I32)hRectype);

    /*  ステータスによって処理を変更    */
    switch(hSts){
        case    IOMSG_SUS_OK:
                /*  正常終了でリカバリ中の場合、リカバリ実施しない  */
                if(IOMN_MREC_PRT != (ioMNstatus.eng_recoversts & IOMN_MREC_PRT)){
                    if(ERROR == (Status = iomnMcnRecover(hJobid, hRectype, 
                                                        MCN_RCV_RSC_PRT, IOMN_MCN_REC_TJSU, 0))){    
                        iomnDebugMessage(">>>Error:iomnMcnRecover() in iomnPrnTjsu()\n",0);
                        return(ERROR);
                    }
                }
                break;
        case    IOMSG_SUS_ERROR:
/* D3.31 START T.Fujii リソースごとの処理に修正 */
#if 0   /* OLD */
/**/                /*  エンジン系エラー非発生時はマシンリカバリ処理に移行する  */
/**/                if(iErrEng != IOMSG_NOT_USED && ioMNstatus.hError_sts == 0){
/**/                    /*  リカバリコマンド送信    */
/**/                    if(ERROR == (Status = iomnMcnRecover(hJobid, hRectype, 
/**/                                                            MCN_RCV_RSC_PRT, IOMN_MCN_REC_TJSU, 0))){
/**/                        iomnDebugMessage(">>>Error:iomnMcnRecover() in iomnPrnTjsu()\n",0);
/**/                        return(ERROR);
/**/                    }
/**/                }
/**/                if(iErrEng != IOMSG_NOT_USED && ioMNstatus.hError_sts != 0){ 
/**/                    /*  JOB中断終了メッセージ送信   */
/**/                    if(ERROR == (Status = iomnSendTjsu(imsgid, hJobid,
/**/                                                            hSts, iErrSys, iErrEng, hOrgBack))){
/**/                        iomnDebugMessage(">>>Error:iomnSendTjsu()\n",0);
/**/                        return(ERROR);
/**/                    }
/**/                }
/**/                if((iErrEng == IOMSG_NOT_USED && iErrSys != IOMSG_NOT_USED)
/**/                    ||(iErrEng == IOMSG_NOT_USED && iErrSys == IOMSG_NOT_USED)){
/**/                    /*  リカバリコマンド送信    */
/**/                    if(ERROR == (Status = iomnMcnRecover(hJobid, hRectype,
/**/                                                             MCN_RCV_RSC_PRT, IOMN_MCN_REC_TJSU, 0))){
/**/                        iomnDebugMessage(">>>Error:iomnMcnRecover() in iomnPrnTjsu()\n",0);
/**/                        return(ERROR);
/**/                    }
/**/                }
#else   /* D3.31 MID    NEW */
                if((iErrEng != IOMSG_NOT_USED) && (ioMNstatus.hError_sts & MCN_RCV_RSC_PRT)){ 
                    /*  JOB中断終了メッセージ送信   */
                    if( iomnSendTjsu(imsgid,hJobid,hSts,iErrSys,iErrEng,hOrgBack) == ERROR ) {
                        puts( "[ioMN]Error:iomnSendTjsu() in iomnFrontPrnTjsu()" );
                        return(ERROR);
                    }
                }
                else {
                    /*  リカバリコマンド送信    */
                    if( iomnMcnRecover(hJobid,hRectype,MCN_RCV_RSC_PRT,IOMN_MCN_REC_TJSU,0) == ERROR ) {
                        puts( "[ioMN]Error:iomnMcnRecover() in iomnFrontPrnTjsu()" );
                        return(ERROR);
                    }
                }
#endif
/* D3.31 END */
                break;
        case    IOMSG_SUS_FAX_MEMFUL:
                break;
        default:
            iomnDebugMessage(">>>Undefined Status!\n",0);
            return(ERROR);
    }
    /*  メモリFULL中の場合  */
    if(ioMNstatus.memfull_sts == IOMN_ON){
    }
    return(OK);
}


/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         7.15.6
TITLE:      
            DSS通常印刷JOB中断ステータス受信処理
MODULE：    STATUS  iomnDssPrnTjsu(HI hJobid, HI hSts, I32 iErrSys, I32 iErrEng, HI hOrgBack, HI hJobtype)
INCLUDE:    ioMNPar.h
            ioMNmsg.h
            ioMNstr.h
PARAMETER:
            HI      hJobid;
            HI      hSts;
            I32     iErrSys;
            I32     iErrEng;
            HI      hOrgBack;
            HI      hJobtype;
RETURNS:    I32 4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            DSSPrivate印刷JOB中断ステータスを受信し、JOB状態変更、状態格納、DSSへの通知を行う。
NOTES:
HISTORY:
        1.1     Y.Matsuda   97/01/22
        V3.0    J.Ootani    97/11/27
            JAM->SUSPEND
        V3.6    J.Ootani    97/12/10
            TC02
        V4.5D   yMatsuda    98/02/23
            JOB中断処理改訂
        M1.1(TJ)H.Takahashi 98/08/12
                中断時にpuJobDelete()をcallしないように修正
        M1.2(TJ)H.Takahashi 98/09/28
                DSS逐次対応
        D2.0.1  T.Fujii     00/06/06 DM45/35 NOP_DEL(ソースコード整理)
                                             puJobDelete()関数の呼び出しを削除
        D3.11.2 Y.Shimada   00/09/07 DM45/35 中断中の割り込み全面見直し
        D3.31   T.Fujii     00/10/10 DM45/35 マシンリカバリ処理修正

        MCK1.1  M.Karasawa 02/06/11 McKinley    新メッセージ対応

    V007.00_04  M.Karasawa  02/09/02    McKinley    TJSU受信処理修正
    V032.03_01  M.Karasawa  03/11/13    McKinley    TJSU->TJSX置換処理削除
*/
STATUS  iomnDssPrnTjsu(HI hJobid, HI hSts, I32 iErrSys, I32 iErrEng, HI hOrgBack, HI hJobtype)
{
    STATUS      Status;
    I32         imsgid;     /*  MessageID   */
    HI          hRectype;   /*  リカバリタイプ  */
    HI          hEntryID;   /*  印刷QUEID   */
    HI          hFrontJobtype;  /*  フロントJOB種別 */
    HI          hFrontJobstatus;/*  フロントJOB状態 */
    I32         iFrontSuscause; /*  フロントJOB中断要因 */
#if 0 /* RIOモジュール削除 */
/**/    MSG_Q_ID    imsgqid;    /* msgQId */
#endif
 
    cmnPrintf_ioMN(0,">>>>Top of iomnDssPrnTjsu()<<<<\n");
 
#if 0 /* RIOモジュール削除 */
    /*  中断終了メッセージＩＤ取得  */
    /*  中断終了メッセージＩＤ取得  */
/**/    switch(hJobtype){
/**/        case COM_JB_RECV_DSI:
/**/        case COM_JB_PRIVATE_DSI:
/**/        case COM_JB_RECV_DSI_SYNC:
/**/        case COM_JB_PRIVATE_DSI_SYNC:
/**/            imsgqid = msgQIdDssPRN;
/**/            break ;
/**/        default:    
/**/            cmnPrintf_ioMN(2,">>>Error:UNDEFINED JOB TYPE hJobtype = %x\n",(I32)hJobtype);
/**/            return(ERROR);
/**/    }
/**/    iomnDebugMessage("[V4.1TEST]>>>OK:ioMNCjst[hJobid].iOldMsgId = %d\n",ioMNCjst[hJobid].iOldMsgId);
#endif
#if 0   /* MCK1.1 新メッセージ対応  */
/**/    if(ioMNCjst[hJobid].iOldMsgId == IOMSG_CJST_UI){
/**/        if(ioMNCjsu[hJobid].iOldTaskId != 0){
/**/            imsgid = IOMSG_TJSU_UI;
/**/        }
/**/        else{
/**/            imsgid = IOMSG_TJED_UI;
/**/        }
/**/    }
/**/    else{
/**/        if(ioMNCjsu[hJobid].iOldTaskId != 0){
/**/            imsgid = IOMSG_TJSU_JOB;
/**/        }
/**/        else{
/**/            imsgid = IOMSG_TJED_JOB;
/**/        }
/**/    }
#else
#if 0   /* V032.03_01 */
/**/        if(ioMNCjsu[hJobid].iOldTaskId != 0){
/**/            imsgid = JOB_MSG_TJSU;
/**/        }
/**/        else{
/**/            imsgid = JOB_MSG_TJSX;
/**/        }
#else   /* V032.03_01 */
        imsgid = JOB_MSG_TJSU;
#endif  /* V032.03_01 */
#endif
    iomnDebugMessage("[V4.1TEST]>>>OK:imsgid = %d\n",imsgid);
    
    /*  マシンリカバリタイプ取得    */
    if(ERROR == (Status = iomnGetMcnRecmode(hJobid, hJobtype, &hRectype))){
        cmnPrintf_ioMN(2,">>>Error:iomnGetMcnRecmode() in iomnDssPrnTjsu()\n");
        return(ERROR);
    }
    iomnDebugMessage("###Machine Recover Type->%x\n",(I32)hRectype);
    
    /*  EntryID 取得    */
    if(ERROR == (Status = iomnGetEntryIdFromJobtype(hJobtype, &hEntryID))){
        cmnPrintf_ioMN(2,">>>Error:iomnGetEntryIdFromJobtype() in iomnDssPrnTjsu()\n");
        return(ERROR);
    }
    iomnDebugMessage("###ENTRY ID-> %x\n",(I32)hEntryID);
    iomnDebugMessage("[V3.0 TEST DUMP] ioMNstatus.BackSuspend[hEntryID].hSusType = %d",
            ioMNstatus.BackSuspend[hEntryID].hSusType);
    iomnDebugMessage("(%x)\n",
            ioMNstatus.BackSuspend[hEntryID].hSusType);
    iomnDebugMessage("[V3.0 TEST DUMP] ioMNstatus.hError_sts = %d",
            ioMNstatus.hError_sts);
    iomnDebugMessage("(%x)\n",
            ioMNstatus.hError_sts);
 
    /*  フロントJOB種別取得 */
    if(ERROR == (Status = jobJobtype(hioMNFrontJobID, &hFrontJobtype))){
        cmnPrintf_ioMN(2,">>>Error:jobJobtype() in iomnDssPrnTjsu()\n");
        return(ERROR);
    }
    iomnDebugMessage("###Front JOB-> %x\n",(I32)hFrontJobtype);
 
    /*  フロントJOB状態取得 */
    if(ERROR == (Status = jobStatus(hioMNFrontJobID, &hFrontJobstatus, &iFrontSuscause))){
        cmnPrintf_ioMN(2,">>>Error:jobStatus() in iomnDssPrnTjsu()\n");
        printErrno(errno);
        return(ERROR);
    }
    iomnDebugMessage("###Front JOB Status-> %x\n",(I32)hFrontJobstatus);
    
    /*  ステータスによって処理を変更    */
    switch(hSts){
        case    IOMSG_SUS_OK:
/* D3.31 START T.Fujii リソースごとの処理に修正 */
#if 0   /* OLD */
                if(ioMNstatus.hError_sts != IOMN_OFF) {
#else   /* D3.31 MID    NEW */
                if( ioMNstatus.hError_sts & MCN_RCV_RSC_PRT ) {
                    /* PRNエラー中の場合 */
#endif
/* D3.31 END */
                    /* D3.11.2 START Y.Shimada ADD */
                    /* エラー復帰先を割込み複写に変えておく */
#if 0   /* PRINT_JOINT *//* 連結割り込み */
/* PRINT_JOINT */                    if( ioMNstatus.hPPCIntReserve == hJobid ) {
/* PRINT_JOINT */                        iomnSetChangeUIQue(&ioMNUiErrClrBuf, IOMN_CPRI_INT_COPY, UI_INTPPC, IOMN_CPRI_INT_COPY);
/* PRINT_JOINT */                    }
#else
                    if( ioMNstatus.hPPCIntReserve == IOMN_ON ) {
                        ioMNPJ_IntSet(hJobid, IOMN_PJ_OFF);
                        if(ioMNPJ_IntReserveCheck() == ERROR ){
                            iomnSetChangeUIQue(&ioMNUiErrClrBuf, IOMN_CPRI_INT_COPY, UI_INTPPC, IOMN_CPRI_INT_COPY);
                        }
                    }
#endif
                    /* D3.11.2 END */
                    /*  JOB中断終了メッセージ送信   */
                    if(ERROR == (Status = iomnSendTjsu(imsgid, hJobid,
                                                        hSts, iErrSys, iErrEng, hOrgBack))){
                        cmnPrintf_ioMN(2,">>>Error:iomnSendTjsu() in iomnDssPrnTjsu()\n");
                        return(ERROR);
                    }
                    iomnDebugMessage(">>>>Send TJSU MESSAGE!\n",0);
                }
                else{
                    if(ERROR == (Status = iomnMcnRecover(hJobid, hRectype,
                                                    MCN_RCV_RSC_PRT, IOMN_MCN_REC_TJSU, 0))){
                        iomnDebugMessage(">>>Error:iomnMcnRecover() in iomnPrnTjsu()\n",0);
 
                        return(ERROR);
                    }
                    iomnDebugMessage(">>>>Send CF02 MESSAGE!\n",0);

                    /* D3.31 START T.Fujii リソースごとの処理に修正 */
                    if( ioMNstatus.hError_sts ) {
                        /* エラー復帰先を割込み複写に変えておく */
#if 0   /* PRINT_JOINT *//* 連結割り込み */
/* PRINT_JOINT */                        if( ioMNstatus.hPPCIntReserve == hJobid ) {
/* PRINT_JOINT */                            iomnSetChangeUIQue(&ioMNUiErrClrBuf, IOMN_CPRI_INT_COPY, UI_INTPPC, IOMN_CPRI_INT_COPY);
/* PRINT_JOINT */                        }
#else
                        if( ioMNstatus.hPPCIntReserve == IOMN_ON ) {
                            ioMNPJ_IntSet(hJobid, IOMN_PJ_OFF);
                            if(ioMNPJ_IntReserveCheck() == ERROR ){
                                iomnSetChangeUIQue(&ioMNUiErrClrBuf, IOMN_CPRI_INT_COPY, UI_INTPPC, IOMN_CPRI_INT_COPY);
                            }
                        }
#endif
                    }
                    /* D3.31 END */
                }
                break;

        case    IOMSG_SUS_ERROR:
/* D3.31 START T.Fujii リソースごとの処理に修正 */
#if 0   /* OLD */
/**/                /*  エンジン系エラー非発生時はマシンリカバリ処理に移行する  */
/**/                if(iErrEng != IOMSG_NOT_USED && ioMNstatus.hError_sts == 0){
/**/                    /*  リカバリコマンド送信    */
/**/                    if(ERROR == (Status = iomnMcnRecover(hJobid, hRectype,
/**/                                                            MCN_RCV_RSC_PRT, IOMN_MCN_REC_TJSU, 0))){
/**/                        cmnPrintf_ioMN(2,">>>Error:iomnMcnRecover() in iomnDssPrnTjsu()\n");
/**/                        return(ERROR);
/**/                    }
/**/                }
/**/                if(iErrEng != IOMSG_NOT_USED && ioMNstatus.hError_sts != 0){
/**/                    /* D3.11.2 START Y.Shimada ADD */
/**/                    /* エラー復帰先を割込み複写に変えておく */
/**/                    if( ioMNstatus.hPPCIntReserve == hJobid ) {
/**/                        iomnSetChangeUIQue(&ioMNUiErrClrBuf, IOMN_CPRI_INT_COPY, UI_INTPPC, IOMN_CPRI_INT_COPY);
/**/                    }
/**/                    /* D3.11.2 END */
/**/                    /*  JOB中断終了メッセージ送信   */
/**/                    if(ERROR == (Status = iomnSendTjsu(imsgid, hJobid,
/**/                                                            hSts, iErrSys, iErrEng, hOrgBack))){
/**/                        cmnPrintf_ioMN(2,">>>Error:iomnSendTjsu() in iomnDssPrnTjsu()\n");
/**/                        return(ERROR);
/**/                    }
/**/                }
/**/                if((iErrEng == IOMSG_NOT_USED && iErrSys != IOMSG_NOT_USED)
/**/                    ||(iErrEng == IOMSG_NOT_USED && iErrSys == IOMSG_NOT_USED)){
/**/                    /*  リカバリコマンド送信    */
/**/                    if(ERROR == (Status = iomnMcnRecover(hJobid, hRectype,
/**/                                                             MCN_RCV_RSC_PRT, IOMN_MCN_REC_TJSU, 0))){
/**/                        cmnPrintf_ioMN(2,">>>Error:iomnMcnRecover() in iomnDssPrnTjsu()\n");
/**/                        return(ERROR);
/**/                    }
/**/                }
#else   /* D3.31 MID    NEW */
                if( (iErrEng != IOMSG_NOT_USED) && (ioMNstatus.hError_sts & MCN_RCV_RSC_PRT) ) {
                    /* D3.11.2 START Y.Shimada ADD */
                    /* エラー復帰先を割込み複写に変えておく */
#if 0   /* PRINT_JOINT *//* 連結割り込み */
/* PRINT_JOINT */                    if( ioMNstatus.hPPCIntReserve == hJobid ) {
/* PRINT_JOINT */                        iomnSetChangeUIQue(&ioMNUiErrClrBuf, IOMN_CPRI_INT_COPY, UI_INTPPC, IOMN_CPRI_INT_COPY);
/* PRINT_JOINT */                    }
#else
                    if( ioMNstatus.hPPCIntReserve == IOMN_ON ) {
                        ioMNPJ_IntSet(hJobid, IOMN_PJ_OFF);
                        if(ioMNPJ_IntReserveCheck() == ERROR ){
                            iomnSetChangeUIQue(&ioMNUiErrClrBuf, IOMN_CPRI_INT_COPY, UI_INTPPC, IOMN_CPRI_INT_COPY);
                        }
                    }
#endif
                    /* D3.11.2 END */
                    /*  JOB中断終了メッセージ送信   */
                    if( iomnSendTjsu(imsgid,hJobid,hSts,iErrSys,iErrEng,hOrgBack) == ERROR ) {
                        puts( "[ioMN]Error:iomnSendTjsu() in iomnDssPrnTjsu()" );
                        return(ERROR);
                    }
                }
                else {
                    /*  リカバリコマンド送信    */
                    if( iomnMcnRecover(hJobid,hRectype,MCN_RCV_RSC_PRT,IOMN_MCN_REC_TJSU,0) == ERROR ) {
                        puts( "[ioMN]Error:iomnMcnRecover() in iomnDssPrnTjsu()" );
                        return(ERROR);
                    }
                    if( ioMNstatus.hError_sts ) {
                        /* エラー復帰先を割込み複写に変えておく */
#if 0   /* PRINT_JOINT *//* 連結割り込み */
/* PRINT_JOINT */                        if( ioMNstatus.hPPCIntReserve == hJobid ) {
/* PRINT_JOINT */                            iomnSetChangeUIQue(&ioMNUiErrClrBuf, IOMN_CPRI_INT_COPY, UI_INTPPC, IOMN_CPRI_INT_COPY);
/* PRINT_JOINT */                        }
#else
                        if( ioMNstatus.hPPCIntReserve == IOMN_ON ) {
                            ioMNPJ_IntSet(hJobid, IOMN_PJ_OFF);
                            if(ioMNPJ_IntReserveCheck() == ERROR ){
                                iomnSetChangeUIQue(&ioMNUiErrClrBuf, IOMN_CPRI_INT_COPY, UI_INTPPC, IOMN_CPRI_INT_COPY);
                            }
                        }
#endif
                    }
                }
#endif
/* D3.31 END */
                break;
        case    IOMSG_SUS_FAX_MEMFUL:
                break;
        default:    
            cmnPrintf_ioMN(2,">>>Undefined Status! in iomnDssPrnTjsu()\n");
    }
#if 0   /* V007.00_04   */
/**/    if(ioMNstatus.BackSuspend[hEntryID].hSusType == IOMSG_SUS_CS
/**/                   && ioMNstatus.hError_sts == IOMN_OFF) {
/**/        /*  中断要因によって、ＵＩ優先度変更処理実施    */
/**/        if(ioMNstatus.BackSuspend[hEntryID].hSusType == IOMSG_SUS_CS
/**/            || ioMNstatus.BackSuspend[hEntryID].hSusType == IOMSG_SUS_FAX_MEMFUL
/**/            || ioMNstatus.BackSuspend[hEntryID].hSusType == IOMSG_SUS_INT
/**/            || (ioMNhw.hPriUi == UI_PRINT && ioMNstatus.BackSuspend[hEntryID].hSusType == IOMN_SUS_FRONT)
/**/            || (ioMNhw.hPriUi == UI_PRINT && hFrontJobstatus != COM_JS_READY)){
/**/            /*  優先度変更処理  */
/**/            if(ERROR == (Status = iomnPrnChangeUI(hJobid, hFrontJobtype, hJobtype))){
/**/                cmnPrintf_ioMN(2,">>>Error:iomnPrnChangeUI() in iomnDssPrnTjsu()\n");
/**/                return(ERROR);
/**/            }       
/**/        }
/**/    }
#endif
    /*  メモリFULL中で、フロントJOBが中断中でない場合   */
    if(ioMNstatus.memfull_sts == IOMN_ON
        && ioMNstatus.FrontSuspend.hSusTask == IOMN_OFF
        && hJobtype != COM_JB_DRC_PRT_FAX){
    }
 
    /*  遅延中断中の場合    */
    if(ioMNCccn.iOldTaskId != 0){
        /*  JOB管理テーブルクリア   */
        memset(&ioMNCccn, 0x00, sizeof(struct msgstore));
        iomnDebugMessage(">>>OK:CCCN Table Clear in iomnDssPrnTjsu()\n",0);
    }
 
    iomnDebugMessage(">>>>Bottom of iomnDssPrnTjsu()<<<<\n",0);
 
    return(OK);
}

#if 0   /* MCK1.2   *//* 関数削除   */
/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         7.16
TITLE:      JOB受け付け許可通知受信処理
MODULE：    STATUS  iomnJobAcceptable()
INCLUDE:    ioMNPar.h
PARAMETER:
RETURNS:    I32 4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            JOB受け付け許可通知受信時の処理。
NOTES:
HISTORY:
        1.1     Y.Matsuda   96/9/18

        MCK1.1  M.Karasawa 02/06/11 McKinley    新メッセージ対応
*/
/**/STATUS  iomnJobAcceptable()
/**/{
/**/    STATUS  Status;         /*  関数戻り値  */
/**/    HI      hJobID;         /*  JOBID   */
/**/    I32     imsgid;         /*  MessageID   */
/**/    HI      hJobsts;        /*  JOB状態 */
/**/    I32     iSuscause;      /*  中断要因    */
/**/
/**/    /*  受信パラメータ取得  */
/**/    hJobID = ioMNrmyparam.sEnjeCtl.hJobid;
/**/    iomnDebugMessage("###JOBID->%x\n",(I32)hJobID);
/**/    /*  JOB状態取得 */
/**/    if(ERROR == (Status = jobStatus(hJobID, &hJobsts, &iSuscause))){
/**/        iomnDebugMessage(">>>Error:jobStatus()\n",0);
/**/        return(ERROR);
/**/    }
/**/
/**/    /*  ERROR発生中か否かをチェック */
/**/    if(ioMNstatus.hError_sts != IOMN_OFF){
/**/        /*  MessageIDの取得 */
/**/        if(ioMNCjst[hJobID].iOldMsgId == IOMSG_CJST_UI) imsgid = IOMSG_TJFN_UI;
/**/        else if(ioMNCjst[hJobID].iOldMsgId == IOMSG_CJST_DIG)   imsgid = IOMSG_TJFN_DIG;
/**/        else    imsgid = IOMSG_TJFN_JOB;
/**/        /*  JOB受け付け許可メッセージ送信   */
/**/        if(ERROR == (Status = iomnSendJobAcceptEvent(hJobID, ioMNCjst[hJobID].OldmsgQId, IOMSG_ENJE_UI))){
/**/            iomnDebugMessage(">>>Error:iomnSendJobAcceptEvent()\n",0);
/**/            return(ERROR);
/**/        }
/**/        /*  JOB状態変更 */
/**/        if(hJobsts == COM_JS_RUNNING){
/**/            if(ERROR == (Status = jobSetStatus(hJobID, COM_JS_SUSPEND, 0))){
/**/                iomnDebugMessage(">>>Error:jobSetStatus()\n",0);
/**/                return(ERROR);
/**/            }
/**/            iomnDebugMessage(">>>JobStatus->COM_JS_SUSPEND\n",0);
/**/        }
/**/    }
/**/    else{
/**/        /*  マシン管理に対してスキャナのみのリカバリ実施    */
/**/        if(ERROR == (Status = iomnMcnRecover(hJobID, MCN_RCV_MODE_PPC, MCN_RCV_RSC_SCN, IOMN_MCN_REC_TJFN, 0))){
/**/            iomnDebugMessage(">>>Error:iomnMcnRecover(in iomnJobAcceptable)\n",0);
/**/            return(ERROR);
/**/        }
/**/    }
/**/    return(OK);
/**/}
#endif


/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         7.17
TITLE:      ADD用紙残通知受信処理
MODULE：    STATUS  iomnPaperInAdd()
INCLUDE:    ioMNPar.h
PARAMETER:
            N/A
RETURNS:    I32 4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            ADD用紙残通知受信時の処理。LOCAL状態管理テーブルに
            状態を設定する。
NOTES:
HISTORY:
        1.1     Y.Matsuda   96/9/18
*/
STATUS  iomnPaperInAdd()
{
#ifdef IOMN_OLDMSG  /* MCK  *//* 020610 *//* Member */
    HI  hJobID;

    /*  受信パラメータ取得  */
    hJobID = ioMNrmyparam.sEaddCtl.hJobid;
    iomnDebugMessage(">>>Paper in ADD\n",0);
    iomnDebugMessage("###JOBID->%x\n",(I32)hJobID);

    /*  ADD用紙残状態をテーブルに設定   */
    ioMNstatus.hAdd_sts = IOMN_ON;
#endif
    return(OK);
}


/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         7.18
TITLE:      JOB属性変更通知受信処理
MODULE：    STATUS  iomnChangeJobAtr()

INCLUDE:    ioMNPar.h
PARAMETER:
            N/A
RETURNS:    I32 4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            JOB属性変更通知受信時の処理。JOB管理、又はUIにイベントを
            通知する。
NOTES:
HISTORY:
        1.1     Y.Matsuda   96/9/18
        D1.6    T.Fujii     00/02/09 DM45/35 prnUI表示対応
        D1.6.7  T.Fujii     00/04/10 DM45/35 ジョブ属性変更通知送信処理修正
        D2.8    T.Fujii     00/07/01 DM45/35 印刷系イベント送信処理修正

        MCK1.1  M.Karasawa 02/06/11 McKinley    新メッセージ対応
*/
STATUS  iomnChangeJobAtr()
{
    STATUS      Status;
    HI          hJobID;
    MSG_Q_ID    msgqid;
    I32         imsgid  = -1;   /* D1.1 T.Fujii ダミー定義追加 */

    /*  受信パラメータ取得  */
#if 0   /* MCK1.1   */
/**/    hJobID = ioMNrmyparam.sEatsCtl.hJobid;
/**/    iomnDebugMessage("###JOBID ->%x\n",(I32)hJobID);
#else
    hJobID = ioMNrmyparam.sEantCtl.hJobid;
    iomnDebugMessage("###JOBID ->%x\n",(I32)hJobID);
#endif

    /*  JOB毎メッセージQue取得  */
    msgqid = ioMNCjst[hJobID].OldmsgQId;
#if 0   /* MCK1.1   */
/**/    /*  基本複写リカバリ中でJOBが完了してしまっている場合、複写UIに送信する */
/**/    if(msgqid == NULL){
/**/        msgqid = msgQIdCpyUI;
/**/    }
/**/    if(ioMNCjst[hJobID].iOldMsgId == IOMSG_CJST_UI 
/**/        || ioMNCjst[hJobID].iOldMsgId == 0){
/**/        imsgid = IOMSG_EATR_UI;
/**/    }
/**/    if(ioMNCjst[hJobID].iOldMsgId == IOMSG_CJST_JOB)    imsgid = IOMSG_EATR_JOB;
#else
    if(msgqid == NULL){
        iomnDebugMessage(">>>Undefined MessageID(iomnChangeJobAtr)->%x\n",ioMNCjst[hJobID].iOldMsgId);
        return(OK);
    }
    imsgid = JOB_MSG_EJAB;
#endif
    /*  メッセージ送信  */
    if(ERROR == (Status = iomnSendJobAtrEvent(msgqid, imsgid))){
        iomnDebugMessage(">>>error:iomnSendJobAtrEvent()\n",0);
        return(ERROR);
    }
    return(OK);
}

/*EBX_DCR_41129 start*/
STATUS  iomnChangeJobCustomAtr()
{
    STATUS      Status;
    HI          hJobID;
    MSG_Q_ID    msgqid;
    I32         imsgid  = -1;  

    hJobID = ioMNrmyparam.sEjcaCtl.hJobid;
    iomnDebugMessage("###JOBID ->%x\n",(I32)hJobID);

    msgqid = ioMNCjst[hJobID].OldmsgQId;

    if(msgqid == NULL){
        iomnDebugMessage(">>>Undefined MessageID(iomnChangeJobCustomAtr)->%x\n",ioMNCjst[hJobID].iOldMsgId);
        return(OK);
    }
    imsgid = JOB_MSG_EJCA;
    if(ERROR == (Status = iomnSendJobAtrEvent(msgqid, imsgid))){
        iomnDebugMessage(">>>error:iomnSendJobAtrEvent()\n",0);
        return(ERROR);
    }
    return(OK);
}
/*EBX_DCR_41129 end*/

/*
(C) Copyright TOSHIBA TEC Corporation 2000. ALL Right Reserved
NO.         
TITLE:      JOB属性変更通知受信処理
MODULE：    STATUS  iomnChangeJobAtrPrn()

INCLUDE:    ioMNPar.h
PARAMETER:
            N/A
RETURNS:    I32 4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            JOB属性変更通知受信時の処理。JOB管理、又はUIにイベントを
            通知する。
NOTES:
HISTORY:
        D1.6.7      T.Fujii     00/04/10    DM45/35 original
        D2.8        T.Fujii     00/07/01    DM45/35 印刷系イベント送信処理修正

        MCK1.1  M.Karasawa 02/06/11 McKinley    新メッセージ対応
*/
STATUS  iomnChangeJobAtrPrn()
{
#if 0   /* MCK1.1 新メッセージ対応  *//* 現状使用する予定はないです・・・ */
/**/    HI          hJobID;
/**/    HI          hPrnID = -1;
/**/
/**/    iomnDebugMessage(">> ioMN >> CHECK:call iomnChangeJobAtrPrn()\n",(I32)0);
/**/
/**/    /*  受信パラメータ取得  */
/**/    hJobID = ioMNrmyparam.sEatrCtl.hJobid;
/**/    iomnDebugMessage(">> ioMN >> CHECK:JOBID in EATR ->%x\n",(I32)hJobID);
/**/
/**/    /* 印刷種別を取得してメッセージを送信 */
/**/    if( (hPrnID = iomnPrnQueGetPrnType(hJobID)) >= 0 ) {
/**/        iomnDebugMessage(">> ioMN >> CHECK:Try to send EATR message (hPrnID=%x)\n",(I32)hPrnID);
/**/        /* prnUI or 管理タスクにEATRを送信 */
/**/        return( iomnSendPrintEventMsg(hJobID,hPrnID,IOMSG_EATR_UI,IOMSG_EATR_JOB,&ioMNrmyparam) );
/**/    }
/**/
/**/    cmnPrintf_ioMN(2, ">> ioMN >> ERROR:cannot send EATR. hJobID=%d, hPrnID=%d\n", hJobID, hPrnID );
/**/    return(ERROR);
#else
    return(OK);
#endif
}


/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         7.18
TITLE:      カウントダウン通知受信処理
MODULE：    STATUS  iomnCountDown()

INCLUDE:    ioMNPar.h
PARAMETER:
            N/A
RETURNS:    I32 4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            カウントダウン通知受信時の処理。JOB管理、又はUIにイベントを
            通知する。
NOTES:
HISTORY:
        1.1     Y.Matsuda   96/9/18
        D1.6    T.Fujii     00/02/09 DM45/35 prnUI表示対応
        D2.8    T.Fujii     00/07/01 DM45/35 印刷系イベント送信処理修正
        D3.20   T.Fujii     00/09/12 DM45/35 ECDN送信処理修正
        D4.8    T.Fujii     00/11/21 DM45/35 05テストコピーでcpyUIにメッセージ送信する不具合修正

        MCK1.1  M.Karasawa  02/06/11    McKinley    新メッセージ対応
        MCK1.4  M.Karasawa  02/07/18    McKinley    カウントダウンイベントの送信先はCJST送信元にのみ通知
*/
STATUS  iomnCountDown()
{
    STATUS  Status;
    HI          hJobID;
    MSG_Q_ID    msgqid;
    I32         imsgid = -1;    /* D1.1 T.Fujii ダミー定義追加 */
/*    HI          hPrnID = -1;  *//* V028.00_00 */

    HI          hJcbId;         /* MCK  *//* 020423 */
    HI          hTNo;           /* MCK  *//* 020423 */
    HI          hRemainVolume;  /* MCK  *//* 020423 */
    HI          hRemainPage;    /* MCK  *//* 020423 */
    HI          hRemainTotal;   /* MCK  *//* 020423 */

    /*  受信パラメータ取得  */
#if 0   /* MCK1.1   */
    hJobID = ioMNrmyparam.sEcdnCtl.hJobid;
    iomnDebugMessage("###JOBID->%x\n",(I32)hJobID);
#else
    hJobID          = ioMNrmyparam.sEpcdCtl.hJobid;
    hRemainVolume   = ioMNrmyparam.sEpcdCtl.hRemainVolume;
    hRemainPage     = ioMNrmyparam.sEpcdCtl.hRemainPage;
    hRemainTotal    = ioMNrmyparam.sEpcdCtl.hRemainTotal;
    hJcbId          = ioMNrmyparam.sEpcdCtl.hJcbID;
    hTNo            = ioMNrmyparam.sEpcdCtl.hTNo;
#if 0
    iomnDebugMessage("###JOBID      ->%x\n",(I32)hJobID);
    iomnDebugMessage("###Re Vol     ->%x\n",(I32)hRemainVolume);
    iomnDebugMessage("###Re Page    ->%x\n",(I32)hRemainPage);
    iomnDebugMessage("###Re Total   ->%x\n",(I32)hRemainTotal);
    iomnDebugMessage("###JCBID      ->%x\n",(I32)hJcbId);
    iomnDebugMessage("###TNO        ->%x\n",(I32)hTNo);
#endif
	cmnPrintf_ioMN(7,"###JOBID      ->%x, Re Vol ->%x, Re Page    ->%x, Re Total   ->%x, JCBID      ->%x, TNO        ->%x", (I32)hJobID, (I32)hRemainVolume, (I32)hRemainPage, (I32)hRemainTotal, (I32)hJcbId, (I32)hTNo);
#endif

#if 0   /* MCK1.4   *//* --- CPY逐次 --- */
/**/    /* 印刷ジョブの場合、印刷種別を取得してメッセージを送信 */
/**/    if( (hPrnID = iomnPrnQueCheckPrnType(hJobID)) >= 0 ) {
/**/        /* D3.20 START T.Fujii ECDN送信処理修正 */
/**/        if( hPrnID == PRNQUE_CPY_NORMAL ) {
/**/            /* 印刷種別がコピーの場合はcpyUIにECDNを送信 */
/**//* D4.8 START T.Fujii メッセージ送信先修正 */
/**/#if 0   /* OLD */
/**/            iomnSendCountDownEvent( msgQIdCpyUI, IOMSG_ECDN_UI );
/**/#else   /* D4.8 MID     NEW */
/**/            if( msgQIdioMNforCpyJob == msgQIdCpyUI ) {
/**/                iomnSendCountDownEvent( msgQIdioMNforCpyJob, IOMSG_ECDN_UI );
/**/            }
/**/#endif
/**//* D4.8 END */
/**/        }
/**/        /* D3.20 END */
/**/
/**/        /* prnUI or 管理タスクにECDNを送信 */
/**/        return( iomnSendPrintEventMsg(hJobID,hPrnID,IOMSG_ECDN_UI,IOMSG_ECDN_JOB,&ioMNrmyparam) );
/**/    }
#endif

    /*  JOB毎メッセージQue取得  */
    msgqid = ioMNCjst[hJobID].OldmsgQId;
#if 0   /* MCK1.1   */
/**/    if(msgqid == NULL){
/**/        msgqid = msgQIdCpyUI;
/**/    }
/**/    if(ioMNCjst[hJobID].iOldMsgId == IOMSG_CJST_UI 
/**/        || ioMNCjst[hJobID].iOldMsgId == 0){
/**/        imsgid = IOMSG_ECDN_UI;
/**/    }
/**/    if(ioMNCjst[hJobID].iOldMsgId == IOMSG_CJST_JOB)    imsgid = IOMSG_ECDN_JOB;
#else
    if(msgqid == NULL){
        iomnDebugMessage(">>>Undefined MessageID(iomnCountDown)->%x\n",ioMNCjst[hJobID].iOldMsgId);
        return(OK);
    }
    imsgid = JOB_MSG_EPCD;
#endif

/* D3.20 START T.Fujii ECDN送信処理修正(ソースコード整理) */
#if 0   /* OLD */
/**/    /*  印刷ＵＩが表示されている場合は、印刷ＵＩに対してメッセージ送信  */
/**/    if(ioMNhw.hPriUi == UI_PRINT){
/**/    }
#endif
/* D3.20 END */

    /*  メッセージ送信  */
    if(ERROR == (Status = iomnSendCountDownEvent(msgqid, imsgid))){
        iomnDebugMessage(">>>error:iomnSendCountDownEvent()\n",0);
        return(ERROR);
    }
    return(OK);
}


/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         7.20
TITLE:      ページ印刷終了通知受信処理
MODULE：    STATUS  iomnFinishPagePrn()

INCLUDE:    ioMNPar.h
PARAMETER:
            N/A
RETURNS:    I32 4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            ページ印刷終了通知受信時の処理。JOB管理、又はUIにイベントを
            通知する。
NOTES:
HISTORY:
        1.1     Y.Matsuda   96/9/18
        D1.6    T.Fujii     00/02/09 DM45/35 prnUI表示対応
        D2.8    T.Fujii     00/07/01 DM45/35 印刷系イベント送信処理修正

        MCK1.1  M.Karasawa 02/06/11 McKinley    新メッセージ対応
*/
STATUS  iomnFinishPagePrn()
{
#if 0   /* MCK1.1 新メッセージ対応  *//* 要検討 */
/**/    STATUS  Status;
/**/    HI          hJobID;
/**/    MSG_Q_ID    msgqid;
/**/    I32         imsgid = -1;    /* D1.1 T.Fujii ダミー定義追加 */
/**/    HI          hPrnID = -1;
/**/
/**/    /*  受信パラメータ取得  */
/**/    hJobID = ioMNrmyparam.sEpedCtl.hJobid;
/**/    iomnDebugMessage("###JOBID->%x\n",(I32)hJobID);
/**/
/**/    /* 印刷ジョブの場合、印刷種別を取得してメッセージを送信 */
/**/    if( (hPrnID = iomnPrnQueCheckPrnType(hJobID)) >= 0 ) {
/**/        /* prnUI or 管理タスクにEPEDを送信 */
/**/        return( iomnSendPrintEventMsg(hJobID,hPrnID,IOMSG_EPED_UI,IOMSG_EPED_JOB,&ioMNrmyparam) );
/**/    }
/**/
/**/    /*  JOB毎メッセージQue取得  */
/**/    msgqid = ioMNCjst[hJobID].OldmsgQId;
/**/    /*  基本複写リカバリ中でJOBが完了してしまっている場合、複写UIに送信する */
/**/    msgqid = ioMNCjst[hJobID].OldmsgQId;
/**/    if(msgqid == NULL){
/**/        msgqid = msgQIdCpyUI;
/**/    }
/**/    if(ioMNCjst[hJobID].iOldMsgId == IOMSG_CJST_UI 
/**/        || ioMNCjst[hJobID].iOldMsgId == 0){
/**/        imsgid = IOMSG_EPED_UI;
/**/    }
/**/    if(ioMNCjst[hJobID].iOldMsgId == IOMSG_CJST_JOB)    imsgid = IOMSG_EPED_JOB;
/**/
/**/    /*  印刷ＵＩが表示されている場合は、印刷ＵＩに対してメッセージ送信  */
/**/    if(ioMNhw.hPriUi == UI_PRINT){
/**/    }
/**/    /*  メッセージ送信  */
/**/    if(ERROR == (Status = iomnSendPageEndEvent(msgqid, imsgid))){
/**/        iomnDebugMessage(">>>error:iomnSendPageEndEvent()\n",0);
/**/        return(ERROR);
/**/    }
#endif
    return(OK);
}

/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         7.21
TITLE:      メモリ残量通知受信処理
MODULE：    STATUS  iomnMemoryFullEvent()

INCLUDE:    ioMNPar.h
PARAMETER:
            N/A
RETURNS:    I32 4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            メモリ残量通知受信時の処理。JOB管理、又はUIにイベントを
            通知する。
NOTES:
HISTORY:
        1.1     Y.Matsuda   96/9/18

        MCK1.1  M.Karasawa 02/06/11 McKinley    新メッセージ対応
    RioGrande
        Beta3   M.Karasawa 04/10/26 RioGrande   原稿枚数対応
*/
STATUS  iomnRecMemoryEvent()
{
#if 0   /* MCK1.1   */
/**/    STATUS      Status;
/**/    HI          hJobID;
/**/    MSG_Q_ID    msgqid;
/**/    HI          hJobType;
/**/    I32         imsgid = IOMSG_EMEM_UI; /* D1.1 T.Fujii ダミー定義追加 */
/**/    HI          hOrgNum;        /*  原稿ファイル化枚数  */
/**/    I32         iRemainCapacity;    /*  ファイル残容量  */
/**/    I32         iFileArea;      /*  種別毎ファイル領域  */
/**/
/**/    /*  受信パラメータ取得  */
/**/    hJobID          = ioMNrmyparam.sEmemCtl.hJobid;
/**/    hOrgNum         = ioMNrmyparam.sEmemCtl.hOrgNum;
/**/    iRemainCapacity = ioMNrmyparam.sEmemCtl.iRemainCapacity;
/**/    iFileArea       = ioMNrmyparam.sEmemCtl.iFileArea;
/**/
/**/    /*  受信パラメータ表示  */
/**/    iomnDebugMessage("### JOBID -> %x\n",(I32)hJobID);
/**/    iomnDebugMessage("### File Count -> %x\n",(I32)hOrgNum);
/**/    iomnDebugMessage("### Residue of File Size -> %d(Byte)\n",iRemainCapacity);
/**/    iomnDebugMessage("### All File Size -> %d(Byte)\n",iFileArea);
/**/
/**/    /*  JOBIDよりJOB種別取得    */
/**/    if(ERROR == (Status = jobJobtype(hJobID, &hJobType))){
/**/        iomnDebugMessage(">>>error:jobJobtype() in iomnRecMemoryEvent()\n",0);
/**/        return(ERROR);
/**/    }
/**/    /*  Save Message Parameter  */
/**/    if(hJobID == hioMNFrontJobID || hJobID == hioMNIntJobID) iomnSetJobManagementMsg(&ioMNEmemFront);
/**/    else    iomnSetJobManagementMsg(&ioMNEmemBack);
/**/
/**/    /*  JOB毎メッセージQue取得  */
/**/    msgqid = ioMNCjst[hJobID].OldmsgQId;
/**/    if(ioMNCjst[hJobID].iOldMsgId == IOMSG_CJST_UI) imsgid = IOMSG_EMEM_UI;
/**/    if(ioMNCjst[hJobID].iOldMsgId == IOMSG_CJST_JOB)    imsgid = IOMSG_EMEM_JOB;
/**/
/**/    /*  印刷ＵＩが表示されている場合は、印刷ＵＩに対してメッセージ送信  */
/**/    if(ioMNhw.hPriUi == UI_PRINT){
/**/    }
/**/    /*  メッセージ送信  */
/**/    if(ERROR == (Status = iomnSendMemoryResidue(msgqid, imsgid))){
/**/        iomnDebugMessage(">>>error:iomnSendMemoryResidue()\n",0);
/**/        return(ERROR);
/**/    }
#endif
/* Beta3 *//* START */
    STATUS      Status;
    HI          hJobID;
    MSG_Q_ID    msgqid;
    I32         imsgid;

    /*  受信パラメータ取得  */
    hJobID          = ioMNrmyparam.sEjmeCtl.hJobid;

    /*  Save Message Parameter  *//* 目的不明だが一応・・・ */
    if(hJobID == hioMNFrontJobID || hJobID == hioMNIntJobID) iomnSetJobManagementMsg(&ioMNEmemFront);
    else    iomnSetJobManagementMsg(&ioMNEmemBack);

    msgqid = ioMNCjst[hJobID].OldmsgQId;
    imsgid = JOB_MSG_EJME;
    if(ERROR == (Status = iomnSendMemoryResidue(msgqid, imsgid))){
        iomnDebugMessage(">>>error:iomnSendMemoryResidue()\n",0);
        return(ERROR);
    }
/* Beta3 *//* END   */
    return(OK);
}


/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         7.22
TITLE:      リスト／レポート作成要求
MODULE：    STATUS  iomnRecieveRLST()

INCLUDE:    ioMNPar.h
            ioMNstr.h
PARAMETER:
            N/A
RETURNS:    I32 4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            ＦＡＸＪＯＢ管理よりリスト／レポート作成のリクエストを受信した
            場合の処理
NOTES:
HISTORY:
        1.1     Y.Matsuda   96/9/18
*/
STATUS  iomnRecieveRLST()
{
    return(OK);
}


/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         7.23
TITLE:      遅延中断取り消し指示受信処理
MODULE：    STATUS  iomnRecieveCCCN()
INCLUDE:    ioMNPar.h
            ioMNstr.h
PARAMETER:
            N/A
RETURNS:    I32 4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            ＪＯＢ遅延中断取り消しが指示された場合、シーケンス状態を見た上で
            中断取り消し可能であれば遅延中断取り消しを行う
NOTES:
HISTORY:
        1.1     Y.Matsuda   96/01/27

        MCK1.1  M.Karasawa 02/06/11 McKinley    新メッセージ対応
    V016.00_02  M.Karasawa  03/03/26    McKinley    メッセージ置換                  構造体置換
                                                    JOB_MSG_CCIC -> JOB_MSG_CPSW    JOB_STR_CCIC -> JOB_STR_CPSW
                                                    JOB_MSG_TCIC -> JOB_MSG_TPSW    JOB_STR_TCIC -> JOB_STR_TPSW
                                                    新規メッセージ
                                                    JOB_MSG_EPDS                    JOB_STR_ECIC -> JOB_STR_EPDS
    V023.00_02  M.Karasawa  03/07/16    McKinley    ソース整理（不要処理削除）
*/
STATUS  iomnRecieveCCCN()
{
    STATUS  Status;
    HI      hJobid;

    iomnDebugMessage(">>>Top of iomnSendCCCN()\n",0);

    /*  受信パラメータ取得  */
    hJobid = ioMNrmyparam.sCccnCtl.hJobid;
    iomnDebugMessage("###JOBID ->%x\n",(I32)hJobid);

    /*  Save Message Parameter  */
    iomnSetJobManagementMsg(&ioMNCccn);
    /*  すでに該当ＪＯＢのＴＪＳＵを受信していた場合    */
    if(ioMNTjsu[hJobid].iOldTaskId != 0){
#if 0  /* V023.00_02 START */
/**/        /*  シーケンスErrorメッセージ送信   */
/**/        if(ERROR == (Status = iomnSendErrMessage(ioMNCccn.OldmsgQId, 0))){
/**/            cmnPrintf_ioMN(2,">>>Error:iomnSendErrMessage()\n");
/**/            return(ERROR);
/**/        }
/**/        iomnDebugMessage(">>>Send EA01 Message\n",0);
#endif /* V023.00_02 END   */
        /*  CCCN Message Table Initialize   */
        memset(&ioMNCccn, 0x00, sizeof(struct msgstore));
        iomnDebugMessage(">>>Clear CCCN Message Table\n",0);
    }
    /*  JOBのTJSUを受信していない場合   */
    else{
        /*  JOB遅延中断中止メッセージ送信   */
#if 0   /* MCK1.1 新メッセージ対応  */
/**/        if(ERROR == (Status = iomnSendCCCN(hJobid, msgQIdIoExMN, IOMSG_CCCN_EX))){
#else
#if 0   /* V016.00_02   */
/**/        if(ERROR == (Status = iomnSendCCCN(hJobid, msgQIdIoExMN, JOB_MSG_CCIC))){
#else
        if(ERROR == (Status = iomnSendCCCN(hJobid, msgQIdIoExMN, JOB_MSG_CPSW))){
#endif  /* V016.00_02   */
#endif
            iomnDebugMessage(">>>Error:iomnSendCCCN()\n",0);
            return(ERROR);
        }
        iomnDebugMessage(">>>OK:Send CCCN Message\n",0);
    }

    iomnDebugMessage(">>>Bottom of iomnSendCCCN()\n",0);
    return(OK);
}


/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         7.24
TITLE:      遅延中断取り消しTrm受信処理
MODULE：    STATUS  iomnRecieveTCCN()
INCLUDE:    ioMNPar.h
            ioMNstr.h
PARAMETER:
            N/A
RETURNS:    I32 4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            遅延中断取り消し指示に対するステータスを受け取り、中断処理中の
            状態管理テーブルを初期化し、上位タスクに対して遅延中断取り消し
            ステータスを通知する。
NOTES:
HISTORY:
        1.1     Y.Matsuda   96/01/27
        D1.2.5  T.Fujii     00/04/10    DM45/35 中止処理修正
        D3.11.2 Y.Shimada   00/09/07    DM45/35 中断中の割り込み全面見直し

        MCK1.1  M.Karasawa 02/06/11 McKinley    新メッセージ対応
    V016.00_02  M.Karasawa  03/03/26    McKinley    メッセージ置換                  構造体置換
                                                    JOB_MSG_CCIC -> JOB_MSG_CPSW    JOB_STR_CCIC -> JOB_STR_CPSW
                                                    JOB_MSG_TCIC -> JOB_MSG_TPSW    JOB_STR_TCIC -> JOB_STR_TPSW
                                                    新規メッセージ
                                                    JOB_MSG_EPDS                    JOB_STR_ECIC -> JOB_STR_EPDS
    RioGrande:
    VTR14.000_1 M.Karasawa  05/06/18    RioGrande   遅延中断キャンセル後の割り込み動作不具合(McK8534)
    eB3-01      M.Taki      07/07/25    BP/MASH     eB3コンパイルオプション追加(BP_ENG,MASH_ENG)

*/
STATUS  iomnRecieveTCCN()
{
    STATUS  Status; 
    HI      hJobid;     
    I32     imsgid;     /*  遅延中断中止ステータスメッセージＩＤ    */
/*    HI      hJobtype;                         *//* VTR14.000 */
/*    HI      hQueId;     *//*  中断該当QUEID   *//* VTR14.000 */

    /*  パラメータ取得  */
    hJobid = ioMNrmyparam.sTccnCtl.hJobid;
    iomnDebugMessage("###JOBID->%x\n",(I32)hJobid);
    
    /*  メッセージＩＤ取得  */
#if 0   /* MCK1.1 新メッセージ対応  */
/**/    if(ioMNCccn.iOldMsgId == IOMSG_CCCN_UI){
/**/        imsgid = IOMSG_TCCN_UI;
/**/    }
/**/    else{
/**/        imsgid = IOMSG_TCCN_JOB;
/**/    }
#else
#if 0   /* V016.00_02   */
/**/    imsgid = JOB_MSG_TCIC;
#else
    imsgid = JOB_MSG_TPSW;
#endif
#endif
    iomnDebugMessage("###MSGID->%x\n",imsgid);

#if 0   /* VTR14.000 */
/**/    /*  ＪＯＢ種別取得  */
/**/    if(ERROR == (Status = jobJobtype(hJobid, &hJobtype))){
/**/        cmnPrintf_ioMN(2,">>>Error:jobJobtype()\n");
/**/        printErrno(errno);
/**/        return(ERROR);
/**/    }
#endif  /* VTR14.000 */
/* eB3-01 */
/* #ifdef K2_ENG *//* EFI接続時の割り込み対応 */
#if defined (K2_ENG) || defined (BP_ENG) || defined (MASH_ENG) || defined (WEISS_ENG) || defined (S2_ENG)
#if 0 /* missing.c change*/
    jcLibQUE_intruptset(JCLIB_PRN_DEL);
#endif
#endif
    /*  割り込み処理中フラグ解除    */
    ioMNstatus.hPPCInt  = IOMN_OFF;
/* D3.11.2 START Y.Shimada CHG */
#if 0   /* OLD */
    ioMNstatus.hPPCIntReserve = IOMN_OFF;
#else   /* D3.11.2 MID  NEW */
    ioMNPJ_IntSet(hJobid, IOMN_PJ_OFF);             /* VTR14.000 */
    if(ioMNPJ_IntReserveCheck() == ERROR ){         /* VTR14.000 */
        ioMNstatus.hPPCIntReserve = IOMN_UNDEF;
    }                                               /* VTR14.000 */
#endif
/* D3.11.2 END */
    iomnDebugMessage(">>>OK:Clear Int Flag & Int Reserve Flag Clear\n",0);

    /*  FrontJOB中断処理中フラグクリア   */
    if(hJobid == hioMNFrontJobID || hJobid == hioMNIntJobID){
        ioMNstatus.FrontSuspend.hSusTask = IOMN_OFF;
        ioMNstatus.FrontSuspend.hSusType = IOMN_OFF;
        iomnDebugMessage(">>>OK:Clear Front JOB SUS Flag\n",0);
    }
    else if( hJobid == hioMNDataRecvJobID ) {
        /* データ転送ジョブのときは何もしない */
    }
    /*  BackGround JOB中止/中断フラグクリア */
    else{
        ioMNstatus.BackCancel.hCanTask = IOMN_OFF;
        ioMNstatus.BackCancel.hCanType = IOMN_OFF;

#if 0   /* VTR14.000 */
/**/        /*  エントリＩＤ取得    */
/**/        if(ERROR == (Status = iomnGetEntryIdFromJobtype(hJobtype ,&hQueId))){
/**/            iomnDebugMessage(">>>Error:iomnGetEntryIdFromJobtype()\n",0);
/**/            return(ERROR);
/**/        }
/**/        iomnDebugMessage("###QUEID->%x\n",(I32)hQueId);
/**/
/**/        /*  中断フラグクリア    */
/**/        ioMNstatus.BackSuspend[hQueId].hSusTask = IOMN_OFF;
/**/        ioMNstatus.BackSuspend[hQueId].hSusType = IOMN_OFF;
#else   /* VTR14.000 */
        ioMNPJ_SetSusInfo(hJobid, IOMN_OFF, IOMN_OFF);
#endif  /* VTR14.000 */
        iomnDebugMessage(">>>OK:Clear Back JOB CAN/SUS Flug\n",0);
    }
    
    /*  JOB遅延中断中止ステータス送信   */
    if(ERROR == (Status = iomnSendTCCN(hJobid, ioMNCccn.OldmsgQId, imsgid))){
        iomnDebugMessage(">>>Error:iomnSendTCCN()\n",0);
        return(ERROR);
    }
    iomnDebugMessage(">>>Send TCCN Message\n",0);
/* V016.00_02 START */
    if(ERROR == (Status = iomnSendTCCN(hJobid, ioMNCjst[hJobid].OldmsgQId, imsgid))){
        iomnDebugMessage(">>>Error:iomnSendTCCN()\n",0);
        return(ERROR);
    }
    iomnDebugMessage(">>>Send TCCN Message\n",0);
/* V016.00_02 END   */
    return(OK);
}


/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         7.25
TITLE:      JOB削除指示受信処理
MODULE：    STATUS  iomnRecieveCDJB()
INCLUDE:    ioMNPar.h
            ioMNstr.h
PARAMETER:
            N/A
RETURNS:    I32 4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            JOB操作UIよりJOB削除指示を受け取り、JOB状態により、JOB中止処理
            または印刷QUE削除を行う。
NOTES:
HISTORY:
        1.1     Y.Matsuda   97/04/5
        V2.6    J.Ootani    97/12/08
            ＤＳＳＪＯＢ削除処理対応
        M4.1?   (TJ)H.Takahashi 99/01/26
                CDJB変更対応
        M6.2    (TJ)H.Takahashi 99/05/20
                不具合対応(F300T2BA0014)
                予めカバーオープン中にJOB受信。JOB削除をパソコンから送信.
                JOB削除されず。
        D1.2    T.Fujii     99/12/24 DM45/35 (後追い)逐次印刷対応
        D1.13.1 T.Fujii     00/05/20 DM45/35 サブジョブ中止処理修正
        D2.1    T.Fujii     00/06/06 DM45/35 V004.60不具合対応(サブジョブ中止処理再修正)
        D5.0    T.Fujii     00/12/12 DM45/35 CDJB多重動作対応
        D6.0    T.Fujii     01/10/20 DM45/35 TDJBを送信しない不具合を修正

        MCK1.1  M.Karasawa 02/06/11 McKinley    新メッセージ対応
*/
STATUS  iomnRecieveCDJB()
{
#if 0   /* MCK1.1 新メッセージ対応  *//* 関数削除可能*/
/**/    STATUS  Status;     /*  関数戻り値  */
/**/    STATUS  JobDelSts;  /*  JOB削除可能判断結果 */
/**/    HI      hJobid;     /*  JOBID   */
/**/    HI      hJobstatus; /*  JOB状態 */
/**/    I32     iSuscause;  /*  中断要因    */
/**/    HI      hJobtype;   /*  JOB種別 */
/**/    HI      hDelFacter; /*  JOB削除要因 */
/**/
/**/    /*  JOBID取得   */
/**/    hJobid = ioMNrmyparam.sCdjbCtl.hJobid;
/**/    iomnDebugMessage("###JOBID->%x\n",(I32)hJobid);
/**/
/**/    /* JOB削除要因取得 */
/**/    hDelFacter = ioMNrmyparam.sCdjbCtl.hDelFacter;
/**/    /* エントリＩＤの取り出し */
/**/    hIoMNPrnQueEntryID = ioMNrmyparam.sCdjbCtl.hEntryid;
/**/
/**/    /*  JOB削除可能判断 */
/**/    if(ERROR == (JobDelSts = iomnJobDeleteConfirm(hJobid, &hJobtype, &hJobstatus, &iSuscause))){
/**/        iomnDebugMessage(">>>Error:iomnJobDeleteConfirm()\n",0);
/**/        return(ERROR);
/**/    }
/**/    iomnDebugMessage("###JOB Delete Status->%x\n",JobDelSts);
/**/
/**/    /*  ＪＯＢ削除メッセージ格納    */
/**//* D5.0 START T.Fujii CDJB多重動作対応 */
/**/#if 0   /* OLD */
/**/    iomnSetJobManagementMsg(&ioMNCdjb);
/**/#else   /* D5.0 NEW */
/**/    if( ioMNCdjb[hJobid].OldmsgQId != NULL ) {
/**/        /*  送信パラメータ作成  */
/**/        ioMNsmyparam.sTdjbCtl.hJobid = hJobid;
/**/        ioMNsmyparam.sTdjbCtl.hSts = FALSE;
/**/        ioMNsmyparam.sTdjbCtl.hErr = IOMSG_ERR_JOBID;
/**/
/**/        /*  メッセージ送信  */
/**/        if( iomnSendMessage(ioMNMsgQId,IOMSG_TDJB_UI,0,&ioMNsmyparam) == ERROR ) {
/**/            puts( "[ioMN]Error:iomnSendMessage() in iomnRecieveCDJB()" );
/**/            return( ERROR );
/**/        }
/**/        return( OK );
/**/    }
/**/    else {
/**/        iomnSetJobManagementMsg(&ioMNCdjb[hJobid]);
/**/    }
/**/#endif
/**//* D5.0 END */
/**/
/**/    /*  ＪＯＢ削除処理  */
/**/    if(JobDelSts == IOMN_ON){
/**/        /*  JOB削除受付Ack送信  */
/**/        if(ERROR == (Status = iomnSendADJB(hJobid,hJobtype))){
/**/            cmnPrintf_ioMN(2,">>>Error:iomnSendADJB()\n");
/**/            return(ERROR);
/**/        }
/**/
/**/        /*  JOB中断状態または   */
/**/        /*  JOB実行中でAck受信済みの場合、JOB中止指示送信   */
/**/        /* JOB状態が中断中、実行中、印刷中でAJST受信済みの場合、JOB中止指示送信*/
/**/        /* JOB状態が中断中でもAJSTを未受信(CJST未送信)の場合があるため */
/**/        if ( (hJobstatus==COM_JS_SUSPEND || hJobstatus==COM_JS_RUNNING ||
/**/            hJobstatus==COM_JS_PRINTING) 
/**/            && ioMNAjst[hJobid].OldmsgQId != NULL ){
/**/            /*  ＪＯＢ中止指示送信  */
/**/            if( iomnSubJobCancel(hJobid,hJobtype,hDelFacter) == ERROR ) {   /* 入出力ジョブの中止処理 */
/**/                iomnDebugMessage(">> ioMN >> ERROR:iomnSubJobCancel() in iomnJobCancel()\n",0);
/**/                return(ERROR);
/**/            }
/**/            iomnDebugMessage(">>>OK:iomnSendJobCancel()\n",0);
/**/        }
/**/
/**/        /*  JOB実行中状態で且つAck未受信の場合、JOB削除フラグ設定   */
/**/        else if((hJobstatus == COM_JS_RUNNING || hJobstatus == COM_JS_PRINTING)
/**/                    && ioMNAjst[hJobid].OldmsgQId == NULL){
/**/            ioMNstatus.hJobDelete = IOMN_ON;
/**/        }
/**/
/**/        /*  上記以外の場合はJOB削除処理 */
/**/        else{
/**/            if( iomnSubJobCancel(hJobid,hJobtype,hDelFacter) == ERROR ) {
/**/                /* 入出力ジョブの中止処理 */
/**/                iomnDebugMessage(">> ioMN >> ERROR:iomnSubJobCancel() in iomnJobCancel()\n",0);
/**/                /* D6.0 START T.Fujii TDJBを送信しない不具合を修正 */
/**/                /*  JOB削除ステータス送信   */
/**/                if( iomnSendTDJB(hJobid, FALSE, IOMSG_ERR_JOBID) == ERROR ) {
/**/                    iomnDebugMessage(">>>Error:iomnSendTDJB()\n",0);
/**/                }
/**/                iomnDebugMessage(">>>Impossible to Job Delete\n",0);
/**/                /* D6.0 END */
/**/                return( ERROR );
/**/            }
/**/            iomnDebugMessage(">> ioMN >> OK:iomnSendJobCancel()\n",0);
/**/ 
/**/            /*  JOB削除ステータス送信   */
/**/            if(ERROR == (Status = iomnSendTDJB(hJobid, TRUE, IOMSG_NOT_USED))){
/**/                iomnDebugMessage("ERROR:iomnSendTDJB()\n",0);
/**/                return(ERROR);
/**/            }
/**/            iomnDebugMessage(">>>OK:iomnSendTDJB()\n",0);
/**/        }
/**/    }
/**/    else if(JobDelSts == IOMN_OFF){
/**/        /*  JOB削除ステータス送信   */
/**/        if(ERROR == (Status = iomnSendTDJB(hJobid, FALSE, IOMSG_ERR_JOBID))){
/**/            iomnDebugMessage(">>>Error:iomnSendTDJB()\n",0);
/**/            return(ERROR);
/**/        }
/**/        iomnDebugMessage(">>>Impossible to Job Delete\n",0);
/**/    }
/**/    else{
/**/        /*  JOB削除確認処理でERROR発生する場合  */
/**/        return(ERROR);
/**/    }
#endif
    return(OK);
}


/*
(C) Copyright TOSHIBA Corporation 2000,2000. ALL Right Reserved
NO.
TITLE:      印刷ジョブ入れ替え処理
MODULE：    STATUS  iomnRecieveCCJB()
INCLUDE:
PARAMETER:
RETURNS:    I32 4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            ｐｒｎＵＩより印刷ジョブの入れ替え指示を受け取り、
            印刷ジョブの入れ替えを行う。
NOTES:
HISTORY:
        D1.8    Y.Shimada       '00/ 3/ 9 DM45/35 印刷ジョブ１本化
*/
STATUS iomnRecieveCCJB()
{
    HI    hSEntryID;
    HI    hDEntryID;
    HI    hMoveMode;
    HI    hJobID;
    HI    hJobMode;
    HI    hJobstatus;
    I32   iSuscause;

    /* 引数の取り出し */
    hSEntryID = ioMNrmyparam.sCcjbCtl.hSEntryID;
    hDEntryID = ioMNrmyparam.sCcjbCtl.hDEntryID;
    hMoveMode = ioMNrmyparam.sCcjbCtl.hMoveMode;

    /* 無条件にＵＩへ、印刷ジョブ入れ替え指示受付（ＡＣＪＢ）を返す */
    if( iomnSendACJB( hSEntryID ) == ERROR ){
        cmnPrintf_ioMN(2,">>>Error:iomnSendACJB() in iomnRecieveCCJB()\n");
        printErrno(errno);
        /* 印刷ジョブ入れ替え終了（ＴＣＪＢ：エラー）送信 */
        iomnSendTCJB( hSEntryID, ERROR, S_ioMN_JOB_MOVE_NOT_EXEC );
        return( ERROR );
    }
    iomnDebugMessage(">>>OK:iomnSendACJB() in iomnRecieveCCJB()\n",0);

    /* 印刷ジョブのエントリＩＤと移動モードの正当性のチェック */
    if(( hSEntryID < 0 ) || ( hSEntryID >= MAX_NUM_QUEENTRY ) ||
       ( hDEntryID < 0 ) || ( hDEntryID >= MAX_NUM_QUEENTRY ) ||
       (( hMoveMode != PRNQUE_MOVE_PREFIX ) && ( hMoveMode != PRNQUE_MOVE_BEHIND ))){
        /* 印刷ジョブ入れ替え終了（ＴＣＪＢ：エラー）送信 */
        iomnSendTCJB( hSEntryID, ERROR, S_ioMN_JOB_MOVE_NOT_EXEC );
        cmnPrintf_ioMN(2,">>>Error:Parameter in iomnRecieveCCJB()\n");
        return( ERROR );
    }
    iomnDebugMessage(">>>OK:Parameter Check in iomnRecieveCCJB()\n",0);

    /* 移動元エントリＩＤ未登録チェック */
    if( ioMNQueLibSEntryIDToJobMode( hSEntryID, &hJobID, &hJobMode) == ERROR ){
        /* 印刷ジョブ入れ替え終了（ＴＣＪＢ：エラー）送信 */
        iomnSendTCJB( hSEntryID, ERROR, S_ioMN_JOB_MOVE_SRC_EID );
        cmnPrintf_ioMN(2,">>>Error:Src EntryID in iomnRecieveCCJB()\n");
        return( ERROR );
    }
    iomnDebugMessage(">>>OK:Src EntryID Check in iomnRecieveCCJB()\n",0);

    /* 移動元実行可否チェック（ジョブ状態） */
    if( jobStatus( hJobID, &hJobstatus, &iSuscause) == ERROR ){
        /* 印刷ジョブ入れ替え終了（ＴＣＪＢ：エラー）送信 */
        iomnSendTCJB( hSEntryID, ERROR, S_ioMN_JOB_MOVE_NOT_EXEC );
        cmnPrintf_ioMN(2,">>>Error:Src Job jobStatus()\n");
        printErrno(errno);
        return(ERROR);
    }
    iomnDebugMessage("###MOVE:SRC_JOB Status->%x\n",hJobstatus);
    /* 移動元ジョブが動作中、又は、印刷中なら移動は不可とする */
    if(( hJobstatus == COM_JS_RUNNING )||( hJobstatus == COM_JS_PRINTING )){
        /* 印刷ジョブ入れ替え終了（ＴＣＪＢ：エラー）送信 */
        iomnSendTCJB( hSEntryID, ERROR, S_ioMN_JOB_MOVE_NOT_EXEC );
        cmnPrintf_ioMN(2,">>>Error:Src Job RUNNING!!\n");
        return(ERROR);
    }

    /* 移動先エントリＩＤ未登録チェック */
    if( ioMNQueLibSEntryIDToJobMode( hSEntryID, &hJobID, &hJobMode) == ERROR ){
        /* 印刷ジョブ入れ替え終了（ＴＣＪＢ：エラー）送信 */
        iomnSendTCJB( hSEntryID, ERROR, S_ioMN_JOB_MOVE_DIS_EID );
        cmnPrintf_ioMN(2,">>>Error:Dis EntryID in iomnRecieveCCJB()\n");
        return( ERROR );
    }
    iomnDebugMessage(">>>OK:Dis EntryID Check in iomnRecieveCCJB()\n",0);

    /* 移動先実行可否チェック（ジョブ状態） */
    if( jobStatus( hJobID, &hJobstatus, &iSuscause) == ERROR ){
        /* 印刷ジョブ入れ替え終了（ＴＣＪＢ：エラー）送信 */
        iomnSendTCJB( hSEntryID, ERROR, S_ioMN_JOB_MOVE_NOT_EXEC );
        cmnPrintf_ioMN(2,">>>Error:Dis Job jobStatus()\n");
        printErrno(errno);
        return(ERROR);
    }
    iomnDebugMessage("###MOVE:DIS_JOB Status->%x\n",hJobstatus);
    /* 移動先ジョブが動作中、又は、印刷中で、尚且つ */
    if((( hJobstatus == COM_JS_RUNNING )||( hJobstatus == COM_JS_PRINTING ))&&
       /* 移動モードが前なら移動は不可とする */
       ( hMoveMode == PRNQUE_MOVE_PREFIX )){
        /* 印刷ジョブ入れ替え終了（ＴＣＪＢ：エラー）送信 */
        iomnSendTCJB( hSEntryID, ERROR, S_ioMN_JOB_MOVE_NOT_EXEC );
        cmnPrintf_ioMN(2,">>>Error:Src Job RUNNING!!\n");
        return(ERROR);
    }

    /* 印刷ジョブの入れ替え実行 */
    if( iomnPrnQueJobMove( hSEntryID, hDEntryID, hMoveMode) == ERROR ){
        cmnPrintf_ioMN(2,">>>Error:iomnPrnQueJobMove() in iomnRecieveCCJB()\n");
        printErrno(errno);
        /* 印刷ジョブ入れ替え終了（ＴＣＪＢ：エラー）送信 */
        iomnSendTCJB( hSEntryID, ERROR, S_ioMN_JOB_MOVE_NOT_EXEC );
        return(ERROR);
    }
    iomnDebugMessage(">>>OK:iomnPrnQueJobMove() in iomnRecieveCCJB()\n",0);
    /* 印刷ジョブ入れ替え終了（ＴＣＪＢ：正常）送信 */
    iomnSendTCJB( hSEntryID, OK, IOMSG_NOT_USED );

    return( OK );
}


/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         7.25
TITLE:      メモリＦＵＬＬ通知受信処理
MODULE：    STATUS  iomnRecieveEFPR()

INCLUDE:    ioMNPar.h
            ioMNstr.h
PARAMETER:
            N/A
RETURNS:    I32 4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            Faxまたは拡張複写でメモリFULLが発生した場合に受け取り、拡張複写の場合は上位タスクに対して
            メモリFULL通知メッセージを送信する。
            又、Faxの場合は、Fax印刷Queの優先度を高くし、優先的にFaxJOBの排出を実行するよう設定する。
NOTES:
HISTORY:
        1.1     Y.Matsuda   96/01/27
        D4.8    T.Fujii     00/11/21 DM45/35 05テストコピーでcpyUIにメッセージ送信する不具合修正

        MCK1.1  M.Karasawa 02/06/11 McKinley    新メッセージ対応
*/
STATUS  iomnRecieveEFPR()
{
    STATUS  Status;     /*      */
    HI      hJobid;     /*  JOBID   */
    HI      hJobtype;   /*  JOB種別 */

    iomnDebugMessage(">>>Top of iomnRecieveEFPR()<<<\n",0);

    /*  Message格納 */
    hJobid = ioMNrmyparam.sEfprCtl.hJobid;
    iomnDebugMessage("###JOBID->%x\n",(I32)hJobid);

    /*  JOB種別取得 */
    if(ERROR == (Status = jobJobtype(hJobid, &hJobtype))){
        iomnDebugMessage(">>>Error:jobJobtype()\n",0);
        printErrno(errno);
    }
    iomnDebugMessage("###JOB TYPE -> %x\n",(I32)hJobtype);

    /*  FaxにおけるメモリFULLの場合 */
    if(hJobtype == COM_JB_POLL_RECV_FAX1
        || hJobtype == COM_JB_POLL_RECV_FAX2
        || hJobtype == COM_JB_PRT_FAX1
        || hJobtype == COM_JB_PRT_FAX2
        || hJobtype == COM_JB_DRC_PRT_FAX){

        /*  Memory Full状態フラグ設定   */
        ioMNstatus.memfull_sts = IOMN_ON;
        iomnDebugMessage("###Memory Full Status ->%x\n",(I32)ioMNstatus.memfull_sts);

        /*  割り込み複写中の場合は、メモリFULL処理予約して終了  */
        if(ioMNhw.hPriUi == UI_INTPPC){
            /*  並行印刷要求送信    */
            if(ERROR == (Status = iomnSendRDUP(hJobid))){
                iomnDebugMessage(">>>Error:iomnSendRDUP()\n",0);
                return(ERROR);
            }
            iomnDebugMessage(">>>OK:iomnSendRDUP()\n",0);
        }

        /*  割り込み複写以外の場合はメモリFULL処理を行う。  */
        else{
            /*  メモリFULL並行印刷開始処理  */
            if(ERROR == (Status = iomnFaxMemFull2(hJobid))){
                iomnDebugMessage(">>>Error:iomnFaxMemFull2()\n",0);
                return(ERROR);
            }
            iomnDebugMessage(">>>OK:iomnFaxMemFull2()\n",0);
        }
    }

    /*  拡張複写におけるメモリFULLの場合    */
    else{
        /*  メモリFULLメッセージ送信    */
/* D4.8 START T.Fujii メッセージ送信先修正 */
#if 0   /* OLD */
/**/        if(ERROR == (Status = iomnSendEFPR(msgQIdCpyUI, IOMSG_EFPR_UI, hJobid))){
/**/            iomnDebugMessage(">>>Error:iomnSendEFPR()\n",0);
/**/            return(ERROR);
/**/        }
#else   /* D4.8 MID     NEW */
        if( msgQIdioMNforCpyJob == msgQIdCpyUI ) {
#if 0   /* MCK1.1 新メッセージ対応  *//* 要検討 */
/**/            if(ERROR == (Status = iomnSendEFPR(msgQIdioMNforCpyJob, IOMSG_EFPR_UI, hJobid))){
#else
            if(ERROR == (Status = iomnSendEFPR(msgQIdioMNforCpyJob, SYS_MSG_EFPR, hJobid))){
#endif
                iomnDebugMessage(">>>Error:iomnSendEFPR()\n",0);
                return(ERROR);
            }
        }
#endif
/* D4.8 END */
        iomnDebugMessage(">>>OK:iomnSendEFPR()\n",0);
    }
    iomnDebugMessage(">>>Bottom of iomnRecieveEFPR()<<<\n",0);
    return(OK);
}


/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         7.25.2
TITLE:      メモリFULL時の並行印刷移行処理
MODULE：    STATUS  iomnFaxMemFull2(HI hJobid)

INCLUDE:    ioMNPar.h
            ioMNstr.h
PARAMETER:
            HI hJobid;
RETURNS:    I32 4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            FaxメモリFULL発生時に印刷待ちＱＵＥを調べ、Ｆａｘ印刷待ちＪＯＢが存在する場合は
            通常の印刷処理に移行し、印刷待ちＪＯＢが存在しない場合は並行印刷に移行する。
NOTES:
            並行印刷要求メッセージのJOBIDは重要な意味を持たないので、規定の範囲のJOBIDであれば
            任意の値が可能。
HISTORY:
        1.1     Y.Matsuda   97/01/27
        1.5     Y.Matsuda   97/06/02
*/
STATUS  iomnFaxMemFull2(HI hJobid)
{
    STATUS  Status;
    HI      hEntryId[MAX_NUM_PRN_QUE];  /*  Print QueID */
    HI      hQueNo;             /*  印刷Que */
    HI      hCount;             /*  Queエントリ数   */
    HI      hBackJobtype[MAX_NUM_PRN_QUE];  /*  バックグランドJOB種別   */
    HI      hBackJobid[MAX_NUM_PRN_QUE];    /*  バックグランドJOBID */

    iomnDebugMessage(">>>Top of iomnFaxMemFull()<<<\n",0);

    /*  印刷JOB取得 */
    if(ERROR == (Status = iomnPrnQueNextEntry( MAX_NUM_PRN_QUE, &hCount, hEntryId, hBackJobid, hBackJobtype))){
        iomnDebugMessage(">>>Error:quePrnNextEntry() in iomnPPCStartConfirm()\n",0);
        return(ERROR);
    }
    iomnDebugMessage(">>>OK:quePrnNextEntry()\n",0);

    /*  印刷待ちJOBが存在する場合   */
    if(hCount != 0){
        /*  各Queの先頭JOBにFax印刷が存在するかいなか確認   */
        for(hQueNo = 0; hQueNo<MAX_NUM_PRN_QUE; ++hQueNo){
            iomnDebugMessage("###BackJobid[%x]= ",(I32)hQueNo);
            iomnDebugMessage("%x\n",(I32)hBackJobid[hQueNo]);

            iomnDebugMessage("###BackJobtype->%x\n",(I32)hBackJobtype[hQueNo]);

            /*  Fax印刷JOBの場合はそのまま終了  */
            if(hBackJobtype[hQueNo] == COM_JB_POLL_RECV_FAX1
                || hBackJobtype[hQueNo] == COM_JB_POLL_RECV_FAX2
                || hBackJobtype[hQueNo] == COM_JB_PRT_FAX1
                || hBackJobtype[hQueNo] == COM_JB_PRT_FAX2
                || hBackJobtype[hQueNo] == COM_JB_DRC_PRT_FAX){
                iomnDebugMessage(">>>Fax Print JOB Exist!!\n",0);

                /*  JOB種別変更処理 */
                if(ERROR == (Status = iomnChangeMemfullJobtype(PRNQUE_FAX))){
                    cmnPrintf_ioMN(2,">>>Error:iomnChangeMemfullJobtype()\n");
                    return(ERROR);
                }
                iomnDebugMessage(">>>OK:iomnChangeMemfullJobtype()\n",0);
                break;
            }
        }
        /*  並行印刷要求送信    */
        if(ERROR == (Status = iomnSendRDUP(hJobid))){
            iomnDebugMessage(">>>Error:iomnSendRDUP()\n",0);
            return(ERROR);
        }
        iomnDebugMessage(">>>OK:iomnDirectPrint()\n",0);
    }

    /*  印刷待ちJOBが存在しない場合 */
    else{
        /*  並行印刷要求送信    */
        if(ERROR == (Status = iomnSendRDUP(hJobid))){
            iomnDebugMessage(">>>Error:iomnSendRDUP()\n",0);
            return(ERROR);
        }
        iomnDebugMessage(">>>OK:iomnDirectPrint()\n",0);
    }
    return(OK);
}


/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         7.25.3
TITLE:      メモリFULLに伴うJOB種別変更処理
MODULE：    STATUS  iomnChangeMemfullJobtype(HI hQueNo)
INCLUDE:    ioMNPar.h
            ioMNstr.h
PARAMETER:
            HI  hQueNo;     
RETURNS:    I32 4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            メモリFULL発生時点で受信済み印刷待ちJOBがあれば、それらの
            JOB種別をすべてメモリFULL印刷に変更する。
NOTES:
HISTORY:
        1.1     Y.Matsuda   97/10/08
*/
STATUS  iomnChangeMemfullJobtype(HI hQueNo)
{
    HI      hJobNumber; /*  印刷JOBのQueの先頭からの順番    */
    STATUS  Status;     
    I32     iEntryId;   /*  EntryID */
    HI      hFaxJobid;  /*  JOB種別変更を行うJOB    */
    HI      hDummy;

    iomnDebugMessage(">>>Top of iomnChangeMemfullJobtype()\n",0);

    iomnDebugMessage("###QUENO -> %x\n",(I32)hQueNo);
    /*  FAX印刷ＱｕｅにエントリがなくなるまでＪＯＢ種別変更処理実施 */
    for(hJobNumber = 1;hJobNumber <= MAX_NUM_PRN_QUEENTRY;++hJobNumber){
        iomnDebugMessage("###FAX JOB No.%x\n",(I32)hJobNumber);
        /*  EntryID取得 */
        iomnPrnQueSeqEntry( hQueNo, hJobNumber, (HI *)&iEntryId, &hFaxJobid, &hDummy);
        iomnDebugMessage("###Fax Print JOBID->%x\n",(HI)hFaxJobid);

        /*  JOB種別変更 */
        if(ERROR == (Status = jobFuncSelect(hFaxJobid, COM_JB_DRC_PRT_FAX, 
                                            COM_JB_BACKGROND, COM_JS_WAITING))){
            cmnPrintf_ioMN(2,">>>Error:jobFuncSelect()\n");
            printErrno(errno);
            return(ERROR);
        }
        iomnDebugMessage(">>>OK:Change JOB TYPE--->COM_JB_DRC_PRT_FAX\n",0);
    }

    iomnDebugMessage(">>>Bottom of iomnChangeMemfullJobtype()\n",0);
    return(OK);
}


/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         7.26
TITLE:      印刷開始通知受信処理
MODULE：    STATUS  iomnRecieveESPR()
INCLUDE:    ioMNPar.h
            ioMNstr.h
PARAMETER:
            N/A
RETURNS:    I32 4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            完了型拡張複写時、入力ＪＯＢが終了し印刷が開始された段階で
            入出力実行管理タスクより通知される。
            これをうけると、ＪＯＢ開始通知元に対して印刷開始通知を送信する。
NOTES:
HISTORY:
        1.1     Y.Matsuda   96/01/27
        D1.1    T.Fujii     99/11/15 DM45/35 後追い逐次機能組み込み

        MCK1.1  M.Karasawa 02/06/11 McKinley    新メッセージ対応
*/
STATUS  iomnRecieveESPR()
{
    HI      hJobid;
    I32     imsgid;
    HI      hPrnID;     /* 印刷ジョブ種別 */

    HI      hJcbID;     /* MCK1.1   */
    HI      hTNo;       /* MCK1.1   */

    /*  パラメータ獲得  */
#if 0   /* MCK1.1   */
/**/    hJobid = ioMNrmyparam.sEsprCtl.hJobid;
/**/    iomnDebugMessage("###JOBID->%x\n",(I32)hJobid);
#else
    hJobid  = ioMNrmyparam.sEpstCtl.hJobid;
    hJcbID  = ioMNrmyparam.sEpstCtl.hJcbID;
    hTNo    = ioMNrmyparam.sEpstCtl.hTNo;
#if 0
    iomnDebugMessage("###JOB ID ->%x\n",(I32)hJobid);
    iomnDebugMessage("###JCB ID ->%x\n",(I32)hJcbID);
    iomnDebugMessage("###TNO    ->%x\n",(I32)hTNo);
#endif
	cmnPrintf_ioMN(7,"###JOB ID ->%x, JCB ID ->%x, TNO    ->%x\n", (I32)hJobid, (I32)hJcbID, (I32)hTNo);
#endif

    if(ERROR == ioMNPJ_EPSTClear(hJobid)){                                                  /* PRINT_JOINT */
        iomnDebugMessage("[ioMN] ERROR : ioMNPJ_EPSTClear() in iomnRecieveESPR()\n",0);     /* PRINT_JOINT */
    }                                                                                       /* PRINT_JOINT */
	/* Feedback EBX_STFR_17062 --Start*/
	/* In case of SYNC COPY job , ioMNPJ_EPSTWait() is not called and hence based on hioMN_PrintEPSTWait, it is not possible to judge 
		if EPST is received or not. Hence hioMNEPST_received_status is assigned TRUE indicating EPST is received */
	if(iomnCheckSCS(hJobid) == TRUE)
		hioMNEPST_received_status = TRUE;
	/* Feedback EBX_STFR_17062 --End*/
#if 0   /* MCK1.1   */
/**/    if(ioMNCjst[hJobid].iOldMsgId == IOMSG_CJST_UI) imsgid = IOMSG_ESPR_UI;
/**/    else if(ioMNCjst[hJobid].iOldMsgId == IOMSG_CJST_JOB) imsgid = IOMSG_ESPR_JOB;
/**/    else{
/**/        iomnDebugMessage(">>>Undefined MessageID->%x\n",ioMNCjst[hJobid].iOldMsgId);
/**/        return(OK);
/**/    }
#else
    if(ioMNCjst[hJobid].iOldMsgId != JOB_MSG_CJST){
        iomnDebugMessage(">>>Undefined MessageID(iomnRecieveESPR)->%x\n",ioMNCjst[hJobid].iOldMsgId);
        return(OK);
    }
    imsgid = JOB_MSG_EPST;
#endif

    /* 印刷ジョブ種別を取得 */
    hPrnID = iomnPrnQueGetPrnType( hJobid );

    /* フロントUI,prnUI/管理タスクに印刷開始(ESPR)を送信 */
    if( iomnSendESPR(hJobid, hPrnID) == ERROR ) {
        iomnDebugMessage(">> ioMN >> Error:iomnSendESPR(%d) in iomnRecieveESPR()\n",hJobid);
        /* return(ERROR);   処理は継続させる */
    }

    iomnDebugMessage(">>>OK:iomnSendPrintStartEvent()\n",0);
    return(OK);
}


/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         7.27
TITLE:      ERROR発生通知受信処理
MODULE：    STATUS  iomnRecieveEERX()
INCLUDE:    ioMNPar.h
            ioMNstr.h
            ioMNmsg.h
PARAMETER:
            N/A
RETURNS:    I32 4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            拡張複写時、入力でERRORが発生し、且つJOBを継続可能な場合、ERRORを入出力実行管理
            より通知する際に使用する。
NOTES:
            マシンERRORが発生しJOBが停止する場合や、JOB継続しない場合には使用してはいけない。
            また、ここで通知される要因は中断要因としては保存されない。
HISTORY:
        1.1 Y.Matsuda   97/04/21
*/
STATUS  iomnRecieveEERX()
{

    return(OK);
}


/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         7.28
TITLE:      PM解放指示受信処理
MODULE：    STATUS  iomnRecieveCFPM()
INCLUDE:    ioMNPar.h
            ioMNstr.h
            ioMNmsg.h
PARAMETER:
            N/A
RETURNS:    I32 4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            HDを使用しない基本GDIの場合に、ＧＤＩより転送されたデータがすべて印刷されたことを
            gdiMNよりioMNに通知する際に使用する。
            入出力JOB管理ではpuJOBを削除し,PMJOBを解放した上で、GDIJOB管理タスクに通知する。

        -> ソースコード整理に伴い、上記処理前面削除（CFPMは未使用メッセージの為、CFPM受信時、
        ERRORを返す。）
NOTES:
HISTORY:
        1.1 Y.Matsuda   97/04/21
        1.2 E.Saka      97/11/07  削除時にJOB種別チェック追加
    D2.0  M.Fukuda  00/05/29  DM45/35 NOP_DEL  関数削除 iomnRecieveCFPMの処理削除
*/
STATUS  iomnRecieveCFPM()
{
    return(ERROR);
}

/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         7.29
TITLE:      入力完了イベント受信処理
MODULE：    STATUS  iomnRecieveCFPM()
INCLUDE:    ioMNPar.h
            ioMNstr.h
            ioMNmsg.h
PARAMETER:
            N/A
RETURNS:    I32 4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            拡張複写（逐次）時に、入力が終了し、印刷を開始することを上位側に通知するために使用する。
NOTES:
HISTORY:
        1.1 Y.Matsuda   97/05/07

        MCK1.1  M.Karasawa  02/06/11    McKinley    新メッセージ対応
        MCK1.4  M.Karasawa  02/07/18    IOMSG_EFSC_UI   →  JOB_MSG_ESCP変更
*/
STATUS  iomnRecieveEFSC()
{
    STATUS  Status;     /*  関数戻り値  */
    HI      hJobid;     /*  ＪＯＢＩＤ  */
#if 0   /* MCK1.4   */
/**/    HI      hSts;       /*  入力処理結果    */
/**/    I32     iErrSys;    /*  アプリ系ＥＲＲＯＲ要因  */
/**/    I32     iErrEng;    /*  エンジン系ＥＲＲＯＲ要因    */
#endif

    iomnDebugMessage(">>>Top of iomnRecieveEFSC()<<<\n",0);

    /*  Get Parameter   */
    hJobid = ioMNrmyparam.sEfscCtl.hJobid;
#if 0   /* MCK1.4   */
/**/    hSts = ioMNrmyparam.sEfscCtl.hSts;
/**/    iErrSys = ioMNrmyparam.sEfscCtl.iErrSys;
/**/    iErrEng = ioMNrmyparam.sEfscCtl.iErrEng;
/**/    iomnDebugMessage("###JOBID ->%x\n",(I32)hJobid);
/**/    iomnDebugMessage("###STATUS ->%x\n",(I32)hSts);
/**/    iomnDebugMessage("###System ERROR ->%x\n",iErrSys);
/**/    iomnDebugMessage("###Engine ERROR ->%x\n",iErrEng);
/**/    
/**/    /*  ＪＯＢ開始通知元に対して印刷開始通知を送信  */
/**/    if(ERROR == (Status = iomnSendEFSC(hJobid, ioMNCjst[hJobid].OldmsgQId, IOMSG_EFSC_UI, hSts, iErrSys, iErrEng))){
/**/        iomnDebugMessage(">>>Error:iomnSendEFSC()\n",0);
/**/        return(ERROR);
/**/    }
/**/    iomnDebugMessage(">>>OK:iomnSendEFSC()\n",0);
#else
    memcpy(&ioMNsmyparam, &ioMNrmyparam, sizeof(union ioMNparam));
    if(ERROR == (Status = iomnSendMessage(ioMNCjst[hJobid].OldmsgQId, JOB_MSG_ESCP, 0, &ioMNsmyparam))){
        iomnDebugMessage(">>>Error:iomnSendMessage EFSC\n",0);
        return(ERROR);
    }
    iomnDebugMessage(">>>OK:iomnSendMessage EFSC\n",0);
#endif

    iomnDebugMessage(">>>Bottom of iomnRecieveEFSC()<<<\n",0);
    
    return(OK);
}


/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         7.25
TITLE:      メモリfull解除処理
MODULE：    STATUS  iomnRecieveEFCL()

INCLUDE:    ioMNPar.h
            ioMNstr.h
PARAMETER:
RETURNS:    I32 4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            メモリFULL状態が解除された場合に入出力実行管理タスク、またはFaxJOB管理タスク
            より通知される。
            このメッセージを受けると、メモリFULL状態フラグを解除する。
NOTES:
HISTORY:
        1.1     Y.Matsuda   96/01/27
        D2.0.2  T.Fujii     00/07/04 DM45/35 NOP_DEL(ソースコード整理)
                                             if文,case文でのCOM_JB_EXPPC1を削除

        MCK1.1  M.Karasawa 02/06/11 McKinley    新メッセージ対応
*/
STATUS  iomnRecieveEFCL()
{
    STATUS  Status;     /*      */
    HI      hJobid;     /*  JOBID   */
    HI      hJobtype;   /*  JOB種別 */

    iomnDebugMessage(">>>Top of iomnRecieveEFCL()<<<\n",0);

    /*  Message格納 */
    hJobid = ioMNrmyparam.sEfclCtl.hJobid;
    iomnDebugMessage("###JOBID->%x\n",(I32)hJobid);

    /*  JOB種別取得 */
    if(ERROR == (Status = jobJobtype(hJobid, &hJobtype))){
        iomnDebugMessage(">>>Error:jobJobtype()\n",0);
        printErrno(errno);
    }
    iomnDebugMessage("###JOB TYPE -> %x\n",(I32)hJobtype);

    /*  拡張複写の場合  */
    if(hJobtype == COM_JB_EXPPC2){
        /*  メモリFULL解除メッセージ送信    */
#if 0   /* MCK1.1 新メッセージ対応  */
/**/        if(ERROR == (Status = iomnSendEFCL(ioMNCjst[hJobid].OldmsgQId, IOMSG_EFCL_UI, hJobid))){
#else
        if(ERROR == (Status = iomnSendEFCL(ioMNCjst[hJobid].OldmsgQId, SYS_MSG_EFCL, hJobid))){
#endif
            iomnDebugMessage(">>>Error:iomnSendEFCL()\n",0);
            return(ERROR);
        }
        iomnDebugMessage(">>>Send:EFCL Message\n",0);
    }
    /*  上記以外の場合  */
    else{
        /*  メモリFULLフラグ解除    */
        ioMNstatus.memfull_sts = IOMN_OFF;
        iomnDebugMessage(">>>OK:Clear Memory Full\n",0);
    }

    iomnDebugMessage(">>>Bottom of iomnRecieveEFCL()<<<\n",0);
    return(OK);
}


/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         7.26
TITLE:      ステイプル解除通知受信処理
MODULE：    STATUS  iomnRecieveEFST()
INCLUDE:    ioMNPar.h
            ioMNstr.h
PARAMETER:  N/A
RETURNS:    I32 4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            逐次型拡張複写時、フィニッシャスタックオーバ、またはメモリＦＵＬＬなどの
            要因によりステイプルが解除されたことをＵＩに通知するために使用する。
NOTES:
HISTORY:
        1.1     Y.Matsuda   96/01/27

        MCK1.1  M.Karasawa 02/06/11 McKinley    新メッセージ対応
*/
STATUS  iomnRecieveEFST()
{
    STATUS  Status;     /*      */
    HI      hJobid;     /*  JOBID   */
    HI      hType   ;   /*  解除タイプ */

    iomnDebugMessage(">>>Top of iomnRecieveEFST()<<<\n",0);

    /*  Message格納 */
    hJobid = ioMNrmyparam.sEfstCtl.hJobid;
    hType = ioMNrmyparam.sEfstCtl.hType;
    iomnDebugMessage("###JOBID->%x\n",(I32)hJobid);
    iomnDebugMessage("###hType->%x\n",(I32)hType);

    /*  ＪＯＢ開始通知元に対してステイプル解除通知を送信  */
#if 0   /* MCK1.1 新メッセージ対応  */
/**/    if(ERROR == (Status = iomnSendEFST(hJobid, ioMNCjst[hJobid].OldmsgQId, IOMSG_EFST_UI, hType))){
#else
    if(ERROR == (Status = iomnSendEFST(hJobid, ioMNCjst[hJobid].OldmsgQId, JOB_MSG_EMSC, hType))){
#endif
        iomnDebugMessage(">>>Error:iomnSendEFST()\n",0);
        return(ERROR);
    }
    iomnDebugMessage(">>>OK:iomnSendEFST()\n",0);

    iomnDebugMessage(">>>Bottom of iomnRecieveEFST()<<<\n",0);
    return(OK);
}


/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         7.27
TITLE:      データ転送キャンセル通知受信処理
MODULE：    STATUS  iomnRecieveEDCN()
INCLUDE:    ioMNPar.h
            ioMNstr.h
PARAMETER:  N/A
RETURNS:    I32 4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
        GDI、DSS印刷JOBで何らかの要因でデータ転送がキャンセルされたことを
        ioMNに通知する際に使用する。
        このメッセージを受信するとパワーOFF可能状態に移行する。
NOTES:
HISTORY:
        1.1     Y.Matsuda   97/10/15
        M1.1(TJ)H.Takahashi 98/09/28
                DSS逐次対応
        D1.2    T.Fujii     99/12/24    DM45/35 (後追い)逐次印刷対応

        MCK1.1  M.Karasawa 02/06/11 McKinley    新メッセージ対応
*/
STATUS  iomnRecieveEDCN()
{
#if 0   /* MCK1.1 新メッセージ対応  */
/**/    HI      hJobid;     /*  JOBID   */
/**/    HI      hJobtype;   /*  JOB種別 */
/**/
/**/    iomnDebugMessage(">>>>Top of iomnRecieveEDCN()<<<<<\n",0);
/**/    /*  パラメータ取得  */
/**/    hJobid = ioMNrmyparam.sEdcnCtl.hJobid;
/**/    hJobtype = ioMNrmyparam.sEdcnCtl.hJobtype;
/**/    iomnDebugMessage("###JOBID->%x\n",(I32)hJobid);
/**/    iomnDebugMessage("###JOB TYPE->%x\n",(I32)hJobtype);
/**/
/**/    /*  ＧＤＩの場合    */
/**/    if(hJobtype == COM_JB_PRT_GDI1 || hJobtype == COM_JB_PRT_GDI2){
/**/        /*  データ転送フラグ解除    */
/**/        ioMNstatus.hGdiDataTrans = IOMN_OFF;
/**/        iomnDebugMessage(">>>Clear GDI Data Flag\n",0);
/**/    }
/**/
/**/    /*  ＤＳＳの場合    */
/**/    if( hJobtype == COM_JB_PRIVATE_DSI ||
/**/        hJobtype == COM_JB_RECV_DSI ||
/**/        hJobtype == COM_JB_PRIVATE_DSI_SYNC ||
/**/        hJobtype == COM_JB_RECV_DSI_SYNC ){
/**/
/**/        /* 入出力ジョブの中止処理 */
/**/        if( ioMNstatus.hDssDataTrans != IOMN_OFF ) {
/**/            /* データ転送開始後(ioExMNへCJST送信後)の場合 */
/**/            /* 入出力ョブの中止(サブジョブに対し中止メッセージを送信する) */
/**/            if( iomnSubJobCancel(hJobid, hJobtype, IOMSG_CAN_ETC) == ERROR ) {
/**/                iomnDebugMessage(">> ioMN >> ERROR:iomnSubJobCancel(%d) in iomnRecieveEDCN()\n",(I32)hJobid);
/**/                return( ERROR );
/**/            }
/**/        }
/**/        else {
/**/            /* 上記以外の場合、フラグを解除して終了 */
/**/            /* データ転送フラグ解除 */
/**/            ioMNstatus.hDssDataTrans = IOMN_OFF;
/**/            iomnDebugMessage(">>>Clear DSI Data Flag\n",0);
/**/        }
/**/    }
/**/
/**/    iomnDebugMessage(">>>>Bottom of iomnRecieveEDCN()<<<<<\n",0);
#endif
    return(OK);
}


/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         7.28
TITLE:      DSSデータ転送開始要求受信処理
MODULE：    STATUS  iomnRecieveRDDS()
INCLUDE:    ioMNPar.h
            ioMNstr.h
PARAMETER:  N/A
RETURNS:    I32 4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            DSSよりデータ転送が発生した場合に受信し、これにより
            OFF状態から解除するとともに、解除されたことをＤＳＳに
            通知する。
NOTES:
HISTORY:
        1.1     Y.Matsuda   97/10/15
        M1.1(TJ)H.Takahashi 98/09/28
                DSS逐次対応
        D1.2    T.Fujii     99/12/24    DM45/35 (後追い)逐次印刷対応

        MCK1.1  M.Karasawa 02/06/11 McKinley    新メッセージ対応
*/
STATUS  iomnRecieveRDDS()
{
    STATUS  Status;     /*      */
    HI      hJobid;     /*  JOBID   */
    HI      hJobtype;   /*  JOB種別 */

    /* MCK1.1 新メッセージ メンバー対応 START   */
    B       *pUiWorkPointer;
    HI      hJcbID;
    HI      hTNo;
    HI      hIndiType;
    HI      hTrigger;
    HI      hContinue;
    HI      hIpTblID;
    HI      hProcessNo;
    /* MCK1.1 END   */

    iomnDebugMessage(">>>>Top of iomnRecieveRDDS()<<<<<\n",0);
    /*  パラメータ取得  */
#if 0   /* MCK1.1 CJSTメンバ追加対応    */
    hJobid = ioMNrmyparam.sEdcnCtl.hJobid;
#else
    pUiWorkPointer  = ioMNrmyparam.sCjstCtl.pUiWorkPointer; /* MCK1.1   */
    hJobid          = ioMNrmyparam.sCjstCtl.hJobid;         /* MCK1.1   */
    hTrigger        = ioMNrmyparam.sCjstCtl.hTrigger;
    hContinue       = ioMNrmyparam.sCjstCtl.hContinue;
    /* hPrtTypeがここに入る */
    hIpTblID        = ioMNrmyparam.sCjstCtl.hIpTblId;
    hJcbID          = ioMNrmyparam.sCjstCtl.hJcbID;
    hTNo            = ioMNrmyparam.sCjstCtl.hTNo;
    hIndiType       = ioMNrmyparam.sCjstCtl.hIndiType;
    hProcessNo      = ioMNrmyparam.sCjstCtl.hProcessType;
    /* hfaxSndQueIdがここに入る */
#endif

    iomnDebugMessage("###JOBID->%x\n",(I32)hJobid);
    /* MCK1.1 CJSTメンバ追加対応    */
    iomnDebugMessage("### Trigger   -->%x\n",(I32)hTrigger);
    iomnDebugMessage("### Continue  -->%x\n",(I32)hContinue);
    iomnDebugMessage("### IpTblID   -->%x\n",(I32)hIpTblID);
    iomnDebugMessage("### JCB ID    -->%x\n",(I32)hJcbID);
    iomnDebugMessage("### TNo       -->%x\n",(I32)hTNo);
    iomnDebugMessage("### IndiType  -->%x\n",(I32)hIndiType);
    iomnDebugMessage("### ProcessNo -->%x\n",(I32)hProcessNo);
    /* MCK1.1 END   */

    /*  JOB種別取得 */
    if(ERROR == (Status = jobJobtype(hJobid, &hJobtype))){
        cmnPrintf_ioMN(2,">>>Error:jobJobtype()\n");
        printErrno(errno);
        return(ERROR);
    }
    iomnDebugMessage("###JOB TYPE->%x\n",(I32)hJobtype);

    /*  Message格納 */
    iomnStoreCmdSender( hJobid );   /* コマンド送信元の保存 */
#if 0   /* MCK1.1 RDDS -> CJST+プロセス に変更したので保存はCJSTを使用  */
/**/    iomnStoreRddsParam( hJobid );   /* RDDSパラメータの保存 */
#else
    iomnStoreCjstParam( hJobid );   /* CJSTパラメータの保存 */
#endif

    /*  ＤＳＳの場合    */
    if( hJobtype == COM_JB_PRIVATE_DSI ||
        hJobtype == COM_JB_RECV_DSI ||
        hJobtype == COM_JB_PRIVATE_DSI_SYNC ||
        hJobtype == COM_JB_RECV_DSI_SYNC ){
        /*  データ転送フラグ設定    */
        ioMNstatus.hDssDataTrans = IOMN_READY;
        iomnDebugMessage(">>>Set DSI Data Transfer Flag\n",0);
    }

    /* D3.9.4 Start J.Ootani LAN-P/DSS Sleep/Save解除タイミング修正 */
    #if 1 /* NEW */
    /* ON/OFF状態に関係なく */
    if( iomnDataReceiveStart(hJobid) == ERROR ) {
        cmnPrintf_ioMN(2,">> ioMN >> Error:iomnDataReceiveStart()\n");
        return( ERROR );
    }
    #else /* OLD */
/**/    /*  ON状態の場合    */
/**/    if(ioMNstatus.hWeeklysts == IOMN_ON){
/**/        iomnDebugMessage(">>>ON_STATUS\n",0);
/**/
/**/        if( iomnDataReceiveStart(hJobid) == ERROR ) {
/**/            cmnPrintf_ioMN(2,">> ioMN >> Error:iomnDataReceiveStart()\n");
/**/            return( ERROR );
/**/        }
/**/    }
/**/
/**/    /*  ON状態であるがOFF処理中の場合   */
/**/    else if(ioMNstatus.hTimerProccess == IOMN_OFF){
/**/        /*  ＯＮ処理予約フラグ設定  */
/**/        ioMNstatus.hReserveTimerON = IOMN_ON;
/**/        ioMNstatus.hReserveTimerONtype = IOMN_PWON_BY_GDI_DSS;
/**/        iomnDebugMessage(">>>OK:Reserve Timer ON\n",0);
/**/
/**/        /*  電源オン通知予約    */
/**/        ioMNstatus.hReserveEPWN = IOMN_ON;
/**/    }
/**/    /*  OFF状態であるがON処理中の場合   */
/**/    else if(ioMNstatus.hTimerProccess == IOMN_ON){
/**/        iomnDebugMessage(">>>ON Proccess\n",0);
/**/
/**/        /*  電源オン通知予約    */
/**/        ioMNstatus.hReserveEPWN = IOMN_ON;
/**/    }
/**/    else{
/**/        /*  OFFモードでOFF状態にある場合、マシン管理に対しタイマ解除を指示する  */
/**/        if(ERROR == (Status = iomnSendTimer(TRUE, IOMN_PWON_BY_PRINT))){
/**/            iomnDebugMessage(">>>Error:iomnSendTimer()\n",0);
/**/            return(ERROR);
/**/        }
/**/
/**/        /*  電源オン通知予約    */
/**/        ioMNstatus.hReserveEPWN = IOMN_ON;
/**/        iomnDebugMessage(">>>OK:iomnSendTimer()\n",0);
/**/    }
    #endif
    /* D3.9.4 End */

    /*  電源オン通知パラメータ格納  */
    ioMNEpwn.hJobid = hJobid;
    ioMNEpwn.iSts = TRUE;
    iomnDebugMessage(">>>Set EPWN Parameter\n",0);

    iomnDebugMessage(">>>>Bottom of iomnRecieveRDDS()<<<<<\n",0);
    return(OK);
}


/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         12.1
TITLE:      JOB管理関連メッセージの保存
            Storing the Message about Job Management
        
MODULE：    void    iomnSetJobManagementMsg(IOMNMSG_STORE *)

INCLUDE:    ioMNPar.h
PARAMETER:
            HI  hJobid;
RETURNS:    I32 4B
            IOMN_ON(1)  :   中止可能
            IOMN_OFF(0) :   中止不可能
            ERROR(-1)   :   異常終了
OUTLINE:
            中止処理を実行してよいか否かの判断を行う。
NOTES:
HISTORY:
        1.1     Y.Matsuda   96/9/18
*/
void    iomnSetJobManagementMsg(IOMNMSG_STORE *pMsgparam)
{
    pMsgparam->iOldTaskId = iioMNTaskId;
    pMsgparam->iOldMsgId = iioMNMsgId;
    pMsgparam->OldmsgQId = ioMNMsgQId;
    memcpy(&(pMsgparam->msgparam), &ioMNrmyparam, sizeof(union ioMNparam));
}


/*
(C) Copyright TOSHIBA Corporation 1999,2000. ALL Right Reserved
NO.         100.2
TITLE:  JOB管理情報の保存

MODULE：    void iomnStoreCmdSender(HI hJobID)

INCLUDE:    
PARAMETER:  HI  hJobid;
RETURNS:    (void)
OUTLINE:    ジョブ管理用情報の保存を行う。
NOTES:
HISTORY:
    D1.1    T.Fujii 99/11/15
*/
void iomnStoreCmdSender( HI hJobID )
{
    /* CJSTのパラメータとしてコマンド送信元を保存 */
    ioMNCjst[hJobID].iOldTaskId = iioMNTaskId;  /* 送信元タスクID */
    ioMNCjst[hJobID].iOldMsgId  = iioMNMsgId;   /* 送信されたメッセージ */
    ioMNCjst[hJobID].OldmsgQId  = ioMNMsgQId;   /* 送信元メッセージキュー */
}


/*
(C) Copyright TOSHIBA Corporation 1999,2000. ALL Right Reserved
NO.         100.2
TITLE:  CJSTパラメータの保存

MODULE：    void iomnStoreCjstParam(HI hJobID)

INCLUDE:    
PARAMETER:  HI  hJobid;
RETURNS:    (void)
OUTLINE:    CJSTメッセージパラメータの保存を行う。
NOTES:
HISTORY:
    D1.1    T.Fujii 99/11/15

    MCK1.1      M.Karasawa  02/06/11    McKinley    新メッセージ対応
    V018.00_01  M.Karasawa  03/04/09    McKinley    メッセージ変更対応
    V018.00_04  M.Karasawa  00/00/00    McKinley    トナーエンプティ状態からの再開処理対応
    V021.00_01  M.Karasawa  03/06/16    McKinley    V018.00_04の修正（トナーエンプティ状態からの再開処理対応）を削除
*/
void iomnStoreCjstParam( HI hJobID )
{
    /* CJSTパラメータの保存 */
    ioMNCjst[hJobID].msgparam.sCjstCtl.pUiWorkPointer   = ioMNrmyparam.sCjstCtl.pUiWorkPointer; /* MCK1.1   */
    ioMNCjst[hJobID].msgparam.sCjstCtl.hJobid           = ioMNrmyparam.sCjstCtl.hJobid;
    ioMNCjst[hJobID].msgparam.sCjstCtl.hTrigger         = ioMNrmyparam.sCjstCtl.hTrigger;
    ioMNCjst[hJobID].msgparam.sCjstCtl.hContinue        = ioMNrmyparam.sCjstCtl.hContinue;
    ioMNCjst[hJobID].msgparam.sCjstCtl.hPrtType         = ioMNrmyparam.sCjstCtl.hPrtType;
    /* MCK1.1 CJSTメンバー追加対応  STSRT   */
    ioMNCjst[hJobID].msgparam.sCjstCtl.hIpTblId         = ioMNrmyparam.sCjstCtl.hIpTblId;
    ioMNCjst[hJobID].msgparam.sCjstCtl.hJcbID           = ioMNrmyparam.sCjstCtl.hJcbID;
    ioMNCjst[hJobID].msgparam.sCjstCtl.hTNo             = ioMNrmyparam.sCjstCtl.hTNo;
    ioMNCjst[hJobID].msgparam.sCjstCtl.hIndiType        = ioMNrmyparam.sCjstCtl.hIndiType;
    ioMNCjst[hJobID].msgparam.sCjstCtl.hProcessType     = ioMNrmyparam.sCjstCtl.hProcessType;
    ioMNCjst[hJobID].msgparam.sCjstCtl.hfaxSndQueId     = ioMNrmyparam.sCjstCtl.hfaxSndQueId;
    /* MCK1.1 END   */
    /* V018.00_01 START */
    ioMNCjst[hJobID].msgparam.sCjstCtl.hAppliType       = ioMNrmyparam.sCjstCtl.hAppliType;
    ioMNCjst[hJobID].msgparam.sCjstCtl.hCassette        = ioMNrmyparam.sCjstCtl.hCassette;
    /* V018.00_01 END   */
#if 0  /* V021.00_01 START */
/**/    /* V018.00_04 START */
/**/    ioMNCjst[hJobID].msgparam.sCjstCtl.hRestartType     = ioMNrmyparam.sCjstCtl.hRestartType;
/**/    /* V018.00_04 END   */
#endif /* V021.00_01 END   */
}


/*
(C) Copyright TOSHIBA Corporation 1999,2000. ALL Right Reserved
NO.         100.x
TITLE:  RDDSパラメータの保存

MODULE：    void iomnStoreRddsParam(HI hJobID)

INCLUDE:    
PARAMETER:  HI  hJobid;
RETURNS:    (void)
OUTLINE:    RDDSメッセージパラメータの保存を行う。
NOTES:
HISTORY:
    D1.2    T.Fujii 99/12/24

    MCK1.1  M.Karasawa 02/06/11 McKinley    新メッセージ対応
*/
void iomnStoreRddsParam( HI hJobID )
{
    /* RDDSパラメータの保存 */
    /* ジョブIDか必ずCJSTと異なるのでCJSTエリアを使用する */
    /* RDDS専用メモリは確保しない(削除する) */
#if 0   /* MCK1.1 新メッセージ対応  *//* パラメータ保存はどうするか？？ */
    ioMNCjst[hJobID].msgparam.sCjstCtl.hJobid    = ioMNrmyparam.sRddsCtl.hJobid;
#else
    ioMNCjst[hJobID].msgparam.sCjstCtl.hJobid    = ioMNrmyparam.sCjstCtl.hJobid;
#endif
}


/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         12.1
TITLE:      JOB管理関連メッセージ保存用テーブルの初期化
            Initialization for the Message table about Job Management
MODULE：    void    iomnCleanJobManagementMsg(HI hjobid)
INCLUDE:    ioMNPar.h
            ioMNstr.h
PARAMETER:
            HI  hJobid;
RETURNS:    
            N/A
OUTLINE:    
            メッセージ格納用テーブルをＮＵＬＬクリアする。
NOTES:
HISTORY:
        1.1     Y.Matsuda   96/9/18
*/
void    iomnCleanJobManagementMsg(HI hjobid)
{
    memset(&ioMNCjst[hjobid], 0x00, sizeof(struct msgstore));
    memset(&ioMNAjst[hjobid], 0x00, sizeof(struct msgstore));
    memset(&ioMNTjfn[hjobid], 0x00, sizeof(struct msgstore));
    memset(&ioMNTjed[hjobid], 0x00, sizeof(struct msgstore));
    memset(&ioMNCjsu[hjobid], 0x00, sizeof(struct msgstore));
    memset(&ioMNAjsu[hjobid], 0x00, sizeof(struct msgstore));
    memset(&ioMNTjsu[hjobid], 0x00, sizeof(struct msgstore));
    memset(&ioMNCjcn[hjobid], 0x00, sizeof(struct msgstore));
    memset(&ioMNCjfn[hjobid], 0x00, sizeof(struct msgstore));
/* 10911 fix*/
	memset(&ioMNRjre[hjobid], 0x00, sizeof(struct msgstore));
	memset(&ioMNRjcn[hjobid], 0x00, sizeof(struct msgstore));
}

/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         
TITLE:      TJED受信時JOB状態算出処理
MODULE：    STATUS  iomnCalcJobStatus();
INCLUDE:    ioMNPar.h
PARAMETER:
        HI   hJobID
        HI   hSts
        I32  iErrSys
        I32  iErrEng
        HI   *hoJobStat;
RETURNS:
        STATUS
        IOMN_ON :JOB状態変更の必要あり  
        IOMN_OFF:JOB状態変更の必要なし
        ERROR   :エラー

OUTLINE:
            フロントのJOB状態及びTJEDの要因から，設定すべきJOB状態
        を算出する。手置き逐次型複写対応のため。
NOTES:

HISTORY:
        M1.1     (TJ)H.Takahashi   98/7/6
*/

static STATUS  iomnCalcJobStatus( HI hJobID, HI hSts, I32 iErrSys, I32 iErrEng, HI *hoJobStat)
{
    HI  hJobStat;
    I32 iCause;
    STATUS  Status, sRet;

    /* パラメータチェック */
    if ( hoJobStat == 0 ){
        return (ERROR);
    }

    /* JOB状態取得 */
    if(ERROR == (Status=jobStatus(hJobID, &hJobStat, &iCause)) ){
        iomnDebugMessage(">>>Error:jobStatus() in iomnCalcJobStatus()\n",0);
        return(ERROR);
    }
    iomnDebugMessage(">>>iomnCalcJobStatus() JOB STATUS[%x]\n",
            hJobStat);

    /* JOB状態による分岐 */
    switch( hJobStat ){
        case COM_JS_RUNNING :
            /* JOB動作中 */
            if ( hSts == IOMSG_SUS_ERROR &&
                 iErrSys == S_scnEX_NO_NEXT_ORG_SCS &&
                 iErrEng == IOMSG_NOT_USED ){
                /* 逐次手置き複写スキャン終了 */
                *hoJobStat = COM_JS_SUSPEND_SCN_END;
                sRet = IOMN_ON;
            }else if ( hSts == IOMSG_SUS_MNL &&
                   iErrSys == IOMSG_NOT_USED &&
                   iErrEng == IOMSG_NOT_USED ){
                /* 逐次手置き複写印刷終了 */
                *hoJobStat = COM_JS_RUNNING;
                sRet = IOMN_OFF;
            }else{
                /* 何らかのエラーによる終了 */
                *hoJobStat = COM_JS_SUSPEND;
                sRet = IOMN_ON;
            }
            break;
        case COM_JS_SUSPEND :
            /* JOB中断中　*/
            *hoJobStat = COM_JS_SUSPEND;
            sRet = IOMN_OFF;
            break;
        case COM_JS_SUSPEND_SCN_END:
            /* 逐次手置きスキャン終了かつ印刷中 */
            if ( hSts == IOMSG_SUS_ERROR &&
                 iErrSys == S_scnEX_NO_NEXT_ORG_SCS &&
                 iErrEng == IOMSG_NOT_USED ){
                /* 逐次手置き複写スキャン終了 */
                *hoJobStat = COM_JS_SUSPEND_SCN_END;
                sRet = IOMN_OFF;
            }else if ( hSts == IOMSG_SUS_MNL &&
                   iErrSys == IOMSG_NOT_USED &&
                   iErrEng == IOMSG_NOT_USED ){
                /* 逐次手置き複写印刷終了 */
                *hoJobStat = COM_JS_SUSPEND;
                sRet = IOMN_ON;
            }else{
                /* 何らかのエラーによる終了 */
                *hoJobStat = COM_JS_SUSPEND;
                sRet = IOMN_ON;
            }
            break;
        default         :
            iomnDebugMessage(">>>ILLEGAL:jobStatus() in iomnCalcJobStatus():hJobStat[%x]\n", hJobStat);
            sRet = ERROR;
    }

    iomnDebugMessage(">>>iomnCalcJobStatus()NEXT JOB STATUS[%x]\n",
            *hoJobStat);

    return(sRet);
}


/* 関数本体 */
/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         
TITLE:      複写JOB種類Check処理
MODULE：    STATUS  iomnCalcJobStatus();
INCLUDE:    ioMNPar.h
PARAMETER:
        HI   hJobID
RETURNS:
        STATUS
        IOMN_ON :手置き逐次型複写
        IOMN_OFF:その他の複写
        ERROR   :エラー

OUTLINE:
            parMNの情報から手置き逐次型複写かどうかを判断する。
NOTES:

HISTORY:
        M1.1     (TJ)H.Takahashi   98/8/28
*/

STATUS iomnPPCCheck( HI hJobId )
{
    STATUS  Status;
    HI      hScanMethod, hIoMode;

    /* ADF情報を取得 */
    Status = parMNScanMethod( hJobId, &hScanMethod );
    if ( Status == ERROR ){
        cmnPrintf_ioMN(2,"ERROR parMNScanMethod()-iomnPPCCheck()[%d]\n", errno );
        return ERROR;
    }

    /* 逐次/完了情報を取得 */
    Status = parMNIOMode( hJobId, &hIoMode );
    if ( Status == ERROR ){
        cmnPrintf_ioMN(2,"ERROR parMNIOMode()-iomnPPCCheck()[%d]\n", errno );
        return ERROR;
    }

    /* 手置き逐次か否かを判断 */
    if ( hScanMethod == COM_SM_MNL && hIoMode == COM_OT_SCS ){
        /* 手置き逐次 */
        return IOMN_ON;
    }
    else{
        /* 手置き逐次ではない */
        return IOMN_OFF;
    }
}

/* */
/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         
TITLE:      TJED受信時JOB状態算出処理
MODULE：    STATUS  iomnRegularTJEDMcnRecover();
INCLUDE:    ioMNPar.h
PARAMETER:
        HI   hJobID
RETURNS:
        STATUS
        OK      :正常終了
        ERROR   :エラー

OUTLINE:
            無条件にスキャナおよびプリンタのリカバリ要求を送信する
NOTES:

HISTORY:
        M1.1     (TJ)H.Takahashi   98/8/28
        D1.1    T.Fujii     99/11/15    DM45/35 後追い逐次・並行動作対応
*/
STATUS iomnRegularTJEDMcnRecover( HI hJobId )
{
    STATUS  Status;

    /* スキャナリカバリ */
    Status = iomnMcnRecover(hJobId, MCN_RCV_MODE_PPC,MCN_RCV_RSC_SCN, 
                                    IOMN_MCN_REC_TJED, 0);
    if ( Status == ERROR ){
        iomnDebugMessage(">>>Error:iomnMcnRecover() in iomnPPCTJED()\n",0);
        return(ERROR);
    }
    iomnDebugMessage(">>>Mcn Recover for TJED Message on Resource Scanner\n",0);

    return OK;
}

/* 関数本体 */
/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         
TITLE:      手置き逐次型複写におけるTJED受信時マシンリカバリ処理
MODULE：    STATUS  iomnSCSTJEDMcnRecover();
INCLUDE:    ioMNPar.h
PARAMETER:
        HI   hJobId
        HI   hJobStat
        HI   hSts
        I32  iErrSys
        I32  iErrEng
RETURNS:
        STATUS
        OK      :正常終了
        ERROR   :エラー

OUTLINE:
            フロントのJOB状態及びTJEDの要因から，設定すべきJOB状態
        を算出する。手置き逐次型複写対応のため。
NOTES:

HISTORY:
        M1.1     (TJ)H.Takahashi   98/8/28
*/
STATUS iomnSCSTJEDMcnRecover( HI hJobId, HI hJobStat, HI hSts, I32 iErrSys, I32 iErrEng )
{
    STATUS  Status;

    if ( hJobStat == COM_JS_RUNNING &&
         hSts == IOMSG_SUS_MNL &&
         iErrSys == IOMSG_NOT_USED &&
         iErrEng == IOMSG_NOT_USED ){
        /* 手置き逐次スキャン中かつ印刷正常終了時 */
        iomnDebugMessage("Recieve TJED But No Mcn Recover\n",0);
    }
    else if ( hSts == IOMSG_SUS_ERROR &&
              iErrSys == S_scnEX_NO_NEXT_ORG_SCS &&
              iErrEng == IOMSG_NOT_USED ){
        /* 手置き逐次スキャン終了 */
        /* スキャナリカバリ */
        Status = iomnMcnRecover(hJobId, MCN_RCV_MODE_PPC,MCN_RCV_RSC_SCN, 
                                    IOMN_MCN_REC_TJED, 0);
        if ( Status == ERROR ){
            iomnDebugMessage(">>>Error:iomnMcnRecover() in iomnPPCTJED()\n",0);
            return(ERROR);
        }

        iomnDebugMessage(">>>Mcn Recover for TJED Message on Resource Scanner\n",0);
    }
    else{
        /* JOB状態がRUNNING以外の印刷正常終了，もしくは紙無し時 */
        /* プリンタリカバリ */
        Status = iomnMcnRecover(hJobId,MCN_RCV_MODE_PPC,MCN_RCV_RSC_PRT,IOMN_MCN_REC_TJED,0);
        if ( Status == ERROR ){
            iomnDebugMessage(">>>Error:iomnMcnRecover() in iomnPPCTJED()\n",0);
            return(ERROR);
        }
        iomnDebugMessage(">>>Mcn Recover for TJED Message on Resource Printer\n",0);
    }

    return OK;
}

/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         X.XX
TITLE:      ADUフル発生通知受信処理
MODULE：    STATUS  iomnRecieveEDFL()

INCLUDE:    
PARAMETER:
            N/A
            RETURNS:    STATUS
                        OK:     正常
                        ERROR:  異常終了
OUTLINE:
            ADUフルイベント発生通知受信時の処理。cpyUIまたはdssPRNにイベントを
            通知する。
NOTES:
HISTORY:
        M1.1(TJ)H.Takahashi 98/11/30
        D1.6    T.Fujii     00/02/09 DM45/35 prnUI表示対応
        D2.8    T.Fujii     00/07/01 DM45/35 印刷系イベント送信処理修正
        D4.8    T.Fujii     00/11/21 DM45/35 05テストコピーでcpyUIにメッセージ送信する不具合修正

        MCK1.1  M.Karasawa  02/06/11    McKinley    新メッセージ対応
    V023.00_02  M.Karasawa  03/07/16    McKinley    ソース整理（不要処理削除）
*/
STATUS  iomnRecieveEDFL()
{
    STATUS      Status;
    HI          hJobID;
/*    MSG_Q_ID    msgqid;   *//* V028.00_00 */
    HI          hJobType;
/*    I32         imsgid;   *//* V028.00_00 */
    HI          hPrnID = -1;

    iomnDebugMessage("Top of iomnRecieveEDFL()\n",0);

    /*  受信パラメータ取得  */
    hJobID = ioMNrmyparam.sEdflCtl.hJobid;
    iomnDebugMessage("###JOBID->%x\n",(I32)hJobID);

    /*  JOBIDよりJOB種別取得    */
    if(ERROR == (Status = jobJobtype(hJobID, &hJobType))){
        iomnDebugMessage(">>>error:jobJobtype() iomnRecieveEDFL()\n",0);
        return(ERROR);
    }

    /* 印刷ジョブの場合、印刷種別を取得してメッセージを送信 */
    if( (hPrnID = iomnPrnQueCheckPrnType(hJobID)) >= 0 ) {
        /* prnUI or 管理タスクにEDFLを送信 */
#if 0   /* MCK1.1 新メッセージ対応  */
/**/        return( iomnSendPrintEventMsg(hJobID,hPrnID,IOMSG_EDFL_UI,IOMSG_EDFL_UI,&ioMNrmyparam) );
#else
        return( iomnSendPrintEventMsg(hJobID,hPrnID,SYS_MSG_EDFL,SYS_MSG_EDFL,&ioMNrmyparam) );
#endif
    }
#if 0  /* V023.00_02 START */
/**/    /*  JOB種別毎メッセージQue取得  */
/**/    switch( hJobType ){
/**/        case    COM_JB_IPPC     :
/**/        case    COM_JB_EXPPC2   :
/**/            iomnDebugMessage(">>>複写系\n",0);
/**/            /* D4.8 START T.Fujii メッセージ送信先修正 */
/**/            if( msgQIdioMNforCpyJob == msgQIdDigUI ) {
/**/                return(OK);
/**/            }
/**/            /* D4.8 END */
/**/            msgqid = msgQIdCpyUI;
/**/            break;
/**/        case    COM_JB_RECV_DSI :
/**/        case    COM_JB_RECV_DSI_SYNC:
/**/            iomnDebugMessage(">>>DSS印刷系\n",0);
/**/            msgqid = msgQIdDssPRN;
/**/            break;
/**/        default                 :
/**/            cmnPrintf_ioMN(2,"Undefined jobType!!! iomnRecieveEDFL\n");
/**/            return ERROR;
/**/    }
/**/#if 0   /* MCK1.1 新メッセージ対応  */
/**//**/    imsgid = IOMSG_EDFL_UI;
/**/#else
/**/    imsgid = SYS_MSG_EDFL;
/**/#endif
/**/
/**/    /*  メッセージ送信  */
/**/    if(ERROR == (Status = iomnSendEDFL(msgqid, imsgid))){
/**/        iomnDebugMessage(">>>error:iomnSendEDFL()-iomnRecieveEDFL()\n",0);
/**/        return(ERROR);
/**/    }
#endif /* V023.00_02 END   */

    iomnDebugMessage("Bottom of iomnRecieveEDFL()\n",0);

    return(OK);
}


/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         X.XX
TITLE:      最終原稿通知受信処理
MODULE：    STATUS  iomnRecieveELST()

INCLUDE:    
PARAMETER:
            N/A
            RETURNS:    STATUS
                        OK:     正常
                        ERROR:  異常終了
OUTLINE:
            最終原稿通知受信時の処理。cpyUIにイベントを
            通知する。
NOTES:
HISTORY:
        M1.1(TJ)H.Takahashi 99/04/27
        D4.8    T.Fujii     00/11/21 DM45/35 05テストコピーでcpyUIにメッセージ送信する不具合修正

        MCK1.1  M.Karasawa  02/06/11    McKinley    新メッセージ対応
        MCK1.3  M.Karasawa  02/07/03    McKinley    確認処理をスイッチ（DEBUG）化
        MCK1.3  M.Karasawa  02/07/03    McKinley    送信先(msgQIdDigUI)確認削除
        MCK1.4  M.Karasawa  02/07/18    McKinley    送信先をCJST送信元に変更（cpyJC固定でもいい）
*/
STATUS  iomnRecieveELST()
{
    STATUS  Status;
    HI          hJobId;
    MSG_Q_ID    msgqid;
    I32         imsgid;

    iomnDebugMessage("Top of iomnRecieveELST()\n",0);

#if 0   /* MCK1.3 送信先なし    */
/**/    /* D4.8 START T.Fujii メッセージ送信先修正 */
/**/    if( msgQIdioMNforCpyJob == msgQIdDigUI ) {
/**/        return(OK);
/**/    }
/**/    /* D4.8 END */
#endif

    /*  受信パラメータ取得  */
    hJobId = ioMNrmyparam.sElstCtl.hJobid;
    iomnDebugMessage("###JOBID->%x\n",(I32)hJobId);

#if 0   /* MCK1.4   */
/**/    msgqid = msgQIdCpyUI;
#else
    msgqid = ioMNCjst[hJobId].OldmsgQId;
#endif

#if 0   /* MCK1.1 新メッセージ対応  *//* 要検討 */
/**/    imsgid = IOMSG_ELST_UI;
#else
    imsgid = JOB_MSG_ESLP;
#endif

    /*  メッセージ送信  */
    if(ERROR == (Status = iomnSendELST(msgqid, imsgid))){
        iomnDebugMessage(">>>error:iomnSendELST()-iomnRecieveELST()\n",0);
        return(ERROR);
    }

    iomnDebugMessage("Bottom of iomnRecieveELST()\n",0);

    return(OK);
}

/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         X.XX
TITLE:      ファイル化完了通知受信処理
MODULE：    STATUS  iomnRecieveEFED()

INCLUDE:    
PARAMETER:
            N/A
            RETURNS:    STATUS
                        OK:     正常
                        ERROR:  異常終了
OUTLINE:
            ファイル化完了通知受信時の処理。dssSCNにイベントを
            通知する。
            タンデム入力JOB及びDSS入力(半逐次)のみ適用
NOTES:
HISTORY:
        M1.1(TJ)H.Takahashi 99/09/02

        MCK1.1  M.Karasawa 02/06/11 McKinley    新メッセージ対応
*/
STATUS  iomnRecieveEFED()
{
    STATUS  Status;
    HI          hJobId, hOrgNum;
    MSG_Q_ID    msgqid;
    I32         imsgid;

    iomnDebugMessage("Top of iomnRecieveEFED()\n",0);

    /*  受信パラメータ取得  */
    hJobId = ioMNrmyparam.sEfedCtl.hJobid;
    hOrgNum= ioMNrmyparam.sEfedCtl.hOrgNum;
    iomnDebugMessage("###JOBID->%x\n",(I32)hJobId);
    iomnDebugMessage("###OrgNum->%x\n",(I32)hOrgNum);

#if 0   /* V007.00  */
/**/    msgqid = msgQIdDssSCN;
#else
    msgqid = ioMNCjst[hJobId].OldmsgQId;
#endif

#if 0   /* MCK1.1 新メッセージ対応  */
/**/    imsgid = IOMSG_EFED_JOB;
#else
    imsgid = JOB_MSG_ESNC;
#endif

    /*  メッセージ送信  */
    if(ERROR == (Status = iomnSendEFED(msgqid, imsgid))){
        iomnDebugMessage(">>>error:iomnSendEFED()-iomnRecieveEFED()\n",0);
        return(ERROR);
    }

    iomnDebugMessage("Bottom of iomnRecieveEFED()\n",0);

    return(OK);
}

/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         
TITLE:      FSMS開始指示受信処理
MODULE：    STATUS  iomnReccieveCFST()
INCLUDE:    
PARAMETER:
RETURNS:    STATUS
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            FSMS開始指示メッセージ受信時の処理。
NOTES:
HISTORY:
        M4.11   (TJ)H.Takahashi 99/01/26
        M5.00   (TJ)H.Takahashi 99/02/16
        M5.20   (TJ)H.Takahashi 99/03/18
                FSMSΦ4対応
        D3.24   Y.Shimada       00/09/11  DM45/35 ＲＤＣ(FAX)対応
                                          ※CFSTとCRSTの共通関数

        MCK1.1  M.Karasawa 02/06/11 McKinley    新メッセージ対応
*/
STATUS  iomnRecieveCFST()
{
    STATUS  sSndSts, Status;
    HI      hRdWr, hChkSts;
    HI      hCount;
    
    /* メッセージパラメータ取り出し */
    /* Read/Write種別 */
    hRdWr = ioMNrmyparam.sCfstCtl.hRdWr;

    iomnSetJobManagementMsg(&ioMNCfst);

    /* D3.24 START Y.Shimada CHG OLD */
    #if 0
    /* FSMS開始条件チェック */
    hChkSts = iomnFsmsStartConfirm( hRdWr, IOMN_FSMS_NVRAM_MS );
    #else /* D3.24 MID NEW */
    hChkSts = IOMN_NG;
    /* 既にRDC/FSMSを実行中はエラー -> TFST/TRSTを返送 */
    if( ioMNFsmsStatus.hSts == IOMN_ON ){
        iomnDebugMessage("Error(Exec):iomnRecieveCFST()\n",0);
        /* FSMSへTFST(NG)を転送 */
        sSndSts = iomnSendTFST( hRdWr, hChkSts );
        if ( sSndSts == ERROR ){
            cmnPrintf_ioMN(2,"Error:iomnSendTFST()-iomnRecieveCFST()[%x]\n",errno);
            return ERROR;
        }
        return OK;
    }

    /* FSMS開始条件チェック */
#if 0   /* MCK1.1 新メッセージ対応  */
/**/    if(( ioMNCfst.iOldMsgId == IOMSG_CFST_UI )||
/**/       ( ioMNCfst.iOldMsgId == IOMSG_CFST_JOB)){
#else
    if( ioMNCfst.iOldMsgId == SYS_MSG_CFST ){
#endif
        hChkSts = iomnFsmsStartConfirm( hRdWr, IOMN_FSMS_NVRAM_MS );
    }else{
        hChkSts = iomnRdcStartConfirm( hRdWr, IOMN_FSMS_NVRAM_MS );
    }
    #endif
    /* D3.24 END */

    /* チェック結果OK? */
    switch( hChkSts ){
        case    IOMN_OK :   /* OKの場合 */
        iomnDebugMessage("IOMN_OK:iomnRecieveCFST()\n",0);
            ioMNFsmsStatus.hSts = IOMN_ON;

            iomnDebugMessage( "bf iomnFsmsInitStartMngTbl()-iomnRecveCFST()\n",                                                                     0 );
            /* 開始シーケンス管理テーブル初期化 */
            /* テーブル初期化及び送信先決定 */
            /* D3.24 START Y.Shimada CHG OLD */
            #if 0
            Status = iomnFsmsInitStartMngTbl( hRdWr );
            #else /* D3.24 MID NEW */
#if 0   /* MCK1.1 新メッセージ対応  */
/**/            if(( ioMNCfst.iOldMsgId == IOMSG_CFST_UI )||
/**/               ( ioMNCfst.iOldMsgId == IOMSG_CFST_JOB)){
#else
            if( ioMNCfst.iOldMsgId == SYS_MSG_CFST ){
#endif
                Status = iomnFsmsInitStartMngTbl( hRdWr );
            }else{
                Status = iomnRdcInitStartMngTbl( hRdWr );
            }
            #endif
            /* D3.24 END */
            if ( Status == ERROR ){
                cmnPrintf_ioMN(2,"Error!:iomnFsmsInitStartMngTbl()-iomnRecvCFST()[%x]\n",
                                                                errno );
                return ERROR;
            }
            iomnDebugMessage( "af iomnFsmsInitStartMngTbl()-iomnRecveCFST()\n",                                                                     0 );
            /* 開始シーケンス管理テーブル分ループ */
            for ( hCount=0; hCount < ioMNFsmsStatus.hStartTblNum; hCount++ ){
                /* 対象タスクへCFSTを転送 */
                Status = iomnSendCFST( ioMNFsmsStartSeqMng[hCount].msgqid, 
                                        ioMNFsmsStartSeqMng[hCount].iMsgId,
                                        hRdWr );
                if ( Status == ERROR ){
                    cmnPrintf_ioMN(2,"Error!:iomnSendCFST()-iomnRecieveCFST()[%x]\n",
                                                                errno );
                    return ERROR;
                }
            }
            break;

        case    IOMN_NG :   /* NGの場合 */
            iomnDebugMessage("IOMN_NG:iomnRecieveCFST()\n",0);
            /* FSMSへTFST(NG)を転送 */
            sSndSts = iomnSendTFST( hRdWr, hChkSts );
            if ( sSndSts == ERROR ){
                cmnPrintf_ioMN(2,"Error:iomnSendTFST()-iomnRecieveCFST()[%x]\n",errno);
                return ERROR;
            }
            break;
        default         :   /* ERRORの場合 */
            cmnPrintf_ioMN(2,"Error:iomnFsmsStartConfirm()-iomnRecieveCFST()[%x]\n",
            errno);
            return ERROR;
            break;
    }

    return OK;
}
/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         
TITLE:      開始シーケンス管理テーブル初期化処理
MODULE：    STATUS  iomnFsmsInitStartMngTbl()
INCLUDE:    
PARAMETER:
RETURNS:    STATUS
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            開始シーケンス管理テーブルの初期化及び
            設定(メッセージ転送先決定)処理。
NOTES:
HISTORY:
        M5.20   (TJ)H.Takahashi 99/03/18
        D3.24   Y.Shimada       00/09/11  DM45/35 ＲＤＣ(FAX)対応
                                          ※FSMS専用通関数

        MCK1.1  M.Karasawa 02/06/11 McKinley    新メッセージ対応
*/
STATUS iomnFsmsInitStartMngTbl( HI hRdWr )
{
    STATUS      Status;
    MSG_Q_ID    msgqid;
    I32         iTaskId;
    
    ioMNFsmsStatus.hStartTblNum = 1;
    
    /* テーブル初期化 */
    memset( &ioMNFsmsStartSeqMng[0], 0x00, 
                sizeof(IOMN_FSMS_SEQ_MNG) * IOMN_FSMS_MAX_SEQ_MNG );
    
    /* フロントUI情報取得 */
    Status = iomnGetFrontUIInfo( &msgqid, &iTaskId );
    if ( Status == ERROR ){
        cmnPrintf_ioMN(2,"Error:iomnGetFrontUIInfo()-iomnFsmsInitStartMngTbl()[%x]\n",
                                                                errno );
        return ERROR;
    }
    
    /* フロントUI分の設定 */
#if 0   /* MCK1.1 新メッセージ対応  */
/**/    Status = iomnFsmsSetSeqMngTbl( msgqid, iTaskId, IOMSG_CFST_UI, hRdWr,
#else
    Status = iomnFsmsSetSeqMngTbl( msgqid, iTaskId, SYS_MSG_CFST, hRdWr,
#endif
                                        &ioMNFsmsStartSeqMng[0] );
    if ( Status == ERROR ){
        cmnPrintf_ioMN(2,"Error:iomnFsmsSetSeqMngTbl()-iomnFsmsInitStartMngTbl()[%x]\n",
                                                                errno );
        return ERROR;
    }

    iomnDebugMessage("ioMNFsmsStartSeqMng[0]\n", 0 );
    iomnDebugMessage("      msgqid[%x]\n", (int)(ioMNFsmsStartSeqMng[0].msgqid) );
    iomnDebugMessage("      iTaskId[%x]\n", ioMNFsmsStartSeqMng[0].iTaskId );
    iomnDebugMessage("      iMsgId[%x]\n", ioMNFsmsStartSeqMng[0].iMsgId );
    iomnDebugMessage("      hRdWr[%x]\n", ioMNFsmsStartSeqMng[0].hRdWr );
    iomnDebugMessage("      hTrmRcvFlg[%x]\n", ioMNFsmsStartSeqMng[0].hTrmRcvFlg );
    iomnDebugMessage("      hSts[%x]\n", ioMNFsmsStartSeqMng[0].hSts );
    
    /* HWによる分岐 */
    if ( HW_GDI == (ioMNhw.hHWtype & HW_GDI) ){
        /* LANあり */
        /* テーブル数の決定 */
        ioMNFsmsStatus.hStartTblNum = 2;
        
        /* I/F先にgdiMNを追加 */
        /* gdiMNに関する情報を取得 */
        Status = iomnGetOptMNInfo( &msgqid, &iTaskId );
        if ( Status == ERROR ){
            cmnPrintf_ioMN(2,"Error:iomnGetGdiMNInfo()-iomnFsmsInitStartMngTbl()[%x]\n",
                                                                errno );
            return ERROR;
        }
        
        /* gdiMN分を設定 */
#if 0   /* MCK1.1 新メッセージ対応  */
/**/        Status = iomnFsmsSetSeqMngTbl( msgqid, iTaskId, IOMSG_CFST_JOB, hRdWr,
#else
        Status = iomnFsmsSetSeqMngTbl( msgqid, iTaskId, SYS_MSG_CFST, hRdWr,
#endif
                                        &ioMNFsmsStartSeqMng[1] );
        if ( Status == ERROR ){
            cmnPrintf_ioMN(2,"Error:iomnFsmsSetSeqMngTbl()-iomnFsmsInitMngTbl()[%x]\n",
                                                                errno );
            return ERROR;
        }
        iomnDebugMessage("ioMNFsmsStartSeqMng[1]\n", 0 );
        iomnDebugMessage("      msgqid[%x]\n", (int)(ioMNFsmsStartSeqMng[1].msgqid) );
        iomnDebugMessage("      iTaskId[%x]\n", ioMNFsmsStartSeqMng[1].iTaskId );
        iomnDebugMessage("      iMsgId[%x]\n", ioMNFsmsStartSeqMng[1].iMsgId );
        iomnDebugMessage("      hRdWr[%x]\n", ioMNFsmsStartSeqMng[1].hRdWr );
        iomnDebugMessage("      hTrmRcvFlg[%x]\n", ioMNFsmsStartSeqMng[1].hTrmRcvFlg );
        iomnDebugMessage("      hSts[%x]\n", ioMNFsmsStartSeqMng[1].hSts );
    }

    /* D3.24 START Y.Shimada ADD */
    if ( ( HW_FAX   == (ioMNhw.hHWtype & HW_FAX  ) )||
         ( HW_FAXOP == (ioMNhw.hHWtype & HW_FAXOP) ) ){
        /* FAXあり */
        /* テーブル数の決定 */
        ioMNFsmsStatus.hStartTblNum++;

        /* faxMN分を設定 */
#if 0   /* MCK1.1 新メッセージ対応  */
/**/        Status = iomnFsmsSetSeqMngTbl( msgQIdFaxMN, iTaskIdFaxMN, IOMSG_CFST_JOB, hRdWr,
#else
        Status = iomnFsmsSetSeqMngTbl( msgQIdFaxMN, iTaskIdFaxMN, SYS_MSG_CFST, hRdWr,
#endif
                                        &ioMNFsmsStartSeqMng[ioMNFsmsStatus.hStartTblNum-1] );
        if ( Status == ERROR ){
            cmnPrintf_ioMN(2,"Error:iomnFsmsSetSeqMngTbl()-iomnFsmsInitMngTbl()[%x]\n",
                                                                errno );
            return ERROR;
        }
        iomnDebugMessage("ioMNFsmsStartSeqMng[%d]\n", (I32)ioMNFsmsStatus.hStartTblNum-1 );
        iomnDebugMessage("      msgqid[%x]\n", (int)(ioMNFsmsStartSeqMng[ioMNFsmsStatus.hStartTblNum-1].msgqid) );
        iomnDebugMessage("      iTaskId[%x]\n", ioMNFsmsStartSeqMng[ioMNFsmsStatus.hStartTblNum-1].iTaskId );
        iomnDebugMessage("      iMsgId[%x]\n", ioMNFsmsStartSeqMng[ioMNFsmsStatus.hStartTblNum-1].iMsgId );
        iomnDebugMessage("      hRdWr[%x]\n", ioMNFsmsStartSeqMng[ioMNFsmsStatus.hStartTblNum-1].hRdWr );
        iomnDebugMessage("      hTrmRcvFlg[%x]\n", ioMNFsmsStartSeqMng[ioMNFsmsStatus.hStartTblNum-1].hTrmRcvFlg );
        iomnDebugMessage("      hSts[%x]\n", ioMNFsmsStartSeqMng[ioMNFsmsStatus.hStartTblNum-1].hSts );

        ioMNFsmsStatus.hStartTblNum++;
        /* faxEX1分を設定 */
#if 0   /* MCK1.1 新メッセージ対応  */
/**/        Status = iomnFsmsSetSeqMngTbl( msgQIdFaxEX[0], iTaskIdFaxEX[0], IOMSG_CFST_JOB, hRdWr,
#else
        Status = iomnFsmsSetSeqMngTbl( msgQIdFaxEX[0], iTaskIdFaxEX[0], SYS_MSG_CFST, hRdWr,
#endif
                                        &ioMNFsmsStartSeqMng[ioMNFsmsStatus.hStartTblNum-1] );
        if ( Status == ERROR ){
            cmnPrintf_ioMN(2,"Error:iomnFsmsSetSeqMngTbl()-iomnFsmsInitMngTbl()[%x]\n",
                                                                errno );
            return ERROR;
        }
        iomnDebugMessage("ioMNFsmsStartSeqMng[%d]\n", (I32)ioMNFsmsStatus.hStartTblNum-1 );
        iomnDebugMessage("      msgqid[%x]\n", (int)(ioMNFsmsStartSeqMng[ioMNFsmsStatus.hStartTblNum-1].msgqid) );
        iomnDebugMessage("      iTaskId[%x]\n", ioMNFsmsStartSeqMng[ioMNFsmsStatus.hStartTblNum-1].iTaskId );
        iomnDebugMessage("      iMsgId[%x]\n", ioMNFsmsStartSeqMng[ioMNFsmsStatus.hStartTblNum-1].iMsgId );
        iomnDebugMessage("      hRdWr[%x]\n", ioMNFsmsStartSeqMng[ioMNFsmsStatus.hStartTblNum-1].hRdWr );
        iomnDebugMessage("      hTrmRcvFlg[%x]\n", ioMNFsmsStartSeqMng[ioMNFsmsStatus.hStartTblNum-1].hTrmRcvFlg );
        iomnDebugMessage("      hSts[%x]\n", ioMNFsmsStartSeqMng[ioMNFsmsStatus.hStartTblNum-1].hSts );

        ioMNFsmsStatus.hStartTblNum++;
        /* faxEX2分を設定 */
#if 0   /* MCK1.1 新メッセージ対応  */
/**/        Status = iomnFsmsSetSeqMngTbl( msgQIdFaxEX[1], iTaskIdFaxEX[1], IOMSG_CFST_JOB, hRdWr,
#else
        Status = iomnFsmsSetSeqMngTbl( msgQIdFaxEX[1], iTaskIdFaxEX[1], SYS_MSG_CFST, hRdWr,
#endif
                                        &ioMNFsmsStartSeqMng[ioMNFsmsStatus.hStartTblNum-1] );
        if ( Status == ERROR ){
            cmnPrintf_ioMN(2,"Error:iomnFsmsSetSeqMngTbl()-iomnFsmsInitMngTbl()[%x]\n",
                                                                errno );
            return ERROR;
        }
        iomnDebugMessage("ioMNFsmsStartSeqMng[%d]\n", (I32)ioMNFsmsStatus.hStartTblNum-1 );
        iomnDebugMessage("      msgqid[%x]\n", (int)(ioMNFsmsStartSeqMng[ioMNFsmsStatus.hStartTblNum-1].msgqid) );
        iomnDebugMessage("      iTaskId[%x]\n", ioMNFsmsStartSeqMng[ioMNFsmsStatus.hStartTblNum-1].iTaskId );
        iomnDebugMessage("      iMsgId[%x]\n", ioMNFsmsStartSeqMng[ioMNFsmsStatus.hStartTblNum-1].iMsgId );
        iomnDebugMessage("      hRdWr[%x]\n", ioMNFsmsStartSeqMng[ioMNFsmsStatus.hStartTblNum-1].hRdWr );
        iomnDebugMessage("      hTrmRcvFlg[%x]\n", ioMNFsmsStartSeqMng[ioMNFsmsStatus.hStartTblNum-1].hTrmRcvFlg );
        iomnDebugMessage("      hSts[%x]\n", ioMNFsmsStartSeqMng[ioMNFsmsStatus.hStartTblNum-1].hSts );
    }
    /* D3.24 END */

    iomnDebugMessage("af iomnFsmsInitStartMngTbl()[%x]\n", ioMNhw.hHWtype );
    return OK;
}


/* D3.24 START Y.Shimada ADD */
/*
(C) Copyright TOSHIBA Corporation 2000. ALL Right Reserved
NO.         
TITLE:      開始シーケンス管理テーブル初期化処理(RDC)
MODULE：    STATUS  iomnRdcInitStartMngTbl()
INCLUDE:    
PARAMETER:
RETURNS:    STATUS
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            開始シーケンス管理テーブルの初期化及び
            設定(メッセージ転送先決定)処理。
NOTES:
HISTORY:
        D3.24   Y.Shimada       00/09/11  DM45/35 ＲＤＣ(FAX)対応
                                          ※RDC専用通関数

        MCK1.1  M.Karasawa 02/06/11 McKinley    新メッセージ対応
*/
STATUS iomnRdcInitStartMngTbl( HI hRdWr )
{
    STATUS      Status;
    MSG_Q_ID    msgqid;
    I32         iTaskId;

    ioMNFsmsStatus.hStartTblNum = 1;

    /* テーブル初期化 */
    memset( &ioMNFsmsStartSeqMng[0], 0x00, 
                sizeof(IOMN_FSMS_SEQ_MNG) * IOMN_FSMS_MAX_SEQ_MNG );

    /* フロントUI情報取得 */
    Status = iomnGetFrontUIInfo( &msgqid, &iTaskId );
    if ( Status == ERROR ){
        cmnPrintf_ioMN(2,"Error:iomnGetFrontUIInfo()-iomnRdcInitStartMngTbl()[%x]\n",
                                                                errno );
        return ERROR;
    }

    /* フロントUI分の設定 */
#if 0   /* MCK1.1 新メッセージ対応  *//* 要検討 */
/**/    Status = iomnFsmsSetSeqMngTbl( msgqid, iTaskId, IOMSG_CRST_UI, hRdWr,
/**/                                        &ioMNFsmsStartSeqMng[0] );
#endif
    if ( Status == ERROR ){
        cmnPrintf_ioMN(2,"Error:iomnFsmsSetSeqMngTbl()-iomnRdcInitStartMngTbl()[%x]\n",
                                                                errno );
        return ERROR;
    }

    iomnDebugMessage("ioMNRdcStartSeqMng[0]\n", 0 );
    iomnDebugMessage("      msgqid[%x]\n", (int)(ioMNFsmsStartSeqMng[0].msgqid) );
    iomnDebugMessage("      iTaskId[%x]\n", ioMNFsmsStartSeqMng[0].iTaskId );
    iomnDebugMessage("      iMsgId[%x]\n", ioMNFsmsStartSeqMng[0].iMsgId );
    iomnDebugMessage("      hRdWr[%x]\n", ioMNFsmsStartSeqMng[0].hRdWr );
    iomnDebugMessage("      hTrmRcvFlg[%x]\n", ioMNFsmsStartSeqMng[0].hTrmRcvFlg );
    iomnDebugMessage("      hSts[%x]\n", ioMNFsmsStartSeqMng[0].hSts );
    
    /* HWによる分岐 */
    if ( HW_GDI == (ioMNhw.hHWtype & HW_GDI) ){
        /* LANあり */
        /* テーブル数の決定 */
        ioMNFsmsStatus.hStartTblNum = 2;
        
        /* I/F先にgdiMNを追加 */
        /* gdiMNに関する情報を取得 */
        Status = iomnGetOptMNInfo( &msgqid, &iTaskId );
        if ( Status == ERROR ){
            cmnPrintf_ioMN(2,"Error:iomnGetGdiMNInfo()-iomnRdcInitStartMngTbl()[%x]\n",
                                                                errno );
            return ERROR;
        }
        
        /* gdiMN分を設定 */
#if 0   /* MCK1.1 新メッセージ対応  *//* 要検討 */
/**/        Status = iomnFsmsSetSeqMngTbl( msgqid, iTaskId, IOMSG_CRST_JOB, hRdWr,
/**/                                        &ioMNFsmsStartSeqMng[1] );
#endif
        if ( Status == ERROR ){
            cmnPrintf_ioMN(2,"Error:iomnFsmsSetSeqMngTbl()-iomnRdcInitMngTbl()[%x]\n",
                                                                errno );
            return ERROR;
        }
        iomnDebugMessage("ioMNRdcStartSeqMng[1]\n", 0 );
        iomnDebugMessage("      msgqid[%x]\n", (int)(ioMNFsmsStartSeqMng[1].msgqid) );
        iomnDebugMessage("      iTaskId[%x]\n", ioMNFsmsStartSeqMng[1].iTaskId );
        iomnDebugMessage("      iMsgId[%x]\n", ioMNFsmsStartSeqMng[1].iMsgId );
        iomnDebugMessage("      hRdWr[%x]\n", ioMNFsmsStartSeqMng[1].hRdWr );
        iomnDebugMessage("      hTrmRcvFlg[%x]\n", ioMNFsmsStartSeqMng[1].hTrmRcvFlg );
        iomnDebugMessage("      hSts[%x]\n", ioMNFsmsStartSeqMng[1].hSts );
    }else if( HW_DSS == (ioMNhw.hHWtype & HW_DSS) ){
        /* DSSあり */
        /* テーブル数の決定 */
        ioMNFsmsStatus.hStartTblNum = 2;
        
        /* I/F先にdssMNを追加 */
        /* dssMNに関する情報を取得 */
        Status = iomnGetOptMNInfo( &msgqid, &iTaskId );
        if ( Status == ERROR ){
            cmnPrintf_ioMN(2,"Error:iomnGetGdiMNInfo()-iomnRdcInitStartMngTbl()[%x]\n",
                                                                errno );
            return ERROR;
        }
        
        /* dssMN分を設定 */
#if 0   /* MCK1.1 新メッセージ対応  *//* 要検討 */
/**/        Status = iomnFsmsSetSeqMngTbl( msgqid, iTaskId, IOMSG_CRST_JOB, hRdWr,
/**/                                        &ioMNFsmsStartSeqMng[1] );
#endif
        if ( Status == ERROR ){
            cmnPrintf_ioMN(2,"Error:iomnFsmsSetSeqMngTbl()-iomnRdcInitMngTbl()[%x]\n",
                                                                errno );
            return ERROR;
        }
        iomnDebugMessage("ioMNRdcStartSeqMng[1]\n", 0 );
        iomnDebugMessage("      msgqid[%x]\n", (int)(ioMNFsmsStartSeqMng[1].msgqid) );
        iomnDebugMessage("      iTaskId[%x]\n", ioMNFsmsStartSeqMng[1].iTaskId );
        iomnDebugMessage("      iMsgId[%x]\n", ioMNFsmsStartSeqMng[1].iMsgId );
        iomnDebugMessage("      hRdWr[%x]\n", ioMNFsmsStartSeqMng[1].hRdWr );
        iomnDebugMessage("      hTrmRcvFlg[%x]\n", ioMNFsmsStartSeqMng[1].hTrmRcvFlg );
        iomnDebugMessage("      hSts[%x]\n", ioMNFsmsStartSeqMng[1].hSts );
    }

    iomnDebugMessage("af iomnRdcInitStartMngTbl()[%x]\n", ioMNhw.hHWtype );
    return OK;
}
/* D3.24 END */


/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         
TITLE:      FSMS解除指示受信処理
MODULE：    STATUS  iomnReccieveCFED()
INCLUDE:    
PARAMETER:
RETURNS:    STATUS
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            FSMS解除指示メッセージ受信時の処理。
NOTES:
HISTORY:
        M4.11   (TJ)H.Takahashi 99/01/26
        M5.00   (TJ)H.Takahashi 99/02/18
        M5.20   (TJ)H.Takahashi 99/03/18
                FSMSΦ4対応
        D3.24.2 Y.Shimada       00/09/11  DM45/35 ＲＤＣ(FAX)対応
                                          ※RDC/FSMS共通関数
*/
STATUS  iomnRecieveCFED()
{
    STATUS      sStatus;
    HI          hRdWr;

    hRdWr = ioMNrmyparam.sCfedCtl.hRdWr;

    iomnDebugMessage( "###hRdWr[%x]\n", hRdWr );

    iomnSetJobManagementMsg(&ioMNCfed);

    /* D3.24.2 START Y.Shimada ADD */
    /*
        RDC/FSMS非実行中にCFED/CREDを受信した時に、
        上位へエラーＴＲＭ(TFED/TRED)を返す処理の追加
    */
    if( ioMNFsmsStatus.hSts == IOMN_OFF ){
        iomnDebugMessage("Error(Non Exec):iomnRecieveCFED()\n",0);
        /* FSMSへTFST(NG)を転送 */
        sStatus = iomnSendTFED( hRdWr, IOMN_NG );
        if ( sStatus == ERROR ){
            cmnPrintf_ioMN(2,"Error:iomnSendTFED()-iomnRecieveCFED()[%x]\n",errno);
            return ERROR;
        }
        return OK;
    }
    /* D3.24.2 END */

    /* 解除処理 */
    sStatus = iomnFsmsCFEDProc( IOMN_OK );
    if ( sStatus == ERROR ){
        cmnPrintf_ioMN(2,"Error:iomnFsmsCFEDProc()-iomnRecieveCFED()\n");
        return ERROR;
    }

    return OK;
}

/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         
TITLE:      解除シーケンス管理テーブル初期化処理
MODULE：    STATUS  iomnFsmsInitEndMngTbl()
INCLUDE:    
PARAMETER:
RETURNS:    STATUS
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            解除シーケンス管理テーブルの初期化及び
            設定(メッセージ転送先決定)処理。
NOTES:
HISTORY:
        M5.20   (TJ)H.Takahashi 99/03/18
        D3.24   Y.Shimada       00/09/11  DM45/35 ＲＤＣ(FAX)対応
                                          ※FSMS/RDCの共通関数

        MCK1.1  M.Karasawa 02/06/11 McKinley    新メッセージ対応
*/
STATUS iomnFsmsInitEndMngTbl()
{
    STATUS      Status;
    I32         iMsgId;
    HI          hCount, hEndTblNum = 0;
    
    /* テーブル初期化 */
    memset( &ioMNFsmsEndSeqMng[0], 0x00, 
                sizeof(IOMN_FSMS_SEQ_MNG) * IOMN_FSMS_MAX_SEQ_MNG );
    
    /* 開始シーケンステーブル数分ループ */
    for ( hCount = 0; hCount < ioMNFsmsStatus.hStartTblNum; hCount++ ){
        iomnDebugMessage("ioMNFsmsStartSeqMng[%x]\n", hCount );
        iomnDebugMessage("      msgqid[%x]\n", (int)ioMNFsmsStartSeqMng[hCount].msgqid );
        iomnDebugMessage("      iTaskId[%x]\n", ioMNFsmsStartSeqMng[hCount].iTaskId );
        iomnDebugMessage("      iMsgId[%x]\n", ioMNFsmsStartSeqMng[hCount].iMsgId );
        iomnDebugMessage("      hRdWr[%x]\n", ioMNFsmsStartSeqMng[hCount].hRdWr );
        iomnDebugMessage("      hTrmRcvFlg[%x]\n", ioMNFsmsStartSeqMng[hCount].hTrmRcvFlg );
        iomnDebugMessage("      hSts[%x]\n", ioMNFsmsStartSeqMng[hCount].hSts );
        
        if ( ioMNFsmsStartSeqMng[hCount].hSts == IOMN_OK ){
            /* TFST(OK)時 */

            /* 開始指示メッセージIDによる分岐 */
            /* D3.24 START Y.Shimada CHG OLD */
            #if 0
/**/            if ( ioMNFsmsStartSeqMng[hCount].iMsgId == IOMSG_CFST_UI ){
/**/                iMsgId = IOMSG_CFED_UI;
/**/            }
/**/            else {
/**/                iMsgId = IOMSG_CFED_JOB;
/**/            }
            #else /* D3.24 MID NEW */
            switch( ioMNFsmsStartSeqMng[hCount].iMsgId ){
#if 0   /* MCK1.1 新メッセージ対応  */
/**/                case IOMSG_CFST_UI:
/**/                case IOMSG_CRST_UI:
/**/                    iMsgId = IOMSG_CFED_UI;
/**/                    break;
/**/                case IOMSG_CFST_JOB:
/**/                case IOMSG_CRST_JOB:
/**/                    iMsgId = IOMSG_CFED_JOB;
#else
                case SYS_MSG_CFST:
                    iMsgId = SYS_MSG_CFED;
#endif
                    break;
                default:
                    cmnPrintf_ioMN(2,">>>Error:MsgID[%08x] in iomnFsmsInitEndMngTbl()\n",
                        ioMNFsmsStartSeqMng[hCount].iMsgId
                    );
                    return(ERROR);
            }
            #endif
            /* D3.24 END */

            /* 開始シーケンステーブルの内容を解除シーケンステーブルへ設定 */
            Status = iomnFsmsSetSeqMngTbl( ioMNFsmsStartSeqMng[hCount].msgqid,
                                        ioMNFsmsStartSeqMng[hCount].iTaskId,
                                        iMsgId, ioMNFsmsStartSeqMng[hCount].hRdWr,
                                        &ioMNFsmsEndSeqMng[hEndTblNum] );
            if ( Status == ERROR ){
                cmnPrintf_ioMN(2,"Error:iomnFsmsSetSeqMngTbl()[%x]\n", errno );
                return ERROR;
            }
            iomnDebugMessage("ioMNFsmsEndSeqMng[%x]\n", hEndTblNum );
            iomnDebugMessage("      msgqid[%x]\n", (int)ioMNFsmsEndSeqMng[hEndTblNum].msgqid );
            iomnDebugMessage("      iTaskId[%x]\n", ioMNFsmsEndSeqMng[hEndTblNum].iTaskId );
            iomnDebugMessage("      iMsgId[%x]\n", ioMNFsmsEndSeqMng[hEndTblNum].iMsgId );
            iomnDebugMessage("      hRdWr[%x]\n", ioMNFsmsEndSeqMng[hEndTblNum].hRdWr );
            iomnDebugMessage("      hTrmRcvFlg[%x]\n", ioMNFsmsEndSeqMng[hEndTblNum].hTrmRcvFlg );
            iomnDebugMessage("      hSts[%x]\n", ioMNFsmsEndSeqMng[hEndTblNum].hSts );

            hEndTblNum++;
        }
    }
    
    ioMNFsmsStatus.hEndTblNum = hEndTblNum;

    return OK;
}

/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         
TITLE:      再起動イベント受信処理
MODULE：    STATUS  iomnReccieveEC20()
INCLUDE:    
PARAMETER:
RETURNS:    STATUS
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            再起動イベント受信時の処理。
NOTES:
HISTORY:
        M4.11   (TJ)H.Takahashi 99/01/26
        M5.00   (TJ)H.Takahashi 99/02/18
    V023.00_02  M.Karasawa  03/07/16    McKinley    ソース整理（不要処理削除）
*/
STATUS  iomnRecieveEC20()
{
#if 0  /* V023.00_02 START */
/**/    STATUS  sStatus;
/**/    MSG_Q_ID    msgqid;
/**/
/**/    /* 送信先を決定 */
/**/    sStatus = iomnGetFrontUIMsgQId( &msgqid );
/**/    if ( sStatus == ERROR ){
/**/        cmnPrintf_ioMN(2, "Error!:iomnGetFrontUIMsgQId()-iomnRecieveEC20()[%x]\n",
/**/                                                            errno );
/**/        return ERROR;
/**/    }
/**/
/**/    /* 再起動イベント送信 */
/**/    sStatus = iomnSendEC20( msgqid );
/**/    if ( sStatus == ERROR ){
/**/        cmnPrintf_ioMN(2,"Error:iomnSendEC20()-iomnRecieveEC20()[%x]\n", errno );
/**/        return ERROR;
/**/    }
/**/
/**/    if ( HW_GDI == (ioMNhw.hHWtype & HW_GDI) ){
/**/        /* 再起動イベント送信 */
/**/        sStatus = iomnSendEC20( msgQIdDssPRN );
/**/        if ( sStatus == ERROR ){
/**/            cmnPrintf_ioMN(2,"Error:iomnSendEC20()-iomnRecieveEC20()[%x]\n", errno );
/**/            return ERROR;
/**/        }
/**/    }
#endif /* V023.00_02 END   */
    return OK;
}
/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         
TITLE:      FSMS開始結果受信処理
MODULE：    STATUS  iomnReccieveTFST()
INCLUDE:    
PARAMETER:
RETURNS:    STATUS
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            FSMS開始結果受信時の処理。
NOTES:
HISTORY:
        M5.00   (TJ)H.Takahashi 99/02/18
        M5.20   (TJ)H.Takahashi 99/03/18
        D3.24   Y.Shimada       00/09/11  DM45/35 ＲＤＣ(FAX)対応
                                          ※TFSTとTRSTの共通関数

        MCK1.1  M.Karasawa 02/06/11 McKinley    新メッセージ対応
*/
STATUS iomnRecieveTFST()
{
    HI      hRdWr, hSts, hChkSts, hCount, hResult = 0;
    STATUS  sStatus;

    iomnDebugMessage("<<<Top of iomnRecieveTFST()\n", 0);

    /* メッセージパラメータの取得 */
    hRdWr = ioMNrmyparam.sTfstCtl.hRdWr;
    hSts = ioMNrmyparam.sTfstCtl.hSts;

    iomnDebugMessage("   hRdWr[%x]", hRdWr);
    iomnDebugMessage(" : hSts[%x]\n", hSts );

    iomnSetJobManagementMsg(&ioMNTfst);

/* D3.24 START Y.SHimada ADD TEST */
/* ioMNから来たTFST/TRSTはTEST用 */
#if 0
#ifdef DEBUG
if( ioMNTfst.iOldTaskId == iTaskIdIoMN ){
    cmnPrintf_ioMN(0,"TEST:Rdc/Fsms RecieveTFST()\n");
    return(OK);
}
#endif
#endif
/* D3.24 END TEST */

    /* 管理テーブル数分ループ */
    for ( hCount = 0; hCount < ioMNFsmsStatus.hStartTblNum; hCount++ ){
        /* 受信結果を開始シーケンス管理テーブルに設定 */
        if ( iioMNTaskId == ioMNFsmsStartSeqMng[hCount].iTaskId &&
             ioMNMsgQId  == ioMNFsmsStartSeqMng[hCount].msgqid ){
            ioMNFsmsStartSeqMng[hCount].hTrmRcvFlg = IOMN_ON;
            ioMNFsmsStartSeqMng[hCount].hSts = hSts;
            iomnDebugMessage("   Set StartSeqMng[%x]\n",hCount);
        }
    }

    /* 全てのタスクから受信？ */
    /* 管理テーブル数分ループ */
    for ( hCount = 0; hCount < ioMNFsmsStatus.hStartTblNum; hCount++ ){
        /* 受信結果を開始シーケンス管理テーブルに設定 */
        if ( ioMNFsmsStartSeqMng[hCount].hTrmRcvFlg == IOMN_OFF ){
            /* 未受信のものがある場合、呼び出しもとに即復帰 */
            return OK;
        }
        else {
            /* 受信済み */
            hResult = hResult | ioMNFsmsStartSeqMng[hCount].hSts;
        }
    }
    iomnDebugMessage(" hResult[%x]\n", hResult);

    /* 要求結果による分岐 */
    switch( hResult ){
        case    IOMN_OK     :
            /* OKの場合 */

            iomnDebugMessage("hResult OK!!!\n", 0);
            /* メッセージすれ違い対策のため、もう一度confirmを行う */
            /* D3.24 START Y.Shimada CHG OLD */
            #if 0
            hChkSts = iomnFsmsStartConfirm( hRdWr, IOMN_FSMS_NVRAM_MS );
            #else /* D3.24 MID NEW */
#if 0   /* MCK1.1 新メッセージ対応  */
/**/            if(( ioMNCfst.iOldMsgId == IOMSG_CFST_UI )||
/**/               ( ioMNCfst.iOldMsgId == IOMSG_CFST_JOB)){
#else
            if( ioMNCfst.iOldMsgId == SYS_MSG_CFST ){
#endif
                hChkSts = iomnFsmsStartConfirm( hRdWr, IOMN_FSMS_NVRAM_MS );
            }else{
                hChkSts = iomnRdcStartConfirm( hRdWr, IOMN_FSMS_NVRAM_MS );
            }
            #endif
            /* D3.24 END */
            switch( hChkSts ){
                case    IOMN_OK :/* OKの場合 */
                    iomnDebugMessage("Reconfirm OK!!!\n", 0);
                    /* FSMS開始可能 */
                    /* TFST(OK)をFSMSタスクへ送信 */
                    sStatus = iomnSendTFST( hRdWr, hChkSts );
                    if ( sStatus == ERROR ){
                        cmnPrintf_ioMN(2, "Error!:iomnSendTFST()-iomnRecieveTFST()[%x]\n",                                                                    errno );
                        return ERROR;
                    }
                    break;
                case    IOMN_NG :/* NGの場合 */
                    iomnDebugMessage("Reconfirm NG!!!\n", 0);
                    /* FSMS開始不可能 */
                    /* TFST(OK)を送信してきたタスクへFSMS解除指示を送信 */
                    /* 解除処理 */
                    sStatus = iomnFsmsCFEDProc( hChkSts );
                    if ( sStatus == ERROR ){
                        cmnPrintf_ioMN(2,"Error:iomnFsmsCFEDProc()-iomnRecvTFST()\n");
                        return ERROR;
                    }
                    break;
                default         :/* ERRORの場合 */
                    cmnPrintf_ioMN(2, "Error!:iomnFsmsStartConfirm()[%x]\n", errno );
            }
            break;
        case    IOMN_NG     :
            /* NGの場合 */
            iomnDebugMessage("hResult NG!!!\n", 0);
            /* 解除シーケンス管理テーブル初期化 */
            sStatus = iomnFsmsCFEDProc( hResult );
            if ( sStatus == ERROR ){
                cmnPrintf_ioMN(2,"Error!:iomnFsmsCFEDProc()-iomnRecieveTFST()\n");
                return ERROR;
            }
            
            /* 解除数による分岐 */
            if ( ioMNFsmsStatus.hEndTblNum == 0 ){
                /* TFST(NG)をFSMSタスクへ送信 */
                sStatus = iomnSendTFST( hRdWr, hResult );
                if ( sStatus == ERROR ){
                    cmnPrintf_ioMN(2, "Error!:iomnSendTFST()-iomnRecieveTFST()[%x]\n", errno );
                    return ERROR;
                }
            }
            break;
        default             :
            /* ERRORの場合 */
            cmnPrintf_ioMN(2, "Error TFST parm!\n");
            break;
    }

    iomnDebugMessage("<<<Bottom of iomnRecieveTFST()\n", 0);
    return OK;
}

STATUS  iomnFsmsCFEDProc( HI hCanFact )
{
    STATUS      Status;
    HI          hCount;
    
    iomnDebugMessage(">>>Top of iomnFsmsCFEDProc() Arg(%x)\n", hCanFact);
    /* 解除シーケンス管理テーブル初期化 */
    Status = iomnFsmsInitEndMngTbl();
    if ( Status == ERROR ){
        cmnPrintf_ioMN(2,"Error:iomnInitEndMngTbl()-iomnRecvTFST()\n");
        return ERROR;
    }

    iomnDebugMessage("ioMNFsmsStatus.hEndTblNum[%x]\n", ioMNFsmsStatus.hEndTblNum);

    if ( ioMNFsmsStatus.hEndTblNum > 0 ){

        if ( hCanFact != IOMN_OK ){
            /* FSMSキャンセル発生 */
            ioMNFsmsStatus.hCancel = IOMN_ON;
            ioMNFsmsStatus.hCanFact = hCanFact;
            iomnDebugMessage("Cancel Flag On\n", 0);
        }

        /* 解除シーケンス管理テーブル数分ループ */
        for( hCount=0; hCount < ioMNFsmsStatus.hEndTblNum; hCount++ ){
            /* 解除指示送信 */
            Status = iomnSendCFED( ioMNFsmsEndSeqMng[hCount].msgqid,
                                ioMNFsmsEndSeqMng[hCount].iMsgId,
                                ioMNFsmsEndSeqMng[hCount].hRdWr );
            if ( Status == ERROR ){
                cmnPrintf_ioMN(2, "Error!:iomnSendCFED()-iomnRecieveTFST()[%x]\n", errno );
                return ERROR;
            }
        }
    }
    
    return OK;
}

/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         
TITLE:      FSMS解除結果受信処理
MODULE：    STATUS  iomnReccieveTFED()
INCLUDE:    
PARAMETER:
RETURNS:    STATUS
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            FSMS解除結果受信時の処理。
NOTES:
HISTORY:
        M5.00   (TJ)H.Takahashi 99/02/19
        M5.30   (TJ)H.Takahashi 99/03/19
                FSMSΦ4対応
*/
STATUS iomnRecieveTFED()
{
    HI      hRdWr, hSts, hChkSts=0, hCount;
    STATUS  Status;

    /* メッセージパラメータの取得 */
    hRdWr = ioMNrmyparam.sTfedCtl.hRdWr;
    hSts = ioMNrmyparam.sTfedCtl.hSts;

    iomnSetJobManagementMsg(&ioMNTfed);

/* D3.24 START Y.SHimada ADD TEST */
/* ioMNから来たTFST/TRSTはTEST用 */
#if 0
#ifdef DEBUG
if( ioMNTfed.iOldTaskId == iTaskIdIoMN ){
    cmnPrintf_ioMN(0,"TEST:Rdc/Fsms RecieveTFED()\n");
    return(OK);
}
#endif
#endif
/* D3.24 END TEST */

    /* 管理テーブル数分ループ */
    for ( hCount = 0; hCount < ioMNFsmsStatus.hEndTblNum; hCount++ ){
        /* 受信結果を開始シーケンス管理テーブルに設定 */
        if ( iioMNTaskId == ioMNFsmsEndSeqMng[hCount].iTaskId &&
             ioMNMsgQId  == ioMNFsmsEndSeqMng[hCount].msgqid ){
            ioMNFsmsEndSeqMng[hCount].hTrmRcvFlg = IOMN_ON;
            ioMNFsmsEndSeqMng[hCount].hSts = hSts;
        }
    }

    /* 全てのタスクから受信？ */
    /* 管理テーブル数分ループ */
    for ( hCount = 0; hCount < ioMNFsmsStatus.hEndTblNum; hCount++ ){
        /* 受信結果を開始シーケンス管理テーブルに設定 */
        if ( ioMNFsmsEndSeqMng[hCount].hTrmRcvFlg == IOMN_OFF ){
            /* 未受信のものがある場合、呼び出しもとに即復帰 */
            return OK;
        }
        else {
            /* 受信済み */
            hChkSts = hChkSts | hSts;
        }
    }

    /* FSMS状態による分岐 */
    if ( ioMNFsmsStatus.hSts == IOMN_ON && ioMNFsmsStatus.hCancel == IOMN_ON ){
        /* FSMS開始失敗時(メッセージすれ違い等) */
        Status = iomnSendTFST( hRdWr, ioMNFsmsStatus.hCanFact );
        if ( Status == ERROR ){
            cmnPrintf_ioMN(2,"Error!:iomnRecieveTFST()-iomnRecieveTFED()[%x]\n", errno );
            return ERROR;
        }
    }
    else {
        /* 通常のFSMS解除終了時 */
        /* FSMSタスクへFSMS解除結果を送信 */
        Status = iomnSendTFED( hRdWr, hChkSts );
        if ( Status == ERROR ){
            cmnPrintf_ioMN(2,"Error!:iomnSendTFED()-iomnRecieveTFED()[%x]\n", errno );
            return ERROR;
        }
    }

    /* FSMS状態テーブルの初期化 */
    memset( &ioMNFsmsStatus, 0x00, sizeof(ioMNFsmsStatus) );

    return OK;
}

STATUS iomnGetFrontUIMsgQId( MSG_Q_ID *msgqid )
{
    /* フロントUIのメッセージキューIDを取得 */
    /* D3.24 START Y.Shimada CHG OLD */
    #if 0
    switch( ioMNhw.hPriUi ){
    #else /* D3.24 MID NEW */
    switch( ioMNhw.hTopUI ){
    #endif
    /* D3.24 END */
        case    UI_PPC      :
        case    UI_INTPPC   :
            /* コピーUIの場合 */
            *msgqid = msgQIdCpyUI;
            iomnDebugMessage( "UI_PPC or UI_INTPPC\n", 0 );
            break;
        case    UI_MACHINE  :
            /* マシンUIの場合 */
            *msgqid = msgQIdMcnUI;
            iomnDebugMessage( "UI_MACHINE\n", 0 );
            break;
        case    UI_DIG      :
            *msgqid = msgQIdDigUI;
            iomnDebugMessage( "UI_DIG\n", 0 );
            break;
#if 0 /* RIOモジュール削除 */
/**/        case    UI_JOBMN    :
/**/            *msgqid = msgQIdJobUI;
/**/            iomnDebugMessage( "UI_JOBMN\n", 0 );
/**/            break;
/**/        /* D3.24 START Y.Shimada ADD */
#endif
        case    UI_PRINT    :
            *msgqid = msgQIdPrnUI;
            iomnDebugMessage( "UI_PRINT\n", 0 );
            break;
        case    UI_FAX     :
            *msgqid = msgQIdFaxUI;
            iomnDebugMessage( "UI_FAX\n", 0 );
            break;
        /* D3.24 END */
        default             :
            cmnPrintf_ioMN(2,"Error!:Undefined UI!iomnRecieveCFST()[%x]\n", 
                                                ioMNhw.hPriUi );
            return ERROR;
    }

    return OK;
}

/* フロントUI情報取得 */
STATUS  iomnGetFrontUIInfo( MSG_Q_ID *msgqid, I32 *iTaskId )
{
    /* 引数チェック */
    if ( msgqid == NULL || iTaskId == 0 ){
        cmnPrintf_ioMN(2,"Error Arg!:iomnGetFrontUIInfo()\n");
        return ERROR;
    }

    /* UIによる分岐 */
    /* D3.24 START Y.Shimada CHG OLD */
    #if 0
    switch( ioMNhw.hPriUi ){
    #else /* D3.24 MID NEW */
    switch( ioMNhw.hTopUI ){
    #endif
    /* D3.24 END */
        case    UI_PPC      :
        case    UI_INTPPC   :
            /* コピーUIの場合 */
            *msgqid = msgQIdCpyUI;
            *iTaskId = iTaskIdCpyUI;
            iomnDebugMessage( "UI_PPC or UI_INTPPC\n", 0 );
            break;
        case    UI_MACHINE  :
            /* マシンUIの場合 */
            *msgqid = msgQIdMcnUI;
            *iTaskId = iTaskIdMcnUI;
            iomnDebugMessage( "UI_MACHINE\n", 0 );
            break;
        case    UI_DIG      :
            *msgqid = msgQIdDigUI;
            *iTaskId = iTaskIdDigUI;
            iomnDebugMessage( "UI_DIG\n", 0 );
            break;
#if 0 /* RIOモジュール削除 */
/**/        case    UI_JOBMN    :
/**/            *msgqid = msgQIdJobUI;
/**/            *iTaskId = iTaskIdJobUI;
/**/            iomnDebugMessage( "UI_JOBMN\n", 0 );
/**/            break;
#endif
        /* D3.24 START Y.Shimada ADD */
        case    UI_PRINT    :
            *msgqid = msgQIdPrnUI;
            *iTaskId = iTaskIdPrnUI;
            iomnDebugMessage( "UI_PRINT\n", 0 );
            break;
        case    UI_FAX     :
            *msgqid = msgQIdFaxUI;
            *iTaskId = iTaskIdFaxUI;
            iomnDebugMessage( "UI_FAX\n", 0 );
            break;
        /* D3.24 END */
        default             :
            cmnPrintf_ioMN(2,"Error!:Undefined UI!iomnGetFrontUIInfo()[%x]\n", 
                                                ioMNhw.hPriUi );
            return ERROR;
    }
    
    return OK;
}

/* gdiMNに関する情報を取得 */
STATUS  iomnGetOptMNInfo( MSG_Q_ID *msgqid, I32 *iTaskId )
{
#if 0 /* RIOモジュール削除 */
/**/    if ( msgqid == NULL || iTaskId == 0 ){
/**/        cmnPrintf_ioMN(2,"Error Arg:iomnGetOptMNInfo()\n");
/**/        return ERROR;
/**/    }
/**/
/**/    /* D3.24 START Y.Shimada CHG OLD */
/**/    #if 0
/**/    *msgqid = msgQIdDssPRN;
/**/    *iTaskId = iTaskIdDssPRN;
/**/    #else /* D3.24 MID NEW */
/**/    if ( HW_GDI == (ioMNhw.hHWtype & HW_GDI) ){
/**/        *msgqid = msgQIdDssPRN;
/**/        *iTaskId = iTaskIdDssPRN;
/**/    }else if ( HW_DSS == (ioMNhw.hHWtype & HW_DSS) ){
/**/        *msgqid = msgQIdDssFMN;
/**/        *iTaskId = iTaskIdDssFMN;
/**/    }
/**/    #endif
/**/    /* D3.24 END */
/**/
/**/    return OK;
#else
    return ERROR;
#endif
}

STATUS  iomnFsmsSetSeqMngTbl( MSG_Q_ID msgqid, I32 iTaskId, I32 iMsgId, 
                            HI hRdWr, IOMN_FSMS_SEQ_MNG *SeqMngTbl )
{
    if ( SeqMngTbl == 0 ){
        cmnPrintf_ioMN(2,"Error Arg!:iomnFsmsSetSeqMngTbl()\n");
        return ERROR;
    }
    
    SeqMngTbl->msgqid = msgqid;
    SeqMngTbl->iTaskId = iTaskId;
    SeqMngTbl->iMsgId = iMsgId;
    SeqMngTbl->hRdWr  = hRdWr;
    
    return OK;
}


/*
    通常(拡張)コピーおよび割り込みコピーの場合の印刷開始処理

    HISTORY:
        D1.1 T.Fujii 99/11/15 DM45/35 original
    V016.00_04  M.Karasawa  03/03/31    McKinley    複写ジョブ管理エリアに関する処理を前面削除
*/
STATUS iomnPPCPrnStart( HI hJobID )
{
    #ifdef IOMN_DM_DEBUG_991115  /* D1.1 T.Fujii 99/11/15 */
        cmnPrintf_ioMN(0, ">> ioMN >> START:iomnPPCPrnStart()\n" );
    #endif
#if 0   /* V016.00_04   START   */
/**/    /* コピー履歴の更新(ジョブ状態を出力中に) */
/**/    if( iomnCpyMNStoreCopyLog(iomnGetCpyJobIDByPrnJobID(hJobID), IOMN_CPYJOB_PRN_START)
/**/        == ERROR ) {
/**/        iomnDebugMessage(">>>Error:iomnCpyMNStoreCopyLog() in iomnPPCPrnStart()\n",0);
/**/        return( ERROR );
/**/    }
#endif  /* V016.00_04   END     */
    return( OK );
}


/*
    通常(拡張)コピーおよび割り込みコピーの場合の印刷終了処理

    HISTORY:
        D1.1 T.Fujii 99/11/15 DM45/35 original
    V016.00_04  M.Karasawa  03/03/31    McKinley    複写ジョブ管理エリアに関する処理を前面削除
*/
STATUS iomnPPCPrnEnd( HI hJobID )
{
    #ifdef IOMN_DM_DEBUG_991115  /* D1.1 T.Fujii 99/11/15 */
        cmnPrintf_ioMN(0, ">> ioMN >> START:iomnPPCPrnEnd()\n" );
    #endif
#if 0   /* V016.00_04   START   */
/**/    /* コピー履歴の更新(ジョブ状態を出力完了に) */
/**/    if( iomnCpyMNStoreCopyLog(iomnGetCpyJobIDByPrnJobID(hJobID), IOMN_CPYJOB_PRN_END)
/**/        == ERROR ) {
/**/        iomnDebugMessage(">>>Error:iomnCpyMNStoreCopyLog() in iomnPPCPrnEnd()\n",0);
/**/        /* return( ERROR );     処理は継続させる */
/**/    }
#endif  /* V016.00_04   END     */
    return( OK );
}


/*
    逐次制御テーブルの開放

    HISTORY:
        D1.1    T.Fujii     99/11/15 DM45/35 original
        D3.12.1 T.Fujii     00/09/04 DM45/35 BG印刷ジョブ中断処理修正
*/
/* D3.12.1 START T.Fujii 印刷禁止処理修正 */
#if 0   /* OLD */
STATUS iomnReleaseSyncLink( HI hJobID )
#else   /* D3.12.1 MID  NEW */
#if 0 
STATUS iomnReleaseSyncLink( HI hIpTblID ,HI hSubJobMode)
#else
STATUS iomnReleaseSyncLink( HI hIpTblID )
#endif
#endif
/* D3.12.1 END */
{
/* D3.12.1 START T.Fujii 印刷禁止処理修正 */
#if 0   /* OLD */
    HI hIpTblID;    /* ipテーブルID(逐次制御テーブルIDにも利用) */

    /* ipテーブルIDの取得 */
    hIpTblID = iomnJobInfo[hJobID].hIpTblID;
#endif
/* D3.12.1 END */
/*Ashwin Changed For Completion Copy*/
#if 0 
	if(COM_JB_SUB_INPUT == hSubJobMode)
	{
		iomnDebugMessage("TJFN for input reached\n",0);
		return ERROR;
	
	}
#endif
    /* ipテーブルの解放、原稿入力と印刷ジョブのリンクを解除 */
    /* 逐次制御テーブルの開放を試みる */
    if( iomnSyncTblDeleteJob(hIpTblID) == OK ) {
        /* テーブル開放に成功すればIPテーブルも開放 */
        ipLibDeleteJob( hIpTblID );
/* D3.12.1 START T.Fujii 印刷禁止処理修正 */
#if 0   /* OLD */
        iomnJobInfo[hJobID].hIpTblID = IPLIB_INIT;
#endif
/* D3.12.1 END */
        iomnDebugMessage(">> ioMN >> OK:ipLibDeleteJob() in iomnReleaseSyncLink()\n",0);
        return( OK );
    }
    else {
        return( ERROR );
    }
}


/*
    逐次制御テーブルのクリア処理

    HISTORY:
        D2.4.4 T.Fujii  00/06/20 DM45/35 original
        D3.12.1 T.Fujii 00/09/04 DM45/35 BG印刷ジョブ中断処理修正
*/
STATUS iomnClearSyncLink( HI hJobID, HI hSubJob )
{
    /* 逐次制御テーブルの開放を試みる */
/* D3.12.1 START T.Fujii 印刷禁止処理修正 */
#if 0   /* OLD */
    if( iomnReleaseSyncLink(hJobID) == ERROR ) {
#else   /* D3.12.1 MID  NEW */
	/*Ashwin Changed For Completion Copy*/
#if 0 
    if( iomnReleaseSyncLink(iomnJobInfo[hJobID].hIpTblID,hSubJob) == ERROR ) {
#else
 if( iomnReleaseSyncLink(iomnJobInfo[hJobID].hIpTblID)==ERROR){
 #endif
#endif
/* D3.12.1 END */
        /* 逐次テーブルの開放ができなかった場合 */
        iomnDebugMessage( "[ioMN]CHECK:iomnSyncTbl for JobID=%x remain.\n", (I32)hJobID );

        /* 逐次制御テーブルのジョブIDはクリアしておく */
		/*Function Definition changed for STFR_12963*/
        if( iomnSyncTblClearJobID(iomnJobInfo[hJobID].hIpTblID, hSubJob, hJobID)
            == ERROR ) {
            /* D3.12.1 START T.Fujii 印刷禁止処理修正 */
            iomnJobInfo[hJobID].hIpTblID = IPLIB_INIT;
            /* D3.12.1 END */
            iomnDebugMessage("[ioMN]ERROR:in iomnClearSyncLink(JobID=%d)\n",(I32)hJobID);
            return( ERROR );
        }
    }

    /* ここまで来たときは何らかの処理は成功 */
    /* D3.12.1 START T.Fujii 印刷禁止処理修正 */
    iomnJobInfo[hJobID].hIpTblID = IPLIB_INIT;
    /* D3.12.1 END */
    iomnDebugMessage("[ioMN]OK:in iomnClearSyncLink(JobID=%d)\n",(I32)hJobID);
    return( OK );
}


/*
    入出力サブジョブの中止(中止メッセージの送信)

    HISTORY:
        D1.1     T.Fujii     99/11/15    DM45/35 original
        D1.7.12  J.Ootani    00/04/19    DM45/35 リスト印刷不具合対応
        D1.13    T.Fujii     00/05/17    DM45/35 サブジョブ中止処理修正
        D1.13.1  T.Fujii     00/05/20    DM45/35 サブジョブ中止処理ソースコード整理
                                                 (#if 0～#endifを整理)
        D2.9     M.Fukuda    00/07/04    DM45/35 FAXリストレポート対応
        D2.9.2   M.Fukuda    00/07/12    DM45/35 FAXリストレポート対応
    V011.00_04  M.Karasawa  03/01/14    McKinley    中止処理対応
*/
STATUS iomnSubJobCancel( HI hJobID, HI hJobType, HI hCanFactor )
{
    HI hSubJobmode;
/*    HI hPartnerJobID; *//* V028.00_00 */
/*    HI hIpTblID;      *//* V028.00_00 */

    #ifdef IOMN_DM_DEBUG_991115  /* D1.1 T.Fujii 99/11/15 */
        cmnPrintf_ioMN(0, ">> ioMN >> START:iomnSubJobCancel()\n" );
        iomnSyncTblCheck();
        iomnCpyMNCheck();
    #endif

    /* サブジョブ種別の取得 */
    if( parMNSubJobmode(hJobID, &hSubJobmode) == ERROR ) {
        return( ERROR );
    }

    /* サブジョブごとのキャンセル処理 */
    if( hSubJobmode == COM_JB_SUB_INPUT ) {
#if 0   /* V011.00_04   *//* START  */
/**/        /* 入力ジョブのキャンセル */
/**/        /*  対象JOB種別にFAXリストレポート送信を追加 */
/**/        if( hJobType == COM_JB_LIST_PRT_FAX ||
/**/            hJobType == COM_JB_LIST_RESULT_SEND)
/**/        {
/**/            return(iomnOutputJobCancel(hJobID, hCanFactor));
/**/        }
/**/        /* 出力ジョブのジョブIDを取得する */
/**/        hPartnerJobID = iomnSyncTblGetJobID( iomnJobInfo[hJobID].hIpTblID, COM_JB_SUB_OUTPUT );
/**/
/**/        /* 出力ジョブが存在すれば中止させる */
/**/        if( hPartnerJobID > IOMN_INPUTJOB_ID_MAX ) {
/**/            iomnOutputJobCancel(hPartnerJobID, hCanFactor);
/**/        }
#endif  /* V011.00_04   *//* END    */
        /* 入力ジョブのキャンセルを行い終了 */
        return( iomnInputJobCancel(hJobID, hCanFactor) );
    }
    else {
#if 0   /* V011.00_04   *//* START  */
/**/        /* 入力ジョブのジョブIDを取得する */
/**/        hIpTblID = iomnSyncTblGetTblId( hJobID );
/**/        hPartnerJobID = iomnSyncTblGetJobID( hIpTblID, COM_JB_SUB_INPUT );
/**/
/**/        /* 入力ジョブが存在する場合 */
/**/        if( hPartnerJobID>=0 && hPartnerJobID<=IOMN_INPUTJOB_ID_MAX ) {
/**/            /* 同期テーブルも同じなら中止 */
/**/            if( iomnJobInfo[hPartnerJobID].hIpTblID == hIpTblID ) {
/**/                iomnInputJobCancel(hPartnerJobID, hCanFactor);
/**/            }
/**/        }
#endif  /* V011.00_04   *//* END    */
        /* 出力ジョブのキャンセル */
        return( iomnOutputJobCancel(hJobID, hCanFactor) );
    }

    /* ここまで来たときはエラー */
    cmnPrintf_ioMN(2, ">> ioMN >> ERROR:invalid subjob mode=0x%x in iomnSubJobCancel()\n", hSubJobmode );
    return( ERROR );
}

/*
    入力ジョブのキャンセル(中止メッセージの送信)

    HISTORY:
        D1.1 T.Fujii 99/11/15 DM45/35 original
*/
STATUS iomnInputJobCancel( HI hInputJobID, HI hCanFactor )
{
    #ifdef IOMN_DM_DEBUG_991115  /* D1.1 T.Fujii 99/11/15 */
        cmnPrintf_ioMN(0, ">> ioMN >> START:iomnInputJobCancel()\n" );
    #endif

    /* 入力ジョブの中止指示 */
    return( iomnSendJobCancel(hInputJobID, msgQIdIoExMN, hCanFactor) );
}

/*
    出力ジョブのキャンセル(中止メッセージの送信)

    HISTORY:
        D1.1     T.Fujii     99/11/15 DM45/35 original
        D1.13.1  T.Fujii     00/05/20  DM45/35 サブジョブ中止処理修正
        D2.9.1   M.Fukuda    00/07/12  DM45/35 中止処理開始後のリストレポート印刷開始対応
        D3.16    M.Fukuda    00/09/12  DM45/35 JOB削除開始フラグ設定処理修正
*/
STATUS iomnOutputJobCancel( HI hOutputJobID, HI hCanFactor )
{
    HI hJobStatus;
    I32 iSusCause;
    HI  hJobType;       /*  JOB種類             */
    STATUS  Status;     /*  ＪＯＢ状態変更結果  */
/* D3.16 START M.Fukuda JOB削除開始フラグ設定処理修正 */
    HI      hSubJobmode;
/* D3.16 END */

    #ifdef IOMN_DM_DEBUG_991115  /* D1.1 T.Fujii 99/11/15 */
        cmnPrintf_ioMN(0, ">> ioMN >> START:iomnOutputJobCancel()\n" );
    #endif

    /*  JOB種類取得 */
    if(ERROR == (Status = jobJobtype(hOutputJobID, &hJobType))){
        iomnDebugMessage(">>>Error:jobJobtype() in iomnOutputJobCancel()\n",0);
        cmnPrintf_ioMN(2,">>>Error:jobJobtype() in iomnOutputJobCancel())\n");
        return(ERROR);
    }

    /* ジョブ状態の取得 */
    if( jobStatus(hOutputJobID, &hJobStatus, &iSusCause) == ERROR ) {
        iomnDebugMessage(">>>Error:jobStatus() in iomnOutputJobCancel()\n",0);
        return( ERROR );
    }

/* D3.16 START M.Fukuda JOB削除開始フラグ設定処理修正 */
   /* サブジョブ種別の取得 */
    if( parMNSubJobmode(hOutputJobID, &hSubJobmode) == ERROR ) {
        cmnPrintf_ioMN(2,">> ioMN >> Error:parMNSubJobmode() in iomnOutputJobCancel()\n");
        return( ERROR );
    }
/* D3.16 END */

    /* ジョブ中止処理 */
    if( hJobStatus==COM_JS_RUNNING
        || hJobStatus==COM_JS_PRINTING
        || (hJobStatus==COM_JS_SUSPEND && ioMNAjst[hOutputJobID].OldmsgQId != NULL) ) {
       /* JOB中止要求送信 */
        if( iomnSendJobCancel(hOutputJobID, msgQIdIoExMN, hCanFactor) == OK ) {
            /* JOB削除開始中フラグセット */
/* D3.16 START M.Fukuda JOB削除開始フラグ設定処理修正 */
#if 0
            if(hJobType == COM_JB_LIST_PRT_FAX){
                ioMNstatus.hSendCjcntart = IOMN_ON;
            }
#else
            if(hJobType == COM_JB_LIST_PRT_FAX && hSubJobmode == COM_JB_SUB_INPUT){
                ioMNstatus.hSendCjcntart = IOMN_ON;
            }
#endif
/* D3.16 END */
            return( OK );
        }
        else {
            return(ERROR);
        }
    }
    else {
        /* 印刷未動作の場合 */
        return( iomnWaitingOutputJobCancel(hOutputJobID, hJobStatus) );
    }
}


/*
    未実行出力ジョブのキャンセル

    HISTORY:
        D1.2 T.Fujii 99/12/24 DM45/35 original
        D1.6.8 T.Fujii  00/04/10 DM45/35 出力開始チェック仕様変更(入力枚数0に対応)
        D2.4.4 T.Fujii  00/06/20 DM45/35 V004.70不具合対応
                                         ジョブ終了後のJobIDクリア処理対応
        D2.10.1 T.Fujii 00/07/08 DM45/35 JOBEND送信処理追加
        D2.10.2 T.Fujii 00/07/10 DM45/35 JOBEND送信処理修正
        D2.9.2  M.Fukuda   00/07/12  DM45/35 FAXリストレポート対応

    V013.00_04  M.Karasawa  03/02/07    McKinley    ジョブ削除(jobDelete())はioMNでは行わず、上位で行う
    V015.00_05  M.Karasawa  03/03/05    McKinley    未実行の複写JOBに対する中止処理時、TJFN未送信になる不具合を修正
    V016.00_04  M.Karasawa  03/03/31    McKinley    複写ジョブ管理エリアに関する処理を前面削除
    V017.00_02  M.Karasawa  03/04/09    McKinley    カラートナーエンプティ動作対応
    V019.00_03  M.Karasawa  03/05/20    McKinley    ジョブ操作関連の仕様(?)変更対応
    V020.00_01  M.Karasawa  03/06/02    McKinley    ジョブ操作での未実行印刷ジョブ削除時もEPFN送信するように修正(McK-No773)
    V029.00_02  M.Karasawa  03/10/03    McKinley    未実行ジョブ削除時はプリンタ状態しないよう修正(McK4002)
    V045.00_01  M.Karasawa  04/06/02    McKinley    未実行ジョブ削除時にメッセージのクリアを行うようにする(Mck6396)
*/
STATUS iomnWaitingOutputJobCancel( HI hOutputJobID, HI hJobStatus )
{
    HI hPrnType;    /* 印刷種別 */
    HI hsts;        /* TJFNステータス   *//* V017.00_02 ADD */
    /* V017.00_02 START */
    I32 iErrEng;
    /* V017.00_02 END   */
    union ioMNparam strSendPrm;      /* 送信パラメータ *//* V029.00_02 *//* ADD */

    /* 出力ジョブが印刷待ちの場合、印刷開始をキャンセル */
#if 0   /* V015.00_05   */
/**/    if( hJobStatus==COM_JS_PRT_WAITING ) {
#else
    if( hJobStatus==COM_JS_PRT_WAITING ||
    (hJobStatus==COM_JS_SUSPEND && ioMNAjst[hOutputJobID].OldmsgQId == NULL)) {
#endif        /* 印刷中状態をクリア */
        if( iomnSyncTblSetStatus(iomnJobInfo[hOutputJobID].hIpTblID, COM_JB_SUB_OUTPUT,
                                 (HI)IOMN_IPTBL_NOTUSED) == ERROR ) {
            iomnDebugMessage(">> ioMN >> Error:iomnSyncTblSetStatus() in iomnWaitingOutputJobCancel()\n",0);
            /* return( ERROR );     処理は継続させる */
        }
    }

    /* 逐次制御テーブルの開放を試みる */
    iomnClearSyncLink( hOutputJobID, COM_JB_SUB_OUTPUT );

    /* 印刷ジョブ種別の取得 */
    hPrnType = iomnPrnQueGetPrnType( hOutputJobID );

    /* 印刷ジョブを印刷キューから削除 */
    /* 印刷種別がFAXﾘｽﾄﾚﾎﾟｰﾄ送信の場合 */
    if( hPrnType == IOMN_RESULTSNDQUE_FAX) {
        if( iomnPrnQueJobDelete(hOutputJobID,IOMN_SENDQ_ENTRY_ID) == ERROR ) { /* 送信ジョブの削除 */
            cmnPrintf_ioMN(2,">> ioMN >> Error:iomnPrnQueJobDelete() in iomnWaitingOutputJobCancel()\n" );
            return( ERROR );
        }
    }
    else {
        if( iomnPrnQueJobDelete(hOutputJobID,hIoMNPrnQueEntryID) == ERROR ) { /* 印刷ジョブの削除 */
            cmnPrintf_ioMN(2,">> ioMN >> Error:iomnPrnQueJobDelete() in iomnWaitingOutputJobCancel()\n" );
            return( ERROR );
        }
    }

    /* TJFNの送信/コピージョブの場合別処理 */
    if( hPrnType == PRNQUE_CPY_NORMAL ) {
#if 0   /* V016.00_04   START   */
/**/        /* コピージョブの場合 */
/**/        /* コピー履歴の更新(ジョブ状態を出力完に) */
/**/        if( iomnCpyMNStoreCopyLog(iomnGetCpyJobIDByPrnJobID(hOutputJobID), IOMN_CPYJOB_PRN_END)
/**/            == ERROR ) {
/**/            iomnDebugMessage(">> ioMN >> Error:iomnCpyMNStoreCopyLog() in iomnWaitingOutputJobCancel()\n",0);
/**/            /* return( ERROR );     処理は継続させる */
/**/        }
/**/
/**/        /* コピージョブ情報の開放 */
/**/        iomnCpyMNFreeCopyJob( iomnGetCpyJobIDByPrnJobID(hOutputJobID) );
#endif  /* V016.00_04   END     */
#if 0   /* V013.00_04   */
/**/        /* 印刷用ジョブID解放 */
/**/        jobDelete( hOutputJobID );
#endif
    }
#if 0   /* V015.00_05   */
/**/    else {
#endif
#if 0   /* V017.00_02 OLD */
/**/        /* コピーの印刷ジョブ以外の場合、TJFNを送信する */
/**/        if( iomnSendTjfn(hOutputJobID, (HI)IOMSG_CAN_OK, (I32)IOMSG_NOT_USED, (I32)IOMSG_NOT_USED)
#else   /* V017.00_02 NEW */
        /*
            中止指示を受信しており、中止要因がカラートナーエンプティの場合、
            TJFNにその情報を載せる。それ以外の場合は従来通り
        */
        if(ioMNCjcn[hOutputJobID].iOldTaskId != 0 && 
           ioMNCjcn[hOutputJobID].msgparam.sCjcnCtl.hCanFactor == IOMSG_CAN_COLORTONER_EMPTY){
            hsts    = IOMSG_CAN_ERROR;
            iErrEng = (I32)IOMSG_CAN_COLORTONER_EMPTY;
        }
        /* V019.00_03 START */
        else if(ioMNCjcn[hOutputJobID].iOldTaskId != 0 &&
             ioMNCjcn[hOutputJobID].msgparam.sCjcnCtl.hCanFactor == IOMSG_CAN_REMOVE){
            hsts    = IOMSG_CAN_OK;
            iErrEng = (I32)IOMSG_CAN_REMOVE;
        }
        /* V019.00_03 END   */
        else{
#if 0   /* V029.00_02 */
/**/            /* V020.00_01 START */
/**/            if( iomnSendEPFN(hOutputJobID) == ERROR ) {
/**/                iomnDebugMessage(">> ioMN >> Error:iomnSendEPFN(%d) in iomnWaitingOutputJobCancel()\n",(I32)hOutputJobID);
/**/                return(ERROR);
/**/            }
/**/            /* V020.00_01 END   */
#else
            /* EPFNのみ送信し、parMNの書き換えは行わない    */

            memset(&strSendPrm, 0x00, sizeof(union ioMNparam));
            strSendPrm.sEpfnCtl.hJobid      = hOutputJobID;
            strSendPrm.sEpfnCtl.hJcbID      = ioMNCjst[hOutputJobID].msgparam.sCjstCtl.hJcbID;
            strSendPrm.sEpfnCtl.hTNo        = ioMNCjst[hOutputJobID].msgparam.sCjstCtl.hTNo;
            strSendPrm.sEpfnCtl.hAppliType  = ioMNCjst[hOutputJobID].msgparam.sCjstCtl.hAppliType;
            strSendPrm.sEpfnCtl.hProcessType= ioMNCjst[hOutputJobID].msgparam.sCjstCtl.hProcessType;

            if( iomnSendMessage(ioMNCjst[hOutputJobID].OldmsgQId,JOB_MSG_EPFN,0,&strSendPrm) == ERROR){
                iomnDebugMessage(">> ioMN >> Error:SEND EPFN in iomnWaitingOutputJobCancel()\n",0);
                return(ERROR);
            }
#endif
            hsts    = IOMSG_CAN_OK;
            iErrEng = IOMSG_NOT_USED;
        }
        if( iomnSendTjfn(hOutputJobID, hsts, (I32)IOMSG_NOT_USED, iErrEng)
#endif
                == ERROR ) {
            iomnDebugMessage(">> ioMN >> Error:iomnSendTjfn()in iomnOutputJobCancel()\n",0);
            return(ERROR);
        }
/* V045.00_01 START */
        /* メッセージクリア(すくなくともCJSUのクリアは必要) */
        iomnCleanJobManagementMsg(hOutputJobID);
/* V045.00_01 END   */
#if 0   /* V015.00_05   */
/**/    }
#endif
    return( OK );
}


/*
    出力ジョブの中断

    HISTORY:
        D1.2.1 T.Fujii 00/02/23 DM45/35 original
        D2.4   T.Fujii 00/06/15 DM45/35 prnEX自発中断対応
*/
STATUS iomnOutputJobSuspend( HI hInputJobID, HI hSusFactor )
{
    HI hOutputJobID;
    HI hJobType;

    iomnDebugMessage( ">> ioMN >> START:iomnOutputJobSuspend()\n", (I32)NULL );

    /* 出力ジョブのジョブIDを取得する */
    hOutputJobID = iomnSyncTblGetJobID( iomnJobInfo[hInputJobID].hIpTblID, COM_JB_SUB_OUTPUT );

    /* 出力ジョブが存在すれば中断処理を行う */
    if( hOutputJobID > IOMN_INPUTJOB_ID_MAX ) {
        iomnDebugMessage( ">> ioMN >> CHECK:Suspend Sync Job (JobID=%x)\n", (I32)hOutputJobID );
        jobJobtype( hOutputJobID, &hJobType );              /* 出力ジョブのジョブ種別を取得 */
        iomnPrnCjsu( hOutputJobID, hSusFactor, hJobType );  /* 出力ジョブの中断 */
    }
    else {
        iomnDebugMessage( ">> ioMN >> CHECK:No Sync Job for %x\n", (I32)hInputJobID );
        iomnDebugMessage( ">> ioMN >> END:iomnOutputJobSuspend()\n", (I32)NULL );
        return( ERROR );
    }

    iomnDebugMessage( ">> ioMN >> END:iomnOutputJobSuspend()\n", (I32)NULL );
    return( OK );
}


/* D3.0.1 START T.Fujii ソースコード整理 */
#if 0   /* OLD */
/*
NO.         
TITLE:      出力ジョブの再開処理
MODULE：    STATUS iomnOutputJobRestart( HI hOutputJobID )
INCLUDE:
PARAMETER:
            HI hOutputJobID      出力ジョブのJobID
RETURNS:    STATUS
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            出力ジョブを再開する
NOTES:
HISTORY:

    HISTORY:
        D1.2.4 T.Fujii 00/04/10 DM45/35 original
*/
STATUS iomnOutputJobRestart( HI hOutputJobID )
{
    if( iomnJobInfo[hOutputJobID].hIpTblID == IPLIB_INIT ) {
        /* 実行開始前の場合 */
        jobSetStatus( hOutputJobID, COM_JS_READY, 0 );
        iomnDebugMessage(">> ioMN >> CHECK:jobSetStatus(JobID=%d) -> READY\n",(I32)hOutputJobID);
    }
    else {
        /* 実行開始後(ジョブ状態が印刷待ちの場合) */
        jobSetStatus( hOutputJobID, COM_JS_PRT_WAITING, 0 ); /* 印刷開始待ちに戻す */
        iomnDebugMessage(">> ioMN >> CHECK:jobSetStatus(JobID=%d) -> PRT_WAITING\n",(I32)hOutputJobID);
    }

    iomnDebugMessage(">> ioMN >> CHECK:Bottom of iomnOutputJobReStart()\n",(I32)NULL );
    return( OK );
}
#endif
/* D3.0.1 END */


/*
    ページ出力終了通知受信処理

    HISTORY:
        D1.1 T.Fujii 99/11/15 DM45/35 original
        D1.6 T.Fujii 00/02/09 DM45/35 prnUI表示対応のため修正
        D2.8 T.Fujii 00/07/01 DM45/35 印刷系イベント送信処理修正

        MCK1.1  M.Karasawa 02/06/11 McKinley    新メッセージ対応
*/
STATUS iomnReceiveEPOT()
{
    HI hPrnID = -1;     /* 印刷種別 */

    #ifdef IOMN_DM_DEBUG_991201
    cmnPrintf_ioMN(0, "[ioMN]CHECK:in iomnReceiveEPOT()\n" );
    cmnPrintf_ioMN(0, "            sEpotCtl.hJobid=%d\n", ioMNsmyparam.sEpotCtl.hJobid );
    cmnPrintf_ioMN(0, "            sEpotCtl.hOutNum=%d\n", ioMNsmyparam.sEpotCtl.hOutNum );
    cmnPrintf_ioMN(0, "            sEpotCtl.hTaniInf=%d\n", ioMNsmyparam.sEpotCtl.hTaniInf );
    cmnPrintf_ioMN(0, "            sEpotCtl.iSeqNo=%d\n", ioMNsmyparam.sEpotCtl.iSeqNo );
    #endif  /* IOMN_DM_DEBUG_991201 */

    /* 印刷種別を取得してメッセージを送信 */
    if( (hPrnID = iomnPrnQueGetPrnType(ioMNrmyparam.sEpotCtl.hJobid)) >= 0 ) {
        #ifdef DEBUG
        cmnPrintf_ioMN(0,"[ioMN]CHECK:Try to send EPOT message (hPrnID=%x)\n",(I32)hPrnID);
        #endif

        /* prnUI or 管理タスクにEPOTを送信 */
        return( iomnSendPrintEventMsg(ioMNrmyparam.sEpotCtl.hJobid,hPrnID,
#if 0   /* MCK1.1 新メッセージ対応  */
/**/                                      IOMSG_EPOT_UI,IOMSG_EPOT_JOB,&ioMNrmyparam) );
#else
                                      JOB_MSG_EPOP,JOB_MSG_EPOP,&ioMNrmyparam) );
#endif

    }
    else {
        /* 印刷ジョブでなければ何もしない */
        return( OK );
    }
}


/*
    データ受信開始処理

    HISTORY:
        D1.2 T.Fujii 99/12/24 DM45/35 original
*/
STATUS iomnDataReceiveStart( HI hJobID )
{
    /* データ受信準備を行ない、開始する */
    if( iomnPrepareForDataReceive(hJobID) != ERROR ) {
        iomnDebugMessage(">> ioMN >> OK:Enable to Start Data Receive\n",0);
        /* データ受信開始の準備ができた場合 */
        /*  JOB開始メッセージ送信   */
        if( iomnSendJobStart(hJobID, IOMSG_IN_AUTO, IOMSG_JB_NEW, IOMSG_PT_FINISH) != ERROR ) {
            return(OK);     /* データ受信開始(成功) */
        }
        else {
            iomnDebugMessage(">> ioMN >> Error:iomnSendJobStart()\n",0);
            /* 必要ならここで終了する前にジョブ情報を開放 */
        }
    }
    else {
        iomnDebugMessage(">> ioMN >> Error:iomnPrepareForDataReceive()\n",0);
    }

    return( ERROR );
}


/*
    データ受信開始準備

    HISTORY:
        D1.2 T.Fujii 99/12/24 DM45/35 original
*/
LOCAL STATUS iomnPrepareForDataReceive( HI hJobID )
{
    HI hIpTblID = 0;        /* IPテーブルID */
	JOB_EXECDATA_TBL *psJobTable;

    /* サブジョブ種別設定(動作モードは設定しない) */
    if( iomnSetInputMode(hJobID,-1) == ERROR ) {
        cmnPrintf_ioMN(2, ">> >> Error:iomnSetDataInputMode()\n" );
        return( ERROR );
    }

    /* ipテーブルの取得とリンク情報の確立 */
    hIpTblID = ipLibEntryJob( hJobID );

    if( hIpTblID != ERROR ) {
        /* ipテーブルの確保ができた場合 */

        /* ジョブIDごとのジョブ情報エリアにipテーブルIDを保存 */
        iomnJobInfo[hJobID].hIpTblID = hIpTblID;
	/*For multiple Print job performance*/
	psJobTable = (JOB_EXECDATA_TBL*) ioMNrmyparam.sCjstCtl.pUiWorkPointer;
	iomnJobInfo[psJobTable->sCommon.hOutJobId].hIpTblID = hIpTblID;
	/*************************************/
        /* 逐次制御テーブルに入力ジョブを登録 */
        if( iomnSyncTblEntryJob( hIpTblID, COM_JB_SUB_INPUT, hJobID ) != ERROR ) {

            /* 逐次制御テーブルをIPテーブル使用中に更新 */
            if( iomnSyncTblSetStatus(hIpTblID, COM_JB_SUB_INPUT, IOMN_IPTBL_USING)
                != ERROR ) {
                return( OK );   /* 全て成功で終了(データ受信開始可能) */
            }

            /* 結局エラーになってしまった場合、後片付けが必要 */
            /* 逐次制御テーブルからジョブ削除 */
            iomnSyncTblDeleteJob( hIpTblID );
        }
        else {
            /* 逐次制御テーブルへのジョブ登録失敗時 */
            iomnDebugMessage(">>ERROR:iomnSyncTblEntryInputJob()\n",0);
        }

        /* 結局エラーになってしまった場合、後片付けが必要 */
        /* ipテーブル解放 */
        ipLibDeleteJob( hIpTblID );
    }
    else {
        /* ipテーブルの確保に失敗 */
        iomnDebugMessage(">>ERROR:ipLibEntryJob()\n",0);
    }

    return( ERROR );
}


/*
    データ受信ジョブ開始受け付け処理

    HISTORY:
        D1.2    T.Fujii     99/12/24 DM45/35 original
        D3.19.3 T.Fujii     00/09/18 DM45/35 ADDS送信処理修正

        MCK1.1  M.Karasawa 02/06/11 McKinley    新メッセージ対応
*/
STATUS iomnDataRcvJobStartAck( HI hJobID )
{
    /* ADDS送信処理 */
/* D3.19.3 START T.Fujii 中断／再開を考慮して修正 */
#if 0   /* OLD */
/**/    if( ioMNstatus.hDssDataTrans == IOMN_READY ) {
/**/        /* 念のためAck未送信の場合のみ */
#endif
/* D3.19.3 END */
        /* ADDSメッセージ送信  */
#if 0   /* MCK1.1 ADDS -> AJST にメッセージ統一 */
/**/        if( iomnSendAdds(hJobID, TRUE) == ERROR ) {
#else
        /* 受信したメッセージをそのまま送信     */
        memcpy(&ioMNsmyparam, &ioMNrmyparam, sizeof(union ioMNparam));
        if( iomnSendMessage(ioMNCjst[hJobID].OldmsgQId,JOB_MSG_AJST,0,&ioMNsmyparam) == ERROR ) {
#endif
            iomnDebugMessage(">> ioMN >> Error:iomnSendAdds() in iomnDataRcvJobStartAck()\n",0);
            return(ERROR);
        }
        else {
            /* メッセージ送信に成功した場合 */
            ioMNstatus.hDssDataTrans = IOMN_ON;     /* データ転送中を設定 */
            iomnDebugMessage(">> ioMN >> SET:DSI Data Transfer Flag -> ON\n",0);
        }
/* D3.19.3 START T.Fujii 中断／再開を考慮して修正 */
#if 0   /* OLD */
/**/    }
#endif
/* D3.19.3 END */

    return(OK);
}


/*
    データ受信完了処理

    HISTORY:
        D1.2    T.Fujii     99/12/24 DM45/35 original
        D3.25   T.Fujii     00/09/28 DM45/35 データ転送中フラグ設定修正

        MCK1.1  M.Karasawa  02/06/11    McKinley    新メッセージ対応
    V018.00_01  M.Karasawa  03/04/09    McKinley    メッセージ変更対応
*/
STATUS iomnDataRcvJobFinSts( HI hJobID, HI hJobFinSts, I32 iErrApl, I32 iErrEngine, HI hTjfnststype )
{
    I32 iSts = FALSE;  /* ADDS送信用(初期値はデータ転送開始不可) */

    /* Ack未送信の場合、ADDSを送信する */
    if( ioMNstatus.hDssDataTrans == IOMN_READY ) {
        if( hTjfnststype == IOMN_FIN_NORMAL || hTjfnststype == IOMN_CAN_NORMAL ) {
            /* 正常終了またはキャンセル処理正常終了の場合、TRUE状態を設定 */
            iSts = TRUE;
        }
#if 0   /* V009.00  */
/**/        /* ADDSの送信 */
/**/        if( iomnSendAdds(hJobID, iSts) == ERROR ) {
/**/            /* エラーメッセージの表示 */
/**/            iomnDebugMessage(">> ioMN >> Error:iomnSendAdds() in iomnDataRcvJobFinSts()\n",0);
/**/            /* return( ERROR );     処理は継続 */
/**/        }
#else
        memset(&ioMNsmyparam, 0x00, sizeof(union ioMNparam));
        ioMNsmyparam.sAjstCtl.hJobid       = hJobID;
        ioMNsmyparam.sAjstCtl.hInUpDown    = IOMSG_NOT_USED;        /* 何が必要か不明   */
        ioMNsmyparam.sAjstCtl.hOutTiming   = IOMSG_NOT_USED;        /* 何が必要か不明   */
        ioMNsmyparam.sAjstCtl.hWarSts      = IOMSG_NOT_USED;        /* 何が必要か不明   */
        ioMNsmyparam.sAjstCtl.iWarErrSys   = IOMSG_NOT_USED;        /* 何が必要か不明   */
        ioMNsmyparam.sAjstCtl.iWarErrEng   = IOMSG_NOT_USED;        /* 何が必要か不明   */
        ioMNsmyparam.sAjstCtl.iErrStatus   = IOMSG_NOT_USED;        /* 何が必要か不明   */
        ioMNsmyparam.sAjstCtl.hTNo         = ioMNCjst[hJobID].msgparam.sCjstCtl.hTNo;
        ioMNsmyparam.sAjstCtl.hProcessType = ioMNCjst[hJobID].msgparam.sCjstCtl.hProcessType;
        /* V018.00_01 START */
        ioMNsmyparam.sAjstCtl.hJcbID       = ioMNCjst[hJobID].msgparam.sCjstCtl.hJcbID;
        ioMNsmyparam.sAjstCtl.hAppliType   = ioMNCjst[hJobID].msgparam.sCjstCtl.hAppliType;
        /* V018.00_01 END   */
        if( iomnSendMessage(ioMNCjst[hJobID].OldmsgQId,JOB_MSG_AJST,0,&ioMNsmyparam) == ERROR ) {
            iomnDebugMessage(">> ioMN >> Error:iomnSendMessage() in iomnDataRcvJobFinSts()\n",0);
        }
#endif
    }

    /* 入力ジョブの解放(parMNの設定はOFF) */
#if 1 /*Paresh: changed this code to prevent deleting the ipLib table when in job finishes*/
		/*reverted the changes as this change is not require any more but  is affecting when, 
		TJFN of RipWrap is received after the TJFN of Print*/
    if( iomnInputJobFree(hJobID, (HI)IOMN_OFF) == ERROR ) {
        iomnDebugMessage(">> ioMN >> Error:iomnInputJobFree() in iomnDataRcvJobFinSts()\n",0);
        /* return( ERROR );     処理は継続させる */
    }
#else
	if( iomnSyncTblSetStatus(iomnJobInfo[hJobID].hIpTblID, COM_JB_SUB_INPUT, (HI)IOMN_IPTBL_NOTUSED)
        == ERROR ) {
        iomnDebugMessage(">> ioMN >> Error:iomnSyncTblSetStatus() in iomnInputJobFree()\n",0);
        /* return( ERROR );     処理は継続させる */
        return( ERROR );
    }

    /* parMN再設定およびページ確定メッセージ送信 */
    if( iomnSyncTblCpyParMN(iomnJobInfo[hJobID].hIpTblID, IOMN_OFF) == ERROR ) {
        iomnDebugMessage(">> ioMN >> Error:iomnSyncTblCpyParMN() in iomnInputJobFree()\n",0);
        /* return( ERROR );     処理は継続させる */
        return( ERROR );
    }
#endif
    /* ジョブ状態をオペレーティング中に戻す */
    if( jobSetStatus(hJobID, COM_JS_OPERATING,0) == ERROR ) {
        iomnDebugMessage( ">> ioMN >> Error:jobSetStatus(hJobID=%x)\n",(I32)hJobID );
        /* return(ERROR);   処理は継続させる */
    }

    /* データ受信要求元タスクにTDDSを送信 */
#if 0   /* MCK1.1 新メッセージ対応  *//* 暫定   */
/**/    if( iomnSendTdds( hJobID, hJobFinSts, iErrApl, iErrEngine) == ERROR ) {
#else
    /* とりあえず、そのまま送信     */
    memcpy(&ioMNsmyparam, &ioMNrmyparam, sizeof(union ioMNparam));
    if( iomnSendMessage(ioMNCjst[hJobID].OldmsgQId,JOB_MSG_TJFN,0,&ioMNsmyparam) == ERROR ) {
#endif
        iomnDebugMessage(">> ioMN >> Error:iomnSendTdds() in iomnDataRcvJobFinSts()\n",0);
        return( ERROR );
    }

    /* データ転送フラグ解除 */
/* D3.25 START T.Fujii フラグ修正 */
#if 1   /* OLD */ /*Paresh: changed this code to link the output job with input job through Sync Table*/
    ioMNstatus.hGdiDataTrans = IOMN_OFF;
/*#else   *//* D3.25 MID    NEW */																						/* SM-001 */
    ioMNstatus.hDssDataTrans = IOMN_OFF;
#endif
/* D3.25 END */
    iomnDebugMessage(">> ioMN >> Clear:GDI Data Flag -> IOMN_OFF\n",0);

    /* メッセージテーブルの初期化 */
    iomnCleanJobManagementMsg(hJobID);

    return( OK );
}


/*
    ジョブモード(動作モード、サブジョブ種別)設定

    HISTORY:
        D1.2 T.Fujii 99/12/24 DM45/35 original
*/
STATUS iomnSetInputMode( HI hJobID, HI hIOMode )
{
    /* 動作モードを設定する */
    if( hIOMode >= 0 ) {
        if( parMNSetIOMode(hJobID, hIOMode) == ERROR ) {
            #ifdef DEBUG
            cmnPrintf_ioMN(2, ">> ioMN >> ERROR:parMNSetIOMode() in iomnSetInputMode\n" );
            #endif  /* DEBUG */
            return( ERROR );
        }
    }

    /* サブジョブモードを入力ジョブに設定 */
    if( parMNSetSubJobmode( hJobID, COM_JB_SUB_INPUT ) == ERROR ) {
        #ifdef DEBUG
        cmnPrintf_ioMN(2, ">> ioMN >> ERROR:parMNSetSubJobmode() in iomnSetInputMode\n" );
        #endif  /* DEBUG */
        return( ERROR );
    }

    return( OK );
}


/*
    入力ジョブの解放

    HISTORY:
        D1.2 T.Fujii 99/12/24 DM45/35 original
        D2.1 T.Fujii 00/06/06 DM45/35 V004.60不具合対応
                                      parMN再設定処理修正
                                      ページ確定メッセージ送信対応
        D2.4.4 T.Fujii  00/06/20 DM45/35 V004.70不具合対応
                                         ジョブ終了後のJobIDクリア処理対応
*/
STATUS iomnInputJobFree( HI hJobID, HI hFlgCopyParMN )
{
    STATUS sts = OK;    /* 戻り値(最初はOKを設定) */
    /* EBX_DCR_59275 changes START */
    if(hJobID <= 1){
        HI hOutputJobId = JOB_UNDEF_JOBID;
        parMNSetInputOutputStatus(hJobID, hOutputJobId);
    }
    /* EBX_DCR_59275 changes END */
    /* 入力処理と出力処理のリンクを解除する */
    /* 入力中をクリア */
    if( iomnSyncTblSetStatus(iomnJobInfo[hJobID].hIpTblID, COM_JB_SUB_INPUT, (HI)IOMN_IPTBL_NOTUSED)
        == ERROR ) {
        iomnDebugMessage(">> ioMN >> Error:iomnSyncTblSetStatus() in iomnInputJobFree()\n",0);
        /* return( ERROR );     処理は継続させる */
        sts = ERROR;
    }

    /* parMN再設定およびページ確定メッセージ送信 */
    if( iomnSyncTblCpyParMN(iomnJobInfo[hJobID].hIpTblID,hFlgCopyParMN) == ERROR ) {
        iomnDebugMessage(">> ioMN >> Error:iomnSyncTblCpyParMN() in iomnInputJobFree()\n",0);
        /* return( ERROR );     処理は継続させる */
        sts = ERROR;
    }

    /* 逐次制御テーブルの開放を試みる */

    iomnClearSyncLink( hJobID, COM_JB_SUB_INPUT );

    return( sts );
}


/*
(C) Copyright TOSHIBA TEC Corporation 2000. ALL Right Reserved
NO.
TITLE:      ＩＰテーブル、逐次制御テーブルの解放
MODULE：    STATUS  iomnStart_IP_SIS_Cls(HI hJobid )
INCLUDE:
PARAMETER:
            HI      hJobid;             JOBID
RETURNS:    I32 4B
            OK:     正常
OUTLINE:
            ＩＰテーブルと逐次制御テーブルの解放を行う
NOTES:
HISTORY:
        D1.4    Y.Shimada       '00/ 1/25 ORG
                フォームストレージ変更：ＩｐＬｉｂ／逐次制御の組み込み
*/
STATUS iomnStart_IP_SIS_Cls(HI hJobid )
{
    /* 逐次制御テーブルの開放 */
    if( iomnStart_SIS_Cls( hJobid ) == OK ){
        /* テーブル開放に成功すればIPテーブルも開放 */
        iomnStart_IP_Cls( hJobid );
    }else{
        iomnDebugMessage(">>>Error:iomnSyncTblDeleteJob() in iomnStart_IP_SIS_Cls()\n",0);
    }
    return( OK );
}


/*
(C) Copyright TOSHIBA TEC Corporation 2000. ALL Right Reserved
NO.
TITLE:      ＩＰテーブルの解放
MODULE：    STATUS  iomnStart_IP_Cls(HI hJobid )
INCLUDE:
PARAMETER:
            HI      hJobid;             JOBID
RETURNS:    I32 4B
            OK:     正常
OUTLINE:
            ＩＰテーブルの解放を行う
NOTES:
HISTORY:
        D1.4    Y.Shimada       '00/ 1/25 ORG
                フォームストレージ変更：ＩｐＬｉｂ／逐次制御の組み込み
*/
STATUS iomnStart_IP_Cls(HI hJobid )
{
    /* ＩＰテーブルの開放 */
    ipLibDeleteJob( iomnJobInfo[hJobid].hIpTblID );
    iomnJobInfo[hJobid].hIpTblID = IPLIB_INIT;
    return( OK );
}


/*
(C) Copyright TOSHIBA TEC Corporation 2000. ALL Right Reserved
NO.
TITLE:      逐次制御テーブルの解放
MODULE：    STATUS  iomnStart_SIS_Cls(HI hJobid )
INCLUDE:
PARAMETER:
            HI      hJobid;             JOBID
RETURNS:    I32 4B
            OK:     正常
OUTLINE:
            逐次制御テーブルの解放を行う
NOTES:
HISTORY:
        D1.4    Y.Shimada       '00/ 1/25 ORG
                フォームストレージ変更：ＩｐＬｉｂ／逐次制御の組み込み
*/
STATUS iomnStart_SIS_Cls(HI hJobid )
{
    /* 入力中をクリア */
    if( iomnSyncTblSetStatus(iomnJobInfo[hJobid].hIpTblID, COM_JB_SUB_INPUT, (HI)IOMN_IPTBL_NOTUSED) == ERROR ){
        iomnDebugMessage(">>>Error:iomnSyncTblSetStatus() in iomnStart_SIS_Cls()\n",0);
    }
    /* 逐次制御テーブルの開放 */
    return( iomnSyncTblDeleteJob(iomnJobInfo[hJobid].hIpTblID) );
}


/*
(C) Copyright TOSHIBA TEC Corporation 2000. ALL Right Reserved
NO.
TITLE:      ＨＤＤの保存値をｐａｒＭＮへ取り出し
MODULE：    STATUS iomnHddFileToParMN(HI hJobID ,PAR_TBL_COM_FNC *strParTbl ,B *aFileNamebuf)
INCLUDE:
PARAMETER:
            HI      hJobID;             JOBID
            PAR_TBL_COM_FNC *strParTbl; パラメータテーブル領域
            B       *aFileNamebuf;      ＨＤＤファイル名

RETURNS:    I32 4B
            OK:     正常
            ERROR:  異常
OUTLINE:
            ＨＤＤの保存値をｐａｒＭＮへ取り出す
NOTES:
HISTORY:
        D1.4    Y.Shimada       '00/ 1/25 ORG
                フォームストレージ変更：ＩｐＬｉｂ／逐次制御の組み込み
*/

STATUS iomnHddFileToParMN( HI hJobID , PAR_TBL_COM_FNC *strParTbl , B *aFileNamebuf)
{
STATUS  Status;
I32     iFlID;                  /* ファイルＩＤ(flLib)  */

    Status = ERROR;
    /* システムファイルのオープン */
    if((iFlID = flFileOpen( iTaskIdIoMN, hJobID , aFileNamebuf )) != ERROR ){
        /* parMN全体の取り出し */
        if(( Status = flParMNTblGet( iTaskIdIoMN, (HI)iFlID, aFileNamebuf, strParTbl )) != ERROR ){
            /* parMN全体の取得 */
            if( parMNSetFncParam( hJobID , strParTbl ) != ERROR ) {
                iomnDebugMessage( ">>>OK:parMNSetFncParam()()\n", 0 );
                Status = OK;
            }else{
                iomnDebugMessage( ">>>Error:parMNSetFncParam() in iomnHddFileToParMN()\n", 0 );
            }
        }else{
            iomnDebugMessage( ">>>Error:flParMNTblGet() in iomnHddFileToParMN()\n", 0 );
        }
        /* ファイルクローズ */
        flFileClose( iTaskIdIoMN, iFlID );
    }else{
        iomnDebugMessage( ">>>Error:flFileOpen() in iomnHddFileToParMN()\n", 0 );
    }
    return( Status );
}


/*
(C) Copyright TOSHIBA TEC Corporation 2000. ALL Right Reserved
NO.
TITLE:      ファイル名変換（ＨＤＤ）
MODULE：    STATUS iomnFormFileNameChg(HI hFormJobtype , HI hImageID , B *aFileNamebuf)
INCLUDE:
PARAMETER:
            HI      hJobID;             JOBID
            PAR_TBL_COM_FNC *strParTbl; パラメータテーブル領域
            B       *aFileNamebuf;      ＨＤＤファイル名

RETURNS:    I32 4B
            OK:     正常
            ERROR:  異常
OUTLINE:
            ＨＤＤのファイル名を取得する（フォーム関連）
NOTES:
HISTORY:
        D1.4    Y.Shimada       '00/ 1/25 ORG
                フォームストレージ変更：ＩｐＬｉｂ／逐次制御の組み込み
*/
STATUS iomnFormFileNameChg(HI hFormJobtype , HI hImageID , B *aFileNamebuf)
{

    switch( hFormJobtype ){
        case COM_JB_IVSAVE_PPC:
        case COM_JB_IVPRM_PPC:
            hFormJobtype = SP_EXCOPY;
            break;
        case COM_JB_IVSAVE_FAX:
        case COM_JB_IVPRM_FAX:
            hFormJobtype = SP_EXFAX;
            break;
        default:
            iomnDebugMessage(">>>JOB Type Error->%x\n",(I32)hFormJobtype);
            hFormJobtype = SP_EXCOPY;
            break;
    }

    /*  ファイル名取得  */
    if( spFormFileConvert(hFormJobtype, hImageID, aFileNamebuf) == ERROR ){
        iomnDebugMessage(">>>Error:spFormFileConvert()\n",0);
        printErrno(errno);
        return(ERROR);
    }

    return( OK );
}


/*
(C) Copyright TOSHIBA TEC Corporation 2000. ALL Right Reserved
NO.
TITLE:      新規ジョブ準備
MODULE：    iomnPrepareForNewJob( HI hJobID, HI hJobType, HI hContinuity )
INCLUDE:
PARAMETER:
            HI hJobID       ジョブID
            HI hJobType     ジョブ種別
            HI hContinuity  継続or新規

RETURNS:    I32 4B
            OK:     正常
            ERROR:  異常
OUTLINE:
            新規ジョブの場合、準備処理を行なう
NOTES:
HISTORY:
        D1.2.1  T.Fujii     00/02/23    original
        D1.7.9  J.Ootani    00/04/08    DM45/35 FAXリストレポート対応
        D2.9    M.Fukuda    00/07/04    DM45/35 FAXリストレポート対応
        D4.7    T.Fujii     00/11/18    DM45/35 前ジョブがエラー終了するとジャム解除で印刷開始
                                                しなくなる不具合修正
        D4.7.2  T.Fujii     00/11/21    DM45/35 プリンタW-UP中の2つ目のFAX/DSS入力ジョブでロック
                                                する不具合対応
    V025.00_01  M.Karasawa  03/08/20    McKinley    Hudson用リスト仮対応（McKでは使用しないジョブ種別なのでMcKに影響なし）
*/
STATUS iomnPrepareForNewJob( HI hJobID, HI hJobType, HI hContinuity )
{
    HI  hPmJobId;   /* PMJOBID */

    switch( hContinuity ) {
        case IOMSG_JB_NEW_TRK:  /* 後追い逐次で新規ジョブの場合 */
            /* 逐次制御テーブルに出力ジョブを登録して関連付ける */
            if( iomnSyncTblLinkOutputJob(hJobID,hJobType) == ERROR ) {
                cmnPrintf_ioMN(2,">> ioMN >> ERROR:iomnLinkIOJob() in iomnPrepareForPrnJob()\n");
                return ERROR;
            }
            /* このまま以下の処理を行なう */

        case IOMSG_JB_NEW:      /* 新規の場合 */
            /* 通常複写のPMJOBIDを取得 */
            if( jobPmJobid(hioMNFrontJobID, &hPmJobId) == ERROR ) {
                cmnPrintf_ioMN(2,">> ioMN >> ERROR:jobPmJobid() in iomnPrnJobStart()\n");
                printErrno( errno );
                return ERROR;
            }

            /* 該当JOB管理テーブルにPMJOBIDを設定 */
            if( jobSetPmJobid(hJobID, hPmJobId) == ERROR ) {
                cmnPrintf_ioMN(2,">> ioMN >> ERROR:jobSetPmJobid()-iomnPrnJobStart()\n");
                printErrno( errno );
                return ERROR;
            }

            /* サブジョブ種別を設定 */
            if(hJobType == COM_JB_LIST_PRT_FAX ||
               hJobType == COM_JB_LIST_RESULT_SEND)
            /* リストレポート印刷または送信の場合は入力に設定 */
            { /* リストレポート印刷の場合は入力に設定 */
#if 0   /* V025.00_01 START */
/**/                if( parMNSetSubJobmode(hJobID, COM_JB_SUB_INPUT) == ERROR ) {
/**/                    cmnPrintf_ioMN(2, ">> ioMN >> ERROR: parMNSetSubJobmode() in iomnPrnJobStart()\n");
/**/                    return( ERROR );
/**/                }
/**/                iomnDebugMessage( ">>>OK:SubJobmode = SUB_INPUT(JobID=%d)\n", hJobID);
#endif  /* V025.00_01 END   */
            }
            else
            { /* その他の場合は出力に設定 */
                if( parMNSetSubJobmode(hJobID, COM_JB_SUB_OUTPUT) == ERROR ) {
                    cmnPrintf_ioMN(2, ">> ioMN >> ERROR: parMNSetSubJobmode() in iomnPrnJobStart()\n");
                    return( ERROR );
                }
                iomnDebugMessage( ">>>OK:SubJobmode = SUB_OUTPUT(JobID=%d)\n", hJobID);
            }

            /* D4.7 START T.Fujii メッセージ受信情報クリア処理追加 */
            /* メッセージ受信情報の一部をクリアする */
            memset( &ioMNAjst[hJobID], 0x00, sizeof(struct msgstore) );
/* D4.7.2 START T.Fujii メッセージ受信情報クリア処理追加 */
#if 0   /* OLD */
            memset( &ioMNTjfn[hJobID], 0x00, sizeof(struct msgstore) );
#endif
/* D4.7.2 END */
            memset( &ioMNTjed[hJobID], 0x00, sizeof(struct msgstore) );
            memset( &ioMNTjsu[hJobID], 0x00, sizeof(struct msgstore) );
            /* D4.7 END */
            break;

        default:
            /* 上記以外は特に処理をせず、OKを返すことになる */
            break;
    }

    return( OK );
}

/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.         7.7.6
TITLE:      リストレポート作成完了ステータス処理
MODULE：    STATUS  iomnListMakeJobFinSts(HI hJobID, HI hJobFinSts,
                                      I32 iErrApl, I32 iErrEngine,
                                      HI hTjfnststype, HI hJobtype)
INCLUDE:    ioMNPar.h
PARAMETER:
            HI  hJobID;         JOBID
            HI  hJobFinSts;     JOB結果
            I32 iErrApl;        アプリ系エラー
            I32 iErrEngine      エンジン系エラー
            HI  hTjfnststype;   JOB完了タイプ
RETURNS:    I32 4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            リストレポート作成完了ステータス受信時に、サブジョブ種別
            を出力に切替えて再度ジョブ開始指示を送信する。
NOTES:
HISTORY:
    D1.7.9  J.Ootani    00/04/08    DM45/35 FAXリストレポート対応のため
                                    新規作成
    D2.9    M.Fukuda    00/07/04    DM45/35 FAXリストレポート対応
    D2.9.1  M.Fukuda    00/07/12    DM45/35 FAXリストレポート対応
    D2.9.2  M.Fukuda    00/07/12    DM45/35 FAXリストレポート対応
    D2.9.3  M.Fukuda    00/07/19    DM45/35 FAXリストレポート対応（逐次制御テーブルクリア追加）
    D3.10   T.Fujii     00/08/24    DM45/35 FAXリスト印刷処理修正
    D3.13   T.Fujii     00/09/02    DM45/35 コーディングミス等修正
    D5.0    T.Fujii     00/12/12    DM45/35 CDJB多重動作対応

    MCK1.1  M.Karasawa 02/06/11 McKinley    新メッセージ対応
*/
#if 0   /* MCK1.1 新メッセージ対応  引数追加    */
/**/STATUS  iomnListMakeJobFinSts(HI hJobID, HI hJobFinSts, I32 iErrApl, I32 iErrEngine, HI hTjfnststype, HI hJobtype)
#else
STATUS  iomnListMakeJobFinSts(HI hJobID, HI hJobFinSts, I32 iErrApl, I32 iErrEngine, HI hTjfnststype, HI hJobtype, HI hProcessNo)
#endif
{
    STATUS      Status;              /*  マシンリカバリ結果  */
    I32         imsgid;              /*  メッセージＩＤ  */
    HI          hContinuity;         /*  ＪＯＢ継続 */
    HI          hPrtType;            /*  全印刷or継続印刷 */

    iomnDebugMessage(">>>>TOP of iomnListMakeJobFinSts()<<<<\n",0);
    if( hJobtype == COM_JB_LIST_RESULT_SEND ){
        iomnPrnQueJobDelete( hJobID , IOMN_SENDQ_ENTRY_ID ); /* 送信ジョブの削除 */
        /* 印刷中状態をクリア */
        if( iomnSyncTblSetStatus(iomnJobInfo[hJobID].hIpTblID, COM_JB_SUB_OUTPUT, (HI)IOMN_IPTBL_NOTUSED)
            == ERROR ) {
            iomnDebugMessage(">>>Error:iomnSyncTblSetStatus() in iomnDssPrnJobFinSts()\n",0);
            /* return( ERROR );     処理は継続させる */
        }
        /* 逐次制御テーブルをクリアする */
        if( iomnClearSyncLink(hJobID, COM_JB_SUB_OUTPUT) == ERROR ) {
            iomnDebugMessage("[ioMN]ERROR:iomnClearSyncLink(%d) in iomnSendJobFinSts()\n",(I32)hJobID);
        }
    }
    /*  JOB完了タイプごと処理   */
    switch(hTjfnststype){
      /* リストレポート作成の通常のＪＯＢ完了と中止のＪＯＢ完了をJOB完了タイプで区別せずに、ioMN側で */
      /* ＪＯＢ中止の要求を送信したか否かにより区別するように修正                                    */
      case    IOMN_CAN_NORMAL:
      case    IOMN_FIN_NORMAL:
        /* FAXリストレポート印刷 */
        if( hJobtype == COM_JB_LIST_PRT_FAX ){
          /* ＪＯＢ中止開始済みの時 */
          if( ioMNstatus.hSendCjcntart == IOMN_ON ){
            ioMNstatus.hSendCjcntart = IOMN_OFF;
            if(ERROR == (Status = iomnDssPrnJobFinSts(hJobID,
                                  hJobFinSts,iErrApl,iErrEngine,
#if 0   /* MCK1.1 新メッセージ対応  引数追加    */
                                  hTjfnststype,hJobtype)))
#else
                                  hTjfnststype,hJobtype,hProcessNo)))
#endif

            {
                cmnPrintf_ioMN(2,">>>Error:iomnDssPrnJobFinSts()\n");
                return(ERROR);
            }
          }
          /* ＪＯＢ中止開始なしの時 */
          else {
            /* サブジョブ種別を出力に切替え再度ジョブ開始指示送信 */
            /* サブジョブ種別を出力に切替え */
            if( parMNSetSubJobmode(hJobID, COM_JB_SUB_OUTPUT) == ERROR ) {
                cmnPrintf_ioMN(2, ">> ioMN >> ERROR: parMNSetSubJobmode()\n");
                return( ERROR );
            }
            iomnDebugMessage( ">>>OK:SubJobmode = SUB_OUTPUT(JobID=%d)\n", hJobID);

            /* D3.10 START T.Fujii FAXリスト印刷処理修正 */
            /* メッセージ受信情報の一部をクリアする */
            memset(&ioMNTjfn[hJobID], 0x00, sizeof(struct msgstore));
            memset(&ioMNTjed[hJobID], 0x00, sizeof(struct msgstore));
            memset(&ioMNTjsu[hJobID], 0x00, sizeof(struct msgstore));
            /* D3.10 END */

            /* 再度ジョブ開始指示送信 */
            hContinuity = ioMNCjst[hJobID].msgparam.sCjstCtl.hContinue;
/* D3.13 START T.Fujii コーディングミス等修正 */
#if 0   /* OLD */
            hPrtType    = ioMNCjst[hJobID].msgparam.sCjstCtl.hContinue;
#else   /* D3.13 MID    NEW */
            hPrtType    = ioMNCjst[hJobID].msgparam.sCjstCtl.hPrtType;
#endif
/* D3.13 END */
            if( ERROR == iomnPrintJobStart(hJobID, hContinuity,
                                             hPrtType,COM_JS_PRT_WAITING ))
            {
                cmnPrintf_ioMN(2,">>>Error:iomnPrintJobStart()\n");
                return(ERROR);
            }
          }
        }
        /* FAXリストレポート送信 */
        else if( hJobtype == COM_JB_LIST_RESULT_SEND ){
            /*  MessageIDの取得 */
#if 0   /* MCK1.1 新メッセージ対応  */
/**/            if(ioMNCjst[hJobID].iOldMsgId == IOMSG_CJST_UI)
/**/                imsgid = IOMSG_TJFN_UI;
/**/            else if(ioMNCjst[hJobID].iOldMsgId == IOMSG_CJST_DIG)
/**/                imsgid = IOMSG_TJFN_DIG;
/**/            else
/**/                imsgid = IOMSG_TJFN_JOB;
#else
            imsgid = JOB_MSG_TJFN;
#endif

            /*  JOB完了メッセージ送信   */
            if(ERROR == (Status = iomnSendJobFinishSts(
                           ioMNCjst[hJobID].OldmsgQId, imsgid,
#if 0   /* MCK1.1 新メッセージ対応  引数追加    */
                           hJobID, hJobFinSts, iErrApl, iErrEngine)))
#else
                           hJobID, hJobFinSts, iErrApl, iErrEngine, hProcessNo)))
#endif
            {
                cmnPrintf_ioMN(2,">>>Error:iomnSendJobFinishSts()\n");
                return(ERROR);
            }
        }
        break;
      case    IOMN_FIN_ERR_APL:
      case    IOMN_FIN_ERR_ENG:
      case    IOMN_CAN_ERR_APL:
      case    IOMN_CAN_ERR_ENG:
      case    IOMN_FIN_ERR_APL_ENG:
      case    IOMN_CAN_ERR_APL_ENG:
        /*  ERROR発生中か否かをチェック */
        if(hJobtype == COM_JB_LIST_PRT_FAX){
            iomnPrnQueJobDelete( hJobID , hIoMNPrnQueEntryID );  /* 印刷ジョブの削除 */
            /* 印刷中状態をクリア */
            if( iomnSyncTblSetStatus(iomnJobInfo[hJobID].hIpTblID, COM_JB_SUB_OUTPUT, (HI)IOMN_IPTBL_NOTUSED)
                == ERROR ) {
                iomnDebugMessage(">>>Error:iomnSyncTblSetStatus() in iomnDssPrnJobFinSts()\n",0);
                /* return( ERROR );     処理は継続させる */
            }
            /* 逐次制御テーブルをクリアする */
            if( iomnClearSyncLink(hJobID, COM_JB_SUB_OUTPUT) == ERROR ) {
                iomnDebugMessage("[ioMN]ERROR:iomnClearSyncLink(%d) in iomnSendJobFinSts()\n",(I32)hJobID);
            }
        }
        /*  MessageIDの取得 */
#if 0   /* MCK1.1 新メッセージ対応  */
/**/        if(ioMNCjst[hJobID].iOldMsgId == IOMSG_CJST_UI)
/**/            imsgid = IOMSG_TJFN_UI;
/**/        else if(ioMNCjst[hJobID].iOldMsgId == IOMSG_CJST_DIG)
/**/            imsgid = IOMSG_TJFN_DIG;
/**/        else
/**/            imsgid = IOMSG_TJFN_JOB;
#else
        imsgid = JOB_MSG_TJFN;
#endif
        /* FAXリストレポート印刷 */
        if( hJobtype == COM_JB_LIST_PRT_FAX ){
          /* ＪＯＢ中止開始済みの時 */
          if( ioMNstatus.hSendCjcntart == IOMN_ON ){
            ioMNstatus.hSendCjcntart = IOMN_OFF;
          }
        }
        /*  JOB完了メッセージ送信   */
        if(ERROR == (Status = iomnSendJobFinishSts(
                       ioMNCjst[hJobID].OldmsgQId, imsgid,
#if 0   /* MCK1.1 新メッセージ対応  引数追加    */
                       hJobID, hJobFinSts, iErrApl, iErrEngine)))
#else
                       hJobID, hJobFinSts, iErrApl, iErrEngine, hProcessNo)))
#endif
        {
            cmnPrintf_ioMN(2,">>>Error:iomnSendJobFinishSts()\n");
            return(ERROR);
        }
        break;
      default:
        cmnPrintf_ioMN(2,">>>TJFN Undefined JOB Type!\n");
        break;
    }
    /* FAXリストレポート送信 */
    if( hJobtype == COM_JB_LIST_RESULT_SEND ){
        /*  JOB削除指示受信に伴うJOB完了の場合  */
/* D5.0 START T.Fujii CDJB多重動作対応 */
#if 0   /* OLD */
        if(ioMNCdjb.OldmsgQId != NULL && ioMNCdjb.msgparam.sCdjbCtl.hJobid == hJobID){
#else   /* D5.0 NEW */
        if(ioMNCdjb[hJobID].OldmsgQId != NULL){
#endif
/* D5.0 END */
            /*  JOB削除ステータス送信   */
            if(ERROR == (Status = iomnSendTDJB(hJobID, TRUE, IOMSG_NOT_USED))){
                iomnDebugMessage("ERROR:iomnSendTDJB()\n",0);
                return(ERROR);
            }
            iomnDebugMessage(">>>OK:iomnSendTDJB()\n",0);
        }
    }
    iomnDebugMessage(">>>>Bottom of iomnListMakeJobFinSts()<<<<\n",0);

    return(OK);
}

/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.
TITLE:      ダイレクト送信接続完了イベント処理
MODULE：    STATUS  iomnRecieveEC44()
INCLUDE:    ioMNPar.h
PARAMETER:
RETURNS:    I32 4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            ダイレクト送信接続完了イベント受信時に、ジョブ開始指示を送信する。
NOTES:
HISTORY:
    D1.7.10 J.Ootani    00/04/10    DM45/35 FAXリストレポート対応のため
                                    新規作成
    D1.7.13  M.Fukuda    00/05/12    DM45/35 FAXダイレクト送信不具合対応

    MCK1.3  M.Karasawa  02/07/03    McKinley    Mckinley用に処理見なおし（ダイレクト送信）
*/
STATUS  iomnRecieveEC44()
{
#if 0   /* MCK1.3 START JOB開始処理はダイレクト送信ジョブ開始指示（CJST）受信時に行なう */
/**/    STATUS      Status;
/**/    HI          hTrigger;        /*  ＪＯＢ開始要因 */
/**/    HI          hContinue;       /*  ＪＯＢ継続 */
/**/    HI          hJobID;
/**/    HI          hJobType;
/**/    I32         imsgid = IOMSG_TJFN_UI;
/**/    I32         imsgidTjed = IOMSG_TJED_UI;     /*  ＪＯＢ終了メッセージＩＤ    */
/**/
/**/    iomnDebugMessage(">>>>TOP of iomnRecieveEC44()<<<<\n",0);
/**/
/**/    hJobID = ioMNrmyparam.hParam[0];
/**/
/**/    /*  JOB種類取得 */
/**/    if(ERROR == (Status = jobJobtype(hJobID, &hJobType))){
/**/        cmnPrintf_ioMN(2,">>>Error:jobJobtype() in iomnRecieveEC44())\n");
/**/        printErrno(errno);
/**/        return(ERROR);
/**/    }
/**/
/**/    /* サブジョブ種別の設定  */
/**/    if( parMNSetSubJobmode(hJobID, COM_JB_SUB_INPUT) == ERROR ) {
/**/        cmnPrintf_ioMN(2, ">> ioMN >> ERROR: parMNSetSubJobmode()\n");
/**/        return( ERROR );
/**/    }
/**/    iomnDebugMessage( ">>>OK:SubJobmode = SUB_INPUT(JobID=%d)\n", hJobID);
/**/
/**/    /* ＩＯモードの設定  */
/**/    if( parMNSetIOMode( hJobID , COM_OT_TRK_FIN ) == ERROR )
/**/    {
/**/        cmnPrintf_ioMN(2, ">> ioMN >> ERROR: parMNSetIOMode()\n");
/**/        return( ERROR );
/**/    }
/**/    iomnDebugMessage( ">>>OK:IOMode = COM_OT_TRK_FIN(JobID=%d)\n", hJobID);
/**/
/**/    /* ジョブ開始指示送信 */
/**/    hTrigger  = ioMNCjst[hJobID].msgparam.sCjstCtl.hTrigger;
/**/    hContinue = ioMNCjst[hJobID].msgparam.sCjstCtl.hContinue;
/**/    /*  messageID取得   */
/**/    if(ioMNCjst[hJobID].iOldMsgId == IOMSG_CJST_UI){
/**/        imsgid = IOMSG_TJFN_UI;
/**/        imsgidTjed = IOMSG_TJED_UI;
/**/    }
/**/    if(ioMNCjst[hJobID].iOldMsgId == IOMSG_CJST_JOB){
/**/        imsgid = IOMSG_TJFN_JOB;
/**/        imsgidTjed = IOMSG_TJED_JOB;
/**/    }
/**/    if(ioMNCjst[hJobID].iOldMsgId == IOMSG_CJST_DIG){
/**/        imsgid = IOMSG_TJFN_DIG;
/**/    }
/**/    /*  JOB開始準備処理 */
/**/    if(ERROR == (Status = iomnPrepareForFax(hJobID, hTrigger,
/**/                            hContinue, hJobType)))
/**/    {
/**/        iomnDebugMessage(">>>Error:iomnPrepareForFax()\n",0);
/**/        /* ＪＯＢ完了メッセージ送信 */
/**/        if(ERROR == (Status = iomnSendJobFinishSts(ioMNCjst[hJobID].OldmsgQId,
/**/                                imsgid, hJobID, IOMSG_FIN_ERROR, errno,
/**/                                IOMSG_NOT_USED)))
/**/        {
/**/            cmnPrintf_ioMN(2,">>>Error:iomnSendJobFinishSts()\n");
/**/            return(ERROR);
/**/        }
/**/        return(OK);
/**/    }
/**/    /* JOB開始メッセージ送信 */
/**/    if(ERROR == (Status = iomnSendJobStart(hJobID,hTrigger, 
/**/                            hContinue, 0)))
/**/    {
/**/        iomnDebugMessage(">>>Error:iomnSendJobStart()\n",0);
/**/        /* FAXジョブ開始終了対応 */
/**/        /* ＩＰテーブルと逐次制御テーブルの解放 */
/**/        iomnStart_IP_SIS_Cls( hJobID );
/**/        return(ERROR);
/**/    }
/**/    iomnDebugMessage(">>>OK:iomnSendJobStart()\n",0);
/**/    iomnDebugMessage(">>>>Bottom of iomnRecieveEC44()<<<<\n",0);
#endif  /* MCK1.3 END   */
    return(OK);
}

/*
(C) Copyright TOSHIBA Corporation 1996,1997. ALL Right Reserved
NO.
TITLE:      横流しイベント処理
MODULE：    STATUS  iomnRecieveEvent(I32 irmsgId)
INCLUDE:    ioMNPar.h
PARAMETER:
            I32     irmsgId         受信メッセージ
RETURNS:    I32 4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            横流しイベント受信時に、イベントを指定先へ送信する。
NOTES:
HISTORY:
    D1.7.10 J.Ootani    00/04/10    DM45/35 FAXリストレポート対応のため
                                    新規作成

    MCK1.1  M.Karasawa 02/06/11 McKinley    新メッセージ対応

    V010.00_04  M.Karasawa  02/12/17    McKinley    メッセージ変更 JOB_MSG_RFSR -> JOB_MSG_EFSR
*/
STATUS  iomnRecieveEvent(I32 irmsgId)
{
    I32       imsgId = 0;
    MSG_Q_ID  msgQId = NULL;
    HI        hJobID;

    iomnDebugMessage(">>>>TOP of iomnRecieveEvent()<<<<\n",0);

    switch(irmsgId)
    {
#if 0   /* MCK1.1 新メッセージ対応  */
/**/      case IOMSG_EPSE_EX  :     /* １ページ入力終了イベント */
#else
      case JOB_MSG_ESOS  :     /* １ページ入力終了イベント */
#endif
             /*
             hJobID = ioMNrmyparam.sEpseCtl.hJobid;
             */
             hJobID = 1;
             iomnDebugMessage("###JOBID ->%x\n",(I32)hJobID);
             msgQId = ioMNCjst[hJobID].OldmsgQId;
#if 0   /* MCK1.1 新メッセージ対応  */
/**/             imsgId = IOMSG_EPSE_JOB;
#else
             imsgId = JOB_MSG_ESOS;
#endif
             break;
#if 0   /* MCK1.1 新メッセージ対応  */
/**/      case IOMSG_ENXT_EX  :    /* 時原稿有無イベント */
#else
      case JOB_MSG_ESNT  :    /* 時原稿有無イベント */
#endif
             /*
             hJobID = ioMNrmyparam.sEnxtCtl.hJobid;
             */
             hJobID = 1;
             iomnDebugMessage("###JOBID ->%x\n",(I32)hJobID);
             msgQId = ioMNCjst[hJobID].OldmsgQId;
#if 0   /* MCK1.1 新メッセージ対応  */
/**/             imsgId = IOMSG_ENXT_JOB;
#else
             imsgId = JOB_MSG_ESNT;
#endif
             break;
#if 0   /* MCK1.1 新メッセージ対応  */
/**/      case IOMSG_EPNS_JOB :    /* 次ページ入力イベント */
#else
      case JOB_MSG_EFSR :    /* 次ページ入力イベント *//* V010.00_04    */
/*      case JOB_MSG_RFSR :    *//* 次ページ入力イベント */
#endif
             msgQId = msgQIdIoExMN;
#if 0   /* MCK1.1 新メッセージ対応  */
/**/             imsgId = IOMSG_EPNS_EX;
#else
             imsgId = JOB_MSG_EFSR;     /* V010.00_04   *//* V028.00_00 */
/*             imsgId = JOB_MSG_RFSR;*/
#endif
             break;
      default:
             cmnPrintf_ioMN(2,"[ioMN]>>>error iomnRecieveEvent! msgId=%d\n",imsgId);
    }

    /* ジョブイベントメッセージ送信 */
    if( ERROR == iomnSendJobEvent(msgQId, imsgId))
    {
        cmnPrintf_ioMN(2,"[ioMN]>>>Error:iomnSendJobEvent()\n");
        return(ERROR);
    }

    iomnDebugMessage(">>>>Bottom of iomnRecieveEvent()<<<<\n",0);

    return(OK);
}

/*
(C) Copyright TOSHIBA Corporation 2000. ALL Right Reserved
NO.
TITLE:      FAX回線管理機能
MODULE：    void iomnFAXLineBusy()
INCLUDE:
PARAMETER:
RETURNS:
OUTLINE:
            FAX回線を管理するための関数
NOTES:
HISTORY:
    D2.3    T.Fujii     00/06/06    original
    D3.14.1 T.Fujii     00/09/04    DM45/35 Log強化対応

    MCK1.1  M.Karasawa 02/06/11 McKinley    新メッセージ対応
*/
void iomnFAXLineBusy()
{
#if 0   /* MCK1.1 新メッセージ対応  *//* 関数削除可能   */
/* D3.14.1 START T.Fujii ログ表示強化 */
#if 0   /* OLD */
    iomnDebugMessage( "[ioMN] PC-FAX ON LINE\n", (I32)NULL );
#else   /* D3.14.1 MID  NEW */
    cmnPrintf_ioMN(0, "[ioMN]PC-FAX ON LINE (JobID=%d)\n", ioMNrmyparam.sEistCtl.hJobid );
#endif
/* D3.14.1 END */
#endif
}

/*
(C) Copyright TOSHIBA Corporation 2000. ALL Right Reserved
NO.
TITLE:      FAX回線管理機能
MODULE：    void iomnFAXLineOff()
INCLUDE:
PARAMETER:
RETURNS:
OUTLINE:
            FAX回線を管理するための関数
NOTES:
HISTORY:
    D2.3    T.Fujii     00/06/06    original
    D3.14.1 T.Fujii     00/09/04    DM45/35 Log強化対応

    MCK1.1  M.Karasawa 02/06/11 McKinley    新メッセージ対応
*/
void iomnFAXLineOff()
{
#if 0   /* MCK1.1 新メッセージ対応  *//* 関数削除可能   */
/* D3.14.1 START T.Fujii ログ表示強化 */
#if 0   /* OLD */
    iomnDebugMessage( "[ioMN] PC-FAX OFF LINE)\n", (I32)NULL );
#else   /* D3.14.1 MID  NEW */
    cmnPrintf_ioMN(0, "[ioMN]PC-FAX OFF LINE (JobID=%d)\n", ioMNrmyparam.sEiedCtl.hJobid );
#endif
/* D3.14.1 END */
#endif
}


/* D3.12 START T.Fujii ジョブ中断処理修正 */
/*
(C) Copyright TOSHIBA Corporation 2000. ALL Right Reserved
NO.
TITLE:      入力ジョブの中断
MODULE：    STATUS iomnInputJobSuspend(HI hInputJobID, HI hSusFactor, HI hJobType)
INCLUDE:
PARAMETER:
            HI  2B  hInputJobID     入力ジョブのジョブID
            HI  2B  hSusFactor      中断要因
            HI  2B  hJobType        ジョブ種別
RETURNS:    STATUS  4B
            OK:     正常
            ERROR:  異常終了
OUTLINE:
            コピーC/Sキー押下による原稿入力中断処理
NOTES:
HISTORY:
    D3.12   T.Fujii     00/09/02    DM45/35 original
    D3.12.1 T.Fujii     00/09/04    DM45/35 BG印刷ジョブ中断処理修正
    D3.12.4 T.Fujii     00/09/25    DM45/35 BG印刷ジョブ中断処理修正

    V007.00_02  M.Karasawa  02/09/02    McKinley    入力ジョブ中断指示受信時、出力の中断は上位からの指示待ち
*/
STATUS iomnInputJobSuspend( HI hInputJobID, HI hSusFactor, HI hJobType )
{
/*    HI hOutputJobID;    *//* 出力ジョブのJobID *//* V028.00_00 */

    #ifdef DEBUG
    puts("[ioMN]START:iomnInputJobSuspend()");
    cmnPrintf_ioMN(0,"[ioMN]CHECK:InputJobID=%d,SusFactor=0x%x,JobType=0x%x\n",hInputJobID,hSusFactor,hJobType);
    #endif

    /* 入力ジョブの中断指示 */
    iomnFrontCjsu( hInputJobID, hSusFactor, hJobType );

#if 0   /* V007.00_02   *//* START  */
/**/    /* 後追い逐次で出力している場合は出力も中断処理を行う */
/**/    /* D3.12.1 START T.Fujii BG印刷ジョブ中断処理修正 */
/**/    /* 中断BG印刷ジョブ設定 */
/**/    /* 出力ジョブのジョブIDを取得する */
/**/    hOutputJobID = iomnSyncTblGetJobID( iomnJobInfo[hInputJobID].hIpTblID, COM_JB_SUB_OUTPUT );
/**/    /* BG印刷中断設定 */
#endif  /* V007.00_02   *//* END    */
    iomnSyncTblSetJobSuspend( iomnJobInfo[hInputJobID].hIpTblID, IOMN_ON );
#if 0   /* V007.00_02   *//* START  */
/**/    /* D3.12.1 END */
/**/
/**/    /* BG印刷ジョブが存在する場合 */
/**/    if( ioMNstatus.hBackJobId > IOMN_INPUTJOB_ID_MAX ) {
/**//* D3.12.1 START T.Fujii BG印刷ジョブ中断処理修正 */
/**/#if 0   /* OLD */
/**/        /* 出力ジョブのジョブIDを取得する */
/**/        hOutputJobID = iomnSyncTblGetJobID( iomnJobInfo[hInputJobID].hIpTblID, COM_JB_SUB_OUTPUT );
/**/#endif
/**//* D3.12.1 END */
/**/        if( hOutputJobID == ioMNstatus.hBackJobId ) {
/**/            /* 出力中のジョブが入力ジョブとリンクされたジョブの場合は中断処理を行なう */
/**//* D3.12.1 START T.Fujii BG印刷ジョブ中断処理修正 */
/**/#if 0   /* OLD */
/**/            ioMNstatus.hBGPrnSuspend = IOMN_ON;
/**/#endif
/**//* D3.12.1 END */
/**/            jobJobtype( hOutputJobID, &hJobType );              /* 出力ジョブのジョブ種別を取得 */
/**//* D3.12.4 START T.Fujii BG印刷ジョブ中断処理修正 */
/**/#if 0   /* OLD */
/**/            iomnPrnCjsu( hOutputJobID, IOMSG_SUS_CS, hJobType );  /* 出力ジョブの中断 */
/**/#else   /* D3.12.4 MID  NEW */
/**/            iomnPrnCjsu( hOutputJobID, hSusFactor, hJobType );  /* 出力ジョブの中断 */
/**/#endif
/**//* D3.12.4 END */
/**/        }
/**/    }
#endif  /* V007.00_02   *//* END    */
    return( OK );
}
/* D3.12 END */

/* D3.40 START T.Fujii フロントUI取得関数を追加 */
/*
(C) Copyright TOSHIBA Corporation 2000. ALL Right Reserved
NO.         
TITLE:      フロントUI取得関数
MODULE：    HI iomnLibGetFrontUI( void )
INCLUDE:    
PARAMETER:
RETURNS:    HI      フロントUIのID
OUTLINE:
NOTES:
            ioMNでの外部公開関数(特別対応)
HISTORY:
        D3.40   T.Fujii 00/10/18 DM45/35 original
        D3.40.1 T.Fujii 00/10/25 DM45/35 エラー復帰先UI取得方法変更
*/
HI iomnLibGetFrontUI( void )
{
/* D3.40.1 START T.Fujii エラー復帰先UI取得方法変更 */
#if 0
    HI hUI;
    HI hUIType;
    HI hUImode;
#else   /* D3.40.1 MID  NEW */
    HI hReservedUI  = ioMNUiBuffer.hUI;
    HI hErrReturnUI = ioMNUiErrClrBuf.hUI;
#endif
/* D3.40.1 END */

    if( ioMNhw.hPriUi == UI_MACHINE ) {
        /* ERROR解除時の復帰先UIが設定されている場合 */
/* D3.40.1 START T.Fujii エラー復帰先UI取得方法変更 */
#if 0   /* OLD */
        if( iomnGetChangeUIQue(&ioMNUiErrClrBuf, &hUIType, &hUI, &hUImode) != ERROR ) {
            return( hUI );
        }
        else {
            return( IOMN_OFF );
        }
#else   /* D3.40.1 MID  NEW */
        if( hErrReturnUI == IOMN_OFF ) {
            if( hReservedUI == IOMN_OFF ) {
                return( iomnLibGetFrontUI() );
            }
            else {
                #ifdef DEBUG_001025
                cmnPrintf_ioMN(0, "[ioMN]iomnLibGetForntUI=0x%x(Front=MCN_UI)\n", hReservedUI );
                #endif
                return( hReservedUI );
            }
        }
        else {
            #ifdef DEBUG_001025
            cmnPrintf_ioMN(0, "[ioMN]iomnLibGetForntUI=0x%x(Front=MCN_UI)\n", hErrReturnUI );
            #endif
            return( hErrReturnUI );
        }
#endif
/* D3.40.1 END */
    }
    else {
        #ifdef DEBUG_001025
        cmnPrintf_ioMN(0, "[ioMN]iomnLibGetForntUI=0x%x\n", ioMNhw.hPriUi );
        #endif
        return( ioMNhw.hPriUi );
    }
}
/* D3.40 END */

/*
(C) Copyright TOSHIBA Corporation 2002.2003. ALL Right Reserved
NO.         
TITLE:      
MODULE：    iomnJobCancelTestCopy( HI hJobid, HI hJobtype, HI hCanFacter)
INCLUDE:
PARAMETER:
            HI hJobid       ジョブID
            HI hJobtype     ジョブ種別
            HI hCanFacter   中断要因
RETURNS:
OUTLINE:
NOTES:
HISTORY:
    V017.00_01  M.Karasawa  03/04/09    McKinley    original
*/
STATUS  iomnJobCancelTestCopy( HI hJobid, HI hJobtype, HI hCanFacter)
{
    STATUS  CancelStatus;   /*  CANCEL確認処理  */

    HI      hJobstatus;
    I32     iSuscause;

    CancelStatus = iomnConfirmAboutCancel(hJobid);

    switch(CancelStatus){
        case    IOMN_ON:
            if( ERROR == iomnSubJobCancel( hJobid, hJobtype, hCanFacter )){
                iomnDebugMessage(">> ioMN >> ERROR:iomnSubJobCancel() in iomnJobCancelTestCopy()\n",0);
            }
            iomnDebugMessage("*** Send CJCN ---> tioExMN in iomnJobCancelTestCopy\n",0);
            break;
        case    IOMN_OFF:
            if(ERROR == jobStatus(hJobid, &hJobstatus, &iSuscause)){
                iomnDebugMessage(">>>Error:jobStatus()\n",0);
                return(ERROR);
            }
            if( hJobstatus == COM_JS_SUSPEND && iSuscause != IOMSG_SUS_CS){
                /* 自動再開しないようにCS中断を強制的に設定する */
                if(ERROR == jobSetStatus(hJobid,COM_JS_SUSPEND,IOMSG_SUS_CS)){
                    cmnPrintf_ioMN(2,">>>Error:jobSetStatus() in iomnJobCancelTestCopy()\n");
                }
            }
            /* キャンセル処理不可時、TJFNを送信する */
            if( ERROR == iomnSendTjfn(hJobid, (HI)IOMSG_JB_TEST_ERR, (I32)IOMSG_NOT_USED, (I32)IOMSG_NOT_USED) ){
                iomnDebugMessage(">> ioMN >> Error:iomnSendTjfn()in iomnJobCancelTestCopy()\n",0);
                return(ERROR);
            }
            /*  キャンセルフラグ解除    */
            if(hJobid == hioMNFrontJobID || hJobid == hioMNIntJobID){
                ioMNstatus.FrontCancel.hCanTask = ioMNstatus.FrontCancel.hCanType = IOMN_OFF;
            }else if( hJobid == hioMNDataRecvJobID ) {
                /* データ転送ジョブのときは何もしない */
            }else{
                ioMNstatus.BackCancel.hCanTask = ioMNstatus.BackCancel.hCanType = IOMN_OFF;
            }
            break;
        case    ERROR:
            iomnDebugMessage(">>>ERROR:iomnConfirmAboutCancel() in iomnJobCancelTestCopy()\n",0);
            return(ERROR);
            break;
        default:
            break;
    }
    return(ERROR);  /* V028.00_00 */
}

/*
(C) Copyright TOSHIBA Corporation 2002.2003. ALL Right Reserved
NO.         
TITLE:      
MODULE：    iomnJobPause(HI hJobid, HI hCanFactor, HI hJcbID, HI hTNo, HI hProcessType, HI hAppliType)
INCLUDE:
PARAMETER:
            HI hJobid       ジョブID
RETURNS:
OUTLINE:
NOTES:
HISTORY:
    V019.00_03  M.Karasawa  03/05/20    McKinley    original
    V045.00_03  M.Karasawa  04/06/22    McKinley    ジョブ中断不可時の応答パラメータ不正の修正
*/
STATUS iomnJobPause(HI hJobid, HI hCanFactor, HI hJcbID, HI hTNo, HI hProcessType, HI hAppliType)
{
    STATUS  Status;     /*  ステータス  */
    HI      hJobstatus; /*  JOB状態     */
    I32     iSuscause;  /*  中断要因    */

    /* ジョブステータス取得 */
    if(ERROR == (Status = jobStatus(hJobid, &hJobstatus, &iSuscause))){
        iomnDebugMessage(">>>Error:jobStatus()\n",0);
        return(ERROR);
    }

    /*
        ジョブ状態チェック ※実行層に対しての処理は無いので、マシンエラー等は考えない
    */
    if(hJobstatus == COM_JS_READY || hJobstatus == COM_JS_PRT_WAITING){
        /* 終了処理可能 */
        /* ジョブ削除し、EQRN、TJFNを送信 */
        return(iomnWaitingOutputJobCancel(hJobid, hJobstatus));
    }else{
        /* 終了処理不可 */
        /* TJCN送信 */
        memset(&ioMNsmyparam, 0x00, sizeof(union ioMNparam));
        ioMNsmyparam.sTjcnCtl.hJcbID        = hJcbID;
        ioMNsmyparam.sTjcnCtl.hTNo          = hTNo;
        ioMNsmyparam.sTjcnCtl.hCanFactor    = hCanFactor;
/*      ioMNsmyparam.sTjcnCtl.iSts          = IOMSG_CAN_ERROR;  *//* V045.00_03 */
        ioMNsmyparam.sTjcnCtl.iSts          = ERROR;              /* V045.00_03 */
        ioMNsmyparam.sTjcnCtl.hProcessType  = hProcessType;
        ioMNsmyparam.sTjcnCtl.hAppliType    = hAppliType;
        if(ERROR == (Status = iomnSendMessage(ioMNCjcn[hJobid].OldmsgQId, JOB_MSG_TJCN, 0, &ioMNsmyparam))){
            iomnDebugMessage(">>>Error:iomnSendMessage TJCN\n",0);
            return(ERROR);
        }
        iomnDebugMessage(">>>OK:iomnSendMessage TJCN\n",0);
    }
    return(OK);
}
#if 1 /* RioGrande マニュアル対応 */
/*
 マニュアル動作開始要求受信処理
*/
STATUS iomnRecieveRIST(void)
{
    STATUS Status;
    MCN_TBL_OPTIONSTATUS sMcn_Tbl;  /* HM新フィニッシャ対応 */

    /* マニュアル動作可能かどうかの判断 */
    Status = iomnInsmanualConfirm();
    if(Status == OK){
        /* 送信メッセージクリア */
        memset(&ioMNsmyparam, 0x00, sizeof(union ioMNparam));
        /* 仕上げモード開始イベント送信 */
        ioMNsmyparam.sCistCtl.iSts = OK;    /* HM新フィニッシャ対応 */
        if( iomnSendMessage(msgQIdMcnMN,JOB_MSG_CIST,0,&ioMNsmyparam) == ERROR ){
            iomnDebugMessage(">> ioMN >> Error:iomnSendMessage in iomnRecieveCIST\n",0);
            return( ERROR );
        }
        /* マニュアル動作中フラグ 動作中に設定 */
        ioMNstatus.hinserter_status = IOMN_ON;
    }else{
/* HM新フィニッシャ対応 *//* START */
        if(mcnTBLOptionStatus(&sMcn_Tbl) == ERROR){
            /* インサータかHMか分からないので処理無し */
            return(ERROR);
        }
        if(sMcn_Tbl.hFIN == FINTYP_HM_CONSOLE){
            /* 新フィニッシャの場合には応答を返す */
            memset(&ioMNsmyparam, 0x00, sizeof(union ioMNparam));
            /* 仕上げモード開始イベント送信 */
            ioMNsmyparam.sCistCtl.iSts = ERROR;
            if( iomnSendMessage(msgQIdMcnMN,JOB_MSG_CIST,0,&ioMNsmyparam) == ERROR ){
                iomnDebugMessage(">> ioMN >> Error:iomnSendMessage in iomnRecieveCIST\n",0);
                return( ERROR );
            }
        }else{
/* HM新フィニッシャ対応 *//* END */
            /* インサータ動作予約 */
            ioMNstatus.hinserter_reserve = IOMN_ON;
/* HM新フィニッシャ対応 *//* START */
        }
/* HM新フィニッシャ対応 *//* END */
    }
    return(OK);
}
/*
 マニュアル動作開始応答受信処理
*/
STATUS iomnRecieveAIST(void)
{
    /* 送信メッセージクリア */
    memset(&ioMNsmyparam, 0x00, sizeof(union ioMNparam));
    /* 仕上げモード開始イベント送信 */
    if( iomnSendMessage(msgQIdCMPanel,JOB_MSG_EIST,0,&ioMNsmyparam) == ERROR ){
        iomnDebugMessage(">> ioMN >> Error:iomnSendMessage in iomnRecieveAIST\n",0);
        return( ERROR );
    }
    return(OK);
}
/*
 マニュアル動作開始結果受信処理
*/
STATUS iomnRecieveTIFN(void)
{
    STATUS Status;

    Status = OK;
    /* 送信メッセージクリア */
    memset(&ioMNsmyparam, 0x00, sizeof(union ioMNparam));
    /* 仕上げモード開始イベント送信 */
    if( iomnSendMessage(msgQIdCMPanel,JOB_MSG_EIFN,0,&ioMNsmyparam) == ERROR ){
        iomnDebugMessage(">> ioMN >> Error:iomnSendMessage in iomnRecieveTIFN\n",0);
        Status = ERROR;
    }
    if( iomnSendMessage(msgQIdSysMN,JOB_MSG_EIFN,0,&ioMNsmyparam) == ERROR ){
        iomnDebugMessage(">> ioMN >> Error:iomnSendMessage in iomnRecieveTIFN\n",0);
        Status = ERROR;
    }
    /*
        sysMNLibのインサータマニュアル状態設定関数をコールしてフラグを落とすようにする処理が必要
    */
    if( SysLibInserterStsSet(SYSLIB_OFF) == ERROR){
        iomnDebugMessage(">> ioMN >> Error:SysLibInserterStsSet in iomnRecieveTIFN\n",0);
        Status = ERROR;
    }
    /* マニュアル動作中フラグ 未動作に設定
       ERRORであってもフラグだけは落としておく */
    ioMNstatus.hinserter_status  = IOMN_OFF;
    return(Status);
}
#endif
STATUS iomnReceiveEQNE() /* L4.9 Feedback of MDS requirement EBX_DCR_33145 */
{
 STATUS  Status; 
 HI      hJobid; 

  /*  Get Parameter   */
  hJobid = ioMNrmyparam.sEqneCtl.hJobid;
/* ????????????????     */
 memcpy(&ioMNsmyparam, &ioMNrmyparam, sizeof(union ioMNparam));
 if(ERROR==(Status=iomnSendMessage(ioMNCjst[hJobid].OldmsgQId,JOB_MSG_EQNE,0,&ioMNsmyparam))){
 		iomnDebugMessage(">>>Error:iomnSendMessage EQUE \n",0);
 		return(ERROR);
	}
	return OK;
}
/* ----- JobioMN.c Program End ----- */
