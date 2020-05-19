modded class MissionServer
{
    ref PM_RPC_S_handler                m_pm_rpc_handler;
	ref PM_S_Invitations				m_pm_invitations;

    void MissionServer()
    {
        InitPartyMe();
    }

    void InitPartyMe()
    {
        m_pm_rpc_handler = new PM_RPC_S_handler;
		m_pm_invitations = new PM_S_Invitations;
        PM_GetGroupManager();
        PM_Server_GetGroupPositionUpdater();
		GetRPCManager().AddRPC("PartyMe", "InitPlayerList", this, SingleplayerExecutionType.Both);
    }

    override void InvokeOnConnect(PlayerBase player, PlayerIdentity identity)
    {
        super.InvokeOnConnect(player, identity);

        PM_GetGroupManager().SetPlayerGroup(identity.GetId(), string.Empty);
        // PM_GetGroupManager().SetPlayerGroup(identity.GetId(), "FBD0-BhNyNXQdFzx_FWJg_BZ3i62spPr_8LaiiITEJs="); // For tests
        PM_GetGroupManager().SetPlayerEntity(identity.GetId(), player);
        PM_GetGroupManager().SetPlayerIdentity(identity.GetId(), identity);

        // RPCs
        //GetRPCManager().SendRPC("PartyMe", "PlayerJoinServer", new Param2<string, string>(identity.GetId(), identity.GetName()));
		GetRPCManager().SendRPC("PartyMe", "GroupSettings", new Param1<PM_Settings_Group>(GetServerSettings().group), false, identity);
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

	//--------------------------------------------------------------------------
	void InitPlayerList(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target)
    {
		ref array<string> playersId = new array<string>;
		ref array<string> playersName = new array<string>;
		ref array<Man> players = new array<Man>;
		GetGame().GetPlayers( players );
		
		for (int iMan = 0; iMan < players.Count(); iMan++)
		{
			PlayerIdentity playerIdentity = players.Get(iMan).GetIdentity();
			if (playerIdentity && playerIdentity.GetId() != sender.GetId())
			{
				playersId.Insert(playerIdentity.GetId());
				playersName.Insert(playerIdentity.GetName());
			}
		}
		GetRPCManager().SendRPC("PartyMe", "InitPlayerList", new Param2<array<string>, array<string>>(playersId, playersName), false, sender);
    }

	//-------------------------------------------------------------------------- Utils
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