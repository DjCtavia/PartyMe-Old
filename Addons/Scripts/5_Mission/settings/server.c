class PM_S_settings
{
    private const static string SETTINGS_PATH = "$profile:PartyMe/config.json";

    // Group
    bool DebugMode = false;
    int MaxPartySize = 5;

    void Load()
    {
        if (FileExist(SETTINGS_PATH))
        {
            JsonFileLoader<PM_S_settings>.JsonLoadFile(SETTINGS_PATH, this);
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
        Save();
        Load();
    }
};