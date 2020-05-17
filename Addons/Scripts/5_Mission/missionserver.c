modded class MissionServer
{
    ref PM_S_settings                   m_settings;
    ref PM_RPC_S_handler                m_pm_rpc_handler;

    void MissionServer()
    {
        InitPartyMe();
    }

    void InitPartyMe()
    {
        m_settings = new PM_S_settings;
        m_settings.Load();
        m_pm_rpc_handler = new PM_RPC_S_handler;
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
        auto params = new Param2<string, string>(identity.GetId(), identity.GetName());
        GetRPCManager().SendRPC("PartyMe", "PlayerJoinServer", params);
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
	
	// Utils
	PlayerIdentity GetPlayerIdentity(string playerId)
	{
		for (int playerIndex = 0; playerIndex < m_Players.Count(); playerIndex++)
		{
			PlayerIdentity pIdentity = m_Players.Get(playerIndex).GetIdentity();
			
			if (pIdentity && pIdentity.GetId() == playerId)
			{
				return pIdentity;
			}
		}
		return null;
	}
};