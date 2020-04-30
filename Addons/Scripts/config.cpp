class CfgPatches
{
	class partyme_scripts
	{
		units[]={};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Data",
			"RPC_Scripts"
		};
	};
};

class CfgMods
{
	class partyme
	{
		dir = "@PartyMe";
		picture = "";
		action = "";
		hideName = 1;
		hidePicture = 1;
		name = "Party Me";
		credits = "OP, Baroness, GrosTon1, Thurston";
		author = "DjCtavia";
		authorID = "0"; 
		version = "7"; 
		extra = 0;
		type = "mod";
		inputs = "partyme/scripts/partyme_inputs.xml";
		
		dependencies[] = {"Game", "Mission", "World", "gui"};

		class defs
		{
			class gameScriptModule
			{
				value = "";
				files[] = {"partyme/scripts/3_Game"};
			}

			class worldScriptModule
			{
				value = "";
				files[] = {"partyme/scripts/4_World"};
			};

			class missionScriptModule
			{
				value = "";
				files[] = {"partyme/scripts/5_Mission"};
			};
		};
	};
};