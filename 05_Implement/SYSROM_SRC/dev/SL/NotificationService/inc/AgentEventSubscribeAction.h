/*****************************************************************************/
/*  (C) Copyright  TOSHIBA TEC CORPORATION 2006   All Rights Reserved        */
/*****************************************************************************
============================== Source Header =================================
 Filename: %PM%
 Revision: %PR%
 File Spec: %PID%
 Originator: %PO%
 Last Changed: %PRT%
 Outline[by EA]: */
 /**
 * This class has a process of a subscribe/unsubscribe event for a Agent Engine.
 * The subscribe/unsubscribe event to DL is sent and the subscribe/unsubscribe
 * method of EventAction class is called by it.
 * When the subscribe/unsubscribe event is received, this class object is created.
 * @version 1.0
 * @created 25-Jan-2007 14:28:34
 */
/*----------------------------------------------------------------------------
 Related Change Documents:
   %PIRC%
------------------------------------------------------------------------------
 Related Baselines:
   %PIRB%
------------------------------------------------------------------------------
 History:
   %PL%
========================== End of Source Header =============================*/


#if !defined(SL_NOTIFICATIONSERVICE__AGENTEVENTSUBSCRIBEACTION_H)
#define SL_NOTIFICATIONSERVICE__AGENTEVENTSUBSCRIBEACTION_H

#include <list>
#include "SubscribeAction.h"
#include "MessagingSystem/msg.h"
#include "OperatingEnvironment/ref.h"
#include "MessagingSystem/msgport.h"

using std::list;
using CI::MessagingSystem::Msg;
using CI::OperatingEnvironment::Ref;
using CI::MessagingSystem::MsgPortRef;
using CI::MessagingSystem::MsgPort;


namespace SL
{
	namespace NotificationService
	{
		/**
		 * This class has a process of a subscribe/unsubscribe event for a Agent Engine.
		 * The subscribe/unsubscribe event to DL is sent and the subscribe/unsubscribe
		 * method of EventAction class is called by it.
		 * When the subscribe/unsubscribe event is received, this class object is created.
		 * @version 1.0
		 * @created 25-Jan-2007 14:28:34
		 */
		class AgentEventSubscribeAction : public SubscribeAction
		{

		public:
			/**
			 * Constructs the AgentEventSubscribeAction.
			 */
			AgentEventSubscribeAction();

			/**
			 * Destructs the AgentEventSubscribeAction.
			 */
			virtual ~AgentEventSubscribeAction();

		protected:
			/**
			 * Gets the event id.
			 * 
			 * @param pMsg
			 */
			virtual list<unsigned int> getEventId(const Ref<Msg>& pMsg) const;

		};

	}

}
#endif // !defined(SL_NOTIFICATIONSERVICE__AGENTEVENTSUBSCRIBEACTION_H)
