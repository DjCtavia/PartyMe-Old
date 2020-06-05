class PM_Group_Manager
{
    protected ref map<string, string> playerGroup; // Map of players and group assigned to.
    protected ref map<string, PlayerBase> playerEntities;
    protected ref map<string, PlayerIdentity> playerIdentities;
    protected ref map<string, ref array<string>> groups;

    void PM_Group_Manager()
    {
        playerGroup = new map<string, string>;
        playerEntities = new map<string, PlayerBase>;
        playerIdentities = new map<string, PlayerIdentity>;
        groups = new map<string, ref array<string>>;
		AddEvents();
    }
	
	void AddEvents()
	{
		PM_GetEvents().AddEvent("PlayerJoinGroup", this);
		PM_GetEvents().AddEvent("PlayerLeaveGroup", this);
		PM_GetEvents().AddEvent("PlayerDisconnect", this);
	}

	//-------------------------------------------------------------------------- User infos
	bool IsLeader(string playerId)
	{
		string ownerId = playerGroup.Get(playerId);
		
		if (ownerId != string.Empty && ownerId != playerId)
		{
			return false;
		}
		return true;
	}

	string GetPlayerGroupId(string playerId)
    {
        return playerGroup.Get(playerId);
    }

	PlayerBase GetPlayerEntity(string playerId)
    {
        return playerEntities.Get(playerId);
    }

	PlayerIdentity GetPlayerIdentity(string playerId)
    {
        /* Previous version
		return playerIdentities.Get(playerId);
		*/
		array<Man> players = new array<Man>;
		GetGame().GetWorld().GetPlayerList(players);

		for (int iPlayer = 0; iPlayer < players.Count(); iPlayer++)
		{
			PlayerIdentity playerIdentity = players[iPlayer].GetIdentity();

			if (!playerIdentity) continue;
			if (playerIdentity.GetId() == playerId)
				return playerIdentity;
		}
		return null;
    }

	ref map<string, PlayerIdentity> GetGroupIdentities(string ownerId)
	{
		array<string> members = new array<string>;
		ref map<string, PlayerIdentity> groupIdentities = new map<string, PlayerIdentity>;
		array<Man> players = new array<Man>;
		GetGame().GetWorld().GetPlayerList(players);

		if (!groups.Get(ownerId))
			return null;
		members.Copy(groups.Get(ownerId));
		for (int iPlayer = 0; iPlayer < players.Count(); iPlayer++)
		{
			PlayerIdentity playerIdentity = players[iPlayer].GetIdentity();
			if (!playerIdentity) continue;
			for (int iMember = 0; iMember < members.Count(); iMember++)
			{
				if (members[iMember] == playerIdentity.GetId())
				{
					groupIdentities.Set(members[iMember], playerIdentity);
					members.Remove(iMember);
					break;
				}
			}
		}
		return groupIdentities;
	}

	//-------------------------------------------------------------------------- Set user infos
	void SetPlayerEntity(string playerId, PlayerBase playerEntity)
    {
        playerEntities.Set(playerId, playerEntity);
    }

	void SetPlayerIdentity(string playerId, PlayerIdentity identity)
    {
        playerIdentities.Set(playerId, identity);
    }
	
	void SetPlayerGroup(string joinerId, string ownerId)
    {
		string groupOfOwnerId = playerGroup.Get(ownerId);
		ref array<string> groupArray = groups.Get(ownerId);

		if (!groupOfOwnerId || groupOfOwnerId == string.Empty)
			playerGroup.Set(ownerId, ownerId);
		if (!groupOfOwnerId || groupOfOwnerId == ownerId)
		{
			if (!groupArray)
			{
				groupArray = new array<string>;
				groupArray.Insert(ownerId);
				groups.Set(ownerId, groupArray);
			}
			if (groupArray.Find(joinerId) == -1)
			{
				playerGroup.Set(joinerId, ownerId);
				groupArray.Insert(joinerId);
				SendInformationsOfJoiningMember(joinerId, ownerId);
			}
			groupArray.Debug();
		}
    }

	//-------------------------------------------------------------------------- Action on user
	void CleanPlayer(string playerId)
   	{
		LeaveGroup(playerId);
		playerGroup.Remove(playerId);
		playerEntities.Remove(playerId);
		playerIdentities.Remove(playerId);
	}
	
	//-------------------------------------------------------------------------- Group infos
	array<string> GetAllGroups()
    {
        return groups.GetKeyArray();
    }

	ref array<string> GetGroupMembers(string ownerId)
    {
        return groups.Get(ownerId);
    }

	int GetGroupSize(string leaderId)
	{
		ref array<string> group = groups.Get(leaderId);
		
		if (group)
			return group.Count();
		return 0;
	}

	ref array<string> GetGroupMembersFromMember(string playerId)
    {
        return GetGroupMembers(GetPlayerGroupId(playerId));
    }

	//-------------------------------------------------------------------------- Action to group
	void SendGroupMemberInfos(PlayerIdentity toSendIdentity, PlayerIdentity memberIdentity, PlayerBase memberPB)
	{
		if (!toSendIdentity || !memberIdentity || !memberPB) return;
		
		string memberId = memberIdentity.GetId();
		string memberName = memberIdentity.GetName();
		vector memberPos = memberPB.GetPosition();
		float memberHealth = memberPB.GetHealth("", "");
		auto params = new Param4<string, string, vector, float>(memberId, memberName, memberPos, memberHealth);
		GetRPCManager().SendRPC("PartyMe", "PlayerJoinGroup", params, false, toSendIdentity);
	}
	
	//-------------------------------------------------------------------------- Action on group
	void LeaveGroup(string playerId)
	{
		string ownerId = playerGroup.Get(playerId);

		if (playerId == ownerId)
			DestroyGroup(ownerId);
		else
			RemovePlayerFromGroup(playerId);
	}

	void DestroyGroup(string ownerId)
	{
		ref array<string> group = groups.Get(ownerId);

		if (!group)
			return;
		while (group.Count())
		{
			PlayerIdentity memberIdentity = playerIdentities.Get(group[0]);

			playerGroup.Remove(group[0]);
			if (memberIdentity)
				GetRPCManager().SendRPC("PartyMe", "GroupDestroyed", NULL, false, memberIdentity);
			group.Remove(0);
		}
		groups.Remove(ownerId);
	}

	void RemovePlayerFromGroup(string playerId)
    {
		string ownerId = playerGroup.Get(playerId);
        ref array<string> group = groups.Get(ownerId);

        if (!group)
			return;
		for (int iMember = 0; iMember < group.Count(); iMember++)
		{
			PlayerIdentity memberIdentity = playerIdentities.Get(group[iMember]);

			if (memberIdentity)
			{
				if (memberIdentity.GetId() != playerId)
				{
					GetRPCManager().SendRPC("PartyMe", "PlayerLeaveGroup", new Param1<string>(playerId), false, memberIdentity);
				}
				else
				{
					playerGroup.Remove(group[iMember]);
					group.RemoveItem(group[iMember]);
					GetRPCManager().SendRPC("PartyMe", "GroupDestroyed", NULL, false, memberIdentity);
				}
			}
		}
		if (group.Count() == 1)
			DestroyGroup(ownerId);
    }

	//-------------------------------------------------------------------------- Action to members of a group
	void SendInformationsOfJoiningMember(string joinerId, string ownerId)
	{
		ref array<string> groupArray = groups.Get(ownerId);
		PlayerIdentity joinerIdentity = GetPlayerIdentity(joinerId);
		PlayerBase joinerPB = playerEntities.Get(joinerId);

		if (groupArray && joinerIdentity && joinerPB)
		{
			for (int iMember = 0; iMember < groupArray.Count(); iMember++)
			{
				if (groupArray[iMember] == joinerId)
					continue;
				PlayerIdentity memberIdentity = GetPlayerIdentity(groupArray[iMember]);
				PlayerBase memberPB = playerEntities.Get(groupArray[iMember]);

				SendGroupMemberInfos(joinerIdentity, memberIdentity, memberPB);
				SendGroupMemberInfos(memberIdentity, joinerIdentity, joinerPB);
			}
		}
	}

	//-------------------------------------------------------------------------- Events
	void OnPlayerJoinGroup(ref PM_Event_Params eventParams)
	{
		string ownerId = eventParams.playerIdTo;
		string joiningId = eventParams.playerIdFrom;

		Print("[PM][group_manager][OnPlayerJoinGroup] joinerId: " + joiningId + " | ownerId: " + ownerId);
		SetPlayerGroup(joiningId, ownerId);
	}

	void OnPlayerLeaveGroup(ref PM_Event_Params eventParams)
	{
		LeaveGroup(eventParams.playerIdFrom);
	}

	void OnPlayerDisconnect(ref PM_Event_Params eventParams)
	{
		CleanPlayer(eventParams.playerIdFrom);
	}
};

static ref PM_Group_Manager g_Group_Manager;
static ref PM_Group_Manager PM_GetGroupManager()
{
    if (!g_Group_Manager && GetGame().IsServer())
    {
        g_Group_Manager = new PM_Group_Manager();
    }
    return g_Group_Manager;
}