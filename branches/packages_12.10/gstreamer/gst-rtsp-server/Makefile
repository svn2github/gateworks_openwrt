include $(TOPDIR)/rules.mk

PKG_NAME:=gst-rtsp-server
PKG_RELEASE:=1
PKG_VERSION:=1.0

include $(INCLUDE_DIR)/package.mk

define Package/gst-rtsp-server
  SECTION:=base
  CATEGORY:=Gateworks
  TITLE:=RTSP Server based on Gstreamer components
  DEPENDS:=+gst-rtsp +libgstrtsp +gst-mod-videotestsrc +gst-mod-videorate +gstreamer-ti +gst-mod-rtp +libgstrtp +gst-mod-audiotestsrc +gst-mod-alaw +gst-mod-audioconvert +gst-mod-rtpmanager
endef

define Package/gst-rtsp-server/description
	RTSP Server based on Gstreamer components configured for TI davinci based products
endef

EXTRA_LDFLAGS+= \
	-L$(STAGING_DIR)/usr/lib/gstreamer-0.10 \
        -L$(STAGING_DIR)/usr/lib/libiconv-full/lib \
        -Wl,-rpath-link=$(STAGING_DIR)/usr/lib \

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

define Package/gst-rtsp-server/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/gst-rtsp $(1)/usr/bin/gst-rtspd

	$(INSTALL_DIR) $(1)/etc
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/gst-rtsp_ti.conf $(1)/etc
endef

$(eval $(call BuildPackage,gst-rtsp-server))
