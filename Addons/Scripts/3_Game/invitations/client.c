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
};