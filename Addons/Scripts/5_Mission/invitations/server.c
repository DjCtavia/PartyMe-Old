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
		int groupSize = PM_GetGroupManager().GetGroupSize(sender);
		
		if (GetServerSettings().group.IsGroupFull(groupSize))
			return false;
		return true;
	}

	override void InvitePlayer(string sender, string receiver)
	{
		super.InvitePlayer(sender, receiver);
		PlayerIdentity receiverIdentity = MissionServer.Cast(GetGame().GetMission()).GetPlayerIdentity(receiver);
		PlayerIdentity senderIdentity = MissionServer.Cast(GetGame().GetMission()).GetPlayerIdentity(sender);
		
		if (HasInvited(sender, receiver))
		{
			if (senderIdentity && receiverIdentity)
			{
				GetRPCManager().SendRPC("PartyMe", "InvitationReceived", new Param2<string, string>(sender, senderIdentity.GetName()), false, receiverIdentity);
			}
			else if (!receiverIdentity && true) // debug mode
			{
				Print("[PartyMe][Error] " + sender + " tried to invite " + receiver + " but PlayerIdentity has not be found!");
			}
		}
	}

	override void AfterInvitation(string sender, string receiver)
	{
		super.AfterInvitation(sender, receiver);
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
		PlayerIdentity newMemberIdentity = PM_GetGroupManager().GetPlayerIdentity(receiver);

		Print("[PartyMe][group_manager][OnPlayerJoinGroup] joinerId: " + eventParams.playerIdFrom + " | ownerId: " + eventParams.playerIdTo);
		if (hasAccepted && HasInvited(sender, receiver) && ConditionnalInvitation(sender, receiver))
		{
			Print("[PartyMe] " + receiver + " join group of " + sender);
			PM_GetEvents().CallEvent("PlayerJoinGroup", eventParams);
			GetRPCManager().SendRPC("PartyMe", "SetMember", NULL, false, newMemberIdentity);
		}
		RemovePlayerInvite(sender, receiver);
	}
};