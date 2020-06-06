class PM_Settings_Group
{
	int limit = 5;
	bool canShowHUD = true;
	
	void PM_Settings_Group()
	{
		AddRPC();
	}
	
	void AddRPC()
	{
		if (GetGame().IsClient())
		{
			GetRPCManager().AddRPC("PartyMe", "GroupSettings", this, SingleplayerExecutionType.Both);
		}
	}

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
		else if (numberOfPlayersInGroup <= limit)
		{
			return false;
		}
		return true;
	}

	/* RPCs

        Description:
            Save group server settings on client
        Data:
            Param1: Group settings
    */
	void GroupSettings(CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target)
    {
		Param1<PM_Settings_Group> data;
        if (!ctx.Read(data)) return;

		Copy(data.param1);
		Print("[PartyMe][settings] Received.");
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