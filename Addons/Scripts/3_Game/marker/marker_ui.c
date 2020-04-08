/**
 * Contain all elements widget used for the marker.
 * 
 * Note:
 *      Anyone who want a custom tracker from scratch should modify
 *      this class.
*/
class PM_marker_ui
{
    ref Widget w_root;
    ref ImageWidget i_logo;
    ref TextWidget t_playerName;

    void PM_marker_ui()
    {
        InitRoot();
        InitWidgets();
    }

    // We're loading the layout of marker in this method
    void InitRoot()
    {
        w_root = GetGame().GetWorkspace().CreateWidgets("partyme/gui/layouts/tracker.layout");
    }

    // We're creating widget references here based on Widger 'w_root'.
    void InitWidgets()
    {
        i_logo = ImageWidget.Cast(w_root.FindAnyWidget("i_logo"));
        t_playerName = TextWidget.Cast(w_root.FindAnyWidget("t_playerName"));;
    }

    void UpdateFromPlayerInfoStruct(ref PM_Player_Infos_t playerInfos)
    {
        if (playerInfos)
        {
            t_playerName.SetText(playerInfos.name);
            UpdateVisualPositioning(playerInfos.position);
        }
    }

    // Move marker on screen at the new world position given
    private void UpdateVisualPositioning(vector newPosition)
    {
        float widgetWidth, widgetHeight;
        vector screenPosition = GetGame().GetScreenPos(position);

        w_root.GetScreenSize(widgetWidth, widgetHeight);
        if (IsMarkerVisibleOnScreen(newPosition))
            return;
        w_root.SetPos(screenPosition[0] - (widgetWidth / 2), screenPosition[1] - widgetHeight);
    }

    // Need to verify if we can ignore this
    private bool IsMarkerVisibleOnScreen(vector newPosition)
    {
        vector screenPosRelative = GetGame().GetScreenPosRelative(newPosition);

		if( screenPosRelative[0] >= 1 || screenPosRelative[0] == 0 || screenPosRelative[1] >= 1 || screenPosRelative[1] == 0 ) {
			w_root.Show(false);
			return false;
		} else if( screenPosRelative[2] < 0 ) {
			w_root.Show(false);
			return false;
		}
		w_root.Show( true );
		return true;
    }
};