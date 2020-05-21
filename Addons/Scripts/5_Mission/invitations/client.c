class PM_C_Invitations extends PM_InvitationsHandler
{
	void PM_C_Invitations()
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
		ref PM_Event_Params eventParams = new PM_Event_Params;

		eventParams.playerIdFrom = sender;
		if (PM_GetPlayerUtilities().GetPlayerName(sender, eventParams.name))
		{
			PM_GetEvents().CallEvent("InvitationReceivedUI", eventParams);
		}
	}
	
	//-------------------------------------------------------------------------- Events
	void OnInvitationReceived(ref PM_Event_Params eventParams)
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
			RemovePlayerInvite(sender, receiver);
		}
	}
};