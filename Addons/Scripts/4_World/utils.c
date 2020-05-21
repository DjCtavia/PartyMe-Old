bool PM_GetPlayerId(out string playerId)
{
    PlayerBase playerPB = PlayerBase.Cast(GetGame().GetPlayer());
    PlayerIdentity identity;

    if (playerPB)
    {
        identity = playerPB.GetIdentity();
        if (identity)
        {
            playerId = identity.GetId();
            return true;
        }
    }
    return false;
}

string PM_GetPlayerId()
{
	PlayerIdentity pId = GetGame().GetPlayer().GetIdentity();
	
	if (pId)
	{
		return pId.GetId();
	}
	return "";
}

bool PM_GetPlayerName(out string playerName)
{
    PlayerBase playerPB = PlayerBase.Cast(GetGame().GetPlayer());
    PlayerIdentity identity;

    if (playerPB)
    {
        identity = playerPB.GetIdentity();
        if (identity)
        {
            playerName = identity.GetName();
            return true;
        }
    }
    return false;
}

bool PM_GetPlayerIdAndName(out string playerId, out string playerName)
{
    PlayerBase playerPB = PlayerBase.Cast(GetGame().GetPlayer());
    PlayerIdentity identity;

    if (playerPB)
    {
        identity = playerPB.GetIdentity();
        if (identity)
        {
            playerId = identity.GetId();
			playerName = identity.GetName();
            return true;
        }
    }
    return false;
}