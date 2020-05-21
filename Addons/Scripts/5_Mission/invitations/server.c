class PM_S_Invitations extends PM_InvitationsHandler
{
	void PM_S_Invitations()
	{
		AddEvents();
	}

	void AddEvents()
	{
		PM_GetEvents().AddEvent("InvitationReceived", this);
		PM_GetEvents().AddEvent("InvitationResponse", this);
	}

	override bool ConditionnalInvitation(string sender, string receiver)
	{
		// Check size of group
		return true;
	}

	override void AfterInvitation(string sender, string receiver)
	{
		PlayerIdentity receiverIdentity = MissionServer.Cast(GetGame().GetMission()).GetPlayerIdentity(receiver);
		PlayerIdentity senderIdentity = MissionServer.Cast(GetGame().GetMission()).GetPlayerIdentity(sender);
		
		if (senderIdentity && receiverIdentity)
		{
			GetRPCManager().SendRPC("PartyMe", "InvitationReceived", new Param2<string, string>(sender, senderIdentity.GetName()), false, receiverIdentity);
		}
		else if (!receiverIdentity && true) // debug mode
		{
			Print("[PartyMe][Error] " + sender + " tried to invite " + receiver + " but PlayerIdentity has not be found!");
		}
	}
	
	// Events
	void OnInvitationReceived(ref PM_Event_Params eventParams)
	{
		string sender = eventParams.playerIdFrom;
		string receiver = eventParams.playerIdTo;

		Print("[PartyMe][InvitationsHandler][Event] OnInvitationReceived has been called. " + sender + " | " + receiver);
		InvitePlayer(sender, receiver);
	}
	
	void OnInvitationResponse(ref PM_Event_Params eventParams)
	{
		string sender = eventParams.playerIdTo;
		string receiver = eventParams.playerIdFrom;
		bool hasAccepted = eventParams.answer;

		if (HasInvited(sender, receiver))
		{
			if (hasAccepted)
			{
				GetRPCManager().SendRPC("PartyMe", "InvitationResponse", new Param2<string, bool>(sender, true));
			}
			else
			{
				GetRPCManager().SendRPC("PartyMe", "InvitationResponse", new Param2<string, bool>(sender, false));
			}
			RemovePlayerInvite(sender, receiver);
		}
	}
};