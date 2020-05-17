class PM_Settings_Group
{
	int limit = 5;
	bool canShowHUD = true;

	void Copy(PM_Settings_Group toCopy)
	{
		limit = toCopy.limit;
		canShowHUD = toCopy.canShowHUD;
	}

	bool IsGroupFull(int numberOfPlayersInGroup)
	{
		numberOfPlayersInGroup += 1;
		if (limit < 0)
		{
			return false;
		}
		else if (numberOfPlayersInGroup < limit)
		{
			return false;
		}
		return true;
	}
};

static ref PM_Settings_Group g_PM_settings_group;
static ref PM_Settings_Group PM_GetGroupSettings()
{
    if (!g_PM_settings_group)
    {
        g_PM_settings_group = new PM_Settings_Group();
    }
    return g_PM_settings_group;
}