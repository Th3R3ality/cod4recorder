#include "savefile.h"
#include <filesystem>
#include <iostream>
#include <fstream>
#include <exception>
#include "debug.h"

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
            
            f.read((char*)(&curr), sizeof(curr));

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
        //if (recordingIndex < 0 || recordingIndex >= recorder::_recordings.size()) LOG_RET(SaveError::IndexOutOfRange);

        if (dirtyPeek)
            PeekSavedRecordings();

        //recorder::Recording& recording = recorder::_recordings.at(recordingIndex);
        if (recording->onDisk) LOG_RET(SaveError::RecordingAlreadyOnDisk);

        std::fstream f(savefilePath, std::ios::binary | std::ios::in | std::ios::out);
        if (f.bad()) LOG_RET(SaveError::BadFile);

        size_t cmdCount = recording->cmds.size();
        for (auto& disk : peek)
        {
            if (disk.inUse)
                continue;
            if (cmdCount > disk.fragmentCmdCap)
                continue;

            disk.inUse = true;
            disk.cmdCount = cmdCount;
            disk.startPos = recording->startPos;
            disk.startRot = recording->startRot;
            strncpy_s<sizeof(DiskReplay::name)>(disk.name, recording->name, sizeof(DiskReplay::name) - 1);
            disk.name[sizeof(DiskReplay::name)-1] = '\0';
            disk.uuid = recording->uuid;

            f.seekp(disk.offset, std::ios::beg);
            f.write((char*)&disk, sizeof(disk));
            f.write((char*)recording->cmds.data(), cmdCount * sizeof(recorder::Smallcmd));

            recording->onDisk = true;
            recording->onDiskOffset = disk.offset;

            //dirtyPeek = true;
            LOG_RET(SaveError::None);
        }

        DiskReplay disk;
        strncpy_s<sizeof(DiskReplay::name)>(disk.name, recording->name, sizeof(DiskReplay::name) - 1);
        disk.name[sizeof(DiskReplay::name) - 1] = '\0';
        disk.uuid = recording->uuid;
        disk.inUse = true;
        disk.cmdCount = recording->cmds.size();
        disk.startPos = recording->startPos;
        disk.startRot = recording->startRot;
        disk.fragmentCmdCap = cmdCount;

        f.seekp(0, std::ios::end);
        disk.offset = f.tellp();
        printf("saving recording @ %i\n", disk.offset);
        f.write((char*)&disk, sizeof(disk));
        f.write((char*)recording->cmds.data(), disk.cmdCount * sizeof(recorder::Smallcmd));

        recording->onDisk = true;
        recording->onDiskOffset = disk.offset;

        peek.push_back(disk);
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
            rec->onDisk = true;
            rec->onDiskOffset = disk.offset;
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
}
