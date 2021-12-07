include(ECMInstallIcons)

if (UNIX AND NOT APPLE)
    ecm_install_icons(ICONS
            16-apps-${APP_NAME}.png
            32-apps-${APP_NAME}.png
            48-apps-${APP_NAME}.png
        DESTINATION share/icons
    )
endif()
