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
    std::vector<DiskReplay> peek;

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

        PeekSavedRecordings();
        initialized = true;
    }

    std::vector<DiskReplay>& PeekSavedRecordings()
    {
        if (!initialized) return peek;
        if (!dirtyPeek) return peek;

        std::vector<DiskReplay> replaysInSaveFile;

        std::fstream f(savefilePath, std::ios::binary | std::ios::in);
        if (f.bad())
        {
            printf("error opening save file for peek read\n");
            return peek;
        }
        
        while (true)
        {
            DiskReplay curr(0);
            
            f.read((char*)(&curr), sizeof(curr));

            if (f.gcount() != sizeof(DiskReplay))
                break;
            f.seekg(curr.cmdCount, std::ios_base::cur);

            replaysInSaveFile.push_back(curr);
        }
        
        peek = replaysInSaveFile;

        dirtyPeek = false;
        return peek;
    }

    SaveError SaveRecordingToDisk(int recordingIndex)
    {
        if (!initialized) return SaveError::NotInitialized;
        if (recordingIndex < 0 || recordingIndex >= recorder::recordings.size()) return SaveError::IndexOutOfRange;

        if (dirtyPeek)
            PeekSavedRecordings();

        recorder::Recording& recording = recorder::recordings.at(recordingIndex);
        if (recording.onDisk) return SaveError::RecordingAlreadyOnDisk;

        size_t cmdCount = recording.cmds.size();
        for (auto& disk : peek)
        {
            if (disk.inUse)
                continue;
            if (cmdCount > disk.cmdCount)
                continue;

            disk.inUse = true;
            disk.cmdCount = cmdCount;
            strncpy_s<sizeof(DiskReplay::name)>(disk.name, recording.name.c_str(), sizeof(DiskReplay::name) - 1);
            disk.name[sizeof(DiskReplay::name)-1] = '\0';
            disk.uuid = recording.uuid;

            std::fstream f(savefilePath, std::ios_base::binary | std::ios_base::out | std::ios_base::in);
            f.seekp(disk.offset);
            f.write((char*)&disk, sizeof(disk));

            f.seekp(sizeof(disk), std::ios_base::cur);
            f.write((char*)recording.cmds.data(), cmdCount * sizeof(recorder::Smallcmd));

            recording.onDisk = true;
            //dirtyPeek = true;
            return SaveError::None;
        }


        DiskReplay disk;
        strncpy_s<sizeof(DiskReplay::name)>(disk.name, recording.name.c_str(), sizeof(DiskReplay::name) - 1);
        disk.name[sizeof(DiskReplay::name) - 1] = '\0';
        disk.uuid = recording.uuid;
        disk.inUse = true;
        disk.cmdCount = recording.cmds.size();

        std::fstream f(savefilePath, std::ios_base::app);
        disk.offset = f.tellp();
        f.write((char*)&disk, sizeof(disk));
        f.write((char*)recording.cmds.data(), disk.cmdCount * sizeof(recorder::Smallcmd));

        recording.onDisk = true;
        peek.push_back(disk);
        //dirtyPeek = true;
        return SaveError::None;
    }

    SaveError DeleteRecordingOnDisk(unsigned long long uuid)
    {
        if (!initialized) return SaveError::NotInitialized;
        if (dirtyPeek) PeekSavedRecordings();

        bool foundOnDisk = false;
        for (auto& disk : peek)
        {
            if (disk.uuid == uuid)
            {
                foundOnDisk = true;
                disk.inUse = false;

                std::fstream f(savefilePath);
                f.seekp(disk.offset, std::ios::cur);
                f.write((char*)&disk, sizeof(disk));
                break;
            }
        }
        if (!foundOnDisk) return SaveError::RecordingNotOnDisk;

        dirtyPeek = true;
        return SaveError::None;
    }
    SaveError LoadRecordingFromDisk(unsigned long long uuid)
    {
        if (!initialized) return SaveError::NotInitialized;

        return SaveError::None;
    }
}
