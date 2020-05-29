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
        return playerIdentities.Get(playerId);
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
        if (!playerGroup.Contains(joinerId) || playerGroup.Get(joinerId) == string.Empty || ownerId == string.Empty)
        {
            ref array<string> groupArray;

            if (!groups.Contains(ownerId))
            {
				groupArray = new array<string>;
				groupArray.Insert(ownerId);
                groups.Set(ownerId, groupArray);
				playerGroup.Set(ownerId, ownerId);
            }
			groupArray = groups.Get(ownerId);
            if (groupArray.Find(joinerId) == -1)
            {
                groupArray.Insert(joinerId);
				SendInformationsOfJoiningMember(joinerId, ownerId);
            }
            Print("[PM] Player " + joinerId + " joined " + ownerId + " group.");
        }
    }
	
	//-------------------------------------------------------------------------- Action on user
	void CleanPlayer(string playerId)
   	{
		string ownerId = playerGroup.Get(playerId);

		playerGroup.Remove(playerId);
		playerEntities.Remove(playerId);
		playerIdentities.Remove(playerId);
		if (playerId == ownerId)
			DestroyGroup(ownerId);
		else
		{
			ref array<string> group = groups.Get(ownerId);

			if (group)
				group.RemoveItem(playerId);
		}
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
	void KickFromGroup(string playerIdAskingKick, string playerIdToKick)
	{
		if (!IsLeader(playerIdAskingKick) || playerGroup.Get(playerIdToKick) != playerIdAskingKick)
			return;
		LeaveGroup(playerIdToKick);
	}
	
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
		for (int iMember = 0; iMember < group.Count(); iMember++)
			RemovePlayerFromGroup(group[iMember]);
		groups.Remove(ownerId);
	}

	void RemovePlayerFromGroup(string playerId)
    {
        ref array<string> group = GetGroupMembersFromMember(playerId);
		auto params = new Param1<string>(playerId);
		PlayerIdentity memberIdentity;

        if (!group)
			return;
		for (int iMember = 0; iMember < group.Count(); iMember++)
		{
			memberIdentity = playerIdentities.Get(group[iMember]);
			if (memberIdentity)
				GetRPCManager().SendRPC("PartyMe", "PlayerLeaveGroup", params, false, memberIdentity);
		}
		group.RemoveItem(playerId);
		playerGroup.Set(playerId, string.Empty);
    }

	//-------------------------------------------------------------------------- Action to members of a group
	void SendInformationsOfJoiningMember(string joinerId, string ownerId)
	{
		ref array<string> groupArray = groups.Get(ownerId);
		PlayerIdentity joinerIdentity = playerIdentities.Get(joinerId);
		PlayerBase joinerPB = playerEntities.Get(joinerId);

		if (groupArray && joinerIdentity && joinerPB)
		{
			for (int iMember = 0; iMember < groupArray.Count(); iMember++)
			{
				if (groupArray[iMember] == joinerId)
					continue;
				PlayerIdentity memberIdentity = playerIdentities.Get(groupArray[iMember]);
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

		SetPlayerGroup(joiningId, ownerId);
	}
	
	void OnPlayerKick(ref PM_Event_Params eventParams)
	{
		KickFromGroup(eventParams.playerIdFrom, eventParams.playerIdTo);
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