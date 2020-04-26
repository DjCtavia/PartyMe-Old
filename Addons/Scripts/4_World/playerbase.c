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
        ref array<string> playersInGroup = groupManager.GetGroup(groupManager.GetPlayerGroupId(playerKilledId));

        Print("[PM] Entered in ResetPlayerEntityRPC.");
        if (!playersInGroup)
            return;
        Print("[PM] Player is actually in a group.");
        foreach (string playerIdGroup : playersInGroup)
        {
            PlayerIdentity identity;
            if (playerIdGroup == playerKilledId)
                continue;
            identity = groupManager.GetPlayerIdentity(playerIdGroup);
            if (identity)
            {
                // Send RPC for reseting MAN
                Print("[PM] Player identity has been found, RPC has being sent to: " + identity.GetId());
                GetRPCManager().SendRPC("PartyMe", "ResetPlayerEntity", new Param1<string>(playerKilledId), false, identity);
            }
        }
        Print("[PM] Leaving ResetPlayerEntityRPC!");
    }
}