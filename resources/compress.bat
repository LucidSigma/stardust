if exist "./assets.pak" del "./assets.pak"
if exist "./locales.pak" del "./locales.pak"

tar -cf "assets.pak" --format=zip assets\*
tar -cf "locales.pak" --format=zip locales\*

pause