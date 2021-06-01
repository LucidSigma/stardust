#pragma once
#ifndef STARDUST_APP_CONFIG_H
#define STARDUST_APP_CONFIG_H

#include "stardust/data/Containers.h"
#include "stardust/data/Types.h"

namespace stardust
{
    struct AppConfig
    {
        String applicationName;
        String organisationName;

        bool allowResizableWindow = false;
        String windowTitle;

        String assetsArchiveRelativeFilepath;
        String localesArchiveRelativeFilepath;
        String logFileRelativeFilepath;

        String defaultConfigFilepath;
        String gameControllerDatabaseFilepath;
        Optional<String> windowIconFilepath = NullOpt;

        f64 fixedTimestep = 0.0;
        u32 positionIterations = 0u;
        u32 velocityIterations = 0u;

        const char* argv0 = nullptr;
    };
}

#endif