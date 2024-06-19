#include "savefile.h"
#include <filesystem>
#include <iostream>
#include <fstream>
#include <exception>

namespace savefile
{
    bool initialized = false;
    bool dirtyPeek = true;
    std::filesystem::path savefilePath;
    std::vector<WeakReplay> peek;

    constexpr const char* filename = "cod4recorder.recs";

    void Init()
    {
        char buf[32U]; size_t used;
        getenv_s<sizeof(buf)>(&used, buf, "USERPROFILE"); buf[31U] = '\0';

        savefilePath = std::filesystem::path(std::string(buf) + "\\Documents\\" + filename);

        if (!std::filesystem::exists(savefilePath))
        {
            std::fstream f(savefilePath, std::ios::out);
            if (f.bad())
            {
                printf("bad save path access\n");
                return;
            }

            printf("created save file at {%s}\n", savefilePath.string().c_str());
        }

        initialized = true;
        
    }

    std::vector<WeakReplay>& PeekSavedRecordings()
    {
        if (!initialized) return peek;
        if (!dirtyPeek)
            return peek;

        std::vector<WeakReplay> replaysInSaveFile;

        std::fstream f(savefilePath, std::ios::binary | std::ios::in);
        if (f.bad())
        {
            printf("error opening save file for peek read\n");
            return peek;
        }
        
        while (true)
        {
            WeakReplay curr;
            
            f.read((char*)(&curr), sizeof(curr));

            if (f.gcount() != sizeof(WeakReplay)) break;
            f.seekg(curr.cmdCount, std::ios_base::cur);

            replaysInSaveFile.push_back(curr);
        }
        
        peek = replaysInSaveFile;

        dirtyPeek = false;
        return peek;
    }

    SaveError SaveRecording(recorder::Recording recording)
    {
        if (!initialized) return SaveError::NotInitialized;

        if (dirtyPeek)
            PeekSavedRecordings();

        size_t cmdCount = recording.cmds.size();

        for (auto weak : peek)
        {
            if (weak.inUse)
                continue;
            if (cmdCount > weak.cmdCount)
                continue;

            weak.inUse = true;
            weak.cmdCount = cmdCount;
            if (recording.name.size() > 32)
            {
                memcpy(weak.name, recording.name.c_str(), 31);
                weak.name[31] = '\0';
            }
            else
            {
                memcpy(weak.name, recording.name.c_str(), recording.name.size());
            }

            std::fstream f(savefilePath, std::ios_base::binary | std::ios_base::out | std::ios_base::in);
            f.seekp(weak.offset);
            f.write((char*)&weak, sizeof(weak));

            f.seekp(sizeof(weak), std::ios_base::cur);

            for (const auto& cmd : recording.cmds)
            {
                f.write((char*)&cmd, sizeof(cmd));
                f.seekp(sizeof(cmd), std::ios_base::cur);
            }

            dirtyPeek = true;
            return SaveError::None;
        }

        std::fstream f(savefilePath, std::ios_base::app);
        std::cout << "g " << f.tellg() << "\n";
        std::cout << "p " << f.tellp() << "\n";

        dirtyPeek = true;
        return SaveError::None;
    }

    SaveError DeleteRecording(std::string name)
    {
        if (!initialized) return SaveError::NotInitialized;


        dirtyPeek = true;
        return SaveError::None;
    }
}
