/**
 * Player informations structure,
 * mainly use for HUD and tracker.
*/
class PM_Player_Infos_t
{
    string name;
    string id;
    vector position;
    float health;
    ref PM_Marker_Widget w_marker;
    Man localPlayer;
    bool isLocal;

    void ~PM_Player_Infos_t()
    {
        Print("[PartyMe] Destroy PM_Player_Infos_t: " + id);
    }
};