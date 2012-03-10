include $(TOPDIR)/rules.mk

PKG_NAME:=gsc-update
PKG_RELEASE:=1
PKG_VERSION:=1.0

include $(INCLUDE_DIR)/package.mk

define Package/gsc-update
  SECTION:=base
  CATEGORY:=Gateworks
  TITLE:=Gateworks System Controller Firmware Update Application 
endef

define Package/gsc-update/description
	Gateworks System Controller Firmware Update Application
endef

TARGET_CFLAGS += -DTARGET=${CONFIG_TARGET_BOARD}

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Package/gsc-update/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/gsc_update $(1)/usr/bin/
endef

$(eval $(call BuildPackage,gsc-update))
