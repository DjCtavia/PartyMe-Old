class PM_InvitationsHandler
{
	ref map<string, ref array<string>> m_invitations;

	void PM_InvitationsHandler()
	{
		m_invitations = new map<string, ref array<string>>;
	}

	bool HasInvited(string sender, string receiver)
	{
		ref array<string> invitationsList = m_invitations.Get(receiver);
		
		if (invitationsList && invitationsList.Find(sender) > -1)
		{
			return true;
		}
		return false;
	}

	protected bool ConditionnalInvitation(string sender, string receiver) {return false};
	protected void BeforeInvitation(string sender, string receiver) {};
	protected void AfterInvitation(string sender, string receiver) {};

	void InvitePlayer(string sender, string receiver)
	{
		ref array<string> invitationsList = m_invitations.Get(receiver);
		
		if (invitationsList && !HasInvited(sender, receiver) && ConditionnalInvitation(sender, receiver))
		{
			BeforeInvitation(sender, receiver);
			invitationsList.Insert(sender);
			AfterInvitation(sender, receiver);
		}
	}

	void RemovePlayerInvite(string sender, string receiver)
	{
		ref array<string> invitationsList = m_invitations.Get(receiver);
		int inviteIndex;

		if (invitationsList)
		{
			inviteIndex = invitationsList.RemoveItem(sender);
		}
	}
};