/* ScriptData
Name: caio_commandscript
%Complete : 100
Comment : All AIO related server side commands
Category : commandscripts
EndScriptData */

#include "Player.h"
#include "ScriptMgr.h"
#include "World.h"
#include "Language.h"

using namespace Acore::ChatCommands;

class caio_commandscript : public CommandScript
{
public:
    caio_commandscript()
        : CommandScript("caio_commandscript")
    {}

    ChatCommandTable GetCommands() const override
    {
        static ChatCommandTable caioCommandTable =
        {
            { "version",        HandleVersionCommand,      SEC_GAMEMASTER, Console::No },
            { "send",           HandleSendCommand,         SEC_GAMEMASTER, Console::No },
            { "forcereload",    HandleReloadCommand,       SEC_GAMEMASTER, Console::No },
            { "forcereset",     HandleResetCommand,        SEC_GAMEMASTER, Console::No },
            { "sendall",        HandleSendAllCommand,      SEC_GAMEMASTER, Console::No },
            { "forcereloadall", HandleReloadAllCommand,    SEC_GAMEMASTER, Console::No },
            { "forceresetall",  HandleResetAllCommand,     SEC_GAMEMASTER, Console::No },
            { "reloadaddons",   HandleReloadAddonsCommand, SEC_GAMEMASTER, Console::No },
            { "addaddon",       HandleAddAddonCommand,     SEC_GAMEMASTER, Console::No },
            { "removeaddon",    HandleRemoveAddonCommand,  SEC_GAMEMASTER, Console::No }
        };
        static ChatCommandTable commandTable =
        {
            { "caio", caioCommandTable }
        };
        return commandTable;
    }

    static bool HandleVersionCommand(ChatHandler* handler, char const* args)
    {
        handler->PSendSysMessage("AIO version %u.", AIO_VERSION);
        return true;
    }

    static bool HandleSendCommand(ChatHandler* handler, char const* args)
    {
        //Player name
        Player* target;
        if (!handler->extractPlayerTarget((char*)args, &target))
            return false;

        //Quoted message
        char* tailStr = strtok(NULL, "");
        if (!tailStr)
            return false;

        char* msg = handler->extractQuotedArg(tailStr);
        if (!msg)
            return false;

        target->SendSimpleAIOMessage(msg);
        handler->PSendSysMessage(LANG_SENDMESSAGE, target->GetName().c_str(), msg);
        return true;
    };

    static bool HandleReloadCommand(ChatHandler* handler, char const* args)
    {
        Player* target = 0;
        if (!handler->extractPlayerTarget((char*)args, &target, 0, 0))
            return false;

        target->ForceReloadAddons();
        handler->PSendSysMessage(LANG_CAIO_FORCERELOAD_SENT, target->GetName().c_str());
        return true;
    };

    static bool HandleResetCommand(ChatHandler* handler, char const* args)
    {
        Player* target = 0;
        if (!handler->extractPlayerTarget((char*)args, &target, 0, 0))
            return false;

        target->ForceResetAddons();
        handler->PSendSysMessage(LANG_CAIO_FORCERESET_SENT, target->GetName().c_str());
        return true;
    };

    static bool HandleSendAllCommand(ChatHandler* handler, char const* args)
    {
        if (!*args)
            return false;

        //Get message
        char* msg = handler->extractQuotedArg((char*)args);
        if (!msg)
            return false;

        char* permission = strtok(NULL, "");
        uint32 perm = 195;
        if (permission) //Get permission if its there
        {
            try
            {
                perm = std::stoi(args);
            }
            catch (std::exception&)
            {
                return false;
            }
        }

        sWorld->SendAllSimpleAIOMessage(msg);
        handler->PSendSysMessage(LANG_SENDMESSAGE, "all players", msg);
        return true;
    };

    static bool HandleReloadAllCommand(ChatHandler* handler, char const* args)
    {
        uint32 perm = 195;
        if (args && *args) //Get permission if its there
        {
            try
            {
                perm = std::stoi(args);
            }
            catch (std::exception&)
            {
                return false;
            }
        }

        //Force reload required players
        sWorld->ForceReloadPlayerAddons();
        handler->PSendSysMessage(LANG_CAIO_FORCERELOAD_SENT, "all players");
        return true;
    };

    static bool HandleResetAllCommand(ChatHandler* handler, char const* args)
    {
        uint32 perm = 195;
        if (args && *args) //Get permission if its there
        {
            try
            {
                perm = std::stoi(args);
            }
            catch (std::exception&)
            {
                return false;
            }
        }

        //Force reset required players
        sWorld->ForceResetPlayerAddons();
        handler->PSendSysMessage(LANG_CAIO_FORCERESET_SENT, "all players");
        return true;
    };

    static bool HandleReloadAddonsCommand(ChatHandler* handler, char const* args)
    {

        //Reload and force reload required players
        bool success = sWorld->ReloadAddons();
        if (success)
        {
            sWorld->ForceReloadPlayerAddons();
        }
        else
        {
            handler->SendSysMessage(LANG_CAIO_RELOADADDONS_ERROR);
        }
        return true;
    }

    static bool HandleAddAddonCommand(ChatHandler* handler, char const* args)
    {
        if (!*args)
            return false;

        //Addon name
        char* addonName = strtok((char*)args, " ");
        if (!addonName || addonName[0] == '"')
            return false;

        //File
        char* tailStr = strtok(NULL, "");
        char* addonFile = handler->extractQuotedArg(tailStr);
        if (!addonFile)
            return false;

        //Permission
        char* permission = strtok(NULL, "");
        uint32 perm = 195;
        if (permission)
        {
            try
            {
                perm = std::stoi(permission);
            }
            catch (std::exception&)
            {
                return false;
            }
        }

        //Add
        AIOAddon newAddon(addonName, addonFile);
        bool added = sWorld->AddAddon(newAddon);
        if (added)
        {
            sWorld->ForceReloadPlayerAddons();
        }
        else
        {
            handler->PSendSysMessage(LANG_CAIO_ADDADDON_ERROR, addonName);
        }
        return true;
    }

    static bool HandleRemoveAddonCommand(ChatHandler* handler, char const* args)
    {
        if (!*args)
            return false;

        //Remove and reload required players
        if (sWorld->RemoveAddon(args))
        {
            sWorld->ForceReloadPlayerAddons();
        }
        else
        {
            handler->PSendSysMessage(LANG_CAIO_REMOVEADDON_ERROR, args);
        }
        return true;
    }
};

void AddSC_caio_commandscript()
{
    new caio_commandscript();
}
