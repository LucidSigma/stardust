#include "stardust/preferences/UserPrefs.h"

#include <utility>

#include "stardust/filesystem/Filesystem.h"

namespace stardust
{
    [[nodiscard]] auto UserPrefs::Load(const StringView filepath) -> Status
    {
        m_filepath = filepath;

        auto fileReadResult = filesystem::ReadTOML(m_filepath);

        if (fileReadResult.is_err())
        {
            return Status::Fail;
        }

        m_preferenceTable = std::move(fileReadResult).unwrap();

        return Status::Success;
    }

    auto UserPrefs::Save() -> Status
    {
        return filesystem::WriteTOMLToFile(m_filepath, m_preferenceTable);
    }

    [[nodiscard]] auto UserPrefs::HasCategory(const StringView category) const -> bool
    {
        return m_preferenceTable.contains(category);
    }
    
    [[nodiscard]] auto UserPrefs::HasKey(const StringView category, const StringView key) const -> bool
    {
        if (!HasCategory(category))
        {
            return false;
        }

        return m_preferenceTable[category].as_table()->contains(key);
    }

    auto UserPrefs::DeleteKey(const StringView category, const StringView key) -> void
    {
        if (!HasKey(category, key))
        {
            return;
        }

        m_preferenceTable[category].as_table()->erase(key);
    }
}
