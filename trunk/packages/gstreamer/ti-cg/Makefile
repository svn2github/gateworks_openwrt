include $(TOPDIR)/rules.mk

PKG_NAME:=ti-cg
PKG_VERSION:=7.3.1
PKG_MD5SUM:=6ddbd726be4e4cdf5f156c9f10610aab
#PKG_VERSION:=6.1.20
#PKG_MD5SUM:=39485ea8211960de0a958ab27106ab9b
#PKG_VERSION:=6.1.14
#PKG_MD5SUM:=494a980c2e1eb60480fff02e9b5877df

PKG_SOURCE:=ti_cgt_c6000_$(PKG_VERSION)_setup_linux_x86.bin
PKG_BUILD_DIR:=$(BUILD_DIR)/TI_CGT_C6000_$(PKG_VERSION)
PKG_SOURCE_URL:=http://software-dl.ti.com/dsps/dsps_registered_sw/sdo_ccstudio/codegen/C6000/$(PKG_VERSION)/exports/

include $(INCLUDE_DIR)/package.mk

define Package/ti-cg
	SECTION:=local
	CATEGORY:=Local
	TITLE:=TI CGT6X
	DEPENDS:=
endef

define Package/ti-cg/description
	TI Code Generation (compiler for DSP chip), download from https://www-a.ti.com/downloads/sds_support/TICodegenerationTools/, requires TI login/password
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

define Package/ti-cg/install	
	/bin/true
endef

$(eval $(call BuildPackage,ti-cg))
