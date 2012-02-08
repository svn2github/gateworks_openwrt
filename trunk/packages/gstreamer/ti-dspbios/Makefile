include $(TOPDIR)/rules.mk

PKG_NAME:=ti-dspbios
PKG_VERSION:=5_41_11_38
PKG_MD5SUM:=7fab13740c25f5b34b331bcdc759536e
#PKG_VERSION:=5_33_03
#PKG_MD5SUM:=da2dfd304386a8a9dc3c29902d4a3058

PKG_SOURCE:=bios_setuplinux_$(PKG_VERSION).bin
PKG_BUILD_DIR:=$(BUILD_DIR)/ti-dspbios_$(PKG_VERSION)
PKG_SOURCE_URL:=http://software-dl.ti.com/dsps/dsps_public_sw/sdo_sb/targetcontent/bios/dspbios/$(PKG_VERSION)/exports

include $(INCLUDE_DIR)/package.mk

define Package/ti-dspbios
	SECTION:=local
	CATEGORY:=Local
	URL:=http://software-dl.ti.com/dsps/dsps_public_sw/sdo_sb/targetcontent/bios/dspbios/
	TITLE:=TI DSP BIOS
endef

define Package/ti-dspbios/description
	TI DSP BIOS, the operating system for the DSP portion of the Davinci processor
endef

define Build/Prepare
	chmod +x $(DL_DIR)/$(PKG_SOURCE)
	$(DL_DIR)/$(PKG_SOURCE) --prefix $(PKG_BUILD_DIR) --mode silent
	# create a generic symlink to package so that other TI packages can find it
	rm -f $(BUILD_DIR)/$(PKG_NAME)
	ln -s $(PKG_BUILD_DIR) $(BUILD_DIR)/$(PKG_NAME)
endef

define Build/Compile
endef

define Build/Clean
	rm -f $(BUILD_DIR)/$(PKG_NAME)
endef

define Package/ti-dspbios/install
	/bin/true
endef

$(eval $(call BuildPackage,ti-dspbios))
