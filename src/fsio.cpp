#include "fsio.h"
#include <filesystem>
#include <iostream>
#include <fstream>
#include <exception>
#include "debug.h"

namespace fsio // filesystem I/O
{
    bool initialized = false;
    bool dirtyPeek = true;
    std::filesystem::path savefilePath;
    std::vector<DiskReplay> peek;

    constexpr const char* filename = "cod4recorder.recs";

    void Init()
    {
        char buf[64u]; size_t used;
        getenv_s<sizeof(buf)>(&used, buf, "USERPROFILE"); buf[64u - 1u] = '\0';

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

        std::fstream f(savefilePath, std::ios::binary | std::ios::in);
        if (f.bad())
        {
            printf("error opening save file for peek read\n");
            return peek;
        }
        
        size_t idx = 0;
        while (true)
        {
            DiskReplay curr(0);
            
            f.read((char*)(&curr), sizeof(DiskReplay));

            if (f.gcount() != sizeof(DiskReplay))
                break;
            f.seekg(curr.fragmentCmdCap * sizeof(recorder::Smallcmd), std::ios_base::cur);

            if (idx < peek.size())
                peek.at(idx) = curr;
            else
                peek.push_back(curr);

            idx++;
        }
        if (idx < peek.size())
            peek.resize(idx);

        dirtyPeek = false;
        return peek;
    }

    SaveError SaveRecordingToDisk(std::shared_ptr<recorder::Recording> recording)
    {
        if (!initialized) LOG_RET(SaveError::NotInitialized);
        if (recording->onDisk) LOG_RET(SaveError::RecordingAlreadyOnDisk);

        if (dirtyPeek)
            PeekSavedRecordings();

        std::fstream f(savefilePath, std::ios::binary | std::ios::in | std::ios::out);
        if (f.bad()) LOG_RET(SaveError::BadFile);


        DiskReplay newDisk(recording);
        for (auto& disk : peek)
        {
            if (disk.inUse)
                continue;
            if (newDisk.cmdCount > disk.fragmentCmdCap)
                continue;

            newDisk.fragmentCmdCap = disk.fragmentCmdCap;
            newDisk.inUse = true;

            disk = newDisk;

            f.seekp(disk.offset, std::ios::beg);
            f.write((char*)&disk, sizeof(DiskReplay));
            f.write((char*)recording->cmds.data(), disk.cmdCount * sizeof(recorder::Smallcmd));

            recording->onDisk = true;
            recording->onDiskOffset = disk.offset;

            //dirtyPeek = true;
            LOG_RET(SaveError::None);
        }

        f.seekp(0, std::ios::end);

        newDisk.inUse = true;
        newDisk.offset = f.tellp();

        printf("saving recording @ %i\n", newDisk.offset);

        f.write((char*)&newDisk, sizeof(DiskReplay));
        f.write((char*)recording->cmds.data(), newDisk.cmdCount * sizeof(recorder::Smallcmd));

        recording->onDisk = true;
        recording->onDiskOffset = newDisk.offset;

        peek.push_back(newDisk);
        //dirtyPeek = true;
        LOG_RET(SaveError::None);
    }

    SaveError DeleteRecordingOnDisk(unsigned long long uuid)
    {
        if (!initialized) LOG_RET(SaveError::NotInitialized);
        if (dirtyPeek) PeekSavedRecordings();

        std::fstream f(savefilePath, std::ios::binary | std::ios::in | std::ios::out);
        if (f.bad()) LOG_RET(SaveError::BadFile);

        bool foundOnDisk = false;
        for (auto& disk : peek)
        {
            if (disk.uuid != uuid)
                continue;
            if (!disk.inUse)
                continue;

            foundOnDisk = true;

            disk.inUse = false;

            f.seekp(disk.offset, std::ios::beg);
            f.write((char*)&disk, sizeof(disk));

            recorder::RecordingWasRemovedFromDisk(uuid);
            break;
        }
        if (!foundOnDisk) LOG_RET(SaveError::RecordingNotOnDisk);

        dirtyPeek = true;
        LOG_RET(SaveError::None);
    }

    SaveError LoadRecordingFromDisk(unsigned long long uuid)
    {
        if (!initialized) LOG_RET(SaveError::NotInitialized);
        if (dirtyPeek) PeekSavedRecordings();

        if (recorder::IsRecordingInMemory(uuid)) LOG_RET(SaveError::RecordingAlreadyInMemory);

        std::fstream f(savefilePath, std::ios::binary | std::ios::in);
        if (f.bad()) LOG_RET(SaveError::BadFile);

        bool foundOnDisk = false;
        for (auto& disk : peek)
        {
            if (disk.uuid != uuid)
                continue;
            if (!disk.inUse)
                continue;

            foundOnDisk = true;

            auto rec = std::make_shared<recorder::Recording>(disk.name, disk.uuid);
            rec->startPos = disk.startPos;
            rec->startRot = disk.startRot;

            rec->onDiskOffset = disk.offset;
            rec->onDisk = true;

            rec->cmds.resize(disk.cmdCount);
            f.seekg(disk.offset + sizeof(DiskReplay), std::ios::beg);
            f.read((char*)rec->cmds.data(), disk.cmdCount * sizeof(recorder::Smallcmd));

            recorder::recordings.push_back(rec);
            break;
        }
        if (!foundOnDisk) LOG_RET(SaveError::RecordingNotOnDisk);

        LOG_RET(SaveError::None);
    }

    constexpr const char* errorMessages[]
    {
        "None",
        "NotInitialized",
        "RecordingDoesntExist",
        "IndexOutOfRange",
        "RecordingAlreadyOnDisk",
        "RecordingNotOnDisk",
        "BadFile",
        "RecordingAlreadyInMemory",
    };
    const char* GetErrorMessage(SaveError error)
    {
        switch (error)
        {
            case None:
                return errorMessages[0];
            case NotInitialized:
                return errorMessages[1];
            case RecordingDoesntExist:
                return errorMessages[2];
            case IndexOutOfRange:
                return errorMessages[3];
            case RecordingAlreadyOnDisk:
                return errorMessages[4];
            case RecordingNotOnDisk:
                return errorMessages[5];
            case BadFile:
                return errorMessages[6];
        }
        return errorMessages[0];
    }


    void RecordingWasRemovedFromMemory(unsigned long long uuid)
    {
        if (dirtyPeek) PeekSavedRecordings();
        for (auto& disk : peek)
        {
            if (disk.uuid != uuid)
                continue;

            //disk.inMem = false;
            //recording->onDiskOffset = 0;
            break;
        }
    }
}
