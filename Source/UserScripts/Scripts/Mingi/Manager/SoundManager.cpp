#include "Export.h"
#include "ScriptBehaviour.h"
#include "SoundManager.h"

#include "StringHelper.h"

#include <algorithm>
#include <array>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <limits>
#include <stdexcept>

namespace fs = std::filesystem;

namespace
{
    std::string ToLowerCopy(std::string s)
    {
        std::transform(s.begin(), s.end(), s.begin(),
            [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
        return s;
    }
}

MMMEngine::ObjPtr<MMMEngine::SoundManager> MMMEngine::SoundManager::Instance = nullptr;

std::string MMMEngine::SoundManager::Trim(const std::string& s)
{
    const size_t begin = s.find_first_not_of(" \t\r\n");
    if (begin == std::string::npos)
        return {};

    const size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(begin, end - begin + 1);
}

bool MMMEngine::SoundManager::TryParseCsvRow(const std::string& line, std::array<std::string, 4>& outRow)
{
    std::string work = Trim(line);
    if (work.size() >= 2 && work.front() == '"' && work.back() == '"')
        work = Trim(work.substr(1, work.size() - 2));

    const size_t commaCount = static_cast<size_t>(std::count(work.begin(), work.end(), ','));
    const size_t tabCount = static_cast<size_t>(std::count(work.begin(), work.end(), '\t'));
    char delim = (tabCount > commaCount) ? '\t' : ',';

    auto sanitize = [](std::string cell)
    {
        cell = Trim(cell);
        if (cell.size() >= 2 && cell.front() == '"' && cell.back() == '"')
        {
            cell = cell.substr(1, cell.size() - 2);
        }
        else
        {
            if (!cell.empty() && cell.front() == '"')
                cell.erase(cell.begin());
            if (!cell.empty() && cell.back() == '"')
                cell.pop_back();
        }
        return Trim(cell);
    };

    size_t start = 0;
    for (size_t i = 0; i < 3; ++i)
    {
        const size_t sep = work.find(delim, start);
        if (sep == std::string::npos)
            return false;

        outRow[i] = sanitize(work.substr(start, sep - start));
        start = sep + 1;
    }

    outRow[3] = sanitize(work.substr(start));
    if (outRow[3].find(delim) != std::string::npos)
        return false;

    return true;
}

void MMMEngine::SoundManager::Awake()
{
    if (!Instance.IsValid())
    {
        Instance = SelfPtr(this);

        EnsureBGMSource();
        EnsureSFXPool();
        LoadAudioTable();

        DontDestroyOnLoad(SelfPtr(this));
    }
    else
    {
        GetGameObject()->Destroy(GetGameObject());
    }
}

void MMMEngine::SoundManager::OnDestroy()
{
    if (Instance == SelfPtr(this))
        Instance = nullptr;
}

void MMMEngine::SoundManager::Update()
{
    for (auto& slot : m_sfxSlots)
    {
        if (!slot.source.IsValid())
            continue;

        if (!slot.source->IsPlaying())
        {
            slot.trackCaller = false;
            slot.is3D = false;
            slot.followTarget = nullptr;
            continue;
        }

        if (!slot.is3D || !slot.trackCaller)
            continue;

        if (!slot.followTarget.IsValid() || 
            (slot.followTarget->GetGameObject().IsValid() && !slot.followTarget->GetGameObject()->IsActiveInHierarchy()) || 
            slot.followTarget->IsDestroyed())
        {
            slot.trackCaller = false;
            slot.followTarget = nullptr;
            continue;
        }

        if (!slot.emitter.IsValid() || slot.emitter->IsDestroyed())
        {
            slot.trackCaller = false;
            slot.followTarget = nullptr;
            continue;
        }

        auto emitterTransform = slot.emitter->GetTransform();
        if (emitterTransform.IsValid())
            emitterTransform->SetWorldPosition(slot.followTarget->GetWorldPosition());
    }
}

void MMMEngine::SoundManager::LoadAudioTable()
{
    m_bgmTable.clear();
    m_sfxTable.clear();

    fs::path root = ResourceManager::Get().GetCurrentRootPath();
    fs::path csvPath = root / Utility::StringHelper::StringToWString(kAudioCsvPath);

    std::ifstream csv(csvPath);
    if (!csv.is_open())
    {
        std::cout << "[SoundManager] Failed to open csv: " << csvPath.string() << std::endl;
        return;
    }

    std::string line;
    size_t lineNumber = 0;
    while (std::getline(csv, line))
    {
        ++lineNumber;

        std::string trimmedLine = Trim(line);
        // Strip UTF-8 BOM from first data/header line if present.
        if (lineNumber == 1 && trimmedLine.size() >= 3 &&
            static_cast<unsigned char>(trimmedLine[0]) == 0xEF &&
            static_cast<unsigned char>(trimmedLine[1]) == 0xBB &&
            static_cast<unsigned char>(trimmedLine[2]) == 0xBF)
        {
            trimmedLine.erase(0, 3);
            trimmedLine = Trim(trimmedLine);
        }

        if (trimmedLine.size() >= 2 && trimmedLine.front() == '"' && trimmedLine.back() == '"')
            trimmedLine = Trim(trimmedLine.substr(1, trimmedLine.size() - 2));

        if (trimmedLine.empty() || trimmedLine[0] == '#')
            continue;

        std::array<std::string, 4> row{};
        if (!TryParseCsvRow(trimmedLine, row))
        {
            std::cout << "[SoundManager] Invalid csv row(" << lineNumber << "): " << trimmedLine << std::endl;
            continue;
        }

        if (row[0].empty() && row[1].empty() && row[2].empty() && row[3].empty())
            continue;

        std::string type = ToLowerCopy(row[0]);
        const std::string& key = row[1];
        const std::string& path = row[2];

        if (type.empty() || type[0] == '#')
            continue;

        if (type == "type" && ToLowerCopy(key) == "key")
            continue;

        if (key.empty() || path.empty())
        {
            std::cout << "[SoundManager] Empty key/path at row " << lineNumber << std::endl;
            continue;
        }

        float volume = 1.0f;
        try
        {
            size_t parsed = 0;
            volume = std::stof(row[3], &parsed);
            if (parsed == 0)
                throw std::invalid_argument("no numeric volume");

            const std::string trailing = Trim(row[3].substr(parsed));
            if (!trailing.empty())
                throw std::invalid_argument("trailing volume token");
        }
        catch (...)
        {
            std::cout << "[SoundManager] Invalid volume at row " << lineNumber << std::endl;
            continue;
        }

        volume = std::clamp(volume, 0.0f, 1.0f);

        auto clip = ResourceManager::Get().Load<AudioClip>(Utility::StringHelper::StringToWString(path));
        if (!clip)
        {
            std::cout << "[SoundManager] Failed to load clip at row " << lineNumber << ": " << path << std::endl;
            continue;
        }

        AudioEntry entry{};
        entry.clip = clip;
        entry.baseVolume = volume;

        if (type == "bgm")
        {
            m_bgmTable[key] = entry;
        }
        else if (type == "sfx")
        {
            m_sfxTable[key] = entry;
        }
        else
        {
            std::cout << "[SoundManager] Invalid type at row " << lineNumber << ": " << row[0] << std::endl;
            continue;
        }
    }

    //std::cout << "[SoundManager] Loaded BGM: " << m_bgmTable.size()
    //          << ", SFX: " << m_sfxTable.size() << std::endl;
}

void MMMEngine::SoundManager::EnsureBGMSource()
{
    if (!m_bgmSource.IsValid())
        m_bgmSource = GetComponent<AudioSource>();

    if (!m_bgmSource.IsValid())
        m_bgmSource = GetGameObject()->AddComponent<AudioSource>();

    if (!m_bgmSource.IsValid())
    {
        std::cout << "[SoundManager] Failed to create BGM AudioSource." << std::endl;
        return;
    }

    m_bgmSource->SetSpatialize(false);
    m_bgmSource->SetLoop(true);
    m_bgmSource->SetPlayOnAwake(false);
}

void MMMEngine::SoundManager::EnsureSFXPool()
{
    auto managerTransform = GetTransform();

    for (int i = 0; i < kSfxPoolSize; ++i)
    {
        auto& slot = m_sfxSlots[static_cast<size_t>(i)];
        if (slot.source.IsValid() && slot.emitter.IsValid())
            continue;

        slot.trackCaller = false;
        slot.is3D = false;
        slot.followTarget = nullptr;
        slot.currentKey.clear();
        slot.playSerial = 0;

        const std::string emitterName = "SoundManager_SFXEmitter_" + std::to_string(i);
        slot.emitter = Object::NewObject<GameObject>(emitterName);
        if (!slot.emitter.IsValid())
        {
            std::cout << "[SoundManager] Failed to create emitter gameobject." << std::endl;
            continue;
        }

        auto emitterTransform = slot.emitter->GetTransform();
        if (managerTransform.IsValid() && emitterTransform.IsValid())
        {
            emitterTransform->SetParent(managerTransform, false);
            emitterTransform->SetWorldPosition(managerTransform->GetWorldPosition());
        }

        slot.source = slot.emitter->AddComponent<AudioSource>();
        if (!slot.source.IsValid())
        {
            std::cout << "[SoundManager] Failed to create emitter audiosource." << std::endl;
            continue;
        }

        slot.source->SetPlayOnAwake(false);
        slot.source->SetLoop(false);
        slot.source->SetSpatialize(true);
        slot.source->SetVolume(1.0f);
    }
}

int MMMEngine::SoundManager::AcquireSFXSlot()
{
    int oldestIndex = 0;
    uint64_t oldestSerial = std::numeric_limits<uint64_t>::max();

    for (int i = 0; i < kSfxPoolSize; ++i)
    {
        auto& slot = m_sfxSlots[static_cast<size_t>(i)];

        if (!slot.source.IsValid())
            return i;

        if (!slot.source->IsPlaying())
            return i;

        if (slot.playSerial < oldestSerial)
        {
            oldestSerial = slot.playSerial;
            oldestIndex = i;
        }
    }

    return oldestIndex;
}

MMMEngine::ObjPtr<MMMEngine::Transform> MMMEngine::SoundManager::ResolveCallerTransform(const ObjPtr<Component>& caller) const
{
    if (!caller.IsValid() || caller->IsDestroyed())
        return nullptr;

    auto owner = caller->GetGameObject();
    if (!owner.IsValid() || owner->IsDestroyed())
        return nullptr;

    return owner->GetTransform();
}

void MMMEngine::SoundManager::PlaySFXInternal(
    const std::string& key,
    const ObjPtr<Component>& caller,
    bool spatialize,
    bool trackCaller,
    float volumeScale,
    bool loop)
{
    auto it = m_sfxTable.find(key);
    if (it == m_sfxTable.end())
    {
        std::cout << "[SoundManager] SFX key not found: " << key << std::endl;
        return;
    }

    const AudioEntry& entry = it->second;
    if (!entry.clip)
    {
        std::cout << "[SoundManager] SFX clip is null: " << key << std::endl;
        return;
    }

    EnsureSFXPool();

    const int slotIndex = AcquireSFXSlot();
    if (slotIndex < 0 || slotIndex >= kSfxPoolSize)
        return;

    auto& slot = m_sfxSlots[static_cast<size_t>(slotIndex)];
    if (!slot.source.IsValid())
        return;

    if (slot.source->IsPlaying())
        slot.source->Stop();

    slot.source->SetLoop(loop);
    slot.source->SetSpatialize(spatialize);
    slot.source->SetClip(entry.clip);

    const float finalVolume = std::max(0.0f, entry.baseVolume * volumeScale);
    slot.source->SetVolume(finalVolume);

    auto callerTransform = ResolveCallerTransform(caller);

    if (slot.emitter.IsValid())
    {
        auto emitterTransform = slot.emitter->GetTransform();
        if (emitterTransform.IsValid())
        {
            if (callerTransform.IsValid())
                emitterTransform->SetWorldPosition(callerTransform->GetWorldPosition());
            else if (GetTransform().IsValid())
                emitterTransform->SetWorldPosition(GetTransform()->GetWorldPosition());
        }
    }

    slot.is3D = spatialize;
    slot.trackCaller = spatialize && trackCaller && callerTransform.IsValid();
    slot.followTarget = slot.trackCaller ? callerTransform : ObjPtr<Transform>();
    slot.currentKey = key;
    slot.playSerial = ++m_playSerialCounter;

    slot.source->Play();
}

void MMMEngine::SoundManager::PlayBGM(const std::string& key, float volumeScale, bool loop)
{
    auto it = m_bgmTable.find(key);
    if (it == m_bgmTable.end())
    {
        std::cout << "[SoundManager] BGM key not found: " << key << std::endl;
        return;
    }

    EnsureBGMSource();
    if (!m_bgmSource.IsValid())
        return;

    if (m_bgmSource->IsPlaying())
        m_bgmSource->Stop();

    const AudioEntry& entry = it->second;
    if (!entry.clip)
        return;

    m_bgmSource->SetSpatialize(false);
    m_bgmSource->SetLoop(loop);
    m_bgmSource->SetClip(entry.clip);

    const float finalVolume = std::max(0.0f, entry.baseVolume * volumeScale);
    m_bgmSource->SetVolume(finalVolume);

    m_bgmSource->Play();
}

void MMMEngine::SoundManager::StopBGM()
{
    if (m_bgmSource.IsValid())
        m_bgmSource->Stop();
}

void MMMEngine::SoundManager::PlaySFX2D(const std::string& key, const ObjPtr<Component>& caller, float volumeScale)
{
    PlaySFXInternal(key, caller, false, false, volumeScale, false);
}

void MMMEngine::SoundManager::PlaySFX3D(const std::string& key, const ObjPtr<Component>& caller, bool trackCaller, float volumeScale)
{
    PlaySFXInternal(key, caller, true, trackCaller, volumeScale, false);
}

void MMMEngine::SoundManager::StopSFX(const std::string& key)
{
    for (auto& slot : m_sfxSlots)
    {
        if (!slot.source.IsValid())
            continue;

        if (!slot.source->IsPlaying())
            continue;

        if (slot.currentKey != key)
            continue;

        slot.source->Stop();
        slot.trackCaller = false;
        slot.is3D = false;
        slot.followTarget = nullptr;
        slot.currentKey.clear();
    }
}

void MMMEngine::SoundManager::StopAllSFX()
{
    for (auto& slot : m_sfxSlots)
    {
        if (slot.source.IsValid())
            slot.source->Stop();

        slot.trackCaller = false;
        slot.is3D = false;
        slot.followTarget = nullptr;
        slot.currentKey.clear();
    }
}





