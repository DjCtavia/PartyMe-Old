modded class MissionGameplay
{
    void MissionGameplay()
    {
        InitPartyMe();
    }

    void InitPartyMe()
    {
        PM_GetGroupConstraint();
        PM_GetGroup();
        PM_GetGroupPositionUpdater();
    }
};