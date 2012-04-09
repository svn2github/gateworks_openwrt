include $(TOPDIR)/rules.mk

PKG_NAME:=gst-rtsp

# 0.10.8 depends on gstreamer-0.10.31 (see https://dev.openwrt.org/changeset/28532/packages/multimedia/gstreamer)
PKG_VERSION:=0.10.8
PKG_MD5SUM:=b511af07000595f63c3a705946221643
#PKG_VERSION:=0.10.7
#PKG_MD5SUM:=3c2e0bf09bb66e302b6c64cb9e8f1eb6

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.bz2
PKG_SOURCE_URL:=http://people.freedesktop.org/~wtay/
PKG_FIXUP:=autoreconf
PKG_INSTALL:=1

include $(INCLUDE_DIR)/package.mk

GST_VERSION:=0.10

EXTRA_LDFLAGS+= \
	-L$(STAGING_DIR)/usr/lib/libiconv-full/lib \
        -Wl,-rpath-link=$(STAGING_DIR)/usr/lib \

define Package/gst-rtsp
  SECTION:=net
  CATEGORY:=Multimedia
  TITLE:=Gstreamer based RTSP support library
  URL:=http://people.freedesktop.org/~wtay/
  DEPENDS:=+gstreamer +gst-plugins-base +libiconv-full
endef

define Package/gst-rtsp/description
	RTSP library components based on gstreamer.
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include/gstreamer-$(GST_VERSION)/gst/rtsp-server
	$(CP) $(PKG_INSTALL_DIR)/usr/include/gstreamer-$(GST_VERSION)/gst/rtsp-server/*.h \
		$(1)/usr/include/gstreamer-$(GST_VERSION)/gst/rtsp-server/

	$(INSTALL_DIR) $(1)/usr/lib/gstreamer-$(GST_VERSION)
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libgstrtspserver-$(GST_VERSION).{la,so*} \
		$(1)/usr/lib/gstreamer-$(GST_VERSION)/

	$(INSTALL_DIR) $(1)/usr/lib/pkgconfig
	$(INSTALL_DATA) $(PKG_INSTALL_DIR)/usr/lib/pkgconfig/*.pc \
		$(1)/usr/lib/pkgconfig
endef

define Package/gst-rtsp/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/lib/libgstrtspserver-*.so* \
		$(1)/usr/lib/
endef

$(eval $(call BuildPackage,gst-rtsp))
