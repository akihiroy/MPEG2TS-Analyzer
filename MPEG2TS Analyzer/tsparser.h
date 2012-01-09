//
//  tsparser.h
//  MPEG2TS Analyzer
//
//  Created by Akihiro Yamasaki on 12/01/08.
//  Copyright (c) 2012 Akihiro Yamasaki. All rights reserved.
//

#ifndef MPEG2TS_Analyzer_tsparser_h
#define MPEG2TS_Analyzer_tsparser_h

typedef struct tagTSPacket {
	unsigned char data[188];
} TSPacket;

// TS Header
static inline int get_sync_byte(TSPacket *packet) { return packet->data[0]; }
static inline int get_transport_error_indicator(TSPacket *packet) { return packet->data[1] >> 7; }
static inline int get_payload_unit_start_indicator(TSPacket *packet) { return (packet->data[1] >> 6) & 1; }
static inline int get_transport_priority(TSPacket *packet) { return (packet->data[1] >> 5) & 1; }
static inline int get_PID(TSPacket *packet) { return (((int)packet->data[1] << 8) | packet->data[2]) & 0x1FFF; }
static inline int get_transport_scrambling_control(TSPacket *packet) { return (packet->data[3] >> 6) & 0x03; }
static inline int get_adaptation_field_control(TSPacket *packet) { return (packet->data[3] >> 4) & 0x03; }
static inline int get_continuity_counter(TSPacket *packet) { return packet->data[3] & 0x0F; }

// Adaptation Field
int get_af_adaptation_field_length(TSPacket *packet);
int get_af_discontinuity_indicator(TSPacket *packet);
int get_af_random_access_indicator(TSPacket *packet);
int get_af_elementary_stream_priority_indicator(TSPacket *packet);
int get_af_PCR_flag(TSPacket *packet);
int get_af_OPCR_flag(TSPacket *packet);
int get_af_splicing_point_flag(TSPacket *packet);
int get_af_transport_private_data_flag(TSPacket *packet);
int get_af_adaptation_field_extension_flag(TSPacket *packet);
long long get_af_program_clock_reference_base(TSPacket *packet);
int get_af_program_clock_reference_extension(TSPacket *packet);
long long get_af_original_program_clock_reference_base(TSPacket *packet);
int get_af_original_program_clock_reference_extension(TSPacket *packet);
int get_af_splice_countdown(TSPacket *packet);
int get_af_transport_private_data_length(TSPacket *packet);
unsigned char *get_af_private_data(TSPacket *packet);
int get_af_adaptation_field_extension_length(TSPacket *packet);
int get_af_ltw_flag(TSPacket *packet);
int get_af_piecewise_rate_flag(TSPacket *packet);
int get_af_seamless_splice_flag(TSPacket *packet);
int get_af_ltw_valid_flag(TSPacket *packet);
int get_af_ltw_offset(TSPacket *packet);
int get_af_piecewise_rate(TSPacket *packet);
int get_af_splice_type(TSPacket *packet);
long long get_af_DTS_next_AU(TSPacket *packet);


#endif
