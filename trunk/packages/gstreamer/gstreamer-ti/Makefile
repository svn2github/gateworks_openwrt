include $(TOPDIR)/rules.mk

PKG_NAME:=gstreamer-ti
ifdef 0
PKG_VERSION:=1.02.00

PKG_SOURCE:=gst-ti-plugin-minimal-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=https://gstreamer.ti.com/gf/download/frsrelease/437/4107
PKG_MD5SUM:=340baec772847689dfd1f3c37f0650f1
PKG_BUILD_DIR:=$(BUILD_DIR)/gst-ti-plugin-minimal-$(PKG_VERSION)
else
#PKG_REV:=962
PKG_REV:=611
PKG_VERSION:=r$(PKG_REV)
PKG_RELEASE:=1

PKG_SOURCE_PROTO:=svn
PKG_SOURCE_VERSION:=$(PKG_REV)
PKG_SOURCE_SUBDIR:=gstreamer-ti-svn-$(PKG_VERSION)
PKG_SOURCE_URL:=--username=anonymous --password= https://gstreamer.ti.com/svn/gstreamer_ti/trunk/gstreamer_ti/ti_build/ticodecplugin/
PKG_SOURCE:=$(PKG_SOURCE_SUBDIR).tar.bz2
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_SOURCE_SUBDIR)
endif

PATCH_DIR:=patches/$(PKG_VERSION)
PKG_FIXUP:=libtool


include $(INCLUDE_DIR)/package.mk

GST_VERSION:=0.10

define Package/gstreamer-ti
	#CATEGORY:=Multimedia
	CATEGORY:=Local
	TITLE:=TI GStreamer plugin
	URL:=http://gstreamer.ti.com/
	DEPENDS:=@TARGET_davinci +ti-dmai +gstreamer +gst-plugins-base +gst-plugins-good +libiconv-full
endef

define Package/gstreamer-ti/description
	TI GStreamer plugin
endef

EXTRA_CFLAGS += \
	-I$(STAGING_DIR)/usr/lib/libiconv-full/include \
	-I$(STAGING_DIR)/usr/lib/libintl-full/include \
	-I$(BUILD_DIR)/ti-dspbios/packages \
	-Dxdc__RESTRICT__  \

EXTRA_LDFLAGS += \
	-L$(STAGING_DIR)/usr/lib/libintl-full/lib \
	-L$(STAGING_DIR)/usr/lib/libiconv-full/lib \
	-Wl,-rpath-link=$(STAGING_DIR)/usr/lib \

DVSDK_PATHS = \
	DVSDK_PATH=$(BUILD_DIR)/ti-codecs \
	DMAI_INSTALL_DIR=$(BUILD_DIR)/ti-dmai \
	CE_INSTALL_DIR=$(BUILD_DIR)/ti-ce \
	CMEM_INSTALL_DIR=$(BUILD_DIR)/ti-ce/cetools \
	FC_INSTALL_DIR=$(BUILD_DIR)/ti-ce/cetools \
	CODEC_INSTALL_DIR=$(BUILD_DIR)/ti-codecs/dm6446_dvsdk_combos_2_05 \
	LINK_INSTALL_DIR=$(BUILD_DIR)/ti-dsplink \
	LINUXKERNEL_INSTALL_DIR=$(LINUX_DIR) \
	XDAIS_INSTALL_DIR=$(BUILD_DIR)/ti-ce/cetools \
	XDC_INSTALL_DIR=$(BUILD_DIR)/ti-xdc \
	MVTOOL_DIR=$(TOOLCHAIN_DIR)/bin \


define Build/Compile
	$(MAKE) -f Makefile.external -C $(PKG_BUILD_DIR) \
		CFLAGS="$(TARGET_CFLAGS) $(EXTRA_CFLAGS)" \
		LDFLAGS="$(TARGET_LDFLAGS) $(EXTRA_LDFLAGS)" \
		PLATFORM=dm6446 \
		GST_TI_PLATFORM=dm6446 \
		CROSS_COMPILE=$(TARGET_CROSS) \
		$(DVSDK_PATHS) \
		CODEC_SERVER=installdir/ti-codecs-server/cs.x64P \
		ENCODE_COMBO=/etc/ti/encodeCombo.x64P \
		DECODE_COMBO=/etc/ti/decodeCombo.x64P \
		dm6446
	$(MAKE) -C $(PKG_BUILD_DIR) \
		$(TARGET_CONFIGURE_OPTS) \
		GST_TI_PLATFORM=dm6446 \
		PLATFORM=dm6446 \
		DESTDIR=$(PKG_INSTALL_DIR) \
		install
endef

define Package/gstreamer-ti/install
	$(INSTALL_DIR) $(1)/usr/lib/gstreamer-$(GST_VERSION)
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/lib/gstreamer-$(GST_VERSION)/libgstticodecplugin.so* \
		$(1)/usr/lib/gstreamer-$(GST_VERSION)/
endef

$(eval $(call BuildPackage,gstreamer-ti))
