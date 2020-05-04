modded class MissionServer
{
    void MissionServer()
    {
        PM_GetGroupManager();
        PM_Server_GetGroupPositionUpdater();
    }

    override void InvokeOnConnect(PlayerBase player, PlayerIdentity identity)
    {
        super.InvokeOnConnect(player, identity);

        PM_GetGroupManager().SetPlayerGroup(identity.GetId(), string.Empty);
        PM_GetGroupManager().SetPlayerGroup(identity.GetId(), "FBD0-BhNyNXQdFzx_FWJg_BZ3i62spPr_8LaiiITEJs="); // For tests
        PM_GetGroupManager().SetPlayerEntity(identity.GetId(), player);
        PM_GetGroupManager().SetPlayerIdentity(identity.GetId(), identity);

        // tests
        auto params = new Param4<string, string, vector, float>(identity.GetId(), identity.GetName(), player.GetPosition(), player.GetHealth("",""));
        GetRPCManager().SendRPC("PartyMe", "PlayerConnect", params);
    }

    override void InvokeOnDisconnect(PlayerBase player)
	{
        super.InvokeOnDisconnect(player);

        if (player)
        {
            PlayerIdentity identity = player.GetIdentity();

            if (identity)
            {
                PM_GetGroupManager().CleanPlayer(identity.GetId());
            }
        }
	}

    override void OnClientRespawnEvent(PlayerIdentity identity, PlayerBase player)
    {
        super.OnClientRespawnEvent(identity, player);

        // We reasign the PlayerBase for constant update later.
        PM_GetGroupManager().SetPlayerEntity(identity.GetId(), player);
    }
};