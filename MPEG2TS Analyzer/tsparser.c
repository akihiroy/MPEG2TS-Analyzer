//
//  tsparser.c
//  MPEG2TS Analyzer
//
//  Created by Akihiro Yamasaki on 12/01/08.
//  Copyright (c) 2012 Akihiro Yamasaki. All rights reserved.
//

#include "tsparser.h"
#include <assert.h>

int get_af_adaptation_field_length(TSPacket *packet)
{
	assert((get_adaptation_field_control(packet) & 0x02) == 0x02);	// 2 or 3
	return packet->data[4];
}

int get_af_discontinuity_indicator(TSPacket *packet)
{
	assert(get_af_adaptation_field_length(packet) > 0);
	return packet->data[5] >> 7;
}

int get_af_random_access_indicator(TSPacket *packet)
{
	assert(get_af_adaptation_field_length(packet) > 0);
	return (packet->data[5] >> 6) & 1;
}

int get_af_elementary_stream_priority_indicator(TSPacket *packet)
{
	assert(get_af_adaptation_field_length(packet) > 0);
	return (packet->data[5] >> 5) & 1;
}

int get_af_PCR_flag(TSPacket *packet)
{
	assert(get_af_adaptation_field_length(packet) > 0);
	return (packet->data[5] >> 4) & 1;
}

int get_af_OPCR_flag(TSPacket *packet)
{
	assert(get_af_adaptation_field_length(packet) > 0);
	return (packet->data[5] >> 3) & 1;
}

int get_af_splicing_point_flag(TSPacket *packet)
{
	assert(get_af_adaptation_field_length(packet) > 0);
	return (packet->data[5] >> 2) & 1;
}

int get_af_transport_private_data_flag(TSPacket *packet)
{
	assert(get_af_adaptation_field_length(packet) > 0);
	return (packet->data[5] >> 1) & 1;
}

int get_af_adaptation_field_extension_flag(TSPacket *packet)
{
	assert(get_af_adaptation_field_length(packet) > 0);
	return packet->data[5] & 1;	
}

long long get_af_program_clock_reference_base(TSPacket *packet)
{
	assert(get_af_PCR_flag(packet) == 1);
	return ((long long)packet->data[6] << 25) | ((long long)packet->data[7] << 17) | ((long long)packet->data[8] << 9) | ((long long)packet->data[9] << 1) | ((long long)packet->data[10] >> 7);
}

int get_af_program_clock_reference_extension(TSPacket *packet)
{
	assert(get_af_PCR_flag(packet) == 1);
	return (((int)packet->data[10] << 8) | packet->data[11]) & 0x1FF;
}

long long get_af_original_program_clock_reference_base(TSPacket *packet)
{
	assert(get_af_OPCR_flag(packet) == 1);
	unsigned char *p = packet->data + (get_af_PCR_flag(packet) ? 12 : 6);
	return ((long long)p[0] << 25) | ((int)p[1] << 17) | ((int)p[2] << 9) | ((int)p[3] << 1) | ((int)p[4] >> 7);	
}

int get_af_original_program_clock_reference_extension(TSPacket *packet)
{
	assert(get_af_OPCR_flag(packet) == 1);
	unsigned char *p = packet->data + (get_af_PCR_flag(packet) ? 12 : 6);
	return (((int)p[4] << 8) | p[5]) & 0x1FF;	
}

int get_af_splice_countdown(TSPacket *packet)
{
	assert(get_af_splicing_point_flag(packet) == 1);
	unsigned char *p = packet->data + 6;
	if (get_af_PCR_flag(packet)) p += 6;
	if (get_af_OPCR_flag(packet)) p += 6;
	return p[0];
}

int get_af_transport_private_data_length(TSPacket *packet)
{
	assert(get_af_transport_private_data_flag(packet) == 1);
	unsigned char *p = packet->data + 6;
	if (get_af_PCR_flag(packet)) p += 6;
	if (get_af_OPCR_flag(packet)) p += 6;
	if (get_af_splicing_point_flag(packet)) p += 1;
	return p[0];	
}
unsigned char *get_af_private_data(TSPacket *packet)
{
	assert(get_af_transport_private_data_flag(packet) == 1);
	unsigned char *p = packet->data + 6;
	if (get_af_PCR_flag(packet)) p += 6;
	if (get_af_OPCR_flag(packet)) p += 6;
	if (get_af_splicing_point_flag(packet)) p += 1;
	return p + 1;
}

static inline unsigned char *get_af_adaptation_field_extension(TSPacket *packet)
{
	assert(get_af_adaptation_field_extension_flag(packet) == 1);
	unsigned char *p = packet->data + 6;
	if (get_af_PCR_flag(packet)) p += 6;
	if (get_af_OPCR_flag(packet)) p += 6;
	if (get_af_splicing_point_flag(packet)) p += 1;
	if (get_af_transport_private_data_flag(packet)) p += 1 + get_af_transport_private_data_length(packet);
	return p;
}

int get_af_adaptation_field_extension_length(TSPacket *packet)
{
	unsigned char *p = get_af_adaptation_field_extension(packet);
	return p[0];
}

int get_af_ltw_flag(TSPacket *packet)
{
	unsigned char *p = get_af_adaptation_field_extension(packet);
	return p[1] >> 7;
}

int get_af_piecewise_rate_flag(TSPacket *packet)
{
	unsigned char *p = get_af_adaptation_field_extension(packet);
	return (p[1] >> 6) & 1;	
}

int get_af_seamless_splice_flag(TSPacket *packet)
{
	unsigned char *p = get_af_adaptation_field_extension(packet);
	return (p[1] >> 5) & 1;	
}

int get_af_ltw_valid_flag(TSPacket *packet)
{
	assert(get_af_ltw_flag(packet) == 1);
	unsigned char *p = get_af_adaptation_field_extension(packet);
	return p[2] >> 7;
}

int get_af_ltw_offset(TSPacket *packet)
{
	assert(get_af_ltw_flag(packet) == 1);
	unsigned char *p = get_af_adaptation_field_extension(packet);
	return (((int)p[2] << 8) | p[3]) & 0x7FFF;
}

int get_af_piecewise_rate(TSPacket *packet)
{
	assert(get_af_piecewise_rate_flag(packet) == 1);
	unsigned char *p = get_af_adaptation_field_extension(packet);
	if (get_af_ltw_flag(packet)) p += 2;
	return (((int)p[0] << 16) | ((int)p[1] << 8) | p[0]) & 0x3FFFFF;
}

int get_af_splice_type(TSPacket *packet)
{
	assert(get_af_seamless_splice_flag(packet) == 1);
	unsigned char *p = get_af_adaptation_field_extension(packet);
	if (get_af_ltw_flag(packet)) p += 2;
	if (get_af_piecewise_rate_flag(packet)) p += 3;
	return p[0] >> 4;
}

long long get_af_DTS_next_AU(TSPacket *packet)
{
	assert(get_af_seamless_splice_flag(packet) == 1);
	unsigned char *p = get_af_adaptation_field_extension(packet);
	if (get_af_ltw_flag(packet)) p += 2;
	if (get_af_piecewise_rate_flag(packet)) p += 3;
	return (((long long)p[0] & 0x0E) << (30 - 1)) | (((int)p[1] & 0xFE) << (15 - 1)) | ((p[2] & 0xFE) >> 1);
}
