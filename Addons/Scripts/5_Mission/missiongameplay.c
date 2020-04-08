/**
 * Second account uid: FBD0-BhNyNXQdFzx_FWJg_BZ3i62spPr_8LaiiITEJs= 
*/
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

        PM_GetGroup().AddPlayer("FBD0-BhNyNXQdFzx_FWJg_BZ3i62spPr_8LaiiITEJs=", "Bobby", "0 0 0", 100);
    }
};