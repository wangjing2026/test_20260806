/*****************************************************************************/
/*  (C) Copyright  TOSHIBA TEC CORPORATION 2006   All Rights Reserved        */
/*****************************************************************************/

/**
 * @file
 * @author S.Tanaka
 * @date 2009/08/24
 */

#include "actionmanager.h"
#include "cpedef.h"

using namespace std;
using namespace dl;
using namespace layerinterface::dl;
using namespace ci::messagingsystem;

namespace dl {

    namespace cpe {

        /**
         * @brief コンストラクタ
         */
        ActionManager::ActionManager(Messenger *messenger) :
            m_messenger(messenger) {

        }

        /**
         * @brief デストラクタ
         */
        ActionManager::~ActionManager() {
            removeAllAction();
        }

        /**
         * @brief アクションの実行
         * @param msg 受信パラメータ
         */
        bool ActionManager::executeAction(const Msg& msg) {
            LOGGER_FCN("[executeAction]\n");
            if (msg.IsType('C', SL2CPE::executeCPE)) {
                C_executeCPE_Proc(msg);
            } else if (msg.IsType('A', CPE2CPE::executeAction)) {
                //
            } else if (msg.IsType('N', CPE2CPE::executeAction)) {
                N_executeAction_Proc(msg);
            } else if (msg.IsType('T', CPE2CPE::executeAction)) {
                T_executeAction_Proc(msg);
            } else if (msg.IsType('E', CPE2CPE::releaseAction)) {
                E_releaseAction_Proc(msg);
            } else if (msg.IsType('E', DE2DIM::changeStatus)) {
                E_changeStatus_Proc(msg);
            } else {
                // 未対応のメッセージ。Nを返しておく
                LOGGER_ERR("[C_executeCommand_Proc] Unknown Message\n");
                //sendN_executeCPE(msg, 0);
            }

            return true;
        }

        /**
         * C_executeCPE受信時の処理
         */
        void ActionManager::C_executeCPE_Proc(const Msg& msg) {
            LOGGER_FCN("[C_executeCPE_Proc]\n");
            CommandAction *action = NULL;

            SL2CPE::C_executeCPE *content = static_cast<SL2CPE::C_executeCPE*> (msg.GetContentPtr());
            uint32 actionId = BaseAction::createActionID(msg, (uint16)content->groupID);

            // A返信
            m_messenger->sendReplyAMsg(msg);

            // アクションの検索
            action = findAction(actionId);

            // アクションが見つからなければFactoryから生成する
            if (action == NULL) {

                try {
                    // アクションの生成
                    action = m_actionFactory.createAction(m_messenger, msg);
                } catch (CPEError& e) {
                    // 生成失敗はNを返す
                    LOGGER_ERR("[C_executeCPE_Proc] %s\n", e.getMessage().c_str());/* eBN175 */
                    sendN_executeCPE(msg, e.getErrorCode());
                    return;
                }

                // ありえないエラー
                if (action == NULL) {
                    sendN_executeCPE(msg, ERR_DL_CPE_UNKNOWN_COMMAND);
                    return;
                }

                // アクションの登録
                addAction(action);

                // アクションのスレッドを開始する
                action->Start(NULL);
            }

            // アクションへメッセージ送信
            sendC_executeAction(msg, actionId);
        }

        /**
         * N_executeAction受信時の処理
         */
        void ActionManager::N_executeAction_Proc(const Msg& msg) {
            CPE2CPE::N_executeAction *rcontent = static_cast<CPE2CPE::N_executeAction*> (msg.GetContentPtr());

            SL2CPE::N_executeCPE content;
            content.errorCode = rcontent->errorCode;
            content.errorCategory = rcontent->errorCategory;

            // N_executeCPEの送信
            Msg sendMsg = msg;
            sendMsg.ReplyOffNormal(&content, sizeof(content));
            sendMsg.SetId(SL2CPE::executeCPE);
            sendMsg.SetTarget(rcontent->msgPortId);
            sendMsg.SetSender(MSG_PORTID_DL_CPE);
            m_messenger->sendMessage(sendMsg);
        }

        /**
         * T_executeAction受信時の処理
         */
        void ActionManager::T_executeAction_Proc(const Msg& msg) {
            CPE2CPE::T_executeAction *rcontent = static_cast<CPE2CPE::T_executeAction*> (msg.GetContentPtr());

            SL2CPE::T_executeCPE content;
            strcpy(content.shmName, rcontent->shmName);
            content.items = rcontent->items;

            // T_executeCPEの送信
            Msg sendMsg = msg;
            sendMsg.Reply(&content, sizeof(content), MSG_TYPE_RESPONSE);
            sendMsg.SetId(SL2CPE::executeCPE);
            sendMsg.SetTarget(rcontent->msgPortId);
            sendMsg.SetSender(MSG_PORTID_DL_CPE);
            m_messenger->sendMessage(sendMsg);
        }

        /**
         * E_releaseAction受信時の処理
         */
        void ActionManager::E_releaseAction_Proc(const Msg& msg) {
            LOGGER_FCN("[E_releaseAction_Proc]\n");
            CPE2CPE::E_releaseAction *content = static_cast<CPE2CPE::E_releaseAction*>(msg.GetContentPtr());

            // アクションの検索
            CommandAction *action = findAction(content->actionId);
            if (action != NULL) {
                // アクションのスレッドが終了するまで待つ
                //action->WaitTillExit(0);
                action->WaitTillExit(-1);
                // アクションの削除
                removeAction(content->actionId);
            }
        }

        /**
         * E_changeStatus受信時の処理
         */
        void ActionManager::E_changeStatus_Proc(const Msg& msg) {
            LOGGER_FCN("[E_changeStatus_Proc]\n");

            CPE2DIM::E_changeStatus content;
            content.items = 0;

            Msg sendmsg = msg;
            sendmsg.SetId(CPE2DIM::changeStatus);
            sendmsg.SetTarget(MSG_PORTID_DL_DIM);
            sendmsg.SetSender(MSG_PORTID_DL_CPE);
            sendmsg.SetContent(&content, sizeof(content));

            m_messenger->sendMessage(sendmsg);
        }

        /**
         * @brief アクションへC_executeAction送信
         */
        void ActionManager::sendC_executeAction(const Msg& msg, uint32 actionId) {
            //uint32 actionId = BaseAction::createActionID(msg);
            CommandAction *action = findAction(actionId);
            if (action != NULL) {
                SL2CPE::C_executeCPE *rcontent = static_cast<SL2CPE::C_executeCPE*> (msg.GetContentPtr());
                CPE2CPE::C_executeAction content;
                content.groupID = rcontent->groupID;
                strcpy(content.paramShmName, rcontent->paramShmName);
                content.paramItems = rcontent->paramItems;
                content.msgPortId = msg.GetSender();

                Msg sendMsg = msg;
                sendMsg.SetContent(&content, sizeof(content));
                sendMsg.SetId(CPE2CPE::executeAction);
                sendMsg.SetTarget(action->getMsgPortId());
                sendMsg.SetSender(MSG_PORTID_DL_CPE);
                sendMsg.SetSenderSeq(msg.GetSenderSeq());
                m_messenger->sendMessage(sendMsg);
            }
        }

        /**
         * @brief Nメッセージ返信
         */
        void ActionManager::sendN_executeCPE(const ci::messagingsystem::Msg& msg, int errorCode) {
/* eBN175 Start */
            if (msg.IsType('C', SL2CPE::executeCPE)) {
                SL2CPE::C_executeCPE *content = static_cast<SL2CPE::C_executeCPE*> (msg.GetContentPtr());
                if (content->paramItems > 0) {
                    LOGGER_ERR("[sendN_executeCPE] destroySharedMemory >> shmName = %s\n", content->paramShmName);
                    CSharedMemoryHandler sharedMemoryObj(content->paramShmName);
                    sharedMemoryObj.destroySharedMemory();
                }
            }
/* eBN175 End */
            SL2CPE::N_executeCPE content;
            content.errorCode = errorCode;
            content.errorCategory = 0;

            m_messenger->sendReplyNMsg(msg, &content, sizeof(content));
        }

        map<uint32, DataValue> ActionManager::getSharedParameters(char *shmName) {
            LOGGER_FCN("[getSharedParameters] shmName = %s\n", shmName);
            CSharedMemoryHandler sharedMemoryObj(shmName);
            map<uint32, DataValue> parameters = sharedMemoryObj.read32bitData();
            sharedMemoryObj.destroySharedMemory();

            // 受信パラメータの表示
            map<uint32, DataValue>::iterator it = parameters.begin();
//            LOGGER_ITC("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
            LOGGER_FCN("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");	//BPX183
            if (parameters.size() == 0) {
                LOGGER_ITC("++  Not have parameters\n");
            } else {
                LOGGER_ITC("++  Received parameters\n");
            }
            while (it != parameters.end()) {
                uint32 id = it->first;
                DataValue value = it->second;
                LOGGER_ITC("++  id = %d , value = %d\n", id, value.getInt());
                it++;
            }
//            LOGGER_ITC("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
            LOGGER_FCN("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");	//BPX183

            return parameters;
        }

        /**
         * @brief アクション検索
         * @param msg 受信メッセージ
         */
        CommandAction *ActionManager::findAction(uint32 actionId) {
            for (unsigned int i = 0; i < m_actions.size(); i++) {
                if (actionId == m_actions[i]->getId()) {
                    LOGGER_FCN("[findAction] found id = 0x%08x\n", actionId);
                    return m_actions[i];
                }
            }
            LOGGER_FCN("[findAction] not found id = 0x%08x\n", actionId);
            return NULL;
        }

        /**
         * @brief アクション登録
         * @param action 登録するアクション
         */
        void ActionManager::addAction(CommandAction *action) {
            LOGGER_FCN("[addAction] id = 0x%08x\n", action->getId());
            m_actions.push_back(action);
        }

        /**
         * @brief アクション削除
         * @param msg 受信メッセージ
         */
        void ActionManager::removeAction(uint32 actionId) {
            vector<CommandAction*>::iterator it = m_actions.begin();
            while (it != m_actions.end()) {
                if (actionId == (*it)->getId()) {
                    delete (*it);
                    m_actions.erase(it);
                    LOGGER_FCN("[removeAction] OK id = 0x%08x\n", actionId);
                    return;
                }
                it++;
            }
            LOGGER_FCN("[removeAction] NG id = 0x%08x\n", actionId);
        }

        /**
         * @brief 全てのアクションの削除
         */
        void ActionManager::removeAllAction() {
            vector<CommandAction*>::iterator it = m_actions.begin();
            while (it != m_actions.end()) {
                delete (*it);
                it++;
            }
            m_actions.clear();
            LOGGER_FCN("[removeAllAction] OK\n");
        }

    }

}
