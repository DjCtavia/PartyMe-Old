bool PM_GetPlayerId(out string playerId)
{
    PlayerIdentity identity = GetGame().GetPlayer().GetIdentity();

    if (identity)
    {
        playerId = identity.GetId();
        return true;
    }
    return false;
}