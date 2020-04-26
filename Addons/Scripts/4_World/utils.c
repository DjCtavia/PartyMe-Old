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