modded class MissionGameplay
{
    ref PM_Group_Position_Updater groupPositionUpdater;

    void MissionGameplay()
    {
        groupPositionUpdater = new PM_Group_Position_Updater();
    }
};