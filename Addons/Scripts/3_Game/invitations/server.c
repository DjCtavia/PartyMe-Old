class PM_S_Invitations extends PM_InvitationsHandler
{
	void PM_S_Invitations()
	{
		AddEvents();
	}

	void AddEvents()
	{
		PM_GetEvents().AddEvent("InvitationReceive", this);
		PM_GetEvents().AddEvent("InvitationResponse", this);
	}

	override bool ConditionnalInvitation(string sender, string receiver)
	{
		// Check size of group
		return true;
	}

	override void AfterInvitation(string sender, string receiver)
	{
		PlayerIdentity receiverIdentity = GetMission().GetPlayerIdentity(receiver);
		
		if (receiverIdentity)
		{
			GetRPCManager().SendRPC("PartyMe", "ReceiveInvite", new Param1<string>(sender));
		}
		else if (!receiverIdentity && true) // debug mode
		{
			Print("[PartyMe][Error] " + sender + " tried to invite " + receiver + " but PlayerIdentity has not be found!");
		}
	}
	
	// Events
	void OnInvitationReceive(ref PM_Event_Params eventParams)
	{
		string sender = eventParams.playerIdFrom;
		string receiver = eventParams.playerIdTo;

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
				GetRPCManager().SendRPC("PartyMe", "InviteResponse", new Param2<string, bool>(sender, true));
			}
			else
			{
				GetRPCManager().SendRPC("PartyMe", "InviteResponse", new Param2<string, bool>(sender, false));
			}
			RemovePlayerInvite(sender, receiver);
		}
	}
};