class PM_C_Invitations extends PM_InvitationsHandler
{
	void PM_C_Invitations()
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

	bool HasInvited(string sender)
	{
		string receiverId;

		if (PM_GetPlayerId(receiverId))
		{
			return super.HasInvited(sender, receiverId);
		}
		else
		{
			if (true) // Debug mode
			{
				Print("[PartyMe][Error] Invitation has been received, but couldn't get PlayerId!");
			}
		}
		return false;
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
			RemovePlayerInvite(sender, receiver);
		}
	}
};