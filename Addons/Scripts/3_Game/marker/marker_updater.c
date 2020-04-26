/**
 * Main element for updating marker positions
*/
class PM_Marker_Updater
{
    private ScriptCallQueue timer;

    void PM_Marker_Updater()
    {
        timer = GetGame().GetCallQueue(CALL_CATEGORY_GUI);
        timer.CallLater(Update, 0, true);
    }

    void Update()
    {
        ref PM_Group group = PM_GetGroup();
        ref map<string, ref PM_Player_Infos_t> players;
        ref PM_Player_Infos_t pInfos;

        if (group)
        {
            players = group.players;
            if (players)
            {
                for (int player = 0; player < players.Count() ; player++)
                {
                    pInfos = players.GetElement(player);
                    pInfos.w_marker.UpdateFromPlayerInfoStruct(pInfos);
                }
            }
        }
    }
};

static ref PM_Marker_Updater g_PM_marker_updater;
static ref PM_Marker_Updater PM_GetMarkerUpdater()
{
    if (!g_PM_marker_updater)
    {
        g_PM_marker_updater = new PM_Marker_Updater();
    }
    return g_PM_marker_updater;
}