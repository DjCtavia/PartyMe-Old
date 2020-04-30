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
    }

    void AddRPCs()
    {
        GetRPCManager().AddRPC("PartyMe", "PlayerJoinGroup", this, SingleplayerExecutionType.Both);
        GetRPCManager().AddRPC("PartyMe", "PlayerLeaveGroup", this, SingleplayerExecutionType.Both);
        GetRPCManager().AddRPC("PartyMe", "PlayerKickedFromGroup", this, SingleplayerExecutionType.Both);
    }

    /*
        RPC functions

        Data:
            Param1: Player ID who want to join
            Param2: Group id to join
    */
    void PlayerJoinGroup(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target)
    {
        Param2<string, string> data;
        if (!ctx.Read( data )) return;


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
    void SetPlayerGroup(string playerId, string groupOwnerId)
    {
        if (!playerGroup.Contains(playerId) || playerGroup.Get(playerId) == string.Empty || groupOwnerId == string.Empty)
        {
            ref array<string> groupArray;

            playerGroup.Set(playerId, groupOwnerId);
            if (!groups.Contains(groupOwnerId))
            {
                groups.Set(groupOwnerId, new array<string>);
            }
            groupArray = groups.Get(groupOwnerId);
            if (groupArray.Find(playerId) == -1)
            {
                groupArray.Insert(playerId);
            }
            Print("[PM] Player " + playerId + " joined " + groupOwnerId + " group.");
        }
        else
        {
            Print("[PM] Player " + playerId + " can't be assigned to " + groupOwnerId + " group." + "Already in group of " + playerGroup.Get(playerId));
        }
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