include $(TOPDIR)/rules.mk

PKG_NAME:=ti-codecs
PKG_VERSION:=2_00_00_22
PKG_MD5SUM:=2ce99015bb1ed1df0491403c5e8d99fb

PKG_SOURCE:=dm6446_codecs_setuplinux_$(PKG_VERSION).bin
PKG_BUILD_DIR:=$(BUILD_DIR)/dvsdk_$(PKG_VERSION)
PKG_SOURCE_URL:=http://software-dl.ti.com/dsps/dsps_public_sw/sdo_sb/S1SDKLNX/DVSDK_2_00/exports/

include $(INCLUDE_DIR)/package.mk

define Package/ti-codecs
	CATEGORY:=Local
	URL:=http://www.ti.com/tool/dm644xcodecs
	TITLE:=TI Codec algorithms for audio and video
	DEPENDS:=@TARGET_davinci +ti-xdc +ti-cg +ti-dspbios +kmod-ti-dsplink +ti-ce
endef

define Package/ti-codecs/description
	TI Codec algorithm combos
endef

define Package/ti-codecs/config
	menu "Codecs"
		depends on PACKAGE_ti-codecs

	comment "Encoders ---"

	config TI_CODECS_AACHE_ENCODER
		bool "TI High Efficiency C64x AAC Encoder"
		default n
		help
		  Select this to install the TI High Efficiency AAC C64x Encoder

		  You will need to manually download this from TI at http://software-dl.ti.com/dsps/dsps_public_sw/codecs/C64XPlus_Audio/index_FDS.html

	comment "Decoders ---"

	config TI_CODECS_AACHE_DECODER
		bool "TI High Efficiency AAC C64x Decoder"
		default n
		depends foo
		help
		  Select this to install the TI High Efficiency AAC C64x Decoder 
	config TI_CODECS_MP3_DECODER
		bool "TI MP3 C64x Decoder
		default n
		help
		  Select this to install the TI MP3 C64x Decoder 

		  You will need to manually download this from TI at http://software-dl.ti.com/dsps/dsps_public_sw/codecs/C64XPlus_Audio/index_FDS.html

	endmenu
endef

ifeq ($(CONFIG_TI_CODECS_AACHE_ENCODER),y)
PKG_AAC_HE_ENC_NAME:=aacheenc
PKG_AAC_HE_ENC_VERSION:=1_20_005
PKG_AAC_HE_ENC_FILE:=c64xplus_aacheenc_$(PKG_AAC_HE_ENC_VERSION)_production

define Download/aache-encoder
  FILE:=$(PKG_AAC_HE_ENC_FILE).bin
  VERSION:=$(PKG_AAC_HE_ENC_VERSION)
  URL:=http://software-dl.ti.com/dsps/dsps_public_sw/codecs/C64XPlus_Audio/index_FDS.html
  MD5SUM:=49f75e33438e284195d042ace611ab3d
endef
$(eval $(call Download,aache-encoder))
endif

ifeq ($(CONFIG_TI_CODECS_MP3_DECODER),y)
PKG_MP3_DEC_NAME:=mp3dec
PKG_MP3_DEC_VERSION:=01_31_01_001
PKG_MP3_DEC_FILE:=c64xplus_mp3dec_$(PKG_MP3_DEC_VERSION)_production

define Download/mp3-decoder
  FILE:=$(PKG_MP3_DEC_FILE).bin
  VERSION:=$(PKG_MP3_DEC_VERSION)
  URL:=http://software-dl.ti.com/dsps/dsps_public_sw/codecs/C64XPlus_Audio/C64XPlus_Audio_latest
  MD5SUM:=4d958a12f06cf36211c00772acff9364
endef
$(eval $(call Download,mp3-decoder))
endif


define Build/Prepare
	chmod +x $(DL_DIR)/$(PKG_SOURCE)
	$(DL_DIR)/$(PKG_SOURCE) --prefix $(BUILD_DIR) -DInstallMode Silent
	$(CP) ./src/* $(PKG_BUILD_DIR)/
	ln -fs $(PKG_BUILD_DIR) $(BUILD_DIR)/$(PKG_NAME)
ifeq ($(CONFIG_TI_CODECS_AACHE_ENCODER),y)
	chmod +x $(DL_DIR)/$(PKG_AAC_HE_ENC_FILE).bin
	$(DL_DIR)/$(PKG_AAC_HE_ENC_FILE).bin \
		--prefix $(PKG_BUILD_DIR) --mode silent
	tar -xvf $(PKG_BUILD_DIR)/*_aacheenc_*.tar -C $(PKG_BUILD_DIR)
	cp -r $(PKG_BUILD_DIR)/*_aacheenc_*_production/packages/ti/sdo/codecs/aacheenc $(PKG_BUILD_DIR)/*_combos_*/packages/ti/sdo/codecs/
	patch -p1 -d $(PKG_BUILD_DIR) < ./files/aacheenc.patch
endif
ifeq ($(CONFIG_TI_CODECS_MP3_DECODER),y)
	chmod +x $(DL_DIR)/$(PKG_MP3_DEC_FILE).bin
	$(DL_DIR)/$(PKG_MP3_DEC_FILE).bin \
		--prefix $(PKG_BUILD_DIR) --mode silent
	tar -xvf $(PKG_BUILD_DIR)/*_mp3dec_*.tar -C $(PKG_BUILD_DIR)
	cp -r $(PKG_BUILD_DIR)/*_mp3dec_*_production/packages/ti/sdo/codecs/mp3dec $(PKG_BUILD_DIR)/*_combos_*/packages/ti/sdo/codecs/
	#patch -p1 -d $(PKG_BUILD_DIR) < ./files/mp3dec.patch
endif
	$(call Build/Patch)
endef

DVSDK_PATHS = \
	DVSDK_INSTALL_DIR=$(PKG_BUILD_DIR) \
	DVEVM_INSTALL_DIR=$(PKG_BUILD_DIR) \
	DVSDK_PATH=$(BUILD_DIR)/ti-codecs \
	CODEC_INSTALL_DIR=$(BUILD_DIR)/ti-codecs/dm6446_dvsdk_combos_2_05 \
	XDC_INSTALL_DIR=$(BUILD_DIR)/ti-xdc \
	CE_INSTALL_DIR=$(BUILD_DIR)/ti-ce \
	DEMO_INSTALL_DIR=$(PKG_BUILD_DIR)/dvsdk_demos_2_00_00_07 \
	XDAIS_INSTALL_DIR=$(BUILD_DIR)/ti-ce/cetools \
	CMEM_INSTALL_DIR=$(BUILD_DIR)/ti-ce/cetools \
	FC_INSTALL_DIR=$(BUILD_DIR)/ti-ce/cetools \
	LINK_INSTALL_DIR=$(BUILD_DIR)/ti-dsplink \
	BIOS_INSTALL_DIR=$(BUILD_DIR)/ti-dspbios \
	USER_XDC_PATH=$(CE_INSTALL_DIR)/examples \
	CODEGEN_INSTALL_DIR=$(BUILD_DIR)/ti-cg \
	LINUXKERNEL_INSTALL_DIR=$(LINUX_DIR) \
	MVTOOL_DIR=$(TOOLCHAIN_DIR)/bin \
	MVTOOL_PREFIX=$(TARGET_CROSS) \
	EXEC_DIR=$(PKG_BUILD_DIR)/install/$(PLATFORM) \
	LINUXLIBS_INSTALL_DIR=$(TOOLCHAIN_DIR)/usr \
	KERNEL_DIR=$(LINUX_DIR) \
	TARGET_ROOT_DIR=$(PKG_BUILD_DIR) \


define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR)/dm6446_dvsdk_combos_2_05 \
		$(TARGET_CONFIGURE_OPTS) \
		PLATFORM=dm6446 \
		$(DVSDK_PATHS) \
		CROSS_COMPILE=$(TARGET_CROSS) \
		COMPILER=$(TARGET_CC) \
		CROSS_COMPILER=MVL_5_0 \
		XDCARGS="prod" \
		clean

	$(MAKE) -C $(PKG_BUILD_DIR) \
		$(TARGET_CONFIGURE_OPTS) \
		PLATFORM=dm6446 \
		$(DVSDK_PATHS) \
		CROSS_COMPILE=$(TARGET_CROSS) \
		COMPILER=$(TARGET_CC) \
		CROSS_COMPILER=MVL_5_0 \
		XDCARGS="prod" \
		codecs
endef

define Build/Clean
	rm -f $(BUILD_DIR)/$(PKG_NAME)
endef

define Package/ti-codecs/install
	$(INSTALL_DIR) $(1)/etc/ti
	$(CP) $(PKG_BUILD_DIR)/dm6446_dvsdk_combos_2_05/packages/ti/sdo/servers/decode/decodeCombo.x64P $(1)/etc/ti/
	$(CP) $(PKG_BUILD_DIR)/dm6446_dvsdk_combos_2_05/packages/ti/sdo/servers/loopback/loopbackCombo.x64P $(1)/etc/ti/
	$(CP) $(PKG_BUILD_DIR)/dm6446_dvsdk_combos_2_05/packages/ti/sdo/servers/encode/encodeCombo.x64P $(1)/etc/ti/
endef

$(eval $(call BuildPackage,ti-codecs))
