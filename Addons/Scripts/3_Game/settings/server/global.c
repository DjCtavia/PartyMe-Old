class PM_S_settings
{
    private const static string SETTINGS_PATH = "$profile:PartyMe/config.json";

    // Group
    bool DebugMode = false;
	ref PM_Settings_Group group;

    void Load()
    {
		if (FileExist(SETTINGS_PATH))
		{
			JsonFileLoader<PM_S_settings>.JsonLoadFile(SETTINGS_PATH, this);
			Print("[PartyMe][settings] Config has been loaded.");
		}
		else
		{
			Create();
		}
    }

    void Save()
    {
        JsonFileLoader<PM_S_settings>.JsonSaveFile(SETTINGS_PATH, this);
    }

    private void Create()
    {
		Print("[PartyMe][settings] Default config has been created and loaded.");
		group = new PM_Settings_Group;
        Save();
        Load();
    }
};

static ref PM_S_settings g_server_settings;
static ref PM_S_settings GetServerSettings()
{
	if (GetGame().IsServer() && !g_server_settings)
	{
		g_server_settings = new PM_S_settings();
		g_server_settings.Load();
	}
	return g_server_settings;
}