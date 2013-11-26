#
# Copyright (C) 2013 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=canutils
PKG_VERSION:=4.0.6
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.bz2
PKG_SOURCE_URL:=http://www.pengutronix.de/software/socket-can/download/canutils/v4.0/
PKG_MD5SUM:=e9af32bc41da85517b7bfe7de3bb9481

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

include $(INCLUDE_DIR)/package.mk

define Package/canutils
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=Utils for canbus
  DEPENDS:=+libsocketcan
endef

define Package/canutils/description
	canutils is a set of utilities for interacting with CANbus sockets.
endef

define Package/canutils/install
	$(INSTALL_DIR) $(1)/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/src/canconfig $(1)/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/src/candump $(1)/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/src/canecho $(1)/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/src/cansend $(1)/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/src/cansequence $(1)/bin
endef

$(eval $(call BuildPackage,canutils))
