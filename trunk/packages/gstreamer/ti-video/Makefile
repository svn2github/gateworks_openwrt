include $(TOPDIR)/rules.mk

PKG_NAME:=ti-video
PKG_VERSION:=1.0
PKG_RELEASE:=1

include $(INCLUDE_DIR)/package.mk

define Package/ti-video
	SECTION:=local
	CATEGORY:=Local
	TITLE:=Streaming Video client/server
	DEPENDS:=@TARGET_davinci +gstreamer-ti +gstreamer-utils
endef

define Package/ti-video/description
	OpenWRT support for streaming video client/server
endef

define Package/ti-video/conffiles
	/etc/config/tivideo
endef

define Build/Prepare
	$(CP) -r ./rootfs $(PKG_BUILD_DIR)
endef

define Build/Compile
endef

define Package/ti-video/install
	$(CP) $(PKG_BUILD_DIR)/rootfs/* $(1)
endef

$(eval $(call BuildPackage,ti-video))
