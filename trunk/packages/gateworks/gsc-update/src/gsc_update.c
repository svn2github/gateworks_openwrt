/**
 * gsc_update.c Gateworks System Controller Firmware Updater
 *
 * Copyright 2008-2012 Gateworks Corporation
 *
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#include "i2c.h"
#include "i2c_upgrader.h"

#define GSC_UPDATER_REV 5

#define GSC_DEVICE		0x20
#define GSC_UPDATER		0x21
#define GSC_PASSWORD 	0x58
#define GSC_UNLOCK 		0x01
#define GSC_PROGRAM 	0x02
#define GSC_ERASE 		0x04
#define GSC_PUC 			0x06

#define GSC2_PUC			0x1
#define GSC2_ADDR			0x2
#define GSC2_ERASE		0x3
#define GSC2_WORD			0x4
#define GSC2_PROG			0x5

int parse_data_file(char *filename, unsigned char data[16][16384], short address[16], short length[16]);

void print_banner(void)
{
	printf("Gateworks GSC Updater r%i\n", GSC_UPDATER_REV);
	printf("Copyright (C) 2004-2012, Gateworks Corporation, All Rights Reserved\n");
	printf("Built %s, %s\n", __TIME__, __DATE__);
}

void print_help(void)
{
	print_banner();
	printf("\nUsage:\n");
	printf("\t-f <filename>\tProgram flash device using [filename]\n");
	printf("\n");
}

int main(int argc, char **argv)
{
	unsigned long funcs;
	unsigned char data[16][16384] = {0};
	unsigned short address[16];
	unsigned short length[16];
	char *prog_filename = NULL;
	unsigned char buffer[16];
	signed char ret;
#if TARGET == davinci
	unsigned char i2cbus = 1;
#else
	unsigned char i2cbus = 0;
#endif
	char device[16];

	int i, j, k;
	int file;

	for (i = 1; i < argc; i++) {
		if (argv[i][0] == '-') {
			switch(argv[i][1]) {
			case 'f':
				if ((i+1 < argc) && (argv[i+1][0] != '-'))
				{
					i++;
					prog_filename = argv[i];
				}
				else
				{
					print_help();
				}
				break;
			}
		}
	}

	if (!prog_filename) {
		print_help();
		exit(-1);
	}

	print_banner();

	sprintf(device, "/dev/i2c-%d", i2cbus);
	file = open(device, O_RDWR);
	if (file < 0 && errno == ENOENT) {
		sprintf(device, "/dev/i2c/%d", i2cbus);
		file = open(device, O_RDWR);
	}

	if (file < 0) {
		perror("unable to open i2c device");
		exit(1);
	}

	if (ioctl(file, I2C_FUNCS, &funcs) < 0) {
		perror("i2c not functional");
		exit(1);
	}

	if (ioctl(file, I2C_SLAVE, GSC_DEVICE) < 0) {
		perror("couldn't set GSC address");
		exit(1);
	}

	ret = i2c_smbus_read_byte_data(file, 14);
	printf("Current GSC Firmware Rev: %i\n", ret & 0xff);

	if (ioctl(file, I2C_SLAVE, GSC_UPDATER) < 0) {
		perror("couldn't set GSC_UPDATER address");
		exit(1);
	}

	i2c_smbus_write_byte_data(file, 0, GSC_PASSWORD | GSC_UNLOCK);
	ret = i2c_smbus_read_byte_data(file, 0);

	if (parse_data_file(prog_filename, data, address, length)) {
		exit(2);
	}

	/* ##### Stage 1 Upgrader ##### */
	// Erase all of main flash
	for (i = ((address[0] & 0xf000) | 0x400); i < 0xffff; i += 0x200) {
		buffer[0] = i & 0xff;
		buffer[1] = (i >> 8) & 0xff;
		i2c_smbus_write_i2c_block_data(file, 1, 2, buffer);
		i2c_smbus_write_byte_data(file, 0, GSC_PASSWORD | GSC_ERASE);
		while (1) {
			ret = i2c_smbus_read_byte_data(file, 2);
			if (ret != -1)
				break;
			fflush(stdout);
		}
		ret = i2c_smbus_read_byte_data(file, 1);
	}

	// Switch to Programing mode
	i2c_smbus_write_byte_data(file, 0, GSC_PASSWORD | GSC_PROGRAM);

	for (i = 1; i>= 0; i--) {
		if (i2c_upgrader_length[i]) {
			for (j = 0; j < i2c_upgrader_length[i]; j+=2) {
				// Set starting Address
				buffer[0] = (i2c_upgrader_address[i] + j) & 0xff;
				buffer[1] = ((i2c_upgrader_address[i] + j) >> 8) & 0xff;
				i2c_smbus_write_i2c_block_data(file, 1, 2, buffer);
				buffer[0] = i2c_upgrader_data[i][j];
				buffer[1] = i2c_upgrader_data[i][j+1];

				i2c_smbus_write_i2c_block_data(file, 3, 2, buffer);
				while (1) {
					ret = i2c_smbus_read_byte_data(file, 2);
					if (ret != -1)
						break;
					fflush(stdout);
				}
				if (!i) {
					printf("Program Upgrader  %2i%%\r", (int)((((double)j / 2) / i2c_upgrader_length[i]) * 100));
					fflush(stdout);
				}
			}
			if (!i) {
				printf("Program Upgrader  %i%%\n", 100);
				fflush(stdout);
			}
		}
	}
	// Turn off Programing Mode
	i2c_smbus_write_byte_data(file, 0, GSC_PASSWORD | GSC_UNLOCK);
	// Reset GSC
	i2c_smbus_write_byte_data(file, 0, GSC_PASSWORD | GSC_PUC);


	/* ##### Stage 2 Upgrader ##### */
	while (1) {
		ret = i2c_smbus_read_byte(file);
		if (ret != -1)
			break;
	}

	// Erase all of main flash
	for (i = (address[0] & 0xf000); i < 0xffff; i += 0x200) {
		buffer[0] = i & 0xff;
		buffer[1] = (i >> 8) & 0xff;
		i2c_smbus_write_i2c_block_data(file, GSC2_ADDR, 2, buffer);
		i2c_smbus_write_byte(file, GSC2_ERASE);
		while (1) {
			ret = i2c_smbus_read_byte(file);
			if (ret != -1)
				break;
		}
	}

	for (i = 15; i>= 0; i--) {
		if (length[i]) {
			for (j = 0; j < length[i]; j+=2) {
				// Set starting Address
				buffer[0] = (address[i] + j) & 0xff;
				buffer[1] = ((address[i] + j) >> 8) & 0xff;
				i2c_smbus_write_i2c_block_data(file, GSC2_ADDR, 2, buffer);
				buffer[0] = data[i][j];
				buffer[1] = data[i][j+1];

				i2c_smbus_write_i2c_block_data(file, GSC2_WORD, 2, buffer);
				i2c_smbus_write_byte(file, GSC2_PROG);
				while (1) {
					ret = i2c_smbus_read_byte(file);
					if (ret != -1)
						break;
					fflush(stdout);
				}
				printf("MSP Prg B%i   %2i%%\r", i, (int)((((double)j / 2) / length[i]) * 100));
				fflush(stdout);
			}
			printf("MSP Prg B%i  %i%%\n", i, 100);
			fflush(stdout);
		}
	}
	i2c_smbus_write_byte(file, GSC2_PUC);

	return 0;
}

int parse_data_file(char *filename, unsigned char data[16][16384], short address[16], short length[16])
{
	FILE *fd;
	char line[1024];
	char temp[64];
	short address_loc = -1;
	char t[16][4];
	short temp_word = 0;
	int i = 0, j = 0, num_scan = 0;
	int linenum = 0;

	memset(t, 0, 16*4);
	memset(length, 0, 16*2);

	fd = fopen(filename, "r");
	while (fgets(line, 1024, fd)) {
		linenum++;
		if (linenum == 1 && line[0] != '@') {
			fprintf(stderr, "Invalid GSC firmware file\n");
			return linenum;
		} 
		if (line[0] == '@') {
			j = 0;
			address_loc++;
			address[address_loc] = (short) strtol(line+1, 0, 16);
		} else if (line[0] != 'q') {
			num_scan = sscanf(line, "%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s",
				t[0], t[1], t[2], t[3], t[4], t[5], t[6], t[7], t[8], t[9], t[10], t[11], t[12], t[13], t[14], t[15]);
			for (i = 0; i < num_scan; i++) {
				temp_word = strtol(t[i], 0, 16);
				data[address_loc][j++] = temp_word;
				length[address_loc]++;
			}
		}
	}

	return 0;
}
