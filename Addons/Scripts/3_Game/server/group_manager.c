class PM_Group_Manager
{
    protected ref map<string, string> playerGroup; // Map of players and group assigned to.
    protected ref map<string, ref array<string>> groups;

    void PM_Group_Manager()
    {
        playerGroup = new map<string, string>;
        groups = new map<string, ref array<string>>;
    }

    void SetPlayerGroup(string playerId, string groupOwnerId)
    {
        if (!playerGroup.Contains(playerId) || playerGroup.Get(playerId) == string.Empty || groupOwnerId == string.Empty)
        {
            playerGroup.Set(playerId, groupOwnerId);
            Print("[PM] Player " + playerId + " joined " + groupOwnerId + " group.");
        }
        else
        {
            Print("[PM] Player " + playerId + " can't be assigned to " + groupOwnerId + " group." + "Already in group of " + playerGroup.Get(playerId));
        }
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

    string GetPlayerGroupId(string playerId)
    {
        return playerGroup.Get(playerId);
    }
};