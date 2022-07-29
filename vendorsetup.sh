#!/usr/bin/env bash
#
#	This file is part of the OrangeFox Recovery Project
# 	Copyright (C) 2020-2021 The OrangeFox Recovery Project
#
#	OrangeFox is free software: you can redistribute it and/or modify
#	it under the terms of the GNU General Public License as published by
#	the Free Software Foundation, either version 3 of the License, or
#	any later version.
#
#	OrangeFox is distributed in the hope that it will be useful,
#	but WITHOUT ANY WARRANTY; without even the implied warranty of
#	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#	GNU General Public License for more details.
#
# 	This software is released under GPL version 3 or any later version.
#	See <http://www.gnu.org/licenses/>.
#
# 	Please maintain this if you use this script or any part of it
#

FDEVICE="rosemary"
#set -o xtrace

fox_get_target_device() {
	local chkdev=$(echo "$BASH_SOURCE" | grep -w $FDEVICE)
	if [ -n "$chkdev" ]; then
		FOX_BUILD_DEVICE="$FDEVICE"
	else
		chkdev=$(set | grep BASH_ARGV | grep -w $FDEVICE)
		[ -n "$chkdev" ] && FOX_BUILD_DEVICE="$FDEVICE"
	fi
}

if [ -z "$1" ] && [ -z "$FOX_BUILD_DEVICE" ]; then
	fox_get_target_device
fi

# Dirty Fix: Only declare orangefox vars when needed
if [ -f "$(gettop)/bootable/recovery/orangefox.cpp" ]; then
	echo -e "\x1b[96m[INFO]: Setting up OrangeFox build vars for rosemary...\x1b[m"
	if [ "$1" = "$FDEVICE" ] || [  "$FOX_BUILD_DEVICE" = "$FDEVICE" ]; then
		# Version / Maintainer infos
		export OF_MAINTAINER="Woomymy"
		export FOX_VERSION=R11.1_1
		export FOX_BUILD_TYPE="Beta"

		# Device info
		export OF_AB_DEVICE=1
		export OF_VIRTUAL_AB_DEVICE=1
		export TARGET_DEVICE_ALT="secret, maltose"
		
		# OTA / DM-Verity / Encryption
		export OF_DISABLE_MIUI_OTA_BY_DEFAULT=1
		export OF_FIX_OTA_UPDATE_MANUAL_FLASH_ERROR=1
		
		export OF_DONT_PATCH_ON_FRESH_INSTALLATION=1
		export OF_DONT_PATCH_ENCRYPTED_DEVICE=1
		export OF_KEEP_DM_VERITY_FORCED_ENCRYPTION=1
		export OF_SKIP_FBE_DECRYPTION_SDKVERSION=31 # Don't try to decrypt A12
		export OF_SKIP_DECRYPTED_ADOPTED_STORAGE=1

		# Display / Leds
		export OF_SCREEN_H="2400"
		export OF_STATUS_H="100"
		export OF_STATUS_INDENT_LEFT=48
		export OF_STATUS_INDENT_RIGHT=48
		export OF_HIDE_NOTCH=1
		export OF_CLOCK_POS=1 # left and right clock positions available
		export OF_USE_GREEN_LED=0
		export OF_FL_PATH1="/tmp/flashlight" # See /init.recovery.mt6785.rc for more information

		# Other OrangeFox configs
		export OF_ENABLE_LPTOOLS=1
		export OF_ALLOW_DISABLE_NAVBAR=0
        export OF_QUICK_BACKUP_LIST="/boot;/data;"
		export FOX_BUGGED_AOSP_ARB_WORKAROUND="1546300800" # Tue Jan 1 2019 00:00:00 GMT
		export FOX_DELETE_AROMAFM=1
		export FOX_USE_SPECIFIC_MAGISK_ZIP="$(gettop)/device/redmi/rosemary/Magisk/Magisk.zip"

        export BUNDLED_MAGISK_VER="25.1"
        export BUNDLED_MAGISK_SUM="8c50f8b8f854c1279e089c2d7d75b9457159a0f3b3f471415c38587d62ddbaee" # Sha256 sum of the prebuilt magisk

            if [ -f "${FOX_USE_SPECIFIC_MAGISK_ZIP}" -a "$(sha256sum "${FOX_USE_SPECIFIC_MAGISK_ZIP}" 2>/dev/null | awk '{print $1}')" != "${BUNDLED_MAGISK_SUM}" ]
            then
                echo -e "\e[96m[INFO]: Removing invalid magisk zip\e[m"
                rm -v "${FOX_USE_SPECIFIC_MAGISK_ZIP}"
            fi

        if [[ ! -f "${FOX_USE_SPECIFIC_MAGISK_ZIP}" ]]
        then
            # Download prebuilt magisk for OrangeFox builds
            echo -e "\e[96m[INFO]: Downloading Magisk v${BUNDLED_MAGISK_VER}\e[m"
            
            if [[ "$(command -v "curl")" ]]
            then
                if [[ ! -d "$(dirname "${FOX_USE_SPECIFIC_MAGISK_ZIP}")" ]]
                then
                    mkdir -p "$(dirname "${FOX_USE_SPECIFIC_MAGISK_ZIP}")"
                fi

                # Download magisk and verify it
                curl -L --progress-bar "https://github.com/topjohnwu/Magisk/releases/download/v${BUNDLED_MAGISK_VER}/Magisk-v${BUNDLED_MAGISK_VER}.apk" -o "${FOX_USE_SPECIFIC_MAGISK_ZIP}"
                DOWNLOADED_SUM="$(sha256sum "${FOX_USE_SPECIFIC_MAGISK_ZIP}" | awk '{print $1}')"
                
                if [[ "${DOWNLOADED_SUM}" != "${BUNDLED_MAGISK_SUM}" ]]
                then
                    echo -e "\e[91m[ERROR]: Donwloaded Magisk ZIP seems *corrupted*, removing it to protect user's safety\e[m"
                    rm "${FOX_USE_SPECIFIC_MAGISK_ZIP}"
                    unset "FOX_USE_SPECIFIC_MAGISK_ZIP"
                else
                    echo -e "\e[96m[INFO]: Downloaded Magisk v${BUNDLED_MAGISK_VER}\e[m"
                fi
            else
                # Curl is supposed to be installed according to "Establishing a build environnement" section in AOSP docs
                # If it isn't, warn the builder about it and fallback to default Magisk ZIP
                echo -e "\e[91m[ERROR]: Curl not found!\e[m"
                unset "FOX_USE_SPECIFIC_MAGISK_ZIP"
            fi
        fi
    fi
fi
