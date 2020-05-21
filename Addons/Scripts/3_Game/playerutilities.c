class PM_PlayerUtilities
{
	ref map<string, PlayerIdentity>			m_playerIdentities;

	void PM_PlayerUtilities()
	{
		m_playerIdentities = new map<string, PlayerIdentity>;
		InitPlayerIdentities();
		AddEvents();
	}
	
	void AddEvents()
	{
		PM_GetEvents().AddEvent("PlayerJoinServer", this);
		PM_GetEvents().AddEvent("PlayerLeaveServer", this);
	}
	
	void InitPlayerIdentities()
	{
		ref array<PlayerIdentity> players = new array<PlayerIdentity>;

		GetGame().GetPlayerIndentities(players);
		for (int i = 0; i < players.Count(); i++)
		{
			PlayerIdentity playerIdentity = players[i];
			string playerId = playerIdentity.GetId();

			m_playerIdentities.Set(playerId, playerIdentity);
		}
	}

	//-------------------------------------------------------------------------- Utilities
	bool GetPlayerName(string playerId, out string playerName)
	{
		PlayerIdentity identity = m_playerIdentities.Get(playerId);

		if (identity)
		{
			playerName = identity.GetName();
			return true;
		}
		return false;
	}

	PlayerIdentity GetPlayerIdentity(string playerId)
	{
		return m_playerIdentities.Get(playerId);
	}

	//-------------------------------------------------------------------------- Events
	void OnPlayerJoinServer(ref PM_Event_Params eventParams)
	{
		ref array<PlayerIdentity> players = new array<PlayerIdentity>;

		GetGame().GetPlayerIndentities(players);
		for (int i = 0; i < players.Count(); i++)
		{
			PlayerIdentity playerIdentity = players[i];
			string playerId = playerIdentity.GetId();

			if (playerId == eventParams.playerIdFrom)
			{
				m_playerIdentities.Set(playerId, playerIdentity);
				return;
			}
		}
	}
	
	void OnPlayerLeaveServer(ref PM_Event_Params eventParams)
	{
		m_playerIdentities.Remove(eventParams.playerIdFrom);
	}
};

static ref PM_PlayerUtilities g_pm_playerUtilities;
static ref PM_PlayerUtilities PM_GetPlayerUtilities()
{
	if (!g_pm_playerUtilities)
	{
		g_pm_playerUtilities = new PM_PlayerUtilities();
	}
	return g_pm_playerUtilities;
}