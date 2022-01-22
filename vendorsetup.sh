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

if [ -z "$1" -a -z "$FOX_BUILD_DEVICE" ]; then
	fox_get_target_device
fi

# Dirty Fix: Only declare orangefox vars when needed
if [ -f "$(gettop)/bootable/recovery/orangefox.cpp" ]; then
	echo -e "\x1b[96mSetting up OrangeFox build vars...\x1b[m"
	if [ "$1" = "$FDEVICE" -o "$FOX_BUILD_DEVICE" = "$FDEVICE" ]; then
		export OF_FLASHLIGHT_ENABLE=1
        export OF_FL_PATH1="/tmp/flashlight"
		export TARGET_DEVICE_ALT="maltose, secret"
		export OF_USE_GREEN_LED=0
		export OF_HIDE_NOTCH=1
		export OF_MAINTAINER="Woomymy"
		export OF_USE_MAGISKBOOT=1
		export OF_USE_MAGISKBOOT_FOR_ALL_PATCHES=1
		export OF_DONT_PATCH_ENCRYPTED_DEVICE=1
		export OF_NO_TREBLE_COMPATIBILITY_CHECK=1
		export FOX_USE_BASH_SHELL=1
		export FOX_ASH_IS_BASH=1
		export FOX_USE_TAR_BINARY=1
		export FOX_USE_SED_BINARY=1
		export FOX_USE_XZ_UTILS=1
		export OF_USE_TWRP_SAR_DETECT=1
		export OF_NO_SAMSUNG_SPECIAL=1
		export OF_QUICK_BACKUP_LIST="/boot;/data;"
		export FOX_DELETE_AROMAFM=1
		export FOX_ENABLE_APP_MANAGER=1
		export FOX_USE_NANO_EDITOR=1

		# OTA
		export OF_KEEP_DM_VERITY=1
		export OF_KEEP_FORCED_ENCRYPTION="1"
		export OF_FIX_OTA_UPDATE_MANUAL_FLASH_ERROR=1
		export OF_USE_NEW_MAGISKBOOT=1
		export OF_ENABLE_LPTOOLS=1

		# Screen settings
		export OF_SCREEN_H=2400
		export OF_STATUS_H=100
		export OF_ALLOW_DISABLE_NAVBAR=0 # We don't have physical buttons
		export OF_STATUS_INDENT_LEFT=48
		export OF_STATUS_INDENT_RIGHT=48
		export OF_CLOCK_POS=1
		# Maximum permissible splash image size (in kilobytes); do *NOT* increase!
		export OF_SPLASH_MAX_SIZE=104

		export FOX_VERSION=R11.1_1
		export FOX_BUILD_TYPE="Beta"
		export FOX_BUGGED_AOSP_ARB_WORKAROUND="1546300800"
		export FOX_RECOVERY_BOOT_PARTITION="/dev/block/by-name/boot"
		export OF_AB_DEVICE=1
		export FOX_USE_SPECIFIC_MAGISK_ZIP="$(gettop)/device/redmi/rosemary/Magisk/Magisk.zip"
	fi
fi
