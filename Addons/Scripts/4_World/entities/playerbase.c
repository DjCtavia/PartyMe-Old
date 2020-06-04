modded class PlayerBase
{
    override void EEKilled( Object killer )
    {
        super.EEKilled(killer);

        ResetPlayerEntityRPC(GetIdentity().GetId());
    }

    /*
        PartyMe section 
    */

   // Get all members of party, and set Man entity to null. (Except the player who died, can't update himself)
    void ResetPlayerEntityRPC(string playerKilledId)
    {
        PM_Group_Manager groupManager = PM_GetGroupManager();
        ref array<string> playersInGroup = groupManager.GetGroupMembersFromMember(playerKilledId);

        if (!playersInGroup)
            return;
        foreach (string playerIdGroup : playersInGroup)
        {
            PlayerIdentity identity;
            if (playerIdGroup == playerKilledId)
                continue;
            identity = groupManager.GetPlayerIdentity(playerIdGroup);
            if (identity)
            {
                // Send RPC for reseting MAN
                GetRPCManager().SendRPC("PartyMe", "ResetPlayerEntity", new Param1<string>(playerKilledId), false, identity);
            }
        }
    }
};