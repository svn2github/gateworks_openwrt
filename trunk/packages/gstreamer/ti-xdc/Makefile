include $(TOPDIR)/rules.mk

PKG_NAME:=ti-xdc
# TODO bump to latest (need to refresh patches)
#PKG_VERSION:=3_23_00_32
#PKG_MD5SUM:=263aa11795670265551d36a8c77ad27d
PKG_VERSION:=3_16_03_36
PKG_MD5SUM:=239da86ef7a70d920a1f9ea6ea5afc6e
#PKG_VERSION:=3_22_04_46
#PKG_MD5SUM:=4218d98e132e12346ac14411ff4d725a

PKG_SOURCE:=xdctools_setuplinux_$(PKG_VERSION).bin
PKG_BUILD_DIR:=$(BUILD_DIR)/xdctools_$(PKG_VERSION)
PKG_SOURCE_URL:=http://software-dl.ti.com/dsps/dsps_public_sw/sdo_sb/targetcontent/rtsc/$(PKG_VERSION)/exports/

include $(INCLUDE_DIR)/package.mk

define Package/ti-xdc
	SECTION:=local
	CATEGORY:=Local
	URL:=http://software-dl.ti.com/dsps/dsps_public_sw/sdo_sb/targetcontent/rtsc
	TITLE:=TI XDC
	DEPENDS:=@TARGET_davinci
endef

define Package/ti-xdc/description
	XDCtools provides tools necessary for you to use SYS/BIOS components and configure your DSP application.
endef

define Build/Prepare
	chmod +x $(DL_DIR)/$(PKG_SOURCE)
	$(DL_DIR)/$(PKG_SOURCE) --prefix $(PKG_BUILD_DIR) --mode silent
	$(call Build/Patch/Default)
	# create a generic symlink to package so that other TI packages can find it
	rm -f $(BUILD_DIR)/$(PKG_NAME)
	ln -fs $(PKG_BUILD_DIR) $(BUILD_DIR)/$(PKG_NAME)
endef

define Build/Compile
endef

define Build/Clean
	rm -f $(BUILD_DIR)/$(PKG_NAME)
endef

define Package/ti-xdc/install
	/bin/true
endef

$(eval $(call BuildPackage,ti-xdc))
