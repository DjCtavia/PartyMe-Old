class PM_InvitationsHandler
{
	ref map<string, ref array<string>> m_invitations;
	
	void PM_InvitationsHandler()
	{
		m_invitations = new map<string, ref array<string>>;
		AddEvents();
	}
	
	void AddEvents()
	{
		PM_GetEvents().AddEvent("OnPlayerDisconnect", this);
	}

	bool HasInvited(string sender, string receiver)
	{
		ref array<string> invitationsList = m_invitations.Get(receiver);

		if (!invitationsList)
		{
			m_invitations.Set(receiver, new array<string>);
			return false;
		}
		else if (invitationsList && invitationsList.Find(sender) > -1)
		{
			return true;
		}
		return false;
	}

	protected bool ConditionnalInvitation(string sender, string receiver) {return false;};
	protected void BeforeInvitation(string sender, string receiver) {};
	protected void AfterInvitation(string sender, string receiver) {};

	void InvitePlayer(string sender, string receiver)
	{
		if (!HasInvited(sender, receiver) && ConditionnalInvitation(sender, receiver))
		{
			ref array<string> invitationsList = m_invitations.Get(receiver);

			BeforeInvitation(sender, receiver);
			invitationsList.Insert(sender);
			AfterInvitation(sender, receiver);
		}
	}

	void RemovePlayerInvite(string sender, string receiver)
	{
		ref array<string> invitationsList = m_invitations.Get(receiver);

		if (invitationsList)
		{
			invitationsList.RemoveItem(sender);
		}
	}
	
	void CleanInvitationFromId(string sender)
	{
		for (int iReceiver = 0; iReceiver < m_invitations.Count(); iReceiver++)
		{
			ref array<string> invitations = m_invitations.GetElement(iReceiver);
			if (invitations)
				invitations.RemoveItem(sender);
		}
	}

	//-------------------------------------------------------------------------- Events
	void OnPlayerDisconnect(ref PM_Event_Params eventParams)
	{
		CleanInvitationFromId(eventParams.playerIdFrom);
	}
};