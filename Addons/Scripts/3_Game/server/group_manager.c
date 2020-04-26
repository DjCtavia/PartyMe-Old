class PM_Group_Manager
{
    protected ref map<string, string> playerGroup; // Map of players and group assigned to.
    protected ref map<string, PlayerIdentity> playerIdentities;
    protected ref map<string, ref array<string>> groups;

    void PM_Group_Manager()
    {
        playerGroup = new map<string, string>;
        playerIdentities = new map<string, PlayerIdentity>;
        groups = new map<string, ref array<string>>;
    }

    void PlayerLeaveGroup(string playerId)
    {
        ref array<string> group = groups.Get(playerGroup.Get(playerId));

        if (group)
        {
            group.RemoveItem(playerId);
            // Send RPC to party members for informing player left.
        }
    }

    /*
        Getter / Setter
    */

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

    string GetPlayerGroupId(string playerId)
    {
        return playerGroup.Get(playerId);
    }

    void SetPlayerIdentity(string playerId, PlayerIdentity identity)
    {
        playerIdentities.Set(playerId, identity);
    }

    PlayerIdentity GetPlayerIdentity(string playerId)
    {
        return playerIdentities.Get(playerId);
    }

    ref array<string> GetGroup(string groupId)
    {
        return groups.Get(groupId);
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