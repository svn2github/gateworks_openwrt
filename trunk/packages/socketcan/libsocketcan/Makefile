#
# Copyright (C) 2013 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=libsocketcan
PKG_VERSION:=0.0.8
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.bz2
PKG_SOURCE_URL:=http://www.pengutronix.de/software/libsocketcan/download/
PKG_MD5SUM:=7fa608d46553c1e33b6b34cab0a83c00

include $(INCLUDE_DIR)/package.mk

define Package/libsocketcan
  SECTION:=libs
  CATEGORY:=Libraries
  TITLE:=socketcan library
  URL:=http://www.pengutronix.de/software/libsocketcan/download/
endef

define Package/libsocketcan/description
 This package contains a library for applications dealing with CAN sockets
endef

define Build/Compile
	$(call Build/Compile/Default)
	make -C $(PKG_BUILD_DIR) \
		DESTDIR="$(PKG_INSTALL_DIR)" \
		install
endef

TARGET_CFLAGS += \
        -ffunction-sections $(FPIC) \
        -fdata-sections

CONFIGURE_VARS += \
        ac_cv_linux_vers=$(LINUX_VERSION)

MAKE_FLAGS += \
        CCOPT="$(TARGET_CFLAGS) -I$(BUILD_DIR)/linux/include"

define Build/InstallDev
	$(INSTALL_DIR) $(1)
	$(CP) $(PKG_INSTALL_DIR)/* $(1)/
endef

define Package/libsocketcan/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libsocketcan.so* $(1)/usr/lib/
endef

$(eval $(call BuildPackage,libsocketcan))
