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
		AddRPCs();
		AddEvents();
    }

    void AddRPCs()
    {
        GetRPCManager().AddRPC("PartyMe", "PlayerLeaveGroup", this, SingleplayerExecutionType.Both);
        GetRPCManager().AddRPC("PartyMe", "PlayerKickedFromGroup", this, SingleplayerExecutionType.Both);
    }
	
	void AddEvents()
	{
		PM_GetEvents().AddEvent("PlayerJoinGroup", this);
	}

    /*
        Data:
            Param1: Identifier of the player leaving the group
    */
    void PlayerLeaveGroup(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target)
    {
        Param1<string> data;
        if (!ctx.Read( data )) return;

        
    }

    /*
        Data:
            Param1: Player leaving the group
            Param2: Player to kick
    */
    void PlayerKickedFromGroup(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target)
    {
        Param2<string, string> data;
        if (!ctx.Read(data)) return;

        
    }

    /*
        Getter / Setter
    */
   // Mostly used when player disconnect
   void CleanPlayer(string playerId)
   {
       playerGroup.Remove(playerId);
       playerEntities.Remove(playerId);
       playerIdentities.Remove(playerId);
       //groups
   }

    /*****
     *  Player group
    *****/
    // Attribute a group to player
    void SetPlayerGroup(string joinerId, string groupOwnerId)
    {
        if (!playerGroup.Contains(joinerId) || playerGroup.Get(joinerId) == string.Empty || groupOwnerId == string.Empty)
        {
            ref array<string> groupArray;

            if (!groups.Contains(groupOwnerId))
            {
				groupArray = new array<string>;
				groupArray.Insert(groupOwnerId);
                groups.Set(groupOwnerId, groupArray);
				playerGroup.Set(groupOwnerId, groupOwnerId);
            }
			groupArray = groups.Get(groupOwnerId);
            if (groupArray.Find(joinerId) == -1)
            {
                groupArray.Insert(joinerId);
				UpdateGroup(joinerId, groupOwnerId);
            }
            Print("[PM] Player " + joinerId + " joined " + groupOwnerId + " group.");
        }
        else
        {
            Print("[PM] Player " + joinerId + " can't be assigned to " + groupOwnerId + " group." + "Already in group of " + playerGroup.Get(joinerId));
        }
    }

	// Send information of new player to all players already in group, and group members to the new player
	void UpdateGroup(string joinerId, string groupOwnerId)
	{
		ref array<string> groupArray = groups.Get(groupOwnerId);
		PlayerIdentity joinerIdentity = playerIdentities.Get(joinerId);
		PlayerBase joinerPB = playerEntities.Get(joinerId);

		Print("[PartyMe][Event] UpdateGroup | " + groupArray + " | " + joinerIdentity + " | " + joinerPB);
		if (groupArray && joinerIdentity && joinerPB)
		{
			for (int iMember = 0; iMember < groupArray.Count(); iMember++)
			{
				if (groupArray[iMember] == joinerId) continue;
				PlayerIdentity memberIdentity = playerIdentities.Get(groupArray[iMember]);
				PlayerBase memberPB = playerEntities.Get(groupArray[iMember]);

				Print("[PartyMe][GroupManager] Sending | " + joinerIdentity + " | " + joinerPB + " | " + memberIdentity + " | " + memberPB);
				SendGroupMemberInfos(joinerIdentity, memberIdentity, memberPB);
				SendGroupMemberInfos(memberIdentity, joinerIdentity, joinerPB);
			}
		}
	}
	
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

    // Remove a player from a group
    void RemovePlayerGroup(string playerId)
    {
        ref array<string> group = GetGroupMembersFromMember(playerId);

        if (group)
        {
            int indexInGroup = group.Find(playerId);

            if (indexInGroup != -1)
            {
                group.Remove(indexInGroup);
                // Send RPC to players in same party

            }
            playerGroup.Set(playerId, string.Empty);
        }
    }

    // Get group assigned to a player
    string GetPlayerGroupId(string playerId)
    {
        return playerGroup.Get(playerId);
    }

    /*****
     *  Player entity
    *****/
    void SetPlayerEntity(string playerId, PlayerBase playerEntity)
    {
        playerEntities.Set(playerId, playerEntity);
    }

    PlayerBase GetPlayerEntity(string playerId)
    {
        return playerEntities.Get(playerId);
    }

    /*****
     *  Player identity
    *****/ 
    void SetPlayerIdentity(string playerId, PlayerIdentity identity)
    {
        playerIdentities.Set(playerId, identity);
    }

    PlayerIdentity GetPlayerIdentity(string playerId)
    {
        return playerIdentities.Get(playerId);
    }

    /*****
     *  Player members in a group
    *****/
    ref array<string> GetGroupMembers(string groupId)
    {
        return groups.Get(groupId);
    }

    array<string> GetAllGroups()
    {
        return groups.GetKeyArray();
    }

    ref array<string> GetGroupMembersFromMember(string playerId)
    {
        return GetGroupMembers(GetPlayerGroupId(playerId));
    }

	bool IsLeader(string playerId)
	{
		string leaderId = playerGroup.Get(playerId);
		
		if (leaderId != string.Empty && leaderId != playerId)
		{
			return false;
		}
		return true;
	}
	
	int GetGroupSize(string leaderId)
	{
		ref array<string> group = groups.Get(leaderId);
		
		if (group)
		{
			return group.Count();
		}
		return 0;
	}
	
	//-------------------------------------------------------------------------- Events
	void OnPlayerJoinGroup(ref PM_Event_Params eventParams)
	{
		string ownerId = eventParams.playerIdTo;
		string joiningId = eventParams.playerIdFrom;

		Print("[PartyMe][Event] OnPlayerJoinGroup has been called!");
		SetPlayerGroup(joiningId, ownerId);
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